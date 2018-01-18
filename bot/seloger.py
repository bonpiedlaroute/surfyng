#!/usr/bin/env python
# -*- coding: utf-8 -*- 
# (c) Copyright 2017-2018 
# author(s): Noel Tchidjo
# All rights reserved

import scrapy

from hash_id import *
from properties_type import *
from url_builder import *
from Serializer import *


class SelogerSpider(scrapy.Spider):
   
   name = "seloger"

   def __init__(self):
      self.mapping_url_ptype = dict()
      self.announces_cnt = 0
      self.announce_title = dict()

      self.serializer = Serializer('localhost', 5050)

   def start_requests(self):
      prop_list = [APART_ID, HOUSE_ID]
            
      for id in prop_list:
         announcers_id = getSeLogerPropertiesId(id)
         
         url = buildselogerurl(announcers_id)
         self.mapping_url_ptype[url] = id
         yield scrapy.Request(url=url, callback=self.parse)

   def parse(self, response):
      original_request = str(response.request)
      links = response.xpath('//section[@class="liste_resultat"]').xpath('.//a[contains(@href, "ci=780311")]/@href').extract()
      #links = response.xpath('//section[@class="liste_resultat"]').xpath('.//a[contains(@href, "ci=920025")]/@href').extract()
      id_prop = 0
      for url in links:
         if "annonces" in url:
            if self.mapping_url_ptype[original_request[5:-1]] == APART_ID:
               id_prop = APART_ID
            elif self.mapping_url_ptype[original_request[5:-1]] == HOUSE_ID:
               id_prop = HOUSE_ID

            yield scrapy.Request(url, callback= lambda r, url = url, id_prop = id_prop :self.parse_announce_title(r, url, id_prop)) 

      following_link = response.xpath('//a[@title="Page suivante"]/@href').extract()

      if following_link:
         following_link[0] ='http:'+following_link[0]
         self.mapping_url_ptype[following_link[0]] = self.mapping_url_ptype[original_request[5:-1]]
         yield response.follow(following_link[0], self.parse)

   def parse_prop_description(self, response, announce_url, id_property, ID):
      self.announces_cnt += 1

      self.serializer.send(ID, id_property, response.text, "Houilles","ile de france", announce_url, announce_url[11:18], self.announce_title[ID])
            
   def parse_announce_title(self, response, announce_url, id_property):
      title = response.xpath('//h1[@class="detail-title title1"]/text()').extract()
      ID = hash_id(announce_url)

      if title is not None:
         self.announce_title[ID] = title[0].strip()
      else:
         self.announce_title[ID] = ""

      yield scrapy.Request(buildselogerdescriptionurl(announce_url), callback= lambda r, url = announce_url, id_prop = id_property, ID = ID:self.parse_prop_description(r,url, id_prop, ID))

   def closed(self, reason):
      print "Announces found: %d\n" %self.announces_cnt
      self.serializer.close()




