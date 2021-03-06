/**

   context.h
   =========

   thread context definition

**/


#ifndef CONTEXT_H
#define CONTEXT_H


/**

   Includes
   --------

   - types.h

**/


#include <types.h>



/**

   Constants: Registers involved in message passing
   -----------------------------------------------

**/

#define CTX_ESI        0
#define CTX_EDI        1
#define CTX_EAX        2
#define CTX_EBX        3
#define CTX_ECX        4
#define CTX_EDX        5


/**

   Structure: struct context
   -------------------------

   CPU Context.
   Registers order match push order during assembly context save

**/

PUBLIC struct x86_context
{
  reg16_t gs;
  reg16_t fs;
  reg16_t es;
  reg16_t ds;
  reg32_t edi;
  reg32_t esi;
  reg32_t ebp;
  reg32_t orig_esp;
  reg32_t ebx;
  reg32_t edx;
  reg32_t ecx;
  reg32_t eax;
  reg32_t ret_addr;
  reg32_t error_code;
  reg32_t eip;
  reg32_t cs;
  reg32_t eflags;
  reg32_t esp;
  reg32_t ss;
} __attribute__ ((packed));



/**

   Prototypes
   ----------

    Give access to context setup, context post save and switch.

**/

PUBLIC u8_t ctx_setup(struct x86_context* ctx, virtaddr_t base, virtaddr_t stack_base, size_t stack_size);
PUBLIC void ctx_postsave(struct x86_context* ctx, reg32_t* esp);
PUBLIC void ctx_prepare_switch(struct x86_context* ctx);
PUBLIC void ctx_set_register(struct x86_context* ctx, u8_t r, u32_t value);
PUBLIC u32_t ctx_get_register(struct x86_context* ctx, u8_t r);


#endif
