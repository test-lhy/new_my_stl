//
// Created by lhy on 24-9-26.
//

#ifndef SOCKET_ADDR_H
#define SOCKET_ADDR_H
#include <netdb.h>

#include <string>

#include "exception.h"
#include "help_func.h"
namespace lhy {

class SocketAddr {
 public:
  SocketAddr(std::string&& host, std::string&& port) {
    CheckLinuxError(getaddrinfo(host.c_str(), port.c_str(), nullptr, &addrinfo_));
    if (addrinfo_ == nullptr) {
      throw logic_error("getaddrinfo failed");
    }
  }
  SocketAddr(SocketAddr&& other) noexcept : addrinfo_(other.addrinfo_) { other.addrinfo_ = nullptr; }
  ~SocketAddr() { freeaddrinfo(addrinfo_); }
  void GetNextEntry() {
    if (!addrinfo_->ai_next) {
      throw logic_error("No more entries");
    }
    addrinfo_ = addrinfo_->ai_next;
  }
  int CreateSocket() {
    auto sockfd_ = CheckLinuxError(socket(addrinfo_->ai_family, addrinfo_->ai_socktype, addrinfo_->ai_protocol));
    int flag = 1;
    setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
    CheckLinuxError(bind(sockfd_, addrinfo_->ai_addr, addrinfo_->ai_addrlen));
    CheckLinuxError(listen(sockfd_, SOMAXCONN));
    return sockfd_;
  }
  void CheckListen() {}

  addrinfo* addrinfo_;
};
}  // namespace lhy
#endif  // SOCKET_ADDR_H
