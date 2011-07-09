
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	このファイルは直接インクルードしません。
	"game_main.h" からのみ間接的にインクルードします。
---------------------------------------------------------*/

#ifndef _LOAD_STAGE_FILE_H_
#define _LOAD_STAGE_FILE_H_

//#define  MAX_IVENT_ENTRY	512
#define  MAX_PARA1_36		/*64*/(64-20-8-4+32)/*(64-(4*5))*/

typedef struct _stage_command
{
	u32 		v_time; 			/* 1/60 単位の時間カウンタ */
	struct _stage_command *next;
	u32 		user_x; 			/* x */
	int 		user_y; 			/* y */
//(16[Bytes] == (4x4) )
	u8			user_i_code;		/* intermediate code 中間コード */
	u8			user_255_code;		/* 'E'とか'T'とか	*/
	u8			user_item8; 		/* item */
	u8			user_select_gazou;	/* アニメ/画像種類 */
	int 		user_hp;			/* hp */
	int 		user_score; 		/* score */
	int 		user_kougeki_type;	/* 敵が攻撃する場合のスペカの種類 */
//(20[Bytes] == (4x5) )
	char		user_string[MAX_PARA1_36/*(64)*/];/* strings */
//(64[Bytes] == (4x4)+(4)+(44) )
} GAME_COMMAND; 	/* .align 64 [bytes] */
//	u16 	dummy_scroll_speed256;	/* para3 scroll speed256 */


/* 中間コード形式のコマンド */
/* ゲーム中は、中間コード形式のコマンドを扱い構文解析はしない。(構文解析はload_stage.cで行う) */

enum /*_game_command_code_*/
{
/*中間コード*/
/*0x00*/	GC_CODE_00_SJIS_TEXT = 0,		/* 漢字表示[番兵区切りとしても必要] */
/*0x01*/	GC_CODE_01_ALL_CLEAR,			/* ゲーム 全ステージ クリアー */
/*0x02*/	GC_CODE_02_BG_CONTROL,
/*0x03*/	GC_CODE_03_BOSS_COMMON, 		/* 敵 (ボス / 中-ボス)用 スクリプト起動 */
/*0x04*/	GC_CODE_04_CHUU_BOSS_COMMON,	/* 敵 ザコ中-ボス(スクリプト起動しない) */
/*0x05*/	GC_CODE_05_ZAKO,				/* 敵 ザコ */
			//
/*0x06*/	/* 拡張可能 */
/*...*/ 	/* ... */
/*0x2f*/	/* 拡張可能 */
/*0x30*/	GC_CODE_MAX,		/* 0x30 == '0' == 弾幕コマンド (== 速度コマンド、廃止) */
};

#endif /* _LOAD_STAGE_FILE_H_ */
