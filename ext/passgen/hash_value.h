/**
 *	FNV ハッシュ値
 *	https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
 */

#ifndef include_hash_value_h
#define include_hash_value_h

#include <stdint.h>

uint64_t get_hash_value(const char *str);

#endif /* include_hash_value_h */
