# strlen(str)

strlen:
   # PROLOGUE
      subu  $sp, $sp, 8
      sw    $ra, 8($sp)
      sw    $fp, 4($sp)
      addu  $fp, $sp, 8

   # BODY
      li    $v0, 0
      li    $t1, 1

loop:
      lb $t0, 0($a0)
      beqz $t0, endloop
      add $a0, $a0, $t1
      add $v0, $v0, $t1
      j loop
endloop:

   # EPILOGUE
      move  $sp, $fp
      lw    $ra, ($fp)
      lw    $fp, -4($sp)
      j     $ra