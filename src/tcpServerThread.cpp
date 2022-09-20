//
// Created by bismarckkk on 2022/9/20 0020.
// Copyright (c) bismarckkk All rights reserved.
// https://github.com/bismarckkk
//

#include <iostream>

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
    addFdToEpoll(epollFd, socketFd, false);
    workThread = thread(&tcpServerThread::run, this);
}

void tcpServerThread::run() {
    while(running) {
        int eventNum = epoll_wait(epollFd, epollEvents, EVENTS_SIZE, -1);
        if (eventNum < 0 && errno != EINTR) {
            throw epollWaitError();
        }

        for (int i = 0; i < eventNum; i++) {
            std::cout << "get event\r\n";
            int listenFd = epollEvents[i].data.fd;

            if (socketFd == listenFd) {
                sockaddr_in clientAddress{};
                socklen_t clientAddrLength = sizeof(clientAddress);
                int connFd = accept(listenFd, (sockaddr *)&clientAddress, &clientAddrLength);
                if (connFd < 0) {
                    continue;
                }
                workers.erase(connFd);
                workers.emplace(connFd, std::move(worker(epollFd, connFd)));
                std::cout << "someone connect\r\n";
            } else if (epollEvents[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)) {
                if (workers.erase(listenFd) == 0) {
                    removeFdFromEpoll(epollFd, listenFd);
                }
                std::cout << "someone disconnect\r\n";
            } else if (epollEvents[i].events & EPOLLOUT) {
                auto it = workers.find(listenFd);
                if (it != workers.end()) {
                    it->second.workerWrite();
                }
            } else if (epollEvents[i].events & EPOLLIN) {
                std::cout << "someone send message\r\n";
                workers[listenFd].workerRead();
            }
        }
    }
}
