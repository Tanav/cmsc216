# Tanav Marupudi
# 118051659
# tanav

reverse_prefix_sum:
    # PROLOGUE
    subu    $sp, $sp, 12        # expand stack by 8 bytes
    sw      $ra, 8($sp)         # push $ra (ret addr, 4 bytes)
    sw      $fp, 4($sp)         # push $fp (4 bytes)
    move    $fp, $sp            # set $fp to saved $ra

    # BODY
    sw      $a0, 12($fp)        # saves rval to stack

    lw      $t0, 0($a0)         # loads index to $t0
    li      $a0, -1             # initializes $a0 to -1 for comparison

    bne		$t0, $a0, rec   	# if $t0 != -1 then goto rec
    li      $a0, 0              # intializes $a0 to 0
    j		ret				    # ends recursion
    
rec:
    lw      $a0, 12($fp)        # restores arg arr

    addi	$a0, $a0, 4			# increments arr
    jal     reverse_prefix_sum  # recursive call

    move    $t0, $a0            # saves arr to $t0
    lw      $a0, 12($fp)
    lw      $a0, 0($a0)         # loads index

    addu	$a0, $a0, $t0       # adds indices
    sw      $a0, 0($fp)         # saves new arr to stack
    lw      $a0, 12($fp)        # restores old arr
    lw      $t0, 0($fp)

    sw      $t0, 0($a0)         # saves new value to arr
    lw      $a0, 0($fp)         # restores arr

ret:
    # EPILOGUE
    move    $sp, $fp            # restore $sp
    lw      $ra, 8($sp)         # restore saved $ra
    lw      $fp, 4($sp)         # restore saved $fp
    j       $ra                 # return to kernel