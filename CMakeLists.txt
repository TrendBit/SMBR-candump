cmake_minimum_required(VERSION 3.16)
project(can-watch
        VERSION 1.0
        LANGUAGES CXX)

# Compiler settings
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra -pedantic -pg")

# Source files
file(GLOB_RECURSE SOURCES
    "*.cpp"
)

file(GLOB_RECURSE HEADERS
    "*.hpp"
)

# Create executable
add_executable(${PROJECT_NAME} can_message_dump.cpp)

target_link_libraries(${PROJECT_NAME} 
  SMBR.Can.Codes
)

add_dependencies(${PROJECT_NAME} 
  SMBR.Can.Codes
)


# Set include directories
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/magic_enum
        ${CMAKE_CURRENT_SOURCE_DIR}/emio
)

# Disable note about change of packed bit-field behavior in GCC
target_compile_options(${PROJECT_NAME} PRIVATE -Wno-packed-bitfield-compat)

# Set output directories
set_target_properties(${PROJECT_NAME}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)


install(TARGETS ${PROJECT_NAME} 
  RUNTIME DESTINATION ${SMBR_INSTALL_HOME}
)
