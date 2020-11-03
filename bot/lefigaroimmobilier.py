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


config_lefigaroimmobilier = configparser.ConfigParser()
config_lefigaroimmobilier.read('spiders/config.ini')

ip = config_lefigaroimmobilier['COMMON']['db_access_ip']
port = int(config_lefigaroimmobilier['COMMON']['db_access_port'])
tablename = config_lefigaroimmobilier['COMMON']['tablename']


class LefigaroImmobilierSpider(scrapy.Spider):
   
   name = "lefigaroimmobilier"

   def __init__(self, city= '', **kwargs):
      self.city = city
      self.images_folder_name=config_lefigaroimmobilier[city.upper()]['images']
      self.region = config_lefigaroimmobilier[city.upper()]['region']

      if not os.path.exists(self.images_folder_name):
         os.mkdir(self.images_folder_name)

      
      self.announces_cnt = 0
      self.serializer = Serializer(ip, port, tablename)
      self.ads = self.serializer.scanidByCityAndAdSource(city, "lefigaroimmobilier")


   def start_requests(self):
      prop_list = [(APART_ID, BUY_ID), (HOUSE_ID, BUY_ID), (APART_ID, RENT_ID), (HOUSE_ID, RENT_ID)]
            
      for ptype, stype in prop_list:
         announcers_id = getLefigaroImmobilierPropertiesId(ptype)
         search_id = getLefigaroImmobilierSearchTypeId(stype)        

         url = buildLefigaroImmobilierUrl(self.city, announcers_id, search_id)
         yield scrapy.Request(url=url, callback= lambda r, ptype = ptype, stype = stype :self.parse(r, ptype, stype))

   def parse(self, response, ptype, stype):

      raw_data = json.loads(response.text)

      for ads in raw_data['itemClassifieds']:
        ads_info = dict()
        raw_price = ads['priceLabel']
        raw_price = raw_price.encode('ascii', 'ignore')
        raw_price = raw_price.replace('&nbsp;', '')
        price = raw_price.replace('&euro;','')
        ads_info['PRICE'] = price

        raw_rooms = ads['roomCountLabel']
        pos = raw_rooms.find(u'pièce')
        rooms = raw_rooms[:pos].replace(' ','')

        ads_info['ROOMS'] = rooms

        if 'areaLabel' in ads:
            ads_info['SURFACE'] = ads['areaLabel']
        else:
            print("No area found for :" + ads['recordLink'] + ", skip it!")
            continue

        ads_info['IMAGE_COUNT'] = ads['photoCount']

        groundarea = ads['groundArea']

        if groundarea != 0:
            ads_info['LAND_SURFACE'] = str(groundarea)

        announce_url = ads['recordLink']

        ID = hash_id(announce_url)

        if str(ID) not in self.ads:
          yield scrapy.Request(url=announce_url, callback = lambda r, url = announce_url, ptype = ptype, stype = stype, ads_info = ads_info: self.parse_announce(r, url, ptype, stype, ads_info))
        else:
          self.serializer.updateTimeStamp(ID)
 
   def parse_announce(self, response, url, ptype, stype, ads_info):
      bedrooms = response.xpath('//div[@id="js-container-main"]/div/ul[@class="list-features"]/li[contains(text(), "chambres")]/text()').extract()

      if bedrooms:
         val = bedrooms[0].split()

         ads_info['BEDROOMS'] = val[0]

      heating = response.xpath('//div[@id="js-container-main"]/div/ul[@class="list-features"]/li[contains(text(), "Type de chauffage")]/text()').extract()

      if heating:
         heating_list = heating[0].split(':')

         ads_info['TYPE_OF_HEATING'] = "Chauffage "
         val = heating_list[1].replace(' ', '')
         val = val.replace('\n', '')
         val = val.replace('\t', '')
         ads_info['TYPE_OF_HEATING'] += val

      floor = response.xpath(u"//div[@id='js-container-main']/div/ul[@class='list-features']/li[contains(text(), 'étage')]/text()").extract()
      if floor:
         val = floor[0].split()
         ads_info['FLOOR'] = val[0]

      lift = response.xpath(u"//div[@id='js-container-main']/div/ul[@class='list-features']/li[contains(text(), 'Ascenseur')]/text()").extract()

      if lift:
         val = lift[0].split()
         ads_info['LIFT'] = val[0]

      cellar = response.xpath(u"//div[@id='js-container-main']/div/ul[@class='list-features']/li[contains(text(), 'Cave')]/text()").extract()

      if cellar:
         ads_info['CELLAR'] = '1'

      parking = response.xpath(u"//div[@id='js-container-main']/div/ul[@class='list-features']/li[contains(text(), 'Parking')]/text()").extract()

      if cellar:
         ads_info['PARKING'] = '1'


      ad_description = response.xpath('//*[@id="js-clicphone-description"]/text()').extract()
      if ad_description:
         ads_info['AD_TEXT_DESCRIPTION'] = ad_description[0]

      #get images
      images = response.xpath('//div[@id="js-container-main"]/div/div[contains(@class,"owl-carousel")]/div/a/img/@src').extract()

      ID = hash_id(url)

      image_count = 1
      for img in images:
         image_name = os.path.join(self.images_folder_name, str(ID) + '_' + str(image_count) + '.jpg')
         urllib.urlretrieve(img, image_name)
         image_count = image_count + 1

      # prepare data and send it!
      img_cnt = len(images)

      json_data = json.dumps(ads_info)
      announce_image = ""
      if images:
         announce_image = images[0]

      text_title = response.xpath('//title/text()').extract()
      announce_title = ""
      if text_title:
         announce_title = text_title[0]

      
      ret = self.serializer.send(ID, ptype, json_data, self.city, self.region, url, "lefigaroimmobilier", announce_title, stype, announce_image, img_cnt)
      print (ret)
      self.announces_cnt += 1
            
   def closed(self, reason):
      print("Announces found: " + str(self.announces_cnt) + "\n")
      self.serializer.close()

