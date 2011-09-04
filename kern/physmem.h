/*
 * Header de phymem.c
 *
 */

#ifndef PHYSMEM_C
#define PHYSMEM_C


/*========================================================================
 * Includes 
 *========================================================================*/

#include <types.h>
#include "virtmem_slab.h"
#include "virtmem_buddy.h"


/*========================================================================
 * Constantes 
 *========================================================================*/

#define PHYS_PAGE_SIZE           4096
#define PHYS_PAGE_MAX_BUDDY      21        /* 32 - 12 + 1 = 21 */
#define PHYS_PAGE_SHIFT          12        /* 2^12=4096    */
#define PHYS_PAGE_NODE_POOL_ADDR 0x100000  /* Adresse de base du pool de ppage nodes */

#define PHYS_SHIFT1024            10        /* 2^10 = 1024  */
#define PHYS_SHIFT64               6        /* 2^6 = 64    */

#define PHYS_KERN_AREA_START       0
#define PHYS_ROM_AREA_START        0x9FC00
#define PHYS_ROM_AREA_SIZE         0x603FF
#define PHYS_POOL_AREA_START       PHYS_PAGE_NODE_POOL_ADDR
#define PHYS_ACPI_AREA_START       0xFEC00000
#define PHYS_ACPI_AREA_SIZE        0x13FFFFF


#define PHYS_UNMAP_NONE       0
#define PHYS_UNMAP_UNMAP      1
#define PHYS_UNMAP_FREE       2

/*========================================================================
 * Macros
 *========================================================================*/

/* Alignement inferieur sur PPAGE_SIZE */
#define PHYS_ALIGN_INF(addr)			\
  ( ((addr) >> PHYS_PAGE_SHIFT) << PHYS_PAGE_SHIFT )

/* Alignement superieur sur PPAGE_SIZE */
#define PHYS_ALIGN_SUP(addr)						\
  ( (((addr)&0xFFFFF000) == (addr))?((addr) >> PHYS_PAGE_SHIFT) << PHYS_PAGE_SHIFT:(((addr) >> PHYS_PAGE_SHIFT)+1) << PHYS_PAGE_SHIFT )

/* Description de page d une adresse donnee */
#define PHYS_GET_DESC(addr)						\
  ( (struct ppage_desc*)(PHYS_POOL_AREA_START + ((addr) >> PHYS_PAGE_SHIFT)*sizeof(struct ppage_desc)) )

/* Existence d une page description */
#define PHYS_PDESC_ISNULL(pdescaddr)		\
  ( ((physaddr_t)(pdescaddr)) == PHYS_POOL_AREA_START )


/* Zeroing d un ppage_desc */
#define PHYS_NULLIFY_DESC(desc)			\
  {						\
    (desc)->start = 0;				\
    (desc)->size = 0;				\
    (desc)->maps = 0;				\
    (desc)->index = 0;				\
    (desc)->cache = NULL;			\
    (desc)->bufctl = NULL;			\
    (desc)->area = NULL;			\
    (desc)->prev = NULL;			\
    (desc)->next = NULL;			\
  }


/*========================================================================
 * Structures 
 *========================================================================*/

/* Noeud des buddies */

struct ppage_desc 
{
  physaddr_t start;
  u32_t size;
  u16_t maps;
  u8_t  index;
  struct vmem_cache* cache;
  struct vmem_bufctl* bufctl;
  struct vmem_area* area;
  struct ppage_desc* prev;
  struct ppage_desc* next;
}__attribute__((packed));


/*========================================================================
 * Prototypes 
 *========================================================================*/

PUBLIC void phys_init(void);
PUBLIC void* phys_alloc(u32_t size);
PUBLIC void phys_free(void* addr);
PUBLIC void phys_map(physaddr_t addr);
PUBLIC u8_t phys_unmap(physaddr_t addr);

/* DEBUG */
PUBLIC void phys_print_buddy(void);

#endif
