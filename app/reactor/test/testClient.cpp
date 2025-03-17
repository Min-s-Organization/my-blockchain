#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>

int main()
{
  // connect to server which is running on localhost:8080
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
  {
    perror("socket");
    exit(1);
  }
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(8080);
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
  {
    perror("connect");
    exit(1);
  }
  char buffer[1024];
  ssize_t n = send(sockfd, "Hello, World!", 13, 0);
  if (n < 0)
  {
    perror("send");
    exit(1);
  }
  printf("send %zd bytes\n", n);
  close(sockfd);
  return 0;
}