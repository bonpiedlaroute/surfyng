/*
 * Created on Thu Feb 18 2021
 *
 * Copyright (c) 2021 Surfyn
 */
#include "EmailAlertAccess.h"

#include <sstream>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>

#include "sf_services/emailalertservice/thrift_generated/emailalertservice_types.h"
#include "sf_services/sf_utils/inc/Logger.h"



using Log = surfyn::utils::Logger;


using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;


using boost::shared_ptr;


namespace surfyn
{
namespace rest_server
{/*
   EmailAlertResult registerEmailAlert(1: string userid, 2: map<string, string> parameters),
   EmailAlertResult notifyNewAnnounces(1: string city),
   list<map<string, string>> my_realestate_search(1: string userid),
   EmailAlertResult  deactivate_alert(1: string alert_id) 
*/
   EmailAlertAccess::EmailAlertAccess(const std::string& host, int port)
   {
      shared_ptr<TTransport> socket(new TSocket(host, port));
      shared_ptr<TTransport> transport(new TBufferedTransport(socket));
      shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
      m_client = std::make_shared<emailalertserviceClient>(protocol);

      transport->open();
   }
   EmailAlertResult EmailAlertAccess::registerEmailAlert(const std::map<utility::string_t,  utility::string_t>& query)
   {
      Log::getInstance()->info("registerEmailAlert");
      auto iter = query.find("userid");
      std::string userid = "";
      EmailAlertResult _return;
      if(iter == query.end())
      {
         Log::getInstance()->info("userid [" + userid +"] is not found!");
         _return.success = false;
         _return.error = "no userid found!";
         return _return;
      }
      userid = iter->second;
      std::map<std::string, std::string> parameters = query;
      
      m_client->registerEmailAlert(_return, userid, parameters);
      if(_return.success)
      {
         Log::getInstance()->info("Successfully register alert for user [" + userid + "]");
      }else
      {
         Log::getInstance()->error("Unable to register alert for user [" + userid + "] error :" + _return.error);
      }

      return _return;
   }

   void EmailAlertAccess::my_realestate_search(utility::stringstream_t& sstream, const std::map<utility::string_t,  utility::string_t>& query )
   {
      auto iter = query.find("userid");
      if( iter == query.end())
      {
         sstream << U("[]");
         return;
      }

      std::string userid = iter->second;
      std::vector<std::map<std::string, std::string> > _return;
      m_client->my_realestate_search(_return, userid);

      if(_return.empty())
      {
         sstream << U("[]");
         return;
      }

      sstream << U("[\n");
      for(auto iter_my_search = _return.begin(); iter_my_search != _return.end(); ++iter_my_search)
      {
         if( iter_my_search != _return.begin())
         {
            sstream << U(",\n");
         }
         sstream << U("{\n");
         for(auto attribute_iter = iter_my_search->begin(); attribute_iter != iter_my_search->end();++attribute_iter)
         {
            if( attribute_iter != iter_my_search->begin())
            {
               sstream << U(",\n");
            }

            sstream << "\"";
            sstream << attribute_iter->first;
            sstream << "\":\"";
            sstream << attribute_iter->second;
            sstream << "\"";
         }
         sstream << U("\n}");
      }
      sstream << U("\n]");

   }
   EmailAlertResult EmailAlertAccess::deactivate_alert(const std::map<utility::string_t,  utility::string_t>& query )
   {
      auto iter = query.find("alert_id");

      EmailAlertResult _return;
      if(iter == query.end())
      {
         _return.success = false;
         _return.error = "no alert_id found!";

         return _return;
      }

      std::string alert_id = iter->second;

      m_client->deactivate_alert(_return,alert_id);

      return _return;
   }
}
}