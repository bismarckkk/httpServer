//
// Created by bismarckkk on 2022/9/20 0020.
// Copyright (c) bismarckkk All rights reserved.
// https://github.com/bismarckkk
//

#include <netinet/in.h>
#include <sys/socket.h>

#include "tcpServerThread.h"
#include "utils/epoll.h"
#include "utils/exception.h"

tcpServerThread::tcpServerThread(int _socketFd) {
    epollFd = epoll_create(EVENTS_SIZE);
    if (epollFd == -1) {
        throw epollCtlError();
    }
    socketFd = _socketFd;
}

tcpServerThread::tcpServerThread(tcpServerThread &&other) noexcept {
    if (this != &other) {
        running = false;
        if (other.running) {
            running = true;
        }
        epollFd = other.epollFd;
        socketFd = other.socketFd;
        workers = std::move(other.workers);
        workThread = std::move(other.workThread);

        other.epollFd = -1;
        other.socketFd = -1;
    }
}

tcpServerThread::tcpServerThread(const tcpServerThread & other) {
    if (this != &other) {
        epollFd = epoll_create(EVENTS_SIZE);
        if (epollFd == -1) {
            throw epollCtlError();
        }
        socketFd = other.socketFd;
    }
}

tcpServerThread &tcpServerThread::operator=(const tcpServerThread& other) {
    if (this != &other) {
        epollFd = epoll_create(EVENTS_SIZE);
        if (epollFd == -1) {
            throw epollCtlError();
        }
        socketFd = other.socketFd;
    }
    return *this;
}

tcpServerThread::~tcpServerThread() {
    running = false;
    if (workThread.joinable()) {
        workThread.join();
    }
}

void tcpServerThread::start() {
    running = true;
    addFdToEpoll(epollFd, socketFd, true);
    thread(&tcpServerThread::run, this);
}

void tcpServerThread::run() {
    while(running) {
        int eventNum = epoll_wait(epollFd, epollEvents, EVENTS_SIZE, -1);
        if (eventNum < 0 && errno != EINTR) {
            throw epollWaitError();
        }

        for (int i = 0; i < eventNum; i++) {
            int listenFd = epollEvents[i].data.fd;

            if (socketFd == listenFd) {
                sockaddr_in clientAddress{};
                socklen_t clientAddrLength = sizeof(clientAddress);
                int connFd = accept(listenFd, (sockaddr *)&clientAddress, &clientAddrLength);
                if (connFd < 0) {
                    continue;
                }
                workers.insert_or_assign(connFd, worker(epollFd, connFd));
            } else if (epollEvents[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)) {
                removeFdFromEpoll(epollFd, listenFd);
            } else {
                workers[listenFd].process();
            }
        }
    }
}
