//
// Created by lhy31 on 2023/9/4.
//

#ifndef MY_STL_GRAPH_H
#define MY_STL_GRAPH_H
#include <memory>

#include "vector.h"
//warning:总感觉used并没有派上什么用场
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
    bool operator==(const edge& other) { return this->weight_ == other.weight_ && this->from_ == other.from_ && this->to_; }
    bool operator!=(const edge& other) { return !(*this == other); }
  };
  class node {
   public:
    Index index_{};
    T content_;
    bool used_;
    vector<std::shared_ptr<edge>> next_;
    vector<std::shared_ptr<edge>> prev_;
    node(){
      used_=false;
    }
    explicit node(Index index){
      index_=index;
      used_=false;
    }
    ~node(){
      std::cerr<<"releasing node"<<"\n";
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

 protected:
  vector<node*> nodes_;
 private:
  size_t reserved_size_;
  size_t size_;
};
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
  nodes_[index]->used_= false;
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
  if (from >= nodes_.size() || to >= nodes_.size()) {
    throw std::out_of_range("from or to is out of range");
  }
  auto edge_temp=std::make_shared<edge>(weight, from, to);
  nodes_[from]->next_.push_back(edge_temp);
  nodes_[to]->prev_.push_back(edge_temp);
  if (from==to){
    return;
  }
  auto edge_temp_other=std::make_shared<edge>(weight, to, from);
  nodes_[from]->prev_.push_back(edge_temp_other);
  nodes_[to]->next_.push_back(edge_temp_other);
}
template <typename T, typename WeightType>
size_t Graph<T, WeightType>::size() const {
  return size_;
}
template <typename T, typename WeightType>
void Graph<T, WeightType>::AddNode(const T& content, Index index) {
  while(index>=reserved_size_){
    node* node_temp=new node(reserved_size_);
    nodes_.push_back(node_temp);
    reserved_size_++;
  }
  if (!nodes_[index]->used_){
    size_++;
    nodes_[index]->used_= true;
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