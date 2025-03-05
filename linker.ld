ENTRY(_start)

SECTIONS {
    . = 0x100000; /* Загрузка ядра по адресу 1MB */

    .text ALIGN(4K) : {
        *(.multiboot)  /* Multiboot-заголовок */
        *(.text)       /* Код ядра */
    }

    .rodata ALIGN(4K) : {
        *(.rodata)     /* Константы */
    }

    .data ALIGN(4K) : {
        *(.data)       /* Данные */
    }

    .bss ALIGN(4K) : {
        *(COMMON)
        *(.bss)
    }

    /DISCARD/ : {
        *(.comment)
        *(.eh_frame)
        *(.note.gnu.build-id)
    }
}
