#pragma once

#include <stdint.h>
#include <math.h>

inline bool isset(const uint64_t value, const size_t bit) {return (value & (int)pow(2,bit)) > 0; }
inline uint64_t setbit(uint64_t value, size_t bit) {return (value | (int)pow(2,bit));}

// This will work for any number of bits from 32 to 63
inline uint64_t BITMASK(size_t lowbit, size_t highbit)
{
	uint64_t mask = 0ULL;

	for (int i=lowbit; i <= highbit; i++) {
		mask = setbit(mask, i);
    }

	return mask;
}

inline  uint64_t BITSVALUE(uint64_t src, size_t lowbit, size_t highbit)
{
	return ((src & BITMASK(lowbit, highbit)) >> lowbit);
}
