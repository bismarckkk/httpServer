//
// Created by bismarckkk on 2022/9/20 0020.
// Copyright (c) bismarckkk All rights reserved.
// https://github.com/bismarckkk
//

#ifndef HTTPSERVER_TCPSERVERTHREAD_H
#define HTTPSERVER_TCPSERVERTHREAD_H

#define EVENTS_SIZE 128

#include <atomic>
#include <map>
#include <thread>

#include <sys/epoll.h>

#include "worker.h"

using std::atomic, std::map, std::pair, std::thread;

class tcpServerThread {
private:
    int epollFd, socketFd;
    atomic<bool> running;
    epoll_event epollEvents[EVENTS_SIZE] = {};
    map<int, worker> workers;
    thread workThread;

public:
    explicit tcpServerThread(int _socketFd);
    ~tcpServerThread();
    void start();
    void run();
};


#endif //HTTPSERVER_TCPSERVERTHREAD_H
