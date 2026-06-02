message("-- Inside SRC directory")
file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.hpp")

# USE "NNG_INCLUDE_DIRS" AND "NNG_LIBRARY" FOR INCLUDE PATH AND LIBRARY TARGET RESP.
find_package(nng REQUIRED)

include_directories(${HEADERS} ${NNG_INCLUDE_DIRS})
foreach(file ${SOURCES})
    message(STATUS "found file: ${file}")
endforeach()

foreach(file ${HEADERS})
    message(STATUS "found file: ${file}")
endforeach()

add_library(ps STATIC ${SOURCES})
target_link_libraries(ps PUBLIC ${NNG_LIBRARY} pugixml)
target_include_directories(ps PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})