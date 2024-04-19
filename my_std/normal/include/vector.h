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
  using iterator = lhy::NormIterator<T>;
  using reversed_iterator = lhy::ReversedNormIterator<T>;
  vector();
  explicit vector(iterator, iterator);
  explicit vector(size_t);
  vector(T* other);
  vector(const T* other);
  vector(const vector<T>&);
  vector(vector<T>&&);
  vector(const std::initializer_list<T>&);
  ~vector() override;
  vector<T>& operator=(const vector<T>&);
  vector<T>& operator=(vector<T>&&) noexcept;
  vector<T>& operator=(const std::initializer_list<T>&);
  bool operator==(const vector<T>& other) const;
  vector<T>& operator+=(const T&);
  vector<T>& operator+=(const vector<T>&);
  vector<T>& operator&=(const vector<T>&);
  vector<T>& operator|=(const vector<T>&);
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
  [[nodiscard]] const T& front() const;
  [[nodiscard]] const T& back() const;
  [[nodiscard]] const iterator begin() const;
  [[nodiscard]] const iterator end() const;
  [[nodiscard]] const reversed_iterator rbegin() const;
  [[nodiscard]] const reversed_iterator rend() const;
  [[nodiscard]] const iterator cbegin() const;
  [[nodiscard]] const iterator cend() const;
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
  iterator start_;
  iterator end_;
  Index volume_;
  [[nodiscard]] bool check_volume() const;
  void check_index(const Index&) const;
  void extend_volume();
  Pointer getBegin() override { return &start_; }
  Pointer getEnd() override { return &end_; }
};
std::string str(const vector<char>& obj);
// todo:研究这里到底应该右值引用还是直接值返回，编译器是否有优化
template <typename T>
vector<T> operator+(vector<T> a, const vector<T>& b);
template <typename T>
vector<T> operator&(vector<T> a, const vector<T>& b);
template <typename T>
vector<T> operator|(vector<T> a, const vector<T>& b);
template <char_type T>
void getline(std::istream& istream_, vector<T>& obj);
template <not_char_type T>
void getline(std::istream& istream_, vector<T>& obj);
template <char_type T>
std::istream& operator>>(std::istream& istream_, vector<T>& obj);
template <not_char_type T>
std::istream& operator>>(std::istream& istream_, vector<T>& obj);
template <char_type T>
std::ostream& operator<<(std::ostream& ostream_, vector<T>& obj);
template <not_char_type T>
std::ostream& operator<<(std::ostream& ostream_, vector<T>& obj);
template <typename T>
vector<T>::vector(vector<T>&& other) {
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
vector<T>::vector() : vector(0l) {}
template <typename T>
vector<T>::vector(iterator start, iterator end) : vector() {
  for (auto* element = start; element != end; ++element) {
    push_back(*element);
  }
}
template <typename T>
vector<T>::vector(const size_t size) {
  start_ = new T[size + 2];
  end_ = start_ + size;
  volume_ = size + 1;
}
template <typename T>
vector<T>::vector(const vector<T>& other) : vector() {
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
vector<T>::~vector() {
  delete[] start_;
}
template <typename T>
vector<T>& vector<T>::operator=(const vector<T>& other) {
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
vector<T>& vector<T>::operator=(vector<T>&& other) noexcept {
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
bool vector<T>::operator==(const vector<T>& other) const {
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
vector<T>& vector<T>::operator+=(const vector<T>& other) {
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
vector<T>& vector<T>::operator&=(const vector<T>& other) {
  std::set<T> set_temp;
  for (auto& each : other) {
    set_temp.insert(each);
  }
  vector<T> temp(*this);
  this->clear();
  for (auto& each : temp) {
    if (set_temp.count(each)) {
      this->push_back(each);
    }
  }
  return *this;
}
template <typename T>
vector<T>& vector<T>::operator|=(const vector<T>& other) {
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
    throw std::range_error("no element in vector");
  }
  return *start_;
}
template <typename T>
T& vector<T>::back() {
  if (empty()) {
    throw std::range_error("no element in vector");
  }
  return *(end_ - 1);
}
template <typename T>
vector<T>::iterator vector<T>::begin() {
  return start_;
}
template <typename T>
vector<T>::iterator vector<T>::end() {
  return end_;
}
template <typename T>
vector<T>::reversed_iterator vector<T>::rbegin() {
  return end_ - 1;
}
template <typename T>
vector<T>::reversed_iterator vector<T>::rend() {
  return start_ - 1;
}
template <typename T>
const T& vector<T>::front() const {
  if (empty()) {
    throw std::range_error("no element in vector");
  }
  return *start_;
}
template <typename T>
const T& vector<T>::back() const {
  if (empty()) {
    throw std::range_error("no element in vector");
  }
  return *(end_ - 1);
}
template <typename T>
const vector<T>::iterator vector<T>::begin() const {
  return start_;
}
template <typename T>
const vector<T>::iterator vector<T>::end() const {
  return end_;
}
template <typename T>
const vector<T>::reversed_iterator vector<T>::rbegin() const {
  return end_ - 1;
}
template <typename T>
const vector<T>::reversed_iterator vector<T>::rend() const {
  return start_ - 1;
}
template <typename T>
const vector<T>::iterator vector<T>::cbegin() const {
  return const_cast<const iterator>(end_);
}
template <typename T>
const vector<T>::iterator vector<T>::cend() const {
  return const_cast<const iterator>(start_);
}
// todo:这里的clear感觉有问题
template <typename T>
void vector<T>::clear() {
  delete[] start_;
  start_ = new T[1];
  end_ = start_;
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
    target++;
  }
  end_--;
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
  end_++;
}
template <typename T>
void vector<T>::push_back(T&& element) {
  if (check_volume()) {
    extend_volume();
  }
  *end_ = element;
  end_++;
}
// todo:只针对char
template <typename T>
vector<T>::vector(T* other) : vector() {
  size_t N = sizeof(other) / sizeof(T);
  for (int i = 0; i < N; ++i) {
    push_back(other[i]);
  }
}
template <typename T>
void vector<T>::pop() {
  if (size() == 0) {
    throw std::range_error("nothing left to pop");
  }
  end_--;
}
template <typename T>
void vector<T>::reserve(size_t size) {
  if (size < this->size()) {
    throw std::logic_error("the size of the vector cannot be bigger than the reserve size");
  }
  iterator start_substitute = new T[size];
  size_t ex_size = this->size();
  end_ = start_substitute + size;
  volume_ = size;
  // todo:std::move
  for (int i = 0; i < ex_size; ++i) {
    start_substitute[i] = start_[i];
  }
  delete[] start_;
  start_ = start_substitute;
}

template <typename T>
size_t vector<T>::size() const {
  return distance(start_, end_);
}
template <typename T>
bool vector<T>::check_volume() const {
  return size() == volume_;
}
template <typename T>
void vector<T>::check_index(const Index& index) const {
  if (index < 0 || index >= size()) {
    throw std::logic_error("out of range");
  }
}
template <typename T>
void vector<T>::extend_volume() {
  iterator start_substitute = new T[volume_ * 2];
  end_ = start_substitute + volume_;
  for (int i = 0; i < volume_; ++i) {
    start_substitute[i] = start_[i];
  }
  volume_ *= 2;
  delete[] start_;
  start_ = start_substitute;
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
template <char_type T>
void getline(std::istream& istream_, vector<T>& obj) {
  obj.clear();
  char temp_char;
  while ((temp_char = static_cast<char>(istream_.get())) == '\n')
    ;
  istream_.unget();
  while ((temp_char = static_cast<char>(istream_.get())) != EOF && temp_char != '\n') {
    obj.push_back(temp_char);
  }
}
template <not_char_type T>
void getline(std::istream& istream_, vector<T>& obj) {
  throw std::logic_error("non-char-vector is not allowed to read through getline");
}
template <char_type T>
std::istream& operator>>(std::istream& istream_, vector<T>& obj) {
  obj.clear();
  char temp_char;
  while ((temp_char = static_cast<char>(istream_.get())) == ' ' || temp_char == '\n')
    ;
  istream_.unget();
  while ((temp_char = static_cast<char>(istream_.get())) != EOF && temp_char != ' ' && temp_char != '\n') {
    obj.push_back(temp_char);
  };
  if (temp_char == ' ') {
    istream_.unget();
  }
  return istream_;
}
template <not_char_type T>
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
template <char_type T>
std::ostream& operator<<(std::ostream& ostream_, vector<T>& obj) {
  for (auto& each : obj) {
    ostream_ << each;
  }
  return ostream_;
}
template <not_char_type T>
std::ostream& operator<<(std::ostream& ostream_, vector<T>& obj) {
  for (auto& each : obj) {
    ostream_ << each << '\n';
  }
  return ostream_;
}
template <char_type T>
std::ostream& operator<<(std::ostream& ostream_, const vector<T>& obj) {
  for (auto& each : obj) {
    ostream_ << each;
  }
  return ostream_;
}
template <not_char_type T>
std::ostream& operator<<(std::ostream& ostream_, const vector<T>& obj) {
  for (auto& each : obj) {
    ostream_ << each << '\n';
  }
  return ostream_;
}
}  // namespace lhy
#endif