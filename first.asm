; fonctions externes pour les entrees/sortie
extern printf
extern scanf
segment .data  ; memoire globale donnees initialisees
prompt1       db "Entrez un entier : ",0
prompt2       db "Entrez un deuxieme entier : ",0
formatSortie       db "La somme des eux entiers est : %ld",10,0
longIntFormat  db "%ld",0

segment .bss  ; memoire globale donnees non itialisees
entier1  resq 1
entier2  resq 1
resultat resq 1

segment .text ; code du programme
        global  asm_main
asm_main:  ; fonction appelee par le programme C
; sauvegarde des registres sur la pile
	push rbp
	push rbx
; appel printf
	mov rdi,prompt1
        mov rax,0
	call printf
; appel scanf
        mov rdi,longIntFormat
        mov rsi,entier1
        mov rax,0
	call scanf
	mov rbx,[entier1]
; appel printf
        mov rdi,prompt2
        mov rax,0
	call printf
; appel scanf
        mov rdi,longIntFormat
        mov rsi,entier2
        mov rax,0
	call scanf
	mov rcx,[entier2]
; calcul de la somme et sauvegarde du resultat en memoire
	add rbx,rcx
	mov [resultat],rbx
; appel printf
	mov rdi,formatSortie
        mov rsi,[resultat]
        mov rax,0
	call printf
; restauration des registres
	pop rbx
	pop rbp
; envoi de 0 au programme C
        mov     rax, 0            
        ret
