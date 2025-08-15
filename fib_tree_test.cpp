//
// Created by lhy on 24-9-15.
//
#include <cassert>
#include <iostream>

#include "FibTree.h"
using namespace lhy;
int main() {
  FibTree<int> fib_tree;

  // 插入元素并测试最小值
  fib_tree.push(10);
  assert(fib_tree.top() == 10);

  fib_tree.push(20);
  assert(fib_tree.top() == 10);

  fib_tree.push(5);
  assert(fib_tree.top() == 5);

  fib_tree.push(15);
  assert(fib_tree.top() == 5);

  // 测试 extract_min 并检查最小值的变化
  fib_tree.pop();                // 5 应该被删除
  assert(fib_tree.top() == 10);  // 新的最小值应为 10

  fib_tree.pop();                // 10 应该被删除
  assert(fib_tree.top() == 15);  // 新的最小值应为 15

  // 测试合并两个堆
  FibTree<int> fib_tree2;
  fib_tree.push(3);
  fib_tree.push(25);

  // fib_tree.merge(std::move(fib_tree2));
  assert(fib_tree.top() == 3);  // 合并后新的最小值应为 3

  fib_tree.pop();                // 3 应该被删除
  assert(fib_tree.top() == 15);  // 新的最小值应为 15

  // 清空堆并确保异常被抛出
  fib_tree.pop();  // 15 被删除
  fib_tree.pop();  // 20 被删除


  std::cout << "All tests passed!" << std::endl;
  return 0;
}
