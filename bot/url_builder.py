#!/usr/bin/env python
# -*- coding: utf-8 -*- 
# (c) Copyright 2017 Noel Tchidjo
# All rights reserved

import re
import json
from search_features import *
from inseecode_postalcode import *



seloger_prop_type_pos = 39
seloger_search_type_pos = 49

seloger_houilles_url = 'https://www.seloger.com/list.htm?idtt=&naturebien=1,2,4&idtypebien=&ci=780311'
seloger_colombes_url = 'https://www.seloger.com/list.htm?types=&projects=&enterprise=0&natures=1&places=[{ci:920025}]&qsVersion=1.0'
seloger_paris_url    = 'https://www.seloger.com/list.htm?idtt=&naturebien=1,2,4&idtypebien=&ci=750056'


#logicimmo_houilles_sale_baseurl = 'http://www.logic-immo.com/vente-immobilier-houilles-78800,13772_2/options/groupprptypesids='
#logicimmo_houilles_rent_baseurl = 'http://www.logic-immo.com/location-immobilier-houilles-78800,13772_2/options/groupprptypesids='

logicimmo_paris_sale_baseurl = 'http://www.logic-immo.com/vente-immobilier-paris-75,100_1/options/groupprptypesids='
logicimmo_paris_rent_baseurl = 'http://www.logic-immo.com/location-immobilier-paris-75,100_1/options/groupprptypesids='



lbc_base_url = 'https://www.leboncoin.fr/recherche/?category='


def insert_in_url(url, position, value):
   return url[:position] + value + url[position:]

def buildselogerurl(proptype, searchtype):  
   url = insert_in_url(seloger_colombes_url,seloger_search_type_pos, searchtype)
   url = insert_in_url(url,seloger_prop_type_pos, proptype)

   return url


def buildleboncoinurl(proptype, searchtype, city):
   url = lbc_base_url + searchtype
   url += "&locations="
   town = city[0].upper() + city[1:]
   url += town
   url += "_"
   url += postalcodeByCity[city]
   url +="&real_estate_type="
   url += proptype

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

laforet_url = 'https://www.laforet.com/api/immo/properties?cities='

def buildLaforetUrl(city, ptype, stype):
   url = laforet_url + inseecodeByCity[city]
   url += "&types="
   url += ptype
   url += "&transaction="
   url += stype

   return url

orpi_url = 'https://www.orpi.com/recherche/'

def buildOrpiUrl(city, ptype, stype):
   url = orpi_url + stype
   url += "?realEstateTypes[0]="
   url += ptype
   url += "&locations[0][value]="
   url += city   

   return url

stephaneplazaimo_url = "https://www.stephaneplazaimmobilier.com/search/"

def buildStephanePlazaImoUrl(city, ptype, stype):
   url = stephaneplazaimo_url + stype
   url += "?target="
   url += stype
   url += "&type[]="
   url += ptype
   url += "&location[]="
   url += inseecodeByCity[city]

   return url

foncia_url = "https://fr.foncia.com/"

def buildFonciaUrl(city, ptype, stype):
   url = foncia_url + stype + '/'
   url += city
   url += '-'
   url += postalcodeByCity[city]
   url += '/'
   url += ptype
   return url

century21_url = "https://www.century21.fr/annonces/"

def buildCentury21Url(city, ptype, stype):
   url = century21_url + stype
   url += '-'
   url += ptype
   url += '/v-'
   url += city
   return url

guyhoquet_url = "https://www.guy-hoquet.com/catalog/result_carto.php?action=update_search&C_28_search=EGAL&C_28_type=UNIQUE&C_65_search=CONTIENT&C_65_type=TEXT&"

def buildGuyHoquetUrl(city, ptype, stype):
   url = guyhoquet_url + "C_28="
   url += stype
   url += "&cfamille_id="
   url += ptype
   url += "&C_65="
   url += postalcodeByCity[city]
   url += "+"
   url += city
   
   return url

bienici_base_filter = "{\"size\":24,\"from\":0,\"filterType\":\"buy\",\"propertyType\":[\"flat\"],\"newProperty\":false,\"page\":1,\"resultsPerPage\":24,\"maxAuthorizedResults\":2400,\"sortBy\":\"relevance\",\"sortOrder\":\"desc\",\"onTheMarket\":[true],\"mapMode\":\"enabled\",\"showAllModels\":false,\"zoneIdsByTypes\":{\"zoneIds\":[\"-91738\"]}}"

bienici_base_url = "https://www.bienici.com/realEstateAds.json?filters="


def buildBienIciUrl(city, ptype, stype):
   filter_object = json.loads(bienici_base_filter)
   inseecode = inseecodeByCity[city]
   osmid = osmidByInseeCode[inseecode] 
   filter_object['zoneIdsByTypes']['zoneIds'][0] = '-' + osmid
   
   filter_object['propertyType'][0] = ptype
   filter_object['filterType'] = stype

   filter_str = json.dumps(filter_object)
   url = bienici_base_url + filter_str

   return url

arthurimmo_url = "https://www.arthurimmo.com/recherche,basic.htm?ci="
def buildArthurImmoUrl(city, ptype, stype):
   url = arthurimmo_url + inseecodeByCity[city][:2] + "0" + inseecodeByCity[city][2:]
   url += "&idtt="
   url += stype
   url += "&idtypebien="
   url += ptype
   url += "&saisie="
   url += city
   url += "&tri=d_dt_crea"
   
   return url


