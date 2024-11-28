cmake_minimum_required(VERSION 2.8.12)

project(okay)

set(CMAKE_VS_DEVENV_COMMAND "C:\\Program Files (x86)\\Microsoft Visual Studio\\2022\\BuildTools\\Common7\\IDE\\devenv.ico")
include("$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")  
  set(VCPKG_TARGET_TRIPLET x64-windows CACHE STRING "" FORCE)  
  # set(VCPKG_CRT_LINKAGE static)
  # set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
  # set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")  
endif()


find_package(OpenSSL REQUIRED)
find_package(libuv CONFIG REQUIRED)
find_package(ZLIB REQUIRED)

add_subdirectory(cpp-driver)

cmake_policy(SET CMP0079 NEW)

add_executable(test "test.c")

target_include_directories(test PUBLIC cpp-driver/include)
target_link_libraries(test PUBLIC cassandra)