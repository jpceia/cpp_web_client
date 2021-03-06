/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 06:52:06 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/28 07:36:52 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPSCONNECTION_HPP
# define HTTPSCONNECTION_HPP

# include "SSL/Connection.hpp"
# include "SSL/Context.hpp"
# include "HTTP/Connection.hpp"

class HttpsConnection : public SslConnection, public HttpConnection 
{
public:
    HttpsConnection(int fd, const SslContext& ctx);
    HttpsConnection(const HttpsConnection& rhs);
    virtual ~HttpsConnection();
    HttpsConnection& operator=(const HttpsConnection& rhs);
};

#endif
