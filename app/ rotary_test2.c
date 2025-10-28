// rotary_test2.c
// test from chat

#include <gpiod.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>

#define GPIO_CHIP "/dev/gpiochip2"   // Adjust if your pins are under another chip
#define GPIO_A 5                      // GPIO5
#define GPIO_B 6                      // GPIO6

static volatile bool keepRunning = true;

void intHandler(int dummy) {
    keepRunning = false;
}

int main(void)
{
    struct gpiod_chip *chip;
    struct gpiod_line *lineA, *lineB;
    int valueA, valueB;

    signal(SIGINT, intHandler); // handle Ctrl+C cleanly

    // Open GPIO chip
    chip = gpiod_chip_open(GPIO_CHIP);
    if (!chip) {
        perror("gpiod_chip_open");
        return EXIT_FAILURE;
    }

    // Request A and B as inputs
    lineA = gpiod_chip_get_line(chip, GPIO_A);
    lineB = gpiod_chip_get_line(chip, GPIO_B);
    if (!lineA || !lineB) {
        fprintf(stderr, "Error: cannot get GPIO lines\n");
        gpiod_chip_close(chip);
        return EXIT_FAILURE;
    }

    if (gpiod_line_request_input(lineA, "rotary_A") < 0 ||
        gpiod_line_request_input(lineB, "rotary_B") < 0) {
        perror("gpiod_line_request_input");
        gpiod_chip_close(chip);
        return EXIT_FAILURE;
    }

    printf("Reading rotary encoder pins A=GPIO%d, B=GPIO%d...\n", GPIO_A, GPIO_B);
    printf("Press Ctrl+C to exit.\n\n");

    while (keepRunning) {
        valueA = gpiod_line_get_value(lineA);
        valueB = gpiod_line_get_value(lineB);

        if (valueA < 0 || valueB < 0) {
            perror("gpiod_line_get_value");
            break;
        }

        printf("A=%d  B=%d\n", valueA, valueB);
        usleep(100000); // 100 ms
    }

    gpiod_line_release(lineA);
    gpiod_line_release(lineB);
    gpiod_chip_close(chip);

    printf("\nProgram terminated.\n");
    return 0;
}