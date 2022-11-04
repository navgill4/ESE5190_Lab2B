#include "pico/stdlib.h"
#include <stdio.h>
#include "neopixel.h"
#include "hardware/gpio.h"
#include <string.h>
#include <math.h>
#include "registers.h"

#define QTPY_BOOT_PIN 21
#define NEOPIXEL_POWER 11
#define NEOPIXEL 12

volatile uint32_t * boot_pin_address;
volatile uint32_t full_gpio_register_value;
volatile uint32_t pin_21_selection_mask;
volatile uint32_t selected_pin_state;
volatile uint32_t shifted_pin_21_state;
typedef uint32_t VALUE;




typedef struct {
    uint32_t last_serial_byte;
    uint32_t button_is_pressed;
    uint32_t light_color;
    char * reg[8];
    uint32_t * register_value;
} Flashlight; 

void render_to_console(Flashlight status) {
    // adjust console window height to match 'frame size'
    
    printf("button_is_pressed:  0x%08x\n",   status.button_is_pressed);
    printf("light_color:        0x%08x\n",   status.light_color);
    printf("boot pin address:     0x%08x\n", boot_pin_address);
    printf("full gpio reg: 0x%08x\n", full_gpio_register_value);
    printf("selected pin state: 0x%08x\n", selected_pin_state);
}  

int main() {

    stdio_init_all();
    gpio_init(QTPY_BOOT_PIN);
    gpio_set_dir(QTPY_BOOT_PIN, GPIO_IN);
    neopixel_init();

    Flashlight status;
    status.last_serial_byte =  0x00000000;
    status.button_is_pressed = 0x00000000;
    status.light_color =       0x000000ff;
    

    while (true) {
        
        status.last_serial_byte = getchar_timeout_us(0); // don't block main loop
        switch(status.last_serial_byte) { // poll every cycle
            case 'r':
                status.light_color = 0x00ff0000;
                break;
            case 'g':
                status.light_color = 0x0000ff00;
                break;
            case 'b':
                status.light_color = 0x000000ff;
                break;
            case 'w':
                status.light_color = 0x00ffffff;
                break;
            
            //case 'x':
                //scanf("%s", &status.reg);
                //scanf("%08x", &status.register_value);
                //boot_pin_address = (volatile uint32_t *) status.register_value;
                //full_gpio_register_value = (uint32_t) * boot_pin_address;
                //pin_21_selection_mask = 1u << 21;
                //selected_pin_state = full_gpio_register_value & pin_21_selection_mask;
                //shifted_pin_21_state = selected_pin_state >> 21;
                //status.button_is_pressed = 0x00000000;
                //sscanf(status.reg "%08x", &status.register_value);
        }
        //if (gpio_get(QTPY_BOOT_PIN)) { // poll every cycle, 0 = "pressed"
        boot_pin_address = (volatile uint32_t *) 0xd0000004;
        full_gpio_register_value = (uint32_t) * boot_pin_address;
        pin_21_selection_mask = 1u << 21;
        selected_pin_state = full_gpio_register_value & pin_21_selection_mask;
        shifted_pin_21_state = selected_pin_state >> 21;
        if (shifted_pin_21_state){
            status.button_is_pressed = 0x00000000;
        } else {
            status.button_is_pressed = 0x00000001;
        }
        if (status.button_is_pressed) { // poll every cycle
            neopixel_set_rgb(status.light_color);
        } else {
            neopixel_set_rgb(0x00000000);
        }
        render_to_console(status);
        sleep_ms(10); // don't DDOS the serial console
    }
    return 0;
}                  
