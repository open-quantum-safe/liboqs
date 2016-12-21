
GCC = gcc -O3 -Wall -Wextra

all: *.c
	$(GCC) -c vscrypto_kex.c -I../../include
	$(GCC) -c kex_rlwe_vscrypto.c -I../../include
	$(GCC) -o fft_1024_8816641.o  -c fft_crt_MQ.c -DBIGMODULUS=1024 -DBIGPRIME=8816641
	$(GCC) -o fft_1024_17633281.o -c fft_crt_MQ.c -DBIGMODULUS=1024 -DBIGPRIME=17633281
	$(GCC) -o fft_2048_8816641.o  -c fft_crt_MQ.c -DBIGMODULUS=2048 -DBIGPRIME=8816641
	$(GCC) -o fft_2048_17633281.o -c fft_crt_MQ.c -DBIGMODULUS=2048 -DBIGPRIME=17633281
	$(GCC) -o ctx_337_32353.o -c ctx_prime_MQ.c -DMODULUS_M=337 -DPRIME_Q=32353
	$(GCC) -o ctx_433_35507.o -c ctx_prime_MQ.c -DMODULUS_M=433 -DPRIME_Q=35507
	$(GCC) -o ctx_541_41117.o -c ctx_prime_MQ.c -DMODULUS_M=541 -DPRIME_Q=41117
	$(GCC) -o ctx_631_44171.o -c ctx_prime_MQ.c -DMODULUS_M=631 -DPRIME_Q=44171
	$(GCC) -o ctx_739_47297.o -c ctx_prime_MQ.c -DMODULUS_M=739 -DPRIME_Q=47297
	$(GCC) -o ctx_821_49261.o -c ctx_prime_MQ.c -DMODULUS_M=821 -DPRIME_Q=49261
	$(GCC) -o ctx_256_15361.o -c ctx_pot_MQ.c -DMODULUS_M=256 -DPRIME_Q=15361
	$(GCC) -o ctx_512_25601.o -c ctx_pot_MQ.c -DMODULUS_M=512 -DPRIME_Q=25601
	$(GCC) -o ctx_1024_40961.o -c ctx_pot_MQ.c -DMODULUS_M=1024 -DPRIME_Q=40961
	$(GCC) -o set_ring_ctx.o -c set_ring_ctx.c

clean:
	rm -f *.o

