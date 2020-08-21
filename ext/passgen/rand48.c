/**
 *	rand48 乱数
 */

#include "rand48.h"

#include <stdlib.h>

struct rand48_t 
{
	uint64_t x;
};

static const uint64_t c = 0x1234ABCD330E;

rand48_t* rand48_create()
{
	rand48_t* r;

	r = (rand48_t*)malloc(sizeof(*r));
	r->x = c;

	return r;
}

void rand48_destroy(rand48_t* r)
{
	free(r);
}

/* [-2147483648, 214748367] */
int32_t rand48_get_integer(rand48_t* r)
{
	r->x = r->x * 0x5DEECE66D + 0xB;
	return (int32_t)((r->x >> 16) & 0xFFFFFFFF);
}

/* [0, 214748367] */
int32_t rand48_get_nonnegative_integer(rand48_t* r)
{
	r->x = r->x * 0x5DEECE66D + 0xB;
	return (int32_t)((r->x >> 17) & 0x7FFFFFFF);
}

/* [0.0, 1.0) */
double rand48_get_double(rand48_t* r)
{
	r->x = r->x * 0x5DEECE66D + 0xB;
	return (r->x & 0xFFFFFFFFFFFF) * (1.0 / 281474976710656.0);
}

void rand48_set_seed(rand48_t* r, uint64_t s)
{
	r->x = s;
	r->x = (r->x << 16) + 0x330E;
}
