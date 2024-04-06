//
// Created by lhy on 24-3-29.
//

#ifndef TREES_H
#define TREES_H
#include "base_graph.h"
#include "treenode.h"
namespace lhy {
template <node_c Origin_Node_ = Node, edge_c Edge_ = Edge>
class Trees {
  using Node_ = TreeNode<Origin_Node_>;

 public:
  explicit Trees(NodeAllocatorMode node_allocator_mode = NodeAllocatorMode::NormalNodeAllocatorMode,
                 EdgeAllocatorMode edge_allocator_mode = EdgeAllocatorMode::LinkListEdgeAllocatorMode)
      : graph_(node_allocator_mode, edge_allocator_mode) {}
  class return_node : public BaseGraph<Node_, Edge_>::return_node {
   public:
    using BaseGraph<Node_, Edge_>::return_node::return_node;
    using Base = typename BaseGraph<Node_, Edge_>::return_node;
    return_node(const Base& base) : Base(base) {}
    return_node GetFather() { return Base::GetGraph()->operator[](GetNode().GetFather()); }
    Index GetFatherIndex() { return Base::GetNode().GetFather(); }
  };
  void AddRoot(Origin_Node_&& node);
  template <typename... Args>
  void AddNode(Origin_Node_&& node, Index father, Args&&... args);
  template <typename... Args>
  void ChangeFather(Index node, Index father, Args&&... args);
  return_node operator[](Index node) { return graph_[node]; }
  return_node GetNode(Index node) { return graph_[node]; }

 private:
  BaseGraph<Node_> graph_;
  vector<Index> roots_{};
};

template <node_c Origin_Node_, edge_c Edge_>
void Trees<Origin_Node_, Edge_>::AddRoot(Origin_Node_&& node) {
  roots_.push_back(graph_.AddNode(
      std::forward<Node_>(Node_{std::forward<Origin_Node_>(node), std::forward<std::optional<Index>>({})})));
}
template <node_c Origin_Node_, edge_c Edge_>
template <typename... Args>
void Trees<Origin_Node_, Edge_>::AddNode(Origin_Node_&& node, Index father, Args&&... args) {
  if (!graph_.NodeExist(father)) {
    throw std::logic_error("node's father should have been created.");
  }
  auto node_now =
      graph_.AddNode(std::forward<Node_>(Node_{std::forward<Origin_Node_>(node), std::forward<Index>(father)}));
  graph_.AddEdge(GetNode(node_now).GetFatherIndex(), node_now, std::forward<Args>(args)...);
}
template <node_c Origin_Node_, edge_c Edge_>
template <typename... Args>
void Trees<Origin_Node_, Edge_>::ChangeFather(Index node, Index father, Args&&... args) {
  if (!graph_.NodeExist(father)) {
    throw std::logic_error("node's father should have been created.");
  }
  if (!graph_.NodeExist(node)) {
    throw std::logic_error("node itself should have been created.");
  }
  graph_.DeleteEdges(GetNode(node).GetFatherIndex(), node);
  GetNode(node).GetNode().SetFather(father);
  graph_.AddEdge(father, node, std::forward<Args>(args)...);
}
}  // namespace lhy

#endif  // TREES_H
