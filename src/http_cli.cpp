/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_cli.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/20 11:43:02 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/28 07:29:28 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>
#include <iostream>
#include "TCP/Socket.hpp"
#include "HTTP/Connection.hpp"
#include "HTTP/Request.hpp"
#include "HTTP/Response.hpp"
#include "SSL/Context.hpp"

#include <sstream>
#include <iostream>



int main(int argc, char *argv[])
{
    // Now we're taking an ipaddress and a port number as arguments to our program
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <domain> <port>" << std::endl;
        return 1;
    }

    std::string host(argv[1]);
    int port;
    {
        std::stringstream ss(argv[2]);
        ss >> port;
    }
    
    TcpSocket socket;
    SslContext ctx("certs/certificate.crt", "certs/privateKey.key");
    HttpConnection conn = socket.connect(host, port);
    
    // Send a GET request to the server
    HttpRequest request;
    request.setHeader("Accept", "*/*");
    request.setHeader("Accept-Encoding", "gzip, deflate, br");
    request.setHeader("Cache-Control", "max-age=0");
    request.setHeader("Connection", "keep-alive");
    request.setHeader("User-Agent", "ft-cli");
    request.setHeader("Host", host);

    conn.sendRequest(request);

    HttpResponse response = conn.recvResponse();
    std::cout << response << std::endl;

    return 0;
}
