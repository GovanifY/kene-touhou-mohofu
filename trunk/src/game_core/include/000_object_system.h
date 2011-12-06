
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���̃t�@�C���͒��ڃC���N���[�h���܂���B
	"game_main.h" ����̂݊ԐړI�ɃC���N���[�h���܂��B
---------------------------------------------------------*/

#ifndef _OBJECT_SYSTEM_H_
#define _OBJECT_SYSTEM_H_

/*---------------------------------------------------------
	�X�v���C�g�I�u�W�F�N�g �V�X�e��
--------------------------------------------------------- */
/*
����:   s32 JYUMYOU
1[�b]                                   60           >                64 [frame]
1[��]                                   60 x 60      >           64 x 64 [frame]
1[����] 0x034BC0 ==   216000            60 x 60 x 60 >      64 x 64 x 64 [frame] ==   262144 0x040000
2.5[��] 0xC5C100 == 12960000 2.5 x 24 x 60 x 60 x 60 > 64 x 64 x 64 x 64 [frame] == 16777216 0x01000000
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
//#define JYUMYOU_ZAKO_CLIP 		(2048)				/* 2048/60 = 34.13333333333[�b] */

/*
	�������Ԃ���_�Ƃ��������̏ꍇ�A(��ŃV�X�e�����ς�邩������Ȃ�����)
	��L(JYUMYOU_TAMA_HASSEI / JYUMYOU_ZAKO_HASSEI)����Ƃ��Ă����B
*/


/*---------------------------------------------------------
	�v���C�I���e�B�[(�D�揇��)
	-------------------------------------------------------
	�v���C�I���e�B�[�ɂ́A�`��v���C�I���e�B�[�ȊO�ɁA
	�����蔻��v���C�I���e�B�[(==�����蔻���̈�ʂɔ��ʂ���@�\)������̂Œ��ӁB
	-------------------------------------------------------
--------------------------------------------------------- */
/* �����͎�ޕʂł͂Ȃ��āA�@�\�ʂɕ��ނ���B */

#define OBJ_Zff_MASK_SUB_TYPE				(0x00ff)
//		OBJ_Z00_MAHOU_JIN													/* === Gu�����w�� === */
#define OBJ_Z01_JIKI_GET_ITEM				(0x0100)/*0x0100*/				/* === Gu�v���C���[�� === */
#define OBJ_Z02_TEKI						(0x0200)/*0x0400*/				/* === Gu�G�� === */
#define OBJ_Z03_ITEM						(0x0400)/*(0x2000)0x1000*/		/* === GuITEM�� === */
#define OBJ_Z04_TAMA						(0x0800)/*(0x1000)0x0800*/		/* === Gu�G�e�� === */
#define OBJ_Z05_FRONT						(0x1000)/*(0x8000)*/			/* �t�����g��(�����蔻�薳��) Gu�p�l���� === */
//efine OBJ_Z80_SHOT_SPECIAL				(0x8000)/*0x0100*/				/* �얲�̉�]�V���b�g / kodomo_hlaser�̔��� */

//#define S P_GROUP_ALL_SDL_CORE_TYPE		(0x1f00)/* game_core���� */
//#define OBJ_Zff_MASK_SUB_TYPE				(0x007f)


/*---------------------------------------------------------
	�I�u�W�F�N�g�o���N�V�X�e��
	-------------------------------------------------------
	�摜1�e�N�X�`���ɍő�64�̃I�u�W�F�N�g�����蓖�Ă鎖���o����B
	�ő�64�Ȃ̂́Apsp�ł͂��܂葽���ƒᑬ������B
	-------------------------------------------------------
	�I�u�W�F�N�g��o�^���������ł͉摜�͍X�V����Ȃ��̂ŁA
	�摜���I�u�W�F�N�g�ɔ��f������ׂɂ� reflect_sprite_spec(); �ŁA
	���f����e�N�X�`���o���N(�摜�ԍ�)���w�肵�āA
	�I�u�W�F�N�g�̃o���N�ݒ�𔽉f������ƁA�摜���X�V�����B
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
	OBJ_BANK_01_REIMU_A,		// ���@�p�o���N(�얲A�A�아)
	OBJ_BANK_02_REIMU_B,		// ���@�p�o���N(�얲B�A����)
	OBJ_BANK_03_MARISA_A,		// ���@�p�o���N(������_A�A����)
	OBJ_BANK_04_MARISA_B,		// ���@�p�o���N(������_B�A����)
	OBJ_BANK_05_REMILIA,		// ���@�p�o���N(���~���A)
	OBJ_BANK_06_YUYUKO, 		// ���@�p�o���N(�H�X�q)
	OBJ_BANK_07_CIRNO_A,		// ���@�p�o���N(�`���mA�A�X��)
	OBJ_BANK_08_CIRNO_Q,		// ���@�p�o���N(�`���m�H�A�H��)
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

typedef struct /*_point256*/
{
	u16 x;	/*	union POINT */	/* x �ʒu */
	u16 y;	/*	union POINT */	/* y �ʒu */
} POINT_u16;
/* u16���W */

typedef struct /*_point256*/
{
	int x256;	/*	union POINT */	/* x�\���ʒu  (256�Œ菬���_�`��) / akt. Position */
	int y256;	/*	union POINT */	/* y�\���ʒu  (256�Œ菬���_�`��) / akt. Position */
} POINT256;
/* t256���W */

//typedef struct /*_point_vector256*/
//{
// POINT256 c;	/*	union POINT */	/* x�\���ʒu  (256�Œ菬���_�`��) / akt. Position */
//				/*	union POINT */	/* y�\���ʒu  (256�Œ菬���_�`��) / akt. Position */
// POINT256 v;	/*	union POINT_VECTOR */	/* ���pregist_vector(); */	/*union BULLET_VECTOR_DATA*/
//				/*	union POINT_VECTOR */	/* ���pregist_vector(); */	/*union BULLET_VECTOR_DATA*/
//} POINT_VECTOR256;
/* t256���W + t256�x�N�g�� */

/*
	�����X��off�̓���@�\:
	m_zoom.y256 �� M_ZOOM_Y256_NO_TILT ��ݒ肵�Ă����ƁA
	rotationCCW1024(�`��p�p�x) �𖳎����āA��]�����A����1.0�{�ŕ`�悷��B
*/
#define M_ZOOM_Y256_NO_TILT (0)
/*
	�p�`�F�̉��̃A�j���[�V����(�Ε��u�A�O�j�V���C���v)��
	�`���m�⃋�[�~�A�̊ےe���ʒe����]���Ă��Ȃ�������A���̑��F�X�g�p���Ă���B
	�{�Ƃ��`��p�ݒ�p�x���V���v���ɖ������鉽�炩�̋@�\������̂ł�?�Ǝv���������Ă݂��B
	��{�I�Ɋےe�͉�]������Ɩ��ʂɒx���Ȃ�B
*/


#if 1
// union ���p
	#define base_hp 					tairyoku_ti
	#define base_time_out				tairyoku_ti/*bakuhatsu*/
//	#define base_score					user01
	#define base_score					kougeki_ti
	#define atari_hantei				kougeki_ti
	#define ATARI_HANTEI_OFF			(0)
	#define ATARI_HANTEI_TAOSENAI		(0x3fffffff)
	//#define base_state				user01
#endif
//
/* �X�v���C�g�̎g����

����            ���@           ���e        �{�X         ���{�X    �U�R       �G                  �G�e            ����
tairyoku_ti     --             --          [����]     [zako      zako]      base_hp             base_time_out   base_time_out
                --             --           �̗�        �̗�      �̗�        �̗�                ����            ����
----------------------------------------------------------------------------------------------
kougeki_ti     (1)            kougeki_ti                                   base_score             --              --
                ���G������     ���틭��    �X�R�A      �X�R�A     �X�R�A      �X�R�A              --              --
------------
�U���l�ƃX�R�A�͓������Ȃ̂ŁA�v���[���[���{�X�ɑ̓����肳��Ď󂯂�_���[�W�̓{�X�̃X�R�A�Ɠ����ʁB
�X�R�A0�Ȃ�΃v���[���[�͐G���Ă��_���[�W���󂯂Ȃ��̂ŁA�v���[���[�͎��ȂȂ��B
*/

#define tmp_angleCCW65536 tmp_angleCCW1024
typedef struct _obj_tag_
{
	POINT256	center; 		/* �\�� ���S���W�ʒu(x256, y256)  (256�Œ菬���_�`��) / akt. Position */
	POINT256	math_vector;	/* ���w�I�ȈӖ��ł̃x�N�g���B(x256, y256) */	/* ���pregist_vector(); */	/*union BULLET_VECTOR_DATA*/
//[4==1*4]
	int rotationCCW1024;	/* �`��p�p�x(����0�x�ō����(�����v���), �����1024����, 0-1023�x) */
	int tmp_angleCCW1024;	/* �ێ��p�p�x(����0�x�ō����(�����v���), �����1024����, 0-1023�x) */
	s32 jyumyou;			/* ���� */
	int obj_type_set;		/* [�f�o�b�O��/�����o����悤�ɕςȖ��O�ɂ��Ƃ�] ��ދy�юg�p�� / Sprite-Type, ( enum OBJ_TYPE), 0 = remove. */
//[8==2*4]
	u32 color32;			/* Gu color AGBR8888 MAKE32RGBA(RED, GREEN, BLUE, ALPHA)�Ŋe�v�f 0 ���� 255 (0x00 ���� 0xff)�܂ŁB */
	int m_Hit256R;			/* �����蔻��p */
	POINT256	m_zoom; 	/* �\���g�嗦(x256, y256) (�摜�̉��̊g�嗦t256()�`���Ŏw��) */
	//
//[12==3*4]
	int system_data00;		/*(���C���[�V�X�e���p)*/	/*user_data02==(r35-�e���V�X�e���p�r�Ń��C���[�Ǘ����Ă���̂ŏd�v)*/
	int for_align_dummy_22; /*[�g���\��]*/	/* .align ���킹�p�B.align�����Ă��������s���x�͑������A�v���O�������ȗ����o����̂ŏ�ɏ������B�g���ĂȂ��_�~�[�B */
	int kougeki_ti; 		/* �U���l / score / �����蔻�� */		/* �G�X�R�A */
	int tairyoku_ti;		/* user00 ENEMY_BASE base; */			//int health;	int time_out;		/* user03 */
	/* �Ƃ肠�����ڍs�p(2)�ǉ��e�A��{�@�\ */
//[16==4*4]
	/* �Ƃ肠�����ڍs�p(1)�]���݊�(�e) */
	int user_data00;
	int user_data01;
	int user_data02;
	int user_data03;
//[20==5*4]
	int user_data04;
	int user_data05;
	int user_data06;
	int user_data07;
//[24==6*4]
	void (*callback_move01)(struct _obj_tag_ *src); 			/* Custom-Move-Routine. */
	void (*callback_move02)(struct _obj_tag_ *src); 			/* Custom-Attack-Routine. */
	void (*callback_move03)(struct _obj_tag_ *src, struct _obj_tag_ *tama);
	void (*callback_move04)(struct _obj_tag_ *src); 			/* Custom-loser-Routine (���ꂽ��Ƀ{�[�i�X���o���Ƃ��A�Ⴄ��������Ƃ�) / ���@�I�v�V�����̃A�j���[�V�����R�[���o�b�N */
//[28==7*4]
// �`��p
	struct _obj_tag_ *target_obj;		/* �Ƃ肠�����ڍs�p(3)�ڍs������Ȃ��Ȃ�B */
	/*u16*/u16/*int*/ tx;	//	u16 u0;//tx = u0;		int tx; 	/* u �e�N�X�`�����W */
	/*u16*/u16/*int*/ ty;	//	u16 v0;//ty = v0;		int ty; 	/* v �e�N�X�`�����W */
	/*u16*/u16/*int*/ w;	//	u16 u1;//w = (u1-u0);	int w;
	/*u16*/u16/*int*/ h;	//	u16 v1;//h = (v1-v0);	int h;
	int for_align_dummy_11;/* .align ���킹�p�B.align�����Ă��������s���x�͑������A�v���O�������ȗ����o����̂ŏ�ɏ������B�g���ĂȂ��_�~�[�B */
//[32==8*4]
} OBJ;
/* Custom-Tamakesi-Routine / Zako Anime */


//	int user01; 					/* user01 ENEMY_BASE base; */	//int score;	int state;			/* user02 */
/*
kougeki_ti: 	�U���l:
�v���C���[obj:			���G���ԂɎG���ɑ̓����肷��ꍇ�̍U���l�B(1)
�v���C���[�V���b�g: 	�V���b�g�̋����B
�G��obj:				�G�����v���C���[�ɑ̓����肷��ꍇ�̍U���l�B�y�ѓ|�����ꍇ�ɓ�����X�R�A�B
�{�Xobj:				�G�����v���C���[�ɑ̓����肷��ꍇ�̍U���l�B�y�ѓ|�����ꍇ�ɓ�����X�R�A�B
���oobj:				�s����, ���(0)��ێ��B(???)

*/

#if (1)
/*
	-------------------------------------------------------
	�Q�[���ł͉����A�N�V�������N�������ꍇ�A�n�a�i(�L����)�ɂ���đΏ��̕��@���قȂ�B
	�Ⴆ�΂n�a�i(�L����)���Ԃ������ꍇ�B
	���@�ƒe���Ԃ������ꍇ�A���@������(�_���[�W�󂯂�)�B
	���@�ƃA�C�e�����Ԃ������ꍇ�A�A�C�e��������(�A�C�e�����)�B
	�����̊֌W�́A�Ԃ��鏈�܂ł͓����B�ŁA�Ԃ����Ă���̏������Ⴄ�B
	-------------------------------------------------------
	�������Ⴄ�����́A�R�[���o�b�N�Ƃ������@�ŋz������B
	�R�[���o�b�N�ɗ\�ߏ�����o�^���Ă����ƁA�Ԃ������ꍇ�\�ߓo�^���Ă������A
	�R�[���o�b�N���[�`��(�R�[���o�b�N����鏈��)���Ă΂��B
	-------------------------------------------------------
	�R�[���o�b�N�̎�ނƂ��āA�Ԃ������ꍇ�ȊO�ɂ��A
	�ړ�����ꍇ�B���ꂽ�ꍇ�B���R�[���o�b�N�ɂȂ��Ă���B
	-------------------------------------------------------
	�R�[���o�b�N�͕֗������ǁA�����I�ɒx���B�������ɂ����g�p���Ȃ������ɂ́A�ǂ����@�����A
	�ώG�ɌĂ΂�鏈���ɂ͎g���ƒx���Ȃ�B
	-------------------------------------------------------
	�U���������V�X�e�����ɓ��ꂽ��A�ُ�ɒx���Ė����������B
	-------------------------------------------------------
*/
/* callback_move01: �ړ�����ꍇ�̏����B */
	#define callback_mover			callback_move01

/* callback_move02: �X�y�J�B�U�������B */
	#define callback_kougeki		callback_move02/*(r39���ݎg�����̂�)*/

/* callback_move03: �Ԃ������ꍇ�̏����B */
	#define callback_hit_teki		callback_move03

/* callback_move04: [�{�X�ȊO]���ꂽ�ꍇ�̏����B */
/* callback_move04: [�{�X�̂�]�X�y�J�����B */
	#define callback_loser			callback_move04
// �G������݂�̂� card_system �̕����ǂ��C�������̂� card_system �ֈړ��B
	#define callback_root_supeka	callback_move04
/*
[�{�X]�����ꂽ�ꍇ�A���ʏ����Ȃ̂ŁAcallback_loser�͗v��Ȃ��B
�����ŁA[�{�X]�̏ꍇ�́A�X�y�J�R�[���o�b�N�Ɋ��蓖�Ă�B
*/




//[�p�~]	#define callback_boss_tamakesi	callback_move04/* ���p */
//[�p�~]	#define callback_zako_anime 	callback_move02/* ���p */
#endif


/*	struct _sprite *target_obj;
	�p�r��(
		player�p�U���e�A
		���̖����w"�d��2"(��)
	)
	�ق�Ƃ͕��G�ɃL���X�g����Ηv��Ȃ��񂾂��ǃo�O�ł��������B
 */



/*---------------------------------------------------------
	�X�v���C�g�I�u�W�F�N�g �V�X�e��
---------------------------------------------------------*/
/*(�o�^�����n)*/
extern OBJ *obj_regist_tama(void);	/* [A00�o�^�e�̈�]��obj��o�^����B(==�e/�A�C�e����p) */
extern OBJ *obj_regist_teki(void);	/* [A01�o�^�G�̈�]��obj��o�^����B(==�G��p) */
/* �C�ӂ̗̈�ɋ����I�ɓo�^����B[A02�Œ�̈�]��[A04�p�l���̈�]�͂��̕��@�œo�^����B */
/* �����Ǘ��l�̃I�t�Z�b�g��f���Ɏw�肷��B�Ⴆ�ΐ�p�Œ�X�v���C�g�̏ꍇ�͌Œ�l�܂ł̃I�t�Z�b�gOBJ_HEAD_01_0x0800_TOUROKU�𑫂��B */
extern OBJ *obj_regist_direct_number(unsigned int register_object_absolute_direct_number);	/* �����o�^�p�B�댯�Ȃ̂Œ��ӂ��Ďg���B*/

/*(�����蔻��n)*/
/* [A00�o�^�e�̈�]��obj�Ƃ̂����蔻��`�F�b�N�B */
extern OBJ *obj_collision_check_00_tama(OBJ *tocheck, int type);/*�e��p�Atype�ɂ͒e���A�C�e�������w�肷��B*/
/* [A01�G�̈�]��obj�Ƃ̂����蔻��`�F�b�N�B */
extern OBJ *obj_collision_check_A01_A02_teki(OBJ *tocheck);/*�G��p*/	//, int type);

/*(�v���C���[�ɂƂ��Ă̓G��T��)*/
extern OBJ *obj_teki_search(void);

/*(���̑��n)*/
/* [A00�o�^�e�̈�]��[A01�o�^�G�̈�]��[A02�Œ�̈�]��[A04�p�l���̈�]��OBJ��S�����B */
extern void obj_system_cleanup_all(void);

/* [A00�o�^�e�̈�]��[A01�o�^�G�̈�]��[A02�Œ�G�̈�]��[A03�Œ�̈�]��OBJ���ړ���������B[A04�p�l���̈�]��obj�͈ړ����Ȃ��B */
extern void obj_system_move_all(void);

extern OBJ_CALL_FUNC(gamen_gai_nara_zako_osimai);/* ��ʊO�Ȃ炨���܂� */
extern OBJ_CALL_FUNC(check_tukaima_kougeki_time_out);/*(�U��/���Ԑ���/����)*/

/* �g�p���ł��邪�ޔ��������ꍇ�B��ʊO�ɃX�v���C�g���ړ������A�����ɂ���B */
extern OBJ_CALL_FUNC(sprite_kotei_obj_r36_taihi);




#endif/* _OBJECT_SYSTEM_H_ */




#ifndef _OBJ_AREA_H_
#define _OBJ_AREA_H_

#if 1
/*---------------------------------------------------------
	�G�e�Ǘ��V�X�e��
---------------------------------------------------------*/

	/* �`��v���C�I���e�B�[�����@�e�N�X�`���[�̂��� */
/* -- �v���C�I���e�B�[���U�D���@��`�� */
//#define OBJ_111JIKI_POOL_MAX FIX_OBJ_07_JIKI_OPTION3

/*
 �Q�l: kouma_tr013/�����g����/�}�j���A��/html/faq.html
�p�Q�R�@�A�C�e���̕\�����E�́H

�@��ʓ��T�P�Q�܂łł��B
�@�G�e�̐����U�S�O�Ȃ̂ŁA�S�Ă��{�[�i�X�A�C�e���ɂȂ�Ƃ͌���܂��񂪁A
�@�w�ǂ̏ꍇ�̓X�R�A�p�^�[���ɉe������悤�Ȃ��Ƃ͂���܂���B

*/

enum/* [A00]�G���A#00: [A00�e�̈�] */
{
/* �e�̍ő吔�� OBJ_POOL_00_TAMA_1024_MAX (==1024) ����ȏ�o�^���悤�Ƃ��Ă��o�^����Ȃ��B */
//	OBJ_POOL_00_TAMA_1024_MAX = (1<<9),/*(���d�l512�̃���)*/
	OBJ_POOL_00_TAMA_1024_MAX = (1<<10),/* �e�o�^�v���O�����̓s���� 2^n �ł���K�v������܂��B(1024==(1<<10) ) */
//	OBJ_POOL_00_TAMA_1024_MAX = (1<<11),/* �e�o�^�v���O�����̓s���� 2^n �ł���K�v������܂��B(2048==(1<<11) ) */
};

enum/* [A01]�G���A#01: [A01�o�^�G�̈�] */
{
/* �G(���@�A�I�v�V�����A���e���A�e�ȊO�̑S�ẴX�v���C�g�܂�)�̍ő吔�� OBJ_POOL_02_TEKI_FIX_0256_MAX (==256) ����ȏ�o�^���悤�Ƃ��Ă��o�^����Ȃ��B */
	OBJ_POOL_01_TOUROKU_0256_MAX = (1<<8),/* �G�o�^�v���O�����̓s���� 2^n �ł���K�v������܂��B(256==(1<<8) ) */
};

enum/* [A02]�G���A#02: [A02�Œ�G�̈�] */
{
	TEKI_OBJ_00_BOSS00_HONTAI = 0,/*<�g�p��>[r36]*/
	TEKI_OBJ_01_BOSS01_HONTAI,/*<�g���\��t�����A�����J�A>[r39u1]*/
	TEKI_OBJ_02_BOSS02_HONTAI,/*<�g���\��t�����A��������>[r39u1]*/
	TEKI_OBJ_03_BOSS03_HONTAI,/*<�g���\��t�����A>[r39u1]*/
	//
	TEKI_OBJ_04_TUKAIMA00,/*<>[r39u1]*/
	TEKI_OBJ_05_TUKAIMA01,/*<>[r39u1]*/
	TEKI_OBJ_06_TUKAIMA02,/*<>[r39u1]*/
	TEKI_OBJ_07_TUKAIMA03,/*<>[r39u1]*/
	TEKI_OBJ_08_TUKAIMA04,/*<>[r39u1]*/
	TEKI_OBJ_09_TUKAIMA05,/*<>[r39u1]*/
	TEKI_OBJ_0a_TUKAIMA06,/*<>[r39u1]*/
	TEKI_OBJ_0b_TUKAIMA07,/*<>[r39u1]*/
	//


/* �G(���@�A�I�v�V�����A���e���A�e�ȊO�̑S�ẴX�v���C�g�܂�)�̍ő吔�� OBJ_POOL_02_TEKI_FIX_0256_MAX (==256) ����ȏ�o�^���悤�Ƃ��Ă��o�^����Ȃ��B */
	OBJ_POOL_02_TEKI_FIX_0256_MAX = (1<<8),/*  ��(256) */
};

enum/* [A03]�G���A#03: [A03�Œ�̈�] */
{
	/* �`��v���C�I���e�B�[�����@�e�N�X�`���[�̂��� */
	FIX_OBJ_00_PLAYER = 0,		/*<�g�p��>[r32�ύX�ς�] obj_regist_direct_number(OBJ_HEAD_01_0x0800_TOUROKU+FIX_OBJ_00_PLAYER); */
	FIX_OBJ_01_JIKI_MARU,		/*<�g�p��>[r32�ύX�ς�] obj_regist_direct_number(OBJ_HEAD_01_0x0800_TOUROKU+FIX_OBJ_01_JIKI_MARU); */
	FIX_OBJ_02_BOMBER_HONTAI,	/*<�g�p��>[r36] �{���o�[�̐e */
	FIX_O_BJ_03_SEND2,			/*[�\��]*/
	FIX_OBJ_04_JIKI_OPTION0,	/*<�g�p��>[r32�ύX�ς�] obj_regist_direct_number(OBJ_HEAD_01_0x0800_TOUROKU+FIX_OBJ_04_JIKI_OPTION0+jj); */
	FIX_OBJ_05_JIKI_OPTION1,	/*<�g�p��>[r32�ύX�ς�]*/
	FIX_OBJ_06_JIKI_OPTION2,	/*<�g�p��>[r32�ύX�ς�]*/
	FIX_OBJ_07_JIKI_OPTION3,	/*<�g�p��>[r32�ύX�ς�]*/
//
	/* �`��v���C�I���e�B�[��front�e�N�X�`���[�̂��� */
//	xxx_FIX_OBJ_08_BOSS,		/* <�����蔻��̓s����o���Ȃ�> */	/*<�g�p��>[r32�ύX�ς�] obj_regist_direct_number(OBJ_HEAD_01_0x0800_TOUROKU+FIX_OBJ_08_BOSS); */
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
	OBJ_POOL_03_KOTEI_0016_MAX/*MAX*/,/* ��p�����o�^�v���O�����������̂� 2^n �Ɍ��肷��K�v���͂Ȃ��B */
};



enum/* [A04]�G���A#04: [A04�p�l���̈�] */
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
	PANEL_OBJ_31_J01,		/* �c�莞��2���� */
	PANEL_OBJ_32_J02,		/* �c�莞��1���� */
	PANEL_OBJ_33_G03,		/* �O���C�Y5���� */
	PANEL_OBJ_34_G04,		/* �O���C�Y4���� */
	PANEL_OBJ_35_G05,		/* �O���C�Y3���� */
	PANEL_OBJ_36_G06,		/* �O���C�Y2���� */
	PANEL_OBJ_37_G07,		/* �O���C�Y1���� */
	//
	/* �p�l���p�X�v���C�g�� OBJ_POOL_04_PANEL_0056_MAX (==256) ����ȏ�o�^���悤�Ƃ��Ă��o�^����Ȃ��B */
//	OBJ_POOL_04_PANEL_0056_MAX = (256),/*(���d�l�̃���)*/
	OBJ_POOL_04_PANEL_0056_MAX/* MAX */,/* ��p�����o�^�v���O�����������̂� 2^n �Ɍ��肷��K�v���͂Ȃ��B */
};

#if 0
[A00]�G���A#00: [A00�e�̈�]
[A01]�G���A#01: [A01�o�^�G�̈�]
[A02]�G���A#02: [A02�Œ�G�̈�]
[A03]�G���A#03: [A03�Œ�̈�]
[A04]�G���A#04: [A04�p�l���̈�]
�X�v���C�g�I�u�W�F�N�g���΃I�t�Z�b�g�A�h���X�B(the sprites object struct, relative address.)
0x0000 +----------------------------------+ OBJ_HEAD_00_0x0000_TAMA
	   | [A00]�e��p�X�v���C�g(2048)	  | OBJ_POOL_00_TAMA_1024_MAX (need, 2^n)
0x0800 +----------------------------------+ OBJ_HEAD_01_0x0800_TOUROKU
	   | [A01]�o�^�G��p�X�v���C�g(256)   | OBJ_POOL_01_TOUROKU_0256_MAX (need, 2^n)
0x0900 +----------------------------------+ OBJ_HEAD_02_0x0900_TEKI_FIX
	   | [A02]�Œ�G��p�X�v���C�g(��256) | OBJ_POOL_02_TEKI_FIX_0256_MAX (need, 2^n)
0x0900 +----------------------------------+ OBJ_HEAD_03_0x0a00_KOTEI
	   | [A03]���@���Œ�X�v���C�g(16)	  | OBJ_POOL_03_KOTEI_0016_MAX (not need, 2^n)
0x0910 +----------------------------------+ OBJ_HEAD_04_0x0a10_PANEL
	   | [A04]�p�l���p�X�v���C�g(56)	  | OBJ_POOL_04_PANEL_0056_MAX (not need, 2^n)
0x0948 +----------------------------------+ OBJ_LAST_99_0x0a48_MAX
�S�X�v���C�g�I�u�W�F�N�g OBJ_LAST_99_0x0a48_MAX
//
�u�e��p�X�v���C�g�I�u�W�F�N�g�v �y�� �u�G��p�X�v���C�g�I�u�W�F�N�g�v �́A
�����o�^�v���O�����̓s����A�K�� 2^n �ł���K�v������܂��B
//
�u�Œ�X�v���C�g�I�u�W�F�N�g�v �y�� �u�p�l���p�X�v���C�g�I�u�W�F�N�g�v �́A
�A�h���X�Œ�Ȃ̂ŁA�����o�^�v���O�����������ׁA2^n�T�C�Y�ł���K�v�͂���܂���B
//
(r39u1)[A01�o�^�G�̈�]�̓X�N���v�g��������̂ŁA�����I�ɖ����Ȃ�\���������B
�o�^�G�̈���g�������̂́A��ɓ����B
������obj��128obj������Ώ\���Ȃ̂ŁA[A02�Œ�G�̈�]�̌㔼�������o�^�ł���@�\�ɂ���\��B
(r39u1)���ݔp�~���Ă��Ȃ��̂́A�A���X�e(src/game_core/boss/option_11_alice_zako_doll_type_a.c)��
[A01�o�^�G�̈�]�𗘗p���Ă���הp�~�ł��Ȃ��B
obj_regist_teki();���g�p�ł��Ȃ��B
option_11_alice_zako_doll_type_a.c
option_10_alice_zako_doll_type_b.c
option_40_aya_momiji01.c
option_41_aya_momiji02.c
//
�����̎G����[A01�o�^�G�̈�]([A02�Œ�G�̈�]�̌㔼128)�Ƃ��A
���{�X�̖{�́A�y�ђ��{�X�̔w�i�����w(�g�����V�X�e���ֈڍs)��[A02�Œ�G�̈�](�O��128�̌Œ�ʒu)��
�ړ��\��B
(�g�����V�X�e��(�X�N���v�g����g�p�\�Ɋg������)��[A02�Œ�G�̈�]�����g�p�ł��Ȃ��\��B
�o�^�@�\���X�N���v�g�ŊǗ�����ƕ��G�ɂȂ肷����(�x���Ȃ�)�ׁB)
#endif
/*(�S�X�v���C�g�̒�`)*/
/*([�̈��]�擪[���΃I�t�Z�b�g�A�h���X])*/	/*([��O�̗̈��]�ڑ�����)*/	/*([�g�p����]�̈�T�C�Y)*/
#define OBJ_HEAD_00_0x0000_TAMA 		(0) 																/* �o�^�e��p�X�v���C�g�̐擪 */
#define OBJ_HEAD_01_0x0800_TOUROKU		(OBJ_HEAD_00_0x0000_TAMA 		+OBJ_POOL_00_TAMA_1024_MAX) 		/* �o�^�G��p�X�v���C�g�̐擪 */
#define OBJ_HEAD_02_0x0900_TEKI_FIX 	(OBJ_HEAD_01_0x0800_TOUROKU 	+OBJ_POOL_01_TOUROKU_0256_MAX)  	/* �Œ�G��p�X�v���C�g�̐擪 */
#define OBJ_HEAD_03_0x0a00_KOTEI		(OBJ_HEAD_02_0x0900_TEKI_FIX	+OBJ_POOL_02_TEKI_FIX_0256_MAX) 	/* �Œ莩�@���X�v���C�g�̐擪 */
#define OBJ_HEAD_04_0x0a10_PANEL		(OBJ_HEAD_03_0x0a00_KOTEI		+OBJ_POOL_03_KOTEI_0016_MAX)		/* �Œ�p�l���p�X�v���C�g�̐擪 */
#define OBJ_LAST_99_0x0a48_MAX			(OBJ_HEAD_04_0x0a10_PANEL		+OBJ_POOL_04_PANEL_0056_MAX)		/* �S�X�v���C�g�̐� */
extern OBJ obj99[OBJ_LAST_99_0x0a48_MAX];	/* �S�X�v���C�g */

enum/*(OBJ�̎��)*/
{
	OBJECT_TYPE_00_TAMA = 0,/* �e��p�X�v���C�g */
	OBJECT_TYPE_01_TEKI,/* �G��p�X�v���C�g */
	OBJECT_TYPE_02_KOTEI,/* ���@���Œ�X�v���C�g */
	OBJECT_TYPE_03_PANEL,/* �p�l���p�X�v���C�g */
	OBJECT_TYPE_04_MAX/* �S�X�v���C�g */
};

/*---------------------------------------------------------
	�I�u�W�F�o���N�V�X�e��
--------------------------------------------------------- */
extern void reflect_sprite_spec(OBJ *sss, u32 bank_offset); /* �I�u�W�F�N�g�̃o���N�ݒ�𔽉f������B */
#endif

#endif /* _OBJ_AREA_H_ */
