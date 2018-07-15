/*
   (c) copyright 2018
   All rights reserved

   author(s): Noel Tchidjo
*/
#include <iostream>
#include "RealEstateAdClassifier.h"
#include "PurgeRealEstateAd.h"

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <memory>

const int port = 5050;
using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;

using boost::shared_ptr;

using surfyng::classifier::RealEstateAdClassifier;


int main(int argc, char* argv[])
{
   //RealEstateAdClassifier Classifier;

   //Classifier.trainClassifier();
   shared_ptr<TTransport> socket(new TSocket("localhost", port));
   shared_ptr<TTransport> transport(new TBufferedTransport(socket));
   shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
   auto client= std::make_shared<dynamodb_accessClient>(protocol);

   transport->open();

   purgeRealEstateAd(client, "FR_PROPERTIES");

   return 0;
}
