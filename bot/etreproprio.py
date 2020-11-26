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


config_etreproprio = configparser.ConfigParser()
config_etreproprio.read('spiders/config.ini')

ip = config_etreproprio['COMMON']['db_access_ip']
port = int(config_etreproprio['COMMON']['db_access_port'])
tablename = config_etreproprio['COMMON']['tablename']


class LefigaroImmobilierSpider(scrapy.Spider):
   
   name = "etreproprio"

   def __init__(self, city= '', **kwargs):
      self.city = city
      self.images_folder_name=config_etreproprio[city.upper()]['images']
      self.region = config_etreproprio[city.upper()]['region']

      if not os.path.exists(self.images_folder_name):
         os.mkdir(self.images_folder_name)

      
      self.announces_cnt = 0
      self.serializer = Serializer(ip, port, tablename)
      self.ads = self.serializer.scanidByCityAndAdSource(city, "etreproprio")


   def start_requests(self):
      prop_list = [APART_ID, HOUSE_ID]
            
      for ptype in prop_list:
         announcers_id = getEtreProprioPropertiesId(ptype)        

         url = buildEtreProprioUrl(self.city, announcers_id)
         yield scrapy.Request(url=url, callback= lambda r, ptype = ptype:self.parse(r, ptype, 1))

   def parse(self, response, ptype, nextpage):

      links = response.xpath('/html/body/section[3]/div/div/a/@href').extract()


      for link in links:
        
        ID = hash_id(link)

        if str(ID) not in self.ads:
          yield scrapy.Request(url=link, callback = lambda r, url = link, ptype = ptype: self.parse_announce(r, url, ptype))
        else:
          self.serializer.updateTimeStamp(ID)
 

      if nextpage == 1:
         next_link = response.xpath('/html/body/section[4]/div/div/div[1]/ul/li[2]/a/@href').extract()

         if next_link:
            yield scrapy.Request(url=next_link[0], callback= lambda r, ptype = ptype:self.parse(r, ptype, 2))
      else:
         return
  
   def parse_announce(self, response, url, ptype):
      data = dict()
      price = response.xpath('/html/body/div/div[3]/div[2]/text()').extract()

      if price:
         px = price[0].encode('ascii','ignore')
         data['PRICE'] = px.replace('\n', '').replace(' ','')
      else:
         return

      area = response.xpath('/html/body/div/div[3]/div[3]/div[1]/text()').extract()
      if area:
         r = area[0].encode('ascii', 'ignore')
         data['SURFACE'] = r.replace('\n', '').replace('m', '')
      else:
         return

      rooms = response.xpath('/html/body/div/div[3]/div[3]/div[2]/text()').extract()
      if rooms:
         pos  = rooms[0].find(' ')
         data['ROOMS'] = rooms[0][:pos].replace('\n', '')
      else:
         return

      desc = response.xpath('/html/body/div/div[7]/text()').extract()
      text_desc = " ".join(desc)

      data['AD_TEXT_DESCRIPTION'] = text_desc
 

      #get images
      images = response.xpath('/html/body/div/div[6]/div/div/img/@src').extract()

      ID = hash_id(url)

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

      
      ret = self.serializer.send(ID, ptype, json_data, self.city, self.region, url, "etreproprio", announce_title, BUY_ID, announce_image, img_cnt)
      print (ret)
      self.announces_cnt += 1
            
   def closed(self, reason):
      print("Announces found: " + str(self.announces_cnt) + "\n")
      self.serializer.close()

