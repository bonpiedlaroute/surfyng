/*
   (c) copyright 2019
   All rights reserved

   author(s): Noel Tchidjo
*/
#ifndef INC_HTTPREQUESTHANDLER
#define INC_HTTPREQUESTHANDLER
#include <iostream>
#include "stdafx.h"
#include "DBaccess.h"
#include "EstimatorAccess.h"
#include "sf_services/sf_utils/inc/Geolocal.h"
#include "EmailAlertAccess.h"
#include <unordered_map>

using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;

namespace surfyn
{
namespace rest_server
{
   class HttpRequestHandler
   {
       public:
           HttpRequestHandler(utility::string_t url, http_listener_config conf,
                              const std::string& dbaccess_host, int dbaccess_port,
                              const std::string& estimator_host, int estimator_port,
                              const std::string& emailalert_host, int emailalert_port);
           virtual ~HttpRequestHandler();

           pplx::task<void>open(){return m_listener.open();}
           pplx::task<void>close(){return m_listener.close();}

       protected:

       private:
           void handle_get(http_request message);
           void handle_put(http_request message);
           void handle_post(http_request message);
           void handle_delete(http_request message);

           void send_json_response(const utility::stringstream_t& sstream, http_request& message);
           void send_ok_response(http_request& message);
           void send_badrequest_response(http_request& message);
           void send_serviceunavailable_response(http_request& message);

           /* GET SERVICE REQUEST HANDLERS */
           void handle_searchall(http_request& message);
           void handle_searchad(http_request& message);
           void handle_predict(http_request& message);
           void handle_city_info(http_request& message);
           void handle_my_realestate_search(http_request& message);

            /* POST SERVICE REQUEST HANDLERS */
           void handle_sendemailtosurfyn(http_request& message);
           void handle_registeremailalert(http_request& message);
           void handle_change_alert_status(http_request& message);
           void handle_accountcreation(http_request& message);
       private:
           http_listener m_listener;
           std::string m_dbaccess_host;
           int m_dbaccess_port;
           std::string m_estimator_host;
           int m_estimator_port;
           std::string m_emailalert_host;
           int m_emailalert_port;
           std::shared_ptr<surfyn::utils::GeoLocal> m_geoLocalService;

          std::unordered_map<std::string, std::function<void(http_request& message)>> m_http_get_services;
          std::unordered_map<std::string, std::function<void(http_request& message)>> m_http_post_services;
   };
}
}



#endif /* INC_HTTPREQUESTHANDLER */


