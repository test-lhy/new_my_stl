#include "directed_graph.h"
#include <iostream>

using namespace lhy;

int main() {

  directed_graph<int, double> g;

  // Add some nodes
  g.AddNode(1);//0 1
  std::cout << "Added node 1, size: " << g.size() << "\n"; // 1

  g.AddNode(2);//1 2
  std::cout << "Added node 2, size: " << g.size() << "\n"; // 2

  g.AddNode(3, 0);//0 3
  std::cout << "Added node 3 at 0, size: " << g.size() << "\n"; // 3

  // Add edges
  g.AddEdge(0, 1, 1.5);

  // Print out edges
  for (auto& edge : g[0].next_) {
    std::cout << g[edge->from_].index_ << " -> "
              << g[edge->to_].index_ << " : "
              << edge->weight_ << "\n";
  }

  // Output
  // 3 -> 1 : 1.5
  g.AddNode(33);
  g.AddEdge(1, 2, 2.5);

  // Print out edges
  for (auto& edge : g[1].next_) {
    std::cout << g[edge->from_].index_ << " -> "
              << g[edge->to_].index_ << " : "
              << edge->weight_ << "\n";
  }

  // Output
  // 1 -> 2 : 2.5

  // Get node
  auto& node1 = g.GetNode(1);
  std::cout << "Node 1 content: " << node1.index_ << "\n"; // 2

  // Indexing operator
  auto& node0 = g[0];
  std::cout << "Node 0 content: " << node0.index_ << "\n"; // 3

  // Erase node
  g.EraseNode(0);
  std::cout << "Erased node 0, size: " << g.size() << "\n"; // 2

  // Clear graph
  g.clear();
  std::cout << "Cleared graph, size: " << g.size() << "\n"; // 0

  // Add node with index
  g.AddNode(5, 10);
  std::cout << "Added node 5 at 10, size: " << g.size() << "\n"; // 1

  // Add a few more nodes
  g.AddNode(6);
  std::cout << "Added node 6, size: " << g.size() << "\n"; // 2

  g.AddNode(7);
  std::cout << "Added node 7, size: " << g.size() << "\n"; // 3

  // Add edges
  g.AddEdge(10, 11, 1.2);

  // Print out edges
  for (auto& edge : g[10].next_) {
    std::cout << g[edge->from_].index_ << " -> "
              << g[edge->to_].index_ << " : "
              << edge->weight_ << "\n";
  }

  // Output
  // 5 -> 6 : 1.2

  g.AddEdge(0, 1, 3.4);

  // Print out edges
  for (auto& edge : g[0].next_) {
    std::cout << g[edge->from_].index_ << " -> "
              << g[edge->to_].index_ << " : "
              << edge->weight_ << "\n";
  }

  // Output
  // 6 -> 7 : 3.4

  // Get size
  std::cout << "Size: " << g.size() << "\n"; // 3

  return 0;
}