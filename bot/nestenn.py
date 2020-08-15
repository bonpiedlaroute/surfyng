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


config_nestenn = configparser.ConfigParser()
config_nestenn.read('spiders/config.ini')

ip = config_nestenn['COMMON']['db_access_ip']
port = int(config_nestenn['COMMON']['db_access_port'])
tablename = config_nestenn['COMMON']['tablename']
max_pages = int(config_nestenn['COMMON']['max_pages'])




class NestennSpider(scrapy.Spider):
   
   name = "nestenn"

   def __init__(self, city= '', **kwargs):
      self.city = city
      self.images_folder_name=config_nestenn[city.upper()]['images']
      self.region = config_nestenn[city.upper()]['region']
      
      if not os.path.exists(self.images_folder_name):
         os.mkdir(self.images_folder_name)

      
      self.announces_cnt = 0
      self.serializer = Serializer(ip, port, tablename)
      self.ads = self.serializer.scanidByCityAndAdSource(city, "nestenn")

      
   def start_requests(self):
      prop_list = [(APART_ID, BUY_ID), (HOUSE_ID, BUY_ID), (APART_ID, RENT_ID), (HOUSE_ID, RENT_ID)]

            
      for ptype, stype in prop_list:
         prop_id = getNestennPropertiesId(ptype)
         search_id = getNestennSearchTypeId(stype)        

         url = buildNestennUrl(self.city, prop_id, search_id)
         yield scrapy.Request(url=url, callback= lambda r, ptype = ptype, stype = stype :self.parse(r, ptype, stype))

   def parse(self, response, ptype, stype):

      list_text = response.xpath('//div[@class="container"]/section[@id="listing"]/div/div/div[@id="annonceBox"]/div/div[contains(@class, "listing_annonce")]/div/script/text()').extract()
      links = []
      for text in list_text:
         text_encoded = text.encode('utf-8')

         text_in_html = scrapy.http.HtmlResponse(url='https://nestenn.com', body=text_encoded, encoding='utf-8')

         part_url = text_in_html.xpath('//a/@href').extract()
         if part_url:
            links.append(part_url[0])
 
      for link in links:
         announce_url = 'https://www.nestenn.fr' + link
         ID = hash_id(announce_url)

         if str(ID) not in self.ads:
            yield scrapy.Request(url=announce_url, callback = lambda r, url = announce_url, ptype = ptype, stype = stype: self.parse_announce(r, url, ptype, stype))
         else:
            self.serializer.updateTimeStamp(ID)

      
   def parse_announce(self, response, url, ptype, stype):
      ID = hash_id(url)
      data = dict()
 
      #price
      raw_price = response.xpath('//section[@id="annonce_entete"]/div[@class="container"]/div/div/div/div/div[@class="content_prix"]/text()').extract()
      px = raw_price[0].replace('\t', '')
      px = px.replace('\n', '')
      px = px.replace(' ', '')
      pos  = px.find(u'\u20ac') 
      data['PRICE'] = px[:pos] 
      
      #announce details
      details = response.xpath('//section[@id="annonce_detail"]/div/div[@id="groupeIcon"]/div[contains(@class,"blockIcon")]/div/text()').extract()
      for info in details:
         if 'pieces' in info:
            pos = info.find(' ')
            data["ROOMS"] = info[:pos]
         if 'ascenseur' in info:
            data["LIFT"] = "oui"
         if 'cave' in info:
            pos = info.find(' ')
            data["CELLAR"] = info[:pos]
         if 'etage' in info:
            pos = info.find(' ')
            data["FLOOR"] = info[:pos]
         if 'parking' in info:
            pos = info.find(' ')
            data["PARKING"] = info[:pos]
         if 'habitable' in info:
            pos = info.find(' ')
            data["SURFACE"] = info[:pos]
         if 'chambres' in info:
            pos = info.find(' ')
            data["BEDROOMS"] = info[:pos]
 
 
      # get images 
      images = response.xpath('//section[@id="annonce_entete"]/div/div/div/div/div[@class="slider_bien"]/a/@href').extract()
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

      ret = self.serializer.send(ID, ptype, json_data, self.city, self.region, url, "nestenn", announce_title, stype, announce_image, img_cnt)
      print (ret)
      self.announces_cnt += 1
            
   def closed(self, reason):
      print("Announces found: " + str(self.announces_cnt) + "\n")
      self.serializer.close()

