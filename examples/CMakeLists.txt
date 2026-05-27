message("-- Inside Examples directory")
add_executable(spmc_test spmc_test.cpp)
target_link_libraries(spmc_test ps)

add_executable(MTReadWrite MultiThreadedParamReadWrites.cpp)
target_link_libraries( MTReadWrite ps)