#include <esp32-modules/core/low-power/DeepSleep.hpp>

// Platform header
#include <esp_sleep.h>

using namespace Esp32Modules::Core;

namespace
{
constexpr uint32_t SECONDS_TO_US_FACTOR{1000000};
}  // namespace

void LowPower::DeepSleepFor(const std::chrono::seconds duration)
{
  esp_sleep_enable_timer_wakeup(duration.count() * SECONDS_TO_US_FACTOR);
  esp_deep_sleep_start();
}