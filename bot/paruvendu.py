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


config_paruvendu = configparser.ConfigParser()
config_paruvendu.read('spiders/config.ini')

ip = config_paruvendu['COMMON']['db_access_ip']
port = int(config_paruvendu['COMMON']['db_access_port'])
tablename = config_paruvendu['COMMON']['tablename']
max_pages = int(config_paruvendu['COMMON']['max_pages'])


class ParuVenduSpider(scrapy.Spider):
   
   name = "paruvendu"

   def __init__(self, city= '', **kwargs):
      self.city = city
      self.images_folder_name=config_paruvendu[city.upper()]['images']
      self.region = config_paruvendu[city.upper()]['region']

      if not os.path.exists(self.images_folder_name):
         os.mkdir(self.images_folder_name)

      
      self.announces_cnt = 0
      self.serializer = Serializer(ip, port, tablename)
      self.ads = self.serializer.scanidByCityAndAdSource(city, "paruvendu")

      
   def start_requests(self):
      prop_list = [(APART_ID, BUY_ID), (HOUSE_ID, BUY_ID), (APART_ID, RENT_ID), (HOUSE_ID, RENT_ID)]

            
      for ptype, stype in prop_list:
         prop_id = getParuVenduPropertiesId(ptype)
         search_id = getParuVenduSearchTypeId(stype)        

         url = buildParuVenduUrl(self.city, prop_id, search_id)
         yield scrapy.Request(url=url, callback= lambda r, ptype = ptype, stype = stype :self.parse(r, ptype, stype, 1))

   def parse(self, response, ptype, stype, page):

      links = response.xpath('//div[@id="bloc_liste"]/div[contains(@class, "ergov3-annonce")]/a[contains(@href, "immobilier")]/@href').extract()
      
      for link in links:
         announce_url = 'https://www.paruvendu.fr' + link

         ID = hash_id(announce_url)
         if str(ID) not in self.ads:
            yield scrapy.Request(url=announce_url, callback = lambda r, url = announce_url, ptype = ptype, stype = stype: self.parse_announce(r, url, ptype, stype))
         else:
            self.serializer.updateTimeStamp(ID)

      if page == max_pages:
         return
      else:
         page += 1
         n = 'p=' + str(page)
         next_links = response.xpath('//div[@id="bloc_liste"]/div[@class="v2-pagin"]/div/div/a[@class="page" and contains(@href, $nextpage)]/@href', nextpage=n).extract()
         if next_links:
            next_link = 'https://www.paruvendu.fr/immobilier/annonceimmofo/liste/' + next_links[0]
            yield response.follow(next_link, callback= lambda r, ptype = ptype, stype = stype :self.parse(r, ptype, stype, page))


   def parse_announce(self, response, url, ptype, stype):
      ID = hash_id(url)
      data = dict()
 
      #price
      price = response.xpath('//div[@class="navete12-globcontent"]/div[@class="navete12-content"]/div/div/div/div[@id="autoprix"]/text()').extract()
      if not price:
         price = response.xpath('//div[@class="navete12-globcontent"]/div[@class="navete12-content"]/div/div/div/div/div[@id="autoprix"]/text()').extract()
         if not price:
            return
      px = price[0].encode('ascii', 'ignore')
      px = px.replace('CC', '')
      px = px.replace('\n', '')
      px = px.replace('\r', '')
      px = px.replace(' ', '')

      data['PRICE'] = px
      #announce details
      rooms = response.xpath(u'//*[@id="maindetail"]/div[3]/div[12]/div/ul/li[text()="\r\nNombre de pièces"]/span/text()').extract()

      if rooms:
         data['ROOMS'] = rooms[0]
      else:
         return

      raw_area = response.xpath(u'//*[@id="maindetail"]/div[3]/div[12]/div/ul/li[text()="\r\nSurface"]/span/text()').extract()

      if raw_area:
         area = raw_area[0]

         area = area.replace('m', '')
         data['SURFACE'] = area

      else:
         return

      parking = response.xpath(u'//*[@id="maindetail"]/div[3]/div[12]/div/ul/li[text()="Parking / Garage"]/text()').extract()

      if parking:
         data['PARKING'] = '1'

      bedrooms = response.xpath(u'//*[@id="maindetail"]/div[3]/div[14]/div/ul/li/span[starts-with(text(),"- Nombre de chambre(s)")]/text()').extract()

      if bedrooms:
         data['BEDROOMS'] = bedrooms[0][25:]

      floor = response.xpath(u'//*[@id="maindetail"]/div[3]/div[14]/div/ul/li/span[starts-with(text(),"- Etage")]/text()').extract()

      if floor:
         data['FLOOR'] = floor[0][10:]

      land_surface = response.xpath(u'//*[@id="maindetail"]/div[3]/div[14]/div/ul/li/span[starts-with(text(),"- Surface du terrain")]/text()').extract()
      if land_surface:
         data['LAND_SURFACE'] = land_surface[0][23:]
      
      heating = response.xpath(u'//*[@id="maindetail"]/div[3]/div[14]/div/ul/li/span[starts-with(text(),"- Chauffage")]/text()').extract()
      
      if heating:
         data['TYPE_OF_HEATING'] = "Chauffage " + heating[0][14:]

      cellar = response.xpath(u'//*[@id="maindetail"]/div[3]/div[14]/div/ul/li/span[starts-with(text(),"- Cave")]/text()').extract()
      if cellar:
         data['CELLAR'] = '1'

      text = response.xpath('//*[@id="txtAnnonceTrunc"]/text()').extract()
      if text:
         desc = " ".join(text)
         data['AD_TEXT_DESCRIPTION'] = desc

      # get images
      images = response.xpath('//div[@class="navete12-globcontent"]/div[@class="navete12-content"]/div/div/div/div[@id="listePhotos"]/div/div/div/a/p/span/img/@src').extract()
 
      image_count = 1
      for img in images:
         image_name = os.path.join(self.images_folder_name, str(ID) + '_' + str(image_count) + '.jpg')
         pos = img.find('_sizecrop')
         img_url = img[:pos]
         urllib.urlretrieve(img_url, image_name)
         image_count = image_count + 1

      # prepare data and send it!
      img_cnt = len(images)

      json_data = json.dumps(data)
      
      announce_image = ""
      if images:
         pos = images[0].find('_sizecrop')
         announce_image = images[0][:pos]

      text_title = response.xpath('//title/text()').extract()
      announce_title = ""
      if text_title:
         announce_title = text_title[0]

      ret = self.serializer.send(ID, ptype, json_data, self.city, self.region, url, "paruvendu", announce_title, stype, announce_image, img_cnt)
      print (ret)
      self.announces_cnt += 1
            
   def closed(self, reason):
      print("Announces found: " + str(self.announces_cnt) + "\n")
      self.serializer.close()

