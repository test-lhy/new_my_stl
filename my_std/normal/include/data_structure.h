//
// Created by lhy on 23-9-20.
//
#include <stdexcept>

#include "basic.h"
#ifndef MY_STL_DATA_STRUCTURE_H
#define MY_STL_DATA_STRUCTURE_H
namespace lhy {
template <typename T>
concept iter = requires(T x) {
  *x;
  x++;
  x--;
};
template <typename T, typename U>
concept T_iter = iter<T> and (
                                 requires(T x) {
                                   { *x } -> std::convertible_to<U>;
                                 } or
                                 requires(T x) {
                                   { *x } -> std::convertible_to<U&>;
                                 });
template <iter T>
size_t distance(T st, T en) {
  return en - st;
}
template <typename T>
class Iterator {
 public:
  using Pointer = T*;
  Iterator() : data_(nullptr){};
  Iterator(Pointer data) : data_(data) {}
  Iterator<T>& operator=(const Iterator<T>& other) {
    this->data_ = other.data_;
    return *this;
  }
  bool operator==(const Iterator& other) const { return other.data_ == data_; }
  bool operator!=(const Iterator& other) const { return other.data_ != data_; }
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
class ReversedForwardIterator;
template <typename T>
class ForwardIterator : virtual public Iterator<T> {
 public:
  using lhy::Iterator<T>::Iterator;
  ForwardIterator(const Iterator<T>& other) : Iterator<T>(other){};
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
class ReversedForwardIterator : public ForwardIterator<T> {
 public:
  using ForwardIterator<T>::ForwardIterator;
  ReversedForwardIterator(const Iterator<T>& other) : Iterator<T>(other){};
  ReversedForwardIterator& operator++() override {
    this->getPointer()--;
    return *this;
  }
  ReversedForwardIterator& operator++(int) override { return operator++(); }
};
template <typename T>
class BackwardIterator : virtual public Iterator<T> {
 public:
  using lhy::Iterator<T>::Iterator;
  BackwardIterator(const Iterator<T>& other) : Iterator<T>(other){};
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
class ReversedBackwardIterator : public BackwardIterator<T> {
 public:
  using BackwardIterator<T>::BackwardIterator;
  ReversedBackwardIterator(const Iterator<T>& other) : Iterator<T>(other){};
  ReversedBackwardIterator& operator--() override {
    this->getPointer()++;
    return *this;
  }
  ReversedBackwardIterator& operator--(int) override { return operator--(); }
};
template <typename T>
class TwoDirectionIterator : public ForwardIterator<T>, public BackwardIterator<T> {
 public:
  using lhy::ForwardIterator<T>::ForwardIterator;
  using lhy::BackwardIterator<T>::BackwardIterator;
  TwoDirectionIterator(const Iterator<T>& other) : Iterator<T>(other){};
  TwoDirectionIterator& operator++() override {
    this->getPointer()++;
    return *this;
  }
  TwoDirectionIterator& operator++(int) override { return operator++(); }
  TwoDirectionIterator& operator--() override {
    this->getPointer()--;
    return *this;
  }
  TwoDirectionIterator& operator--(int) override { return operator--(); }
  Iterator<T>* next() override {
    operator++();
    return this;
  }
};
template <typename T>
class ReversedTwoDirectionIterator : public ReversedForwardIterator<T>, public ReversedBackwardIterator<T> {
 public:
  using TwoDirectionIterator<T>::TwoDirectionIterator;
  ReversedTwoDirectionIterator(const Iterator<T>& other) : Iterator<T>(other){};
  ReversedTwoDirectionIterator& operator++() override {
    this->getPointer()--;
    return *this;
  }
  ReversedTwoDirectionIterator& operator++(int) override { return operator++(); }
  ReversedTwoDirectionIterator& operator--() override {
    this->getPointer()--;
    return *this;
  }
  ReversedTwoDirectionIterator& operator--(int) override { return operator--(); }
};
template <typename T>
class NormIterator : public TwoDirectionIterator<T> {
 public:
  using lhy::TwoDirectionIterator<T>::TwoDirectionIterator;
  NormIterator(const Iterator<T>& other) : Iterator<T>(other){};
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
template <typename T>
class ReversedNormIterator : public NormIterator<T> {
 public:
  using NormIterator<T>::NormIterator;
  ReversedNormIterator(const Iterator<T>& other) : Iterator<T>(other){};
  ReversedNormIterator& operator++() override {
    this->getPointer()--;
    return *this;
  }
  ReversedNormIterator& operator++(int) override { return operator++(); }
  ReversedNormIterator& operator--() override {
    this->getPointer()++;
    return *this;
  }
  ReversedNormIterator& operator--(int) override { return operator--(); }
  ReversedNormIterator operator+(Index index) { return this->getPointer() - index; }
  ReversedNormIterator operator-(Index index) { return this->getPointer() + index; }
};
template <typename T, typename U = T>
class DataStructure {
 public:
  using Pointer = Iterator<U>*;

 private:
  std::vector<int> observers;
  [[nodiscard]] virtual Pointer getBegin() = 0;
  [[nodiscard]] virtual Pointer getEnd() = 0;
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