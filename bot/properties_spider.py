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



PropertyRecord = namedtuple('PropertyRecord', 'url, id_prop, description')

tablename ="FR_PROPERTIES"

class PropertiesSpider(scrapy.Spider):
   
   name = "properties_spider"

   def __init__(self):
      self.house_announces = set()
      self.apart_announces = set()
      self.mapping_url_ptype = dict()
      self.prop_record = []
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
      values = dict()
      idvalue = ttypes.ValueType()
      idvalue.field = str(hash_id(announce_url))
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
      announce_link_value.field = announce_url[2:]
      announce_link_value.fieldtype = ttypes.Type.STRING
      values["ANNOUNCE_LINK"] = announce_link_value

      announce_url_value = ttypes.ValueType()
      announce_url_value.field = announce_url[6:13]
      announce_url_value.fieldtype = ttypes.Type.STRING
      values["ANNOUNCE_SOURCE"] = announce_url_value 

      result  = self.client.put(tablename, values)
      

   def closed(self, reason):
      print "Announces found: %d\n" % len(self.prop_record)

