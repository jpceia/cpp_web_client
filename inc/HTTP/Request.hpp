/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 13:00:01 by jceia             #+#    #+#             */
/*   Updated: 2022/03/27 00:11:47 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include "HTTP/Message.hpp"
# include "HTTP/Method.hpp"
# include <iostream>
# include <map>

class HttpRequest : public HttpMessage
{
public:
    HttpRequest();
    HttpRequest(const HttpRequest& rhs);
    virtual ~HttpRequest();
    HttpRequest &operator=(const HttpRequest &rhs);

    // Getters
    std::string getPath() const;
    HttpMethod getMethod() const;
    std::string getQueryString() const;
    std::string getEndpoint() const;

    // Setters
    void setPath(const std::string& path);
    void setMethod(const std::string& method);

    // Helpers
    void printStart() const;

    // IO operators
    friend std::istream &operator>>(std::istream &is, HttpRequest &request);
    friend std::ostream &operator<<(std::ostream &out, const HttpRequest &request);
    
protected:
    HttpMethod _method;
    std::string _endpoint;
    std::string _query;
};

#endif
