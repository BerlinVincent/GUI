# Collect source file here
file(GLOB_RECURSE SRC_FILES *.cpp)

# Create library from source files (excluding main.cpp) and make available here
list(FILTER SRC_FILES EXCLUDE REGEX ".*/main\\.cpp$")
add_library(SourceLib ${SRC_FILES})

# Include libraries here
target_include_directories(SourceLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(SourceLib PUBLIC raylib)

# Define executable target Main here
add_executable(Main main.cpp)

target_link_libraries(Main
    PRIVATE
    SourceLib
)