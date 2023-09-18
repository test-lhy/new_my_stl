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
    WeightType weight_;
    node* from_;
    node* to_;
    edge(WeightType weight, node* from, node* to) {
      this->weight_ = weight;
      this->from_ = from;
      this->to_ = to;
    }
    edge() {
      this->weight_ = 0;
      this->from_ = nullptr;
      this->to_ = nullptr;
    }
    bool operator==(const edge& other) { return this->weight_ == other.weight_ && this->from_ == other.from_ && this->to_; }
    bool operator!=(const edge& other) { return !(*this == other); }
  };
  struct node {
    Index index_;
    T content_;
    bool used_;
    vector<edge> next_;
    vector<edge> prev_;
    node(){
      used_=false;
    }
    node(Index index){
      index_=index;
      used_=false;
    }
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
  reserved_size_ = 0;
}
template <typename T, typename WeightType>
void Graph<T, WeightType>::EraseNode(Index index) {
  size_--;
  nodes_[index].used_= false;
  for (auto& edge : nodes_[index].prev_) {
    edge.to_->next_.erase(edge);
  }
  for (auto& edge : nodes_[index].next_) {
    edge.from_->prev_.erase(edge);
  }
}
template <typename T, typename WeightType>
void Graph<T, WeightType>::AddEdge(T from, T to, WeightType weight) {
  if (from >= nodes_.size() || to >= nodes_.size()) {
    throw std::out_of_range("from or to is out of range");
  }
  edge edge_temp(weight, &nodes_[from], &nodes_[to]);
  nodes_[from].next_.push_back(edge_temp);
  nodes_[to].prev_.push_back(std::move(edge_temp));
  edge edge_temp_other(weight, &nodes_[to], &nodes_[from]);
  nodes_[from].prev_.push_back(edge_temp_other);
  nodes_[to].next_.push_back(std::move(edge_temp_other));
}
template <typename T, typename WeightType>
size_t Graph<T, WeightType>::size() const {
  return size_;
}
template <typename T, typename WeightType>
void Graph<T, WeightType>::AddNode(T content, Index index) {
  while(index>=reserved_size_){
    nodes_.push_back(std::move(node(reserved_size_)));
    reserved_size_++;
  }
  if (!nodes_[index].used_){
    size_++;
    nodes_[index].used_= true;
  }
  nodes_[index].content_ = content;
  nodes_[index].index_ = index;
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