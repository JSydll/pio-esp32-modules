#include "esp32-modules/core/time/NtpClient.hpp"

// Platform includes
#include <Arduino.h> // Necessary for configTime and getLocalTime

namespace Esp32Modules::Core::Time
{
namespace
{
// UTC+1 for Berlin, Rome, Paris, Madrid...
constexpr uint32_t TZ_OFFSET_CET{3600};
// Set this to 0 if no daylight time switch is performed in this country
constexpr uint32_t DAYLIGHT_OFFSET{3600};
}  // namespace

NtpClient::NtpClient(const std::string& serverUrl)
{
  configTime(TZ_OFFSET_CET, DAYLIGHT_OFFSET, serverUrl.c_str());
}

NtpClient::TimeInfo NtpClient::Now()
{
  TimeInfo info;
  if (!getLocalTime(&info.data))
  {
    return info;
  }
  info.isValid = true;
  return info;
}

}  // namespace Esp32Modules::Core::Time