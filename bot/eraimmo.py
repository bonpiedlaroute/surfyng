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


config_eraimmo = configparser.ConfigParser()
config_eraimmo.read('spiders/config.ini')

ip = config_eraimmo['COMMON']['db_access_ip']
port = int(config_eraimmo['COMMON']['db_access_port'])
tablename = config_eraimmo['COMMON']['tablename']


def remove_accent(string):
   string = string.replace(u'è',u'e')
   string = string.replace(u'é', u'e')
   string = string.replace(u'É', u'e')

   return string

class EraimmoSpider(scrapy.Spider):
   
   name = "eraimmo"

   def __init__(self, city= '', **kwargs):
      self.city = city
      self.images_folder_name=config_eraimmo[city.upper()]['images']
      self.region = config_eraimmo[city.upper()]['region']

      if not os.path.exists(self.images_folder_name):
         os.mkdir(self.images_folder_name)

      
      self.announces_cnt = 0
      self.serializer = Serializer(ip, port, tablename)
      self.ads = self.serializer.scanidByCityAndAdSource(city, "eraimmo")

      self.fieldmapping = dict()
      self.fieldmapping['Cave'] = 'CELLAR'
      self.fieldmapping['Nbre de pieces'] = 'ROOMS'
      self.fieldmapping['Surface habitable'] = 'SURFACE'
      self.fieldmapping['Garage'] = 'PARKING'
      self.fieldmapping['Ascenseur'] = 'LIFT'
      self.fieldmapping['Nbre de chambres'] = 'BEDROOMS'
      self.fieldmapping['Superficie terrain'] = 'LAND_SURFACE'

   def start_requests(self):
      prop_list = [(APART_ID, BUY_ID), (HOUSE_ID, BUY_ID)]
            
      for ptype, stype in prop_list:
         announcers_id = getEraImmoPropertiesId(ptype)
         search_id = getEraImmoSearchTypeId(stype)        

         url = buildEraImmoUrl(self.city, announcers_id, search_id)
         yield scrapy.Request(url=url, callback= lambda r, ptype = ptype, stype = stype :self.parse(r, ptype, stype))

   def parse(self, response, ptype, stype):

      links = response.xpath('//div[@id="listing_bien"]/div/div/div/div[@class="filigrane_apercu"]/a/@href').extract()

      for url in links:
         pos = url.find('?')
         part_url = url[:pos]
         announce_url = "https://www.erafrance.com" + part_url[2:]
         ID = hash_id(announce_url)
         if str(ID) not in self.ads:
            yield scrapy.Request(url=announce_url, callback = lambda r, url = announce_url, ptype = ptype, stype = stype: self.parse_announce(r, url, ptype, stype))
         else:
            self.serializer.updateTimeStamp(ID)

   def parse_announce(self, response, url, ptype, stype):
      ID = hash_id(url)
      data = dict()
 
      #price
      price_value = response.xpath('//div[@class="container"]/div[@class="row"]/div/div/div/div[@class="prix_bien"]/p/span/span/span[@class="hono_inclus_price"]/text()').extract()

      price = ""
      if price_value:
         price = price_value[0].encode('ascii', 'ignore')
      else:
         price_value = response.xpath('//div[@class="container"]/div[@class="row"]/div/div/div/div[@class="prix_bien"]/p/text()').extract()
         if price_value:
            price = price_value[0].encode('ascii', 'ignore')
         else:
            return
      
      data['PRICE'] = price
      #announce details
      details = response.xpath('//div[@class="container"]/div/div/div/div[@class="criteres-list"]/ul/li')

      chauffage = ""
      for info in details:
         field = info.xpath('.//div[@class="critere-title"]/text()').extract()
         value = info.xpath('.//div[@class="critere-value"]/text()').extract()
         
         if field and value:
            f = remove_accent(field[0])
            if f in self.fieldmapping:
               data[self.fieldmapping[f]] = value[0] 

      # get images
      images = response.xpath('//div[@class="container"]/div[@id="flex_slider_bien"]/ul/li/a[@class="link_img_bien"]/@href').extract()
      
      image_count = 1
      for img in images:
         image_name = os.path.join(self.images_folder_name, str(ID) + '_' + str(image_count) + '.jpg')
         img_url = "https://www.erafrance.com" + img[2:]
         urllib.urlretrieve(img_url, image_name)
         image_count = image_count + 1

      # prepare data and send it!
      img_cnt = len(images)

      json_data = json.dumps(data)
      
      announce_image = ""
      if images:
         announce_image = "https://www.erafrance.com" + images[0][2:]

      text_title = response.xpath('//title/text()').extract()
      announce_title = ""
      if text_title:
         announce_title = text_title[0]

      ret = self.serializer.send(ID, ptype, json_data, self.city, self.region, url, "eraimmo", announce_title, stype, announce_image, img_cnt)
      print (ret)
      self.announces_cnt += 1
            
   def closed(self, reason):
      print("Announces found: " + str(self.announces_cnt) + "\n")
      self.serializer.close()

