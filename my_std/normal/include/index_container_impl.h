//
// Created by lhy on 24-4-12.
//

#ifndef INDEX_CONTAINER_IMPL_H
#define INDEX_CONTAINER_IMPL_H
#include "basic.h"
#include "data_structure.h"
enum IndexContainerMode { VectorIndexContainerMode, MapIndexContainerMode };
namespace lhy {
template <typename T>
class IndexContainerImpl : public DataStructure<std::pair<Index, T>> {
 public:
  using RealT = std::pair<Index, T>;
  template <typename T1>
  friend class IndexContainer;
  class iterator;
  class iteratorImpl;
  class reversed_iterator;
  class reversed_iteratorImpl;
  virtual T& operator[](Index index) = 0;
  virtual const T& operator[](Index index) const = 0;
  virtual void erase(Index index) = 0;
  virtual bool Exist(Index index) = 0;
  ~IndexContainerImpl() override = default;
  [[nodiscard]] virtual iterator begin() = 0;
  [[nodiscard]] virtual iterator end() = 0;
  [[nodiscard]] virtual reversed_iterator rbegin() = 0;
  [[nodiscard]] virtual reversed_iterator rend() = 0;
  [[nodiscard]] virtual IndexContainerMode GetMode() const = 0;
  // 草了，傻逼const，感觉如果要const可能要写一个const_iterator

 private:
  [[nodiscard]] typename DataStructure<RealT>::Pointer getBegin() override = 0;
  [[nodiscard]] typename DataStructure<RealT>::Pointer getEnd() override = 0;
};
// 这个结构可能应该要改一下,太怪了
template <typename T>
class IndexContainerImpl<T>::iteratorImpl : public TwoDirectionIterator<RealT>, public StoredIteratorType<RealT> {
 public:
  using TwoDirectionIterator<RealT>::TwoDirectionIterator;
  using typename Iterator<RealT>::Pointer;
  using typename Iterator<RealT>::OutPointer;
  Pointer extract() override { return this->getPointer(); }
};
template <typename T>
class IndexContainerImpl<T>::iterator : public TwoDirectionIterator<RealT, iteratorImpl>,
                                        public StoredIteratorType<RealT> {
 public:
  using TwoDirectionIterator<RealT, iteratorImpl>::TwoDirectionIterator;
  using typename Iterator<RealT, iteratorImpl>::Pointer;
  using typename Iterator<RealT, iteratorImpl>::OutPointer;
  // 这个构造函数不知道为什么会调用默认构造函数
  // iterator(iteratorImpl* _iterator) : TwoDirectionIterator<T, iteratorImpl>(_iterator) {}
  iterator(const Iterator<RealT, iteratorImpl>& other) : Iterator<RealT, iteratorImpl>(other) {}
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
};
template <typename T>
class IndexContainerImpl<T>::reversed_iteratorImpl : public ReversedTwoDirectionIterator<RealT>,
                                                     public StoredIteratorType<RealT> {
 public:
  using ReversedTwoDirectionIterator<RealT>::ReversedTwoDirectionIterator;
  using typename Iterator<RealT>::Pointer;
  using typename Iterator<RealT>::OutPointer;
  Pointer extract() override { return this->getPointer(); }
};
template <typename T>
class IndexContainerImpl<T>::reversed_iterator : public ReversedTwoDirectionIterator<RealT, reversed_iteratorImpl>,
                                                 public StoredIteratorType<RealT> {
 public:
  using ReversedTwoDirectionIterator<RealT, reversed_iteratorImpl>::ReversedTwoDirectionIterator;
  using typename Iterator<RealT, reversed_iteratorImpl>::Pointer;
  using typename Iterator<RealT, reversed_iteratorImpl>::OutPointer;
  reversed_iterator(const Iterator<RealT, reversed_iteratorImpl>& other)
      : Iterator<RealT, reversed_iteratorImpl>(other) {}
  friend bool operator==(const reversed_iterator& lhs, const reversed_iterator& rhs) {
    return lhs.getPointer()->getPointer() == rhs.getPointer()->getPointer();
  }
  friend bool operator!=(const reversed_iterator& lhs, const reversed_iterator& rhs) { return !(lhs == rhs); }
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
};
}  // namespace lhy

#endif  // INDEX_CONTAINER_IMPL_H
