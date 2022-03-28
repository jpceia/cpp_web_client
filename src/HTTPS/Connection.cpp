/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 06:58:12 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/28 07:17:18 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPS/Connection.hpp"
#include "HTTP/Connection.hpp"
#include "SSL/Connection.hpp"
#include "SSL/Context.hpp"


HttpsConnection::HttpsConnection(int fd, const SslContext& ctx) :
    TcpConnection(fd),
    SslConnection(fd, ctx),
    HttpConnection(fd)
{
}

HttpsConnection::HttpsConnection(const HttpsConnection& rhs) :
    TcpConnection(rhs),
    SslConnection(rhs),
    HttpConnection(rhs)
{
}

HttpsConnection::~HttpsConnection()
{
}

HttpsConnection& HttpsConnection::operator=(const HttpsConnection& rhs)
{
    TcpConnection::operator=(rhs);
    SslConnection::operator=(rhs);
    HttpConnection::operator=(rhs);
    return *this;
}
