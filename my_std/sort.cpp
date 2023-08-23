//
// Created by lhy31 on 2023/8/22.
//
#include <functional>
#include <list>
#include <stdexcept>

#include "random.cpp"
#include "vector.h"
namespace lhy {
enum SortType { QUICK_SORT, INSERT_SORT, MERGE_SORT, COUNT_SORT, BUCKET_SORT,RADIX_SORT };
template <typename T>
void Sort(T* start, T* end) {
  Sort(start, end, SortType::QUICK_SORT);
}
template <typename T,typename CMPTYPE>
void Sort(T* start, T* end, CMPTYPE compare_function) {
  Sort(start, end, SortType::QUICK_SORT, compare_function);
}
template <typename T>
void Sort(T* start, T* end, SortType sort_type) {
  Sort(start, end, SortType::QUICK_SORT, std::less<T>());
}
template <typename T,typename CMPTYPE>
void Sort(const T* start, const T* end, SortType sort_type, CMPTYPE compare_function) {
  if (sort_type == SortType::QUICK_SORT) {
    QuickSort(start, end, compare_function, false);
  } else if (sort_type == SortType::INSERT_SORT) {
    InsertSort(start, end, compare_function);
  } else if (sort_type == SortType::MERGE_SORT) {
    Merge_Sort(start, end, compare_function);
  } else if (sort_type == SortType::COUNT_SORT) {
    if (typeid(T) == typeid(int)|| typeid(T)== typeid(char)) {
      Count_Sort(start, end);
    } else {
      throw std::logic_error("this type can't use Count_Sort");
    }
  }else if (sort_type == SortType::BUCKET_SORT) {
    if (typeid(T) == typeid(int)|| typeid(T)== typeid(char)) {
      Bucket_Sort(start, end);
    } else {
      throw std::logic_error("this type can't use Count_Sort");
    }
  }
}
template<typename ...T>
void Sort(std::tuple<T...>* start,std::tuple<T...>* end,SortType sort_type=SortType::QUICK_SORT){
  RadixSort(start,end,sort_type);
}
template <typename T,typename CMPTYPE>
void InsertSort(T* start, T* end, CMPTYPE compare_function) {
  std::list<T> temp_list;
  for (auto element = start; element != end; ++element) {
    auto temp_list_iterator = temp_list.begin();
    while (temp_list_iterator != temp_list.end() && compare_function(*temp_list_iterator, *element)) {
      temp_list_iterator++;
    }
    temp_list.insert(temp_list_iterator, element);
  }
  auto temp_list_iterator = temp_list.begin();
  for (int i = 0; i < end - start; i++) {
    *(start + i) = *temp_list_iterator;
    temp_list_iterator++;
  }
}
template <typename T,typename CMPTYPE>
void QuickSort(T* start, T* end, CMPTYPE compare_function, bool randomized) {
  if (end - start == 1) {
    return;
  }
  if (!randomized) {
    randomized = true;
    randomize(start, end);
  }
  auto* temp_array = new std::pair<T, int>[end - start];
  auto* temp_array_iterator = temp_array;
  for (auto element = start; element != end; element++) {
    *temp_array_iterator = {*element, static_cast<int>(element - start)};
  }
  std::nth_element(temp_array, temp_array + (end - start) / 2, temp_array + (end - start));
  T pivot = (temp_array + (end - start) / 2)->first;
  T pivot_pos = (temp_array + (end - start) / 2)->second;
  delete[] temp_array;
  T *left = start, right = end - 2;
  std::swap(start + pivot_pos, end - 1);
  while (left < right) {
    while (left < right && (!compare_function(pivot, *left))) {
      left++;
    }
    while (left < right && compare_function(pivot, *right)) {
      right--;
    }
    std::swap(*left, *right);
  }
  std::swap(left, end - 1);
  QuickSort(start, start + left, compare_function, randomized);
  QuickSort(start + left + 1, end, compare_function, randomized);
}
template <typename T,typename CMPTYPE>
void MergeSort(T* start, T* end, CMPTYPE compare_function) {
  T* temp_array = new T[end - start];
  for (int i = 1; i < end - start; i <<= 1) {
    for (int j = 0; j < end - start; ++j) {
      *(temp_array + j) = *(start + j);
    }
    for (int j = 0; j < end - start; j += i * 2) {
      int i1 = j, i2 = j + i;
      int i3 = j;
      while (i1 < j + i && i2 < std::min(j + 2 * i, end - start)) {
        if (compare_function(*(temp_array + i1), *(temp_array + i2))) {
          *(start + i3) = *(temp_array + i1);
          i3++;
          i1++;
        } else {
          *(start + i3) = *(temp_array + i2);
          i3++;
          i2++;
        }
      }
      while (i1 < j + i) {
        *(start + i3) = *(temp_array + i1);
        i3++;
        i1++;
      }
      while (i2 < std::min(j + 2 * i, end - start)) {
        *(start + i3) = *(temp_array + i2);
        i3++;
        i2++;
      }
    }
  }
  delete[] temp_array;
}
template <typename T,typename CMPTYPE>
void CountSort(T* start, T* end,CMPTYPE compare_function) {
  T maxT = *start, minT = *start;
  T* temp_array = new T[end - start];
  for (auto element = start; element != end; ++element) {
    maxT = std::max(*element, maxT);
    minT = std::min(*element, minT);
  }
  int* cnt = new int[maxT - minT + 1];
  for (auto element = start; element != end; ++element) {
    cnt[*element-minT]++;
  }
  for (int i = 1; i < maxT - minT + 1; ++i) {
    cnt[i] += cnt[i - 1];
  }
  for (int i = end - start - 1; i >= 0; --i) {
    *(temp_array + i) = cnt[*(start + i)-minT];
    cnt[*(start + i)-minT]--;
  }
  for (int i = 0; i < start - end; ++i) {
    *(start + i) = *(temp_array + i);
  }
  delete[] temp_array;
}
template <typename T>
void BucketSort(T* start, T* end) {
  T maxT = *start, minT = *start;
  for (auto element = start; element != end; ++element) {
    maxT = std::max(*element, maxT);
    minT = std::min(*element, minT);
  }
  int* cnt = new int[maxT - minT + 1];
  for (auto element = start; element != end; ++element) {
    cnt[*element-minT]++;
  }
  int count=0;
  for (int i = 0; i < maxT-minT+1; ++i) {
    for (int j = 0; j < cnt[i]; ++j) {
      *(start+count)=i;
      count++;
    }
  }
}
template<typename ...T>
void RadixSort(std::tuple<T...>* start,std::tuple<T...>* end,SortType sort_type){
  using TupleWithIndex=std::pair<std::tuple<T...>,int>;
  auto *temp_array=new TupleWithIndex [end-start];
  for (int i = 0; i < end-start; ++i) {
    *(temp_array+i)={*(start+i),i};
  }
  for (int i = 0; i < (*start).size(); ++i) {
    for (int j = 0; j < end-start; ++j) {
      *(temp_array+j).second=j;
    }
    Sort(start,end,sort_type,[i](const TupleWithIndex & a,const TupleWithIndex &b)->bool{
      if (std::get<i>(a.first)==std::get<i>(b.first)){
        return a.second<b.second;
      }else{
        return std::get<i>(a.first)<std::get<i>(b.first);
      }
    });
  }
  for (int i = 0; i < end-start; ++i) {
    *(start+i)=*(temp_array+i).first;
  }
}
}  // namespace lhy