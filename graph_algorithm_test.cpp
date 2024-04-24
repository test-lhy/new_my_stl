//
// Created by lhy on 24-4-12.
//
#include "base_graph_algorithm.h"
using namespace lhy;
using std::cout, std::endl;
int main() {
  DGE<double, Node> a;
  a.AddNode(1);
  a.AddNode(2);
  a.AddNode(3);
  a.AddNode(4);
  a.AddNode(5);
  a.AddEdge(WeightedEdge<double>{6, a[1].GetNode(), a[2].GetNode()});
  a.AddEdge(WeightedEdge<double>{5, a[1].GetNode(), a[4].GetNode()});
  a.AddEdge(WeightedEdge<double>{7, a[2].GetNode(), a[3].GetNode()});
  a.AddEdge(WeightedEdge<double>{9, a[2].GetNode(), a[4].GetNode()});
  a.AddEdge(WeightedEdge<double>{4, a[3].GetNode(), a[5].GetNode()});
  a.AddEdge(WeightedEdge<double>{8, a[4].GetNode(), a[3].GetNode()});
  a.AddEdge(WeightedEdge<double>{3, a[5].GetNode(), a[3].GetNode()});
  for (auto [index, dis] : Dijkstra(&a, 1)) {
    cout << index << " " << dis << endl;
  }
  cout << endl;
}