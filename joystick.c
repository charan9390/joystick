#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"

// Define pin connections
#define JOYSTICK_X_ADC 0   // ADC0 is GP26
#define JOYSTICK_Y_ADC 1   // ADC1 is GP27
#define JOYSTICK_SW_PIN 22 // Digital GP22 for the switch
#define LED 25             // Digital GP25 for the LED

int main() {
    // Initialize USB Serial communication
    stdio_init_all();

    // 1. Initialize ADC hardware and pins
    adc_init();
    adc_gpio_init(26); // Prepare GP26 for analog input
    adc_gpio_init(27); // Prepare GP27 for analog input

    // 2. Initialize Switch (Digital Input with Pull-Up resistor)
    // Most joysticks pull the SW pin to GND when pressed.
    gpio_init(JOYSTICK_SW_PIN);
    gpio_set_dir(JOYSTICK_SW_PIN, GPIO_IN);
    gpio_pull_up(JOYSTICK_SW_PIN);

    // 3. Initialize LED (Digital Output)
    gpio_init(LED);
    gpio_set_dir(LED, GPIO_OUT);
 bool sw_pressed;
    printf("Joystick Initialization Complete.\n");
    while (true) {
        // Read X-Axis (Channel 0)
        adc_select_input(JOYSTICK_X_ADC);
        uint16_t x_raw = adc_read();

        // Read Y-Axis (Channel 1)
        adc_select_input(JOYSTICK_Y_ADC);
        uint16_t y_raw = adc_read();

        // Read Switch State (0 = Pressed, 1 = Idle because of Pull-Up)
         gpio_put(LED, !gpio_get(JOYSTICK_SW_PIN));
        

        // Print data to Serial USB in a clean format
        printf("X: %4d | Y: %4d | Button: %s\n", 
               x_raw, y_raw, sw_pressed ? "PRESSED" : "RELEASED");

        // Delay 100ms to keep the serial stream readable
        sleep_ms(100);
    }

    return 0;
}
