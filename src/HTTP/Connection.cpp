/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 06:02:05 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/28 06:56:07 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTP/Connection.hpp"
#include "HTTP/Parser/Response.hpp"
#include "TCP/Connection.hpp"


HttpConnection::HttpConnection(int fd) :
    TcpConnection(fd)
{
}

HttpConnection::HttpConnection(const TcpConnection& rhs) :
    TcpConnection(rhs)
{
}

HttpConnection::HttpConnection(const HttpConnection& rhs) :
    TcpConnection(rhs)
{
}

HttpConnection::~HttpConnection()
{
}

HttpConnection& HttpConnection::operator=(const HttpConnection& rhs)
{
    TcpConnection::operator=(rhs);
    return *this;
}

void HttpConnection::sendRequest(const HttpRequest& request) const
{
    std::stringstream ss;
    ss << request;
    std::string msg = ss.str();
    while (!msg.empty())
        this->send(msg);
}

HttpResponse HttpConnection::recvResponse() const
{
    HttpResponseParser response;
    ParseState state = PARSE_START;

    while (state != PARSE_COMPLETE)
    {
        std::string chunk = this->recv();
        state = response.parse(chunk);
        if (chunk.empty())
            throw TcpConnection::DisconnectedException();
    }
    return response;
}
