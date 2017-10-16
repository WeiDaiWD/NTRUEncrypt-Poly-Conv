#include "ntru_prime/ref/rq.h"

class Data {
public:
  inline Data() {};
  inline ~Data() {};

  inline void SetZero() {
    memset(a, 0, sizeof(modq) * n);
    memset(b, 0, sizeof(small) * n);
    memset(r, 0, sizeof(modq) * n);
  }

  inline void Randa() {
    small aa[n];
    small_random_weightw(aa);
    rq_recip3(a, aa);
  };

  inline void Randb() {
    do
      small_random(b);
    while (r3_recip(bb, b) != 0);
  }

  inline void PolyMul() {
    rq_mult(r, a, b);
  }

  static const int n = 761;
  modq a[n]; // input a
  small b[n]; // input b
  modq r[n]; // output r = a * b

};
