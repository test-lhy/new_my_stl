//
// Created by lhy on 24-4-14.
//

#ifndef VECTOR_INDEX_CONTAINER_H
#define VECTOR_INDEX_CONTAINER_H
#include <utility>

#include "index_container_impl.h"
#include "list.h"
#include "vector.h"
namespace lhy {
template <typename T>
class VectorIndexContainer : public IndexContainerImpl<T> {
  enum UseStatus { NotUsed, Used, UsedBefore };

 public:
  class BaseIterator;
  class iterator;
  class reversed_iterator;
  void erase(Index index) override;
  bool Exist(Index index) override;
  [[nodiscard]] typename IndexContainerImpl<T>::iterator begin() override;
  [[nodiscard]] typename IndexContainerImpl<T>::iterator end() override;
  [[nodiscard]] typename IndexContainerImpl<T>::reversed_iterator rbegin() override;
  [[nodiscard]] typename IndexContainerImpl<T>::reversed_iterator rend() override;
  T& operator[](Index index) override;
  const T& operator[](Index index) const override;

 private:
  [[nodiscard]] typename DataStructure<T>::Pointer getBegin() override;
  [[nodiscard]] typename DataStructure<T>::Pointer getEnd() override;

  vector<std::pair<T, UseStatus>> container_;
  list<Index> used_index;
};
template <typename T>
class VectorIndexContainer<T>::BaseIterator {
 public:
  BaseIterator(VectorIndexContainer<T>* father, list<Index>::iterator _iterator)
      : father_(father),
        base_iterator_(_iterator),
        base_reversed_iterator_(_iterator),
        iterator_pointer_(&base_iterator_) {}
  BaseIterator(VectorIndexContainer<T>* father, list<Index>::reversed_iterator _iterator)
      : father_(father),
        base_iterator_(_iterator),
        base_reversed_iterator_(_iterator),
        iterator_pointer_(&base_reversed_iterator_) {}
  void add() {
    if (!CheckRange()) {
      throw std::logic_error("out of range");
    }
    iterator_pointer_->operator++();
    CheckIterator();
  }
  void sub() {
    if (!CheckRange()) {
      throw std::logic_error("out of range");
    }
    iterator_pointer_->operator--();
    CheckIterator();
  }
  auto& GetUsedStatus() { return father_->container_[*(*iterator_pointer_)].second; }
  bool CheckExistence() { return GetUsedStatus() == Used; }
  void SetNotUsed() { GetUsedStatus() = NotUsed; }
  bool CheckRange() {
    return *iterator_pointer_ != father_->used_index.end() && *iterator_pointer_ != father_->used_index.rend();
  }
  virtual T*& GetPointer() = 0;
  void UpdatePointer() {
    if (CheckRange()) {
      this->GetPointer() = &(father_->container_[*(*iterator_pointer_)].first);
    } else {
      this->GetPointer() = nullptr;
    }
  }
  virtual void EraseMyself() = 0;
  void CheckIterator() {
    while (CheckRange() && !CheckExistence()) {
      SetNotUsed();
      EraseMyself();
    }
    UpdatePointer();
  }

 protected:
  VectorIndexContainer<T>* father_;
  list<Index>::iterator base_iterator_;
  list<Index>::reversed_iterator base_reversed_iterator_;
  list<Index>::iterator* iterator_pointer_;
};
template <typename T>
class VectorIndexContainer<T>::iterator : public IndexContainerImpl<T>::iteratorImpl, public BaseIterator {
 public:
  iterator(VectorIndexContainer* father, list<Index>::iterator _iterator) : BaseIterator(father, _iterator) {
    BaseIterator::CheckIterator();
    BaseIterator::UpdatePointer();
  }
  typename IndexContainerImpl<T>::iteratorImpl& operator++() override {
    BaseIterator::add();
    return *this;
  }
  typename IndexContainerImpl<T>::iteratorImpl& operator--() override {
    BaseIterator::sub();
    return *this;
  }
  T*& GetPointer() override { return this->getPointer(); }
  void EraseMyself() override {
    BaseIterator::base_iterator_ = BaseIterator::father_->used_index.erase(BaseIterator::base_iterator_);
  }
};
template <typename T>
class VectorIndexContainer<T>::reversed_iterator : public IndexContainerImpl<T>::reversed_iteratorImpl,
                                                   public BaseIterator {
 public:
  reversed_iterator(VectorIndexContainer* father, list<Index>::reversed_iterator _iterator)
      : BaseIterator(father, _iterator) {
    BaseIterator::CheckIterator();
    BaseIterator::UpdatePointer();
  }
  typename IndexContainerImpl<T>::reversed_iteratorImpl& operator++() override {
    BaseIterator::add();
    return *this;
  }
  typename IndexContainerImpl<T>::reversed_iteratorImpl& operator--() override {
    BaseIterator::sub();
    return *this;
  }
  T*& GetPointer() override { return this->getPointer(); }
  void EraseMyself() override {
    BaseIterator::base_reversed_iterator_ =
        BaseIterator::father_->used_index.erase(BaseIterator::base_reversed_iterator_);
  }
};
template <typename T>
void VectorIndexContainer<T>::erase(Index index) {
  if (index >= container_.size()) {
    throw std::logic_error("out of range");
  }
  if (container_[index].second != Used) {
    throw std::logic_error("can't delete an element that does not exist");
  }
  container_[index].second = UsedBefore;
}
template <typename T>
bool VectorIndexContainer<T>::Exist(Index index) {
  if (index >= container_.size()) {
    return false;
  }
  return container_[index].second == Used;
}
template <typename T>
typename IndexContainerImpl<T>::iterator VectorIndexContainer<T>::begin() {
  return new iterator(this, used_index.begin());
}
template <typename T>
typename IndexContainerImpl<T>::iterator VectorIndexContainer<T>::end() {
  return new iterator(this, used_index.end());
}
template <typename T>
typename IndexContainerImpl<T>::reversed_iterator VectorIndexContainer<T>::rbegin() {
  return new reversed_iterator(this, used_index.rbegin());
}
template <typename T>
typename IndexContainerImpl<T>::reversed_iterator VectorIndexContainer<T>::rend() {
  return new reversed_iterator(this, used_index.rend());
}
template <typename T>
T& VectorIndexContainer<T>::operator[](Index index) {
  if (index >= container_.size()) {
    container_.reserve(index * 2 + 100);
  }
  if (container_[index].second != Used) {
    if (container_[index].second == NotUsed) {
      used_index.push_back(index);
    }
    container_[index].second = Used;
  }
  return container_[index].first;
}
template <typename T>
typename DataStructure<T>::Pointer VectorIndexContainer<T>::getBegin() {
  return std::dynamic_pointer_cast<TForwardIterator<T>>(
      std::make_shared<typename IndexContainerImpl<T>::iterator>(new iterator(this, used_index.begin())));
}
template <typename T>
typename DataStructure<T>::Pointer VectorIndexContainer<T>::getEnd() {
  return std::dynamic_pointer_cast<TForwardIterator<T>>(
      std::make_shared<typename IndexContainerImpl<T>::iterator>(new iterator(this, used_index.end())));
}
template <typename T>
const T& VectorIndexContainer<T>::operator[](Index index) const {
  return container_[index].first;
}
}  // namespace lhy

#endif  // VECTOR_INDEX_CONTAINER_H
