/*
 * Assignment #5
 *
 * Author: Santiago Pina Ros
 *
 * Size of the code: 1884 bytes
 *
 * Useful links:
 * http://www2.cs.uidaho.edu/~rinker/cs452/char-comm.pdf
 * http://www.nongnu.org/avr-libc/user-manual/group__avr__stdio.html
 */


#ifndef __LCD_H_INCLUDED__
#define __LCD_H_INCLUDED__

#include <stdbool.h>
#include <stdint.h>

/* CPU frequency */
#define F_CPU 1000000UL


/*
 * Send byte b to the LCD.  rs is the RS signal (register select), 0
 * selects instruction register, 1 selects the data register.
 */
void	lcd_outbyte(uint8_t b, uint8_t rs);

/*
 * Read one byte from the LCD controller.  rs is the RS signal, 0
 * selects busy flag (bit 7) and address counter, 1 selects the data
 * register.
 */
uint8_t	lcd_inbyte(uint8_t rs);

/*
 * Wait for the busy flag to clear.
 */
void	lcd_wait_ready();

/*
 * Initialize the LCD controller hardware.
 */
void	lcd_init(void);

/*
 * Prepare the LCD controller pins for powerdown.
 */
void	lcd_powerdown(void);


/* Send a command to the LCD controller. */
#define lcd_outcmd(n)	lcd_outbyte((n), 0)

/* Send a data byte to the LCD controller. */
#define lcd_outdata(n)	lcd_outbyte((n), 1)

/* Read the address counter and busy flag from the LCD. */
#define lcd_incmd()		lcd_inbyte(0)

/* Read the current data byte from the LCD. */
#define lcd_indata()	lcd_inbyte(1)

#endif // __LCD_H_INCLUDED__ 