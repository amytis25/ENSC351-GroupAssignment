// SPI.c
// ENSC 351 Fall 2025
// SPI functions for BeagleY-AI

#ifndef SPI_H
#define SPI_H

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#define SPI_DEV_PATH "/dev/spidev0.0"

// read a channel from the ADC and return the 12-bit raw value (0..4095)
// static int read_ch(int fd, int ch, uint32_t speed_hz);


// read and return normalized joystick x and y values
int Read_ADC_Values(int channel);

#endif