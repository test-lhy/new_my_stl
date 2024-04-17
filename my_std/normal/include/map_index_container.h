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
  T& operator[](Index index) override;
  T& operator[](Index index) const override;

 private:
  std::map<int, T> container_;
};

template <typename T>
T& MapIndexContainer<T>::operator[](Index index) {
  return container_[index];
}
template <typename T>
T& MapIndexContainer<T>::operator[](Index index) const {
  return container_[index];
}
}  // namespace lhy

#endif  // MAP_INDEX_CONTAINER_H
