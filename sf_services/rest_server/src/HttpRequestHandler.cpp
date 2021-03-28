/*
   (c) copyright 2019
   All rights reserved

   author(s): Noel Tchidjo
*/
#include "HttpRequestHandler.h"
#include "sf_services/sf_utils/inc/Logger.h"

#include <iostream>
#include "SendEmailAccess.h"

using Log = surfyn::utils::Logger;

namespace surfyn
{
namespace rest_server
{

HttpRequestHandler::HttpRequestHandler(utility::string_t url, http_listener_config conf, const std::string& dbaccess_host, int dbaccess_port,
                                       const std::string& estimator_host, int estimator_port, const std::string& emailalert_host, int emailalert_port)
                                       :m_listener(url, conf),
                                             m_dbaccess(dbaccess_host, dbaccess_port),
                                             m_estimatoraccess(estimator_host, estimator_port),
                                             m_geoLocalService(std::make_shared<surfyn::utils::GeoLocal>("../../../bot/data/correspondance_codeinsee_codepostal_iledefrance.csv")),
                                             m_emailalertaccess(emailalert_host, emailalert_port)
{
    m_listener.support(methods::GET, [this](http_request message) { handle_get(message);});
    m_listener.support(methods::PUT, [this](http_request message) { handle_put(message);});
    m_listener.support(methods::POST, [this](http_request message) { handle_post(message);});
    m_listener.support(methods::DEL, [this](http_request message) { handle_delete(message);});

}
HttpRequestHandler::~HttpRequestHandler()
{

}



//
// Get Request 
//
void HttpRequestHandler::handle_get(http_request message)
{
    //ucout <<  message.to_string() << std::endl;
   //Log::getInstance()->info(std::string(message.to_string()));

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
          m_dbaccess.fetchDetails(sstream, query, m_geoLocalService);
       }
       else
       {
          if(paths.size() == 1  && paths[0] == "predict")
           {
            auto query = http::uri::split_query(http::uri::decode(message.relative_uri().query()));
            auto iter_postalcode = query.find("code_postal");
            std::string highpricebym2 = "", lowpricebym2 = "";

            bool isFlat = false;
            auto iter_proptype = query.find("code_type_local");
            if( iter_proptype != query.end())
                isFlat = iter_proptype->second == "2";
            
            if(iter_postalcode != query.end())
            {
                auto inseecode = m_geoLocalService->getInseeCodeFromPostalCode(iter_postalcode->second);
                
                
                
                m_dbaccess.fetchHighAndLowPriceByM2(inseecode, isFlat, highpricebym2, lowpricebym2);
            }
            m_estimatoraccess.fetchHousePrice(sstream, query,  isFlat, highpricebym2, lowpricebym2);

           }else 
           {
                if(paths.size() == 1  && paths[0] == "city_info")
                {
                    auto query = http::uri::split_query(http::uri::decode(message.relative_uri().query()));
                    m_dbaccess.fetchCityInfo(sstream, query, m_geoLocalService);
                }
                else
                {
                    if(paths.size() == 1  && paths[0] == "my_realestate_search" )
                    {
                        auto query = http::uri::split_query(http::uri::decode(message.relative_uri().query()));
                        m_emailalertaccess.my_realestate_search(sstream, query);
                        
                    }else
                    {
                        std::string error = "Unknown requested service: ";
                        for( auto valuepath : paths)
                        {
                            error += valuepath;
                            error +="/";
                        }
                        Log::getInstance()->error(error);
                        http_response response (status_codes::NotFound);
                        response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
                        message.reply(response);
                        
                        return;

                    }
                }
                
           }
           
       }

    }

    //std::cout << sstream.str() << "\n";
    auto body_text = utility::conversions::to_utf8string(sstream.str());
    auto length = body_text.size();

    http_response response (status_codes::OK);
    response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
    response.set_body(concurrency::streams::bytestream::open_istream(std::move(body_text)),length, _XPLATSTR("application/json"));
    message.reply(response);

    return;

};

//
// A POST request
//
void HttpRequestHandler::handle_post(http_request message)
{
    
    Log::getInstance()->info(std::string(message.to_string()));

    auto paths = http::uri::split_path(http::uri::decode(message.relative_uri().path()));


    if( paths.size() == 1  && paths[0] == "sendemailtosurfyn" )
    {
        std::string host = "localhost";
        int port = 7800;
        SendEmailAccess emailAccess(host, port);

        auto query = http::uri::split_query(http::uri::decode(message.relative_uri().query()));
        auto iter = query.find("sender_email");
        std::string sender_email;
        if(iter != query.end())
            sender_email = iter->second;

        iter = query.find("subject");
        std::string subject;
        if(iter != query.end())
            subject = iter->second;
        
        iter = query.find("msg");
        std::string msg;
        if(iter != query.end())
            msg = iter->second;

        if( !emailAccess.sendToSurfyn(sender_email, subject, msg) )
        {
            http_response response (status_codes::ServiceUnavailable);
            response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
            message.reply(response);
            return;
        }
    }
    else 
    {
        if(paths.size() == 1  && paths[0] == "registeremailalert" )
        {
            auto query = http::uri::split_query(http::uri::decode(message.relative_uri().query()));
            auto result =  m_emailalertaccess.registerEmailAlert(query);
            if(!result.success)
            {
                http_response response (status_codes::BadRequest);
                response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
                message.reply(response);
                return;
            }
        }
        else 
        {
            if(paths.size() == 1  && paths[0] == "change_alert_status" )
            {
                auto query = http::uri::split_query(http::uri::decode(message.relative_uri().query()));
                auto result =  m_emailalertaccess.changeAlertStatus(query);
                if(!result.success)
                {
                    http_response response (status_codes::BadRequest);
                    response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
                    message.reply(response);
                    return;
                }
            }
            else {
                    http_response response (status_codes::BadRequest);
                    response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
                    message.reply(response);
                    return;
                }
        }
        
    }

    http_response response (status_codes::OK);
    response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
    message.reply(response);
    return ;
};

//
// A DELETE request
//
void HttpRequestHandler::handle_delete(http_request message)
{
    //ucout <<  message.to_string() << std::endl;
    Log::getInstance()->info(std::string(message.to_string()));

    http_response response (status_codes::OK);
    response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
    message.reply(response);
    return;
};


//
// A PUT request 
//
void HttpRequestHandler::handle_put(http_request message)
{
    //ucout <<  message.to_string() << std::endl;
    Log::getInstance()->info(std::string(message.to_string()));

    http_response response (status_codes::OK);
    response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
    message.reply(response);
    return;
};


}
}
