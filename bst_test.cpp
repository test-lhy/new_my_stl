//
// Created by lhy on 24-9-16.
//
#include "binary_search_tree.h"
#include <cassert>

int main() {
  using namespace lhy;

  // Create a Binary Search Tree
  BinarySearchTree<int> bst;

  // Insert some values
  bst.insert(10);
  bst.insert(5);
  bst.insert(15);
  bst.insert(10); // Duplicate insertion, should update the count

  // Assert that values exist in the tree
  assert(bst.find(10).has_value() && "10 should be found in the tree");
  assert(bst.find(5).has_value() && "5 should be found in the tree");
  assert(bst.find(15).has_value() && "15 should be found in the tree");

  // Assert that a value not in the tree returns no result
  assert(!bst.find(20).has_value() && "20 should not be found in the tree");

  // Optional: Add more tests for other cases, like tree structure or node count

  return 0;
}
