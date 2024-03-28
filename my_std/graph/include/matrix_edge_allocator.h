//
// Created by lhy31 on 2024/1/8
//
#include "edge.h"
#include "edge_allocator.h"
#include"algorithm.h"
#ifndef MY_STL_MATRIX_EDGE_ALLOCATOR_H
#define MY_STL_MATRIX_EDGE_ALLOCATOR_H
namespace lhy {
template <edge_c Edge_>
class MatrixEdgeAllocator : public EdgeAllocator<Edge_> {
 public:
  void AddEdge(const Edge_& edge) override;
  bool Exist(const Edge_& edge) override;
  void DeleteEdge(const Edge_& edge) override;
  list<Edge_>& GetEdges(Index node) override;
  void DeleteNode(Index node) override;
  void Reserve(size_t size) override;
  ~MatrixEdgeAllocator() override=default;

 private:
  vector<vector<list<Edge_>>> edges_;
  list<Edge_> edge_asked_;
};
template <edge_c Edge_>
void MatrixEdgeAllocator<Edge_>::AddEdge(const Edge_& edge) {
  edges_[edge.GetFirst().GetId()][edge.GetSecond().GetId()].push_back(edge);
}
template <edge_c Edge_>
bool MatrixEdgeAllocator<Edge_>::Exist(const Edge_& edge) {
  list<Edge_>& all_edges = edges_[edge.GetFirst().GetId()][edge.GetSecond().GetId()];
  return find(all_edges.begin(), all_edges.end(), edge) == all_edges.end();
}
template <edge_c Edge_>
void MatrixEdgeAllocator<Edge_>::DeleteEdge(const Edge_& edge) {
  list<Edge_>& all_edges = edges_[edge.GetFirst().GetId()][edge.GetSecond().GetId()];
  if (!all_edges.empty()) {
    all_edges.erase(edge);
  }
}
template <edge_c Edge_>
list<Edge_>& MatrixEdgeAllocator<Edge_>::GetEdges(Index node) {
  edge_asked_.clear();
  for (auto& each_vec : edges_[node]) {
    for (auto& each : each_vec) {
      edge_asked_.push_back(each);
    }
  }
  return edge_asked_;
}
template <edge_c Edge_>
void MatrixEdgeAllocator<Edge_>::DeleteNode(Index node) {
  for (int i = 0; i < edges_.size(); i++) {
    edges_[node][i].clear();
    edges_[i][node].clear();
  }
}
template <edge_c Edge_>
void MatrixEdgeAllocator<Edge_>::Reserve(size_t size) {
  if (size < edges_.size()) {
    return;
  }
  edges_.reserve(size);
  for (auto& each : edges_) {
    each.reserve(size);
  }
}

}  // namespace lhy
#endif  // MY_STL_MATRIX_EDGE_ALLOCATOR_H