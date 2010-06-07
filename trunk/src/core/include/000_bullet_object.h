
/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	このファイルは直接インクルードしません。
	"game_main.h" からのみ間接的にインクルードします。
---------------------------------------------------------*/

#ifndef _BULLET_OBJECT_H_
#define _BULLET_OBJECT_H_

enum
{
	BAKUHATSU_MINI00 = 0,/*小爆発*/
	BAKUHATSU_MINI01,
	BAKUHATSU_MINI02,
	BAKUHATSU_MINI03,
//
	BAKUHATSU_ZAKO04,/*ザコ消滅爆発*/
	BAKUHATSU_ZAKO05,
	BAKUHATSU_ZAKO06,
	BAKUHATSU_ZAKO07,
//
	BAKUHATSU_FIRE08,/*火炎爆発*/
	BAKUHATSU_FIRE09,
	BAKUHATSU_FIRE10,
	BAKUHATSU_FIRE11,
//
	BAKUHATSU_MOVE12,/*移動爆発*/
	BAKUHATSU_MOVE13,
	BAKUHATSU_MOVE14,
	BAKUHATSU_MOVE15,
};
#define TAMA_ATARI_02					((2)<<8)
#define TAMA_ATARI_04					((4)<<8)

#define TAMA_ATARI_COMMON16_PNG 		TAMA_ATARI_02
#define TAMA_ATARI_MARU16_PNG			TAMA_ATARI_02
#define TAMA_ATARI_KNIFE18_PNG			TAMA_ATARI_02
#define TAMA_ATARI_JIPPOU32_PNG 		TAMA_ATARI_02

#define TAMA_ATARI_OODAMA_08_PNG		TAMA_ATARI_04



#define TAMA_ATARI_BULLET_DUMMY 		TAMA_ATARI_02
#define JIKI_ATARI_ITEM_16				((16)<<8)
#define JIKI_ATARI_ITEM_80				((80)<<8)


#define ZAKO_ATARI02_PNG				TAMA_ATARI_02
#define ZAKO_ATARI04_PNG				TAMA_ATARI_04
#define ZAKO_ATARI16_PNG				JIKI_ATARI_ITEM_16
#define ZAKO_ATARI_HOMING16_PNG 		JIKI_ATARI_ITEM_16


#if 1
	/* 角度値(0-1023)が1024 を超えた場合の特殊機能 */
//
	/* 自機狙い弾 */
	#define ANGLE_JIKI_NERAI_DAN	(9999)
//
	/* 特別に撃たない弾 */
	#define ANGLE_NO_SHOT_DAN		(8888)
//
	#if 0
		/* 偶数弾の場合に自機狙い(動かないと当たる) / even bullet */
		//#define ANGLE_JIKI_NERAI_GUSUDAN 999
		/* 奇数弾の場合に自機狙い(動かなければ当たらない) / odd bullet */
		//#define ANGLE_JIKI_NERAI_KISUDAN 888
		/*
		よく考えたら、ANGLE_JIKI_NERAI_KISUDAN は必要ない。
		ANGLE_JIKI_NERAI_DAN だけあれば良く、
		発弾数に偶数を指定すれば偶数弾。
		発弾数に奇数を指定すれば奇数弾。
		あたりまえだけど、気が付かなかったww。
		*/
	#endif
#endif


/* とりあえず */
#if 1
	#define boss_base_danmaku_type				user_data10
	#define boss_base_danmaku_time_out			user_data11
	#define boss_base_danmaku_test				user_data12
	#define boss_base_state001					user_data13
#endif


extern int boss_hamidasi;			/* 「移動できなかったフラグ」(使用前に手動でOFF==0にしとく) */
extern POINT256 boss_clip_min;		/* ボス移動範囲(最小値) */
extern POINT256 boss_clip_max;		/* ボス移動範囲(最大値) */

extern POINT256 bullet_clip_min;	/* 弾の範囲(最小値) */
extern POINT256 bullet_clip_max;	/* 弾の範囲(最大値) */

#if 1
enum
{
	REGIST_TYPE_00_MULTI_VECTOR,		/* 多方向弾 */
	REGIST_TYPE_01_HAZUMI,		/* 重力弾01 輝夜、最終形態で投げてくるかなり無茶な弾。 */
	REGIST_TYPE_02_GRAVITY02,	/* 重力弾02 */
	REGIST_TYPE_03_TOMARI,		/*	弾 */
	REGIST_TYPE_04_KURU_KURU,	/*	弾 */
	REGIST_TYPE_99_MAX,
};

/* 弾登録 */
extern void bullet_regist_vector(void);
extern void bullet_regist_angle(void);
/* ベクトル弾の場合 */
typedef struct _bullet_regist_
{
	int BULLET_REGIST_speed256; 							//		/* aa */
	int BULLET_REGIST_angle1024;							//		/* bb */
	int BULLET_REGIST_div_angle1024;						//		/* cc */
	int BULLET_REGIST_bullet_obj_type;						//		/* dd */

	int BULLET_REGIST_n_way;								//		/* ee */
	int BULLET_REGIST_sakuya_kurukurku_knife_height;		//		/* ff */

	int BULLET_REGIST_regist_type;							/* 登録方式 */
} BULLET_REGIST;
extern BULLET_REGIST br;
#endif
#define BULLET_REGIST_hosi_gata_time_out			BULLET_REGIST_speed256
#define BULLET_REGIST_hosi_gata_angle1024			BULLET_REGIST_angle1024
#define BULLET_REGIST_hosi_gata_add_speed256		BULLET_REGIST_div_angle1024

#define BULLET_REGIST_jyuryoku_dan_delta256 		BULLET_REGIST_div_angle1024
#define BULLET_REGIST_jyuryoku_dan_bound_counts 	BULLET_REGIST_n_way

#define BULLET_REGIST_tomari2_dan_delta256			BULLET_REGIST_div_angle1024
#define BULLET_REGIST_tomari2_dan_next_angle1024	BULLET_REGIST_n_way

//
/* 角度弾の場合 */

//typedef struct _bullet_regist_
//{
//	int BULLET_REGIST_speed256; 					//		/* aa */	<同じ>
#define BULLET_REGIST_angle65536			BULLET_REGIST_angle1024 		/* <65536度に決定> */	//		/* bb */
#define BULLET_REGIST_div_angle65536		BULLET_REGIST_div_angle1024 	/* <65536度に決定> */
//	int BULLET_REGIST_bullet_obj_type;				//		/* dd */	<同じ>
//
//	int BULLET_REGIST_n_way;						//		/* ee */	<同じ>
#define BULLET_REGIST_speed_offset			BULLET_REGIST_sakuya_kurukurku_knife_height 	//		/* ff */	<未定>
//
//	int BULLET_REGIST_regist_type;					/* 登録方式 */		<同じ>
//} BULLET_REGIST;


//extern void bakuhatsu_add_type(int x256, int y256, /*dou ble int delay_wait,*/ int type);
extern void bakuhatsu_add_type_ddd(SPRITE *src/*int x256, int y256*/, /*dou ble int delay_wait,*/ int type);
extern void bakuhatsu_add_rect(SPRITE *src);
extern void bakuhatsu_add_circle(SPRITE *src, int mode);
extern void bakuhatsu_add_zako04(SPRITE *src);

extern void callback_hit_zako(SPRITE *src, SPRITE *t);

/* ボス移動処理の共通ルーチン */

extern void boss_move_vx_vy(SPRITE *src);
extern void boss_clip_rect(SPRITE *src);


// 共通形態
extern void common_99_keitai(SPRITE *src);/* 撃破後に画面外にボスが逃げる */

/* thegame.c のみで ザコ／ボスで宣言が必要なもの(グローバル)は、thegame.c へ移動した。 */

/* ボスで宣言が必要なもの(グローバル) */

extern void enemy_set_random_seed(void/*int set_seed*/);
//extern int en emy_get_random_item(void);
extern void lose_random_item(SPRITE *src);

//#include "danmaku.h"

#endif /* _BULLET_OBJECT_H_ */

#ifndef _SPELL_CARD_H_
#define _SPELL_CARD_H_
//	SPELL_CARD_00_OFF = 0,
enum
{
// 咲夜 6面
	SPELL_CARD_00_sakuya_000 = 0,	/* "形態変更" */
	SPELL_CARD_01_sakuya_aaa,		/* 第一形態: 左右に動いて小弾撃ち */
	SPELL_CARD_02_sakuya_bbb,		/* 第二形態: 全方位、豆まき */
	SPELL_CARD_03_sakuya_ccc,		/* 第三形態: 垂直ナイフが落ちてくるよ */
	SPELL_CARD_04_sakuya_ddd,		/* 第四形態: 魔方陣生成 */
	SPELL_CARD_05_sakuya_eee,		/* 第五形態: (黄色マスカット弾) */
//	SPELL_CARD_06_sakuya_fff,		/* 第六形態: (黄色マスカット弾) */
//	SPELL_CARD_07_sakuya_ggg,		/* 第七形態: (分散魔方陣)追加計画中 */
//	SPELL_CARD_08_sakuya_hhh,		/* 第八形態: (時止めナイフ)追加計画中 */
	SPELL_CARD_09_sakuya_iii,		/* 第九形態: 最終形態(その1) */
	SPELL_CARD_10_sakuya_jjj,		/* 第10形態: 最終形態(その2) */
	SPELL_CARD_11_sakuya_kkk,		/* 第11形態: 最終形態(その3) */
// パチェ 5面
	SPELL_CARD_00_pache_000,
	SPELL_CARD_11_pache_bbb,
	SPELL_CARD_12_pache_ccc,
	SPELL_CARD_13_pache_ddd,
	SPELL_CARD_14_pache_eee,
	SPELL_CARD_15_pache_fff,
	SPELL_CARD_16_pache_ggg,
	SPELL_CARD_17_pache_hhh,
//	SPELL_CARD_18_pache_iii,
//	SPELL_CARD_19_pache_jjj,
// 文 4面
	SPELL_CARD_00_aya_000,
	SPELL_CARD_11_aya_bbb,
	SPELL_CARD_12_aya_ccc,
	SPELL_CARD_13_aya_ddd,
	SPELL_CARD_14_aya_eee,
	SPELL_CARD_15_aya_fff,
	SPELL_CARD_16_aya_ggg,
	SPELL_CARD_17_aya_hhh,
	SPELL_CARD_18_aya_iii,
	SPELL_CARD_19_aya_jjj,
// 輝夜 3面
	SPELL_CARD_00_kaguya_000,
	SPELL_CARD_11_kaguya_bbb,
	SPELL_CARD_12_kaguya_ccc,
	SPELL_CARD_13_kaguya_ddd,
	SPELL_CARD_14_kaguya_eee,
	SPELL_CARD_15_kaguya_fff,
	SPELL_CARD_16_kaguya_ggg,
	SPELL_CARD_17_kaguya_hhh,
// 未定 2面
	SPELL_CARD_00_mima_000,
	SPELL_CARD_11_mima_bbb,
	SPELL_CARD_12_mima_ccc,
	SPELL_CARD_13_mima_ddd,
	SPELL_CARD_14_mima_eee,
	SPELL_CARD_15_mima_fff,
	SPELL_CARD_16_mima_ggg,
	SPELL_CARD_17_mima_hhh,
// アリス 1面
	SPELL_CARD_00_alice_000,
	SPELL_CARD_11_alice_bbb,
	SPELL_CARD_12_alice_ccc,
	SPELL_CARD_13_alice_ddd,
	SPELL_CARD_14_alice_eee,
//	SPELL_CARD_15_alice_fff,
//	SPELL_CARD_16_alice_ggg,
//	SPELL_CARD_17_alice_hhh,
//	SPELL_CARD_18_alice_iii,
//	SPELL_CARD_19_alice_jjj,
//
	SPELL_CARD_MAX	/* 最大数 */
};

//------------ スペカ関連

extern void create_spell_card(SPRITE *src, int spell_card_type);

#if 0
//extern void re gist_spell_card222(SPRITE *src);/* スペルカードの登録 */
//extern void sp ell_card_generator222(SPRITE *src);
#else
//extern void ch eck_regist_spell_card(SPRITE *src);/* スペルカードの更新チェック */
extern void check_regist_generate_spell_card(SPRITE *src);	/* スペカ登録可能なら登録 / スペカ生成 */
#endif


extern	void danmaku_state_check_holding(SPRITE *src);/* 弾幕が終わるまで待つ。 */

enum
{
	SPELL_CARD_MODE_00_OFF = 0, 		/* スペカを使用しない(通常攻撃等)。(スペカが撃てるかどうか判断) */
	SPELL_CARD_MODE_01_IDO_JYUNNBI, 	/* 撃てる場合。発弾位置まで移動 */
	SPELL_CARD_MODE_02_TAIHI,		/* 中。 */
	SPELL_CARD_MODE_03_HATUDAN, 		/* 発弾中。 */
};

extern int spell_card_mode; 			/* スペカモード */
extern int spell_card_limit_health; 	/* 規定値以下になればスペカモード解除 */
extern int spell_card_boss_state;		/* 負値になればボススペカモードに入らない */
extern int spell_card_boss_timer;		/* [共用]制限時間 */

extern int spell_card_number;			/* [共用]スペカ番号 */
extern int spell_card_max;				/* [共用]スペカ番号最大限界値 */

/* 出現時x座標 */
#define BOSS_XP256	(t256(GAME_WIDTH/2)-(t256(32/2)))/*sakuya->w128*/


//------------ "回"みたいなマークのエフェクト
/*
「"回"みたいなマーク」は、ボスが持ってるスペルカードだそうです。
だから本当は、スペカの枚数が減ったら、枚数を減らさなきゃいけないのかな？
でも本家そうなってないよね。
*/

/*static*/extern  void move_boss_effect(SPRITE *src);

/*static*/extern  void boss_effect_initialize_position(void);


//	ザコ登録の共通ルーチン
//extern void add_zako_common(STAGE_DATA *l, SPRITE *src);

#endif /* _SPELL_CARD_H_ */
