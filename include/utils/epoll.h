//
// Created by bismarckkk on 2022/9/20 0020.
// Copyright (c) bismarckkk All rights reserved.
// https://github.com/bismarckkk
//

#ifndef HTTPSERVER_EPOLL_H
#define HTTPSERVER_EPOLL_H

#include <sys/epoll.h>

void addFdToEpoll(int epollFd, int socketFd, bool oneShot);
void removeFdFromEpoll(int epollFd, int connFd);
void resetEpollOneShot(int epollFd, int connFd, int event);

#endif //HTTPSERVER_EPOLL_H
