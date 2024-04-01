//
// Created by lhy31 on 2024/1/8
//

#ifndef MY_STL_BASE_GRAPH_H
#define MY_STL_BASE_GRAPH_H
#include "edge.h"
#include "edge_allocator_factory.h"
#include "node.h"
#include "node_allocator_factory.h"
namespace lhy {
template <node_c Node_ = Node, edge_c Edge_ = Edge>
class BaseGraph {
 public:
  struct return_node {
    Node_& node_;
    list<Edge_>& edges_;
    return_node(Node_& node, list<Edge>& edges) : node_(node), edges_(edges) {}
    bool operator==(const return_node& other) const { return node_ == other.node_; }
    bool operator!=(const return_node& other) const { return node_ != other.node_; }
    ~return_node() = default;
  };
  BaseGraph(NodeAllocatorMode node_allocator_mode = NodeAllocatorMode::NormalNodeAllocatorMode,
            EdgeAllocatorMode edge_allocator_mode = EdgeAllocatorMode::LinkListEdgeAllocatorMode);
  virtual ~BaseGraph();
  template <typename... Args>
  void AddEdge(Index first, Index second, Args&&... args);
  virtual void AddEdge(const Edge_& edge);
  template <typename... Args>
  void AddNode(Args&&... args);
  void AddNode(const Node_& node);
  virtual return_node GetNode(Index node);
  virtual return_node GetNode(const Node_& node);
  virtual return_node operator[](Index node);
  virtual return_node operator[](const Node_& node);
  template <typename... Args>
  void DeleteEdge(Args&&... args);
  virtual void DeleteEdge(Edge_&& edge);
  void DeleteNode(Index node);
  void DeleteNode(Node_&& node);

 private:
  NodeAllocator<Node_>* node_allocator_;
  EdgeAllocator<Edge_>* edge_allocator_;
};
template <node_c Node_, edge_c Edge_>
BaseGraph<Node_, Edge_>::BaseGraph(NodeAllocatorMode node_allocator_mode, EdgeAllocatorMode edge_allocator_mode) {
  auto node_allocator_factory = new NodeAllocatorFactory<Node_>();
  auto edge_allocator_factory = new EdgeAllocatorFactory<Edge_>();
  node_allocator_ = node_allocator_factory->Create(node_allocator_mode);
  edge_allocator_ = edge_allocator_factory->Create(edge_allocator_mode);
  node_allocator_->SetEdgeReserveSync(edge_allocator_);
  delete node_allocator_factory;
  delete edge_allocator_factory;
}
template <node_c Node_, edge_c Edge_>
void BaseGraph<Node_, Edge_>::DeleteNode(Node_&& node) {
  DeleteNode(node.GetId());
}
template <node_c Node_, edge_c Edge_>
void BaseGraph<Node_, Edge_>::DeleteNode(Index node) {
  edge_allocator_->DeleteNode(node);
  node_allocator_->DeleteNode(node);
}
template <node_c Node_, edge_c Edge_>
template <typename... Args>
void BaseGraph<Node_, Edge_>::DeleteEdge(Args&&... args) {
  DeleteEdge(Edge_(std::forward<Args>(args)...));
}
template <node_c Node_, edge_c Edge_>
void BaseGraph<Node_, Edge_>::DeleteEdge(Edge_&& edge) {
  edge_allocator_->DeleteEdge(edge);
}
template <node_c Node_, edge_c Edge_>
BaseGraph<Node_, Edge_>::return_node BaseGraph<Node_, Edge_>::operator[](const Node_& node) {
  return GetNode(node);
}
template <node_c Node_, edge_c Edge_>
BaseGraph<Node_, Edge_>::return_node BaseGraph<Node_, Edge_>::GetNode(const Node_& node) {
  return GetNode(node.GetId());
}
template <node_c Node_, edge_c Edge_>
BaseGraph<Node_, Edge_>::return_node BaseGraph<Node_, Edge_>::operator[](Index node) {
  return GetNode(node);
}
template <node_c Node_, edge_c Edge_>
BaseGraph<Node_, Edge_>::return_node BaseGraph<Node_, Edge_>::GetNode(Index node) {
  return {node_allocator_->GetNode(node), edge_allocator_->GetEdges(node)};
}
template <node_c Node_, edge_c Edge_>
BaseGraph<Node_, Edge_>::~BaseGraph() {
  delete node_allocator_;
  delete edge_allocator_;
}
template <node_c Node_, edge_c Edge_>
void BaseGraph<Node_, Edge_>::AddNode(const Node_& node) {
  node_allocator_->AddNode(node);
}
template <node_c Node_, edge_c Edge_>
template <typename... Args>
void BaseGraph<Node_, Edge_>::AddNode(Args&&... args) {
  node_allocator_->EmplaceNode(std::forward<Args>(args)...);
}
template <node_c Node_, edge_c Edge_>
void BaseGraph<Node_, Edge_>::AddEdge(const Edge_& edge) {
  edge_allocator_->AddEdge(edge);
}
template <node_c Node_, edge_c Edge_>
template <typename... Args>
void BaseGraph<Node_, Edge_>::AddEdge(Index first, Index second, Args&&... args) {
  edge_allocator_->AddEdge(
      Edge_{node_allocator_->GetNode(first), node_allocator_->GetNode(second), std::forward<Args>(args)...});
}
}  // namespace lhy
#endif  // MY_STL_BASE_GRAPH_H