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


lbc_base_url = 'https://www.leboncoin.fr/recherche/?category=&locations=&real_estate_type='

lbc_searchtype_pos = 45
lbc_city_pos9 = 57
lbc_city_pos10 = 58


def insert_in_url(url, position, value):
   return url[:position] + value + url[position:]

def buildselogerurl(proptype, searchtype):  
   url = insert_in_url(seloger_houilles_url,seloger_search_type_pos, searchtype)
   url = insert_in_url(url,seloger_prop_type_pos, proptype)

   return url


def buildleboncoinurl(proptype, searchtype, city):
   url = insert_in_url(lbc_base_url, lbc_searchtype_pos,searchtype)

   lbc_city_pos = lbc_city_pos9 if searchtype == '9' else lbc_city_pos10

   url = insert_in_url(url, lbc_city_pos, city)

   lastpos = len(url)

   url = insert_in_url(url, lastpos, proptype)

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
 
