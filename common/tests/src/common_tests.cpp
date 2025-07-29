#include <gtest/gtest.h>

#include <Defer.hpp>

void defer_test(int& value){
    int temp = value;
    Defer defer([&](){value++;});
    ASSERT_TRUE(temp == value);
}

TEST(common_tests, deffer){
    int value = 10;
    defer_test(value);
    ASSERT_TRUE(value == 11);


}
