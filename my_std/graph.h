//
// Created by lhy31 on 2023/9/4.
//

#ifndef MY_STL_GRAPH_H
#define MY_STL_GRAPH_H
#include "vector.h"
namespace lhy {
template <typename T, typename WeightType = int64_t>
class Graph {
 public:
  struct node;
  struct edge {
    WeightType weight;
    node* from;
    node* to;
    edge(WeightType weight, node* from, node* to) {
      this->weight = weight;
      this->from = from;
      this->to = to;
    }
    edge() {
      this->weight = 0;
      this->from = nullptr;
      this->to = nullptr;
    }
    bool operator==(const edge& other) { return this->weight == other.weight && this->from == other.from && this->to; }
    bool operator!=(const edge& other) { return !(*this == other); }
  };
  struct node {
    Index index;
    T content;
    vector<edge> next;
    vector<edge> prev;
    ~node()=default;
  };
  Graph();
  ~Graph();
  void AddNode(T content);
  void AddNode(T content, Index index);
  void EraseNode(Index index);
  void AddEdge(T from, T to, WeightType weight = 1);
  void clear();
  size_t size() const;
  node& GetNode(Index);
  node& operator[](Index);

 private:
  vector<node> nodes_;
  size_t reserved_size_;
  size_t size_;
};
template <typename T, typename WeightType>
typename Graph<T, WeightType>::node& Graph<T, WeightType>::operator[](Index index) {
  return nodes_[index];
}
template <typename T, typename WeightType>
typename Graph<T, WeightType>::node& Graph<T, WeightType>::GetNode(Index index) {
  return nodes_[index];
}
template <typename T, typename WeightType>
void Graph<T, WeightType>::clear() {
  nodes_.clear();
  size_ = 0;
}
template <typename T, typename WeightType>
void Graph<T, WeightType>::EraseNode(Index index) {
  size_--;
  for (auto& edge : nodes_[index].prev) {
    edge.to->next.erase(edge);
  }
  for (auto& edge : nodes_[index].next) {
    edge.from->prev.erase(edge);
  }
}
template <typename T, typename WeightType>
void Graph<T, WeightType>::AddEdge(T from, T to, WeightType weight) {
  if (from >= nodes_.size() || to >= nodes_.size()) {
    throw std::out_of_range("from or to is out of range");
  }
  edge edge(weight, &nodes_[from], &nodes_[to]);
  nodes_[from].next.push_back(edge);
  nodes_[to].prev.push_back(edge);
}
template <typename T, typename WeightType>
size_t Graph<T, WeightType>::size() const {
  return size_;
}
template <typename T, typename WeightType>
void Graph<T, WeightType>::AddNode(T content, Index index) {
  if (index >= reserved_size_) {
    nodes_.reserve(index * 2 + 2);
  }
  nodes_[index].content = content;
  nodes_[index].index = index;
  size_++;
  reserved_size_ = index * 2 + 2;
}
template <typename T, typename WeightType>
void Graph<T, WeightType>::AddNode(T content) {
  AddNode(content, nodes_.size());
}
template <typename T, typename WeightType>
Graph<T, WeightType>::~Graph() = default;
template <typename T, typename WeightType>
Graph<T, WeightType>::Graph() {
  size_ = 0;
  reserved_size_ = 0;
}
}  // namespace lhy
#endif  // MY_STL_GRAPH_H