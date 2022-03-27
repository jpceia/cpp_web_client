/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 06:02:05 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/27 00:11:05 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTP/Connection.hpp"
#include "HTTP/RequestParser.hpp"
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

void HttpConnection::sendResponse(const HttpResponse& response) const
{
    std::stringstream ss;
    ss << response;
    std::string msg = ss.str();
    while (!msg.empty())
        TcpConnection::send(msg);
}

HttpRequest HttpConnection::recvRequest() const
{
    HttpRequestParser request;
    ParseState state = PARSE_START;

    while (state != PARSE_COMPLETE)
    {
        std::string chunk = TcpConnection::recv();
        state = request.parse(chunk);
        if (chunk.empty())
            throw TcpConnection::DisconnectedException();
    }
    return request;
}
