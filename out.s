
.data

global1: .word 15
global2: .word 0
global3: .word 1
.asciiz "Coucou"
.asciiz "trucmuche.c"
.asciiz "La base virale VPS a ete mise a jour"

.text

main:
    addiu $29, $29, -36
    ori   $8, $0, 0xbf2
    sw    $8, 4($29)
    ori   $8, $0, 0x1
    sw    $8, 8($29)
    ori   $8, $0, 0x2
    sw    $8, 12($29)
    ori   $8, $0, 0x3
    sw    $8, 16($29)
    ori   $8, $0, 0x1
    sw    $8, 24($29)
    lw    $8, 28($29)
_L1:
    lw    $8, 28($29)
    lw    $9, 32($29)
    slt   $8, $8, $9
    beq   $8, $0, _L2
    lui   $4, 0x1001
    ori   $4, $4, 0xc
    ori   $2, $0, 0x4
    syscall
    lw    $8, 28($29)
    ori   $9, $0, 0x1
    addu  $8, $8, $9
    sw    $8, 28($29)
    j     _L1
_L2:
    lui   $8, 0x1001
    lw    $8, 4($8)
    beq   $8, $0, _L3
    lui   $8, 0x1001
    lw    $8, 4($8)
    xori  $8, $8, 0x1
    lui   $9, 0x1001
    sw    $8, 4($9)
_L3:
    lui   $8, 0x1001
    lw    $8, 4($8)
    xori  $8, $8, 0x1
    beq   $8, $0, _L4
    ori   $8, $0, 0x0
    sw    $8, 0($29)
    j     _L5
_L4:
    ori   $8, $0, 0x1
    sw    $8, 0($29)
_L5:
_L6:
    lui   $8, 0x1001
    lw    $8, 8($8)
    beq   $8, $0, _L7
    lui   $4, 0x1001
    ori   $4, $4, 0x13
    ori   $2, $0, 0x4
    syscall
    lw    $4, 4($29)
    ori   $2, $0, 0x1
    syscall
    lw    $4, 24($29)
    ori   $2, $0, 0x1
    syscall
    ori   $8, $0, 0x0
    lui   $9, 0x1001
    sw    $8, 8($9)
    j     _L6
_L7:
_L8:
    lui   $4, 0x1001
    ori   $4, $4, 0x1f
    ori   $2, $0, 0x4
    syscall
    lui   $8, 0x1001
    lw    $8, 0($8)
    ori   $9, $0, 0x1
    addu  $8, $8, $9
    lui   $9, 0x1001
    sw    $8, 0($9)
    lui   $8, 0x1001
    lw    $8, 0($8)
    ori   $9, $0, 0x2
    slt   $8, $8, $9
    bne   $8, $0, _L8
    lw    $8, 4($29)
    lw    $9, 0($29)
    subu  $8, $8, $9
    sw    $8, 4($29)
    lw    $8, 4($29)
    lw    $9, 0($29)
    mult  $8, $9
    mflo  $8
    sw    $8, 4($29)
    lw    $8, 0($29)
    ori   $9, $0, 0x1
    addu  $8, $8, $9
    sw    $8, 0($29)
    lw    $8, 4($29)
    lw    $9, 0($29)
    ori   $10, $0, 0x1
    addu  $9, $9, $10
    div   $8, $9
    teq $9, $0
    mflo  $8
    sw    $8, 4($29)
    lw    $8, 4($29)
    lw    $9, 0($29)
    div   $8, $9
    teq $9, $0
    mfhi  $8
    sw    $8, 4($29)
    lw    $8, 4($29)
    lw    $9, 0($29)
    srav  $8, $8, $9
    sw    $8, 4($29)
    lw    $8, 4($29)
    lw    $9, 0($29)
    sllv  $8, $8, $9
    sw    $8, 4($29)
    lw    $8, 4($29)
    lw    $9, 0($29)
    srlv  $8, $8, $9
    sw    $8, 4($29)
    lw    $8, 24($29)
    lui   $9, 0x1001
    lw    $9, 4($9)
    xor   $8, $8, $9
    sltiu $8, $8, 1
    sw    $8, 20($29)
    lw    $8, 24($29)
    lui   $9, 0x1001
    lw    $9, 8($9)
    xor   $8, $8, $9
    sltiu $8, $8, 1
    sw    $8, 20($29)
    lw    $8, 8($29)
    lw    $9, 12($29)
    addu  $8, $8, $9
    lw    $9, 16($29)
    ori   $10, $0, 0x2
    mult  $9, $10
    mflo  $9
    addu  $8, $8, $9
    lw    $9, 8($29)
    lw    $10, 12($29)
    lw    $11, 16($29)
    addu  $10, $10, $11
    mult  $9, $10
    mflo  $9
    addu  $8, $8, $9
    sw    $8, 0($29)
    lui   $8, 0x1001
    lw    $8, 8($8)
    xori  $8, $8, 0x1
    lui   $9, 0x1001
    sw    $8, 4($9)
    addiu $29, $29, 36
    ori   $2, $0, 0xa
    syscall
