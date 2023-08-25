//
// Created by lhy31 on 2023/8/24.
//

#ifndef MY_STL_PRIORITY_QUEUE_H
#define MY_STL_PRIORITY_QUEUE_H
#include "../basic.h"
#include "vector.h"
namespace lhy {
template <typename T, typename CmpType = std::less<T>>
class priority_queue {
 public:
  priority_queue();
  priority_queue(T *, T *);
  ~priority_queue();
  void push(T);
  void pop();
  T &front() const;

 private:
  const Index root = 0;
  vector<T> priority_queue_;
  CmpType compare_function_;
  void Up(Index);
  void Down(Index);
  Index GetFather(Index) const;
  size_t size() const;
  bool empty() const;
  Index GetLeftChild(Index) const;
  Index GetRightChild(Index) const;
  bool CheckRange(Index) const;
  void CreateHeap();
};
template <typename T, typename CmpType>
T &priority_queue<T, CmpType>::front() const {
  return priority_queue_.front();
}
template <typename T, typename CmpType>
void priority_queue<T, CmpType>::CreateHeap() {
  for (int i = size() >> 1; i >= 0; --i) {
    Down(i);
  }
}
template <typename T, typename CmpType>
priority_queue<T, CmpType>::priority_queue(T *start, T *end) {
  for (auto &element = start; element != end; element++) {
    priority_queue_.push_back(element);
  }
}
template <typename T, typename CmpType>
bool priority_queue<T, CmpType>::empty() const {
  return priority_queue_.empty();
}
template <typename T, typename CmpType>
void priority_queue<T, CmpType>::pop() {
  if (empty()) {
    throw std::range_error("nothing left to pop");
  }
  std::swap(priority_queue_.back(), priority_queue_.front());
  priority_queue_.pop();
  Down(0);
}
template <typename T, typename CmpType>
void priority_queue<T, CmpType>::push(T element) {
  priority_queue_.push_back(element);
  Up(size() - 1);
}
template <typename T, typename CmpType>
priority_queue<T, CmpType>::~priority_queue() = default;
template <typename T, typename CmpType>
priority_queue<T, CmpType>::priority_queue() = default;
template <typename T, typename CmpType>
bool priority_queue<T, CmpType>::CheckRange(Index index) const {
  return index < size() && index >= 0;
}
template <typename T, typename CmpType>
Index priority_queue<T, CmpType>::GetRightChild(Index index) const {
  return 2 * index + 2;
}
template <typename T, typename CmpType>
Index priority_queue<T, CmpType>::GetLeftChild(Index index) const {
  return 2 * index + 1;
}
template <typename T, typename CmpType>
size_t priority_queue<T, CmpType>::size() const {
  return priority_queue_.size();
}
template <typename T, typename CmpType>
void priority_queue<T, CmpType>::Down(Index index) {
  Index left_child_index = GetLeftChild(index);
  Index right_child_index = GetRightChild(index);
  if (!CheckRange(left_child_index) && !CheckRange(right_child_index)) {
    return;
  } else {
    Index better_child_index;
    if (CheckRange(left_child_index) && CheckRange(right_child_index)) {
      if (compare_function_(priority_queue_[left_child_index], priority_queue_[right_child_index])) {
        better_child_index = left_child_index;
      } else {
        better_child_index = right_child_index;
      }
    } else if (CheckRange(left_child_index)) {
      better_child_index = left_child_index;
    } else if (CheckRange(right_child_index)) {
      better_child_index = right_child_index;
    }
    if (compare_function_(priority_queue_[better_child_index], priority_queue_[index])) {
      std::swap(priority_queue_[better_child_index], priority_queue_[index]);
      Down(better_child_index);
    }
  }
}
template <typename T, typename CmpType>
void priority_queue<T, CmpType>::Up(Index index) {
  if (index == root) {
    return;
  }
  Index father_index = GetFather(index);
  if (compare_function_(priority_queue_[index], priority_queue_[father_index])) {
    std::swap(priority_queue_[index], priority_queue_[father_index]);
    Up(father_index);
  }
}
template <typename T, typename CmpType>
Index priority_queue<T, CmpType>::GetFather(Index index) const {
  return (index - 1) / 2;
}

}  // namespace lhy
#endif  // MY_STL_PRIORITY_QUEUE_H