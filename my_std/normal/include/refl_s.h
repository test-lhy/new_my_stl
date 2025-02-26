//
// Created by 连昊宇 on 25-2-19.
//

#ifndef REFL_S_H
#define REFL_S_H
#include <meta.h>

#include <any>
#include <ranges>
#include <string>
namespace lhy {
enum class AccessSpecifier { PUBLIC, PROTECTED, PRIVATE };
const std::unordered_map<AccessSpecifier, std::string> access_specifier_map = {
    {AccessSpecifier::PUBLIC, "public"},
    {AccessSpecifier::PROTECTED, "protected"},
    {AccessSpecifier::PRIVATE, "private"}};
template <typename>
class refl_s {};
template <typename T, typename U>
constexpr T& GetMember(U& object, const std::string& name) {
  return std::any_cast<std::reference_wrapper<T>>(refl_s<U>::GetMember(object, name)).get();
}
template <typename T, typename U>
constexpr const T& GetMember(const U& object, const std::string& name) {
  return std::any_cast<std::reference_wrapper<const T>>(refl_s<U>::GetMember(object, name)).get();
}
template <typename T>
constexpr int GetMemberIndex(const std::string& name) {
  return refl_s<T>::GetMemberIndex(name);
}
template <typename T>
constexpr int GetMemberIsStatic(const std::string& name) {
  return refl_s<T>::GetMemberIsStatic(name);
}

template <typename T, typename U>
constexpr T& GetStaticMember(const std::string& name) {
  return std::any_cast<std::reference_wrapper<T>>(refl_s<U>::GetStaticMember(name)).get();
}
template <typename T>
constexpr AccessSpecifier GetMemberAccessSpecifier(const std::string& name) {
  return refl_s<T>::GetMemberAccessSpecifier(name);
}
template <typename T>
constexpr std::string GetMemberAccessSpecifierInString(const std::string& name) {
  return access_specifier_map.at(GetMemberAccessSpecifier<T>(name));
}
template <typename>
class AccessSpecifierHelper {};
template <typename>
class AccessSpecifierGetter {};
}  // namespace lhy
// static对象必须是已经定义的了,不管是类内inline还是类外define
#define CONCEPT_OF_KID_ACCESS(Class, KidName) \
  template <typename T>                       \
  concept kid_access_##Class##_##KidName = requires() { T::KidName; };
#define CREATE_SPECIFIC_ACCESS_SPECIFIER_GETTER_HELPER(Class, KidName) \
  template <typename T>                                                \
  class AccessSpecifierHelperHelper##Class##KidName : public T {       \
   public:                                                             \
    template <typename = int>                                          \
    constexpr static bool Check_##KidName() {                          \
      return false;                                                    \
    }                                                                  \
    template <typename = int>                                          \
      requires requires() { T::KidName; }                              \
    constexpr static bool Check_##KidName() {                          \
      return true;                                                     \
    }                                                                  \
  };

#define CREATE_SPECIFIC_ACCESS_SPECIFIER_GETTER(Class, KidName) \
  constexpr static bool is_protected_##KidName = AccessSpecifierHelperHelper##Class##KidName<Class>::Check_##KidName();
#define CREATE_ACCESS_SPECIFIER_GETTER(Class, ...)                        \
  template <>                                                             \
  class AccessSpecifierHelper<Class> {                                    \
   public:                                                                \
    FOR2EACH(CREATE_SPECIFIC_ACCESS_SPECIFIER_GETTER, Class, __VA_ARGS__) \
  };
#define CREATE_SPECIFIC_ACCESS_SPECIFIER(Class, KidName)                                       \
  static constexpr auto access_specifier_##KidName =                                           \
      lhy::detail::kid_access_##Class##_##KidName<Class>                                       \
          ? AccessSpecifier::PUBLIC                                                            \
          : (AccessSpecifierHelper<Class>::is_protected_##KidName ? AccessSpecifier::PROTECTED \
                                                                  : AccessSpecifier::PRIVATE);
#define CREATE_ACCESS_SPECIFIER(Class, ...)                        \
  template <>                                                      \
  class AccessSpecifierGetter<Class> {                             \
   public:                                                         \
    FOR2EACH(CREATE_SPECIFIC_ACCESS_SPECIFIER, Class, __VA_ARGS__) \
  };
#define CREATE_ROBBER_ANOTHER1(Class, KidName) template <typename T
#define CREATE_ROBBER_ANOTHER2(Class, KidName)                                                                         \
  T* M > class Class##KidName##RetType<M> {                                                                            \
   public:                                                                                                             \
    using RealType = T;                                                                                                \
    using PointerType = T*;                                                                                            \
    friend constexpr auto GetPtr##Class##KidName##Type() { return RealType{}; };                                       \
    friend constexpr auto GetPtr##Class##KidName##PointerType() { return PointerType{}; };                             \
    constexpr static bool is_static = true;                                                                            \
    friend constexpr auto Get##Class##KidName##IsStatic() { return is_static; };                                       \
  };                                                                                                                   \
  template class Class##KidName##RetType<&Class::KidName>;                                                             \
  using Ret##Class##KidName##Type = FunctionProperties<decltype(GetPtr##Class##KidName##Type)>::RetType;               \
  using Ret##Class##KidName##PointerType = FunctionProperties<decltype(GetPtr##Class##KidName##PointerType)>::RetType; \
  constexpr bool Class##KidName##_is_static = Get##Class##KidName##IsStatic();                                         \
  constexpr Ret##Class##KidName##PointerType GetPtr##Class##KidName();                                                 \
  template <Ret##Class##KidName##PointerType M>                                                                        \
  struct Rob##Class##KidName {                                                                                         \
    friend constexpr Ret##Class##KidName##PointerType GetPtr##Class##KidName() { return M; }                           \
  };                                                                                                                   \
  template struct Rob##Class##KidName<&Class::KidName>;
#define CREATE_ROBBER1(Class, KidName)                  \
  template <auto T>                                     \
  class Class##KidName##RetType {};                     \
  constexpr auto GetPtr##Class##KidName##Type();        \
  constexpr auto GetPtr##Class##KidName##PointerType(); \
  constexpr auto Get##Class##KidName##IsStatic();       \
  template <typename T
#define CREATE_ROBBER2(Class, KidName)                                                     \
  T Class::* M > class Class##KidName##RetType<M> {                                        \
   public:                                                                                 \
    using RealType = T;                                                                    \
    using PointerType = T Class::*;                                                        \
    friend constexpr auto GetPtr##Class##KidName##Type() { return RealType{}; };           \
    friend constexpr auto GetPtr##Class##KidName##PointerType() { return PointerType{}; }; \
    constexpr static bool is_static = false;                                               \
    friend constexpr auto Get##Class##KidName##IsStatic() { return is_static; };           \
  };
#define GET_KID(Class, KidName) GetPtr##Class##KidName()
#define GET_KID_TYPE(Class, KidName) decltype(GET_KID(Class, KidName))
#define GET_KID_NAME_AND_NUM1(KidName, num) \
  {                                         \
    #KidName
#define GET_KID_NAME_AND_NUM2(KidName, num) \
  num                                       \
  }
#define GET_KID_STATIC(Class, KidName) Class##KidName##_is_static
#define GET_KID_ACCESS_SPECIFIER(Class, KidName) AccessSpecifierGetter<Class>::access_specifier_##KidName
#define REFL(Class, ...)                                                                                               \
  namespace lhy {                                                                                                      \
  namespace detail {                                                                                                   \
  FOR2EACH(CONCEPT_OF_KID_ACCESS, Class, __VA_ARGS__)                                                                  \
  }                                                                                                                    \
  FOR2EACH(CREATE_SPECIFIC_ACCESS_SPECIFIER_GETTER_HELPER, Class, __VA_ARGS__)                                         \
  CREATE_ACCESS_SPECIFIER_GETTER(Class, __VA_ARGS__)                                                                   \
  CREATE_ACCESS_SPECIFIER(Class, __VA_ARGS__)                                                                          \
  FOR22EACH(CREATE_ROBBER1, CREATE_ROBBER2, Class, __VA_ARGS__)                                                        \
  FOR22EACH(CREATE_ROBBER_ANOTHER1, CREATE_ROBBER_ANOTHER2, Class, __VA_ARGS__)                                        \
  template <>                                                                                                          \
  class refl_s<Class> {                                                                                                \
   public:                                                                                                             \
    constexpr static std::tuple<FOR_COMMA2_EACH(GET_KID_TYPE, Class, __VA_ARGS__)> members{                            \
        FOR_COMMA2_EACH(GET_KID, Class, __VA_ARGS__)};                                                                 \
    constexpr static std::array<std::pair<std::string, int>, COUNT_ARGS(__VA_ARGS__)> member_names{                    \
        {FOR_COMMA22num_EACH(GET_KID_NAME_AND_NUM1, GET_KID_NAME_AND_NUM2, __VA_ARGS__)}};                             \
    constexpr static std::array<bool, COUNT_ARGS(__VA_ARGS__)> member_static{                                          \
        FOR_COMMA2_EACH(GET_KID_STATIC, Class, __VA_ARGS__)};                                                          \
    constexpr static std::array<AccessSpecifier, COUNT_ARGS(__VA_ARGS__)> member_access_specifier{                     \
        FOR_COMMA2_EACH(GET_KID_ACCESS_SPECIFIER, Class, __VA_ARGS__)};                                                \
    template <int Index>                                                                                               \
    constexpr static decltype(auto) GetStaticMemberReal() {                                                            \
      if constexpr (std::get<Index>(member_static)) {                                                                  \
        return *std::get<Index>(members);                                                                              \
      } else {                                                                                                         \
        throw std::runtime_error("member isn't static");                                                               \
        static Class a;                                                                                                \
        return a.*std::get<Index>(members);                                                                            \
      }                                                                                                                \
    }                                                                                                                  \
    template <int Index>                                                                                               \
    constexpr static decltype(auto) GetMemberReal(Class& object) {                                                     \
      if constexpr (std::get<Index>(member_static)) {                                                                  \
        return *std::get<Index>(members);                                                                              \
      } else {                                                                                                         \
        return object.*std::get<Index>(members);                                                                       \
      }                                                                                                                \
    }                                                                                                                  \
    template <int Index>                                                                                               \
    constexpr static decltype(auto) GetMemberReal(const Class& object) {                                               \
      if constexpr (std::get<Index>(member_static)) {                                                                  \
        return *std::get<Index>(members);                                                                              \
      } else {                                                                                                         \
        return object.*std::get<Index>(members);                                                                       \
      }                                                                                                                \
    }                                                                                                                  \
    constexpr static bool GetMemberIsStatic(const std::string& name) {                                                 \
      const int member_index = GetMemberIndex(name);                                                                   \
      return *static_for_with_return<0, MINUS(COUNT_ARGS(__VA_ARGS__))(), IndexAddOne, std::optional<bool>>(           \
          [&member_index]<int I>() -> std::optional<bool> {                                                            \
            if (I == member_index) {                                                                                   \
              return std::get<I>(member_static);                                                                       \
            }                                                                                                          \
            return {};                                                                                                 \
          });                                                                                                          \
    }                                                                                                                  \
    constexpr static int GetMemberIndex(const std::string& name) {                                                     \
      const std::array<std::pair<std::string, int>, COUNT_ARGS(__VA_ARGS__)>::const_iterator result =                  \
          std::ranges::find_if(member_names,                                                                           \
                               [&name](const std::pair<std::string, int>& element) { return element.first == name; }); \
      if (result == member_names.end()) {                                                                              \
        throw std::runtime_error("member not found");                                                                  \
      }                                                                                                                \
      return result->second;                                                                                           \
    }                                                                                                                  \
    constexpr static std::any GetStaticMember(const std::string& name) {                                               \
      const int member_index = GetMemberIndex(name);                                                                   \
      if (member_static[member_index] == 0) {                                                                          \
        throw std::runtime_error("member isn't static");                                                               \
      }                                                                                                                \
      return static_for_with_return<0, MINUS(COUNT_ARGS(__VA_ARGS__))(), IndexAddOne, std::any>(                       \
          [&member_index]<int I>() -> std::any {                                                                       \
            if (I == member_index) {                                                                                   \
              return std::reference_wrapper<std::decay_t<decltype(GetStaticMemberReal<I>())>>(                         \
                  GetStaticMemberReal<I>());                                                                           \
            }                                                                                                          \
            return {};                                                                                                 \
          });                                                                                                          \
    }                                                                                                                  \
    constexpr static std::any GetMember(Class& object, const std::string& name) {                                      \
      const int member_index = GetMemberIndex(name);                                                                   \
      return static_for_with_return<0, MINUS(COUNT_ARGS(__VA_ARGS__))(), IndexAddOne, std::any>(                       \
          [&member_index, &object]<int I>() -> std::any {                                                              \
            if (I == member_index) {                                                                                   \
              return std::reference_wrapper<std::decay_t<decltype(GetMemberReal<I>(object))>>(                         \
                  GetMemberReal<I>(object));                                                                           \
            }                                                                                                          \
            return {};                                                                                                 \
          });                                                                                                          \
    }                                                                                                                  \
    constexpr static std::any GetMember(const Class& object, const std::string& name) {                                \
      const int member_index = GetMemberIndex(name);                                                                   \
      return static_for_with_return<0, MINUS(COUNT_ARGS(__VA_ARGS__))(), IndexAddOne, std::any>(                       \
          [&member_index, &object]<int I>() -> std::any {                                                              \
            if (I == member_index) {                                                                                   \
              return std::reference_wrapper<std::add_const_t<std::decay_t<decltype(GetMemberReal<I>(object))>>>(       \
                  GetMemberReal<I>(object));                                                                           \
            }                                                                                                          \
            return {};                                                                                                 \
          });                                                                                                          \
    }                                                                                                                  \
    constexpr static AccessSpecifier GetMemberAccessSpecifier(const std::string& name) {                               \
      const int member_index = GetMemberIndex(name);                                                                   \
      return *static_for_with_return<0, MINUS(COUNT_ARGS(__VA_ARGS__))(), IndexAddOne,                                 \
                                     std::optional<AccessSpecifier>>(                                                  \
          [&member_index]<int I>() -> std::optional<AccessSpecifier> {                                                 \
            if (I == member_index) {                                                                                   \
              return std::get<I>(member_access_specifier);                                                             \
            }                                                                                                          \
            return {};                                                                                                 \
          });                                                                                                          \
    }                                                                                                                  \
  };                                                                                                                   \
  }

#endif  // REFL_S_H
