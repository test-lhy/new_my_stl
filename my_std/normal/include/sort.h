//
// Created by lhy31 on 2023/8/25.
//

#ifndef MY_STL_SORT_H
#define MY_STL_SORT_H
#include <algorithm>
#include <cmath>
#include <functional>
#include <list>
#include <stdexcept>

#include "algorithm.h"
#include "basic.h"
#include "data_structure.h"
#include "list.h"
#include "mmath.h"
#include "priority_queue.h"
#include "random.h"
#include "type_traits.h"
#include "vector.h"

namespace lhy {
enum SortType {
  QUICK_SORT,
  INSERT_SORT,
  MERGE_SORT,
  COUNT_SORT,
  BUCKET_SORT,
  HEAP_SORT,
  INTRO_SORT,
  SHELL_SORT,
  TOURNAMENT_SORT,
  RADIX_SORT,
  TIM_SORT
};
size_t GetMinRun(size_t size);
class ShellGapGenerationBase {
 public:
  ShellGapGenerationBase() = default;
  virtual ~ShellGapGenerationBase() = default;
  virtual Index Get() = 0;
  virtual void Up() = 0;
  virtual void Down() = 0;
  virtual bool DownIf() = 0;
};
class ShellNormalGapGeneration : public ShellGapGenerationBase {
 public:
  Index number;
  ShellNormalGapGeneration() { number = 1; }
  Index Get() override { return number; }
  void Up() override { number = number * 3 + 1; }
  void Down() override { number = (number - 1) / 3; }
  bool DownIf() override {
    if (number >= 1) {
      return true;
    } else {
      return false;
    }
  }
};
template <typename T>
T GetKthElement(NormIterator<T> start, NormIterator<T> end, Index k, const CmpType<T>& compare_function);
template <typename T>
void Merge(T start, T other_start, T end, const CmpType<T>& compare_function);
template <typename T>
void TimSortUpdate(NormIterator<T> start, list<std::pair<Index, Index>>& runs, const CmpType<T>& compare_function,
                   bool forced) {
  auto last_iterator = runs.rbegin();
  size_t size = runs.size();
  for (int i = 0; i < size - 1; ++i) {
    auto last_second_iterator = last_iterator.Next();
    size_t last_second_size = last_second_iterator->second - last_second_iterator->first;
    size_t last_size = last_iterator->second - last_iterator->first;
    if (last_second_size < last_size * 2 || forced) {
      Merge(start + last_second_iterator->first, start + last_second_iterator->second, start + last_iterator->second,
            compare_function);
      std::pair<Index, Index> new_pair = std::make_pair(last_second_iterator->first, last_iterator->second);
      runs.erase(last_second_iterator);
      auto temp_iterator = runs.erase(last_iterator);
      runs.insert(temp_iterator, new_pair);
    } else {
      break;
    }
    last_iterator = runs.rbegin();
  }
}
template <typename T>
void TimSort(NormIterator<T> start, NormIterator<T> end, const CmpType<T>& compare_function) {
  if (end - start <= 64) {
    Sort(start, end, SortType::INSERT_SORT, compare_function);
    return;
  }
  size_t minrun = GetMinRun(end - start);
  Index now_index = 0;
  Index start_index = 0;
  bool up = true;
  list<std::pair<Index, Index>> runs;
  while (now_index <= end - start) {
    if (now_index - start_index == 1) {
      if (compare_function(start[now_index], start[start_index])) {
        up = false;
      }
    }
    if (now_index == end - start ||
        ((now_index > start_index + 1) && (((up && compare_function(start[now_index], start[now_index])) ||
                                            (!up && !compare_function(start[now_index], start[now_index])))))) {
      while (now_index < end - start && (now_index - start_index) <= minrun) {
        now_index++;
      }
      if (up) {
        Sort(start + start_index, start + now_index, SortType::INSERT_SORT, compare_function);
      } else {
        Sort(start + start_index, start + now_index, SortType::INSERT_SORT,
             [compare_function](const T& a, const T& b) -> bool { return !compare_function(a, b); });
        reverse(start + start_index, start + now_index);
      }
      runs.push_back({start_index, now_index});
      if (runs.size() >= 2) {
        TimSortUpdate(start, runs, compare_function, false);
      }
      start_index = now_index;
    }
    now_index++;
  }
  if (runs.size() >= 2) {
    TimSortUpdate(start, runs, compare_function, true);
  }
}
template <typename T>
void GetWinner(std::pair<T, int>* temp_array, Index index, size_t size, const CmpType<T>& compare_function,
               const T& INF) {
  temp_array[index].first = INF;
  temp_array[index].second = -1;
  if (index * 2 + 1 < size && temp_array[index * 2 + 1].second != -1) {
    if (temp_array[index].first == INF || compare_function(temp_array[index * 2 + 1].first, temp_array[index].first)) {
      temp_array[index] = temp_array[index * 2 + 1];
    }
  }
  if (index * 2 + 2 < size && temp_array[index * 2 + 2].second != -1) {
    if (temp_array[index].first == INF || compare_function(temp_array[index * 2 + 2].first, temp_array[index].first)) {
      temp_array[index] = temp_array[index * 2 + 2];
    }
  }
}
template <typename T>
void TournamentSort(NormIterator<T> start, NormIterator<T> end, const CmpType<T>& compare_function, const T& INF) {
  size_t front_size = pow(2, ceil(log2(end - start))) - 1;
  auto* temp_array = new std::pair<T, int>[front_size + end - start];
  for (Index i = 0; i < end - start; ++i) {
    temp_array[front_size + i] = {start[i], i};
  }
  for (Index i = front_size - 1; i >= 0; --i) {
    GetWinner(temp_array, i, front_size + end - start, compare_function, INF);
  }
  start[0] = temp_array[0].first;
  Index refresh_index = temp_array[0].second + front_size;
  temp_array[refresh_index].second = -1;
  for (int i = 1; i < end - start; ++i) {
    refresh_index = (refresh_index - 1) / 2;
    while (refresh_index != 0) {
      GetWinner(temp_array, refresh_index, front_size + end - start, compare_function, INF);
      refresh_index = (refresh_index - 1) / 2;
    }
    GetWinner(temp_array, refresh_index, front_size + end - start, compare_function, INF);
    start[i] = temp_array[0].first;
    refresh_index = temp_array[0].second + front_size;
    temp_array[refresh_index].first = INF;
  }
}
template <typename T>
void ShellSort(NormIterator<T> start, NormIterator<T> end, const CmpType<T>& compare_function,
               ShellGapGenerationBase* gap_generation) {
  while (gap_generation->Get() < end - start) {
    gap_generation->Up();
  }
  gap_generation->Down();
  while (gap_generation->DownIf()) {
    const auto gap_now = gap_generation->Get();
    for (Index i = gap_now; i < end - start; ++i) {
      for (Index j = i; j >= gap_now && compare_function(start[j], start[j - gap_now]); j -= gap_now) {
        std::swap(start[j], start[j - gap_now]);
      }
    }
    gap_generation->Down();
  }
}
template <typename T>
void HeapSort(NormIterator<T> start, NormIterator<T> end, const CmpType<T>& compare_function) {
  priority_queue<T> priority_queue_temp(start, end, compare_function);
  for (int i = 0; i < end - start; ++i) {
    start[i] = priority_queue_temp.top();
    priority_queue_temp.pop();
  }
}
template <typename... T>
void Sort(std::tuple<T...>* start, std::tuple<T...>* end, SortType sort_type = SortType::QUICK_SORT) {
  RadixSort(start, end, sort_type);
}
template <typename T>
void InsertSort(NormIterator<T> start, NormIterator<T> end, const CmpType<T>& compare_function) {
  list<T> temp_list;
  for (auto element = start; element != end; ++element) {
    auto temp_list_iterator = temp_list.begin();
    while (temp_list_iterator != temp_list.end() && compare_function(*temp_list_iterator, *element)) {
      temp_list_iterator++;
    }
    temp_list.insert(temp_list_iterator, *element);
  }
  auto temp_list_iterator = temp_list.begin();
  for (Index i = 0; i < end - start; i++) {
    start[i] = *temp_list_iterator;
    temp_list_iterator++;
  }
}
template <typename T>
void QuickSort(NormIterator<T> start, NormIterator<T> end, const CmpType<T>& compare_function, int64_t depth,
               int64_t limit_depth) {
  if (end - start == 0) {
    return;
  }
  if ((limit_depth != -1 && depth >= limit_depth) || end - start < 64) {
    HeapSort(start, end, compare_function);
    return;
  }
  if (depth == 0) {
    randomize(start, end);
  }
  T pivot = GetKthElement(start, end, (end - start + 1) / 2, compare_function);
  auto smaller = start, now = start, bigger = end;
  while (now < bigger) {
    if (compare_function(*now, pivot)) {
      std::swap(*now, *smaller);
      smaller++;
      now++;
    } else if (compare_function(pivot, *now)) {
      bigger--;
      std::swap(*now, *bigger);
    } else {
      now++;
    }
  }
  QuickSort(start, smaller, compare_function, depth + 1, limit_depth);
  QuickSort(bigger, end, compare_function, depth + 1, limit_depth);
}
template <typename T>
void Merge(NormIterator<T> start, NormIterator<T> other_start, NormIterator<T> end,
           const CmpType<T>& compare_function) {
  auto* temp_array = new T[end - start];
  for (Index i = 0; i < end - start; ++i) {
    temp_array[i] = start[i];
  }
  Index i1 = 0, i2 = other_start - start;
  Index i3 = 0, i4 = i2;
  while (i1 < i4 && i2 < end - start) {
    if (compare_function(temp_array[i1], temp_array[i2])) {
      start[i3] = temp_array[i1];
      i3++;
      i1++;
    } else {
      start[i3] = temp_array[i2];
      i3++;
      i2++;
    }
  }
  while (i1 < i4) {
    start[i3] = temp_array[i1];
    i3++;
    i1++;
  }
  while (i2 < end - start) {
    start[i3] = temp_array[i2];
    i3++;
    i2++;
  }
  delete[] temp_array;
}
template <typename T>
void MergeSort(NormIterator<T> start, NormIterator<T> end, const CmpType<T>& compare_function) {
  NormIterator<T> temp_array = new T[end - start];
  for (Index i = 1; i < end - start; i <<= 1) {
    for (Index j = 0; j < end - start; ++j) {
      temp_array[j] = start[j];
    }
    for (Index j = 0; j < end - start; j += i * 2) {
      Index i1 = j, i2 = j + i;
      Index i3 = j;
      while (i1 < j + i && i2 < std::min(j + 2 * i, end - start)) {
        if (compare_function(temp_array[i1], temp_array[i2])) {
          start[i3] = temp_array[i1];
          i3++;
          i1++;
        } else {
          start[i3] = temp_array[i2];
          i3++;
          i2++;
        }
      }
      while (i1 < j + i) {
        start[i3] = temp_array[i1];
        i3++;
        i1++;
      }
      while (i2 < std::min(j + 2 * i, end - start)) {
        start[i3] = temp_array[i2];
        i3++;
        i2++;
      }
    }
  }
  delete[] temp_array;
}
template <typename T>
void CountSort(NormIterator<T> start, NormIterator<T> end) {
  T maxT = *start, minT = *start;
  NormIterator<T> temp_array = new T[end - start];
  for (auto element = start; element != end; ++element) {
    maxT = std::max(*element, maxT);
    minT = std::min(*element, minT);
  }
  int* cnt = new int[maxT - minT + 1];
  for (Index i = minT; i <= maxT; ++i) {
    cnt[i - minT] = 0;
  }
  for (auto element = start; element != end; ++element) {
    cnt[*element - minT]++;
  }
  for (Index i = 1; i < maxT - minT + 1; ++i) {
    cnt[i] += cnt[i - 1];
  }
  for (Index i = end - start - 1; i >= 0; --i) {
    temp_array[cnt[start[i] - minT] - 1] = start[i];
    cnt[start[i] - minT]--;
  }
  for (Index i = 0; i < end - start; ++i) {
    start[i] = temp_array[i];
  }
  delete[] temp_array;
}
template <typename T>
void BucketSort(NormIterator<T> start, NormIterator<T> end) {
  T maxT = *start, minT = *start;
  for (auto element = start; element != end; ++element) {
    maxT = std::max(*element, maxT);
    minT = std::min(*element, minT);
  }
  int* cnt = new int[maxT - minT + 1];
  for (Index i = minT; i <= maxT; ++i) {
    cnt[i - minT] = 0;
  }
  for (auto element = start; element != end; ++element) {
    cnt[*element - minT]++;
  }
  int count = 0;
  for (Index i = 0; i < maxT - minT + 1; ++i) {
    for (Index j = 0; j < cnt[i]; ++j) {
      start[count] = i + minT;
      count++;
    }
  }
}
// template <typename... T>
// void RadixSort(std::tuple<T...>* start, std::tuple<T...>* end, SortType sort_type) {
//   using TupleWithIndex = std::pair<std::tuple<T...>, Index>;
//   auto* temp_array = new TupleWithIndex[end - start];
//   for (Index i = 0; i < end - start; ++i) {
//     temp_array[i] = {start[i], i};
//   }
//   for (Index i = 0; i < start->size(); ++i) {
//     for (Index j = 0; j < end - start; ++j) {
//       temp_array[j].second = j;
//     }
//     Sort(start, end, sort_type, [i](const TupleWithIndex& a, const TupleWithIndex& b) -> bool {
//       if (std::get<i>(a.first) == std::get<i>(b.first)) {
//         return a.second < b.second;
//       } else {
//         return std::get<i>(a.first) < std::get<i>(b.first);
//       }
//     });
//   }
//   for (Index i = 0; i < end - start; ++i) {
//     start[i] = temp_array[i].first;
//   }
// }
template <typename T>
T GetApproximateMedian(
    NormIterator<T> start, NormIterator<T> end,
    const CmpType<T>& compare_function = [](const T& a, const T& b) -> bool { return a < b; },
    size_t median_split_length = 5) {
  for (Index i = 0; i < end - start; i += median_split_length) {
    Sort(start + i, start + std::min(i + median_split_length, end - start), SortType::INSERT_SORT, compare_function);
    std::swap(start[i / median_split_length], start[std::min(i + median_split_length / 2, end - start - 1)]);
  }
  if ((end - start) <= median_split_length) {
    return start[0];
  } else {
    return GetApproximateMedian(start, start + (end - start) / median_split_length + 1, compare_function,
                                median_split_length);
  }
}
template <typename T>
T GetKthElement(NormIterator<T> start, NormIterator<T> end, Index k, const CmpType<T>& compare_function) {
  T pivot = GetApproximateMedian(start, end, compare_function);
  auto smaller = start, now = start, bigger = end;
  while (now < bigger) {
    if (compare_function(*now, pivot)) {
      std::swap(*now, *smaller);
      smaller++;
      now++;
    } else if (compare_function(pivot, *now)) {
      bigger--;
      std::swap(*now, *bigger);
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
template <typename T>
void Sort(NormIterator<T> start, NormIterator<T> end) {
  Sort(start, end, SortType::QUICK_SORT);
}
template <typename T, typename UnknownCmpType>
void Sort(NormIterator<T> start, NormIterator<T> end, const UnknownCmpType& compare_function) {
  const CmpType<T>& compare_function_known = compare_function;
  Sort(start, end, SortType::QUICK_SORT, compare_function_known);
}
template <typename T>
void Sort(NormIterator<T> start, NormIterator<T> end, SortType sort_type) {
  const CmpType<T>& compare_function = std::less<T>();
  Sort(start, end, sort_type, compare_function);
}
template <typename T, typename GapGeneration = ShellNormalGapGeneration, typename UnknownCmpType>
void Sort(NormIterator<T> start, NormIterator<T> end, SortType sort_type, const UnknownCmpType& compare_function) {
  const CmpType<T>& compare_function_known = compare_function;
  Sort(start, end, sort_type, compare_function_known);
}
template <int_type T, typename GapGeneration = ShellNormalGapGeneration>
void Sort(NormIterator<T> start, NormIterator<T> end, SortType sort_type, const CmpType<T>& compare_function) {
  if (sort_type == SortType::QUICK_SORT) {
    QuickSort(start, end, compare_function, 0, -1);
  } else if (sort_type == SortType::INSERT_SORT) {
    InsertSort(start, end, compare_function);
  } else if (sort_type == SortType::MERGE_SORT) {
    MergeSort(start, end, compare_function);
  } else if (sort_type == SortType::COUNT_SORT) {
    CountSort(start, end);
  } else if (sort_type == SortType::HEAP_SORT) {
    HeapSort(start, end, compare_function);
  } else if (sort_type == SortType::INTRO_SORT) {
    QuickSort(start, end, compare_function, 0, std::log2(end - start));
  } else if (sort_type == SortType::BUCKET_SORT) {
    BucketSort(start, end);
  } else if (sort_type == SortType::SHELL_SORT) {
    GapGeneration gap_generation;
    ShellSort(start, end, compare_function, &gap_generation);
  } else if (sort_type == SortType::TOURNAMENT_SORT) {
    T max_value = *std::max_element(start, end, compare_function);
    TournamentSort(start, end, compare_function, max_value);
  } else if (sort_type == SortType::TIM_SORT) {
    TimSort(start, end, compare_function);
  } else {
    throw std::logic_error("no such sort algorithm");
  }
}
template <not_int_type T, typename GapGeneration = ShellNormalGapGeneration>
void Sort(NormIterator<T> start, NormIterator<T> end, SortType sort_type, const CmpType<T>& compare_function) {
  if (sort_type == SortType::QUICK_SORT) {
    QuickSort(start, end, compare_function, 0, -1);
  } else if (sort_type == SortType::INSERT_SORT) {
    InsertSort(start, end, compare_function);
  } else if (sort_type == SortType::MERGE_SORT) {
    MergeSort(start, end, compare_function);
  } else if (sort_type == SortType::COUNT_SORT) {
    throw std::logic_error("this type can't use Count_Sort");
  } else if (sort_type == SortType::HEAP_SORT) {
    HeapSort(start, end, compare_function);
  } else if (sort_type == SortType::INTRO_SORT) {
    QuickSort(start, end, compare_function, 0, std::log2(end - start));
  } else if (sort_type == SortType::BUCKET_SORT) {
    throw std::logic_error("this type can't use Bucket_Sort");
  } else if (sort_type == SortType::SHELL_SORT) {
    GapGeneration gap_generation;
    ShellSort(start, end, compare_function, &gap_generation);
  } else if (sort_type == SortType::TOURNAMENT_SORT) {
    T max_value = *std::max_element(start, end, compare_function);
    TournamentSort(start, end, compare_function, max_value);
  } else if (sort_type == SortType::TIM_SORT) {
    TimSort(start, end, compare_function);
  } else {
    throw std::logic_error("no such sort algorithm");
  }
}
}  // namespace lhy
#endif  // MY_STL_SORT_H