#!/bin/bash
export LD_LIBRARY_PATH=/usr/local/lib/:$LD_LIBRARY_PATH
export PYTHONPATH=/home/ubuntu/software/thrift-0.10.0/lib/py/build/lib.linux-x86_64-3.6/:/home/ubuntu/scrapy_env/surfyn_bot/surfyn_bot/spiders:$PYTHONPATH
#cd ~/software/surfyng/sf_services/rest_server/build/
#sudo ./stop_rest
#cd /home/ubuntu/software/surfyng/sklearn
#./stop_predictor

cd ~/software/surfyng/sf_services/dynamodb_access/build
./start_dyndb

for CITY in colombes nanterre puteaux houilles
do
        cd ~/scrapy_env/
        source bin/activate
        cd surfyn_bot/surfyn_bot/
        rm -f ~/scrapy_env/surfyn_bot/surfyn_bot/images_"$CITY"/*
        index=0
        DATE=$(date +%Y_%m_%d"_"%H_%M_%S)
        scrapy crawl seloger -a city="$CITY" >> "seloger_"$DATE".log" 2>&1 &
        pids[$index]=$!
        index=$((index+1))
        sleep 3
        DATE=$(date +%Y_%m_%d"_"%H_%M_%S)
        scrapy crawl leboncoin -a city="$CITY" >> "leboncoin_"$DATE".log" 2>&1 &
        pids[$index]=$!
        index=$((index+1))
        sleep 3
        DATE=$(date +%Y_%m_%d"_"%H_%M_%S)
        scrapy crawl bienici -a city="$CITY" >> "bienici_"$DATE".log" 2>&1 &
        pids[$index]=$!
        index=$((index+1))
        sleep 3
        DATE=$(date +%Y_%m_%d"_"%H_%M_%S)
        scrapy crawl laforet -a city="$CITY" >> "laforet_"$DATE".log" 2>&1 &
        pids[$index]=$!
        index=$((index+1))
        sleep 3
        DATE=$(date +%Y_%m_%d"_"%H_%M_%S)
        scrapy crawl orpi -a city="$CITY" >> "orpi_"$DATE".log" 2>&1 &
        pids[$index]=$!
        index=$((index+1))
        sleep 3
        DATE=$(date +%Y_%m_%d"_"%H_%M_%S)
        scrapy crawl stephaneplazaimo -a city="$CITY" >> "stephaneplazaimo_"$DATE".log" 2>&1 &
        pids[$index]=$!
        index=$((index+1))
        sleep 3
        DATE=$(date +%Y_%m_%d"_"%H_%M_%S)
        scrapy crawl foncia -a city="$CITY" >> "foncia_"$DATE".log" 2>&1 &
        pids[$index]=$!
        index=$((index+1))
        sleep 3
        DATE=$(date +%Y_%m_%d"_"%H_%M_%S)
        scrapy crawl century21 -a city="$CITY" >> "century21_"$DATE".log" 2>&1 &
        pids[$index]=$!
        index=$((index+1))
        sleep 3
        DATE=$(date +%Y_%m_%d"_"%H_%M_%S)
        scrapy crawl guyhoquet -a city="$CITY" >> "guyhoquet_"$DATE".log" 2>&1 &
        pids[$index]=$!
        index=$((index+1))
        sleep 3
        DATE=$(date +%Y_%m_%d"_"%H_%M_%S)
        scrapy crawl arthurimmo -a city="$CITY" >> "arthurimmo_"$DATE".log" 2>&1 &
        pids[$index]=$!
        index=$((index+1))
        sleep 3
        DATE=$(date +%Y_%m_%d"_"%H_%M_%S)
        scrapy crawl eraimmo -a city="$CITY" >> "eraimmo_"$DATE".log" 2>&1 &
        pids[$index]=$!
        index=$((index+1))
        sleep 3
        DATE=$(date +%Y_%m_%d"_"%H_%M_%S)
        scrapy crawl pap -a city="$CITY" >> "pap_"$DATE".log" 2>&1 &
        pids[$index]=$!
        index=$((index+1))
        sleep 3
        DATE=$(date +%Y_%m_%d"_"%H_%M_%S)
        scrapy crawl iadfrance -a city="$CITY" >> "iadfrance_"$DATE".log" 2>&1 &
        pids[$index]=$!
        index=$((index+1))
        sleep 3
        DATE=$(date +%Y_%m_%d"_"%H_%M_%S)
        scrapy crawl paruvendu -a city="$CITY" >> "paruvendu_"$DATE".log" 2>&1 &
        pids[$index]=$!
        index=$((index+1))
        sleep 3
        DATE=$(date +%Y_%m_%d"_"%H_%M_%S)
        scrapy crawl avendrealouer -a city="$CITY" >> "avendrealouer_"$DATE".log" 2>&1 &
        pids[$index]=$!
        index=$((index+1))
        sleep 3
        DATE=$(date +%Y_%m_%d"_"%H_%M_%S)
        scrapy crawl nestenn -a city="$CITY" >> "nestenn_"$DATE".log" 2>&1 &
        pids[$index]=$!
        index=$((index+1))
        sleep 3
        DATE=$(date +%Y_%m_%d"_"%H_%M_%S)
        scrapy crawl agenceprincipale -a city="$CITY" >> "agenceprincipale_"$DATE".log" 2>&1 &
        pids[$index]=$!
        index=$((index+1))
        sleep 3
        DATE=$(date +%Y_%m_%d"_"%H_%M_%S)
        scrapy crawl lefigaroimmobilier -a city="$CITY" >> "lefigaroimmobilier_"$DATE".log" 2>&1 &
        pids[$index]=$!
        index=$((index+1))
        sleep 3
        DATE=$(date +%Y_%m_%d"_"%H_%M_%S)
        scrapy crawl etreproprio -a city="$CITY" >> "etreproprio_"$DATE".log" 2>&1 &
        pids[$index]=$!
        sleep 3
        DATE=$(date +%Y_%m_%d"_"%H_%M_%S)
        scrapy crawl efficity -a city="$CITY" >> "efficity_"$DATE".log" 2>&1 &
        pids[$index]=$!


        # wait for all scrapy pids
        for pid in ${pids[*]}; do
            wait $pid
        done
        deactivate
        #cd ~/software/surfyng/sf_services/sf_classifier/build/
        #./sf_classifier config_fr_properties.ini $CITY
        cd ~/tf_env/
        source bin/activate
        cd ~/software/surfyng/tensorflow/
        rm -rf image_to_labels.json image_vectors nearest_neighbors
        DATE=$(date +%Y_%m_%d"_"%H_%M_%S)
        python classify_images.py "images_"$CITY"/*" >> "tf_output_"$DATE".log" 2>&1
        DATE=$(date +%Y_%m_%d"_"%H_%M_%S)
        python surfyn.py >> "surfyn_"$DATE".log" 2>&1
        deactivate
        cd ~/software/surfyng/sf_services/adapt_model/build
        ./adapt_model config.ini $CITY
        #cd ~/software/surfyng/sf_services/sf_classifier/build/
        #./sf_classifier config_fr_summary.ini $CITY
        #cd /home/ubuntu/software/surfyng/sklearn
        #./start_predictor
        #sleep 10
        #cd ~/software/surfyng/sf_services/rest_server/build/
        #sudo ./start_rest
        cd ~/software/surfyng/sf_services/duplicates_fix/build
        ./duplicates_fix config.ini $CITY
        rm -f ~/scrapy_env/surfyn_bot/surfyn_bot/images_"$CITY"/*
        rm -rf image_to_labels.json image_vectors nearest_neighbors
        sleep 60
done
cd ~/software/surfyng/sf_services/dynamodb_access/build
./stop_dyndb

