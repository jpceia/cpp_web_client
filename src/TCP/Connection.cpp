/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 03:04:11 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/27 02:45:17 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "TCP/Connection.hpp"
# include <map>


TcpConnection::TcpConnection(int sock) :
    _sock(sock)
{
    if (_sock < 0)
        throw std::runtime_error("Invalid file descriptor");
}

TcpConnection::TcpConnection(const TcpConnection& rhs) :
    _sock(rhs._sock)
{
}

TcpConnection::~TcpConnection()
{
}

TcpConnection& TcpConnection::operator=(const TcpConnection& rhs)
{
    if (this != &rhs)
        _sock = rhs._sock;
    return *this;
}

void TcpConnection::send(std::string& msg) const
{
    if (msg.empty())
        return ;
    int n = ::send(_sock, msg.c_str(), msg.size(), 0);
    if (n < 0)
        throw TcpConnection::SendException();
    if (n == 0)
        throw TcpConnection::DisconnectedException();
    msg = msg.substr(n);
}

std::string TcpConnection::recv() const
{
    char buff[BUFF_SIZE];
    int n = ::recv(_sock, buff, sizeof(buff), 0);
    if (n < 0)
        throw TcpConnection::ReadException();
    return std::string(buff, n);
}

int TcpConnection::getSock() const
{
    return _sock;
}

const char* TcpConnection::ConnectionException::what() const throw()
{
    return "Connection error";
}

const char* TcpConnection::SendException::what() const throw()
{
    return "Error sending message";
}

const char* TcpConnection::ReadException::what() const throw()
{
    return "Could not read from socket";
}

const char* TcpConnection::DisconnectedException::what() const throw()
{
    return "Disconnected";
}
