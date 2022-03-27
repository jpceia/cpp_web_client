/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestParser.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 05:15:06 by jpceia            #+#    #+#             */
/*   Updated: 2022/02/24 05:21:27 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTP/Parser/Request.hpp"
#include <sstream>

HttpRequestParser::HttpRequestParser()
{
}

HttpRequestParser::HttpRequestParser(const HttpRequestParser& rhs) :
    HttpRequest(rhs),
    HttpMessageParser(rhs)
{
}

HttpRequestParser::~HttpRequestParser()
{
}

HttpRequestParser& HttpRequestParser::operator=(const HttpRequestParser& rhs)
{
    HttpRequest::operator=(rhs);
    HttpMessageParser::operator=(rhs);
    return *this;
}

ParseState HttpRequestParser::_parse_start()
{
    size_t pos = _buf.find("\r\n");

    // only parsing if we have the first line complete
    if (pos == std::string::npos)
        return _state;
    std::stringstream ss(_buf.substr(0, pos));  // pass first line to stream
    _buf = _buf.substr(pos + 2);                // remove first line from buffer
    std::string method;
    std::string path;
    ss >> method >> path >> _version;           // parse start line
    if (!ss.eof())
        throw HttpRequest::ParseException();
    this->setPath(path);
    this->setMethod(method);
    _state = PARSE_HEADER;
    return this->parse();                       // Consume buffer
}
