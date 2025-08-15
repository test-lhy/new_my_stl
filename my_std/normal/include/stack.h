//
// Created by lhy31 on 2023/9/1.
//

#ifndef MY_STL_STACK_H
#define MY_STL_STACK_H
#include "algorithm.h"
#include "vector.h"
namespace lhy {
template <typename T>
class stack : public DataStructure<T> {
 public:
  using iterator = typename vector<T>::iterator;
  using reversed_iterator = typename vector<T>::iterator;
  using typename DataStructure<T>::Pointer;
  stack();
  stack(const stack<T>&);
  stack(stack<T>&&);
  stack(iterator, iterator);
  ~stack() override;
  void clear();
  [[nodiscard]] bool empty() const;
  void push(const T&);
  void pop();
  [[nodiscard]] T& top();
  [[nodiscard]] std::string show(int count_limit = 20) const;
  stack<T>& operator=(const stack<T>&);
  stack<T>& operator=(stack<T>&&) noexcept;
  stack<T>& operator=(const std::initializer_list<T>&);

 private:
  vector<T> stack_;
  Pointer getBegin() override { return stack_.getBegin(); }
  Pointer getEnd() override { return stack_.getEnd(); }
};

template <typename T>
stack<T>& stack<T>::operator=(const stack<T>& other) {
  if (this == &other) {
    return *this;
  }
  this->clear();
  this->stack_ = other.stack_;
  return *this;
}
template <typename T>
stack<T>& stack<T>::operator=(stack<T>&& other) noexcept {
  std::swap(this->stack_, other.stack_);
  return *this;
}
template <typename T>
stack<T>& stack<T>::operator=(const std::initializer_list<T>& element_list) {
  clear();
  for (auto& element : element_list) {
    push(element);
  }
  return *this;
}
template <typename T>
stack<T>::stack(const stack<T>& other) {
  stack_ = other.stack_;
}
template <typename T>
stack<T>::stack(stack<T>&& other) {
  std::swap(stack_, other.stack_);
}
template <typename T>
stack<T>::stack() = default;
template <typename T>
stack<T>::stack(iterator start, iterator end) {
  for (auto element = start; element != end; element++) {
    stack_.push_back(*element);
  }
}
template <typename T>
stack<T>::~stack() = default;
template <typename T>
void stack<T>::clear() {
  stack_.clear();
}
template <typename T>
bool stack<T>::empty() const {
  return stack_.empty();
}
template <typename T>
void stack<T>::push(const T& data) {
  stack_.push_back(data);
}
template <typename T>
void stack<T>::pop() {
  stack_.pop();
}
template <typename T>
T& stack<T>::top() {
  return stack_.back();
}
template <typename T>
std::string stack<T>::show(int count_limit) const {
  return lhy::show(stack_.begin(), stack_.end(), count_limit);
}
};      // namespace lhy
#endif  // MY_STL_STACK_H