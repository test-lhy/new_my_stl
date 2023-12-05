//
// Created by lhy on 23-9-20.
//
#include <stdexcept>

#include "basic.h"
#include "concept.h"
#ifndef MY_STL_DATA_STRUCTURE_H
#define MY_STL_DATA_STRUCTURE_H
namespace lhy {
template <iter T>
size_t distance(T st, T en) {
  return en - st;
}
template <typename T>
class Iterator {
 public:
  using Pointer = T*;
  Iterator() : data_(nullptr){};
  Iterator(Pointer data) : data_(data){};
  operator Pointer() { return data_; }
  operator const Pointer() const { return data_; }
  T* operator->() { return data_; }
  [[nodiscard]] Pointer& getPointer() { return data_; }
  [[nodiscard]] const Pointer& getPointer() const { return data_; }
  [[nodiscard]] virtual Iterator* next() = 0;

 private:
  Pointer data_;
};
template <typename T>
class ForwardIterator : virtual public Iterator<T> {
 public:
  using lhy::Iterator<T>::Iterator;
  virtual ForwardIterator& operator++() {
    this->getPointer()++;
    return *this;
  }
  virtual ForwardIterator& operator++(int) { return operator++(); }
  Iterator<T>* next() override {
    operator++();
    return this;
  }
};
template <typename T>
class BackwardIterator : virtual public Iterator<T> {
 public:
  using lhy::Iterator<T>::Iterator;
  virtual BackwardIterator& operator--() {
    this->getPointer()--;
    return *this;
  }
  virtual BackwardIterator& operator--(int) { return operator--(); }
  Iterator<T>* next() override {
    throw std::logic_error("BackwardIterator can't not use next function");
    return this;
  }
};
template <typename T>
class TwoDirectionIterator : public ForwardIterator<T>, public BackwardIterator<T> {
 public:
  using lhy::ForwardIterator<T>::ForwardIterator;
  using lhy::BackwardIterator<T>::BackwardIterator;
  virtual TwoDirectionIterator& operator++() {
    this->getPointer()++;
    return *this;
  }
  virtual TwoDirectionIterator& operator++(int) { return operator++(); }
  virtual TwoDirectionIterator& operator--() {
    this->getPointer()--;
    return *this;
  }
  virtual TwoDirectionIterator& operator--(int) { return operator--(); }
  Iterator<T>* next() override {
    operator++();
    return this;
  }
};
template <typename T>
class NormIterator : public TwoDirectionIterator<T> {
 public:
  using lhy::TwoDirectionIterator<T>::TwoDirectionIterator;
  NormIterator& operator++() override {
    this->getPointer()++;
    return *this;
  }
  NormIterator& operator++(int) override { return operator++(); }
  NormIterator& operator--() override {
    this->getPointer()--;
    return *this;
  }
  NormIterator& operator--(int) override { return operator--(); }
  NormIterator operator+(Index index) { return this->getPointer() + index; }
  NormIterator operator-(Index index) { return this->getPointer() - index; }
};
template <typename T, typename U = T>
class DataStructure {
 public:
  using Pointer = Iterator<U>*;

 private:
  std::vector<int> observers;
  virtual Pointer getBegin() = 0;
  virtual Pointer getEnd() = 0;
  void inform();
};
template <typename T, typename U>
void DataStructure<T, U>::inform() {
  for (auto& each_observer : observers) {
    //    each_observer->update(*this);
  }
}
}  // namespace lhy
#endif  // MY_STL_DATA_STRUCTURE_H