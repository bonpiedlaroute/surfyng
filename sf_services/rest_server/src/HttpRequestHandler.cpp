/*
   (c) copyright 2019
   All rights reserved

   author(s): Noel Tchidjo
*/
#include "HttpRequestHandler.h"
#include "sf_services/sf_utils/inc/Logger.h"

#include <iostream>
#include <memory>
#include "SendEmailAccess.h"
#include "DepositAccess.h"

using Log = surfyn::utils::Logger;

namespace surfyn
{
namespace rest_server
{

HttpRequestHandler::HttpRequestHandler(utility::string_t url, http_listener_config conf, const std::string& dbaccess_host, int dbaccess_port,
                                       const std::string& estimator_host, int estimator_port, const std::string& emailalert_host, int emailalert_port, const std::string& deposit_host, int deposit_port)
                                       :m_listener(url, conf),
                                             m_dbaccess_host(dbaccess_host),
                                             m_dbaccess_port(dbaccess_port),
                                             m_estimator_host(estimator_host),
                                             m_estimator_port(estimator_port),
                                             m_geoLocalService(std::make_shared<surfyn::utils::GeoLocal>("../../../bot/data/correspondance_codeinsee_codepostal_iledefrance.csv")),
                                             m_emailalert_host(emailalert_host),
                                             m_emailalert_port(emailalert_port),
                                             m_deposit_host(deposit_host),
                                             m_deposit_port(deposit_port)
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
    Log::getInstance()->info(std::string(message.to_string()));

    auto paths = http::uri::split_path(http::uri::decode(message.relative_uri().path()));

    utility::stringstream_t sstream;

    if( paths.size() == 2  && paths[0] == "search" && paths[1] == "all")
    {
       auto query = http::uri::split_query(http::uri::decode(message.relative_uri().query()));

       std::make_shared<DBaccess>(m_dbaccess_host, m_dbaccess_port)->fetchSummary(sstream, query);
    }
    else
    {
       if(paths.size() == 2  && paths[0] == "search" && paths[1] == "ad")
       {
          auto query = http::uri::split_query(http::uri::decode(message.relative_uri().query()));
          std::make_shared<DBaccess>(m_dbaccess_host, m_dbaccess_port)->fetchDetails(sstream, query, m_geoLocalService);
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
                
                
                
                std::make_shared<DBaccess>(m_dbaccess_host, m_dbaccess_port)->fetchHighAndLowPriceByM2(inseecode, isFlat, highpricebym2, lowpricebym2);
            }
            std::make_shared<EstimatorAccess>(m_estimator_host, m_estimator_port)->fetchHousePrice(sstream, query,  isFlat, highpricebym2, lowpricebym2);

           }
           else 
           {
                if(paths.size() == 1  && paths[0] == "city_info")
                {
                    auto query = http::uri::split_query(http::uri::decode(message.relative_uri().query()));
                    std::make_shared<DBaccess>(m_dbaccess_host, m_dbaccess_port)->fetchCityInfo(sstream, query, m_geoLocalService);
                }
                else
                {
                    if(paths.size() == 1  && paths[0] == "my_realestate_search" )
                    {
                        auto query = http::uri::split_query(http::uri::decode(message.relative_uri().query()));
                        std::make_shared<EmailAlertAccess>(m_emailalert_host, m_emailalert_port)->my_realestate_search(sstream, query);
                    }else
                    {
                        if(paths.size() == 1 && paths[0] == "my_ad_search")
                        {
                            auto query = http::uri::split_query(http::uri::decode(message.relative_uri().query()));
                            std::make_shared<DepositAccess>(m_deposit_host, m_deposit_port)->fetch_user_announces(sstream, query);
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
                            http_response response (status_codes::NotFound);
                            response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
                            message.reply(response);
                            
                            return;
                        }

                    }
                }
                
           }
           
       }

    }

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
    else if(paths.size() == 1 && paths[0] == "announcedeposit")
    {
        auto query = http::uri::split_query(http::uri::decode(message.relative_uri().query()));
        message
            .extract_json(true)
            .then([=](pplx::task<json::value> data_raw)
            {
                try
                {
                    auto const & data = data_raw.get();
                    std::string user_id = data.at(U("user_id")).as_string();
                    auto result = std::make_shared<DepositAccess>(m_deposit_host, m_deposit_port)->announce_deposit(user_id, data.serialize());
                    
                    if(!result.success)
                    {
                        http_response response (status_codes::BadRequest);
                        response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
                        message.reply(response);
                        return;
                    }
                }
                catch(http_exception const & e)
                {
                    Log::getInstance()->info(e.what());
                }
            })
            .wait();
    }
    else if(paths.size() == 1 && paths[0] == "delete_announce")
    {
        auto query = http::uri::split_query(http::uri::decode(message.relative_uri().query()));
        auto result =  std::make_shared<DepositAccess>(m_deposit_host, m_deposit_port)->delete_announce(query);
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
        if(paths.size() == 1  && paths[0] == "registeremailalert" )
        {
            auto query = http::uri::split_query(http::uri::decode(message.relative_uri().query()));
            auto result =  std::make_shared<EmailAlertAccess>(m_emailalert_host, m_emailalert_port)->registerEmailAlert(query);
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
                auto result =  std::make_shared<EmailAlertAccess>(m_emailalert_host, m_emailalert_port)->changeAlertStatus(query);
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
