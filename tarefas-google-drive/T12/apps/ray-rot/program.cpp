/*
 * Copyright (C) 2013 Michael Andersch <michael.andersch@mailbox.tu-berlin.de>
 *
 * This file is part of Starbench.
 *
 * Starbench is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Starbench is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Starbench.  If not, see <http://www.gnu.org/licenses/>.
 */

/**********************************************************************************
                                INCLUDES & DEFINES
*************************************************************************************/
#include "ray_engine.h"
#include "rotation_engine.h"
#include <ompuseclause.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define BAD_EXIT -1;
#define TIME(x) gettimeofday(&x, NULL)

typedef struct timeval timer;
using namespace std;

string srcfiles[100], destfiles[100];
string ifiles, ofiles;
int angle;
unsigned int xres = 1024;
unsigned int yres = 768;
unsigned int rpp = 1;
int qtdFiles = 0;

/**********************************************************************************
                                FUNCTION PROTOTYPES
*************************************************************************************/
static double timevaldiff(timer *start, timer *finish);
string *convertToString(char **in, size_t size);
bool parseArgs(string *args, int &angle, unsigned int &xres, unsigned int &yres,
               unsigned int &rpp, string &inname, string &outname);

/* GLOBAL VARIABLES */
string usage =
    "Usage: ./ray-rot <infile> <outfile> <angle> <xres> <yres> <RPP>\n\n"
    "infile:      input file\n"
    "outfile:     output file\n"
    "angle:       angle to be rotated\n"
    "xres:        horizontal resolution\n"
    "yres:        vertical resolution\n"
    "RPP:        rays shot per pixel\n";

string p_name = "--- StarBENCH - ray-rot Workload ---\n";

/*
*	Function: main
*	--------------
*	The program main function.
*/
int main(int argc, char *argv[]) {
  cout << p_name;

  if (argc != 7) {
    cerr << usage;
    return BAD_EXIT;
  }

  string *args = convertToString(argv, argc);
  if (!parseArgs(args, angle, xres, yres, rpp, ifiles, ofiles)) {
    cerr << usage;
    return BAD_EXIT;
  }
  delete[] args;

  FILE *ifp = fopen(ifiles.c_str(), "r");
  FILE *ofp = fopen(ofiles.c_str(), "r");
  char iname[256], oname[256];

  while (fscanf(ifp, " %s", iname) != EOF) {
    fscanf(ofp, " %s", oname);

    srcfiles[qtdFiles] = iname;
    destfiles[qtdFiles] = oname;
    qtdFiles++;
    printf("%s and %s\n", iname, oname);
  }

  fclose(ifp);
  fclose(ofp);

  int exec = 1;
  int i;
  timer loop_time_start, loop_time_end;

  TIME(loop_time_start);
#pragma omp parallel for firstprivate(srcfiles, xres, yres, rpp, angle,        \
                                      destfiles) check
  for (i = 0; i < qtdFiles; i++) {
    RotateEngine *re = new RotateEngine;
    RayEngine *ra = new RayEngine;

    if (exec) {
      if (!ra->init(srcfiles[i], xres, yres, rpp)) {
        cerr << "Raytracing Kernel Init failed!" << endl;
        exec = 0;
        continue;
      }

      if (!re->init(ra->getOutputImage(), angle, destfiles[i])) {
        cerr << "Rotation Kernel Init failed!" << endl;
        exec = 0;
        continue;
      }
    }
    if (exec) {
      ra->printRaytracingState();
      re->printRotationState();

      ra->run();
      re->run();

      ra->finish();
      re->finish();
    }
  }
  TIME(loop_time_end);

  double t = timevaldiff(&loop_time_start, &loop_time_end);
  fprintf(stderr, "Total execution time: %lf (s)\n", t / 1.0e3);

  return 0;
}

/*
*   Function: convertToString
*   -------------------------
*   Converts the c-string program arguments into c++-strings and returns
*   a pointer to an array of such strings.
*/
string *convertToString(char **in, size_t size) {
  string *args = new string[size];
  for (size_t i = 0; i < size; i++) {
    args[i] = in[i];
  }
  return args;
}

/*
*   Function: parseArgs
*   -------------------
*   Extracts the rotation angle as well as the in- and output file names
*   from the string array args, storing them in the specified variables.
*/
bool parseArgs(string *args, int &angle, unsigned int &xres, unsigned int &yres,
               unsigned int &rpp, string &inname, string &outname) {
  const char *tmp = args[3].c_str();
  angle = atoi(tmp) % 360;
  xres = atoi(args[4].c_str());
  yres = atoi(args[5].c_str());
  rpp = atoi(args[6].c_str());

  if (angle < 0 || xres <= 0 || yres <= 0 || rpp < 1) {
    cerr << "Bad arguments, exiting" << endl;
    exit(-1);
  }

  inname = args[1];
  outname = args[2];
  return true;
}

/*
*   Function: timevaldiff
*   ---------------------
*   Provides a millisecond-resolution timer, computing the elapsed time
*   in between the two given timeval structures.
*/
static double timevaldiff(timer *start, timer *finish) {
  double msec;
  msec = (finish->tv_sec - start->tv_sec) * 1.0e3;
  msec += (finish->tv_usec - start->tv_usec) / 1.0e3;
  return msec;
}
