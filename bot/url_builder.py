#!/usr/bin/env python
# -*- coding: utf-8 -*- 
# (c) Copyright 2017 Noel Tchidjo
# All rights reserved

import re
from search_features import *

inseecodeByCity = dict()
inseecodeByCity['colombes'] = '92025'


seloger_prop_type_pos = 39
seloger_search_type_pos = 49

seloger_houilles_url = 'https://www.seloger.com/list.htm?idtt=&naturebien=1,2,4&idtypebien=&ci=780311'
seloger_colombes_url = 'https://www.seloger.com/list.htm?types=&projects=&enterprise=0&natures=1&places=[{ci:920025}]&qsVersion=1.0'
seloger_paris_url    = 'https://www.seloger.com/list.htm?idtt=&naturebien=1,2,4&idtypebien=&ci=750056'


#logicimmo_houilles_sale_baseurl = 'http://www.logic-immo.com/vente-immobilier-houilles-78800,13772_2/options/groupprptypesids='
#logicimmo_houilles_rent_baseurl = 'http://www.logic-immo.com/location-immobilier-houilles-78800,13772_2/options/groupprptypesids='

logicimmo_paris_sale_baseurl = 'http://www.logic-immo.com/vente-immobilier-paris-75,100_1/options/groupprptypesids='
logicimmo_paris_rent_baseurl = 'http://www.logic-immo.com/location-immobilier-paris-75,100_1/options/groupprptypesids='



lbc_base_url = 'https://www.leboncoin.fr/recherche/?category=&locations=&real_estate_type='

lbc_searchtype_pos = 45
lbc_city_pos9 = 57
lbc_city_pos10 = 58


def insert_in_url(url, position, value):
   return url[:position] + value + url[position:]

def buildselogerurl(proptype, searchtype):  
   url = insert_in_url(seloger_colombes_url,seloger_search_type_pos, searchtype)
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
      return logicimmo_paris_sale_baseurl + getLogicImmoPropertiesId(ptype)
   else:
      return logicimmo_paris_rent_baseurl + getLogicImmoPropertiesId(ptype)

laforet_url = 'https://www.laforet.com/api/immo/properties?cities=&types=&transaction='

def buildLaforetUrl(city, ptype, stype):
   url = insert_in_url(laforet_url,51 , inseecodeByCity[city])
   url = insert_in_url(url, 63, ptype)
   url += stype

   return url

orpi_url = 'https://www.orpi.com/recherche/?realEstateTypes[0]=&locations[0][value]='

def buildOrpiUrl(city, ptype, stype):
   url = insert_in_url(orpi_url,31,stype)
   pos = 54 if stype == 'buy' else 55
   url = insert_in_url(url, pos,ptype)
 
   url += city

   return url

stephaneplazaimo_url = "https://www.stephaneplazaimmobilier.com/search/?target=&type[]=&location[]="

def buildStephanePlazaImoUrl(city, ptype, stype):
   url = insert_in_url(stephaneplazaimo_url,47,stype)
   pos = 58 if stype == 'buy' else 58
   url = insert_in_url(url, pos, stype)
 
   pos = 69 if stype == 'buy' else 71
   url = insert_in_url(url, pos, ptype)
 
   url += inseecodeByCity[city]

   return url

postalcodeByCity = dict()
postalcodeByCity['colombes'] = '92700'

foncia_url = "https://fr.foncia.com/"

def buildFonciaUrl(city, ptype, stype):
   url = foncia_url + stype + '/'
   url += city
   url += '-'
   url += postalcodeByCity[city]
   url += '/'
   url += ptype
   return url
