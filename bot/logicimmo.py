#!/usr/bin/env python
# -*- coding: utf-8 -*-
# (c) Copyright 2017-2018 
# author(s): Noel Tchidjo
# All rights reserved

import scrapy
import json
import os
import urllib
import configparser


from hash_id import *
from search_features import *
from url_builder import *
from Serializer import *



config_logicimmo = configparser.ConfigParser()
config_logicimmo.read('spiders/config.ini')

ip = config_logicimmo['COMMON']['db_access_ip']
port = int(config_logicimmo['COMMON']['db_access_port'])
tablename = config_logicimmo['COMMON']['tablename']
max_pages = int(config_logicimmo['COMMON']['max_pages'])




class LogicImmoSpider(scrapy.Spider):
      
   name = "logicimmo"
   download_delay = 30

   def __init__(self, city='', **kwargs):
      self.city = city
      self.images_folder_name = config_logicimmo[city.upper()]['images']
      self.region = config_logicimmo[city.upper()]['region']

 
      if not os.path.exists(self.images_folder_name):
         os.mkdir(self.images_folder_name)
 
      self.prop_record = []
      self.announce_cnt = 0
      self.nb_pages = 0
      self.serializer = Serializer(ip, port, tablename)
      self.ads = self.serializer.scanidByCityAndAdSource(city,"logicimmo")
     

   def start_requests(self):
      prop_list = [(APART_ID, BUY_ID), (HOUSE_ID, BUY_ID), (APART_ID, RENT_ID), (HOUSE_ID, RENT_ID)]

            
      for ptype, stype in prop_list:
         prop_type = getLogicImmoPropertiesId(ptype)
         search_type = getLogicImmoSearchTypeId(stype)

         url = buildLogicImmoUrl(self.city, prop_type, search_type)
         yield scrapy.Request(url=url, callback= lambda response, id_prop = ptype, search_type = stype, nextpage=2: self.parse(response,id_prop, search_type, nextpage))

   def parse(self, response, id_property, search_type, nextpage):

      if search_type == BUY_ID:
         links =  response.xpath('//div[contains(@class, "offer-list-item")]').xpath('.//a[contains(@href, "detail-vente") and contains(@href, "https")]/@href').extract()
      else:
         links = response.xpath('//div[contains(@class, "offer-list-item")]').xpath('.//a[contains(@href, "detail-location") and contains(@href, "https")]/@href').extract()

      for url in links:
         ID = hash_id(url)
         if str(ID) not in self.ads:
            yield scrapy.Request(url, callback= lambda r, id_prop=id_property, announce_url = url, search_type = search_type: self.parse_announce(r, id_prop, announce_url, search_type))

      # if we reach max_pages, stop crawling
      if self.nb_pages == max_pages:
         return

      n ='page='+str(nextpage)

      following_link = response.xpath('//section[@class="offer-pagination-wrapper"]').xpath('.//a[contains(@href,$nextp)]/@href', nextp=n).extract()

      nextindex = nextpage + 1

      if following_link:
         self.nb_pages += 1
         yield scrapy.Request(following_link[0], callback= lambda response, id_prop=id_property, search_type = search_type, nextindex = nextindex : self.parse(response, id_prop, search_type, nextindex))
      else:
         following_link = response.xpath('//link[contains(@href,$nextp)]/@href', nextp=n).extract()
         if following_link:
            self.nb_pages += 1
            yield scrapy.Request(following_link[0], callback= lambda response, id_prop=id_property, search_type = search_type, nextindex = nextindex : self.parse(response, id_prop, search_type, nextindex))

   def parse_announce(self, response, id_prop, announce_url, search_type):
      announce_title = response.xpath('//title/text()').extract()
      announce_description = response.xpath('//div[@class="offer-description-text"]').xpath('.//meta/@content').extract()
      desc_data = {}

      details = response.xpath('//section[@class="mainPropertySection"]').xpath('.//div/div/div/div/div/div/div/div[@class="leftZone clearfix"]')
      area = details.xpath('.//div[@class="cell area"]/span/text()').extract()
      desc_data['SURFACE'] = area[0]

      nb_room = details.xpath('.//div[@class="cell rooms"]/span/span[@class="offer-rooms-number"]/text()').extract()
      desc_data['ROOMS'] = nb_room[0]

      price =details.xpath('.//div[@class="cell price"]/h2[@class="main-price"]/text()').extract()
      desc_data['PRICE'] = price[0]

         
      json_desc = json.dumps(desc_data)

      imgs = response.xpath('//div[@class="carousel-wrapper"]').xpath('.//li/a/img/@src').extract()
      announce_image = ""
      img_ct = 0

      if imgs:
         announce_image = imgs[0]
         img_ct = len(imgs)

      image_count = 1
      ID = hash_id(announce_url)
      for image in imgs:
         image_name = os.path.join(self.images_folder_name, str(ID) + '_' + str(image_count) + '.jpg')
         urllib.urlretrieve(image, image_name)
         image_count = image_count + 1

      self.serializer.send(ID, id_prop, json_desc, self.city, self.region,announce_url,"logicimmo", announce_title[0], search_type, announce_image, img_ct)

      self.announce_cnt+=1

   def closed(self, reason):
      
      print "Total announces: %d" %(self.announce_cnt)      

      self.serializer.close()

