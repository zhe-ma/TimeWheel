#include <iostream>
#include <thread>

#include "time_wheel_scheduler.h"

int main() {
  TimeWheelScheduler tws;

  std::thread t([&](){
    tws.Run();
  });

  tws.RunAt(GetNowTimestamp() + 10000, []() {
    std::cout << "At now+10s" <<std::endl;
  });

  tws.RunAfter(500, []() {
    std::cout << "After 0.5s" <<std::endl;
  });

  tws.RunEvery(5000, []() {
    std::cout << "Every 5s" <<std::endl;
  });

  tws.RunEvery(30 * 1000, []() {
    std::cout << "Every 30s" <<std::endl;
  });

  tws.RunEvery(60 * 1000, []() {
    std::cout << "Every 1m" <<std::endl;
  });

  t.join();

  return 0;
}