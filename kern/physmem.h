/*
 * Header de phymem.c
 *
 */

#ifndef PHYSMEM_C
#define PHYSMEM_C


/**************
 * Constantes 
 **************/

#define PPAGE_MAX_BUDDY      20        /* 32 - 12 = 20 */
#define PPAGE_SHIFT          12        /* 2^12=4096 */
#define PPAGE_NODE_POOL_ADDR 0x100000  /* Adresse de base du pool de ppage nodes */

/***************
 * Structures 
 ***************/

struct ppage_node 
{
  u32_t start;
  u32_t size;
  u8_t  index;
  struct ppage_node* prev;
  struct ppage_node* next;
};


/***************
 * Prototypes 
 ***************/

PUBLIC void physmem_init(void);
PUBLIC void* phys_alloc(u32_t size);
PUBLIC void phys_free(void* addr);

#endif
