#!/usr/bin/env python
# -*- coding: utf-8 -*- 
# (c) Copyright 2020 Noel Tchidjo
# All rights reserved


import csv

postalcodeByCity = dict()
inseecodeByCity = dict()
with open('spiders/data/inseecode_postalcode_bycity.csv') as csv_file:
    csv_reader = csv.DictReader(csv_file, delimiter=';')
    for row in csv_reader:
            inseecodeByCity[row["Commune"].lower()] = row["Code INSEE"]
            postalcodeByCity[row["Commune"].lower()] = row["Code Postal"]

osmidByInseeCode = dict()
with open('spiders/data/osm_id_insee.csv') as csv_file:
    csv_reader = csv.DictReader(csv_file, delimiter=';')
    for row in csv_reader:
            osmidByInseeCode[row["Code INSEE"]] = row["OSM_ID"]

logicimmocodeByCity = dict()
with open('spiders/data/logicimmo_code.csv') as csv_file:
    csv_reader = csv.DictReader(csv_file, delimiter=';')
    for row in csv_reader:
            logicimmocodeByCity[row["city"]] = row["code"]

papcodeByCity = dict()
with open('spiders/data/pap_code.csv') as csv_file:
    csv_reader = csv.DictReader(csv_file, delimiter=';')
    for row in csv_reader:
            papcodeByCity[row["city"]] = row["code"]

iadfrancecodeByCity = dict()
with open('spiders/data/iadfrance_code.csv') as csv_file:
    csv_reader = csv.DictReader(csv_file, delimiter=';')
    for row in csv_reader:
            iadfrancecodeByCity[row["city"]] = row["code"]

avendrealouercodeByCity = dict()
with open('spiders/data/avendrealouer_code.csv') as csv_file:
    csv_reader = csv.DictReader(csv_file, delimiter=';')
    for row in csv_reader:
            avendrealouercodeByCity[row["city"]] = row["code"]

