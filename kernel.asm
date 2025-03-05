section .multiboot
align 4
  dd 0x1BADB002              ; Magic number для Multiboot 1
  dd 0x00000003              ; Флаги
  dd -(0x1BADB002 + 0x00000003) ; Контрольная сумма

section .text.entry
global _start
extern kernelmain

_start:
  cli                        ; Отключить прерывания
  mov esp, stack_top         ; Установить стек
  call kernelmain            ; Вызов основной функции на Си
  hlt                        ; Остановка процессора

section .note.GNU-stack
  db 0

section .bss
align 16
stack_bottom:
  resb 16384                 ; 16 KB стек
stack_top: