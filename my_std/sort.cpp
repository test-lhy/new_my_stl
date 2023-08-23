//
// Created by lhy31 on 2023/8/22.
//
#include <functional>
#include <list>
#include <stdexcept>

#include "random.cpp"
#include "vector.h"

namespace lhy {
enum SortType { QUICK_SORT, INSERT_SORT, MERGE_SORT, COUNT_SORT, BUCKET_SORT, RADIX_SORT };

template <typename T>
void Sort(T* start, T* end) {
  Sort(start, end, SortType::QUICK_SORT);
}

template <typename T, typename CMPTYPE>
void Sort(T* start, T* end, CMPTYPE compare_function) {
  Sort(start, end, SortType::QUICK_SORT, compare_function);
}

template <typename T>
void Sort(T* start, T* end, SortType sort_type) {
  Sort(start, end, SortType::QUICK_SORT, std::less<T>());
}

//std::less<T>()：这是一个标准库中的函数对象，它表示一个比较器，用于在排序过程中比较两个元素。std::less<T>()会生成一个对象，当第一个参数小于第二个参数时，它的调用结果为true

template <typename T, typename CMPTYPE>
void Sort(const T* start, const T* end, SortType sort_type, CMPTYPE compare_function) {
  if (sort_type == SortType::QUICK_SORT) {
    QuickSort(start, end, compare_function, false);
  } else if (sort_type == SortType::INSERT_SORT) {
    InsertSort(start, end, compare_function);
  } else if (sort_type == SortType::MERGE_SORT) {
    Merge_Sort(start, end, compare_function);
  } else if (sort_type == SortType::COUNT_SORT) {
    if (typeid(T) == typeid(int) || typeid(T) == typeid(char)) {
      Count_Sort(start, end);
    } else {
      throw std::logic_error("this type can't use Count_Sort");
    }
  } else if (sort_type == SortType::BUCKET_SORT) {
    if (typeid(T) == typeid(int) || typeid(T) == typeid(char)) {
      Bucket_Sort(start, end);
    } else {
      throw std::logic_error("this type can't use Count_Sort");
    }
  }
}

template <typename... T>
void Sort(std::tuple<T...>* start, std::tuple<T...>* end, SortType sort_type = SortType::QUICK_SORT) {
  RadixSort(start, end, sort_type);
}

/*允许函数Sort接受任意数量和类型的参数。...是C++11引入的新特性，表示这是一个参数包，它可以将零个或多个参数打包成一个参数集合。
 *std::tuple<T...>* start：一个指向元组数组的指针，这个数组是待排序的序列的开始位置。
 *std::tuple 是 C++ 标准库中的一个类模板，它代表了一个固定大小的异构数据集合，可以包含不同类型的元素。
 */

template <typename T, typename CMPTYPE>
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

/*InsertSort(插入排序):对于少量元素的排序，它是一个有效的算法。将一个记录插入到已经排好序的有序表中，从而一个新的、记录数增 1 的有序表。在其实现过程使用双层循环，外层循环对除了第一个元素之外的所有元素，内层循环对当前元素前面有序表进行待插入位置查找，并进行移动。
 *std::list 容器，用于存储排序过程中的元素。std::list 是一个双向链表，适合在任意位置插入元素。
 *排序过程中使用 compare_function 来确定元素的顺序。
 *对于每个元素，从 temp_list 的开始迭代器开始，使用 compare_function 比较元素，直到找到正确的插入位置。
将元素插入到 temp_list 中的正确位置。
 */

template <typename T, typename CMPTYPE>
void QuickSort(T* start, T* end, CMPTYPE compare_function, bool randomized) {//randomized:一个布尔值，指示是否使用随机化来选择枢纽元
  if (end - start == 1) {
    return;
  }//检查待排序的数组是否只有一个元素，如果是，则直接返回，因为单个元素已经排序。
  if (!randomized) {
    randomized = true;
    randomize(start, end);
  }//调用 randomize 函数来随机化数组元素。
  auto* temp_array = new std::pair<T, int>[end - start];
  //临时数组 temp_array，用于存储元组和整数对，其中元组包含数组元素的值和它们的原始位置。
  auto* temp_array_iterator = temp_array;
  //一个指向 temp_array 的迭代器
  for (auto element = start; element != end; element++) {
    *temp_array_iterator = {*element, static_cast<int>(element - start)};
  }//将数组元素和它们的原始位置复制到 temp_array 中
  std::nth_element(temp_array, temp_array + (end - start) / 2, temp_array + (end - start));
//使用 std::nth_element 算法找到中间位置的元素
  T pivot = (temp_array + (end - start) / 2)->first;
  T pivot_pos = (temp_array + (end - start) / 2)->second;//从 temp_array 中提取枢纽元的值和位置。
  delete[] temp_array;//释放 temp_array 动态分配的内存。
  T* left = start, right = end - 2;
  //初始化两个指针，left 指向数组的开始，right 指向数组的倒数第二个元素（因为最后一个元素是枢纽元）。
  std::swap(start + pivot_pos, end - 1);
  //将枢纽元交换到数组的最后一个位置。
  while (left < right) {
    while (left < right && (!compare_function(pivot, *left))) {
      left++;//递增 left 指针，直到找到一个应该位于枢纽元右边的元素。
    }
    while (left < right && compare_function(pivot, *right)) {
      right--;//递减 right 指针，直到找到一个应该位于枢纽元左边的元素。
    }
    std::swap(*left, *right);
    //交换 left 和 right 指针所指向的元素。
  }
  std::swap(left, end - 1);
  //将枢纽元交换到它的最终位置。
  QuickSort(start, start + left, compare_function, randomized);
  //这行递归地对枢纽元左边的子数组进行快速排序。
  QuickSort(start + left + 1, end, compare_function, randomized);
  //这行递归地对枢纽元右边的子数组进行快速排序。
}
/*快速排序：基准->分区->递归
 *
 */
template <typename T, typename CMPTYPE>
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
/*归并排序：时间复杂度为 O(nlogn)，空间复杂度为 O(n)
 *        适用于数据量大，并且对稳定性有要求的场景
 *
 */
template <typename T, typename CMPTYPE>
void CountSort(T* start, T* end, CMPTYPE compare_function) {
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
  for (int i = 1; i < maxT - minT + 1; ++i) {
    cnt[i] += cnt[i - 1];
  }
  for (int i = end - start - 1; i >= 0; --i) {
    *(temp_array + i) = cnt[*(start + i) - minT];
    cnt[*(start + i) - minT]--;
  }
  for (int i = 0; i < start - end; ++i) {
    *(start + i) = *(temp_array + i);
  }
  delete[] temp_array;
}
/*计数排序：将输入的数据值转化为键存储在额外开辟的数组空间中。作为一种线性时间复杂度的排序，计数排序要求输入的数据必须是有确定范围的整数。
*时间复杂度 O(n + k) 空间复杂度 O(n + k)
 **/

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
  for (int i = 0; i < maxT - minT + 1; ++i) {
    for (int j = 0; j < cnt[i]; ++j) {
      *(start + count) = i;
      count++;
    }
  }
}
/* bucket和count sort 的排序速度很快，但是对序列有要求（整数，且不能特别大）*/

template <typename... T>
void RadixSort(std::tuple<T...>* start, std::tuple<T...>* end, SortType sort_type) {
  using TupleWithIndex = std::pair<std::tuple<T...>, int>;//定义一个元组类型，包含一个元组和一个整型索引。
  auto* temp_array = new TupleWithIndex [end - start];
  for (int i = 0; i < end - start; ++i) {
    *(temp_array + i) = {*(start + i), i};
  }
  for (int i = 0; i < (*start).size(); ++i) {
    for (int j = 0; j < end - start; ++j) {
      *(temp_array + j).second = j;
    }
    Sort(start, end, sort_type, [i](const TupleWithIndex& a, const TupleWithIndex& b)-> bool {  //lambda表达式
      if (std::get<i>(a.first) == std::get<i>(b.first)) {
        return a.second < b.second;
      } else {
        return std::get<i>(a.first) < std::get<i>(b.first);
      }
    });
  }
  for (int i = 0; i < end - start; ++i) {
    *(start + i) = *(temp_array + i).first;
  }
}
/*基数排序
 * 原理是将整数按位数切割成不同的数字，然后按每个位数分别比较。
 * 初始化->复制元组到临时数组->排序->回写->释放
 * 排序
* 在C++中，lambda表达式是一种匿名函数，它可以捕获一定范围内的变量，并可以在函数体内使用。[捕获列表](参数列表) mutable -> 返回类型 {// 函数体}
 */
} // namespace lhy