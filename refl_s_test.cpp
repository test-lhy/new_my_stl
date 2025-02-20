//
// Created by 连昊宇 on 25-2-19.
//
#include <macro.h>
#include <refl_s.h>

#include <iostream>
using namespace lhy;
namespace lhy {
template <typename T>
class FunctionProperties {};
template <typename T, typename... Args>
class FunctionProperties<T(Args...)> {
 public:
  using RetType = T;
};
}  // namespace lhy

#define GETTYPE(Class) Class::*

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
#define f(x, y) void x##y();
#define REFL(Class, ...)                                                                    \
  namespace lhy {                                                                           \
  FOR22EACH(CREATE_ROBBER1, CREATE_ROBBER2, Class, __VA_ARGS__)                             \
  template <>                                                                               \
  class refl_s<Class> {                                                                     \
   public:                                                                                  \
    constexpr static std::tuple<FOR_COMMA2_EACH(GET_KID_TYPE, Class, __VA_ARGS__)> members{ \
        FOR_COMMA2_EACH(GET_KID, Class, __VA_ARGS__)};                                      \
  };                                                                                        \
  }
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

int main() {
  auto t = refl_s<base>::members;
  base a;
  std::cout << a.*(std::get<3>(t)) << '\n';
  std::cout <<1;

}