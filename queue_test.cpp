//
// Created by lhy31 on 2023/9/1.
//

#include "queue.h"

#include <iostream>

#include "algorithm.h"
#include "random.h"
#include "vector.h"

using namespace lhy;

int main() {
  vector<int> a{1, 2, 3, 4, 5, 6, 7};
  reverse(a.begin(), a.end());
  std::cout << a << std::endl;

  queue<int> q;

  // 测试插入
  q.push(1);
  q.push(2);
  q.push(3);
  queue<int> q1;
  q1={3,4,5};
  queue<int> q2;
  q2=std::move(q1);
  std::cout << q2.front();
  // 测试front
  std::cout << q.front() << std::endl;  // 应输出1

  // 测试弹出
  q.pop();
  std::cout << q.front() << std::endl;  // 应输出2

  // 测试判空
  std::cout << q.empty() << std::endl;  // 应输出0

  // 清空队列
  q.clear();
  std::cout << q.empty() << std::endl;  // 应输出1

  // 测试拷贝构造

  //std::cout << q1.front() << std::endl;  // 应输出1
  //q1.pop();
  //std::cout<<q1.front()<<std::endl;
  // 测试遍历显示
  q.push(1);
  q.push(2);
  q.push(3);
  //std::cout << q.show(2) << std::endl;  // 应显示前2个元素

  return 0;
}