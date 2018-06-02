#!/usr/bin/env python
# -*- coding: utf-8 -*- 
# (c) Copyright 2017 Noel Tchidjo
# All rights reserved

import re
from search_features import *

seloger_prop_type_pos = 68
seloger_search_type_pos = 38

seloger_houilles_url = 'https://www.seloger.com/list.htm?idtt=&naturebien=1,2,4&idtypebien=&ci=780311'
seloger_colombes_url = 'https://www.seloger.com/list.htm?idtt=2&naturebien=1,2,4&idtypebien=&ci=920025'


logicimmo_houilles_sale_baseurl = 'http://www.logic-immo.com/vente-immobilier-houilles-78800,13772_2/options/groupprptypesids='
logicimmo_houilles_rent_baseurl = 'http://www.logic-immo.com/location-immobilier-houilles-78800,13772_2/options/groupprptypesids='

def insert_in_url(url, position, value):
   return url[:position] + value + url[position:]

def buildselogerurl(proptype, searchtype):  
   url = insert_in_url(seloger_houilles_url,seloger_search_type_pos, searchtype)
   url = insert_in_url(url,seloger_prop_type_pos, proptype)

   return url


def buildselogerdescriptionurl(url):
   match = re.search('([0-9]+).htm', url)
   description_url = 'https://www.seloger.com/detail,json,caracteristique_bien.json?idannonce='
   
   if match.group(0):
      result = match.group(0)
      return description_url + result[0:-4]
   else:
      return description_url


def buildLogicImmoUrl(ptype, stype):
   if stype == BUY_ID:
      return logicimmo_houilles_sale_baseurl + getLogicImmoPropertiesId(ptype)
   else:
      return logicimmo_houilles_rent_baseurl + getLogicImmoPropertiesId(ptype)
 
