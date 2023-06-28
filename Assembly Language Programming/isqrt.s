# Tanav Marupudi
# 118051659
# tanav

isqrt:
    # PROLOGUE
    subu    $sp, $sp, 8         # expand stack by 8 bytes
    sw      $ra, 8($sp)         # push $ra (ret addr, 4 bytes)
    sw      $fp, 4($sp)         # push $fp (4 bytes)
    addu    $fp, $sp, 8         # set $fp to saved $ra

    # BODY
    bge     $a0, 2, rec         # if $a0 >= 2 goto low
    move    $v0, $a0            # returns either 0 or 1
    j       ret                 # ends recursion

rec:
    subu    $sp, $sp, 4         # grow stack to save n
    sw      $a0, 4($sp)         # saves n to stack

    srl		$a0, $a0, 2         # preps arg to be passed to recursive call
    jal     isqrt               # recursive call

    lw      $t3, 4($sp)         # restore arg to $t3

    sll		$t0, $v0, 1         # saves modified rval to $t0 (small)
    addi	$t1, $t0, 1         # initializes $t1 (large)

    mul     $t2, $t1, $t1       # $t2 holds large * large
    bgt     $t2, $t3, small     # if large * large > n goto small
    move    $v0, $t1            # set rval to $t1 (large)
    j       end                 # skips if actions
small:
    move    $v0, $t0            # set rval to $t0 (small)
end:   

ret:
    # EPILOGUE
    move    $sp, $fp            # restore $sp
    lw      $ra, ($fp)          # restore saved $ra
    lw      $fp, -4($sp)        # restore saved $fp
    j       $ra                 # return to kernel