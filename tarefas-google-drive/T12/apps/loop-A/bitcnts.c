/* +++Date last modified: 05-Jul-1997 */

/*
**  BITCNTS.C - Test program for bit counting functions
**
**  public domain by Bob Stout & Auke Reitsma
*/

#include "bitops.h"
#include "conio.h"
#include <float.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "ompuseclause.h"

#define FUNCS 7

static int CDECL bit_shifter(long int x);

int main1(int argc, char *argv[], int print, double *run_time_acc) {

  struct timeval start_time, end_time;
  long i;
  long j, n, seed;
  int iterations;
  static int (*CDECL pBitCntFunc[FUNCS])(long) = {
      bit_count, bitcount, ntbl_bitcnt, ntbl_bitcount,
      // DOESNT WORK
      // btbl_bitcnt,
      BW_btbl_bitcount, AR_btbl_bitcount, bit_shifter};
  static char *text[FUNCS] = {
      "Optimized 1 bit/loop counter", "Ratko's mystery algorithm",
      "Recursive bit count by nybbles", "Non-recursive bit count by nybbles",
      //"Recursive bit count by bytes",
      "Non-recursive bit count by bytes (BW)",
      "Non-recursive bit count by bytes (AR)", "Shift and count bits"};

  if (argc < 2) {
    fprintf(stderr, "Usage: bitcnts <iterations>\n");
    exit(EXIT_FAILURE);
  }
  iterations = atoi(argv[1]);

  if (print) {
    puts("Bit counter algorithm benchmark\n");
  }

  for (i = 0; i < FUNCS; i++) {
    unsigned int rSeed = i;
    int random = rand_r(&rSeed);
    n = 0;
    gettimeofday(&start_time, NULL);
#pragma omp parallel for private(seed) check
    for (j = 0; j < iterations; j++) {
      int temp = 0;
      seed = 13 * j + random;
      temp += pBitCntFunc[i](seed);
      n += temp;
    }
    gettimeofday(&end_time, NULL);
    double t = (end_time.tv_sec - start_time.tv_sec) * 1.0e6 +
               (end_time.tv_usec - start_time.tv_usec);
    (*run_time_acc) += t;

    if (print) {
      printf("%-38s> Bits: %ld\n", text[i], n);
    }
  }
  return 0;
}

static int CDECL bit_shifter(long int x) {
  int i, n;

  for (i = n = 0; x && (i < (sizeof(long) * CHAR_BIT)); ++i, x >>= 1) {
    n += (int)(x & 1L);
  }
  return n;
}
