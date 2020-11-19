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


config_avendrealouer = configparser.ConfigParser()
config_avendrealouer.read('spiders/config.ini')

ip = config_avendrealouer['COMMON']['db_access_ip']
port = int(config_avendrealouer['COMMON']['db_access_port'])
tablename = config_avendrealouer['COMMON']['tablename']
max_pages = int(config_avendrealouer['COMMON']['max_pages'])

def remove_accent(string):
   string = string.replace(u'è',u'e')
   string = string.replace(u'é', u'e')
   string = string.replace(u'É', u'e')
   return string

class AvendreAlouerSpider(scrapy.Spider):
   
   name = "avendrealouer"

   def __init__(self, city= '', **kwargs):
      self.city = city
      self.images_folder_name=config_avendrealouer[city.upper()]['images']
      self.region = config_avendrealouer[city.upper()]['region']
      
      if not os.path.exists(self.images_folder_name):
         os.mkdir(self.images_folder_name)

      
      self.announces_cnt = 0
      self.serializer = Serializer(ip, port, tablename)
      self.ads = self.serializer.scanidByCityAndAdSource(city, "avendrealouer")

      
   def start_requests(self):
      prop_list = [(APART_ID, BUY_ID), (HOUSE_ID, BUY_ID), (APART_ID, RENT_ID), (HOUSE_ID, RENT_ID)]

            
      for ptype, stype in prop_list:
         prop_id = getAvendreAlouerPropertiesId(ptype)
         search_id = getAvendreAlouerSearchTypeId(stype)        

         url = buildAvendreAlouerUrl(self.city, prop_id, search_id)
         yield scrapy.Request(url=url, callback= lambda r, ptype = ptype, stype = stype :self.parse(r, ptype, stype, 1))

   def parse(self, response, ptype, stype, page):

      links = response.xpath('//*[@id="property-list-content-responsive"]/div[2]/main/div[2]/div[contains(@class, "listing-item")]/div/a/@href').extract() 
      for link in links:

         if "programme-neuf" not in link:
            announce_url = 'https://www.avendrealouer.fr' + link
            ID = hash_id(announce_url)

            if str(ID) not in self.ads:
               yield scrapy.Request(url=announce_url, callback = lambda r, url = announce_url, ptype = ptype, stype = stype: self.parse_announce(r, url, ptype, stype))
            else:
               self.serializer.updateTimeStamp(ID)

      if page == max_pages:
         return
      else:
         page += 1
         next_links = response.xpath('//*[@id="pager-next"]/a/@href').extract()
         if next_links:
            next_link = 'https://www.avendrealouer.fr' + next_links[0]
            yield response.follow(next_link, callback= lambda r, ptype = ptype, stype = stype :self.parse(r, ptype, stype, page))


   def parse_announce(self, response, url, ptype, stype):
      ID = hash_id(url)
      data = dict()
 
      #price
      price = response.xpath('//*[@id="debutBlocDetail"]/main/div[1]/div[2]/div[1]/text()').extract()
      if price:
         px = price[0].encode('ascii', 'ignore')
         data['PRICE'] = px
      else:
         print("Skipping announce [" + url + "] => no price found")
         return
      
      #announce details
      surface = response.xpath('//*[@id="debutBlocDetail"]/main/div[1]/div[3]/div[1]/div[2]/p/span/text()').extract()
      if surface:
         pos = surface[0].find(' ')
         data['SURFACE'] = surface[0][:pos]
      else:
         print("Skipping announce [" + url + "] => no surface found")
         return


      rooms = response.xpath('//*[@id="debutBlocDetail"]/main/div[1]/div[3]/div[2]/div[2]/p/span/text()').extract()
      if rooms:   
         data['ROOMS'] = rooms[0]
      else:
         print("Skipping announce [" + url + "] => no rooms found")
         return

      year = response.xpath('//*[@id="debutBlocDetail"]/main/div[1]/div[3]/div[*]/div[2]/p[text()="Construction"]/span/text()').extract()

      if year:
         data['CONSTRUCTION_YEAR'] = year[0]

      floor = response.xpath(u'//*[@id="debutBlocDetail"]/main/div[1]/div[3]/div[4]/div[2]/p[text()="Étage "]/span/text()').extract()

      if floor:
         data['FLOOR'] = floor[0]

      parking = response.xpath('//*[@id="debutBlocDetail"]/main/div[1]/div[5]/div/div[text()="Parking"]/text()').extract()

      if "Parking" in parking:
         data['PARKING'] = '1'
         
      lift = response.xpath('//*[@id="0Extensions" and text()="Ascenseur"]/text()').extract()

      if lift:
         data['LIFT'] = '1'

      heating = response.xpath('//*[@id="0Heatings"]/text()[3]').extract()
      if heating:
         data['TYPE_OF_HEATING'] = "Chauffage " + heating[0][3:]

      cellar = response.xpath('//*[@id="debutBlocDetail"]/main/div[1]/div[5]/div/div[text()="Cave"]/text()').extract()
      if "Cave" in cellar:
         data['CELLAR'] = '1'

      bedrooms = response.xpath('//*[@id="debutBlocDetail"]/main/div[1]/div[3]/div[3]/div[2]/p[text()="Chambre"]/span/text()').extract()
      if bedrooms:
         data['BEDROOMS'] = bedrooms[0]
 
      land_surface = response.xpath('//*[@id="debutBlocDetail"]/main/div[1]/div[3]/div[4]/div[2]/p[text()="Terrain"]/span/text()').extract()

      if land_surface:
         pos = land_surface[0].find('m')
         land = land_surface[0][:pos]
         land.replace(' ', '')
         data['LAND_SURFACE'] = land

      ad_text_description = response.xpath('//*[@id="blocProfessional"]/p/text()').extract()
      if ad_text_description:
         data['AD_TEXT_DESCRIPTION'] = ad_text_description[0]

      # get images
      images = response.xpath('//div[contains(@id, "react_")]/div[2]/div[2]/div[1]/div[2]/div/div[*]/div/div/img/@src').extract()
      image_count = 1
      img_cnt = 0
      for img in images:
         image_name = os.path.join(self.images_folder_name, str(ID) + '_' + str(image_count) + '.jpg')
         urllib.urlretrieve(img, image_name)
         image_count = image_count + 1
         img_cnt += 1

      # prepare data and send it!

      json_data = json.dumps(data)
      
      announce_image = ""
      if images:
         announce_image = images[0]

      text_title = response.xpath('//title/text()').extract()
      announce_title = ""
      if text_title:
         announce_title = text_title[0]

      ret = self.serializer.send(ID, ptype, json_data, self.city, self.region, url, "avendrealouer", announce_title, stype, announce_image, img_cnt)
      print (ret)
      self.announces_cnt += 1
            
   def closed(self, reason):
      print("Announces found: " + str(self.announces_cnt) + "\n")
      self.serializer.close()

