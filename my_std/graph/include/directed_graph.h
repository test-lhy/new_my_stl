//
// Created by lhy on 24-3-25.
//

#ifndef DIRECTED_GRAPH_H
#define DIRECTED_GRAPH_H
#include "base_graph.h"
#include "weighted_edge.h"
#include "weighted_node.h"
namespace lhy {
template <node_c Node_ = Node, edge_c Edge_ = Edge>
using DirectedGraph = BaseGraph<Node_, Edge_>;
template <node_c Node_ = Node, edge_c Edge_ = Edge>
using DG = DirectedGraph<Node_, Edge_>;
template <typename NodeWeightType, edge_c Edge_ = Edge>
using DirectedGraphWeightedNode = DirectedGraph<WeightedNode<NodeWeightType>, Edge_>;
template <typename NodeWeightType, edge_c Edge_ = Edge>
using DGN = DirectedGraphWeightedNode<NodeWeightType, Edge_>;
template <typename EdgeWeightType, node_c Node_ = Node>
using DirectedGraphWeightedEdge = DirectedGraph<WeightedNode<EdgeWeightType>, Node_>;
template <typename EdgeWeightType, node_c Node_ = Node>
using DGE = DirectedGraphWeightedEdge<EdgeWeightType, Node_>;
template <typename NodeWeightType, typename EdgeWeightType>
using DirectedGraphWeightedNodeWeightedEdge = DirectedGraph<WeightedNode<NodeWeightType>, WeightedEdge<EdgeWeightType>>;
template <typename NodeWeightType, typename EdgeWeightType>
using DGNE = DirectedGraphWeightedNodeWeightedEdge<NodeWeightType, EdgeWeightType>;
}  // namespace lhy
#endif  // DIRECTED_GRAPH_H
