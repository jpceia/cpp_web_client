/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/27 00:26:36 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/28 05:20:25 by jpceia           ###   ########.fr       */
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
    TcpSocket();
    ~TcpSocket();
    TcpSocket(const TcpSocket& rhs);
    TcpSocket& operator=(const TcpSocket& rhs);
    TcpConnection connect(const std::string& host = "127.0.0.1", int port = 80);
private:
    int _sock;
};

struct addrinfo* get_addrinfo(const std::string& host = "127.0.0.1", int port = 80);

#endif
