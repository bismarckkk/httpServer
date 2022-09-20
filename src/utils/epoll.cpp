//
// Created by bismarckkk on 2022/9/20 0020.
// Copyright (c) bismarckkk All rights reserved.
// https://github.com/bismarckkk
//

#include <unistd.h>

#include <fcntl.h>

#include "utils/epoll.h"
#include "utils/exception.h"

inline void setNonBlocking(int fd) {
    int option = fcntl(fd, F_GETFL);
    option |= O_NONBLOCK;
    fcntl(fd, F_SETFL, option);
}

// TODO: use is-enabled
void addFdToEpoll(int epollFd, int socketFd, bool oneShot) {
    epoll_event event{};
    event.data.fd = socketFd;
    event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
    if (oneShot) {
        event.events |= EPOLLONESHOT;
    }
    int result = epoll_ctl(epollFd, EPOLL_CTL_ADD, socketFd, &event);
    if (result == -1) {
        throw epollCtlError();
    }
    setNonBlocking(socketFd);
}

void removeFdFromEpoll(int epollFd, int connFd) {
    int result = epoll_ctl(epollFd, EPOLL_CTL_DEL, connFd, nullptr);
    if (result == -1) {
        throw epollCtlError();
    }
    close(connFd);
}

void resetEpollOneShot(int epollFd, int connFd) {
    epoll_event newEvent{};
    newEvent.data.fd = connFd;
    newEvent.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
    int result = epoll_ctl(epollFd, EPOLL_CTL_MOD, connFd, &newEvent);
    if (result == -1) {
        throw epollCtlError();
    }
}

void resetEpollWrite(int epollFd, int connFd) {
    epoll_event newEvent{};
    newEvent.data.fd = connFd;
    newEvent.events = EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLOUT;
    int result = epoll_ctl(epollFd, EPOLL_CTL_MOD, connFd, &newEvent);
    if (result == -1) {
        throw epollCtlError();
    }
}
