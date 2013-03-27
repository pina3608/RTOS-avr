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


#include "lcd.h"


#include <avr/io.h>
#include <util/delay.h>


/*  LCD port connections */
#define LCD_RS D, 1
#define LCD_RW D, 2
#define LCD_E  D, 0
#define LCD_D0 A


#define GLUE(a, b)     a##b

/* single-bit macros, used for control bits */
#define SET_(what, p, m) GLUE(what, p) |= (1 << (m))
#define CLR_(what, p, m) GLUE(what, p) &= ~(1 << (m))
#define GET_(/* PIN, */ p, m) GLUE(PIN, p) & (1 << (m))
#define SET(what, x) SET_(what, x)
#define CLR(what, x) CLR_(what, x)
#define GET(/* PIN, */ x) GET_(x)

/* macros used for data path */

#define ASSIGN(what, p, v) GLUE(what, p) = (v)
#define READ(what, p) (GLUE(what, p))

#define LCD_BUSYFLAG 0x80

/* init LCD macros */

#define INIT_START 0x30
#define INIT_FSET 0x38
#define INIT_DOFF 0x08
#define INIT_DCLEAR 0x01
#define INIT_ENTRYM 0x06
#define INIT_DENABLE 0x0C //0x0E

/*
 * Send one pulse to the E signal (enable). If readback is set to true,
 * read the LCD data pins right before the falling edge of E, and return
 * that value.
 */

static uint8_t lcd_pulse_e(bool readback){

  uint8_t x;

  SET(PORT, LCD_E);
  _delay_us(0.5);

  if (readback)
    x = READ(PIN, LCD_D0);
  else
    x = 0;
  CLR(PORT, LCD_E);

  return x;
}

/*
 * Send one byte to the LCD controller.
 */
void lcd_outbyte(uint8_t b, uint8_t rs){

  CLR(PORT, LCD_RW);
  if (rs)
    SET(PORT, LCD_RS);
  else
    CLR(PORT, LCD_RS);
  ASSIGN(PORT, LCD_D0, b);
  (void)lcd_pulse_e(false);
}


/*
 * Read one byte from the LCD controller.
 */
uint8_t lcd_inbyte(uint8_t rs){

  uint8_t x;

  SET(PORT, LCD_RW);
  ASSIGN(DDR, LCD_D0, 0x00);
  if (rs)
    SET(PORT, LCD_RS);
  else
    CLR(PORT, LCD_RS);
  x = lcd_pulse_e(true);
  ASSIGN(DDR, LCD_D0, 0xFF);
  CLR(PORT, LCD_RW);

  return x;
}


/*
 * Wait until the busy flag is cleared.
 */
void lcd_wait_ready(){

  while (lcd_incmd() & LCD_BUSYFLAG) ;

}
/*
 * Initilize the LCD.
 */
void lcd_init(void){

  SET(DDR, LCD_RS);
  SET(DDR, LCD_RW);
  SET(DDR, LCD_E);
  ASSIGN(DDR, LCD_D0, 0x0F);

  _delay_ms(15); // Wait time specified in documentation
  lcd_outcmd(INIT_START);
  _delay_ms(4.1); // Wait time specified in documentation
  lcd_outcmd(INIT_START);
  _delay_ms(0.1); // Wait time specified in documentation
  lcd_outcmd(INIT_START);
  _delay_us(37); // Wait time specified in documentation

  lcd_outcmd(INIT_FSET);
  lcd_wait_ready();
  lcd_outcmd(INIT_DOFF);
  lcd_wait_ready();
  
    /*
   * Clear the display.
   */
  lcd_outcmd(INIT_DCLEAR);
  lcd_wait_ready();

  /*
   * Entry mode.
   */
  lcd_outcmd(INIT_ENTRYM);
  lcd_wait_ready();

  /*
   * Enable display
   */
  
  lcd_outcmd(INIT_DENABLE);
  lcd_wait_ready();
}

/*
 * Prepare the LCD controller pins for powerdown.
 */
void lcd_powerdown(void){

  ASSIGN(PORT, LCD_D0, 0);
  CLR(PORT, LCD_RS);
  CLR(PORT, LCD_RW);
  CLR(PORT, LCD_E);
}
