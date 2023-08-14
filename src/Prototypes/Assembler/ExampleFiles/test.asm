.data
    argument:   .word   7
    abyte:      .byte   2 

.text
main:   addi x32, x32, 0
        addi x8, x0, 0
        addi x9, x0, 0          
        addi x10, x0, 0 
        jal x0, exit
        case2:
        add x10, x10, x7
        lw x11, 0(x10)

    exit: