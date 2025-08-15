//
// Created by lhy on 24-8-11.
//
#include "tuple.h"

#include <cassert>
#include <iostream>
#include <tuple>

#include "util.h"
using namespace lhy;
template <lhy::size_t... I>
void fun(IndexSequence<I...>) {}

void test_basic_construction() {
  lhy::tuple<int, double, char> t1;
  lhy::tuple<int, double, char> t2(42, 3.14, 'a');
  assert(lhy::get<0>(t2) == 42);
  assert(lhy::get<1>(t2) == 3.14);
  assert(lhy::get<2>(t2) == 'a');

  lhy::tuple<int, double, char> t3(42, 3.14, 'a');
  lhy::tuple<int, double, char> t4(t3);  // Copy constructor
  assert(lhy::get<0>(t4) == 42);
  assert(lhy::get<1>(t4) == 3.14);
  assert(lhy::get<2>(t4) == 'a');
}

void test_element_access() {
  lhy::tuple<int, double, char> t(42, 3.14, 'a');
  assert(lhy::get<0>(t) == 42);
  assert(lhy::get<1>(t) == 3.14);
  assert(lhy::get<2>(t) == 'a');

  lhy::get<0>(t) = 100;
  assert(lhy::get<0>(t) == 100);
}

void test_copy_and_move_semantics() {
  lhy::tuple<int, double, char> t1(42, 3.14, 'a');
  lhy::tuple<int, double, char> t2(t1);  // Copy constructor
  assert(lhy::get<0>(t2) == 42);
  assert(lhy::get<1>(t2) == 3.14);
  assert(lhy::get<2>(t2) == 'a');

  lhy::tuple<int, double, char> t3(std::move(t2));  // Move constructor
  assert(lhy::get<0>(t3) == 42);
  assert(lhy::get<1>(t3) == 3.14);
  assert(lhy::get<2>(t3) == 'a');
}

void test_comparison_operators() {
  lhy::tuple<int, double, char> t1(42, 3.14, 'a');
  lhy::tuple<int, double, char> t2(42, 3.14, 'a');
  lhy::tuple<int, double, char> t3(43, 2.71, 'b');

  assert(t1 == t2);
  assert(t1 != t3);
  assert(t1 < t3);
  assert(t3 > t2);
  assert(t1 <= t2);
  assert(t3 >= t2);
}

void test_tuple_utilities() {
  auto t1 = lhy::make_tuple(42, 3.14, 'a');
  assert(lhy::get<0>(t1) == 42);
  assert(lhy::get<1>(t1) == 3.14);
  assert(lhy::get<2>(t1) == 'a');

  int i = 10;
  double d = 2.5;
  auto t2 = lhy::tie(i, d);
  i = 20;
  assert(lhy::get<0>(t2) == 20);
  assert(lhy::get<1>(t2) == 2.5);

  auto t3 = lhy::forward_as_tuple(100, 200);
  assert(lhy::get<0>(t3) == 100);
  assert(lhy::get<1>(t3) == 200);
  // 右值tuple也太红瘟了
  // auto t4 = lhy::tuple_cat(t1, t3);
  // assert(lhy::get<0>(t4) == 42);
  // assert(lhy::get<1>(t4) == 3.14);
  // assert(lhy::get<2>(t4) == 'a');
  // assert(lhy::get<3>(t4) == 100);
  // assert(lhy::get<4>(t4) == 200);
}

void test_edge_cases() {
  lhy::tuple<> empty;
  static_assert(lhy::tuple_size_v<decltype(empty)> == 0, "Size of empty tuple should be 0");

  int x = 10;
  lhy::tuple<int&> ref_tuple(x);
  x = 20;
  assert(lhy::get<0>(ref_tuple) == 20);
}
int main() {
  tuple<int, int, string> a{1, 2, "string111111111111"};
  tuple<int, int, string> a1 = std::move(a);
  std::cout << tuple_size<decltype(a)>() << std::endl;
  std::cout << tuple_size_v<decltype(a)> << std::endl;
  std::cout << get_type<tuple_element<2, decltype(a)>::type>();
  std::cout << get<2>(a);
  IndexSequenceFor<int, int, double> a3;
  std::cout << get_type<CreateIndexSequence<3>::type>();
  // std::cout << get_type<decltype(a3)::typeIndex>();
  IndexSequence<0, 1, 2> a4(a3);
  fun(a3);
  // static_cast<IndexSequence<0, 1, 2>>(a3);
  tuple<int, int, double> a2{1, 2, 3};
  apply(a2, [](auto&& args) { args++; });
  apply(a2, [](auto&& args) { std::cout << args << std::endl; });
  std::cout << get_type<tuple_cat_type<tuple<int, int, double>, tuple<int, double>>::type>();
  auto b = tuple_cat(a1, a2);
  apply(b, [](auto&& args) { std::cout << args << ' '; });
  test_basic_construction();
  test_element_access();
  test_copy_and_move_semantics();
  test_comparison_operators();
  test_tuple_utilities();
  test_edge_cases();

  std::cout << "All tests passed!" << std::endl;
}