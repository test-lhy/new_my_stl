//
// Created by lhy31 on 2024/1/8
//

#ifndef MY_STL_EDGE_H
#define MY_STL_EDGE_H
#include <type_traits>

#include "basic.h"
#include "weighted.h"
namespace lhy {
class Node;
class Edge {
 public:
  Edge() = default;
  Edge(const Index first, const Index second) : first_(first), second_(second) {}
  virtual ~Edge() = default;
  virtual bool operator==(const Edge& other) const { return first_ == other.first_ && second_ == other.second_; }
  bool operator!=(const Edge& other) const { return !(*this == other); }
  [[nodiscard]] Index GetAnotherNode(const Index other) const { return ((other == first_) ? second_ : first_); }
  [[nodiscard]] Index GetFirst() const { return first_; }
  [[nodiscard]] Index GetSecond() const { return second_; }
  [[nodiscard]] Edge GetReversed() const {
    Edge zan = *this;
    zan.first_ = second_;
    zan.second_ = first_;
    return zan;
  }

 private:
  Index first_;
  Index second_;
};
template <typename T>
concept edge_c = std::is_base_of_v<Edge, T>;
template <typename WeightType>
using WeightedEdge = Weighted<WeightType, Edge>;
}  // namespace lhy
#endif  // MY_STL_EDGE_H