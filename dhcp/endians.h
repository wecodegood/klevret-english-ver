#pragma once

#include <vector>
#include <cstdint>
#include <type_traits>
#include <stdexcept>
#include <string>
#include <bit>

template<typename T>
T network_to_host_endian(
    const std::vector<uint8_t>::const_iterator begin,
    const std::vector<uint8_t>::const_iterator end
) {
    static_assert(std::is_integral<T>::value, "T должен быть целочисленным типом данных.");

    std::size_t distance = 0;
    if ((distance = std::distance(begin, end)) != sizeof(T)) {
        throw std::runtime_error(
            "Ожидалось байт - " + std::to_string(sizeof(T)) + "." +
            "Получено байт - " + std::to_string(distance) + "."
        );
    }

    T result = 0;
    if (std::endian::native == std::endian::big) {
        for (size_t i = 0; i < sizeof(T); i++) {
            result = (result << 8) | static_cast<T>(begin[sizeof(T) - 1 - i]);
        }
    } else {
        for (size_t i = 0; i < sizeof(T); i++) {
            result = (result << 8) | static_cast<T>(begin[i]);
        }
    }

    return result;
}
