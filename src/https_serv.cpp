/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/19 01:16:09 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/19 03:41:56 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <openssl/ssl.h> /* core library */
#include <openssl/err.h> /* errors */
#include <openssl/x509.h> /* X509 */
#include <openssl/x509_vfy.h> /* X509_STORE_CTX */
#include <openssl/x509v3.h> /* X509V3_EXT_print */

#include <string.h> /* strlen */

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

void print_san_name(const char* label, X509* const cert)
{
    int success = 0;
    GENERAL_NAMES* names = NULL;
    unsigned char* utf8 = NULL;

    do
    {
        if(!cert)
            break; /* failed */

        names = (GENERAL_NAMES*)X509_get_ext_d2i(cert, NID_subject_alt_name, 0, 0 );
        if(!names) break;

        int i = 0, count = sk_GENERAL_NAME_num(names);
        if(!count) break; /* failed */

        for( i = 0; i < count; ++i )
        {
            GENERAL_NAME* entry = sk_GENERAL_NAME_value(names, i);
            if(!entry) continue;

            if(GEN_DNS == entry->type)
            {
                int len1 = 0, len2 = -1;

                len1 = ASN1_STRING_to_UTF8(&utf8, entry->d.dNSName);
                if(utf8) {
                    len2 = (int)strlen((const char*)utf8);
                }

                if(len1 != len2) {
                    fprintf(stderr, "  Strlen and ASN1_STRING size do not match (embedded null?): %d vs %d\n", len2, len1);
                }

                /* If there's a problem with string lengths, then     */
                /* we skip the candidate and move on to the next.     */
                /* Another policy would be to fails since it probably */
                /* indicates the client is under attack.              */
                if(utf8 && len1 && len2 && (len1 == len2)) {
                    fprintf(stdout, "  %s: %s\n", label, utf8);
                    success = 1;
                }

                if(utf8) {
                    OPENSSL_free(utf8), utf8 = NULL;
                }
            }
            else
            {
                fprintf(stderr, "  Unknown GENERAL_NAME type: %d\n", entry->type);
            }
        }

    } while (0);

    if(names)
        GENERAL_NAMES_free(names);

    if(utf8)
        OPENSSL_free(utf8);

    if(!success)
        fprintf(stdout, "  %s: <not available>\n", label);

}

void print_cn_name(const char* label, X509_NAME* name)
{
	int idx = -1;
	unsigned char *utf8 = NULL;
	X509_NAME_ENTRY* entry;
	ASN1_STRING* data;

	if (!name)
		return;

	idx = X509_NAME_get_index_by_NID(name, NID_commonName, -1);
	if (idx < 0)
		return;

	entry = X509_NAME_get_entry(name, idx);
	if (!entry)
		return;

	data = X509_NAME_ENTRY_get_data(entry);
	if (!data)
		return;

	if (!ASN1_STRING_to_UTF8(&utf8, data) || !utf8)
		return;

	printf("%s: %s\n", label, utf8);
	OPENSSL_free(utf8);
}


int verify_callback(int preverify, X509_STORE_CTX* x509_ctx)
{
    int depth = X509_STORE_CTX_get_error_depth(x509_ctx);
    //int err = X509_STORE_CTX_get_error(x509_ctx);
    
    X509* cert = X509_STORE_CTX_get_current_cert(x509_ctx);
    X509_NAME* iname = cert ? X509_get_issuer_name(cert) : NULL;
    X509_NAME* sname = cert ? X509_get_subject_name(cert) : NULL;

    if (cert)
    {
        print_cn_name("Issuer (cn)", iname);
        print_cn_name("Subject (cn)", sname);
    }

    if(depth == 0) {
        /* If depth is 0, its the server's certificate. Print the SANs too */
        print_san_name("Subject (san)", cert);
    }

    return preverify;
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

    /* Cannot fail ??? */
    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, verify_callback);

    /* Cannot fail ??? */
    SSL_CTX_set_verify_depth(ctx, 4);

    return ctx;
}

void configure_context(SSL_CTX *ctx, KeyPair *key_pair)
{
    SSL_CTX_set_ecdh_auto(ctx, 1);
    
    /* Set the key and cert */
    if (SSL_CTX_use_certificate_file(ctx, key_pair->cert.c_str(), SSL_FILETYPE_PEM) <= 0)
    {
        std::cerr << "Unable to load certificate file" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (SSL_CTX_use_PrivateKey_file(ctx, key_pair->key.c_str(), SSL_FILETYPE_PEM) <= 0 )
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
    std::cout << "Listening on port " << port << std::endl;
    
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
    //int ret = SSL_accept(ssl);
    SSL_set_accept_state(ssl);
    int ret = SSL_do_handshake(ssl);
    if (ret != 1)
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
