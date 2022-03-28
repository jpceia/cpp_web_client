/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Status.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 00:12:30 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/28 05:43:50 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTP/Status.hpp"
#include <iostream>


// Constructors
HttpStatus::HttpStatus(int code, const std::string& reason) :
    _code(code),
    _reason(reason)
{
}

HttpStatus::HttpStatus(const HttpStatus& rhs)
{
    if (this != &rhs)
    {
        _code = rhs._code;
        _reason = rhs._reason;
    }
}

HttpStatus::~HttpStatus()
{
}

HttpStatus& HttpStatus::operator=(const HttpStatus& rhs)
{
    if (this != &rhs)
    {
        _code = rhs._code;
        _reason = rhs._reason;
    }
    return *this;
}

// -----------------------------------------------------------------------------
//                                   Getters
// -----------------------------------------------------------------------------

int HttpStatus::getCode() const
{
    return _code;
}

// -----------------------------------------------------------------------------
//                                IO operators
// -----------------------------------------------------------------------------

std::istream &operator>>(std::istream &is, HttpStatus& status)
{
    is >> status._code;
    std::getline(is, status._reason);
    return is;
}

std::ostream &operator<<(std::ostream &out, const HttpStatus& status)
{
    out << status._code << " " << status._reason;
    return out;
}
