import numpy as np
import matplotlib.pyplot as plt

def analyse(filename):
    with open(filename) as f:
        data = f.read().split('\n')
    data = data[:-1]
    data = map(int, data)
    print np.mean(data), np.std(data)/np.mean(data)
    return

analyse("ntru_encrypt_c_0.txt")
analyse("ntru_encrypt_c_1.txt")
analyse("ntru_encrypt_c_2.txt")

analyse("ntru_encrypt_avx2_0.txt")
analyse("ntru_encrypt_avx2_1.txt")
analyse("ntru_encrypt_avx2_2.txt")

analyse("ntru_prime_c_0.txt")
analyse("ntru_prime_c_1.txt")
analyse("ntru_prime_c_2.txt")

analyse("ntru_prime_avx2_0.txt")
analyse("ntru_prime_avx2_1.txt")
analyse("ntru_prime_avx2_2.txt")