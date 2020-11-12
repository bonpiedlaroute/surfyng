/*
   (c) copyright 2019
   All rights reserved

   author(s): Noel Tchidjo
*/
#include "SendEmailAccess.h"

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
   SendEmailAccess::SendEmailAccess(const std::string& host, int port)
   {
      shared_ptr<TTransport> socket(new TSocket(host, port));
      shared_ptr<TTransport> transport(new TBufferedTransport(socket));
      shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
      m_client = std::make_shared<sendemailserviceClient>(protocol);

      transport->open();
   }

   bool SendEmailAccess::sendToSurfyn(const std::string& sender_email, const std::string& subject, const std::string& msg)
   {
      SendEmailResult  result;
      m_client->sendemailtosurfyn(result,  sender_email, subject, msg);
      if(result.success)
         Log::getInstance()->info("Email successfully sent to Surfyn!");
      else {
            std::string error_msg = "Some issue appears during email sent [";
            error_msg += result.error;
            error_msg += "]";
            Log::getInstance()->error(error_msg);
      }
      return result.success;
   }
}
}
