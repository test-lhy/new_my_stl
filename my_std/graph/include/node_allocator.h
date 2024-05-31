
//
// Created by lhy31 on 2024/1/8
//
#ifndef MY_STL_NODE_ALLOCATOR_H
#define MY_STL_NODE_ALLOCATOR_H
#include <map>
#include <stdexcept>

#include "basic.h"
#include "index_container.h"
#include "node.h"
#include "vector.h"
namespace lhy {
enum NodeAllocatorMode { NormalNodeAllocatorMode, MapNodeAllocatorMode };
template <node_c Node_>
class NodeAllocator : public DataStructure<std::pair<Index, Node_>> {
 public:
  using RealNode_ = std::pair<Index, Node_>;
  template <node_c Nodee, edge_c Edge_>
  friend class BaseGraph;
  using iterator = typename IndexContainer<Node_>::iterator;
  using reversed_iterator = typename IndexContainer<Node_>::reversed_iterator;
  NodeAllocator(EdgeReserveSync* edge_allocator_sync, const NodeAllocatorMode mode)
      : edge_allocator_sync_(edge_allocator_sync) {
    container_ = new IndexContainer<Node_>(relation_between_nodeallocator_indexcontainer.at(mode));
    Sync(100);
  }
  Index AddNode(const Node_& node);
  template <typename... Args>
  Index EmplaceNode(Args&&... args);
  void DeleteNode(Index index);
  Node_& GetNode(Index index);
  bool Exist(Index index);
  void Sync(Index index);
  [[nodiscard]] IndexContainerMode GetContainerMode() const;
  [[nodiscard]] iterator begin();
  [[nodiscard]] iterator end();
  [[nodiscard]] reversed_iterator rbegin();
  [[nodiscard]] reversed_iterator rend();

 private:
  [[nodiscard]] typename DataStructure<RealNode_>::Pointer getBegin() override;
  [[nodiscard]] typename DataStructure<RealNode_>::Pointer getEnd() override;

 public:
  ~NodeAllocator();

  inline static std::map<NodeAllocatorMode, IndexContainerMode> relation_between_nodeallocator_indexcontainer = {
      {NormalNodeAllocatorMode, VectorIndexContainerMode}, {MapNodeAllocatorMode, MapIndexContainerMode}};

 private:
  EdgeReserveSync* edge_allocator_sync_;
  IndexContainer<Node_>* container_;
  Index max_index_{};
};
template <node_c Node_>
Index NodeAllocator<Node_>::AddNode(const Node_& node) {
  if (Exist(node.GetId()) && node != GetNode(node.GetId())) {
    throw std::logic_error("can not add a node with the same id and different content");
  }
  Sync(node.GetId());
  container_->operator[](node.GetId()) = node;
  return node.GetId();
}
template <node_c Node_>
void NodeAllocator<Node_>::DeleteNode(Index index) {
  container_->erase(index);
}
template <node_c Node_>
Node_& NodeAllocator<Node_>::GetNode(Index index) {
  if (Exist(index)) {
    return container_->operator[](index);
  }
  throw std::logic_error("the node does not exist");
}
template <node_c Node_>
bool NodeAllocator<Node_>::Exist(Index index) {
  return container_->Exist(index);
}
template <node_c Node_>
void NodeAllocator<Node_>::Sync(Index index) {
  if (index > max_index_) {
    max_index_ = index * 2 + 100;
    edge_allocator_sync_->Reserve(max_index_);
  }
}
template <node_c Node_>
IndexContainerMode NodeAllocator<Node_>::GetContainerMode() const {
  return container_->GetMode();
}
template <node_c Node_>
typename NodeAllocator<Node_>::iterator NodeAllocator<Node_>::begin() {
  return container_->begin();
}
template <node_c Node_>
typename NodeAllocator<Node_>::iterator NodeAllocator<Node_>::end() {
  return container_->end();
}
template <node_c Node_>
typename NodeAllocator<Node_>::reversed_iterator NodeAllocator<Node_>::rbegin() {
  return container_->rbegin();
}
template <node_c Node_>
typename NodeAllocator<Node_>::reversed_iterator NodeAllocator<Node_>::rend() {
  return container_->rend();
}
template <node_c Node_>
typename DataStructure<typename NodeAllocator<Node_>::RealNode_>::Pointer NodeAllocator<Node_>::getBegin() {
  return container_->getBegin();
}
template <node_c Node_>
typename DataStructure<typename NodeAllocator<Node_>::RealNode_>::Pointer NodeAllocator<Node_>::getEnd() {
  return container_->getEnd();
}
template <node_c Node_>
NodeAllocator<Node_>::~NodeAllocator() {
  delete container_;
}
template <node_c Node_>
template <typename... Args>
Index NodeAllocator<Node_>::EmplaceNode(Args&&... args) {
  return AddNode(Node_(std::forward<Args>(args)...));
}
template <typename T, typename Node_>
concept node_allocator_c = std::is_base_of_v<NodeAllocator<Node_>, T>;
}  // namespace lhy
#endif  // MY_STL_NODE_ALLOCATOR_H