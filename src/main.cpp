/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/19 01:16:09 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/19 02:27:17 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <openssl/ssl.h> /* core library */
#include <openssl/err.h> /* errors */

// https://stackoverflow.com/questions/16255323/make-an-https-request-using-sockets-on-linux
// https://stackoverflow.com/questions/61969517/tcp-server-client-using-openssl
// https://opensource.com/article/19/6/cryptography-basics-openssl-part-1
// https://opensource.com/article/19/6/cryptography-basics-openssl-part-2
// https://github.com/z4pu/tcp_to_tls

#define BUFF_SIZE 1024

struct KeyPair {
    std::string key;
    std::string cert;
};

void init_openssl()
{
    SSL_load_error_strings();
    SSL_library_init();
    // OpenSSL_add_all_algorithms();
    OpenSSL_add_ssl_algorithms();
}

void cleanup_openssl()
{
    EVP_cleanup();
}

SSL_CTX *create_context()
{
    const SSL_METHOD *method;
    SSL_CTX *ctx;

    //method = SSLv23_client_method();
    method = SSLv23_server_method();

    ctx = SSL_CTX_new(method);
    if (!ctx)
    {
        std::cerr << "Unable to create SSL context" << std::endl;
        exit(EXIT_FAILURE);
    }

    return ctx;
}

void configure_context(SSL_CTX *ctx)
{
    SSL_CTX_set_ecdh_auto(ctx, 1);
    
    /* Set the key and cert */
    if (SSL_CTX_use_certificate_file(ctx, "certificate.crt", SSL_FILETYPE_PEM) <= 0)
    {
        std::cerr << "Unable to load certificate file" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (SSL_CTX_use_PrivateKey_file(ctx, "privateKey.key", SSL_FILETYPE_PEM) <= 0 )
    {
        std::cerr << "Unable to load private key file" << std::endl;
        exit(EXIT_FAILURE);
    }
}

int create_listener(int port)
{
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
    {
        std::cerr << "Could not create socket" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        std::cerr << "Could not bind socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (listen(fd, SOMAXCONN) < 0)
    {
        std::cerr << "Could not listen on socket" << std::endl;
        exit(EXIT_FAILURE);
    }
    return fd;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: ./webserv <port>" << std::endl;
        exit(EXIT_FAILURE);
    }

    int port;
    std::stringstream ss(argv[1]);
    ss >> port;

    int sock = create_listener(port);
    
    // accept connections
    struct sockaddr_in cli_addr;
    socklen_t cli_len = sizeof(cli_addr);
    int connection = accept(sock, (struct sockaddr *)&cli_addr, &cli_len);
    if (connection < 0)
    {
        std::cerr << "Could not accept connection" << std::endl;
        exit(EXIT_FAILURE);
    }

    init_openssl();
    SSL_CTX *ctx = create_context();
    KeyPair key_pair;
    key_pair.key = "certs/privateKey.key";
    key_pair.cert = "certs/certificate.crt";
    configure_context(ctx, &key_pair);
    SSL *ssl = SSL_new(ctx);
    SSL_set_fd(ssl, connection);
    if (SSL_accept(ssl) != 1)
    {
        std::cerr << "Could not accept SSL connection" << std::endl;
        exit(EXIT_FAILURE);
    }

    // read from the socket
    char buffer[BUFF_SIZE];
    if (SSL_read(ssl, buffer, sizeof(buffer)) < 0)
    {
        std::cerr << "Could not read from socket" << std::endl;
        return -1;
    }
    std::cout << buffer << std::endl;
    // write to the socket
    std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello World</h1></body></html>";
    if (SSL_write(ssl, response.c_str(), response.length()) < 0)
    {
        std::cerr << "Could not write to socket" << std::endl;
        return -1;
    }
    // close the connection
    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(connection);

    // close the listening socket
    close(sock);
    SSL_CTX_free(ctx);
    cleanup_openssl();
    return 0;
}
