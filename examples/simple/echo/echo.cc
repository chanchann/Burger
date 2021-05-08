#include "echo.h"

using namespace std::placeholders;

EchoServer::EchoServer(const InetAddress& listenAddr, int threadNum, const std::string& name)
    : server_(listenAddr, threadNum, name) {
    server_.setConnectionHandler(std::bind(&EchoServer::connHandler, this, _1));
}

void EchoServer::start() {
    server_.start();
}

void EchoServer::connHandler(CoTcpConnection::ptr conn) {
    RingBuffer::ptr buffer = std::make_shared<RingBuffer>();
    while(conn->recv(buffer) > 0) {
        conn->send(buffer);
    }
}



