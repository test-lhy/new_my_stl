//
// Created by lhy31 on 2023/8/24.
//

#ifndef MY_STL_ALGORITHM_H
#define MY_STL_ALGORITHM_H
#include "sort.cpp"
#include"../basic.h"
namespace lhy{
template<typename T,typename CmpType>
T GetApproximateMedian(T* start,T* end,CmpType compare_function=[](const T& a,const T& b)->bool{return a<b;},size_t median_split_length=5){
  for (Index i = 0; i <end-start; i+=median_split_length) {
    Sort(start+i,start+ min(i+median_split_length,end-start),SortType::INSERT_SORT,compare_function);
    std::swap(*(start+i/median_split_length),*(start+min(i+median_split_length/2,end-start)));
  }
  if ((end-start)<=median_split_length){
    return *start;
  }else{
    return GetApproximateMedian(start,start+(end-start)/median_split_length+1,compare_function,median_split_length);
  }
}
template <typename T, typename CmpType>
T GetKthElement(T* start, T* end, Index k,CmpType compare_function=[](const T& a,const T& b)->bool{return a<b;}) {
  T pivot = GetApproximateMedian(start,end,compare_function);
  T *smaller = start, *now = start, *bigger = end - 1;
  while (now < bigger) {
    if (compare_function(*now, pivot)) {
      std::swap(*now, *smaller);
      smaller++;
      now++;
    } else if (compare_function(pivot, *now)) {
      std::swap(*now, *bigger);
      bigger--;
    } else {
      now++;
    }
  }
  if (k<=(smaller-start)){
    return GetKthElement(start,smaller,k,compare_function);
  }else if(k>(bigger-start+1)){
    return GetKthElement(bigger+1,end,k-(bigger-start+1),compare_function);
  }else{
    return pivot;
  }
}
}
#endif  // MY_STL_ALGORITHM_H