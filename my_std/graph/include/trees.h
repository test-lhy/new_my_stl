//
// Created by lhy on 24-3-29.
//

#ifndef TREES_H
#define TREES_H
#include "algorithm.h"
#include "base_graph.h"
#include "treenode.h"
namespace lhy {
template <node_c Origin_Node_ = Node, edge_c Edge_ = Edge>
class Trees : protected BaseGraph<TreeNode<Origin_Node_>, Edge_> {
  using Node_ = TreeNode<Origin_Node_>;
  using Base = BaseGraph<Node_, Edge_>;
  using BaseReturnNode = typename Base::return_node;

 public:
  explicit Trees(NodeAllocatorMode node_allocator_mode = NodeAllocatorMode::NormalNodeAllocatorMode,
                 EdgeAllocatorMode edge_allocator_mode = EdgeAllocatorMode::LinkListEdgeAllocatorMode)
      : Base(node_allocator_mode, edge_allocator_mode) {}
  class return_node : public BaseReturnNode {
   public:
    using Base::return_node::return_node;
    return_node(const BaseReturnNode& base) : BaseReturnNode(base) {}
    return_node GetFather() { return BaseReturnNode::GetGraph()->operator[](GetFatherIndex()); }
    Index GetFatherIndex() { return BaseReturnNode::GetNode().GetFather(); }
    [[nodiscard]] bool IsRoot() { return (GetFatherIndex() == -1); };
  };
  void AddRoot(Origin_Node_&& node);
  template <typename... Args>
  void AddNode(Origin_Node_&& node, Index father, Args&&... args);
  template <typename... Args>
  void ChangeFather(Index node, Index father, Args&&... args);
  template <typename... Args>
  void ChangeFatherWithCheck(Index node, Index father, Args&&... args);
  // note:如果需要返回值，形参不同，但名字相同的子类函数，那么直接将基类的不设为virtual即可，直接通过作用域的范围进行一个隐藏
  return_node operator[](Index node) { return Base::operator[](node); }
  return_node GetNode(Index node) { return Base::GetNode(node); }
  [[nodiscard]] bool IsFather(Index father, Index node);
  void CheckNodeExist(Index node) const;
  void SetRoot(Index node);

 private:
  std::set<Index> roots_{};
};

template <node_c Origin_Node_, edge_c Edge_>
void Trees<Origin_Node_, Edge_>::AddRoot(Origin_Node_&& node) {
  roots_.insert(Base::AddNode(
      std::forward<Node_>(Node_{std::forward<Origin_Node_>(node), std::forward<std::optional<Index>>({})})));
}
template <node_c Origin_Node_, edge_c Edge_>
template <typename... Args>
void Trees<Origin_Node_, Edge_>::AddNode(Origin_Node_&& node, Index father, Args&&... args) {
  if (!Base::NodeExist(father)) {
    throw std::logic_error("node's father should have been created.");
  }
  auto node_now =
      Base::AddNode(std::forward<Node_>(Node_{std::forward<Origin_Node_>(node), std::forward<Index>(father)}));
  Base::AddEdge(GetNode(node_now).GetFatherIndex(), node_now, std::forward<Args>(args)...);
}
template <node_c Origin_Node_, edge_c Edge_>
template <typename... Args>
void Trees<Origin_Node_, Edge_>::ChangeFather(Index node, Index father, Args&&... args) {
  CheckNodeExist(node);
  CheckNodeExist(father);
  if (GetNode(node).IsRoot()) {
    roots_.erase(node);
  }
  Base::DeleteEdges(GetNode(node).GetFatherIndex(), node);
  GetNode(node).GetNode().SetFather(father);
  Base::AddEdge(father, node, std::forward<Args>(args)...);
}
template <node_c Origin_Node_, edge_c Edge_>
template <typename... Args>
void Trees<Origin_Node_, Edge_>::ChangeFatherWithCheck(Index node, Index father, Args&&... args) {
  if (IsFather(node, father)) {
    throw std::logic_error("node's kid can't be node's father");
  }
  ChangeFather(node, father, std::forward<Args>(args)...);
}
template <node_c Origin_Node_, edge_c Edge_>
bool Trees<Origin_Node_, Edge_>::IsFather(Index father, Index node) {
  while (!GetNode(node).IsRoot()) {
    if (node == father) {
      return true;
    }
    node = GetNode(node).GetFatherIndex();
  }
  if (node == father) {
    return true;
  }
  return false;
}
template <node_c Origin_Node_, edge_c Edge_>
void Trees<Origin_Node_, Edge_>::CheckNodeExist(Index node) const {
  if (!Base::NodeExist(node)) {
    const string error_string = static_cast<string>("node") + ToString(node) +
                                static_cast<string>(" being used, so it should have been created");
    throw std::logic_error(str(error_string));
  }
}
template <node_c Origin_Node_, edge_c Edge_>
void Trees<Origin_Node_, Edge_>::SetRoot(Index node) {
  CheckNodeExist(node);
  if (GetNode(node).IsRoot()) {
    return;
  }
  Base::DeleteEdges(GetNode(node).GetFatherIndex(), node);
  GetNode(node).GetNode().SetFather({});
  roots_.insert(node);
}
}  // namespace lhy

#endif  // TREES_H
