#include "tree.h"
#include <iostream>

int main() {

  lhy::tree<int> t;

  // 添加节点
  t.AddNode(0, 10); // root node

  t.AddNode(0, 20, 1);
  t.AddNode(0, 30, 2);

  t.AddNode(1, 40, 3);
  t.AddNode(1, 50, 4);

  t.AddNode(2, 60, 5);
  t.AddNode(2, 70, 6);

  // 设置根节点
  t.set_root(0);

  // 打印根节点
  std::cout << t[t.get_root()].content_ << std::endl; // 10

  // 打印子节点
  std::cout << t[1].content_ << std::endl; // 20

  // 打印叶子节点
  std::cout << t[4].content_ << std::endl; // 50

  // 获取大小
  std::cout << t.size() << std::endl; // 7

  // 删除节点
  t.EraseNode(5);

  // 大小
  std::cout << t.size() << std::endl; // 6

}