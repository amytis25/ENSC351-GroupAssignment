# Build the reaction timer application, using the HAL

# Find all source files in current directory
include_directories(include)
file(GLOB MY_SOURCES "*.c")

# Create the Light Dip Detector executable
add_executable(Light_Dip_Detector ${MY_SOURCES})

# Link with the HAL library
target_link_libraries(Light_Dip_Detector LINK_PRIVATE hal)

# Link additional libraries needed for timing and math functions
target_link_libraries(Light_Dip_Detector LINK_PRIVATE rt m)

# Copy executable to final location for deployment
add_custom_command(TARGET Light_Dip_Detector POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy
     "$<TARGET_FILE:Light_Dip_Detector>"
     "~/ensc351/public/as2/Light_Dip_Detector"
  COMMENT "Copying ARM executable to public NFS directory")


