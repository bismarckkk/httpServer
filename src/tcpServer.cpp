//
// Created by bismarckkk on 2022/9/20 0020.
// Copyright (c) bismarckkk All rights reserved.
// https://github.com/bismarckkk
//

#include <arpa/inet.h>

#include "../include/utils/exception.h"
#include "../include/tcpServer.h"

[[maybe_unused]] tcpServer::tcpServer(uint16_t port, in_addr_t addr) {
    tcpServer::init(htons(port), htons(addr));
}

[[maybe_unused]] tcpServer::tcpServer(uint16_t port, const std::string& addr) {
    tcpServer::init(htons(port), inet_addr(addr.c_str()));
}

void tcpServer::init(uint16_t port, in_addr_t addr) {
    socketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socketFd == -1) {
        throw socketCreateError();
    }

    sockaddr_in socketAddr{};
    socketAddr.sin_port = port;
    socketAddr.sin_family = AF_INET;
    socketAddr.sin_addr.s_addr = addr;

    int result = bind(socketFd, (sockaddr *)&socketAddr, sizeof socketAddr);
    if (result == -1) {
        throw socketBindError();
    }
}

void tcpServer::start(const int threadNum) {
    threadsPoll.resize(threadNum, tcpServerThread(socketFd));
    for (auto& workThread :threadsPoll) {
        workThread.start();
    }
}

