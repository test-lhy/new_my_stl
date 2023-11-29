//
// Created by lhy on 23-11-28.
//
#include <compare>
#include<vector>
#ifndef MY_STL_DS_H
#define MY_STL_DS_H
template<typename T>
class Iterator{
 public:
  virtual Iterator* next() const;
  Iterator():data_(nullptr){};
  Iterator(T* data):data_(data){};
  operator T*(){return data_;};
  T& operator*(){return *data_;};
  virtual void deletePointer()=0;
  T* getPointer(){
    return data_;
  }
 private:
  T* data_;
};
template<typename T>
class IteratorDeprived:public Iterator<T>{
 public:
  Iterator<T>* next() const override { ++(*this);return this;}
  IteratorDeprived& operator++(){};
  IteratorDeprived& operator++(int){};
  IteratorDeprived& operator--(){};
  IteratorDeprived& operator--(int){};
  using Iterator<T>::Iterator;
  std::strong_ordering operator<=>(const IteratorDeprived& other){};
};

template<typename T>
class DataStructure{
  std::vector<Observer*> observers;
  Iterator<T>* getBegin()const=0;
  Iterator<T>* getEnd()const=0;
  void inform(){
    Iterator<T>* st=getBegin();
    Iterator<T>* en=getEnd();
    for (auto& each_observer :observers) {
      each_observer->update(st,en);
    }
  };
};
template<typename T>
class DataStructureDeprived:public DataStructure<T>{

};
#endif  // MY_STL_DS_H