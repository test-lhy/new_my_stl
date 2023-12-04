//
// Created by lhy on 23-9-20.
//
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
  operator Pointer () { return data_;}
  operator const Pointer () const { return data_; }
  [[nodiscard]]Pointer& getPointer() { return data_; }
  [[nodiscard]]const Pointer& getPointer() const { return data_; }
  [[nodiscard]]virtual Iterator* next()=0;

 private:
  Pointer data_;
};
template<typename T>
class NormIterator:public Iterator<T>{
 public:
  using lhy::Iterator<T>::Iterator;
  virtual NormIterator& operator++() {
    this->getPointer()++;
    return *this;
  }
  virtual NormIterator& operator++(int) { return operator++(); }
  NormIterator& operator--() {
    this->getPointer()--;
    return *this;
  }
  NormIterator& operator--(int) { return operator--(); }
  Iterator<T>* next() override {
    operator++();
    return this;
  }
  NormIterator operator+(Index index){
    return this->getPointer()+index;
  }
  NormIterator operator-(Index index){
    return this->getPointer()-index;
  }
};
template<typename T>
class DataStructure {
 public:
  using Pointer = Iterator<T>*;
 private:
  std::vector<int> observers;
  virtual Pointer getBegin()=0;
  virtual Pointer getEnd()=0;
  void inform();
};
template <typename T>
void DataStructure<T>::inform() {
  for (auto& each_observer :observers) {
//    each_observer->update(*this);
  }
}
}  // namespace lhy
#endif  // MY_STL_DATA_STRUCTURE_H