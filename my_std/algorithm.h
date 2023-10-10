//
// Created by lhy31 on 2023/8/24.
//

#ifndef MY_STL_ALGORITHM_H
#define MY_STL_ALGORITHM_H
#include <cstdint>
#include <sstream>
#include <string>
#include <utility>

#include "vector.h"
namespace lhy {
std::string ToString(int64_t);
//todo:总感觉将T*改为T好怪
template <typename T>
void reverse(T start, T end) {
  end--;
  while (start < end) {
    std::swap(*start, *end);
    start++;
    end--;
  }
}
template <typename T>
std::string show(T* start,T* end,int count_limit=20) {
  std::stringstream string_stream;
  for (auto* element = start; element != end; element++) {
    string_stream << *element << " ";
    count_limit--;
    if (count_limit <= 0) {
      break;
    }
  }
  return string_stream.str();
}
template<typename T>
T max(const T& a,const T& b){
  return a>b?a:b;
}
template<typename T>
T max(const vector<T>& vec){
  T max_temp=vec.front();
  for (auto &each :vec) {
    max_temp= max(max_temp,each);
  }
  return max_temp;
}
template<typename T>
T min(const T& a,const T& b){
  return a<b?a:b;
}
template<typename T>
T min(const vector<T>& vec){
  T min_temp=vec.front();
  for (auto &each :vec) {
    min_temp= min(min_temp,each);
  }
  return min_temp;
}
}  // namespace lhy
#endif  // MY_STL_ALGORITHM_H