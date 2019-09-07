/**
 * @file read.c
 * @authors SSGL
 * @version 0.1
 * @details
 *
 * Interface (read and print) functions. Use responsibly :)
 */

#ifndef INTERFACE_H_
#define INTERFACE_H_

#define MAX_N 10510
#define BUFFER_SIZE 30

#ifdef PRECISION_LONG_DOUBLE

#define FLOAT_FMT_IN "Le"
#define FLOAT_FMT_OUT "0.18Le"
typedef long double myfloat;

#else

#define FLOAT_FMT_IN "le"
#define FLOAT_FMT_OUT "0.15le"
typedef double myfloat;

#endif

void print_usage(void);
void read_argv(int argc, char **argv, int *n, int *k, char *m);
void read_data(int n, int maxLen, myfloat *p, int *len);
void print_data(int n, myfloat *p);
void fatal(char *message);

#endif /* INTERFACE_H_ */
