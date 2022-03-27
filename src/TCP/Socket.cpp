/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/27 03:09:30 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/27 03:09:49 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TCP/Socket.hpp"


TcpSocket::TcpSocket(const std::string& host, int port)
{
    struct addrinfo hints;
    
    std::stringstream ss;
    ss << port;

    std::memset(&hints, '\0', sizeof(hints));
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = AI_PASSIVE;

    // https://man7.org/linux/man-pages/man3/getaddrinfo.3.html
    int ecode = getaddrinfo(host.c_str(), ss.str().c_str(), &hints, &_addr);
    if (ecode != 0)
    {
        std::cerr << gai_strerror(ecode) << std::endl; // GetAddrInfoException
        throw std::runtime_error("getaddrinfo() failed");
    }
    if (_addr == NULL)
    {
        std::cerr << "No addresses found" << std::endl; // NoFoundAddrException
        throw std::runtime_error("getaddrinfo() failed");
    }

    _sock = socket(_addr->ai_family, _addr->ai_socktype, _addr->ai_protocol);
    if (_sock < 0)
        throw std::runtime_error("socket() failed"); // CreateException
    // https://man7.org/linux/man-pages/man2/fcntl.2.html
    if (fcntl(_sock, F_SETFL, O_NONBLOCK) < 0)
    {
        // display errno
        std::cerr << "fcntl failed" << std::endl;
    }

    int on = 1; //char yes='1'; // Solaris people use this
    // lose the pesky "Address already in use" error message
    
    if (setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
        throw std::runtime_error("setsockopt() failed"); // SetOptException  
}

TcpSocket::TcpSocket(const TcpSocket& rhs) :
    _sock(rhs._sock)
{
    _addr = new struct addrinfo;
    *_addr = *rhs._addr;
}

TcpSocket::~TcpSocket()
{
    freeaddrinfo(_addr);
}

TcpSocket& TcpSocket::operator=(const TcpSocket& rhs)
{
    if (this != &rhs)
    {
        _sock = rhs._sock;
        *_addr = *rhs._addr;
    }
    return *this;
}

TcpConnection TcpSocket::connect()
{
    int connection = ::connect(_sock, _addr->ai_addr, _addr->ai_addrlen);
    if (connection < 0)
        throw std::runtime_error("connect() failed"); // ConnectException
    return connection;
}

void TcpSocket::close()
{
    ::close(_sock);
}
