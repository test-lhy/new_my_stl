//
// Created by lhy on 24-3-25.
//

#ifndef WEIGHTED_EDGE_H
#define WEIGHTED_EDGE_H
#include "edge.h"
namespace lhy {
template <typename WeightType>
class WeightedEdge : public Edge {
 public:
  WeightedEdge() = default;
  WeightedEdge(Node& first, Node& second, WeightType&& weight) : Edge(first, second), weight_(weight){};
  friend bool operator==(const WeightedEdge& lhs, const WeightedEdge& rhs) {
    return static_cast<const Edge&>(lhs) == static_cast<const Edge&>(rhs) && lhs.weight_ == rhs.weight_;
  }
  friend bool operator!=(const WeightedEdge& lhs, const WeightedEdge& rhs) { return !(lhs == rhs); }

  WeightType weight_{};
};

}  // namespace lhy

#endif  // WEIGHTED_EDGE_H
