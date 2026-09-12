#ifndef _MORTON_H_
#define _MORTON_H_
#include <cstdint>
#include <algorithm>
// UTILITY FUNCTIONS
uint32_t part1by1_16(uint16_t n);
uint64_t part1by1_32(uint32_t n);
uint64_t part1by2_32(uint32_t n);
uint32_t morton_encode_2d16(uint16_t x, uint16_t y);
uint64_t morton_encode_2d32(uint32_t x, uint32_t y);
uint64_t morton_encode_3d32(uint32_t x, uint32_t y, uint32_t z);
uint32_t count_leading_zeros(uint32_t n);
#endif // !_MORTON_H_
