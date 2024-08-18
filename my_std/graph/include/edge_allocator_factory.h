//
// Created by lhy on 24-1-11.
//
#ifndef MY_STL_EDGE_ALLOCATOR_FACTORY_H
#define MY_STL_EDGE_ALLOCATOR_FACTORY_H
#include "exception.h"
#include "linklist_edge_allocator.h"
#include "map_edge_allocator.h"
#include "matrix_edge_allocator.h"
#include "shared_ptr.h"

namespace lhy {
enum EdgeAllocatorMode { LinkListEdgeAllocatorMode, MapEdgeAllocatorMode, MatrixEdgeAllocatorMode };

template <edge_c Edge_>
class EdgeAllocatorFactory {
 public:
  shared_ptr<EdgeAllocator<Edge_>> Create(EdgeAllocatorMode mode);
};

template <edge_c Edge_>
shared_ptr<EdgeAllocator<Edge_>> EdgeAllocatorFactory<Edge_>::Create(EdgeAllocatorMode mode) {
  switch (mode) {
    case EdgeAllocatorMode::LinkListEdgeAllocatorMode:
      return make_shared<LinkListEdgeAllocator<Edge_>>();
    case EdgeAllocatorMode::MapEdgeAllocatorMode:
      return make_shared<MapEdgeAllocator<Edge_>>();
    case EdgeAllocatorMode::MatrixEdgeAllocatorMode:
      return make_shared<MatrixEdgeAllocator<Edge_>>();
  }
  throw logic_error("unsupported mode");
}
}  // namespace lhy
#endif  // MY_STL_EDGE_ALLOCATOR_FACTORY_H
