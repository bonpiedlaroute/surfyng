#!/usr/bin/env python
# -*- coding: utf-8 -*- 
# (c) Copyright 2017-2019 
# author(s): Noel Tchidjo
# All rights reserved

import scrapy
import json
import urllib
import os
import configparser


from thrift_generated.dynamodb_access.ttypes import Type
from thrift_generated.dynamodb_access.ttypes import ValueType
from thrift_generated.dynamodb_access.ttypes import KeyValue
from thrift_generated.dynamodb_access import *


from url_builder import *
from Serializer import Serializer
from search_features import *


config_notaires = configparser.ConfigParser()
config_notaires.read('spiders/config_notaires.ini')

ip = config_notaires['COMMON']['db_access_ip']
port = int(config_notaires['COMMON']['db_access_port'])
tablename = config_notaires['COMMON']['tablename']


class NotairesSpider(scrapy.Spider):
   
   name = "notaires"

   def __init__(self, city='', **kwargs):
      self.city = city.lower()

      self.serializer = Serializer(ip, port, tablename)

   def start_requests(self):
      prop_list = [APART_ID, HOUSE_ID]
            
      for ptype in prop_list:
         announcers_id = getNotairesPropertiesId(ptype)
         referer_announcers_id = getNotairesRefererPropertiesId(ptype)

         url = buildNotairesUrl(self.city, announcers_id)
         referer_url = buildNotairesRefererUrl(self.city, referer_announcers_id)

         yield scrapy.Request(url=url, headers={'Referer':referer_url}, callback= lambda r, ptype = ptype :self.parse(r, ptype))

   def parse(self, response, ptype):
      values = dict()

      idcityvalue = ttypes.ValueType()
      idcityvalue.field = inseecodeByCity[self.city]
      idcityvalue.fieldtype = ttypes.Type.NUMBER
      item_key = KeyValue( 'ID',  idcityvalue)

      cityvalue = ttypes.ValueType()
      cityvalue.field = self.city
      cityvalue.fieldtype = ttypes.Type.STRING
      values['CITY'] = cityvalue

      pricebym2value = ttypes.ValueType()
      pricebym2value.field = response.text
      pricebym2value.fieldtype = ttypes.Type.STRING

      if ptype == APART_ID:
         values['PRICE_BY_M2_FLAT'] =  pricebym2value
      else:
         values['PRICE_BY_M2_HOUSE'] = pricebym2value

      ret = self.serializer.client.update(self.serializer.tablename, item_key, values)

      print(ret)
           
   def closed(self, reason):
      self.serializer.close()

