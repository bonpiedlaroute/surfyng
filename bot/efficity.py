#!/usr/bin/env python
# -*- coding: utf-8 -*- 
# (c) Copyright 2017-2021 
# author(s): Stephane Fedim
# All rights reserved



from hash_id import hash_id
from url_builder import buildEfficityUrl
from search_features import getEfficityPropertiesId, APART_ID, HOUSE_ID, BUY_ID
from unidecode import unidecode
from Serializer import Serializer


import os
import json
import scrapy
import urllib
import configparser

config_efficity = configparser.ConfigParser()
config_efficity.read('spiders/config.ini')

ip = config_efficity['COMMON']['db_access_ip']
port = int(config_efficity['COMMON']['db_access_port'])
tablename = config_efficity['COMMON']['tablename']	# To change

class EfficitySpider(scrapy.Spider):
	name = 'efficity'
	def __init__(self, city='', **kwargs):

		self.city = city
		self.images_folder_name = config_efficity[city.upper()]['images']
		self.region = config_efficity[city.upper()]['region']

		if not os.path.exists(self.images_folder_name):
			os.mkdir(self.images_folder_name)
		
		self.announces_count = 0
		self.serializer = Serializer(ip, port, tablename)
		self.ads = self.serializer.scanidByCityAndAdSource(city, "efficity")

	def start_requests(self):
		prop_list = [APART_ID, HOUSE_ID]
		
		for ptype in prop_list:
			announcers_id = getEfficityPropertiesId(ptype)

			url = buildEfficityUrl(self.city, announcers_id)
			yield scrapy.Request(url=url, callback=lambda response, ptype = ptype: self.parse(response, ptype, 1))

	def parse(self, response, ptype, nextpage):
		base_url = 'https://www.efficity.com'
		
		links = response.css('.programs-list-item-visual a::attr("href")').getall()

		for link in links:
			true_link = (base_url + link).encode('utf-8', 'ignore')
			ID = hash_id(true_link)

			if str(ID) not in self.ads:
				yield scrapy.Request(url=true_link, callback=lambda response, url=true_link, ptype=ptype: self.parse_announce(response, url, ptype))
			else:
				self.serializer.updateTimeStamp(ID)
		
		href = response.xpath('/html/body/div[2]/ul/li[3]/a/@href').get()
		if nextpage == 1:
			if href is not None:
				next_link = base_url + href
			else:
				return
				
			if next_link:
				yield scrapy.Request(url=next_link, callback=lambda response, ptype=ptype: self.parse(response, ptype, 2))
		else:
			return

	def parse_announce(self, response, url, ptype):
		def extract_css_query(query):
			return response.css(query).get(default='')
		def extract_css_equip(query):
			items = []
			it = None
			for item in response.css(query).getall():
				it = unidecode(' '.join(item.split()))
				if it:
					items.append(it)
			return items

		data = dict()
		price = extract_css_query('.program-details-price::text')

		if price:
			data['PRICE'] = price.encode('ascii', 'ignore').replace('*', '').strip()
		else:
			return
		
		area = extract_css_query('h1.program-details-title::text')
		if area:
			data['SURFACE'] = area.split('-')[1].encode('ascii', 'ignore').replace('m', '').strip()
		else:
			return

		rooms = extract_css_query('h1.program-details-title::text')
		if rooms:
			data['ROOMS'] = rooms.split()[1].encode('ascii', 'ignore')
		else:
			return

		desc = dict()
		description = unidecode(response.xpath('/html/body/section[1]/div[4]/div/div/div[1]/div[3]/div/p/text()').get())
		desc['description'] = description.split('<br>')[0].strip()
		desc['equipments'] = extract_css_equip('.program-details-features li::text')

		features = map(lambda item: unidecode(item), response.css('.program-details-sidebar ul li::text').getall())
		desc['features'] = features
		data['AD_TEXT_DESCRIPTION'] = desc

		# Images
		images = map(lambda item: 'https:' + unidecode(item), response.css('img.d-none.d-sm-block.img-fluid::attr("src")').getall())

		ID = hash_id(url)

		images_count = 1
		for img in images:
			image_name = os.path.join(self.images_folder_name, str(ID) + '_' + str(images_count) + '.jpg')
			urllib.urlretrieve(img, image_name)
			images_count += 1

		images_count = len(images)

		json_data = json.dumps(data)
		announce_image = ''
		if images:
			announce_image = images[0]


		text_title = unidecode(response.xpath('//title/text()').get())
		announce_title = text_title
		ret = self.serializer.send(ID, ptype, json_data, self.city, self.region, url, 'efficity', announce_title, BUY_ID, announce_image, images_count)
		print(ret)
		self.announces_count += 1

	def closed(self, reason):
		print('Announces found: {}\n'.format(self.announces_count))