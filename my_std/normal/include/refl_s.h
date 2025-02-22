//
// Created by 连昊宇 on 25-2-19.
//

#ifndef REFL_S_H
#define REFL_S_H
#include <any>
#include <array>
#include <ranges>
#include <string>
#include <type_traits>

#include "meta.h"
namespace lhy {
template <typename T>
class refl_s {};
template <typename T, typename U>
T& GetMember(U& object, const std::string& name) {
  return std::any_cast<std::reference_wrapper<T>>(refl_s<U>::GetMember(object, name)).get();
}
template <typename T, typename U>
const T& GetMember(const U& object, const std::string& name) {
  return std::any_cast<std::reference_wrapper<const T>>(refl_s<U>::GetMember(object, name)).get();
}
}  // namespace lhy

#define CREATE_ROBBER1(Class, KidName)           \
  template <auto T>                              \
  class Class##KidName##RetType {};              \
  constexpr auto GetPtr##Class##KidName##Type(); \
  template <typename T
#define CREATE_ROBBER2(Class, KidName)                                                                   \
  T Class::* M > class Class##KidName##RetType<M> {                                                      \
   public:                                                                                               \
    using RealType = T;                                                                                  \
    friend constexpr auto GetPtr##Class##KidName##Type() { return RealType{}; };                         \
  };                                                                                                     \
  template class Class##KidName##RetType<&Class::KidName>;                                               \
  using Ret##Class##KidName##Type = FunctionProperties<decltype(GetPtr##Class##KidName##Type)>::RetType; \
  constexpr Ret##Class##KidName##Type Class::* GetPtr##Class##KidName();                                 \
  template <Ret##Class##KidName##Type Class::* M>                                                        \
  struct Rob##Class##KidName {                                                                           \
    friend constexpr Ret##Class##KidName##Type Class::* GetPtr##Class##KidName() { return M; }           \
  };                                                                                                     \
  template struct Rob##Class##KidName<&Class::KidName>;
#define GET_KID(Class, KidName) GetPtr##Class##KidName()
#define GET_KID_TYPE(Class, KidName) decltype(GET_KID(Class, KidName))
#define GET_KID_NAME_AND_NUM1(KidName, num) \
  {                                         \
    #KidName
#define GET_KID_NAME_AND_NUM2(KidName, num) \
  num                                       \
  }
#define REFL(Class, ...)                                                                                               \
  namespace lhy {                                                                                                      \
  FOR22EACH(CREATE_ROBBER1, CREATE_ROBBER2, Class, __VA_ARGS__)                                                        \
  template <>                                                                                                          \
  class refl_s<Class> {                                                                                                \
   public:                                                                                                             \
    constexpr static std::tuple<FOR_COMMA2_EACH(GET_KID_TYPE, Class, __VA_ARGS__)> members{                            \
        FOR_COMMA2_EACH(GET_KID, Class, __VA_ARGS__)};                                                                 \
    constexpr static std::array<std::pair<std::string, int>, COUNT_ARGS(__VA_ARGS__)> member_names{                    \
        {FOR_COMMA22num_EACH(GET_KID_NAME_AND_NUM1, GET_KID_NAME_AND_NUM2, __VA_ARGS__)}};                             \
    constexpr static std::any GetMember(Class& object, const std::string& name) {                                      \
      if (std::array<std::pair<std::string, int>, COUNT_ARGS(__VA_ARGS__)>::const_iterator result;                     \
          (result = std::ranges::find_if(member_names, [&name](const std::pair<std::string, int>& element) {           \
             return element.first == name;                                                                             \
           })) != member_names.end()) {                                                                                \
        return static_for_with_return<0, MINUS(COUNT_ARGS(__VA_ARGS__))(), IndexAddOne, std::any>(                     \
            [&result, &object]<int I>() -> std::any {                                                                  \
              if (I == result->second) {                                                                               \
                return std::reference_wrapper<std::decay_t<decltype(object.*std::get<I>(members))>>(                   \
                    object.*std::get<I>(members));                                                                     \
              }                                                                                                        \
              return {};                                                                                               \
            });                                                                                                        \
      }                                                                                                                \
      return {};                                                                                                       \
    }                                                                                                                  \
    constexpr static std::any GetMember(const Class& object, const std::string& name) {                                \
      if (std::array<std::pair<std::string, int>, COUNT_ARGS(__VA_ARGS__)>::const_iterator result;                     \
          (result = std::ranges::find_if(member_names, [&name](const std::pair<std::string, int>& element) {           \
             return element.first == name;                                                                             \
           })) != member_names.end()) {                                                                                \
        return static_for_with_return<0, MINUS(COUNT_ARGS(__VA_ARGS__))(), IndexAddOne, std::any>(                     \
            [&result, &object]<int I>() -> std::any {                                                                  \
              if (I == result->second) {                                                                               \
                return std::reference_wrapper<std::add_const_t<std::decay_t<decltype(object.*std::get<I>(members))>>>( \
                    object.*std::get<I>(members));                                                                     \
              }                                                                                                        \
              return {};                                                                                               \
            });                                                                                                        \
      }                                                                                                                \
      return {};                                                                                                       \
    }                                                                                                                  \
  };                                                                                                                   \
  }
#endif  // REFL_S_H
