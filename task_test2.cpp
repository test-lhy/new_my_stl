//
// Created by lhy on 24-9-19.
//
#include "co.h"
using namespace lhy;
Task<int> world() {
  debug(), "world";
  co_return 41;
}

Task<int> hello() {
  int i = co_await world();
  debug(), "hello得到world结果为", i;
  co_return i + 1;
}

int main() {
  debug(), "main即将调用hello";
  Task t = hello();
  debug(), "main调用完了hello";  // 其实只创建了task对象，并没有真正开始执行
  while (!t.handle_.done()) {
    t.handle_.resume();
    debug(), "main得到hello结果为",
        t.get_value();
  }
  return 0;
}