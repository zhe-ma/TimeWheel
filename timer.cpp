#include "timer.h"

Timer::Timer(uint32_t id, int64_t when_ms, uint32_t interval_ms, const TimerTask& handler)
    : interval_ms_(interval_ms)
    , repeated_(interval_ms_ > 0)
    , when_ms_(when_ms)
    , id_(id)
    , handler_(handler) {
}

void Timer::Run() {
  if (handler_) {
    handler_();
  }
}
