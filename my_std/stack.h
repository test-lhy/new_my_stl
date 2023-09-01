//
// Created by lhy31 on 2023/9/1.
//

#ifndef MY_STL_STACK_H
#define MY_STL_STACK_H
#include "algorithm.h"
#include "vector.h"
namespace lhy {
template <typename T>
class stack {
 public:
  stack();
  stack(T *, T *);
  ~stack();
  void push(const T &);
  bool empty() const;
  void clear();
  T &top() const;
  void pop();
  std::string show(int count_limit=20) const;
 private:
  vector<T> stack_;
};
template <typename T>
std::string stack<T>::show(int count_limit) const {
  return lhy::show(stack_.begin(), stack_.end(), count_limit);
}
template <typename T>
void stack<T>::pop() {
  stack_.pop();
}
template <typename T>
T &stack<T>::top() const {
  return stack_.back();
}
template <typename T>
void stack<T>::clear() {
  stack_.clear();
}
template <typename T>
bool stack<T>::empty() const {
  return stack_.empty();
}
template <typename T>
void stack<T>::push(const T &data) {
  stack_.push_back(data);
}
template <typename T>
stack<T>::~stack()=default;
template <typename T>
stack<T>::stack(T *start, T *end) {
  for (auto *element = start; element!= end; element++) {
    stack_.push_back(*element);
  }
}
template <typename T>
stack<T>::stack()=default;

};      // namespace lhy
#endif  // MY_STL_STACK_H