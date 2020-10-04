#include <iostream>
#include <thread>

#include "time_wheel_scheduler.h"

int main() {
  TimeWheelScheduler tws;

  tws.Start();

  tws.CreateTimerAt(GetNowTimestamp() + 10000, []() {
    std::cout << "At now+10s" <<std::endl;
  });

  tws.CreateTimerAfter(500, []() {
    std::cout << "After 0.5s" <<std::endl;
  });

  auto timer_id = tws.CreateTimerEvery(5000, []() {
    std::cout << "Every 5s" <<std::endl;
  });

  tws.CreateTimerEvery(30 * 1000, []() {
    std::cout << "Every 30s" <<std::endl;
  });

  tws.CreateTimerEvery(60 * 1000, []() {
    std::cout << "Every 1m" <<std::endl;
  });

  std::this_thread::sleep_for(std::chrono::seconds(30));
  tws.CancelTimer(timer_id);

  std::this_thread::sleep_for(std::chrono::minutes(10));
  tws.Stop();

  return 0;
}