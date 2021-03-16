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
#include "sf_services/sf_utils/inc/Logger.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using Log = surfyn::utils::Logger;

#ifdef _WIN32
using std::shared_ptr;
#else
using boost::shared_ptr;
#endif

namespace surfyn
{
namespace rest_server
{
   EstimatorAccess::EstimatorAccess(const std::string& host, int port)
   {
      shared_ptr<TTransport> socket(new TSocket(host, port));
      shared_ptr<TTransport> transport(new TBufferedTransport(socket));
      shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
      m_client = std::make_shared<house_predictionClient>(protocol);

      transport->open();
   }

   void EstimatorAccess::fetchHousePrice(utility::stringstream_t& sstream, const std::map<utility::string_t,  utility::string_t>& query,
       bool isFlat,const std::string& highpricebym2, const std::string& lowpricebym2)
   {
      PredictionResult  result;
      m_client->getprice(result, query);

      sstream << U("[");

      if( result.error.empty()) // fill the value if no error
      {
         if( highpricebym2.empty() || lowpricebym2.empty())
            sstream << result.predictedvalue;
         else 
         {
            double high = atof(highpricebym2.c_str());
            double low = atof(lowpricebym2.c_str());
            std::string surface = "", surfaceAttribute = "";
            if(isFlat)
            {
               surfaceAttribute = "lot1_surface_carrez";
            }
            else {
               surfaceAttribute = "surface_reelle_bati";
            }
            auto iter_surface = query.find(surfaceAttribute);
            if(iter_surface != query.end())
            {
               surface = iter_surface->second;
            }
            double real_surface = atof(surface.c_str());
            double highhouseprice = high * real_surface;
            double lowhouseprice = low * real_surface;
            std::stringstream infomsg;
            infomsg << " Real Surface [";
            infomsg << real_surface << "] ";
            infomsg << " High price [" << highhouseprice << "] ";
            infomsg << " Low price  [" << lowhouseprice << "] ";
            Log::getInstance()->info(infomsg.str());

            if( result.predictedvalue >= lowhouseprice && result.predictedvalue <= highhouseprice )
            {
               sstream << result.predictedvalue;
            }else {
               if(result.predictedvalue < lowhouseprice )
               {
                  double corrected_price = low * real_surface;
                  sstream << (static_cast<int>(corrected_price));
               }
               else {
                  double corrected_price = high * real_surface;
                  sstream << (static_cast<int>(corrected_price));
               }
            }

         }
      }
        

      sstream << U("]");
   }
}
}
