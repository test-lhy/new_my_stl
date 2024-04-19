//
// Created by lhy on 23-9-20.
//
#include <stdexcept>

#include "basic.h"
#include "type_traits.h"
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
template <typename T, typename U>
concept notsame = not std::is_same_v<T, U>;
template <typename T>
class StoredIteratorType {
 public:
  StoredIteratorType() = default;
  virtual T* extract() = 0;
  virtual ~StoredIteratorType() = default;
};
// note:*出来是T类型,->使用的时候应该也是T*，但内部存储的是U类型
template <typename T, typename U = T>
class Iterator {
 public:
  using Pointer = U*;
  using OutPointer = T*;
  Iterator() : data_(nullptr){};
  Iterator(Pointer data) : data_(data) {}
  Iterator<T, U>& operator=(const Iterator<T, U>& other) {
    this->data_ = other.data_;
    return *this;
  }
  bool operator==(const Iterator& other) const { return other.data_ == data_; }
  bool operator!=(const Iterator& other) const { return other.data_ != data_; }
  virtual operator Pointer() { return data_; }
  virtual operator Pointer() const { return data_; }
  virtual T& operator*() { return *Extract(); }
  virtual const T& operator*() const { return *Extract(); }
  virtual OutPointer operator->() { return Extract(); }
  template <typename T1 = T, typename U1 = U>
  OutPointer Extract() {
    return data_;
  }
  template <typename T1 = T, notsame<T> U1 = U>
  OutPointer Extract() {
    return data_->extract();
  }
  template <typename T1 = T, typename U1 = U>
  OutPointer Extract() const {
    return data_;
  }
  template <typename T1 = T, notsame<T> U1 = U>
  OutPointer Extract() const {
    return data_->extract();
  }
  [[nodiscard]] Pointer& getPointer() { return data_; }
  [[nodiscard]] const Pointer& getPointer() const { return data_; }
  [[nodiscard]] virtual Iterator* next() = 0;

 private:
  Pointer data_;
};
template <typename T, typename U = T>
class ForwardIterator : virtual public Iterator<T, U> {
 public:
  using lhy::Iterator<T, U>::Iterator;
  using Iterator<T, U>::Pointer;
  using Iterator<T, U>::OutPointer;
  ForwardIterator(const Iterator<T, U>& other) : Iterator<T, U>(other){};
  virtual ForwardIterator& operator++() {
    this->getPointer()++;
    return *this;
  }
  virtual ForwardIterator& operator++(int) { return operator++(); }
  Iterator<T, U>* next() override {
    operator++();
    return this;
  }
};
template <typename T, typename U = T>
class ReversedForwardIterator : public ForwardIterator<T, U> {
 public:
  using Iterator<T, U>::Pointer;
  using Iterator<T, U>::OutPointer;
  using ForwardIterator<T, U>::ForwardIterator;
  ReversedForwardIterator(const Iterator<T, U>& other) : Iterator<T, U>(other){};
  ReversedForwardIterator& operator++() override {
    this->getPointer()--;
    return *this;
  }
  ReversedForwardIterator& operator++(int) override { return operator++(); }
};
template <typename T, typename U = T>
class BackwardIterator : virtual public Iterator<T, U> {
 public:
  using Iterator<T, U>::Pointer;
  using Iterator<T, U>::OutPointer;
  using lhy::Iterator<T, U>::Iterator;
  BackwardIterator(const Iterator<T, U>& other) : Iterator<T, U>(other){};
  virtual BackwardIterator& operator--() {
    this->getPointer()--;
    return *this;
  }
  virtual BackwardIterator& operator--(int) { return operator--(); }
  Iterator<T, U>* next() override { throw std::logic_error("BackwardIterator can't not use next function"); }
};
template <typename T, typename U = T>
class ReversedBackwardIterator : public BackwardIterator<T, U> {
 public:
  using Iterator<T, U>::Pointer;
  using Iterator<T, U>::OutPointer;
  using BackwardIterator<T, U>::BackwardIterator;
  ReversedBackwardIterator(const Iterator<T, U>& other) : Iterator<T, U>(other){};
  ReversedBackwardIterator& operator--() override {
    this->getPointer()++;
    return *this;
  }
  ReversedBackwardIterator& operator--(int) override { return operator--(); }
};
template <typename T, typename U = T>
class TwoDirectionIterator : public ForwardIterator<T, U>, public BackwardIterator<T, U> {
 public:
  using Iterator<T, U>::Pointer;
  using Iterator<T, U>::OutPointer;
  using lhy::ForwardIterator<T, U>::ForwardIterator;
  using lhy::BackwardIterator<T, U>::BackwardIterator;
  TwoDirectionIterator(const Iterator<T, U>& other) : Iterator<T, U>(other){};
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
  Iterator<T, U>* next() override {
    operator++();
    return this;
  }
};
template <typename T, typename U = T>
class ReversedTwoDirectionIterator : TwoDirectionIterator<T, U> {
 public:
  using Iterator<T, U>::Pointer;
  using Iterator<T, U>::OutPointer;
  using TwoDirectionIterator<T, U>::TwoDirectionIterator;
  ReversedTwoDirectionIterator(const Iterator<T, U>& other) : Iterator<T, U>(other){};
  ReversedTwoDirectionIterator& operator++() override {
    this->getPointer()--;
    return *this;
  }
  ReversedTwoDirectionIterator& operator++(int) override { return operator++(); }
  ReversedTwoDirectionIterator& operator--() override {
    this->getPointer()++;
    return *this;
  }
  ReversedTwoDirectionIterator& operator--(int) override { return operator--(); }
};
template <typename T, typename U = T>
class NormIterator : public TwoDirectionIterator<T, U> {
 public:
  using Iterator<T, U>::Pointer;
  using Iterator<T, U>::OutPointer;
  using lhy::TwoDirectionIterator<T, U>::TwoDirectionIterator;
  NormIterator(const Iterator<T, U>& other) : Iterator<T, U>(other){};
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
  NormIterator operator+(Index index) { return this->getPointer() + index; }  // 真的不const吗
  NormIterator operator-(Index index) { return this->getPointer() - index; }
};
template <typename T, typename U = T>
class ReversedNormIterator : public NormIterator<T, U> {
 public:
  using Iterator<T, U>::Pointer;
  using Iterator<T, U>::OutPointer;
  using NormIterator<T, U>::NormIterator;
  ReversedNormIterator(const Iterator<T, U>& other) : Iterator<T, U>(other){};
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
  using Pointer = Iterator<T, U>*;
  virtual ~DataStructure() = default;

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