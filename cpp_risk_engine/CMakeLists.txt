cmake_minimum_required(VERSION 3.25)
project(cpp_risk_engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_TOOLCHAIN_FILE "/Users/alan/.clion-vcpkg/vcpkg/scripts/buildsystems/vcpkg.cmake")

add_executable(cpp_risk_engine main.cpp)

# Find the installed Boost package
#find_package(Boost)
find_package(Boost REQUIRED COMPONENTS filesystem)
find_package(fmt CONFIG REQUIRED)

target_link_libraries(cpp_risk_engine PRIVATE fmt::fmt ${Boost_LIBRARIES})

# Include the Boost headers
include_directories(${Boost_INCLUDE_DIRS})

# Link against the Boost libraries
#target_link_libraries(cpp_risk_engine ${Boost_LIBRARIES})


