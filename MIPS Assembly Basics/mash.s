# mash(x, y)

mash:
   # PROLOGUE
      subu  $sp, $sp, 8
      sw    $ra, 8($sp)
      sw    $fp, 4($sp)
      addu  $fp, $sp, 8

   # BODY
      li    $t2, 10
      mul	$v0, $t0, $t2
      add   $v0, $v0, $t1

   # EPILOGUE
      move  $sp, $fp
      lw    $ra, ($fp)
      lw    $fp, -4($sp)
      j     $ra