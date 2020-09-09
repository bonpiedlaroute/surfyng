#!/bin/bash
export LD_LIBRARY_PATH=/usr/local/lib/:$LD_LIBRARY_PATH
export PYTHONPATH=/home/ubuntu/software/thrift-0.10.0/lib/py/build/lib.linux-x86_64-3.6/:/home/ubuntu/scrapy_env/surfyn_bot/surfyn_bot/spiders:$PYTHONPATH

cd ~/software/surfyng/sf_services/dynamodb_access/build
./start_dyndb
declare -a cities=("HOUILLES" "ANTONY" "CHATENAY-MALABRY" "SCEAUX" "BOURG-LA-REINE" "BAGNEUX" "FONTENAY-AUX-ROSES" "LE PLESSIS-ROBINSON" "CLAMART" "CHATILLON" "MONTROUGE" "MALAKOFF" "VANVES" "ISSY-LES-MOULINEAUX" "BOULOGNE-BILLANCOURT" "MEUDON" "SEVRES" "CHAVILLE" "VILLE-D'AVRAY" "SAINT-CLOUD" "MARNES-LA-COQUETTE" "VAUCRESSON" "GARCHES" "SURESNES" "PUTEAUX" "NANTERRE" "COLOMBES" "LA GARENNE-COLOMBES" "BOIS-COLOMBES" "COURBEVOIE" "NEUILLY-SUR-SEINE" "LEVALLOIS-PERRET" "CLICHY" "ASNIERES-SUR-SEINE" "GENNEVILLIERS" "VILLENEUVE-LA-GARENNE")

for CITY in "${cities[@]}"
do
 cd ~/scrapy_env/
 source bin/activate
 cd surfyn_bot/surfyn_bot/
 index=0
 DATE=$(date +%Y_%m_%d"_"%H_%M_%S)
 scrapy crawl notaires -a city="$CITY" >> "notaires_"$CITY"_"$DATE".log" 2>&1 &
 pids[$index]=$!
 index=$((index+1))
 sleep 7
done
for pid in ${pids[*]}; do
 wait $pid
done
deactivate
cd ~/software/surfyng/sf_services/extracthousingprice/build
./extracthousingprice config.ini
cd ~/software/surfyng/sf_services/dynamodb_access/build
./stop_dyndb


