#!/usr/bin/env python
# -*- coding: utf-8 -*- 
# (c) Copyright 2019 
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

config_spimo = configparser.ConfigParser()
config_spimo.read('spiders/config.ini')

ip = config_spimo['COMMON']['db_access_ip']
port = int(config_spimo['COMMON']['db_access_port'])
tablename = config_spimo['COMMON']['tablename']

class StephanePlazaImoSpider(scrapy.Spider):
   
   name = "stephaneplazaimo"

   def __init__(self, city='', **kwargs):
      self.city = city
      self.images_folder_name = config_spimo[city.upper()]['images']
      self.region = config_spimo[city.upper()]['region']

      if not os.path.exists(self.images_folder_name):
         os.mkdir(self.images_folder_name)

      
      self.mapping_url_ptype = dict()
      self.mapping_url_stype = dict()
      self.announces_cnt = 0
      self.announce_title = dict()

      self.serializer = Serializer(ip, port, tablename)
      self.ads = self.serializer.scanidByCityAndAdSource(city, "stephaneplazaimo")

   def start_requests(self):
      prop_list = [(APART_ID, BUY_ID), (HOUSE_ID, BUY_ID), (APART_ID, RENT_ID), (HOUSE_ID, RENT_ID)]
            
      for ptype, stype in prop_list:
         announcers_id = getStephanePlazaImoPropertiesId(ptype)
         search_id = getStephanePlazaImoSearchTypeId(stype)        

         url = buildStephanePlazaImoUrl(self.city, announcers_id, search_id)
         self.mapping_url_ptype[url] = ptype
         self.mapping_url_stype[url] = stype
         yield scrapy.Request(url=url, callback= lambda r, ptype = ptype, stype = stype :self.parse(r, ptype, stype))

   def parse(self, response, ptype, stype):
     
      all_data = json.loads(response.text)
       
      for data in all_data["results"]:
         _id = data["id"]
         slug = data["slug"]

         announce_url = "https://www.stephaneplazaimmobilier.com/immobilier-acheter/" + str(_id) + "/" + slug
         
         ID = hash_id(announce_url)
         if str(ID) not in self.ads:
            json_data = json.dumps(data)
            announce_title = data["title"]
            announce_image = data["thumbnails"][0]
            img_cnt = len(data["thumbnails"])
         
            image_count = 1
            for img in data["thumbnails"]:
               image_name = os.path.join(self.images_folder_name, str(ID) + '_' + str(image_count) + '.jpg')
               urllib.urlretrieve(img, image_name)
               image_count = image_count + 1

 
            ret = self.serializer.send(ID, ptype, json_data, self.city, self.region, announce_url, "stephaneplazaimo", announce_title, stype, announce_image, img_cnt)
         
            print (ret)
            self.announces_cnt += 1
         else:
            self.updateTimeStamp(ID)


               
   def closed(self, reason):
      print("Announces found: " + str(self.announces_cnt) + "\n")
      self.serializer.close()

