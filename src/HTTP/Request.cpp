/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 15:33:45 by jceia             #+#    #+#             */
/*   Updated: 2022/03/27 00:11:11 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "HTTP/Message.hpp"
# include "HTTP/Request.hpp"
# include "HTTP/Method.hpp"
# include <sstream>


HttpRequest::HttpRequest() :
    HttpMessage(),
    _method(GET),
    _endpoint("/"),
    _query("")
{
}

HttpRequest::HttpRequest(const HttpRequest& rhs) :
    HttpMessage(rhs),
    _method(rhs._method),
    _endpoint(rhs._endpoint),
    _query(rhs._query)
{
}

HttpRequest::~HttpRequest()
{
}

HttpRequest& HttpRequest::operator=(const HttpRequest &rhs)
{
    HttpMessage::operator=(rhs);
    if (this != &rhs)
    {
        _method = rhs._method;
        _endpoint = rhs._endpoint;
        _query = rhs._query;
    }
    return *this;
}

// -----------------------------------------------------------------------------
//                                   Getters
// -----------------------------------------------------------------------------

std::string HttpRequest::getPath() const
{
    if (_query.empty())
        return _endpoint;
    return _endpoint + "?" + _query;
}

std::string HttpRequest::getQueryString() const
{
    return _query;
}

std::string HttpRequest::getEndpoint() const
{
    return _endpoint;
}

HttpMethod HttpRequest::getMethod() const
{
    return _method;
}

// -----------------------------------------------------------------------------
//                                   Setters
// -----------------------------------------------------------------------------

void HttpRequest::setPath(const std::string& path)
{
    size_t pos = path.find('?');
    if (pos != std::string::npos)
    {
        _endpoint = path.substr(0, pos);
        _query = path.substr(pos + 1);
    }
    else
    {
        _endpoint = path;
        _query = "";
    }
}

void HttpRequest::setMethod(const std::string& method)
{
    std::stringstream ss(method);
    ss >> _method;
}

// -----------------------------------------------------------------------------
//                                  Helpers
// -----------------------------------------------------------------------------

void HttpRequest::printStart() const
{
    std::cout << _method << " " << this->getPath() << " " << _version << std::endl;
}

// -----------------------------------------------------------------------------
//                                IO operators
// -----------------------------------------------------------------------------

std::istream &operator>>(std::istream &is, HttpRequest &request)
{
    // Start line
    std::string line;
    std::getline(is, line);
    std::stringstream ss(_drop_carriage_return(line));
    std::string method;
    std::string path;
    ss >> method >> path >> request._version;
    request.setPath(path);
    request.setMethod(method);
    if (!ss.eof())
        throw HttpMessage::ParseException();
    // remaining message
    is >> dynamic_cast<HttpMessage&>(request);
    return is;
}

std::ostream &operator<<(std::ostream &out, const HttpRequest &request)
{
    out << request._method << " "
        << request.getPath() << " "
        << request._version << "\r\n"
        << dynamic_cast<const HttpMessage&>(request);
    return out;
}
