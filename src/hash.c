#include "durex.h"
t_ctx					durex;

uint32_t    _hash(char *key, size_t len) {
	uint32_t hash = 0;

	for(uint32_t i = 0; i < len; ++i)
	{
		hash += key[i];
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}
	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);
	return hash;
}