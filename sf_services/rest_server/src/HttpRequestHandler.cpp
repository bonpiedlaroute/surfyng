/*
   (c) copyright 2019
   All rights reserved

   author(s): Noel Tchidjo
*/
#include "HttpRequestHandler.h"
#include "sf_services/sf_utils/inc/Logger.h"

#include <cpprest/http_msg.h>
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

    m_http_get_services.insert(std::make_pair("/search/all", [this](http_request& message){handle_searchall(message);}));
    m_http_get_services.insert(std::make_pair("/search/ad", [this](http_request& message){handle_searchad(message);}));
    m_http_get_services.insert(std::make_pair("/predict", [this](http_request& message){handle_predict(message);}));
    m_http_get_services.insert(std::make_pair("/city_info", [this](http_request& message){handle_city_info(message);}));
    m_http_get_services.insert(std::make_pair("/my_realestate_search", [this](http_request& message){handle_my_realestate_search(message);}));

    m_http_post_services.insert(std::make_pair("/sendemailtosurfyn", [this](http_request& message){handle_sendemailtosurfyn(message);}));
    m_http_post_services.insert(std::make_pair("/registeremailalert", [this](http_request& message){handle_registeremailalert(message);}));
    m_http_post_services.insert(std::make_pair("/change_alert_status", [this](http_request& message){handle_change_alert_status(message);}));
    m_http_post_services.insert(std::make_pair("/accountcreation", [this](http_request& message){handle_accountcreation(message);}));
    m_http_post_services.insert(std::make_pair("/announcedeposit", [this](http_request& message){handle_announce_deposit(message);}));
    m_http_post_services.insert(std::make_pair("/delete_announce", [this](http_request& message){handle_delete_announce(message);}));
}
HttpRequestHandler::~HttpRequestHandler()
{

}

void HttpRequestHandler::send_json_response(const utility::stringstream_t& sstream, http_request& message)
{
    auto body_text = utility::conversions::to_utf8string(sstream.str());
    auto length = body_text.size();

    http_response response (status_codes::OK);
    response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
    response.set_body(concurrency::streams::bytestream::open_istream(std::move(body_text)),length, _XPLATSTR("application/json"));
    message.reply(response);
}

void HttpRequestHandler::handle_searchall(http_request& message)
{  
    auto query = http::uri::split_query(http::uri::decode(message.relative_uri().query()));

    utility::stringstream_t sstream;
    std::make_shared<DBaccess>(m_dbaccess_host, m_dbaccess_port)->fetchSummary(sstream, query);

    send_json_response(sstream, message);
}

void HttpRequestHandler::handle_searchad(http_request& message)
{
    auto query = http::uri::split_query(http::uri::decode(message.relative_uri().query()));

    utility::stringstream_t sstream;
    std::make_shared<DBaccess>(m_dbaccess_host, m_dbaccess_port)->fetchDetails(sstream, query, m_geoLocalService);

    send_json_response(sstream, message);
}

void HttpRequestHandler::handle_predict(http_request& message)
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
    utility::stringstream_t sstream;
    std::make_shared<EstimatorAccess>(m_estimator_host, m_estimator_port)->fetchHousePrice(sstream, query,  isFlat, highpricebym2, lowpricebym2);

    send_json_response(sstream, message);
}
void HttpRequestHandler::handle_city_info(http_request& message)
{
    auto query = http::uri::split_query(http::uri::decode(message.relative_uri().query()));

    utility::stringstream_t sstream;
    std::make_shared<DBaccess>(m_dbaccess_host, m_dbaccess_port)->fetchCityInfo(sstream, query, m_geoLocalService);

    send_json_response(sstream, message);
}
void HttpRequestHandler::handle_my_realestate_search(http_request& message)
{
    auto query = http::uri::split_query(http::uri::decode(message.relative_uri().query()));

    utility::stringstream_t sstream;
    std::make_shared<EmailAlertAccess>(m_emailalert_host, m_emailalert_port)->my_realestate_search(sstream, query);

    send_json_response(sstream, message);
}

//
// Get Request 
//
void HttpRequestHandler::handle_get(http_request message)
{
    Log::getInstance()->info(std::string(message.to_string()));

    auto service = http::uri::decode(message.relative_uri().path()) ;

    auto iter  = m_http_get_services.find(service);
    if( iter != m_http_get_services.end())
    {
        iter->second(message);
    }
    else 
    {
        std::string error = "Unknown requested service: ";
        error += service;

        Log::getInstance()->error(error);
        http_response response (status_codes::NotFound);
        response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
        message.reply(response);
    }
}

void HttpRequestHandler::send_ok_response(http_request& message)
{
    http_response response (status_codes::OK);
    response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
    message.reply(response);
}
void HttpRequestHandler::send_badrequest_response(http_request& message)
{
    http_response response (status_codes::BadRequest);
    response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
    message.reply(response);
}

void HttpRequestHandler::send_serviceunavailable_response(http_request& message)
{
    http_response response (status_codes::ServiceUnavailable);
    response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
    message.reply(response);
} 

void HttpRequestHandler::handle_sendemailtosurfyn(http_request& message)
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
        send_serviceunavailable_response(message);
        return;
    }

    send_ok_response(message);

}
void HttpRequestHandler::handle_registeremailalert(http_request& message)
{
    auto query = http::uri::split_query(http::uri::decode(message.relative_uri().query()));
    auto result =  std::make_shared<EmailAlertAccess>(m_emailalert_host, m_emailalert_port)->registerEmailAlert(query);
    if(!result.success)
    {
        send_badrequest_response(message);
        return;
    }

    send_ok_response(message);

}
void HttpRequestHandler::handle_change_alert_status(http_request& message)
{
    auto query = http::uri::split_query(http::uri::decode(message.relative_uri().query()));
    auto result =  std::make_shared<EmailAlertAccess>(m_emailalert_host, m_emailalert_port)->changeAlertStatus(query);
    if(!result.success)
    {
        send_badrequest_response(message);
        return;
    }

    send_ok_response(message);

}

void HttpRequestHandler::handle_accountcreation(http_request& message)
{
    std::string host = "localhost";
    int port = 7800;
    SendEmailAccess emailAccess(host, port);

    auto query = http::uri::split_query(http::uri::decode(message.relative_uri().query()));
    auto iter = query.find("user_displayname");
    std::string user_displayname;
    if(iter != query.end())
        user_displayname = iter->second;

    iter = query.find("user_email");
    std::string user_email;
    if(iter != query.end())
        user_email = iter->second;
    else 
    {
        send_badrequest_response(message);
        return;
    }
    
    if( !emailAccess.sendAccountCreationEmail(user_displayname,user_email))
    {
        send_serviceunavailable_response(message);
        return;
    }

    send_ok_response(message);

}

void HttpRequestHandler::handle_announce_deposit(http_request& message)
{
    auto query = http::uri::split_query(http::uri::decode(message.relative_uri().query()));
    message.extract_json(true)
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
    send_ok_response(message);
}
void HttpRequestHandler::handle_delete_announce(http_request& message)
{
    auto query = http::uri::split_query(http::uri::decode(message.relative_uri().query()));
    auto result =  std::make_shared<DepositAccess>(m_deposit_host, m_deposit_port)->delete_announce(query);
    if(!result.success)
    {
        send_badrequest_response(message);
        return;
    }
    send_ok_response(message);
}
//
// A POST request
//
void HttpRequestHandler::handle_post(http_request message)
{
    Log::getInstance()->info(std::string(message.to_string()));

    auto service = http::uri::decode(message.relative_uri().path()) ;

    auto iter  = m_http_post_services.find(service);
    if( iter != m_http_post_services.end())
    {
        iter->second(message);
    }
    else
    {
        std::string error = "Unknown requested service: ";
        error += service;

        Log::getInstance()->error(error);
        http_response response (status_codes::NotFound);
        response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
        message.reply(response);
    }
}

//
// A DELETE request
//
void HttpRequestHandler::handle_delete(http_request message)
{
    std::string error = "Unknown DELETE Request ";

    Log::getInstance()->error(error);
    http_response response (status_codes::NotFound);
    response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
    message.reply(response);
}


//
// A PUT request 
//
void HttpRequestHandler::handle_put(http_request message)
{    
    std::string error = "Unknown PUT Request ";

    Log::getInstance()->error(error);
    http_response response (status_codes::NotFound);
    response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
    message.reply(response);
}


}
}
