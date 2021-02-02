#include "burger/net/TcpServer.h"
#include "burger/net/EventLoop.h"
#include "burger/net/InetAddress.h"

using namespace burger;
using namespace burger::net;

class TestServer {
public:
    TestServer(EventLoop* loop, const InetAddress& listenAddr) :
                    loop_(loop), server_(loop, listenAddr, "TestServer") {
        server_.setConnectionCallback(
            std::bind(&TestServer::onConnection, this, std::placeholders::_1));
        server_.setMessageCallback(
            std::bind(&TestServer::onMessage, this, 
            std::placeholders::_1,      // conn
            std::placeholders::_2,      // data
            std::placeholders::_3));    // len
    }
    void start() {
        server_.start();
    }
private:
    void onConnection(const TcpConnectionPtr& conn) {
        if(conn->isConnected()) {
            std::cout << "onConnection(): new connection [" 
                << conn->getName() <<  "] from " 
                << conn->getPeerAddress().getIpPortStr() << std::endl;
        } else {
            std::cout << "onConnection() : connection " 
                << conn->getName() << " is down" << std::endl;
        }   
    }

    void onMessage(const TcpConnectionPtr& conn, 
                    const char* data, ssize_t len) {
        std::cout << "onMessage(): received " << len 
            << " bytes from connection " << conn->getName() << std::endl;
    }   

    EventLoop* loop_;
    TcpServer server_;
};

int main() {
    if (!Logger::Instance().init("log", "logs/test.log", spdlog::level::trace)) {
        std::cout << "Logger init error" << std::endl;
		return 1;
	}
    std::cout << "main() : pid = " << ::getpid() << std::endl;
    InetAddress listenAddr(8888);
    EventLoop loop;
    TestServer server(&loop, listenAddr);
    server.start();
    loop.loop();
}