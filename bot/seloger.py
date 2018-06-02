#!/usr/bin/env python
# -*- coding: utf-8 -*- 
# (c) Copyright 2017-2018 
# author(s): Noel Tchidjo
# All rights reserved

import scrapy

from hash_id import *
from search_features import *
from url_builder import *
from Serializer import *


class SelogerSpider(scrapy.Spider):
   
   name = "seloger"

   def __init__(self):
      self.mapping_url_ptype = dict()
      self.mapping_url_stype = dict()
      self.announces_cnt = 0
      self.announce_title = dict()

      self.serializer = Serializer('localhost', 5050)

   def start_requests(self):
      prop_list = [(APART_ID, BUY_ID), (HOUSE_ID, BUY_ID), (APART_ID, RENT_ID), (HOUSE_ID, RENT_ID)]
            
      for ptype, stype in prop_list:
         announcers_id = getSeLogerPropertiesId(ptype)
         search_id = getSeLogerSearchTypeId(stype)        

         url = buildselogerurl(announcers_id, search_id)
         self.mapping_url_ptype[url] = ptype
         self.mapping_url_stype[url] = stype
         yield scrapy.Request(url=url, callback=self.parse)

   def parse(self, response):
      original_request = str(response.request)
      links = response.xpath('//section[@class="liste_resultat"]').xpath('.//a[contains(@href, "ci=780311")]/@href').extract()
      #links = response.xpath('//section[@class="liste_resultat"]').xpath('.//a[contains(@href, "ci=920025")]/@href').extract()
      id_prop = 0
      id_search = 0
      for url in links:
         if "annonces" in url:
            id_prop = self.mapping_url_ptype[original_request[5:-1]]
            id_search = self.mapping_url_stype[original_request[5:-1]] 

            yield scrapy.Request(url, callback= lambda r, url = url, id_prop = id_prop, id_search = id_search :self.parse_announce_title(r, url, id_prop, id_search)) 

      following_link = response.xpath('//a[@title="Page suivante"]/@href').extract()

      if following_link:
         new_link = following_link[0]
         self.mapping_url_ptype[new_link] = self.mapping_url_ptype[original_request[5:-1]]
         self.mapping_url_stype[new_link] = self.mapping_url_stype[original_request[5:-1]]

         yield response.follow(new_link, self.parse)

   def parse_prop_description(self, response, announce_url, id_property, ID, id_search):
      self.announces_cnt += 1

      self.serializer.send(ID, id_property, response.text, "Houilles","ile de france", announce_url, announce_url[12:19], self.announce_title[ID], id_search)
            
   def parse_announce_title(self, response, announce_url, id_property, id_search):
      title = response.xpath('//h1[@class="detail-title title1"]/text()').extract()
      ID = hash_id(announce_url)

      if title is not None:
         self.announce_title[ID] = title[0].strip()
      else:
         self.announce_title[ID] = ""

      yield scrapy.Request(buildselogerdescriptionurl(announce_url), callback= lambda r, url = announce_url, id_prop = id_property, ID = ID, id_search = id_search:self.parse_prop_description(r,url, id_prop, ID, id_search))

   def closed(self, reason):
      print "Announces found: %d\n" %self.announces_cnt
      self.serializer.close()




