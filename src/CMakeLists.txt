message("-- Inside SRC directory")
file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.hpp")

foreach(file ${SOURCES})
    message(STATUS "found file: ${file}")
endforeach()

add_library(ps STATIC ${SOURCES})
target_include_directories(ps PUBLIC ${HEADERS})