//
// Created by lhy31 on 2023/8/26.
//

#ifndef MY_STL_LIST_H
#define MY_STL_LIST_H
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
    ListNode* operator++() {
      if (this->next_ == nullptr) {
        throw std::logic_error("the iterator is the last one");
      }
      return this->next_;
    }
    ListNode* operator--() {
      if (this->last_ == nullptr) {
        throw std::logic_error("the iterator is the last one");
      }
      return this->last_;
    }
    T content_;
    ListNode* next_;
    ListNode* last_;
  };

 public:
  list();
  list(T*,T*);
  void push_back(const T&);
  ListNode* begin() const;
  ListNode* end() const;
  ListNode* rbegin() const;
  ListNode* rend() const;
  size_t size() const;
  void insert(ListNode*, const T&);
  void pop();
  ListNode* erase(ListNode*);
 private:
  ListNode* rend_;
  ListNode* end_;
  size_t size_;
};
template <typename T>
list<T>::list(T* start, T* end) {
  for (auto element = start; element !=end ; ++element) {
    push_back(*element);
  }
  size_=end-start;
}
template <typename T>
void list<T>::pop() {
  erase(end_->last_);
}
template <typename T>
typename list<T>::ListNode* list<T>::erase(list::ListNode* node) {
  if (node==rend_||node==end_){
    throw std::logic_error("end iterator can not be erased");
  }
  if (size_==0){
    throw std::logic_error("nothing left to erase");
  }
  auto* node_behind=node->next_;
  auto* node_before=node->last_;
  delete node;
  node_behind->last_=node_before;
  node_before->next_=node_behind;
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
  size_ = 0;
}

}  // namespace lhy
#endif  // MY_STL_LIST_H