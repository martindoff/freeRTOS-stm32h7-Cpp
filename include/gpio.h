#ifndef __STARm_GPIO_H
#define __STARm_GPIO_H

// Project includes.
#include "core.h"

// Project macro definitions.
// GPIO pin state macros.
#define pGPIO_MODE_IN        (0x00)
#define pGPIO_MODE_OUT       (0x01)
#define pGPIO_MODE_ALT       (0x02)
#define pGPIO_MODE_AIN       (0x03)
#define pGPIO_OTYPE_PP       (0x00)
#define pGPIO_OTYPE_OD       (0x01)
#define pGPIO_SPEED_LOW      (0x00)
#define pGPIO_SPEED_MED      (0x01)
#define pGPIO_SPEED_HI       (0x02)
#define pGPIO_SPEED_VHI      (0x03)
#define pGPIO_PUPD_NONE      (0x00)
#define pGPIO_PUPD_UP        (0x01)
#define pGPIO_PUPD_DOWN      (0x02)

/*
 * GPIO peripheral class.
 */
class pGPIO : public pIO {
public:
  // Constructors.
  pGPIO();
  pGPIO(GPIO_TypeDef* bank);
  // Common r/w methods from the core I/O class.
  unsigned read(void);
  void     write(unsigned dat);
  void     stream(volatile void* buf, int len);
  // GPIO-specific methods.
  // Generic pin manipulation methods.
  bool     read_pin(unsigned pin_num);
  void     pin_on(unsigned pin_num);
  void     pins_on(uint16_t pin_mask);
  void     pin_off(unsigned pin_num);
  void     pins_off(uint16_t pin_mask);
  void     pin_toggle(unsigned pin_num);
  void     pins_toggle(uint16_t pin_mask);
  // Register modification methods; platform-specific.
    void   set_pin_mode(unsigned pin_num, unsigned mode);
    void   set_pin_type(unsigned pin_num, unsigned otype);
    void   set_pin_speed(unsigned pin_num, unsigned ospeed);
    void   set_pin_pupd(unsigned pin_num, unsigned pupd);
    void   set_pin_af(unsigned pin_num, unsigned af);
  
protected:
  // Reference GPIO register struct.
  GPIO_TypeDef* gpio        = NULL;
private:
};

#endif
