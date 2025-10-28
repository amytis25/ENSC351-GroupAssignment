// SPI.c
// ENSC 351 Fall 2025
// SPI functions for BeagleY-AI
// For reading photoresistor from CH0 MCP3202 ADC 

#include "hal/led.h"
#include "hal/timing.h"
#include "hal/SPI.h"
#include <stdio.h> // fopen, fprintf, fclose, perror
#include <stdlib.h>  // exit, EXIT_FAILURE, EXIT_SUCCESS
#include <stdbool.h>
#include <time.h>

// from SPI guide
static int read_ch(int fd, int ch, uint32_t speed_hz) {
    // fd is the file descriptor for the SPI device
    // ch is the channel number on the ADC
    // speed_hz SPI clock speed
    // tx this is our request message to the ADC
    // rx this is our receive buffer
    uint8_t tx[3] = { (uint8_t)(0x06 | ((ch & 0x04) >> 2)),
                      (uint8_t)((ch & 0x03) << 6),
                      0x00 };

    uint8_t rx[3] = { 0 };

    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)tx,
        .rx_buf = (unsigned long)rx,
        .len = 3,
        .speed_hz = speed_hz,
        .bits_per_word = 8,
        .cs_change = 0
    };

    if (ioctl(fd, SPI_IOC_MESSAGE(1), &tr) < 1) return -1;

    return ((rx[1] & 0x0F) << 8) | rx[2];  // 12-bit result
}


// from SPI guide
int Read_ADC_Values(int channel) {
    const char* dev = SPI_DEV_PATH;
    uint8_t mode = 0;       // SPI mode 0
    uint8_t bits = 8;
    uint32_t speed = 250000;

    int fd = open(dev, O_RDWR);
    if (fd < 0) { perror("open"); return -1; }
    if (ioctl(fd, SPI_IOC_WR_MODE, &mode) == -1) { perror("mode"); return -1; }
    if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits) == -1) { perror("bpw"); return -1; }
    if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) == -1) { perror("speed"); return -1; }

    int channelRead = read_ch(fd, channel, speed);

    close(fd);
    return channelRead;
}