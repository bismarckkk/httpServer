//
// Created by bismarckkk on 2022/9/20 0020.
// Copyright (c) bismarckkk All rights reserved.
// https://github.com/bismarckkk
//

#ifndef HTTPSERVER_TCPSERVER_H
#define HTTPSERVER_TCPSERVER_H


#include <string>
#include <vector>

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "tcpServerThread.h"

using std::vector;

class tcpServer {
private:
    int socketFd{};
    vector<tcpServerThread> threadsPoll;

    void init(uint16_t port, in_addr_t addr);
public:
    explicit tcpServer(uint16_t port=8888, in_addr_t addr=INADDR_ANY);
    explicit tcpServer(uint16_t port=8888, const std::string& addr="0.0.0.0");
    void start(int threadNum=1);
};


#endif //HTTPSERVER_TCPSERVER_H
