#include "esp32-modules/core/time/NtpClient.hpp"

// Platform includes
#include <Arduino.h>  // Necessary for configTime and getLocalTime

namespace Esp32Modules::Core::Time
{
namespace
{
// UTC+1 for Berlin, Rome, Paris, Madrid...
constexpr uint32_t TZ_OFFSET_CET{3600};
// Set this to 0 if no daylight time switch is performed in this country
constexpr uint32_t DAYLIGHT_OFFSET{3600};
constexpr int SANITY_CHECK_YEAR{2020};
constexpr int TM_YEAR_BASE{1900};
}  // namespace

NtpClient::NtpClient(const std::string& serverUrl) : mServerUrl{serverUrl}
{
  configTime(TZ_OFFSET_CET, DAYLIGHT_OFFSET, mServerUrl.c_str(), DEFAULT_NTP_SERVER.c_str());
}

NtpClient::TimeInfo NtpClient::Now()
{
  TimeInfo info{false, {}};
  if (!getLocalTime(&info.data))
  {
    return info;
  }
  info.isValid = (info.data.tm_year > (SANITY_CHECK_YEAR - TM_YEAR_BASE));
  return info;
}

}  // namespace Esp32Modules::Core::Time