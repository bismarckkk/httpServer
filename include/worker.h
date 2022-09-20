//
// Created by bismarckkk on 2022/9/20 0020.
// Copyright (c) bismarckkk All rights reserved.
// https://github.com/bismarckkk
//

#ifndef HTTPSERVER_WORKER_H
#define HTTPSERVER_WORKER_H

#define BUFFER_SIZE 20

#include <string>

#include <async_simple/coro/Lazy.h>

using std::string;
using namespace async_simple::coro;

class worker {
private:
    int epollFd=-1, connFd=-1;
    string buffer;
    string writeBuffer;
    char tmpBuffer[BUFFER_SIZE]={};

    Lazy<> process();
    Lazy<> work();
    Lazy<> _workerRead();
    void stop();

public:
    worker() = default;
    worker(int epollFd, int connFd);
    worker(worker&& other) noexcept;
    worker(const worker&) = delete;
    ~worker();
    void workerRead();
    bool workerWrite();
};


#endif //HTTPSERVER_WORKER_H
