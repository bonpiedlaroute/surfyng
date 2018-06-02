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

class LogicImmoSpider(scrapy.Spider):
      
   name = "logicimmo"

   def __init__(self):
      
      self.nextpage = dict()
      self.nextpage[APART_ID] = 1
      self.nextpage[HOUSE_ID] = 1
      self.prop_record = []
      self.announce_cnt = 0
      self.serializer = Serializer('localhost', 5050)

   def start_requests(self):
      prop_list = [(APART_ID, BUY_ID), (HOUSE_ID, BUY_ID), (APART_ID, RENT_ID), (HOUSE_ID, RENT_ID)]

            
      for ptype, stype in prop_list:
         url = buildLogicImmoUrl(ptype, stype)
         yield scrapy.Request(url=url, callback= lambda response, id_prop = ptype, search_type = stype: self.parse(response,id_prop, search_type))

   def parse(self, response, id_property, search_type):

      if search_type == BUY_ID:
         links = response.xpath('//p[@class="offer-type"]').xpath('.//a[contains(@href, "detail-vente")]/@href').extract()
      else:
         links = response.xpath('//p[@class="offer-type"]').xpath('.//a[contains(@href, "detail-location")]/@href').extract()

      for url in links:
         yield scrapy.Request(url, callback= lambda r, id_prop=id_property, announce_url = url, search_type = search_type: self.parse_announce(r, id_prop, announce_url, search_type))

      self.nextpage[id_property]+=1

      n ='page='+str(self.nextpage[id_property])

      following_link = response.xpath('//section[@class="offer-pagination-wrapper"]').xpath('.//a[contains(@href,$next)]/@href', next=n).extract()
      
      if following_link:
         yield scrapy.Request(following_link[0], callback= lambda response, id_prop=id_property, search_type = search_type : self.parse(response, id_prop, search_type))
      else:
         following_link = response.xpath('//link[contains(@href,$next)]/@href', next=n).extract()
         if following_link:
            yield scrapy.Request(following_link[0], callback= lambda response, id_prop=id_property, search_type = search_type : self.parse(response, id_prop, search_type))

   def parse_announce(self, response, id_prop, announce_url, search_type):
      announce_title = response.xpath('//title/text()').extract()
      announce_description = response.xpath('//div[@class="offer-description-text"]').xpath('.//meta/@content').extract()

      self.serializer.send(hash_id(announce_url),id_prop, announce_description[0], "Houilles","ile de france",announce_url,"LogicImmo", announce_title[0], search_type)

      self.announce_cnt+=1

   def closed(self, reason):
      
      print "Total announces: %d" %(self.announce_cnt)      

      self.serializer.close()

