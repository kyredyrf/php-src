/**
 *	rand48 乱数
 */

#ifndef include_rand48_h
#define include_rand48_h

#include <stdint.h>

typedef struct rand48_t rand48_t;

rand48_t* rand48_create();
void rand48_destroy(rand48_t* r);
int32_t rand48_get_integer(rand48_t* r);
int32_t rand48_get_nonnegative_integer(rand48_t* r);
double rand48_get_double(rand48_t* r);
void rand48_set_seed(rand48_t* r, uint64_t s);

#endif /* include_rand48_h */
