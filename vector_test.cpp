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

  /*cout << flush 的作用是强制清空输出缓冲区，使得所有缓冲区中的数据立即被输出到屏幕上，而不是等待缓冲区满或者遇到 endl 才输出。这在需要确保某些信息立即显示给用户时很有用，例如在等待用户输入之前需要先显示某些提示信息。
   *如果你只是想要确保缓冲区被刷新而不需要换行，应该使用 flush。如果你同时需要换行和刷新缓冲区，使用 endl 更为方便。不过，由于 endl 会导致缓冲区刷新，这可能会带来一些性能开销，因此在不需要换行的情况下，使用 flush 可能是更好的选择。*/
  std::cout<<std::flush;
  // 测试默认构造
  assert(vec.empty());

  //assert 是一个宏，用于在程序中插入断言。断言是一种检查条件是否为真的语句，如果条件为假，assert 会触发一个错误，通常会导致程序终止。

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
  std::less<int>();
  sort(vec.begin(),vec.end(),std::less<int>());
  return 0;
}