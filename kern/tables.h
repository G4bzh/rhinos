#ifndef PROT_H
#define PROT_H

/*************
 * Includes 
 *************/

#include <types.h>
#include "seg.h"

/**************
 * Constantes
 **************/


/* Indexes de la GDT */

#define NULL_INDEX     0
#define CS_INDEX       1
#define XS_INDEX       2            /* DS,ES,FS,SS */
#define TSS_INDEX      3
#define LDT_INDEX      4            /* Index de la premiere LDT */
#define MAX_INDEX      LDT_INDEX    /* Nombre maximum d index d une GDT */

/* Taille de la GDT & IDT */

#define GDT_SIZE       MAX_INDEX+1  /* Debute a 0 */
#define IDT_SIZE       255

/* Selecteurs de segment */

#define CS_SELECTOR	8    /*  CS = 0000000000001  0  00   =  8  */
#define	DS_SELECTOR     16   /*  DS = 0000000000010  0  00   =  16 */
#define	ES_SELECTOR	16   /*  ES = 0000000000010  0  00   =  16 */
#define	SS_SELECTOR	16   /*  SS = 0000000000010  0  00   =  16 */
#define TSS_SELECTOR    24   /* TSS = 0000000000011  0  00   =  24 */

#define SHIFT_SELECTOR  3    /* INDEX << SHIFT_SELECTOR = SELECTOR */

/* Rings */

#define RING0   0
#define RING1   1
#define RING2   2
#define RING3   3

/* Limite des segments  */

#define KERN_BASE         0x0        /* Adresse de base du noyau */
#define KERN_LIMIT_4G     0x0        /* Limite de l'espace Noyau (4G) */
#define KERN_TOP_STACK    0x7C00     /* ESP Noyau au boot */

/* IRQs */

#define IRQ_VECTORS     16


/* Descripteur de Table (GDT & LDT) */

PUBLIC struct table_desc
{
  u16_t limit;
  u32_t base;
} __attribute__ ((packed));

/* TSS */

PUBLIC struct tss
{
  u16_t previous;
  u16_t zero_0;
  u32_t esp0;
  u16_t ss0;
  u16_t zero_1;
  u32_t esp1;
  u16_t ss1;
  u16_t zero_2;
  u32_t esp2;
  u16_t ss2;
  u16_t zero_3;
  u32_t cr3;
  u32_t eip;
  u32_t eflags;
  u32_t eax;
  u32_t ecx;
  u32_t edx;
  u32_t ebx;
  u32_t esp;
  u32_t ebp;
  u32_t esi;
  u32_t edi;
  u16_t es;
  u16_t zero_4;
  u16_t cs;
  u16_t zero_5;
  u16_t ss;
  u16_t zero_6;
  u16_t ds;
  u16_t zero_7;
  u16_t fs;
  u16_t zero_8;
  u16_t gs;
  u16_t zero_9;
  u16_t ldt;
  u16_t zero_10;
  u16_t debug;
  u16_t iomap;
} __attribute__ ((packed));

/* Liste chainee des handlers d'IRQs */

PUBLIC struct irq_chaine
{
  u8_t irq;                  /* IRQ */
  struct irq_chaine* next;   /* Chaine */
  u8_t (*handler)(void);     /* ISR */
  u32_t id;                  /* ID  */
} __attribute__ ((packed));

/* Alias pour le pointeur d'ISR */

typedef u8_t (*irq_handler_t)();

/******************
 * ISR assembleur
 ******************/

EXTERN void hwint_00(void);
EXTERN void hwint_01(void);
EXTERN void hwint_02(void);
EXTERN void hwint_03(void);
EXTERN void hwint_04(void);
EXTERN void hwint_05(void);
EXTERN void hwint_06(void);
EXTERN void hwint_07(void);
EXTERN void hwint_08(void);
EXTERN void hwint_09(void);
EXTERN void hwint_10(void);
EXTERN void hwint_11(void);
EXTERN void hwint_12(void);
EXTERN void hwint_13(void);
EXTERN void hwint_14(void);
EXTERN void hwint_15(void);
EXTERN void excep_00(void);
EXTERN void excep_01(void);
EXTERN void excep_02(void);
EXTERN void excep_03(void);
EXTERN void excep_04(void);
EXTERN void excep_05(void);
EXTERN void excep_06(void);
EXTERN void excep_07(void);
EXTERN void excep_08(void);
EXTERN void excep_09(void);
EXTERN void excep_10(void);
EXTERN void excep_11(void);
EXTERN void excep_12(void);
EXTERN void excep_13(void);
EXTERN void excep_14(void);
EXTERN void excep_16(void);
EXTERN void excep_17(void);
EXTERN void excep_18(void);


/**************
 * Prototypes 
 **************/

PUBLIC struct seg_desc gdt[GDT_SIZE];  /* GDT */
PUBLIC struct gate_desc idt[IDT_SIZE]; /* IDT */
PUBLIC struct table_desc gdt_desc;     /* Descripteur de la GDT */
PUBLIC struct table_desc idt_desc;     /* Descripteur de l'IDT */
PUBLIC struct irq_chaine* irq_handlers[IRQ_VECTORS];  /* Tableau des irq handlers */
PUBLIC u32_t  irq_active[IRQ_VECTORS];                /* Tableau des bitmaps d'irq actives */


PUBLIC void gdt_init();
PUBLIC void idt_init();

#endif
