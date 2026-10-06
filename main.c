#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "tusb.h"
#include "bsp/board.h"


// --------------------------------------------------
// Joystick connections
// --------------------------------------------------

// Joystick 1 X-axis
// GPIO26 = ADC0
#define JOY_LEFT_RIGHT 0


// --------------------------------------------------
// Joystick thresholds
// --------------------------------------------------

#define LOW_THRESHOLD  1500
#define HIGH_THRESHOLD 2500


// --------------------------------------------------
// Main
// --------------------------------------------------

int main(void)
{
    board_init();
    // Initialize ADC
    adc_init();

    // GPIO26 -> ADC0
    adc_gpio_init(26);

    

    // Initialize USB
    tusb_init();


    while (true)
    {
        // Let TinyUSB process USB events
        tud_task();


        // Only send when USB keyboard is ready
        if (tud_hid_ready())
        {
            uint8_t keycode[6] = { 0 };

            // ==========================================
            // JOYSTICK 1
            // LEFT / RIGHT
            // ==========================================

            adc_select_input(JOY_LEFT_RIGHT);

            uint16_t left_right = adc_read();


            if (left_right < LOW_THRESHOLD)
            {
                keycode[0] = HID_KEY_ARROW_LEFT;
            }
            else if (left_right > HIGH_THRESHOLD)
            {
                keycode[0] = HID_KEY_ARROW_RIGHT;
            }

            // ==========================================
            // SEND KEYBOARD REPORT
            // ==========================================

            tud_hid_keyboard_report(
                0,
                0,
                keycode
            );
        }


        // Run at approximately 100 Hz
        sleep_ms(10);
    }
}