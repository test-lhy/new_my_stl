//
// Created by lhy on 25-8-7.
//

#ifndef CON_MUTEX_LIST_H
#define CON_MUTEX_LIST_H
#include <atomic>
#include <functional>
#include <list>
#include <memory>
#include <mutex>

template <typename T>
class ConMutexList {
 public:
  struct ListNode {
    explicit ListNode(const T& value) : value_(value) {}
    ListNode() = default;
    T value_{};
    std::shared_ptr<ListNode> next_;
    std::weak_ptr<ListNode> last_;
  };
  ConMutexList();
  ~ConMutexList();
  ConMutexList(const ConMutexList& other);
  ConMutexList(ConMutexList&& other) noexcept;
  ConMutexList& operator=(const ConMutexList& other);
  ConMutexList& operator=(ConMutexList&& other) noexcept;

  void push_back(const T& value);
  void push_back(T&& value);

  void pop_back();

  void push_front(const T& value);
  void push_front(T&& value);

  void pop_front();

  size_t size() const;

  bool empty() const;

  void clear();

  T at(int index) const;

  T operator[](int index) const;

  T front() const;

  T back() const;
  template <typename U>
  std::vector<U> for_each(std::function<U(T&)> f);

  void for_each(std::function<void(T&)> f);

 private:
  std::shared_ptr<ListNode> head_;
  std::shared_ptr<ListNode> tail_;
  mutable std::atomic<size_t> size_{};
  mutable std::mutex mutex_;
};
template <typename T>
ConMutexList<T>::ConMutexList() : head_(std::make_shared<ListNode>()), tail_(std::make_shared<ListNode>()) {
  head_->next_ = tail_;
  tail_->last_ = head_;
}
template <typename T>
ConMutexList<T>::~ConMutexList() {
  clear();
}
template <typename T>
ConMutexList<T>::ConMutexList(const ConMutexList& other) : ConMutexList() {
  std::unique_lock lk(other.mutex_);
  auto now = other.head_->next_;
  while (now != other.tail_) {
    push_back(now->value_);
    now = now->next_;
  }
}
template <typename T>
ConMutexList<T>::ConMutexList(ConMutexList&& other) noexcept : ConMutexList() {
  std::unique_lock lk(other.mutex_);
  std::swap(head_, other.head_);
  std::swap(tail_, other.tail_);
  other.size_ = size_.exchange(other.size_);
}
template <typename T>
ConMutexList<T>& ConMutexList<T>::operator=(const ConMutexList& other) {
  std::unique_lock lk(other.mutex_);
  if (this == &other) return *this;
  clear();
  auto now = other.head_->next_;
  while (now != other.tail_) {
    push_back(now->value_);
    now = now->next_;
  }
  return *this;
}
template <typename T>
ConMutexList<T>& ConMutexList<T>::operator=(ConMutexList&& other) noexcept {
  std::unique_lock lk(other.mutex_);
  if (this == &other) return *this;
  std::unique_lock lk_self(mutex_);
  std::swap(head_, other.head_);
  std::swap(tail_, other.tail_);
  other.size_ = size_.exchange(other.size_);
  return *this;
}
template <typename T>
void ConMutexList<T>::push_back(const T& value) {
  std::unique_lock lk(mutex_);
  auto node = std::make_shared<ListNode>(value);
  node->next_ = tail_;
  node->last_ = tail_->last_.lock();
  tail_->last_.lock()->next_ = node;
  tail_->last_ = node;
  ++size_;
}
template <typename T>
void ConMutexList<T>::push_back(T&& value) {
  std::unique_lock lk(mutex_);
  auto node = std::make_shared<ListNode>(std::move(value));
  node->next_ = tail_;
  node->last_ = tail_->last_.lock();
  tail_->last_.lock()->next_ = node;
  tail_->last_ = node;
  ++size_;
}
template <typename T>
void ConMutexList<T>::pop_back() {
  std::unique_lock lk(mutex_);
  if (empty()) {
    throw std::out_of_range("list is empty");
  }
  auto now = tail_->last_.lock()->last_.lock();
  tail_->last_ = now;
  now->next_ = tail_;
  --size_;
}
template <typename T>
void ConMutexList<T>::push_front(const T& value) {
  std::unique_lock lk(mutex_);
  auto node = std::make_shared<ListNode>(value);
  node->next_ = head_->next_;
  node->last_ = head_;
  head_->next_->last_ = node;
  head_->next_ = node;
  ++size_;
}
template <typename T>
void ConMutexList<T>::push_front(T&& value) {
  std::unique_lock lk(mutex_);
  auto node = std::make_shared<ListNode>(std::move(value));
  node->next_ = head_->next_;
  node->last_ = head_;
  head_->next_->last_ = node;
  head_->next_ = node;
  ++size_;
}
template <typename T>
void ConMutexList<T>::pop_front() {
  std::unique_lock lk(mutex_);
  if (empty()) {
    throw std::out_of_range("list is empty");
  }
  auto now = head_->next_->next_;
  head_->next_ = now;
  now->last_ = head_;
  --size_;
}
template <typename T>
size_t ConMutexList<T>::size() const {
  return size_;
}
template <typename T>
bool ConMutexList<T>::empty() const {
  size_t size = 0;
  return size_.compare_exchange_strong(size, 0ul);
}
template <typename T>
void ConMutexList<T>::clear() {
  std::unique_lock lk(mutex_);
  if (!empty()) {
    auto now = tail_->last_.lock()->last_.lock();
    while (now != head_) {
      now->next_.reset();
      now = now->last_.lock();
    }
  }
  head_->next_ = tail_;
  tail_->last_ = head_;
  size_ = 0;
}
template <typename T>
T ConMutexList<T>::at(const int index) const {
  std::unique_lock lk(mutex_);
  if (index < 0) {
    if (index < -static_cast<int>(size_)) {
      throw std::out_of_range("index out of range");
    }
    auto now = tail_;
    for (int i = index; i < 0; i++) {
      now = now->last_.lock();
    }
    return now->value_;
  }
  if (index >= size_) {
    throw std::out_of_range("index out of range");
  }
  auto now = head_;
  for (int i = 0; i <= index; i++) {
    now = now->next_;
  }
  return now->value_;
}
template <typename T>
T ConMutexList<T>::operator[](const int index) const {
  return at(index);
}
template <typename T>
T ConMutexList<T>::front() const {
  return at(0);
}
template <typename T>
T ConMutexList<T>::back() const {
  return at(-1);
}
template <typename T>
template <typename U>
std::vector<U> ConMutexList<T>::for_each(std::function<U(T&)> f) {
  std::vector<U> ret{};
  std::unique_lock lk(mutex_);
  auto now = head_->next_;
  while (now != tail_) {
    ret.push_back(f(now->value_));
    now = now->next_;
  }
  return ret;
}
template <typename T>
void ConMutexList<T>::for_each(std::function<void(T&)> f) {
  std::unique_lock lk(mutex_);
  auto now = head_->next_;
  while (now != tail_) {
    f(now->value_);
    now = now->next_;
  }
}
#endif  // CON_MUTEX_LIST_H
