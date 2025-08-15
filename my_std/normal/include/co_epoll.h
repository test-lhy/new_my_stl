//
// Created by lhy on 24-9-26.
//

#ifndef CO_EPOLL_H
#define CO_EPOLL_H
#include <sys/epoll.h>

#include "co.h"

namespace lhy {
struct EpollFilePromise : PromiseType<int> {
  auto get_return_object() {
    return Task<int, EpollFilePromise>{std::coroutine_handle<EpollFilePromise>::from_promise(*this)};
  }
  int FileNo;
  int events;
};
class EpollLoop {
 public:
  void AddListener(EpollFilePromise* promise) const {
    /// 设置监听的时候会保存该promise，使得在读取后能回到该协程
    epoll_event event{};
    event.data.ptr = promise;
    /// 处理什么样的事件
    event.events = promise->events;
    epoll_ctl(epfd, EPOLL_CTL_ADD, promise->FileNo, &event);
  }
  void TryRun(int ms) {
    epoll_event events[10];
    debug(), "开始监听";
    int nfds = epoll_wait(epfd, events, 10, ms);
    for (int i = 0; i < nfds; i++) {
      auto promise = static_cast<EpollFilePromise*>(events[i].data.ptr);
      std::coroutine_handle<EpollFilePromise>::from_promise(*promise).resume();
      epoll_ctl(epfd, EPOLL_CTL_DEL, promise->FileNo, nullptr);
    }
    debug(), "结束监听";
  }
  ~EpollLoop() { close(epfd); }
  int epfd = epoll_create1(0);
};
inline EpollLoop& GetEpollLoop() {
  static EpollLoop epoll_loop;
  return epoll_loop;
}
class EpollFileAwaiter {
 public:
  explicit EpollFileAwaiter(int fileNo, int events) : FileNo(fileNo), events(events) {}
  [[nodiscard]] bool await_ready() noexcept { return false; }
  void await_suspend(std::coroutine_handle<EpollFilePromise> previous_handle) noexcept {
    previous_handle.promise().FileNo = FileNo;
    previous_handle.promise().events = events;
    GetEpollLoop().AddListener(&previous_handle.promise());
  }
  void await_resume() noexcept {}
  int FileNo;
  int events;
};

inline Task<int, EpollFilePromise> wait_file(int fileNo, uint32_t events) {
  co_await EpollFileAwaiter(fileNo, events);
  co_return 1;
}
}  // namespace lhy
#endif  // CO_EPOLL_H
