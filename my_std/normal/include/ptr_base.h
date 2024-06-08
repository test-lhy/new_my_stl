//
// Created by lhy on 24-6-4.
//

#ifndef PTR_BASE_H
#define PTR_BASE_H
#include <functional>
namespace lhy {
template <typename T>
using Deleter = std::function<void(T)>;
template <typename T>
class ptrType {
 public:
  using Type = std::add_pointer_t<T>;
  inline static Deleter<Type> default_deleter_ = [](Type ptr) -> void { delete ptr; };
};
template <typename T>
concept array_c = std::is_array_v<T>;
template <typename T>
concept not_array_c = !array_c<T>;
template <typename T>
concept bounded_array_c = std::is_bounded_array_v<T>;
template <typename T>
concept unbounded_array_c = std::is_unbounded_array_v<T>;
template <array_c T>
class ptrType<T> {
 public:
  using Type = std::add_pointer_t<std::remove_extent_t<T>>;
  inline static Deleter<Type> default_deleter_ = [](Type ptr) -> void { delete[] ptr; };
};
template <typename T, typename U>
concept convertiable_c = std::is_convertible_v<T, U>;
template <typename T, typename U>
concept convertiable_pointer_c = std::is_convertible_v<typename ptrType<T>::Type, U>;
}  // namespace lhy
#endif  // PTR_BASE_H
