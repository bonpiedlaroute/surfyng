#!/bin/bash
sudo /home/ubuntu/software/monit/stop_monit
sleep 61
sudo /home/ubuntu/software/surfyng/sf_services/rest_server/build/stop_rest
sleep 2
/home/ubuntu/software/surfyng/sf_services/emailalertservice/stop_emailalert.sh
sleep 2
/home/ubuntu/software/surfyng/sf_services/dynamodb_access/build/stop_dyndb_rest
sleep 2
/home/ubuntu/software/surfyng/sklearn/stop_predictor
sleep 2
/home/ubuntu/software/surfyng/sf_services/mail/stop_mailservice
sleep 2
/home/ubuntu/software/surfyng/sf_services/dynamodb_access/build/start_dyndb_rest
sleep 2
/home/ubuntu/software/surfyng/sklearn/start_predictor
sleep 2
/home/ubuntu/software/surfyng/sf_services/emailalertservice/start_emailalert.sh
sleep 2
sudo /home/ubuntu/software/surfyng/sf_services/rest_server/build/start_rest
sleep 2
/home/ubuntu/software/surfyng/sf_services/mail/start_mailservice
sleep 2
sudo /home/ubuntu/software/monit/start_monit
