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
class listNode;
template <typename T>
class queue;

template <typename T>
class list : public DataStructure<T> {
 public:
  friend class queue<T>;
  class iterator;
  class reversed_iterator;
  using ListNode = listNode<T>;
  using typename DataStructure<T>::Pointer;

  list();
  list(iterator, iterator);
  list(const list&);
  list(list&&);
  list(const std::initializer_list<T>&);
  ~list() override;
  [[nodiscard]] iterator begin();
  [[nodiscard]] iterator end();
  [[nodiscard]] reversed_iterator rbegin();
  [[nodiscard]] reversed_iterator rend();
  [[nodiscard]] iterator begin() const;
  [[nodiscard]] iterator end() const;
  [[nodiscard]] reversed_iterator rbegin() const;
  [[nodiscard]] reversed_iterator rend() const;
  [[nodiscard]] iterator cbegin() const;
  [[nodiscard]] iterator cend() const;
  [[noreturn]] void clear();
  list& operator=(const list&);
  list& operator=(list&&) noexcept;
  list& operator=(const std::initializer_list<T>&);
  [[nodiscard]] bool empty() const;
  iterator erase(const T&);
  iterator erase(iterator);
  reversed_iterator erase(reversed_iterator);
  [[nodiscard]] iterator find(const T&);
  [[nodiscard]] iterator find(const T&) const;
  void insert(iterator, const T&);
  void pop();
  void push_back(const T&);
  [[nodiscard]] std::string show(int count_limit = 20) const;
  [[nodiscard]] size_t size() const;

 private:
  std::pair<iterator, reversed_iterator> eraseImpl(iterator);
  // note:实际上这些节点的释放是有顺序的，但是shared_ptr的自动释放可能会不遵从这个顺序，导致出现性能上的一些问题，大概
  iterator end_;
  reversed_iterator rend_;
  size_t size_{};
  Pointer getBegin() override { return make_shared<iterator>(rend_.getNext()); }
  Pointer getEnd() override { return make_shared<iterator>(end_); }
};

template <typename T>
list<T>& list<T>::operator=(const list& other) {
  if (this == &other) {
    return *this;
  }
  this->clear();
  size_ = 0;
  for (auto element = other.begin(); element != other.end(); element++) {
    this->push_back(*element);
  }
  return *this;
}

template <typename T>
list<T>& list<T>::operator=(list&& other) noexcept {
  if (this == &other) {
    return *this;
  }
  std::swap(rend_, other.rend_);
  std::swap(end_, other.end_);
  size_ = other.size_;
  return *this;
}

template <typename T>
list<T>& list<T>::operator=(const std::initializer_list<T>& element_list) {
  clear();
  size_ = 0;
  for (auto& element : element_list) {
    push_back(element);
  }
  return *this;
}

template <typename T>
class listNode : public StoredIteratorType<T> {
 public:
  listNode() : next_(nullptr), last_(nullptr){};
  explicit listNode(const T& content) : listNode() { content_ = content; };
  T& operator*() { return content_; }
  T* extract() override { return &content_; }
  list<T>::iterator next_;
  list<T>::iterator last_;
  T content_;
};
template <typename T>
class list<T>::iterator : public TwoDirectionIterator<T, ListNode> {
  friend class list<T>;
  using typename Iterator<T, ListNode>::Pointer;
  using typename Iterator<T, ListNode>::OutPointer;

 public:
  using TwoDirectionIterator<T, ListNode>::TwoDirectionIterator;

  iterator(const Iterator<T, ListNode>& other) : Iterator<T, ListNode>(other) {}

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
  virtual iterator& Next() { return this->getPointer()->next_; }
  virtual const iterator& Next() const { return this->getPointer()->next_; }
  virtual iterator& Last() { return this->getPointer()->last_; }
  virtual const iterator& Last() const { return this->getPointer()->last_; }
  ~iterator() override = default;

 protected:
  iterator& getNext() { return this->getPointer()->next_; }
  const iterator& getNext() const { return this->getPointer()->next_; }
  iterator& getLast() { return this->getPointer()->last_; }
  const iterator& getLast() const { return this->getPointer()->last_; }
};

template <typename T>
class list<T>::reversed_iterator : public iterator {
  friend class list;
  using typename Iterator<T, ListNode>::Pointer;
  using typename Iterator<T, ListNode>::OutPointer;

 public:
  using iterator::iterator;

  reversed_iterator(const Iterator<T, ListNode>& other) : Iterator<T, ListNode>(other) {}

  reversed_iterator& operator++() override {
    this->getPointer() = this->getLast();
    return *this;
  }

  reversed_iterator& operator--() override {
    this->getPointer() = this->getNext();
    return *this;
  }
  ~reversed_iterator() override = default;
  reversed_iterator& operator++(int i) override { return operator++(); }
  reversed_iterator& operator--(int i) override { return operator--(); }
  const reversed_iterator& Next() const override { return this->getPointer()->last_; }
  const reversed_iterator& Last() const override { return this->getPointer()->next_; }
};

template <typename T>
list<T>::list(const list& other) {
  rend_ = new ListNode();
  end_ = new ListNode();
  rend_.getNext() = end_;
  end_.getLast() = rend_;
  size_ = 0;
  for (auto element = other.begin(); element != other.end(); element++) {
    this->push_back(*element);
  }
}

template <typename T>
list<T>::list(list&& other) {
  std::swap(rend_, other.rend_);
  std::swap(end_, other.end_);
  size_ = other.size();
}
template <typename T>
list<T>::list(const std::initializer_list<T>& element_list) : list() {
  for (auto& element : element_list) {
    push_back(element);
  }
}

template <typename T>
list<T>::list() {
  rend_ = new ListNode();
  end_ = new ListNode();
  rend_.getNext() = end_;
  end_.getLast() = rend_;
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
  delete end_;
  delete rend_;
}

template <typename T>
typename list<T>::iterator list<T>::begin() {
  return rend_.getNext();
}

template <typename T>
typename list<T>::iterator list<T>::end() {
  return end_;
}

template <typename T>
typename list<T>::reversed_iterator list<T>::rbegin() {
  return end_.getLast();
}

template <typename T>
typename list<T>::reversed_iterator list<T>::rend() {
  return rend_;
}

template <typename T>
typename list<T>::iterator list<T>::begin() const {
  return rend_.getNext();
}

template <typename T>
typename list<T>::iterator list<T>::end() const {
  return end_;
}

template <typename T>
typename list<T>::reversed_iterator list<T>::rbegin() const {
  return end_.getLast();
}

template <typename T>
typename list<T>::reversed_iterator list<T>::rend() const {
  return rend_;
}

template <typename T>
typename list<T>::iterator list<T>::cbegin() const {
  return rend_.getNext();
}

template <typename T>
typename list<T>::iterator list<T>::cend() const {
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
typename list<T>::iterator list<T>::erase(const T& content) {
  iterator answer = find(content);
  if (answer != end_) {
    return erase(answer);
  }
  throw std::logic_error("the element is not in the list");
}

template <typename T>
std::pair<typename list<T>::iterator, typename list<T>::reversed_iterator> list<T>::eraseImpl(iterator node) {
  if (node == rend_ || node == end_) {
    throw std::logic_error("end iterator can not be erased");
  }
  if (empty()) {
    throw std::logic_error("nothing left to_ erase");
  }
  // 当这边是auto&的时候，由于node被删掉会出问题，但是，在Iterator基类没有写析构函数的时候没有出问题，但是在其写了析构函数之后即便是default也会出现虚表找不到的问题
  auto node_behind = node.Next();
  auto node_before = node.Last();
  delete node;
  node_behind.Last() = node_before;
  node_before.Next() = node_behind;
  size_--;
  return {node_behind, node_before};
}
template <typename T>
typename list<T>::iterator list<T>::erase(iterator node) {
  return eraseImpl(node).first;
}

template <typename T>
typename list<T>::reversed_iterator list<T>::erase(reversed_iterator node) {
  return eraseImpl(node).second;
}
template <typename T>
typename list<T>::iterator list<T>::find(const T& content) const {
  iterator answer = begin();
  while (answer != end_ && *answer != content) {
    ++answer;
  }
  return answer;
}

template <typename T>
typename list<T>::iterator list<T>::find(const T& content) {
  iterator answer = begin();
  while (answer != end_ && *answer != content) {
    ++answer;
  }
  return answer;
}

template <typename T>
void list<T>::insert(iterator node_behind, const T& content) {
  auto node_before = node_behind.getLast();
  // todo:多个一个的new需要通过allocator之类的方法来优化掉
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
