//
// Created by lhy31 on 2023/8/26.
//

#ifndef MY_STL_LIST_H
#define MY_STL_LIST_H
#include <iostream>
#include <sstream>
#include <stdexcept>
namespace lhy {
template <typename T>
class list {
 private:
  struct ListNode {
    explicit ListNode(const T& content) {
      next_ = nullptr;
      last_ = nullptr;
      content_ = content;
    }
    ListNode() {
      next_ = nullptr;
      last_ = nullptr;
    }
    T& operator*() { return content_; }
    ListNode* operator++() {
      if (this->next_ == nullptr) {
        throw std::logic_error("the iterator is the last one");
      }
      this = this->next_;
      return this;
    }
    ListNode* operator--() {
      if (this->last_ == nullptr) {
        throw std::logic_error("the iterator is the last one");
      }
      this = this->last_;
      return this;
    }
    T content_;
    ListNode* next_;
    ListNode* last_;
  };

 public:
  list();
  list(T*, T*);
  ~list();
  void clear();
  void push_back(const T&);
  ListNode* begin() const;
  ListNode* end() const;
  ListNode* rbegin() const;
  ListNode* rend() const;
  size_t size() const;
  bool empty() const;
  void insert(ListNode*, const T&);
  void pop();
  ListNode* find(const T&);
  ListNode* erase(ListNode*);
  ListNode* erase(const T&);
  std::string show(int count_limit = 20) const;

 private:
  ListNode* rend_;
  ListNode* end_;
  size_t size_{};
};
template <typename T>
typename list<T>::ListNode* list<T>::find(const T& content) {
  ListNode* answer = begin();
  while (answer != end_ && answer->content_ != content) {
    answer = answer->next_;
  }
  return answer;
}
template <typename T>
typename list<T>::ListNode* list<T>::erase(const T& content) {
  ListNode* answer = find(content);
  if (answer != end_) {
    return erase(answer);
  } else {
    throw std::logic_error("the element is not in the list");
  }
}
template <typename T>
std::string list<T>::show(int count_limit) const {
  std::stringstream string_stream;
  for (auto* element = begin(); element != end(); element = element->next_) {
    string_stream << element->content_ << " ";
    count_limit--;
    if (count_limit <= 0) {
      break;
    }
  }
  return string_stream.str();
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
list<T>::~list() {
  clear();
}
template <typename T>
list<T>::list(T* start, T* end) : list() {
  for (auto element = start; element != end; ++element) {
    push_back(*element);
  }
}
template <typename T>
void list<T>::pop() {
  if (empty()) {
    throw std::logic_error("the list is empty");
  }
  erase(end_->last_);
}
template <typename T>
typename list<T>::ListNode* list<T>::erase(list::ListNode* node) {
  if (node == rend_ || node == end_) {
    throw std::logic_error("end iterator can not be erased");
  }
  if (empty()) {
    throw std::logic_error("nothing left to_ erase");
  }
  auto* node_behind = node->next_;
  auto* node_before = node->last_;
  delete node;
  node_behind->last_ = node_before;
  node_before->next_ = node_behind;
  size_--;
  return node_behind;
}
template <typename T>
void list<T>::insert(list::ListNode* node_behind, const T& content) {
  auto* node_before = node_behind->last_;
  auto* node_temp = new ListNode(content);
  node_before->next_ = node_temp;
  node_behind->last_ = node_temp;
  node_temp->next_ = node_behind;
  node_temp->last_ = node_before;
  size_++;
}
template <typename T>
size_t list<T>::size() const {
  return size_;
}
template <typename T>
typename list<T>::ListNode* list<T>::rend() const {
  return rend_;
}
template <typename T>
typename list<T>::ListNode* list<T>::rbegin() const {
  return end_->last_;
}
template <typename T>
typename list<T>::ListNode* list<T>::end() const {
  return end_;
}
template <typename T>
typename list<T>::ListNode* list<T>::begin() const {
  return rend_->next_;
}
template <typename T>
void list<T>::push_back(const T& content) {
  insert(end_, content);
}
template <typename T>
list<T>::list() {
  rend_ = new ListNode();
  end_ = new ListNode();
  rend_->next_ = end_;
  end_->last_ = rend_;
  size_ = 0;
}

}  // namespace lhy
#endif  // MY_STL_LIST_H