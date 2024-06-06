//
// Created by lhy on 24-3-25.
//

#ifndef UNDIRECTED_GRAPH_H
#define UNDIRECTED_GRAPH_H
#include "base_graph.h"
#include "basic.h"
#include "directed_graph.h"
namespace lhy {
template <node_c Node_ = Node, edge_c Edge_ = Edge>
class UndirectedGraph : public BaseGraph<Node_, Edge_> {
 public:
  using BaseGraph<Node_, Edge_>::BaseGraph;
  void AddEdge(Edge_&& edge) override;
  void DeleteEdge(Edge_&& edge) override;
  void DeleteEdges(Index first, Index second) override;
};

template <node_c Node_, edge_c Edge_>
void UndirectedGraph<Node_, Edge_>::AddEdge(Edge_&& edge) {
  BaseGraph<Node_, Edge_>::AddEdge(std::forward<Edge_>(edge));
  BaseGraph<Node_, Edge_>::AddEdge(std::forward<Edge_>(edge.GetReversed()));
}
template <node_c Node_, edge_c Edge_>
void UndirectedGraph<Node_, Edge_>::DeleteEdge(Edge_&& edge) {
  BaseGraph<Node_, Edge_>::DeleteEdge(std::forward<Edge_>(edge));
  BaseGraph<Node_, Edge_>::DeleteEdge(std::forward<Edge_>(edge.GetReversed()));
}
template <node_c Node_, edge_c Edge_>
void UndirectedGraph<Node_, Edge_>::DeleteEdges(Index first, Index second) {
  BaseGraph<Node_, Edge_>::DeleteEdges(first, second);
  BaseGraph<Node_, Edge_>::DeleteEdges(second, first);
}
template <node_c Node_ = Node, edge_c Edge_ = Edge>
using UG = UndirectedGraph<Node_, Edge_>;
template <typename NodeWeightType, edge_c Edge_ = Edge>
using UndirectedGraphWeightedNode = UndirectedGraph<WeightedNode<NodeWeightType>, Edge_>;
template <typename NodeWeightType, edge_c Edge_ = Edge>
using UGN = UndirectedGraphWeightedNode<NodeWeightType, Edge_>;
template <typename EdgeWeightType, node_c Node_ = Node>
using UndirectedGraphWeightedEdge = UndirectedGraph<WeightedNode<EdgeWeightType>, Node_>;
template <typename EdgeWeightType, node_c Node_ = Node>
using UGE = UndirectedGraphWeightedEdge<EdgeWeightType, Node_>;
template <typename NodeWeightType, typename EdgeWeightType>
using UndirectedGraphWeightedNodeWeightedEdge =
    UndirectedGraph<WeightedNode<NodeWeightType>, WeightedEdge<EdgeWeightType>>;
template <typename NodeWeightType, typename EdgeWeightType>
using UGNE = UndirectedGraphWeightedNodeWeightedEdge<NodeWeightType, EdgeWeightType>;
}  // namespace lhy

#endif  // UNDIRECTED_GRAPH_H
