//
// Created by lhy31 on 2023/8/22.
//
#include <functional>
#include <list>
#include <stdexcept>
#include <cmath>

#include "../basic.h"
#include "priority_queue.h"
#include "random.cpp"
#include "vector.h"
namespace lhy {
enum SortType { QUICK_SORT, INSERT_SORT, MERGE_SORT, COUNT_SORT, BUCKET_SORT, HEAP_SORT, INTRO_SORT, RADIX_SORT };
template <typename T>
void Sort(T* start, T* end) {
  Sort(start, end, SortType::QUICK_SORT);
}
template <typename T, typename CmpType>
void Sort(T* start, T* end, CmpType compare_function) {
  Sort(start, end, SortType::QUICK_SORT, compare_function);
}
template <typename T>
void Sort(T* start, T* end, SortType sort_type) {
  Sort(start, end, SortType::QUICK_SORT, std::less<T>());
}
template <typename T, typename CmpType>
void Sort(const T* start, const T* end, SortType sort_type, CmpType compare_function) {
  if (sort_type == SortType::QUICK_SORT) {
    QuickSort(start, end, compare_function, 0,-1);
  } else if (sort_type == SortType::INSERT_SORT) {
    InsertSort(start, end, compare_function);
  } else if (sort_type == SortType::MERGE_SORT) {
    MergeSort(start, end, compare_function);
  } else if (sort_type == SortType::COUNT_SORT) {
    if (typeid(T) == typeid(int) || typeid(T) == typeid(char)) {
      CountSort(start, end);
    } else {
      throw std::logic_error("this type can't use Count_Sort");
    }
  } else if (sort_type == SortType::HEAP_SORT) {
    HeapSort(start, end, compare_function);
  } else if (sort_type == SortType::INTRO_SORT) {
    QuickSort(start,end,compare_function,0,std::log2(end-start));
  } else if (sort_type == SortType::BUCKET_SORT) {
    if (typeid(T) == typeid(int) || typeid(T) == typeid(char)) {
      BucketSort(start, end);
    } else {
      throw std::logic_error("this type can't use Count_Sort");
    }
  }
}

template <typename T, typename CmpType>
void HeapSort(T* start, T* end, CmpType compare_function) {
  struct compare_function_type {
    bool operator()(const T& a, const T& b) { return compare_function(a, b); }
  };
  priority_queue<T, compare_function_type> priority_queue_temp(start, end);
  for (int i = 0; i < end - start; ++i) {
    *(start + i) = priority_queue_temp.front();
    priority_queue_temp.pop();
  }
}
template <typename... T>
void Sort(std::tuple<T...>* start, std::tuple<T...>* end, SortType sort_type = SortType::QUICK_SORT) {
  RadixSort(start, end, sort_type);
}
template <typename T, typename CmpType>
void InsertSort(T* start, T* end, CmpType compare_function) {
  std::list<T> temp_list;
  for (auto element = start; element != end; ++element) {
    auto temp_list_iterator = temp_list.begin();
    while (temp_list_iterator != temp_list.end() && compare_function(*temp_list_iterator, *element)) {
      temp_list_iterator++;
    }
    temp_list.insert(temp_list_iterator, element);
  }
  auto temp_list_iterator = temp_list.begin();
  for (Index i = 0; i < end - start; i++) {
    *(start + i) = *temp_list_iterator;
    temp_list_iterator++;
  }
}
template <typename T, typename CmpType>
void QuickSort(T* start, T* end, CmpType compare_function, int64_t depth,int64_t limit_depth) {
  if (limit_depth!=-1&&depth>=limit_depth){
    HeapSort(start,end,compare_function);
    return ;
  }
  if (end - start == 1) {
    return;
  }
  if (depth==0) {
    randomize(start, end);
  }
  auto* temp_array = new std::pair<T, Index>[end - start];
  auto* temp_array_iterator = temp_array;
  for (auto element = start; element != end; element++) {
    *temp_array_iterator = {*element, static_cast<Index>(element - start)};
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
  QuickSort(start, start + left, compare_function, depth+1,limit_depth);
  QuickSort(start + left + 1, end, compare_function, depth+1,limit_depth);
}
template <typename T, typename CmpType>
void MergeSort(T* start, T* end, CmpType compare_function) {
  T* temp_array = new T[end - start];
  for (Index i = 1; i < end - start; i <<= 1) {
    for (Index j = 0; j < end - start; ++j) {
      *(temp_array + j) = *(start + j);
    }
    for (Index j = 0; j < end - start; j += i * 2) {
      Index i1 = j, i2 = j + i;
      Index i3 = j;
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
template <typename T, typename CmpType>
void CountSort(T* start, T* end, CmpType compare_function) {
  T maxT = *start, minT = *start;
  T* temp_array = new T[end - start];
  for (auto element = start; element != end; ++element) {
    maxT = std::max(*element, maxT);
    minT = std::min(*element, minT);
  }
  int* cnt = new int[maxT - minT + 1];
  for (auto element = start; element != end; ++element) {
    cnt[*element - minT]++;
  }
  for (Index i = 1; i < maxT - minT + 1; ++i) {
    cnt[i] += cnt[i - 1];
  }
  for (Index i = end - start - 1; i >= 0; --i) {
    *(temp_array + i) = cnt[*(start + i) - minT];
    cnt[*(start + i) - minT]--;
  }
  for (Index i = 0; i < start - end; ++i) {
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
    cnt[*element - minT]++;
  }
  int count = 0;
  for (Index i = 0; i < maxT - minT + 1; ++i) {
    for (Index j = 0; j < cnt[i]; ++j) {
      *(start + count) = i;
      count++;
    }
  }
}
template <typename... T>
void RadixSort(std::tuple<T...>* start, std::tuple<T...>* end, SortType sort_type) {
  using TupleWithIndex = std::pair<std::tuple<T...>, Index>;
  auto* temp_array = new TupleWithIndex[end - start];
  for (Index i = 0; i < end - start; ++i) {
    *(temp_array + i) = {*(start + i), i};
  }
  for (Index i = 0; i < (*start).size(); ++i) {
    for (Index j = 0; j < end - start; ++j) {
      *(temp_array + j).second = j;
    }
    Sort(start, end, sort_type, [i](const TupleWithIndex& a, const TupleWithIndex& b) -> bool {
      if (std::get<i>(a.first) == std::get<i>(b.first)) {
        return a.second < b.second;
      } else {
        return std::get<i>(a.first) < std::get<i>(b.first);
      }
    });
  }
  for (Index i = 0; i < end - start; ++i) {
    *(start + i) = *(temp_array + i).first;
  }
}
}  // namespace lhy