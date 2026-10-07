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
#define JOY_UP_DOWN 1


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
    adc_gpio_init(27); // GPIO27 -> ADC1
   

    // Initialize USB
    tusb_init();
        unsigned int val_h = 0;
        unsigned int val_v = 0;
    uint current_left_right = 0,prev_left_right = 0,current_space = 0;
    uint current_up_down = 0,prev_up_down = 0,prev_space = 0;
 uint space = 10;
    gpio_init(space);
    gpio_set_dir(space, GPIO_IN);
    gpio_pull_up(space);
       
    while (true)
    {
        // Let TinyUSB process USB events
        tud_task();
        

        // Only send when USB keyboard is ready
        if (tud_hid_ready())
        {
            

            // ==========================================
            // JOYSTICK 1
            // LEFT / RIGHT
            // ==========================================

            adc_select_input(JOY_LEFT_RIGHT);

            val_h = adc_read();

            adc_select_input(JOY_UP_DOWN);
             val_v = adc_read();


            if (val_h < LOW_THRESHOLD)
            {
                current_left_right = HID_KEY_ARROW_LEFT;
            }
            else if (val_h > HIGH_THRESHOLD)
            {
                current_left_right = HID_KEY_ARROW_RIGHT;
            }else{
                current_left_right = 0;
            }
            if (val_v < LOW_THRESHOLD)
            {
                current_up_down = HID_KEY_ARROW_UP;
            }
            else if (val_v > HIGH_THRESHOLD)
            {
                current_up_down = HID_KEY_ARROW_DOWN;
            }else{
                current_up_down = 0;
            }
            if(gpio_get(space) == 0){
                current_space = HID_KEY_SPACE;
            }else{
                current_space = 0;
            }
            // ==========================================
            // SEND KEYBOARD REPORT
            // ==========================================
            if(current_left_right != prev_left_right || current_up_down != prev_up_down || current_space != prev_space){
                uint8_t keycode[6] = { 0 };
                keycode[0] = current_left_right;
                keycode[1] = current_up_down;
                keycode[2] = current_space;

                 tud_hid_keyboard_report(0,0,keycode);
                 
                prev_left_right = current_left_right;
                prev_up_down = current_up_down;
                prev_space = current_space;
            }

           
        }


        // Run at approximately 100 Hz
        sleep_ms(10);
    }
}