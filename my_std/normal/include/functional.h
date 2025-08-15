//
// Created by lhy on 24-8-13.
//

#ifndef FUNCTIONAL_H
#define FUNCTIONAL_H
#include <utility>

#include "shared_ptr.h"

namespace lhy {
// 为了让他是一个模板类
template <typename T>
class function {
  static_assert(false);
};
template <typename R, typename... Args>
class function<R(Args...)> {
 public:
  function() = default;
  template <typename F>
    requires std::is_invocable_r_v<R, F, Args...>
  function(F&& f) : impl(make_shared<functionImpl<F>>(std::forward<F>(f))) {}
  function(const function& other) = default;
  function(function&& other) = default;
  function& operator=(const function& other) = default;
  function& operator=(function&& other) = default;
  ~function() = default;
  R operator()(Args... args) const {
    /// 只能不拷贝左值，就不能处理右值了？
    return impl->operator()(std::forward<Args>(args)...);
  }
  explicit operator bool() { return impl; }

 private:
  /// 擦除类型，也就是将其统一到一个基类，什么类型不重要，只要你输入参数和返回值类型一样，那么就可以调用，实际上就是一种隐式的虚
  class functionImplBase {
   public:
    /// 里面为了完美转发可以&&
    virtual R operator()(Args&&...) = 0;
    virtual ~functionImplBase() = default;
  };
  template <typename F>
  class functionImpl : public functionImplBase {
   public:
    explicit functionImpl(F&& f) : f(std::forward<F>(f)) {}
    /// 当涉及到成员指针时，其调用非常特殊，因此必须使用std::invoke
    R operator()(Args&&... args) override { return std::invoke(f,std::forward<Args>(args)...); }
    F f;
  };
  shared_ptr<functionImplBase> impl{};
};

}  // namespace lhy

#endif //FUNCTIONAL_H
