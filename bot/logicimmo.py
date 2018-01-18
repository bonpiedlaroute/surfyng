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
      prop_list = [APART_ID, HOUSE_ID]
            
      for id in prop_list:
         url = buildLogicImmoUrl(id)
         yield scrapy.Request(url=url, callback= lambda response, id_prop = id : self.parse(response,id_prop))

   def parse(self, response, id_property):

      links = response.xpath('//p[@class="offer-type"]').xpath('.//a[contains(@href, "detail-vente")]/@href').extract()

      for url in links:
         yield scrapy.Request(url, callback= lambda r, id_prop=id_property, announce_url = url: self.parse_announce(r, id_prop, announce_url))

      self.nextpage[id_property]+=1

      n ='page='+str(self.nextpage[id_property])

      following_link = response.xpath('//section[@class="offer-pagination-wrapper"]').xpath('.//a[contains(@href,$next)]/@href', next=n).extract()
      
      if following_link:
         yield scrapy.Request(following_link[0], callback= lambda response, id_prop=id_property : self.parse(response, id_prop))
      else:
         following_link = response.xpath('//link[contains(@href,$next)]/@href', next=n).extract()
         if following_link:
            yield scrapy.Request(following_link[0], callback= lambda response, id_prop=id_property : self.parse(response, id_prop))

   def parse_announce(self, response, id_prop, announce_url):
      announce_title = response.xpath('//title/text()').extract()
      announce_description = response.xpath('//div[@class="offer-description-text"]').xpath('.//meta/@content').extract()

      self.serializer.send(hash_id(announce_url),id_prop, announce_description[0], "Houilles","ile de france",announce_url,"LogicImmo", announce_title[0])

      self.announce_cnt+=1

   def closed(self, reason):
      
      print "Total announces: %d" %(self.announce_cnt)      

      self.serializer.close()

