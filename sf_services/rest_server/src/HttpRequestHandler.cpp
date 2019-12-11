/*
   (c) copyright 2019
   All rights reserved

   author(s): Noel Tchidjo
*/
#include "HttpRequestHandler.h"
#include "sf_services/sf_utils/inc/Logger.h"

#include <iostream>
using Log = surfyn::utils::Logger;

namespace surfyn
{
namespace rest_server
{

HttpRequestHandler::HttpRequestHandler(utility::string_t url, http_listener_config conf, const std::string& dbaccess_host, int dbaccess_port,
                                       const std::string& estimator_host, int estimator_port):m_listener(url, conf),
                                             m_dbaccess(dbaccess_host, dbaccess_port),
                                             m_estimatoraccess(estimator_host, estimator_port)
{
    m_listener.support(methods::GET, [this](http_request message) { handle_get(message);});
    m_listener.support(methods::PUT, [this](http_request message) { handle_put(message);});
    m_listener.support(methods::POST, [this](http_request message) { handle_post(message);});
    m_listener.support(methods::DEL, [this](http_request message) { handle_delete(message);});

}
HttpRequestHandler::~HttpRequestHandler()
{

}

void HttpRequestHandler::handle_error(pplx::task<void>& t)
{
    try
    {
        t.get();
    }
    catch(...)
    {
        // Ignore the error, Log it if a logger is available
    }
}


//
// Get Request 
//
void HttpRequestHandler::handle_get(http_request message)
{
    //ucout <<  message.to_string() << std::endl;
   Log::getInstance()->info(std::string(message.to_string()));

    auto paths = http::uri::split_path(http::uri::decode(message.relative_uri().path()));

    utility::stringstream_t sstream;

    if( paths.size() == 2  && paths[0] == "search" && paths[1] == "all")
    {
       auto query = http::uri::split_query(http::uri::decode(message.relative_uri().query()));
       m_dbaccess.fetchSummary(sstream, query);
    }
    else
    {
       if(paths.size() == 2  && paths[0] == "search" && paths[1] == "ad")
       {
          auto query = http::uri::split_query(http::uri::decode(message.relative_uri().query()));
          m_dbaccess.fetchDetails(sstream, query);
       }
       else
       {
          if(paths.size() == 1  && paths[0] == "predict")
           {
              auto query = http::uri::split_query(http::uri::decode(message.relative_uri().query()));
              m_estimatoraccess.fetchHousePrice(sstream, query);

           }
           else
           {
              std::string error = "Unknown requested service: ";
              for( auto valuepath : paths)
              {
                  error += valuepath;
                  error +="/";
              }
              Log::getInstance()->error(error);
              message.reply(status_codes::NotFound, "resource not found!");
              return;

           }
       }

    }

    //std::cout << sstream.str() << "\n";
    auto body_text = utility::conversions::to_utf8string(sstream.str());
    auto length = body_text.size();

    http_response response (status_codes::OK);
    response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
    response.set_body(concurrency::streams::bytestream::open_istream(std::move(body_text)),length, _XPLATSTR("application/json"));
    message.reply(response)
     .then([](pplx::task<void> t)
     {
        try{
           t.get();
        }
        catch(...){
           //
        }
     });

    return;

};

//
// A POST request
//
void HttpRequestHandler::handle_post(http_request message)
{
    ucout <<  message.to_string() << std::endl;


    message.reply(status_codes::OK,"OK");
    return ;
};

//
// A DELETE request
//
void HttpRequestHandler::handle_delete(http_request message)
{
    ucout <<  message.to_string() << std::endl;


    message.reply(status_codes::OK,"OK");
    return;
};


//
// A PUT request 
//
void HttpRequestHandler::handle_put(http_request message)
{
    ucout <<  message.to_string() << std::endl;

    message.reply(status_codes::OK, "OK");
    return;
};


}
}
