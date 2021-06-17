#include "esp32-modules/core/time/Algorithm.hpp"

// Standard header
#include <cmath>

namespace Esp32Modules::Core::Time
{
namespace
{
constexpr uint16_t DOY_OFFSET{8};                     // 8 days between 23.12. and 1.1.
constexpr float COSINE_OF_YEAR_NORMALIZATION{58.09};  // 365/2*pi

/**
 * Common implementation for estimating sunrise / sunset times.
 *
 * Approximation taken from
 * https://www.instructables.com/Calculating-Sunset-and-Sunrise-for-a-Microcontroll/. Assumes a
 * cosinoidal curve for the sunrise / sunset and adjusts it to oscillate around the average observed
 * time from the earliest to latest observations.
 */
tm ApproximatedSuntime(const tm& currentTime, const uint16_t average, const uint16_t diff,
                       const bool isSunrise)
{
  const auto doy = currentTime.tm_yday + 1;
  const float approx = average + (isSunrise ? 0.5 : -0.5) * diff *
                                     cos((doy + DOY_OFFSET) / COSINE_OF_YEAR_NORMALIZATION);
  // Overwrite the calculated time for sunrise/sunset
  tm calculatedTime{currentTime};
  const uint16_t hours = static_cast<uint16_t>(approx / 60);
  calculatedTime.tm_hour = hours + (currentTime.tm_isdst ? 1 : 0);
  calculatedTime.tm_min = static_cast<uint16_t>(approx - hours * 60);
  calculatedTime.tm_sec = 0;
  return calculatedTime;
}
}  // namespace

tm Algorithm::GetSunriseTime(const tm& currentTime, const uint16_t averageMinuteFromMidnight,
                               const uint16_t diffMinutesEarliestLatest)
{
  return ApproximatedSuntime(currentTime, averageMinuteFromMidnight,
                             diffMinutesEarliestLatest, true);
}

tm Algorithm::GetSunsetTime(const tm& currentTime, const uint16_t averageMinuteFromMidnight,
                              const uint16_t diffMinutesEarliestLatest)
{
  return ApproximatedSuntime(currentTime, averageMinuteFromMidnight,
                             diffMinutesEarliestLatest, false);
}
}  // namespace Esp32Modules::Core::Time