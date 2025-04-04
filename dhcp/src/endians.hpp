#pragma once

#include <vector>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <bit>

template<std::integral T>
T network_to_host_endian(
    const std::vector<uint8_t>::const_iterator begin,
    const std::vector<uint8_t>::const_iterator end
) {
    std::size_t distance = 0;
    if ((distance = std::distance(begin, end)) != sizeof(T)) {
        throw std::runtime_error(
            "Ожидалось байт - " + std::to_string(sizeof(T)) + "." +
            "Получено байт - " + std::to_string(distance) + "."
        );
    }

    std::array<uint8_t, sizeof(T)> buffer;
    std::copy(begin, end, buffer.begin());
    T result = std::bit_cast<T>(buffer);

    if (std::endian::native == std::endian::little) {
        result = std::byteswap(result);
    }

    return result;
}

template<std::integral T>
T network_to_host_endian(T value) {
    if (std::endian::native == std::endian::little) {
        return std::byteswap(value);
    }
    return value;
}
