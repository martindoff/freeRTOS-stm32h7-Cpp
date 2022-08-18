#ifndef __STARm_CORE_H
#define __STARm_CORE_H

#include <stdio.h>
#include "stm32h7xx.h"
#include "register_macros.h"

// Global macro definitions.
#define pSTATUS_ERR (0)
#define pSTATUS_SET (1)
#define pSTATUS_ON  (2)
#define pSTATUS_RUN (3)

// Class declarations for basic structures common
// to many peripherals.
/*
 * Input/Output class. This covers things that read and/or write
 * data between devices. I2C, SPI, UART, etc.
 */
class pIO {
public:
  pIO();
  // Common read/write methods.
  virtual unsigned read(void);
  virtual void     write(unsigned dat);
  virtual void     stream(volatile void* buf, int len);
  // Common peripheral control methods.
  virtual void     clock_en(void);
  virtual void     reset(void);
  virtual void     disable(void);
  virtual int      get_status(void);
protected:
  // Expected peripheral status.
  int status = pSTATUS_ERR;
  // Enable/disable/reset register definitions.
  __IO uint32_t *enable_reg = 0;
  __IO uint32_t *reset_reg  = 0;
  uint32_t       enable_bit = 0;
  uint32_t       reset_bit  = 0;
private:
};

#endif
