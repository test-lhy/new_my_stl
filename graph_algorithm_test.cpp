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
  a.AddEdge(6, 1, 2);
  a.AddEdge(5, 1, 4);
  a.AddEdge(7, 2, 3);
  a.AddEdge(9, 2, 4);
  a.AddEdge(4, 3, 5);
  a.AddEdge(8, 4, 3);
  a.AddEdge(3, 5, 3);
  for (auto [index, dis] : Dijkstra<double>(&a, 1)) {
    cout << index << " " << dis << endl;
  }
  cout << endl;
}