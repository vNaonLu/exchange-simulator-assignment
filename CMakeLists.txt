cmake_minimum_required(VERSION 3.20)

project(
  exchange-simulator
  LANGUAGES CXX
  VERSION   0.1
)

# set compiler flags
set(CMAKE_EXPORT_COMPILE_COMMANDS   ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS         "-Wall -Werror -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG   "-g")

# glob the source file
file(GLOB PROJECT_SOURCE   ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)
set(PROJECT_INCLUDES       ${CMAKE_CURRENT_SOURCE_DIR}/include/)

add_library(
  ${PROJECT_NAME}
  ${PROJECT_SOURCE}
)

target_include_directories(
  ${PROJECT_NAME}
  PRIVATE ${PROJECT_INCLUDES}
)