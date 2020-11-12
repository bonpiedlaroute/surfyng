/*
   (c) copyright 2020
   All rights reserved

   author(s): Noel Tchidjo
*/
#ifndef INC_SENDEMAILACCESS_H_
#define INC_SENDEMAILACCESS_H_
#include "stdafx.h"
#include <map>
#include "sf_services/mail/thrift_generated/sendemailservice.h"
#include <memory>
#include <string>

namespace surfyn
{
namespace rest_server
{
   class SendEmailAccess
   {
   private:
      std::shared_ptr<sendemailserviceClient> m_client;
   public:
      SendEmailAccess(const std::string& host, int port);
      bool sendToSurfyn(const std::string& sender_email, const std::string& subject, const std::string& msg);

   };
}
}


#endif /* INC_SENDEMAILACCESS_H_ */
