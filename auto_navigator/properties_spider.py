#!/usr/bin/env python
# -*- coding: utf-8 -*- 
# (c) Copyright 2017 Noel Tchidjo
# All rights reserved

import scrapy

from properties_type import *
from url_builder import *
from collections import namedtuple


PropertyRecord = namedtuple('PropertyRecord', 'url, id_prop, description')



class PropertiesSpider(scrapy.Spider):
   
   name = "properties_spider"

   def __init__(self):
      self.house_announces = set()
      self.apart_announces = set()
      self.mapping_url_ptype = dict()
      self.prop_record = []

   def start_requests(self):
      urls = []
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
      for url in links:
         if "annonces" in url:
            if self.mapping_url_ptype[original_request[5:-1]] == APART_ID:
               self.apart_announces.add(url.lower())
               id_prop = APART_ID
               yield scrapy.Request(buildselogerdescriptionurl(url), callback= lambda r, url = url, id_prop = id_prop :self.parse_prop_description(r,url, id_prop))

            elif self.mapping_url_ptype[original_request[5:-1]] == HOUSE_ID:
               self.house_announces.add(url.lower())
               id_prop = HOUSE_ID
               yield scrapy.Request(buildselogerdescriptionurl(url), callback= lambda r, url = url, id_prop = id_prop :self.parse_prop_description(r,url, id_prop))
 

      following_link = response.xpath('//a[@title="Page suivante"]/@href').extract()

      if following_link:
         self.mapping_url_ptype[following_link[0]] = self.mapping_url_ptype[original_request[5:-1]]
         yield response.follow(following_link[0], self.parse)

   def parse_prop_description(self, response, announce_url, id_property):
      pr = PropertyRecord(announce_url, id_property, response.text)
      self.prop_record.append(pr)
            

   def closed(self, reason):
      print "Apart announces found: %d\n" % len(self.apart_announces)
      print "House announces found: %d\n" % len(self.house_announces)
      print "Appart description found: %d\n" % len(self.prop_record)
      #print "\n self.announces excluded:\n"
      for url in self.house_announces:
         print "%s:" %url
      for record in self.prop_record:
         print "url: %s | id_prop : %s | description: %s " %(record.url, record.id_prop, record.description)

#http://www.seloger.com/detail,json,caracteristique_bien.json?idannonce=118385789
#jsonresponse = json.loads(response.body_as_unicode())
# jsonresponse = json.loads(response.text))
