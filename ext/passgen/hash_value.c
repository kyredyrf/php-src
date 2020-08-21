/**
 *	FNV ハッシュ値
 *	https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
 */

#include "hash_value.h"

#include <string.h>

uint64_t get_hash_value(const char *str)
{
	const uint64_t offset_basis = 14695981039346656037U;
	const uint64_t prime = 1099511628211U;
	uint64_t value = offset_basis;
	int length = strlen(str);
	int i;

	for (i = 0; i < length; i++)
	{
		value ^= (uint64_t)str[i];
		value *= prime;
	}

	return value;
}
