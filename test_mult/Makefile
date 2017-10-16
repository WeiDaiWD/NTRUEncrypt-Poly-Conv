Compile = g++

HEADERS = results.h measure.h

all: test_ntru_encrypt_c

test_ntru_encrypt_c: test_ntru_encrypt_c.cpp ntru_encrypt/*.cpp ntru_encrypt/simd_poly.h $(HEADERS)
	$(Compile) test_ntru_encrypt_c.cpp ntru_encrypt/*.cpp -O3 -mavx2 -o test_ntru_encrypt_c

clean:
	rm test_ntru_encrypt_c
