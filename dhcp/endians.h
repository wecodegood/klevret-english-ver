#pragma once

#include <cstdint>
#include <vector>

int32_t int32_big_endian_to_host_endian(std::vector<uint8_t>::iterator begin, std::vector<uint8_t>::iterator end);
uint32_t uint32_big_endian_to_host_endian(std::vector<uint8_t>::iterator begin, std::vector<uint8_t>::iterator end);
uint16_t uint16_big_endian_to_host_endian(std::vector<uint8_t>::iterator begin, std::vector<uint8_t>::iterator end);
