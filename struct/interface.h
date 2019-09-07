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

void read_argv(int argc, char **argv, int *n, char *filename);
void fatal(char *message);
void warning(char *message);

#endif /* INTERFACE_H_ */
