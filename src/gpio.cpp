#include "gpio.h"

/*
 * GPIO peripheral class methods.
 */
// Default constructor.
pGPIO::pGPIO() {}

// Convenience initialization constructor.
pGPIO::pGPIO(GPIO_TypeDef* bank) {
  gpio = bank;
  // Gather required register addresses for the GPIO bank.
  
  if (bank == GPIOA) {
    enable_reg = RCC_AHB4ENR ;
    enable_bit = RCC_AHB4ENR_GPIOAEN;
    reset_reg  = RCC_AHB4RSTR;
    reset_bit  = RCC_AHB4RSTR_GPIOARST;
  }
  else {
    // (Unrecognized GPIO bank.)
    status = pSTATUS_ERR;
    return;
  }

  // TODO: Any more GPIO banks on larger chips?
  status = pSTATUS_SET;
}

/*
 * Read the entire 16 bits/pins in the GPIO bank.
 */
unsigned pGPIO::read(void) {
  if (status == pSTATUS_ERR) { return 0; }
  // Return the current value of all 16 pins.
  return gpio->IDR;
}

/*
 * Write all 16 bits/pins in the GPIO bank.
 * This will set all 16 pins to the provided value, 0s included.
 */
void pGPIO::write(unsigned dat) {
  if (status == pSTATUS_ERR) { return; }
  // Write to all pins at once.
  gpio->ODR = dat & 0xFFFF;
}

/*
 * Stream a series of values to the GPIO pins.
 * I don't see why you would, but maybe I can add timed delays.
 */
void pGPIO::stream(volatile void* buf, int len) {
  volatile uint32_t* gpiobuf = (volatile uint32_t*) buf;
  if (status == pSTATUS_ERR) { return; }
  // Write all of the values, one by one.
  for (int si = 0; si < len; ++si) {
    write(gpiobuf[si]);
  }
}

/*
 * Read a single pin's state.
 */
bool pGPIO::read_pin(unsigned pin_num) {
  if (status == pSTATUS_ERR) { return 0; }
  return (gpio->IDR & (1 << pin_num));
}

/*
 * Turn a single pin on.
 */
void pGPIO::pin_on(unsigned pin_num) {
  if (status == pSTATUS_ERR) { return; }
  gpio->ODR |= (1 << pin_num);
}

/*
 * Turn all of the requested pins on, leaving the rest unaffected.
 */
void pGPIO::pins_on(uint16_t pin_mask) {
  if (status == pSTATUS_ERR) { return; }
  // Turn on all of the requested pins.
  gpio->ODR |= pin_mask;
}

/*
 * Turn a single pin off.
 */
void pGPIO::pin_off(unsigned pin_num) {
  if (status == pSTATUS_ERR) { return; }
  // Pull a single pin to ground.
  gpio->ODR &= ~(1 << pin_num);
}

/*
 * Turn off all requested pins, leaving the rest unaffected.
 */
void pGPIO::pins_off(uint16_t pin_mask) {
  if (status == pSTATUS_ERR) { return; }
  // Pull all of the requested pins down.
  gpio->ODR &= ~(pin_mask);
}

/*
 * Toggle a single pin between an on/off state.
 */
void pGPIO::pin_toggle(unsigned pin_num) {
  if (status == pSTATUS_ERR) { return; }
  gpio->ODR ^= (1 << pin_num);
}

/*
 * Toggle all requested pins, leaving the rest unaffected.
 */
void pGPIO::pins_toggle(uint16_t pin_mask) {
  if (status == pSTATUS_ERR) { return; }
  // Toggle all of the requested pins between on/off states.
  gpio->ODR ^= pin_mask;
}


/*
 * Set the MODER register to choose between:
 * 0: Input mode (reset state)
 * 1: Output mode
 * 2: Alternate function mode
 * 3: Analog-Digital Converter input mode
 */
void pGPIO::set_pin_mode(unsigned pin_num, unsigned mode) {
  if (status == pSTATUS_ERR) { return; }
  gpio->MODER &= ~(3 << (pin_num * 2));
  gpio->MODER |=  (mode << (pin_num * 2));
}

/*
 * Set the OTYPER register to choose between:
 * 0: Push-pull output.
 * 1: Open-drain output.
 * (Only applicable to 'output' and 'alternate function' modes.)
 */
void pGPIO::set_pin_type(unsigned pin_num, unsigned otype) {
  if (status == pSTATUS_ERR) { return; }
  gpio->OTYPER &= ~(1 << pin_num);
  gpio->OTYPER |=  (otype << pin_num);
}

/*
 * Set the OSPEEDR register to choose between:
 * 0: Low-speed (<= 2MHz)
 * 1: Mid-speed (<= 10MHz)
 * 2: Low-speed (<= 2MHz)
 * 3: Hi-speed  (<= 50MHz)
 * (Only applicable to 'output' and 'alternate function' modes.)
 */
void pGPIO::set_pin_speed(unsigned pin_num, unsigned ospeed) {
  if (status == pSTATUS_ERR) { return; }
  gpio->OSPEEDR &= ~(3 << (pin_num * 2));
  gpio->OSPEEDR |=  (ospeed << (pin_num * 2));
}

/*
 * Set the PUPDR register to choose whether to use an internal
 * pull-up or pull-down resistor:
 * 0: No pull-up or pull-down resistor.
 * 1: Use a pull-up resistor. (~47K?)
 * 2: Use a pull-down resistor. (~47K?)
 * 3: Reserved (do not use.)
 * (Not applicable to 'analog input' mode.)
 */
void pGPIO::set_pin_pupd(unsigned pin_num, unsigned pupd) {
  if (status == pSTATUS_ERR) { return; }
  gpio->PUPDR &= ~(3 << (pin_num * 2));
  gpio->PUPDR |=  (pupd << (pin_num * 2));
}

/*
 * Set the pin's alternate function mode. Any one pin on the STM32
 * can map to up to 15 peripherals in alternate-function mode.
 * This is how the chip knows that you want to use pins for
 * a specific peripheral like I2C, SPI, PWM, etc.
 * (Only applicable to 'alternate function' mode.)
 */
void pGPIO::set_pin_af(unsigned pin_num, unsigned af) {
  if (status == pSTATUS_ERR) { return; }
  if (pin_num < 8) {
    gpio->AFR[0] &= ~(0xF << (pin_num * 4));
    gpio->AFR[0] |=  (af  << (pin_num * 4));
  }
  else {
    gpio->AFR[1] &= ~(0xF << ((pin_num - 8) * 4));
    gpio->AFR[1] |=  (af  << ((pin_num - 8) * 4));
  }
}