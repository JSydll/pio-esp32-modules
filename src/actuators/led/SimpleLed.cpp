#include "esp32-modules/actuators/led/SimpleLed.hpp"

// Platform header
#include <Arduino.h>

using namespace Esp32Modules::Actuators::LED;
using namespace Esp32Modules::Core::Scheduling;

SimpleLed::SimpleLed(const uint8_t pin, CooperativeScheduler* scheduler)
    : mPin{pin}, mScheduler{scheduler}, mIsOn{false}, mBlinkTask{INVALID_TASKID}
{
  pinMode(mPin, OUTPUT);
}

void SimpleLed::On()
{
  digitalWrite(mPin, HIGH);
  mIsOn = true;
}

void SimpleLed::Off()
{
  digitalWrite(mPin, LOW);
  mIsOn = false;
}

void SimpleLed::Toggle() { (mIsOn ? Off() : On()); }

void SimpleLed::StartBlink(const TaskDuration interval)
{
  if (not mScheduler)
  {
    return;  // Blinking is not supported if the SimpleLed is used without scheduling.
  }
  if (mBlinkTask != INVALID_TASKID)
  {
    Reset();
  }
  mBlinkTask = mScheduler->AddCyclicTask(interval, [this]() { Toggle(); });
}

void SimpleLed::Reset()
{
  if (not mScheduler)
  {
    return;  // Nothing to do if the scheduler is not available.
  }
  mScheduler->AbortTask(mBlinkTask);
  mBlinkTask = INVALID_TASKID;
}