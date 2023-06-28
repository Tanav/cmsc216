# Tanav Marupudi
# 118051659
# tanav

is_palindrome:
    # PROLOGUE
    subu    $sp, $sp, 8         # expand stack by 8 bytes
    sw      $ra, 8($sp)         # push $ra (ret addr, 4 bytes)
    sw      $fp, 4($sp)         # push $fp (4 bytes)
    addu    $fp, $sp, 8         # set $fp to saved $ra

    # BODY
    jal     strlen              # v0 = strlen(*j)
    move    $t0, $v0            # save length in $t0

    add     $t1, $a0, $zero     # set $t1 to beginning of string
    add     $t2, $a0, $v0
    addi    $t2, $t2, -1        # set $t2 to end of string

    li      $t5, 2              # initialize $t5 to 2 for divsion
    div     $t0, $t0, $t5       # divide length ($t0) by 2
    li      $t5, 0              # intialize $t5 to 0 for count

    li      $v0, 1              # set $v0 to true (1)

loop:
    bge	    $t5, $t0, endloop   # endloop if count ($t5) >= length/2 ($t0)

    lb      $t3, 0($t1)         # stores byte pointed to from $t1 in $t3
    lb      $t4, 0($t2)         # stores byte pointed to from $t2 in $t4
    bne     $t3, $t4, not_pali  # not_pali if $t3 != $t4 (diff chars)

    addi    $t1, $t1, 1         # $t1 points to next char
    addi    $t2, $t2, -1        # $t2 points to previous char

    addi    $t5, $t5, 1         # increment count ($t5)
    j       loop                # repeat loop
not_pali:
    li      $v0, 0              # set $v0 to false (0)
    j       endloop             # jumps to endloop
endloop:

    # EPILOGUE
    move    $sp, $fp            # restore $sp
    lw      $ra, ($fp)          # restore saved $ra
    lw      $fp, -4($sp)        # restore saved $fp
    j       $ra                 # return to kernel

strlen:
    # PROLOGUE
    subu    $sp, $sp, 8         # expand stack by 8 bytes
    sw      $ra, 8($sp)         # push $ra (ret addr, 4 bytes)
    sw      $fp, 4($sp)         # push $fp (4 bytes)
    addu    $fp, $sp, 8         # set $fp to saved $ra

    # BODY
    li      $v0, 0              # intialize $v0 to use as count
    move    $t1, $a0            # save $a0 to $t1

str_loop:
    lb      $t0, 0($t1)         # access char $a0 points to and store in $t0
    beqz    $t0, str_endloop    # endloop if $t0 equals null byte
    addi    $t1, $t1, 1         # $t1 will access next byte
    addi    $v0, $v0, 1         # increment $v0
    j       str_loop            # repeat loop
str_endloop:

    # EPILOGUE
    move    $sp, $fp            # restore $sp
    lw      $ra, ($fp)          # restore saved $ra
    lw      $fp, -4($sp)        # restore saved $fp
    j       $ra                 # return to kernel