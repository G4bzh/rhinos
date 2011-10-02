/*
 * RhinOS Main
 *
 */


/*========================================================================
 * Includes 
 *========================================================================*/

#include <types.h>
#include "const.h"
#include <llist.h>
#include "start.h"
#include "klib.h"
#include "assert.h"
#include "physmem.h"
#include "paging.h"
#include "virtmem.h"
#include "context_cpu.h"
#include "irq.h"
#include "pit.h"


/*========================================================================
 * Fonction principale 
 *========================================================================*/

PUBLIC int main()
{

  /* Initialisation de la memoire physique */
  phys_init();
  bochs_print("Physical Memory Manager initialized\n");

  /* Initialisation de la pagination */
  paging_init();
  bochs_print("Paging enabled\n");

  /* Initialisation de la memoire virtuelle */
  virt_init();
  bochs_print("Virtual Memory Manager initialized\n");

  /* Initialisation des contextes */
  context_cpu_init();
  bochs_print("Kernel Context initialized\n");

  /* Initialisation du gestionnaire des IRQ */
  irq_init();

  /* Initialisation Horloge */
  pit_init();
  bochs_print("Clock initialized (100Hz)\n");

  //context_cpu_switch_to(kern_ctx);
  
  /*
  virtaddr_t* a = (virtaddr_t*)0x98765;
  u32_t b;
  b = *a; 
  */ 

  /* On ne doit plus arriver ici (sauf DEBUG) */
  while(1)
    {
      //bochs_print("");
    }

  return EXIT_SUCCESS;
}
