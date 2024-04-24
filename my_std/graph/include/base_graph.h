//
// Created by lhy31 on 2024/1/8
//

#ifndef MY_STL_BASE_GRAPH_H
#define MY_STL_BASE_GRAPH_H
#include "edge.h"
#include "edge_allocator_factory.h"
#include "node.h"
#include "node_allocator.h"
namespace lhy {
template <node_c Node_ = Node, edge_c Edge_ = Edge>
class BaseGraph : public DataStructure<Node_> {
 public:
  // 使用return node作为iterator不是不能做，但是，需要提前将return_node声明，暂时不想这么做,并且比较麻烦
  using iterator = typename IndexContainer<Node_>::iterator;
  using reversed_iterator = typename IndexContainer<Node_>::reversed_iterator;
  class return_node {
   public:
    return_node(Index node, BaseGraph* graph) : node_(node), graph_(graph) {}
    Node_& GetNode() { return graph_->GetNodeImpl(node_); }
    const list<Edge_>& GetEdges() const { return graph_->GetEdgesImpl(node_); }  // const &应该问题不大吧
    bool operator==(const return_node& other) const { return node_ == other.node_; }
    bool operator!=(const return_node& other) const { return node_ != other.node_; }
    virtual ~return_node();

   protected:
    BaseGraph* GetGraph() { return graph_; }

   private:
    Index node_;
    BaseGraph* graph_;
  };
  friend return_node;
  explicit BaseGraph(NodeAllocatorMode node_allocator_mode = NodeAllocatorMode::NormalNodeAllocatorMode,
                     EdgeAllocatorMode edge_allocator_mode = EdgeAllocatorMode::LinkListEdgeAllocatorMode);
  virtual ~BaseGraph();
  template <typename... Args>
  void AddEdge(Index first, Index second, Args&&... args);
  virtual void AddEdge(Edge_&& edge);
  template <typename... Args>
  Index AddNode(Args&&... args);
  Index AddNode(Node_&& node);
  return_node GetNode(Index node);
  return_node GetNode(const Node_& node);
  return_node operator[](Index node);
  return_node operator[](Node_&& node);
  template <typename... Args>
  void DeleteEdge(Args&&... args);
  virtual void DeleteEdge(Edge_&& edge);
  virtual void DeleteEdges(Index first, Index second);
  void DeleteEdges(const Node_& first, const Node_& second);
  void DeleteNode(Index node);
  void DeleteNode(Node_&& node);
  [[nodiscard]] bool NodeExist(Index node) const;
  [[nodiscard]] IndexContainerMode GetNodeContainerMode() const;
  [[nodiscard]] iterator begin();
  [[nodiscard]] iterator end();
  [[nodiscard]] reversed_iterator rbegin();
  [[nodiscard]] reversed_iterator rend();

 private:
  [[nodiscard]] typename DataStructure<Node_>::Pointer getBegin() override;
  [[nodiscard]] typename DataStructure<Node_>::Pointer getEnd() override;
  Node_& GetNodeImpl(Index node);
  const list<Edge_>& GetEdgesImpl(Index node);
  NodeAllocator<Node_>* node_allocator_;
  EdgeAllocator<Edge_>* edge_allocator_;
};
template <node_c Node_, edge_c Edge_>
BaseGraph<Node_, Edge_>::return_node::~return_node() {}
template <node_c Node_, edge_c Edge_>
BaseGraph<Node_, Edge_>::BaseGraph(NodeAllocatorMode node_allocator_mode, EdgeAllocatorMode edge_allocator_mode) {
  auto edge_allocator_factory = new EdgeAllocatorFactory<Edge_>();
  edge_allocator_ = edge_allocator_factory->Create(edge_allocator_mode);
  node_allocator_ = new NodeAllocator<Node_>(edge_allocator_, node_allocator_mode);
  delete edge_allocator_factory;
}
template <node_c Node_, edge_c Edge_>
void BaseGraph<Node_, Edge_>::DeleteNode(Node_&& node) {
  DeleteNode(node.GetId());
}
template <node_c Node_, edge_c Edge_>
bool BaseGraph<Node_, Edge_>::NodeExist(Index node) const {
  return node_allocator_->Exist(node);
}
template <node_c Node_, edge_c Edge_>
IndexContainerMode BaseGraph<Node_, Edge_>::GetNodeContainerMode() const {
  return node_allocator_->GetContainerMode();
}
template <node_c Node_, edge_c Edge_>
typename BaseGraph<Node_, Edge_>::iterator BaseGraph<Node_, Edge_>::begin() {
  return node_allocator_->begin();
}
template <node_c Node_, edge_c Edge_>
typename BaseGraph<Node_, Edge_>::iterator BaseGraph<Node_, Edge_>::end() {
  return node_allocator_->end();
}
template <node_c Node_, edge_c Edge_>
typename BaseGraph<Node_, Edge_>::reversed_iterator BaseGraph<Node_, Edge_>::rbegin() {
  return node_allocator_->rbegin();
}
template <node_c Node_, edge_c Edge_>
typename BaseGraph<Node_, Edge_>::reversed_iterator BaseGraph<Node_, Edge_>::rend() {
  return node_allocator_.rend();
}
template <node_c Node_, edge_c Edge_>
typename DataStructure<Node_>::Pointer BaseGraph<Node_, Edge_>::getBegin() {
  return node_allocator_->getBegin();
}
template <node_c Node_, edge_c Edge_>
typename DataStructure<Node_>::Pointer BaseGraph<Node_, Edge_>::getEnd() {
  return node_allocator_->getEnd();
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
void BaseGraph<Node_, Edge_>::DeleteEdges(Index first, Index second) {
  edge_allocator_->DeleteEdges(first, second);
}
template <node_c Node_, edge_c Edge_>
void BaseGraph<Node_, Edge_>::DeleteEdges(const Node_& first, const Node_& second) {
  DeleteEdges(first.GetId(), second.GetId());
}
template <node_c Node_, edge_c Edge_>
typename BaseGraph<Node_, Edge_>::return_node BaseGraph<Node_, Edge_>::operator[](Node_&& node) {
  return GetNode(std::forward<Node_>(node));
}
template <node_c Node_, edge_c Edge_>
typename BaseGraph<Node_, Edge_>::return_node BaseGraph<Node_, Edge_>::GetNode(const Node_& node) {
  return GetNode(node.GetId());
}
template <node_c Node_, edge_c Edge_>
const list<Edge_>& BaseGraph<Node_, Edge_>::GetEdgesImpl(Index node) {
  return edge_allocator_->GetEdges(node);
}
template <node_c Node_, edge_c Edge_>
Node_& BaseGraph<Node_, Edge_>::GetNodeImpl(Index node) {
  return node_allocator_->GetNode(node);
}
template <node_c Node_, edge_c Edge_>
typename BaseGraph<Node_, Edge_>::return_node BaseGraph<Node_, Edge_>::operator[](Index node) {
  return GetNode(node);
}
template <node_c Node_, edge_c Edge_>
typename BaseGraph<Node_, Edge_>::return_node BaseGraph<Node_, Edge_>::GetNode(Index node) {
  return {node, this};
}
template <node_c Node_, edge_c Edge_>
BaseGraph<Node_, Edge_>::~BaseGraph() {
  delete node_allocator_;
  delete edge_allocator_;
}
template <node_c Node_, edge_c Edge_>
Index BaseGraph<Node_, Edge_>::AddNode(Node_&& node) {
  return node_allocator_->AddNode(std::forward<Node_>(node));
}
template <node_c Node_, edge_c Edge_>
template <typename... Args>
Index BaseGraph<Node_, Edge_>::AddNode(Args&&... args) {
  return node_allocator_->EmplaceNode(std::forward<Args>(args)...);
}
template <node_c Node_, edge_c Edge_>
void BaseGraph<Node_, Edge_>::AddEdge(Edge_&& edge) {
  edge_allocator_->AddEdge(std::forward<Edge_>(edge));
}
template <node_c Node_, edge_c Edge_>
template <typename... Args>
void BaseGraph<Node_, Edge_>::AddEdge(Index first, Index second, Args&&... args) {
  edge_allocator_->AddEdge(
      Edge_{node_allocator_->GetNode(first), node_allocator_->GetNode(second), std::forward<Args>(args)...});
}
}  // namespace lhy
#endif  // MY_STL_BASE_GRAPH_H