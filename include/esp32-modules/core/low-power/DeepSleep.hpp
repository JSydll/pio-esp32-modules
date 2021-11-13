/**
 * @file DeepSleep.hpp
 * @author Joschka Seydell (joschka@seydell.org)
 * @brief Provides functions to leverage the ESP's deep sleep functionality.
 * @version 0.1
 * @date 2021-04-13
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef ESP32MODULES__CORE_LOWPOWER_DEEPSLEEP_HPP_
#define ESP32MODULES__CORE_LOWPOWER_DEEPSLEEP_HPP_

// Standard header
#include <chrono>

namespace Esp32Modules::Core::LowPower
{

/**
 * @brief Enters deep sleep and awakes after the specified amount of seconds.
 * 
 * Waking up from deep sleep is essentially like a clean reboot, so expect the setup code to be run.
 * 
 * @param duration Seconds after which the deep sleep mode shall be left again.
 */
void DeepSleepFor(const std::chrono::seconds duration);
}  // namespace Esp32Modules::Core::LowPower

#endif  // ESP32MODULES__CORE_LOWPOWER_DEEPSLEEP_HPP_