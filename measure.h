#include <stdlib.h>
#include <fstream>
#include <string.h>
#include <stdint.h>
using namespace std;
/*
inline uint64_t rdtsc() {
  unsigned int lo,hi;
  __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
  return ((uint64_t)hi << 32) | lo;
}
#define start() { asm volatile ("mfence"); startc = rdtsc();}
#define stop() { asm volatile ("mfence"); stopc = rdtsc();}
*/
inline uint64_t rdtscp() {
  unsigned int lo,hi;
  __asm__ __volatile__ ("rdtscp" : "=a" (lo), "=d" (hi));
  return ((uint64_t)hi << 32) | lo;
}
#define start() { startc = rdtscp();}
#define stop() { stopc = rdtscp();}

static const uint32_t count = 10000;

template <class Data>
void Measure(const char *file) {
  Data data;
  uint64_t startc, stopc;
  ofstream fout;

  data.GenConsta();
  data.GenConstb();
  fout.open(file);
  for (uint32_t i = 0; i < count; i ++) {
    data.GenRanda(); data.GenRandb();
    data.Consta(); data.Constb();
    data.PolyMul(); // extra warm up
    start(); data.PolyMul(); stop();
    fout<< stopc - startc <<endl;
    data.Randb();
    data.PolyMul(); // extra warm up
    start(); data.PolyMul(); stop();
    fout<< stopc - startc <<endl;
    /////////////////
    // Zhenfei !!! //////////////////////////////////////
    // remove the following measurement for NTRU Prime //
    data.Randa();
    data.PolyMul(); // extra warm up
    start(); data.PolyMul(); stop();
    fout<< stopc - startc <<endl;
    /////////////////////////////////////////////////////
  }
  fout.close();

/*  data.Randa();
  data.Randb();
  // warm up
  fout.open(files[3]);
  for (uint32_t j = 0; j < outer; j ++) {
  for (uint32_t i = 0; i < count; i ++) {
    data.Randa();
    data.Randb();
    data.PolyMul(); // extra warm up
    start();
    data.PolyMul();
    stop();
    fout<< stopc - startc <<endl;
  }}
  fout.close();

  // fixed * fixed
  fout.open(files[0]);
  for (uint32_t j = 0; j < outer; j ++) {
  for (uint32_t i = 0; i < count; i ++) {
    data.PolyMul(); // extra warm up
    start();
    data.PolyMul();
    stop();
    fout<< stopc - startc <<endl;
  }}
  fout.close();

  // fixed * rand
  fout.open(files[1]);
  for (uint32_t j = 0; j < outer; j ++) {
  for (uint32_t i = 0; i < count; i ++) {
    data.Randb();
    data.PolyMul(); // extra warm up
    start();
    data.PolyMul();
    stop();
    fout<< stopc - startc <<endl;
  }}
  fout.close();

  // rand * rand
  fout.open(files[2]);
  for (uint32_t j = 0; j < outer; j ++) {
  for (uint32_t i = 0; i < count; i ++) {
    data.Randa();
    data.Randb();
    data.PolyMul(); // extra warm up
    start();
    data.PolyMul();
    stop();
    fout<< stopc - startc <<endl;
  }}
  fout.close();*/
}

void Plot(const char *files[]) {
/*  FILE * gnuplot = popen ("gnuplot -p", "w");
  fprintf(gnuplot, "set term png\n");
  fprintf(gnuplot, "set output \"%s\"\n", files[4]);
  char cmd[200];
  fprintf(gnuplot, "plot '%s' with points, '%s' with impulses, '%s' with lines\n", files[0], files[1], files[2]);
  fflush(gnuplot);*/
}
