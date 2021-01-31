#include "Acceptor.h"
#include "Channel.h"
#include "EventLoop.h"
#include "SocketsOps.h"
#include "Socket.h"
#include "InetAddress.h"

using namespace burger;
using namespace burger::net;

Acceptor::Acceptor(EventLoop* loop, const InetAddress& listenAddr, bool reuseport):
    loop_(loop),
    acceptSocket_(util::make_unique<Socket>(sockets::createNonblockingOrDie())),
    acceptChannel_(util::make_unique<Channel>(loop, acceptSocket_->getFd())),
    listening_(false),
    idleFd_(::open("/dev/null", O_RDONLY | O_CLOEXEC)) {  // 打开一个空的fd,用于占位
    assert(idleFd_ > 0);
    acceptSocket_->setReuseAddr(true);
    acceptSocket_->setReusePort(reuseport);
    acceptSocket_->bindAddress(listenAddr);

    acceptChannel_->setReadCallback(std::bind(&Acceptor::handleRead, 
                        this, std::placeholders::_1));
}

Acceptor::~Acceptor() {
    acceptChannel_->disableAll();
    acceptChannel_->remove();
    sockets::close(idleFd_);
}

void Acceptor::listen() {
    loop_->assertInLoopThread();
    listening_ = true;
    acceptSocket_->listen();
    acceptChannel_->enableReading();
}

void Acceptor::handleRead(Timestamp timestamp) {
    loop_->assertInLoopThread();
    InetAddress peerAddr;
    int connfd = acceptSocket_->accept(peerAddr);
    if(connfd >= 0) {
        TRACE("Accept of {}", peerAddr.getIpPortStr());
        if(newConnectionCallback_) {
            newConnectionCallback_(connfd, peerAddr);
        } else {
            sockets::close(connfd);
        }
    } else {
        ERROR("in Acceptor::handleRead");
        // 当fd达到上限，先占住一个空的fd,然后当fd满了，就用这个接受然后关闭
        // 这样避免一直水平电平一直触发，先去读走他
        if(errno == EMFILE) {
            sockets::close(idleFd_);
            idleFd_ = ::accept(acceptSocket_->getFd(), nullptr, nullptr);
            sockets::close(idleFd_);
            idleFd_ = ::open("/dev/null", O_RDONLY | O_CLOEXEC);
        }
    }
}











