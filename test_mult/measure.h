#include <stdlib.h>
#include <fstream>
#include <string.h>
#include <stdint.h>
using namespace std;

inline uint64_t rdtsc() {
  unsigned int lo,hi;
  __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
  return ((uint64_t)hi << 32) | lo;
}

#define start() { asm volatile ("mfence"); startc = rdtsc();}
#define stop() { asm volatile ("mfence"); stopc = rdtsc();}

template <class Data>
void Measure(const char *files[]) {
  Data data;
  uint64_t startc, stopc;
  uint16_t count = 10;
  uint64_t sum[4] = {0, 0, 0, 0};
  ofstream fout;

  // warm up
  fout.open(files[3]);
  for (int i = 0; i < count; i ++) {
    data.Randa();
    data.Randb();
    data.PolyMul(); // extra warm up
    start();
    data.PolyMul();
    stop();
    sum[3] += stopc - startc;
    fout<< stopc - startc <<endl;
  }
  fout.close();

  // fixed * fixed
  fout.open(files[0]);
  for (int i = 0; i < count; i ++) {
    data.PolyMul(); // extra warm up
    start();
    data.PolyMul();
    stop();
    sum[0] += stopc - startc;
    fout<< stopc - startc <<endl;
  }
  fout.close();

  // fixed * rand
  fout.open(files[1]);
  for (int i = 0; i < count; i ++) {
    data.Randb();
    data.PolyMul(); // extra warm up
    start();
    data.PolyMul();
    stop();
    sum[1] += stopc - startc;
    fout<< stopc - startc <<endl;
  }
  fout.close();

  // rand * rand
  fout.open(files[2]);
  for (int i = 0; i < count; i ++) {
    data.Randa();
    data.Randb();
    data.PolyMul(); // extra warm up
    start();
    data.PolyMul();
    stop();
    sum[2] += stopc - startc;
    fout<< stopc - startc <<endl;
  }
  fout.close();
}

void Plot(const char *files[]) {
  char cmd[100];
  sprintf(cmd, "gnuplot -p '%s' with points, '%s' with points pt 7 rgb '#0060ad', '%s' with points pt 5 rgb '#dd181f'", files[0], files[1], files[2]);
  system(cmd);
}
