//
// Created by lhy on 24-1-10.
//

#include "directed_graph.h"
#include "undirected_graph.h"
#include "weighted.h"
using namespace lhy;

int main() {
  clock_t start, end;  // 定义clock_t变量
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
      for (auto& edge : graph[n1].edges_) {
        std::cout << edge.GetFirst().GetId() << " -> " << edge.GetSecond().GetId() << std::endl;
      }
      for (auto& edge : graph[n2].edges_) {
        std::cout << edge.GetFirst().GetId() << " -> " << edge.GetSecond().GetId() << std::endl;
      }
      for (auto& edge : graph[n3].edges_) {
        std::cout << edge.GetFirst().GetId() << " -> " << edge.GetSecond().GetId() << std::endl;
      }
      graph.DeleteEdge(graph[1].node_, graph[3].node_);
      std::cout << "after detete node 2" << std::endl;
      graph.DeleteNode(n2);

      for (auto& edge : graph[n1].edges_) {
        std::cout << edge.GetFirst().GetId() << " -> " << edge.GetSecond().GetId() << std::endl;
      }
      try {
        for (auto& edge : graph[n2].edges_) {
          std::cout << edge.GetFirst().GetId() << " -> " << edge.GetSecond().GetId() << std::endl;
        }
      } catch (...) {
        std::cout << "node 2 does not exist" << std::endl;
      }
      for (auto& edge : graph[n3].edges_) {
        std::cout << edge.GetFirst().GetId() << " -> " << edge.GetSecond().GetId() << std::endl;
      }
      std::cout << "finished one test" << std::endl;
      end = clock();                                                                       // 结束时间
      std::cout << "time = " << double(end - start) / CLOCKS_PER_SEC << "s" << std::endl;  // 输出时间（单位：ｓ）
    }
  }
}