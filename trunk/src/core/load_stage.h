#ifndef _LOAD_STAGE_FILE_H_
#define _LOAD_STAGE_FILE_H_

#include "game_main.h"

//#define  MAX_IVENT_ENTRY	512
#define  MAX_PARA1_44		/*64*/44-4/*(64-(4*5))*/

typedef struct _stage_data
{
	Uint32 v_time;		/* 1/60 単位の時間カウンタ */
	int done;
	Uint32 user_x;		/* para 0 */	/*ctype*/	/* [***090210	追加 */
	int user_y; 		/* para 2 */
//(16[Bytes] == (4x4) )
	Uint16 scroll_speed256; /* para3 scroll speed256 */ /* [***090210	追加 */
	char user_command;
	char user_command_B; /*resurved now*/		/* [***090210	追加 */
//(20[Bytes] == (4x4)+(4) )
	char user_string[MAX_PARA1_44/*64*/];/* para 1 */
//(64[Bytes] == (4x4)+(4)+(44) )
	struct _stage_data *next;
} STAGE_DATA;

/* 中間コード形式のコマンド */
/* ゲーム中は、中間コード形式のコマンドを扱い構文解析はしない。(構文解析はloadlv.cで行う) */

enum
{
/*中間コード*/
/*0x00*/		CTYPE_00_NONE = 0,/* [番兵区切り] */	/*error*/
			//	その他
/*0x01*/		CTYPE_01_ALL_CLEAR,
			//	ボス
/*0x13*/		CTYPE_19_BOSS_ALICE,
/*0x14*/		CTYPE_20_BOSS_CIRNO,
/*0x15*/		CTYPE_21_BOSS_FUMI,
/*0x16*/		CTYPE_22_BOSS_KAGUYA,
/*0x17*/		CTYPE_23_BOSS_SAKUYA,	// [***090207		追加
			//	特殊敵[中型敵]
/*0x12*/		CTYPE_18_YOKAI1,		// [***090207		追加
			//	その他ザコ
/*0x01*/		CTYPE_01_OBAKE,
/*0x02*/		CTYPE_02_YUKARI1,
/*0x03*/		CTYPE_03_YUKARI2,
			//	毛玉
/*0x04*/		CTYPE_04_AKA_KEDAMA1,
/*0x05*/		CTYPE_05_NIJI_KEDAMA1,
/*0x06*/		CTYPE_06_MIDORI_KEDAMA1,
/*0x07*/		CTYPE_07_MIDORI_KEDAMA2,
/*0x08*/		CTYPE_08_KEDAMA1,
			//	竜巻 陰陽玉
/*0x09*/		CTYPE_09_INYOU1,
/*0x0a*/		CTYPE_10_TATSUMAKI1,
			//	妖怪
/*0x0b*/		CTYPE_11_KAKOMI1,
			//	中妖精
/*0x0c*/		CTYPE_12_MEIDO1,
/*0x0d*/		CTYPE_13_MEIDO2,
/*0x0e*/		CTYPE_14_MEIDO3,
/*0x0f*/		CTYPE_15_MEIDO4,
			//	小妖精
/*0x10*/		CTYPE_16_AO_YOUSEI1,	// [***090124		追加
/*0x11*/		CTYPE_17_AO_YOUSEI2,	// [***090207		追加
				//
/*0x18*/		BTYPE_00_NONE,/* [番兵区切り] */	/*error*/
/*0x19*/		BTYPE_01_BGPANEL1,
/*0x1a*/		BTYPE_02_BGPANEL2,
/*0x1b*/		BTYPE_03_GROUNDER,
/*0x1c*/		BTYPE_04_MAGIC_FORMATION,
/*0x1d*/		BTYPE_05_BG_CONTROL,
				//
/*0x1e*/		ETYPE_01_ENGLISH_TEXT,
/*0x1f*/		ETYPE_02_LOAD_BG,
/*0x20*/		ETYPE_03_PICTURE,
				//
/*0x21*/		/*拡張可能*/
/*0x22*/		/*拡張可能*/
/*0x23*/		/*拡張可能*/
/*0x24*/		/*拡張可能*/
/*0x25*/		/*拡張可能*/
/*0x26*/		/*拡張可能*/
/*0x27*/		/*拡張可能*/
/*0x28*/		/*拡張可能*/
/*0x29*/		/*拡張可能*/
/*0x2a*/		/*拡張可能*/
/*0x2b*/		/*拡張可能*/
/*0x2c*/		/*拡張可能*/
/*0x2d*/		/*拡張可能*/
/*0x2e*/		/*拡張可能*/
/*0x2f*/		/*拡張可能*/
/*0x30*/		ETYPE_MAX,		/* 0x30 == '0' == 速度コマンド */
};
/* 現在 23 種類 */
#define CTYPE_MAX_23_ BTYPE_00_NONE/*(23)*/
//#undef CTYPE_MAX_23_

#define BTYPE_MAX (BTYPE_05_BG_CONTROL+1)

#endif /* _LOAD_STAGE_FILE_H_ */
