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


config_century21 = configparser.ConfigParser()
config_century21.read('spiders/config.ini')

ip = config_century21['COMMON']['db_access_ip']
port = int(config_century21['COMMON']['db_access_port'])
tablename = config_century21['COMMON']['tablename']


def remove_accent(string):
   string = string.replace(u'è',u'e')
   string = string.replace(u'é', u'e')
   string = string.replace(u'É', u'e')

   return string

class Century21Spider(scrapy.Spider):
   
   name = "century21"

   def __init__(self, city ='', **kwargs):
      self.city = city
      self.images_folder_name=config_century21[city.upper()]['images']
      self.region = config_century21[city.upper()]['region']


      if not os.path.exists(self.images_folder_name):
         os.mkdir(self.images_folder_name)

      self.announces_cnt = 0
      self.serializer = Serializer(ip, port, tablename)
      self.ads = self.serializer.scanidByCityAndAdSource(city,"century21")

      self.fieldmapping = dict()
      self.fieldmapping['Annee construction'] = 'CONSTRUCTION_YEAR'
      self.fieldmapping['Surface totale'] = 'SURFACE'
      self.fieldmapping['Chauffage'] = 'TYPE_OF_HEATING'

   def start_requests(self):
      prop_list = [(APART_ID, BUY_ID), (HOUSE_ID, BUY_ID), (APART_ID, RENT_ID), (HOUSE_ID, RENT_ID)]
            
      for ptype, stype in prop_list:
         announcers_id = getCentury21PropertiesId(ptype)
         search_id = getCentury21SearchTypeId(stype)        

         url = buildCentury21Url(self.city, announcers_id, search_id)
         yield scrapy.Request(url=url, callback= lambda r, ptype = ptype, stype = stype :self.parse(r, ptype, stype))

   def parse(self, response, ptype, stype):
      all_links = response.xpath('//li[@class="annonce" or @class="annonce margR0"]').xpath('.//a/@href').extract()
      links = set(all_links)

      for url in links:
         announce_url = 'https://www.century21.fr' + url
         ID = hash_id(announce_url)
         if str(ID) not in self.ads:
            yield scrapy.Request(url=announce_url, callback = lambda r, url = announce_url, ptype = ptype, stype = stype: self.parse_announce(r, url, ptype, stype))
         else:
            self.serializer.updateTimeStamp(ID)

   def parse_announce(self, response, url, ptype, stype):
      ID = hash_id(url)
      data = dict()
      
      #get price
      raw_price = response.xpath('//section[@class="tarif"]/span/b/text()').extract()[0]
      pos = raw_price.rfind(' ')
      price = raw_price[:pos]

      data['PRICE'] = price

      #announce details
      #surface
      surface = response.xpath('//section[@id="ficheDetail"]/div/div/div[@class="box"]/ul/li/span[starts-with(text(),"Surface totale")]/parent::li/text()').extract()
      if surface:
         begin_surface = surface[0].find(':')
         raw_surface = surface[0][begin_surface+2:]
         end = raw_surface.find(' ')
         data['SURFACE'] = raw_surface[:end].replace(',','.')

      #nbroom
      nb_room = response.xpath(u"//section[@id='ficheDetail']/div/div/div[@class='box']/ul/li/span[text()='Nombre de pièces : ']/parent::li/text()").extract()
      if nb_room:
         room = nb_room[1].replace(' ', '')
         data['ROOMS'] = room.replace('\n', '')      
      
      #construction year
      year = response.xpath(u"//section[@id='ficheDetail']/div/div/div[@class='box']/ul/li/span[text()='Année construction']/parent::li/text()").extract()
      if year:
         data['CONSTRUCTION_YEAR'] = year[0][3:]

      #heating
      heating = response.xpath(u"//section[@id='ficheDetail']/div/div/div[@class='box']/ul/li/span[text()='Chauffage']/parent::li/text()").extract()
      if heating:
         data['TYPE_OF_HEATING'] = heating[0][3:]

      #land surface
      land = response.xpath(u"//section[@id='ficheDetail']/div/div/div[@class='box']/ul/li/span[text()='Surface terrain']/parent::li/text()").extract()
      if land:
         begin = land[0].find(':')
         end = land[0][begin+2:].find('m')
         landsurface=land[0][begin+2:]
         landsurface[:end]
         data['LAND_SURFACE'] = landsurface[:end].replace(' ', '')

      #parking
      parking = response.xpath(u"//section[@id='ficheDetail']/div/div/div[@class='box']/ul/li[@class='picto parking']").extract()

      if parking:
         data['PARKING'] = "1"
               
      # get images
      images = response.xpath('//div[@id="galeriePIX"]/div[@id="formatL"]/div[contains(@class, "zone-galerie")]').xpath('.//img/@src').extract() 
      image_count = 1
      for img in images:
         if img[1:11] == 'imagesBien': 
            image_name = os.path.join(self.images_folder_name, str(ID) + '_' + str(image_count) + '.jpg')
            img_url = 'https://www.century21.fr' + img
            urllib.urlretrieve(img_url, image_name)
            image_count = image_count + 1

      # prepare data and send it!
      img_cnt = image_count-1

      json_data = json.dumps(data)
      
      announce_image = ""
      if images:
         announce_image = 'https://www.century21.fr' + images[0]

      text_title = response.xpath('//h1[@class="h1_page"]/text()').extract()
      announce_title = text_title[0]

      ret = self.serializer.send(ID, ptype, json_data, self.city, self.region, url, "century21", announce_title, stype, announce_image, img_cnt)
      print (ret)
      self.announces_cnt += 1
            
   def closed(self, reason):
      print("Announces found: " + str(self.announces_cnt) + "\n")
      self.serializer.close()

