#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

/*========================================================================
 * Includes 
 *========================================================================*/

#include <types.h>
#include "context_cpu.h"


/*========================================================================
 * Prototypes
 *========================================================================*/

PUBLIC void excep_handle(u32_t num, struct context_cpu* ctx);

#endif
