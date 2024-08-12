//
// Created by lhy on 24-8-11.
//

#ifndef TUPLE_H
#define TUPLE_H
#include <type_traits>
#include <utility>

#include "meta.h"

namespace lhy {
template <typename... T>
class tuple {};
template <typename T>
class tuple_size;
template <typename T>
static constexpr size_t tuple_size_v = tuple_size<T>();
template <size_t I, class T>
  requires(I < tuple_size_v<T>)
class tuple_element;
template <size_t I, class T>
using tuple_element_t = typename tuple_element<I, T>::type;
template <typename FirstT, typename... RestT>
class tuple<FirstT, RestT...> {
 public:
  constexpr tuple() = default;
  explicit tuple(const FirstT& first_arg, const RestT&... rest_args) : first_(first_arg), rest_(rest_args...) {}
  template <typename FirstUtype, class... RestUTypes>
    requires(sizeof...(RestUTypes) == sizeof...(RestT) && (std::is_constructible_v<RestT, RestUTypes> && ...) &&
             std::is_constructible_v<FirstT, FirstUtype>)
  explicit tuple(FirstUtype&& first_arg, RestUTypes&&... args)
      : first_(std::forward<FirstUtype>(first_arg)), rest_(std::forward<RestUTypes>(args)...) {}
  // 没搞懂在在什么时候会用到
  // template <typename FirstUtype, class... RestUTypes>
  // requires(sizeof...(RestUTypes) == sizeof...(RestT) + 1 && (std::is_constructible_v<RestT, RestUTypes> && ...) &&
  // std::is_constructible_v<FirstT, FirstUtype>)
  // tuple(tuple<FirstUtype, RestUTypes...>& other);
  template <typename FirstUtype, class... RestUTypes>
    requires(sizeof...(RestUTypes) == sizeof...(RestT) && (std::is_constructible_v<RestT, RestUTypes> && ...) &&
             std::is_constructible_v<FirstT, FirstUtype>)
  tuple(const tuple<FirstUtype, RestUTypes...>& other) : first_(other.first_), rest_(other.rest_) {}
  template <typename FirstUtype, class... RestUTypes>
    requires(sizeof...(RestUTypes) == sizeof...(RestT) && (std::is_constructible_v<RestT, RestUTypes> && ...) &&
             std::is_constructible_v<FirstT, FirstUtype>)
  tuple(tuple<FirstUtype, RestUTypes...>&& other) : first_(std::move(other.first_)), rest_(std::move(other.rest_)) {}
  tuple(const tuple& other) = default;
  tuple(tuple&& other) = default;

  tuple& operator=(const tuple& other) = default;
  tuple& operator=(tuple&& other) = default;
  template <typename FirstUtype, class... RestUTypes>
    requires(sizeof...(RestUTypes) == sizeof...(RestT) && (std::is_constructible_v<RestT, RestUTypes> && ...) &&
             std::is_constructible_v<FirstT, FirstUtype>)
  tuple& operator=(const tuple<FirstUtype, RestUTypes...>& other) {
    first_ = other.first_;
    rest_ = other.rest_;
    return *this;
  }
  template <typename FirstUtype, class... RestUTypes>
    requires(sizeof...(RestUTypes) == sizeof...(RestT) && (std::is_constructible_v<RestT, RestUTypes> && ...) &&
             std::is_constructible_v<FirstT, FirstUtype>)
  tuple& operator=(tuple<FirstUtype, RestUTypes...>&& other) {
    first_ = std::move(other.first_);
    rest_ = std::move(other.rest_);
    return *this;
  }
  template <size_t I>
  constexpr tuple_element_t<I, tuple>& get() {
    if constexpr (I == 0) {
      return first_;
    } else {
      return rest_.template get<I - 1>();
    }
  }
  template <size_t I>
  constexpr const tuple_element_t<I, tuple>& get() const {
    if constexpr (I == 0) {
      return first_;
    } else {
      return rest_.template get<I - 1>();
    }
  }
  template <size_t I>
  constexpr tuple_element_t<I, tuple>&& get() && {
    if constexpr (I == 0) {
      return std::move(first_);
    } else {
      return std::move(rest_).template get<I - 1>();
    }
  }
  template <size_t I>
  constexpr const tuple_element_t<I, tuple>&& get() const&& {
    if constexpr (I == 0) {
      return std::move(first_);
    } else {
      return std::move(rest_).template get<I - 1>();
    }
  }
  friend constexpr std::partial_ordering operator<=>(const tuple& lhs, const tuple& rhs) {
    return lhs.first_ <=> rhs.first_;
    if constexpr (tuple_size_v<tuple> != 1) {
      if (lhs.first_ == rhs.first_) {
        return lhs.rest_ <=> rhs.rest_;
      }
    }
    return lhs.first_ <=> rhs.first_;
  }
  friend bool operator==(const tuple& lhs, const tuple& rhs) {
    if constexpr (tuple_size_v<tuple> != 1) {
      if (lhs.first_ == rhs.first_) {
        return lhs.rest_ == rhs.rest_;
      }
    }
    return lhs.first_ == rhs.first_;
  }

 private:
  FirstT first_;
  tuple<RestT...> rest_;
};
template <>
class tuple<> {};
template <typename T>
class unwrap_decay {
 public:
  using type = T;
};
/// 但是如果有多层怎么办
template <typename T>
class unwrap_decay<std::reference_wrapper<T>> {
 public:
  using type = T&;
};
template <typename T>
using unwrap_decay_t = std::decay_t<typename unwrap_decay<T>::type>;
template <class... Types>
tuple<unwrap_decay_t<Types>...> make_tuple(Types&&... args) {
  return tuple<unwrap_decay_t<Types>...>(std::forward<Types>(args)...);
}
template <typename... Args>
// 需要注意的是Args的&
tuple<Args&...> tie(Args&... args) {
  return tuple<Args&...>(args...);
}
template <class... Types>
tuple<Types&&...> forward_as_tuple(Types&&... args) {
  return tuple<Types&&...>(std::forward<Types&&>(args)...);
}
template <typename TupleFirst, class... RestTuples>
class tuple_cat_type {
 public:
  using type = typename tuple_cat_type<std::decay_t<TupleFirst>,
                                       typename tuple_cat_type<std::decay_t<RestTuples>...>::type>::type;
};
template <typename... TupleFirstTypes, typename... RestTuplesTypes>
class tuple_cat_type<tuple<TupleFirstTypes...>, tuple<RestTuplesTypes...>> {
 public:
  using type = tuple<TupleFirstTypes..., RestTuplesTypes...>;
};
template <typename... TupleFirstTypes>
class tuple_cat_type<tuple<TupleFirstTypes...>> {
 public:
  using type = tuple<TupleFirstTypes...>;
};
template <typename TupleFirst, class RestTuples>
typename tuple_cat_type<TupleFirst, RestTuples>::type tuple_cat(TupleFirst&& t1, RestTuples&& t2) {
  return tuple_catImpl(std::forward<TupleFirst>(t1), std::forward<RestTuples>(t2));
}
/// 要么就得将这边扩展成4种情况
template <typename... Args1, typename... Args2>
tuple<Args1..., Args2...> tuple_catImpl(tuple<Args1...> t1, tuple<Args2...> t2) {
  return tuple_catImpl(std::forward<tuple<Args1...>>(t1), IndexSequenceFor<Args1...>(),
                       std::forward<tuple<Args2...>>(t2), IndexSequenceFor<Args2...>());
}
template <typename... Args1, typename... Args2, size_t... Index1, size_t... Index2>
tuple<Args1..., Args2...> tuple_catImpl(tuple<Args1...> t1, IndexSequence<Index1...>, tuple<Args2...> t2,
                                        IndexSequence<Index2...>) {
  return tuple<Args1..., Args2...>(get<Index1>(t1)..., get<Index2>(t2)...);
}
template <typename TupleFirst, class... RestTuples>
typename tuple_cat_type<TupleFirst, RestTuples...>::type&& tuple_cat(TupleFirst&& first_arg, RestTuples&&... args) {
  if constexpr (sizeof...(RestTuples) == 0) {
    return std::forward<TupleFirst>(first_arg);
  } else {
    return tuple_cat(std::forward<TupleFirst>(first_arg), tuple_cat(std::forward<RestTuples>(args)...));
  }
}
template <typename T>
class tuple_size {};
template <typename... Types>
class tuple_size<tuple<Types...>> {
 public:
  constexpr operator size_t() const { return value; }

 private:
  static constexpr size_t value = sizeof...(Types);
};
template <size_t I, class T>
  requires(I < tuple_size_v<T>)
class tuple_element {};
template <size_t I, class FirstT, class... T>
class tuple_element<I, tuple<FirstT, T...>> {
 public:
  using type = typename tuple_element<I - 1, tuple<T...>>::type;
};
template <class FirstT, class... T>
class tuple_element<0, tuple<FirstT, T...>> {
 public:
  using type = FirstT;
};
template <size_t I, class... Types>
constexpr tuple_element_t<I, tuple<Types...>>& get(tuple<Types...>& t) {
  return t.template get<I>();
}
template <size_t I, class... Types>
constexpr tuple_element_t<I, tuple<Types...>>&& get(tuple<Types...>&& t) {
  return std::move(t).template get<I>();
}
template <size_t I, class... Types>
constexpr const tuple_element_t<I, tuple<Types...>>& get(const tuple<Types...>& t) {
  return t.template get<I>();
}
template <size_t I, class... Types>
constexpr const tuple_element_t<I, tuple<Types...>>&& get(const tuple<Types...>&& t) {
  return std::move(t).template get<I>();
}
/// 这样就实现了非Tuple的泛化
template <template <typename... Types> class Tuple, typename... Types, typename Func>
constexpr void apply(Tuple<Types...>& t, Func&& func) {
  apply(t, IndexSequenceFor<Types...>(), std::forward<Func>(func));
}
template <class Tuple, typename Func, size_t... Indexes>
constexpr void apply(Tuple& t, IndexSequence<Indexes...>, Func&& func) {
  (func(get<Indexes>(t)),...);
}
}  // namespace lhy

#endif  // TUPLE_H
