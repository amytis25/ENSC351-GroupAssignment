# Build the reaction timer application, using the HAL

# Option to allow excluding the helper/test file noworky.c when it's not needed
option(BUILD_NOWORKY "Build noworky.c helper/test file" OFF)

# Find all source files in current directory
include_directories(include)
file(GLOB MY_SOURCES "*.c")

# If requested, remove noworky.c from the sources so it won't be compiled
if(NOT BUILD_NOWORKY)
   list(REMOVE_ITEM MY_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/noworky.c")
endif()

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
     "/home/melissa/ENSC351/public/light_sampler"
  COMMENT "Copying ARM executable to public NFS directory")


