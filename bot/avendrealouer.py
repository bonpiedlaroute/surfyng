#!/usr/bin/env python
# -*- coding: utf-8 -*- 
# (c) Copyright 2017-2020 
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


config_avendrealouer = configparser.ConfigParser()
config_avendrealouer.read('spiders/config.ini')

ip = config_avendrealouer['COMMON']['db_access_ip']
port = int(config_avendrealouer['COMMON']['db_access_port'])
tablename = config_avendrealouer['COMMON']['tablename']
max_pages = int(config_avendrealouer['COMMON']['max_pages'])

def remove_accent(string):
   string = string.replace(u'è',u'e')
   string = string.replace(u'é', u'e')
   string = string.replace(u'É', u'e')
   return string

class AvendreAlouerSpider(scrapy.Spider):
   
   name = "avendrealouer"

   def __init__(self, city= '', **kwargs):
      self.city = city
      self.images_folder_name=config_avendrealouer[city.upper()]['images']
      self.region = config_avendrealouer[city.upper()]['region']
      self.mapping = dict()
      self.mapping['Surface: '] = "SURFACE"
      self.mapping['Piece(s): '] = "ROOMS"
      self.mapping['etage du bien: '] = "FLOOR"
      self.mapping['Chauffage: '] = "TYPE_OF_HEATING"
      self.mapping['Construit en: '] = "CONSTRUCTION_YEAR"
      self.mapping['Parking: '] = "PARKING"
      self.mapping['Ascenseur'] = "LIFT"
      self.mapping['Cave'] = "CELLAR"
      
      if not os.path.exists(self.images_folder_name):
         os.mkdir(self.images_folder_name)

      
      self.announces_cnt = 0
      self.serializer = Serializer(ip, port, tablename)
      self.ads = self.serializer.scanidByCityAndAdSource(city, "avendrealouer")

      
   def start_requests(self):
      prop_list = [(APART_ID, BUY_ID), (HOUSE_ID, BUY_ID), (APART_ID, RENT_ID), (HOUSE_ID, RENT_ID)]

            
      for ptype, stype in prop_list:
         prop_id = getAvendreAlouerPropertiesId(ptype)
         search_id = getAvendreAlouerSearchTypeId(stype)        

         url = buildAvendreAlouerUrl(self.city, prop_id, search_id)
         yield scrapy.Request(url=url, callback= lambda r, ptype = ptype, stype = stype :self.parse(r, ptype, stype, 1))

   def parse(self, response, ptype, stype, page):

      links = response.xpath('//section/div[@class="container-lr"]/div/div/div[@class="property-list"]/ul[@id="result-list"]/li/a/@href').extract()
 
      for link in links:

         if "programme-neuf" not in link:
            ID = hash_id(link)
            if str(ID) not in self.ads:
               announce_url = 'https://www.avendrealouer.fr' + link
               yield scrapy.Request(url=announce_url, callback = lambda r, url = announce_url, ptype = ptype, stype = stype: self.parse_announce(r, url, ptype, stype))
            else:
               self.serializer.updateTimeStamp(ID)

      if page == max_pages:
         return
      else:
         page += 1
         next_links = response.xpath('//div[@class="container-lr"]/div/div/div/div[@class="pagination-block"]/ul[@class="pagination"]/li/a[@class="next"]/@href').extract()
         if next_links:
            next_link = 'https://www.avendrealouer.fr' + next_links[0]
            yield response.follow(next_link, callback= lambda r, ptype = ptype, stype = stype :self.parse(r, ptype, stype, page))


   def parse_announce(self, response, url, ptype, stype):
      ID = hash_id(url)
      data = dict()
 
      #price
      price = response.xpath('//div[@class="page-wrapper"]/div/div/div[@class="price"]/div/span/text()').extract()
      if price:
         px = price[0].encode('ascii', 'ignore')

      data['PRICE'] = px
      
      #announce details
      details = response.xpath('//div[@class="page-wrapper"]/div[@class="main-pane"]/div/div/div[@class="property-description"]/div[@class="property-description-characteristics"]/table/tr/td/span/text()').extract() 
      
      heating = ""
      for i in range(len(details)):
         value = remove_accent(details[i])
         if value == "Surface: ":
            raw_area = details[i+1]
            pos = raw_area.find(' ')
            area = raw_area[:pos]
            data[self.mapping[value]] = area
         if value == "Piece(s): ":
            data[self.mapping[value]] = details[i+1]
         if value == "etage du bien: ":
            raw_floor = details[i+1]
            pos = raw_floor.find('/')
            floor = raw_floor[:pos]
            data[self.mapping[value]] = floor
         if value == "Chauffage: ":
            heating += details[i+1]
            heating += " "

         if value == "Construit en: ":
            data[self.mapping[value]] = details[i+1]
         if value == "Parking: ":
            data[self.mapping[value]] = details[i+1]
         if value == "Ascenseur":
            data[self.mapping[value]] = "oui"
         if value == "Cave":
            data[self.mapping[value]] = "1"

      if heating:
         real_heating = "Chauffage " + heating[:-1]
         data["HEATING"] = real_heating
 
      # get images
      images = response.xpath('//div[@class="page-wrapper"]/div[@class="main-pane"]/div[@class="tabPanel"]/div[@id="property-tab-content"]/div/div[@id="bxSliderContainer"]/ul/li/img/@src').extract()
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

      text_title = response.xpath('//title/text()').extract()
      announce_title = ""
      if text_title:
         announce_title = text_title[0]

      ret = self.serializer.send(ID, ptype, json_data, self.city, self.region, url, "avendrealouer", announce_title, stype, announce_image, img_cnt)
      print (ret)
      self.announces_cnt += 1
            
   def closed(self, reason):
      print("Announces found: " + str(self.announces_cnt) + "\n")
      self.serializer.close()

