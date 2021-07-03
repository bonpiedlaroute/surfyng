#!/usr/bin/env python
# -*- coding: utf-8 -*-
# (c) Copyright 2017-2021
# author(s): Stephane Fedim
# All rights reserved


from hash_id import hash_id
from url_builder import buildIdalUrl
from search_features import getEfficityPropertiesId, APART_ID, HOUSE_ID, BUY_ID
from unidecode import unidecode
from Serializer import Serializer
from requests.auth import HTTPBasicAuth
from pprint import pprint

import os
import sys
import json
import urllib
import configparser
import re
import requests

config_idal = configparser.ConfigParser()
config_idal.read('config.ini')

ip = config_idal['COMMON']['db_access_ip']
port = int(config_idal['COMMON']['db_access_port'])
tablename = config_idal['COMMON']['tablename']

idal_id = config_idal['APIMO']['idal_id']
url = buildIdalUrl(idal_id)

user = config_idal['APIMO']['provider']
password = config_idal['APIMO']['token']

response = requests.get(url, auth=HTTPBasicAuth(user, password))
units_response = requests.get(
    'https://api.apimo.pro/catalogs/unit_area', auth=HTTPBasicAuth(user, password))
heating_response = requests.get(
    'https://api.apimo.pro/catalogs/property_heating_type', auth=HTTPBasicAuth(user, password))
heating_access_response = requests.get(
    'https://api.apimo.pro/catalogs/property_heating_access', auth=HTTPBasicAuth(user, password))
floor_response = requests.get(
    'https://api.apimo.pro/catalogs/property_floor', auth=HTTPBasicAuth(user, password))

data = response.json()


def main():
    city = sys.argv[1]
    region = config_idal[city.upper()]['region']

    announces_count = data['total_items']
    serializer = Serializer(ip, port, tablename)
    ads = serializer.scanidByCityAndAdSource(city, "idal")

    announce = dict()

    for ad in data['properties']:
        region = ad['region']['name']
        language = ad['comments']['language']
        prop_type = ad['type']
        search_type = ad['category']

        search_filter = region == 'Hauts-de-Seine' and language == 'fr' and prop_type in (
            1, 2) and search_type in (1, 2)

        # Region
        if search_filter:
			announce['ID'] = hash_id(str(ad['id']) + ad['pictures'][0]['url'])
			announce['ANNOUNCE_TITLE'] = ad['comments']['title']
			announce['ANNOUNCE_SOURCE'] = 'idal/apimo'
			announce['CITY'] = ad['city']['name']
			announce['AD_TEXT_DESCRIPTION'] = ad['comments']['comment']
			announce['PROPERTY_TYPE'] = ad['type']
			announce['REGION'] = region
			announce['SEARCH_TYPE'] = search_type
			announce['ANNOUNCE_IMAGE'] = ad['pictures'][0]['url']
			# Get images
			images = {}
			for picture in ad['pictures']:
				images['image{}'.format(picture['rank'])] = picture['url']
			announce['IMAGE'] = str(images)
			announce['PRICE'] = ad['price']['value']
			# For surface we must consult units_response to get the right unit of area
			announce['SURFACE'] = ad['area']['value']
			announce['ROOMS'] =  ad['rooms']
			announce['BEDROOMS'] = ad['bedrooms']
			
			if ad['construction']['construction_year']:
				announce['CONSTRUCTION_YEAR'] = ad['construction']['construction_year']
			if ad['heating']['type']:
				heating = heating_response[ad['heating']['type']-1]['name']
				announce['TYPE_OF_HEATING'] =  heating
				if ad['heating']['access']:
					access = heating_access_response[ad['heating']['access']-1]['name']
					heating += ' {}'.format(access)
			# Get cave
			cellars = 0
			for area in ad['areas']:
				if area['type'] == 6:
					cellars += 1
			if cellars:
				announce['CELLAR'] =  str(cellars)
			# Get floor
			if ad['floor']['value']:
				announce['FLOOR'] = ad['floor']['value']
			
			# Get parking
			parkings = 0
			for area in ad['areas']:
				if area['type'] == 5:
					parkings += 1
			if parkings:
				announce['PARKING'] = str(parkings)
			
			if prop_type == 2:
				announce['LAND_SURFACE'] = ad['area']['total']

			# Get balcony
			balcony = False
			for area in ad['areas']:
				if area['type'] == 43:
					balcony = True
					break
			if balcony:
				announce['BALCONY'] = 'Oui'
			else:
				announce['BALCONY'] = 'Non'
			
			if ad['address']:
				announce['ADDRESS'] = ad['address']
			if ad['longitude'] and ad['latitude']:
				announce['LOCATION'] = "{}, {}".format(ad['longigute'], ad['latitude'])
			pprint(announce)



if __name__ == '__main__':
    main()

# class EfficitySpider(scrapy.Spider):
# 	name = 'efficity'
# 	def __init__(self, city='', **kwargs):

# 		self.city = city
# 		self.images_folder_name = config_efficity[city.upper()]['images']
# 		self.region = config_efficity[city.upper()]['region']

# 		if not os.path.exists(self.images_folder_name):
# 			os.mkdir(self.images_folder_name)

# 		self.announces_count = 0
# 		self.serializer = Serializer(ip, port, tablename)
# 		self.ads = self.serializer.scanidByCityAndAdSource(city, "efficity")

# 	def start_requests(self):
# 		prop_list = [APART_ID, HOUSE_ID]

# 		for ptype in prop_list:
# 			announcers_id = getEfficityPropertiesId(ptype)

# 			url = buildEfficityUrl(self.city, announcers_id)
# 			yield scrapy.Request(url=url, callback=lambda response, ptype = ptype: self.parse(response, ptype, 1))

# 	def parse(self, response, ptype, nextpage):
# 		base_url = 'https://www.efficity.com'

# 		links = response.css('.programs-list-item-visual a::attr("href")').getall()

# 		for link in links:
# 			true_link = (base_url + link).encode('utf-8', 'ignore')
# 			ID = hash_id(true_link)

# 			if str(ID) not in self.ads:
# 				yield scrapy.Request(url=true_link, callback=lambda response, url=true_link, ptype=ptype: self.parse_announce(response, url, ptype))
# 			else:
# 				self.serializer.updateTimeStamp(ID)

# 		href = response.xpath('/html/body/div[2]/ul/li[3]/a/@href').get()
# 		if nextpage == 1:
# 			if href is not None:
# 				next_link = base_url + href
# 			else:
# 				return

# 			if next_link:
# 				yield scrapy.Request(url=next_link, callback=lambda response, ptype=ptype: self.parse(response, ptype, 2))
# 		else:
# 			return

# 	def parse_announce(self, response, url, ptype):
# 		def extract_css_query(query):
# 			return response.css(query).get(default='')
# 		def extract_css_equip(query):
# 			items = []
# 			it = None
# 			for item in response.css(query).getall():
# 				it = unidecode(' '.join(item.split()))
# 				if it:
# 					items.append(it)
# 			return items

# 		data = dict()
# 		price = extract_css_query('.program-details-price::text')

# 		if price:
# 			data['PRICE'] = price.encode('ascii', 'ignore').replace('*', '').strip()
# 		else:
# 			return

# 		area = extract_css_query('h1.program-details-title::text')
# 		if area:
# 			data['SURFACE'] = area.split('-')[1].encode('ascii', 'ignore').replace('m', '').strip()
# 		else:
# 			return

# 		rooms = extract_css_query('h1.program-details-title::text')
# 		if rooms:
# 			data['ROOMS'] = rooms.split()[1].encode('ascii', 'ignore')
# 		else:
# 			return

# 		description = response.xpath('/html/body/section[1]/div[4]/div/div/div[1]/div[3]/div/p/text()').get()
# 		description = description.split('<br>')[0].strip()
# 		data['AD_TEXT_DESCRIPTION'] = description

# 		if 'balcon' in description.lower():
# 			data['BALCONY'] = '1'

# 		equipments = extract_css_equip('.program-details-features li::text')
# 		features = map(lambda item: unidecode(item), response.css('.program-details-sidebar ul li::text').getall())

# 		# First step data extraction
# 		for feature in equipments:
# 			if 'chambre' in feature.lower():
# 				data['BEDROOMS'] = feature.split()[0]
# 			if 'parking' in feature.lower():
# 				data['PARKING'] = '1'

# 		parsed_features = {}
# 		linear_features = []

# 		# parsing
# 		for feature in features:
# 			data_splited = feature.split(':')
# 			if len(data_splited) == 2:
# 				key = data_splited[0].strip()
# 				value = data_splited[1].strip()
# 				parsed_features[key] = value
# 			else:
# 				linear_features.append(feature)

# 		# Second step extraction
# 		for key in parsed_features.keys():
# 			heating = ""
# 			if 'origine du chauffage' in key.lower():
# 				heating = parsed_features[key]
# 				data['TYPE_OF_HEATING'] = heating

# 			if 'type du chauffage' in key.lower():
# 				heating += ' + {}'.format(parsed_features[key])
# 				data['TYPE_OF_HEATING'] = heating

# 		# Third step extraction
# 		for feature in linear_features:
# 			if 'ascenseur' in feature.lower():
# 				data['LIFT'] = '1'

# 			if  'construit en' in feature.lower():
# 				data['CONSTRUCTION_YEAR'] = re.findall(r'\d+', feature.lower())[-1]

# 		# Images
# 		images = map(lambda item: 'https:' + unidecode(item), response.css('img.d-none.d-sm-block.img-fluid::attr("src")').getall())

# 		ID = hash_id(url)

# 		images_count = 1
# 		for img in images:
# 			image_name = os.path.join(self.images_folder_name, str(ID) + '_' + str(images_count) + '.jpg')
# 			urllib.urlretrieve(img, image_name)
# 			images_count += 1

# 		images_count = len(images)

# 		json_data = json.dumps(data)
# 		announce_image = ''
# 		if images:
# 			announce_image = images[0]


# 		text_title = response.xpath('//title/text()').get()
# 		announce_title = text_title
# 		ret = self.serializer.send(ID, ptype, json_data, self.city, self.region, url, 'efficity', announce_title, BUY_ID, announce_image, images_count)
# 		print(ret)
# 		self.announces_count += 1

# 	def closed(self, reason):
# 		print('Announces found: {}\n'.format(self.announces_count))
