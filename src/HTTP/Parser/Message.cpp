/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpMessageParser.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 05:15:06 by jpceia            #+#    #+#             */
/*   Updated: 2022/02/24 05:21:27 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTP/Parser/Message.hpp"
#include <sstream>

HttpMessageParser::HttpMessageParser() :
    _state(PARSE_START),
    _buf(""),
    _content_length(0)
{
}

HttpMessageParser::HttpMessageParser(const HttpMessageParser& rhs) :
    HttpMessage(rhs),
    _state(rhs._state),
    _buf(rhs._buf),
    _content_length(rhs._content_length)
{
}

HttpMessageParser::~HttpMessageParser()
{
}

HttpMessageParser& HttpMessageParser::operator=(const HttpMessageParser& rhs)
{
    HttpMessage::operator=(rhs);
    if (this != &rhs)
    {
        _state = rhs._state;
        _buf = rhs._buf;
        _content_length = rhs._content_length;
    }
    return *this;
}

// -----------------------------------------------------------------------------
//                                  Getters
// -----------------------------------------------------------------------------

ParseState HttpMessageParser::getState() const
{
    return _state;
}

// -----------------------------------------------------------------------------
//                                  Setters
// -----------------------------------------------------------------------------

void HttpMessageParser::setHeader(const std::string& key, const std::string& value)
{
    HttpMessage::setHeader(key, value);
    if (key == "Content-Length")
    {
        std::stringstream ss(value);
        ss >> _content_length;
    }
}

// -----------------------------------------------------------------------------
//                                   Parsers
// -----------------------------------------------------------------------------

ParseState HttpMessageParser::parse(const std::string& s)
{
    _buf += s;  // append last received chunk to buffer
    switch (_state)
    {
    case PARSE_START:
        return _parse_start();
    case PARSE_HEADER:
        return _parse_headers();
    case PARSE_BODY:
        return _parse_body();
    case PARSE_CHUNK_HEAD:
        return _parse_chunk_head();
    case PARSE_CHUNK_CONTENT:
        return _parse_chunk_content();
    case PARSE_CHUNK_TAIL:
        return _parse_chunk_tail();
    case PARSE_COMPLETE:
        break ;
    }
    return PARSE_COMPLETE;
}

ParseState HttpMessageParser::_parse_headers()
{
    size_t pos = _buf.find("\r\n");

    // only parsing if we have at least one header line complete
    if (pos == std::string::npos)
        return _state;
        
    std::string line = _buf.substr(0, pos);     // get first available line
    _buf = _buf.substr(pos + 2);                // remove first line from buffer
    
    // if the line is empty the header section is over
    if (line.empty())                           
    {
        if (this->getHeader("Transfer-Encoding") == "chunked")
            _state = PARSE_CHUNK_HEAD;
        else if(this->getHeader("Content-Length") != "")
            _state = PARSE_BODY;
        else
        {
            _state = PARSE_COMPLETE;            // otherwise we are done
            return _state;
        }
    }
    else
        HttpMessage::setHeader(line);
    return this->parse();                       // Consume buffer
}

ParseState HttpMessageParser::_parse_body()
{
    _body += _buf;
    _buf = "";  // clear buffer
    size_t current_body_len = _body.size();
    if (current_body_len > _content_length)
        throw HttpMessage::ParseException();
    if (current_body_len == _content_length)
        _state = PARSE_COMPLETE;
    return _state;
}

ParseState HttpMessageParser::_parse_chunk_head()
{
    // check if the body is complete or if we need to consume a new chunk
    size_t pos = _buf.find("\r\n");
    if (pos == std::string::npos)
        return _state;  // fetch more data
    std::string line = _buf.substr(0, pos);
    _buf = _buf.substr(pos + 2);
    std::stringstream ss(line);
    int chunk_size;
    // parse chunk size in hexadecimal format
    ss >> std::hex >> chunk_size;
    if (!ss.eof())
        throw HttpMessage::ParseException();
    if (chunk_size == 0)
    {
        _content_length = 0;
        _state = PARSE_CHUNK_TAIL;
    }
    else
    {
        _content_length += chunk_size;
        _state = PARSE_CHUNK_CONTENT;
    }
    return this->parse(); // consume buffer
}

ParseState HttpMessageParser::_parse_chunk_content()
{
    size_t current_body_len = _body.size();
    if (_content_length < current_body_len)
        throw HttpMessage::ParseException();
    // else
    if (_content_length > current_body_len)
    {
        // consume buffer up to _content_length
        size_t missing_data_len = _content_length - current_body_len;
        if (_buf.size() < missing_data_len)
        {
            _body += _buf;
            _buf = "";
            return _state; // fetch more data
        }
        // else - chunk is complete
        _body += _buf.substr(0, missing_data_len);
        _buf = _buf.substr(missing_data_len);
        _state = PARSE_CHUNK_TAIL;
        return this->parse(); // consume buffer
    }
    _state = PARSE_CHUNK_TAIL;
    return this->parse();
}

ParseState HttpMessageParser::_parse_chunk_tail()
{
    size_t pos = _buf.find("\r\n");
    if (pos == std::string::npos)
        return _state; // feed more data
    if (pos > 0)
        throw HttpMessage::ParseException();
    // else (pos == 0)
    _buf = _buf.substr(2);
    if (_content_length == 0)
        _state = PARSE_COMPLETE;
    else
        _state = PARSE_CHUNK_HEAD; // fetch the next chunk
    return this->parse();
}
