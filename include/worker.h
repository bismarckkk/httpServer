//
// Created by bismarckkk on 2022/9/20 0020.
// Copyright (c) bismarckkk All rights reserved.
// https://github.com/bismarckkk
//

#ifndef HTTPSERVER_WORKER_H
#define HTTPSERVER_WORKER_H


class worker {
public:
    worker() = default;
    worker(int epollFd, int connFd);
    ~worker();
    void process();
};


#endif //HTTPSERVER_WORKER_H
