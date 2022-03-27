/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 03:04:11 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/27 02:15:06 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TCPCONNECTION_HPP
# define TCPCONNECTION_HPP

# include <iostream>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <sys/resource.h>
# include <map>

# define BUFF_SIZE 2048

class TcpConnection
{
public:
    class socket_compare
    {
    public:
        bool operator()(const TcpConnection& lhs, const TcpConnection& rhs) const
        {
            return lhs._sock < rhs._sock;
        }

        bool operator()(const TcpConnection* lhs, const TcpConnection* rhs) const
        {
            return lhs->_sock < rhs->_sock;
        }

    private:
        friend class TcpConnection;
    };

    TcpConnection(int sock);
    TcpConnection(const TcpConnection& rhs);

    virtual ~TcpConnection();
    virtual TcpConnection& operator=(const TcpConnection& rhs);

    virtual void send(std::string& msg) const;
    virtual std::string recv() const;

    int getSock() const;

    class ConnectionException : public std::exception
    {
        public:
            virtual const char* what(void) const throw();
    };

    class SendException : public TcpConnection::ConnectionException
    {
        public:
            virtual const char* what(void) const throw();
    };
    
    class ReadException : public TcpConnection::ConnectionException
    {
        public:
            virtual const char* what(void) const throw();
    };

    class DisconnectedException : public TcpConnection::ConnectionException
    {
        public:
            virtual const char* what(void) const throw();
    };

private:
    int _sock;
};

#endif
