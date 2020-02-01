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

from hash_id import *
from search_features import *
from url_builder import *
from Serializer import Serializer


config_guyhoquet = configparser.ConfigParser()
config_guyhoquet.read('spiders/config.ini')

ip = config_guyhoquet['COMMON']['db_access_ip']
port = int(config_guyhoquet['COMMON']['db_access_port'])
tablename = config_guyhoquet['COMMON']['tablename']


def remove_accent(string):
   string = string.replace(u'è',u'e')
   string = string.replace(u'é', u'e')
   string = string.replace(u'É', u'e')

   return string

class GuyHoquetSpider(scrapy.Spider):
   
   name = "guyhoquet"

   def __init__(self, city= '', **kwargs):
      self.city = city
      self.images_folder_name=config_guyhoquet[city.upper()]['images']
      self.region = config_guyhoquet[city.upper()]['region']

      if not os.path.exists(self.images_folder_name):
         os.mkdir(self.images_folder_name)

      
      self.announces_cnt = 0
      self.serializer = Serializer(ip, port, tablename)
      self.ads = self.serializer.scanidByCityAndAdSource(city, "guyhoquet")

      self.fieldmapping = dict()
      self.fieldmapping['Cave(s)'] = 'CELLAR'
      self.fieldmapping['Nombre pieces'] = 'ROOMS'
      self.fieldmapping['Prix'] = 'PRICE'
      self.fieldmapping['Etage'] = 'FLOOR'
      self.fieldmapping['Rez de chaussee'] = 'FLOOR'
      self.fieldmapping['Surface'] = 'SURFACE'
      self.fieldmapping['Nombre places parking'] = 'PARKING'
      self.fieldmapping['Ascenseur'] = 'LIFT'
      self.fieldmapping['Type Chauffage'] = 'TYPE_OF_HEATING'
      self.fieldmapping['Annee de construction'] = 'CONSTRUCTION_YEAR'
      self.fieldmapping['Mode Chauffage'] = 'TYPE_OF_HEATING'

   def start_requests(self):
      prop_list = [(APART_ID, BUY_ID), (HOUSE_ID, BUY_ID), (APART_ID, RENT_ID), (HOUSE_ID, RENT_ID)]
            
      for ptype, stype in prop_list:
         announcers_id = getGuyHoquetPropertiesId(ptype)
         search_id = getGuyHoquetSearchTypeId(stype)        

         url = buildGuyHoquetUrl(self.city, announcers_id, search_id)
         yield scrapy.Request(url=url, callback= lambda r, ptype = ptype, stype = stype :self.parse(r, ptype, stype))

   def parse(self, response, ptype, stype):
      results = response.xpath('//div[@id="main-content"]/div/div[contains(@class, "listing-bloc")]/div[@id="main-result_carto_content"]')

      links = results.xpath('.//div/div[@id="result_carto_listing"]/div[@id="listing_bien"]/div/div/div/div[@class="div-title"]/a/@href').extract()

      for url in links:
         announce_url = 'https://www.guy-hoquet.com' + url[2:]
         ID = hash_id(announce_url)
         if str(ID) not in self.ads:
            yield scrapy.Request(url=announce_url, callback = lambda r, url = announce_url, ptype = ptype, stype = stype: self.parse_announce(r, url, ptype, stype))
         else:
            self.serializer.updateTimeStamp(ID)

   def parse_announce(self, response, url, ptype, stype):
      ID = hash_id(url)
      data = dict()
      
      #announce details
      details = response.xpath('//div[@class="fiche-bien"]/div/div[@class="contenu"]/div/div/div[@class="criteres"]/div/div/div[@class="bloc_tech"]')

      chauffage = ""
      for info in details:
         field = info.xpath('.//strong/text()').extract()
         value = info.xpath('.//p/text()').extract()
         
         if field and value:
            f = remove_accent(field[0])
            if f in self.fieldmapping:
               if f == "Type Chauffage" or f == "Mode Chauffage":
                  chauffage += value[0]
                  chauffage += " "
               else:
                  if f == "Rez de chaussee":
                     if value[0] == "Oui":
                        data[self.fieldmapping[f]] = "rez-de-chaussée"
                  else:
                     data[self.fieldmapping[f]] = value[0] 

      if chauffage:
         data['TYPE_OF_HEATING'] = chauffage.strip()
      # get images
      images = response.xpath('//div[@class="fiche-bien"]/div/div/div[@class="flexslider"]/ul/li/a/img/@src').extract()
      
      image_count = 1
      for img in images:
         image_name = os.path.join(self.images_folder_name, str(ID) + '_' + str(image_count) + '.jpg')
         urllib.urlretrieve(img, image_name)
         image_count = image_count + 1

      # prepare data and send it!
      img_cnt = len(images)

      json_data = json.dumps(data)
      
      announce_image = ""
      if images:
         announce_image = images[0]

      text_title = response.xpath('//div[@class="fiche-bien"]/div/div/div[@class="infos-fixed"]/div/div/p[@class="titre"]/text()').extract()
      announce_title = ""
      if text_title:
         announce_title = text_title[0]

      ret = self.serializer.send(ID, ptype, json_data, self.city, self.region, url, "guyhoquet", announce_title, stype, announce_image, img_cnt)
      print (ret)
      self.announces_cnt += 1
            
   def closed(self, reason):
      print("Announces found: " + str(self.announces_cnt) + "\n")
      self.serializer.close()

