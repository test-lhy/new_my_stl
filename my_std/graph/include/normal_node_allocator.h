
//
// Created by lhy31 on 2024/1/8
//
#ifndef MY_STL_NORMAL_NODE_ALLOCATOR_H
#define MY_STL_NORMAL_NODE_ALLOCATOR_H
#include "node_allocator.h"
namespace lhy {
template <node_c Node_>
class NormalNodeAllocator : public NodeAllocator<Node_> {
 public:
  explicit NormalNodeAllocator(size_t size = 1);
  void AddNode(const Node_& node) override;
  bool Exist(Index index) override;
  Node_& GetNode(Index index) override;
  void DeleteNode(Index index) override;
  void Sync() override;
  ~NormalNodeAllocator() override = default;

 private:
  vector<std::pair<Node_, bool>> nodes_;
};
template <node_c Node_>
void NormalNodeAllocator<Node_>::Sync() {
  std::cout << nodes_.size() << std::endl;
  this->GetEdgeReserveSync()->Reserve(nodes_.size());
}
template <node_c Node_>
NormalNodeAllocator<Node_>::NormalNodeAllocator(size_t size) {
  nodes_.reserve(size);
}
template <node_c Node_>
void NormalNodeAllocator<Node_>::AddNode(const Node_& node) {
  if (node.GetId() >= nodes_.size()) {
    size_t size = nodes_.size() * 2 + 100;
    nodes_.reserve(size);
    Sync();
  }
  if (Exist(node.GetId()) && GetNode(node.GetId()) != node) {
    throw std::logic_error("can not add a node with the same id and different content");
  }
  nodes_[node.GetId()].first = node;
  nodes_[node.GetId()].second = true;
}
template <node_c Node_>
bool NormalNodeAllocator<Node_>::Exist(Index index) {
  if (index >= nodes_.size()) {
    return false;
  }
  return nodes_[index].second;
}
template <node_c Node_>
Node_& NormalNodeAllocator<Node_>::GetNode(Index index) {
  if (Exist(index)) {
    return nodes_[index].first;
  }
  throw std::logic_error("the node does not exist");
}
template <node_c Node_>
void NormalNodeAllocator<Node_>::DeleteNode(Index index) {
  nodes_[index].second = 0;
}
}  // namespace lhy
#endif  // MY_STL_NORMAL_NODE_ALLOCATOR_H