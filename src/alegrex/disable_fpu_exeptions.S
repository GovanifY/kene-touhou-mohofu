
# /*
# ---------------------------------------------------------
#   東方模倣風 ～ Toho Imitation Style.
#   http://mohou.huuryuu.com/
# ---------------------------------------------------------
# これは PSPSDK の サンプル
# /pspsdk/psp/sdk/samples/gu/spharm/disablefpu.S
# に入ってた物を修正したものです。
# ---------------------------------------------------------
# 制限
# ---------------------------------------------------------
# main();直下に書かないと巧くいかないみたい。
# -------------------------------------------------------
# */

# /*
# FPU例外の無効化
# -------------------------------------------------------
#   これはFPU例外を無効にします。(FPU例外を無効にして高速化します)
#   This disables FPU exceptions.
#
#   この関数は($31==$raを弄るので)main()直下に書かないとダメかもしれない。
# */

#include <as_reg_compat.h>

.text
.align  4

.set    push 
.set    noreorder 
.set    nomacro
# /* extern void disable_FPU_exeptions_in_main(void); */
.globl  disable_FPU_exeptions_in_main
.ent    disable_FPU_exeptions_in_main
disable_FPU_exeptions_in_main: 
    cfc1    $2, $ra 
    lui     $8, 0x80        # R8 &= 0x80;

    and     $8, $2, $8      # Mask off all bits except for 23 of FCR 
    ctc1    $8, $ra 

    jr      $ra             # return ();
    nop

.end    disable_FPU_exeptions_in_main
.set    pop
.set    macro
