#!/usr/bin/env python
# -*- coding: utf-8 -*- 
# (c) Copyright 2017-2021 
# author(s): Stephane Fedim
# All rights reserved



from hash_id import hash_id
from url_builder import *
from search_features import *
from unidecode import unidecode
from Serializer import Serializer
from scrapy.crawler import CrawlerProcess


import os
import json
import scrapy
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
			yield scrapy.Request(url=url, callback=lambda response, ptype = ptype: self.parse(r, ptype, 1))

	def parse(self, response, ptype, nextpage):
		links = response.css('.programs-list-item-visual a')

		for link in links:

			ID = hash_id(link)

			if str(ID) not in self.ads:
				yield scrapy.Request(url=link, callback=lambda response, url=link, ptype=ptype: self.parse_announce(response, url, ptype))
			else:
				self.serializer.updateTimeStamp(ID)

		if nextpage == 1:
			next_link = response.css('div.pagination a.next')

			if next_link:
				yield scrapy.Request(url=next_link, callback=lambda response, ptype=ptype: self.parse(response, ptype, 2))
		else:
			return

	def parse_announce(self, response, url, ptype):
		def extract_css_query(query):
			return response.css(query).get(default='')
		def extract_css_all(query):
			items = []
			for item in response.css(query).getall():
				items.append(unidecode(' '.join(item.split())))
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
		description = response.xpath('/html/body/section[1]/div[4]/div/div/div[1]/div[3]/div/p/text()').get()
		desc['description'] = description.split('<br>')[0].strip()
		desc['features'] = extract_css_all('.program-details-features li::text') 
		
		data['AD_TEXT_DESCRIPTION'] = desc

		# yield {
		# 	'link': response.request,
		# 	'name': extract_css_query('h1.program-details-title::text'),
		# 	'area': 
		# 	'price': ,
		# 	'features':  ,
		# 	'description': response.xpath('//*[@id="default-presentation"]').get().split('<br>')[0].strip()
		# }

		# Images
		json_data = json.dumps(data)
		text_title = unidecode(response.xpath('//title/text()').get())
		announce_title = ""
		if text_title:
			announce_title = text_title
		
		ret = self.serializer.send(ID, ptype, json_data, self.city, self.region, url, 'efficity', announce_title, BUY_ID)
		
	

# if __name__ == '__main__':
# 	scraper = CrawlerProcess({
#     'USER_AGENT': 'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/34.0.1847.131 Safari/537.36',
# 	'FEED_FORMAT': 'json',
# 	'FEED_URI': 'annonces.json'
# 	})
# 	scraper.crawl(EfficitySpider)
# 	scraper.start()