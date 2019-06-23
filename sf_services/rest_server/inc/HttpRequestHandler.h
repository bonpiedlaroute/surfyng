/*
   (c) copyright 2019
   All rights reserved

   author(s): Noel Tchidjo
*/
#ifndef _HTTP_REQUEST_HANDLER_
#define _HTTP_REQUEST_HANDLER_
#include <iostream>
#include "stdafx.h"
#include "DBaccess.h"
#include "EstimatorAccess.h"

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
           HttpRequestHandler();
           HttpRequestHandler(utility::string_t url, http_listener_config conf);
           virtual ~HttpRequestHandler();

           pplx::task<void>open(){return m_listener.open();}
           pplx::task<void>close(){return m_listener.close();}

       protected:

       private:
           void handle_get(http_request message);
           void handle_put(http_request message);
           void handle_post(http_request message);
           void handle_delete(http_request message);
           void handle_error(pplx::task<void>& t);
           http_listener m_listener;
           DBaccess m_dbaccess;
           EstimatorAccess m_estimatoraccess;

   };
}
}



#endif // _HTTP_REQUEST_HANDLER_


