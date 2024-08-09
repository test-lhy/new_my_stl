#include <iostream>
//
// Created by lhy on 24-8-9.
//
#include "sort.h"
using namespace lhy;
using TestTuple = std::tuple<int, int, char>;

// 打印元组的帮助函数
template <typename... T>
void PrintTuple(const std::tuple<T...>& t) {
  std::apply([](auto&&... args) { ((std::cout << args << ' '), ...); }, t);
}

// 打印数组的帮助函数
template <typename... T>
void PrintArray(const vector<std::tuple<T...>>& array) {
  for (const auto& element : array) {
    PrintTuple(element);
    std::cout << std::endl;
  }
}
void TestRadixSort() {
  vector<TestTuple> data = {{3, 9, 'c'}, {1, 7, 'a'}, {2, 8, 'b'}, {1, 5, 'd'}, {3, 6, 'e'}};

  std::cout << "Before sorting:\n";
  PrintArray(data);

  RadixSort(&data[0], &data[0] + data.size(), SortType::QUICK_SORT);

  std::cout << "\nAfter sorting:\n";
  PrintArray(data);
}
int main() {
  TestRadixSort();
}