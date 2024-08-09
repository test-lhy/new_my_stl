//
// Created by lhy on 24-8-9.
//

#ifndef SPAN_H
#define SPAN_H
#include "data_structure.h"
#include "type_traits.h"
namespace lhy {
template <typename T>
concept autoable = requires(T& obj) {
  obj.begin();
  obj.end();
};
template <typename T, typename U>
concept spanable = autoable<T> and requires(T& obj) {
  obj.begin() + 1;
} and (requires(T& obj) {
  { *obj.begin() } -> std::convertible_to<U>;
}or requires(T& obj)
{
  { *obj.begin() } -> std::convertible_to<U&>;
});
template <typename T, typename U>
concept NormTIter = T_iter<T, U> and requires(T& obj) { obj + 1; };
template <typename T, typename U>
concept DataStructureSpan = spanable<T, U> && requires(T& obj) { obj.begin().getPointer(); };
template <typename T, typename U = T>
class span : public DataStructure<T> {
 public:
  using iterator = NormIterator<T, U>;
  using reversed_iterator = ReversedNormIterator<T, U>;
  using typename DataStructure<T>::Pointer;
  using NormalPointer = U*;
  span() = default;
  template <NormTIter<T> TIter>
  span(TIter begin, TIter end);
  template <NormTIter<T> TIter>
  span(TIter begin, size_t size);
  template <spanable<T> Span_>
  span(const Span_& other);
  template <DataStructureSpan<T> Span_>
  span(const Span_& other);
  span(const span& other) = default;
  span& operator=(const span& other) = default;
  const T& operator[](const Index& index) const;
  [[nodiscard]] const T& At(const Index& index) const;
  [[nodiscard]] T& front() const;
  [[nodiscard]] T& back() const;
  [[nodiscard]] iterator begin() const;
  [[nodiscard]] iterator end() const;
  [[nodiscard]] reversed_iterator rbegin() const;
  [[nodiscard]] reversed_iterator rend() const;
  [[nodiscard]] iterator cbegin() const;
  [[nodiscard]] iterator cend() const;
  [[nodiscard]] NormalPointer data() const;
  [[nodiscard]] size_t size() const;
  [[nodiscard]] bool empty() const;
  [[nodiscard]] span subspan(Index offset, size_t count);
  [[nodiscard]] span first(size_t count);
  [[nodiscard]] span last(size_t count);

 private:
  [[nodiscard]] typename DataStructure<T>::Pointer getBegin() override;
  [[nodiscard]] typename DataStructure<T>::Pointer getEnd() override;
  iterator begin_{};
  iterator end_{};
};
template <typename T, typename U>
template <NormTIter<T> TIter>
span<T, U>::span(TIter begin, TIter end) {
  begin_ = ToTPointer<T, U>(begin);
  end_ = ToTPointer<T, U>(end);
}
template <typename T, typename U>
template <NormTIter<T> TIter>
span<T, U>::span(TIter begin, size_t size) {
  begin_ = ToTPointer<T, U>(begin);
  end_ = begin_ + size;
}
template <typename T, typename U>
template <spanable<T> Span_>
span<T, U>::span(const Span_& other) {
  begin_ = static_cast<U*>(other.begin());
  end_ = static_cast<U*>(other.end());
}
template <typename T, typename U>
template <DataStructureSpan<T> Span_>
span<T, U>::span(const Span_& other) {
  begin_ = static_cast<U*>(other.begin().getPointer());
  end_ = static_cast<U*>(other.end().getPointer());
}
template <typename T, typename U>
const T& span<T, U>::operator[](const Index& index) const {
  return begin_[index];
}
template <typename T, typename U>
const T& span<T, U>::At(const Index& index) const {
  if (index >= size()) {
    throw std::out_of_range("Index out of range");
  }
  return begin_[index];
}
template <typename T, typename U>
T& span<T, U>::front() const {
  return *begin();
}
template <typename T, typename U>
T& span<T, U>::back() const {
  return *(end() - 1);
}
template <typename T, typename U>
typename span<T, U>::iterator span<T, U>::begin() const {
  return begin_;
}
template <typename T, typename U>
typename span<T, U>::iterator span<T, U>::end() const {
  return end_;
}
template <typename T, typename U>
typename span<T, U>::reversed_iterator span<T, U>::rbegin() const {
  return end_ - 1;
}
template <typename T, typename U>
typename span<T, U>::reversed_iterator span<T, U>::rend() const {
  return begin_ - 1;
}
template <typename T, typename U>
typename span<T, U>::iterator span<T, U>::cbegin() const {
  return begin();
}
template <typename T, typename U>
typename span<T, U>::iterator span<T, U>::cend() const {
  return end();
}
template <typename T, typename U>
typename span<T, U>::NormalPointer span<T, U>::data() const {
  return begin_.getPointer();
}
template <typename T, typename U>
size_t span<T, U>::size() const {
  return end_ - begin_;
}
template <typename T, typename U>
bool span<T, U>::empty() const {
  return begin_ == end_;
}
template <typename T, typename U>
span<T, U> span<T, U>::subspan(Index offset, size_t count) {
  return span(begin_ + offset, count);
}
template <typename T, typename U>
span<T, U> span<T, U>::first(size_t count) {
  return span(begin_, count);
}
template <typename T, typename U>
span<T, U> span<T, U>::last(size_t count) {
  return span(end_ - count, count);
}
template <typename T, typename U>
typename DataStructure<T>::Pointer span<T, U>::getBegin() {
  return make_shared<iterator>(begin_);
}
template <typename T, typename U>
typename DataStructure<T>::Pointer span<T, U>::getEnd() {
  return make_shared<iterator>(end_);
}
template <autoable Span_>
std::ostream& operator<<(std::ostream& ostream_, Span_ obj) {
  for (auto& each : obj) {
    ostream_ << each;
    if constexpr (!std::is_same_v<std::decay_t<decltype(each)>, char>) {
      ostream_ << '\n';
    }
  }
  return ostream_;
}
}  // namespace lhy
#endif  // SPAN_H
