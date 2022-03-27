/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/27 02:21:11 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/27 02:54:17 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSEPARSER_HPP
# define HTTPRESPONSEPARSER_HPP

# include "HTTP/Response.hpp"
# include "HTTP/Parser/Message.hpp"

class HttpResponseParser : public HttpResponse, public HttpMessageParser
{
public:
    HttpResponseParser();
    HttpResponseParser(const HttpResponseParser& rhs);
    virtual ~HttpResponseParser();
    HttpResponseParser& operator=(const HttpResponseParser& rhs);

private:
    ParseState _parse_start();
};

#endif
