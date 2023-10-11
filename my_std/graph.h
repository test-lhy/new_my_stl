//
// Created by lhy31 on 2023/9/4.
//

#ifndef MY_STL_GRAPH_H
#define MY_STL_GRAPH_H
#include <memory>

#include "vector.h"
//todo:感觉应该写一个graph的抽象基类，undirected_graph和directed_graph和tree都继承于这个类,整体需要大重构
// warning:总感觉used并没有派上什么用场
namespace lhy {
template <typename T, typename WeightType = int64_t>
class Graph {
 public:
  class node;
  class edge {
   public:
    WeightType weight_;
    Index from_;
    Index to_;
    edge(const WeightType& weight, Index from, Index to) {
      this->weight_ = weight;
      this->from_ = from;
      this->to_ = to;
    }
    edge() {
      this->weight_ = 0;
      this->from_ = -1;
      this->to_ = -1;
    }
    bool operator==(const edge& other) {
      return this->weight_ == other.weight_ && this->from_ == other.from_ && this->to_;
    }
    bool operator!=(const edge& other) { return !(*this == other); }
  };
  class node {
   public:
    Index index_{};
    T content_;
    bool used_;
    vector<std::shared_ptr<edge>> next_;
    vector<std::shared_ptr<edge>> prev_;
    node() { used_ = false; }
    explicit node(Index index) {
      index_ = index;
      used_ = false;
    }
  };
  Graph();
  ~Graph();
  void AddNode(const T& content);
  void AddNode(const T& content, Index index);
  void EraseNode(Index index);
  virtual void AddEdge(Index from, Index to, const WeightType& weight = 1);
  void clear();
  size_t size() const;
  node& GetNode(Index);
  node& operator[](Index);
  void AddEdge(Index from, Index to, std::shared_ptr<edge> edge_from_to, std::shared_ptr<edge> edge_to_from);

 protected:
  virtual node* GetNodes(size_t);
  vector<node*> nodes_;

 private:
  size_t reserved_size_;
  size_t size_;
};
template <typename T, typename WeightType>
typename Graph<T, WeightType>::node* Graph<T, WeightType>::GetNodes(size_t size) {
  return new node(size);
}
template <typename T, typename WeightType>
typename Graph<T, WeightType>::node& Graph<T, WeightType>::operator[](Index index) {
  return *nodes_[index];
}
template <typename T, typename WeightType>
typename Graph<T, WeightType>::node& Graph<T, WeightType>::GetNode(Index index) {
  return *nodes_[index];
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
  nodes_[index]->used_ = false;
  for (const auto& edge : nodes_[index]->next_) {
    nodes_[edge->to_]->prev_.erase(edge);
  }
  for (const auto& edge : nodes_[index]->prev_) {
    nodes_[edge->from_]->next_.erase(edge);
  }
  nodes_[index]->next_.clear();
  nodes_[index]->prev_.clear();
}
template <typename T, typename WeightType>
void Graph<T, WeightType>::AddEdge(Index from, Index to, const WeightType& weight) {
  AddEdge(from, to, std::make_shared<edge>(weight, from, to), std::make_shared<edge>(weight, to, from));
}
template <typename T, typename WeightType>
void Graph<T, WeightType>::AddEdge(Index from, Index to, std::shared_ptr<edge> edge_from_to,
                                   std::shared_ptr<edge> edge_to_from) {
  if (from >= nodes_.size() || to >= nodes_.size()) {
    throw std::out_of_range("from or to is out of range");
  }
  nodes_[from]->next_.push_back(edge_from_to);
  nodes_[to]->prev_.push_back(edge_from_to);
  if (from == to) {
    return;
  }
  nodes_[from]->prev_.push_back(edge_to_from);
  nodes_[to]->next_.push_back(edge_to_from);
}
template <typename T, typename WeightType>
size_t Graph<T, WeightType>::size() const {
  return size_;
}
template <typename T, typename WeightType>
void Graph<T, WeightType>::AddNode(const T& content, Index index) {
  while (index >= reserved_size_) {
    // 创建一个函数来返回node
    node* node_temp = GetNodes(reserved_size_);
    nodes_.push_back(node_temp);
    reserved_size_++;
  }
  if (!nodes_[index]->used_) {
    size_++;
    nodes_[index]->used_ = true;
  }
  nodes_[index]->content_ = content;
  nodes_[index]->index_ = index;
}
template <typename T, typename WeightType>
void Graph<T, WeightType>::AddNode(const T& content) {
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