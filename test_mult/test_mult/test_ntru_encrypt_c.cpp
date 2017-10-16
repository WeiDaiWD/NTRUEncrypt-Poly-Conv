#include "measure.h"
#include "results.h"
#include "ntru_encrypt/simd_poly.h"

class Data {
public:
  inline Data() {
    a = (uint16_t *)malloc(sizeof(uint16_t) * 2 * n);
    b = (uint16_t *)malloc(sizeof(uint16_t) * 2 * n);
    t = (uint16_t *)malloc(sizeof(uint16_t) * 4 * n);
    r = (uint16_t *)malloc(sizeof(uint64_t) * 4 * n);
  };
  inline ~Data() {
    free(a);
    free(b);
    free(r);
    free(t);
  }

  inline void SetZero() {
    memset(a, 0, sizeof(uint16_t) * 2 * n);
    memset(b, 0, sizeof(uint16_t) * 2 * n);
    memset(t, 0, sizeof(uint16_t) * 4 * n);
    memset(r, 0, sizeof(uint16_t) * 4 * n);
  }

  inline void Rand(uint16_t *x, uint16_t N) {
    for (int i = 0; i < N; i ++)
      x[i] = rand() & 0x07ff;
  }

  inline void Randa() { Rand(a, n);}

  inline void Randb() { Rand(b, n);}

  inline void PolyMul() {
    karatsuba_toom4(r, t, a, b, n);
    //__mm256i_karatsuba__mm256_toom4(r, t, a, b, n);
  }

  static const uint16_t n = 768;
  uint16_t *a;//[2 * n];
  uint16_t *b;//[2 * n];
  uint16_t *r;//[4 * n];
  uint16_t *t;//[4 * n];

};

int main() {
  Measure<Data>(ntru_encrypt_c_results);
  Plot(ntru_encrypt_c_results);
  return 0;
}
