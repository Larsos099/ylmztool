cmake_minimum_required(VERSION 3.7)
project(ylmztool CXX)

# Set the toolchain file
set(CMAKE_TOOLCHAIN_FILE /opt/devkitpro/cmake/WiiU.cmake)
include_directories(${DEVKITPRO}/include)
link_directories(${DEVKITPRO}/lib)

# Include the DevkitPro Wii U CMake module
include("/opt/devkitpro/cmake/WiiU.cmake")

add_executable(ylmztool achdukacke.cpp)
add_executable(ylmztool_old achdukacke_old.cpp)
wut_create_rpx(ylmztool)
wut_create_rpx(ylmztool_old)
