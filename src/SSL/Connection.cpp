/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 06:59:10 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/28 07:30:48 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SSL/Connection.hpp"
#include "SSL/Context.hpp"
#include <openssl/ssl.h> /* core library */
#include <openssl/err.h> /* errors */


SslConnection::SslConnection(int sock, const SslContext& ctx) :
    TcpConnection(sock)
{
    _ssl = SSL_new(ctx.getContext());
    SSL_set_fd(_ssl, sock);

    if (SSL_connect(_ssl) < 0)
    {
        ERR_print_errors_fp(stderr);
        throw std::runtime_error("SSL_connect failed");
    }
}

SslConnection::SslConnection(const SslConnection& rhs) :
    TcpConnection(rhs)
{
    _ssl = rhs._ssl;
}

SslConnection::~SslConnection()
{
    SSL_free(_ssl); // ?
}

SslConnection& SslConnection::operator=(const SslConnection& rhs)
{
    if (this != &rhs)
    {
        TcpConnection::operator=(rhs);
        _ssl = rhs._ssl;
    }
    return *this;
}

void SslConnection::send(std::string& msg) const
{
    if (msg.empty())
        return ;
    int n = SSL_write(_ssl, msg.c_str(), msg.size());
    if (n < 0)
    {
        ERR_print_errors_fp(stderr);
        throw TcpConnection::SendException();
    }
    if (n == 0)
        throw TcpConnection::DisconnectedException();
    msg = msg.substr(n);
}

std::string SslConnection::recv() const
{
    char buff[BUFF_SIZE];
    int n = SSL_read(_ssl, buff, sizeof(buff));
    if (n < 0)
    {
        ERR_print_errors_fp(stderr);
        throw TcpConnection::SendException();
    }
    return std::string(buff, n);
}
