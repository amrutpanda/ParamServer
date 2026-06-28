message("-- inside db directory")
file(GLOB SOURCES CONFIGURE_DEPENDS "*.cpp")
file(GLOB HEADERS CONFIGURE_DEPENDS "*.hpp")

function(printList var)
    message(STATUS "Contents inside ${var}")
    foreach(item IN LISTS ${var})
        message(STATUS " ${item}")        
    endforeach()
endfunction()

# foreach(file ${SOURCES})
#     message(STATUS "found file inside db : ${file}")
# endforeach()

# foreach(file ${HEADERS})
#     message(STATUS "found file inside db: ${file}")
# endforeach()

# include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(DB_SOURCES ${SOURCES} PARENT_SCOPE)
set(DB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)