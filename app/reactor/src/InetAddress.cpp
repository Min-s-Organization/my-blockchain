#include "InetAddress.h"

#include <string.h>
#include <arpa/inet.h>

InetAddress::InetAddress(uint16_t port, std::string ip)
{
  memset(&addr_, 0, sizeof(addr_));
  addr_.sin_family = AF_INET;
  addr_.sin_port = htons(port);
  addr_.sin_addr.s_addr = inet_addr(ip.c_str());
}

std::string InetAddress::toIp() const
{
  char buf[64] = {0};
  inet_ntop(AF_INET, &addr_.sin_addr, buf, sizeof(buf));
  return buf;
}

std::string InetAddress::toIpPort() const
{
  char buf[64] = {0};
  inet_ntop(AF_INET, &addr_.sin_addr, buf, sizeof(buf));
  uint16_t port = ntohs(addr_.sin_port);
  return std::string(buf) + ":" + std::to_string(port);
}

uint16_t InetAddress::toPort() const
{
  return ntohs(addr_.sin_port);
}
