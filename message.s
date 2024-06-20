.global printMessageInBinary
printMessageInBinary:
    push %rbp
    mov %rsp, %rbp
    mov $0b01010111, %al
    call print_char
    mov $0b01001111, %al
    call print_char
    mov $0b01010111, %al
    call print_char
    mov $0b00100000, %al
    call print_char
    mov $0b01111001, %al
    call print_char
    mov $0b01101111, %al
    call print_char
    mov $0b01110101, %al
    call print_char
    mov $0b00100000, %al
    call print_char
    mov $0b01100111, %al
    call print_char
    mov $0b01101111, %al
    call print_char
    mov $0b01110100, %al
    call print_char
    mov $0b00100000, %al
    call print_char
    mov $0b01101001, %al
    call print_char
    mov $0b01110100, %al
    call print_char
    mov $0b00101100, %al
    call print_char
    mov $0b00100000, %al
    call print_char
    mov $0b01001001, %al
    call print_char
    mov $0b00100000, %al
    call print_char
    mov $0b01110011, %al
    call print_char
    mov $0b01110000, %al
    call print_char
    mov $0b01100101, %al
    call print_char
    mov $0b01101110, %al
    call print_char
    mov $0b01110100, %al
    call print_char
    mov $0b00100000, %al
    call print_char
    mov $0b01110100, %al
    call print_char
    mov $0b01101111, %al
    call print_char
    mov $0b00100000, %al
    call print_char
    mov $0b01101101, %al
    call print_char
    mov $0b01110101, %al
    call print_char
    mov $0b01100011, %al
    call print_char
    mov $0b01101000, %al
    call print_char
    mov $0b00100000, %al
    call print_char
    mov $0b01110100, %al
    call print_char
    mov $0b01101001, %al
    call print_char
    mov $0b01101101, %al
    call print_char
    mov $0b01100101, %al
    call print_char
    mov $0b00100000, %al
    call print_char
    mov $0b01100111, %al
    call print_char
    mov $0b01100101, %al
    call print_char
    mov $0b01110100, %al
    call print_char
    mov $0b01110100, %al
    call print_char
    mov $0b01101001, %al
    call print_char
    mov $0b01101110, %al
    call print_char
    mov $0b01100111, %al
    call print_char
    mov $0b00100000, %al
    call print_char
    mov $0b01110100, %al
    call print_char
    mov $0b01101000, %al
    call print_char
    mov $0b01101001, %al
    call print_char
    mov $0b01110011, %al
    call print_char
    mov $0b00100000, %al
    call print_char
    mov $0b01110100, %al
    call print_char
    mov $0b01101111, %al
    call print_char
    mov $0b00100000, %al
    call print_char
    mov $0b01110111, %al
    call print_char
    mov $0b01101111, %al
    call print_char
    mov $0b01110010, %al
    call print_char
    mov $0b01101011, %al
    call print_char
    leave
    ret
print_char:
    mov $8, %cl
print_char_loop:
    rol %al
    setc %dl
    add $48, %dl
    mov %dl, %rsi
    mov $1, %rdi
    mov $1, %rax
    syscall
    dec %cl
    jnz print_char_loop
    ret