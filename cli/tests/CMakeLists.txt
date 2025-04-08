cmake_minimum_required(VERSION 3.21)

# ~~~ файлы исходного кода ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>>

file(GLOB KLEVRET_CLI_TESTS_SOURCE_FILES
    "${KLEVRET_CLI_PATH}/tests/src/*.cpp"
    "${KLEVRET_CLI_PATH}/tests/src/*.hpp"
)

# Добавляем файлы тестируемого таргета
list(APPEND KLEVRET_CLI_TESTS_SOURCE_FILES ${KLEVRET_CLI_SOURCE_FILES})
# Убираем main.cpp, поскольку он будет конфликтовать с тестами
list(REMOVE_ITEM KLEVRET_CLI_TESTS_SOURCE_FILES "${KLEVRET_CLI_PATH}/src/main.cpp")

# ~~~ cli-tests target ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>>

set(KLEVRET_CLI_TESTS_TARGET "cli-tests")
add_executable(${KLEVRET_CLI_TESTS_TARGET} ${KLEVRET_CLI_TESTS_SOURCE_FILES})

target_include_directories(${KLEVRET_CLI_TESTS_TARGET}
    PRIVATE
        "${KLEVRET_CLI_PATH}/src/"
        "${KLEVRET_CLI_PATH}/tests/src/"
)

target_compile_features(${KLEVRET_CLI_TESTS_TARGET} PRIVATE cxx_std_23)

target_compile_options(${KLEVRET_CLI_TESTS_TARGET}
    PRIVATE ${KLEVRET_COMMON_COMPILE_OPTIONS}
)

set_target_properties(${KLEVRET_CLI_TESTS_TARGET} PROPERTIES
    CXX_EXTENSIONS OFF
    POSITION_INDEPENDENT_CODE ON
)

# ~~~ cli-tests зависимости ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>>

target_link_libraries(${KLEVRET_CLI_TESTS_TARGET}
    PRIVATE gtest gtest_main gmock
)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>>
