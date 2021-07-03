#!/usr/bin/env python
# -*- coding: utf-8 -*- 
# (c) Copyright 2017-2018 
# author(s): Noel Tchidjo
# All rights reserved

from thrift_generated.dynamodb_access.ttypes import Type
from thrift_generated.dynamodb_access.ttypes import ValueType
from thrift_generated.dynamodb_access.ttypes import KeyValue
from thrift_generated.dynamodb_access import *

from thrift import Thrift
from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol
from search_features import *
import datetime




class Serializer:
   def __init__(self, ip, port, tablename):
      self.tablename = tablename
      self.transport = TSocket.TSocket(ip, port)

      self.transport = TTransport.TBufferedTransport(self.transport)

      protocol = TBinaryProtocol.TBinaryProtocol(self.transport)

      self.client = dynamodb_access.Client(protocol)

      self.transport.open()

   def send(self, ID, property_type, property_description, city, region, announce_link, announce_source, announce_title, id_search, announce_image="", img_cnt = 0):
      values = dict()

      idvalue = ttypes.ValueType()
      idvalue.field = str(ID)
      idvalue.fieldtype = ttypes.Type.NUMBER
      values["ID"] = idvalue

      property_type_value = ttypes.ValueType()
      if property_type == APART_ID:
         property_type_value.field = "Appartement"
      else:
         property_type_value.field = "Maison"

      property_type_value.fieldtype = ttypes.Type.STRING

      values["PROPERTY_TYPE"] = property_type_value

      property_desc_value = ttypes.ValueType()
      property_desc_value.field = property_description
      property_desc_value.fieldtype = ttypes.Type.STRING

      values["PROPERTY_DESCRIPTION"] = property_desc_value

      city_value = ttypes.ValueType()
      city_value.field = city
      city_value.fieldtype = ttypes.Type.STRING
      values["CITY"] = city_value
   
      region_value = ttypes.ValueType()
      region_value.field = region
      region_value.fieldtype = ttypes.Type.STRING
      values["REGION"] = region_value

      announce_link_value = ttypes.ValueType()
      announce_link_value.field = announce_link
      announce_link_value.fieldtype = ttypes.Type.STRING
      values["ANNOUNCE_LINK"] = announce_link_value

      announce_url_value = ttypes.ValueType()
      announce_url_value.field = announce_source
      announce_url_value.fieldtype = ttypes.Type.STRING
      values["ANNOUNCE_SOURCE"] = announce_url_value 

      if announce_image:
         announce_image_value = ttypes.ValueType()
         announce_image_value.field = announce_image
         announce_image_value.fieldtype = ttypes.Type.STRING
         values["ANNOUNCE_IMAGE"] = announce_image_value   

      if img_cnt != 0:
         img_cnt_value = ttypes.ValueType()
         img_cnt_value.field = str(img_cnt)
         img_cnt_value.fieldtype = ttypes.Type.STRING
         values["IMAGE_COUNT"] = img_cnt_value

      announce_title_value = ttypes.ValueType()
      announce_title_value.field = announce_title  
      announce_title_value.fieldtype = ttypes.Type.STRING
      values["ANNOUNCE_TITLE"] = announce_title_value

      now = datetime.datetime.utcnow().replace(microsecond=0).isoformat()

      timestamp = ttypes.ValueType()
      timestamp.field = now
      timestamp.fieldtype = ttypes.Type.STRING
      values["TIMESTAMP"] = timestamp 

      first_timestamp = ttypes.ValueType()
      first_timestamp.field = now
      first_timestamp.fieldtype = ttypes.Type.STRING
      values["FIRST_TIMESTAMP"] = first_timestamp      

      search_type = ttypes.ValueType()
      if id_search == BUY_ID:
         search_type.field = "For sale"
      else:
         search_type.field = "For rent"
      
      search_type.fieldtype = ttypes.Type.STRING

      values["SEARCH_TYPE"] = search_type

      return self.client.put(self.tablename, values)

   def send_data(self, values):
      return self.client.put(self.tablename, values)

   def scanidByCityAndAdSource(self, city, source):
      IDs = []
      filterexpression = "CITY = :ct and ANNOUNCE_SOURCE = :as"
      while True:
         attribute_to_get = {}
         idvalue = ValueType()
         idvalue.fieldtype = Type.NUMBER
         attribute_to_get["ID"] = idvalue 
         expression_value = {}
         city_value = ValueType()
         city_value.field =  city
         city_value.fieldtype = Type.STRING
         expression_value [":ct"] = city_value
         source_value = ValueType()
         source_value.field =  source
         source_value.fieldtype = Type.STRING
         expression_value [":as"] = source_value
         query_result = self.client.scan(self.tablename, attribute_to_get, filterexpression, expression_value)

         if query_result.result.success and query_result.values:
            for value in query_result.values:
               if "ID" in value.keys():
                  IDs.append(value["ID"])   
         if query_result.scanend:
            break

      return IDs

   def updateTimeStamp(self, ID):
      id_value = ValueType()
      id_value.field = str(ID)
      id_value.fieldtype = Type.NUMBER
      item_key = KeyValue('ID', id_value)

      values = {}
      timestamp = ValueType()
      timestamp.field = datetime.datetime.utcnow().replace(microsecond=0).isoformat()
      timestamp.fieldtype = Type.STRING
      values["TIMESTAMP"] = timestamp 
 
      print ("update timestamp for item {}".format(ID))
      ret = self.client.update(self.tablename, item_key, values)
      print (ret)
 
   def close(self):
      self.transport.close()

