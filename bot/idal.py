#!/usr/bin/env python
# -*- coding: utf-8 -*-
# (c) Copyright 2017-2021
# author(s): Stephane Fedim
# All rights reserved


from hash_id import hash_id
from url_builder import buildIdalUrl
from search_features import getEfficityPropertiesId, APART_ID, HOUSE_ID, BUY_ID
from unidecode import unidecode
from Serializer import Serializer, Type, ValueType
from requests.auth import HTTPBasicAuth
from pprint import pprint


import re
import os
import sys
import pytz
import json
import urllib
import configparser
import requests
import datetime

config_idal = configparser.ConfigParser()
config_idal.read('config.ini')

ip = config_idal['COMMON']['db_access_ip']
port = int(config_idal['COMMON']['db_access_port'])
tablename = config_idal['APIMO']['tablename']

idal_id = config_idal['APIMO']['idal_id']
url = buildIdalUrl(idal_id)

user = config_idal['APIMO']['provider']
password = config_idal['APIMO']['token']

print('Fetch properties')
response = requests.get(url, auth=HTTPBasicAuth(user, password))
print('Fetch ended')

print('Fetch units catalog')
units_response = requests.get(
	'https://api.apimo.pro/catalogs/unit_area', auth=HTTPBasicAuth(user, password)).json()
print('Fetch ended')

print('Fetch heating type catalog')
heating_response = requests.get(
	'https://api.apimo.pro/catalogs/property_heating_type', auth=HTTPBasicAuth(user, password)).json()
print('Fetch ended')

print('Fetch heating access catalog')
heating_access_response = requests.get(
	'https://api.apimo.pro/catalogs/property_heating_access', auth=HTTPBasicAuth(user, password)).json()
print('Fetch ended')

print('Fetch floor catalog')
floor_response = requests.get(
	'https://api.apimo.pro/catalogs/property_floor', auth=HTTPBasicAuth(user, password)).json()
print('Fetch ended')

data = response.json()


def main():
	city = sys.argv[1]
	# region = config_idal[city.upper()]['region']

	announces_count = data['total_items']
	print('{} total items'.format(announces_count))
	serializer = Serializer(ip, port, tablename)
	ads = serializer.scanidByCityAndAdSource(city, "idal")

	announce = dict()

	for ad in data['properties']:
		if ad['region']:
			region = ad['region']['name']
		language = ad['comments'][0]['language']
		prop_type = ad['type']
		search_type = ad['category']
		# region == 'Hauts-de-Seine'
		search_filter = language == 'fr' and prop_type in (
			1, 2) and search_type in (1, 2) and city.upper() in ad['city']['name'].upper()

		# Region
		if search_filter:
			id_value = ValueType()
			id_value.field = str(int(hash_id(
				'https://www.idal-agenceimmobiliere.com/fr/' + str(ad['id']))))
			id_value.fieldtype = Type.NUMBER
			announce['ID'] = id_value

			announce_link_value =  ValueType()
			announce_link_value.field = 'https://www.idal-agenceimmobiliere.com/fr/'
			announce_link_value.fieldtype =  Type.STRING
			announce['ANNOUNCE_LINK'] = announce_link_value

			announce_title_value = ValueType()
			announce_title_value.field = ad['comments'][0]['title']
			announce_title_value.fieldtype = Type.STRING
			announce['ANNOUNCE_TITLE'] = announce_title_value

			announce_source_value = ValueType()
			announce_source_value.field = 'idal/apimo'
			announce_source_value.fieldtype = Type.STRING
			announce['ANNOUNCE_SOURCE'] = announce_source_value

			city_value = ValueType()
			city_value.field = ad['city']['name']
			city_value.fieldtype = Type.STRING
			announce['CITY'] = city_value

			description_value = ValueType()
			description_value.field = ad['comments'][0]['comment']
			description_value.fieldtype = Type.STRING
			announce['AD_TEXT_DESCRIPTION'] = description_value

			prop_type_value = ValueType()
			prop_type_value.fieldtype = Type.STRING
			if ad['type'] == APART_ID:
				prop_type_value.field = 'Appartement'
			else:
				prop_type_value.field = 'Maison'
			announce['PROPERTY_TYPE'] = prop_type_value

			region_value = ValueType()
			region_value.field = region
			region_value.fieldtype = Type.STRING
			announce['REGION'] = region_value

			search_type_value = ValueType()
			search_type_value.fieldtype = Type.STRING
			if search_type == BUY_ID:
				search_type_value.field = 'For sale'
			else:
				search_type_value.field = 'For rent'
			announce['SEARCH_TYPE'] = search_type_value

			announce_image_value = ValueType()
			announce_image_value.field = ad['pictures'][0]['url']
			announce_image_value.fieldtype = Type.STRING
			announce['ANNOUNCE_IMAGE'] = announce_image_value

			# Get images
			images = {}
			for picture in ad['pictures']:
				images['image{}'.format(picture['rank'])] = picture['url']
			
			images_value = ValueType()
			images_value.field = str(images) 
			images_value.fieldtype = Type.STRING
			announce['IMAGE'] = images_value

			price_value = ValueType()
			price_value.field = str(ad['price']['value'])
			price_value.fieldtype = Type.NUMBER
			announce['PRICE'] = price_value

			# For surface we must consult units_response to get the right unit of area
			surface_value = ValueType()
			surface_value.field = str(ad['area']['value'])
			surface_value.fieldtype = Type.NUMBER
			announce['SURFACE'] = surface_value

			rooms_value = ValueType()
			rooms_value.field = str(ad['rooms'])
			rooms_value.fieldtype = Type.NUMBER
			announce['ROOMS'] = rooms_value

			if ad['bedrooms']:
				bedrooms_value = ValueType()
				bedrooms_value.field = str(ad['bedrooms'])
				bedrooms_value.fieldtype = Type.STRING
				announce['BEDROOMS'] = bedrooms_value

			if ad['construction']['construction_year']:
				construction_value = ValueType()
				construction_value.field = str(ad['construction']['construction_year'])
				construction_value.fieldtype = Type.STRING
				announce['CONSTRUCTION_YEAR'] = construction_value

			if ad['heating']['type']:
				heating_value = ValueType()
				heating = heating_response[ad['heating']['type']-1]['name']
				if ad['heating']['access']:
					access = heating_access_response[ad['heating']
													 ['access']-1]['name']
					heating += '- {}'.format(access)
				heating_value.field = str(heating)
				heating_value.fieldtype = Type.STRING
				announce['TYPE_OF_HEATING'] = heating_value

			# Get cave
			cellars = 0
			for area in ad['areas']:
				if area['type'] == 6:
					cellars += 1
			cellar_value = ValueType()
			if cellars:
				cellar_value.field = str(cellars)
			else:
				cellar_value.field = 'Non'
			cellar_value.fieldtype = Type.STRING
			announce['CELLAR'] = cellar_value

			# Get floor
			if ad['floor']['value']:
				floor_value = ValueType()
				floor_value.field = str(ad['floor']['value'])
				floor_value.fieldtype = Type.STRING
				announce['FLOOR'] = floor_value

			# Get parking
			parkings = 0
			for area in ad['areas']:
				if area['type'] == 5:
					parkings += 1
			parking_value = ValueType()
			if parkings:
				parking_value.field = str(parkings)
			else:
				parking_value.field = 'Non'
			parking_value.fieldtype = Type.STRING
			announce['PARKING'] = parking_value

			if prop_type == 2:
				if ad['area']['total']:
					land_surface = ValueType()
					land_surface.field = str(ad['area']['total'])
					land_surface.fieldtype = Type.STRING
					announce['LAND_SURFACE'] = land_surface

			# Get balcony
			balcony = False
			for area in ad['areas']:
				if area['type'] == 43:
					balcony = True
					break
			balcony_value = ValueType()
			if balcony:
				balcony_value.field = 'Oui'
			else:
				balcony_value.field = 'Non'
			balcony_value.fieldtype = Type.STRING
			announce['BALCONY'] = balcony_value

			if ad['address']:
				address_value = ValueType()
				address_value.field = ad['address']
				address_value.fieldtype = Type.STRING
				announce['ADDRESS'] = address_value

			if ad['longitude'] and ad['latitude']:
				location_value = ValueType()
				location_value.field = "{}, {}".format(ad['longitude'], ad['latitude'])
				location_value.fieldtype = Type.STRING
				announce['LOCATION'] = location_value

			utc_now = datetime.datetime.now(pytz.utc)
			utc_now = utc_now.strftime("%Y-%m-%dT%H:%M:%S")

			timestamp_value = ValueType()
			timestamp_value.field = utc_now
			timestamp_value.fieldtype = Type.STRING

			announce['FIRST_TIMESTAMP'] = timestamp_value
			announce['TIMESTAMP'] = timestamp_value

			images_count_value = ValueType()
			images_count_value.field = str(len(ad['pictures']))
			images_count_value.fieldtype = Type.STRING
			announce['IMAGE_COUNT'] = images_count_value

			ret = serializer.send_data(announce)
			print(ret)

if __name__ == '__main__':
	main()
