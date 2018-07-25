/*
   (c) copyright 2018
   All rights reserved

   author(s): Noel Tchidjo
*/
#include <iostream>
#include "surfyng/sf_services/sf_utils/inc/Logger.h"
#include "PurgeRealEstateAd.h"
#include "Classifier.h"

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include "surfyng/sf_services/dynamodb_access/thrift_generated/dynamodb_access.h"


const int port = 5050;
using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;

using boost::shared_ptr;


using Log = surfyng::utils::Logger;


int main(int argc, char* argv[])
{

   Log::getInstance()->info("Starting Classifier ...");
   shared_ptr<TTransport> socket(new TSocket("localhost", port));
   shared_ptr<TTransport> transport(new TBufferedTransport(socket));
   shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
   auto client= std::make_shared<dynamodb_accessClient>(protocol);

   transport->open();

   //surfyng::classifier::purgeRealEstateAd(client, "FR_PROPERTIES");


   surfyng::classifier::detectSimilarRealEstateAd(client, "FR_PROPERTIES");


   return 0;
}
