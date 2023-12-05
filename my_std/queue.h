//
// Created by lhy31 on 2023/9/1.
//

#ifndef MY_STL_QUEUE_H
#define MY_STL_QUEUE_H
#include "list.h"
namespace lhy {
template <typename T>
class queue : public DataStructure<T, typename list<T>::ListNode> {
 public:
  using iterator = list<T>::iterator;
  using typename DataStructure<T, typename list<T>::ListNode>::Pointer;
  queue();
  queue(iterator, iterator);
  ~queue();
  void clear();
  [[nodiscard]] bool empty() const;
  [[nodiscard]] T& front();
  void pop();
  void push(const T&);
  [[nodiscard]] std::string show(int) const;
  [[nodiscard]] size_t size() const;

 private:
  list<T> queue_;
  Pointer getBegin() override { return queue_.getBegin(); }
  Pointer getEnd() override { return queue_.getEnd(); }
};
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