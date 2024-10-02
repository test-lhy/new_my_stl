//
// Created by lhy on 24-9-26.
//

#ifndef SOCKET_H
#define SOCKET_H
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

#include <string>
#include <thread>

#include "help_func.h"
#include "http.h"
#include "socket_addr.h"
namespace lhy {

class Socket {
 public:
  Socket(std::string&& host, std::string&& port, std::string&& base_path, SocketType type = SocketType::TCP)
      : socket_addr_(std::move(host), std::move(port)) {
    GetHtmlSolver().base_path = base_path;
    sockfd_ = socket_addr_.CreateSocket(type);
    while (true) {
      if (type == SocketType::UDP) {
        char buf[1024];
        sockaddr_storage client_addr{};
        socklen_t addr_len = sizeof(client_addr);
        std::string req;
        auto n = CheckLinuxError(recvfrom(sockfd_, buf, sizeof(buf), 0, (sockaddr*)&client_addr, &addr_len));
        req = std::string(buf, n);
        /// 解析一下html
        auto response = HtmlSolver(req);
        if (!response.starts_with("HTTP/1.1")) {
          response =
              "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " + std::to_string(response.size()) +
              "\r\n\r\n" + response;
        }
        CheckLinuxError(sendto(sockfd_, response.c_str(), response.size(), 0, (sockaddr*)&client_addr, addr_len));
      } else {
        const auto client_fd =
            CheckLinuxError(accept(sockfd_, socket_addr_.addrinfo_->ai_addr, &socket_addr_.addrinfo_->ai_addrlen));
        threads_.emplace_back([=]() {
          char buf[1024];
          std::string req;
          auto n = CheckLinuxError(read(client_fd, buf, sizeof(buf)));
          req = std::string(buf, n);
          /// 解析一下html
          auto response = HtmlSolver(req);
          if (!response.starts_with("HTTP/1.1")) {
            response =
                "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " + std::to_string(response.size()) +
                "\r\n\r\n" + response;
          }
          CheckLinuxError(write(client_fd, response.c_str(), response.size()));
          CheckLinuxError(close(client_fd));
        });
      }
    }
  }

 private:
  int sockfd_;
  SocketAddr socket_addr_;
  std::vector<std::jthread> threads_{};
};
}  // namespace lhy
#endif  // SOCKET_H
