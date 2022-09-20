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
    int epollFd=-1, socketFd=-1;
    atomic<bool> running;
    epoll_event epollEvents[EVENTS_SIZE] = {};
    map<int, worker> workers;
    thread workThread;

    void run();

public:
    tcpServerThread() = default;
    explicit tcpServerThread(int _socketFd);
    tcpServerThread(tcpServerThread&& other) noexcept;
    tcpServerThread(const tcpServerThread&);
    tcpServerThread& operator=(const tcpServerThread&);
    ~tcpServerThread();
    void start();
};


#endif //HTTPSERVER_TCPSERVERTHREAD_H
