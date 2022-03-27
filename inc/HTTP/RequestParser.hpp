/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/27 00:11:34 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/27 00:11:42 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUESTPARSE_HPP
# define HTTPREQUESTPARSE_HPP

# include "HTTP/Request.hpp"


enum ParseState
{
    PARSE_START,
    PARSE_HEADER,
    PARSE_BODY,
    PARSE_CHUNK_HEAD,
    PARSE_CHUNK_CONTENT,
    PARSE_CHUNK_TAIL,
    PARSE_COMPLETE
};

class HttpRequestParser : public HttpRequest
{
public:
    HttpRequestParser();
    HttpRequestParser(const HttpRequestParser& rhs);
    virtual ~HttpRequestParser();
    HttpRequestParser& operator=(const HttpRequestParser& rhs);

    // Getters
    ParseState getState() const;

    // Setters
    void setHeader(const std::string& key, const std::string& value);

    // Parsers
    ParseState parse(const std::string& s = "");

private:
    ParseState _parse_start();
    ParseState _parse_headers();
    ParseState _parse_body();
    ParseState _parse_chunk_head();
    ParseState _parse_chunk_content();
    ParseState _parse_chunk_tail();

    ParseState _state;
    std::string _buf;
    size_t _content_length;
};

#endif
