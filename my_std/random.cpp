#include <cstdint>
//
// Created by lhy31 on 2023/8/22.
//
#include <random>
int64_t rand(int64_t a,int64_t b){
  std::mt19937 ran(time(nullptr));
  return ran()%(b-a)+a;
}

//1.在 C++ 中，std::mt19937 是一个类，它提供了生成随机数的能力。使用时，需要先实例化一个 std::mt19937 对象，然后可以通过调用该对象的成员函数 () 来生成随机数。
//2.函数会返回一个在 a 和 b 之间的随机整数（包括 a，但不包括 b）。

template<typename T>
void randomize(T* start,T* end){
  for (int i = 0; i < end - start; ++i) {
    int ran = rand(-1, end - start - 1);
    if (ran != i) {
      std::swap(start + i, start + ran);
    }
  }
}

//1.rand函数包括 -1，但不包括 end - start - 1，该范围被修改（0，end-start)
//2.这个函数通过随机选择一个不同的索引，并将当前索引处的元素与随机索引处的元素交换，从而打乱整个序列的顺序。
