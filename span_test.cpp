//
// Created by lhy on 24-8-9.
//
#include "span.h"

#include <vector.h>

#include <cassert>
#include <iostream>

#include "list.h"

// 简单的测试结构体，用于测试 span
struct TestData {
  TestData() = default;
  int value{};
  TestData(int v) : value(v) {}
};
using namespace lhy;
// 测试函数
void runTests() {
  std::vector<TestData> vec = {TestData(1), TestData(2), TestData(3), TestData(4), TestData(5)};

  // 创建 span 对象
  lhy::span<TestData> s1(vec.begin(), vec.end());

  // 测试大小
  assert(s1.size() == vec.size());
  std::cout << "Size test passed.\n";

  // 测试遍历
  int i = 1;
  for (auto& element : s1) {
    assert(element.value == i);
    i++;
  }
  std::cout << "Iteration test passed.\n";

  // 测试索引访问
  assert(s1[0].value == 1);
  assert(s1[4].value == 5);
  std::cout << "Index access test passed.\n";

  // 测试 front 和 back
  assert(s1.front().value == 1);
  assert(s1.back().value == 5);
  std::cout << "Front and back test passed.\n";

  // 测试 subspan
  auto sub = s1.subspan(1, 3);  // 应该是 {2, 3, 4}
  assert(sub.size() == 3);
  assert(sub[0].value == 2);
  assert(sub[2].value == 4);
  std::cout << "Subspan test passed.\n";

  // 测试 first 和 last
  auto firstTwo = s1.first(2);  // 应该是 {1, 2}
  assert(firstTwo.size() == 2);
  assert(firstTwo[0].value == 1);
  assert(firstTwo[1].value == 2);
  std::cout << "First test passed.\n";

  auto lastTwo = s1.last(2);  // 应该是 {4, 5}
  assert(lastTwo.size() == 2);
  assert(lastTwo[0].value == 4);
  assert(lastTwo[1].value == 5);
  std::cout << "Last test passed.\n";

  // 测试空 span
  lhy::span<TestData> emptySpan;
  assert(emptySpan.empty());
  std::cout << "Empty span test passed.\n";
}

int main() {
  try {
    runTests();
    std::cout << "All tests passed successfully!\n";
  } catch (const std::exception& e) {
    std::cerr << "Test failed: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
