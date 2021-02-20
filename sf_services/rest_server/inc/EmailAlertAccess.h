/*
 * Created on Wed Feb 17 2021
 *
 * Copyright (c) 2021 Surfyn
 */

#ifndef INC_EMAILALERTACCESS_H_
#define INC_EMAILALERTACCESS_H_
#include "stdafx.h"
#include <map>
#include "sf_services/emailalertservice/thrift_generated/emailalertservice.h"
#include <memory>
#include <string>

namespace surfyn
{
namespace rest_server
{
   class EmailAlertAccess
   {
   private:
      std::shared_ptr<emailalertserviceClient> m_client;
   public:
      EmailAlertAccess(const std::string& host, int port);
      EmailAlertResult registerEmailAlert(const std::map<utility::string_t,  utility::string_t>& query);
      void my_realestate_search(utility::stringstream_t& sstream, const std::map<utility::string_t,  utility::string_t>& query );
      EmailAlertResult deactivate_alert(const std::map<utility::string_t,  utility::string_t>& query );

   };
}
}
#endif /* INC_EMAILALERTACCESS */
