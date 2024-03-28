//
// Created by lhy on 24-1-11.
//
#ifndef MY_STL_EDGE_ALLOCATOR_FACTORY_H
#define MY_STL_EDGE_ALLOCATOR_FACTORY_H
#include "linklist_edge_allocator.h"
#include "map_edge_allocator.h"
#include "matrix_edge_allocator.h"

namespace lhy
{
  enum EdgeAllocatorMode { LinkListEdgeAllocatorMode, MapEdgeAllocatorMode, MatrixEdgeAllocatorMode };

  template <edge_c Edge_>
  class EdgeAllocatorFactory
  {
  public:
    EdgeAllocator<Edge_>* Create(EdgeAllocatorMode mode);
  };

  template <edge_c Edge_>
  EdgeAllocator<Edge_>* EdgeAllocatorFactory<Edge_>::Create(EdgeAllocatorMode mode)
  {
    switch (mode)
    {
    case EdgeAllocatorMode::LinkListEdgeAllocatorMode:
      return new LinkListEdgeAllocator<Edge_>();
    case EdgeAllocatorMode::MapEdgeAllocatorMode:
      return new MapEdgeAllocator<Edge_>();
    case EdgeAllocatorMode::MatrixEdgeAllocatorMode:
      return new MatrixEdgeAllocator<Edge_>();
    }
    throw std::logic_error("unsupported mode");
  }
} // namespace lhy
#endif  // MY_STL_EDGE_ALLOCATOR_FACTORY_H
