
//
// Created by lhy31 on 2024/1/8
//
#ifndef MY_STL_NODE_ALLOCATOR_H
#define MY_STL_NODE_ALLOCATOR_H
#include <map>
#include <stdexcept>

#include "basic.h"
#include "node.h"
#include "vector.h"
namespace lhy {
template <node_c Node_>
class NodeAllocator {
 public:
  virtual void AddNode(const Node_& node) = 0;
  template <typename... Args>
  void EmplaceNode(Args&&... args);
  virtual void DeleteNode(Index index) = 0;
  virtual Node_& GetNode(Index index) = 0;
  virtual bool Exist(Index index) = 0;
  virtual void Sync(){};
  void SetEdgeReserveSync(EdgeReserveSync* edge_allocator_sync);
  EdgeReserveSync* GetEdgeReserveSync();
  virtual ~NodeAllocator() = default;

 private:
  EdgeReserveSync* edge_allocator_sync_;
};
template <node_c Node_>
EdgeReserveSync* NodeAllocator<Node_>::GetEdgeReserveSync() {
  return edge_allocator_sync_;
}
template <node_c Node_>
void NodeAllocator<Node_>::SetEdgeReserveSync(EdgeReserveSync* edge_allocator_sync) {
  edge_allocator_sync_ = edge_allocator_sync;
  Sync();
}
template <node_c Node_>
template <typename... Args>
void NodeAllocator<Node_>::EmplaceNode(Args&&... args) {
  AddNode(Node_(std::forward<Args>(args)...));
}
template <typename T, typename Node_>
concept node_allocator_c = std::is_base_of_v<NodeAllocator<Node_>, T>;
}  // namespace lhy
#endif  // MY_STL_NODE_ALLOCATOR_H