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
	VOICE00_SHOT = 0,			/* �V���b�g��						[���肿�肿��] */
	VOICE01_HIT,				/* ��								[�т傢�[] */
	VOICE02_ZAKO_HAKAI, 		/* �T�R���ɉ� + �e������ + ���j���[ [����] */
	VOICE03_BOSS_HAKAI, 		/* �{�X���ɉ�?						[�ڂ��[�[��] */
	VOICE04_SHIP_HAKAI, 		/* ���@�j��						[�҂���[��] */
	VOICE05_BONUS,				/* �A�C�e���擾��					[����] */
	VOICE06_EXTEND, 			/* �c�@������						[�т������] */
	VOICE07_BOMB,				/* ��								[������邵�Ⴕ���] */
//
	VOICE08_DEATH,				/* �T�R���ɉ���p					[�ǂ��[�[�[] */
	VOICE09_GRAZE,				/* �O���C�Y��						[��] */
	VOICE10_MAGICF_E_SHOT,		/* �� [�ӂ�] */
	VOICE11_SAKUYA_E_SHOT00,	/* �� [�ǂǂǂǂ�] */
	VOICE12_SAKUYA_B2_SHOT, 	/* �� [�ς���] */
	VOICE13_SAKUYA_SHOT02,		/* �� [����邿��邿���] */
	VOICE14_YOKAI1_E_SHOT01,	/* �� [�΂���] */
	VOICE15_COUNT_TIMER,		/* ���Ԑ����J�E���g�� [] */
//
	VOICE16_MAX_FILES,
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
