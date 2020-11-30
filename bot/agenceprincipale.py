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


config_agenceprincipale = configparser.ConfigParser()
config_agenceprincipale.read('spiders/config.ini')

ip = config_agenceprincipale['COMMON']['db_access_ip']
port = int(config_agenceprincipale['COMMON']['db_access_port'])
tablename = config_agenceprincipale['COMMON']['tablename']


class AgenceprincipaleSpider(scrapy.Spider):
   
   name = "agenceprincipale"

   def __init__(self, city= '', **kwargs):
      self.city = city
      self.images_folder_name=config_agenceprincipale[city.upper()]['images']
      self.region = config_agenceprincipale[city.upper()]['region']

      if not os.path.exists(self.images_folder_name):
         os.mkdir(self.images_folder_name)

      
      self.announces_cnt = 0
      self.serializer = Serializer(ip, port, tablename)
      self.ads = self.serializer.scanidByCityAndAdSource(city, "agenceprincipale")


   def start_requests(self):
      prop_list = [(APART_ID, BUY_ID), (HOUSE_ID, BUY_ID), (APART_ID, RENT_ID), (HOUSE_ID, RENT_ID)]
            
      for ptype, stype in prop_list:
         announcers_id = getAgencePrincipalePropertiesId(ptype)
         search_id = getAgencePrincipaleSearchTypeId(stype)        

         url = buildAgencePrincipaleUrl(self.city, announcers_id, search_id)
         yield scrapy.Request(url=url, callback= lambda r, ptype = ptype, stype = stype :self.parse(r, ptype, stype))

   def parse(self, response, ptype, stype):

      all_links = response.xpath('//div[@id="listing_bien"]/div/div/a/@href').extract()
      rent_links = response.xpath('//div[@id="listing_bien"]/div/div/a/div/div[@class="products-price" and contains(text(), "Loyer")]/parent::div/parent::a/@href').extract()
      
      links = []
      if stype == BUY_ID:
         links = list(set(all_links)^set(rent_links))
      else:
         links = rent_links

      for url in links:
         pos = url.find('?')
         part_url = url[:pos]
         announce_url = "https://www.agenceprincipale.com" + part_url[2:]
         ID = hash_id(announce_url)
         if str(ID) not in self.ads:
            yield scrapy.Request(url=announce_url, callback = lambda r, url = announce_url, ptype = ptype, stype = stype: self.parse_announce(r, url, ptype, stype))
         else:
            self.serializer.updateTimeStamp(ID)

   def parse_announce(self, response, url, ptype, stype):
      ID = hash_id(url)
      data = dict()

      price_value = ""

      if stype == BUY_ID: 
         #price
         price = response.xpath('//div[@id="infos_bien"]/div/div/div/h2/span/span/span[@class="hono_inclus_price"]/text()').extract()

         if price:
            price_value = price[0].encode('ascii', 'ignore')
         else:
            return
      else:
         loyer = response.xpath('//div[@id="infos_bien"]/div/div/div/h2/div[@class="prix loyer"]/span[@class="alur_loyer_price"]/text()').extract()
         if loyer:
            new_px = loyer[0].encode('ascii', 'ignore')
            begin = new_px.find(' ')
            end = new_px.find('/')
            price_value = new_px[begin:end]
         else:
            return

      data['PRICE'] = price_value

      #announce details
      rooms = response.xpath(u"//div[@id='infos_bien']/div[@class='container']/div/div/div/div[@id='container_caract']/div/div/ul[@class='list-group']/li/div/div[text()='Nombre pièces']/following-sibling::div/b/text()").extract()
      if rooms:
         data['ROOMS'] = rooms[0]
      else:
         return

      area = response.xpath(u"//div[@id='infos_bien']/div[@class='container']/div/div/div/div[@id='container_caract']/div/div/ul[@class='list-group']/li/div/div[text()='Surface']/following-sibling::div/b/text()").extract()

      if area:
         pos = area[0].find('m')
         data['SURFACE'] = area[0][:pos]
      else:
         return

      bedrooms = response.xpath(u"//div[@id='infos_bien']/div[@class='container']/div/div/div/div[@id='container_caract']/div/div/ul[@class='list-group']/li/div/div[text()='Chambres']/following-sibling::div/b/text()").extract()

      if bedrooms:
         data['BEDROOMS'] = bedrooms[0]

      type_heating = response.xpath(u"//div[@id='infos_bien']/div[@class='container']/div/div/div/div[@id='container_caract']/div/div/ul[@class='list-group']/li/div/div[text()='Type Chauffage']/following-sibling::div/b/text()").extract()
      if type_heating:
         
         heating = "Chauffage "
         heating += type_heating[0]
         mode_heating = response.xpath(u"//div[@id='infos_bien']/div[@class='container']/div/div/div/div[@id='container_caract']/div/div/ul[@class='list-group']/li/div/div[text()='Mode Chauffage']/following-sibling::div/b/text()").extract()
         heating += " " + mode_heating[0]

         data['TYPE_OF_HEATING'] = heating

      construction_year = response.xpath(u"//div[@id='infos_bien']/div[@class='container']/div/div/div/div[@id='container_caract']/div/div/ul[@class='list-group']/li/div/div[text()='Année construction']/following-sibling::div/b/text()").extract()

      if construction_year:
         data['CONSTRUCTION_YEAR'] = construction_year[0]

      lift = response.xpath(u"//div[@id='infos_bien']/div[@class='container']/div/div/div/div[@id='container_caract']/div/div/ul[@class='list-group']/li/div/div[text()='Ascenseur']/following-sibling::div/b/text()").extract()

      if lift:
         data['LIFT'] = '1'

      floor = response.xpath(u"//div[@id='infos_bien']/div[@class='container']/div/div/div/div[@id='container_caract']/div/div/ul[@class='list-group']/li/div/div[text()='Etage']/following-sibling::div/b/text()").extract()

      if floor:
         data['FLOOR'] = floor[0]

      land_area = response.xpath(u"//div[@id='infos_bien']/div[@class='container']/div/div/div/div[@id='container_caract']/div/div/ul[@class='list-group']/li/div/div[text()='Surface terrain']/following-sibling::div/b/text()").extract()

      if land_area:
         pos = land_area[0].find('m')
         data['LAND_SURFACE'] = land_area[0][:pos]

      parking = response.xpath(u"//div[@id='infos_bien']/div[@class='container']/div/div/div/div[@id='container_caract']/div/div/ul[@class='list-group']/li/div/div[text()='Nombre places parking']/following-sibling::div/b/text()").extract()

      if parking:
         data['PARKING'] = parking[0]

      desc = response.xpath('//*[@id="infos_bien"]/div/div/div[7]/div/text()').extract()
      if desc:
         data['AD_TEXT_DESCRIPTION'] = " ".join(desc)

      # get images
      images = response.xpath(u"//div[@id='infos_bien']/div[@class='container']/div/div/div/div[@id='slider_bien']/div[@class='slide']/a/img/@src").extract() 
      image_count = 1
      for img in images:
         image_name = os.path.join(self.images_folder_name, str(ID) + '_' + str(image_count) + '.jpg')
         img_url = "https://www.agenceprincipale.com/" + img[2:]
         urllib.urlretrieve(img_url, image_name)
         image_count = image_count + 1

      # prepare data and send it!
      img_cnt = len(images)

      json_data = json.dumps(data)
      announce_image = ""
      if images:
         announce_image = "https://www.agenceprincipale.com/" + images[0][2:]

      text_title = response.xpath('//title/text()').extract()
      announce_title = ""
      if text_title:
         announce_title = text_title[0]

      ret = self.serializer.send(ID, ptype, json_data, self.city, self.region, url, "agenceprincipale", announce_title, stype, announce_image, img_cnt)
      print (ret)
      self.announces_cnt += 1
            
   def closed(self, reason):
      print("Announces found: " + str(self.announces_cnt) + "\n")
      self.serializer.close()

