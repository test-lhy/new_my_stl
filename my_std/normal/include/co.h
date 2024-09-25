//
// Created by lhy on 24-9-17.
//

#ifndef CO_H
#define CO_H
#include <condition_variable>
#include <coroutine>
#include <mutex>
#include <variant>

#include "functional.h"
#include "meta.h"
#include "optional.h"
#include "priority_queue.h"
#include "queue.h"
namespace lhy {
template <typename T>
struct PromiseType;
template <typename T, typename promise_type_ = PromiseType<T>>
class Task;
class Loop {
  class TaskWaiter {
   public:
    std::chrono::system_clock::time_point time_point_;
    std::coroutine_handle<> handle_;
    bool operator<(const TaskWaiter& other) const { return time_point_ < other.time_point_; }
  };
  // deque
  queue<std::coroutine_handle<>> ready_queue_;
  priority_queue<TaskWaiter> time_queue_;

 public:
  template <typename T>
  void AddTask(const Task<T>& task) {
    ready_queue_.push(task.handle_);
  }
  void AddTask(std::coroutine_handle<>& handle) { ready_queue_.push(handle); }
  void AddTask(std::chrono::system_clock::time_point time_point, std::coroutine_handle<> handle) {
    time_queue_.push(TaskWaiter{time_point, handle});
  }
  void RunAll() {
    while (!(ready_queue_.empty() && time_queue_.empty())) {
      while (!ready_queue_.empty()) {
        auto task = ready_queue_.front();
        ready_queue_.pop();
        if (task) {
          task.resume();
        }
      }
      while (!time_queue_.empty() && time_queue_.top().time_point_ <= std::chrono::system_clock::now()) {
        ready_queue_.push(time_queue_.top().handle_);
        time_queue_.pop();
      }
    }
  }
};
inline auto& GetLoop() {
  static Loop loop;
  return loop;
}
class SleepAwaiter {
 public:
  explicit SleepAwaiter(std::chrono::system_clock::time_point time_point) : time_point_(time_point) {}
  [[nodiscard]] bool await_ready() const noexcept { return false; }
  void await_suspend(std::coroutine_handle<> previous_handle) const noexcept {
    GetLoop().AddTask(time_point_, previous_handle);
  }
  void await_resume() const noexcept {}
  std::chrono::system_clock::time_point time_point_;
};
inline SleepAwaiter sleep_for(std::chrono::system_clock::duration time) {
  return SleepAwaiter(std::chrono::system_clock::now() + time);
}
inline SleepAwaiter sleep_until(std::chrono::system_clock::time_point time_point) { return SleepAwaiter(time_point); }
template <typename T>
Task<T> WhenAllAwaiterHelper(Task<T>& task, auto& counter, auto& ret, auto previous_coroutine_handle) {
  ret = co_await task;
  --counter;
  if (counter == 0) {
    GetLoop().AddTask(previous_coroutine_handle);
  }
  co_return std::move(ret);
}
template <size_t Index, size_t MAX_, typename T>
Task<T> WhenAnyAwaiterHelper(Task<T>& task, auto& ret, auto counter, auto previous_coroutine_handle) {
  auto ret_ = co_await task;
  if (*counter == MAX_) {
    --(*counter);
    std::get<Index>(ret) = ret_;
    GetLoop().AddTask(previous_coroutine_handle);
  }
  co_return ret_;
}
template <typename... Args>
class WhenAllAwaiter {
 public:
  explicit WhenAllAwaiter(Task<Args>&&... args) : counter_(sizeof...(Args)), args_(std::forward<Task<Args>>(args)...) {}
  [[nodiscard]] bool await_ready() noexcept { return false; }
  template <size_t Index>
  void await_more_one(auto previous_coroutine_handle) noexcept {
    std::get<Index>(args__) =
        WhenAllAwaiterHelper(std::get<Index>(args_), counter_, std::get<Index>(rets_), previous_coroutine_handle);
    std::get<Index>(args__).handle_.resume();
  }
  template <size_t... Index>
  void await_more(IndexSequence<Index...>, auto previous_coroutine_handle) noexcept {
    (await_more_one<Index>(previous_coroutine_handle), ...);
  }
  void await_suspend(std::coroutine_handle<> previous_handle) noexcept {
    await_more(IndexSequenceFor<Args...>(), previous_handle);
  }
  std::tuple<Args...> await_resume() noexcept { return std::move(rets_); }
  int counter_{};
  std::tuple<Args...> rets_;
  std::tuple<Task<Args>...> args_;
  std::tuple<Task<Args>...> args__;
};
template <typename... Args>
class WhenAnyAwaiter {
 public:
  explicit WhenAnyAwaiter(Task<Args>&&... args)
      : counter_(std::make_shared<int>(sizeof...(Args))), args_(std::forward<Task<Args>>(args)...) {}
  [[nodiscard]] bool await_ready() noexcept { return false; }
  template <size_t Index>
  void await_more_one(auto previous_coroutine_handle) noexcept {
    debug(), Index;
    std::get<Index>(args__) = WhenAnyAwaiterHelper<Index, sizeof...(Args)>(std::get<Index>(args_), rets_, counter_,
                                                                           previous_coroutine_handle);
    std::get<Index>(args__).handle_.resume();
  }
  template <size_t... Index>
  void await_more(IndexSequence<Index...>, auto previous_coroutine_handle) noexcept {
    (await_more_one<Index>(previous_coroutine_handle), ...);
  }
  void await_suspend(std::coroutine_handle<> previous_handle) noexcept {
    await_more(IndexSequenceFor<Args...>(), previous_handle);
  }
  ~WhenAnyAwaiter() {
    std::apply([](auto&&... args) { ((args.handle_ = {}), ...); }, args__);
    std::apply([](auto&&... args) { ((args.handle_ = {}), ...); }, args_);
  }
  std::variant<Args...> await_resume() noexcept { return std::move(rets_); }
  std::shared_ptr<int> counter_;
  std::variant<Args...> rets_;
  std::tuple<Task<Args>...> args_;
  std::tuple<Task<Args>...> args__;
};
template <typename... Args>
Task<std::tuple<Args...>> when_all(Task<Args>&&... args) {
  std::tuple<Args...> ret = co_await WhenAllAwaiter(std::forward<Task<Args>>(args)...);
  co_return ret;
}
template <typename... Args>
Task<std::variant<Args...>> when_any(Task<Args>&&... args) {
  std::variant<Args...> ret = co_await WhenAnyAwaiter(std::forward<Task<Args>>(args)...);
  co_return ret;
}
class suspend_always {
 public:
  [[nodiscard]] bool await_ready() const noexcept { return false; }
  void await_suspend(std::coroutine_handle<>) const noexcept {}
  void await_resume() const noexcept {}
};
class suspend_never {
 public:
  [[nodiscard]] bool await_ready() const noexcept { return true; }
  void await_suspend(std::coroutine_handle<>) const noexcept {}
  void await_resume() const noexcept {}
};
class PreviousWaiter {
 public:
  PreviousWaiter(std::coroutine_handle<> previous_handle) : previous_handle_(previous_handle) {}
  [[nodiscard]] bool await_ready() const noexcept { return false; }
  std::coroutine_handle<> await_suspend(std::coroutine_handle<>) const noexcept {
    if (previous_handle_) {
      return previous_handle_;
    }
    return std::noop_coroutine();
  }
  void await_resume() const noexcept {}
  std::coroutine_handle<> previous_handle_;
};
template <typename T>
struct PromiseType {
  auto get_return_object() { return Task<T>{std::coroutine_handle<PromiseType>::from_promise(*this)}; }
  suspend_always initial_suspend() { return {}; }
  PreviousWaiter final_suspend() noexcept { return {previous_handle_}; }
  template <typename U>
  typename Task<U>::Awaiter await_transform(const Task<U>& other) {
    return typename Task<U>::Awaiter{other.handle_};
  }
  auto& await_transform(const SleepAwaiter& other) { return other; }
  template <typename... Args>
  auto&& await_transform(WhenAllAwaiter<Args...>&& other) {
    return other;
  }
  template <typename... Args>
  auto&& await_transform(WhenAnyAwaiter<Args...>&& other) {
    return other;
  }
  void unhandled_exception() {}
  PreviousWaiter yield_value(T&& value) {
    value_ = std::move(value);
    return {previous_handle_};
  }
  void return_value(T&& value) { value_ = std::move(value); }
  std::variant<T, std::exception_ptr>& get_value() { return *value_; }
  std::optional<std::variant<T, std::exception_ptr>> value_;
  std::coroutine_handle<> previous_handle_;
  T& getorthrow() {
    auto ret = std::get_if<0>(&get_value());
    if (!ret) {
      std::rethrow_exception(std::get<1>(get_value()));
    }
    return *ret;
  }
};
template <typename T, typename promise_type_>
class Task {
 public:
  using promise_type = promise_type_;
  class Awaiter {
   public:
    explicit Awaiter(std::coroutine_handle<promise_type> handle) : handle_(handle) {}
    [[nodiscard]] bool await_ready() noexcept { return false; }
    void await_suspend(std::coroutine_handle<> previous_handle) noexcept {
      handle_.promise().previous_handle_ = previous_handle;
      if (!handle_.done()) {
        // 因为把resume放到了waiter里面，所以不能用co之类的关键字
        handle_.resume();
      }
    }
    T& await_resume() noexcept { return handle_.promise().getorthrow(); }
    std::coroutine_handle<promise_type> handle_;
  };

  Task() = default;
  explicit Task(const std::coroutine_handle<promise_type> handle) : handle_(handle) {}
  ~Task() {
    if (handle_) {
      handle_.destroy();
    }
  }
  Task(const Task& other) = delete;
  Task(Task&& other) noexcept : handle_(std::exchange(other.handle_, {})) {}
  Task& operator=(const Task& other) = delete;
  Task& operator=(Task&& other) noexcept {
    handle_ = std::exchange(other.handle_, {});
    return *this;
  }
  T& get_value() { return handle_.promise().getorthrow(); }
  std::coroutine_handle<promise_type> handle_;
};
}  // namespace lhy

#endif  // CO_H
