/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/20 00:50:16 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/27 00:11:06 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTP/Message.hpp"
#include <sstream>
#include <fstream>

HttpMessage::HttpMessage() :
    _version("Http/1.1"),
    _body("")
{
}

HttpMessage::HttpMessage(const HttpMessage &rhs)
{
    if (this != &rhs)
    {
        _version = rhs._version;
        _body = rhs._body;
        _headers = rhs._headers;
    }
}

HttpMessage::~HttpMessage()
{
}

HttpMessage& HttpMessage::operator=(const HttpMessage &rhs)
{
    if (this != &rhs)
    {
        _version = rhs._version;
        _body = rhs._body;
        _headers = rhs._headers;
    }
    return *this;
}

// -----------------------------------------------------------------------------
//                                   Getters
// -----------------------------------------------------------------------------

std::string HttpMessage::getVersion() const
{
    return _version;
}

HeaderMap HttpMessage::getHeaders() const
{
    return _headers;
}

std::string HttpMessage::getHeader(const std::string& key) const
{
    HeaderMap::const_iterator it = _headers.find(key);
    if (it == _headers.end())
        return "";
    return it->second;
}

std::string HttpMessage::getBody() const
{
    return _body;
}

// -----------------------------------------------------------------------------
//                                   Setters
// -----------------------------------------------------------------------------

void HttpMessage::setVersion(const std::string& version)
{
    _version = version;
}

void HttpMessage::setHeader(const std::string& key, const std::string& value)
{
    _headers[key] = value;
}

// Parses a line with format 'key: value' and adds it to the headers map
void HttpMessage::setHeader(const std::string& line)
{
    size_t pos = line.find(':');
    if (pos == std::string::npos)
        throw HttpMessage::ParseException();
    std::string key = line.substr(0, pos);
    // find first non-whitespace character after ':'
    pos = line.find_first_not_of(" \t", pos + 1);
    if (pos == std::string::npos)
        throw HttpMessage::ParseException();
    std::string value = line.substr(pos);
    this->setHeader(key, value);
}

void HttpMessage::setBody(const std::string& body)
{
    _body = body;
}

void HttpMessage::setBody(const std::ifstream& f)
{
    std::stringstream ss;
    ss << f.rdbuf();
    this->setBody(ss.str());
}

// -----------------------------------------------------------------------------
//                                 Removers
// -----------------------------------------------------------------------------

void HttpMessage::removeHeader(const std::string& key)
{
    _headers.erase(key);
}

// -----------------------------------------------------------------------------
//                                  Helpers
// -----------------------------------------------------------------------------

void HttpMessage::printStart() const
{
    std::cout << _version << std::endl;
}

void HttpMessage::printHeaders() const
{
    for (HeaderMap::const_iterator it = _headers.begin();
        it != _headers.end(); it++)
        std::cout << it->first << ": " << it->second << std::endl;
}

void HttpMessage::printBody() const
{
    size_t max_size = 250;
    size_t size = _body.size();
    if (size > max_size)
        std::cout << _body.substr(0, max_size) << "..." << std::endl;
    else
        std::cout << _body << std::endl;
    if (size > 0)
        std::cout << "(size: " << _body.size() << ")" << std::endl;
}

void HttpMessage::print() const
{
    std::cout << "----------------------------------------" << std::endl;
    this->printStart();
    this->printHeaders();
    std::cout << std::endl;
    this->printBody();
    std::cout << "----------------------------------------" << std::endl;
    std::cout << std::endl;
}

std::string& _drop_carriage_return(std::string& s, bool raise)
{
    if (!s.empty())
    {
        size_t pos = s.length() - 1;
        if (s[pos] != '\r')
        {
            if (raise)
                throw HttpMessage::ParseException();
            else
                return s;
        }
        s = s.substr(0, pos);
    }
    return s;
}

// -----------------------------------------------------------------------------
//                                IO operators
// -----------------------------------------------------------------------------

// Parses headers and body
std::istream &operator>>(std::istream &is, HttpMessage &msg)
{
    std::string line;

    // Headers
    while (std::getline(is, line))
    {
        _drop_carriage_return(line);
        if (line.empty())
            break;
        msg.setHeader(line);
    }
    
    // Body
    if (std::getline(is, line))
        msg._body = _drop_carriage_return(line, false); 
    while (std::getline(is, line))
    {
        msg._body += "\n";
        msg._body += _drop_carriage_return(line, false);   
    }
    // Add Content-Length header if not present
    std::stringstream ss;
    ss << msg._body.size();
    msg.setHeader("Content-Length", ss.str());
    return is;
}

// Write headers and body to an output stream
std::ostream &operator<<(std::ostream &out, const HttpMessage &msg)
{
    for (HeaderMap::const_iterator it = msg._headers.begin();
        it != msg._headers.end(); ++it)
        out << it->first << ": " << it->second << "\r\n";
    out << "\r\n" << msg._body;
    return out;
}

// -----------------------------------------------------------------------------
//                             Custom exceptions
// -----------------------------------------------------------------------------

const char* HttpMessage::ParseException::what(void) const throw()
{
    return "Error parsing Http Message";
}
