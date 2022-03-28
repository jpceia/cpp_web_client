/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/27 03:09:30 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/28 05:50:12 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TCP/Socket.hpp"
#include <cerrno>


struct addrinfo* get_addrinfo(const std::string& host, int port)
{
    struct addrinfo req;
    struct addrinfo *res;
    std::string s_port;
    int ecode;

    std::memset(&req, '\0', sizeof(req));
    req.ai_family   = AF_UNSPEC;
    req.ai_socktype = SOCK_STREAM;
    req.ai_flags    = AI_PASSIVE;

    {
        std::stringstream ss;
        ss << port;
        s_port = ss.str();
    }

    ecode = getaddrinfo(host.c_str(), s_port.c_str(), &req, &res);
    if (ecode != 0)
        throw std::runtime_error("get_addrinfo(): " + std::string(gai_strerror(ecode)));
    if (res == NULL)
        throw std::runtime_error("get_addrinfo(): No addresses found");
    return res;
}

TcpSocket::TcpSocket()
{
    _sock = socket(AF_INET, SOCK_STREAM, 0);
    if (_sock < 0)
        throw std::runtime_error("socket(): " + std::string(strerror(errno)));

    /*
    https://stackoverflow.com/questions/6202454/operation-now-in-progress-error-on-connect-function-error
    if (fcntl(_sock, F_SETFL, O_NONBLOCK) < 0)
        throw std::runtime_error("fcntl(): " + std::string(strerror(errno)));
    */
    int on = 1; //char yes='1'; // Solaris people use this
    if (setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
        throw std::runtime_error("setsockopt(): " + std::string(strerror(errno)));
}

TcpSocket::TcpSocket(const TcpSocket& rhs) :
    _sock(rhs._sock)
{
}

TcpSocket::~TcpSocket()
{
    ::close(_sock);
}

TcpSocket& TcpSocket::operator=(const TcpSocket& rhs)
{
    if (this != &rhs)
        _sock = rhs._sock;
    return *this;
}

TcpConnection TcpSocket::connect(const std::string& host, int port)
{
    struct addrinfo *addr = get_addrinfo(host, port);
    int ecode = ::connect(_sock, addr->ai_addr, addr->ai_addrlen);
    freeaddrinfo(addr);

    if (ecode < 0)
        throw std::runtime_error("connect(): " + std::string(strerror(errno)));
    return _sock;
}
