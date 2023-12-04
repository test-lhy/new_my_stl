//
// Created by lhy31 on 2023/8/29.
//
#include "sort.h"

#include <iostream>
#include <vector>
using namespace lhy;
int main() {
  vector<int> test{};
  for (int i = 0; i < 10000; ++i) {
    test.push_back(rand(-10000, 10000)*1.0/100);
  }
  Sort(test.begin(), test.end(), lhy::SortType::MERGE_SORT);
  for (int i = 0; i < test.size() - 1; i++) {
    std::cerr << test[i] <<"|"<<test[i+1]<< " ";
    if (test[i] > test[i + 1]) {
      std::cerr << "Count Sort failed!" << std::endl;
    }
  }
}