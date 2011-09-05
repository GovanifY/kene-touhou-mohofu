
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

# /*
# memset32。子関数
# -------------------------------------------------------
# 32 bit専用の memset。
# セットする値の引数が char(8bit) から int(32bit)になっているので、
# 最低転送単位が、4byte。
# これと memset(dest, 0x12, 8);
# これが memset32(dest, 0x12121212, 2); # 2==8/4
# 一緒。
# -------------------------------------------------------
# 引数
# -------------------------------------------------------
# int *dest;    転送先のアドレス(C言語ではポインタで指定)
#               通常は (void *)にキャストして指定する事。
# int data;     埋めつくすデーター。
#               0x12 で埋めたいなら 0x12121212 と言う感じ。
#               0xff で埋めたいなら 0xffffffff と言う感じ。
#               0x12, 0x34, 0x56, 0x78 の 繰り返しで埋めたいなら 0x78563412 と言う感じ。
# int count     転送量。long(4byte) 単位。
#               count に 1 を指定すると 4bytes 転送する。
# -------------------------------------------------------
# 戻り値
# -------------------------------------------------------
# void          なし。
# -------------------------------------------------------
# 仕様。
# -------------------------------------------------------
# 1. 転送サイズが 4byte で割り切れる必要がある事。
# 2. ある程度転送量が無いと、 memsetより遅い。
# */

.set noreorder
.set noat

.text
.align 4

# /* extern void memset32(s32 *dest, s32 data, int count); */
# /* extern void memset32(int *dest, int data, int count); */
.globl  memset32
.ent    memset32
# /* ------------------------------------------------------ */
# /* [1]. 始めに 64bytes .align されていないデーターを転送する。 */
# /* 32bytes でないのは、キャッシュ幅が 64bytesなので。(要後で確認: pspのキャッシュ幅って32bytesじゃなかったっけ？)  */
memset32:
L_memset32_1st_loop:
    andi    $t0, $a0, 0x3f              # (先読み実行して大丈夫な気もする) 評価式0 = (dest & 0x3f);  とにかく 64 bytes で .align 。
    nop                 #

    beqz    $t0, L_memset32_2nd_start   # if (0x00 == 評価式0 ) goto L_memset32_2nd_start
    nop                 #
# /* $t0先読み実行しても良い気もするが、細かい仕様が判らなかったので止めといた。 */
# /* a0の値を多少弄ってもっと最適化出来そうな気がする。(mipsはアドレスオフセットに負数が使える。) */

    sw      $a1, 0($a0)                 # *((s32*)dest+0) = (s32)data;  32bit(4bytes) 転送.
    addiu   $a2, -1                     # count--;

    beqz    $a2, L_memset32_return_void            # if (0x00 == count ) { return(); }
    addiu   $a0, 4                      # dest += 4;

    j       L_memset32_1st_loop         # goto L_memset32_1st_loop;
    nop

# /* ------------------------------------------------------ */
# /* [2]. 次に .alignされているデーターを転送する。 */
                                        # /* srl は 1クロックディレイ するので、ペアリング出来ません。 */
L_memset32_2nd_start:
# /* we will do 64 bytes per iteration (cache line). */
    srl     $t0, $a2, 4                 # (srl は先読み実行出来ない) 評価式1 = (count / 16);
    andi    $a2, $a2, 0x0f              # (同じレジスタは先読み実行出来ない) count &= 0x0f;
    # /* 64byte未満なら([2].で)転送する必要が無いので、次([3].)へ。(64bytes == 16 long words. ) */
    beqz    $t0, L_memset32_quit_check  # if (0x00 == 評価式1 ) goto L_memset32_quit_check
    nop                                 #  ここに nopを書いた方が良い。

L_memset32_2nd_loop:
    addiu   $t0, -1                     # 評価式1--;
    cache   0x18, ($a0)                 # /* create dirty exclusive. (dest のアドレスをキャッシュ書き換え候補にする？)良くわかんない(要調査)。 */
    # /* 16bytes転送(16==4x4) */
    sw      $a1, 0x00($a0)              # *((s32*)dest+0x00) = (s32)data;  32bit(4bytes) 転送.
    sw      $a1, 0x04($a0)              # *((s32*)dest+0x04) = (s32)data;  32bit(4bytes) 転送.
    sw      $a1, 0x08($a0)              # *((s32*)dest+0x08) = (s32)data;  32bit(4bytes) 転送.
    sw      $a1, 0x0c($a0)              # *((s32*)dest+0x0c) = (s32)data;  32bit(4bytes) 転送.
    # /* 16bytes転送(16==4x4) */
    sw      $a1, 0x10($a0)              # *((s32*)dest+0x10) = (s32)data;  32bit(4bytes) 転送.
    sw      $a1, 0x14($a0)              # *((s32*)dest+0x14) = (s32)data;  32bit(4bytes) 転送.
    sw      $a1, 0x18($a0)              # *((s32*)dest+0x18) = (s32)data;  32bit(4bytes) 転送.
    sw      $a1, 0x1c($a0)              # *((s32*)dest+0x1c) = (s32)data;  32bit(4bytes) 転送.
    # /* 16bytes転送(16==4x4) */
    sw      $a1, 0x20($a0)              # *((s32*)dest+0x20) = (s32)data;  32bit(4bytes) 転送.
    sw      $a1, 0x24($a0)              # *((s32*)dest+0x24) = (s32)data;  32bit(4bytes) 転送.
    sw      $a1, 0x28($a0)              # *((s32*)dest+0x28) = (s32)data;  32bit(4bytes) 転送.
    sw      $a1, 0x2c($a0)              # *((s32*)dest+0x2c) = (s32)data;  32bit(4bytes) 転送.
    # /* 16bytes転送(16==4x4) */
    sw      $a1, 0x30($a0)              # *((s32*)dest+0x30) = (s32)data;  32bit(4bytes) 転送.
    sw      $a1, 0x34($a0)              # *((s32*)dest+0x34) = (s32)data;  32bit(4bytes) 転送.
    sw      $a1, 0x38($a0)              # *((s32*)dest+0x38) = (s32)data;  32bit(4bytes) 転送.
    sw      $a1, 0x3c($a0)              # *((s32*)dest+0x3c) = (s32)data;  32bit(4bytes) 転送.
    # /* 64bytes転送した(64==4x4x4) */
    # /* 残りがあれば転送する必要があるのでループ */
    bnez    $t0, L_memset32_2nd_loop    # if (0x00 != 評価式1 ) goto L_memset32_2nd_loop
    addiu   $a0, 0x40                   # dest += 0x40; // 0x40==64bytes転送した。

L_memset32_quit_check:
    # /* 残りが 0 の場合転送する必要が無いので終了。(return ();) */
    beqz    $a2, L_memset32_return_void # if (0x00 == count ) { return(); }
    nop                                 #  ここに nopを書いた方が良い。

# /* ------------------------------------------------------ */
# /* [C]. 最後に残りのデーターを転送する。(C-Loop) */
L_memset32_3rd_loop:
    addiu   $a2, -1                     # count--;
    sw      $a1, 0($a0)                 # *((s32*)dest+0) = (s32)data;  32bit(4bytes) 転送.

    bnez    $a2, L_memset32_3rd_loop
    addiu   $a0, 4                      # dest += 4;

L_memset32_return_void:
    jr      $ra                         # return();
    nop

.set    reorder
.end    memset32
