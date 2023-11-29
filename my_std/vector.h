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
#include <compare>

#include "basic.h"
#include "data_structure.h"
#include "concept.h"
namespace lhy {
template <typename T>
class vector : public DataStructure<T> {
 public:
  class Iterator : public lhy::Iterator<T> {
   public:
    lhy::Iterator<T>* next() override {
      ++(*this);
      return this;
    }
    Iterator& operator++() {
      this->getPointer()++;
      return *this;
    }

    Iterator& operator++(int) {
      this->getPointer()++;
      return *this;
    }
    Iterator& operator--() {
      this->getPointer()--;
      return *this;
    }
    Iterator& operator--(int) {
      this->getPointer()--;
      return *this;
    }
    void deletePointer() override { delete[] this->getPointer(); }
    using lhy::Iterator<T>::Iterator;
    bool operator==(const Iterator& other) const  { return other.getPointer() == this->getPointer(); };
    std::strong_ordering operator<=>(const Iterator& other) const  { return other.getPointer() <=> this->getPointer(); };
    const T& operator[](Index index)const{
      return *(this->getPointer()+index);
    }
    T& operator[](Index index){
      return *(this->getPointer()+index);
    }
    Iterator operator+(Index index){
      return Iterator(this->getPointer()+index);
    }

    Iterator operator-(Index index){
      return Iterator(this->getPointer()-index);
    }
  };

 private:
  [[nodiscard]] const lhy::Iterator<T>* getBegin() const override { return &start_; }
  [[nodiscard]] const lhy::Iterator<T>* getEnd() const override { return &end_; }
  [[nodiscard]] lhy::Iterator<T>* getBegin() override { return &start_; }
  [[nodiscard]] lhy::Iterator<T>* getEnd() override { return &end_; }

 public:
  vector();
  explicit vector(const size_t&);
  explicit vector(Iterator, Iterator);
  vector(const std::initializer_list<T>&);
  vector(const vector<T>&);
  ~vector();
  vector<T>& operator=(const std::initializer_list<T>&);
  vector<T>& operator=(const vector<T>&);
  vector<T>& operator=(vector<T>&&) noexcept;
  vector<T>& operator+=(const vector<T>&);
  vector<T>& operator|=(const vector<T>&);
  vector<T>& operator&=(const vector<T>&);
  void push_back(const T&);
  void push_back(T&&);
  T& operator[](const Index&);
  const T& operator[](const Index&) const;
  T& At(const Index&);
  [[nodiscard]] const T& At(const Index&) const;
  void clear();
  [[nodiscard]] size_t size() const;
  [[nodiscard]] bool empty() const;
  [[nodiscard]] Iterator begin();
  [[nodiscard]] Iterator rbegin();
  [[nodiscard]] Iterator rend();
  [[nodiscard]] Iterator end();
  [[nodiscard]] Iterator begin() const;
  [[nodiscard]] Iterator rbegin() const;
  [[nodiscard]] Iterator rend() const;
  [[nodiscard]] Iterator end() const;
  [[nodiscard]] const Iterator cend() const;
  [[nodiscard]] const Iterator cbegin() const;
  void pop();
  [[nodiscard]] const T& front() const;
  [[nodiscard]] T& front();
  [[nodiscard]] const T& back() const;
  [[nodiscard]] T& back();
  void reserve(size_t);
  void erase(Iterator);
  void erase(const T&);

 private:
  Iterator start_;
  Iterator end_;
  Index volume_;
  void extend_volume();
  [[nodiscard]] bool check_volume() const;
  void check_index(const Index&) const;
};
template <typename T>
bool vector<T>::check_volume() const {
  return size() == volume_;
}
template <typename T>
const T& vector<T>::At(const Index& index) const {
  check_index(index);
  return start_[index];
}
template <typename T>
const T& vector<T>::operator[](const Index& index) const {
  return At(index);
}
template <typename T>
vector<T>::vector(const vector<T>& other) : vector() {
  for (auto& each : other) {
    this->push_back(each);
  }
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
vector<T>& vector<T>::operator+=(const vector<T>& other) {
  for (auto& each : other) {
    this->push_back(each);
  }
  return *this;
}

template <typename T>
void vector<T>::push_back(T&& element) {
  if (check_volume()) {
    extend_volume();
  }
  *end_ = element;
  end_++;
}
template <typename T>
vector<T>& vector<T>::operator=(vector<T>&& other) noexcept {
  std::swap(this, &other);
  return *this;
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
// todo:不知道为什么graph erase的时候不能const 也不能&
void vector<T>::erase(const T& target) {
  for (auto it = begin(); it != end(); ++it) {
    if (*it == target) {
      erase(it);
      break;
    }
  }
}
template <typename T>
void vector<T>::erase(Iterator target) {
  while (target != end_ - 1) {
    target[0] = target[1];
    target++;
  }
  end_--;
}
template <typename T>
void vector<T>::reserve(size_t size) {
  if (size < this->size()) {
    throw std::logic_error("the size of the vector cannot be bigger than the reserve size");
  }
  Iterator start_substitute = new T[size];
  size_t ex_size = this->size();
  end_ = start_substitute + size;
  volume_ = size;
  for (int i = 0; i < ex_size; ++i) {
    start_substitute[i] = start_[i];
  }
  start_.deletePointer();
  start_ = start_substitute;
}

template <typename T>
vector<T>::vector(Iterator start, Iterator end) : vector() {
  for (auto* element = start; element != end; ++element) {
    push_back(*element);
  }
}
template <typename T>
T& vector<T>::back()  {
  if (empty()) {
    throw std::range_error("no element in vector");
  }
  return *(end_ - 1);
}
template <typename T>
const T& vector<T>::back() const {
  if (empty()) {
    throw std::range_error("no element in vector");
  }
  return *(end_ - 1);
}
template <typename T>
const T& vector<T>::front() const {
  if (empty()) {
    throw std::range_error("no element in vector");
  }
  return *start_;
}
template <typename T>
T& vector<T>::front() {
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
  start_ = new T[size + 2];
  end_ = start_ + size;
  volume_ = size + 1;
}
template <typename T>
const vector<T>::Iterator vector<T>::cbegin() const {
  return end_;
}
template <typename T>
const vector<T>::Iterator vector<T>::cend() const {
  return start_;
}

template <typename T>
vector<T>::Iterator vector<T>::rend() {
  return start_ - 0;
}
template <typename T>
vector<T>::Iterator vector<T>::rbegin() {
  return end_ - 0;
}
template <typename T>
vector<T>::Iterator vector<T>::end() {
  return end_;
}
template <typename T>
vector<T>::Iterator vector<T>::begin() {
  return start_;
}
template <typename T>
vector<T>::Iterator vector<T>::rend() const{
  return start_ - 0;
}
template <typename T>
vector<T>::Iterator vector<T>::rbegin() const {
  return end_ - 0;
}
template <typename T>
vector<T>::Iterator vector<T>::end() const {
    return end_;
}
template <typename T>
vector<T>::Iterator vector<T>::begin() const {
  return start_;
}
template <typename T>
bool vector<T>::empty() const {
  return size() == 0;
}
template <typename T>
size_t vector<T>::size() const {
  return distance((const T*)start_, (const T*)end_);
}
// todo:这里的clear感觉有问题
template <typename T>
void vector<T>::clear() {
  start_.deletePointer();
  start_ = new T[1];
  end_ = start_;
  volume_ = 1;
}
template <typename T>
void vector<T>::extend_volume() {
  Iterator start_substitute = new T[volume_ * 2];
  end_ = start_substitute + volume_;
  for (int i = 0; i < volume_; ++i) {
    start_substitute[i] = start_[i];
  }
  volume_ *= 2;
  start_.deletePointer();
  start_ = start_substitute;
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
vector<T>::vector() : vector(0) {}
template <typename T>
vector<T>::~vector() {
  start_.deletePointer();
}
template <typename T>
vector<T>&& operator+(vector<T> a, const vector<T>& b) {
  return a += b;
}
template <typename T>
vector<T>&& operator&(vector<T> a, const vector<T>& b) {
  return a &= b;
}
template <typename T>
vector<T>&& operator|(vector<T> a, const vector<T>& b) {
  return a |= b;
}
template<char_type T>
void getline(std::istream& istream_,vector<T>& obj){
  obj.clear();
  char temp_char;
  while((temp_char=static_cast<char>(istream_.get()))=='\n');
  istream_.unget();
  while((temp_char=static_cast<char>(istream_.get()))!=EOF&&temp_char!='\n'){
    obj.push_back(temp_char);
  }
}
template<char_type T>
std::istream& operator>>(std::istream& istream_,vector<T>& obj){
  obj.clear();
  char temp_char;
  while((temp_char=static_cast<char>(istream_.get()))==' '||temp_char=='\n');
  istream_.unget();
  while((temp_char=static_cast<char>(istream_.get()))!=EOF&&temp_char!=' '&&temp_char!='\n'){
    obj.push_back(temp_char);
  };
  if (temp_char==' '){
    istream_.unget();
  }
  return istream_;
}
template<char_type T>
std::ostream& operator<<(std::ostream& ostream_,vector<T>& obj){
  for(auto& each:obj){
    ostream_<<each;
  }
  return ostream_;
}
template <not_char_type T>
void getline(std::istream& istream_, vector<T>& obj) {
  throw std::logic_error("non-char-vector is not allowed to read through getline");
}
std::string str(const vector<char>& obj);
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
template <not_char_type T>
std::ostream& operator<<(std::ostream& ostream_, vector<T>& obj) {
  for (auto& each : obj) {
    ostream_ << each << '\n';
  }
  return ostream_;
}
}  // namespace lhy
#endif