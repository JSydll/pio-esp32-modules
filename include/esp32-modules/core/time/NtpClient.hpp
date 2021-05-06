/**
 * @file NtpClient.hpp
 * @author Joschka Seydell (joschka@seydell.org)
 * @brief Provides a class for network time synchronization using the NTP protocol.
 * @version 0.1
 * @date 2021-04-13
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef ESP32MODULES__CORE_TIME_NTPCLIENT_HPP_
#define ESP32MODULES__CORE_TIME_NTPCLIENT_HPP_

// Standard header
#include <cstdint>
#include <ctime>
#include <string>

namespace Esp32Modules::Core::Time
{
/** @brief Default to a european NTP server. */
const std::string DEFAULT_NTP_SERVER{"pool.ntp.org"};

/**
 * @brief Provides time synchronization via NTP protocol.
 */
class NtpClient
{
 public:
  /**
   * @brief Setup the NTP client.
   *
   * @note Uses CET as timezone and daylight saving time switch by default.
   *
   * @param serverUrl URL of the NTP master server providing actual time.
   */
  NtpClient(const std::string& serverUrl = DEFAULT_NTP_SERVER);
  ~NtpClient() = default;

  /**
   * @brief Represents all information about the queried time.
   */
  struct TimeInfo
  {
    bool isValid;    //!< Indicates whether the tm struct is valid.
    struct tm data;  //!< Actual time information.
  };

  /**
   * @brief Queries the NTP server for the current time.
   *
   * @return Information about the current time (only valid if the isValid member is true).
   */
  TimeInfo Now();

 private:
  const std::string mServerUrl;  // Persist the server URL as the ctime interfaces do not do that.
};
}  // namespace Esp32Modules::Core::Time

#endif  // ESP32MODULES__CORE_TIME_NTPCLIENT_HPP_