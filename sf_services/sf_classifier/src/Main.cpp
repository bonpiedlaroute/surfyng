/*
   (c) copyright 2018
   All rights reserved

   author(s): Noel Tchidjo
*/
#include <iostream>
#include "sf_services/sf_utils/inc/Logger.h"
#include "sf_services/sf_utils/inc/Config.h"
#include "PurgeRealEstateAd.h"
#include "Classifier.h"

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include "dynamodb_access.h"


const int port = 5050;
using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;

#ifdef _WIN32
using std::shared_ptr;
#else
using boost::shared_ptr;
#endif

using Log = surfyn::utils::Logger;


int main(int argc, char* argv[])
{
   Log::Init("sf_classifier");
   Log::getInstance()->info("Starting Classifier ...");
   std::string tablename = "";
   std::string city ="";
   int maxDelayBeforePurgeSec = 86400;
   int maxDelayBeforePurgeSecForSurfynAd = 5184000;
   if(argc == 3)
   {
      surfyn::utils::Config conf(argv[1]);
      conf.loadconfig();
      tablename = conf.getStringValue("tablename");
      city = argv[2];
      maxDelayBeforePurgeSec = conf.getIntValue("max_delay_before_purge");
      maxDelayBeforePurgeSecForSurfynAd = conf.getIntValue("max_delay_before_purge_surfyn_ad");;
   }
   else
   {
      Log::getInstance()->error("No config file set! you need to set the tablename in the config file");
      Log::getInstance()->error("No city set! [./sf_classifier config_file city]");
      return 1;
   }


   shared_ptr<TTransport> socket(new TSocket("localhost", port));
   shared_ptr<TTransport> transport(new TBufferedTransport(socket));
   shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
   auto client= std::make_shared<dynamodb_accessClient>(protocol);



   transport->open();

   surfyn::classifier::purgeRealEstateAd(client, tablename, city, maxDelayBeforePurgeSec, maxDelayBeforePurgeSecForSurfynAd);

   //surfyn::classifier::detectSimilarRealEstateAd(client, "FR_PROPERTIES");


   return 0;
}
