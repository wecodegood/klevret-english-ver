#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>

#include "../src/endians.hpp"

TEST(DhcpEndians, endians) {
    std::vector<uint8_t> bytes_in_big_endian = {0xFF, 0xA, 0x13, 0x20};
    ASSERT_TRUE(network_to_host_endian<uint32_t>(bytes_in_big_endian.begin(), bytes_in_big_endian.end()) == 0xFF0A1320u);
}


int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::InitGoogleMock(&argc, argv);

  return RUN_ALL_TESTS();
}
