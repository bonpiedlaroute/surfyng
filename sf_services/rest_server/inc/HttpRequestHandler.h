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
#include "DepositAccess.h"

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
                              const std::string& emailalert_host, int emailalert_port,
                              const std::string& deposit_host, int deposit_port);
           virtual ~HttpRequestHandler();

           pplx::task<void>open(){return m_listener.open();}
           pplx::task<void>close(){return m_listener.close();}

       protected:

       private:
           void handle_get(http_request message);
           void handle_put(http_request message);
           void handle_post(http_request message);
           void handle_delete(http_request message);
           http_listener m_listener;
           std::string m_dbaccess_host;
           int m_dbaccess_port;
           std::string m_estimator_host;
           int m_estimator_port;
           std::string m_emailalert_host;
           int m_emailalert_port;
           std::string m_deposit_host;
           int m_deposit_port;
           std::shared_ptr<surfyn::utils::GeoLocal> m_geoLocalService;

        //    DepositAccess m_depositaccess;
           /*DBaccess m_dbaccess;
           EstimatorAccess m_estimatoraccess;
           EmailAlertAccess m_emailalertaccess;*/
   };
}
}



#endif /* INC_HTTPREQUESTHANDLER */


