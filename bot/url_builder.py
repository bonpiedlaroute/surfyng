#!/usr/bin/env python
# -*- coding: utf-8 -*- 
# (c) Copyright 2017 Noel Tchidjo
# All rights reserved

import re
from properties_type import *

seloger_prop_type_pos = 67

seloger_houilles_url = 'http://www.seloger.com/list.htm?idtt=2&naturebien=1,2,4&idtypebien=&ci=780311'
seloger_colombes_url = 'http://www.seloger.com/list.htm?idtt=2&naturebien=1,2,4&idtypebien=&ci=920025'


logicimmo_houilles_baseurl = 'http://www.logic-immo.com/vente-immobilier-houilles-78800,13772_2/options/groupprptypesids='

def insert_in_url(url, position, value):
   return url[:position] + value + url[position:]

def buildselogerurl(proptype,url = seloger_houilles_url ):  

   return insert_in_url(url,seloger_prop_type_pos, proptype)


def buildselogerdescriptionurl(url):
   match = re.search('([0-9]+).htm', url)
   description_url = 'http://www.seloger.com/detail,json,caracteristique_bien.json?idannonce='
   
   if match.group(0):
      result = match.group(0)
      return description_url + result[0:-4]
   else:
      return description_url


def buildLogicImmoUrl(id):
   return logicimmo_houilles_baseurl + getLogicImmoPropertiesId(id)
