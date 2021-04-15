/**
 * @file Http.hpp
 * @author Joschka Seydell (joschka@seydell.org)
 * @brief Provides classes to communicate via the HTTP protocol.
 * @version 0.1
 * @date 2021-04-13
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef ESP32MODULES__CONNECTIVITY_HTTP_HPP_
#define ESP32MODULES__CONNECTIVITY_HTTP_HPP_

// Standard header
#include <cstdint>
#include <functional>
#include <string>

// Platform header
#include <HTTPClient.h>

// Third-party header
#include <ESPAsyncWebServer.h>

namespace Esp32Modules::Connectivity::Http
{
/**
 * @brief Provides methods to issue HTTP requests towards an existing server.
 */
class HttpClient
{
 public:
  /**
   * @brief Sets up the client for sending HTTP request.
   *
   * @note Assumes an already established network.
   */
  HttpClient();

  /**
   * @brief Cleans up all resources.
   */
  ~HttpClient();

  /**
   * @brief Send an HTTP GET request to the specified URL.
   *
   * @param url URL to be queried.
   * @param result Result string obtained from the URL.
   * @return HTTP response code (below zero if an error occurred on client side).
   */
  int Get(const std::string url, std::string& result);

 private:
  HTTPClient mClient;
};

class HttpServer
{
 public:
  HttpServer(const uint16_t port);

  ~HttpServer();

  /**
   * @brief Represents the callback to be executed on an HTTP request without payload.
   *
   * Function signature explanation:
   *   Parameters: None.
   *   Return value: Serialized response.
   */
  using OnRequestWithoutParams = std::function<std::string()>;

  /**
   * @brief Registers a callback to be executed on a void HTTP request with the given path.
   *
   * @param path Path on the parent URL used to identify the request.
   * @param methodType One of HTTP_GET, HTTP_POST, HTTP_PUT or HTTP_DELETE.
   * @param cb Callback to be executed.
   */
  void SetCallback(const std::string& path, const WebRequestMethod methodType,
                   const OnRequestWithoutParams& cb);

 private:
  AsyncWebServer mServer;
};

}  // namespace Esp32Modules::Connectivity::Http

#endif  // ESP32MODULES__CONNECTIVITY_HTTP_HPP_