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


config_foncia = configparser.ConfigParser()
config_foncia.read('spiders/config.ini')

ip = config_foncia['COMMON']['db_access_ip']
port = int(config_foncia['COMMON']['db_access_port'])
tablename = config_foncia['COMMON']['tablename']


def remove_accent(string):
   string = string.replace(u'è',u'e')
   string = string.replace(u'é', u'e')
   string = string.replace(u'É', u'e')

   return string

class FonciaSpider(scrapy.Spider):
   
   name = "foncia"

   def __init__(self, city='', **kwargs):
      self.city = city
      self.images_folder_name=config_foncia[city.upper()]['images']
      self.region = config_foncia[city.upper()]['region']

      if not os.path.exists(self.images_folder_name):
         os.mkdir(self.images_folder_name)

      self.announces_cnt = 0
      self.serializer = Serializer(ip, port, tablename)
      self.ads = self.serializer.scanidByCityAndAdSource(city, "foncia")

      self.fieldmapping = dict()
      self.fieldmapping['Cave(s)'] = 'CELLAR'
      self.fieldmapping['Nombre de pieces'] = 'ROOMS'
      self.fieldmapping['Construction'] = 'CONSTRUCTION_YEAR'
      self.fieldmapping['etage'] = 'FLOOR'
      self.fieldmapping['Surface Carrez'] = 'SURFACE'
      self.fieldmapping['Parking interieur'] = 'PARKING'
      self.fieldmapping['Balcon'] = 'BALCONY'
      self.fieldmapping['Ascenseur'] = 'LIFT'
      self.fieldmapping['Nature chauffage'] = 'TYPE_OF_HEATING'
      self.fieldmapping['Annee de construction'] = 'CONSTRUCTION_YEAR'
      self.fieldmapping['Chauffage'] = 'TYPE_OF_HEATING'
      self.fieldmapping['Surface habitable'] = 'SURFACE'
      self.fieldmapping['Parking exterieur'] = 'PARKING'
      self.fieldmapping['Parking prive'] = 'PARKING'
      self.fieldmapping['Nombre de chambres'] = 'BEDROOMS'
      self.fieldmapping['Surface totale terrain'] = 'LAND_SURFACE'

   def start_requests(self):
      prop_list = [(APART_ID, BUY_ID), (HOUSE_ID, BUY_ID), (APART_ID, RENT_ID), (HOUSE_ID, RENT_ID)]
            
      for ptype, stype in prop_list:
         announcers_id = getFonciaPropertiesId(ptype)
         search_id = getFonciaSearchTypeId(stype)        

         url = buildFonciaUrl(self.city, announcers_id, search_id)
         yield scrapy.Request(url=url, callback= lambda r, ptype = ptype, stype = stype :self.parse(r, ptype, stype))

   def parse(self, response, ptype, stype):
      links = response.xpath('//h3[@class="TeaserOffer-title"]').xpath('.//a/@href').extract()

      for url in links:
         announce_url = 'https://fr.foncia.com' + url
         ID = hash_id(announce_url)
         if str(ID) not in self.ads:
            yield scrapy.Request(url=announce_url, callback = lambda r, url = announce_url, ptype = ptype, stype = stype: self.parse_announce(r, url, ptype, stype))
         else:
            self.serializer.updateTimeStamp(ID)

   def parse_announce(self, response, url, ptype, stype):
      ID = hash_id(url)
      data = dict()
      
      #get price
      raw_price = response.xpath('//div[@class="OfferTop-cols"]').xpath('.//p[@class="OfferTop-price"]/text()').extract()
      px = raw_price[0]
      px = px.strip()
      pos = px.find(u'\xa0')
      price = px[:pos]

      data['PRICE'] = price

      #announce details
      div_details = response.xpath('//div[@class="OfferDetails"]').xpath('.//div[@class="OfferDetails-columnize"]')
      details = div_details.xpath('.//div[@data-widget="ToggleBlockMobile-content"]').xpath('.//li')

      for info in details:
         field = info.xpath('.//span/text()').extract()
         value = info.xpath('.//strong/text()').extract()

         if field and value:
            f = remove_accent(field[0])
            if f in self.fieldmapping:
               data[self.fieldmapping[f]] = value[0] 

      more_info = div_details.xpath('//div[@data-widget="ToggleBlockMobile-content"]').xpath('.//ul[@class="List List--bullet"]').xpath('.//li/text()').extract()

      for info in more_info:
         if info in self.fieldmapping:
            data[self.fieldmapping[info]] = '1'

      # get images
      images = response.xpath('//div[@class="OfferSlider"]//ul[@class="OfferSlider-main-slides"]//img/@src').extract()
      
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

      ret = self.serializer.send(ID, ptype, json_data, self.city, self.region, url, "foncia", announce_title, stype, announce_image, img_cnt)
      print (ret)
      self.announces_cnt += 1
            
   def closed(self, reason):
      print("Announces found: " + str(self.announces_cnt) + "\n")
      self.serializer.close()

