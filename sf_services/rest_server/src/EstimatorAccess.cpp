/*
   (c) copyright 2019
   All rights reserved

   author(s): Noel Tchidjo
*/
#include "EstimatorAccess.h"

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <stdlib.h>

const int port = 6060;
using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;

#ifdef _WIN32
using std::shared_ptr;
#else
using boost::shared_ptr;
#endif

namespace surfyn
{
namespace rest_server
{
   EstimatorAccess::EstimatorAccess()
   {
      shared_ptr<TTransport> socket(new TSocket("localhost", port));
      shared_ptr<TTransport> transport(new TBufferedTransport(socket));
      shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
      m_client = std::make_shared<house_predictionClient>(protocol);

      //transport->open();
   }

   void EstimatorAccess::fetchHousePrice(utility::stringstream_t& sstream, const std::map<utility::string_t,  utility::string_t>& query)
   {
      PredictionResult  result;
      m_client->getprice(result, query);

      sstream << U("[");

      if( result.error.empty()) // fill the value if no error
         sstream << result.predictedvalue;

      sstream << U("]");
   }
}
}
