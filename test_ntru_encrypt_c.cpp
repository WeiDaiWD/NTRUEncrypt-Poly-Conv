#include "measure.h"
#include "results.h"
#include "ntru_encrypt/c/simd_poly.h"

class Data {
public:
  inline Data() {
    pool = (char *)malloc(sizeof(uint16_t) * 20 * n + 64);
    a = (uint16_t *)&pool[32 - (((uint64_t)pool & 0x3f) >> 1)];
    b = (uint16_t *)&a[2 * n];
    t = (uint16_t *)&b[2 * n];
    r = (uint16_t *)&t[4 * n];
    const_a = (uint16_t *)&r[4 * n];
    rand_a = (uint16_t *)&const_a[2 * n];
    const_b = (uint16_t *)&rand_a[2 * n];
    rand_b = (uint16_t *)&const_b[2 * n];
  };

  inline ~Data() { free(pool); }

  inline void SetZero() { memset(pool, 0, sizeof(uint16_t) * 12 * n); }

  inline void Rand(uint16_t *x, uint16_t N) {
    for (int i = 0; i < N; i ++)
      x[i] = rand() & 0x07ff;
  }
  inline void GenRanda() { Rand(rand_a, n);}
  inline void GenRandb() { Rand(rand_b, n);}
  inline void GenConsta() { Rand(const_a, n);}
  inline void GenConstb() { Rand(const_b, n);}
  inline void Randa() { memcpy(a, rand_a, sizeof(uint16_t) * n); }
  inline void Randb() { memcpy(b, rand_b, sizeof(uint16_t) * n); }
  inline void Consta() { memcpy(a, const_a, sizeof(uint16_t) * n); }
  inline void Constb() { memcpy(b, const_b, sizeof(uint16_t) * n); }

  inline void PolyMul() {
    karatsuba_toom4(r, t, a, b, n);
  }

  static const uint16_t n = 768;
  uint16_t *a;//[2 * n];
  uint16_t *b;//[2 * n];
  uint16_t *r;//[4 * n];
  uint16_t *t;//[4 * n];
  uint16_t *const_a;
  uint16_t *rand_a;
  uint16_t *const_b;
  uint16_t *rand_b;
  char *pool;
};

int main() {
  Measure<Data>("ntru_encrypt_c.txt");
  return 0;
}
