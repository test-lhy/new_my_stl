//
// Created by lhy on 24-4-12.
//

#ifndef INDEX_CONTAINER_IMPL_H
#define INDEX_CONTAINER_IMPL_H
#include "basic.h"
namespace lhy {
template <typename T>
class IndexContainerImpl {
 public:
  virtual T& operator[](Index index) = 0;
  virtual T& operator[](Index index) const = 0;
  virtual ~IndexContainerImpl() = default;
};
}  // namespace lhy

#endif  // INDEX_CONTAINER_IMPL_H
