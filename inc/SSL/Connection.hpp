/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 06:32:38 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/28 07:31:25 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SSLCONNECTION_HPP
# define SSLCONNECTION_HPP

# include "TCP/Connection.hpp"
# include "SSL/Context.hpp"


class SslConnection : virtual public TcpConnection
{
public:
    SslConnection(int sock, const SslContext& ctx);
    SslConnection(const SslConnection& rhs);
    SslConnection& operator=(const SslConnection& rhs);
    ~SslConnection();

    virtual void send(std::string& msg) const;
    virtual std::string recv() const;
private:

    SSL *_ssl;
};

#endif
