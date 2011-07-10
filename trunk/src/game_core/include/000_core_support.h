
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���̃t�@�C���͒��ڃC���N���[�h���܂���B
	"game_main.h" ����̂݊ԐړI�ɃC���N���[�h���܂��B
---------------------------------------------------------*/

#ifndef _BGM_VOICE_H_
#define _BGM_VOICE_H_

//;�u���y�f�ށv�K�v�ȃt�@�C��

enum	//; ���ʉ�(Voice)
{
	VOICE00_SHOT = 0,			//	"�V���b�g", 		/* a				�V���b�g��							[���肿�肿��] */
	VOICE01_MENU_OK,			//	"���j���[����", 	/*[b]	[s(?)]		��									[�т傢�[] */
	VOICE02_MENU_SELECT,		//	"���j���[�I��", 	/* c				�T�R���ɉ� + �e������ + ���j���[	[����] */
	VOICE03_BOSS_HAKAI, 		//	"�{�X�ޏ�", 		/*[d];	[s(?)]		�{�X���ɉ�? 						[�ڂ��[�[��] */
	VOICE04_SHIP_HAKAI, 		//	"�s�`���[��",		/* e :				���@�j��							[�҂���[��] */
	VOICE05_BONUS,				//	"�A�C�e���擾", 	/* f				�A�C�e���擾��						[����] */
	VOICE06_EXTEND, 			//	"������",			/* g :				�c�@������							[�т������] */
	VOICE07_BOMB,				//	"�����J�[�h",		/*[h]	[s(?)]		��									[������邵�Ⴕ���] */
//
	VOICE08_DEATH,				//	"�U�R�|����",		/* i				�T�R���ɉ���p						[�ǂ��[�[�[] */
	VOICE09_GRAZE,				//	"�O���C�Y", 		/* j				�O���C�Y��							[��] */
	VOICE10_COUNT_TIMER,		//	"���Ԑ���", 		/* p :				���Ԑ����J�E���g��					[�ւɂ��] */
	VOICE11_BOSS_KIRARIN,		//	"�L��������",		/* l				������ */
//
	VOICE12_MAGICF_E_SHOT,		//	"�{�X�V���b�g1",	/* k				��									[�ӂ�] */
	VOICE13_SAKUYA_B2_SHOT, 	//	"�{�X�V���b�g2",	/* m				��� �� 							[�ς���] */
	VOICE14_SAKUYA_SHOT02,		//	"�{�X�V���b�g3",	/* n				��邶���V���b�g��				[����邿��邿���] */
	VOICE15_BOSS_KOUGEKI_01,	//	"�{�X�V���b�g4",	/*[o];	[s(?)]		�{�X�U����? 						[�΂���] */
	VOICE16_BOSS_KYUPIN,		//	"�{�X�V���b�g5",	/* l				�L���s�[���� */
//
	VOICE17_BOSS_TAME01,		//	"�{�X����1",		/* r				�����[�� */
	VOICE18_MAX_FILES,
};
	/* �{�Ƃɒቹ�n�̌��ʉ��͖�����������Ȃ��̂Ŕp�~�B ��� �� [�ǂǂǂǂ�] */


enum	//; ��(Music) [.it �� .ogg�`��]
{
	BGM_00_stop = 0,	/* ----- BGM ���t��~�R�}���h */
	BGM_01_stage1,		/* 1�ʓ��� */
	BGM_02_stage2,		/* 2�ʓ��� */
	BGM_03_stage3,		/* 3�ʓ��� */
	BGM_04_stage4,		/* 4�ʓ��� */
	BGM_05_stage5,		/* 5�ʓ��� */
	BGM_06_stage6,		/* 6�ʓ��� */
	BGM_07_stage7,		/* �G���f�B���O(����) */
	BGM_08_stage8,		/* �G�L�X�g���X�e�[�W���� */
	BGM_09_stage9,		/* �t�@���^�Y���X�e�[�W���� */
	BGM_10_menu04,		/* ��C�g���� */
	BGM_11_boss01,		/* 1�ʃ{�X (�A���X) */
	BGM_12_boss02,		/* 2�ʃ{�X (����) */
	BGM_13_boss03,		/* 3�ʃ{�X (�P��) */
	BGM_14_boss04,		/* 4�ʃ{�X (��) */
	BGM_15_boss05,		/* 5�ʃ{�X (�p�`�F) */
	BGM_16_boss06,		/* 6�ʃ{�X (���) */
	BGM_17_boss07,		/* �`���m �G�L�X�g���X�e�[�W1�{�X */
	BGM_18_boss08,		/* �A���X �G�L�X�g���X�e�[�W2�{�X */
	BGM_19_boss09,		/* ������ �G�L�X�g���X�e�[�W3�{�X */
	BGM_20_boss10,		/* �d�� �B���{�X */
	BGM_21_boss11,		/* ���~���A �B���{�X */
	BGM_22_boss00,		/* �t���� �B���{�X */
	BGM_23_boss12,		/* �얲 */
	BGM_24_boss13,		/* ���[�~�A */
	BGM_24_menu03,		/* �G���f�B���O(�V�i���I) */
	BGM_25_menu02,		/* �L�[�R���t�B�O */
	BGM_26_menu01,		/* �^�C�g����� */
	BGM_27_stage10,		/* ���g�p�Ȃł��B */
/* ----- �Ȃ̐�(�ǂݍ��݃t�@�C����) */
	USE_28_MUSIC_FILES
};

enum
{
	TRACK00_BULLETS = 0,
	TRACK01_EXPLODE,
	TRACK02_ALEART_IVENT,
	TRACK03_SHORT_MUSIC,
	TRACK04_TEKIDAN,
	TRACK05_ZAKO_DEATH,
	TRACK06_ALEART_IVENT_02,
	TRACK07_GRAZE_AUDIO_DRIVER_INTERNAL/*(voice_play_graze();�ȊO�ŁA�O������g�p���Ă�����ɖ�Ȃ�)*/
};

extern void init_audio(void);
extern void exit_audio(void);

extern void set_voice_volume(int volume);
extern void set_music_volume(int volume);

//extern void stop_music(void);/*(�p�~)*/
//#define stop_music(aaa)	play_music_num(BGM_00_stop)
extern void fadeout_music(void);
extern void pause_music(void);
extern void resume_music(void);

extern void play_music_num(int num);

extern void play_voice_auto_track(int req_idx);
extern void bullet_play_04_auto(int req_idx);
extern void voice_play_graze(void); 	/* �O���C�Y���Ɋւ��ẮA��p�̔����������K�v�B */


extern void voice_play_vbl(void);
extern void voice_play(int req, int play_track );

/* ���ʉ��̉����݌v���s��(�g���b�N����U��) */
#define USE_DESIGN_TRACK	(1)

#endif /* _BGM_VOICE_H_ */





/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���̃t�@�C���͒��ڃC���N���[�h���܂���B
	"game_main.h" ����̂݊ԐړI�ɃC���N���[�h���܂��B
---------------------------------------------------------*/
// "000_my_math.h" ����̕K�v����B

#ifndef _GAME_FILE_H_
#define _GAME_FILE_H_

/*---------------------------------------------------------
	�f�B���N�g������
	-------------------------------------------------------
	�f�B���N�g�����͕̂ς��\�������邩�� game_main.h �Œ�`
---------------------------------------------------------*/

#define DIRECTRY_NAME_DATA_STR			"data"
/* 'data' �̕������� 4 ���� */
#define DIRECTRY_NAME_DATA_LENGTH		(4)

#define DIRECTRY_NAME_KAIWA_STR 		"/kaiwa/"
/* '/kaiwa/' �̕������� 7 ���� */
#define DIRECTRY_NAME_KAIWA_LENGTH		(7)

#define DIRECTRY_NAME_KAKUCHOUSI_TEXT_STR			".txt"

/*
����:
/fonts/ �̏ꍇ�A/src/core/font.c �̂݁B
	"font01.png"
	"font03.png"
	"font05.png"
/fonts/ �̏ꍇ�A/src/core/draw/draw_kanji.c �̂݁B
	"font_bg16.png"
/fonts/ �̏ꍇ�A/src/core/draw/score_panel.c �̂݁B
	"panel_base.png"
	"hosi_gauge.png"
/douchuu/ �̏ꍇ�A/src/core/load_stage.c �̂݁B
/kaiwa/ �̏ꍇ�A/src/core/menu/kaiwa.c �̂݁B

*/
/*---------------------------------------------------------
	�X�R�A�t�H�[�}�b�g
---------------------------------------------------------*/

typedef struct
{
	char name[16/*4*/];
//
	u32 score;
	int player;
	int final_stage;	/* ���B�X�e�[�W */
	int difficulty; 	/* ��Փx */
//
	int use_continue;	/* �R���e�B�j���[�� */
	int count_miss; 	/* �~�X�� */
	int used_bomber;	/* �{���g�p�� */
	int count_bonus;	/* �J�[�h�{�[�i�X�� */
} SCORE_FORMAT;

#define MAX_8_SAVE_PLAYERS	(8)
#define MAX_5_RANKING		(5)

extern SCORE_FORMAT high_score_table[MAX_8_SAVE_PLAYERS][MAX_5_RANKING];

/*---------------------------------------------------------
	�I�v�V�������j���[�Őݒ肷��ݒ�l
---------------------------------------------------------*/

enum /*_option_config_*/
{
	OPTION_CONFIG_00_PLAYER = 0,
	OPTION_CONFIG_01_BOMB,
	OPTION_CONFIG_02_BGM,
	OPTION_CONFIG_03_SOUND,
	OPTION_CONFIG_04_CURRENT_DIFFICULTY,
	OPTION_CONFIG_05_CURRENT_PLAYER,
	OPTION_CONFIG_06_ANALOG,
	OPTION_CONFIG_07_OPEN,
	OPTION_CONFIG_08_MAX	/* �ő吔 */
};

extern int option_config[OPTION_CONFIG_08_MAX]; 	/* 8 */ 	// ����=�{�^���ϐ�

/*---------------------------------------------------------
	�R���t�B�O
---------------------------------------------------------*/

extern u32 pad_config[KINOU_08_WARIATE_MAX];
//extern int ke yconfig[KINOU_08_WARIATE_MAX];

/*---------------------------------------------------------
	�w�i�ǂ݂���
---------------------------------------------------------*/

enum /*_bg_resource_*/
{
	BG_TYPE_00_title_bg = 0,
	BG_TYPE_01_name_regist,
	BG_TYPE_02_key_config,
	BG_TYPE_03_music_room,
	BG_TYPE_04_loading, 		/*(�@�r�̉摜)*/
	BG_TYPE_99_MAX_HAIKEI,
};
extern void load_SDL_bg(int bg_type_number);


#endif /* _GAME_FILE_H_ */








/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���̃t�@�C���͒��ڃC���N���[�h���܂���B
	"game_main.h" ����̂݊ԐړI�ɃC���N���[�h���܂��B
---------------------------------------------------------*/

#ifndef _SDL_USE_FONT_H_
#define _SDL_USE_FONT_H_

/*---------------------------------------------------------
	�t�H���g
---------------------------------------------------------*/

enum FONTS
{
	FONT10W = 0,		/*	8x10 x WHITE �� (mini) */
	FONT16B,			/* 16x16 x �����̂a�f�p  */
	FONT16K,			/* 16x16 x �L�[�R���t�B�O�p */
	FONT16W,			/* 16x16 x WHITE �� */
	FONT_MAX
};
/*(-r34�݊��p)*/	/* 16x16 x RED   �g(�p�~) */
#define FONT16R 	FONT16W

extern void font_init(void);

/* �T�[�t�F�C�X�����Ȃ��ŁA���ډ�ʂɕ\�� */
extern void font88_print_screen_xy(void);//	char *text, int font_type, int x, int y);

/* [�p�~�\��]�V�K�T�[�t�F�C�X���쐬���A������������_�����O */
extern SDL_Surface *font_render_new_surface(void);//	char *text, int font_type);

/* */
extern void font_print(void);// 			char *text, int font_type, int x, int y);

#define MY_FONT_256_TEXT_LENGTH 		(256)
extern char my_font_text[MY_FONT_256_TEXT_LENGTH];

#endif /* _SDL_USE_FONT_H_ */
