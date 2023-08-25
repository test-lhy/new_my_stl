//
// Created by lhy31 on 2023/8/22.
//
#ifndef MY_STL_SORT_CPP
#define MY_STL_SORT_CPP
#include "sort.h"
namespace lhy{
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

template <typename T, typename CmpType, typename GapGeneration = ShellNormalGapGeneration>
void Sort(const T* start, const T* end, SortType sort_type, CmpType compare_function) {
  if (sort_type == SortType::QUICK_SORT) {
    QuickSort(start, end, compare_function, 0, -1);
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
    QuickSort(start, end, compare_function, 0, std::log2(end - start));
  } else if (sort_type == SortType::BUCKET_SORT) {
    if (typeid(T) == typeid(int) || typeid(T) == typeid(char)) {
      BucketSort(start, end);
    } else {
      throw std::logic_error("this type can't use Count_Sort");
    }
  } else if (sort_type == SortType::SHELL_SORT) {
    GapGeneration gap_generation;
    ShellSort(start, end, compare_function, &gap_generation);
  } else if (sort_type == SortType::TOURNAMENT_SORT) {
    try {
      T::INF;
    } catch (...) {
      throw std::logic_error("no INF for tournament_sort");
    }
    TournamentSort(start, end, compare_function);
  }
}
template <typename T, typename CmpType>
void GetWinner(T* temp_array, Index index, size_t size, CmpType compare_function) {
  *(temp_array + index).first = T::INF;
  if (temp_array + index * 2 + 1 < size) {
    if (*(temp_array + index).first == T::INF ||
        compare_function(*(temp_array + index * 2 + 1).first, *(temp_array + index).first)) {
      *(temp_array + index) = *(temp_array + index * 2 + 1);
    }
  }
  if (temp_array + index * 2 + 2 < size) {
    if (*(temp_array + index).first == T::INF ||
        compare_function(*(temp_array + index * 2 + 2).first, *(temp_array + index).first)) {
      *(temp_array + index) = *(temp_array + index * 2 + 2);
    }
  }
}
template <typename T, typename CmpType>
void TournamentSort(T* start, T* end, CmpType compare_function, const T INF) {
  size_t front_size = pow(2, ceil(log2(end - start))) - 1;
  auto* temp_array = new std::pair<T, int>[front_size + end - start];
  for (Index i = 0; i < end - start; ++i) {
    *(temp_array + front_size + i) = {*(start + i), i};
  }
  for (Index i = front_size - 1; i >= 0; --i) {
    GetWinner(temp_array, i, front_size + end - start, compare_function);
  }
  *start = (*temp_array).first;
  Index refresh_index = (*temp_array).second;
  for (int i = 1; i < end - start; ++i) {
    refresh_index = (refresh_index - 1) / 2;
    while (refresh_index != 0) {
      GetWinner(temp_array, refresh_index, front_size + end - start, compare_function);
      refresh_index = (refresh_index - 1) / 2;
    }
    GetWinner(temp_array, refresh_index, front_size + end - start, compare_function);
    *(start + i) = (*temp_array).first;
    refresh_index = (*temp_array).second;
    (*temp_array + refresh_index).first = T::INF;
  }
}
template <typename T, typename CmpType>
void ShellSort(T* start, T* end, CmpType compare_function, ShellGapGenerationBase* gap_generation) {
  while (gap_generation->Get() < end - start) {
    gap_generation->Up();
  }
  gap_generation->Down();
  while (gap_generation->DownIf()) {
    const auto gap_now = gap_generation->Get();
    for (Index i = gap_now; i < end - start; ++i) {
      for (Index j = i; j >= gap_now && compare_function(*(start + j), *(start + j - gap_now)); j -= gap_now) {
        std::swap(*(start + j), *(start + j - gap_now));
      }
    }
    gap_generation->Down();
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
void QuickSort(T* start, T* end, CmpType compare_function, int64_t depth, int64_t limit_depth) {
  if (limit_depth != -1 && depth >= limit_depth) {
    HeapSort(start, end, compare_function);
    return;
  }
  if (end - start == 1) {
    return;
  }
  if (depth == 0) {
    randomize(start, end);
  }
  T pivot = GetKthElement(start, end, (end - start) / 2, compare_function);
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
  QuickSort(start, smaller, compare_function, depth + 1, limit_depth);
  QuickSort(bigger + 1, end, compare_function, depth + 1, limit_depth);
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
template <typename T, typename CmpType>
T GetApproximateMedian(
    T* start, T* end, CmpType compare_function = [](const T& a, const T& b) -> bool { return a < b; },
    size_t median_split_length = 5) {
  for (Index i = 0; i < end - start; i += median_split_length) {
    Sort(start + i, start + min(i + median_split_length, end - start), SortType::INSERT_SORT, compare_function);
    std::swap(*(start + i / median_split_length), *(start + min(i + median_split_length / 2, end - start)));
  }
  if ((end - start) <= median_split_length) {
    return *start;
  } else {
    return GetApproximateMedian(start, start + (end - start) / median_split_length + 1, compare_function,
                                median_split_length);
  }
}
template <typename T, typename CmpType>
T GetKthElement(
    T* start, T* end, Index k, CmpType compare_function = [](const T& a, const T& b) -> bool { return a < b; }) {
  T pivot = GetApproximateMedian(start, end, compare_function);
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
  if (k <= (smaller - start)) {
    return GetKthElement(start, smaller, k, compare_function);
  } else if (k > (bigger - start + 1)) {
    return GetKthElement(bigger + 1, end, k - (bigger - start + 1), compare_function);
  } else {
    return pivot;
  }
}
}  // namespace lhy
#endif