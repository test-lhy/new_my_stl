//
// Created by lhy on 23-9-18.
//

#ifndef MY_STL_DIRECTED_GRAPH_H
#define MY_STL_DIRECTED_GRAPH_H
#include "graph.h"
namespace lhy{
template <typename T, typename WeightType = int64_t>
class directed_graph:public Graph<T,WeightType>{
 public:
  using edge = typename Graph<T,WeightType>::edge;
  directed_graph()=default;
  ~directed_graph()=default;
  void AddEdge(Index from, Index to, const WeightType& weight=1) override ;
 private:
  using Graph<T,WeightType>::nodes_;///<using必须写在类声明内
};
template <typename T, typename WeightType>
void directed_graph<T, WeightType>::AddEdge(Index from, Index to, const WeightType& weight) {
  if (from >= nodes_.size() || to >= nodes_.size()) {
    throw std::out_of_range("from or to is out of range");
  }
  auto edge_temp=std::make_shared<edge>(weight, from, to);
  nodes_[from]->next_.push_back(edge_temp);
  nodes_[to]->prev_.push_back(std::move(edge_temp));
}
}
#endif  // MY_STL_DIRECTED_GRAPH_H