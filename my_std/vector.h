//
// Created by lhy31 on 2023/8/21.
//

#ifndef MY_STL_VECTOR_H
#define MY_STL_VECTOR_H

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "basic.h"
namespace lhy {
template <typename T>
class vector {
 public:
  vector();
  explicit vector(const size_t&);
  explicit vector(T*,T*);
  vector(const std::initializer_list<T>&);
  ~vector();
  vector<T>& operator=(const std::initializer_list<T>&);
  vector<T>& operator=(const vector<T>&);
  vector<T>& operator=(vector<T>&&) noexcept;
  void push_back(const T&);
  T& operator[](const Index&);
  T& At(const Index&);
  void clear();
  size_t size() const;
  bool empty() const;
  T* begin() const;
  T* rbegin() const;
  T* rend() const;
  T* end() const;
  const T* cend() const;
  const T* cbegin() const;
  void pop();
  T& front() const;
  T& back() const;
  void reserve(size_t);
  void erase(Index);
  void erase(T*);
  void erase(const T&);

 private:
  T* start_;
  T* end_;
  Index volume_;
  void check_volume();
  void check_index(const Index&) const;
};
template <typename T>
vector<T>& vector<T>::operator=(vector<T>&& other)  noexcept {
  std::swap(this,&other);
  return *this;
}
template <typename T>
vector<T>& vector<T>::operator=(const vector<T>& other) {
  if (this==&other) {
    return *this;
  }
  this->clear();
  for (auto element=other.begin();element!=other.end();element++) {
    this->push_back(*element);
  }
  return *this;
}
template <typename T>
//todo:不知道为什么graph erase的时候不能const 也不能&
void vector<T>::erase(const T& target) {
  for (auto it = begin(); it!= end(); ++it) {
    if (*it == target) {
      erase(it);
      break;
    }
  }
}
template <typename T>
void vector<T>::erase(T* target) {
  while(target!= end_-1) {
    target[0] = target[1];
    target++;
  }
}

template <typename T>
void vector<T>::erase(Index index) {
  for (Index i = index; i < size() - 1; i++) {
    start_[i] = start_[i + 1];
  }
}
template <typename T>
void vector<T>::reserve(size_t size) {
  if (size<this->size()) {
    throw std::logic_error("the size of the vector cannot be bigger than the reserve size");
  }
  T* start_substitute = new T[size];
  size_t ex_size=this->size();
  end_ =  start_substitute+size;
  volume_ = size;
  for (int i = 0; i < ex_size; ++i) {
    start_substitute[i] = start_[i];
  }
  delete[] start_;
  start_ = start_substitute;
}

template <typename T>
vector<T>::vector(T* start, T* end):vector() {
  for (auto *element = start; element != end; ++element) {
    push_back(*element);
  }
}
template <typename T>
T* vector<T>::rend() const {
  return start_-1;
}
template <typename T>
T* vector<T>::rbegin() const {
  return end_-1;
}
template <typename T>
T& vector<T>::back() const {
  if (empty()) {
    throw std::range_error("no element in vector");
  }
  return *(end_ - 1);
}
template <typename T>
T& vector<T>::front() const {
  if (empty()) {
    throw std::range_error("no element in vector");
  }
  return *start_;
}
template <typename T>
void vector<T>::pop() {
  if (size() == 0) {
    throw std::range_error("nothing left to pop");
  }
  end_--;
}
template <typename T>
T& vector<T>::operator[](const Index& index) {
  return At(index);
}
template <typename T>
void vector<T>::check_index(const Index& index) const {
  if (index < 0 || index >= size()) {
    throw std::logic_error("out of range");
  }
}
template <typename T>
T& vector<T>::At(const Index& index) {
  check_index(index);
  return start_[index];
}

template <typename T>
vector<T>::vector(const std::initializer_list<T>& element_list) : vector() {
  for (auto& element : element_list) {
    push_back(element);
  }
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
vector<T>::vector(const size_t& size) {
  start_ = new T[size];
  end_ = start_;
  volume_ = size;
}
template <typename T>
const T* vector<T>::cbegin() const {
  return const_cast<const T*>(end_);
}
template <typename T>
const T* vector<T>::cend() const {
  return const_cast<const T*>(start_);
}
template <typename T>
T* vector<T>::end() const {
  return end_;
}
template <typename T>
T* vector<T>::begin() const {
  return start_;
}
template <typename T>
bool vector<T>::empty() const {
  return size() == 0;
}
template <typename T>
size_t vector<T>::size() const {
  return end_ - start_;
}
template <typename T>
void vector<T>::clear() {
  delete[] start_;
  start_ = new T[1];
  end_ = start_;
  volume_ = 1;
}
template <typename T>
void vector<T>::check_volume() {
  if (size() == volume_) {
    T* start_substitute = new T[volume_ * 2];
    end_ = start_substitute + volume_;
    for (int i = 0; i < volume_; ++i) {
      start_substitute[i] = start_[i];
    }
    volume_ *= 2;
    delete[] start_;
    start_ = start_substitute;
  }
}
template <typename T>
void vector<T>::push_back(const T& element) {
  check_volume();
  *end_ = element;
  end_++;
}
template <typename T>
vector<T>::vector() : vector(1) {}
template <typename T>
vector<T>::~vector() {
  delete[] start_;
}
}  // namespace lhy
#endif