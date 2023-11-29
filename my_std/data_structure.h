//
// Created by lhy on 23-9-20.
//
#include <stdexcept>
#ifndef MY_STL_DATA_STRUCTURE_H
#define MY_STL_DATA_STRUCTURE_H
namespace lhy {
template <typename T>
size_t distance(T* st, T* en) {
  return en - st;
}
template <typename T>
class Iterator {
 public:
  virtual Iterator* next() = 0;
  Iterator() : data_(nullptr){};
  Iterator(T* data) : data_(data){};

  explicit operator T*() { return data_; };
  explicit operator const T*() const { return data_; };

  T& operator*() { return *data_; }

  virtual void deletePointer() = 0;


  T*& getPointer() { return data_; }
  T*const& getPointer() const { return data_; }
  int operator-(const Iterator& i) { return data_ - i.data_; }

 private:
  T* data_;
};
template <typename T>
class DataStructure {
  std::vector<int> observers;
  virtual const Iterator<T>* getBegin() const = 0;
  virtual const Iterator<T>* getEnd() const = 0;
  virtual Iterator<T>* getBegin() = 0;
  virtual Iterator<T>* getEnd() = 0;
  void inform() {
    auto* st = new Iterator<T>(*getBegin());
    auto* en = new Iterator<T>(*getEnd());
    for (auto& each_observer : observers) {
      //      each_observer->update(st,en);
    }
    delete st;
    delete en;
  };
};

}  // namespace lhy
#endif  // MY_STL_DATA_STRUCTURE_H