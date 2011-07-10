
# /*
# ---------------------------------------------------------
#	東方模倣風 ～ Toho Imitation Style.
#	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
# ---------------------------------------------------------
# notaz 氏 http://notaz.gp2x.de/ の psp picodrive version1.35 source.
# http://notaz.gp2x.de/releases/PicoDrive/PicoDrive_135src.tar.bz2
# に入ってた物を修正したものです。
# -------------------------------------------------------
# */

.set noreorder
.set noat

.text
.align 4

# /* extern void memcpy32(s32 *dest, s32 *src, int count); // 32bit word count */
# /* extern void memcpy32(int *dest, int *src, int count); // 32bit word count */
.globl  memcpy32
.ent    memcpy32
memcpy32:
mc32_aloop:
    andi    $t0, $a0, 0x3f

    beqz    $t0, mc32_bloop_prep
    nop

    lw      $t1, 0($a1)
    addiu   $a2, -1

    sw      $t1, 0($a0)

    beqz    $a2, mc32_return
    addiu   $a0, 4

    j       mc32_aloop
    addiu   $a1, 4

mc32_bloop_prep:
    srl     $t0, $a2, 4    # /* we will do 64 bytes per iteration (cache line) */
    beqz    $t0, mc32_bloop_end

mc32_bloop:
    addiu   $t0, -1
    cache   0x18, ($a0)    # /* create dirty exclusive */

    lw      $t2, 0x00($a1)
    lw      $t3, 0x04($a1)

    lw      $t4, 0x08($a1)
    lw      $t5, 0x0c($a1)

    lw      $t6, 0x10($a1)
    lw      $t7, 0x14($a1)

    lw      $t8, 0x18($a1)
    lw      $t9, 0x1c($a1)

    sw      $t2, 0x00($a0)
    sw      $t3, 0x04($a0)

    sw      $t4, 0x08($a0)
    sw      $t5, 0x0c($a0)

    sw      $t6, 0x10($a0)
    sw      $t7, 0x14($a0)

    sw      $t8, 0x18($a0)
    sw      $t9, 0x1c($a0)

    lw      $t2, 0x20($a1)
    lw      $t3, 0x24($a1)

    lw      $t4, 0x28($a1)
    lw      $t5, 0x2c($a1)

    lw      $t6, 0x30($a1)
    lw      $t7, 0x34($a1)

    lw      $t8, 0x38($a1)
    lw      $t9, 0x3c($a1)

    sw      $t2, 0x20($a0)
    sw      $t3, 0x24($a0)

    sw      $t4, 0x28($a0)
    sw      $t5, 0x2c($a0)

    sw      $t6, 0x30($a0)
    sw      $t7, 0x34($a0)

    sw      $t8, 0x38($a0)
    sw      $t9, 0x3c($a0)

    addiu   $a0, 0x40

    bnez    $t0, mc32_bloop
    addiu   $a1, 0x40

mc32_bloop_end:
    andi    $a2, $a2, 0x0f
    beqz    $a2, mc32_return

mc32_cloop:
    lw      $t1, 0($a1)
    addiu   $a2, -1
    addiu   $a1, 4
    sw      $t1, 0($a0)
    bnez    $a2, mc32_cloop
    addiu   $a0, 4

mc32_return:
    jr      $ra
    nop

.set    reorder
.end    memcpy32
