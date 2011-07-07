
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
	u32 		v_time; 			/* 1/60 単位の時間カウンタ */	//	int 		dummy_done;
	struct _stage_command *next;
	u32 		user_x; 			/* para 0 */	/*ctype*/	/* [***090210	追加 */
	int 		user_y; 			/* para 2 */
//(16[Bytes] == (4x4) )
	u8			user_i_code;		/* intermediate code 中間コード */
	u8			user_255_code;		/* 'E'とか'T'とか	*/
	u8			user_item8; 		/* item */
	u8			user_select_gazou;	/* アニメ/画像種類 */
	int 		user_hp;			/* para 2 */
	int 		user_score; 		/* para 2 */
	int 		user_kougeki_type;	/* 敵が攻撃する場合のスペカの種類 */
//(20[Bytes] == (4x5) )
	char		user_string[MAX_PARA1_36/*(64)*/];/* para 1 */
//(64[Bytes] == (4x4)+(4)+(44) )
} GAME_COMMAND; 	/* .align 64 [bytes] */
//	u16 	dummy_scroll_speed256;	/* para3 scroll speed256 */ /* [***090210	追加 */


/* 中間コード形式のコマンド */
/* ゲーム中は、中間コード形式のコマンドを扱い構文解析はしない。(構文解析はload_stage.cで行う) */

enum
{
/*中間コード*/
/*0x00*/		CTYPE_00_NONE = 0,/* [番兵区切り] */	/*error*/
			/* その他 */
/*0x01*/		CTYPE_01_ALL_CLEAR, 			/* ゲーム 全ステージ クリアー */
/*0x02*/		CTYPE_02_BG_CONTROL,
				// 敵
/*0x03*/		CTYPE_03_BOSS_COMMON,			/* (ボス / 中-ボス)用 スクリプト起動 */
/*0x04*/		CTYPE_04_CHUU_BOSS_COMMON,		/* ザコ中-ボス(スクリプト起動しない) */
/*0x05*/		CTYPE_05_ZAKO,					/* ザコ */
				//
/*0x06*/		ETYPE_01_SJIS_TEXT,
/*0x07*/		ETYPE_02_LOAD_BG,
/*0x08*/	//	ETYPE_03_PICTURE,	/* この方式は処理落ち解消しにくいので都合により廃止 */
				//
/*0x09*/		/* 拡張可能 */
/*...*/ 		/* ... */
/*0x2f*/		/* 拡張可能 */
/*0x30*/		ETYPE_MAX,		/* 0x30 == '0' == 弾幕コマンド (== 速度コマンド、廃止) */
};

/* 現在 32 種類 */
//#define CTYPE_99_MAX (CTYPE_17_AO_YOUSEI4+1)
#define CTYPE_99_MAX (ETYPE_01_SJIS_TEXT)

#endif /* _LOAD_STAGE_FILE_H_ */
