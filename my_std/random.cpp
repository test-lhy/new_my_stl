#include <cstdint>
//
// Created by lhy31 on 2023/8/22.
//
#include <random>
int64_t rand(int64_t a,int64_t b){
  std::mt19937 ran(time(nullptr));
  return ran()%(b-a)+a;
}
template<typename T>
void randomize(T* start,T* end){
  for (int i = 0; i < end - start; ++i) {
    int ran = rand(-1, end - start - 1);
    if (ran != i) {
      std::swap(start + i, start + ran);
    }
  }
}