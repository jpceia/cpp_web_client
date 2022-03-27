/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/27 02:19:48 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/27 02:53:57 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPMESSAGEPARSER_HPP
# define HTTPMESSAGEPARSER_HPP

# include "HTTP/Message.hpp"

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

class HttpMessageParser : virtual public HttpMessage
{
public:
    HttpMessageParser();
    HttpMessageParser(const HttpMessageParser& rhs);
    virtual ~HttpMessageParser();
    HttpMessageParser& operator=(const HttpMessageParser& rhs);

    // Getters
    ParseState getState() const;

    // Setters
    void setHeader(const std::string& key, const std::string& value);

    // Parsers
    virtual ParseState parse(const std::string& s = "");

private:
    virtual ParseState _parse_start() = 0;
    virtual ParseState _parse_headers();
    virtual ParseState _parse_body();
    virtual ParseState _parse_chunk_head();
    virtual ParseState _parse_chunk_content();
    virtual ParseState _parse_chunk_tail();

protected:
    ParseState _state;
    std::string _buf;
    size_t _content_length;
};


#endif
