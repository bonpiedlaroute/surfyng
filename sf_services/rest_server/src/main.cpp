/*
   (c) copyright 2019
   All rights reserved

   author(s): Noel Tchidjo
*/
#define BOOST_SYSTEM_NO_DEPRECATED
#include <iostream>

#include "HttpRequestHandler.h"
#include "stdafx.h"
#include "sf_services/sf_utils/inc/Config.h"
#include "sf_services/sf_utils/inc/Logger.h"
#include <string>
#include <thread>
#include <chrono>

using namespace std;
using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;

using surfyn::rest_server::HttpRequestHandler;
using Log = surfyn::utils::Logger;


std::unique_ptr<HttpRequestHandler> g_httpHandler;
std::string certificate_file = "";
std::string private_key_file = "";
std::string certificate_chain_file = "";

void on_initialize(const string_t& address, const std::string& dbaccess_host, int dbaccess_port,  const std::string& estimator_host, int estimator_port,
const std::string& emailalert_host, int emailalert_port, const std::string& deposit_host, int deposit_port)
{
    uri_builder uri(address);
    web::http::experimental::listener::http_listener_config             conf;
    conf.set_ssl_context_callback([](boost::asio::ssl::context &ctx)
    {
        ctx.set_options(boost::asio::ssl::context::default_workarounds);

        // Password callback needs to be set before setting cert and key.
        /*ctx.set_password_callback([](std::size_t max_length, boost::asio::ssl::context::password_purpose purpose)
        {
            return "";
        });*/

        //ctx.use_certificate_file(certificate_file.c_str(), boost::asio::ssl::context::pem);
        ctx.use_certificate_chain_file(certificate_chain_file.c_str());

        ctx.use_private_key_file(private_key_file.c_str(), boost::asio::ssl::context::pem);
     });

    auto addr = uri.to_uri().to_string();
    g_httpHandler = std::make_unique<HttpRequestHandler>(addr, conf, dbaccess_host, dbaccess_port, estimator_host, estimator_port,
    emailalert_host, emailalert_port, deposit_host, deposit_port);
    g_httpHandler->open().wait();

    //ucout << utility::string_t(U("Listening for requests at: ")) << addr << std::endl;
    std::string msg = "Listening for requests at: ";
    msg += addr;
    Log::getInstance()->info(msg);

    return;
}

void on_shutdown()
{
     g_httpHandler->close().wait();
    return;
}


#ifdef _WIN32
int wmain(int argc, wchar_t *argv[])
#else
int main(int argc, char *argv[])
#endif
{
    utility::string_t port = U("7878");
    utility::string_t address = U("http://127.0.0.1:");
    Log::Init("rest_server");
    Log::getInstance()->info("Starting rest server ...");
    std::string dbaccess_host = "localhost";
    int         dbaccess_port = 5050;
    std::string estimator_host = "localhost";
    int         estimator_port = 12200;
    std::string emailalert_host = "localhost";
    int         emailalert_port = 7810;
    std::string deposit_host = "localhost";
    int         deposit_port = 9000;

    if(argc == 2)
    {
       surfyn::utils::Config restserver_conf(argv[1]);
       restserver_conf.loadconfig();
       port = U(restserver_conf.getStringValue("port").c_str());

       std::string enable_ssl = restserver_conf.getStringValue("enable_ssl");

       std::string host = enable_ssl == "true" ? "https://": "http://";
       host+= restserver_conf.getStringValue("ip");
       host+=":";

       address = U(host.c_str());

       certificate_file = restserver_conf.getStringValue("certificate_file");
       private_key_file = restserver_conf.getStringValue("private_key_file");
       certificate_chain_file = restserver_conf.getStringValue("certificate_chain_file");

       dbaccess_host = restserver_conf.getStringValue("dbaccess_host");
       dbaccess_port = restserver_conf.getIntValue("dbaccess_port");
       estimator_host = restserver_conf.getStringValue("estimator_host");
       estimator_port = restserver_conf.getIntValue("estimator_port");
       emailalert_host = restserver_conf.getStringValue("emailalert_host");
       emailalert_port = restserver_conf.getIntValue("emailalert_port");
       deposit_host = restserver_conf.getStringValue("deposit_host");
       deposit_port = restserver_conf.getIntValue("deposit_port");
    }

    address.append(port);

    // Initialize and run rest_server
    try
    {
      on_initialize(address, dbaccess_host, dbaccess_port, estimator_host, estimator_port, emailalert_host, emailalert_port, deposit_host, deposit_port);
      Log::getInstance()->info("Listening to request at " + address);
      Log::getInstance()->info("rest server started");
      while(true) std::this_thread::sleep_for(std::chrono::hours(1)); // loop forever

    }
    catch(const std::exception& e)
    {
       std::cout << e.what() << std::endl;
    }

    on_shutdown();
    Log::getInstance()->info("rest server stopped");
    return 0;
}
