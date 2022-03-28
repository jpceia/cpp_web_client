/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/27 00:26:36 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/28 07:51:16 by jpceia           ###   ########.fr       */
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
# include <vector>

class TcpSocket
{
public:
    TcpSocket();
    ~TcpSocket();
    TcpSocket(const TcpSocket& rhs);
    TcpSocket& operator=(const TcpSocket& rhs);
    TcpConnection connect(const std::string& host, int port = 80);

    int getFd() const;

private:
    int _sock;
};

struct addrinfo get_addrinfo(const std::string& host, int port = 80);
std::vector<struct in_addr> get_hostbyname(const std::string& host);

#endif
