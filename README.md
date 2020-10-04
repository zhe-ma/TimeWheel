## TimeWheel

- C++11 implementation of hierarchy time wheel

- Customizable time wheel.

- Customizable timer frequency(ms).

## Build

```bash
$ mkdir build

$ cd build

$ cmake -DCMAKE_BUILD_TYPE=debug ..

$ make -j4

$ ./build/timw-wheel
```

### Example

```cpp
int main() {
  // Four level time wheels: Hour, Minute, Secon, Millisecond.
  int timer_step_ms = 50;
  TimeWheelScheduler tws(timer_step_ms);
  // Hour time wheel. 24 scales, 1 scale = 60 * 60 * 1000 ms.
  tws.AppendTimeWheel(24, 60 * 60 * 1000, "HourTimeWheel");
  // Minute time wheel. 60 scales, 1 scale = 60 * 1000 ms.
  tws.AppendTimeWheel(60, 60 * 1000, "MinuteTimeWheel");
  // Second time wheel. 60 scales, 1 scale = 1000 ms.
  tws.AppendTimeWheel(60, 1000, "SecondTimeWheel");
  // Millisecond time wheel. 1000/timer_step_ms scales, 1 scale = timer_step ms.
  tws.AppendTimeWheel(1000 / timer_step_ms, timer_step_ms, "MillisecondTimeWheel");

  tws.Start();

  tws.CreateTimerAt(GetNowTimestamp() + 10000, []() {
    std::cout << "At now+10s" << std::endl;
  });

  tws.CreateTimerAfter(500, []() {
    std::cout << "After 0.5s" << std::endl;
  });

  std::cout << timetoStr() << std::endl;
  auto timer_id = tws.CreateTimerEvery(5000, []() {
    std::cout << "Every 5s: " << timetoStr() << std::endl;
  });

  tws.CreateTimerEvery(30 * 1000, []() {
    std::cout << "Every 30s: " << timetoStr() <<std::endl;
  });

  tws.CancelTimer(timer_id);

  std::this_thread::sleep_for(std::chrono::minutes(20));
  tws.Stop();

  return 0;
}
```

## Note

- The timer step should be greater than `50ms`. 

- The tolerance is equal to the timer step.

- `Start()` will start a thread to run the time wheel scheduler.
