
# /*
# ---------------------------------------------------------
#   東方模倣風 ～ Toho Imitation Style.
#   http://mohou.huuryuu.com/
# ---------------------------------------------------------
# notaz 氏 http://notaz.gp2x.de/ の psp picodrive version1.35 source.
# http://notaz.gp2x.de/releases/PicoDrive/PicoDrive_135src.tar.bz2
# に入ってた物を修正したものです。
# -------------------------------------------------------
# */

# /* vim:filetype=mips */

# /* Some misc routines for Allegrex MIPS */
# /* (c) Copyright 2007, Grazvydas "notaz" Ignotas */
# /* All Rights Reserved */

.set noreorder
.set noat

.text
.align 4

# /* extern void memset32_uncached(s32 *dest, s32 c, int count); */
# /* extern void memset32_uncached(int *dest, int c, int count); */
.globl  memset32_uncached
.ent    memset32_uncached
memset32_uncached:
# /*  srl なので ペアリング出来ない。 */
    srl     $t0, $a2, 3    # /* we will do 32 bytes per iteration */

    beqz    $t0, ms32u_bloop_end
    nop                    # ここに nopを書いた方が良い。

ms32u_bloop:
    addiu   $t0, -1
    nop                    # ここに nopを書いた方が良い。

    sw      $a1, 0x00($a0)
    sw      $a1, 0x04($a0)

    sw      $a1, 0x08($a0)
    sw      $a1, 0x0c($a0)

    sw      $a1, 0x10($a0)
    sw      $a1, 0x14($a0)

    sw      $a1, 0x18($a0)
    sw      $a1, 0x1c($a0)

    bnez    $t0, ms32u_bloop
    addiu   $a0, 0x20

ms32u_bloop_end:
    andi    $a2, $a2, 0x0f
    beqz    $a2, ms32u_return

ms32u_cloop:
    addiu   $a2, -1
    sw      $a1, 0($a0)

    bnez    $a2, ms32u_cloop
    addiu   $a0, 4

ms32u_return:
    jr      $ra
    nop

.set    reorder
.end    memset32_uncached
