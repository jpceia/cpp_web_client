/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Method.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/20 02:16:05 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/27 00:11:49 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPMETHOD_HPP
# define HTTPMETHOD_HPP

# include <iostream>

enum HttpMethod {
    GET,
    POST,
    PUT,
    DELETE,
    HEAD,
    CONNECT,
    OPTIONS,
    TRACE,
    PATCH
};

std::istream &operator>>(std::istream &is, HttpMethod &method);
std::ostream &operator<<(std::ostream &out, const HttpMethod &method);

#endif
