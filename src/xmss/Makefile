CC := gcc
CFLAGS := -Wall -Wextra -Wpedantic -pedantic-errors -fPIC -g -gdwarf-2

LIB := libxmss.a
TEST := xmss_test
TEST_EVP := makesk_evp
TEST_EVP_P := makepk_evp
TEST_CERT := makecert
TEST_CERT_VERIF := verifycert
TEST_SSL_S := ssl_server
TEST_SSL_C := ssl_client
OBJ := hash_draft.o hfas_draft.o KeccakP-1600-reference.o KeccakSponge.o prf_draft.o SimpleFIPS202.o wots_draft.o xmss_draft.o xmssmt_draft.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

all: $(LIB) $(TEST) $(TEST_EVP) $(TEST_EVP_P) $(TEST_CERT) $(TEST_CERT_VERIF) $(TEST_SSL_S) $(TEST_SSL_C)

$(LIB): $(OBJ)
	ar rc $(LIB) $(OBJ)

$(TEST): xmss_test.o $(LIB)
	$(CC) xmss_test.o -o $(TEST) $(LIB) ../../liboqs.a ../../../../libcrypto.a -lm -ldl

$(TEST_EVP): makesk_evp.o $(LIB)
	$(CC) makesk_evp.o -o makesk_evp $(LIB) ../../liboqs.a ../../../../libcrypto.a -lm -ldl -fPIC

$(TEST_EVP_P): makepk_evp.o $(LIB)
	$(CC) makepk_evp.o -o makepk_evp $(LIB) ../../liboqs.a ../../../../libcrypto.a -lm -ldl -fPIC

$(TEST_CERT): makecert.o $(LIB)
	$(CC) makecert.o -o makecert $(LIB) ../../liboqs.a ../../../../libcrypto.a -lm -ldl -fPIC

$(TEST_CERT_VERIF): verifycert.o $(LIB)
	$(CC) verifycert.o -o verifycert $(LIB) ../../liboqs.a ../../../../libcrypto.a -lm -ldl -fPIC

$(TEST_SSL_S): ssl_server.o $(LIB)
	$(CC) ssl_server.o -o ssl_server $(LIB)  ../../../../libssl.a ../../../../libcrypto.a ../../liboqs.a  -lm -ldl -fPIC

$(TEST_SSL_C): ssl_client.o $(LIB)
	$(CC) ssl_client.o -o ssl_client $(LIB)  ../../../../libssl.a ../../../../libcrypto.a ../../liboqs.a -lm -ldl -fPIC

.PHONY: clean
clean:
	rm -rf $(LIB) $(OBJ) $(TEST) xmss_test.o makesk_evp.o makesk_evp makepk_evp.o makepk_evp makecert.o makecert verifycert.o verifycert sk_xmss.pem pk_xmss.pem cert_xmss.pem ssl_server.o ssl_server ssl_client.o ssl_client

