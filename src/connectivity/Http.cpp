#include "esp32-modules/connectivity/Http.hpp"

namespace Esp32Modules::Connectivity::Http
{

HttpClient::HttpClient() : mClient{} {}

HttpClient::~HttpClient()
{
  // Make sure, no active connection is left.
  mClient.end();
}

int HttpClient::Get(const std::string url, std::string& result)
{
  mClient.begin(url.c_str());
  const auto responseCode = mClient.GET();
  if (responseCode > 0)
  {
    const auto raw = mClient.getString();
    result = {raw.begin(), raw.end()};
  }
  mClient.end();
  return responseCode;
}

HttpServer::HttpServer(const uint16_t port) : mServer{port} {}

HttpServer::~HttpServer() {}

void HttpServer::SetCallback(const std::string& path, const WebRequestMethod methodType,
                             const OnRequestWithoutParams& cb)
{
  mServer.on(path.c_str(), methodType, [cb](AsyncWebServerRequest* request) {
    const auto response = cb();
    request->send_P(200, "text/plain", response.c_str());
  });
}

}  // namespace Esp32Modules::Connectivity::Http