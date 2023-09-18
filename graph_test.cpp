//
// Created by lhy31 on 2023/9/4.
//
#include "graph.h"
#include <iostream>
#include<vector>
int main() {
  lhy::Graph<int> g;

  // 测试 AddNode,预期节点数为1
  g.AddNode(0);
  std::cout << "Expect node count: 1, actual: " << g.size() << std::endl;

  // 测试 AddNode,预期节点数为2
  g.AddNode(1);
  std::cout << "Expect node count: 2, actual: " << g.size() << std::endl;

  // 测试预留空间的 AddNode,预期节点数为3
  g.AddNode(2, 10);
  std::cout << "Expect node count: 3, actual: " << g.size() << std::endl;

  // 测试边的添加
  g.AddEdge(0, 2);
  g.AddEdge(1, 2);
  g.AddNode(2, 100);
  // 测试清空,预期节点数为0
  g.clear();
  std::cout << "Expect node count: 0, actual: " << g.size() << std::endl;

  // 测试节点的插入,预期节点数为2
  g.AddNode(0);
  g.AddNode(1, 0);
  g.AddNode(2);
  std::cout << "Expect node count: 2, actual: " << g.size() << std::endl;

  // 测试边的添加
  g.AddEdge(0, 2);
  g.AddEdge(1, 2);

  // 测试删除节点,预期节点数为1
  g.EraseNode(0);
  std::cout << "Expect node count: 1, actual: " << g.size() << std::endl;

  // 测试节点1的边数,预期为1条
  std::cout << "Expect node 1 edges: 1, actual: " << g[1].next_.size() << std::endl;

  return 0;
}