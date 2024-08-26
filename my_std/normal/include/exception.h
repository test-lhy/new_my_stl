//
// Created by lhy on 24-8-18.
//

#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <iostream>
#include <string>
namespace lhy {
#define CREATE_EXCEPTION_CLASS(class_name)                                                                       \
  class class_name : public exception {                                                                          \
   public:                                                                                                       \
    using exception::exception;                                                                                  \
    class_name& operator=(const class_name& other) noexcept = default;                                           \
    [[nodiscard]] std::string what() const noexcept override { return std::string{#class_name} + ":" + what_; }; \
  };
class exception {
 public:
  exception() noexcept = default;
  explicit exception(std::string&&) noexcept;
  exception(const exception& other) noexcept = default;
  virtual ~exception() noexcept = default;
  exception& operator=(const exception& other) noexcept = default;
  [[nodiscard]] virtual std::string what() const noexcept;

 protected:
  std::string what_;
};
void Terminate() {
  /// 实际上当throw出来一个异常导致teminate的时候，c++会自动调用terminate_handler，也就是可以被set_terminate设置的一个函数来处理
  std::cout << "\n";
  if (std::exception_ptr exptr = std::current_exception()) {
    try {
      std::rethrow_exception(exptr);
    } catch (const exception& ex) {
      std::cerr << "Terminated due to an unhandled MyException: \n  what():    " << ex.what() << std::endl;
    } catch (const std::exception& ex) {
      std::cerr << "Terminated due to an unhandled Exception: \n  what():    " << ex.what() << std::endl;
    } catch (...) {
      std::cerr << "Terminated due to an unknown exception." << std::endl;
    }
  } else {
    std::cerr << "Terminated without any active exception." << std::endl;
  }
  std::abort();  // 确保程序终止
}
inline int InitTerminate = [] {
  std::set_terminate(Terminate);
  return 1;
}();
CREATE_EXCEPTION_CLASS(logic_error);
CREATE_EXCEPTION_CLASS(out_of_range);
CREATE_EXCEPTION_CLASS(invalid_argument);
CREATE_EXCEPTION_CLASS(range_error);
CREATE_EXCEPTION_CLASS(bad_optional_access);
CREATE_EXCEPTION_CLASS(bad_weak_ptr);
#undef CREATE_EXCEPTION_CLASS
}  // namespace lhy

#endif  // EXCEPTION_H
