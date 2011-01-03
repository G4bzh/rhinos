#ifndef START_H
#define START_H


/**************
 * Includes
 **************/

#include <types.h>

/********************** 
 * Structure boot_info
 **********************/

PUBLIC struct boot_info
{
  u32_t kern_start;     /* Debut du noyau */
  u32_t kern_end;       /* Fin du noyau */
  u32_t mem_addr;       /* Adresse du memory map */
  u16_t mem_entry;      /* Nombre d entree dans le memory map */
  u16_t mem_lower;      /* Memoire basse en Ko */
  u16_t mem_upper;      /* Memoire haute en 64Ko */
};

/******************************
 * Structure boot_memmap_entry
 ******************************/

PUBLIC struct boot_memmap_entry
{
  u64_t addr;           /* Adresse de la zone */
  u64_t size;           /* Taille de la zone */
  u32_t type;           /* Type de memoire */
} __attribute__ ((packed));


/*************
 * Prototypes
 *************/

PUBLIC struct boot_info bootinfo;

#endif
