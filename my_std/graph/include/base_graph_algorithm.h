//
// Created by lhy on 24-4-12.
//

#ifndef BASE_GRAPH_ALGORITHM_H
#define BASE_GRAPH_ALGORITHM_H
#include "base_graph.h"
#include "directed_graph.h"
#include "index_container.h"
#include "priority_queue.h"
namespace lhy {
template <typename EdgeWeightType, node_c Node_>
IndexContainer<double> Dijkstra(DGE<EdgeWeightType, Node_>* graph, Index node) {
  IndexContainer<double> distance(graph->GetNodeContainerMode());
  IndexContainer<bool> book(graph->GetNodeContainerMode());
  distance[node] = 0;
  priority_queue<std::pair<double, Index>> pos_next;
  pos_next.push({0, node});
  while (!pos_next.empty()) {
    auto [dis, now] = pos_next.TopAndPop();
    while (book.Exist(now)) {
      auto nowpair = pos_next.TopAndPop();
      dis = nowpair.first;
      now = nowpair.second;
    }
    book[now] = true;
    for (auto& each : graph->operator[](now).GetEdges()) {
      Index to_node = each.GetSecond().GetId();
      if (!book.Exist(to_node) && (!distance.Exist(to_node) || distance[to_node] > dis + each.GetWeight())) {
        distance[to_node] = dis + each.GetWeight();
        pos_next.push({distance[to_node], to_node});
      }
    }
  }
  return distance;
}
}  // namespace lhy
#endif  // BASE_GRAPH_ALGORITHM_H