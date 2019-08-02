#!/usr/bin/env python
# -*- coding: utf-8 -*- 
# (c) Copyright 2019 
# author(s): Noel Tchidjo
# All rights reserved

import scrapy
import json
import urllib
import os

from hash_id import *
from search_features import *
from url_builder import *
from Serializer import *

search_base_url = "https://api.leboncoin.fr/finder/classified/"

IMAGES_FOLDER_NAME='images'

class LeboncoinSpider(scrapy.Spider):
   
   name = "leboncoin"

   def __init__(self): 
      if not os.path.exists(IMAGES_FOLDER_NAME):
         os.mkdir(IMAGES_FOLDER_NAME)

      self.mapping_url_ptype = dict()
      self.mapping_url_stype = dict()
      self.announces_cnt = 0
      self.serializer = Serializer('localhost', 5050)

   def start_requests(self):
      prop_list = [(APART_ID, BUY_ID), (HOUSE_ID, BUY_ID), (APART_ID, RENT_ID), (HOUSE_ID, RENT_ID)]
            
      for ptype, stype in prop_list:
         prop_id = getLeboncoinPropertiesId(ptype)
         search_id = getLeboncoinSearchTypeId(stype)        

         url = buildleboncoinurl(prop_id, search_id, 'Colombes_92700')
         self.mapping_url_ptype[url] = ptype
         self.mapping_url_stype[url] = stype
         yield scrapy.Request(url=url, callback= lambda r, nextpage=2: self.parse(r, nextpage))


   def parse(self, response, nextpage):
      original_request = str(response.request)

      search_type = self.mapping_url_stype[original_request[5:-1]]
      property_type = self.mapping_url_ptype[original_request[5:-1]]
      links = []
      
      # get all links
      if search_type == BUY_ID :
         links = response.xpath('//div[@class="_2r1q3"]').xpath('.//a[contains(@href, "/ventes_immobilieres")]/@href').extract()
      else:
         links = response.xpath('//div[@class="_2r1q3"]').xpath('.//a[contains(@href, "/locations")]/@href').extract()

      
      for link in links:
         ad_id = link[21:-5]
         search_url = search_base_url + ad_id
         ad_url = "https://www.leboncoin.fr" + link[:-1]

         yield scrapy.Request(search_url, callback = lambda r, ad_url = ad_url, search_type = search_type, property_type = property_type:self.parse_data(r, ad_url, search_type, property_type))
      
      n = "page="+str(nextpage)
      # parse next link
      next_link = response.xpath('//div[@class="_1evK6"]').xpath('.//a[@class="_1f-eo" and contains(@href,$nextp)]/@href', nextp=n).extract()

      if next_link:
         new_link = "https://www.leboncoin.fr" + next_link[0]
         self.mapping_url_ptype[new_link] = property_type
         self.mapping_url_stype[new_link] = search_type
         nextp = nextpage + 1
         yield response.follow(new_link, callback= lambda r, nextp = nextp : self.parse(r, nextp))

  
   def parse_data(self, response, ad_url, search_type, property_type):
      
      self.announces_cnt +=1
      ID = hash_id(ad_url)
      data = json.loads(response.text)
      
      announce_image = ""
      image_cnt = 1
      # store images on disk
      if 'images' in data and 'urls' in data['images']:
         images = data['images']['urls']
     
         if images: 
            announce_image = images[0]

         for img_url in images:
            filename =  str(ID) + '_' + str(image_cnt) + '.jpg'
            fullfilename = os.path.join(IMAGES_FOLDER_NAME,filename)
            urllib.urlretrieve(img_url, fullfilename)
            image_cnt += 1

      
      # send data to db
      self.serializer.send(ID, property_type, response.text, 'Paris', 'ile de france', ad_url, 'leboncoin', data['subject'], search_type, announce_image, image_cnt-1)

       
 
   def closed(self, reason):
      print "Announces found: %d\n" %self.announces_cnt
      self.serializer.close()




