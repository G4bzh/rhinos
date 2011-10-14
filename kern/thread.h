/*
 * thread.h
 * Header de thread.c
 *
 */


#ifndef THREAD_H
#define THREAD_H


/*========================================================================
 * Includes
 *========================================================================*/

#include <types.h>
#include "const.h"
#include "context_cpu.h"


/*========================================================================
 * Constantes
 *========================================================================*/


#define THREAD_NAMELEN        32
#define THREAD_STACK_SIZE     4096


/*========================================================================
 * Structures
 *========================================================================*/


/* Etats */

PUBLIC enum thread_state
{
  THREAD_READY,
  THREAD_RUNNING,
  THREAD_BLOCKED,
  THREAD_DEAD
};


/* Structure thread */

PUBLIC struct thread
{
  char name[THREAD_NAMELEN];
  struct context_cpu* ctx;
  virtaddr_t stack_base;
  u32_t stack_size;
  enum thread_state state;
  struct thread* prev;
  struct thread* next;
};


/*========================================================================
 * Prototypes
 *========================================================================*/


PUBLIC void thread_init(void);
PUBLIC struct thread* thread_create(const char* nom, virtaddr_t start_entry, void* start_arg, virtaddr_t exit_entry, void* exit_arg, u32_t stack_size);
PUBLIC u8_t thread_destroy(struct thread* th);

#endif


