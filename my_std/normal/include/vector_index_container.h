//
// Created by lhy on 24-4-14.
//

#ifndef VECTOR_INDEX_CONTAINER_H
#define VECTOR_INDEX_CONTAINER_H
#include "index_container_impl.h"
#include "vector.h"
namespace lhy {
template <typename T>
class VectorIndexContainer : public IndexContainerImpl<T> {
 public:
  T& operator[](Index index) override;
  T& operator[](Index index) const override;

 private:
  vector<T> container_;
};

template <typename T>
T& VectorIndexContainer<T>::operator[](Index index) {
  if (index >= container_.size()) {
    container_.reserve(index * 2 + 100);
  }
  return container_[index];
}
template <typename T>
T& VectorIndexContainer<T>::operator[](Index index) const {
  return container_[index];
}
}  // namespace lhy

#endif  // VECTOR_INDEX_CONTAINER_H
