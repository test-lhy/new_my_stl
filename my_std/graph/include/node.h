//
// Created by lhy31 on 2024/1/8
//

#ifndef MY_STL_NODE_H
#define MY_STL_NODE_H
#include <concepts>
#include <type_traits>

#include "basic.h"
#include "weighted.h"

namespace lhy {
class Node {
 public:
  Node() = default;
  explicit Node(Index&& id) : id_(id){};
  virtual ~Node() = default;
  virtual bool operator==(const Node& other) const { return id_ == other.id_; }
  bool operator!=(const Node& other) const { return !(*this == other); }
  [[nodiscard]] Index GetId() const { return id_; }

 private:
  Index id_{};
};

template <typename T>
concept node_c = std::is_base_of_v<Node, T>;
template <typename WeightType>
using WeightedNode = Weighted<WeightType, Node>;
}  // namespace lhy
#endif  // MY_STL_NODE_H
