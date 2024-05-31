//
// Created by lhy31 on 2024/1/8
//
#include "edge.h"
#include "edge_allocator.h"
#include "list.h"
#ifndef MY_STL_MAP_EDGE_ALLOCATOR_H
#define MY_STL_MAP_EDGE_ALLOCATOR_H
namespace lhy {
template <edge_c Edge_>
class MapEdgeAllocator : public EdgeAllocator<Edge_> {
 public:
  void AddEdge(const Edge_& edge) override;
  bool Exist(const Edge_& edge) override;
  void DeleteEdge(const Edge_& edge) override;
  void DeleteEdges(Index first, Index second) override;
  const list<Edge_>& GetEdges(Index node) override;
  void DeleteNode(Index node) override;
  ~MapEdgeAllocator() override = default;

 private:
  std::map<std::pair<Index, Index>, list<Edge_>> edges_;
  std::map<Index, list<Edge_>> vec_edges_;
  list<Edge_> no_edges_;
};
template <edge_c Edge_>
void MapEdgeAllocator<Edge_>::AddEdge(const Edge_& edge) {
  edges_[{edge.GetFirst(), edge.GetSecond()}].push_back(edge);
  vec_edges_[edge.GetFirst()].push_back(edge);
}

template <edge_c Edge_>
bool MapEdgeAllocator<Edge_>::Exist(const Edge_& edge) {
  std::pair<Index, Index> place = {edge.GetFirst(), edge.GetSecond()};
  if (edges_.find(place) == edges_.end()) {
    return false;
  }
  return find(edges_[place].begin(), edges_[place].end(), edge) != edges_[place].end();
}
template <edge_c Edge_>
void MapEdgeAllocator<Edge_>::DeleteEdge(const Edge_& edge) {
  if (Exist(edge)) {
    std::pair<Index, Index> place = {edge.GetFirst(), edge.GetSecond()};
    edges_[place].erase(edge);
    vec_edges_[edge.GetFirst()].erase(edge);
  }
}
template <edge_c Edge_>
void MapEdgeAllocator<Edge_>::DeleteEdges(Index first, Index second) {
  auto& each_list = vec_edges_[first];
  auto each_iter = each_list.begin();
  while (each_iter != each_list.end()) {
    if (each_iter->GetSecond() == second) {
      if (Exist(*each_iter)) {
        edges_.erase({each_iter->GetFirst(), each_iter->GetSecond()});
      }
      each_iter = each_list.erase(each_iter);
    } else {
      each_iter++;
    }
  }
  edges_[{first, second}].clear();
}
template <edge_c Edge_>
const list<Edge_>& MapEdgeAllocator<Edge_>::GetEdges(Index node) {
  if (vec_edges_.find(node) == vec_edges_.end()) {
    return no_edges_;
  }
  return vec_edges_[node];
}
template <edge_c Edge_>
void MapEdgeAllocator<Edge_>::DeleteNode(Index node) {
  for (auto& each : vec_edges_) {
    auto& each_list = each.second;
    auto each_iter = each_list.begin();
    while (each_iter != each_list.end()) {
      if (each_iter->GetFirst() == node || each_iter->GetSecond() == node) {
        if (Exist(*each_iter)) {
          edges_.erase({each_iter->GetFirst(), each_iter->GetSecond()});
        }
        each_iter = each_list.erase(each_iter);
      } else {
        each_iter++;
      }
    }
  }
  // todo:是不是应该反记一下
  for (auto& [key, content] : edges_) {
    if (key.first == node || key.second == node) {
      content.clear();
    }
  }
}
}  // namespace lhy
#endif  // MY_STL_MAP_EDGE_ALLOCATOR_H