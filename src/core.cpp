#include "core.h"

/* Common Input/Output class default constructor. */
pIO::pIO() {}

/*
 * Default virtual read/write methods for the common I/O class.
 * Most of these methods don't have a default behavior, but I
 * don't want to use a pure virtual class for several reasons.
 */
// Read the peripheral's default data length. (Up to a word)
unsigned pIO::read(void) { return 0; }

// Write the peripheral's default data width (up to a word)
void pIO::write(unsigned dat) {}

// Write multiple words to the peripheral in a stream.
void pIO::stream(volatile void* buf, int len) {}

// Enable the peripheral clock.
void pIO::clock_en(void) {
  if (status == pSTATUS_ERR) { return; }
  *enable_reg |= enable_bit;
  status = pSTATUS_ON;
}

// Reset the peripheral.
void pIO::reset(void) {
  if (status == pSTATUS_ERR) { return; }
  *reset_reg |= reset_bit;
  *reset_reg &= ~(reset_bit);
}

// Turn the peripheral off.
void pIO::disable(void) {
  if (status == pSTATUS_ERR) { return; }
  *enable_reg &= ~(enable_bit);
  status = pSTATUS_SET;
}

// Return the current peripheral status,
// as far as the library knows.
int pIO::get_status(void) { return status; }
