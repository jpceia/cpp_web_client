/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/27 00:11:34 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/27 02:54:13 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUESTPARSE_HPP
# define HTTPREQUESTPARSE_HPP

# include "HTTP/Request.hpp"
# include "HTTP/Parser/Message.hpp"

class HttpRequestParser : public HttpRequest, public HttpMessageParser
{
public:
    HttpRequestParser();
    HttpRequestParser(const HttpRequestParser& rhs);
    virtual ~HttpRequestParser();
    HttpRequestParser& operator=(const HttpRequestParser& rhs);

private:
    ParseState _parse_start();
};

#endif
