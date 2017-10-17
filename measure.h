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
  uint16_t count = 1000;
  ofstream fout;

  data.Randa();
  data.Randb();
  // warm up
  fout.open(files[3]);
  for (int i = 0; i < count; i ++) {
//    data.PolyMul(); // extra warm up
    start();
    data.PolyMul();
    stop();
    fout<< stopc - startc <<endl;
  }
  fout.close();

  // fixed * fixed
  fout.open(files[0]);
  for (int i = 0; i < count; i ++) {
//    data.PolyMul(); // extra warm up
    start();
    data.PolyMul();
    stop();
    fout<< stopc - startc <<endl;
  }
  fout.close();

  // warm up
  fout.open(files[3]);
  for (int i = 0; i < count; i ++) {
    data.Randb();
//    data.PolyMul(); // extra warm up
    start();
    data.PolyMul();
    stop();
    fout<< stopc - startc <<endl;
  }
  fout.close();

  // fixed * rand
  fout.open(files[1]);
  for (int i = 0; i < count; i ++) {
    data.Randb();
//    data.PolyMul(); // extra warm up
    start();
    data.PolyMul();
    stop();
    fout<< stopc - startc <<endl;
  }
  fout.close();

  // warm up
  fout.open(files[3]);
  for (int i = 0; i < count; i ++) {
    data.Randa();
    data.Randb();
//    data.PolyMul(); // extra warm up
    start();
    data.PolyMul();
    stop();
    fout<< stopc - startc <<endl;
  }
  fout.close();

  // rand * rand
  fout.open(files[2]);
  for (int i = 0; i < count; i ++) {
    data.Randa();
    data.Randb();
//    data.PolyMul(); // extra warm up
    start();
    data.PolyMul();
    stop();
    fout<< stopc - startc <<endl;
  }
  fout.close();
}

void Plot(const char *files[]) {
  FILE * gnuplot = popen ("gnuplot -p", "w");
  fprintf(gnuplot, "set term png\n");
  fprintf(gnuplot, "set output \"%s\"\n", files[4]);
  char cmd[200];
  fprintf(gnuplot, "plot '%s' with points, '%s' with impulses, '%s' with lines\n", files[0], files[1], files[2]);
  fflush(gnuplot);
}
