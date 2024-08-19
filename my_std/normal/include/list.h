//
// Created by lhy31 on 2023/8/26.
//

#ifndef MY_STL_LIST_H
#define MY_STL_LIST_H
#include <iostream>
#include <memory_resource>
#include <sstream>
#include <stdexcept>

#include "data_structure.h"
#include "exception.h"

namespace lhy {
template <typename T>
class listNode;
template <typename T>
class queue;
template <typename T>
class listiterator;
template <typename T>
class listreversed_iterator;
/// 速度提升了四倍
template <typename T, typename Alloc = std::pmr::polymorphic_allocator<listNode<T>>>
class list : public DataStructure<T> {
 public:
  friend class queue<T>;
  using ListNode = listNode<T>;
  using typename DataStructure<T>::Pointer;
  using iterator = listiterator<T>;
  using reversed_iterator = listreversed_iterator<T>;

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
  void clear();
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
  std::pmr::memory_resource* memory_resource_{};
  Alloc* allocator_{};
  Pointer getBegin() override { return make_shared<iterator>(rend_.getNext()); }
  Pointer getEnd() override { return make_shared<iterator>(end_); }
};

template <typename T, typename Alloc>
list<T, Alloc>& list<T, Alloc>::operator=(const list& other) {
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

template <typename T, typename Alloc>
list<T, Alloc>& list<T, Alloc>::operator=(list&& other) noexcept {
  if (this == &other) {
    return *this;
  }
  std::swap(rend_, other.rend_);
  std::swap(end_, other.end_);
  std::swap(memory_resource_, other.memory_resource_);
  std::swap(allocator_, other.allocator_);
  size_ = other.size_;
  return *this;
}

template <typename T, typename Alloc>
list<T, Alloc>& list<T, Alloc>::operator=(const std::initializer_list<T>& element_list) {
  clear();
  size_ = 0;
  for (auto& element : element_list) {
    push_back(element);
  }
  return *this;
}

template <typename T, typename Alloc>
list<T, Alloc>::list(const list& other) : list() {
  for (auto element = other.begin(); element != other.end(); element++) {
    this->push_back(*element);
  }
}

template <typename T, typename Alloc>
list<T, Alloc>::list(list&& other) : list() {
  std::swap(rend_, other.rend_);
  std::swap(end_, other.end_);
  std::swap(memory_resource_, other.memory_resource_);
  std::swap(allocator_, other.allocator_);
  size_ = other.size();
}
template <typename T, typename Alloc>
list<T, Alloc>::list(const std::initializer_list<T>& element_list) : list() {
  for (auto& element : element_list) {
    push_back(element);
  }
}

template <typename T, typename Alloc>
list<T, Alloc>::list() {
  if constexpr (std::is_same_v<Alloc, std::pmr::polymorphic_allocator<ListNode>>) {
    memory_resource_ = new std::pmr::unsynchronized_pool_resource();
    allocator_ = new Alloc(memory_resource_);
  } else {
    allocator_ = new Alloc();
  }
  rend_ = std::allocator_traits<Alloc>::allocate(*allocator_, 1);
  std::allocator_traits<Alloc>::construct(*allocator_, static_cast<ListNode*>(rend_));
  end_ = std::allocator_traits<Alloc>::allocate(*allocator_, 1);
  std::allocator_traits<Alloc>::construct(*allocator_, static_cast<ListNode*>(end_));
  rend_.getNext() = end_;
  end_.getLast() = rend_;
  size_ = 0;
}

template <typename T, typename Alloc>
list<T, Alloc>::list(iterator start, iterator end) : list() {
  for (auto element = start; element != end; ++element) {
    push_back(*element);
  }
}

template <typename T, typename Alloc>
list<T, Alloc>::~list() {
  clear();
  std::allocator_traits<Alloc>::destroy(*allocator_, static_cast<ListNode*>(rend_));
  std::allocator_traits<Alloc>::deallocate(*allocator_, static_cast<ListNode*>(rend_), 1);
  std::allocator_traits<Alloc>::destroy(*allocator_, static_cast<ListNode*>(end_));
  std::allocator_traits<Alloc>::deallocate(*allocator_, static_cast<ListNode*>(end_), 1);
  delete allocator_;
  delete memory_resource_;
}

template <typename T, typename Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::begin() {
  return rend_.getNext();
}

template <typename T, typename Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::end() {
  return end_;
}

template <typename T, typename Alloc>
typename list<T, Alloc>::reversed_iterator list<T, Alloc>::rbegin() {
  return end_.getLast();
}

template <typename T, typename Alloc>
typename list<T, Alloc>::reversed_iterator list<T, Alloc>::rend() {
  return rend_;
}

template <typename T, typename Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::begin() const {
  return rend_.getNext();
}

template <typename T, typename Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::end() const {
  return end_;
}

template <typename T, typename Alloc>
typename list<T, Alloc>::reversed_iterator list<T, Alloc>::rbegin() const {
  return end_.getLast();
}

template <typename T, typename Alloc>
typename list<T, Alloc>::reversed_iterator list<T, Alloc>::rend() const {
  return rend_;
}

template <typename T, typename Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::cbegin() const {
  return rend_.getNext();
}

template <typename T, typename Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::cend() const {
  return end_;
}

template <typename T, typename Alloc>
void list<T, Alloc>::clear() {
  while (!empty()) {
    pop();
  }
}

template <typename T, typename Alloc>
bool list<T, Alloc>::empty() const {
  return size_ == 0;
}

template <typename T, typename Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::erase(const T& content) {
  iterator answer = find(content);
  if (answer != end_) {
    return erase(answer);
  }
  throw logic_error("the element is not in the list");
}

template <typename T, typename Alloc>
std::pair<typename list<T, Alloc>::iterator, typename list<T, Alloc>::reversed_iterator> list<T, Alloc>::eraseImpl(
    iterator node) {
  if (node == rend_ || node == end_) {
    throw logic_error("end iterator can not be erased");
  }
  if (empty()) {
    throw logic_error("nothing left to_ erase");
  }
  // 当这边是auto&的时候，由于node被删掉会出问题，但是，在Iterator基类没有写析构函数的时候没有出问题，但是在其写了析构函数之后即便是default也会出现虚表找不到的问题
  auto node_behind = node.Next();
  auto node_before = node.Last();
  std::allocator_traits<Alloc>::destroy(*allocator_, static_cast<ListNode*>(node));
  std::allocator_traits<Alloc>::deallocate(*allocator_, static_cast<ListNode*>(node), 1);
  node_behind.Last() = node_before;
  node_before.Next() = node_behind;
  size_--;
  return {node_behind, node_before};
}
template <typename T, typename Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::erase(iterator node) {
  return eraseImpl(node).first;
}

template <typename T, typename Alloc>
typename list<T, Alloc>::reversed_iterator list<T, Alloc>::erase(reversed_iterator node) {
  return eraseImpl(node).second;
}
template <typename T, typename Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::find(const T& content) const {
  iterator answer = begin();
  while (answer != end_ && *answer != content) {
    ++answer;
  }
  return answer;
}

template <typename T, typename Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::find(const T& content) {
  iterator answer = begin();
  while (answer != end_ && *answer != content) {
    ++answer;
  }
  return answer;
}

template <typename T, typename Alloc>
void list<T, Alloc>::insert(iterator node_behind, const T& content) {
  auto node_before = node_behind.getLast();
  // todo:多个一个的new需要通过allocator之类的方法来优化掉
  iterator node_temp = std::allocator_traits<Alloc>::allocate(*allocator_, 1);
  std::allocator_traits<Alloc>::construct(*allocator_, static_cast<ListNode*>(node_temp), content);
  node_before.getNext() = node_temp;
  node_behind.getLast() = node_temp;
  node_temp.getNext() = node_behind;
  node_temp.getLast() = node_before;
  size_++;
}

template <typename T, typename Alloc>
void list<T, Alloc>::pop() {
  if (empty()) {
    throw logic_error("the list is empty");
  }
  erase(end_.getLast());
}

template <typename T, typename Alloc>
void list<T, Alloc>::push_back(const T& content) {
  insert(end_, content);
}

template <typename T, typename Alloc>
std::string list<T, Alloc>::show(int count_limit) const {
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

template <typename T, typename Alloc>
size_t list<T, Alloc>::size() const {
  return size_;
}
template <typename T>
class listNode : public StoredIteratorType<T> {
 public:
  listNode() : next_(nullptr), last_(nullptr){};
  explicit listNode(const T& content) : listNode() { content_ = content; };
  T& operator*() { return content_; }
  T* extract() override { return &content_; }
  listiterator<T> next_;
  listiterator<T> last_;
  T content_;
};
template <typename T>
class listiterator : public TwoDirectionIterator<T, listNode<T>> {
  template <typename T_, typename Alloc>
  friend class list;

 public:
  using typename Iterator<T, listNode<T>>::Pointer;
  using typename Iterator<T, listNode<T>>::OutPointer;
  using TwoDirectionIterator<T, listNode<T>>::TwoDirectionIterator;

  listiterator(const Iterator<T, listNode<T>>& other) : Iterator<T, listNode<T>>(other) {}

  listiterator& operator++() override {
    this->getPointer() = ToTPointer<T, listNode<T>>(this->getNext());
    return *this;
  }

  listiterator& operator--() override {
    this->getPointer() = ToTPointer<T, listNode<T>>(this->getLast());
    return *this;
  }

  listiterator& operator++(int i) override { return operator++(); }
  listiterator& operator--(int i) override { return operator--(); }
  virtual listiterator& Next() { return this->getPointer()->next_; }
  virtual const listiterator& Next() const { return this->getPointer()->next_; }
  virtual listiterator& Last() { return this->getPointer()->last_; }
  virtual const listiterator& Last() const { return this->getPointer()->last_; }
  ~listiterator() override = default;

 protected:
  listiterator& getNext() { return this->getPointer()->next_; }
  const listiterator& getNext() const { return this->getPointer()->next_; }
  listiterator& getLast() { return this->getPointer()->last_; }
  const listiterator& getLast() const { return this->getPointer()->last_; }
};

template <typename T>
class listreversed_iterator : public listiterator<T> {
  template <typename T_, typename Alloc>
  friend class list;
  using typename Iterator<T, listNode<T>>::Pointer;
  using typename Iterator<T, listNode<T>>::OutPointer;

 public:
  using listiterator<T>::listiterator;

  listreversed_iterator(const Iterator<T, listNode<T>>& other) : Iterator<T, listNode<T>>(other) {}

  listreversed_iterator& operator++() override {
    this->getPointer() = ToTPointer<T, listNode<T>>(this->getLast());
    return *this;
  }

  listreversed_iterator& operator--() override {
    this->getPointer() = ToTPointer<T, listNode<T>>(this->getNext());
    return *this;
  }
  ~listreversed_iterator() override = default;
  listreversed_iterator& operator++(int i) override { return operator++(); }
  listreversed_iterator& operator--(int i) override { return operator--(); }
  const listreversed_iterator& Next() const override { return this->getPointer()->last_; }
  const listreversed_iterator& Last() const override { return this->getPointer()->next_; }
};
}  // namespace lhy
#endif  // MY_STL_LIST_H
