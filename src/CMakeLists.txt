message("-- Inside SRC directory")
file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.hpp")

include_directories(${HEADERS})
foreach(file ${SOURCES})
    message(STATUS "found file: ${file}")
endforeach()

foreach(file ${HEADERS})
    message(STATUS "found file: ${file}")
endforeach()

add_library(ps STATIC ${SOURCES})
target_include_directories(ps PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})