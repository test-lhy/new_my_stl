//
// Created by lhy on 24-8-14.
//
#include "array.h"

#include <cassert>
#include <iostream>

using namespace lhy;

void testElementAccess() {
  array<int, 5> arr = {1, 2, 3, 4, 5};
  // 测试 at() 方法
  assert(arr.at(0) == 1);
  assert(arr.at(4) == 5);
  try {
    arr.at(5);  // 应该抛出异常
    assert(false);
  } catch (const std::range_error& e) {
    assert(true);
  }

  // 测试 operator[] 方法
  assert(arr[0] == 1);
  assert(arr[4] == 5);

  // 测试 front() 和 back()
  assert(arr.front() == 1);
  assert(arr.back() == 5);

  // 测试 data()
  assert(arr.data() == &arr[0]);
}

void testIterators() {
  array<int, 5> arr = {1, 2, 3, 4, 5};
  // 测试正向迭代器
  auto it = arr.begin();
  assert(*it == 1);
  ++it;
  assert(*it == 2);

  // 测试反向迭代器
  auto rit = arr.rbegin();
  assert(*rit == 5);
  ++rit;
  assert(*rit == 4);

  // 测试常量迭代器
  const array<int, 5> constArr = {1, 2, 3, 4, 5};
  auto cit = constArr.cbegin();
  assert(*cit == 1);

  auto crit = constArr.crbegin();
  assert(*crit == 5);
}

void testCapacity() {
  array<int, 5> arr = {1, 2, 3, 4, 5};
  // 测试 empty()
  assert(!arr.empty());

  // 测试 size() 和 max_size()
  assert(arr.size() == 5);
  assert(arr.max_size() == 5);

  // 测试零大小数组
  array<int, 0> emptyArr;
  assert(emptyArr.empty());
  assert(emptyArr.size() == 0);
  assert(emptyArr.max_size() == 0);
}

void testOperations() {
  array<int, 5> arr;
  // 测试 fill() 方法
  arr.fill(10);
  for (const auto& elem : arr) {
    assert(elem == 10);
  }

  // 测试 swap() 方法
  array<int, 5> otherArr = {1, 2, 3, 4, 5};
  arr.swap(otherArr);
  assert(arr[0] == 1 && otherArr[0] == 10);
}

void testGetAndTupleSize() {
  array<int, 3> arr = {10, 20, 30};
  // 测试 get()
  assert(get<0>(arr) == 10);
  assert(get<1>(arr) == 20);
  assert(get<2>(arr) == 30);

  // 测试 tuple_size
  static_assert(tuple_size_v<array<int, 3>> == 3, "tuple_size is incorrect");
}

void testArrayZeroSize() {
  array<int, 0> emptyArr;
  // 测试零大小数组的各种操作
  assert(emptyArr.empty());
  assert(emptyArr.data() == nullptr);
  assert(emptyArr.begin() == nullptr);
  assert(emptyArr.end() == nullptr);
  assert(emptyArr.rbegin() == nullptr);
  assert(emptyArr.rend() == nullptr);
}

int main() {
  testElementAccess();
  testIterators();
  testCapacity();
    testOperations();
    testGetAndTupleSize();
    testArrayZeroSize();

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
