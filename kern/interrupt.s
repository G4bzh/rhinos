
	[BITS 32]

	;;========================================================================
	;; Gestion bas niveau des interruptions/exceptions
	;;========================================================================

	
global	hwint_00		; ISR visibles pour le C
global	hwint_01
global	hwint_02
global	hwint_03
global	hwint_04
global	hwint_05
global	hwint_06
global	hwint_07
global	hwint_08
global	hwint_09
global	hwint_10
global	hwint_11
global	hwint_12
global	hwint_13
global	hwint_14
global	hwint_15

global	excep_00
global	excep_01
global	excep_02
global	excep_03
global	excep_04
global	excep_05
global	excep_06
global	excep_07
global	excep_08
global	excep_09
global	excep_10
global	excep_11
global	excep_12
global	excep_13
global	excep_14
global	excep_16
global	excep_17
global	excep_18

extern	irq_handle_flih			; Handlers pour les IRQ en C
extern	excep_handle			; Handlers pour les exceptions en C
extern	cur_ctx				; Contexte courant
extern	context_cpu_postsave		; Pretraitement de la sauvegarde de context en C
	

	;;========================================================================
	;; Constantes
	;;========================================================================

	
	;; 
	;; Segment Selector
	;;

%assign		CS_SELECTOR		8  ; CS  = 00000001  0  00   = (byte) 8
%assign		DS_SELECTOR		16 ; DS  = 00000010  0  00   = (byte) 16
%assign		ES_SELECTOR		16 ; ES  = 00000010  0  00   = (byte) 16
%assign		SS_SELECTOR		16 ; SS  = 00000010  0  00   = (byte) 16
	
	;;
	;; IRQ Magic Numbers
	;;

%assign		IRQ_EOI			0x20
%assign		IRQ_MASTER		0x20
%assign		IRQ_SLAVE		0xA0

	;;
	;; Fake Error Code
	;;
%assign		FAKE_ERROR		0xFEC	

	;;
	;; Offset dans struct context_cpu
	;;

%assign		CONTEXT_RET_OFFSET		40
%assign		CONTEXT_CS_OFFSET		12
%assign		CONTEXT_ESP_OFFSET		20
	
	;;
	;; Taille de la pile d interruption
	;; 

%assign		INT_STACK_SIZE		1024	
	
	;;
	;; Vecteurs Exceptions
	;;

%assign		DIVIDE_VECTOR		0
%assign		DEBUG_VECTOR		1
%assign		NMI_VECTOR		2
%assign		BREAKPT_VECTOR		3
%assign		OVERFLOW_VECTOR		4
%assign		BOUND_VECTOR		5
%assign		OPCODE_VECTOR		6
%assign		NOMATH_VECTOR		7
%assign		DFAULT_VECTOR		8
%assign		COSEG_VECTOR		9
%assign		TSS_VECTOR		10
%assign		NOSEG_VECTOR		11
%assign		SSFAULT_VECTOR		12
%assign		GPROT_VECTOR		13
%assign		PFAULT_VECTOR		14
%assign		MFAULT_VECTOR		16
%assign		ALIGN_VECTOR		17
%assign		MACHINE_VECTOR		18


	;;========================================================================
	;; Macros
	;;========================================================================


	
	;;
	;; Traitement generique des IRQ (macro maitre)
	;;
	
%macro	hwint_generic0	1
	push	FAKE_ERROR	; Faux erreur code
   	call	save_ctx	; Sauvegarde des registres
	push	dword [cur_ctx]	; Empile le contexte courant
	push	%1		; Empile l'IRQ
 	call	irq_handle_flih	; Appel les handles C
	add	esp,8		; Depile l'IRQ
	mov	al,IRQ_EOI	; Envoi la fin d interruption
	out	IRQ_MASTER,al	; au PIC Maitre
	call	restore_ctx	; Restaure les registres
%endmacro

	;;
	;; Traitement generique des IRQ (macro esclave)
	;;

%macro	hwint_generic1	1
	push	FAKE_ERROR	; Faux erreur code	
	call	save_ctx	; Sauvegarde des registres
	push	dword [cur_ctx]	; Empile le contexte courant
	push	%1		; Empile l'IRQ
	call	irq_handle_flih	; Appel les handles C
	add	esp,8		; Depile l'IRQ
	mov	al,IRQ_EOI	; Envoi la fin d interruption
	out	IRQ_SLAVE,al	; au PIC Esclave
	out	IRQ_MASTER,al	; puis au Maitre
	call	restore_ctx	; Restaure les registres
%endmacro


	
	;;========================================================================
	;; IRQ Handlers
	;;========================================================================

	

hwint_00:
	hwint_generic0	0	

hwint_01:
	hwint_generic0	1
	
hwint_02:
	hwint_generic0	2

hwint_03:
	hwint_generic0	3
	
hwint_04:
	hwint_generic0	4

hwint_05:
	hwint_generic0	5

hwint_06:
	hwint_generic0	6

hwint_07:
	hwint_generic0	7

hwint_08:
	hwint_generic1	8

hwint_09:
	hwint_generic1	9

hwint_10:
	hwint_generic1	10

hwint_11:
	hwint_generic1	11

hwint_12:
	hwint_generic1	12

hwint_13:
	hwint_generic1	13

hwint_14:
	hwint_generic1	14

hwint_15:
	hwint_generic1	15
	
	
	
	;;========================================================================
	;; Sauvegarde du contexte pour les IRQ et les exceptions
	;;========================================================================

save_ctx:
	cld		        ; Positionne le sens d empilement
	
	mov dword [save_esp],esp	; Sauvegarde ESP
	mov esp, [cur_ctx] 		; Placement de ESP sur le contexte courant
	add esp,CONTEXT_RET_OFFSET	; Placement a ret_addr

	pushad			; Sauve les registres generaux 32bits
	o16 push	ds	; Sauve les registres de segments (empile en 16bits)
	o16 push	es
	o16 push	fs
	o16 push	gs

	mov	esp, int_stack_top 	; Positionne la pile d interruption
	mov	ax,DS_SELECTOR		; Ajuste les segments noyau (CS & SS sont deja positionnes)
	mov	ds,ax
	mov	ax,ES_SELECTOR
	mov	es,ax			; note: FS & GS ne sont pas utilises par le noyau

	push	dword [save_esp]	; Empile le pointeur de pile
	push	ss			; Empile le stack segment
	call	context_cpu_postsave 	; Passe par le C pour finaliser le contexte
	add	esp,8			; Depile les arguments

	mov	eax,dword [save_esp] 	; La pile sauvee pointe sur l adresse de retour
	jmp	[eax]		     	; Saute a l adresse de retour

	
	;;======================================================================== 
	;; Restauration du contexte pour les IRQ et les exceptions
	;;======================================================================== 

	
restore_ctx:	
	mov 	esp, [cur_ctx]
	o16 pop gs		; Restaure les registres
	o16 pop fs		; sauves par save_ctx
	o16 pop	es		; en 16bits
	o16 pop	ds
	popad		    	; Restaure les registre generaux
	 
	cmp 	dword [esp+CONTEXT_CS_OFFSET], CS_SELECTOR 	; Teste si le contexte est un contexte noyau
	jne 	restore_ctx_next	    			; Saute a la suite si ce n'est pas le cas
	mov 	esp, dword [esp+CONTEXT_ESP_OFFSET]	    	; Retourne sur la pile interrompue (qui contient les bonnes infos pour iret) sinon

restore_ctx_next:	
	add 	esp,4		; Depile l adresse de retour de save_ctx
	add 	esp,4		; Depile le code d erreur
	iretd


	
	;;========================================================================
	;; Exceptions Handlers
	;;======================================================================== 

excep_00:
	push	FAKE_ERROR
	push	DIVIDE_VECTOR
	jmp	excep_next

excep_01:
	push	FAKE_ERROR
	push	DEBUG_VECTOR
	jmp	excep_next

excep_02:
	push	FAKE_ERROR
	push	NMI_VECTOR
	jmp	excep_next

excep_03:
	push	FAKE_ERROR
	push	BREAKPT_VECTOR
	jmp	excep_next

excep_04:
	push	FAKE_ERROR
	push	OVERFLOW_VECTOR	
	jmp	excep_next

excep_05:
	push	FAKE_ERROR
	push	BOUND_VECTOR
	jmp	excep_next

excep_06:
	push	FAKE_ERROR
	push	OPCODE_VECTOR	
	jmp	excep_next

excep_07:
	push	FAKE_ERROR
	push	NOMATH_VECTOR
	jmp	excep_next
	
excep_08:
	push	DFAULT_VECTOR
	jmp	excep_next

excep_09:
	push	FAKE_ERROR
	push	COSEG_VECTOR	
	jmp	excep_next	

excep_10:
	push	TSS_VECTOR	
	jmp	excep_next

excep_11:
	push	NOSEG_VECTOR
	jmp	excep_next	

excep_12:
	push	SSFAULT_VECTOR	
	jmp	excep_next

excep_13:
	push	GPROT_VECTOR	
	jmp	excep_next

excep_14:
	push	PFAULT_VECTOR	
	jmp	excep_next
	
excep_16:
	push	FAKE_ERROR
	push	MFAULT_VECTOR
	jmp	excep_next	

excep_17:
	push	ALIGN_VECTOR
	jmp	excep_next

excep_18:
	push	FAKE_ERROR
	push	MACHINE_VECTOR	
	jmp	excep_next	
	
	
	;;========================================================================
	;; Gestion des exceptions
	;;======================================================================== 

	
excep_next:
	pop	dword [excep_num] 	; Recupere le vecteur
	call	save_ctx		; Sauve le contexte
	push	dword [cur_ctx]		; Empile le contexte courant	
	push	dword [excep_num]	; Argument 1 de excep_handle
	call	excep_handle		; Gestion de l exception en C
	add	esp,8			; Depile les arguments
	call	restore_ctx		; Restaure les registres



	
	;;========================================================================
	;; Declaration des Donnees
	;;========================================================================

	;;
	;; Divers
	;; 
	
	excep_num	dd	0 ; Vecteur de l exception
	save_esp	dd	0 ; Sauvegarde de ESP
	
	;;
	;; La pile d interruption
	;;

int_stack:
	times	INT_STACK_SIZE	db 0
int_stack_top:
	