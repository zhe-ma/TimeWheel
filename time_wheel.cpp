#include "time_wheel.h"

TimeWheel::TimeWheel(uint32_t scales, uint32_t scale_unit_ms, const std::string& name)
    : name_(name)
    , current_index_(0)
    , scales_(scales)
    , scale_unit_ms_(scale_unit_ms)
    , slots_(scales)
    , greater_level_tw_(nullptr)
    , less_level_tw_(nullptr) {
}

int64_t TimeWheel::GetCurrentTime() const {
  int64_t time = current_index_ * scale_unit_ms_;
  if (less_level_tw_ != nullptr) {
    time += less_level_tw_->GetCurrentTime();
  }

  return time;
}

void TimeWheel::AddTimer(TimerPtr timer) {
  int64_t less_tw_time = 0;
  if (less_level_tw_ != nullptr) {
    less_tw_time = less_level_tw_->GetCurrentTime();
  }
  int64_t diff = timer->when_ms() + less_tw_time - GetNowTimestamp();

  if (diff >= scale_unit_ms_) {
    size_t n = (current_index_ + diff / scale_unit_ms_) % scales_;
    slots_[n].push_back(timer);
    return;
  }

  if (less_level_tw_ == nullptr) {
    slots_[current_index_].push_back(timer);
    return;
  }

  less_level_tw_->AddTimer(timer);
}

void TimeWheel::Increase() {
  ++current_index_;
  if (current_index_ < scales_) {
    return;
  }

  current_index_ = current_index_ % scales_;
  if (greater_level_tw_ != nullptr) {
    greater_level_tw_->Increase();
    std::list<TimerPtr> slot = std::move(greater_level_tw_->GetAndClearCurrentSlot());
    for (TimerPtr timer : slot) {
      AddTimer(timer);
    }
  }
}

std::list<TimerPtr> TimeWheel::GetAndClearCurrentSlot() {
  std::list<TimerPtr> slot;
  slot = std::move(slots_[current_index_]);
  return slot;
}
