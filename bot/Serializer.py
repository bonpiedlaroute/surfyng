#!/usr/bin/env python
# -*- coding: utf-8 -*- 
# (c) Copyright 2017-2018 
# author(s): Noel Tchidjo
# All rights reserved

from thrift_generated.dynamodb_access import *

from thrift import Thrift
from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol
from properties_type import *

tablename ="FR_PROPERTIES"

class Serializer:
   def __init__(self, ip, port):
      self.transport = TSocket.TSocket(ip, port)

      self.transport = TTransport.TBufferedTransport(self.transport)

      protocol = TBinaryProtocol.TBinaryProtocol(self.transport)

      self.client = dynamodb_access.Client(protocol)

      self.transport.open()

   def send(self, ID, property_type, property_description, city, region, announce_link, announce_source, announce_title):
      values = dict()

      idvalue = ttypes.ValueType()
      idvalue.field = str(ID)
      idvalue.fieldtype = ttypes.Type.NUMBER
      values["ID"] = idvalue

      property_type_value = ttypes.ValueType()
      if property_type == APART_ID:
         property_type_value.field = "apartment"
      else:
         property_type_value.field = "house"

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


      announce_title_value = ttypes.ValueType()
      announce_title_value.field = announce_title
      announce_title_value.fieldtype = ttypes.Type.STRING
      values["ANNOUNCE_TITLE"] = announce_title_value

      return self.client.put(tablename, values)

   def close(self):
      self.transport.close()

