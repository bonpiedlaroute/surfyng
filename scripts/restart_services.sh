#!/bin/bash
sudo /home/ubuntu/software/monit/stop_monit
sleep 2
sudo /home/ubuntu/software/surfyng/sf_services/rest_server/build/stop_rest
sleep 2
/home/ubuntu/software/surfyng/sf_services/dynamodb_access/build/stop_dyndb_rest
sleep 2
/home/ubuntu/software/surfyng/sf_services/dynamodb_access/build/start_dyndb_rest
sleep 2
sudo /home/ubuntu/software/surfyng/sf_services/rest_server/build/start_rest
sleep 2
sudo /home/ubuntu/software/monit/start_monit
