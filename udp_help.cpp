//
// Created by lhy on 24-10-2.
//
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <thread>

#include "basic.h"

const int TCP_PORT = 8080;
const char* UDP_SERVER_IP = "127.0.0.1";
const int UDP_SERVER_PORT = 41234;

void handleClient(int client_sockfd) {
  int udp_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (udp_sockfd < 0) {
    perror("UDP socket creation failed");
    close(client_sockfd);
    return;
  }

  sockaddr_in udp_server_addr{};
  udp_server_addr.sin_family = AF_INET;
  udp_server_addr.sin_port = htons(UDP_SERVER_PORT);
  inet_pton(AF_INET, UDP_SERVER_IP, &udp_server_addr.sin_addr);

  char buffer[65536];
  ssize_t n;

  while ((n = read(client_sockfd, buffer, sizeof(buffer))) > 0) {
    sendto(udp_sockfd, buffer, n, 0, (sockaddr*)&udp_server_addr, sizeof(udp_server_addr));

    socklen_t addr_len = sizeof(udp_server_addr);
    n = recvfrom(udp_sockfd, buffer, sizeof(buffer), 0, (sockaddr*)&udp_server_addr, &addr_len);
    if (n < 0) {
      perror("Failed to receive from UDP server");
      break;
    }

    write(client_sockfd, buffer, n);
  }

  close(udp_sockfd);
  close(client_sockfd);
}

int main() {
  int tcp_sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (tcp_sockfd < 0) {
    perror("TCP socket creation failed");
    return 1;
  }

  sockaddr_in tcp_server_addr{};
  tcp_server_addr.sin_family = AF_INET;
  tcp_server_addr.sin_addr.s_addr = INADDR_ANY;
  tcp_server_addr.sin_port = htons(TCP_PORT);

  if (bind(tcp_sockfd, (sockaddr*)&tcp_server_addr, sizeof(tcp_server_addr)) < 0) {
    perror("Bind failed");
    close(tcp_sockfd);
    return 1;
  }

  if (listen(tcp_sockfd, 5) < 0) {
    perror("Listen failed");
    close(tcp_sockfd);
    return 1;
  }

  while (true) {
    int client_sockfd = accept(tcp_sockfd, nullptr, nullptr);
    if (client_sockfd < 0) {
      perror("Accept failed");
      continue;
    }
    std::thread(handleClient, client_sockfd).detach();
  }
}
