//
// Created by lhy31 on 2023/8/24.
//

#ifndef MY_STL_PRIORITY_QUEUE_H
#define MY_STL_PRIORITY_QUEUE_H
#include "basic.h"
#include "vector.h"
namespace lhy {
template <typename T>
class priority_queue : public DataStructure<T> {
 public:
  using iterator = vector<T>::iterator;
  using typename DataStructure<T>::Pointer;
  explicit priority_queue(const CmpType<T> &cmp = std::less<T>());
  priority_queue(iterator, iterator, const CmpType<T> &cmp = std::less<T>());
  ~priority_queue();
  [[nodiscard]] bool empty() const;
  void push(T);
  void pop();
  T &top();

 private:
  void Up(Index);
  void Down(Index);
  void CreateHeap();
  [[nodiscard]] bool CheckRange(Index) const;
  [[nodiscard]] Index GetFather(Index) const;
  [[nodiscard]] Index GetLeftChild(Index) const;
  [[nodiscard]] Index GetRightChild(Index) const;
  [[nodiscard]] size_t size() const;

  const Index root = 0;
  vector<T> priority_queue_;
  CmpType<T> compare_function_;
  Pointer getBegin() override { return priority_queue_.getBegin(); }
  Pointer getEnd() override { return priority_queue_.getEnd(); }
};
template <typename T>
priority_queue<T>::priority_queue(const CmpType<T> &cmp) {
  compare_function_ = cmp;
}
template <typename T>
priority_queue<T>::priority_queue(iterator start, iterator end, const CmpType<T> &cmp) {
  compare_function_ = cmp;
  for (auto &element = start; element != end; element++) {
    priority_queue_.push_back(*element);
  }
  CreateHeap();
}
template <typename T>
priority_queue<T>::~priority_queue() = default;
template <typename T>
bool priority_queue<T>::empty() const {
  return priority_queue_.empty();
}
template <typename T>
void priority_queue<T>::push(T element) {
  priority_queue_.push_back(element);
  Up(size() - 1);
}
template <typename T>
void priority_queue<T>::pop() {
  if (empty()) {
    throw std::range_error("nothing left to_ pop");
  }
  std::swap(priority_queue_.back(), priority_queue_.front());
  priority_queue_.pop();
  Down(0);
}
template <typename T>
T &priority_queue<T>::top() {
  return priority_queue_.front();
}
template <typename T>
void priority_queue<T>::Up(Index index) {
  if (index == root) {
    return;
  }
  Index father_index = GetFather(index);
  if (compare_function_(priority_queue_[index], priority_queue_[father_index])) {
    std::swap(priority_queue_[index], priority_queue_[father_index]);
    Up(father_index);
  }
}
template <typename T>
void priority_queue<T>::Down(Index index) {
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
template <typename T>
void priority_queue<T>::CreateHeap() {
  for (int i = size() >> 1; i >= 0; --i) {
    Down(i);
  }
}
template <typename T>
bool priority_queue<T>::CheckRange(Index index) const {
  return index < size() && index >= 0;
}
template <typename T>
Index priority_queue<T>::GetFather(Index index) const {
  return (index - 1) / 2;
}
template <typename T>
Index priority_queue<T>::GetLeftChild(Index index) const {
  return 2 * index + 1;
}
template <typename T>
Index priority_queue<T>::GetRightChild(Index index) const {
  return 2 * index + 2;
}
template <typename T>
size_t priority_queue<T>::size() const {
  return priority_queue_.size();
}
}  // namespace lhy
#endif  // MY_STL_PRIORITY_QUEUE_H