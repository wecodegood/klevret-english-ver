#include <gtest/gtest.h>

#include <IpAddress.hpp>

TEST(dhcp_server_tests, IPv4Address_postfix_increment){
    IPv4Address ip("192.168.1.1");
    ip++;
    ASSERT_TRUE(ip.to_string() == "192.168.1.2");


}
