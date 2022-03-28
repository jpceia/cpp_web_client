/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Context.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 06:59:55 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/28 07:10:07 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CONTEXT_HPP
# define CONTEXT_HPP

# include <openssl/ssl.h> /* core library */
# include <iostream>


class SslContext
{
public:
    SslContext(const std::string &cert, const std::string &key);
    SslContext(const SslContext &rhs);
    ~SslContext();
    SslContext& operator=(const SslContext &rhs);
    
    SSL_CTX *getContext() const;

private:
    SSL_CTX *_ctx;
};

#endif
