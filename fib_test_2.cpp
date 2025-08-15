#include <ctime>
#include <vector>

#include "FibTree.h"
//
// Created by lhy on 24-9-15.
//
using namespace lhy;
void test_fibon(std::vector<int> &test) {
  clock_t start, end;
  start = clock();
  FibTree<int> now;
  for (int i : test) {
    now.push(i);
  }
  std::cout << "fibon: " << std::endl;
  std::cout << "Extracting values: ";
  while (!now.empty()) {
    // std::cout << now.top() << std::endl;
    now.pop();
  }
  std::cout << std::endl;
  end = clock();
  std::cout << "total time: " << end - start << std::endl;
}
int main() {
  std::vector<int> v;
  for (int i = 0; i < 100; i++) {
    v.push_back(i);
  }
  test_fibon(v);
  return 0;
}
