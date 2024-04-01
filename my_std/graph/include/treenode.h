//
// Created by lhy on 24-3-29.
//

#ifndef TREENODE_H
#define TREENODE_H
#include "node.h"
namespace lhy {

class TreeNode : public Node {
 public:
  TreeNode() = default;
  explicit TreeNode(Index&& id, Node* father = nullptr) : Node(std::forward<Index>(id)), father_(father) {}
  ~TreeNode() override = default;
  friend bool operator==(const TreeNode& lhs, const TreeNode& rhs) {
    return static_cast<const Node&>(lhs) == static_cast<const Node&>(rhs) && lhs.father_ == rhs.father_;
  }
  friend bool operator!=(const TreeNode& lhs, const TreeNode& rhs) { return !(lhs == rhs); }
  [[nodiscard]] Node& GetFather() const { return *father_; }

 private:
  Node* father_{};
};
template <typename T>
concept tree_node_c = std::is_base_of_v<TreeNode, T>;
template <typename WeightType>
using WeightedTreeNode = Weighted<WeightType, TreeNode>;
}  // namespace lhy

#endif  // TREENODE_H
