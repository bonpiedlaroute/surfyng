#!/usr/bin/env python
# -*- coding: utf-8 -*- 
# (c) Copyright 2017-2021 
# author(s): Stephane Fedim
# All rights reserved

from hash_id import hash_id
from url_builder import buildFnaimUrl
from search_features import getFnaimPropertiesId, getFnaimSearchPropertiesId, APART_ID, HOUSE_ID, BUY_ID, RENT_ID
from unidecode import unidecode
from Serializer import Serializer

import os
import json
import scrapy
import urllib
import configparser

config_fnaim = configparser.ConfigParser()
config_fnaim.read('spiders/config.ini')

ip = config_fnaim['COMMON']['db_access_ip']
port = int(config_fnaim['COMMON']['db_access_port'])
tablename = config_fnaim['COMMON']['tablename']
max_pages = int(config_fnaim['FNAIM']['max_pages'])

class FnaimSpider(scrapy.Spider):

    name = 'fnaim'
    
    def __init__(self, city='', **kwargs):
        self.city = city
        self.images_folder_name = config_fnaim[city.upper()]['images']
        self.region = config_fnaim[city.upper()]['region']

        if not os.path.exists(self.images_folder_name):
         os.mkdir(self.images_folder_name)

      
        self.announces_cnt = 0
        self.serializer = Serializer(ip, port, tablename)
        self.ads = self.serializer.scanidByCityAndAdSource(city, "fnaim")

    def start_requests(self):
        prop_list = [(APART_ID, BUY_ID), (HOUSE_ID, BUY_ID), (APART_ID, RENT_ID), (HOUSE_ID, RENT_ID)]

        for ptype, stype in prop_list:
            prop_id = getFnaimPropertiesId(ptype)
            search_id = getFnaimSearchPropertiesId(stype)

            url = buildFnaimUrl(self.city, prop_id, search_id)
            yield scrapy.Request(url=url, callback= lambda r, ptype = ptype, stype = stype :self.parse(r, ptype, stype, 1))
        
    def parse(self, response, ptype, stype, page):
        links = response.css("div#annonceListContainer ul.liste li.item div.itemInfo div.itemImage a::attr('href')").getall()

        for link in links:
            announce_url = 'https://www.fnaim.fr' + link
            ID = hash_id(announce_url)

            if str(ID) not in self.ads:
                yield scrapy.Request(url=announce_url, callback = lambda response, url = announce_url, ptype = ptype, stype = stype: self.parse_announce(response, url, ptype, stype))
            else:
                self.serializer.updateTimeStamp(ID)
            
        
        if page == max_pages:
            return
        else:
            page += 1
            next_links = response.css("div#p.blocNavigation.regletteBis div.regletteNavigation a::attr('href')").getall()
            if next_links:
                next_link = 'https://www.fnaim.fr' + next_links[0]
                yield response.follow(next_link, callback= lambda r, ptype = ptype, stype = stype :self.parse(r, ptype, stype, page))

    def parse_announce(self, response, url, ptype, stype):
        ID = hash_id(url)
        data = dict()

        # Extract price
        price = response.css("h3.prix span::text").get()
        if price:
            price = price.replace(' ', '')
            data['PRICE'] = price
        else:
            print('Announce at {} has no price'.format(url))
            return
        
        # Extract surface
        surface = response.css("ul.infos li.picto.surface b::text").get()
        if surface:
            surface = surface.encode('ascii', 'ignore').replace('m', '')
            data['SURFACE'] = surface
        else:
            print('Announce at {} has no surface'.format(url))
            return
        
        # Extract rooms
        rooms = response.css("ul.infos li.picto.pieces b::text").get()
        if rooms:
            data['ROOMS'] = rooms
        else:
            print('Announce at {} has no rooms'.format(url))
            return

        # Extract parking
        parking = response.css("ul.infos li.picto.parking b::text").get()
        if "oui" in parking.lower():
            data['PARKING'] = '1'
        
        # Extracting data 
        features_home = response.css("div#logementBlock ul li label::text").getall()
        features_home = map(lambda item: unidecode(item.strip()), features_home)

        data_home = response.css("div#logementBlock ul li::text").getall()
        data_home = map(lambda item: unidecode(item.strip()), data_home)

        if len(features_home) == len(data_home):
            for label in features_home:
                if 'etage' == label.lower():
                    floor = data_home[features_home.index(label)]
                    data['FLOOR'] = floor
                if 'parking' in label.lower():
                    parking = ''
                    parking_tmp = data_home[features_home.index(label)]
                    if 'oui' in parking_tmp.lower():
                        parking = '1'
                        data['PARKING'] = parking
                if 'construction' in label.lower():
                    construction_year = data_home[features_home.index(label)]
                    data['CONSTRUCTION_YEAR'] = construction_year

        # Extracting another data
        features_label = response.css("div#compositionBlock ul li label::text").getall()
        features_label = map(lambda item: unidecode(item.strip()), features_label)

        features_data = response.css("div#compositionBlock ul li::text").getall()
        features_data = map(lambda item: unidecode(item.strip()), features_data)

        if len(features_label)  == len(features_data):
            for label in features_label:
                if 'chambre' in label.lower():
                    bedrooms = features_data[features_label.index(label)]
                    data['BEDROOMS'] = bedrooms
                if 'cave' in label.lower():
                    cave = features_data[features_label.index(label)]
                    if 'oui' in cave.lower():
                        data['CELLAR'] = '1'
                if 'chauffage' in label.lower():
                    heating = features_data[features_label.index(label)]
                    data['TYPE_OF_HEATING'] = heating
                if 'ascenseur' in label.lower():
                    lift = features_data[features_label.index(label)]
                    if 'oui' in lift.lower():
                        data['LIFT'] = '1'
                if 'balcon' in label.lower():
                    balcony = features_data[features_label.index(label)]
                    if 'oui' in balcony:
                        data['BALCONY'] = '1'

        ad_text_description = response.css("div.description p::text").getall()[-1]
        if ad_text_description:
            data['AD_TEXT_DESCRIPTION'] = ad_text_description
        
        # Extract images
        images = response.css("div.annonce_fiche.fiche div#diapo_annonce.diaporama  div a.imageAnnonce img::attr('src')").getall()
        image_count = 1
        img_cnt = 0
        for img in images:
            try:
                image_name = os.path.join(self.images_folder_name, str(ID) + '_' + str(image_count) + '.jpg')
                urllib.urlretrieve(img, image_name)
                image_count += 1
            except:
                import traceback
                traceback.print_stack()
                

        json_data = json.dumps(data)
        
        announce_image = ""
        if images:
            announce_image = images[0]
        
        announce_title = response.css("h1.titreFiche::text").get()
        ret = self.serializer.send(ID, ptype, json_data, self.city, self.region, url, "fnaim", announce_title, stype, announce_image, len(images))
        print(ret)
        self.announces_cnt += 1

    def closed(self, reason):
        print("Announces found: " + str(self.announces_cnt) + "\n")
        self.serializer.close()