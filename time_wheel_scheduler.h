#ifndef TIME_WHEEL_SCHEDULER_H_
#define TIME_WHEEL_SCHEDULER_H_

#include <mutex>
#include <vector>
#include <thread>
#include <unordered_set>

#include "time_wheel.h"

class TimeWheelScheduler {
public:
  explicit TimeWheelScheduler(uint32_t timer_step_ms = 50);

  // Return timer id. Return 0 if the timer creation fails.
  uint32_t CreateTimerAt(int64_t when_ms, const TimerTask& task);
  uint32_t CreateTimerAfter(int64_t delay_ms, const TimerTask& task);
  uint32_t CreateTimerEvery(int64_t interval_ms, const TimerTask& task);

  void CancelTimer(uint32_t timer_id);

  bool Start();
  void Stop();

  void AppendTimeWheel(uint32_t scales, uint32_t scale_unit_ms, const std::string& name = "");

private:
  void Run();

  TimeWheelPtr GetGreatestTimeWheel();
  TimeWheelPtr GetLeastTimeWheel();

private:
  std::mutex mutex_;
  std::thread thread_;

  bool stop_;

  std::unordered_set<uint32_t> cancel_timer_ids_;

  uint32_t timer_step_ms_;
  std::vector<TimeWheelPtr> time_wheels_;
};

#endif  // TIME_WHEEL_SCHEDULER_H_
