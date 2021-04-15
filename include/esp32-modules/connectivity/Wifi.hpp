/**
 * @file Wifi.hpp
 * @author Joschka Seydell (joschka@seydell.org)
 * @brief Provides a classes to use the ESP32 wifi module.
 * @version 0.1
 * @date 2021-04-13
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef ESP32MODULES__CONNECTIVITY_WIFI_HPP_
#define ESP32MODULES__CONNECTIVITY_WIFI_HPP_

// Standard header
#include <string>

// Platform header
#include <WiFi.h>

namespace Esp32Modules::Connectivity::Wifi
{
/**
 * @brief Establishes a connection to an existing wifi network.
 */
class WifiConnection
{
 public:
  /**
   * @brief Sets up the client and attempts to establish a connection to the provided network.
   *
   * @param ssid Identifier (name) of the network to connect to.
   * @param password Password to be used (may be empty for unsecured networks).
   */
  WifiConnection(const std::string& ssid, const std::string& password);

  /**
   * @brief Cleans up all wifi resources.
   */
  ~WifiConnection();

  /**
   * @brief Indicates whether the wifi client is connected.
   *
   * @return true if the client is connected to the network, false if not.
   */
  bool IsConnected() const;
};

/**
 * @brief Sets up the ESP as wifi access point.
 */
class WifiAccessPoint
{
 public:
  /**
   * @brief Sets up the access point.
   *
   * @param ssid Identifier (name) of the network to be visible to other devices.
   * @param password Password used to protect network access.
   */
  WifiAccessPoint(const std::string& ssid, const std::string& password = "");

  /**
   * @brief Tears down the access point.
   */
  ~WifiAccessPoint();

  /**
   * @brief Returns the IP the internal stack allocated for the access point.
   *
   * @return Assigned IP address.
   */
  IPAddress GetAssignedIP() const;

 private:
  IPAddress mIp;
};

}  // namespace Esp32Modules::Connectivity::Wifi

#endif  // ESP32MODULES__CONNECTIVITY_WIFI_HPP_