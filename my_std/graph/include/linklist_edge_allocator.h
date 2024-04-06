//
// Created by lhy31 on 2024/1/8
//
#include <ranges>

#include "edge.h"
#include "edge_allocator.h"
#include "list.h"
#ifndef MY_STL_LINKLIST_EDGE_ALLOCATOR_H
#define MY_STL_LINKLIST_EDGE_ALLOCATOR_H
namespace lhy {
template <edge_c Edge_>
class LinkListEdgeAllocator : public EdgeAllocator<Edge_> {
 public:
  void AddEdge(const Edge_& edge) override;
  bool Exist(const Edge_& edge) override;
  void DeleteEdge(const Edge_& edge) override;
  void DeleteEdges(Index first, Index second) override;
  const list<Edge_>& GetEdges(Index node) override;
  void DeleteNode(Index node) override;
  void Reserve(size_t size) override;
  ~LinkListEdgeAllocator() override = default;

 private:
  vector<list<Edge_>> edges_;
};
template <edge_c Edge_>
void LinkListEdgeAllocator<Edge_>::AddEdge(const Edge_& edge) {
  edges_[edge.GetFirst().GetId()].push_back(edge);
}
template <edge_c Edge_>
bool LinkListEdgeAllocator<Edge_>::Exist(const Edge_& edge) {
  const auto& all_edges = GetEdges(edge.GetFirst().GetId());
  return find(all_edges.begin(), all_edges.end(), edge) == all_edges.end();
}
template <edge_c Edge_>
void LinkListEdgeAllocator<Edge_>::DeleteEdge(const Edge_& edge) {
  auto& all_edges = edges_[edge.GetFirst().GetId()];
  all_edges.erase(edge);
}
template <edge_c Edge_>
void LinkListEdgeAllocator<Edge_>::DeleteEdges(Index first, Index second) {
  auto& all_edges = edges_[first];
  auto each_iter = all_edges.begin();
  while (each_iter != all_edges.end()) {
    if (each_iter->GetSecond().GetId() == second) {
      each_iter = all_edges.erase(each_iter);
    } else {
      each_iter++;
    }
  }
}
template <edge_c Edge_>
const list<Edge_>& LinkListEdgeAllocator<Edge_>::GetEdges(Index node) {  //?
  return edges_[node];
}
template <edge_c Edge_>
void LinkListEdgeAllocator<Edge_>::DeleteNode(Index node) {
  auto& all_edges = edges_[node];
  all_edges.clear();
  for (auto& each_all_edges : edges_) {
    auto each_iter = each_all_edges.begin();
    while (each_iter != each_all_edges.end()) {
      if (each_iter->GetSecond().GetId() == node) {
        each_iter = each_all_edges.erase(each_iter);
      } else {
        each_iter++;
      }
    }
  }
}
template <edge_c Edge_>
void LinkListEdgeAllocator<Edge_>::Reserve(size_t size) {
  if (size < edges_.size()) {
    return;
  }
  edges_.reserve(size);
}
}  // namespace lhy
#endif  // MY_STL_LINKLIST_EDGE_ALLOCATOR_H