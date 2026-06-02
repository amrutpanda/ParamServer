cmake_minimum_required(VERSION 3.20)
project(ParamServer)

include(FetchContent)
# Fetch pugixml for xml parsing.
FetchContent_Declare(
    pugixml
    GIT_REPOSITORY https://github.com/zeux/pugixml.git
    GIT_TAG        v1.13        # Specify the version you want
)
FetchContent_MakeAvailable(pugixml)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

add_subdirectory(src)
add_subdirectory(examples)
