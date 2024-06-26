//
// Created by lhy on 23-9-20.
//
#include <iostream>
#include <memory>
#include <stdexcept>
#include <variant>

#include "basic.h"
#include "shared_ptr.h"
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
template <typename T>
class TIterator {
 public:
  using OutPointer = T*;
  virtual T& operator*() = 0;
  virtual const T& operator*() const = 0;
  virtual OutPointer operator->() = 0;
  virtual ~TIterator() = default;
};
template <typename T>
class TForwardIterator : public TIterator<T> {
 public:
  using TIterator<T>::OutPointer;
  virtual TForwardIterator& operator++() = 0;
  virtual TForwardIterator& operator++(int) { return operator++(); }
  [[nodiscard]] virtual TForwardIterator* next() {
    this->operator++();
    return this;
  }
};
// note:*出来是T类型,->使用的时候应该也是T*，但内部存储的是U类型
template <typename T, typename U = T>
class Iterator : public TIterator<T> {
 public:
  using Pointer = U*;
  using SharedPointer = shared_ptr<U>;
  using OutPointer = typename TIterator<T>::OutPointer;
  using RealPointer = std::variant<Pointer, SharedPointer>;
  Iterator() : data_(nullptr){};
  Iterator(Pointer data) : data_(data) {}
  Iterator(SharedPointer data) : data_(data) {}
  Iterator(const Iterator& other) : data_(other.data_) {}
  Iterator& operator=(const Iterator& other) {
    this->data_ = other.data_;
    return *this;
  }
  bool operator==(const Iterator& other) const { return other.data_ == data_; }  // 要不要改成extract?
  bool operator!=(const Iterator& other) const { return other.data_ != data_; }
  virtual operator Pointer() { return getPointer(); }
  virtual operator Pointer() const { return getPointer(); }
  T& operator*() override { return *Extract(); }
  const T& operator*() const override { return *Extract(); }
  OutPointer operator->() override { return Extract(); }
  template <typename T1 = T, typename U1 = U>
  OutPointer Extract() {
    return getPointer();
  }
  template <typename T1 = T, notsame<T> U1 = U>
  OutPointer Extract() {
    return getPointer()->extract();
  }
  template <typename T1 = T, typename U1 = U>
  OutPointer Extract() const {
    return getPointer();
  }
  template <typename T1 = T, notsame<T> U1 = U>
  OutPointer Extract() const {
    return getPointer()->extract();
  }
  [[nodiscard]] Pointer& getPointer() {
    return std::visit(
        []<typename T0>(T0& arg) -> Pointer& {
          using ArgType = std::decay_t<T0>;
          if constexpr (std::is_same_v<ArgType, Pointer>) {
            return arg;
          } else {
            return arg.getRef();
          }
        },
        data_);
  }

  [[nodiscard]] Pointer getPointer() const {
    return std::visit(
        []<typename T0>(const T0& arg) -> Pointer {
          using ArgType = std::decay_t<T0>;
          if constexpr (std::is_same_v<ArgType, Pointer>) {
            return arg;
          } else {
            return arg.get();
          }
        },
        data_);
  }

 private:
  RealPointer data_;
};
template <typename T, typename U = T>
class ForwardIterator : virtual public Iterator<T, U>, virtual public TForwardIterator<T> {
 public:
  using Iterator<T, U>::Iterator;
  using typename Iterator<T, U>::Pointer;
  using typename Iterator<T, U>::OutPointer;
  ForwardIterator(const Iterator<T, U>& other) : Iterator<T, U>(other){};
  T& operator*() override { return Iterator<T, U>::operator*(); }
  const T& operator*() const override { return Iterator<T, U>::operator*(); }
  TIterator<T>::OutPointer operator->() override { return Iterator<T, U>::operator->(); }
  virtual ForwardIterator& operator++() {
    this->getPointer()++;
    return *this;
  }
  virtual ForwardIterator& operator++(int) { return operator++(); }
};

template <typename T, typename U = T>
class ReversedForwardIterator : public ForwardIterator<T, U> {
 public:
  using typename Iterator<T, U>::Pointer;
  using typename Iterator<T, U>::OutPointer;
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
  using typename Iterator<T, U>::Pointer;
  using typename Iterator<T, U>::OutPointer;
  using lhy::Iterator<T, U>::Iterator;
  BackwardIterator(const Iterator<T, U>& other) : Iterator<T, U>(other){};
  virtual BackwardIterator& operator--() {
    this->getPointer()--;
    return *this;
  }
  virtual BackwardIterator& operator--(int) { return operator--(); }
};
template <typename T, typename U = T>
class ReversedBackwardIterator : public BackwardIterator<T, U> {
 public:
  using typename Iterator<T, U>::Pointer;
  using typename Iterator<T, U>::OutPointer;
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
  using typename Iterator<T, U>::Pointer;
  using typename Iterator<T, U>::OutPointer;
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
};
template <typename T, typename U = T>
class ReversedTwoDirectionIterator : public TwoDirectionIterator<T, U> {
 public:
  using typename Iterator<T, U>::Pointer;
  using typename Iterator<T, U>::OutPointer;
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
  using typename Iterator<T, U>::Pointer;
  using typename Iterator<T, U>::OutPointer;
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
  using typename Iterator<T, U>::Pointer;
  using typename Iterator<T, U>::OutPointer;
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
// note:shared_ptr持有一个Iterator，但Iterator并不持有shared_ptr
template <typename T>
class DataStructure {
 public:
  using Pointer = shared_ptr<TForwardIterator<T>>;
  virtual ~DataStructure() = default;

 private:
  std::vector<int> observers;
  [[nodiscard]] virtual Pointer getBegin() = 0;
  [[nodiscard]] virtual Pointer getEnd() = 0;
  void inform();
};
template <typename T>
void DataStructure<T>::inform() {
  for (auto& each_observer : observers) {
    //    each_observer->update(*this);
  }
}
}  // namespace lhy
#endif  // MY_STL_DATA_STRUCTURE_H