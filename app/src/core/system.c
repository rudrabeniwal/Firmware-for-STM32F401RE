// Include the core system header file which typically defines system-level configurations and functionalities
#include "core/system.h"

// Include the necessary library files from libopencm3, a lightweight library for microcontroller peripherals
// - cm3/systick.h: Contains functions and definitions for the system tick timer, part of the Cortex-M core
// - cm3/vector.h: Manages interrupt vectors for the ARM Cortex-M microcontrollers
// - stm32/rcc.h: Provides functions to configure the RCC (Reset and Clock Control) for STM32 microcontrollers
#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/vector.h>
#include <libopencm3/stm32/rcc.h>

// Declare a static volatile variable to store the number of system ticks
// - static: Limits the scope of the variable to this file
// - volatile: Prevents compiler optimizations, ensuring the variable is always read from memory
static volatile uint64_t ticks = 0;

// Define the system tick handler function
// This function is automatically called by the system when the SysTick timer overflows
// It increments the 'ticks' variable to keep track of the number of system ticks
void sys_tick_handler(void) {
    ticks++;
}

// Define a static function to configure the SysTick timer
// - static: Limits the scope of the function to this file
static void systick_setup(void) {
    // Configure the frequency of the SysTick timer
    // systick_set_frequency(frequency, cpu_frequency) sets the SysTick timer to generate interrupts at a specific frequency
    // SYSTICK_FREQ and CPU_FREQ are pre-defined macros that specify the desired tick frequency and CPU clock frequency, respectively
    systick_set_frequency(SYSTICK_FREQ, CPU_FREQ);
    
    // Enable the SysTick counter
    // This starts the SysTick timer
    systick_counter_enable();
    
    // Enable SysTick interrupts
    // This allows the SysTick handler function to be called when the timer overflows
    systick_interrupt_enable();
}

// Define a function to retrieve the current tick count
// This function returns the value of the 'ticks' variable, providing a way to measure elapsed time
uint64_t system_get_ticks(void) {
    return ticks;
}

// Define a static function to configure the RCC (Reset and Clock Control) system
// - static: Limits the scope of the function to this file
static void rcc_setup(void) {
    // Configure the clock using the PLL (Phase-Locked Loop) with a specific configuration
    // rcc_clock_setup_pll(config) sets up the system clock using a predefined configuration
    // &rcc_hsi_configs[RCC_CLOCK_3V3_84MHZ] is a pointer to a configuration structure that sets the clock to 84 MHz with 3.3V operation
    rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_3V3_84MHZ]);
}

// Define a function to perform overall system setup
// This function calls other setup functions to configure the RCC and SysTick timer
void system_setup(void) {
    // Set up the RCC system to configure the microcontroller's clock
    rcc_setup();
    
    // Set up the SysTick timer to generate periodic interrupts
    systick_setup();
}
