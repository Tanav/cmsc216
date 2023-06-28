# max(x, y)

max:
   # PROLOGUE
      subu  $sp, $sp, 8
      sw    $ra, 8($sp)
      sw    $fp, 4($sp)
      addu  $fp, $sp, 8

   # BODY
      bge   $a0, $a1, else
      move  $v0, $a1
      j     endif

else: move  $v0, $a0
endif:

   # EPILOGUE
      move  $sp, $fp
      lw    $ra, ($fp)
      lw    $fp, -4($sp)
      j     $ra