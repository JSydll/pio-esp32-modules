/**
 * @file Algorithm.hpp
 * @author Joschka Seydell (joschka@seydell.org)
 * @brief Provides algorithms for processing time and to derive further information from it.
 * @version 0.1
 * @date 2021-06-06
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef ESP32MODULES__CORE_TIME_ALGORITHM_HPP_
#define ESP32MODULES__CORE_TIME_ALGORITHM_HPP_

// Standard header
#include <ctime>

namespace Esp32Modules::Core::Time::Algorithm
{
/**
 * @brief Approximates when the sunrise will begin for a given day.
 *
 * @param currentTime Current time including the day of the year.
 * @param averageMinuteFromMidnight Average time on which sunrise occurs at the location chosen by
 * the caller (expressed in minutes from midnight).
 * @param diffMinutesEarliestLatest Difference between the earliest and the latest time
 * of the sunrise (in minutes)
 * @return Time structure with the same properties as the @p currentTime but with the time set to
 * the approximated sunrise.
 */
tm GetSunriseTime(const tm& currentTime, const uint16_t averageMinuteFromMidnight,
                  const uint16_t diffMinutesEarliestLatest);

/**
 * @brief Approximates when the sunrise will begin for a given day.
 *
 * @param currentTime Current time including the day of the year.
 * @param averageMinuteFromMidnight Average time on which sunset occurs at the location chosen by
 * the caller (expressed in minutes from midnight).
 * @param diffMinutesEarliestLatest Difference between the earliest and the latest time
 * of the sunset (in minutes)
 * @return Time structure with the same properties as the @p currentTime but with the time set to
 * the approximated sunset.
 */
tm GetSunsetTime(const tm& currentTime, const uint16_t averageMinuteFromMidnight,
                 const uint16_t diffMinutesEarliestLatest);
}  // namespace Esp32Modules::Core::Time::Algorithm

#endif  // ESP32MODULES__CORE_TIME_ALGORITHM_HPP_