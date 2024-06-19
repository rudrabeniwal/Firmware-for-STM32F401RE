// Include necessary library files from libopencm3
// - stm32/rcc.h: Provides functions for Reset and Clock Control (RCC) to manage peripheral clocks
// - stm32/gpio.h: Provides functions for General-Purpose Input/Output (GPIO) to manage pin configurations
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include <libopencm3/cm3/scb.h>

// Include the core system header file for system-level configurations and functionalities
#include "core/system.h"
#include "core/timer.h"

#define BOOTLOADER_SIZE (0x8000U)

// Define macros for the LED port and pin
// LED_PORT: The GPIO port where the LED is connected (GPIOA)
// LED_PIN: The GPIO pin number where the LED is connected (GPIO5)
#define LED_PORT (GPIOA)
#define LED_PIN (GPIO5)

static void vector_setup(void) {
    SCB_VTOR = BOOTLOADER_SIZE; //Before this the offset of that register is zero and looked into normal location, so we add bootloader location
}

// Define a static function to set up the GPIO
// This function enables the clock for GPIOA and configures the specified pin as an output
static void gpio_setup(void) {
    // Enable the clock for GPIO port A
    // rcc_periph_clock_enable(peripheral): Enables the clock for the specified peripheral
    // RCC_GPIOA: Identifier for GPIO port A
    rcc_periph_clock_enable(RCC_GPIOA);
    
    // Set up the mode for the specified GPIO pin
    // gpio_mode_setup(port, mode, pull_up_down, pin): Configures the mode for a specific GPIO pin
    // LED_PORT: The port where the LED is connected (GPIOA)
    // GPIO_MODE_OUTPUT: Set the pin as an output
    // GPIO_PUPD_NONE: No pull-up or pull-down resistor
    // LED_PIN: The pin number where the LED is connected (GPIO5)
    //gpio_mode_setup(LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PIN);

    gpio_mode_setup(LED_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, LED_PIN);
    gpio_set_af(LED_PORT, GPIO_AF1, LED_PIN);
}

// The main function where program execution starts
int main(void) {

    vector_setup();

    // Perform system setup which includes setting up RCC and SysTick
    system_setup();
    
    // Perform GPIO setup to configure the LED pin as an output
    gpio_setup();

    timer_setup();

    // Retrieve the current system tick count to use as a reference point for timing
    uint64_t start_time = system_get_ticks();

    float duty_cycle = 0.0f;

    timer_pwm_set_duty_cycle(duty_cycle);

    // Infinite loop to continuously check the tick count and toggle the LED
    while (1) {
        // Check if 100 system ticks have elapsed since the last toggle
        if (system_get_ticks() - start_time >= 10) {
            duty_cycle += 1.0f;
            if (duty_cycle > 100.0f){
                duty_cycle = 0.0f;
            }
            timer_pwm_set_duty_cycle(duty_cycle);
            
            // Update the reference start time to the current tick count
            start_time = system_get_ticks();
        }
        
        // Perform useful work here
        // This is a placeholder for additional functionality that might be added later
    }

    // This return statement is never reached because the main function contains an infinite loop
    // In embedded systems, the main function typically does not return
    return 0;
}
