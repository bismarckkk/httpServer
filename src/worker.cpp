//
// Created by bismarckkk on 2022/9/20 0020.
// Copyright (c) bismarckkk All rights reserved.
// https://github.com/bismarckkk
//

#include <unistd.h>
#include <iostream>

#include "worker.h"
#include "utils/epoll.h"

worker::worker(int epollFd, int connFd) {
    this->epollFd = epollFd;
    this->connFd = connFd;
    buffer = string(BUFFER_SIZE, 0);
    addFdToEpoll(epollFd, connFd, true);
}

worker::worker(worker&& other) noexcept {
    epollFd = other.epollFd;
    connFd = other.connFd;
    buffer = std::move(other.buffer);
    writeBuffer = std::move(other.writeBuffer);

    other.epollFd = -1;
    other.connFd = -1;
}

worker::~worker() {
    if (epollFd != -1 && connFd != -1) {
        stop();
    }
}

void worker::workerRead() {
    work().start([](auto&&){std::cout << "recv finish\r\n";});
}

Lazy<> worker::_workerRead() {
    buffer.clear();
    while (true) {
        ssize_t len = read(connFd, tmpBuffer, BUFFER_SIZE);
        std::cout << "recv: " << len << " bytes\r\n";
        if (len == -1) {
            if (errno==EAGAIN||errno==EWOULDBLOCK) {
                break;
            } else {
                stop();
            }
        }
        buffer.insert(buffer.length(), tmpBuffer, len);
    }
    co_return ;
}

Lazy<> worker::work() {
    co_await worker::_workerRead();

    resetEpollOneShot(epollFd, connFd);

    co_await process();

    if(!workerWrite()) {
        resetEpollWrite(epollFd, connFd);
    }
    co_return ;
}

void worker::stop() {
    removeFdFromEpoll(epollFd, connFd);
    epollFd = -1;
    connFd = -1;
}

bool worker::workerWrite() {
    if (writeBuffer.empty()) {
        return true;
    }
    ssize_t len = write(connFd, writeBuffer.c_str(), writeBuffer.length());
    if (len == -1) {
        writeBuffer.clear();
        return true;
    } else {
        return false;
    }
}

Lazy<> worker::process() {
    std::cout << "recv: " << buffer << std::endl;
    writeBuffer = "ok\r\n";
    co_return ;
}


