//
// Created by lhy on 24-8-18.
//

#ifndef OPTIONAL_H
#define OPTIONAL_H
#include <concepts>

#include "data_structure.h"
#include "span.h"

namespace lhy {
class nullopt_t {
 public:
  constexpr nullopt_t() = default;
};
constexpr nullopt_t nullopt{};
class in_place_t {
 public:
  constexpr in_place_t() = default;
};
constexpr in_place_t in_place{};
template <typename T>
class optional_value {
 public:
  using type = std::decay_t<T>;
  using begin_type = ForwardIterator<T>;
};
template <typename T>
  requires(requires { std::declval<std::decay_t<T>>().begin(); })
class optional_value<T> {
 public:
  using type = std::decay_t<decltype(std::declval<std::decay_t<T>>().front())>;
  using begin_type = decltype(std::declval<std::decay_t<T>>().begin());
};
template <typename T>
using optional_value_t = typename optional_value<T>::type;
template <typename T>
using optional_value_begin_t = typename optional_value<T>::begin_type;
template <typename T>
class optional : public DataStructure<optional_value_t<T>> {
 public:
  constexpr optional() = default;
  constexpr explicit optional(nullopt_t);
  constexpr optional(const optional& other);
  constexpr optional(optional&& other);
  template <std::convertible_to<T> U>
  explicit optional(const optional<U>& other);
  template <std::convertible_to<T> U>
  optional(optional<U>&& other);
  template <class... Args>
  constexpr explicit optional(in_place_t, Args&&... args);
  // template <class U, class... Args>
  // constexpr explicit optional(in_place_t, std::initializer_list<U> ilist, Args&&... args);
  template <std::convertible_to<T> U = T>
  constexpr optional(U&& value);
  constexpr ~optional() = default;
  optional& operator=(nullopt_t);
  constexpr optional& operator=(const optional& other);
  constexpr optional& operator=(optional&& other);
  template <std::convertible_to<T> U = T>
  optional& operator=(U&& other);
  template <std::convertible_to<T> U>
  optional& operator=(const optional<U>& other);
  template <std::convertible_to<T> U>
  optional& operator=(optional<U>&& other);
  auto begin();
  auto begin() const;
  auto end();
  auto end() const;
  constexpr const T* operator->() const;
  constexpr T* operator->();
  constexpr const T& operator*() const&;
  constexpr T& operator*() &;
  constexpr const T&& operator*() const&&;
  constexpr T&& operator*() &&;
  constexpr explicit operator bool() const;
  constexpr bool has_value() const;
  constexpr T& value() &;
  constexpr const T& value() const&;
  constexpr T&& value() &&;
  constexpr const T&& value() const&&;
  template <class U>
  constexpr T value_or(U&& default_value) const&;
  template <class U>
  constexpr T value_or(U&& default_value) &&;
  template <class F>
  constexpr auto and_then(F&& f) &;
  template <class F>
  constexpr auto and_then(F&& f) const&;
  template <class F>
  constexpr auto and_then(F&& f) &&;
  template <class F>
  constexpr auto and_then(F&& f) const&&;
  template <class F>
  constexpr auto transform(F&& f) &;
  template <class F>
  constexpr auto transform(F&& f) const&;
  template <class F>
  constexpr auto transform(F&& f) &&;
  template <class F>
  constexpr auto transform(F&& f) const&&;
  template <class F>
  constexpr optional or_else(F&& f) const&;
  template <class F>
  constexpr optional or_else(F&& f) &&;
  void swap(optional& other);
  void reset();
  template <class... Args>
  T& emplace(Args&&... args);
  // template <class U, class... Args>
  // T& emplace(std::initializer_list<U> ilist, Args&&... args);
  [[nodiscard]] typename DataStructure<optional_value_t<T>>::Pointer getBegin() override {
    return make_shared<optional_value_begin_t<T>>(begin());
  }
  [[nodiscard]] typename DataStructure<optional_value_t<T>>::Pointer getEnd() override {
    return make_shared<optional_value_begin_t<T>>(end());
  }

 protected:
  unique_ptr<T> value_{};
};
template <typename T>
constexpr optional<T>::optional(nullopt_t) {}
template <typename T>
constexpr optional<T>::optional(const optional& other) {
  if (other) {
    value_ = make_unique<T>(*other.value_);
  }
}
template <typename T>
constexpr optional<T>::optional(optional&& other) {
  if (other) {
    value_ = std::move(other.value_);
  }
}
template <typename T>
template <std::convertible_to<T> U>
optional<T>::optional(const optional<U>& other) {
  if (other) {
    value_ = make_unique<T>(*other.value_);
  }
}
template <typename T>
template <std::convertible_to<T> U>
optional<T>::optional(optional<U>&& other) {
  if (other) {
    value_ = make_unique<T>(std::move(*other.value_));
  }
}
template <typename T>
template <class... Args>
constexpr optional<T>::optional(in_place_t, Args&&... args) {
  value_ = make_unique<T>(std::forward<Args>(args)...);
}
template <typename T>
template <std::convertible_to<T> U>
constexpr optional<T>::optional(U&& value) {
  value_ = make_unique<T>(std::forward<U>(value));
}
template <typename T>
optional<T>& optional<T>::operator=(nullopt_t) {
  value_.reset();
  return *this;
}
template <typename T>
constexpr optional<T>& optional<T>::operator=(const optional& other) {
  if (value_ != other.value_) {
    value_ = make_unique<T>(*other.value_);
  }
  return *this;
}
template <typename T>
constexpr optional<T>& optional<T>::operator=(optional&& other) {
  if (value_ != other.value_) {
    value_ = std::move(other.value_);
  }
  return *this;
}
template <typename T>
template <std::convertible_to<T> U>
optional<T>& optional<T>::operator=(U&& other) {
  value_ = make_unique<T>(std::forward<U>(other));
  return *this;
}
template <typename T>
template <std::convertible_to<T> U>
optional<T>& optional<T>::operator=(const optional<U>& other) {
  value_ = make_unique<T>(*other.value_);
  return *this;
}
template <typename T>
template <std::convertible_to<T> U>
optional<T>& optional<T>::operator=(optional<U>&& other) {
  value_ = make_unique<T>(std::move(*other.value_));
  return *this;
}
template <typename T>
auto optional<T>::begin() {
  if constexpr (autoable<T>) {
    return value_->begin();
  } else {
    return ForwardIterator<T>(value_.get());
  }
}
template <typename T>
auto optional<T>::begin() const {
  if constexpr (autoable<T>) {
    return value_->begin();
  } else {
    return ForwardIterator<T>(value_.get());
  }
}
template <typename T>
auto optional<T>::end() {
  if constexpr (autoable<T>) {
    return value_->end();
  } else {
    return ForwardIterator<T>(nullptr);
  }
}
template <typename T>
auto optional<T>::end() const {
  if constexpr (autoable<T>) {
    return value_->end();
  } else {
    return ForwardIterator<T>(nullptr);
  }
}
template <typename T>
constexpr const T* optional<T>::operator->() const {
  return value_.get();
}
template <typename T>
constexpr T* optional<T>::operator->() {
  return value_.get();
}
template <typename T>
constexpr const T& optional<T>::operator*() const& {
  return *value_;
}
template <typename T>
constexpr T& optional<T>::operator*() & {
  return *value_;
}
template <typename T>
constexpr const T&& optional<T>::operator*() const&& {
  return std::move(*value_);
}
template <typename T>
constexpr T&& optional<T>::operator*() && {
  return std::move(*value_);
}
template <typename T>
constexpr optional<T>::operator bool() const {
  return value_;
}
template <typename T>
constexpr bool optional<T>::has_value() const {
  return value_;
}
template <typename T>
constexpr T& optional<T>::value() & {
  if (value_) {
    return *value_;
  }
  throw bad_optional_access();
}
template <typename T>
constexpr const T& optional<T>::value() const& {
  if (value_) {
    return *value_;
  }
  throw bad_optional_access();
}
template <typename T>
constexpr T&& optional<T>::value() && {
  if (value_) {
    return std::move(*value_);
  }
  throw bad_optional_access();
}
template <typename T>
constexpr const T&& optional<T>::value() const&& {
  if (value_) {
    return std::move(*value_);
  }
  throw bad_optional_access();
}
template <typename T>
template <class U>
constexpr T optional<T>::value_or(U&& default_value) const& {
  return value_ ? *value_ : std::forward<U>(default_value);
}
template <typename T>
template <class U>
constexpr T optional<T>::value_or(U&& default_value) && {
  return value_ ? std::move(*value_) : std::forward<U>(default_value);
}
template <typename T>
template <class F>
constexpr auto optional<T>::and_then(F&& f) & {
  if (*this) return std::invoke(std::forward<F>(f), **this);
  return std::remove_cvref_t<std::invoke_result_t<F, T&>>{};
}
template <typename T>
template <class F>
constexpr auto optional<T>::and_then(F&& f) const& {
  if (*this) return std::invoke(std::forward<F>(f), **this);
  return std::remove_cvref_t<std::invoke_result_t<F, const T&>>{};
}
template <typename T>
template <class F>
constexpr auto optional<T>::and_then(F&& f) && {
  if (*this) return std::invoke(std::forward<F>(f), std::move(**this));
  return std::remove_cvref_t<std::invoke_result_t<F, T&&>>{};
}
template <typename T>
template <class F>
constexpr auto optional<T>::and_then(F&& f) const&& {
  /// 拿存储的值去调用F
  if (*this) return std::invoke(std::forward<F>(f), std::move(**this));
  return std::remove_cvref_t<std::invoke_result_t<F, const T&&>>{};
}
template <typename T>
template <class F>
constexpr auto optional<T>::transform(F&& f) & {
  if (*this) return optional<std::invoke_result_t<F, T&>>{std::invoke(std::forward<F>(f), **this)};
  return optional<std::invoke_result_t<F, T&>>{};
}
template <typename T>
template <class F>
constexpr auto optional<T>::transform(F&& f) const& {
  if (*this) return optional<std::invoke_result_t<F, const T&>>{std::invoke(std::forward<F>(f), **this)};
  return optional<std::invoke_result_t<F, const T&>>{};
}
template <typename T>
template <class F>
constexpr auto optional<T>::transform(F&& f) && {
  if (*this) return optional<std::invoke_result_t<F, T&&>>{std::invoke(std::forward<F>(f), std::move(**this))};
  return optional<std::invoke_result_t<F, T&&>>{};
}
template <typename T>
template <class F>
constexpr auto optional<T>::transform(F&& f) const&& {
  if (*this) return optional<std::invoke_result_t<F, const T&&>>{std::invoke(std::forward<F>(f), std::move(**this))};
  return optional<std::invoke_result_t<F, const T&&>>{};
}
template <typename T>
template <class F>
constexpr optional<T> optional<T>::or_else(F&& f) const& {
  return *this ? *this : std::forward<F>(f)();
}
template <typename T>
template <class F>
constexpr optional<T> optional<T>::or_else(F&& f) && {
  return *this ? std::move(*this) : std::forward<F>(f)();
}
template <typename T>
void optional<T>::swap(optional& other) {
  value_.swap(other.value_);
}
template <typename T>
void optional<T>::reset() {
  value_.reset();
}
template <typename T>
template <class... Args>
T& optional<T>::emplace(Args&&... args) {
  value_ = make_unique<T>(std::forward<Args>(args)...);
  return *value_;
}
template <typename T>
optional(T) -> optional<T>;
template <typename T>
void swap(optional<T>& lhs, optional<T>& rhs) {
  lhs.swap(rhs);
}
template <class T>
constexpr optional<std::decay_t<T>> make_optional(T&& value) {
  return optional<std::decay_t<T>>(std::forward<T>(value));
}
template <class T, class... Args>
constexpr optional<T> make_optional(Args&&... args) {
  return optional<T>(in_place, std::forward<Args>(args)...);
}

}  // namespace lhy

#endif  // OPTIONAL_H
