//
// Created by lhy on 24-4-14.
//

#ifndef INDEX_CONTAINER_H
#define INDEX_CONTAINER_H
#include "index_container_impl.h"
#include "map_index_container.h"
#include "vector_index_container.h"
namespace lhy {
enum IndexContainerMode { VectorIndexContainerMode, MapIndexContainerMode };
template <typename T>
class IndexContainer : public IndexContainerImpl<T> {
 public:
  explicit IndexContainer(IndexContainerMode mode = VectorIndexContainerMode);
  T& operator[](Index index) override;
  T& operator[](Index index) const override;
  ~IndexContainer() override;

 private:
  IndexContainerImpl<T>* container_;
};

template <typename T>
IndexContainer<T>::IndexContainer(const IndexContainerMode mode) {
  if (mode == VectorIndexContainerMode) {
    container_ = new VectorIndexContainer<T>();
  } else if (mode == MapIndexContainerMode) {
    container_ = new MapIndexContainer<T>();
  } else {
    throw std::logic_error("Unsupported mode");
  }
}
template <typename T>
T& IndexContainer<T>::operator[](Index index) {
  return container_->operator[](index);
}
template <typename T>
T& IndexContainer<T>::operator[](Index index) const {
  return container_->operator[](index);
}
template <typename T>
IndexContainer<T>::~IndexContainer() {
  delete container_;
}
}  // namespace lhy

#endif  // INDEX_CONTAINER_H
