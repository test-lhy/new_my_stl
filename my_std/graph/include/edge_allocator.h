//
// Created by lhy31 on 2024/1/8
//

#ifndef MY_STL_EDGE_ALLOCATOR_H
#define MY_STL_EDGE_ALLOCATOR_H

#include <map>

#include "basic.h"
#include "edge.h"
#include "list.h"
#include "vector.h"
namespace lhy {
class EdgeReserveSync {
 public:
  virtual void Reserve(size_t size) { size; };
};
template <edge_c Edge_>
class EdgeAllocator : public EdgeReserveSync {
 public:
  virtual void AddEdge(const Edge_& edge) = 0;
  template <typename... Args>
  void EmplaceEdge(Args&&... args);
  virtual bool Exist(const Edge_& edge) = 0;
  virtual void DeleteEdge(const Edge_& edge) = 0;
  virtual void DeleteEdges(Index first, Index second) = 0;
  virtual const list<Edge_>& GetEdges(Index node) = 0;
  virtual void DeleteNode(Index node) = 0;
  virtual ~EdgeAllocator() = default;
};
template <edge_c Edge_>
template <typename... Args>
void EdgeAllocator<Edge_>::EmplaceEdge(Args&&... args) {
  AddEdge(Edge_(std::forward<Args>(args)...));
}
template <typename T, typename Edge_>
concept edge_allocator_c = std::is_base_of_v<EdgeAllocator<Edge_>, T>;
}  // namespace lhy
#endif  // MY_STL_EDGE_ALLOCATOR_H