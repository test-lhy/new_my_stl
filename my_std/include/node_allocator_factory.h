//
// Created by lhy on 24-1-11.
//
#include "map_node_allocator.h"
#include "normal_node_allocator.h"
#ifndef MY_STL_NODE_ALLOCATOR_FACTORY_H
#define MY_STL_NODE_ALLOCATOR_FACTORY_H
namespace lhy {
enum NodeAllocatorMode { NormalNodeAllocatorMode, MapNodeAllocatorMode };
template <node_c Node_>
class NodeAllocatorFactory {
 public:
  NodeAllocator<Node_>* Create(NodeAllocatorMode mode);
};
template <node_c Node_>
NodeAllocator<Node_>* NodeAllocatorFactory<Node_>::Create(NodeAllocatorMode mode) {
  switch (mode) {
    case NodeAllocatorMode::NormalNodeAllocatorMode:
      return new lhy::NormalNodeAllocator<Node_>();
    case NodeAllocatorMode::MapNodeAllocatorMode:
      return new lhy::MapNodeAllocator<Node_>();
  }
  throw std::logic_error("unsupported mode");
}
}  // namespace lhy
#endif  // MY_STL_NODE_ALLOCATOR_FACTORY_H
