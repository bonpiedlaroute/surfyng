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


config_iadfrance = configparser.ConfigParser()
config_iadfrance.read('spiders/config.ini')

ip = config_iadfrance['COMMON']['db_access_ip']
port = int(config_iadfrance['COMMON']['db_access_port'])
tablename = config_iadfrance['COMMON']['tablename']


def remove_accent(string):
   string = string.replace(u'è',u'e')
   string = string.replace(u'é', u'e')
   string = string.replace(u'É', u'e')

   return string

class IadfranceSpider(scrapy.Spider):
   
   name = "iadfrance"

   def __init__(self, city='', **kwargs):
      self.city = city
      self.images_folder_name=config_iadfrance[city.upper()]['images']
      self.region = config_iadfrance[city.upper()]['region']

      if not os.path.exists(self.images_folder_name):
         os.mkdir(self.images_folder_name)

      self.announces_cnt = 0
      self.serializer = Serializer(ip, port, tablename)
      self.ads = self.serializer.scanidByCityAndAdSource(city, "iadfrance")

      self.fieldmapping = dict()
      self.fieldmapping['Surface'] = 'SURFACE'
      self.fieldmapping['Nb de pieces'] = 'ROOMS'
      self.fieldmapping['Annee de construction'] = 'CONSTRUCTION_YEAR'
      self.fieldmapping['Nb de chambres'] = 'BEDROOMS'
      self.fieldmapping['Surface terrain'] = 'LAND_SURFACE'

   def start_requests(self):
      prop_list = [(APART_ID, BUY_ID), (HOUSE_ID, BUY_ID), (APART_ID, RENT_ID), (HOUSE_ID, RENT_ID)]
            
      for ptype, stype in prop_list:
         announcers_id = getIadfrancePropertiesId(ptype)
         search_id = getIadfranceSearchTypeId(stype)        

         url = buildIadfranceUrl(self.city, announcers_id, search_id)
         yield scrapy.Request(url=url, callback= lambda r, ptype = ptype, stype = stype :self.parse(r, ptype, stype))

   def parse(self, response, ptype, stype):
      links = response.xpath('//div[contains(@class, "l-result__list")]/div/div/div/div/a/@href').extract()
      key = '-'+self.city+'-'
      for url in links:
         if key in url:
            announce_url = 'https://iadfrance.fr' + url
            ID = hash_id(announce_url)
            if str(ID) not in self.ads:
               yield scrapy.Request(url=announce_url, callback = lambda r, url = announce_url, ptype = ptype, stype = stype: self.parse_announce(r, url, ptype, stype))
            else:
               self.serializer.updateTimeStamp(ID)

   def parse_announce(self, response, url, ptype, stype):
      ID = hash_id(url)
      data = dict()
      
      #get price
      raw_price = response.xpath('//div[@class="offer__price"]/div/p[@class="price"]/text()').extract()
      if raw_price:
         px = raw_price[0]
         price = px.strip()

         data['PRICE'] = price
      else:
         return

      #announce details
      details = response.xpath('//div[@class="offer"]/div/div/div/div/div/p/text()').extract()
      for i in range(len(details)):
         value = remove_accent(details[i])
         if value == 'Surface':
            data[self.fieldmapping[value]] = details[i+1]
         if value == 'Nb de pieces':
            data[self.fieldmapping[value]] = details[i+2]
         if value == 'Annee de construction':
            data[self.fieldmapping[value]] = details[i+1]
         if value == 'Nb de chambres':
            data[self.fieldmapping[value]] = details[i+2]
         if value == 'Surface terrain':
            data[self.fieldmapping[value]] = details[i+2]



      desc = response.xpath('//*[@id="fiche-bien"]/div[5]/div/div/div/div[2]/p/text()').extract()
      if desc:
         data['AD_TEXT_DESCRIPTION'] = desc[0]

      # get images
      images = response.xpath('//div[contains(@class, "offer__slider-item")]/img/@src').extract()
      
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
      announce_title = text_title[0]

      ret = self.serializer.send(ID, ptype, json_data, self.city, self.region, url, "iadfrance", announce_title, stype, announce_image, img_cnt)
      print (ret)
      self.announces_cnt += 1
            
   def closed(self, reason):
      print("Announces found: " + str(self.announces_cnt) + "\n")
      self.serializer.close()

