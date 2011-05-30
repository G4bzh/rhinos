/*
 * Header de phymem.c
 *
 */

#ifndef PHYSMEM_C
#define PHYSMEM_C


/**************
 * Constantes 
 **************/

#define PPAGE_MAX_BUDDY      21        /* 32 - 12 + 1 = 21 */
#define PPAGE_SHIFT          12        /* 2^12=4096    */
#define PPAGE_NODE_POOL_ADDR 0x100000  /* Adresse de base du pool de ppage nodes */

#define SHIFT1024            10        /* 2^10 = 1024  */
#define SHIFT64               6        /* 2^6 = 64    */

#define KERN_AREA_START       0
#define ROM_AREA_START        0x9FC00
#define ROM_AREA_SIZE         0x603FF
#define POOL_AREA_START       PPAGE_NODE_POOL_ADDR
#define ACPI_AREA_START       0xFEC00000
#define ACPI_AREA_SIZE        0x13FFFFF


#define PHYS_UNMAP_NONE       0
#define PHYS_UNMAP_UNMAP      1
#define PHYS_UNMAP_FREE       2


/***************
 * Structures 
 ***************/

struct ppage_node 
{
  physaddr_t start;
  u32_t size;
  u16_t maps;
  u8_t  index;
  struct ppage_node* prev;
  struct ppage_node* next;
}__attribute__((packed));


/***************
 * Prototypes 
 ***************/

PUBLIC void physmem_init(void);
PUBLIC void* phys_alloc(u32_t size);
PUBLIC void phys_free(void* addr);
PUBLIC void phys_map(void* addr);
PUBLIC u8_t phys_unmap(void* addr);

#endif
