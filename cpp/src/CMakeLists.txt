# Fetch third-party dependencies
include(FetchContent)

# Fetch spdlog
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.15.1  # Change version if needed
)
FetchContent_MakeAvailable(spdlog)

# Fetch cxxopts
FetchContent_Declare(
    cxxopts
    GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
    GIT_TAG v3.1.1  # Change version if needed
)
FetchContent_MakeAvailable(cxxopts)

# Find required packages
find_package(Boost REQUIRED COMPONENTS regex)

# Add subdirectories
add_subdirectory(common)
add_subdirectory(miProxy)
add_subdirectory(loadBalancer)