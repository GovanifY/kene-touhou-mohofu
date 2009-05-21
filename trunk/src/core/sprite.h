#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "support.h"

/* Sprites */
//#define MAX_SPRITE 200

enum
{
	BASE_PLAZMA_PNG = 0,
	BASE_PLAZMA_PNG_ma,
	BASE_PLAZMA_PNG_oz,
	BASE_PLAZMA_PNG_ci,
	BASE_PLAZMA_PNG_yu,
//
	BASE_NEEDLE_PNG,
	BASE_NEEDLE_PNG_ma,
	BASE_NEEDLE_PNG_oz,
	BASE_NEEDLE_PNG_ci,
	BASE_NEEDLE_PNG_yu,
//
	BASE_TSHOOT_PNG,
	BASE_TSHOOT_PNG_ma,
	BASE_TSHOOT_PNG_oz,
	BASE_TSHOOT_PNG_ci,
	BASE_TSHOOT_PNG_yu,
//
	BASE_SHIP_MED_PNG,
	BASE_SHIP_MED_PNG_ma,
	BASE_SHIP_MED_PNG_oz,
	BASE_SHIP_MED_PNG_ci,
	BASE_SHIP_MED_PNG_yu,
//
	BASE_CORE_PNG,
	BASE_CORE_PNG_ma,
	BASE_CORE_PNG_oz,
	BASE_CORE_PNG_ci,
	BASE_CORE_PNG_yu,
//
	BASE_OPTION1_PNG,
	BASE_OPTION1_PNG_ma,
	BASE_OPTION1_PNG_oz,
	BASE_OPTION1_PNG_ci,
	BASE_OPTION1_PNG_yu,
//
	BASE_OPTION2_PNG,
	BASE_OPTION2_PNG_ma,
	BASE_OPTION2_PNG_oz,
	BASE_OPTION2_PNG_ci,
	BASE_OPTION2_PNG_yu,
//
	BASE_OPTION3_PNG,
	BASE_OPTION3_PNG_ma,
	BASE_OPTION3_PNG_oz,
	BASE_OPTION3_PNG_ci,
	BASE_OPTION3_PNG_yu,
//
	BASE_OPTION4_PNG,
	BASE_OPTION4_PNG_ma,
	BASE_OPTION4_PNG_oz,
	BASE_OPTION4_PNG_ci,
	BASE_OPTION4_PNG_yu,
//
//
	BASE_BOMBER1_PNG,
	BASE_BOMBER1_PNG_ma,
	BASE_BOMBER1_PNG_oz,
	BASE_BOMBER1_PNG_ci,
	BASE_BOMBER1_PNG_yu,
//
	BASE_BOMBER2_PNG,
	BASE_BOMBER2_PNG_ma,
	BASE_BOMBER2_PNG_oz,
	BASE_BOMBER2_PNG_ci,
	BASE_BOMBER2_PNG_yu,
//
	BASE_BOMBER3_PNG,
	BASE_BOMBER3_PNG_ma,
	BASE_BOMBER3_PNG_oz,
	BASE_BOMBER3_PNG_ci,
	BASE_BOMBER3_PNG_yu,
//
	BASE_BOMBER4_PNG,
	BASE_BOMBER4_PNG_ma,
	BASE_BOMBER4_PNG_oz,
	BASE_BOMBER4_PNG_ci,
	BASE_BOMBER4_PNG_yu,
//
	BASE_BOMBER5_PNG,
	BASE_BOMBER5_PNG_ma,
	BASE_BOMBER5_PNG_oz,
	BASE_BOMBER5_PNG_ci,
	BASE_BOMBER5_PNG_yu,
//
	BASE_BOMBER6_PNG,
	BASE_BOMBER6_PNG_ma,
	BASE_BOMBER6_PNG_oz,
	BASE_BOMBER6_PNG_ci,
	BASE_BOMBER6_PNG_yu,
//
	BASE_BOMBER_SLOW_PNG,
	BASE_BONUS_ITEMS_PNG,
//
	//BASE_TAMA_KUGEL_PNG,
	//BASE_TAMA_KUGEL_MINI2_PNG,
	BASE_TAMA_BULLET_BEAM16_PNG,
	BASE_TAMA_BULLET_MARU16_PNG,
//
	BASE_TAMA_BULLET_MING32_PNG,
	BASE_TAMA_BULLET_JIPPOU32_PNG,
//
	BASE_TAMA_OODAMA_08_PNG,/* ���(����...)  ����(��) PRIORITY_03_ENEMY �́A�����蔻�蕔�� */
//	BASE_TAMA_OODAMA_00_PNG,/* ����(��)  �����蔻�蕔��*/
//	BASE_TAMA_OODAMA_01_PNG,/* ���(��) �\������ */
//	BASE_TAMA_OODAMA_02_PNG,/* ���(��) �\������*/
//
	BASE_TAMA_BULLET_KNIFE01_PNG,/*�����~���i�C�t(��)*/
	BASE_TAMA_BULLET_KNIFE18_PNG,/*�S�����i�C�t(��)*/
//
	BASE_MAHOUJIN_0_PNG,
	BASE_TIKEI_BGPANEL1_PNG,
	BASE_TIKEI_BGPANEL2_PNG,
	BASE_TIKEI_GROUNDER08_PNG,
//
	BASE_OBAKE08_PNG,
	BASE_YUKARI32_PNG,
	BASE_AKA_KEDAMA08_PNG,
	BASE_NIJI_KEDAMA16_PNG,
//
	BASE_MIDOORI_KEDAMA16_PNG,
	BASE_KEDAMA16_PNG,
	BASE_INYOU1_16_PNG,
	BASE_TATSUMAKI16_PNG,
//
	BASE_GREAT_FAIRY02_PNG,
	BASE_AKA_MEIDO08_PNG,
	BASE_AO_YOUSEI24_PNG,
	BASE_HOMING16_PNG,
//	/* ������ */
	BASE_TR_BLUE_PNG,
	BASE_TR_RED_PNG,
	BASE_TR_GREEN_PNG,
//	/* �U�R���Ŕ��� */
	BASE_BAKUHA05_PNG,
	BASE_BAKUHA06_PNG,
	BASE_BAKUHA07_PNG,
//	/* �Ή����� */
	BASE_BAKUHA_EX_PNG,
//
	BASE_BOSS_ALICE_0_PNG,
	BASE_BOSS_ALICE_1_PNG,
	BASE_BOSS_ALICE_2_PNG,
	BASE_BOSS_ALICE_3_PNG,
	BASE_BOSS_ALICE_4_PNG,
//
	BASE_BOSS_AYA_PNG,
//
	BASE_BOSS_KAGUYA_0_PNG,
	BASE_BOSS_KAGUYA_1_PNG,
	BASE_BOSS_KAGUYA_2_PNG,
	BASE_BOSS_KAGUYA_3_PNG,
	BASE_BOSS_KAGUYA_4_PNG,
	BASE_BOSS_KAGUYA_5_PNG,
//
	BASE_BOSS_SAKUYA_PNG,
//
	//BASE_KEY_ICON_PNG,
//
	FILE_RESOURCE_MAX/*�ő吔*/
};
// �e���ɂ���
// �e��(�G�e�̂ݎ��e�͊܂܂Ȃ�)��Gu�ŕ`�������v�撆(�Ƃ������ϑz��)�B
// ���̏ꍇ�ASDL��ʕ`����Gu�ŕ`���̂ŁA
// �e���̃v���C�I���e�B�[�͍őO�ʂɂȂ�B
//
// �g�������Q�l�Ɍ������A���@�����A�C�e������
// �e���͑O(�܂肠�ƂœZ�߂ĕ`�悵�Ă���)������
//
//���������͈Ⴄ��������Ȃ����A�����{�Ƃ͂���Ȋ���(�z��)
// [�w�i�֘A]
// �P�D���z���(448x448)��3D�w�i�������_�����O(��ݏo���`��)
// [���@�֘A]
// �Q�D���z���(448x448)�Ɏ��e�������_�����O(��ݏo���`��)					(���e�͎��@��艺)
// �R�D���z���(448x448)�Ɏ��@�I�v�V�����������_�����O(��ݏo���`��)		(���@�^���@�I�v�V�����̓{�X�^�G��艺)
// �S�D���z���(448x448)�Ɏ��@�������_�����O(��ݏo���`��)					(���@�^���@�I�v�V�����̓{�X�^�G��艺)
// [�G�֘A]
// �T�D���z���(448x448)�ɓG��{�X�������_�����O(��ݏo���`��)
// [�A�C�e���֘A]
// �U�D���z���(448x448)�ɃA�C�e���������_�����O(��ݏo���`��)				(�A�C�e���͓G�e��艺/�A�C�e���͓G����)
// �X�D���z���(448x448)�Ɋ����X�R�A�Ȃǂ������_�����O(��ݏo���`��)		(�����X�R�A�͓G�e��艺(???))
// [�G�e�֘A]
// �V�D���z���(448x448)�Ƀ��[�U�[�����C���`��������_�����O(��ݏo���`��)
// �W�D���z���(448x448)�ɒe���������_�����O(��ݏo���`��)
// [�C�x���g�֘A]
// �X�D���z���(448x448)�ɕ���{�X���[�^�Ȃǂ������_�����O(��ݏo���`��)
// �P�O�D���z���(448x448)�ɃC�x���g�摜�������_�����O(��ݏo���`��)
// �P�P�D���z���(448x448)�ɃC�x���g�����������_�����O(��ݏo���`��)
// �P�Q�D���z���(448x448)�̈ꕔ(384x448)����`�ɐ؂�o���āA�{���(640x480)�ɓ]������B
/*	���F384==12x32�A448==14x32�Ȃ̂œs���悭�����]���\�B */
/*	���F�����̓]���Ŏ����I�ɃN���b�s���O�������ł���B */
// �P�R�D���_�Ȃǂ̕\���͎n�߂���{���(640x480)�ɕ`�悵���z���(448x448)�͎g��Ȃ��B
// �P�S�D�{���(640x480)��VRAM�ɓ]��(���Ԃ�)�B
/*	���F���z���(448x448)�͑������z���(512x512)�B������̕����F�X�������ȗ��ł��āA�s�����悢�B */
//�����������Ƃ������Đ��@�D�܂ň�т��ăQ�[����ʂ�(384x448)����ˁH

/*New r23*/
enum /*_priority_*/
{
// [�w�i�֘A]
	PRIORITY_00_BG = 0, 			//	P R_BACK0 = 0,
	PRIORITY_01_SHOT,				//	P RIORITY_01_GROUNDER,		/*���e/�n��U�R�G/�����w*/
//	P R_BACK1,
//	P R_BACK2,
// [���@�֘A]
	PRIORITY_02_PLAYER, 	/* ���@ */
// [�G�֘A]
	PRIORITY_03_ENEMY,		/* �{�X/�U�R�G */
// [�A�C�e���֘A]
	PRIORITY_04_ITEM,		/* �A�C�e��/�����X�R�A */	//P R_PLAYER2,			/*������\��*/
// [�G�e�֘A]
	PRIORITY_05_BULLETS/*PRIORITY_03_ENEMY_WEAPON*/,		/*�G�e*/
//
//	P R_TARGET,
//	P R_TMP,
// [�e�L�X�g�\��]
//	PR IORITY_06_TEXT,		/*�e�L�X�g�\��*/
//	P R_FRONT0,
//	P R_FRONT1,
//	P R_FRONT2
};

#define USER_BOMOUT_WAIT (30)

/* �g�p�ς݂���������Ă���t���O */
#define SP_DELETE				(0)
/*	0 �Ȃ�Δ��ʏ������ȗ��ł���B -1 ���ƈ�X���ʏ���������
	KETM�ł͔��ʂ��Ȃ��ƂȂ�Ȃ��ꏊ�ɔ��ʏ����������ĂȂ��ׁA
	�X�v���C�g�����������Ȃ�o�O�����������ɂ���B */

#define SP_GROUP_PLAYER 		0x0100
#define SP_GROUP_SHOTS			0x0200
#define SP_GROUP_ENEMYS 		0x0400
#define SP_GROUP_BULLETS		0x0800
#define SP_GROUP_ITEMS			0x1000
#define SP_GROUP_TEXTS			0x2000
#define SP_GROUP_ETC			0x4000
#define SP_GROUP_ALL			0xff00


enum SPRITE_TYPE
{
/*
	�����͎�ޕʂł͂Ȃ��āA�@�\�ʂɕ��ނ��Ă��������B
*/
//	����
	SP_PLAYER/*�v���C���[�̖{��(�����蔻�肪�A�C�e���擾)*/ 	= /*0x0100*/SP_GROUP_PLAYER,	/* Unser Held */
	SP_PLAYER2,/*�v���C���[�̃R�A(�����蔻�肪���S����)*/
//	���e
	SP_SHOT_BOSS/*�v���C���[�̃{�X�ɗL���e*/ = /*0x0200*/SP_GROUP_SHOTS,	/* Waffen des Helden */
	SP_SHOT_ZAKO,/*�v���C���[�̃{�X�ɖ����e*/
//	�G
	SP_ZAKO 					= /*0x0400*/SP_GROUP_ENEMYS, /* Die Boesen */
	SP_CHUU,	/*����G[���^�G]*/ /*SP_ZAKO_YOKAI1*/
	SP_BOSS,	// [***090325
//	�G�e
	SP_BULLET					= /*0x0800*/SP_GROUP_BULLETS,		/* ... und ihre Waffen */
//	�A�C�e��
	SP_ITEM_00_P001 	= /*0x1000*/SP_GROUP_ITEMS, 	/* Bonus items */
	SP_ITEM_01_P008,	//���E�F�|���A�C�e��(��)	//�E�F�|���A�C�e��(��)��SP_ITEM_EXTRA_SHIELD,
	SP_ITEM_02_BOMB,			//�������͒ᑬ�{���ɋz�����ꂽ�BSP_ITEM_EXTRA_HOMING, SP_ITEM_EXTRA_HLASER,
	SP_ITEM_03_1UP,
	SP_ITEM_04_P128,
	SP_ITEM_05_HOSI,
	SP_ITEM_06_TENSU,
	SP_ITEM_07_SPECIAL,
//	����
	SP_MENU_TEXT				= /*0x2000*/SP_GROUP_TEXTS,
//	���̑�
	SP_ETC						= /*0x4000*/SP_GROUP_ETC,		/* diverses */
};
//#define SP_LASER			SP_BULLET
//#define SP_BOSS02ICE		SP_BULLET
//#define SP_BIGBULLET	SP_BULLET

//	SP_ZAKO //	SP_ENEMY_HOMING,/*����G[�G���̃z�[�~���O�e]*/
//	SP_ZAKO //	SP_TIKEI,	//SP_GROUNDER,SP_BGPANEL,

//	SP_BOSS01,
//	SP_BOSS02,
//	SP_BOSS03,
//	SP_BOSS04,

//	SP_LASER,
//	SP_BOSS02ICE,
//	SP_BIGBULLET,
//	SP_BIGBULLET_DUMMY == SP_ETC,

//	SP_PL_FIREBALL,
//	SP_PL_KILLRAY,
//	SP_PL_BOMB,
//	SP_PL_HOMING,
//	SP_PL_HLASER,
	#if 0
	/* ���̕ӕ�����K�v���������� (���ӁG�����̏����͒��ԃR�[�h�`���Ɠ����ɍ��킹��\��) */
	//	���̑�
	SP_ZAKO_01_OBAKE	= /*0x0400*/SP_GROUP_ENEMYS, /* Die Boesen */
	SP_ZAKO_02_YUKARI1,
	SP_ZAKO_03_YUKARI2,
	//	�ы�
	SP_ZAKO_04_AKA_KEDAMA1,
	SP_ZAKO_05_NIJI_KEDAMA1,
	SP_ZAKO_06_MIDORI_KEDAMA1,
	SP_ZAKO_07_MIDORI_KEDAMA2,
	SP_ZAKO_08_KEDAMA1,
	//	���� �A�z��
	SP_ZAKO_09_INYOU1,
	SP_ZAKO_10_TATSUMAKI1,
	//	�d��
	SP_ZAKO_11_KAKOMI1,
	//	���d��
	SP_ZAKO_12_MEIDO1,
	SP_ZAKO_13_MEIDO2,
	SP_ZAKO_14_MEIDO3,
	SP_ZAKO_15_MEIDO4,
	//	���d��
	SP_ZAKO_16_AO_YOUSEI1,
	SP_ZAKO_17_AO_YOUSEI2,
	/* ���̕ӕ�����K�v���������� (���ӁG�����̏����͒��ԃR�[�h�`���Ɠ����ɍ��킹��\��) */
	#endif



typedef struct _colision_map_cache
{
	SDL_Surface *src;	/* Zeiger auf Quell-Src in Pixmap cache (nur fuer Index, *NO* free!) */
	int frames; 		/* Anzahl eintraege in dyn.Array **col */
	int refcount;		/* Wieviele Sprites nutzen diese Colision map? */
	Uint8 **col;		/* Dyn.Array mit Zeiger auf Colision maps */
	struct _colision_map_cache *next;
} COLISION_MAP_CACHE;

/* �j�e�Ƃ����邩��A���ݐ}�`���肵�Ȃ��ƍ��\����ɂȂ�B */
/* �����蔻�薳�����Ă����x�͕ς��Ȃ������������ߖ�ł���̂ŏ����I�ɂ͂��̕ӂ����ǂ������B */
#define USE_ZUKEI_ATARI_HANTEI (1)
typedef struct _sprite
{
	int type;						/* ��ދy�юg�p�� / Sprite-Type, (siehe enum SPRITE_TYPE), 0 = remove */
	SDL_Surface *sprite_bmp;				/* �摜 / Images */
	#if (1==USE_ZUKEI_ATARI_HANTEI)
	COLISION_MAP_CACHE *colision_bmp;		/* �����蔻��p�摜 / Zeiger auf Col-Map-Cache entry */
	#endif /* (1==USE_ZUKEI_ATARI_HANTEI) */
	int timeover_ticks; 			/* �쐬���Ă���̌o�ߎ��� (����KETM���̂Ƀo�O������ׁA��莞�Ԍo�߂���Ƌ�����������) / number of ticks since creation */
//[4]
	int x256;						/* x�\���ʒu (256�Œ菬���_�`��) / akt. Position */ 	/*dou ble*/
	int y256;						/* y�\���ʒu (256�Œ菬���_�`��) / akt. Position */ 	/*dou ble*/
	int w;							/* ���� (�����`��) / Breite, H�he */
	int h;							/* ���� (�����`��) / Breite, H�he */
//[8]
	void *data; 					/* Zeiger auf Strukur mit zus�tzlichen Daten */
	void (*callback_mover)(struct _sprite *s);/* Custom-Move-Routine */
	void (*callback_loser)(struct _sprite *s);/* Custom-loser-Routine (���ꂽ��Ƀ{�[�i�X���o���Ƃ��A�Ⴄ��������Ƃ�) */
	struct _sprite *next;
//[12]
	Uint8 flags;					/* �t���O�Z�b�g / siehe unten (SP_FLAG...) */
	Uint8 priority; 				/* �\���D�揇�� / Prioritaet (0: unten, 9 oben) */
	Uint8 alpha;					/* �����x	 / 255: opak, -  0: durchsichtig */
	Uint8 alpha_chache; 			/* �O��̃A���t�@�l */
//
	Uint8 anim_frame;				/* �A�j���p�^�[���̔ԍ�(���R�}�ڂ�) */	 /*int*/			/* akt. Frame */
	Uint8 frames;					/* �\���p�^�[������ / Anzahl Frames */
	Uint8 anim_count;				/*int*/ /*dou ble*/ 	// Sint8 anim_count;			/* intern: Zaehler f. anim_speed */
	Uint8 anim_speed/* 256 */;		/*�t�]�A�j���֎~�ɕύX*/		/* �A�j���[�V�������x / Geschw. der Animation (negative Werte: Anim l�uft r�ckw�rts */	 /*Sint8*/ /*Uint8*/
//[16]
} SPRITE;


#define SP_FLAG_COLISION_CHECK	0x01	/* Col-Check aktiv */
#define SP_FLAG_VISIBLE 		0x02	/* Sprite sichtbar */
#define SP_FLAG_NOT_CACHE		0x04	/* Image nicht im Cache */
#define SP_FLAG_FREE_SURFACE	0x08	/* Surface loeschen */
#define SP_FLAG_GRAZE			0x10	/* �O���C�Y�ς݂��Ƃ����̃t���O */
//#define SP_FLAG_CHEAP_ALPHA	0x10	/* "Cheapalpha", nur jedes 2te Pixel setzen */
#define SP_FLAG_LOCK_TARGET 	0x20	/* SIE sind hinter mir her */

#define SP_FLAG_TIME_OVER		0x80	/* ��莞�Ԃł̎��������A���t���O(ON�Ȃ玩����������) */


//extern SPRITE *sprite_add_file 0(char *filename, int frames, Uint8 priority, int use_alpha);
extern SPRITE *sprite_add_file(char *filename, int frames, Uint8 priority);
extern SPRITE *sprite_add_file2(char *filename, int frames, Uint8 priority);
extern SPRITE *sprite_add_res(int image_resource_num);

extern SPRITE *sprite_add_000(SDL_Surface *surface, int frames, Uint8 priority, int set_flags/*nocache*/, int anime_speed);

extern void sprite_remove_all000(int type);
extern void sprite_remove_all222(int type);/*�e���p*/

extern void sprite_display000(int type);/**/
extern void sprite_display222(int type);/*�e���p*/
extern void sprite_work000(int type);/**/
extern void sprite_work222(int type);/*�e���p*/

extern SPRITE *sprite_collision_check(SPRITE *tocheck, int type);
#endif
