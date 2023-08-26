//
// Created by lhy31 on 2023/8/24.
//

#ifndef MY_STL_ALGORITHM_H
#define MY_STL_ALGORITHM_H
#include <cstdint>
#include <utility>
namespace lhy {
template<typename T>
void reverse(T* start,T* end){
  end--;
  while(start<end){
    std::swap(*start,*end);
    start++;
    end--;
  }
}
}
#endif  // MY_STL_ALGORITHM_H