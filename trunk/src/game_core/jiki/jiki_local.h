
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
---------------------------------------------------------*/

/* �I�v�V�����̃f�[�^�z�u */
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

enum	/* _player_option_type_ */
{
	OPTION_C1 = 0,
	OPTION_C2,
	OPTION_C3,
	OPTION_C4,
};

/* cg.bomber_time �� ->jyumyou�œ����ɓ��� */
#define PD_BOMBER_JYUMYOU180					(180)		/* �{���̎���==180[flame] */


/*---------------------------------------------------------
	player_local.h
---------------------------------------------------------*/

extern void player_weapon_colision_check(SPRITE *shot, int erase_shot_type);
extern SPRITE *search_teki_from_obj(void);

