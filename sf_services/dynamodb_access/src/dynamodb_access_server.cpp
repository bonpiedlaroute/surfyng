/*
   (c) copyright 2017
   All rights reserved

   author(s): Noel Tchidjo
*/

#include "dynamodb_access.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include "dynamodb_access_handler.h"
#include "sf_services/sf_utils/inc/Config.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

#ifdef _WIN32
using std::shared_ptr;
#else
using boost::shared_ptr;
#endif

int main(int argc, char **argv)
{

  ddba::dynamodb_accessHandler::Init();
  std::string config_filename = "dynamodb_access.ini";
  if(argc == 2)
      config_filename = argv[1];

  shared_ptr<ddba::dynamodb_accessHandler> handler(new ddba::dynamodb_accessHandler(config_filename));
  shared_ptr<TProcessor> processor(new dynamodb_accessProcessor(handler));

  surfyn::utils::Config conf(config_filename);
  conf.loadconfig();
  std::string address = conf.getStringValue("host");
  int port = conf.getIntValue("port");
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(address, port));
  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TThreadedServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return 0;
}

