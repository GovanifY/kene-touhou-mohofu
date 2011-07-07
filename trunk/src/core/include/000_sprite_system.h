
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���̃t�@�C���͒��ڃC���N���[�h���܂���B
	"game_main.h" ����̂݊ԐړI�ɃC���N���[�h���܂��B
---------------------------------------------------------*/

#ifndef _SPRITE_H_
#define _SPRITE_H_

/*---------------------------------------------------------
	�X�v���C�g �}�l�[�W��
--------------------------------------------------------- */
/*
����:   s32 JYUMYOU
1[�b]                                   60           >                64 [flame]
1[��]                                   60 x 60      >           64 x 64 [flame]
1[����] 0x034BC0 ==   216000            60 x 60 x 60 >      64 x 64 x 64 [flame] ==   262144 0x040000
2.5[��] 0xC5C100 == 12960000 2.5 x 24 x 60 x 60 x 60 > 64 x 64 x 64 x 64 [flame] == 16777216 0x01000000
-------------------------------------------------
�����A���t���[�����Z����J�E���^������Ƃ��āA
signed int ���ƁA�����l�� 0x01ff ffff �������ꍇ�A2.5[��]�o�߂��Ă��A�J�E���^�̒l��0x01-- ----�B
�܂�ŏ��bit �͕����Ƃ��āA�ŏ��bit��������7bit�͎��R�Ɏg����B(�J�E���^���o���Ƃ�0x00ffffff�Ń}�X�N����Ƃ���)
*/

#define JYUMYOU_MUGEN	(16777216)
#define JYUMYOU_1MIN	byou60(60)
#define JYUMYOU_NASI	(0)
/*
JYUMYOU_MUGEN:	(��������==�����A��2.5[��])�v���C���[��I�v�V�������A�����ŏ����Ă͍�����́B
JYUMYOU_1MIN:	(�����A��1[��])
JYUMYOU_NASI:	(�����A�Ȃ��B�����ɂȂ�ƁA�����ŏ��������̂Œ��ӁB�u���t���[������=2�ɐݒ肵�Ƃ��Ώ����Ȃ��B(����=1���Ə������Ⴄ����)�v)

*/

/* �e�̐�������(�W��) */
#define JYUMYOU_TAMA_HASSEI 	(JYUMYOU_1MIN)
/* �U�R�̐�������(�W��) */
#define JYUMYOU_ZAKO_HASSEI 	(JYUMYOU_1MIN)
/* ��ʊO�Ȃ�U�R�����ꍇ�́A���~�b�g���� */
//#define JYUMYOU_ZAKO_CLIP 		(2048)				/* 2048/60 = 34.1333333333333333333333333333333[�b] */

/*
	�������Ԃ���_�Ƃ��������̏ꍇ�A(��ŃV�X�e�����ς�邩������Ȃ�����)
	��L(JYUMYOU_TAMA_HASSEI / JYUMYOU_ZAKO_HASSEI)����Ƃ��Ă����B
*/


//#define SP_FLAG_COLISION_CHECK	(0x01)				/* �����蔻�肠��t���O */
//#define SP_FLAG_GRAZE 			(0x10)				/* �O���C�Y�ς݂��Ƃ����̃t���O */
#define SP_FLAG_COLISION_CHECK		(0x8000)			/* �����蔻�肠��t���O */
#define SP_FLAG_GRAZE				(0x0080)			/* �O���C�Y�ς݂��Ƃ����̃t���O */
#define SP_FLAG_OPTION_VISIBLE		(SP_FLAG_GRAZE) 	/* �v���C���[�I�v�V�����L���t���O */


/* Sprites */
//#define MAX_SPRITE 200

#define USER_BOMOUT_WAIT (30)

/* �g�p�ς݂���������Ă���t���O */
//#define SP_DELETE 			(0)
/*	0 �Ȃ�Δ��ʏ������ȗ��ł���B -1 ���ƈ�X���ʏ���������
	KETM�ł͔��ʂ��Ȃ��ƂȂ�Ȃ��ꏊ�ɔ��ʏ����������ĂȂ��ׁA
	�X�v���C�g�����������Ȃ�o�O�����������ɂ���B */

/* �Ǘ��������Ⴄ�̂ŁA�P�Ȃ�OFF */
#define FIX_OBJ_OFF 			(SP_DELETE)

/*
	�����͎�ޕʂł͂Ȃ��āA�@�\�ʂɕ��ނ��Ă��������B
*/
#define SP_GROUP_SHOT_ZAKO					(0x0000)/*0x0100*/
#define SP_GROUP_SHOT_SPECIAL				(0x0080)/*0x0100*/		/* �얲�̉�]�V���b�g */		/* kodomo_hlaser�̔��� */
//
#define SP_GROUP_JIKI_GET_ITEM				(0x0100)/*0x0100*/		/* === Gu�v���C���[�� === */
//#define dummy_SP_GROUP_PLAYER_CORE		(0x0200)/*0x0100*/
#define SP_GROUP_TEKI						(0x0200)/*0x0400*/		/* === Gu�G�� === */
//#define SP_GROUP_BOSS 					(0x0800)/*0x0400*/		/* === Gu�G�� === */
//#define SP_GROUP_TEKI 					(0x0400)/*0x0400*/
//#define SP_GROUP_TEKI 					(SP_GROUP_TEKI|0)
//#define SP_GROUP_BOSS 					(SP_GROUP_TEKI|SP_GROUP_SHOT_SPECIAL)
#define SP_GROUP_BULLETS					(0x0400)/*(0x1000)0x0800*/		/* === Gu�e���� === */
#define SP_GROUP_ITEMS						(0x0800)/*(0x2000)0x1000*/
//#define SP_GROUP_PAUSE_S P_ME NU_TEXT 	(0x4000)/*0x2000*/		/*SDL*/
/*???*/#define SP_GROUP07_FRONT 				(0x1000)/*(0x8000)*/				/* === Gu�t�����g�� === */

//#define SP_GROUP_ALL_SDL_TYPE (0xff00)
#define SP_GROUP_ALL_SDL_WORK_TYPE			(0x1f00)/* SDL���� */
//#define SP_GROUP_ALL_SDL_CORE_TYPE		((0x1f00) & (~(SP_GROUP_PAUSE_S P_ME NU_TEXT|0)))/* game_core���� */
#define SP_GROUP_ALL_SDL_CORE_TYPE			(0x1f00)/* game_core���� */

//#define SP_GROUP_ALL_SDL_DRAW_TYPE		(SP_GROUP_ALL_SDL_WORK_TYPE & (~(SP_GROUP_TEKI|SP_GROUP_ITEMS|SP_GROUP_BULLETS)))/*SDL�\��*/
/* �G���A�C�e�����G�e�͏��Ȃ��Ƃ�SDL�ŕ`���Ȃ� */
//#define SP_GROUP_ALL_SDL_DRAW_TYPE		(0x4000)/* SDL�\��(����SP_GROUP_PAUSE_S P_ME NU_TEXT�̂�SDL�`��) */

//#define SP_GROUP_SUB_TYPE_256 			(0x00ff)
#define SP_GROUP_SUB_TYPE_128				(0x007f)
/* �ȉ������蔻��Ȃ� */
/*???*/ 	//#define SP_GROUP_MAHOU_JIN			(0x2000)/*(0x00010000)*/			/* === Gu�����w�� === */
/*???*/ 	//#define SP_GROUP_PANEL				(0x4000)/*(0x00020000)*/			/* === Gu�p�l���� === */
//#define SP_FLAG_COLISION_CHECK			(0x8000)/*(0x00020000)*/			/* === Gu�p�l���� === */

#define SP_GROUP_ALL_GAME_OBJS				(0x00ffff00)			/* �Q�[���R�A�I�����ɂ����Â���obj */
//#define SP_GROUP_SHOTS					(0x0200)
//#define SP_GROUP07_FRONT						(0x4000)
//#define SP_GROUP_ENEMYS					(0x0200)/*(0x0400)*/
/* ���p */
//#define SP_GROUP_TEXTS					(SP_GROUP_PLAYER)/*(0x2000)*/

/* SP_GROUP07_FRONT == SP_GROUP07_FRONT */
//#define SP_GROUP07_FRONT						(SP_GROUP07_FRONT)/* �t�����g�� */

		//���E�F�|���A�C�e��(��)	//�E�F�|���A�C�e��(��)��SP_ITEM_EXTRA_SHIELD,
		//�������͒ᑬ�{���ɋz�����ꂽ�BSP_ITEM_EXTRA_HOMING, SP_ITEM_EXTRA_HLASER,
/*	����A */
//	S P_PLAYER_ITEM/*�v���C���[�̖{��(�����蔻�肪�A�C�e���擾)*/			= /*0x0100*/SP_GROUP_JIKI_GET_ITEM, /* Unser Held */
/*	���e */
//	S P_SHOT_BOSS,/*�v���C���[�̃{�X�ɗL���e*/							//	= /*0x0200*/SP_GROUP_SHOTS, 	/* Waffen des Helden */
//	S P_SHOT_ZAKO,/*�v���C���[�̃{�X�ɖ����e*/
/*	����B */
	//dummy_SP_PLAYER_CORE/*�v���C���[�̃R�A(�����蔻�肪���S����)*/		= /*0x0100*/SP_GROUP_PLAYER_CORE,
/* ------------- */
/*	�U�R�G (�{�����A�̓�����Ńv���C���[���ȂȂ�) */
//	SP_ZAKO 																= /*0x0400*/SP_GROUP_TEKI,		/* Die Boesen */
/* ------------- */
/*	�{�X�G / ���U�R�G (�{�����A�̓�����Ńv���C���[����) */
//	S P_CHUU,// /*����G[���^�G]*/ /*S P_ZA KO_YOKAI1*/ 					= /*0x0400*/SP_GROUP_BOSS,		/* [***090325 */
//	S P_BO SS,
/* ------------- */
/*	�G�e */
	//dummy_SP_BULLET														= /*0x0800*/SP_GROUP_BULLETS,	/* ... und ihre Waffen */
/* ------------- */

//enum /*sprite_type*/
//{
/* ------------- */
/*	�|�[�Y���ړ��\�� */
//	S P_ME NU_TEXT	/* ���� */												= /*0x2000*/SP_GROUP_PAUSE_S P_ME NU_TEXT,
/*	���̑� */
//	SP_MUTEKI																= /*0x4000*/SP_GROUP07_FRONT,		/* diverses */
//};

//#define SP_MUTEKI 	SP_GROUP07_FRONT

/*---------------------------------------------------------
	�I�u�W�F�o���N�V�X�e��
--------------------------------------------------------- */

/* �Ǘ�����I�u�W�F�N�g�o���N */
enum
{
	OBJ_BANK_00_TAMA = 0,		// �G�e�p�o���N 		//	��1.�G�e/�A�C�e���p�e�N�X�`���͋��p
	OBJ_BANK_01_ITEM,			// �A�C�e���p�o���N 	//	��1.�G�e/�A�C�e���p�e�N�X�`���͋��p
	OBJ_BANK_02_FRONT_BANK0,	// �t�����g�ʗp�o���N	//	��2.�t�����g/�p�l���p�e�N�X�`���͋��p
//	OBJ_BANK_07_FRONT_BANK1,	// �p�o���N
//	OBJ_BANK_07_FRONT_BANK2,	// �p�o���N
	OBJ_BANK_03_TITLE_dummy,	/*[�\��]*/	// �^�C�g����ʗp�o���N //	��2.�t�����g/�p�l���p�e�N�X�`���͋��p
//	OBJ_BANK_09_EFFECT, 		/*[�\��]*/	// �p�o���N
//
	OBJ_BANK_01_REIMU_A,	// ���@�p�o���N(�얲A�A�아)
	OBJ_BANK_02_REIMU_B,	// ���@�p�o���N(�얲B�A����)
	OBJ_BANK_03_MARISA_A,	// ���@�p�o���N(������_A�A����)
	OBJ_BANK_04_MARISA_B,	// ���@�p�o���N(������_B�A����)
	OBJ_BANK_05_REMILIA,	// ���@�p�o���N(���~���A)
	OBJ_BANK_06_YUYUKO, 	// ���@�p�o���N(�H�X�q)
	OBJ_BANK_07_CIRNO_A,	// ���@�p�o���N(�`���mA)
	OBJ_BANK_08_CIRNO_Q,	// ���@�p�o���N(�`���m�H)
//
	OBJ_BANK_21_BOSS_STAGE1,	// �{�X�p�o���N(�X�e�[�W 1�A�A���X)
	OBJ_BANK_22_BOSS_STAGE2,	// �{�X�p�o���N(�X�e�[�W 2�A����)
	OBJ_BANK_23_BOSS_STAGE3,	// �{�X�p�o���N(�X�e�[�W 3�A�P��)
	OBJ_BANK_24_BOSS_STAGE4,	// �{�X�p�o���N(�X�e�[�W 4�A��)
	OBJ_BANK_25_BOSS_STAGE5,	// �{�X�p�o���N(�X�e�[�W 5�A�p�`�����[)
	OBJ_BANK_26_BOSS_STAGE6,	// �{�X�p�o���N(�X�e�[�W 6�A���)
	OBJ_BANK_11_ZAKO_STAGE1,	// �U�R�p�o���N(�X�e�[�W 1-6, extra, phantasm)		// OBJ_BANK_27_BOSS_STAGE7_dummy,
	OBJ_BANK_28_BOSS_STAGE8,	// �{�X�p�o���N(�X�e�[�W 8�A�G�L�X�g��)
	OBJ_BANK_29_BOSS_STAGE9,	// �{�X�p�o���N(�X�e�[�W 9�A�t�@���^�Y��)
//
//	OBJ_BANK_11_ZAKO_STAGE1,
//	OBJ_BANK_12_ZAKO_STAGE2,
//	OBJ_BANK_13_ZAKO_STAGE3,
//	OBJ_BANK_14_ZAKO_STAGE4,
//	OBJ_BANK_15_ZAKO_STAGE5,
//	OBJ_BANK_16_ZAKO_STAGE6,
//	OBJ_BANK_17_ZAKO_STAGE7_dummy,
//	OBJ_BANK_18_ZAKO_STAGE8,
//
	OBJ_BANK_MAX
};
#define 	OBJ_BANK_SIZE			(8*8)
#define 	OBJ_BANK_SIZE_00_TAMA	(OBJ_BANK_00_TAMA*OBJ_BANK_SIZE)
#define 	OBJ_BANK_SIZE_01_ITEM	(OBJ_BANK_01_ITEM*OBJ_BANK_SIZE)

/*---------------------------------------------------------
	�X�v���C�g
--------------------------------------------------------- */


/*-------------------*/
/* �j�e�Ƃ����邩��A���ݐ}�`���肵�Ȃ��ƍ��\����ɂȂ�B */
/* �����蔻�薳�����Ă����x�͕ς��Ȃ������������ߖ�ł���̂ŏ����I�ɂ͂��̕ӂ����ǂ������B */
//#define USE_ZUKEI_ATARI_HANTEI (1)
//#define USE_ZUKEI_ATARI_HANTEI (0)

typedef struct /*_point256*/
{
	Uint16 x;	/*	union POINT */	/* x �ʒu */
	Uint16 y;	/*	union POINT */	/* y �ʒu */
} POINT_u16;
/* u16���W */

typedef struct /*_point256*/
{
	int x256;	/*	union POINT */	/* x�\���ʒu  (256�Œ菬���_�`��) / akt. Position */
	int y256;	/*	union POINT */	/* y�\���ʒu  (256�Œ菬���_�`��) / akt. Position */
} POINT256;
/* t256���W */

typedef struct /*_point_vector256*/
{
	int x256;	/*	union POINT */	/* x�\���ʒu  (256�Œ菬���_�`��) / akt. Position */
	int y256;	/*	union POINT */	/* y�\���ʒu  (256�Œ菬���_�`��) / akt. Position */
	int vx256;	/*	union POINT_VECTOR */	/* ���pregist_vector(); */	/*union BULLET_VECTOR_DATA*/
	int vy256;	/*	union POINT_VECTOR */	/* ���pregist_vector(); */	/*union BULLET_VECTOR_DATA*/
} POINT_VECTOR256;
/* t256���W + t256�x�N�g�� */

/*
	�����X��off�̓���@�\:
	m_zoom_y256 �� M_ZOOM_Y256_NO_TILT ��ݒ肵�Ă����ƁA
	rotationCCW1024(�`��p�p�x) �𖳎����āA��]�����A����1.0�{�ŕ`�悷��B
*/
#define M_ZOOM_Y256_NO_TILT (0)
/*
	�A���X�̐l�`���O�Ղ͉�]�e�Ȃ̂ɃO���͉�]���Ă��Ȃ�������A
	�p�`�F�̉��̃A�j���[�V����(�Ε��u�A�O�j�V���C���v)��
	�`���m�⃋�[�~�A�̊ےe���ʒe����]���Ă��Ȃ�������A���̑��F�X�g�p���Ă���B
	�{�Ƃ��`��p�ݒ�p�x���V���v���ɖ������鉽�炩�̋@�\������̂ł�?�Ǝv���������Ă݂��B
	��{�I�Ɋےe�͉�]������Ɩ��ʂɒx���Ȃ�B
*/


#if 1
// union ���p
	#define base_hp 					katasa
	#define base_weapon_strength		katasa
	#define base_time_out				katasa/*bakuhatsu*/
//	#define base_score					user01
	#define base_score					user_data13
	//#define base_state				user01
#endif
//
/* �X�v���C�g�̎g����

����		���@		���e					�G					�G�e			����
katasa		--			base_weapon_strength	base_hp 			base_time_out	base_time_out
			--			���틭��				�̗�				����			����
user_data13 	--			--						base_score			--				--
			--			--						�X�R�A				--				--
*/

#define tmp_angleCCW65536 tmp_angleCCW1024
typedef struct _sprite
{
	int cx256;						/*	union POINT */			/* center x �\�� ���S���W�ʒu  (256�Œ菬���_�`��) / akt. Position */
	int cy256;						/*	union POINT */			/* center y �\�� ���S���W�ʒu  (256�Œ菬���_�`��) / akt. Position */
	int vx256;						/*	union POINT_VECTOR */	/* ���pregist_vector(); */	/*union BULLET_VECTOR_DATA*/
	int vy256;						/*	union POINT_VECTOR */	/* ���pregist_vector(); */	/*union BULLET_VECTOR_DATA*/
//[4]
	int rotationCCW1024;			/* �`��p�p�x(����0�x�ō����(�����v���), �����1024����, 0-1023�x) */
	int tmp_angleCCW1024;			/* �ێ��p�p�x(����0�x�ō����(�����v���), �����1024����, 0-1023�x) */
	int katasa; 					/* user00 ENEMY_BASE base; */	//int health;	int time_out;		/* user03 */
	int type;						/* ��ދy�юg�p�� / Sprite-Type, (siehe enum SPRITE_TYPE), 0 = remove. */
//[8]
	u32 color32;					/* Gu color AGBR8888 MAKE32RGBA(RED, GREEN, BLUE, ALPHA)�Ŋe�v�f 0 ���� 255 (0x00 ���� 0xff)�܂ŁB */
	int m_Hit256R;					/* �����蔻��p */
	int m_zoom_x256;				/* �\���g�嗦x (�摜�̉��̊g�嗦t256()�`���Ŏw��) */
	int m_zoom_y256;				/* �\���g�嗦y (�摜�̏c�̊g�嗦t256()�`���Ŏw��) */
//[12]
	/* �Ƃ肠�����ڍs�p(1)�]���݊�(�e) */
	int user_data00;
	int user_data01;
	int user_data02;
	int user_data03;
//[16]
	int user_data04;
	int user_data05;
	int user_data06;
	int user_data07;
//[20]
	/* �Ƃ肠�����ڍs�p(2)�ǉ��e�A��{�@�\ */
	int user_data10;
	int user_data11;
	int user_data12;
	int user_data13;		/* �G�X�R�A */
//	int user01; 					/* user01 ENEMY_BASE base; */	//int score;	int state;			/* user02 */
//
	void (*callback_loser)(struct _sprite *src);	/* Custom-loser-Routine (���ꂽ��Ƀ{�[�i�X���o���Ƃ��A�Ⴄ��������Ƃ�) / ���@�I�v�V�����̃A�j���[�V�����R�[���o�b�N */
	void (*callback_mover)(struct _sprite *src);	/* Custom-Move-Routine */
	void (*callback_hit_teki)(struct _sprite *c, struct _sprite *src);	// [***090325		�ǉ�
	struct _sprite *target_obj; 		/* �Ƃ肠�����ڍs�p(3)�ڍs������Ȃ��Ȃ�B */
//[20]
//
	/*u8*/u32 flags;					/* �t���O�Z�b�g / siehe unten (SP_FLAG...) */
	s32 jyumyou;	/* ���� */
//	u32 dummy_used;/*???*/
//	int dummy111;/* �g���ĂȂ��_�~�[ */
// �`��p
//	u16 u0;
//	u16 v0;
//	u16 u1;
//	u16 v1;
	/*u16*/u16/*int*/ tx;	//tx = u0;	int tx; 	/* u �e�N�X�`�����W */
	/*u16*/u16/*int*/ ty;	//ty = v0;	int ty; 	/* v �e�N�X�`�����W */
	/*u16*/u16/*int*/ w;	//w = (u1-u0);	int w;
	/*u16*/u16/*int*/ h;	//h = (v1-v0);	int h;
} SPRITE;
// 1858355 1858307
/*---------------------------------------------------------
	jyumyou ���� life_cycle sprite_time_out sprite_longevity
	-------------------------------------------------------
	���O�͖Y�ꂽ���ǁAKETM�ɂ������l�����B
	��x�͔r���������A�����������x����������B
	�������A�V�X�e���ŋ����I�ɒl�����Z����B
	�X�v���C�g�����������ƁA�l�� 1�B
	�X�v���C�g���t���[����������x�ɁA1 ���Z�����B
	-------------------------------------------------------
	psp�� 0 ���W�X�^������̂ŁA0 �����A
	�܂蕉���� ���������B
	-------------------------------------------------------
	int ����Ȃ��� short �ɂ���āB
	0x7fff==32767 �͐��������A1���Z����� signed short �̏ꍇ�A
	0x8000==32768==-32768==�����ɂȂ�̂ŁA�����ɂȂ����玩�����������B
---------------------------------------------------------*/

/*	struct _sprite *target_obj;
	�p�r��(
		player�p�U���e�A
		���̖����w"�d��2"(��)
	)
	�ق�Ƃ͕��G�ɃL���X�g����Ηv��Ȃ��񂾂��ǃo�O�ł��������B
 */


//#ifndef GRP_SCREEN_H
//#define GRP_SCREEN_H
/*	@since		Jul.27.2005 GRP_SCREEN_H		��ʕ\���Ǘ� */



/*---------------------------------------------------------
	���\�[�X resource
---------------------------------------------------------*/

extern SPRITE *obj_add_00_tama_error(void); 							/* gu�ڍs���A�e��p 				int bullet_type_num */
extern SPRITE *obj_add_01_teki_error(void); 							/* gu�ڍs���A�ėp�X�v���C�g 		int bullet_type_num */
extern SPRITE *obj_add_nn_direct(unsigned int register_object_absolute_direct_number);	/* �����o�^�p�B�댯�Ȃ̂Œ��ӂ��Ďg���B*/
/* (r33)�Ŗ��O�����łȂ��d�l���ς�����̂Œ��ӁB / ��p�Œ�X�v���C�g�̏ꍇ�͌Œ�l�܂ł̃I�t�Z�b�g
OBJ_HEAD_02_KOTEI�𑫂��B */

extern void sprite_all_cleanup(void);
extern void sprite_move_all(void);		/* �X�v���C�g�I�u�W�F�N�g�̈ړ����� */


extern SPRITE *obj_collision_check_01_teki(SPRITE *tocheck);/*�G��p*/	//, int type);
extern SPRITE *obj_collision_check_00_tama(SPRITE *tocheck, int type);/*gu�e����p*/

extern void gamen_gai_nara_zako_osimai(SPRITE *src);/* ��ʊO�Ȃ炨���܂� */
extern void check_boss_option_time_out(SPRITE *src);/* */

#if 1
/*�p�x�e�̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�)*/
/*��ʊO�Ȃ�e������*/
extern void bullet_angle_all_gamen_gai_nara_kesu(void);

/*��ʊO�Ȃ�e�𔽎�*/
extern void bullet_angle_all_gamen_gai_nara_hansya(void);		/* ���˒e�̂Ă��� */

/*��ʊO�Ȃ�e�𔽎ˌ���*/
extern void bullet_angle_all_gamen_gai_nara_hansya_gensoku(void);	/* ���ˌ����e�̂Ă��� */
#endif

/* �g�p���ł��邪�ޔ��������ꍇ�B��ʊO�ɃX�v���C�g���ړ������A�����ɂ���B */
extern void sprite_initialize_position(SPRITE *src);

//#if 1
//	/* �\�t�g�E�F�A�[�� Z�\�[�g */
//	#define USE_ZBUFFER 	(0)
//#else
//	/* �n�[�h�E�F�A�[�Ń\�[�g */
//	#define USE_ZBUFFER 	(1)
//#endif
//



#endif/* _SPRITE_H_ */



#ifndef _BULLET_SYSTEM_H_
#define _BULLET_SYSTEM_H_

/*---------------------------------------------------------
	�G�e�Ǘ��V�X�e��
---------------------------------------------------------*/

//#if 000/* �ڍs���ɂ����� */
//	/* MAX_POOL_BULLET: 2��n��̕K�v����(1024�Ƃ�2048�Ƃ�) */
//#define MAX_POOL_BULLET 1024
//extern TGameSprite bullet_pool[MAX_POOL_BULLET];
//
//extern void bullet_system_init(void);
//extern void bullet_system_exit(void);
//#endif /* 000 �ڍs���ɂ����� */




#if 1
/*---------------------------------------------------------
	�V�A�G�e�Ǘ��V�X�e��(��)
---------------------------------------------------------*/

enum
{
	PANEL_OBJ_00_S00 = 0,	/* �X�R�A10���� */
	PANEL_OBJ_01_S01,		/* �X�R�A9���� */
	PANEL_OBJ_02_S02,		/* �X�R�A8���� */
	PANEL_OBJ_03_S03,		/* �X�R�A7���� */
	PANEL_OBJ_04_S04,		/* �X�R�A6���� */
	PANEL_OBJ_05_S05,		/* �X�R�A5���� */
	PANEL_OBJ_06_S06,		/* �X�R�A4���� */
	PANEL_OBJ_07_S07,		/* �X�R�A3���� */
	PANEL_OBJ_08_S08,		/* �X�R�A2���� */
	PANEL_OBJ_09_S09,		/* �X�R�A1���� */
	PANEL_OBJ_0a_P00,		/* �p���[1���� */
	PANEL_OBJ_0b_P01,		/* �p���[�����_1���� */
	PANEL_OBJ_0c_P02,		/* �p���[�����_2���� */
	PANEL_OBJ_0d_Time,		/* "Time:" */
	PANEL_OBJ_0e_P0p,		/* �p���['.' */
	PANEL_OBJ_0f_NAN_IDO,	/* ��Փx�\�� "Easy", "Normal", "Hard", "Lunatic", */
	//
	PANEL_OBJ_10_H00,		/* �n�C�X�R�A10���� */
	PANEL_OBJ_11_H01,		/* �n�C�X�R�A9���� */
	PANEL_OBJ_12_H02,		/* �n�C�X�R�A8���� */
	PANEL_OBJ_13_H03,		/* �n�C�X�R�A7���� */
	PANEL_OBJ_14_H04,		/* �n�C�X�R�A6���� */
	PANEL_OBJ_15_H05,		/* �n�C�X�R�A5���� */
	PANEL_OBJ_16_H06,		/* �n�C�X�R�A4���� */
	PANEL_OBJ_17_H07,		/* �n�C�X�R�A3���� */
	PANEL_OBJ_18_H08,		/* �n�C�X�R�A2���� */
	PANEL_OBJ_19_H09,		/* �n�C�X�R�A1���� */
	PANEL_OBJ_1a_fps00, 	/* fps2���� */
	PANEL_OBJ_1b_fps01, 	/* fps1���� */
	PANEL_OBJ_1c_fps02, 	/* fps�����_1���� */
	PANEL_OBJ_1d_fps03, 	/* fps�����_2���� */
	PANEL_OBJ_1e_fps0p, 	/* fps '.' */
	PANEL_OBJ_1f_fps_str,	/* "fps" */
	//
	PANEL_OBJ_20_N00,		/* �c��K�b�c8���� */
	PANEL_OBJ_21_N01,		/* �c��K�b�c7���� */
	PANEL_OBJ_22_N02,		/* �c��K�b�c6���� */
	PANEL_OBJ_23_N03,		/* �c��K�b�c5���� */
	PANEL_OBJ_24_N04,		/* �c��K�b�c4���� */
	PANEL_OBJ_25_N05,		/* �c��K�b�c3���� */
	PANEL_OBJ_26_N06,		/* �c��K�b�c2���� */
	PANEL_OBJ_27_N07,		/* �c��K�b�c1���� */
	PANEL_OBJ_28_B00,		/* �{��8���� */
	PANEL_OBJ_29_B01,		/* �{��7���� */
	PANEL_OBJ_2a_B02,		/* �{��6���� */
	PANEL_OBJ_2b_B03,		/* �{��5���� */
	PANEL_OBJ_2c_B04,		/* �{��4���� */
	PANEL_OBJ_2d_B05,		/* �{��3���� */
	PANEL_OBJ_2e_B06,		/* �{��2���� */
	PANEL_OBJ_2f_B07,		/* �{��1���� */
	//
	PANEL_OBJ_30_J00,		/* �c�莞��3���� */
	PANEL_OBJ_30_J01,		/* �c�莞��2���� */
	PANEL_OBJ_30_J02,		/* �c�莞��1���� */
	PANEL_OBJ_31_G03,		/* �O���C�Y5���� */
	PANEL_OBJ_31_G04,		/* �O���C�Y4���� */
	PANEL_OBJ_31_G05,		/* �O���C�Y3���� */
	PANEL_OBJ_32_G06,		/* �O���C�Y2���� */
	PANEL_OBJ_33_G07,		/* �O���C�Y1���� */
	//
	OBJ_POOL_03_PANEL_MAX/* MAX */
};
//enum /*_common_my_obj_*/
//{
//	MY_O BJ_00_JIKI = 0,		/*[r32�ύX�ς�]*/
//	MY_O BJ_01_JIKI_MARU,		/**/		/*[r32�ύX�ς�]*/
//	MY_O BJ_02_JIKI_OPT01,		/**/		/*[r32�ύX�ς�]*/
//	MY_O BJ_03_JIKI_OPT02,		/**/		/*[r32�ύX�ς�]*/
//	MY_O BJ_04_JIKI_OPT03,		/**/		/*[r32�ύX�ς�]*/
//	MY_O BJ_05_JIKI_OPT04,		/**/		/*[r32�ύX�ς�]*/
//	MY_O BJ_06_SEND1,			/*[�\��]*/
//	MY_O BJ_07_,				/*[�\��]*/
//
//	MY_O BJ_08_BOSS,			/*[�\��]*/
//	MY_O BJ_09_,				/*[�\��]*/
//	MY_O BJ_0a_,				/*[�\��]*/
//	MY_O BJ_0b_EFFECT01,		/*[�\��]*/
//	MY_O BJ_0c_EFFECT02,		/*[�\��]*/
//	MY_O BJ_0d_EFFECT03,		/*[�\��]*/
//	MY_O BJ_0e_EFFECT04,		/*[�\��]*/
//	MY_O BJ_0f_EFFECT05,		/*[�\��]*/
//
//	MY_O BJ_99_MAX
//};
enum
{
	/* �`��v���C�I���e�B�[�����@�e�N�X�`���[�̂��� */
	FIX_OBJ_00_PLAYER = 0,		/*<�g�p��>[r32�ύX�ς�] obj_add_nn_direct(OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER); */
	FIX_OBJ_01_JIKI_MARU,		/*<�g�p��>[r32�ύX�ς�] obj_add_nn_direct(OBJ_HEAD_02_KOTEI+FIX_OBJ_01_JIKI_MARU); */
	FIX_O_BJ_02_SEND1,			/*[�\��]*/
	FIX_O_BJ_03_SEND2,			/*[�\��]*/
	FIX_OBJ_04_JIKI_OPTION0,	/*<�g�p��>[r32�ύX�ς�] obj_add_nn_direct(OBJ_HEAD_02_KOTEI+FIX_OBJ_04_JIKI_OPTION0+jj); */
	FIX_OBJ_05_JIKI_OPTION1,	/*<�g�p��>[r32�ύX�ς�]*/
	FIX_OBJ_06_JIKI_OPTION2,	/*<�g�p��>[r32�ύX�ς�]*/
	FIX_OBJ_07_JIKI_OPTION3 ,	/*<�g�p��>[r32�ύX�ς�]*/
//
	/* �`��v���C�I���e�B�[��front�e�N�X�`���[�̂��� */
//	xxx_FIX_OBJ_08_BOSS,		/* <�����蔻��̓s����o���Ȃ�> */	/*<�g�p��>[r32�ύX�ς�] obj_add_nn_direct(OBJ_HEAD_02_KOTEI+FIX_OBJ_08_BOSS); */
//	FIX_O_BJ_09_,				/*[�\��]*/
//	FIX_O_BJ_10_,				/*[�\��]*/
//	FIX_OBJ_11_EFFECT01,				/*<�g�p��>*/
//	FIX_O_BJ_12_SUPPORT0,		/*[�\��]*/		/*<�g�p��>*/
//	FIX_O_BJ_13_SUPPORT1,		/*[�\��]*/		/*<�g�p��>*/
//	FIX_O_BJ_14_SUPPORT2,		/*[�\��]*/		/*<�g�p��>*/
//	FIX_O_BJ_15_SUPPORT3,		/*[�\��]*/		/*<�g�p��>*/
	FIX_OBJ_08_EFFECT01,		/*[�\��]*/		/*<�g�p��>*/
	FIX_OBJ_09_EFFECT02,		/*[�\��]*/		/*<�g�p��>*/
	FIX_OBJ_10_EFFECT03,		/*[�\��]*/		/*<�g�p��>*/
	FIX_OBJ_11_EFFECT04,		/*[�\��]*/		/*<�g�p��>*/
	FIX_OBJ_12_EFFECT05,		/*[�\��]*/		/*<�g�p��>*/
	FIX_OBJ_13, 	/*[�\��]*/
	FIX_OBJ_14, 	/*[�\��]*/
	FIX_OBJ_15_JIKI_TEISOKU_EFFECT, 	/*[�\��]*/				/*<�g�p��>*/
	OBJ_POOL_02_KOTEI_MAX/*MAX*/
};
	/* �`��v���C�I���e�B�[�����@�e�N�X�`���[�̂��� */
/* -- �v���C�I���e�B�[���U�D���@��`�� */
//#define SPRITE_111JIKI_POOL_MAX FIX_OBJ_07_JIKI_OPTION3

/*
 �Q�l: kouma_tr013/�����g����/�}�j���A��/html/faq.html
�p�Q�R�@�A�C�e���̕\�����E�́H

�@��ʓ��T�P�Q�܂łł��B
�@�G�e�̐����U�S�O�Ȃ̂ŁA�S�Ă��{�[�i�X�A�C�e���ɂȂ�Ƃ͌���܂��񂪁A
�@�w�ǂ̏ꍇ�̓X�R�A�p�^�[���ɉe������悤�Ȃ��Ƃ͂���܂���B

*/

/* �e�̍ő吔�� OBJ_POOL_00_TAMA_MAX (==1024) ����ȏ�o�^���悤�Ƃ��Ă��o�^����Ȃ��B */
//#define OBJ_POOL_00_TAMA_MAX		(512)/* �Ƃ肠���� */
#define OBJ_POOL_00_TAMA_MAX		(1024)/* �e�o�^�v���O�����̓s���� 2^n �ł���K�v������܂��B(1024==(1<<10) ) */
//extern SPRITE obj99[OBJ_HEAD_00_TAMA+OBJ_POOL_00_TAMA_MAX];		/* �e��p�X�v���C�g */

/* �G(���@�A�I�v�V�����A���e���A�e�ȊO�̑S�ẴX�v���C�g�܂�)�̍ő吔�� OBJ_POOL_01_TEKI_MAX (==256) ����ȏ�o�^���悤�Ƃ��Ă��o�^����Ȃ��B */
#define OBJ_POOL_01_TEKI_MAX		(256)/* �G�o�^�v���O�����̓s���� 2^n �ł���K�v������܂��B(1024==(1<<10) ) */
//extern SPRITE obj99[OBJ_HEAD_01_TEKI+OBJ_POOL_01_TEKI_MAX];		/* �G��p�X�v���C�g */

//extern SPRITE obj99[OBJ_HEAD_02_KOTEI+OBJ_POOL_02_KOTEI_MAX]; 	/* �G��p�X�v���C�g */

/* �p�l���p�X�v���C�g�� OBJ_POOL_03_PANEL_MAX (==256) ����ȏ�o�^���悤�Ƃ��Ă��o�^����Ȃ��B */
//#define OBJ_POOL_03_PANEL_MAX 	(256)/* �G�o�^�v���O�����̓s���� 2^n �ł���K�v������܂��B(1024==(1<<10) ) */
//extern SPRITE obj99[OBJ_HEAD_03_PANEL+OBJ_POOL_03_PANEL_MAX]; 	/* �p�l���p�X�v���C�g */





	/* �S�X�v���C�g */
#define OBJ_HEAD_00_TAMA	(0) 										/* �e��p�X�v���C�g */
#define OBJ_HEAD_01_TEKI	(OBJ_HEAD_00_TAMA+OBJ_POOL_00_TAMA_MAX) 	/* �G��p�X�v���C�g */
#define OBJ_HEAD_02_KOTEI	(OBJ_HEAD_01_TEKI+OBJ_POOL_01_TEKI_MAX) 	/* ���@���Œ�X�v���C�g */
#define OBJ_HEAD_03_PANEL	(OBJ_HEAD_02_KOTEI+OBJ_POOL_02_KOTEI_MAX)	/* �p�l���p�X�v���C�g */
#define OBJ99_MAX			(OBJ_HEAD_03_PANEL+OBJ_POOL_03_PANEL_MAX)
extern SPRITE obj99[OBJ99_MAX]; 	/* �S�X�v���C�g */


/*---------------------------------------------------------
	�I�u�W�F�o���N�V�X�e��
--------------------------------------------------------- */
extern void reflect_sprite_spec444(SPRITE *sss, unsigned int bank_offset);	/* �I�u�W�F�N�g�̃o���N�ݒ�𔽉f������B */
#endif

#endif /* _BULLET_SYSTEM_H_ */
