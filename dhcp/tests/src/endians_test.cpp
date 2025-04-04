#include <gtest/gtest.h>

#include <endians.hpp>

TEST(dhcp_server_tests, endians_iterator)
{
    std::vector<uint8_t> bytes_in_big_endian = {0x22, 0xC1, 0xB2, 0x2B, 0x3E, 0x4F, 0x1D, 0x13};
    ASSERT_TRUE(
        network_to_host_endian<uint64_t>(
            bytes_in_big_endian.begin(),
            bytes_in_big_endian.end()
        ) == 0x22C1B22B3E4F1D13ull
    );
    bytes_in_big_endian = {0xFF, 0xA, 0x13, 0x20};
    ASSERT_TRUE(
        network_to_host_endian<uint32_t>(
            bytes_in_big_endian.begin(),
            bytes_in_big_endian.end()
        ) == 0xFF0A1320u
    );
    bytes_in_big_endian = {0xAB, 0x11};
    ASSERT_TRUE(
        network_to_host_endian<uint16_t>(
            bytes_in_big_endian.begin(),
            bytes_in_big_endian.end()
        ) == 0xAB11u
    );
    bytes_in_big_endian = {0x80, 0x0, 0x0, 0xd6, 0x34, 0x45, 0xf0, 0x0};
    ASSERT_TRUE(
        network_to_host_endian<int64_t>(
            bytes_in_big_endian.begin(),
            bytes_in_big_endian.end()
        ) == -9223371116854775808LL
    );
    bytes_in_big_endian = {0x81, 0xCD, 0x38, 0xD7};
    ASSERT_TRUE(
        network_to_host_endian<int32_t>(
            bytes_in_big_endian.begin(),
            bytes_in_big_endian.end()
        ) == -2117257001
    );
    bytes_in_big_endian = {0x82, 0x8B};
    ASSERT_TRUE(
        network_to_host_endian<int16_t>(
            bytes_in_big_endian.begin(),
            bytes_in_big_endian.end()
        ) == -32117
    );
}

TEST(dhcp_server_tests, endians)
{
    union{
        uint8_t array[8]{0x22, 0xC1, 0xB2, 0x2B, 0x3E, 0x4F, 0x1D, 0x13};
        uint64_t number;
    } uint64_value;
    ASSERT_TRUE(network_to_host_endian<uint64_t>(uint64_value.number) == 0x22C1B22B3E4F1D13ull);

    union{
        uint8_t array[4]{0xFF, 0xA, 0x13, 0x20};
        uint32_t number;
    } uint32_value;
    ASSERT_TRUE(network_to_host_endian<uint32_t>(uint32_value.number) == 0xFF0A1320u);

    union{
        uint8_t array[2]{0xAB, 0x11};
        uint16_t number;
    } uint16_value;
    ASSERT_TRUE(network_to_host_endian<uint16_t>(uint16_value.number) == 0xAB11u);

    union{
        uint8_t array[8]{0x80, 0x0, 0x0, 0xd6, 0x34, 0x45, 0xf0, 0x0};
        int64_t number;
    } int64_value;
    ASSERT_TRUE(network_to_host_endian<int64_t>(int64_value.number) == -9223371116854775808LL);

    union{
        uint8_t array[4]{0x81, 0xCD, 0x38, 0xD7};
        int32_t number;
    } int32_value;
    ASSERT_TRUE(network_to_host_endian<int32_t>(int32_value.number) == -2117257001);

    union{
        uint8_t array[2]{0x82, 0x8B};
        int16_t number;
    } int16_value;
    ASSERT_TRUE(network_to_host_endian<int16_t>(int16_value.number) == -32117);
}
