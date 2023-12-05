//
// Created by lhy31 on 2023/8/26.
//

#ifndef MY_STL_LIST_H
#define MY_STL_LIST_H
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "data_structure.h"
namespace lhy {
template <typename T>
struct listNode;
template <typename T>
class queue;
template <typename T>
class list : public DataStructure<T, listNode<T>> {
 public:
  friend class queue<T>;
  class iterator;
  using ListNode = listNode<T>;
  using typename DataStructure<T, ListNode>::Pointer;

  list();
  list(iterator, iterator);
  ~list();
  [[nodiscard]] iterator begin();
  [[nodiscard]] iterator end();
  [[nodiscard]] iterator rbegin();
  [[nodiscard]] iterator rend();
  [[nodiscard]] const iterator begin() const;
  [[nodiscard]] const iterator end() const;
  [[nodiscard]] const iterator rbegin() const;
  [[nodiscard]] const iterator rend() const;
  [[nodiscard]] const iterator cbegin() const;
  [[nodiscard]] const iterator cend() const;
  void clear();
  [[nodiscard]] bool empty() const;
  iterator erase(const T&);
  iterator erase(iterator);
  [[nodiscard]] iterator find(const T&);
  void insert(iterator, const T&);
  void pop();
  void push_back(const T&);
  [[nodiscard]] std::string show(int count_limit = 20) const;
  [[nodiscard]] size_t size() const;

 private:
  iterator start_;
  iterator end_;
  iterator rend_;
  size_t size_{};
  void updateStart() { start_ = rend_.getNext(); };
  Pointer getBegin() override { return &start_; }
  Pointer getEnd() override { return &end_; }
};

template <typename T>
struct listNode {
  listNode() : next_(nullptr), last_(nullptr){};
  explicit listNode(const T& content) : listNode() { content_ = content; };
  T& operator*() { return content_; }
  list<T>::iterator next_;
  list<T>::iterator last_;
  T content_;
};
template <typename T>
class list<T>::iterator : public TwoDirectionIterator<ListNode> {
 public:
  using TwoDirectionIterator<ListNode>::TwoDirectionIterator;
  T& operator*() { return this->getPointer()->content_; }
  const T& operator*() const { return this->getPointer()->content_; }
  T* operator->() { return &(this->getPointer()->content_); }
  iterator& operator++() override {
    this->getPointer() = this->getNext();
    return *this;
  }
  iterator& operator--() override {
    this->getPointer() = this->getLast();
    return *this;
  }
  iterator& operator++(int i) override { return operator++(); }
  iterator& operator--(int i) override { return operator--(); }
  iterator& getNext() { return this->getPointer()->next_; }
  const iterator& getNext() const { return this->getPointer()->next_; }
  iterator& getLast() { return this->getPointer()->last_; }
  const iterator& getLast() const { return this->getPointer()->last_; }
};

template <typename T>
list<T>::list() {
  rend_ = new ListNode();
  end_ = new ListNode();
  rend_.getNext() = end_;
  end_.getLast() = rend_;
  start_ = end_;
  size_ = 0;
}
template <typename T>
list<T>::list(iterator start, iterator end) : list() {
  for (auto element = start; element != end; ++element) {
    push_back(*element);
  }
}
template <typename T>
list<T>::~list() {
  clear();
}
template <typename T>
list<T>::iterator list<T>::begin() {
  updateStart();
  return start_;
}
template <typename T>
list<T>::iterator list<T>::end() {
  return end_;
}
template <typename T>
list<T>::iterator list<T>::rbegin() {
  return end_.getLast();
}
template <typename T>
list<T>::iterator list<T>::rend() {
  return rend_;
}
template <typename T>
const list<T>::iterator list<T>::begin() const {
  // 由于是const的所以没有update start_
  return start_;
}
template <typename T>
const list<T>::iterator list<T>::end() const {
  return end_;
}
template <typename T>
const list<T>::iterator list<T>::rbegin() const {
  return end_.getLast();
}
template <typename T>
const list<T>::iterator list<T>::rend() const {
  return rend_;
}
template <typename T>
const list<T>::iterator list<T>::cbegin() const {
  updateStart();
  return start_;
}
template <typename T>
const list<T>::iterator list<T>::cend() const {
  return end_;
}
template <typename T>
void list<T>::clear() {
  while (!empty()) {
    pop();
  }
}
template <typename T>
bool list<T>::empty() const {
  return size_ == 0;
}
template <typename T>
list<T>::iterator list<T>::erase(const T& content) {
  iterator answer = find(content);
  if (answer != end_) {
    return erase(answer);
  } else {
    throw std::logic_error("the element is not in the list");
  }
}
template <typename T>
list<T>::iterator list<T>::erase(list::iterator node) {
  if (node == rend_ || node == end_) {
    throw std::logic_error("end iterator can not be erased");
  }
  if (empty()) {
    throw std::logic_error("nothing left to_ erase");
  }
  auto node_behind = node.getNext();
  auto node_before = node.getLast();
  delete node;
  node_behind.getLast() = node_before;
  node_before.getNext() = node_behind;
  size_--;
  return node_behind;
}
template <typename T>
list<T>::iterator list<T>::find(const T& content) {
  iterator answer = begin();
  while (answer != end_ && answer->content_ != content) {
    answer++;
  }
  return answer;
}

template <typename T>
void list<T>::insert(list::iterator node_behind, const T& content) {
  auto node_before = node_behind.getLast();
  iterator node_temp = new ListNode(content);
  node_before.getNext() = node_temp;
  node_behind.getLast() = node_temp;
  node_temp.getNext() = node_behind;
  node_temp.getLast() = node_before;
  size_++;
}

template <typename T>
void list<T>::pop() {
  if (empty()) {
    throw std::logic_error("the list is empty");
  }
  erase(end_.getLast());
}
template <typename T>
void list<T>::push_back(const T& content) {
  insert(end_, content);
}
template <typename T>
std::string list<T>::show(int count_limit) const {
  std::stringstream string_stream;
  for (auto element = begin(); element != end(); element++) {
    string_stream << *element << " ";
    count_limit--;
    if (count_limit <= 0) {
      break;
    }
  }
  return string_stream.str();
}
template <typename T>
size_t list<T>::size() const {
  return size_;
}

}  // namespace lhy
#endif  // MY_STL_LIST_H