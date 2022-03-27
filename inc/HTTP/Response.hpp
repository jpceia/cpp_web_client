/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 15:40:09 by jceia             #+#    #+#             */
/*   Updated: 2022/03/27 02:40:05 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include "HTTP/Status.hpp"
# include "HTTP/Message.hpp"
# include <iostream>
# include <map>

class HttpResponse : virtual public HttpMessage
{

public:
    HttpResponse();
    HttpResponse(const HttpResponse& rhs);
    virtual ~HttpResponse();
    HttpResponse &operator=(const HttpResponse &rhs);

    // Setters
    void setStatus(const HttpStatus& status);
    void setBody(const std::string& body);
    using HttpMessage::setBody;

    // Helpers
    void printStart() const;

    // IO operators
    friend std::ostream &operator<<(std::ostream &out, const HttpResponse &request);

protected:

    HttpStatus _status;
};


#endif
