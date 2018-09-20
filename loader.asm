[bits 16]
org 0x7c00

start:
	mov al, 6        ;число секторов
	mov cl, 2        ;номер сектора
	mov bx, 0x8000   ; адрес
	call loadToMem   ;загрузка основного кода

	mov al, 1        ;число секторов
	mov cl, 8        ;номер сектора
	mov bx, 0xD000   ; адрес
	call loadToMem   ;загрузка шрифтов

	call gotoVesa
	cli
	mov sp, 0xfffc
	xor ax, ax
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	in al, 0x70
	or al, 0x80  ;немаск прерывание
	out 0x70, al

	in  al, 0x92
	or  al, 2     ;a20
	out 0x92, al

	lgdt[descriptor_pointer]
	mov eax, cr0
	or eax, 1
	mov cr0, eax
	mov esi, [LFBADDR]
	jmp CODE_RULES:0x8000
ret


loadToMem:
	mov ah, 2 ;функцияя
	mov ch, 0 ;номер дорожки
	mov dh, 0 ;номер головки
	mov dl, 0x80	;указание жесткий диск
	int 0x13
ret

gotoVesa:
	mov ax, 0x4f01
	mov cx, 0x112
	mov di, buffer
	int 0x10
	mov ax, 0x4f02
	mov bx, 0x4112
	int 0x10
	mov edx, [buffer+0x28]
	mov dword[LFBADDR],edx
ret

Table_start:
null_descriptor:
	dd 0
	dd 0
code_descriptor:
	dw 0xffff
	dw 0
	db 0
	db 0b10011010
	db 0b11001111
	db 0
data_descriptor:
	dw 0xffff
	dw 0
	db 0
	db 0b10010010
	db 0b11001111
	db 0
Table_end:

descriptor_pointer:
	dw Table_end - Table_start
	dd Table_start

CODE_RULES equ code_descriptor - Table_start

DATA_RULES equ data_descriptor - Table_start

buffer dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0

LFBADDR dd 0