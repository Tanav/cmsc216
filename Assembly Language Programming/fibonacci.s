# Tanav Marupudi
# 118051659
# tanav

fibonacci:
    # PROLOGUE
    subu    $sp, $sp, 8         # expand stack by 8 bytes
    sw      $ra, 8($sp)         # push $ra (ret addr, 4 bytes)
    sw      $fp, 4($sp)         # push $fp (4 bytes)
    addu    $fp, $sp, 8         # set $fp to saved $ra

    # BODY
    bgt		$a0, 1, rec         # if $a0 > 1 goto rec
    move    $v0, $a0            # store $a0 in $v0
    j		ret				    # ends recursion
    
rec:
    subu    $sp, $sp, 8         # expands stack
    sw      $a0, 4($sp)         # saves arg

    sub     $a0, $a0, 1
    jal     fibonacci           # recursive call on n-1

    lw      $a0, 4($sp)         # restores arg
    sw      $v0, 8($sp)         # saves rval of n-1

    sub     $a0, $a0, 2
    jal     fibonacci           # recursive call on n-2

    lw      $t0, 8($sp)         # restores rval of n-1
    add     $v0, $v0, $t0       # returns rval of n-2 + n-1

ret:
    # EPILOGUE
    move    $sp, $fp            # restore $sp
    lw      $ra, ($fp)          # restore saved $ra
    lw      $fp, -4($sp)        # restore saved $fp
    j       $ra                 # return to kernel