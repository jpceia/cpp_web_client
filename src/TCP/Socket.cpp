/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/27 03:09:30 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/28 06:19:48 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TCP/Socket.hpp"
#include <cerrno>
#include <vector>

struct addrinfo get_addrinfo(const std::string& host, int port)
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
    req = *res;
    freeaddrinfo(res);
    return req;
}

std::vector<struct in_addr> get_hostbyname(const std::string& host)
{
	struct hostent *he;
    std::vector<struct in_addr> addrs;

    he = gethostbyname(host.c_str());
    if (he == NULL)
        throw std::runtime_error("get_hostbyname(): " + std::string(hstrerror(h_errno)));
    
    if (he->h_addrtype != AF_INET)
        throw std::runtime_error("get_hostbyname(): Only IPv4 supported");
    if (he->h_addr_list[0] == NULL)
        throw std::runtime_error("get_hostbyname(): No addresses found");
    
    for (size_t i = 0; he->h_addr_list[i]; ++i)
        addrs.push_back(*(struct in_addr*)he->h_addr_list[i]);
    return addrs;
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
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    std::vector<struct in_addr> addrs = get_hostbyname(host);
    if (addrs.empty())
        throw std::runtime_error("connect(): No addresses found");
    addr.sin_addr = addrs[0];
    if (::connect(_sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        throw std::runtime_error("connect(): " + std::string(strerror(errno)));
    return _sock;
}
