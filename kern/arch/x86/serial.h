/**

   serial.h
   ========

   Basic serial port management header

**/



#ifndef SERIAL_H
#define SERIAL_H


/**

   Includes
   --------

   - define.h
   - types.h
 
**/

#include <define.h>
#include <types.h>


/**

   Prototypes
   ==========

   Give access to serial port initialization and character output
   serial_printf is a temporary function, for debug purpose only

**/


PUBLIC void serial_init(void);
PUBLIC void serial_printf(const char* str,...);
PUBLIC void serial_putc(char c);

#endif
