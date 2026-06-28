message("-- Inside SRC directory")
file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.hpp")

# USE "NNG_INCLUDE_DIRS" AND "NNG_LIBRARY" FOR INCLUDE PATH AND LIBRARY TARGET RESP.
find_package(nng REQUIRED)

include_directories(${HEADERS} ${NNG_INCLUDE_DIRS})

add_subdirectory(utils)
add_subdirectory(nanomsg)
add_subdirectory(db)


# BUILD THE PS LIBRARY.
add_library(ps ${DB_SOURCES} ${NET_SOURCES})
target_link_libraries(ps PUBLIC proto)
target_include_directories(ps PUBLIC ${UTILS_INCLUDE_DIR} ${DB_INCLUDE_DIR} ${NET_INCLUDE_DIR})
# target_include_directories(ps PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})