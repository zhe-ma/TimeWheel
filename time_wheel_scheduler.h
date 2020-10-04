#ifndef TIME_WHEEL_SCHEDULER_H_
#define TIME_WHEEL_SCHEDULER_H_

#include <mutex>
#include <thread>
#include <unordered_set>

#include "time_wheel.h"

class TimeWheelScheduler {
public:
  explicit TimeWheelScheduler(uint32_t timer_step_ms = 50);

  // Return timer id. Return 0 if the timer creation fails.
  uint32_t CreateTimerAt(int64_t when_ms, const TimerTask& handler);
  uint32_t CreateTimerAfter(int64_t delay_ms, const TimerTask& handler);
  uint32_t CreateTimerEvery(int64_t interval_ms, const TimerTask& handler);

  void CancelTimer(uint32_t timer_id);

  void Start();
  void Stop();

private:
  std::mutex mutex_;
  std::thread thread_;

  bool stop_;

  std::unordered_set<uint32_t> cancel_timer_ids_;

  uint32_t timer_step_ms_;

  TimeWheel millisecond_tw_;  // Level one time wheel. The least scale unit.
  TimeWheel second_tw_;
  TimeWheel minute_tw_;
  TimeWheel hour_tw_;  // Level four time wheel. The greatest scale unit.
};

#endif  // TIME_WHEEL_SCHEDULER_H_
