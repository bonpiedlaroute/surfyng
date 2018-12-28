#include "HttpRequestHandler.h"

namespace surfyn
{
namespace rest_server
{

HttpRequestHandler::HttpRequestHandler()
{

}
HttpRequestHandler::HttpRequestHandler(utility::string_t url):m_listener(url)
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
    ucout <<  message.to_string() << std::endl;

    auto paths = http::uri::split_path(http::uri::decode(message.relative_uri().path()));

    message.relative_uri().path();

    utility::stringstream_t sstream;
    m_dbaccess.fetchData(sstream);
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
