//
// Created by lhy on 24-4-14.
//

#ifndef MAP_INDEX_CONTAINER_H
#define MAP_INDEX_CONTAINER_H
#include <map>

#include "index_container_impl.h"
namespace lhy {
template <typename T>
class MapIndexContainer : public IndexContainerImpl<T> {
 public:
  class iterator;
  class reversed_iterator;
  T& operator[](Index index) override;
  const T& operator[](Index index) const override;
  void erase(Index index) override;
  [[nodiscard]] typename IndexContainerImpl<T>::iterator begin() override;
  [[nodiscard]] typename IndexContainerImpl<T>::iterator end() override;
  [[nodiscard]] typename IndexContainerImpl<T>::reversed_iterator rbegin() override;
  [[nodiscard]] typename IndexContainerImpl<T>::reversed_iterator rend() override;

 private:
  [[nodiscard]] typename DataStructure<T>::Pointer getBegin() override;
  [[nodiscard]] typename DataStructure<T>::Pointer getEnd() override;
  std::map<Index, T> container_;
};
template <typename T>
class MapIndexContainer<T>::iterator : public IndexContainerImpl<T>::iteratorImpl {
 public:
  iterator(const typename std::map<Index, T>::iterator& iterator) : iterator_(iterator) { UpdatePointer(); }
  iterator(const typename std::map<Index, T>::const_iterator& iterator) : iterator_(iterator) { UpdatePointer(); }
  typename IndexContainerImpl<T>::iteratorImpl& operator++() override {
    iterator_++;
    UpdatePointer();
    return *this;
  }
  typename IndexContainerImpl<T>::iteratorImpl& operator--() override {
    iterator_--;
    UpdatePointer();
    return *this;
  }
  void UpdatePointer() { this->getPointer() = &(iterator_->second); }

 private:
  typename std::map<Index, T>::iterator iterator_;
};
template <typename T>
class MapIndexContainer<T>::reversed_iterator : public IndexContainerImpl<T>::reversed_iteratorImpl {
 public:
  reversed_iterator(const typename std::map<Index, T>::reverse_iterator& iterator) : iterator_(iterator) {
    UpdatePointer();
  }
  reversed_iterator(const typename std::map<Index, T>::const_reverse_iterator& iterator) : iterator_(iterator) {
    UpdatePointer();
  }
  typename IndexContainerImpl<T>::reversed_iteratorImpl& operator++() override {
    iterator_++;
    UpdatePointer();
    return *this;
  }
  typename IndexContainerImpl<T>::reversed_iteratorImpl& operator--() override {
    iterator_--;
    UpdatePointer();
    return *this;
  }
  void UpdatePointer() { this->getPointer() = &(iterator_->second); }

 private:
  typename std::map<Index, T>::reverse_iterator iterator_;
};
template <typename T>
T& MapIndexContainer<T>::operator[](Index index) {
  return container_[index];
}
template <typename T>
const T& MapIndexContainer<T>::operator[](Index index) const {
  return container_.at(index);
}
template <typename T>
void MapIndexContainer<T>::erase(Index index) {
  container_.erase(index);
}
template <typename T>
typename IndexContainerImpl<T>::iterator MapIndexContainer<T>::begin() {
  return new iterator(container_.begin());
}
template <typename T>
typename IndexContainerImpl<T>::iterator MapIndexContainer<T>::end() {
  return new iterator(container_.end());
}
template <typename T>
typename IndexContainerImpl<T>::reversed_iterator MapIndexContainer<T>::rbegin() {
  return new reversed_iterator(container_.rbegin());
}
template <typename T>
typename IndexContainerImpl<T>::reversed_iterator MapIndexContainer<T>::rend() {
  return new reversed_iterator(container_.rend());
}
template <typename T>
typename DataStructure<T>::Pointer MapIndexContainer<T>::getBegin() {
  return std::dynamic_pointer_cast<TForwardIterator<T>>(
      std::make_shared<typename IndexContainerImpl<T>::iterator>(new iterator(container_.begin())));
}
template <typename T>
typename DataStructure<T>::Pointer MapIndexContainer<T>::getEnd() {
  return std::dynamic_pointer_cast<TForwardIterator<T>>(
      std::make_shared<typename IndexContainerImpl<T>::iterator>(new iterator(container_.end())));
}
}  // namespace lhy

#endif  // MAP_INDEX_CONTAINER_H
