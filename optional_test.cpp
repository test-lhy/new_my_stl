//
// Created by lhy on 24-8-18.
//
#include "optional.h"

#include <cassert>
#include <iostream>
#include <string>

#include "strings.h"
#include "strings_.h"
#include "vector.h"
using namespace lhy;
void test_constructors() {
  // 默认构造函数
  lhy::optional<int> opt1;
  assert(!opt1.has_value());

  // nullopt_t 构造函数
  lhy::optional<int> opt2(lhy::nullopt);
  assert(!opt2.has_value());

  // 从值构造
  lhy::optional<int> opt3(10);
  assert(opt3.has_value() && *opt3 == 10);

  // in_place_t 构造函数
  lhy::optional<string> opt4(lhy::in_place, "Hello");
  assert(opt4.has_value() && *opt4 == "Hello");

  // 拷贝构造函数
  lhy::optional<int> opt5(opt3);
  assert(opt5.has_value() && *opt5 == 10);

  // 移动构造函数
  lhy::optional<int> opt6(std::move(opt5));
  assert(opt6.has_value() && *opt6 == 10);
  assert(!opt5.has_value());
}

void test_assignment() {
  lhy::optional<int> opt;
  opt = 5;
  assert(opt.has_value() && *opt == 5);

  opt = lhy::nullopt;
  assert(!opt.has_value());

  lhy::optional<int> opt2(10);
  opt = opt2;
  assert(opt.has_value() && *opt == 10);

  opt = std::move(opt2);
  assert(opt.has_value() && *opt == 10);
  assert(!opt2.has_value());
}

void test_access() {
  lhy::optional<int> opt(42);
  assert(opt.has_value());
  assert(opt.value() == 42);
  assert(opt.value_or(0) == 42);

  opt = lhy::nullopt;
  try {
    opt.value();
    assert(false);  // Should not reach here
  } catch (const exception& e) {
    assert(true);  // Exception should be thrown
  }
  assert(opt.value_or(100) == 100);
}

void test_functions() {
  lhy::optional<int> opt(7);
  auto opt2 = opt.and_then([](int x) { return x * 2; });
  assert(opt2 == 14);

  auto opt3 = opt.transform([](int x) { return int(x); });
  assert(opt3.has_value() && *opt3 == 7);

  lhy::optional<int> opt4;
  auto opt5 = opt4.or_else([]() { return lhy::optional<int>(8); });
  assert(opt5.has_value() && *opt5 == 8);
}

void test_iterators() {
  lhy::optional<vector<int>> opt(vector<int>{1, 2, 3});
  assert(opt.has_value());

  auto it = opt.begin();
  assert(it != opt.end() && *it == 1);
}

void test_swap_reset_emplace() {
  lhy::optional<int> opt1(5), opt2(10);
  swap(opt1, opt2);
  assert(opt1.has_value() && *opt1 == 10);
  assert(opt2.has_value() && *opt2 == 5);

  opt1.reset();
  assert(!opt1.has_value());

  opt1.emplace(20);
  assert(opt1.has_value() && *opt1 == 20);
}

int main() {
  test_constructors();
  test_assignment();
  test_access();
  test_functions();
  test_iterators();
  test_swap_reset_emplace();

  std::cout << "All tests passed!\n";
  return 0;
}
