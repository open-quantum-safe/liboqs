#include "xmss.c"

#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/err.h>

int create_socket(int port)
{
    int s;
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
	perror("Unable to create socket!");
	exit(EXIT_FAILURE);
    }

    if (bind(s, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
	perror("Unable to bind!");
	exit(EXIT_FAILURE);
    }

    if (listen(s, 1) < 0) {
	perror("Unable to listen!");
	exit(EXIT_FAILURE);
    }

    return s;
}

void init_openssl()
{ 
    SSL_load_error_strings();	
    OpenSSL_add_ssl_algorithms();
    load_xmss_all();
}

void cleanup_openssl()
{
    EVP_cleanup();
}

SSL_CTX *create_context()
{
    const SSL_METHOD *method;
    SSL_CTX *ctx;

    method = SSLv23_server_method();

    ctx = SSL_CTX_new(method);
    if (!ctx) {
	perror("Unable to create SSL context!");
	ERR_print_errors_fp(stderr);
	exit(EXIT_FAILURE);
    }
    const char *used_cipher = "RLWE-XMSS-AES256-GCM-SHA384"; // Use this cipher exclusively
    SSL_CTX_set_cipher_list(ctx,used_cipher);
    fprintf(stderr, "[ssl_server] Will use cipher %s.\n", used_cipher);
    return ctx;
}

X509 *read_certificate(char *path) {
        FILE *fp = fopen(path,"r"); 
        if (!fp) {  
                fprintf(stderr,"Unable to open file!\n");
                abort();            
        }           
	X509 *x509 = NULL;
	PEM_read_X509(fp, &x509, NULL, NULL);
	fprintf(stderr, "X509 = %p\n", (void *)x509);
	return x509;
}

void configure_context(SSL_CTX *ctx,char *keypath, char *certpath)
{

    /* Set the key and cert */
    if (SSL_CTX_use_certificate(ctx, read_certificate(certpath)) < 0) {
        ERR_print_errors_fp(stderr);
	exit(EXIT_FAILURE);
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, keypath, SSL_FILETYPE_PEM) < 0 ) {
        ERR_print_errors_fp(stderr);
	exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv)
{
    if (argc < 4) {
	printf("usage:\n\t %s <key> <cert> <port>\n",argv[0]);
	exit(1);
    }
    char *keypath = argv[1];
    char *certpath = argv[2];

    int sock;
    SSL_CTX *ctx;
    init_openssl();
    ctx = create_context();
    configure_context(ctx,keypath,certpath);
    sock = create_socket(atoi(argv[3]));

    /* Handle connections */
    while(1) {
        struct sockaddr_in addr;
        unsigned int len = sizeof(addr);
        SSL *ssl;
        const char reply[] = "test\n";
        int client = accept(sock, (struct sockaddr*)&addr, &len);
        if (client < 0) {
            perror("Unable to accept!");
            exit(EXIT_FAILURE);
        }

        ssl = SSL_new(ctx);
        SSL_set_fd(ssl, client);
        int SSL_accept_value = SSL_accept(ssl);
        if (SSL_accept_value <= 0) {
            ERR_print_errors_fp(stderr);
        }
        else {
            if (SSL_accept_value == 1) fprintf(stderr, "[ssl_server] TLS handshake successful.\n");
            SSL_write(ssl, reply, strlen(reply));
        }

        SSL_free(ssl);
        close(client);
    }

    close(sock);
    SSL_CTX_free(ctx);
    cleanup_openssl();
}
