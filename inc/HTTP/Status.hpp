/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Status.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 00:10:54 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/27 00:11:25 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPSTATUS_HPP
# define HTTPSTATUS_HPP

# include <map>
# include <iostream>

class HttpStatus
{
public:
    HttpStatus(int code = 200, const std::string& reason = "OK");
    HttpStatus(const HttpStatus& rhs);
    virtual ~HttpStatus();
    HttpStatus &operator=(const HttpStatus &rhs);

    // Getters
    int getCode() const;

    // IO operators
    friend std::ostream &operator<<(std::ostream &out, const HttpStatus& status);

private:
    // Private members
    int _code;
    std::string _reason;
};

// IO operators
std::ostream &operator<<(std::ostream &out, const HttpStatus& status);

#endif
