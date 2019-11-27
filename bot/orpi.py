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


config_orpi = configparser.ConfigParser()
config_orpi.read('spiders/config.ini')

IMAGES_FOLDER_NAME=config_orpi['COMMON']['images']
city = config_orpi['COMMON']['city']
region = config_orpi['COMMON']['region']
ip = config_orpi['COMMON']['db_access_ip']
port = int(config_orpi['COMMON']['db_access_port'])



class OrpiSpider(scrapy.Spider):
   
   name = "orpi"

   def __init__(self):
      if not os.path.exists(IMAGES_FOLDER_NAME):
         os.mkdir(IMAGES_FOLDER_NAME)

      self.mapping_url_ptype = dict()
      self.mapping_url_stype = dict()
      self.announces_cnt = 0
      self.announce_title = dict()

      self.serializer = Serializer(ip, port)

   def start_requests(self):
      prop_list = [(APART_ID, BUY_ID), (HOUSE_ID, BUY_ID), (APART_ID, RENT_ID), (HOUSE_ID, RENT_ID)]
            
      for ptype, stype in prop_list:
         announcers_id = getOrpiPropertiesId(ptype)
         search_id = getOrpiSearchTypeId(stype)        

         url = buildOrpiUrl(city, announcers_id, search_id)
         self.mapping_url_ptype[url] = ptype
         self.mapping_url_stype[url] = stype
         yield scrapy.Request(url=url, callback= lambda r, ptype = ptype, stype = stype :self.parse(r, ptype, stype))

   def parse(self, response, ptype, stype):
      all_data = response.xpath('//div[@class="o-container"]/@data-result').extract()
      
      json_data = json.loads(all_data[0])
      if "items" in json_data:
         ads_list = json_data["items"]
         for ad in ads_list:
            part_url = ad['slug']
            announce_url = ""
            if stype == BUY_ID:
               announce_url = "https://www.orpi.com/annonce-vente-" + part_url
            else:
               announce_url = "https://www.orpi.com/annonce-location-" + part_url

            yield scrapy.Request(url=announce_url, callback = lambda r, url = announce_url, ptype = ptype, stype = stype: self.parse_ad(r, url, ptype, stype))
 
   def parse_ad(self, response, url, ptype, stype):
      ID = hash_id(url)

      ad_data = response.xpath('//div[@class="o-container"]/div/@data-estate').extract()
      json_data = ad_data[0]
      json_obj = json.loads(json_data)

      img_cnt = 0
      if "images" in json_obj:
         images = json_obj["images"]
         image_count = 1
         for img in images:
            image_name = os.path.join(IMAGES_FOLDER_NAME, str(ID) + '_' + str(image_count) + '.jpg')
            urllib.urlretrieve(img, image_name)
            image_count = image_count + 1


      
         img_cnt = len(images)

      announce_image = ""
      if images:
         announce_image = images[0]

      announce_title = json_obj['slug']
      ret = self.serializer.send(ID, ptype, json_data, city, region, url, "orpi", announce_title, stype, announce_image, img_cnt)
      print (ret)
      self.announces_cnt += 1
            
   def closed(self, reason):
      print("Announces found: " + str(self.announces_cnt) + "\n")
      self.serializer.close()

