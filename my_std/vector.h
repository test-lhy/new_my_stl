//
// Created by lhy31 on 2023/8/21.
//

#ifndef MY_STL_VECTOR_H
#define MY_STL_VECTOR_H

#include <algorithm>
#include <cstdint>
#include <stdexcept>
namespace lhy {
template <typename T>
class vector {
 public:
  vector();
  explicit vector(const size_t&);
  vector(const std::initializer_list<T>&);
  ~vector();
  vector<T>& operator=(const std::initializer_list<T>&);
  void push_back(const T&);
  T& operator[](const size_t&);
  T& At(const size_t&);
  void clear();
  size_t size() const;
  bool empty() const;
  T* begin() const;
  T* end() const;
  const T* cend() const;
  const T* cbegin() const;
  void pop();
  T& front() const;
  T& back() const;

 private:
  using size_T = int64_t;
  T* start_;
  T* end_;
  T* volume_;
  void check_volume();
  void check_index(const size_t&) const;
};
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
T& vector<T>::operator[](const size_t& index) {
  return At(index);
}
template <typename T>
void vector<T>::check_index(const size_t& index) const {
  if (index < 0 || index >= size()) {
    throw std::logic_error("out of range");
  }
}
template <typename T>
T& vector<T>::At(const size_t& index) {
  check_index(index);
  return *(start_ + index);
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
  volume_ = start_ + size;
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
  volume_ = start_ + 1;
}
template <typename T>
void vector<T>::check_volume() {
  if (end_ == volume_) {
    int64_t size = (volume_ - start_);
    T* start_substitute = new T[size * 2];
    end_ = start_substitute + size;
    volume_ = start_substitute + size * 2;
    for (int i = 0; i < size; ++i) {
      *(start_substitute + i) = std::move(*(start_ + i));
    }
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
}  // namespace lhy
namespace lhy {
template <typename T>
vector<T>::vector() : vector(1) {}
template <typename T>
vector<T>::~vector() {
  delete[] start_;
}

}  // namespace lhy
#endif  // MY_STL_VECTOR_H