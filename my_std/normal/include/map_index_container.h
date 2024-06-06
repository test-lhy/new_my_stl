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
  using typename IndexContainerImpl<T>::RealT;
  class iterator;
  class reversed_iterator;
  T& operator[](Index index) override;
  const T& operator[](Index index) const override;
  void erase(Index index) override;
  bool Exist(Index index) override;
  [[nodiscard]] IndexContainerMode GetMode() const override;
  [[nodiscard]] typename IndexContainerImpl<T>::iterator begin() override;
  [[nodiscard]] typename IndexContainerImpl<T>::iterator end() override;
  [[nodiscard]] typename IndexContainerImpl<T>::reversed_iterator rbegin() override;
  [[nodiscard]] typename IndexContainerImpl<T>::reversed_iterator rend() override;

 private:
  [[nodiscard]] typename DataStructure<RealT>::Pointer getBegin() override;
  [[nodiscard]] typename DataStructure<RealT>::Pointer getEnd() override;
  std::map<Index, RealT> container_;
};
template <typename T>
class MapIndexContainer<T>::iterator : public IndexContainerImpl<T>::iteratorImpl {
 public:
  iterator(const typename std::map<Index, RealT>::iterator& iterator) : iterator_(iterator) { UpdatePointer(); }
  iterator(const typename std::map<Index, RealT>::const_iterator& iterator) : iterator_(iterator) { UpdatePointer(); }
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
  typename std::map<Index, RealT>::iterator iterator_;
};
template <typename T>
class MapIndexContainer<T>::reversed_iterator : public IndexContainerImpl<T>::reversed_iteratorImpl {
 public:
  reversed_iterator(const typename std::map<Index, RealT>::reverse_iterator& iterator) : iterator_(iterator) {
    UpdatePointer();
  }
  reversed_iterator(const typename std::map<Index, RealT>::const_reverse_iterator& iterator) : iterator_(iterator) {
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
  typename std::map<Index, RealT>::reverse_iterator iterator_;
};
template <typename T>
T& MapIndexContainer<T>::operator[](Index index) {
  container_[index].first = index;
  return container_[index].second;
}
template <typename T>
const T& MapIndexContainer<T>::operator[](Index index) const {
  return container_.at(index).second;
}
template <typename T>
void MapIndexContainer<T>::erase(Index index) {
  container_.erase(index);
}
template <typename T>
bool MapIndexContainer<T>::Exist(Index index) {
  return container_.contains(index);
}
template <typename T>
IndexContainerMode MapIndexContainer<T>::GetMode() const {
  return MapIndexContainerMode;
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
typename DataStructure<typename MapIndexContainer<T>::RealT>::Pointer MapIndexContainer<T>::getBegin() {
  return make_shared<iterator>(container_.begin());
}
template <typename T>
typename DataStructure<typename MapIndexContainer<T>::RealT>::Pointer MapIndexContainer<T>::getEnd() {
  return make_shared<iterator>(container_.end());
  // return std::dynamic_pointer_cast<TForwardIterator<RealT>>(
  // std::make_shared<typename IndexContainerImpl<T>::iterator>(new iterator(container_.end())));
}
}  // namespace lhy

#endif  // MAP_INDEX_CONTAINER_H
