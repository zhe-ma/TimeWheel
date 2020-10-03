#include "time_wheel_scheduler.h"

#include <thread>

static uint32_t s_inc_id = 1;

// TODO: Timer_wheel, use level time 1 to add timer. 误差也是timer_step_ms
TimeWheelScheduler::TimeWheelScheduler(uint32_t timer_step_ms)
    : timer_step_ms_(timer_step_ms)
    , millisecond_tw_(1000 / timer_step_ms, timer_step_ms)
    , second_tw_(60, 1000)
    , minute_tw_(60, 60 * 1000)
    , hour_tw_(24, 60 * 60 * 1000) {

  hour_tw_.set_less_level_tw(&minute_tw_);

  minute_tw_.set_less_level_tw(&second_tw_);
  minute_tw_.set_greater_level_tw(&hour_tw_);

  second_tw_.set_less_level_tw(&millisecond_tw_);
  second_tw_.set_greater_level_tw(&minute_tw_);

  millisecond_tw_.set_greater_level_tw(&second_tw_);
}

// TODO: how to exit.
void TimeWheelScheduler::Run() {
  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(timer_step_ms_));
    std::lock_guard<std::mutex> lock(mutex_);
    millisecond_tw_.Increase();
    auto c = millisecond_tw_.GetAndClearCurrentSlot();
    for (auto t : c) {
      t->Run();
      if (t->repeated()) {
        t->UpdateTime();
        hour_tw_.AddTimer(t);
      }
    }
  }
}

uint32_t TimeWheelScheduler::RunAt(int64_t when_ms, const TimerTask& handler) {
  std::lock_guard<std::mutex> lock(mutex_);
  ++s_inc_id;
  hour_tw_.AddTimer(std::make_shared<Timer>(s_inc_id, when_ms, 0, handler));

  return s_inc_id;
}

uint32_t TimeWheelScheduler::RunAfter(uint32_t delay_ms, const TimerTask& handler) {
  int64_t when = GetNowTimestamp() + static_cast<int64_t>(delay_ms);
  return RunAt(when, handler);
}

uint32_t TimeWheelScheduler::RunEvery(uint32_t interval_ms, const TimerTask& handler) {
  std::lock_guard<std::mutex> lock(mutex_);
  ++s_inc_id;
  hour_tw_.AddTimer(std::make_shared<Timer>(s_inc_id, GetNowTimestamp() + interval_ms, interval_ms, handler));

  return s_inc_id;
}

void TimeWheelScheduler::CancelTimer(uint32_t timer_id) {
  std::lock_guard<std::mutex> lock(mutex_);
}
