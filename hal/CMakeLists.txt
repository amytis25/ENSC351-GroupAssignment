# CMakeList.txt for HAL (Hardware Abstraction Layer)
#   Build a library (`hal`) which exposes the LED control functionality
#   Use header as: #include "led.h"

# Option to disable building the rotary encoder HAL (useful for temporary testing)
option(BUILD_ROTARY_ENCODER "Build rotary encoder HAL module" OFF)
option(BUILD_SAMPLER "Build sampler HAL module" OFF)

# Find all source files in the src directory
include_directories(hal/include)
file(GLOB MY_SOURCES "src/*.c")

# If requested, remove the rotary encoder source from the list so it is not
# compiled into the `hal` library. This is a cheap way to temporarily omit
# a single problematic module without changing other files.
if(NOT BUILD_ROTARY_ENCODER)
	list(REMOVE_ITEM MY_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/rotary_encoder.c")
endif()

# Allow temporarily excluding the sampler module while it's being worked on.
if(NOT BUILD_SAMPLER)
  list(REMOVE_ITEM MY_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/sampler.c")
endif()

# Create the HAL static library
add_library(hal STATIC ${MY_SOURCES})

# Make the include directory available to users of this library
target_include_directories(hal PUBLIC include)

# Link required libraries for LED control and timing
target_link_libraries(hal PUBLIC rt)  # For nanosleep and real-time functions

