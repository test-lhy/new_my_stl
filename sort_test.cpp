//
// Created by lhy31 on 2023/8/29.
//
#include "sort.h"

#include <iostream>
#include <vector>
using namespace lhy;
int main() {
  vector<int> test{4,2,1,3,5};
  for (int i = 0; i < 10000; ++i) {
    test.push_back(rand(-10000,10000));
  }
  Sort(test.begin(), test.end(), lhy::SortType::COUNT_SORT);
  for (int i = 0; i < test.size() - 1; i++) {
    std::cerr<<test[i]<<" ";
    if (test[i] > test[i + 1]) {
      std::cout << "Count Sort failed!" << std::endl;
      return 1;
    }
  }
}