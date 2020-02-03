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

search_base_url = "https://www.bienici.com/realEstateAds.json?filters="


ad_base_url = 'https://www.bienici.com/annonce/vente/'


config_bienici = configparser.ConfigParser()
config_bienici.read('spiders/config.ini')

ip = config_bienici['COMMON']['db_access_ip']
port = int(config_bienici['COMMON']['db_access_port'])
tablename = config_bienici['COMMON']['tablename']

class BieniciSpider(scrapy.Spider):
   
   name = "bienici"

   def __init__(self, city='', **kwargs):
      self.city = city
      self.images_folder_name = config_bienici[city.upper()]['images']
      self.region = config_bienici[city.upper()]['region']

      if not os.path.exists(self.images_folder_name):
         os.mkdir(self.images_folder_name)

      self.mapping_url_ptype = dict()
      self.mapping_url_stype = dict()
      self.announces_cnt = 0
      self.serializer = Serializer(ip, port, tablename)
      self.ads = self.serializer.scanidByCityAndAdSource(city,"bienici")

   def start_requests(self):
      prop_list = [(APART_ID, BUY_ID), (HOUSE_ID, BUY_ID), (APART_ID, RENT_ID), (HOUSE_ID, RENT_ID)]
      
      for ptype, stype in prop_list:
         prop_id = getBienIciPropertiesId(ptype)
         search_id = getBienIciSearchTypeId(stype)        

         url = buildBienIciUrl(self.city, prop_id, search_id) 
         self.mapping_url_ptype[url] = ptype
         self.mapping_url_stype[url] = stype
         yield scrapy.Request(url=url, callback= lambda r, nextpage = 2: self.parse(r, nextpage))


   def parse(self, response, nextpage):
      data = json.loads(response.text)
      ad_url = ""
      property_type = 0
      search_type = 0

      
      for ad in data['realEstateAds']:
         if ad['propertyType'] == 'flat':
            ad_url = ad_base_url + self.city +'/appartement/'+ ad['id']
            property_type = APART_ID
         else:
            ad_url = ad_base_url + self.city + '/maison/' + ad['id']
            property_type = HOUSE_ID
      

         if ad['adType'] == 'buy':
            search_type = BUY_ID
         else:
            search_type = RENT_ID

         ID = hash_id(ad_url)

         if str(ID) not in self.ads:
            # store images on disk
            images = ad['photos']

            if images:
               announce_image = images[0]['url']

            image_cnt = 1

            for img in images:
               image_name = os.path.join(self.images_folder_name, str(ID) + '_' + str(image_cnt) + '.jpg')
               urllib.urlretrieve(img['url'], image_name)
               image_cnt += 1


            self.announces_cnt += 1
            announce_title = ""
            if not ad['title']:
               announce_title = "Appartement" if property_type == APART_ID else "Maison"
            else:
               announce_title = ad['title']

            # send data to db
            ret = self.serializer.send(ID, property_type, json.dumps(ad), self.city, self.region, ad_url, 'bienici', announce_title, search_type, announce_image, image_cnt-1)
            print (ret)

         else:
            self.serializer.updateTimeStamp(ID)
      if (data['perPage'] + data['from']) < data['total'] :
         # check for next data   
         original_request = str(response.request)
         filter_str = urllib.unquote(original_request[56:-1])
         filter_object = json.loads(filter_str)

         filter_object['size'] =  data['total'] - data['from'] if  data['from'] + 24 > data['total'] else 24
         filter_object['from'] = data['from'] + data['perPage']
         filter_object['page'] = nextpage + 1

         url = search_base_url + json.dumps(filter_object)

         yield scrapy.Request(url=url, callback=lambda r, nextpage=nextpage+1: self.parse(r, nextpage))
     
 
   def closed(self, reason):
      print("Announces found: " + str(self.announces_cnt) + "\n")
      self.serializer.close()




