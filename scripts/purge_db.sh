#!/bin/bash
export LD_LIBRARY_PATH=/usr/local/lib/:$LD_LIBRARY_PATH
export PYTHONPATH=/home/ubuntu/software/thrift-0.10.0/lib/py/build/lib.linux-x86_64-3.6/:/home/ubuntu/scrapy_env/surfyn_bot/surfyn_bot/spiders:$PYTHONPATH

cd ~/software/surfyng/sf_services/dynamodb_access/build
./start_dyndb
for CITY in colombes nanterre puteaux houilles
do
        cd ~/software/surfyng/sf_services/sf_classifier/build/
        ./sf_classifier config_fr_properties.ini $CITY
        cd ~/software/surfyng/sf_services/sf_classifier/build/
        ./sf_classifier config_fr_summary.ini $CITY
        sleep 60
done
cd ~/software/surfyng/sf_services/dynamodb_access/build
./stop_dyndb

