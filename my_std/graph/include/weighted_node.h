//
// Created by lhy on 24-3-25.
//

#ifndef WEIGHTED_NODE_H
#define WEIGHTED_NODE_H
#include "node.h"
namespace lhy {
template <typename WeightType>
class WeightedNode : public Node {
 public:
  WeightedNode() = default;
  WeightedNode(Index&& id, WeightType&& weight) : Node(id), weight_(weight){};
  friend bool operator==(const WeightedNode& lhs, const WeightedNode& rhs) {
    return static_cast<const Node&>(lhs) == static_cast<const Node&>(rhs) && lhs.weight_ == rhs.weight_;
  }
  friend bool operator!=(const WeightedNode& lhs, const WeightedNode& rhs) { return !(lhs == rhs); }

  WeightType weight_{};
};

}  // namespace lhy

#endif  // WEIGHTED_NODE_H
