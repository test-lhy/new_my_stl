//
// Created by lhy31 on 2023/8/21.
//
#include "vector.h"

#include <cassert>
#include <iostream>
#include <stdexcept>
using namespace lhy;
int main() {

  vector<int> vec;
  printf("!");
  // 测试默认构造
  assert(vec.empty());

  // 测试插入
  vec.push_back(1);
  vec.push_back(2);

  assert(vec.size() == 2);
  assert(vec[0] == 1);
  assert(vec[1] == 2);



  // 测试清空
  vec.clear();
  assert(vec.empty());

  // 测试初始化列表构造和赋值
  vec = {3, 4, 5};
  assert(vec.size() == 3);
  assert(vec.At(0) == 3);

  // 测试拷贝构造

  // 测试迭代器
  auto it = vec.begin();
  assert(*it == 3);
  ++it;
  assert(*it == 4);

  std::cout << "all test passed" << std::endl;

  return 0;
}