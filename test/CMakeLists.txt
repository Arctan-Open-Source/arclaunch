add_subdirectory(gtest)
enable_testing()

include_directories(${CMAKE_CURRENT_BINARY_DIR}/resources)

# Test for runnning binaries
add_subdirectory(binary)

# Test for running python scripts
add_subdirectory(python)

# Test for running perl scripts
add_subdirectory(perl)

# Test for running R scripts
add_subdirectory(R)
