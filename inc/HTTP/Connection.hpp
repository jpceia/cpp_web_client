/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 05:15:40 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/27 00:11:53 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPCONNECTION_HPP
# define HTTPCONNECTION_HPP

# include "TCP/Connection.hpp"
# include "HTTP/Request.hpp"
# include "HTTP/Response.hpp"
# include <sstream>

// https://stackoverflow.com/questions/68468641/parse-http-responses-from-a-tcp-stream

class TcpConnection;

class HttpConnection : public TcpConnection
{
public:
    HttpConnection(int fd);
    HttpConnection(const TcpConnection& rhs);
    HttpConnection(const HttpConnection& rhs);

    virtual ~HttpConnection();
    virtual HttpConnection& operator=(const HttpConnection& rhs);

    virtual void sendResponse(const HttpResponse& response) const;
    virtual HttpRequest recvRequest() const;
};

#endif