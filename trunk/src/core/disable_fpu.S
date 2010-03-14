/*
#	これはFPU例外を無効にします。(FPU例外を無効にして高速化します)
#	This disables FPU exceptions
#
#	この関数は($31を弄るので)main()直下に書かないとダメかもしれない
#
#	extern void disable_FPU_exeptions_in_main(void);
*/

#include <as_reg_compat.h>

	.set	push 
	.set	noreorder 
	.globl	disable_FPU_exeptions_in_main
	.ent	disable_FPU_exeptions_in_main
disable_FPU_exeptions_in_main: 
	cfc1	$2, $31 
	lui 	$8, 0x80 		# R8 &= 0x80;
	and 	$8, $2, $8		# Mask off all bits except for 23 of FCR 
	ctc1	$8, $31 
	jr		$31 			# return();
	nop
	.end	disable_FPU_exeptions_in_main
	.set	pop

/*
# たぶん汎用性のまるでないstrcmp。子関数
# -------------------------------------------------------
# 字数、大小判定はありません。asciiコード(とシフトJISコード)以外は対応していません。
# 文字列B側は必ずconst定数なので(0で終わると保証されているとみなして)エラーチェックを省いてます。
#
# MIPS R4000系にあわせて、最適化してみました。
# こんなコードになります。(これ以上短く(速く)ならない気がする)
# 先読み実行やスロットのペアリング、ゼロレジスタ等、意識してます。
# Ａ、ＢのスロットペアはＢが延滞スロット[先読み実行]になります。
#
#	extern int tiny_strcmp(char *aaa, const char *bbb);
*/
	#if 1
	.text
	.align	2
	.globl	tiny_strcmp
	.ent	tiny_strcmp
tiny_strcmp:
	.frame	$sp,0,$31				# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
L_loop:
	lb		$3,0($5)				# 評価式3 = (unsigned char)(*bbb)
	lb		$2,0($4)				# 評価式2 = (unsigned char)(*aaa)				# スロットＡ

	bne 	$3,$2,L_not_equal		# if ( 評価式3 != 評価式2 ) { goto not_equal; }	# スロットＡ
	addiu	$5,$5,1 				# bbb++ 										# スロットＢ[先読み実行]

	bnel	$3,$0,L_loop			# if (0x00 != 評価式3 ) {	goto loop;	}		# スロットＡ
	addiu	$4,$4,1 				# aaa++ 										# スロットＢ[先読み実行]
#L_equal:
	j		$31 					# return (0);                                   # スロットＡ
	move	$2,$0					#												# スロットＢ[先読み実行]
L_not_equal:
	j		$31 					# return (1);                                   # スロットＡ
	li		$2,1		# 0x1		#												# スロットＢ[先読み実行]
	.set	macro
	.set	reorder
	.end	tiny_strcmp
	#endif
