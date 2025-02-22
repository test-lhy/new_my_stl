//
// Created by 连昊宇 on 25-2-19.
//
#include <macro.h>
#include <refl_s.h>

#include <cassert>
#include <functional>
#include <iostream>
using namespace lhy;

class base {
 public:
  int a1;
  int a2;
  // int f1();

 protected:
  int a3;
  // int f2();

 private:
  double a4{0.1};
  // int f3();
};
REFL(base, a1, a2, a3, a4)

// class node2 {
//   int a;
//   int b, c;
//   float d;
//   char e;
//   double f;
//   bool g;
//
//  public:
//   node2() {};
//   node2(int a, int b, int c, float d, char e, double f, bool g) : a(a), b(b), c(c), d(d), e(e), f(f), g(g) {}
//   ~node2() = default;
//   node2(const node2& other) = default;
//   node2(node2&& other) = default;
//   node2& operator=(const node2& other) = default;
//   node2& operator=(node2&& other) = default;
//   void h();
//   static int m;
//   static int m1() {};
//
//  protected:
//   void i();
//   int j;
//
//  private:
//   int k;
//   void l();
// };
// struct node : base {
//   using type_now = int;
//   int a;
//   int b, c;
//   float d;
//   char e;
//   double f;
//   bool g;
//   node() = default;
//   node(int a, int b, int c, float d, char e, double f, bool g) : a(a), b(b), c(c), d(d), e(e), f(f), g(g) {}
//   ~node() = default;
//   node(const node& other) = default;
//   node(node&& other) = default;
//   node& operator=(const node& other) = default;
//   node& operator=(node&& other) = default;
//   void h();
//   node2 n;
//   static int m;
//   static int m1() {};
//
//  protected:
//   void i();
//   int j;
//
//  private:
//   int k;
//   void l();
// };

void test1() {
  auto t = refl_s<base>::members;
  base a;
  assert(a.*(std::get<3>(t)) == 0.1);
}
void test2() {
  base a;
  auto& c = GetMember<int>(a, "a1");
  c = 5;
  assert(c == 5);
  c = 10;
  assert(c == 10);
  const base a1{};
  auto& c1 = GetMember<int>(a1, "a1");
  static_assert(std::is_same_v<decltype(c1), const int&>);
  // c = 5;
  // static_assert(c==5);
  // c = 10;
  // static_assert(c==10);
}
int main() {
  test1();
  test2();
  std::cout << "all test passed";
}