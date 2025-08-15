//
// Created by lhy on 24-9-19.
//
#include "co.h"
using namespace lhy;
Task<std::string> baby() {
  debug(), "baby";
  co_return "aaa\n";
}

Task<double> world() {
  debug(), "world";
  co_return 3.14;
}

Task<int> hello() {
  auto ret = co_await baby();
  debug(), ret;
  int i = (int)co_await world();
  debug(), "hello得到world结果为", i;
  co_return i + 1;
}

int main() {
  debug(), "main即将调用hello";
  auto t = hello();
  debug(), "main调用完了hello";  // 其实只创建了task对象，并没有真正开始执行
  while (!t.handle_.done()) {
    t.handle_.resume();
    debug(), "main得到hello结果为",
        t.handle_.promise().getorthrow();
  }
  return 0;
}