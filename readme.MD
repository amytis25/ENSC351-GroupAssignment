# ASSIGNMENT 2 - LIGHT_SAMPLER.C

## PROGRAM OUTLINE

- Uses the light sensor to read current light level.
- Computes and print out the number of dips in the light intensity seen in the previous second.
- Uses the rotary encoder to control how fast the LED blinks (using PWM). This LED emitter is designed to flash directly at the “detector” light sensor.
- Each second, prints some information to the terminal.
- Listens to a UDP socket and respond to commands
- Uses the terminal to display some simple status information.

## BEAGLE-Y-AI PIN CONNECTIONS 

- LED (PWM) is connected on GPIO 15 gpiochip2 13
- Photoresistor Sensor is connected via SPI - using ch0 - GPIO4 gpiochip1 38

- ROTARY ENCODER:
    - select - not used
    - output A - GPIO5 gpiochip2 15
    - output B - GPIO6 gpiochip2 17

## RUNNING CMAKE
- for host:
    ```shell
        ./buildHost.sh
    ```
- for target:
    ```shell
        ./buildTarget.sh
    ```

## Manually Running CMake

To manually run CMake from the command line use:

### Host (build for the machine you're running on)

    ```shell
    # Regenerate build/ folder and makefiles:
    rm -rf build/         # Wipes temporary build folder
    cmake -S . -B build   # Generate makefiles in build

    # Build (compile & link) the project
    cmake --build build
    ```

### Target (cross-compile for aarch64)

Use one of these minimal options (do this from the repo root). Remove the old `build/` or `build/CMakeCache.txt` before changing compilers.

    Option A — pass compilers directly to CMake:

        ```shell
        rm -rf build/
        cmake -S . -B build \
        -DCMAKE_C_COMPILER=aarch64-linux-gnu-gcc \
        -DCMAKE_CXX_COMPILER=aarch64-linux-gnu-g++
        cmake --build build
        ```

    Option B — set environment variables for a one-shot configure:

        ```shell
        rm -rf build/
        CC=aarch64-linux-gnu-gcc CXX=aarch64-linux-gnu-g++ cmake -S . -B build
        cmake --build build
        ```



## NOWORKY.C STATUS 

noworky should be fixed but needs testing 

