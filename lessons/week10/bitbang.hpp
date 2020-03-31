#pragma once

#include <stdint.h>
#include <math.h>

// return true if the specified bit is set in the value
inline bool isset(const uint64_t value, const size_t bit) {return (value & (int)pow(2,bit)) > 0; }

// set a specific bit within a value
inline uint64_t setbit(uint64_t value, size_t bit) {return (value | (int)pow(2,bit));}

// This will work for any number of bits from 32 to 63
// create a bit mask based on the low bit to high bit inclusive
// This is useful when you're trying to get a certain value
// out of an integer of some sort
inline uint64_t BITMASK(size_t lowbit, size_t highbit)
{
	uint64_t mask = 0ULL;

	for (int i=lowbit; i <= highbit; i++) {
		mask = setbit(mask, i);
    }

	return mask;
}

// retrieve an integer value from a bitmask on an integer
inline  uint64_t BITSVALUE(uint64_t src, size_t lowbit, size_t highbit)
{
	return ((src & BITMASK(lowbit, highbit)) >> lowbit);
}
