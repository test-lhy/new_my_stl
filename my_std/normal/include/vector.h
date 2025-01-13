//
// Created by lhy31 on 2023/8/21.
//

#ifndef MY_STL_VECTOR_H
#define MY_STL_VECTOR_H

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <set>
#include <sstream>
#include <stdexcept>

#include "basic.h"
#include "data_structure.h"
#include "type_traits.h"
namespace lhy {
template <typename T>
class priority_queue;
template <typename T>
class stack;
template <typename T>
class vector : public DataStructure<T> {
 public:
  friend class priority_queue<T>;
  friend class stack<T>;
  using typename DataStructure<T>::Pointer;
  using iterator = NormIterator<T>;
  using reversed_iterator = ReversedNormIterator<T>;
  vector();
  explicit vector(iterator, iterator);
  explicit vector(size_t);
  vector(T* other);
  vector(const T* other);
  vector(const vector&);
  vector(vector&&);
  vector(const std::initializer_list<T>&);
  ~vector() override = default;
  vector& operator=(const vector&);
  vector& operator=(vector&&) noexcept;
  vector& operator=(const std::initializer_list<T>&);
  bool operator==(const vector& other) const;
  vector& operator+=(const T&);
  vector& operator+=(const vector&);
  vector& operator&=(const vector&);
  vector& operator|=(const vector&);
  T& operator[](const Index&);
  const T& operator[](const Index&) const;
  [[nodiscard]] T& At(const Index&);
  [[nodiscard]] const T& At(const Index&) const;
  [[nodiscard]] T& front();
  [[nodiscard]] T& back();
  [[nodiscard]] iterator begin();
  [[nodiscard]] iterator end();
  [[nodiscard]] reversed_iterator rbegin();
  [[nodiscard]] reversed_iterator rend();
  [[nodiscard]] T& front() const;
  [[nodiscard]] T& back() const;
  [[nodiscard]] iterator begin() const;
  [[nodiscard]] iterator end() const;
  [[nodiscard]] reversed_iterator rbegin() const;
  [[nodiscard]] reversed_iterator rend() const;
  [[nodiscard]] iterator cbegin() const;
  [[nodiscard]] iterator cend() const;
  void clear();
  [[nodiscard]] bool empty() const;
  void erase(iterator);
  void erase(const T&);
  void push_back(const T&);
  void push_back(T&&);
  void pop();
  void reserve(size_t);
  [[nodiscard]] size_t size() const;

 private:
  unique_ptr<T[]> start_;
  iterator end_;
  Index volume_;
  [[nodiscard]] bool check_volume() const;
  void check_index(const Index&) const;
  void extend_volume();
  Pointer getBegin() override { return lhy::make_shared<iterator>(begin()); }
  Pointer getEnd() override { return lhy::make_shared<iterator>(end()); }
};
std::string str(const vector<char>& obj);
// todo:研究这里到底应该右值引用还是直接值返回，编译器是否有优化
template <typename T>
vector<T> operator+(vector<T> a, const vector<T>& b);
template <typename T>
vector<T> operator&(vector<T> a, const vector<T>& b);
template <typename T>
vector<T> operator|(vector<T> a, const vector<T>& b);
template <typename T>
void getline(std::istream& istream_, vector<T>& obj);
template <typename T>
std::istream& operator>>(std::istream& istream_, vector<T>& obj);
template <typename T>
vector<T>::vector(vector&& other) {
  std::swap(start_, other.start_);
  std::swap(end_, other.end_);
  volume_ = other.volume_;
}
template <typename T>
vector<T>::vector(T const* other) : vector() {
  size_t N = std::string(other).size();
  for (int i = 0; i < N; ++i) {
    push_back(other[i]);
  }
}
template <typename T>
vector<T>::vector() : vector(0ll) {}
template <typename T>
vector<T>::vector(iterator start, iterator end) : vector() {
  for (auto element = start; element != end; ++element) {
    push_back(*element);
  }
}
template <typename T>
vector<T>::vector(const size_t size) {
  start_ = make_unique_for_overwrite<T[]>(size + 2);
  end_ = begin() + size;
  volume_ = size + 1;
}
template <typename T>
vector<T>::vector(const vector& other) : vector() {
  for (auto& each : other) {
    this->push_back(each);
  }
}
template <typename T>
vector<T>::vector(const std::initializer_list<T>& element_list) : vector() {
  for (auto& element : element_list) {
    push_back(element);
  }
}
template <typename T>
vector<T>& vector<T>::operator=(const vector& other) {
  if (this == &other) {
    return *this;
  }
  this->clear();
  for (auto element = other.begin(); element != other.end(); element++) {
    this->push_back(*element);
  }
  return *this;
}
template <typename T>
vector<T>& vector<T>::operator=(vector&& other) noexcept {
  /// 好像这么写确实有点问题，好像移动要保证出来是nullptr？
  std::swap(this->start_, other.start_);
  std::swap(this->end_, other.end_);
  std::swap(this->volume_, other.volume_);
  return *this;
}
template <typename T>
vector<T>& vector<T>::operator=(const std::initializer_list<T>& element_list) {
  clear();
  for (auto& element : element_list) {
    push_back(element);
  }
  return *this;
}
template <typename T>
bool vector<T>::operator==(const vector& other) const {
  if (size() != other.size()) {
    return false;
  }
  for (Index i = 0; i < size(); i++) {
    if (At(i) != other.At(i)) {
      return false;
    }
  }
  return true;
}
template <typename T>
vector<T>& vector<T>::operator+=(const vector& other) {
  for (auto& each : other) {
    this->push_back(each);
  }
  return *this;
}
template <typename T>
vector<T>& vector<T>::operator+=(const T& other) {
  this->push_back(other);
  return *this;
}
template <typename T>
vector<T>& vector<T>::operator&=(const vector& other) {
  std::set<T> set_temp;
  for (auto& each : other) {
    set_temp.insert(each);
  }
  vector temp;
  for (auto& each : *this) {
    if (set_temp.count(each)) {
      temp.push_back(each);
    }
  }
  *this = std::move(temp);
  return *this;
}
template <typename T>
vector<T>& vector<T>::operator|=(const vector& other) {
  std::set<T> set_temp;
  for (auto& each : *this) {
    set_temp.insert(each);
  }
  for (auto& each : other) {
    if (!set_temp.count(each)) {
      this->push_back(each);
    }
  }
  return *this;
}
template <typename T>
T& vector<T>::operator[](const Index& index) {
  return At(index);
}
template <typename T>
const T& vector<T>::operator[](const Index& index) const {
  return At(index);
}
template <typename T>
T& vector<T>::At(const Index& index) {
  check_index(index);
  return start_[index];
}
template <typename T>
const T& vector<T>::At(const Index& index) const {
  check_index(index);
  return start_[index];
}
template <typename T>
T& vector<T>::front() {
  if (empty()) {
    throw range_error("no element in vector");
  }
  return *begin();
}
template <typename T>
T& vector<T>::back() {
  if (empty()) {
    throw range_error("no element in vector");
  }
  return *rbegin();
}
template <typename T>
typename vector<T>::iterator vector<T>::begin() {
  return start_.get();
}
template <typename T>
typename vector<T>::iterator vector<T>::end() {
  return end_;
}
template <typename T>
typename vector<T>::reversed_iterator vector<T>::rbegin() {
  return end_ - 1;
}
template <typename T>
typename vector<T>::reversed_iterator vector<T>::rend() {
  return begin() - 1;
}
template <typename T>
T& vector<T>::front() const {
  if (empty()) {
    throw range_error("no element in vector");
  }
  return *begin();
}
template <typename T>
T& vector<T>::back() const {
  if (empty()) {
    throw range_error("no element in vector");
  }
  return *rbegin();
}
template <typename T>
typename vector<T>::iterator vector<T>::begin() const {
  return start_.get();
}
template <typename T>
typename vector<T>::iterator vector<T>::end() const {
  return end_;
}
template <typename T>
typename vector<T>::reversed_iterator vector<T>::rbegin() const {
  return end_ - 1;
}
template <typename T>
typename vector<T>::reversed_iterator vector<T>::rend() const {
  return begin() - 1;
}
template <typename T>
typename vector<T>::iterator vector<T>::cbegin() const {
  return start_.get();
}
template <typename T>
typename vector<T>::iterator vector<T>::cend() const {
  return end_;
}
// todo:这里的clear感觉有问题
template <typename T>
void vector<T>::clear() {
  start_ = make_unique_for_overwrite<T[]>(1);
  end_ = begin();
  volume_ = 1;
}
template <typename T>
bool vector<T>::empty() const {
  return size() == 0;
}
template <typename T>
void vector<T>::erase(iterator target) {
  while (target != end_ - 1) {
    target[0] = target[1];
    ++target;
  }
  --end_;
}
template <typename T>
void vector<T>::erase(const T& target) {
  for (auto it = begin(); it != end(); ++it) {
    if (*it == target) {
      erase(it);
      break;
    }
  }
}
template <typename T>
void vector<T>::push_back(const T& element) {
  if (check_volume()) {
    T element_temp = element;
    extend_volume();
    *end_ = element_temp;
  } else {
    *end_ = element;
  }
  ++end_;
}
template <typename T>
void vector<T>::push_back(T&& element) {
  if (check_volume()) {
    extend_volume();
  }
  *end_ = element;
  ++end_;
}
// todo:只针对char
template <typename T>
vector<T>::vector(T* other) : vector() {
  const size_t N = sizeof(other) / sizeof(T);
  for (int i = 0; i < N; ++i) {
    push_back(other[i]);
  }
}
template <typename T>
void vector<T>::pop() {
  if (size() == 0) {
    throw range_error("nothing left to pop");
  }
  --end_;
}
template <typename T>
void vector<T>::reserve(size_t size) {
  if (size < this->size()) {
    throw logic_error("the size of the vector cannot be bigger than the reserve size");
  }
  unique_ptr<T[]> start_substitute = make_unique_for_overwrite<T[]>(size);
  const size_t ex_size = this->size();
  end_ = start_substitute.get() + size;
  volume_ = size;
  // note:用不了std::move，因为是不同大小
  for (int i = 0; i < ex_size; ++i) {
    start_substitute[i] = std::move(start_[i]);
  }
  start_.swap(start_substitute);
}

template <typename T>
size_t vector<T>::size() const {
  return distance(begin(), end());
}
template <typename T>
bool vector<T>::check_volume() const {
  return size() == volume_;
}
template <typename T>
void vector<T>::check_index(const Index& index) const {
  if (index < 0 || index >= size()) {
    throw logic_error("out of range");
  }
}
template <typename T>
void vector<T>::extend_volume() {
  unique_ptr<T[]> start_substitute = make_unique_for_overwrite<T[]>(volume_ * 2);
  end_ = start_substitute.get() + volume_;
  for (int i = 0; i < volume_; ++i) {
    start_substitute[i] = std::move(start_[i]);
  }
  volume_ *= 2;
  start_.swap(start_substitute);
}
template <typename T>
vector<T> operator+(vector<T> a, const vector<T>& b) {
  return a += b;
}
template <typename T>
vector<T> operator&(vector<T> a, const vector<T>& b) {
  return a &= b;
}
template <typename T>
vector<T> operator|(vector<T> a, const vector<T>& b) {
  return a |= b;
}
template <typename T>
void getline(std::istream& istream_, vector<T>& obj) {
  throw logic_error("non-char-vector is not allowed to read through getline");
}
template <typename T>
std::istream& operator>>(std::istream& istream_, vector<T>& obj) {
  vector<char> temp;
  getline(istream_, temp);
  std::stringstream temp_ss(str(temp));
  T temp_each;
  while (temp_ss >> temp_each) {
    obj.push_back(std::move(temp_each));
  }
  return istream_;
}
}  // namespace lhy
#endif