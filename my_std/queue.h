//
// Created by lhy31 on 2023/9/1.
//

#ifndef MY_STL_QUEUE_H
#define MY_STL_QUEUE_H
#include "list.h"
namespace lhy {
template <typename T>
class queue {
 public:
  queue();
  queue(T*, T*);
  ~queue();
  void push(const T&);
  bool empty() const;
  void clear();
  T& front() const;
  void size() const;
  void pop();
  std::string show(int) const;

 private:
  list<T> queue_;
};
template <typename T>
std::string queue<T>::show(int count_limit) const {
  return queue_.show(count_limit);
}
template <typename T>
void queue<T>::pop() {
  if (empty()) {
    throw std::logic_error("queue is empty");
  }
  queue_.erase(queue_.begin());
}
template <typename T>
queue<T>::queue(T* start, T* end) {
  for (auto* element = start; element != end; ++element) {
    queue_.push_back(*element);
  }
}
template <typename T>
void queue<T>::size() const {
  return queue_.size();
}
template <typename T>
T& queue<T>::front() const {
  return queue_.begin()->content_;
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
void queue<T>::push(const T& data) {
  queue_.push_back(data);
}
template <typename T>
queue<T>::~queue() = default;
template <typename T>
queue<T>::queue() = default;
}  // namespace lhy
#endif  // MY_STL_QUEUE_H