
.data

i: .word -1

.text

main:
    addiu $29, $29, 0
    lui   $4, 0x1001
    lw    $4, 0($4)
    ori   $2, $0, 0x1
    syscall
    addiu $29, $29, 0
    ori   $2, $0, 0xa
    syscall
