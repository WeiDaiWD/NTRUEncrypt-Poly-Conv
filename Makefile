Compile = g++

HEADERS = results.h measure.h

all: test_ntru_encrypt_c test_ntru_prime_c test_ntru_encrypt_avx2 test_ntru_prime_avx2

test_ntru_encrypt_c: test_ntru_encrypt_c.cpp ntru_encrypt/c/*.cpp ntru_encrypt/c/simd_poly.h $(HEADERS)
	$(Compile) test_ntru_encrypt_c.cpp ntru_encrypt/c/*.cpp -std=c++11 -O3 -o test_ntru_encrypt_c

test_ntru_encrypt_avx2: test_ntru_encrypt_avx2.cpp ntru_encrypt/avx2/*.cpp ntru_encrypt/avx2/simd_poly.h $(HEADERS)
	$(Compile) test_ntru_encrypt_avx2.cpp ntru_encrypt/avx2/*.cpp -std=c++11 -O3 -mavx2 -o test_ntru_encrypt_avx2

test_ntru_prime_c: test_ntru_prime_c.cpp ntru_prime/c/*.c ntru_prime/c/*.h $(HEADERS)
	$(Compile) test_ntru_prime_c.cpp ntru_prime/c/*.c /usr/local/lib/nacl/libnacl.a /usr/local/lib/nacl/randombytes.o -std=c++11 -O3 -I/usr/local/include/nacl -o test_ntru_prime_c

test_ntru_prime_avx2: test_ntru_prime_avx2.cpp ntru_prime/avx2/*.c ntru_prime/avx2/*.h $(HEADERS)
	$(Compile) test_ntru_prime_avx2.cpp ntru_prime/avx2/*.c /usr/local/lib/nacl/libnacl.a /usr/local/lib/nacl/randombytes.o -O3 -mavx2 -mfma -I/usr/local/include/nacl -o test_ntru_prime_avx2

clean:
	rm test_ntru_encrypt_c test_ntru_prime_c test_ntru_encrypt_avx2 test_ntru_prime_avx2
