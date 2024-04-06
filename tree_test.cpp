//
// Created by lhy on 24-4-2.
//
#include "trees.h"
using namespace lhy;
int main() {
  Trees<> a;
  a.AddRoot(Node{0});
  a.AddNode(Node{1}, 0);
  a.AddNode(Node{2}, 0);
  a.ChangeFather(1, 2);
}