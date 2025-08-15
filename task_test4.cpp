//
// Created by lhy on 24-9-19.
//
#include <chrono>

#include "co.h"
using std::chrono_literals::operator""s;
using namespace lhy;
Task<int> hello1() {
  debug(), "hello1开始睡5秒";
  co_await sleep_for(2s);  // 1s 等价于 std::chrono::seconds(1)
  debug(), "hello1睡醒了", (std::chrono::steady_clock::now() - st_time) / 1s;
  co_return 1;
}

Task<int> hello2() {
  debug(), "hello2开始睡6秒";
  auto ret = co_await sleep_for(3s);  // 2s 等价于 std::chrono::seconds(2)
  debug(), "hello2睡醒了", (std::chrono::steady_clock::now() - st_time) / 1s;
  co_return 2;
}
Task<int> hello() {
  auto ret = co_await hello1();
  co_return std::move(ret);
}
int main() {
  auto t1 = hello1();
  auto t2 = hello2();
  auto t = hello();
  GetLoop().AddTask(t1);
  GetLoop().AddTask(t2);
  GetLoop().RunAll();
  debug(), "主函数中得到hello1结果:", t1.handle_.promise().getorthrow();
  return 0;
}