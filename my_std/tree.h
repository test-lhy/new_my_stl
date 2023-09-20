//
// Created by lhy on 23-9-20.
//

#ifndef MY_STL_TREE_H
#define MY_STL_TREE_H
#include "graph.h"
#include <optional>
namespace lhy{
template<typename T,typename WeightType=int64_t>
class tree{
 private:
  Graph<T,WeightType> graph_;
 public:
  using Graphnode = typename Graph<T,WeightType>::node;
  using edge = typename Graph<T,WeightType>::edge;
  struct node{
    Index index_;
    T content_;
    vector<edge> kids_;
    Index father_;
    //todo:改为引用
    void set(Graphnode& graph_node,Index father){
        index_= graph_node.index_;
        content_=graph_node.content_;
        kids_=graph_node.next_;
        father_=father;
    };
  };
  explicit tree(T content=1,Index root=0);
  ~tree()=default;
  node& operator[](Index index);
  void AddNode(Index,const T&,Index index=kwithout_father,const WeightType& weight=1);
  void EraseNode(Index);
  size_t size() const;
  static const Index kwithout_father;
  void set_root(Index);
  Index get_root() const;
 private:
  void FreshNode(Index);
  vector<Index> fathers_;
  Index root_;
  node current_node_;
};
template <typename T, typename WeightType>
tree<T, WeightType>::tree(T content, Index root) {
  while(fathers_.size()<root){
    fathers_.push_back(kwithout_father);
  }
  fathers_.push_back(kwithout_father);
  graph_.AddNode(content,root);
  root_=root;
}
template<typename T,typename WeightType>
const Index tree<T,WeightType>::kwithout_father=-1;
template <typename T, typename WeightType>
void tree<T, WeightType>::FreshNode(Index index) {
  for (auto kid_edge=graph_[index].next_.begin();kid_edge!=graph_[index].next_.end();kid_edge++) {
    auto kid=this->operator[]((*kid_edge).to_).index_;
    if (kid!=fathers_[index]){
      fathers_[kid]=index;
      FreshNode(kid);
    }
  }
}
template <typename T, typename WeightType>
Index tree<T, WeightType>::get_root() const {
  return root_;
}
template <typename T, typename WeightType>
void tree<T, WeightType>::set_root(Index root) {
  FreshNode(root);
  root_=root;
}
template <typename T, typename WeightType>
typename tree<T,WeightType>::node& tree<T, WeightType>::operator[](Index index) {
  current_node_.set(graph_[index],fathers_[index]);
  return current_node_;
}
template <typename T, typename WeightType>
void tree<T, WeightType>::EraseNode(Index index) {
  graph_.EraseNode(index);
  fathers_[index]=kwithout_father;
}
template <typename T, typename WeightType>
size_t tree<T, WeightType>::size() const {
  return graph_.size();
}
template <typename T, typename WeightType>
void tree<T, WeightType>::AddNode(Index father_index, const T& content, Index index, const WeightType& weight) {
  if (index==kwithout_father){
    index=graph_.size();
  }
  graph_.AddNode(content,index);
  graph_.AddEdge(index,father_index,weight);
  while(fathers_.size()<index){
    fathers_.push_back(kwithout_father);
  }
  fathers_.push_back(father_index);
}
}
#endif  // MY_STL_TREE_H