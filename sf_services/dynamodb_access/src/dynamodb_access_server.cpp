/*
   (c) copyright 2017
   All rights reserved

   author(s): Noel Tchidjo
*/

#include "dynamodb_access.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include "dynamodb_access_handler.h"

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
  int port = 5050;
  ddba::dynamodb_accessHandler::Init();
  shared_ptr<ddba::dynamodb_accessHandler> handler(new ddba::dynamodb_accessHandler());
  shared_ptr<TProcessor> processor(new dynamodb_accessProcessor(handler));
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return 0;
}

