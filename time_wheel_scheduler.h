#ifndef TIME_WHEEL_SCHEDULER_H_
#define TIME_WHEEL_SCHEDULER_H_

#include <mutex>

#include "time_wheel.h"

class TimeWheelScheduler {
public:
  explicit TimeWheelScheduler(uint32_t timer_step_ms = 50);

  // Return timer id. Return 0 if the timer creation fails.
  uint32_t RunAt(int64_t when_ms, const TimerTask& handler);
  uint32_t RunAfter(uint32_t delay_ms, const TimerTask& handler);
  uint32_t RunEvery(uint32_t interval_ms, const TimerTask& handler);

  void CancelTimer(uint32_t timer_id);

  void Run();

private:
  std::mutex mutex_;
  uint32_t timer_step_ms_;

  TimeWheel millisecond_tw_;  // Level one time wheel. The least scale unit.
  TimeWheel second_tw_;
  TimeWheel minute_tw_;
  TimeWheel hour_tw_;  // Level four time wheel. The greatest scale unit.
};

#endif  // TIME_WHEEL_SCHEDULER_H_
