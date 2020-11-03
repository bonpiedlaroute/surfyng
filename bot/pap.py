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


config_pap = configparser.ConfigParser()
config_pap.read('spiders/config.ini')

ip = config_pap['COMMON']['db_access_ip']
port = int(config_pap['COMMON']['db_access_port'])
tablename = config_pap['COMMON']['tablename']


class PapSpider(scrapy.Spider):
   
   name = "pap"

   def __init__(self, city= '', **kwargs):
      self.city = city
      self.images_folder_name=config_pap[city.upper()]['images']
      self.region = config_pap[city.upper()]['region']

      if not os.path.exists(self.images_folder_name):
         os.mkdir(self.images_folder_name)

      
      self.announces_cnt = 0
      self.serializer = Serializer(ip, port, tablename)
      self.ads = self.serializer.scanidByCityAndAdSource(city, "pap")

   def start_requests(self):
      prop_list = [(APART_ID, BUY_ID), (HOUSE_ID, BUY_ID), (APART_ID, RENT_ID), (HOUSE_ID, RENT_ID)]
            
      for ptype, stype in prop_list:
         announcers_id = getPapPropertiesId(ptype)
         search_id = getPapSearchTypeId(stype)        

         url = buildPapUrl(self.city, announcers_id, search_id)
         yield scrapy.Request(url=url, callback= lambda r, ptype = ptype, stype = stype :self.parse(r, ptype, stype))

   def parse(self, response, ptype, stype):
      links = response.xpath('//div/div[@id="pages-list"]/div/div/div[@class="search-list-item-alt"]/div[@class="item-body"]/a[@class="item-title"]/@href').extract()
      urls = []
      key_code = self.city + '-' + postalcodeByCity[self.city]
      for link in links:
         if '/annonces/' in link and key_code in link:
            urls.append(link)
 
      for part_url in urls:
         announce_url = 'https://pap.fr' + part_url

         ID = hash_id(announce_url)
         if str(ID) not in self.ads:
            yield scrapy.Request(url=announce_url, callback = lambda r, url = announce_url, ptype = ptype, stype = stype: self.parse_announce(r, url, ptype, stype))
         else:
            self.serializer.updateTimeStamp(ID)

   def parse_announce(self, response, url, ptype, stype):
      ID = hash_id(url)
      data = dict()
 
      #price
      price = response.xpath('//h1[@class="item-title"]/span[@class="item-price"]/text()').extract()
      pos = price[0].find(u'\xa0')
      real_price = price[0][:pos]
      px = real_price.replace('.', '')

      data['PRICE'] = px

      #announce details
      details  = response.xpath('//div[contains(@class, "item-description")]/ul[contains(@class, "item-tags")]/li/strong/text()').extract()
      nb_room = details[0]
      pos = nb_room.find(' ')
            
      data['ROOMS'] = nb_room[:pos]


      surface = details[2]
      pos = surface.find(u'\xa0')
      data['SURFACE'] = surface[:pos]

      bedrooms = response.xpath('//div[contains(@class, "item-description")]/ul[contains(@class, "item-tags")]/li/strong[contains(text(),"chambres" )]/text()').extract()
      if bedrooms:
         pos_bedrooms = bedrooms[0].find('c')
         data['BEDROOMS'] = bedrooms[0][:pos_bedrooms].replace(' ', '')
      landsurface = response.xpath('//div[contains(@class, "item-description")]/ul[contains(@class, "item-tags")]/li/strong[contains(text(),"Terrain" )]/text()').extract()
      if landsurface:
         land = landsurface[0].encode('ascii', 'ignore')
         pos_end = land[8:].find('m')
         data['LAND_SURFACE'] = land[8:][:pos_end]

      text_description = response.xpath('/html/body/div[2]/div/div[1]/div[6]/div/p[1]/text()').extract()

      if text_description:
         data['AD_TEXT_DESCRIPTION'] = text_description[0]
  
      # get images
      images_list = response.xpath('//div[contains(@class, "owl-thumbs")]/a/img/@src').extract() 
      images = []
      for image in images_list:
         if 'https://' in image:
            images.append(image)
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

      ret = self.serializer.send(ID, ptype, json_data, self.city, self.region, url, "pap", announce_title, stype, announce_image, img_cnt)
      print (ret)
      self.announces_cnt += 1
            
   def closed(self, reason):
      print("Announces found: " + str(self.announces_cnt) + "\n")
      self.serializer.close()

