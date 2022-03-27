/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 15:33:45 by jceia             #+#    #+#             */
/*   Updated: 2022/03/27 05:27:55 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "HTTP/Response.hpp"
# include <sstream>
# include <fstream>


HttpResponse::HttpResponse()
{
}

HttpResponse::HttpResponse(const HttpResponse& rhs) :
    HttpMessage(rhs),
    _status(rhs._status)
{
}

HttpResponse::~HttpResponse()
{
}

HttpResponse& HttpResponse::operator=(const HttpResponse &rhs)
{
    HttpMessage::operator=(rhs);
    if (this != &rhs)
        _status = rhs._status;
    return *this;
}

// -----------------------------------------------------------------------------
//                                   Setters
// -----------------------------------------------------------------------------

void HttpResponse::setStatus(const HttpStatus& status)
{
    _status = status;
}

void HttpResponse::setBody(const std::string& body)
{
    HttpMessage::setBody(body);

    // set content length
    std::stringstream ss;
    ss << body.size();
    HttpMessage::setHeader("Content-Length", ss.str());
}

// -----------------------------------------------------------------------------
//                                  Helpers                                     
// -----------------------------------------------------------------------------

void HttpResponse::printStart() const
{
    std::cout << _version << " " << _status << std::endl;
}

// -----------------------------------------------------------------------------
//                                IO operators
// -----------------------------------------------------------------------------

std::istream &operator>>(std::istream &is, HttpResponse &response)
{
    // Start line
    std::string line;
    std::getline(is, line);
    std::stringstream ss(_drop_carriage_return(line));
    ss >> response._version >> response._status;
    if (!ss.eof())
        throw HttpMessage::ParseException();
    // remaining message
    is >> dynamic_cast<HttpMessage&>(response);
    return is;
}

std::ostream &operator<<(std::ostream &out, const HttpResponse &response)
{
    out << response._version << " "
        << response._status << "\r\n"
        << dynamic_cast<const HttpMessage&>(response);
    return out;
}
