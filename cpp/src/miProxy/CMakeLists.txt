# Set the source files for miProxy
set(
    MIPROXY_SOURCES 
    miproxy.cpp
    server.cpp
)

# Create the miProxy executable
add_executable(miProxy ${MIPROXY_SOURCES})

# Link necessary libraries
target_link_libraries(miProxy PRIVATE cxxopts common spdlog::spdlog pugixml::pugixml Boost::regex)

# Include directories
target_include_directories(miProxy PRIVATE ${PROJECT_SOURCE_DIR}/common ${PROJECT_SOURCE_DIR}/spdlog)