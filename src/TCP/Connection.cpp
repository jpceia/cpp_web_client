/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 03:04:11 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/27 00:11:03 by jpceia           ###   ########.fr       */
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

TcpConnection::TcpConnection(const TcpConnectionArgs& args) :
    _sock(args.sock),
    _server_addr(args.server_addr),
    _client_addr(args.client_addr)
{
    if (_sock < 0)
        throw std::runtime_error("Invalid file descriptor");
}

TcpConnection::TcpConnection(const TcpConnection& rhs) :
    _sock(rhs._sock),
    _server_addr(rhs._server_addr),
    _client_addr(rhs._client_addr)
{
}

TcpConnection::~TcpConnection()
{
}

TcpConnection& TcpConnection::operator=(const TcpConnection& rhs)
{
    if (this != &rhs)
    {
        _sock = rhs._sock;
        _server_addr = rhs._server_addr;
        _client_addr = rhs._client_addr;
    }
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

std::string TcpConnection::getServerIP() const
{
    return inet_ntoa(_server_addr.sin_addr);
}

std::string TcpConnection::getClientIP() const
{
    return inet_ntoa(_client_addr.sin_addr);
}

int TcpConnection::getServerPort() const
{
    return ntohs(_server_addr.sin_port);
}

int TcpConnection::getClientPort() const
{
    return ntohs(_client_addr.sin_port);
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
