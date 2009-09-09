#ifndef _BGM_VOICE_H_
#define _BGM_VOICE_H_

extern void init_audio(void);
extern void exit_audio(void);

extern void set_voice_volume(int volume);
extern void set_music_volume(int volume);

extern void stop_music(void);
extern void fadeout_music(void);
extern void pause_music(void);
extern void resume_music(void);

extern void play_music(int num);

extern void play_voice_auto_track(int req_idx);

extern void voice_play_vbl(void);
extern void voice_play(int req, int play_track );

enum
{
	BGM_00_intro = 0,
	BGM_01_stage1,
	BGM_02_stage2,
	BGM_03_stage3,
	BGM_04_stage4,
	BGM_05_stage5,
	BGM_06_stage6,
	BGM_07_stage7,
	BGM_08_menu2,
	BGM_09_boss1,
	BGM_10_boss2,
	BGM_11_boss3,
	BGM_12_boss4,
	BGM_13_boss5,
	BGM_14_boss6,
	BGM_15_boss7,
};

enum
{
	VOICE00_SHOT = 0,			/* ショット音 */
	VOICE01_HIT,				/* 音 */
	VOICE02_ZAKO_HAKAI, 		/* サコ死に音 + 弾消し音 + メニュー */
	VOICE03_BOSS_HAKAI, 		/* ボス死に音? */
	VOICE04_SHIP_HAKAI, 		/* 自機破壊音 */
	VOICE05_BONUS,				/* アイテム取得音 */
	VOICE06_EXTEND, 			/* 残機増加音 */
	VOICE07_BOMB,				/* 音 */
	VOICE08_DEATH,				/* サコ死に音専用 */
	VOICE09_GRAZE,				/* グレイズ音 */
	VOICE10_MAGICF_E_SHOT,		/* 音 */
	VOICE11_SAKUYA_E_SHOT00,	/* 音 */
	VOICE12_SAKUYA_B2_SHOT, 	/* 音 */
	VOICE13_SAKUYA_SHOT02,		/* 音 */
	VOICE14_YOKAI1_E_SHOT01,	/* 音 */
};

enum
{
	TRACK00_BULLETS = 0,
	TRACK01_EXPLODE,
	TRACK02_ALEART_IVENT,
	TRACK03_SHORT_MUSIC,
	TRACK04,
	TRACK05,
	TRACK06,
	TRACK07
};
#endif
