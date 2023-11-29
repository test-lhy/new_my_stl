//
// Created by lhy on 23-11-27.
//

#ifndef MY_STL_CONCEPT_H
#define MY_STL_CONCEPT_H
#include <concepts>
#include <set>
#include <typeindex>
#include <typeinfo>
#include "type_traits.h"
namespace lhy{
template<typename T>
concept int_type= requires(){
  TypeTraits<T>::isInteger;
};
template<typename T>
concept not_int_type=not int_type<T>;
template <typename T>
concept char_type= requires(){
  TypeTraits<T>::isChar;
};
template <typename T>
concept not_char_type = not char_type<T>;
template<typename T>
concept hashable=requires(){
  TypeTraits<T>::hash_func;
};
}
#endif  // MY_STL_CONCEPT_H