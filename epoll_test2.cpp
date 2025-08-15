//
// Created by lhy on 24-9-26.
//

#include <sys/epoll.h>
#include <sys/ioctl.h>

#include "co.h"
#include "co_epoll.h"
lhy::Task<std::string> reader() {
  co_await lhy::wait_file(0, EPOLLIN);
  std::string s;
  while (true) {
    char c;
    ssize_t len = read(0, &c, 1);
    if (len == -1) {
      if (errno != EWOULDBLOCK) [[unlikely]] {
        throw std::system_error(errno, std::system_category());
      }
      break;
    }
    s.push_back(c);
  }
  co_return std::move(s);
}

lhy::Task<int> async_main() {
  while (true) {
    auto s = co_await reader();
    lhy::debug(), "读到了", s;
    if (s == "quit\n") break;
  }
  co_return 1;
}

int main() {
  int attr = 1;
  ioctl(0, FIONBIO, &attr);

  auto t = async_main();
  t.handle_.resume();
  while (!t.handle_.done()) {
    lhy::GetEpollLoop().TryRun(10);
  }

  return 0;
}