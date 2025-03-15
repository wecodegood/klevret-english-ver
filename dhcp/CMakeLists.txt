cmake_minimum_required(VERSION 3.25)

project(klevret-dhcp-server)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED 23)

add_executable(dhcp-server
    main.cpp
    DhcpMessage.cpp
    DhcpOption.cpp
    HardwareAddress.cpp
    IpAddress.cpp)
