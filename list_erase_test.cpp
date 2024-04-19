//
// Created by lhy31 on 2023/8/21.
//
#include <cassert>
#include <iostream>
#include <stdexcept>

#include "list.h"
#include "vector.h"
using namespace lhy;
int main() {
  list<int> a = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  auto now = a.begin();
  auto second = a.erase(now);
  a.erase(second);
  for (auto& each : a) {
    std::cout << each;
  }
  std::cout << std::endl;
  auto now1 = a.rbegin();
  auto second1 = a.erase(now1);
  a.erase(second1);
  for (auto& each : a) {
    std::cout << each;
  }
  std::cout << std::endl;
  return 0;
}