/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/27 00:26:36 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/27 03:09:19 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TCPSOCKET_CPP
# define TCPSOCKET_CPP

# include "TCP/Connection.hpp"
# include <iostream>

# include <sstream>
# include <fcntl.h>
# include <netdb.h>
# include <cstring>

class TcpSocket
{
public:
    TcpSocket(const std::string& host, int port);
    ~TcpSocket();
    TcpSocket(const TcpSocket& rhs);
    TcpSocket& operator=(const TcpSocket& rhs);
    TcpConnection connect();
    void close();
private:
    int _sock;
    struct addrinfo *_addr;
};

#endif
