#ifndef MWA_CLIENT_H
#define MWA_CLIENT_H

#if defined(__EMSCRIPTEN__)
#define MWA_EMSCRIPTEN
#endif

#if defined(MWA_IMPLEMENTATION)

#if !defined(MWA_EMSCRIPTEN)
#define CPPHTTPLIB_IMPLEMENTATION
#endif

#define SHA1_IMPLEMENTATION
#endif

#if defined(MWA_EMSCRIPTEN)
#include <emscripten.h>
#else

#define CPPHTTPLIB_OPENSSL_SUPPORT
//#define CPPHTTPLIB_ZLIB_SUPPORT
#include "httplib.h"

#endif

#include <chrono>
#include <string>

namespace vendor
{
    #include "sha1.h"
}

#include "json.hpp"

#if defined(MWA_EMSCRIPTEN) && defined(MWA_IMPLEMENTATION)
    EM_JS(char *, mwa_get_user_agent, (), {
        var len = lengthBytesUTF8(window.navigator.userAgent) + 1;
        var buffer = _malloc(len);
        stringToUTF8(window.navigator.userAgent, buffer, len);
        return buffer;
    });

    EM_JS(char*, mwa_do_fetch, (const char* pHost, const char* pMethod, const char* pURI, const char* pBody, const char* pUserAgent, const char* pTime, const char* pSignature), {
        return Asyncify.handleSleep(wakeUp => {

            const host = UTF8ToString(pHost);
            const method = UTF8ToString(pMethod);
            const uri = UTF8ToString(pURI);
            const body = UTF8ToString(pBody);
            const userAgent = UTF8ToString(pUserAgent);
            const time = UTF8ToString(pTime);
            const sign = UTF8ToString(pSignature);
            
            fetch(host+uri, {
                method: method,
                mode: 'cors',
                body: (body.length > 0) ? body : null,
                headers: {
                    'Content-Type': 'application/json',
                    'Authorization': 'Bearer ' + time.toString() + '.' + sign.toString()
                }
            }).then(response => response.text()).then((response) => {
                var buffer = _malloc(response.length);
                stringToUTF8(response, buffer, response.length * 4 + 1);
                wakeUp(buffer);
            });
        });
    });
#endif    

namespace mwa
{
    class WebApiClient
    {
    public:
        // Constructs an empty WebApiClient
        WebApiClient();
        // Constructs a WebApiClient with the specified host, secret, and user agent
        WebApiClient(const std::string& host, const std::string& secret, const std::string& userAgent="MWA Client v0");
        ~WebApiClient();

        void SetHost(const std::string& host);
        void SetSecret(const std::string& secret);
        void SetUserAgent(const std::string& userAgent);

        std::string Get(const std::string& uri);
        std::string Post(const std::string& uri, const std::string& body  = "[]");

    protected:
        virtual std::string Sign(const std::string& input);
        std::string Time();

    private:
        std::string userAgent;
        std::string secret;
        std::string host;
    };

} // mwa

#if defined(MWA_IMPLEMENTATION)
#undef MWA_IMPLEMENTATION

namespace mwa
{

    WebApiClient::WebApiClient()
    {
#if defined(MWA_EMSCRIPTEN)
        char* temp = mwa_get_user_agent();
        this->userAgent = temp;
        delete temp;
#else
        this->userAgent = "MWA Client v1";
#endif
    }

    WebApiClient::WebApiClient(const std::string& host, const std::string& secret, const std::string& userAgent)
    {
        SetHost(host);
        SetSecret(secret);
        SetUserAgent(userAgent);
    }

    WebApiClient::~WebApiClient()
    {
    }

    void WebApiClient::SetHost(const std::string& host)
    {
        this->host = host;
    }

    void WebApiClient::SetSecret(const std::string& secret)
    {
        this->secret = secret;
    }

    void WebApiClient::SetUserAgent(const std::string& userAgent)
    {
        this->userAgent = userAgent;
    }

#if defined(MWA_EMSCRIPTEN)
    std::string WebApiClient::Get(const std::string& uri)
    {
        std::string sTime = Time();
        std::string sSignature = Sign(secret + sTime + userAgent + "\nescape");
        char* temp = mwa_do_fetch(host.c_str(), "GET", uri.c_str(), "", userAgent.c_str(), sTime.c_str(), sSignature.c_str());
        std::string sRet = temp;
        delete temp;

        return sRet;
    }
    
    std::string WebApiClient::Post(const std::string& uri, const std::string& body)
    {
        std::string sTime = Time();
        std::string sSignature = Sign(secret + sTime + userAgent + "\nescape");
        char* temp = mwa_do_fetch(host.c_str(), "POST", uri.c_str(), body.c_str(), userAgent.c_str(), sTime.c_str(), sSignature.c_str());
        std::string sRet = temp;
        delete temp;

        return sRet;
    }

#else
    std::string WebApiClient::Get(const std::string& uri)
    {
        std::string sTime = Time();
        std::string sSignature = Sign(secret + sTime + userAgent + "\nescape");
        
        httplib::Headers headers = {
            { "Accepts", "application/json" },
            { "User-Agent", userAgent },
            { "Authorization", "Bearer " + sTime + "." + sSignature }
            // { "time", sTime },
            // { "sign", sSignature }
        };

        httplib::Client client(host.c_str());
        client.enable_server_certificate_verification(true);

        auto response = client.Get(uri.c_str(), headers);
        return response->body;
    }

    std::string WebApiClient::Post(const std::string& uri, const std::string& body)
    {
        std::string sTime = Time();
        std::string sSignature = Sign(secret + sTime + userAgent + "\nescape");
        
        httplib::Headers headers = {
            { "Accepts", "application/json" },
            { "User-Agent", userAgent },
            { "Authorization", "Bearer " + sTime + "." + sSignature }
        };

        httplib::Client client(host.c_str());
        client.enable_server_certificate_verification(true);

        auto response = client.Post(uri.c_str(), headers, body.c_str(), body.size(), "application/json");
        return response->body;
    }
#endif

    std::string WebApiClient::Sign(const std::string& input)
    {
        vendor::SHA1 sha1;
        return sha1(input);
    }

    std::string WebApiClient::Time()
    {
        return std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count() / 1000);
    }
} // mwa



#endif

#endif