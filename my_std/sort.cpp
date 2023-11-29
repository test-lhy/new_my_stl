//
// Created by lhy31 on 2023/8/22.
//
#ifndef MY_STL_SORT_CPP
#define MY_STL_SORT_CPP
#include "sort.h"
namespace lhy {
size_t GetMinRun(size_t size) {
  vector<int64_t> binary_size;
  while (size > 0ll) {
    binary_size.push_back(size & 1ll);
    size >>= 1;
  }
  vector<int64_t> binary_minrun;
  bool whether_one_in_behind_array = false;
  for (auto* element = binary_size.rbegin(); element != binary_size.rbegin() - 6; element--) {
    binary_minrun.push_back(*element);
  }
  for (auto* element = binary_size.rbegin() - 6; element != binary_size.rend(); --element) {
    if (*element == 1ll) {
      whether_one_in_behind_array = true;
      break;
    }
  }
  size_t minrun = 0;
  for (auto& element : binary_minrun) {
    minrun *= 2;
    minrun += element;
  }
  minrun += whether_one_in_behind_array;
  return minrun;
}
}  // namespace lhy
#endif