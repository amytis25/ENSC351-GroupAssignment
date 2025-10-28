# Build the reaction timer application, using the HAL

# Find all source files in current directory
include_directories(include)
file(GLOB MY_SOURCES "*.c")

# Create the Light Dip Detector executable
add_executable(light_sampler ${MY_SOURCES})

# Link with the HAL library
target_link_libraries(light_sampler LINK_PRIVATE hal)

# Link additional libraries needed for timing and math functions
target_link_libraries(light_sampler LINK_PRIVATE rt m)

# Copy executable to final location for deployment
add_custom_command(TARGET light_sampler POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy
     "$<TARGET_FILE:light_sampler>"
     "~/ensc351/public/as2/light_sampler"
  COMMENT "Copying ARM executable to public NFS directory")


