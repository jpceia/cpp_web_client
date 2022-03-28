/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Context.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 07:09:39 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/28 07:10:23 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SSL/Context.hpp"
#include <openssl/ssl.h> /* core library */
#include <openssl/err.h> /* errors */
#include <openssl/x509.h> /* X509 */
#include <openssl/x509_vfy.h> /* X509_STORE_CTX */
#include <openssl/x509v3.h> /* X509V3_EXT_print */

SslContext::SslContext(const std::string &cert, const std::string &key)
{
    // Init SSL
    SSL_load_error_strings();
    SSL_library_init();
    // OpenSSL_add_all_algorithms();
    OpenSSL_add_ssl_algorithms();

    // Create context
    const SSL_METHOD *method = SSLv23_client_method();
    // method = SSLv23_server_method();

    _ctx = SSL_CTX_new(method);
    if (!_ctx)
    {
        ERR_print_errors_fp(stderr);
        throw std::runtime_error("Unable to create SSL context");
    }
    /* Cannot fail ??? */
    SSL_CTX_set_verify_depth(_ctx, 4);

    // Configure the context

    SSL_CTX_set_ecdh_auto(ctx, 1);
    
    /* Set the key and cert */
    if (SSL_CTX_use_certificate_file(_ctx, cert.c_str(), SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        throw std::runtime_error("Unable to load certificate file");
    }
    if (SSL_CTX_use_PrivateKey_file(_ctx, key.c_str(), SSL_FILETYPE_PEM) <= 0 )
    {
        ERR_print_errors_fp(stderr);
        throw std::runtime_error("Unable to load private key file");
    }
}

SslContext::SslContext(const SslContext &rhs)
{
    _ctx = rhs._ctx;
}

SslContext::~SslContext()
{
    SSL_CTX_free(_ctx);
    EVP_cleanup();
}

SslContext& SslContext::operator=(const SslContext &rhs)
{
    if (this != &rhs)
        _ctx = rhs._ctx;
    return *this;
}

SSL_CTX *SslContext::getContext() const
{
    return _ctx;
}
