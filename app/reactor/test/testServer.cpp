#include <TcpServer.h>
#include <string>
#include <functional>

class EchoServer
{
public:
  EchoServer(EventLoop *loop, const InetAddress &listenAddr, const std::string &nameArg) : server_(loop, listenAddr, nameArg), loop_(loop)
  {
  }
  void start()
  {
    server_.setConnectionCallback(std::bind(&EchoServer::onConnection, this, std::placeholders::_1));
    server_.setMessageCallback(std::bind(&EchoServer::onMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    server_.start();
  };

private:
  void onConnection(const TcpConnectionPtr &conn)
  {
    if (conn->isConnected())
    {
      LOG_INFO("Connection %s", conn->getPeerAddress().toIpPort().c_str());
    }
    else
    {
      LOG_INFO("Connection %s", conn->getPeerAddress().toIpPort().c_str());
    }
  };
  void onMessage(const TcpConnectionPtr &conn, Buffer *buffer, Timestamp receiveTime)
  {
    LOG_INFO("onMessage[%s] %s", conn->getName().c_str(), buffer->retrieveAllAsString().c_str());
  };

  EventLoop *loop_;
  TcpServer server_;
};

int main()
{
  EventLoop loop;
  InetAddress listenAddr(8080);
  EchoServer server(&loop, listenAddr, "EchoServer");
  server.start();
  loop.loop();
}