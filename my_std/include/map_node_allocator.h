
//
// Created by lhy31 on 2024/1/8
//
#ifndef MY_STL_MAP_NODE_ALLOCATOR_H
#define MY_STL_MAP_NODE_ALLOCATOR_H
#include "node.h"
#include "node_allocator.h"
namespace lhy {
template <node_c Node_>
class MapNodeAllocator : public NodeAllocator<Node_> {
 public:
  void AddNode(const Node_& node) override;
  bool Exist(Index index) override;
  Node_& GetNode(Index index) override;
  void DeleteNode(Index index) override;
  ~MapNodeAllocator() override = default;

 private:
  std::map<Index, Node_> nodes_;
};

template <node_c Node_>
void MapNodeAllocator<Node_>::AddNode(const Node_& node) {
  if (Exist(node.GetId()) && node != GetNode(node.GetId())) {
    throw std::logic_error("can not add a node with the same id and different content");
  }
  this->GetEdgeReserveSync()->Reserve(node.GetId() + 1);
  nodes_[node.GetId()] = node;
}
template <node_c Node_>
bool MapNodeAllocator<Node_>::Exist(Index index) {
  return nodes_.find(index) != nodes_.end();
}
template <node_c Node_>
Node_& MapNodeAllocator<Node_>::GetNode(Index index) {
  if (Exist(index)) {
    return nodes_[index];
  }
  throw std::logic_error("the node does not exist");
}
template <node_c Node_>
void MapNodeAllocator<Node_>::DeleteNode(Index index) {
  nodes_.erase(index);
}
}  // namespace lhy
#endif  // MY_STL_MAP_NODE_ALLOCATOR_H