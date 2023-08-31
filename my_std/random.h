//
// Created by lhy31 on 2023/8/25.
//

#ifndef MY_STL_RANDOM_H
#define MY_STL_RANDOM_H
#include <cstdint>
#include <random>
namespace lhy {
template <typename T>
void randomize(T *start, T *end) {
  for (int i = 0; i < end - start; ++i) {
    int ran = rand(0, end - start - 1);
    if (ran != i) {
      std::swap(start[i], start[ran]);
    }
  }
}
int64_t rand(int64_t, int64_t);
}  // namespace lhy
#endif  // MY_STL_RANDOM_H