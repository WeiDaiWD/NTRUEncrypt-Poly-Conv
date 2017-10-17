# NTRUEncrypt-Poly-Conv
Experiments on constant-time polynomial multiplication for NTRUEncrypt

## Dependencies
- NTRU Prime relies on NaCL library. To install:
1. `cd nacl-20110221 && ./do`
1. `sudo mkdir /usr/local/include/nacl /usr/local/lib/nacl`
1. `sudo cp nacl-20110221/build/$(name)/include/amd64/* /usr/local/include/nacl`
1. `sudo cp nacl-20110221/build/$(name)/lib/amd64/* /usr/local/lib/nacl`

- NTRU Prime also uses FMA instructions besides AVX2 instructions.

- My cpp code calls "gnuplot" to generate ".png" plots.

## Compile
`make` outputs 4 executables. Each of them takes 1000 measurements of all 3 scenarios (random or fixed input) for a specific code and optimization. Optimization level `-O3` is always enabled.

## Files

- ".txt" records measurements. Appendix "0" means fixed input polynomials. Appendix "1" means one random one fixed. Appendix "2" means both random. Appendix "3" should be ignored.

- ".png" plots the measurements. They now look unclear. Please edit "measure.h" to your needs.

- In "ntru_encrypt", I separate AVX2 and C codes. When they are compiled together, I had "ILLEGAL INSTRUCTION" error when running C code with -O3, which disappears with -O2.
