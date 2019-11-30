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


config_laforet = configparser.ConfigParser()
config_laforet.read('spiders/config.ini')

IMAGES_FOLDER_NAME=config_laforet['COMMON']['images']
city = config_laforet['COMMON']['city']
region = config_laforet['COMMON']['region']
ip = config_laforet['COMMON']['db_access_ip']
port = int(config_laforet['COMMON']['db_access_port'])
tablename = config_laforet['COMMON']['tablename']

class LaforetSpider(scrapy.Spider):
   
   name = "laforet"

   def __init__(self):
      if not os.path.exists(IMAGES_FOLDER_NAME):
         os.mkdir(IMAGES_FOLDER_NAME)

      self.announces_cnt = 0
      self.announce_title = dict()
      self.serializer = Serializer(ip, port, tablename)

   def start_requests(self):
      prop_list = [(APART_ID, BUY_ID), (HOUSE_ID, BUY_ID), (APART_ID, RENT_ID), (HOUSE_ID, RENT_ID)]
            
      for ptype, stype in prop_list:
         announcers_id = getLaforetPropertiesId(ptype)
         search_id = getLaforetSearchTypeId(stype)        

         url = buildLaforetUrl(city, announcers_id, search_id)
         yield scrapy.Request(url=url, callback= lambda r, ptype = ptype, stype = stype :self.parse(r, ptype, stype))

   def parse(self, response, ptype, stype):
      all_data = response.xpath('//ul[@class="results-compact"]').xpath('.//li[@class="js-stats-property-roll"]/@data-json').extract()

      for data in all_data:
         part_url = json.loads(data)['url']
         announce_url = "http://www.laforet.com" + part_url
         yield scrapy.Request(url=announce_url, callback = lambda r, url = announce_url, json_data = data, ptype = ptype, stype = stype: self.parse_img(r, url, json_data, ptype, stype))


   def parse_img(self, response, url, json_data, ptype, stype):
      ID = hash_id(url)
      images = response.xpath('//div[@class="cycle-slideshow"]/div/img/@src').extract()

      image_count = 1
      for img in images:
         image_name = os.path.join(IMAGES_FOLDER_NAME, str(ID) + '_' + str(image_count) + '.jpg')
         urllib.urlretrieve(img, image_name)
         image_count = image_count + 1

      img_cnt = len(images)
      data = json.loads(json_data)
      announce_image = data['imageUrl']
      announce_title = data['title']
      ret = self.serializer.send(ID, ptype, json_data, city, region, url, "laforet", announce_title, stype, announce_image, img_cnt)
      print (ret)
      self.announces_cnt += 1
            
   def closed(self, reason):
      print("Announces found: " + str(self.announces_cnt) + "\n")
      self.serializer.close()

