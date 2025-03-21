#pragma once

#include <cstdint>
#include <string>
#include <netinet/in.h>

class InetAddress
{
public:
  explicit InetAddress(uint16_t port, std::string ip = "127.0.0.1");
  explicit InetAddress(const struct sockaddr_in &addr) : addr_(addr) {}

  std::string toIp() const;
  std::string toIpPort() const;
  uint16_t toPort() const;

  const sockaddr_in *getSockAddr() const { return &addr_; }

private:
  struct sockaddr_in addr_;
};