/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 05:15:40 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/28 07:36:33 by jpceia           ###   ########.fr       */
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

class HttpConnection : virtual public TcpConnection
{
public:
    HttpConnection(int fd);
    HttpConnection(const TcpConnection& rhs);
    HttpConnection(const HttpConnection& rhs);

    virtual ~HttpConnection();
    HttpConnection& operator=(const HttpConnection& rhs);

    virtual void sendRequest(const HttpRequest& response) const;
    virtual HttpResponse recvResponse() const;
};

#endif
