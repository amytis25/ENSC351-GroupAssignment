# ASSIGNMENT 2 - LIGHT_SAMPLER.C

## PROGRAM OUTLINE

- Uses the light sensor to read current light level.
- Computes and print out the number of dips in the light intensity seen in the previous second.
- Uses the rotary encoder to control how fast the LED blinks (using PWM). This LED emitter is designed to flash directly at the “detector” light sensor.
- Each second, prints some information to the terminal.
- Listens to a UDP socket and respond to commands
- Uses the terminal to display some simple status information.

## BEAGLE-Y-AI PIN CONNECTIONS 

- LED (PWM) is connected on GPIO 15
- Photoresistor Sensor is connected via SPI - using ch0 - GPIO4

- ROTARY ENCODER:
    - select - not used
    - output A - GPIO5
    - output B - GPIO6 


## Manually Running CMake

To manually run CMake from the command line use:

```shell
  # Regenerate build/ folder and makefiles:
  rm -rf build/         # Wipes temporary build folder
  cmake -S . -B build   # Generate makefiles in build\

  # Build (compile & link) the project
  cmake --build build
```

## NOWORKY.C STATUS 

noworky should be fixed but needs testing 

