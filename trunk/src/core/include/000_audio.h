
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	このファイルは直接インクルードしません。
	"game_main.h" からのみ間接的にインクルードします。
---------------------------------------------------------*/

#ifndef _BGM_VOICE_H_
#define _BGM_VOICE_H_

enum
{
	BGM_00_stop = 0,	/* ----- BGM 演奏停止コマンド */
	BGM_01_stage1,		/* 1面道中 */
	BGM_02_stage2,		/* 2面道中 */
	BGM_03_stage3,		/* 3面道中 */
	BGM_04_stage4,		/* 4面道中 */
	BGM_05_stage5,		/* 5面道中 */
	BGM_06_stage6,		/* 6面道中 */
	BGM_07_stage7,		/* エンディング(道中) */
	BGM_08_stage8,		/* エキストラステージ道中 */
	BGM_09_stage9,		/* ファンタズムステージ道中 */
	BGM_10_menu04,		/* エンディング(上海紅茶館) */
	BGM_11_boss01,		/* 1面ボス */
	BGM_12_boss02,		/* 2面ボス */
	BGM_13_boss03,		/* 3面ボス */
	BGM_14_boss04,		/* 4面ボス */
	BGM_15_boss05,		/* 5面ボス */
	BGM_16_boss06,		/* 6面ボス */
	BGM_17_boss07,		/* エキストラステージ1ボス */
	BGM_18_boss08,		/* エキストラステージ2ボス */
	BGM_19_boss09,		/* エキストラステージ3ボス */
	BGM_20_boss10,		/* 隠しボス */
	BGM_21_boss00,		/* 隠しボス */
	BGM_21_menu03,		/* エンディング(シナリオ) */
	BGM_22_menu02,		/* キーコンフィグ */
	BGM_23_menu01,		/* タイトル画面 */
/* ----- 曲の数(読み込みファイル数) */
//#define USE_MUSIC_FILES 15/*14*/ /**/
	USE_MUSIC_FILES
};

enum
{
	VOICE00_SHOT = 0,			//	"ショット", 		/* a				ショット音							[ちりちりちり] */
	VOICE01_MENU_OK,			//	"メニュー決定", 	/*[b]	[s(?)]		音									[びょいー] */
	VOICE02_MENU_SELECT,		//	"メニュー選択", 	/* c				サコ死に音 + 弾消し音 + メニュー	[ちゅ] */
	VOICE03_BOSS_HAKAI, 		//	"ボス退場", 		/*[d];	[s(?)]		ボス死に音? 						[ぼがーーん] */
	VOICE04_SHIP_HAKAI, 		//	"ピチューン",		/* e :				自機破壊音							[ぴちゅーん] */
	VOICE05_BONUS,				//	"アイテム取得", 	/* f				アイテム取得音						[かつん] */
	VOICE06_EXTEND, 			//	"増えた",			/* g :				残機増加音							[びゅわわわわん] */
	VOICE07_BOMB,				//	"自分スペカ",		/*[h]	[s(?)]		音									[かしゅるしゃしゃん] */
//
	VOICE08_DEATH,				//	"ザコ倒した",		/* i				サコ死に音専用						[どすーーー] */
	VOICE09_GRAZE,				//	"グレイズ", 		/* j				グレイズ音							[か] */
	VOICE15_COUNT_TIMER,		//	"時間制限", 		/* p :				時間制限カウント音					[へにょん] */
	VOICE11_BOSS_KIRARIN,		//	"キラリン☆",		/* l				きらりん音 */
//
	VOICE10_MAGICF_E_SHOT,		//	"ボスショット1",	/* k				音									[ふわ] */
	VOICE12_SAKUYA_B2_SHOT, 	//	"ボスショット2",	/* m				咲夜 音 							[ぱあん] */
	VOICE13_SAKUYA_SHOT02,		//	"ボスショット3",	/* n				咲夜じゅわショット音				[ちゅるちゅるちゅる] */
	VOICE14_BOSS_KOUGEKI_01,	//	"ボスショット4",	/*[o];	[s(?)]		ボス攻撃音? 						[ばあん] */
	VOICE16_BOSS_KYUPIN,		//	"ボスショット5",	/* l				キュピーン音 */
//
	VOICE17_BOSS_TAME01,		//	"ボス溜め1",		/* r				うぃーん音 */
	VOICE16_MAX_FILES,
};
	/* 本家に低音系の効果音は無い＝＝合わないので廃止。 咲夜 音 [どどどどど] */
enum
{
	TRACK00_BULLETS = 0,
	TRACK01_EXPLODE,
	TRACK02_ALEART_IVENT,
	TRACK03_SHORT_MUSIC,
	TRACK04_TEKIDAN,
	TRACK05_ZAKO_DEATH,
	TRACK06_ALEART_IVENT_02,
	TRACK07_GRAZE
};

extern void init_audio(void);
extern void exit_audio(void);

extern void set_voice_volume(int volume);
extern void set_music_volume(int volume);

//extern void stop_music(void);/*廃止*/
//#define stop_music(aaa)	play_music_num(BGM_00_stop)
extern void fadeout_music(void);
extern void pause_music(void);
extern void resume_music(void);

extern void play_music_num(int num);

extern void play_voice_auto_track(int req_idx);
extern void bullet_play_04_auto(int req_idx);

extern void voice_play_vbl(void);
extern void voice_play(int req, int play_track );

/* 効果音の音響設計を行う(トラック割り振り) */
#define USE_DESIGN_TRACK	(1)

#endif /* _BGM_VOICE_H_ */
