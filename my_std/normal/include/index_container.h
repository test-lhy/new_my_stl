//
// Created by lhy on 24-4-14.
//

#ifndef INDEX_CONTAINER_H
#define INDEX_CONTAINER_H
#include "index_container_impl.h"
#include "map_index_container.h"
#include "node.h"
#include "vector_index_container.h"
namespace lhy {
template <typename T>
class IndexContainer : public IndexContainerImpl<T> {
 public:
  using typename IndexContainerImpl<T>::RealT;
  template <node_c Node_>
  friend class NodeAllocator;
  explicit IndexContainer(IndexContainerMode mode = VectorIndexContainerMode);
  T& operator[](Index index) override;
  const T& operator[](Index index) const override;
  void erase(Index index) override;
  bool Exist(Index index) override;
  [[nodiscard]] typename IndexContainerImpl<T>::iterator begin() override;
  [[nodiscard]] typename IndexContainerImpl<T>::iterator end() override;
  [[nodiscard]] typename IndexContainerImpl<T>::reversed_iterator rbegin() override;
  [[nodiscard]] typename IndexContainerImpl<T>::reversed_iterator rend() override;
  [[nodiscard]] IndexContainerMode GetMode() const override;
  ~IndexContainer() override = default;

 private:
  [[nodiscard]] typename DataStructure<RealT>::Pointer getBegin() override;
  [[nodiscard]] typename DataStructure<RealT>::Pointer getEnd() override;

  unique_ptr<IndexContainerImpl<T>> container_;
};

template <typename T>
IndexContainer<T>::IndexContainer(const IndexContainerMode mode) {
  if (mode == VectorIndexContainerMode) {
    container_ = make_unique<VectorIndexContainer<T>>();
  } else if (mode == MapIndexContainerMode) {
    container_ = make_unique<MapIndexContainer<T>>();
  } else {
    throw std::logic_error("Unsupported mode");
  }
}
template <typename T>
T& IndexContainer<T>::operator[](Index index) {
  return container_->operator[](index);
}
template <typename T>
const T& IndexContainer<T>::operator[](Index index) const {
  return container_->operator[](index);
}
template <typename T>
void IndexContainer<T>::erase(Index index) {
  container_->erase(index);
}
template <typename T>
bool IndexContainer<T>::Exist(Index index) {
  return container_->Exist(index);
}
template <typename T>
typename IndexContainerImpl<T>::iterator IndexContainer<T>::begin() {
  return container_->begin();
}
template <typename T>
typename IndexContainerImpl<T>::iterator IndexContainer<T>::end() {
  return container_->end();
}
template <typename T>
typename IndexContainerImpl<T>::reversed_iterator IndexContainer<T>::rbegin() {
  return container_->rbegin();
}
template <typename T>
typename IndexContainerImpl<T>::reversed_iterator IndexContainer<T>::rend() {
  return container_->rend();
}
template <typename T>
IndexContainerMode IndexContainer<T>::GetMode() const {
  return container_->GetMode();
}
template <typename T>
typename DataStructure<typename IndexContainer<T>::RealT>::Pointer IndexContainer<T>::getBegin() {
  return container_->getBegin();
}
template <typename T>
typename DataStructure<typename IndexContainer<T>::RealT>::Pointer IndexContainer<T>::getEnd() {
  return container_->getEnd();
}
}  // namespace lhy

#endif  // INDEX_CONTAINER_H
