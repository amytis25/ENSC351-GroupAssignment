# CMake Build Configuration for ENSC351 Assignment 2 - Light Sampler
cmake_minimum_required(VERSION 3.18)
project(ENSC351_Assignment2 VERSION 1.0 DESCRIPTION "Light Sampler" LANGUAGES C)

# Compiler options (inherited by sub-folders)
set(CMAKE_C_STANDARD 11)
add_compile_options(-Wall -Werror -Wpedantic -Wextra)
add_compile_options(-fdiagnostics-color)

# Enable address sanitizer for debugging
# (Comment this out to make your code faster)
add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

# Enable PThread library for linking
add_compile_options(-pthread)
add_link_options(-pthread)

# Add support for timing functions (nanosleep)
add_link_options(-lrt)

# What folders to build
add_subdirectory(hal)  
add_subdirectory(app)

