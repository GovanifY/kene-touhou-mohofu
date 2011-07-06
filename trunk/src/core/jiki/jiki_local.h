
/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
---------------------------------------------------------*/

/*---------------------------------------------------------
	player_bomber.h
---------------------------------------------------------*/

//typedef struct
//{
//	int speed256;
//	int state;
//	int time_out;			/* �ݒ莞�ԓ��Ɍ������Ȃ��ꍇ�͂����܂� */
//	SPRITE *target_obj; 	/* int target_id */
//} PL_HOMING_DATA; 		/* == PL_HLASER_DATA */

#define PL_HOMING_DATA_speed256 				user_data01
/*???*/#define PL_HOMING_DATA_time_out			user_data02 		/* �ݒ莞�ԓ��Ɍ������Ȃ��ꍇ�͂����܂� */
//efine PL_HOMING_KODOMO_DATA_color256			user_data04

//	int angleCCW1024;
//	int nr; 				/* �E����(�z�[�~���O), nr==number */


//typedef struct
//{
#define PL_HOMING_KODOMO_DATA_check_x256		user_data00
#define PL_HOMING_KODOMO_DATA_check_y256		user_data01
/*???*/#define PL_HOMING_KODOMO_DATA_time_out	user_data02 		/* �ݒ莞�ԓ��Ɍ������Ȃ��ꍇ�͂����܂� */
//#define PL_HOMING_KODOMO_DATA_color256		user_data04
//	SPRITE *target_obj; 	/* int target_id */
//} PL_HOMING_KODOMO_DATA;	/* == PL_HLASER_DATA */

//typedef struct
//{
#define PL_KEKKAI_DATA_angleCCW1024 			user_data00
#define PL_KEKKAI_DATA_add_r1024				user_data01
#define PL_KEKKAI_DATA_radius					user_data02 		/* ���a(radius) */
//} PL_KEKKAI_DATA;

//typedef struct
//{
#define PL_OPTION_DATA_angleCCW1024 			user_data00 		/* �����O�Ƃ��č����(�t���)�̊p�x */
#define PL_OPTION_DATA_anime_wait				user_data01 		/* �A�j���[�V�������x�̃J�E���^ */
#define PL_OPTION_DATA_opt_anime_add_id 		user_data02 		/* �A�j���[�V���������̉��Z�l / �I�v�V����ID / (���~���A & �`���m)x offset */	/* pos */
#define PL_OPTION_DATA_opt_shot_interval		user_data03 		/* �I�v�V���������˂���V���b�g�̍X�V�Ԋu */
//	/* ���~���A & �`���m */
#define PL_OPTION_DATA_offset_x256				user_data04 		/* �~�̔��a�ƌ��p */
#define PL_OPTION_DATA_offset_y256				user_data05
#define PL_OPTION_DATA_slow_count				user_data06 		/* FORMATION_03: �������p(�D���Ȉʒu�ɒu�����) */
//�p�~#define P L_OPTION_DATA_state2			user_data04
//	int st ate1;					/* ���ʂȂ̂ŁA�����ɂ͗v��Ȃ� */
//} PL_OPTION_DATA;

	/*---------------------------------------------------------
		�e�v���C���[�̐��\��
	---------------------------------------------------------*/

#define PLAYERS8					(8)/*5*/

#define BASE_OPT_SHOT_INTERVAL0 	(PLAYERS8*0)
#define BASE_OPT_SHOT_INTERVAL1 	(PLAYERS8*1)
#define BASE_OPT_SHOT_INTERVAL2 	(PLAYERS8*2)
#define BASE_OPT_SHOT_INTERVAL3 	(PLAYERS8*3)
#define BASE_OPT_SHOT_INTERVAL4 	(PLAYERS8*4)
#define BASE_OPT_SHOT_INTERVAL5 	(PLAYERS8*5)
//
#define BASE_SPEED_ANIME			(PLAYERS8*6)
#define BASE_HIT_BOMB_DEC_WAIT		(PLAYERS8*7)	/* ��炢�{���̎󂯕t���P�\�������� */
#define BASE_STD_BOMB_STRENGTH		(PLAYERS8*8)
#define BASE_LOW_BOMB_STRENGTH		(PLAYERS8*9)
//
#define BASE_OPT_ANIM_TYPE			(PLAYERS8*10)
#define BASE_OPT_ANIM_SPEED 		(PLAYERS8*11)
//
#define BASE_MAX					(PLAYERS8*12)		/* �ő吔 */


/*---------------------------------------------------------
	�v���C���[����̂����蔻��
---------------------------------------------------------*/

enum /*_player_state_*/
{
	PLAYER_WEAPON_TYPE_00_SHOT = 0, 	/* �V���b�g�n */	/* ���e���G�ɂ��������ꍇ�Ɏ��e�����ł��� */	/* �G�e�ɂ�����ƓG�e�������Ȃ� */
	PLAYER_WEAPON_TYPE_01_BOMB, 		/* �{���n */		/* ���e���G�ɂ��������ꍇ�Ɏ��e�����ł��Ȃ� */	/* �G�e�ɂ�����ƓG�e������ */
};

/*---------------------------------------------------------
	player_shot.h
---------------------------------------------------------*/

enum /*_weapon_type_*/	/* ���탌�x�� */
{
	WEAPON_L0 = 0,		/* WEAPON_L0(P000-P008) */
	WEAPON_L1,			/* WEAPON_L1(P008-P015) */
	WEAPON_L2,			/* WEAPON_L2(P016-P031) */
	WEAPON_L3,			/* WEAPON_L3(P032-P063) */
	WEAPON_L4,			/* WEAPON_L4(P064-P127) */
	WEAPON_L5,			/* WEAPON_L5(P128)		*/
	WEAPON_L_MAX		/* �ő吔 */
};

enum
{
//	/* (�U���V���b�g) */
	REIMU_SHOT_TYPE_00 = 0,
	/* (��U���V���b�g) */
	REIMU_SHOT_TYPE_01,
	REIMU_SHOT_TYPE_02,
	REIMU_SHOT_TYPE_03,
	REIMU_SHOT_TYPE_04,
	REIMU_SHOT_TYPE_05,
	REIMU_SHOT_TYPE_06,
	REIMU_SHOT_TYPE_07,
//	/* ��U���V���b�g(�H�X�q) */
	YUYUKO_SHOT_TYPE_08,
	YUYUKO_SHOT_TYPE_09,
	YUYUKO_SHOT_TYPE_0a,
	YUYUKO_SHOT_TYPE_0b,
//	/* �j(�T�u�V���b�g) */
	NEEDLE_ANGLE_270,
	NEEDLE_ANGLE_263,	/* ���~���A �p */
	NEEDLE_ANGLE_277,	/* ���~���A �p */
	NEEDLE_ANGLE_271,	/* �H�X�q �p */
//
	REIMU_SHOT_TYPE_MAX 	/* �ő吔 */	//	NEEDLE_MAX
};

enum	/* _player_option_type_ */
{
	OPTION_C1 = 0,
	OPTION_C2,
	OPTION_C3,
	OPTION_C4,
};

/* pd.bomber_time �� ->jyumyou�œ����ɓ��� */
#define PD_BOMBER_JYUMYOU180					(180)		/* �{���̎���==180[flame] */


/*---------------------------------------------------------
	player_local.h
---------------------------------------------------------*/

extern int select_player;

extern const u8 player_fix_status[BASE_MAX];

extern void player_weapon_colision_check(SPRITE *shot, int erase_shot_type);
extern SPRITE *search_enemy_by_sprite(void);

