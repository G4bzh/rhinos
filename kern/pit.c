/*
 * Gestion du timer i82C54
 *
 */

#include <types.h>
#include "assert.h"
#include "const.h"
#include "klib.h"
#include "tables.h"
#include "interrupt.h"
#include "irq.h"
#include "thread.h"
#include "sched.h"
#include "pit.h"



/*======================================================================== 
 * Declarations PRIVATE 
 *========================================================================*/


PRIVATE void pit_handler(struct context_cpu* ctx);
PRIVATE struct int_node pit_irq_node;


/*========================================================================
 * Initialise l'horloge avec une frequence
 *========================================================================*/

PUBLIC void pit_init()
{
  u32_t ticks;

  /* Determine le nombre de pulsations horloge avant interruption */
  ticks = PIT_MAX_FREQ/PIT_FREQ;

  /* Les compteurs sont sur 16bits
   * La valeur max est donc 2^16=65535
   * La valeur 65536 equivaut a 0 
   */
  if (ticks <= 65536)
    {
      /* 65536 = 0 */
      ticks = (ticks==65536?0:ticks);

      /* Envoie le mot de controle */
      klib_outb(PIT_CWREG,PIT_MODE2);

      /* Envoie la frequence */
      klib_outb(PIT_COUNTER0,(u8_t)ticks);        /* LSB d abord */
      klib_outb(PIT_COUNTER0,(u8_t)(ticks>>8));   /* MSB ensuite */
    }
  
  /* Cree le noeud irq */
  pit_irq_node.flih = pit_handler;
  irq_add_flih(0,&pit_irq_node);

  return;
}


/*========================================================================
 * Handler (flih) 
 *========================================================================*/


PRIVATE void pit_handler(struct context_cpu* ctx)
{
  struct thread* th;

  th = sched_get_running_thread();
  ASSERT_RETURN_VOID( th!=NULL );
  ASSERT_RETURN_VOID( th->ctx==ctx );

  thread_switch(th, THREAD_READY, THREAD_SWITCH_NO_INT);

  return;
}


/*========================================================================
 * Lit l horloge
 *========================================================================*/


PUBLIC u16_t pit_read()
{
  u8_t r1,r2;

  klib_outb(PIT_CWREG,PIT_LATCH); /* Active le counter latch */
  klib_inb(PIT_COUNTER0,&r1);      /* Lit le LSB */
  klib_inb(PIT_COUNTER0,&r2);      /* Lit le MSB */

  return ( r1 | (r2<<8) );
}
