/**
 * @file SimpleLed.hpp
 * @author Joschka Seydell (joschka@seydell.org)
 * @brief Provides a class representing a simple LED directly wired to the
 * board.
 *
 * The simplest setup is to connect the anode (longer leg) of the LED to a GPIO
 * pin and the kathode in row with a 230-500 ohm resistor to ground.
 *
 * @version 0.1
 * @date 2021-02-07
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef ARDUINOMODULES__ACTUATORS_LED_SIMPLELED_HPP_
#define ARDUINOMODULES__ACTUATORS_LED_SIMPLELED_HPP_

// Standard header
#include <cstdint>

// Project header
#include <esp32-modules/core/scheduling/CooperativeScheduler.hpp>

namespace Esp32Modules::Actuators::LED
{

/**
 * @brief Represents a single, simple LED directly connected to the ESP32.
 */
class SimpleLed
{
 public:
  /**
   * @brief Setup the board pin for controlling the LED.
   *
   * @param pin Hardware pin connected to the LED.
   * @param scheduler Scheduler necessary for blinking the LED - if not provided, only one-shot
   * functionalities (On(), Off(), Toggle()) are available.
   */
  SimpleLed(const uint8_t pin, Core::Scheduling::CooperativeScheduler* scheduler = nullptr);
  ~SimpleLed() = default;

  /**
   * @brief Switch the LED on.
   */
  void On();

  /**
   * @brief Switch the LED off.
   */
  void Off();

  /**
   * @brief Toggle the LED off if it was on or on if it was off.
   */
  void Toggle();

  /**
   * @brief Starts an async task to blink the LED in the specified interval.
   *
   * @note The on and off cycles are symmetric, i.e. of the same duration.
   *
   * @param interval Duration of one on/off cycle. Use the helpers provided by the TaskScheduler
   * library to determine the order of magnitudes (TASK_MILLISECOND, TASK_SECOND, TASK_MINUTE,
   * TASK_HOUR).
   */
  void StartBlink(const Core::Scheduling::TaskDuration interval);

  /**
   * @brief Stop any running blinking task.
   */
  void Reset();

 private:
  const uint8_t mPin;
  Core::Scheduling::CooperativeScheduler* mScheduler;
  bool mIsOn;
  Core::Scheduling::TaskId mBlinkTask;
};
}  // namespace Esp32Modules::Actuators::LED

#endif  // ARDUINOMODULES__ACTUATORS_LED_SIMPLELED_HPP_