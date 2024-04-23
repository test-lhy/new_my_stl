//
// Created by lhy on 24-4-12.
//

#ifndef INDEX_CONTAINER_IMPL_H
#define INDEX_CONTAINER_IMPL_H
#include "basic.h"
#include "data_structure.h"
namespace lhy {
template <typename T>
class IndexContainerImpl : public DataStructure<T> {
 public:
  template <typename T1>
  friend class IndexContainer;
  class iterator;
  class iteratorImpl;
  class reversed_iterator;
  class reversed_iteratorImpl;
  virtual T& operator[](Index index) = 0;
  virtual const T& operator[](Index index) const = 0;
  virtual void erase(Index index) = 0;
  ~IndexContainerImpl() override = default;
  [[nodiscard]] virtual iterator begin() = 0;
  [[nodiscard]] virtual iterator end() = 0;
  [[nodiscard]] virtual reversed_iterator rbegin() = 0;
  [[nodiscard]] virtual reversed_iterator rend() = 0;
  // 草了，傻逼const，感觉如果要const可能要写一个const_iterator

 private:
  [[nodiscard]] typename DataStructure<T>::Pointer getBegin() override = 0;
  [[nodiscard]] typename DataStructure<T>::Pointer getEnd() override = 0;
};
template <typename T>
class IndexContainerImpl<T>::iteratorImpl : public TwoDirectionIterator<T>, public StoredIteratorType<T> {
 public:
  using TwoDirectionIterator<T>::TwoDirectionIterator;
  using typename Iterator<T>::Pointer;
  using typename Iterator<T>::OutPointer;
  T* extract() override { return this->getPointer(); }
};
template <typename T>
class IndexContainerImpl<T>::iterator : public TwoDirectionIterator<T, iteratorImpl>, public StoredIteratorType<T> {
 public:
  using TwoDirectionIterator<T, iteratorImpl>::TwoDirectionIterator;
  using typename Iterator<T, iteratorImpl>::Pointer;
  using typename Iterator<T, iteratorImpl>::OutPointer;
  // 这个构造函数不知道为什么会调用默认构造函数
  // iterator(iteratorImpl* _iterator) : TwoDirectionIterator<T, iteratorImpl>(_iterator) {}
  iterator(const Iterator<T, iteratorImpl>& other) : Iterator<T, iteratorImpl>(other) {}
  friend bool operator==(const iterator& lhs, const iterator& rhs) {
    return lhs.getPointer()->getPointer() == rhs.getPointer()->getPointer();
  }
  friend bool operator!=(const iterator& lhs, const iterator& rhs) { return !(lhs == rhs); }
  OutPointer extract() override { return this->getPointer()->getPointer(); }
  iterator& operator++() override {
    this->getPointer()->operator++();
    return *this;
  }
  iterator& operator++(int) override { return operator++(); }
  iterator& operator--() override {
    this->getPointer()->operator--();
    return *this;
  }
  iterator& operator--(int) override { return operator--(); }
  ~iterator() override { delete this->getPointer(); }
};
template <typename T>
class IndexContainerImpl<T>::reversed_iteratorImpl : public ReversedTwoDirectionIterator<T>,
                                                     public StoredIteratorType<T> {
 public:
  using ReversedTwoDirectionIterator<T>::ReversedTwoDirectionIterator;
  using typename Iterator<T>::Pointer;
  using typename Iterator<T>::OutPointer;
  T* extract() override { return this->getPointer(); }
};
template <typename T>
class IndexContainerImpl<T>::reversed_iterator : public ReversedTwoDirectionIterator<T, reversed_iteratorImpl>,
                                                 public StoredIteratorType<T> {
 public:
  using ReversedTwoDirectionIterator<T, reversed_iteratorImpl>::ReversedTwoDirectionIterator;
  using typename Iterator<T, reversed_iteratorImpl>::Pointer;
  using typename Iterator<T, reversed_iteratorImpl>::OutPointer;
  reversed_iterator(reversed_iteratorImpl* _iterator)
      : ReversedTwoDirectionIterator<T, reversed_iteratorImpl>(_iterator) {}
  reversed_iterator(const Iterator<T, reversed_iteratorImpl>& other) : Iterator<T, reversed_iteratorImpl>(other) {}
  OutPointer extract() override { return this->getPointer()->getPointer(); }
  reversed_iterator& operator++() override {
    this->getPointer()->operator++();
    return *this;
  }
  reversed_iterator& operator++(int) override { return operator++(); };
  reversed_iterator& operator--() override {
    this->getPointer()->operator--();
    return *this;
  }
  reversed_iterator& operator--(int) override { return operator--(); }
  ~reversed_iterator() override { delete this->getPointer(); }
};
}  // namespace lhy

#endif  // INDEX_CONTAINER_IMPL_H
