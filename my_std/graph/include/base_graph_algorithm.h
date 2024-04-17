//
// Created by lhy on 24-4-12.
//

#ifndef BASE_GRAPH_ALGORITHM_H
#define BASE_GRAPH_ALGORITHM_H
#include "base_graph.h"
#include "index_container.h"
namespace lhy {
template <node_c Node_, edge_c Edge_>
void Dijkstra(BaseGraph<Node_, Edge_>* graph, Index node, bool use_map = false) {
  IndexContainer<double> distance(use_map ? MapIndexContainerMode : VectorIndexContainerMode);
}
}  // namespace lhy
#endif  // BASE_GRAPH_ALGORITHM_H
