//
// Created by lhy on 24-3-29.
//

#ifndef TREENODE_H
#define TREENODE_H
#include "node.h"
namespace lhy {
template <node_c Node_>
class TreeNode : public Node_ {
 public:
  TreeNode() = default;
  TreeNode(Node_&& node, std::optional<Index>&& father)
      : Node_(std::forward<Node_>(node)), father_(std::forward<std::optional<Index>>(father)) {}
  ~TreeNode() override = default;
  friend bool operator==(const TreeNode& lhs, const TreeNode& rhs) {
    return static_cast<const Node_&>(lhs) == static_cast<const Node_&>(rhs) && lhs.father_ == rhs.father_;
  }
  friend bool operator!=(const TreeNode& lhs, const TreeNode& rhs) { return !(lhs == rhs); }
  [[nodiscard]] Index GetFather() const { return father_.value_or(-1); }
  void SetFather(std::optional<Index>&& father) { father_ = father; }

 private:
  std::optional<Index> father_{};
};
template <typename WeightType>
using WeightedTreeNode = Weighted<WeightType, TreeNode<Node>>;
}  // namespace lhy

#endif  // TREENODE_H
