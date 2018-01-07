#!/usr/bin/env python
# -*- coding: utf-8 -*- 
# (c) Copyright 2017 
# author(s): Noel Tchidjo
# All rights reserved

import scrapy

from hash_id import *
from properties_type import *
from url_builder import *
from collections import namedtuple
from thrift_generated.dynamodb_access import *

from thrift import Thrift
from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol


tablename ="FR_PROPERTIES"

class PropertiesSpider(scrapy.Spider):
   
   name = "properties_spider"

   def __init__(self):
      self.mapping_url_ptype = dict()
      self.announces_cnt = 0
      self.announce_title = dict()
      # Make socket
      transport = TSocket.TSocket('localhost', 5050)

      # Buffering is critical. Raw sockets are very slow
      transport = TTransport.TBufferedTransport(transport)

      # Wrap in a protocol
      protocol = TBinaryProtocol.TBinaryProtocol(transport)

      # Create a client to use the protocol encoder
      self.client = dynamodb_access.Client(protocol)

      # Connect!
      transport.open()

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
      id_prop = 0
      for url in links:
         if "annonces" in url:
            url = "http:"+url
            if self.mapping_url_ptype[original_request[5:-1]] == APART_ID:
               id_prop = APART_ID
            elif self.mapping_url_ptype[original_request[5:-1]] == HOUSE_ID:
               id_prop = HOUSE_ID

            yield scrapy.Request(url, callback= lambda r, url = url, id_prop = id_prop :self.parse_announce_title(r, url, id_prop)) 

      following_link = response.xpath('//a[@title="Page suivante"]/@href').extract()

      if following_link:
         self.mapping_url_ptype[following_link[0]] = self.mapping_url_ptype[original_request[5:-1]]
         yield response.follow(following_link[0], self.parse)

   def parse_prop_description(self, response, announce_url, id_property, ID):
      self.announces_cnt += 1
      values = dict()
      idvalue = ttypes.ValueType()
      idvalue.field = str(ID)
      idvalue.fieldtype = ttypes.Type.NUMBER
      values["ID"] = idvalue

      property_type_value = ttypes.ValueType()
      if id_property == APART_ID:
         property_type_value.field = "apartment"
      else:
         property_type_value.field = "house"

      property_type_value.fieldtype = ttypes.Type.STRING

      values["PROPERTY_TYPE"] = property_type_value

      property_desc_value = ttypes.ValueType()
      property_desc_value.field = response.text
      property_desc_value.fieldtype = ttypes.Type.STRING

      values["PROPERTY_DESCRIPTION"] = property_desc_value

      city_value = ttypes.ValueType()
      city_value.field = "Houilles"
      city_value.fieldtype = ttypes.Type.STRING
      values["CITY"] = city_value
   
      region_value = ttypes.ValueType()
      region_value.field = "ile de france"
      region_value.fieldtype = ttypes.Type.STRING
      values["REGION"] = region_value

      announce_link_value = ttypes.ValueType()
      announce_link_value.field = announce_url
      announce_link_value.fieldtype = ttypes.Type.STRING
      values["ANNOUNCE_LINK"] = announce_link_value

      announce_url_value = ttypes.ValueType()
      announce_url_value.field = announce_url[11:18]
      announce_url_value.fieldtype = ttypes.Type.STRING
      values["ANNOUNCE_SOURCE"] = announce_url_value 


      announce_title_value = ttypes.ValueType()
      announce_title_value.field = self.announce_title[ID]
      announce_title_value.fieldtype = ttypes.Type.STRING
      values["ANNOUNCE_TITLE"] = announce_title_value

      result  = self.client.put(tablename, values)
      
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

