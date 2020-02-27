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


config_arthurimmo = configparser.ConfigParser()
config_arthurimmo.read('spiders/config.ini')

ip = config_arthurimmo['COMMON']['db_access_ip']
port = int(config_arthurimmo['COMMON']['db_access_port'])
tablename = config_arthurimmo['COMMON']['tablename']


class ArthurImmoSpider(scrapy.Spider):
   
   name = "arthurimmo"

   def __init__(self, city='', **kwargs):
      self.city = city
      self.images_folder_name=config_arthurimmo[city.upper()]['images']
      self.region = config_arthurimmo[city.upper()]['region']

      if not os.path.exists(self.images_folder_name):
         os.mkdir(self.images_folder_name)

      self.announces_cnt = 0
      self.serializer = Serializer(ip, port, tablename)
      self.ads = self.serializer.scanidByCityAndAdSource(city, "arthurimmo")

   def start_requests(self):
      prop_list = [(APART_ID, BUY_ID), (HOUSE_ID, BUY_ID), (APART_ID, RENT_ID), (HOUSE_ID, RENT_ID)]
            
      for ptype, stype in prop_list:
         announcers_id = getArthurImmoPropertiesId(ptype)
         search_id = getArthurImmoSearchTypeId(stype)        

         url = buildArthurImmoUrl(self.city, announcers_id, search_id)
         yield scrapy.Request(url=url, callback= lambda r, ptype = ptype, stype = stype :self.parse(r, ptype, stype))

   def parse(self, response, ptype, stype):
      all_links = response.xpath("//div/div/div/div[@id='recherche-resultats-listing']").xpath(".//div/div/div/a/@href").extract()

      links = set(all_links)

      for announce_url in links:
         if announce_url[0:5] != 'https':
            continue
         ID = hash_id(announce_url)
         if str(ID) not in self.ads:
            yield scrapy.Request(url=announce_url, callback = lambda r, url = announce_url, ptype = ptype, stype = stype: self.parse_announce(r, url, ptype, stype))
         else:
            self.serializer.updateTimeStamp(ID)

   def parse_announce(self, response, url, ptype, stype):
      ID = hash_id(url)
      data = dict()
      
      #get price
      raw_price = response.xpath("//header/div/div/div/div/div/div/span[@itemprop='price']/text()").extract()

      if raw_price:
         data['PRICE'] = raw_price[0]

      #announce details
      details = response.xpath("//div/div/div/div/div/ul[@class='detail-fiche-sign']")
      surface = details.xpath(u".//li[@title='Surface']").xpath(".//div[@class='fiche-sign-val']/text()").extract()
      if surface:
         data['SURFACE'] = surface[0]

      cellar = details.xpath(u".//li[@title='Cave']").xpath(".//div[@class='fiche-sign-val']/text()").extract()
      if cellar:
         data['CELLAR'] = cellar[0]

      rooms = details.xpath(u".//li[@title='Pièce']").xpath(".//div[@class='fiche-sign-val']/text()").extract()

      if not rooms:
         rooms = details.xpath(u".//li[@title='Pièces']").xpath(".//div[@class='fiche-sign-val']/text()").extract()


      if rooms:
         data['ROOMS'] = rooms[0]

      floor = details.xpath(u".//li[@title='Etage']").xpath(".//div[@class='fiche-sign-val']/text()").extract()

      if floor:
         data['FLOOR'] = floor[0]

      construction_year = details.xpath(u".//li[@title='Année de construction']").xpath(".//div[@class='fiche-sign-val']/text()").extract()

      if construction_year:
         data['CONSTRUCTION_YEAR'] = construction_year[0]

      parking = details.xpath(u".//li[@title='Parking']").xpath(".//div[@class='fiche-sign-val']/text()").extract()

      if parking:
         data['PARKING'] = parking[0]

      balcony = details.xpath(u".//li[@title='Balcon']").xpath(".//div[@class='fiche-sign-val']/text()").extract()

      if balcony:
         data['BALCONY'] = balcony[0]

      type_of_heating = details.xpath(u".//li[@title='Chauffage']").xpath(".//div[@class='fiche-sign-val']/text()").extract()

      if type_of_heating:
         data['TYPE_OF_HEATING'] = type_of_heating[0]


      # get images
      images = response.xpath('//div/div/div[@class="row-fluid"]/div/div/div/div[@class="item"]/img/@src').extract()
 
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

      ret = self.serializer.send(ID, ptype, json_data, self.city, self.region, url, "arthurimmo", announce_title, stype, announce_image, img_cnt)
      print (ret)
      self.announces_cnt += 1
            
   def closed(self, reason):
      print("Announces found: " + str(self.announces_cnt) + "\n")
      self.serializer.close()

