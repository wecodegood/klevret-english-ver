#pragma once

#include <vector>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <bit>

template<std::integral T, std::size_t SIZE>
T network_array_to_host_endian(std::array<uint8_t, SIZE> arr) {
    if (arr.size() != sizeof(T)) {
        throw std::runtime_error(
            "Ожидалось байт - " + std::to_string(sizeof(T)) + "." +
            "Получено байт - " + std::to_string(arr.size()) + "."
        );
    }

    T result = std::bit_cast<T>(arr);

    if (std::endian::native == std::endian::little) {
        result = std::byteswap(result);
    }

    return result;
}

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

template<std::integral T, std::size_t SIZE>
std::array<uint8_t, SIZE> host_to_network_endian_array(T value){
    if (SIZE != sizeof(T)) {
        throw std::runtime_error(
            "Ожидалось байт - " + std::to_string(sizeof(T)) + "." +
            "Получено байт - " + std::to_string(SIZE) + "."
        );
    }
    if (std::endian::native == std::endian::little) {
        value = std::byteswap(value);
    }
    std::array<uint8_t, SIZE> result = std::bit_cast<std::array<uint8_t, SIZE>>(value);



    return result;
}
