[BITS 32]

global bochs_print
global outb
global inb
global phys_copy
global random
global idle_task
	
	;; 
	;; Affichage dans bochs (%d et %x supporte)
	;;

ASCII_OFFSET0	equ	48
ASCII_OFFSETA	equ	55	
	
bochs_print:
	push 	ebp         	; Sauvegarde de EBP
	mov  	ebp,esp 	; Mise en place de la base
	push	esi		; Sauvegarde ESI (Requis par GCC)
	push	edi		; Sauvegarde EDI (Requis par GCC)
	push	edx		; Sauvegarde EDX
	mov	edx,ebp		; Copie EBP
	add	edx,8		; Place EDX sur le premier argument
	mov  	esi,[edx]	; Recupere l'argument dans ESI
bochs_loop:	
	lodsb			; Identique a print_message
	cmp	al,0		; Fin de chaine ?
	je	bochs_end	; On retourne si oui
	cmp	al,'%'		; Caractere '%' ?
	je	bochs_percent	; On s'en occupe si oui
	push	edx		; Sauvegarde EDX
	mov	dx,0xe9		; Port de Bochs
	out	dx,al		; Emet le caractere courant
	pop	edx		; Restaure EDX
	jmp	bochs_loop	; Boucle
bochs_percent:
	lodsb			; Avale le caractere suivant
bochs_base10:
	cmp	al,'d'		; Regarde si c est un 'd'
	jne	bochs_base16	; Si non, on saute
	mov	[BASE],dword 10 ; Base decimale
	jmp	bochs_next	; Saute a la decomposition
bochs_base16:
	cmp	al,'x'		; Regarde si c est un 'd'
	jne	bochs_loop	; Si non, on saute
	mov	[BASE],dword 16 ; Base hexadecimale
bochs_next:	
	push	eax	 	; Sauvegarde EAX
	push	ebx		; Sauvegarde EBX
	push	ecx		; Sauvegarde ECX
	add	edx,4		; EDX pointe sur l argument suivant
	mov	eax,[edx]	; Met la valeur de l argument dans EAX
	mov	ecx,0		; Initialise le compteur de division
	mov	ebx,dword [BASE]; Initialise le diviseur
	push	edx		; Sauvegarde EDX
bochs_decomp:
	mov	edx,0		; Zero EDX (car on divise EDX-EAX)
	div	ebx		; Divise par la base
	add	ecx,1		; Increment le compteur de division
	push	edx		; Empile le Reste
	cmp	eax,0		; Fin de la division ?
	jnz	bochs_decomp	; Si non, on continue la decomposition
bochs_depop:
	pop	eax		; Depile dans AL
	cmp	eax,9		; Compare la valeur a 9
	jg	bochs_ascii16	; Change l'offset pour les nombres hexa
	add	eax,ASCII_OFFSET0 ; Ajoute l offset pour avoir le code ASCII (0->9)
	jmp	bochs_ascii_next  ; Saute a l affichage
bochs_ascii16:
	add	eax,ASCII_OFFSETA ; Ajoute l offset pour avoir le code ASCII (A->F)
bochs_ascii_next:	
	mov	dx,0xe9		; Port de Bochs
	out	dx,al		; Emet le caractere courant
	sub	ecx,1		; Decremente le compteur
	jnz	bochs_depop	; Boucle tant que le compteur est non nul
	pop	edx		; Restaure EDX
	pop	ecx		; Restaure ECX
	pop	ebx		; Restaure EBX
	pop	eax		; Restaure EAX
	jmp	bochs_loop	; Boucle sur le reste de la chaine	
bochs_end:
	pop	edx		; Restaure EDX
	pop	edi		; Restaure EDI
	pop	esi		; Restaure ESI
	mov	esp,ebp		; Restaure la pile
	pop	ebp		; Restaure EBP
	ret	
	
	;;
	;; outb(u16_t,u8_t)
	;;

outb:
	push 	ebp         	; Sauvegarde de EBP
	mov  	ebp,esp 	; Mise en place de la base
	push	esi		; Sauvegarde ESI (Requis par GCC)
	push	edi		; Sauvegarde EDI (Requis par GCC)
	mov  	dx,[ebp+8]	; Recupere le port dans dx
	mov     al,[ebp+12]     ; Recupere la valeur dans al
	out     dx,al		; instruction out
	pop	edi		; Restaure EDI
	pop	esi		; Restaure ESI
	mov	esp,ebp		; Restaure la pile
	pop	ebp		; Restaure EBP
	ret

	;;
	;; inb(u16_t,u8_t*)
	;;

inb:
	push 	ebp         	; Sauvegarde de EBP
	mov  	ebp,esp 	; Mise en place de la base
	push	esi		; Sauvegarde ESI (Requis par GCC)
	push	edi		; Sauvegarde EDI (Requis par GCC)
	mov  	dx,[ebp+8]	; Recupere le port dans dx
	mov	ebx,[ebp+12]	; Recupere le buffer dans ebx
	in      al,dx		; Instruction in
	mov	byte [ebx],al	; Affecte la valeur
	pop	edi		; Restaure EDI
	pop	esi		; Restaure ESI
	mov	esp,ebp		; Restaure la pile
	pop	ebp		; Restaure EBP
	ret

	;;
	;; phys_copy(u32_t src, u32_t dest, u32_t len)
	;;
	
phys_copy:
	push 	ebp         	; Sauvegarde de EBP
	mov  	ebp,esp 	; Mise en place de la base
	push	esi		; Sauvegarde ESI (Requis par GCC)
	push	edi		; Sauvegarde EDI (Requis par GCC)
	push	ecx		; Sauvegarde ECX
	cld			; Fixe le sens du decompte
	mov  	esi,[ebp+8]	; Recupere l addresse source
	mov	edi,[ebp+12]	; Recupere l addresse de destination
	mov	ecx,[ebp+16] 	; Recupere la taille
	shr	ecx,0x2		; Divise la taille par 4
	rep movsd		; Copie !
	pop	ecx		; Restaure ECX
	pop	edi		; Restaure EDI
	pop	esi		; Restaure ESI
	mov	esp,ebp		; Restaure la pile
	pop	ebp		; Restaure EBP
	ret

	;;
	;; u32_t random(void)
	;;

mod     equ     0x7FFFFFFF	; 2^31 - 1 (modulo de park-miller)
cons    equ     16807		; constante de park-miller

random:
	push 	ebp         	; Sauvegarde de EBP
	mov  	ebp,esp 	; Mise en place de la base
	push	esi		; Sauvegarde ESI (Requis par GCC)
	push	edi		; Sauvegarde EDI (Requis par GCC)
        mov     eax,cons        ; EAX = 16807
	mov	ebx,dword [seed] ; EBX =1
	mul     ebx             ; EDX:EAX = EAX*EBX
	add     eax,edx         ; EAX = EDX+EAX (algorithme de Carta)
	jno     random_end 	; Saut a la fin si on ne depasse pas la limit 2^32
	and     eax,mod       	; Sinon, bit 31 mis a 0
	inc     eax             ; Incremente EAX
random_end:	
	mov     dword [seed],eax ; Met a jour seed pour l alea suivant
	pop	edi		; Restaure EDI
	pop	esi		; Restaure ESI
	mov	esp,ebp		; Restaure la pile
	pop	ebp		; Restaure EBP	
	ret			; Retourne (le resultat est dans EAX)

	

	;;
	;; void idle_task()
	;;

idle_task:
	;; hlt	Commente pour ne pas polluer la sortie de bochs
	jmp	idle_task


BASE	dd	0		; Base de decomposition
seed	dd	1		; Debut de l alea