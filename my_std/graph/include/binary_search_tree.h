//
// Created by lhy on 24-9-16.
//

#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H
#include "optional.h"
#include "trees.h"

namespace lhy {
template <typename T>
class BinarySearchTreeNode {
 public:
  BinarySearchTreeNode() = default;
  BinarySearchTreeNode(const T& data) : data_(data), count_(1) {}
  BinarySearchTreeNode(T&& data) : data_(std::move(data)), count_(1) {}
  BinarySearchTreeNode& operator=(const T& data) {
    if (data_ == data) {
      count_++;
    } else {
      throw logic_error("?");
    }
    return *this;
  }
  friend auto operator<=>(const BinarySearchTreeNode& lhs, const BinarySearchTreeNode& rhs) {
    return lhs.data_ <=> rhs.data_;
  }
  friend auto operator<=>(const T& lhs, const BinarySearchTreeNode& rhs) { return lhs <=> rhs.data_; }
  friend auto operator<=>(const BinarySearchTreeNode& lhs, const T& rhs) { return lhs.data_ <=> rhs; }
  friend auto operator==(const BinarySearchTreeNode& lhs, const BinarySearchTreeNode& rhs) {
    return lhs.data_ == rhs.data_;
  }
  friend auto operator==(const T& lhs, const BinarySearchTreeNode& rhs) { return lhs == rhs.data_; }
  friend auto operator==(const BinarySearchTreeNode& lhs, const T& rhs) { return lhs.data_ == rhs; }

 private:
  T data_;
  int count_{};
};
template <typename T>
class BinarySearchTree : public Trees<WeightedNode<optional<BinarySearchTreeNode<T>>>> {
 public:
  using Base = Trees<WeightedNode<optional<BinarySearchTreeNode<T>>>>;
  BinarySearchTree() = default;
  ~BinarySearchTree() override = default;
  void insert(const T& data);
  optional<Index> findImpl(const T& data);
  optional<Index> find(const T& data);
  Index root();

 private:
  int max_index_{};
};
template <typename T>
void BinarySearchTree<T>::insert(const T& data) {
  if (auto cur = findImpl(data); !cur.has_value()) {
    Base::AddRoot({data, ++max_index_});
    Base::AddNode({{}, ++max_index_}, root());
    Base::AddNode({{}, ++max_index_}, root());
  } else {
    auto& cur_node_data = Base::GetNode(*cur).GetNode().GetWeight();
    cur_node_data = data;
    Base::AddNode({{}, ++max_index_}, *cur);
    Base::AddNode({{}, ++max_index_}, *cur);
  }
}
template <typename T>
optional<Index> BinarySearchTree<T>::findImpl(const T& data) {
  if (Base::empty()) {
    return {};
  }
  Index cur = root();
  while (true) {
    auto& cur_node_data = Base::GetNode(cur).GetNode().GetWeight();
    if (!cur_node_data.has_value()) {
      return cur;
    }
    if (data < Base::GetNode(cur).GetNode().GetWeight()) {
      cur = Base::GetNode(cur).GetEdges().begin()->GetAnotherNode(cur);
    } else if (data > Base::GetNode(cur).GetNode().GetWeight()) {
      cur = Base::GetNode(cur).GetEdges().begin().Next()->GetAnotherNode(cur);
    } else {
      return cur;
    }
  }
}
template <typename T>
optional<Index> BinarySearchTree<T>::find(const T& data) {
  if (auto cur = findImpl(data); Base::GetNode(*cur).GetNode().GetWeight().has_value()) {
    return cur;
  }
  return {};
}
template <typename T>
Index BinarySearchTree<T>::root() {
  return *Base::roots_.begin();
}

}  // namespace lhy

#endif  // BINARY_SEARCH_TREE_H
