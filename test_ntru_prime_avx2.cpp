#include "measure.h"
#include "results.h"
#include "ntru_prime/avx2/rq.h"
#include "ntru_prime/avx2/r3.h"

class Data {
public:
  inline Data() {
    pool = (char *)malloc(pool_size);
    a = (modq *)&pool[64 - ((uint64_t)pool & 0x3f)];
    b = (small *)&a[N];
    r = (modq *)&b[N];
    const_a = (modq *)&r[N];
    rand_a = (modq *)&const_a[N];
    const_b = (small *)&rand_a[N];
    rand_b = (small *)&const_b[N];
  };
  inline ~Data() { free(pool); };

  inline void SetZero() {
    memset(pool, 0, pool_size);
  }

  inline void Randmodq(modq *x) {
    small aa[n];
    small_random_weightw(aa);
    rq_recip3(x, aa);
  };

  inline void Randsmall(small *x) {
    small bb[n];
    do
      small_random(x);
    while (r3_recip(bb, x) != 0);
  }
  inline void GenRanda() { Randmodq(rand_a);}
  inline void GenRandb() { Randsmall(rand_b);}
  inline void GenConsta() { Randmodq(const_a);}
  inline void GenConstb() { Randsmall(const_b);}
  inline void Randa() { memcpy(a, rand_a, sizeof(modq) * n); }
  inline void Randb() { memcpy(b, rand_b, sizeof(small) * n); }
  inline void Consta() { memcpy(a, const_a, sizeof(modq) * n); }
  inline void Constb() { memcpy(b, const_b, sizeof(small) * n); }

  inline void PolyMul() {
    rq_mult(r, a, b);
  }

  static const int n = 761;
  modq *a; // input a
  small *b; // input b
  modq *r; // output r = a * b
  modq *const_a;
  modq *rand_a;
  small *const_b;
  small *rand_b;
  char *pool;
  static const uint32_t N = (n + 63) / 64 * 64; // for cache alignment
  static const size_t pool_size = sizeof(modq) * 4 * N + sizeof(small) * 3 * N + 64;
};

int main() {
  Measure<Data>("ntru_prime_avx2.txt");
  return 0;
}
