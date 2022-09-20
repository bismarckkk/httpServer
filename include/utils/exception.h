//
// Created by bismarckkk on 2022/9/20 0020.
// Copyright (c) bismarckkk All rights reserved.
// https://github.com/bismarckkk
//

#ifndef HTTPSERVER_EXCEPTION_H
#define HTTPSERVER_EXCEPTION_H

#include <exception>

struct socketCreateError : public std::exception
{
    [[nodiscard]] const char * what () const noexcept override
    {
        return "Cannot create socket";
    }
};

struct socketBindError : public std::exception
{
    [[nodiscard]] const char * what () const noexcept override
    {
        return "Cannot bind socket to this port and addr";
    }
};

struct epollCtlError : public std::exception
{
    [[nodiscard]] const char * what () const noexcept override
    {
        return "Cannot set epoll event";
    }
};

struct epollWaitError : public std::exception
{
    [[nodiscard]] const char * what () const noexcept override
    {
        return "Error happen when epoll_wait";
    }
};

#endif //HTTPSERVER_EXCEPTION_H
