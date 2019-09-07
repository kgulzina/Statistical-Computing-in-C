/**
 * @file main.h
 * @authors SSGL
 * @version 0.1
 * @details
 *
 * main headers. Set/unset DEBUG here.
 */


#ifndef C_MAIN_H_
#define C_MAIN_H_

#define MAX_FILENAME_LENGTH 500
#define BUFFER_SIZE 300
#define HASH_SIZE_FACTOR 0.5 * 0.5 * 0.5 * 0.5 * 0.5
/* Multiplies the number of readings by factor to compute an initial size for
 * the hash table. My guess? Five uniformly distributed bases (ACGTU). */

//#define DEBUG

/* Credits to Joey Adams https://stackoverflow.com/a/3553321/2860744 */
#define member_size(type, member) sizeof(((type *)0)->member)

#endif /* C_MAIN_H_ */
