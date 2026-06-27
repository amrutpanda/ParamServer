message("-- inside nanomsg folder")
file(GLOB SOURCES CONFIGURE_DEPENDS "*.c")
file(GLOB HEADERS CONFIGURE_DEPENDS "*.h")

function(printList var)
    message(STATUS "Contents inside ${var}")
    foreach(item IN LISTS ${var})
        message(STATUS " ${item}")        
    endforeach()
endfunction()

add_library(proto ${SOURCES})
target_include_directories(proto PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
