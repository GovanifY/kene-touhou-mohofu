#ifndef _LOAD_STAGE_FILE_H_
#define _LOAD_STAGE_FILE_H_

#include "game_main.h"

//#define  MAX_IVENT_ENTRY	512
#define  MAX_PARA1_36		/*64*/(64-20-8)/*(64-(4*5))*/

typedef struct _stage_data
{
	u32 		v_time; 			/* 1/60 単位の時間カウンタ */	//	int 		dummy_done;
	struct _stage_data *next;
	u32 		user_x; 			/* para 0 */	/*ctype*/	/* [***090210	追加 */
	int 		user_y; 			/* para 2 */
//(16[Bytes] == (4x4) )
	char		user_i_code;		/* intermediate code 中間コード */
	char		user_1_moji;		/* 'E'とか'T'とか	*/
	u16 		user_item16;		/* item */
	int 		user_hp; 			/* para 2 */
	int 		user_score; 		/* para 2 */
//(20[Bytes] == (4x5) )
	char		user_string[MAX_PARA1_36/*(64)*/];/* para 1 */
//(64[Bytes] == (4x4)+(4)+(44) )
} STAGE_DATA;	/* .align 64 [bytes] */
//	u16 	dummy_scroll_speed256;	/* para3 scroll speed256 */ /* [***090210	追加 */



/* 中間コード形式のコマンド */
/* ゲーム中は、中間コード形式のコマンドを扱い構文解析はしない。(構文解析はload_stage.cで行う) */

enum
{
/*中間コード*/
/*0x00*/		CTYPE_00_NONE = 0,/* [番兵区切り] */	/*error*/
			/* その他 */
/*0x01*/		CTYPE_01_ALL_CLEAR,
/*0x02*/		CTYPE_02_BG_CONTROL,
			/* ボス */
/*0x03*/		CTYPE_03_BOSS_KENE,
/*0x04*/		CTYPE_04_BOSS_CIRNO,
/*0x05*/		CTYPE_05_BOSS_SAKUYA,	// [***090207		追加
/*0x06*/		CTYPE_06_BOSS_PACHE,
/*0x07*/		CTYPE_07_BOSS_KAGUYA,
/*0x08*/		CTYPE_08_BOSS_EIRIN,		/* 3面はどうするか未定 */
/*0x09*/		CTYPE_09_BOSS_FUMI,
/*0x0a*/		CTYPE_10_BOSS_ALICE,
			/* 特殊敵[中型敵] */
/*0x0b*/		CTYPE_11_6, 			//		追加
/*0x0c*/		CTYPE_12_KOAKUMA,		//		追加
/*0x0d*/		CTYPE_13_UDONGE,		//		追加
/*0x0e*/		CTYPE_14_TEI,			//		追加
/*0x0f*/		CTYPE_15_2, 			//		追加
/*0x10*/		CTYPE_16_RUMIA, 		//		追加
			//		廃止?
/*0x11*/		CTYPE_17_YOKAI2,		//		追加
/*0x12*/		CTYPE_18_YOKAI1,		// [***090207		追加
			/* 魔方陣 */
/*0x13*/		CTYPE_19_MAHOUJIN_A,
/*0x14*/		CTYPE_20_MAHOUJIN_B,
			/* 竜巻 陰陽玉 */
/*0x15*/		CTYPE_21_INYOU1,
/*0x16*/		CTYPE_22_TATSUMAKI1,
			/* 妖怪 */
/*0x17*/		CTYPE_23_KAKOMI1,
/*0x1a*/		CTYPE_26_AKA_KEDAMA1,		/* 橙 */
			/* 毛玉 */
/*0x18*/		CTYPE_24_YUKARI1,			/* その他ザコ */
/*0x19*/		CTYPE_25_YUKARI2,			/* その他ザコ */
/*0x1b*/		CTYPE_27_MIDORI_KEDAMA1,
/*0x1c*/		CTYPE_28_MIDORI_KEDAMA2,
/*0x1d*/		CTYPE_29_KEDAMA1,
/*0x1e*/		CTYPE_30_KEDAMA2,
			/* [C妖精]その他ザコ */
/*0x1f*/		CTYPE_31_OBAKE1,
/*0x20*/		CTYPE_32_OBAKE2,	/* 旧"虹毛玉1" */
/*0x21*/		CTYPE_33_KARASU,
			/* [B妖精]中妖精 */
/*0x22*/		CTYPE_34_MEIDO1,
/*0x23*/		CTYPE_35_MEIDO2,
/*0x24*/		CTYPE_36_MEIDO3,
/*0x25*/		CTYPE_37_MEIDO4,
			/* [A妖精]小妖精 */
/*0x26*/		CTYPE_38_AO_YOUSEI1,	// [***090124		追加
/*0x27*/		CTYPE_39_AO_YOUSEI2,	// [***090207		追加
/*0x28*/		CTYPE_40_AO_YOUSEI3,	//		追加
/*0x29*/		CTYPE_41_AO_YOUSEI4,	//		追加
				//
				//
/*0x2a*/		ETYPE_01_SJIS_TEXT,
/*0x2b*/		ETYPE_02_LOAD_BG,
/*0x2c*/	//	ETYPE_03_PICTURE,	/* この方式は処理落ち解消しにくいので都合により廃止 */
				//
/*0x2d*/		/*拡張可能*/
/*0x2e*/		/*拡張可能*/
/*0x2f*/		/*拡張可能*/
/*0x30*/		ETYPE_MAX,		/* 0x30 == '0' == 弾幕コマンド (== 速度コマンド、廃止) */
};

/* 現在 32 種類 */
//#define CTYPE_99_MAX (CTYPE_17_AO_YOUSEI4+1)
#define CTYPE_99_MAX (ETYPE_01_SJIS_TEXT)

#endif /* _LOAD_STAGE_FILE_H_ */
