/*
 * Const.h
 * Constantes partagees du noyau
 *
 */

#ifndef CONST_H
#define CONST_H

#include "start.h"
#include "paging.h"

/*========================================================================
 * Memoire
 *========================================================================*/


#define CONST_PAGE_SIZE              4096
#define CONST_PAGE_SHIFT             12         /* 2^12=4096    */
#define CONST_PAGE_NODE_POOL_ADDR    PAGING_ALIGN_SUP(bootinfo->kern_end) //0x100000

#define CONST_ROM_AREA_START        0x9F000
#define CONST_ROM_AREA_SIZE         0x60FFF
#define CONST_ACPI_AREA_START       0xFEC00000
#define CONST_ACPI_AREA_SIZE        0x13FFFFF

#define CONST_KERN_CS_SELECTOR       8    /*  CS = 0000000000001  0  00   =  8  */
#define	CONST_KERN_DS_SELECTOR       16   /*  DS = 0000000000010  0  00   =  16 */
#define	CONST_KERN_ES_SELECTOR	     16   /*  ES = 0000000000010  0  00   =  16 */
#define	CONST_KERN_SS_SELECTOR	     16   /*  SS = 0000000000010  0  00   =  16 */

#define CONST_USER_CS_SELECTOR       27   /*  CS = 0000000000011  0  11   =  8  */
#define	CONST_USER_DS_SELECTOR       35   /*  DS = 0000000000100  0  11   =  16 */
#define	CONST_USER_ES_SELECTOR	     35   /*  ES = 0000000000100  0  11   =  16 */
#define	CONST_USER_SS_SELECTOR	     35   /*  SS = 0000000000100  0  11   =  16 */

#define CONST_RING0                  0
#define CONST_RING1                  1
#define CONST_RING2                  2
#define CONST_RING3                  3

#define CONST_TSS_SELECTOR           40   /* TSS = 0000000000101  0  00   =  40 */

#define CONST_KERN_HIGHMEM           (1<<30)

#endif
