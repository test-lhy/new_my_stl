//
// Created by lhy on 24-1-10.
//

#include "directed_graph.h"
#include "undirected_graph.h"
#include "weighted.h"
using namespace lhy;

int main() {
  clock_t start, end;  // 定义clock_t变量
  start = clock();     // 开始时间
  for (int i = 0; i < 100000; i++) {
    int* a = new int();
    delete a;
  }
  end = clock();                                                                       // 开始时间
  std::cout << "time = " << double(end - start) / CLOCKS_PER_SEC << "s" << std::endl;  // 输出时间（单位：ｓ）
  start = clock();                                                                     // 开始时间
  int* a = new int[100000](1);
  end = clock();                                                                       // 开始时间
  std::cout << "time = " << double(end - start) / CLOCKS_PER_SEC << "s" << std::endl;  // 输出时间（单位：ｓ）
  delete[] a;
  auto allnodemode = {MapNodeAllocatorMode, NormalNodeAllocatorMode};
  auto alledgemode = {MatrixEdgeAllocatorMode, MapEdgeAllocatorMode, LinkListEdgeAllocatorMode};
  for (auto& eachedgemode : alledgemode) {
    for (auto& eachnodemode : allnodemode) {
      start = clock();  // 开始时间
      BaseGraph<Weighted<int, Node>, Edge> graph(eachnodemode, eachedgemode);

      graph.AddNode(1, 1);
      graph.AddNode(2, 2);
      graph.AddNode(3, 3);
      graph.AddEdge(1, 2);
      graph.AddEdge(1, 3);
      graph.AddEdge(2, 3);
      graph.AddEdge(2, 3);
      graph.AddEdge(3, 3);
      graph.AddEdge(3, 2);
      int n1 = 1;
      int n2 = 2;
      int n3 = 3;
      for (auto& [index, node] : graph) {
        std::cout << "node" << node.GetId() << std::endl;
        for (auto& edge : graph[node.GetId()].GetEdges()) {
          std::cout << edge.GetFirst() << " -> " << edge.GetSecond() << std::endl;
        }
      }
      graph.DeleteEdge(1, 3);
      graph.DeleteNode(n2);
      std::cout << "after detete node 2 and the edge from 1 to 3" << std::endl;

      for (auto& [index, node] : graph) {
        std::cout << "node" << node.GetId() << std::endl;
        for (auto& edge : graph[node.GetId()].GetEdges()) {
          std::cout << edge.GetFirst() << " -> " << edge.GetSecond() << std::endl;
        }
      }
      std::cout << "finished one test" << std::endl;
      end = clock();                                                                       // 结束时间
      std::cout << "time = " << double(end - start) / CLOCKS_PER_SEC << "s" << std::endl;  // 输出时间（单位：ｓ）
    }
  }
}