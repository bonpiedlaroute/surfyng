/*
   (c) copyright 2019
   All rights reserved

   author(s): Noel Tchidjo
*/
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


std::unique_ptr<HttpRequestHandler> g_httpHandler;


void on_initialize(const string_t& address)
{
    uri_builder uri(address);


    auto addr = uri.to_uri().to_string();
    g_httpHandler = std::make_unique<HttpRequestHandler>(addr);
    g_httpHandler->open().wait();

    ucout << utility::string_t(U("Listening for requests at: ")) << addr << std::endl;

    return;
}

void on_shutdown()
{
     g_httpHandler->close().wait();
    return;
}
using Log = surfyn::utils::Logger;

#ifdef _WIN32
int wmain(int argc, wchar_t *argv[])
#else
int main(int argc, char *argv[])
#endif
{
    utility::string_t port = U("7878");
    utility::string_t address = U("http://127.0.0.1:");

    Log::getInstance()->info("Starting rest server ...");

    if(argc == 2)
    {
       surfyn::utils::Config restserver_conf(argv[1]);
       restserver_conf.loadconfig();
       port = U(restserver_conf.getStringValue("port").c_str());

       std::string host = "http://";
       host+= restserver_conf.getStringValue("ip");
       host+=":";

       address = U(host.c_str());
    }

    address.append(port);

    try
    {
      on_initialize(address);
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
