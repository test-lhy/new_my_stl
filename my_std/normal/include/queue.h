//
// Created by lhy31 on 2023/9/1.
//

#ifndef MY_STL_QUEUE_H
#define MY_STL_QUEUE_H
#include "list.h"
namespace lhy {
template <typename T>
class queue : public DataStructure<T> {
 public:
  using iterator = typename list<T>::iterator;
  using reversed_iterator = typename list<T>::reversed_iterator;
  using typename DataStructure<T>::Pointer;
  queue();
  queue(const queue<T>&);
  queue(queue<T>&&);
  queue(iterator, iterator);
  ~queue() override;
  void clear();
  [[nodiscard]] bool empty() const;
  [[nodiscard]] T& front();
  void pop();
  void push(const T&);
  [[nodiscard]] std::string show(int) const;
  [[nodiscard]] size_t size() const;
  queue<T>& operator=(const queue<T>&);
  queue<T>& operator=(queue<T>&&) noexcept;
  queue<T>& operator=(const std::initializer_list<T>&);

 private:
  list<T> queue_;
  Pointer getBegin() override { return queue_.getBegin(); }
  Pointer getEnd() override { return queue_.getEnd(); }
};

template <typename T>
queue<T>& queue<T>::operator=(const queue<T>& other) {
  if (this == &other) {
    return *this;
  }
  this->clear();
  this->queue_ = other.queue_;
  return *this;
}
template <typename T>
queue<T>& queue<T>::operator=(queue<T>&& other) noexcept {
  if (this == &other) return *this;
  clear();
  std::swap(queue_, other.queue_);
  return *this;
}
template <typename T>
queue<T>& queue<T>::operator=(const std::initializer_list<T>& element_list) {
  clear();
  for (auto& element : element_list) {
    push(element);
  }
  return *this;
}
template <typename T>
queue<T>::queue(const queue<T>& other) {
  queue_ = other.queue_;
}
template <typename T>
queue<T>::queue(queue<T>&& other) {
  std::swap(queue_, other.queue_);
}
template <typename T>
queue<T>::queue() = default;
template <typename T>
queue<T>::~queue() = default;
template <typename T>
queue<T>::queue(iterator start, iterator end) {
  for (auto element = start; element != end; ++element) {
    queue_.push_back(*element);
  }
}
template <typename T>
void queue<T>::clear() {
  queue_.clear();
}
template <typename T>
bool queue<T>::empty() const {
  return queue_.empty();
}

template <typename T>
T& queue<T>::front() {
  return *(queue_.begin());
}

template <typename T>
void queue<T>::push(const T& data) {
  queue_.push_back(data);
}
template <typename T>
void queue<T>::pop() {
  if (empty()) {
    throw std::logic_error("queue is empty");
  }
  queue_.erase(queue_.begin());
}
template <typename T>
std::string queue<T>::show(int count_limit) const {
  return queue_.show(count_limit);
}
template <typename T>
size_t queue<T>::size() const {
  return queue_.size();
}
}  // namespace lhy
#endif  // MY_STL_QUEUE_H