//
// Created by lhy on 24-9-21.
//
#include <netdb.h>

#include <cstring>
#include <filesystem>
#include <fstream>
#include <map>
#include <thread>

#include "basic.h"
#include "database.h"
#include "strings_.h"
std::string base_path = "../http";
std::string now_page = "/index.html";
inline char hexCharToDec(char c) {
  if (c >= '0' && c <= '9') return c - '0';
  if (c >= 'A' && c <= 'F') return c - 'A' + 10;
  if (c >= 'a' && c <= 'f') return c - 'a' + 10;
  return -1;  // Invalid input
}

// URL解码函数
std::string urlDecode(const std::string& url) {
  std::ostringstream result;
  for (size_t i = 0; i < url.length(); ++i) {
    char ch = url[i];
    if (ch == '%') {
      if (i + 2 < url.length()) {
        // 读取接下来的两个字符
        char hex1 = url[i + 1];
        char hex2 = url[i + 2];

        // 将这两个字符转换为对应的数字并还原成字符
        char decodedChar = hexCharToDec(hex1) * 16 + hexCharToDec(hex2);
        result << decodedChar;

        // 跳过处理过的两个字符
        i += 2;
      }
    } else if (ch == '+') {
      // '+' 在 URL 编码中表示空格
      result << ' ';
    } else {
      // 普通字符直接添加
      result << ch;
    }
  }
  return result.str();
}
int CheckLinuxError(int err) {
  if (err == -1) {
    lhy::debug(), "error:", strerror(errno);
  }
  return err;
}
std::string ReadHtml(const std::string& path) {
  /// 读文件
  std::ifstream t(path);
  return {std::istreambuf_iterator<char>(t), std::istreambuf_iterator<char>()};
}
class SocketAddr {
 public:
  SocketAddr(std::string&& host, std::string&& port) {
    CheckLinuxError(getaddrinfo(host.c_str(), port.c_str(), nullptr, &addrinfo_));
    if (addrinfo_ == nullptr) {
      throw lhy::logic_error("getaddrinfo failed");
    }
  }
  SocketAddr(SocketAddr&& other) noexcept : addrinfo_(other.addrinfo_) { other.addrinfo_ = nullptr; }
  ~SocketAddr() { freeaddrinfo(addrinfo_); }
  void GetNextEntry() {
    if (!addrinfo_->ai_next) {
      throw lhy::logic_error("No more entries");
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
class HtmlSolver_ {
 public:
  std::map<std::string, std::string> parseQuery(const std::string& query) {
    std::map<std::string, std::string> params;
    std::istringstream stream(query);
    std::string keyValue;

    while (std::getline(stream, keyValue, '&')) {
      auto pos = keyValue.find('=');
      if (pos != std::string::npos) {
        std::string key = urlDecode(keyValue.substr(0, pos));
        std::string value = urlDecode(keyValue.substr(pos + 1));
        params[key] = value;
      }
    }
    return params;
  }
  std::map<std::string, std::string> parseMultipart(const std::string& body) {
    std::map<std::string, std::string> formData;
    std::string delimiter = "Content-Disposition: form-data; name=\"";
    size_t start = 0;

    while ((start = body.find(delimiter, start)) != std::string::npos) {
      start += delimiter.length();
      size_t nameEnd = body.find("\"", start);
      if (nameEnd == std::string::npos) break;

      std::string name = body.substr(start, nameEnd - start);
      start = nameEnd + 5;  // Skip past the closing quote and CRLF

      size_t contentEnd = body.find("\r\n", start);
      if (contentEnd == std::string::npos) break;

      std::string content = body.substr(start, contentEnd - start);
      formData[urlDecode(name)] = urlDecode(content);

      start = contentEnd + 2;  // Move past the CRLF to the next part
    }

    return formData;
  }

  std::string operator()(const std::string& req) {
    std::string method;
    if (req.starts_with("GET ")) {
      method = "GET";
    } else if (req.starts_with("POST ")) {
      method = "POST";
    } else {
      return "Unsupported request method.";
    }

    std::string path;
    std::map<std::string, std::string> params;

    if (method == "GET") {
      path = req.substr(req.find("GET /") + 5, req.find(" HTTP/1.1") - 5);
      auto queryStart = path.find('?');
      if (queryStart != std::string::npos) {
        params = parseQuery(path.substr(queryStart + 1));  // Extract query parameters
        path = path.substr(0, queryStart);                 // Update path
      }
    } else if (method == "POST") {
      path = req.substr(req.find("POST /") + 6, req.find(" HTTP/1.1") - 6);
      auto body_pos = req.find("\r\n\r\n");
      if (body_pos != std::string::npos) {
        std::string contentType = req.substr(req.find("Content-Type: ") + 14);

        if (contentType.starts_with("multipart/form-data")) {
          // Handle multipart form data
          std::string boundary = contentType.substr(contentType.find("boundary=") + 9);
          std::string body = req.substr(body_pos + 4);  // Skip headers
          auto multipartParams = parseMultipart(body);
          params.insert(multipartParams.begin(), multipartParams.end());  // Combine with existing params
        } else {
          // Handle regular form data
          std::string body = req.substr(body_pos + 4);  // Skip headers
          params = parseQuery(body);
        }
      }
    }
    for (auto& [key, value] : params) {
      lhy::debug(), key, ":", value;
    }
    path = '/' + path;
    // Continue processing based on the path and parameters
    if (router.contains(path)) {
      return router[path](std::move(params));  // Pass the entire params map to the handler
    }

    return ReadHtml(base_path + path);  // Fallback to static HTML
  }

  std::function<std::string(std::map<std::string, std::string>&&)>& GetRouter(const std::string& path) {
    return router[path];
  }
  std::map<std::string, std::function<std::string(std::map<std::string, std::string>&&)>> router;
};
HtmlSolver_& GetHtmlSolver() {
  static HtmlSolver_ solver;
  return solver;
}
std::string HtmlSolver(const std::string& req) { return GetHtmlSolver()(req); }

class Router_ {
 public:
  std::function<std::string(std::map<std::string, std::string>&&)>& operator()(const std::string& path) {
    return GetHtmlSolver().GetRouter(path);
  }
};
std::function<std::string(std::map<std::string, std::string>&&)>& Router(const std::string& path) {
  static Router_ router;
  return router(path);
}
class Socket {
 public:
  Socket(std::string&& host, std::string&& port) : socket_addr_(std::move(host), std::move(port)) {
    sockfd_ = socket_addr_.CreateSocket();
    while (true) {
      const auto client_fd =
          CheckLinuxError(accept(sockfd_, socket_addr_.addrinfo_->ai_addr, &socket_addr_.addrinfo_->ai_addrlen));
      threads_.emplace_back([client_fd] {
        // lhy::debug(), "client connected";
        char buf[1024];
        auto n = CheckLinuxError(read(client_fd, buf, sizeof(buf)));
        auto req = std::string(buf, n);
        /// 解析一下html
        auto response = HtmlSolver(req);
        // auto html_content = ReadHtml(base_path + now_page);
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

 private:
  int sockfd_;
  SocketAddr socket_addr_;
  std::vector<std::jthread> threads_{};
};
int main() {
  // setlocale(LC_ALL, "zh_CN.UTF-8");
  /// 因为一个域名可以对应多个IP，所以返回的是一个链表
  // Router("/") = [](std::string&&) { return ReadHtml(base_path + "/index.html"); };
  Router("/send_message") = [](std::map<std::string, std::string>&& query) {
    if (query.contains("message") && query.contains("username")) {
      lhy::DataBase db{};
      db.Open("chat", "message");
      db.InsertRow({"message"}, query["username"] + ": " + query["message"]);
    }
    return std::string();
  };

  Router("/messages") = [](std::map<std::string, std::string>&& query) {
    std::string chat;
    lhy::DataBase db{};
    db.Open("chat", "message");
    for (const auto& msg : db.SelectColumns(std::tuple{"message"}, {})) {
      chat += "<div class='message'>" + msg["message"].get<std::string>() + "</div>";
    }
    return chat;
  };

  // Route to handle login logic
  Router("/process_login") = [](std::map<std::string, std::string>&& query) -> std::string {
    if (query.contains("username") && query.contains("password")) {
      auto& username = query["username"];
      auto& password = query["password"];
      // 验证用户名和密码
      lhy::DataBase db{};
      db.Open("chat", "credits");
      if (!db.SelectColumns(std::tuple{"username", "password"},
                            lhy::DataBase::Condition().Equal("username", username).And().Equal("password", password))
               .empty()) {
        return "/index.html?username=" + username + "&password=" + password;
      }
      return "Invalid credentials";  // 失败时返回 Invalid credentials
    }
    return "Invalid request";
  };

  // Route to handle registration logic
  Router("/process_register") = [](std::map<std::string, std::string>&& query) -> std::string {
    if (query.contains("username") && query.contains("password")) {
      auto& username = query["username"];
      auto& password = query["password"];

      // 存储用户和密码
      lhy::DataBase db{};
      db.Open("chat", "credits");
      if (!db.SelectColumns(std::tuple{"username"}, lhy::DataBase::Condition().Equal("username", username)).empty()) {
        return "Invalid request.";
      }
      db.InsertRow({"username", "password"}, username, password);

      // 注册成功，重定向到登录页面
      return "HTTP/1.1 302 Found\r\nLocation: /login.html\r\n\r\n";
    }
    return "Invalid request.";
  };

  Router("/index.html") = [](std::map<std::string, std::string>&& query) -> std::string {
    if (query.contains("username") && query.contains("password")) {
      auto& username = query["username"];
      auto& password = query["password"];
      lhy::DataBase db{};
      db.Open("chat", "credits");
      if (!db.SelectColumns(std::tuple{"username", "password"},
                            lhy::DataBase::Condition().Equal("username", username).And().Equal("password", password))
               .empty()) {
        return ReadHtml(base_path + "/index.html");
        // return "/index.html?username=" + username + "&password=" + password;
      }
    }
    // 验证失败，跳转到login.html
    return "HTTP/1.1 302 Found\r\nLocation: /login.html\r\n\r\n";
  };
  Router("/") = [](std::map<std::string, std::string>&& query)->std::string {
    // Check if the user is logged in by verifying their session
        return "HTTP/1.1 302 Found\r\nLocation: /login.html\r\n\r\n";
  };
  Socket socket("127.0.0.1", "8080");
}