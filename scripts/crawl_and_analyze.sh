#!/bin/bash
cd ~/software/surfyng/sf_services/dynamodb_access/build
./start_dyndb
cd ~/scrapy_env/
source bin/activate
cd surfyn_bot/surfyn_bot/
rm -f images/*
DATE=$(date +%Y_%m_%d"_"%H_%M_%S)
scrapy crawl seloger >> "seloger_"$DATE".log" 2>&1
DATE=$(date +%Y_%m_%d"_"%H_%M_%S)
scrapy crawl leboncoin >> "leboncoin_"$DATE".log" 2>&1
DATE=$(date +%Y_%m_%d"_"%H_%M_%S)
scrapy crawl bienici >> "bienici_"$DATE".log" 2>&1
deactivate
cd ~/software/surfyng/sf_services/sf_classifier/build/
./sf_classifier config_fr_properties.ini
cd ~/tf_env/
source bin/activate
cd ~/software/surfyng/tensorflow/
rm -rf image_to_labels.json image_vectors nearest_neighbors
python classify_images.py "images/*"
DATE=$(date +%Y_%m_%d"_"%H_%M_%S)
python surfyn.py >> "surfyn_"$DATE".log" 2>&1
deactivate
cd ~/software/surfyng/sf_services/adapt_model/build
./adapt_model
cd ~/software/surfyng/sf_services/sf_classifier/build/
./sf_classifier config_fr_summary.ini
cd ~/software/surfyng/sf_services/dynamodb_access/build
./stop_dyndb

