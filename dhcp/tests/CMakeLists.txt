cmake_minimum_required(VERSION 3.21)

# ~~~ файлы исходного кода ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>>

file(GLOB KLEVRET_DHCP_TESTS_SOURCE_FILES
    "${KLEVRET_DHCP_PATH}/tests/src/*.cpp"
    "${KLEVRET_DHCP_PATH}/tests/src/*.hpp"
)

# Добавляем файлы тестируемого таргета
list(APPEND KLEVRET_DHCP_TESTS_SOURCE_FILES ${KLEVRET_DHCP_SOURCE_FILES})
# Убираем main.cpp, поскольку он будет конфликтовать с тестами
list(REMOVE_ITEM KLEVRET_DHCP_TESTS_SOURCE_FILES "${KLEVRET_DHCP_PATH}/src/main.cpp")

# ~~~ dhcp-server-tests target ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>>

set(KLEVRET_DHCP_TESTS_TARGET "dhcp-server-tests")
add_executable(${KLEVRET_DHCP_TESTS_TARGET} ${KLEVRET_DHCP_TESTS_SOURCE_FILES})

target_include_directories(${KLEVRET_DHCP_TESTS_TARGET}
    PRIVATE
        "${KLEVRET_DHCP_PATH}/src/"
        "${KLEVRET_DHCP_PATH}/tests/src/"
)

target_compile_features(${KLEVRET_DHCP_TESTS_TARGET} PRIVATE cxx_std_23)

target_compile_options(${KLEVRET_DHCP_TESTS_TARGET}
    PRIVATE ${KLEVRET_COMMON_COMPILE_OPTIONS}
)

set_target_properties(${KLEVRET_DHCP_TESTS_TARGET} PROPERTIES
    CXX_EXTENSIONS OFF
    POSITION_INDEPENDENT_CODE ON
)

# ~~~ dhcp-server-tests зависимости ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>>

target_link_libraries(${KLEVRET_DHCP_TESTS_TARGET}
    PRIVATE gtest gtest_main gmock
)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>>
