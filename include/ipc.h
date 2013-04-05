/**

   ipc.h
   =====

   Prototypes and constants for ipc primitives

**/


#ifndef IPC_H
#define IPC_H


/**
  
   Includes
   --------

   - types.h

**/

#include <types.h>


/**

    Constante: IPC_ANY
    ------------------

    Wildcard acceptance for Receive

**/

#define IPC_ANY    0


/**

   Constantes: IPC Return Values
   -----------------------------

   - IPC_SUCCESS  : All is OK
   - IPC_FAILURE  : Something got wrong
   - IPC_DEADLOCK : A is sending to B which is sending to A

**/

#define IPC_SUCCESS   0
#define IPC_FAILURE   1
#define IPC_DEADLOCK  2


/**

   Constante: IPC_DATA_LEN
   -----------------------

   Max data in an IPC message

**/

#define IPC_DATA_LEN  251



/**
   
   Structure: struct ipc_message
   -----------------------------

   Define an IPC message. Members are
   
   - from : originated thread
   - len  : size of message content
   - data : message content

**/
   

PUBLIC struct ipc_message
{
  s32_t from;
  u8_t len;
  u8_t  data[IPC_DATA_LEN];
} __attribute ((packed))__ ;



/**

  Prototypes
  ----------
  
  Declare the 4 ipc primitives.
  EXTERN scope due to assembly defintion (lib/ipc/ipc.s)

**/

EXTERN u8_t ipc_send(int to, struct ipc_message* msg);
EXTERN u8_t ipc_receive(int from, struct ipc_message* msg);
EXTERN u8_t ipc_notify(int to);
EXTERN u8_t ipc_sendrec(int to, struct ipc_message* msg);


#endif
