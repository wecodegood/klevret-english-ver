#include <gtest/gtest.h>

#include <endians.hpp>

TEST(dhcp_server_tests, endians)
{
    std::vector<uint8_t> bytes_in_big_endian = {0xFF, 0xA, 0x13, 0x20};
    ASSERT_TRUE(
        network_to_host_endian<uint32_t>(
            bytes_in_big_endian.begin(),
            bytes_in_big_endian.end()
        ) == 0xFF0A1320u // Разве значение не зависит от платформы?
    );
}
