/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponseParser.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 05:15:06 by jpceia            #+#    #+#             */
/*   Updated: 2022/02/24 05:21:27 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTP/Parser/Response.hpp"
#include <sstream>

HttpResponseParser::HttpResponseParser()
{
}

HttpResponseParser::HttpResponseParser(const HttpResponseParser& rhs) :
    HttpResponse(rhs),
    HttpMessageParser(rhs)
{
}

HttpResponseParser::~HttpResponseParser()
{
}

HttpResponseParser& HttpResponseParser::operator=(const HttpResponseParser& rhs)
{
    HttpResponse::operator=(rhs);
    HttpMessageParser::operator=(rhs);
    return *this;
}

ParseState HttpResponseParser::_parse_start()
{
    size_t pos = _buf.find("\r\n");

    // only parsing if we have the first line complete
    if (pos == std::string::npos)
        return _state;
    std::stringstream ss(_buf.substr(0, pos));  // pass first line to stream
    _buf = _buf.substr(pos + 2);                // remove first line from buffer
    std::string path;
    ss >> _version;
    ss >> _status;                              // parse start line
    if (!ss.eof())
        throw HttpMessage::ParseException();
    _state = PARSE_HEADER;
    return this->parse();                       // Consume buffer
}
