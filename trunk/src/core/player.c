
/*---------------------------------------------------------
	�e�v���C���[(�얲 �� ������ �� ���~���A �� �`���m �� �H�X�q)
	REIMU(A/B) MARISA(A/B/C) REMILIA CIRNO YUYUKO
	-------------------------------------------------------
---------------------------------------------------------*/

#include "game_main.h"
#include "player.h"
#include "scenario_script.h"

extern int select_player;

static	SPRITE *pd_core;
	SPRITE *pd_boss;		// [***090305		�ǉ�


int player_now_stage;		/* */
int pd_bomber_time; 		/* Use Gu */


//
	int pd_state_flag;		/* �ݒ�t���O */	// [***090116		�ǉ�
	int pd_weapon_power;	/* 0x00-0x80  (0-128 ��129�i�K==�{�ƂƓ���)   max==128==�u129�i�K�v*/
	int pd_chain_point;
//
	u32 pd_my_score;		/* �X�R�A���_ */
	u32 pd_graze_point; 	/* �O���C�Y���_ */
	int pd_bombs;			/* �{���� */
	int pd_zanki;			/* �c��`�����X */
//
	int pd_use_continue;	/* �R���e�B�j���[�� */
	int pd_count_miss;		/* �~�X�� */
	int pd_used_bomber; 	/* �{���g�p�� */
//	int pd_count_bonus; 	/* �X�y���J�[�h�{�[�i�X�� */
//
	int pd_use_kurai_bomb; 	/* ��炢�{���g�p�� */


enum /*_player_state_*/
{
	PLAYER_STATE_00_NORMAL,
	PLAYER_STATE_01_HIT_BOMB,		/* [***090125		�ǉ�:PLAYER_STATE_01_HIT_BOMB */
	PLAYER_STATE_02_SAVE_01,
	PLAYER_STATE_03_SAVE_02,
	PLAYER_STATE_04_GAME_OUT,
};

typedef struct
{
	int speed256;
	int state;
	int time_out;			/* �ݒ莞�ԓ��Ɍ������Ȃ��ꍇ�͂����܂� */
	SPRITE *target_obj; 	/* int target_id */
} PL_HOMING_DATA;			/* == PL_HLASER_DATA */

//	int angleCCW512;
//	int nr; 				/* �E����(�z�[�~���O), nr==number */


typedef struct
{
	int check_x256;
	int check_y256;
	int time_out;			/* �ݒ莞�ԓ��Ɍ������Ȃ��ꍇ�͂����܂� */
	SPRITE *target_obj; 	/* int target_id */
} PL_HOMING_KODOMO_DATA;	/* == PL_HLASER_DATA */

typedef struct
{
	int angleCCW512;
	int add_r512;
	int radius; 			/* ���a(radius) */
} PL_KEKKAI_DATA;

typedef struct
{
	int angleCCW512;				/* �����O�Ƃ��č����(�t���)�̊p�x */
	int anime_wait; 				/* �A�j���[�V�������x�̃J�E���^ */
	int opt_anime_add_id;			/* �A�j���[�V���������̉��Z�l / �I�v�V����ID / (���~���A & �`���m)x offset */	/* pos */
	int opt_shot_interval;			/* �I�v�V���������˂���V���b�g�̍X�V�Ԋu */
//
	#if 1
	/* ���~���A & �`���m */
//	int st ate1;					/* ���ʂȂ̂ŁA�����ɂ͗v��Ȃ� */
	int state2;
	int offset_x256;				/* �~�̔��a�ƌ��p */
	int offset_y256;
	int slow_count; 				/* FORMATION_03: �������p(�D���Ȉʒu�ɒu�����) */
	#endif
} PL_OPTION_DATA;

#define OPTION_ID_01_LL 	(0x1dff)/*��-1 0  c 1 1101 (0x01ff) 0001 1111 1111 */
#define OPTION_ID_02_LM 	(0x1cff)/*��-1 0  c 1 1100 (0x02ff) 0010 1111 1111 */
#define OPTION_ID_03_RM 	(0x1401)/*��+1 1  4 1 0100 (0x0401) 0100 0000 0001 */
#define OPTION_ID_04_RR 	(0x1501)/*��+1 1  4 1 0101 (0x0701) 0111 0000 0001 */

/*
	0	2	4	6	8	a	c	e
		   10  11  00  01  10  11
		   -4				4
*/
	/*---------------------------------------------------------
		�e�v���C���[�̐��\��
	---------------------------------------------------------*/
/* �얲 ����\�́F��炢�{���̎�t���Ԃ����� */
/* �`���m ����\�́F�H */

#define PLAYERS8				(8)/*5*/

#define BASE_OPT_SHOT_INTERVAL0 	(PLAYERS8*0)
#define BASE_OPT_SHOT_INTERVAL1 	(PLAYERS8*1)
#define BASE_OPT_SHOT_INTERVAL2 	(PLAYERS8*2)
#define BASE_OPT_SHOT_INTERVAL3 	(PLAYERS8*3)
#define BASE_OPT_SHOT_INTERVAL4 	(PLAYERS8*4)
#define BASE_OPT_SHOT_INTERVAL5 	(PLAYERS8*5)
//
#define BASE_SPEED_ANIME			(PLAYERS8*6)
#define BASE_HIT_BOMB_WAIT			(PLAYERS8*7)
#define BASE_STD_BOMB_STRENGTH		(PLAYERS8*8)
#define BASE_LOW_BOMB_STRENGTH		(PLAYERS8*9)
//
#define BASE_MAX					(PLAYERS8*10)		/* �ő吔 */

static const u8 player_fix_status[BASE_MAX] =
{/* REIMU(A/B) MARISA(A/B)	REMILIA YUYUKO CIRNO(A/Q) */
//
//	  14,  13,	11,   8,	8,	31,   8,   9,	 /* WEAPON_L0 / BASE_OPT_SHOT_INTERVAL �I�v�V�����V���b�g�̍X�V�Ԋu / option shot interval. */
//	  12,  11,	10,   8,	8,	28,   8,   9,	 /* WEAPON_L1 / BASE_OPT_SHOT_INTERVAL �I�v�V�����V���b�g�̍X�V�Ԋu / option shot interval. */
//	  10,	9,	 9,   8,	8,	25,   8,   9,	 /* WEAPON_L2 / BASE_OPT_SHOT_INTERVAL �I�v�V�����V���b�g�̍X�V�Ԋu / option shot interval. */
//	   8,	7,	 8,   8,	8,	22,   8,   9,	 /* WEAPON_L3 / BASE_OPT_SHOT_INTERVAL �I�v�V�����V���b�g�̍X�V�Ԋu / option shot interval. */
//	   6,	5,	 7,   8,	8,	19,   8,   9,	 /* WEAPON_L4 / BASE_OPT_SHOT_INTERVAL �I�v�V�����V���b�g�̍X�V�Ԋu / option shot interval. */
//	   4,	3,	 6,   8,	8,	16,   8,   9,	 /* WEAPON_L5 / BASE_OPT_SHOT_INTERVAL �I�v�V�����V���b�g�̍X�V�Ԋu / option shot interval. */
//
	  14,  13,	11,   4,	8,	31,   8,   9,	 /* WEAPON_L0 / BASE_OPT_SHOT_INTERVAL �I�v�V�����V���b�g�̍X�V�Ԋu / option shot interval. */
	  12,  11,	10,   4,	8,	28,   8,   9,	 /* WEAPON_L1 / BASE_OPT_SHOT_INTERVAL �I�v�V�����V���b�g�̍X�V�Ԋu / option shot interval. */
	  10,	9,	 9,   4,	8,	25,   8,   9,	 /* WEAPON_L2 / BASE_OPT_SHOT_INTERVAL �I�v�V�����V���b�g�̍X�V�Ԋu / option shot interval. */
	   8,	7,	 8,   4,	8,	22,   8,   9,	 /* WEAPON_L3 / BASE_OPT_SHOT_INTERVAL �I�v�V�����V���b�g�̍X�V�Ԋu / option shot interval. */
	   6,	5,	 7,   4,	8,	19,   8,   9,	 /* WEAPON_L4 / BASE_OPT_SHOT_INTERVAL �I�v�V�����V���b�g�̍X�V�Ԋu / option shot interval. */
	   4,	3,	 6,   3,	8,	16,   8,   9,	 /* WEAPON_L5 / BASE_OPT_SHOT_INTERVAL �I�v�V�����V���b�g�̍X�V�Ԋu / option shot interval. */
//
	   8,	8,	 2,   2,	3,	 4,   1,   1,	 /* BASE_SPEED_ANIME	   �v���C���[�̃A�j���[�V�������x */
	  16,  16,	 8,   8,	4,	12,   9,   9,	 /* BASE_HIT_BOMB_WAIT	   ��炢�{���̎�t���� / hit_bomb_wait. */
	  40,  40,	40,  40,   24,	48,   8,   8,	 /* BASE_STD_BOMB_STRENGTH �ʏ�{���̋��� / standard bomb strength. */
	 192, 192, 255, 255,   96, 160,  96,  96,	 /* BASE_LOW_BOMB_STRENGTH �ᑬ�{���̋��� / lower bomb strength. */

};
//	  5*8,	 5*8,	5*8,	 5*8,	 3*8,	6*8,	 1*8,	1*8,	/* BASE_STD_BOMB_STRENGTH �ʏ�{���̋��� / standard bomb strength. */
//	 30*8,	30*8,  48*8,	48*8,	12*8,  24*8,	12*8,  12*8,	/* BASE_LOW_BOMB_STRENGTH �ᑬ�{���̋��� / lower bomb strength. */

/*
���~���A�{��: �ʏ�{���̋���:[��==8] [�V==4]
�� 3��==((256/64)-1) [24==(8)*3]
�V 7��==((256/32)-1) [21==(3)*7]
REIMU		5,[�V==5] [��==3]
MARISA		5,
REMILIA 	3,[�V==3] [��==8]
YUYUKO		6,[�V==6] [��==4]
CIRNO		1,[�V==1] [��==4]
	�ʏ�{���̋��� / standard bomb strength.
*/

static int pd_player_status;
static int pd_save_timer;

/*---------------------------------------------------------
	�v���C���[����̂����蔻��
---------------------------------------------------------*/

enum /*_player_state_*/
{
	PLAYER_WEAPON_TYPE_00_SHOT = 0, 	/* �V���b�g�n */	/* ���e���G�ɂ��������ꍇ�Ɏ��e�����ł��� */	/* �G�e�ɂ�����ƓG�e�������Ȃ� */
	PLAYER_WEAPON_TYPE_01_BOMB, 		/* �{���n */		/* ���e���G�ɂ��������ꍇ�Ɏ��e�����ł��Ȃ� */	/* �G�e�ɂ�����ƓG�e������ */
};

static void player_weapon_colision_check(SPRITE *shot, int erase_shot_type)
{
//	SPRITE *shot;	/* ���e */
	SPRITE *tt; 	/* �ꎞ�g�p�̃e���|����(�G�X�v���C�g�A�܂��́A�G�e�X�v���C�g) */
	#define teki_obj		tt
	#define tekidan_obj 	tt
	/* �G�e�ɂ��������ꍇ�ɓG�e������(�{���n�̂�) */
	if (/*PLAYER_WEAPON_TYPE_01_BOMB==*/erase_shot_type/*erase_bullets*/)	/* �{���n�̂� */
	{
		/* ���e�ɂ��������͓̂G�e�Ȃ̂����ׂ�D */
		tekidan_obj = sprite_collision_check_444(shot, SP_GROUP_BULLETS);/* �e����p(�e��) */
		if (NULL != tekidan_obj)		/* �G�e�ɓ��������� */
		{
			tekidan_obj->type = SP_DELETE;	/* �G�e������ */
			voice_play(VOICE02_MENU_SELECT, TRACK01_EXPLODE);
		}
	}
	/* �G�ɂ��������ꍇ�ɓG������ */
	{
		/* ���e�ɂ��������͓̂G���̂Ȃ̂����ׂ�D */
		teki_obj = sprite_collision_check_SDL_teki(shot);//, (SP_GROUP_TEKI));	/*|SP_GROUP_BOSS*/	/*SP_GROUP_ENEMYS*/
		if (NULL != teki_obj)			/* �G���̂ɓ��������� */
		{
			if (NULL != (teki_obj->callback_hit_enemy)) 	/*	*/
			{
				(teki_obj->callback_hit_enemy)(teki_obj/*�G����*/, shot/*���e*/);
			}
			/* ���e���G�ɂ��������ꍇ�Ɏ��e�����ł��� */
			if (/*erase_player_tama*/PLAYER_WEAPON_TYPE_00_SHOT == erase_shot_type) 	/* �V���b�g�n�̂� */
			{
			//	/* ���e���ŕ��@������ȏꍇ */
			//	if (NULL != (shot->callback_loser))
			//	{
			//		(shot->callback_loser)(shot/*���e*/);
			//	}
				shot->type = SP_DELETE; 		/* ���e������ */
				/* �V���b�g���G�ɓ��������ꍇ�A�ł����ݓ_�����Z�B(���G�L�����ł��������ݓ_�ŉ҂���) */
				player_add_score(score(10));
			}
		}
	}
}


/*---------------------------------------------------------
	�v���C���[�A�j�e�A�V���b�g�e�̈ړ�
	�얲 �� ������ �� ���~���A �� �`���m �� �H�X�q
---------------------------------------------------------*/

static void player_move_shot(SPRITE *src)
{
	src->x256 += src->vx256;		/*fps_factor*/
	src->y256 += src->vy256;		/*fps_factor*/
	if (REMILIA==select_player) 	/* ���~���A�͂���� */
	{	u16 rand_int;
		rand_int = ra_nd();
		src->x256 -= (rand_int&0x0200);/*0x0100*/
		src->x256 += (rand_int&0x0100);/*0x0080*/
		src->x256 += (rand_int&0x0100);/*0x0080*/
	}
	else
	if (YUYUKO==select_player)		/* �H�X�q�͂���� */
	{

		src->vx256 += ((src->vx256*pd_weapon_power)>>11);
	}
	else
//	if (REIMU==select_player)		/* �얲�̉�]�V���b�g */
	if ((REIMU_B+1) > select_player)		/* �얲�̉�]�V���b�g */
	{
		if (((JIKI_SHOT_01)|SP_GROUP_SHOT_SPECIAL)==src->type)
		{
			/* �`��p�O����] */
			src->m_angleCCW512 += 16;/*deg_360_to_512CCW((10))*/
			mask512(src->m_angleCCW512);
		}
	}
	if (src->y256 < t256(-10))
	{
		src->type = SP_DELETE;
	}
	else
	{
		player_weapon_colision_check(src, PLAYER_WEAPON_TYPE_00_SHOT);
	}
}

/*---------------------------------------------------------
	�G��T���q�֐�
---------------------------------------------------------*/
extern SPRITE *sprite_list000_head;
static SPRITE *search_enemy_by_sprite(void)
{
	/* Suche Enemy-Sprite, das noch nicht verfolgt wird, liefert SPRITE-ID oder -1 */
	SPRITE *s = sprite_list000_head;/* �X�v���C�g ���X�g�̐擪 ����T�� */
	while (NULL != s)/* �X�v���C�g ���X�g�̍Ō�܂Œ��ׂ� */
	{
		if (
			#if 1
			(SP_DELETE != s->type ) && /* �폜�ς݂͔�΂� */
			#endif
			(0 != (s->type & (SP_GROUP_TEKI/*|SP_GROUP_BOSS*/))/*SP_GROUP_ENEMYS*/) 	/* �v���C���[�ɂƂ��Ă̓G(�U�R��{�X) */
		)
		{
			if (
#if 0
/* SP_FLAG_LOCK_TARGET ���̍l���͌��ׂ������ */
//				(0 == (s->flags & SP_FLAG_LOCK_TARGET)) &&							/* ����t���O�������G(�ʂ̗U���e�������Ă��Ȃ��G) */
#endif
				(s->flags & SP_FLAG_VISIBLE)										/* �g�p���̓G */
			)
			{
				#if 0/*SDL(�������W)*/
				if ((s->x256 > 0) && (s->x256 < t256(GAME_WIDTH )-((s->w128+s->w128))) &&
					(s->y256 > 0) && (s->y256 < t256(GAME_HEIGHT)-((s->h128+s->h128)))) 	/* �\����ʓ��̓G */
				#else/*Gu(���S���W)*/
				if ((s->x256 > 0) && (s->x256 < t256(GAME_WIDTH ) ) &&
					(s->y256 > 0) && (s->y256 < t256(GAME_HEIGHT) ))	/* �\����ʓ��̓G */
				#endif
				{
					return (s);/*��������*/
				}
			}
		}
		s = s->next;/*��*/
	}
	return (player);/*������Ȃ�����*/
}


/*---------------------------------------------------------
	�v���C���[�A�U���e�̈ړ�(�e)����
---------------------------------------------------------*/

static void player_move_parrent_hlaser(SPRITE *src)
{
	PL_HOMING_DATA *data;
	data=(PL_HOMING_DATA *)src->data;
//
	if (0==data->state)
	{
		/* ��]���Ȃ��琳�ʂ������B */
		#if 1
	//	/* CCW�̏ꍇ */
	//	mask512(src->m_angleCCW512/*data->angleCCW512*/);
		if (256 < src->m_angleCCW512/*data->angleCCW512*/)
				{	src->m_angleCCW512/*data->angleCCW512*/ -= deg_360_to_512CCW(/*360-*/(8));/*fps_factor*/ }	/* �E���ɉ�]������B */
		else	{	src->m_angleCCW512/*data->angleCCW512*/ += deg_360_to_512CCW(/*360-*/(8));/*fps_factor*/ }	/* �����ɉ�]������B */
		mask512(src->m_angleCCW512/*data->angleCCW512*/);
		#endif
		//
		if ( t256(1.0) < data->speed256 )
		{
			data->speed256 -= /*(int)*/t256(0.5);/*����*/
		}
		//
		#if 1
		/* CCW�̏ꍇ */
		if ((src->m_angleCCW512/*data->angleCCW512*/ >/*=*/ deg_360_to_512CCW((180-8/*4*/)/*266*/)) &&
			(src->m_angleCCW512/*data->angleCCW512*/ </*=*/ deg_360_to_512CCW((180+8/*4*/)/*274*/))) /* �قڐ���(180)���������� */
		{
			data->state++;// = SH_01;						/* ���� */
			src->m_angleCCW512/*data->angleCCW512*/ = deg_360_to_512CCW((180)); /* �����𐳖�(180)�ɌŒ肷�� */
		}
		#endif
	}
	else
	{
		/* ���̗U���{�����A���ɓ|�������H */
		if (player == data->target_obj) 	/* ������Ȃ�(���̗U���{�����|���Ă��܂����ꍇ) */
		{
			/* �^�[�Q�b�g�����A�G��T���B */
			data->target_obj = search_enemy_by_sprite();
		}
		else	/* �܂��^�[�Q�b�g�������Ă� */
		{
			if ( t256(12.0) > data->speed256)
			{
				data->speed256 += /*(int)*/t256(0.5);/*����*/
			}
			SPRITE *target; 	/* Target */
			target=data->target_obj;
			#if 0/*SDL(�������W)*/
			if (//(NULL != target) &&	/* �d�����Ă�̂Ŕr�� */
				(target->flags&SP_FLAG_VISIBLE) &&
				(target->x256 > t256(0)) &&
				(target->x256 < (t256(GAME_WIDTH))-((target->w128+target->w128))) &&
				(target->y256 > t256(0)) &&
				(target->y256 < (t256(GAME_HEIGHT))-((target->h128+target->h128))))
			#else/*Gu(���S���W)*/
			if (//(NULL != target) &&	/* �d�����Ă�̂Ŕr�� */
				(target->flags&SP_FLAG_VISIBLE) &&
				(target->x256 > t256(0)) &&
				(target->x256 < (t256(GAME_WIDTH)) ) &&
				(target->y256 > t256(0)) &&
				(target->y256 < (t256(GAME_HEIGHT)) ))
			#endif
			{
				#if (1)/*�U��(000)*/
				{int ta512;
				#if 1
				/* CCW�̏ꍇ */
				//	ta512=atan_512(target->y256-src->y256,target->x256-src->x256);
					ta512=atan_512(target->y256-src->y256,target->x256-src->x256);
					#if 0
					/* �x��U�� */
					/* ����ĂȂ� */
					if (data->angleCCW512>ta512)
					{
							 if (data->angleCCW512-ta512<deg_360_to_512CCW((175)))	{	data->angleCCW512+=deg_360_to_512CCW((8));/*fps_factor*/} /* hlaser:8, homing:9 */
						else if (data->angleCCW512-ta512>deg_360_to_512CCW((185)))	{	data->angleCCW512-=deg_360_to_512CCW((8));/*fps_factor*/} /* hlaser:8, homing:9 */
					}
					else if (data->angleCCW512<ta512)
					{
							 if (ta512-data->angleCCW512<deg_360_to_512CCW((175)))	{	data->angleCCW512-=deg_360_to_512CCW((8));/*fps_factor*/} /* hlaser:8, homing:9 */
						else if (ta512-data->angleCCW512>deg_360_to_512CCW((185)))	{	data->angleCCW512+=deg_360_to_512CCW((8));/*fps_factor*/} /* hlaser:8, homing:9 */
					}
					#else
					/* ���ڗU�� */
					src->m_angleCCW512/*data->angleCCW512*/ = ta512;
					#endif
				#endif
				}
				mask512(src->m_angleCCW512/*data->angleCCW512*/);
				#endif/*�U��(000)*/
			}
			else
			{
				data->target_obj = player;		/* ��ʊO�ɓ�����ꂽ */
			}
		}
	}
	#if 1
	/* (�����K�v�Ȃ�����)�O�̈׃}�X�N */
	mask512(src->m_angleCCW512/*data->angleCCW512*/);
	#endif
	#if 1
	/* �\���p */
//	src->m_angleCCW512		= src->m_angleCCW512/*data->angleCCW512*/;
	#endif
	#if 1
	/* CCW�̏ꍇ */
	src->x256 += ((sin512((src->m_angleCCW512/*data->angleCCW512*/))*data->speed256)>>8);/*fps_factor*/
	src->y256 += ((cos512((src->m_angleCCW512/*data->angleCCW512*/))*data->speed256)>>8);/*fps_factor*/
	#endif
	player_weapon_colision_check(src, PLAYER_WEAPON_TYPE_00_SHOT);
//
	data->time_out--;/*fps_factor*/
	if (0 > data->time_out ) /* ���Ԑ؂�H */
	{
		src->type = SP_DELETE;		/* ���Ԑ؂�Ȃ̂ł����܂� */
	}
}


/*---------------------------------------------------------
	�v���C���[�A�U���e�̈ړ�(�q��)
---------------------------------------------------------*/

static void player_move_kodomo_hlaser(SPRITE *src)
{
	#if 1
	PL_HOMING_KODOMO_DATA *data=(PL_HOMING_KODOMO_DATA *)src->data;
	SPRITE *oya;
	oya = data->target_obj;
	if (	(SP_DELETE == oya->type) || 			/* �e�������ς݂Ȃ�Ύq������ */
		//	((SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_SPECIAL) != oya->type)			/* (SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_SPECIAL)�ȊO�͐e�łȂ��̂ŏ��� */
			(0==(SP_GROUP_SHOT_SPECIAL & oya->type))	/* (SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_SPECIAL)�ȊO�͐e�łȂ��̂ŏ��� */
		)
	{
		src->type = SP_DELETE;/*�����܂�*/
	}
//
	#if 1
	/* �\���p */
	src->m_angleCCW512		= oya->m_angleCCW512;/*data->angleCCW512*/
	#endif
	src->x256 = oya->x256;
	src->y256 = oya->y256;

	/* ���t���[�������Ȃ��ꍇ�́A������������ */
	if ((data->check_x256 != src->x256) ||
		(data->check_y256 != src->y256))
	{
		data->time_out	= 8;/* �������玩���������Ȃ� */
	}
	data->time_out--;
	if ( 0 > data->time_out)
	{
		src->type = SP_DELETE;/*�����܂�*/
	}
	data->check_x256 = src->x256;
	data->check_y256 = src->y256;
	#endif
}


/*---------------------------------------------------------
	�얲��p �ᑬ�{��
---------------------------------------------------------*/

enum
{
	LL_00 = 0,
	LL_01,
};

static void move_reimu_musou_fuuin(SPRITE *src)
{
	PL_HOMING_DATA *data;
	data=(PL_HOMING_DATA *)src->data;
//
	/* ���̗U���{�����A���ɓ|�������H */
	if (player == data->target_obj) 	/* ������Ȃ�(���̗U���{�����|���Ă��܂����ꍇ) */
	{
		/* �^�[�Q�b�g�����A�G��T���B */
		data->target_obj = search_enemy_by_sprite();
	}
	else	/* �܂��^�[�Q�b�g�������Ă� */
	{
		if ( t256(3.0) > data->speed256)
		{
			data->speed256 += /*(int)*/t256(0.5);/*����*/
		}
		SPRITE *target;
		target=data->target_obj;
		#if 0/*SDL(�������W)*/
		if (
			(target->flags&SP_FLAG_VISIBLE) &&
			(target->x256 > t256(0)) &&
			(target->x256 < (t256(GAME_WIDTH))-((target->w128+target->w128))) &&
			(target->y256 > t256(0)) &&
			(target->y256 < (t256(GAME_HEIGHT))-((target->h128+target->h128))))
		#else/*Gu(���S���W)*/
		if (
			(target->flags&SP_FLAG_VISIBLE) &&
			(target->x256 > t256(0)) &&
			(target->x256 < (t256(GAME_WIDTH)) ) &&
			(target->y256 > t256(0)) &&
			(target->y256 < (t256(GAME_HEIGHT)) ))
		#endif
		{
		#if (1)/*�U��(000)*/
			{int ta512;
		#if 1
		/* CCW�̏ꍇ */
		//		ta512=atan_512(target->y256-src->y256,target->x256-src->x256);
				ta512 = atan_512(target->y256-src->y256, target->x256-src->x256);
		//		ta512 = (atan_512(src->ty256-src->y256, src->tx256-src->x256));
				#if 1
				/* �x��U�� */
				#if 0/* KETM�͍̂l���������G�Ȃ̂Ŕj���A0�x����̍����ōl��180�x�����ɕ��򂷂�Γ���(1����360�x�����Ȃ��̂�) */
				/* ����ĂȂ� */
			//	if (data->angleCCW512>ta512)
			//	{
			//			 if (data->angleCCW512-ta512<deg_360_to_512CCW((175)))	{	data->angleCCW512+=deg_360_to_512CCW((8));/*fps_factor*/} /* hlaser:8, homing:9 */
			//		else if (data->angleCCW512-ta512>deg_360_to_512CCW((185)))	{	data->angleCCW512-=deg_360_to_512CCW((8));/*fps_factor*/} /* hlaser:8, homing:9 */
			//	}
			//	else if (data->angleCCW512<ta512)
			//	{
			//			 if (ta512-data->angleCCW512<deg_360_to_512CCW((175)))	{	data->angleCCW512-=deg_360_to_512CCW((8));/*fps_factor*/} /* hlaser:8, homing:9 */
			//		else if (ta512-data->angleCCW512>deg_360_to_512CCW((185)))	{	data->angleCCW512+=deg_360_to_512CCW((8));/*fps_factor*/} /* hlaser:8, homing:9 */
			//	}
				#endif
			//	const int sabun_aaa = (src->tmp_angleCCW512-src->m_angleCCW512);
				const int sabun_aaa = (ta512-src->m_angleCCW512);
				if (0 > sabun_aaa/*(src->tmp_angleCCW512-src->m_angleCCW512)*/ )/* 0���W�X�^+���ʍœK�� */
				{	/* �ڕW�p�x < ����p�x */
					if (256 > sabun_aaa/*(src->tmp_angleCCW512-src->m_angleCCW512)*/ )/* ���ʍœK�� */
							{	src->m_angleCCW512 -= 5;	}	/* 180�x�ȓ��Ȃ�߂��ق������� */
					#if 0	/* �ȗ����Ė��Ȃ��H */
					else	{	src->m_angleCCW512 += 5;	}	/* 180�x�ȏ�Ȃ甽�Ύ���̂ق����߂� */
					#endif
				}
				else	/* �ڕW�p�x > ����p�x */
				{
					if (256 > -(sabun_aaa)/*(src->m_angleCCW512-src->tmp_angleCCW512)*/ )
							{	src->m_angleCCW512 += 5;	}	/* 180�x�ȓ��Ȃ�߂��ق������� */
					#if 0	/* �ȗ����Ė��Ȃ��H */
					else	{	src->m_angleCCW512 -= 5;	}	/* 180�x�ȏ�Ȃ甽�Ύ���̂ق����߂� */
					#endif
				}
				#else
				/* ���ڗU�� */
				src->m_angleCCW512/*data->angleCCW512*/ = ta512;
				#endif
		#endif
			}
			mask512(src->m_angleCCW512/*data->angleCCW512*/);
		//	tg->x=target->x+((target->w-tg->w)>>1)+ra_nd()%6-3; /* �^�[�Q�b�g�ڕW�\���}�[�J�[ */
		//	tg->y=target->y+((target->h-tg->h)>>1)+ra_nd()%6-3; /* �^�[�Q�b�g�ڕW�\���}�[�J�[ */
		#endif/*�U��(000)*/
		}
		else
		{
			/* Ziel wurde anderweitig vernichtet, neues Ziel suchen */
			data->target_obj = player;		/* ��ʊO�ɓ�����ꂽ */
		}
	}


//
	#if 1
	/* (�����K�v�Ȃ�����)�O�̈׃}�X�N */
	mask512(src->m_angleCCW512/*data->angleCCW512*/);
	#endif
	#if 1
	/* �\���p */
//	src->m_angleCCW512		= src->m_angleCCW512/*data->angleCCW512*/;
	#endif

#define reimu_musou_id time_out
#if 1
/* CCW�̏ꍇ */
	if((pd_bomber_time&0x0f)==(data->reimu_musou_id&0x0f))
	{
		src->vx256 = ((sin512((src->m_angleCCW512/*data->angleCCW512*/))*data->speed256)>>8);/*fps_factor*/
		src->vy256 = ((cos512((src->m_angleCCW512/*data->angleCCW512*/))*data->speed256)>>8);/*fps_factor*/
	}
#endif
		src->x256 += (src->vx256);/*fps_factor*/
		src->y256 += (src->vy256);/*fps_factor*/
	player_weapon_colision_check(src, PLAYER_WEAPON_TYPE_01_BOMB/*PLAYER_WEAPON_TYPE_00_SHOT*/);
//
	if (0 >= pd_bomber_time) 	/* ���Ԑ؂�̏ꍇ */
	{
		gamen_gai_nara_osimai(src);/* ��ʊO�Ȃ炨���܂� */
		src->base_weapon_strength--;/* ���Ԑ؂�̏ꍇ�A�̗͂����炷 */
	}
//
	if (0 > src->base_weapon_strength)	{	src->type = SP_DELETE;	}	/* �̗͐؂�ł����܂� */
}
//
//	if (0 >= pd_bomber_time) {	src->type = SP_DELETE;}/* ���Ԑ����ł����܂� */
//	data->time_out--;/*fps_factor*/
//	if (0 > data->time_out ) /* ���Ԑ؂�H */
//	{
//		src->type = SP_DELETE;		/* ���Ԑ؂�Ȃ̂ł����܂� */
//	}

/*---------------------------------------------------------
	�v���C���[�A�������{���̈ړ�(�얲)
---------------------------------------------------------*/

/*
	src->x		�V�[���h��x���W
	player->x	�v���C���[��x���W
	player->w/2 �v���C���[�̉����̔���
	co_s512((data->angle512))	angle�̊p�x���΂ɒ��������Ƃ̃R�T�C���B���̏ꍇ��40���̔{���{�P�O*fps�̃R�T�C���B
	data->radius	�~�̔��a40
	src->w/2		�V�[���h�̉����̔���
	�v���C���[��x���W�{�v���C���[�̉����̔������v���C���[�̒��S���W
	�Ȃ̂�
	�V�[���h��x���W���v���C���[��x���W�{�v���C���[�̉����̔����{co s(radian((�S�O��*x)�{speed*fps))�~40�|�V�[���h�̉����̔���
				   ���v���C���[�̒��S���W�{co s(radian((�S�O��*x)�{speed*fps))�~40�|�V�[���h�̉����̔���
*/
// ���v��� �� �����v��� ���p
static void player_move_kekkai(SPRITE *src) /* �얲 */
{
	PL_KEKKAI_DATA *data	= (PL_KEKKAI_DATA *)src->data;
	data->angleCCW512 += (data->add_r512);	/*fps_factor*/	/*...*/
	mask512(data->angleCCW512);// if (data->angle>360) {	data->angle-=360;}
	#if 1
	/* �`��p�p�x(����0�x�ō����(�����v���)) */
	src->m_angleCCW512	= (data->angleCCW512);
	#endif
//
	/* CCW�̏ꍇ */
	#if 0/*SDL(�������W)*/
	src->x256 = player->x256 + sin512((data->angleCCW512))*data->radius+((player->w128-src->w128));
	src->y256 = player->y256 + cos512((data->angleCCW512))*data->radius+((player->h128-src->h128));
	#else/*Gu(���S���W)*/
	src->x256 = player->x256 + sin512((data->angleCCW512))*data->radius;
	src->y256 = player->y256 + cos512((data->angleCCW512))*data->radius;
	#endif

//	if (REIMU==select_player)
	{
		if (data->radius<255)	{	data->radius += (ra_nd()&1);	}	/* �������L����悤�ɂ��� */
		else					{	src->type = SP_DELETE;			}	/* ��]���L�������炨���܂� */
	//	src->alpha				= ((int)(255-data->radius))&(256-1);	/* ���񂾂������悤�ɂ��� */
	//	src->color32			= ((((255-data->radius))&(0xff))<<24)|0x00ffffff;	/* ���񂾂������悤�ɂ��� */
		src->color32			= ((((255-data->radius))&(0xff))<<24)|0x00ffffff;	/* ���񂾂������悤�ɂ��� */
	}
//	else //if (YUYUKO==select_player)
//	{

//		if (pd_bomber_time<=0) {	src->type = SP_DELETE;}/* ���Ԑ����ł����܂� */
//	}
	if(0==((data->radius)&7))
	{
		//player_weapon_colision_check_kekkai(src, data->radius);
		bullets_to_hosi();/* �e�S���A���A�C�e���ɂ��� */
	}
	player_weapon_colision_check(src, PLAYER_WEAPON_TYPE_01_BOMB);/* �{���œG��|�������蔻�� */
}


/*---------------------------------------------------------
	�v���C���[�A�������{���B��ʊO�͂ݏo���`�F�b�N�B�����蔻��`�F�b�N�B
---------------------------------------------------------*/
static void player_bomber_out_colision_check(SPRITE *src)
{
	#if 0/*SDL(�������W)*/
	if ((src->x256+((src->w128+src->w128)) < t256(0))||(src->x256 > t256(GAME_WIDTH))||
		(src->y256+((src->h128+src->h128)) < t256(0))||(src->y256 > t256(GAME_HEIGHT)))
	#else/*Gu(���S���W)*/
	if ((src->x256 < t256(0))||(src->x256 > t256(GAME_WIDTH))||
		(src->y256 < t256(0))||(src->y256 > t256(GAME_HEIGHT)))
	#endif
	{
		src->type = SP_DELETE;/* ��ʊO�ɏo���炨���܂� */
	}
	else
	{
		player_weapon_colision_check(src, PLAYER_WEAPON_TYPE_01_BOMB);/* �{���œG��|�������蔻�� */
	}
}

/*---------------------------------------------------------
	�v���C���[�A�������{���̈ړ�(�������A�`���m�A���H�X�q(�Ⴄ�����ɂ�����) )
---------------------------------------------------------*/

static void player_move_levarie(SPRITE *src)
{
	src->x256 += src->vx256;	/*fps_factor*/
	src->y256 += src->vy256;	/*fps_factor*/
	#if 1
	/* YUYUKO MARISA_A �ȊO */
	if (
	//	(CIRNO_A==select_player) || (CIRNO_Q==select_player) ||
		((CIRNO_A-1)<select_player) ||
		(MARISA_B==select_player))/* �}�X�p(���ǂ�) */
	#endif
	{	/*�`���m�p(�b��I)*/	/*1.5*(d->speed);*/ 	/*fps_factor*/
		src->y256 -= (abs((src->vx256+(src->vx256>>1) )));
		src->y256 -= (abs((src->vy256+(src->vy256>>1) )));
		src->y256 -= t256(1.0);
	}
	{
		static const u8 aaa_sss[16] =
		{/* REIMU(A/B) MARISA(A/B) REMILIA YUYUKO CIRNO(A/Q) */
			0, 0, 2, 8, 0, 0,  9,  9,	/* ��]���x */	/* YUYUKO			�ȊO�� ��] ����B */
			0, 0, 2, 0, 0, 0,  3,  3,	/* �g�呬�x */	/* YUYUKO  MARISA_B �ȊO�� �g�� ����B */
		};
		/* �\���p�x(����0�x�ō����(�����v���)) */
		src->m_angleCCW512		+= aaa_sss[select_player];
		mask512((src->m_angleCCW512));
		if ( t256(4.0) > src->m_zoom_x256)
		{
			src->m_zoom_x256 += aaa_sss[select_player+(PLAYERS8)];
		}
	}
	player_bomber_out_colision_check(src);
}

/*---------------------------------------------------------
	�v���C���[�A�������{���̈ړ�(���~���A)
---------------------------------------------------------*/

static void remilia_move_burn_fire(SPRITE *src)
{
	#if 1
	// ����
	// x1.10
//	src->vx256 = ((src->vx256 * t256(1.1))>>8);//co_s512((data->angle512))*p->speed;/*fps_factor*/
//	src->vy256 = ((src->vx256 * t256(1.1))>>8);//si_n512((data->angle512))*p->speed;/*fps_factor*/
	// x1.125
	src->vx256 += ((src->vx256)>>3);//co_s512((data->angle512))*p->speed;/*fps_factor*/
	src->vy256 += ((src->vy256)>>3);//si_n512((data->angle512))*p->speed;/*fps_factor*/
	// x1.0625
//	src->vx256 += ((src->vx256)>>4);//co_s512((data->angle512))*p->speed;/*fps_factor*/
//	src->vy256 += ((src->vy256)>>4);//si_n512((data->angle512))*p->speed;/*fps_factor*/
	/* �`�悪�d������̂ŉ������\�A�኱����( x1.0625�� x1.125)���� */
	//
	#endif
	src->x256 += src->vx256;	//	src->vx256;//co_s512((data->angle512))*p->speed;/*fps_factor*/
	src->y256 += src->vy256;	//	src->vy256;//si_n512((data->angle512))*p->speed;/*fps_factor*/
//	if ((src->x256+((src->w128+src->w128)) < t256(0))||(src->x256 > t256(GAME_WIDTH))||
//		(src->y256+((src->h128+src->h128)) < t256(0)))
//
	player_bomber_out_colision_check(src);
}


/* �\�����{���̉��̕��� */
/*		r_or_l	[xxx_r] l=2, [xxx_l] r=0 ,u=3 ,d=1	*/
static void remilia_add_burn_fire(SPRITE *src/*, int ggg*/ /*r_or_l*/)	/* [***090221 �ǉ� */
{
	int angCCW512;
	for (angCCW512=0; angCCW512<512; angCCW512+=128/*10*/)
	{
		int i;
		for (i=0; i<10; i+=1)
		{
			SPRITE *s;
			s = sprite_add_gu(JIKI_ATARI_ITEM_16);
			//
			s->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);
			s->type 			= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_04|SP_GROUP_SHOT_SPECIAL)/*�{�X�ɗL��*/;/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) �{�X�ɖ���*/
			s->callback_mover	= remilia_move_burn_fire;
			s->color32			= 0x64ffffff;		/*	s->alpha			= 0x64 100;*/
			#if 0/*SDL(�������W)*/
			s->x256 			= src->x256+((src->w128-s->w128));
			s->y256 			= src->y256+t256(15);
			#else/*Gu(���S���W)*/
			s->x256 			= src->x256;
			s->y256 			= src->y256+t256(15);
			#endif
			/*data->*/s->base_weapon_strength		= (8*(5));/* �d�l�ύX���� */	/*9*/ /*10*/
			{
			//	int rnd_spd 		= (ra_nd() & 0xff/*angCCW512*/)+256+((angCCW512+32+64) & 0x40);
			//	int aaa_spd 		= (((angCCW512+32+64) & 0x40));
			//	int rnd_spd 		= (ra_nd() & 0xff/*angCCW512*/)+256+(aaa_spd+aaa_spd);
		u16 my_rand;
			my_rand = ra_nd();
		int rnd_spd 		= (my_rand & 0xff/*angCCW512*/)+256+(0x40+0x40);
		int ang_aaa_512;
			ang_aaa_512 = angCCW512;
			ang_aaa_512 += ((my_rand>>8) & (0x3f));
			ang_aaa_512 += ((my_rand) & (0x1f));
		//	ang_aaa_512 -= (0x1f);
		//	ang_aaa_512 -= (0x0f);
			ang_aaa_512 -= (0x2f);
			mask512(ang_aaa_512);
			#if 1
			/* �`��p�p�x(����0�x�ō����(�����v���)) */
			s->m_angleCCW512		= ang_aaa_512;
			#endif
			#if 1
			/* CCW�̏ꍇ */
			s->vx256		= ((sin512((/*data->angle512*/ang_aaa_512))*/*data->speed*/(rnd_spd))>>8);/*fps_factor*/
			s->vy256		= ((cos512((/*data->angle512*/ang_aaa_512))*/*data->speed*/(rnd_spd))>>8);/*fps_factor*/
			#endif
			}
		}
	}
}


/*---------------------------------------------------------
	�X�y�J�������̗����G
---------------------------------------------------------*/

static void player_move_tachie(SPRITE *src) 	/* [***100113 �ǉ� */
{
	if ((128-1) < pd_bomber_time)/* �c�莞�Ԃ� 128 �ȏ�Ȃ�A�ړ����đҋ@ */
	{
		/* ��ʊO����o���A��~�B���΂��҂B */
		if ( t256(100) > src->x256 )
		{
			src->x256 += t256(5.0);
		}
	}
	else
	if ((1/*-1*/) < pd_bomber_time)/* �g�� */
	{
		/* (24+(1)) , �����ł� pd_bomber_time �́A�ő�0x7f�Ȃ̂ŁB */
		src->color32		= (pd_bomber_time<<(24+(1)))|0x00ffffff;
		src->m_zoom_x256	+= (10);
	}
	else
	{
		src->type = SP_DELETE;	/* ���Ԑ����ł����܂� */
	}
}


/*---------------------------------------------------------
	���~���A�{���̏\����
---------------------------------------------------------*/

static void player_move_add_cross_red(SPRITE *src)	/* [***090220 �ǉ� */
{

	//
//	if (0x3f==(((int)pd_bomber_time)&0x3f))
//	if (0x0f==(((int)pd_bomber_time)&0x0f))
	if (0x0f==(((int)pd_bomber_time)&0x0f))/* �g�������^�C�~���O */
	{
		remilia_add_burn_fire(src);/* �g������(�������t?) */
	}
	#if 0/*SDL(�������W)*/
	src->x256 = player->x256+((player->w128-src->w128));
	src->y256 = player->y256+((player->h128-src->h128));
	#else/*Gu(���S���W)*/
	src->x256 = player->x256;
	src->y256 = player->y256;
	#endif

	if (0 >= pd_bomber_time) { src->type = SP_DELETE;}/* ���Ԑ����ł����܂� */
}

/* ====================================================================================== */
/* ====================================================================================== */
/* ====================================================================================== */

/*---------------------------------------------------------
	�v���C���[�e(�V���b�g/�j���p)�����A�q�֐�
	�v���C���[�A�j�e(�얲)�̒ǉ�	 [***090220 �ǉ�
		(�얲)		[�j�e]�A
		(������)	[�X�e]�A
		(���~���A)	[�R�E�����e]�A
		(�H�X�q)	[���e] �A
		(���`���m)	[�ەX�e]�A
---------------------------------------------------------*/

static int weapon_level_offset; /* �����������탌�x��(�ԍ�)�̂W�{�Ƀv���C���[�l�𑫂����l���I�t�Z�b�g�Ƃ��ĕێ� */
/*
	weapon_level_offset == player�ԍ� + (���탌�x�� x 8)
	weapon_level_offset =  select_player + (weapon_level<<3);
*/

enum /*_weapon_type_*/	/* ���탌�x�� */
{
	WEAPON_L0 = 0,		/* WEAPON_L0(P000-P008) */
	WEAPON_L1,			/* WEAPON_L1(P008-P015) */
	WEAPON_L2,			/* WEAPON_L2(P016-P031) */
	WEAPON_L3,			/* WEAPON_L3(P032-P063) */
	WEAPON_L4,			/* WEAPON_L4(P064-P127) */
	WEAPON_L5,			/* WEAPON_L5(P128)		*/
	WEAPON_L_MAX				/* �ő吔 */
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

enum
{
	REI00_x_offset=0,
	REI01_y_offset,
	REI02_vx256,
	REI03_status,	/* 2==�얲�̉�]�V���b�g */
	REI04_MAX		/* �ő吔 */
};
#define TEST_OFFSET (8)

static void player_register_shot_object(SPRITE *src, int /*set_*/shot_type)
{
	static const s16 r_tbl[REIMU_SHOT_TYPE_MAX][REI04_MAX] =
	{	/*x_offset256	y_offset256  vx256 status */
//	/* �U���V���b�g(�얲 �� ������) */
/*00*/	{t256(	0), t256(0/*+25*/),  (	0), 0x01},	 /*shot_regist_triple_fuda*/ /*(-24)==(4*(- 6))*/
//	/* ��U���V���b�g(�얲) */
/*01*/	{t256(TEST_OFFSET+	0), t256( 20),	(  0), 0x00},	/* �� �܂����� */
/*02*/	{t256(TEST_OFFSET  -5), t256( 20),	(  0), 0x00},	/* �� �܂����� */
/*03*/	{t256(TEST_OFFSET  +5), t256( 20),	(  0), 0x00},	/* �E �܂����� */
/*04*/	{t256(TEST_OFFSET  -5), t256(+20),	(-20), 0x00},	/* �� ���� */
/*05*/	{t256(TEST_OFFSET  +5), t256(+20),	( 20), 0x00},	/* �E �E�� */
/*06*/	{t256(TEST_OFFSET  -5), t256(+20),	(-35), 0x00},	/* �� ���Ώ� */
/*07*/	{t256(TEST_OFFSET  +5), t256(+20),	( 35), 0x00},	/* �E �E�Ώ� */
//	/* ��U���V���b�g(�H�X�q) */
/*08*/	{t256(TEST_OFFSET -15), t256(-25),	(-25), 0x00},	/* �H�X�q */					/*(-32)==(4*(- 8))*/
/*09*/	{t256(TEST_OFFSET +15), t256(-25),	(+25), 0x00},	/* �H�X�q */					/*( 32)==(4*(  8))*/
/*0a*/	{t256(TEST_OFFSET -25), t256(-25),	(-50), 0x00},	/* �H�X�q */					/*(-64)==(4*(-16))*/
/*0b*/	{t256(TEST_OFFSET +25), t256(-25),	(+50), 0x00},	/* �H�X�q */					/*( 64)==(4*( 16))*/
//	/* �j(�T�u�V���b�g) */		/* CCW�̏ꍇ */
		{t256(	0), t256( -7),	(	0), 0x01},//	deg_360_to_512CCW((180)),					/* ������ */	/* ������B */	/* �얲 */	/* ���~���A �� �`���m */
		{t256(	0), t256( -7),	(-108), 0x01},//	deg_360_to_512CCW((180-25)),/*263=270-7*/	/* ���~���A �p */	/* ���~���A �� �`���m */
		{t256(	0), t256( -7),	( 108), 0x01},//	deg_360_to_512CCW((180+25)),/*277=270+7*/	/* ���~���A �p */	/* ���~���A �� �`���m */
		{t256(	0), t256(-47),	(	5), 0x01},//	deg_360_to_512CCW((181)),					/* �H�X�q �p */ 	/* �H�X�q */	/*-48*/
	};
//	int shot_type;
	int shot_status;/*�j?*/
	shot_status = ((r_tbl[shot_type][REI03_status]));

	int is_needle;/*�j?*/
//	is_needle = (shot_type <= NEEDLE_ANGLE_270)?(1/*�j*/):(0/*�e*/);
	is_needle = ((shot_status)&0x01);
//
	SPRITE *s;/* shot */

#define OFFS_IS_NEEDLE (select_player+(is_needle<<3))

	s					= sprite_add_gu( BASE_SHOT_ATARI_0a+OFFS_IS_NEEDLE );/* �����蔻��̑傫�� */

//	s->type 			= (/*SP_GROUP_JIKI_GET_ITEM*/JI KI_SHOT_02|SP_GROUP_SHOT_SPECIAL)-shot_status;
	#if 1
//	else
//	if(0x01== shot_status)	/* �j */
	{
		s->type 			= (JIKI_SHOT_07|SP_GROUP_SHOT_SPECIAL); 	/*SP_GROUP_JIKI_GET_ITEM*/	/*SP_PL_FIREBALL*/
	}
//
	if (0x00 == shot_status)	/* �j�ȊO */
	{
		if (REIMU_B+1 > select_player)	/* �얲? */
		{
		//	if(0x02== shot_status)	/* �얲�̉�]�V���b�g */
			if (REIMU_SHOT_TYPE_00 != shot_type)/* �����V���b�g�ȊO�H */
			{
				s->type 			= (JIKI_SHOT_01|SP_GROUP_SHOT_SPECIAL); 	/*SP_GROUP_JIKI_GET_ITEM*/
				/* �`��p�O����] */
				s->m_angleCCW512 =	(ra_nd() & 0x1ff);/* �o�����̊p�x�̓����_���łȂ���(���[�U�[�݂�����)�ςȉ�ʂɂȂ� */
			}
		/*	else	{	s->type = �����V���b�g�́A�j�Ɠ���; 	}	*/
		}
		else
		{
			s->type 			= (JIKI_SHOT_02|SP_GROUP_SHOT_SPECIAL); 	/*SP_GROUP_JIKI_GET_ITEM*/
		}
	}
//	else
	#endif
	#if 0/*SDL(�������W)*/
	s->x256 			= (src->x256)+((src->w128-src->w128))	+ r_tbl[shot_type][REI00_x_offset]; /*+ x_offs*/
	s->y256 			= (src->y256)-((src->h128+src->h128))	+ r_tbl[shot_type][REI01_y_offset];/*(20)*/
	#else/*Gu(���S���W)*/
	s->x256 			= (src->x256) + r_tbl[shot_type][REI00_x_offset]; /*+ x_offs*/
	s->y256 			= (src->y256) + r_tbl[shot_type][REI01_y_offset];/*(20)*/
	#endif
	s->callback_mover	= player_move_shot;
	s->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);
	s->color32			= 0x96ffffff;	/*	s->alpha		= 0x96; 150 �������ɂ���B */	/*shot_regist_triplestar*/
//
	#define CCC_STRENGTH_L1 	(PLAYERS8* 0)/* WEAPON_L0 */
	#define CCC_STRENGTH_L2 	(PLAYERS8* 1)/* WEAPON_L1 */
	#define CCC_STRENGTH_L3 	(PLAYERS8* 2)/* WEAPON_L2 */
	#define CCC_STRENGTH_L4 	(PLAYERS8* 3)/* WEAPON_L3 */
	#define CCC_STRENGTH_L5 	(PLAYERS8* 4)/* WEAPON_L4 */
	#define CCC_STRENGTH_L6 	(PLAYERS8* 5)/* WEAPON_L5 */
	#define CCC_SPEED			(PLAYERS8* 6)
	#define DDD_SPEED			(PLAYERS8* 7)
//
	#define DDD_STRENGTH_L1 	(PLAYERS8* 8)/* WEAPON_L0 */
	#define DDD_STRENGTH_L2 	(PLAYERS8* 9)/* WEAPON_L1 */
	#define DDD_STRENGTH_L3 	(PLAYERS8*10)/* WEAPON_L2 */
	#define DDD_STRENGTH_L4 	(PLAYERS8*11)/* WEAPON_L3 */
	#define DDD_STRENGTH_L5 	(PLAYERS8*12)/* WEAPON_L4 */
	#define DDD_STRENGTH_L6 	(PLAYERS8*13)/* WEAPON_L5 */
//	#define DDD_MAX 			(PLAYERS8*2)		/* �ő吔 */
	#define CCC_MAX 			(PLAYERS8*14)		/* �ő吔 */
	static const u8/*u8*/ ccc_tbl[CCC_MAX] =
	{/* REIMU(A/B) MARISA(A/B) REMILIA YUYUKO CIRNO(A/Q) */
//		 5*8, 5*8,	5*8, 5*8,  5*8,16*8,  5*8, 5*8, 	/* WEAPON_L0  strength �V���b�g�̋��� */ /* (L1�V���b�g�́A�e�������Ȃ��̂� 5�ȉ��ɂ��Ȃ�) */
//		 2*8, 3*8,	3*8, 2*8,  4*8,16*8,  5*8, 5*8, 	/* WEAPON_L1  strength �V���b�g�̋��� */
//		 2*8, 3*8,	3*8, 1*8,  4*8,16*8,  5*8, 5*8, 	/* WEAPON_L2  strength �V���b�g�̋��� */
//		 2*8, 3*8,	3*8, 1*8,  4*8,16*8,  5*8, 5*8, 	/* WEAPON_L3  strength �V���b�g�̋��� */
//		 2*8, 3*8,	3*8, 1*8,  4*8,16*8,  5*8, 5*8, 	/* WEAPON_L4  strength �V���b�g�̋��� */
//		 2*8, 3*8,	3*8, 1*8,  4*8,16*8,  5*8, 5*8, 	/* WEAPON_L5  strength �V���b�g�̋��� */
//
		 5*8, 5*8,	5*8, 5*8,  5*8,16*8,  5*8, 5*8, 	/* WEAPON_L0  strength �V���b�g�̋��� */ /* (L1�V���b�g�́A�e�������Ȃ��̂� 5�ȉ��ɂ��Ȃ�) */
		 2*8, 3*8,	3*8, 3*8,  4*8,16*8,  5*8, 5*8, 	/* WEAPON_L1  strength �V���b�g�̋��� */
		 2*8, 3*8,	3*8, 3*8,  4*8,16*8,  5*8, 5*8, 	/* WEAPON_L2  strength �V���b�g�̋��� */
		 2*8, 3*8,	3*8, 3*8,  4*8,16*8,  5*8, 5*8, 	/* WEAPON_L3  strength �V���b�g�̋��� */
		 2*8, 3*8,	3*8, 3*8,  4*8,16*8,  5*8, 5*8, 	/* WEAPON_L4  strength �V���b�g�̋��� */
		 2*8, 3*8,	3*8, 3*8,  4*8,16*8,  5*8, 5*8, 	/* WEAPON_L5  strength �V���b�g�̋��� */

		16,14, 10,12,  9, 4,  8, 8, 	/* speed	�V���b�g�̑��� (�x��������ʏ�̒e����������̂ŋ����B�������d���Ȃ�) */
		18,18, 10,10,  3, 4,  3, 3, 	/* speed	�j�̑��� (�x��������ʏ�̒e����������̂ŋ����B�������d���Ȃ�) */
//
		 4*8, 4*8,	4*8,20*8,  6*8, 8*8,  6*8, 6*8, 	/* WEAPON_L0  strength �j�̋��� */	/*REMILIA, CIRNO,  6 5 ��������*/
		 4*8, 4*8,	4*8,20*8,  6*8, 8*8,  6*8, 6*8, 	/* WEAPON_L1  strength �j�̋��� */	/*REMILIA, CIRNO,  6 5 ��������*/
		 4*8, 4*8,	4*8,20*8,  6*8, 8*8,  6*8, 6*8, 	/* WEAPON_L2  strength �j�̋��� */	/*REMILIA, CIRNO,  6 5 ��������*/
		 4*8, 4*8,	4*8,20*8,  6*8, 8*8,  6*8, 6*8, 	/* WEAPON_L3  strength �j�̋��� */	/*REMILIA, CIRNO,  6 5 ��������*/
		 4*8, 4*8,	4*8,20*8,  6*8, 8*8,  6*8, 6*8, 	/* WEAPON_L4  strength �j�̋��� */	/*REMILIA, CIRNO,  6 5 ��������*/
		 4*8, 4*8,	4*8,20*8,  6*8, 8*8,  6*8, 6*8, 	/* WEAPON_L5  strength �j�̋��� */	/*REMILIA, CIRNO,  6 5 ��������*/
/* [***20090822 REIMU 3 <- 2 (�Ƃ肠�����j�シ����̂�) */
	};

//		 2, 3,	3, 2,  4,16, 5, 5,	/* strength �V���b�g�̋��� */
//		16,14, 10,12,  9, 4, 8, 8,	/* speed	�V���b�g�̑��� (�x��������ʏ�̒e����������̂ŋ����B�������d���Ȃ�) */
	//};
//		 2, 2, 3, 3, 4, 3, 3, 3,	/* strength �V���b�g�̋��� */
//		15,15,12,12,18,12,12,12,	/* speed	�V���b�g�̑���	*/
/* �ɒ[�ɋ@�̂̐��\���΂�̂͗ǂ��Ȃ��B(�����đ����̂͂��邢) */
	//static const u8 ddd_tbl[DDD_MAX] =
	//{/* REIMU(A/B) MARISA(A/B) REMILIA YUYUKO CIRNO(A/Q) */
//		 3, 3, 4, 4, 6, 8, 6, 6,	/* strength �j�̋��� */ 	/*REMILIA, CIRNO,  6 5 ��������*/
/* �얲(�������Ă݂� [***20090930 ) */
//		 4, 4, 4,10, 6, 8, 6, 6,	/* strength �j�̋��� */ 	/*REMILIA, CIRNO,  6 5 ��������*/
//		 4, 4, 4,25, 6, 8, 6, 6,	/* strength �j�̋��� */ 	/*REMILIA, CIRNO,  6 5 ��������*/
//		18,18,10,15, 3, 4, 3, 3,	/* speed	�j�̑��� (�x��������ʏ�̒e����������̂ŋ����B�������d���Ȃ�) */


	/*data->*/s->base_weapon_strength	= ccc_tbl[CCC_STRENGTH_L1+(weapon_level_offset)+(is_needle<<(4+2))];/* 64 == 8[players]*8[line] */
//	/*data->*/s->base_weapon_strength	= ddd_tbl[DDD_STRENGTH_L1+(weapon_level_offset)];

//
//	int ok;
//	ok=0;
	{
		#if 0
	//	if (1==r_tbl[shot_type][REI03_yudo])	/* psp��0���W�X�^������̂�0�Ɣ�r�����ق������� */
		if (0 != r_tbl[shot_type][REI03_yudo])/* �����V���b�g���H */
		#else
	//	if ( (REIMU_SHOT_TYPE_01) > shot_type)/* �����V���b�g���H */
		if ( (REIMU_SHOT_TYPE_00) == shot_type)/* �����V���b�g���H */
		#endif
		{
			SPRITE *target;
			target = search_enemy_by_sprite();
			if (target != player)
			{
				int int_angle512;
				/*data->*/int_angle512	= atan_512(target->y256-player->y256,target->x256-player->x256);
			//	if (255 < /*data->*/int_angle512 )/* ���180-360�x[/360�x]�̗̈�̂ݒǔ����� */
				/* CCW�̏ꍇ */
				if ( /*(384)*/(128*3) < ((/*data->*/int_angle512+64+128)&(512-1)) )/* ���225-315�x[/360�x]�̗̈�̂ݒǔ����� */
				{
					#if 1
					/* CCW�̏ꍇ */
					s->vx256	 = sin512((/*angle512*/int_angle512))*/*p->speed*/(16/*ccc_tbl[CCC_SPEED+select_player]*/);/*fps_factor*/
					s->vy256	 = cos512((/*angle512*/int_angle512))*/*p->speed*/(16/*ccc_tbl[CCC_SPEED+select_player]*/);/*fps_factor*/
					#endif
					#if 1
					/* �`��p�O����] */
				//	s->m_angleCCW512	=	int_angle512;
					/* �`��p�O����](�V���b�g�͏オ���ʂȂ̂ŁA��]�p���v�Z)[180/360�x]�񂷁B */
					int_angle512 += deg_360_to_512CCW((180));
					mask512(int_angle512);
					s->m_angleCCW512	=	int_angle512;/* �������ʂ̊p�x���`��p�p�x */
					#endif
				//	ok=1;/* �����ǔ��\ */
					return;
				}
			}
		}
	}
//
//	if (0==ok)/* �����ǔ��s�H */
	{	/*�ŗL�̐ݒ�p�x���g��*/
		s->vx256 = ((/*int_vx*/r_tbl[shot_type][REI02_vx256]))*/*p->speed*/(ccc_tbl[CCC_SPEED+OFFS_IS_NEEDLE/*4*/]);/*fps_factor*/
		s->vy256 = -(((int)ccc_tbl[CCC_SPEED+OFFS_IS_NEEDLE/*4*/])<<8);/*fps_factor*/ /*p->speed*/
	}
}
//	data->speed 	= ccc_tbl[CCC_SPEED+select_player];

/*---------------------------------------------------------
	�v���C���[�e(�V���b�g�̂�)����
---------------------------------------------------------*/

/* �얲A (�U���ޏ�) �����V���b�g */

static void shot_regist_re_yudou(SPRITE *s) 	/* �얲A ������A */
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_00);/* �����V���b�g */
}

static void shot_regist_yu_gggg(SPRITE *s)	/* �H�X�q */
{
	/*shot_regist_needle*/player_register_shot_object(s, NEEDLE_ANGLE_271);/*-48*/	/*t256( 0), t256(-40),*/
}

static void shot_regist_ma_gggg(SPRITE *s)	/* ������A */	/* ������B */	/* ��(���݃_�~�[�p) */
{
	/*shot_regist_needle*/player_register_shot_object(s, NEEDLE_ANGLE_270); 		/*t256( 0), t256( 0),*/
}

static void shot_regist_ci_gggg(SPRITE *s)	/* ���~���A */	/* �`���m */
{
	/*shot_regist_needle*/player_register_shot_object(s, NEEDLE_ANGLE_277 ); /*t256( 0), t256( 0),*/
	/*shot_regist_needle*/player_register_shot_object(s, NEEDLE_ANGLE_263 ); /*t256( 0), t256( 0),*/
}
static void shot_regist_ma_lazer(SPRITE *s) /* ������B ���[�U�[ */
{
	{
		static int count123;count123++;
		if (count123&0x20)	{	return; }
	}
	shot_regist_ma_gggg(s);
}

/*---------------------------------------------------------
	�v���C���[�e(�V���b�g�̂�)����
---------------------------------------------------------*/

/* �얲A/B ��]�V���b�g */	/* �ʏ�V���b�g */
static void shot_regist_1(SPRITE *s)	/* �얲 */	/* �얲 �� ������ �� �`���m �� ���~���A */	/* [***090220 �ǉ� */
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_01); 	/* �ʏ�V���b�g */
}
static void shot_regist_2(SPRITE *s)	/* �얲 */	/* �얲 �� ������ �� �`���m �� ���~���A */	/* [***090220 �ǉ� */
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_02); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_03); 	/* �ʏ�V���b�g */
//	player_register_shot_object(s, MARISA_SHOT_TYPE_03);	/* ���g�U */
//	player_register_shot_object(s, MARISA_SHOT_TYPE_04);	/* ���g�U */
}
static void shot_regist_3(SPRITE *s)	/* �얲 */	/* ������ �� �`���m */
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_04); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_01); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_05); 	/* �ʏ�V���b�g */
//	player_register_shot_object(s, MARISA_SHOT_TYPE_03);	/* ���g�U */	/* REIMU_SHOT_TYPE_02 �܂����� */
//	player_register_shot_object(s, REIMU_SHOT_TYPE_01);
//	player_register_shot_object(s, MARISA_SHOT_TYPE_04);	/* ���g�U */	/* REIMU_SHOT_TYPE_03 �܂����� */
}
static void shot_regist_4(SPRITE *s)	/* �얲 */	/* ������ */
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_04); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_02); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_03); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_05); 	/* �ʏ�V���b�g */
//	player_register_shot_object(s, REIMU_SHOT_TYPE_04);/* �ʏ�V���b�g */
//	player_register_shot_object(s, REIMU_SHOT_TYPE_02); 	/* REIMU_SHOT_TYPE_02 �܂����� */	/*MARISA_SHOT_TYPE_05*/
//	player_register_shot_object(s, REIMU_SHOT_TYPE_03); 	/* REIMU_SHOT_TYPE_03 �܂����� */	/*MARISA_SHOT_TYPE_06*/
//	player_register_shot_object(s, REIMU_SHOT_TYPE_05);/* �ʏ�V���b�g */
//	player_register_shot_object(s, MARISA_SHOT_TYPE_07);
//	player_register_shot_object(s, MARISA_SHOT_TYPE_08);
}

static void shot_regist_5(SPRITE *s)	/* �얲B */ /* ������ */
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_06); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_04); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_01); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_05); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_07); 	/* �ʏ�V���b�g */
//	player_register_shot_object(s, REIMU_SHOT_TYPE_02); 	/* REIMU_SHOT_TYPE_02 �܂����� */	/*MARISA_SHOT_TYPE_05*/
//	player_register_shot_object(s, REIMU_SHOT_TYPE_03); 	/* REIMU_SHOT_TYPE_03 �܂����� */	/*MARISA_SHOT_TYPE_06*/
//	player_register_shot_object(s, MARISA_SHOT_TYPE_07);
//	player_register_shot_object(s, MARISA_SHOT_TYPE_08);
}

/* �얲B (�j�ޏ�) */

/* ������A(�~�T�C��������) */
/* ������B(���[�U�[������) */


static void shot_regist_chou(SPRITE *s) 		/* �H�X�q */
{
	player_register_shot_object(s, YUYUKO_SHOT_TYPE_08);
	player_register_shot_object(s, YUYUKO_SHOT_TYPE_09);
	player_register_shot_object(s, YUYUKO_SHOT_TYPE_0a);
	player_register_shot_object(s, YUYUKO_SHOT_TYPE_0b);
}


/*---------------------------------------------------------
	�v���C���[�A�I�v�V�����̈ړ�����
---------------------------------------------------------*/
static void re_ma_yu_control_option(SPRITE *s, PL_OPTION_DATA *data ) /* �얲�A������	���H�X�q */
{
	mask512(data->angleCCW512);
	{
	#define SSS_LOCATE			(PLAYERS8*0)
	#define SSS_SPEED			(PLAYERS8*1)
	#define SSS_MAX 			(PLAYERS8*2)		/* �ő吔 */
		static const u8 sss_tbl[(SSS_MAX)] =
		{/* REIMU(A/B) MARISA(A/B) REMILIA YUYUKO CIRNO(A/Q) */
			7,4,7,7, 3, 3, 3, 3,	/* SSS_LOCATE (YUYUKO==select_player)?(3):(7); */
			7,7,7,7, 3, 3, 7, 7,	/* SSS_SPEED  (YUYUKO==select_player)?(3):(7); */
		};
		int vv512;
		int ss512;
	//	vv512 = (YUYUKO==select_player)?(3):(7);	/*ra d2deg512(0.1) == 8.1*/
		vv512 = sss_tbl[SSS_LOCATE+select_player];
		ss512 = sss_tbl[SSS_SPEED+select_player];
		if (my_pad & PSP_KEY_SLOW)		/* �����ƑO���Ɉړ� */
		{
			if ((OPTION_ID_03_RM/*1*/&0xff)==(data->opt_anime_add_id&0xff)/*pos*/) /* 1: �E���n�̃I�v�V����(OPTION_ID_03_RM, OPTION_ID_04_RR ) */
			{
				if (data->angleCCW512 < /*(288)*/(256+16	-16-64+(7<<4)+((data->opt_anime_add_id&0x100)>>2)-(vv512<<4) )/*272 (256+16)*/ )/*����(�E)�ʒu*/
				{
					data->angleCCW512 += ss512;
				}
			}
			else	/* 0: �����n�̃I�v�V���� */
			{
				if (data->angleCCW512 > /*(224)*/(256-16	+16+64-(7<<4)-((data->opt_anime_add_id&0x100)>>2)+(vv512<<4) )/*240 (256-16)*/ )/*����(��)�ʒu*/
				{
					data->angleCCW512 -= ss512;
				}
			}
		}
		else	/* �����ƌ���Ɉړ� */
		{
			if ( (OPTION_ID_03_RM/*1*/&0xff)==(data->opt_anime_add_id&0xff)/*pos*/) /* 1: �E���n�̃I�v�V���� */
			{
				if (data->angleCCW512 > 64+16 +8+32-64+((data->opt_anime_add_id&0x100)>>2) )/*�ŏ�(�E)�ʒu*/
				{
					data->angleCCW512 -= ss512;
				}
			}
			else	/* 0: �����n�̃I�v�V���� */
			{
				if (data->angleCCW512 < 448-16 -8-32+64-((data->opt_anime_add_id&0x100)>>2) )/*�ő�(��)�ʒu*/
				{
					data->angleCCW512 += ss512;
				}
			}
		}
	}
	#if (0000)
	if (0==(pd_state_flag&STATE_FLAG_08_OPTION_HIDE))	/* �I�v�V�����������Ă�� */
	{
	//	s->type = SP_DELETE;/*�����܂�*/
	//	s->type = SP_DELETE;/*�����܂�*/
		s->flags			|= (SP_FLAG_VISIBLE);/*�����Ȃ�*/
	}
	#endif/*0000*/
	#if 1
	mask512(data->angleCCW512);/*�˂�̂���*/
	#endif
//	s->x256 = player->x256+cos512((data->angle512))*(16)/*data->length*/-((s->w)>>1)+((player->w-12-1)*data->opt_anime_add_id)+6;
//	s->x256 = player->x256+cos512((data->angle512))*(16)/*data->length*/+((/*player->w*/24-12-1)*data->opt_anime_add_id)+6-8;
//	s->x256 = player->x256+cos512((data->angle512))*(16)/*data->length*/+((/*player->w*/24+11+11)*data->opt_anime_add_id)-11-8;
// -1 : 		 -11 -8
//	1 : +24+11+11-11 -8
//	s->x256 = player->x256+cos512((data->angle512))*(16)/*data->length*/+((/*player->w*/24-6-6)*data->opt_anime_add_id)+6-8;
// -1 : 		 +6 -8
//	1 : +24 -6 -6 +6 -8
//	s->x256 = player->x256+cos512((data->angle512))*(16)/*data->length*/+((/*player->w*/(24)-6-6)*data->opt_anime_add_id)+6-((s->w)>>1)/*(8)*/;
//
	//	s->x256 = player->x256+cos512((data->angle512))*(16)/*data->length*/+(((int)(data->pos+1)*player->w-s->w)/2)-data->pos*6;
	//	s->x256 = player->x256+cos512((data->angle512))*(16)/*data->length*/+(((int)(data->opt_anime_add_id+1)*/*player->w*/(24)-s->w)/2)-data->opt_anime_add_id*6; 		//2598187
	//	s->x256 = player->x256+cos512((data->angle512))*(16)/*data->length*/+ (((24/*player->w*/ /2)-6)*(data->opt_anime_add_id)) -((s->w)>>1) +(24/*player->w*/ /2);	//2598219

		/* -1: 6*256  1: 18*256 */
	//	s->x256 = player->x256+cos512((data->angle512))*(16)/*data->length*/+ (((6)*(2+data->opt_anime_add_id))<<8) -((s->w128)); //2598187 2079061

	{int hankei;
	//	hankei = (YUYUKO==select_player)?(32):(16);
		hankei = (YUYUKO==select_player)?(32):(24);
		/* CCW�̏ꍇ */
		#if 0/*SDL(�������W)*/
		s->x256 = player->x256+sin512((data->angleCCW512))*(hankei)/*data->length*/+ (18*256)-(((data->opt_anime_add_id&2)<<8)*6) -((s->w128)); // 2079061 (4�I�v�V�����Ή�)
		int aaa;
		aaa = cos512((data->angleCCW512))*(hankei)/*data->length*/+((player->h128-s->h128))-t256(5.0);
		aaa = (REIMU_A==select_player)?(-aaa):(aaa);
		s->y256 = player->y256+aaa;
		#else/*Gu(���S���W)*/
	//	s->x256 = player->x256+sin512((data->angleCCW512))*(hankei)/*data->length*/+ (18*256)-(((data->opt_anime_add_id&2)<<8)*6) ; // 2079061 (4�I�v�V�����Ή�)
		s->x256 = player->x256+sin512((data->angleCCW512))*(20/*hankei*/)/*data->length*/+ (18*256)-(((data->opt_anime_add_id&2)<<8)*6) ; // 2079061 (4�I�v�V�����Ή�)
		int aaa;
	//	aaa = cos512((data->angleCCW512))*(hankei)/*data->length*/ -t256(5.0);
	//	aaa = (REIMU_A==select_player)?(-aaa):(aaa);
	//	s->y256 = player->y256+aaa;
		aaa = cos512((data->angleCCW512))*(hankei)/*data->length*/;
		aaa = (REIMU_A==select_player)?(-aaa):(aaa);
		s->y256 = player->y256+aaa +t256(10.0);
		#endif
	}
}

/*---------------------------------------------------------
	�v���C���[�A�I�v�V�����̈ړ�(�얲�A�������A���H�X�q)
---------------------------------------------------------*/

/*
����: [51]	[461=256+128+64+16-3]
����: [  ]	[314=256	+64   -6]
*/

//#define BASE_OPT_SHOT_ANIME	(PLAYERS8*3)
//	 12,  12,	6,	 6,    3,  yuuyko(12),	 CIRNO(3),	 CIRNO(3),	/* �I�v�V�����V���b�g�̃A�j���[�V�������x */

static void animate_option_re_ma_yu(SPRITE *src)
{
#if 1
	if (YUYUKO==select_player)
	{
		/*REMILIA_OPTION_DATA*/PL_OPTION_DATA *data=(/*REMILIA_OPTION_DATA*/PL_OPTION_DATA *)src->data;
		data->anime_wait--;
		if (data->anime_wait<1)
		{
			data->anime_wait=/*(anime_speed)*/(12);/*(12)*/ /*2*/ /*�A�z���܂͂���������*/
		#if 1
		//	src->type=((src->type+(data->opt_anime_add_id)+(data->opt_anime_add_id)-1/*data->pos*/)&(8-1)/*%8*/);
		//	src->type=((src->type+(data->opt_anime_add_id))&(0xfff7)/*%8*/);
			src->type=((src->type+(1))&(0xfff7)/*%8*/);
		//	if (src->type<0)		{	src->type=7;}
		#else
			src->type++;
			src->type &= (0xfff7);//if (src->type==8)	{src->type=0;}
		#endif
		}
	}
	else
	{
		/* �`��p�O����] */
		/* �`��p�p�x(����0�x�ō����(�����v���)) */
		src->m_angleCCW512 += (2);/*...�Ƃ肠����*/
		mask512(src->m_angleCCW512);
	}
#endif
}

static void animate_option_oz_ti(SPRITE *src)
{
	if ((CIRNO_A==select_player)||(CIRNO_Q==select_player))
	{
		/*REMILIA_OPTION_DATA*/PL_OPTION_DATA *data=(/*REMILIA_OPTION_DATA*/PL_OPTION_DATA *)src->data;
		data->anime_wait--;
		if (data->anime_wait<1)
		{
			data->anime_wait=/*(anime_speed)*/(3);/*12*/ /*2*/ /*�A�z���܂͂���������*/
		#if 0
		//	src->an im_frame=((src->an im_frame+(data->opt_anime_add_id)+(data->opt_anime_add_id)-1/*data->pos*/)&(8-1)/*%8*/);
			src->an im_frame=((src->an im_frame+(data->opt_anime_add_id))&(8-1)/*%8*/);
		//	if (src->an im_frame<0) 	{	src->an im_frame=7;}
		#else
			#if 0
			src->an im_frame++;
			src->an im_frame &= (8-1);//if (src->an im_frame==8)	{src->an im_frame=0;}
			#else
			src->type = ((src->type) & (0xfff8))|((src->type+1) & (8-1));//if (src->an im_frame==8) {src->an im_frame=0;}
			#endif
		#endif
		}
	}
	else
	{
		/* �`��p�O����] */
		/* �`��p�p�x(����0�x�ō����(�����v���)) */
		src->m_angleCCW512 += (2);/*...�Ƃ肠����*/
		mask512(src->m_angleCCW512);
	}
}


/*---------------------------------------------------------

---------------------------------------------------------*/

static void re_ma_yu_move_option(SPRITE *src)	/* �얲 	������	���H�X�q */
{
	/* �I�v�V��������\���̏ꍇ�A�������Ȃ��B */
	if (0==(src->flags & ( SP_FLAG_VISIBLE)))	{	return; 	}
//
	// �A�j���[�V����
	animate_option_re_ma_yu(src);
//
	/*REMILIA_OPTION_DATA*/PL_OPTION_DATA *data = (/*REMILIA_OPTION_DATA*/PL_OPTION_DATA *)src->data;

	if (pd_state_flag & STATE_FLAG_15_KEY_SHOT)
	{
		{
			{
				data->opt_shot_interval--;
				if (data->opt_shot_interval<0)
				{
					data->opt_shot_interval=player_fix_status[BASE_OPT_SHOT_INTERVAL0+(weapon_level_offset)];/* (�얲	������	���H�X�q)�I�v�V�����̃V���b�g�Ԋu */
					#if 0//1975613
					static /*const*/ void (*ggg[PLAYERS8])(SPRITE *sss) =
					{
						/*REIMU_A*/ 	shot_regist_re_yudou,	/* �얲 A(�U���ԎD) */
						/*REIMU_B*/ 	shot_regist_ma_gggg,	/* �얲 B(�j) */	/*re_bbbb*/
						/*MARISA_A*/	shot_regist_re_yudou,	/* ������ A(�U���X�e) */
						/*MARISA_B*/	shot_regist_ma_lazer,	/* ������ B(���܂��ꃌ�[�U�[) */
						/*REMILIA*/ 	shot_regist_ma_gggg,	/* ���݃_�~�[ */
						/*YUYUKO*/		shot_regist_yu_gggg,	/* �H�X�q(��) */
						/*CIRNO_A*/ 	shot_regist_ma_gggg,	/* ���݃_�~�[ */
						/*CIRNO_Q*/ 	shot_regist_ma_gggg,	/* ���݃_�~�[ */
					};
					(ggg[select_player])(src);
					#else//1975597
					switch (select_player)
					{
					case MARISA_A:	//	shot_regist_re_yudou(src);	break;	/* ������ A(�U���X�e) */
					case REIMU_A:		shot_regist_re_yudou(src);	break;	/* �얲(�U���ԎD) */
					case YUYUKO:		shot_regist_yu_gggg(src);	break;	/* �H�X�q(��) */
					case MARISA_B:		shot_regist_ma_lazer(src);	break;	/* ������B(���܂��ꃌ�[�U�[) */
						#if 0
				//	case REIMU_B:		shot_regist_ma_gggg(src);	break;	/* �얲(�j) */		/*re_bbbb*/
						#endif
					default:			shot_regist_ma_gggg(src);	break;	/* ������A */
					}
					#endif
				}
			}
		}
	}
	re_ma_yu_control_option(src, data );
}

/*---------------------------------------------------------
	�v���C���[�A�I�v�V�����̒�`
---------------------------------------------------------*/

/* [***090128 [***090220	�ǉ�:���~���A�p�I�v�V����.c3,c4 */

enum	/* _player_option_type_ */
{
	OPTION_C1 = 0,
	OPTION_C2,
	OPTION_C3,
	OPTION_C4,
};
static SPRITE *option[4];

/*---------------------------------------------------------
	�v���C���[�A�I�v�V�����̈ړ�(���~���A �� �`���m)
---------------------------------------------------------*/

enum	/* _remilia_move_state_ */
{
	RS00 = 0,
	RS01,
	RS02,
};
enum	/* _remilia_formation_ */
{
	FORMATION_00 = 0,
	FORMATION_01,
	FORMATION_02,
	FORMATION_03,
};
/*
	�ڕW:st ate1
	FORMATION_00: ���~���A�̒��O�Ɏl�� / �`���m�̒���Ɏl�l
	FORMATION_01: ���~���A�̎��������
	FORMATION_02: ��ʌ������x������� (y=250�̈ʒu�ɎU��΂�)
	FORMATION_03: �������p(�D���Ȉʒu�ɒu�����) (���~���A�ɏd�Ȃ�悤�ɁBslow�������ƒi�K�I��option��~)
*/





//#define REMILIA_angle512		angleCCW512
//#define REMILIA_OPTION_DATA	PL_OPTION_DATA

static void oz_ci_move_option(SPRITE *src) /* ���~���A �� �`���m	[***090220 �ǉ� */
{
	/* �I�v�V��������\���̏ꍇ�A�������Ȃ��B */
	if (0==(src->flags & ( SP_FLAG_VISIBLE)))	{	return; }
//
	// !--------------------
	/*REMILIA_OPTION_DATA*/PL_OPTION_DATA *data=(/*REMILIA_OPTION_DATA*/PL_OPTION_DATA *)src->data;

//
	static int state_zz1=0;
	/* �A�j���[�V���� */
	animate_option_oz_ti(src);
	/* �U���x�� */
	if (pd_state_flag & STATE_FLAG_15_KEY_SHOT)
	{
		{
			{
				data->opt_shot_interval--;
				if (data->opt_shot_interval<0)
				{
				//	data->opt_shot_interval=(5+3);/* (���~���A �� �`���m)�I�v�V�����̃V���b�g�Ԋu */
					data->opt_shot_interval=player_fix_status[BASE_OPT_SHOT_INTERVAL0+(weapon_level_offset)];/* (�얲	������	���H�X�q)�I�v�V�����̃V���b�g�Ԋu */
					switch (data->opt_anime_add_id)
					{
					case OPTION_C1:
					case OPTION_C2:
/*
 WEAPON_L0(P000-P008)
 WEAPON_L1(P008-P015)
 WEAPON_L2(P016-P031)
 WEAPON_L3(P032-P063)
 WEAPON_L4(P064-P127)
 WEAPON_L5(P128)
*/
					//	if (pd_weapon_power > (100-1))	/* ���V�X�e���̒����l 100 */
						if (pd_weapon_power > (96)) 	/* �V�X�e�����ς�����̂ŏC�����Ă݂�(96==128-32) */
						{
							/*shot_regist_needle*/shot_regist_ci_gggg(src); /*t256( 0), t256( 0),*/
				//			data->opt_shot_interval=(7+3);
						}
						else
						{
							/*shot_regist_needle*/shot_regist_ma_gggg(src); /*t256( 0), t256( 0),*/
				//			data->opt_shot_interval=(5+3);
						}
						break;
					case OPTION_C3:
					case OPTION_C4:
						/*shot_regist_needle*/shot_regist_ma_gggg(src); /*t256( 0), t256( 0),*/
				//		data->opt_shot_interval=(5+3);
						break;
					}
				}
			}
		}
	}
	//
	int slow_flag;
	slow_flag=0;
	/* �ᑬ�ړ� */
	if (my_pad & PSP_KEY_SLOW)
	{
		if (REMILIA==select_player)
		{
			slow_flag=1;
		}
	//	else
	//	{
	//		/* �`���m */
	//	}
	}
	int player_offs_x256;
	int player_offs_y256;
	#if 0/*SDL(�������W)*/
	player_offs_x256 = player->x256+((player->w128-src->w128));
	player_offs_y256 = player->y256+((player->h128-src->h128));
	#else/*Gu(���S���W)*/
	player_offs_x256 = player->x256;
	player_offs_y256 = player->y256;
	#endif
//	if (1==slow_flag)	/* psp��0���W�X�^������̂�0�Ɣ�r�����ق������� */
	if (0!=slow_flag)	/* ���Ȃ��Ƃ��ᑬ�ړ��ŃI�v�V�������~�߂���̂̓��~���A�̂�(�`���m�ɂ͍���Ȃ�) */
	{
		/* ���~���A�p */
		data->slow_count += (1<<2);
		if ((/*data->*/state_zz1==FORMATION_03) && /* FORMATION_03: �������p(�D���Ȉʒu�ɒu�����) */
			(data->slow_count < ((data->opt_anime_add_id+1)<<(6+2))/* *30 64*/))
		{
			src->x256 = player_offs_x256;
			src->y256 = player_offs_y256;
		}
		else if (/*data->*/state_zz1==FORMATION_02) /* FORMATION_02: ��ʌ������x������� */
		{
			switch (data->opt_anime_add_id)
			{
			#if 0/*SDL(�������W)*/
			case OPTION_C1: if (src->x256 < t256(170)-((src->w128+src->w128)))	{src->x256 += t256(2.0);} break;	/*fps_factor*/
			case OPTION_C2: if (src->x256 > t256(210))							{src->x256 -= t256(2.0);} break;	/*fps_factor*/
			case OPTION_C3: if (src->x256 < t256(100)-((src->w128+src->w128)))	{src->x256 += t256(4.0);} break;	/*fps_factor*/
			case OPTION_C4: if (src->x256 > t256(280))							{src->x256 -= t256(4.0);} break;	/*fps_factor*/
			#else/*Gu(���S���W)*/
			case OPTION_C1: if (src->x256 < t256(170))	{src->x256 += t256(2.0);} break;	/*fps_factor*/
			case OPTION_C2: if (src->x256 > t256(210))	{src->x256 -= t256(2.0);} break;	/*fps_factor*/
			case OPTION_C3: if (src->x256 < t256(100))	{src->x256 += t256(4.0);} break;	/*fps_factor*/
			case OPTION_C4: if (src->x256 > t256(280))	{src->x256 -= t256(4.0);} break;	/*fps_factor*/
			#endif
			}
		}
	}
	else
	{
		/* ���~���A�p */
		if (data->slow_count)
		{
			data->slow_count=0;
			data->state2=0;
		}
	//
		/* �t�H�[���[�V�����ύX */
		/* OPTION_C1 �̏ꍇ�̂݃L�[���͎󂯕t�� */
		if (OPTION_C1==data->opt_anime_add_id) //opt_anime_add_id==1�ɂ��flag�Ǘ�
		{
		//	if (my_pad & PSP_KEY_OPTION)
		/* (�I�v�V�����L�[�L�[�𗣂����u�ԂȂ�) */
			if ((0==(my_pad & PSP_KEY_OPTION))) 	/* �I�v�V�����L�[��������Ă��� */
			{
				if ( (my_pad^my_pad_alter) & PSP_KEY_OPTION)	/* �I�v�V�����L�[�̏�Ԃ��ς����(�g�O���A�������܂��͗������ꍇ) */
				{
	#if 0/*�f�o�b�O�p*/
	/*	���u�{�X�Ƒ��ł�����ƃn���O�A�b�v�v�o�O(�`r26)�΍� */
	/*(�{�X�|�����ɋ�炢�{�����?����start��������Ȃ��Ȃ�o�O������B���I�v�V�����L�[�͌���)*/
	/* �p�l���̃X�R�A����move_type���A�O���C�Y����state_zz1��\��������B���Ă����������������Ⴄ�B */
	/*
	my_score0000460960 == 0xb410
	0x1
		STATE_FLAG_05_IS_BOSS						(0x0010)
	0x4
		ST ATE_FLAG_11_IS_BOSS_DESTROY				(0x0400)
	0xb
		STATE_FLAG_13_DRAW_BOSS_GAUGE				(0x1000)
		STATE_FLAG_14_GAME_LOOP_QUIT				(0x2000)
		STATE_FLAG_16_NOT_ALLOW_KEY_CONTROL 			(0x8000)
	 */
	pd_my_score 	= pd_state_flag/*spell_card_number*/;
	pd_graze_point	= /*data->*/state_zz1;
	#endif
					/*data->*/state_zz1++;
					if (/*data->*/state_zz1==(FORMATION_03+1) )
					{	/*data->*/state_zz1=FORMATION_00;}
				//
					data->state2=0; 								/* OPTION_C1 �� state2 */
					((/*REMILIA_OPTION_DATA*/PL_OPTION_DATA *)option[OPTION_C2]->data)->state2=0; /* OPTION_C2 �� state2 */
					((/*REMILIA_OPTION_DATA*/PL_OPTION_DATA *)option[OPTION_C3]->data)->state2=0; /* OPTION_C3 �� state2 */
					((/*REMILIA_OPTION_DATA*/PL_OPTION_DATA *)option[OPTION_C4]->data)->state2=0; /* OPTION_C4 �� state2 */
				}
			}
		}
		switch (data->state2)
		{
		case RS00:	/* ������ */
			{
				enum
				{
					FORMATION_01_ADD_R = 0, 		/* 0 */
					FORMATION_01_RADIUS_R,			/* 1 */
					FORMATION_02_LOCATE_X,			/* 2 */
					FORMATION_02_LOCATE_Y,			/* 3 */
					FORMATION_00_LOCATE_Y_REMILIA,	/* 4 */ 	/* == 4 == (��2) == REMILIA �v���C���[�ԍ��ɍ��킹�Ă��鎖�ɒ��� */
					FORMATION_00_LOCATE_X,			/* 5 */
					FORMATION_00_LOCATE_Y_CIRNO_A,	/* 6 */ 	/* == 6 == (��3) == CIRNO	�v���C���[�ԍ��ɍ��킹�Ă��鎖�ɒ��� */
					FORMATION_00_LOCATE_Y_CIRNO_Q,	/* 7 */ 	/* == 7 == (��3) == CIRNO	�v���C���[�ԍ��ɍ��킹�Ă��鎖�ɒ��� */
					FORMATION_MAX					/* 8 */ 	/* �ő吔 */
				};
				//	{	t256(15),  t256(-15),	t256(25),  t256(-25) }, 				/* FORMATION_00:			��FORMATION_00_LOCATE_X */
				//	{	t256(15),  t256( 15),	t256(20),  t256( 20) }, 				/* FORMATION_00:			��FORMATION_00_LOCATE_Y */
//				//	{	( 1*115+10),  ( 2*115+10),	( 0*115+10),  ( 3*115+10) },		/* FORMATION_02:			��ʌ������x�������(wideth380dot) */
				static const s16 fff[FORMATION_MAX][4] =
				{
					{	( 1),  (  2),	(-3),  ( -6) }, 								/* 0 FORMATION_01_ADD_R:		��]�����A�p�x���Z�l */
					{	(20),  ( 30),	(40),  ( 50) }, 								/* 1 FORMATION_01_RADIUS_R: ��]���a */
					{	( 1*104+20),  ( 2*104+20),	( 0*104+20),  ( 3*104+20) },		/* 2 FORMATION_02_LOCATE_X: ��ʌ������x�������(wideth352dot) */
					{  (240),  (240),  (220),  (220) }, 								/* 3 FORMATION_02_LOCATE_Y: ��ʌ������x������� */
					{	t256(12-33),  t256( 12-33), t256(26-33),  t256( 26-33) },		/* 4 FORMATION_00_LOCATE_Y_REMILIA: ���~���A�̒��O�Ɏl�� FORMATION_00_LOCATE_Y_REMILIA */	/* 2: ���~���A�p */ 	/* �O���z�u */
					{	t256(14+12-11-1),	t256(14-12-1), t256(14+28-11-1),  t256(14-28-1) },	/* 5 FORMATION_00_LOCATE_X: ���~���A�̒��O�Ɏl�� FORMATION_00_LOCATE_X */
					{	t256(20),  t256( 20),	t256(25),  t256( 25) }, 				/* 6 FORMATION_00_LOCATE_Y_CIRNO_A: 	  �`���m�̒���Ɏl�l FORMATION_00_LOCATE_Y_CIRNO */ /* 3: �`���m�p */
					{	t256(20),  t256( 20),	t256(25),  t256( 25) }, 				/* 7 FORMATION_00_LOCATE_Y_CIRNO_Q: 	  �`���m�̒���Ɏl�l FORMATION_00_LOCATE_Y_CIRNO */ /* 3: �`���m�p */
				};
				switch (/*data->*/state_zz1)
				{
				case FORMATION_00:	/* FORMATION_00: ���~���A�̒��O�Ɏl�� / �`���m�̒���Ɏl�l */
					data->offset_x256=((fff[FORMATION_00_LOCATE_X][(data->opt_anime_add_id)]));
					data->offset_y256=((fff[select_player/*FORMATION_00_LOCATE_Y*/][(data->opt_anime_add_id)]));	/* �v���C���[�ԍ��ɍ��킹�Ă��鎖�ɒ��� */
					/* ���~���A�p */
				//	if (REMILIA==select_player)
				//	{
				//		data->offset_y256 -= t256(32);/* �O���z�u */
				//	}
					break;
				case FORMATION_01:	/* FORMATION_01: ���~���A�̎�������� */
					data->offset_x256 = fff[FORMATION_01_RADIUS_R][(data->opt_anime_add_id)];/*40*/ /* �~�̔��a�ƌ��p */
					data->offset_y256 = fff[FORMATION_01_ADD_R ][(data->opt_anime_add_id)]; 		/* �~�̉�]�����A�p�x���Z�l�ƌ��p */
/// 				data->f_angle512=0/*l_angle512*/;
					break;
				case FORMATION_02:	/* FORMATION_02: ��ʌ������x������� */
					data->offset_x256=((fff[FORMATION_02_LOCATE_X][(data->opt_anime_add_id)])<<8);
					data->offset_y256=((fff[FORMATION_02_LOCATE_Y][(data->opt_anime_add_id)])<<8);/*240*/
					break;
				case FORMATION_03:	/* FORMATION_03: �������p(�D���Ȉʒu�ɒu�����) */
					break;
				}
			}
			data->state2=1; 	/* ���� */
			break;
		case RS01:	/* �ڕW�n�_�ֈړ��� */
			#define oz_offset_r 	(data->offset_x256) 	/* FORMATION_01: �~�̔��a�ƌ��p */
			#define oz_offset_add_r (data->offset_y256) 	/* FORMATION_01: �~�̉�]�����A�p�x���Z�l�ƌ��p */
			{
			int delta_y256=0;				/* delta_y : y���W�ɂ�����ڕW�n�_�ƌ��ݒn�̍� */
			int delta_x256=0;				/* delta_x : x���W�ɂ�����ڕW�n�_�ƌ��ݒn�̍� */
				switch (/*data->*/state_zz1)
				{
				case FORMATION_00:	/* FORMATION_00: ���~���A�̒��O�Ɏl�� / �`���m�̒���Ɏl�l */
					delta_y256 = player_offs_y256-src->y256 + data->offset_y256;
					delta_x256 = player_offs_x256-src->x256 + data->offset_x256;
					break;
				case FORMATION_01:	/* FORMATION_01: ���~���A�̎�������� */						//static int l_angle512;
#if 0
					/* ���~���A�p */
					if (REMILIA==select_player)
					{
						/*l_angle512*/data->/*f_*/angle512 += oz_offset_add_r/*data->offset_y*/ /*5==ra d2deg512(0.05)*/;	/*fps_factor*/
						mask512(/*l_angle512*/data->/*f_*/angle512);	//if (l_angle>M_PI) 	{	l_angle-=2*M_PI;	}
					}
#endif
#if 1
/* CCW�̏ꍇ */
					delta_x256 = player_offs_x256-src->x256 + (sin512((data->/*REMILIA_angle512*/angleCCW512/*+l_angle512*/))*oz_offset_r);
					delta_y256 = player_offs_y256-src->y256 + (cos512((data->/*REMILIA_angle512*/angleCCW512/*+l_angle512*/))*oz_offset_r);
#endif
					break;
				case FORMATION_02:	/* FORMATION_02: ��ʌ������x������� */
					#if 0/*SDL(�������W)*/
					delta_y256 = (-((src->h128))-src->y256+data->offset_y256);/*240*/
					delta_x256 = (-((src->w128))-src->x256+data->offset_x256);
					#else/*Gu(���S���W)*/
					delta_y256 = (-src->y256+data->offset_y256);/*240*/
					delta_x256 = (-src->x256+data->offset_x256);
					#endif
					break;
				case FORMATION_03:	/* FORMATION_03: �������p(�D���Ȉʒu�ɒu�����) */
					delta_y256 = player_offs_y256-src->y256;
					delta_x256 = player_offs_x256-src->x256;
					break;
				}
				{	/* �ڕW�n�_�Ɉړ����� */
					int ccc_angle512;
					/*data->*/ccc_angle512=atan_512(((delta_y256)>>8), ((delta_x256)>>8));
#if 1
/* CCW�̏ꍇ */
					src->x256 += ((sin512((/*data->*/ccc_angle512))*/*10*/((REMILIA==select_player)?t256(8.0):t256(1.0)))>>8);/*fps_factor*/
					src->y256 += ((cos512((/*data->*/ccc_angle512))*/*10*/((REMILIA==select_player)?t256(8.0):t256(1.0)))>>8);/*fps_factor*/
#endif
				}
#if 1
				/* ���~���A�p */
				if (REMILIA==select_player)
				{
					/* �ڕW�t�߂܂ł��ǂ蒅������ */
					if (( t256(-5) < delta_y256) &&
						(delta_y256 < t256(5)) )
					{
						data->state2=2; 	/* ���� */
					}
				}
#endif
			}
			break;
#if 1
		case RS02:	/* �ʒu�̈ێ� & �z�u����flag */
			switch (/*data->*/state_zz1)
			{
			case FORMATION_00:	/* FORMATION_00: ���~���A�̒��O�Ɏl�� / �`���m�̒���Ɏl�l */
				src->x256 = player_offs_x256 + data->offset_x256;
				src->y256 = player_offs_y256 + data->offset_y256;
				break;
			case FORMATION_01:	/* FORMATION_01: ���~���A�̎�������� */
				/*l_angle512*//*f_*/ /*REMILIA_angle512*/data->angleCCW512 += oz_offset_add_r/*data->offset_y*/ /*5==ra d2deg512(0.05)*/;	/*fps_factor*/
				mask512(/*l_angle512*/ /*f_*/ /*REMILIA_angle512*/ data->angleCCW512);	//if (l_angle>M_PI) 	{	l_angle-=2*M_PI;	}
#if 1
/* CCW�̏ꍇ */
				src->x256 = player_offs_x256 + sin512((data->angleCCW512/*REMILIA_angle512*/ /*+data->f_angle512*/ /*l_angle512*/))*oz_offset_r;
				src->y256 = player_offs_y256 + cos512((data->angleCCW512/*REMILIA_angle512*/ /*+data->f_angle512*/ /*l_angle512*/))*oz_offset_r;
#endif
				break;
			case FORMATION_02:	/* FORMATION_02: ��ʌ������x������� */
				src->x256 = data->offset_x256;
				src->y256 = data->offset_y256;
				break;
			case FORMATION_03:	/* FORMATION_03: �������p(�D���Ȉʒu�ɒu�����) */
				src->x256 = player_offs_x256;
				src->y256 = player_offs_y256;
				break;
			}
#endif
			break;
		}
	}
}
//	if (src->flags&SP_FLAG_VISIBLE) /* �\������Ă���Ώ�L�̏����������B */	{	}
//
	#if (0000)
	if (0==(pd_state_flag&STATE_FLAG_08_OPTION_HIDE))	/* �I�v�V�����������Ă�� */
	{
	//	src->type = SP_DELETE;/*�����܂�*/
	//	src->type = SP_DELETE;/*�����܂�*/
		src->flags			|= (SP_FLAG_VISIBLE);/*�����Ȃ�*/
	}
	#endif/*0000*/

/*---new add-----*/

/* ====================================================================================== */
/* ====================================================================================== */
/* ====================================================================================== */

/*---------------------------------------------------------
	�v���C���[�V�[���h����(�얲)
---------------------------------------------------------*/

static void player_create_bomber_kekkai_parrent(SPRITE *src)		/* �얲 */		//�V�[���h�̒ǉ�
{
	pd_state_flag	|= STATE_FLAG_02_BOMB_AUTO_GET_ITEM;	/*�{���ɂ�鎩�����W�\*/
	int ii;//	int i;	/* ���ی����� */
	for (ii=0; ii<(8); ii++)//	for (i=0;i<360/*i<=359*/;i+=45)
	{
		SPRITE *s;
			s				= sprite_add_gu(TAMA_ATARI_04);
		PL_KEKKAI_DATA *data;
			data			= mmalloc(sizeof(PL_KEKKAI_DATA));
			s->data 		= data;

		if (0 == (ii&1))// if (i%90 == 0)
		{
			data->add_r512	= (23); 							/* ���v��� */
			data->radius	= (5)/*(48)*/;	//d->radius=38; 	/*���a�����l*/	/* �������L����悤�ɂ��� */
		}
		else		/* 1���ƕs���Ȃ̂�2������B */
		{
			data->add_r512	= (-17);							/* �����v��� */
			data->radius	= (0)/*(45)*/;	//d->radius=35; 	/*���a�����l*/	/* �������L����悤�ɂ��� */
		}
//		if (YUYUKO==select_player)
//		{	/* �H�X�q */
//			data->radius		+= 45/*45*/;		/*���a�����l*/
//			s->type 		= (SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO)/*�{�X�ɖ���*/;
//		}
//		else // if (REIMU==select_player)
		{	/* �얲 */
			s->type 		= (ii&(4-1))+(/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_04|SP_GROUP_SHOT_SPECIAL)/*�{�X�ɗL��*/;/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) �{�X�ɖ���*/
		}
		s->callback_mover	= player_move_kekkai;			/* ���v��聕�����v��� */
	//	s->callback_mover	= player_move_kekkai/*2*/;		/* �����v��� */
		s->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);
	//	data->angle512		= (ii*deg_360_to_512((45)))/*i*/;
//		data->angleCCW512		= (ii*deg_360_to_512((45)))/*i*/;
		data->angleCCW512		= (ii*deg_360_to_512CCW((45)))/*i*/;
		/* �o�n�߂̓v���C���[���W */
		s->x256 			= player->x256;//+co_s512(((ii*deg_360_to_512(45))/*i*/))*d->radius+((player->w-s->w)>>1);
		s->y256 			= player->y256;//+si_n512(((ii*deg_360_to_512(45))/*i*/))*d->radius+((player->h-s->h)>>1); /* ���v��� */
		/*data->*/s->base_weapon_strength		= (8*1)/*5*/;/* [***090214 �ǉ� */
	}
}

/*---------------------------------------------------------
	[�v���C���[�V�[���h]�{������
	(������ �� �`���m�� �H�X�q)
---------------------------------------------------------*/

static void player_create_bomber_levarie_parrent(SPRITE *src)
{
	pd_state_flag	|= STATE_FLAG_02_BOMB_AUTO_GET_ITEM;	/*�{���ɂ�鎩�����W�\*/
	int jjj;		jjj=0;
	int angCCW512; angCCW512 = (0);/* 0 �`���m�p(�b��I)*/
	int iii;
	for (iii=0; iii<(18*3)/*16*/; iii++)
	{
		SPRITE *s;
		s					= sprite_add_gu(BASE_BOMBER_ATARI_0a+select_player);
//
		jjj++;if (4==jjj)
		{
			jjj=0;
		}
//		s->type 		= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_04|SP_GROUP_SHOT_ZAKO)/*�{�X�ɖ���*/;/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_SPECIAL)*/ /*�{�X�ɗL��*/
		s->type 		= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_04|SP_GROUP_SHOT_SPECIAL)/*�{�X�ɗL��*/ /*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) �{�X�ɖ���*/;
		s->type += jjj;
//

		s->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);
		s->callback_mover	= player_move_levarie;
		#if 0
		{
			static const u16 player_fix_status_ggg[8] =
			{/* REIMU(A/B) MARISA(A/B) REMILIA YUYUKO CIRNO(A/Q) */
				256, 256, 256, 256, 32, 256, 32, 32,	/* �`��p�{���̏����T�C�Y / size of bomb at first. */
			};
			s->m_zoom_x256		= player_fix_status_ggg[select_player]/*8*/;/* 64 == (1/4) */
		}
		#else
		if (YUYUKO!=select_player)
		{
			s->m_zoom_x256		= 1+(ra_nd()&(64-1));	/* �`��p�{���̏����T�C�Y / size of bomb at first. */
		}
		#endif

		/*data->*/s->base_weapon_strength		= player_fix_status[BASE_STD_BOMB_STRENGTH+select_player] /*k_strength*/ /*5*/ /*15-k-k*/;/* [***090214 �ǉ� */
		/* MARISA�͋������g�U���Ă��܂��̂ł�����Ȃ��B  */
		/* �`���m�͓��Ă₷���̂ł��̕��キ���Ȃ��Ƌ��������Ⴄ�B  */
//		data->angle512		= angCCW512;
		int d_speed256;
		#if 0
		if (0==(iii&(2-1)/*%2*/))
		{			/*d->*/d_speed256	= t256(1.0);	c->alpha	= 150;	}
		else	{	/*d->*/d_speed256	= t256(1.2);	c->alpha	= 180;	}
		#else
		/*d->*/d_speed256	= t256(1.0)+(iii<<3);
		#endif
#if 1
/* CCW�̏ꍇ */
		s->vx256		= ((sin512((/*angle512*/angCCW512))*/*p->speed*/(d_speed256))>>8);/*fps_factor*/
		s->vy256		= ((cos512((/*angle512*/angCCW512))*/*p->speed*/(d_speed256))>>8);/*fps_factor*/
#endif
//
		#if 0/*SDL(�������W)*/
		s->x256 			= src->x256+((src->w128-s->w128));
		s->y256 			= src->y256+((src->h128-s->h128));
		#else/*Gu(���S���W)*/
		s->x256 			= src->x256;
		s->y256 			= src->y256;
		#endif
		/* �M���X�g�h���[��(���ǂ�)�B�X�^�[�_�X�g�����@���G�ƈႢ�ߐڐ��\�͖����B */
		if (YUYUKO==select_player)	/* �H�X�q */
		{
			s->x256 		+= ((s->vx256)<<5);
			s->y256 		+= ((s->vy256)<<5);
//			/* �H�X�q �������邩��ȁ`(���ʎア�����ʂ���̂��o����悤�ɃV�X�e�����������ق�����������) */
		}
		#if 1
		/* �`��p�p�x(����0�x�ō����(�����v���)) */
		s->m_angleCCW512		= angCCW512;
		#endif
	//	angCCW512 += (int)((512)/16); // 22.5�x/360, ��/8 /* 0.392699081698724154810 */
		angCCW512 += (int)((512)/18); // 20.0�x/360, ��/9 /* 0.349065850398865915384 */
	}
}

/*---------------------------------------------------------
	�v���C���[�����G����
	(�H�X�q)[�H�X�q�{���̐�{��]
---------------------------------------------------------*/

static void player_create_bomber_tachie(SPRITE *src)
{
	SPRITE *s;
	s					= sprite_add_gu(JIKI_ATARI_ITEM_80);
	s->flags			|= (/*SP_FLAG_VISIBLE|*/SP_FLAG_TIME_OVER);
	s->color32			= 0xffffffff;		/*	s->alpha			= 0xff;*/
	s->callback_mover	= player_move_tachie;
	s->type 			= (JIKI_BOMBER_00|SP_GROUP_SHOT_ZAKO);	/*SP_GROUP_JIKI_GET_ITEM*/ /*JI KI_SHOT_00*/	/* �{�X�̒��ڍU���͋֎~ */	/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) �{�X�ɖ���*/
	/* �����G�����ʒu */
	s->x256 			= -t256(150);	// (200)	/*0*/  /*(src->x256)+((src->w128-s->w128))*/;
	s->y256 			=  t256(150);	// (100)	/*GAME_HEIGHT+1*/ /*(src->y256)+((src->h128-s->h128))*/;
//
	/* �`��p�p�x */
//	s->m_angleCCW512		= (128);
	s->m_angleCCW512		= (0);
//
/*???*/ 	/*data->*/s->base_weapon_strength		= (1/*8*1*/);		/* ��{�� �̋��� */
//
}

/*---------------------------------------------------------
	�v���C���[�V�[���h����(���~���A)[���~���A�{���̏\�����{��]
---------------------------------------------------------*/

static void player_create_bomber_cross_red_parrent(SPRITE *src) /* ���~���A */ /* [***090220 �ǉ� */
{
	pd_state_flag		|= STATE_FLAG_02_BOMB_AUTO_GET_ITEM;	/*�{���ɂ�鎩�����W�\*/
	SPRITE *s;
	s					= sprite_add_gu(JIKI_ATARI_ITEM_16);
	s->flags			|= (/*SP_FLAG_VISIBLE|*/SP_FLAG_TIME_OVER);
	s->flags			&= (~(SP_FLAG_VISIBLE));	/*��\��*/
	s->color32			= 0xdcffffff;		/*	s->alpha			= 0xdc;*/
	s->callback_mover	= player_move_add_cross_red;
//	s->type 			= (SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO);/* �{�X�̒��ڍU���͋֎~ */		/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) �{�X�ɖ���*/
	s->type 			= (/*�\�����Ȃ�*/SP_GROUP_ETC_DUMMY_REMILIA/*SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO*/);/* �{�X�̒��ڍU���͋֎~ */		/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) �{�X�ɖ���*/
	#if 0/*SDL(�������W)*/
	s->x256 			= (src->x256)+((src->w128-s->w128));
	s->y256 			= (src->y256)+((src->h128-s->h128));
	#else/*Gu(���S���W)*/
	s->x256 			= (src->x256);
	s->y256 			= (src->y256);
	#endif
}



/*---------------------------------------------------------
	�v���C���[�U���e����
---------------------------------------------------------*/

static void player_add_hlaser(SPRITE *src)
{
	int j;
	j=(ra_nd()&(2-1));//for (j=0;j<2;j++)
	{
		SPRITE *s_old;	s_old = NULL;
		SPRITE *s;		s = NULL;
		//#define hlaser_NUM_OF_ENEMIES (24)
		#define hlaser_NUM_OF_ENEMIES (12)		/* [***090128		�����ɂ��Ă݂� */
		int i;
		for (i=0; i<hlaser_NUM_OF_ENEMIES; i++)
		#undef hlaser_NUM_OF_ENEMIES
		{
			s_old = s;
			s				= sprite_add_gu(JIKI_ATARI_ITEM_16);
//			s->type 		= (SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_SPECIAL)/*�{�X�ɗL��*/ /*SP_PL_HLASER*/;
			s->type 		= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_01|SP_GROUP_SHOT_SPECIAL)/*�{�X�ɗL��*/ /*SP_PL_HLASER*/;
			#if 0/*SDL(�������W)*/
			s->x256 		= (src->x256) + ((src->w128)) - t256(5);
			s->y256 		= (src->y256) + ((src->h128)) + t256(15);
			#else/*Gu(���S���W)*/
		//	s->x256 		= src->x256 - t256(5);
		//	s->y256 		= src->y256 + t256(15);
			s->x256 		= src->x256;
			s->y256 		= src->y256;
			#endif
		//	s->flags		|= (SP_FLAG_VISIBLE/*|SP_FLAG_TIME_OVER*/);
			s->flags			&= (~(SP_FLAG_VISIBLE));	/*��\��*/
			if (0==i)
			{
				s->callback_mover	= player_move_parrent_hlaser;
				PL_HOMING_DATA *data;
				data				= mmalloc(sizeof(PL_HOMING_DATA));
				s->data 			= data;
//				s->m_angleCCW512/*data->angleCCW512*/	= deg_360_to_512CCW(0);
				s->m_angleCCW512/*data->angleCCW512*/	= (j&1)?(deg_360_to_512CCW(  0+10)):(deg_360_to_512CCW(360-10));
				data->speed256		= t256(12.0);
				/*data->*/s->base_weapon_strength		= player_fix_status[BASE_LOW_BOMB_STRENGTH+select_player]/*6*/;/*�ᑬ�{���̋���*/
				data->time_out/*range*/ 				= (100);	/*(200)*/
				data->state 		= 0;
				data->target_obj	= player;	/* (data->target_obj==player)==������Ȃ� */
			}
			else
			{
				s->callback_mover	= player_move_kodomo_hlaser/*NULL*/;
				PL_HOMING_KODOMO_DATA *data;
				data				= mmalloc(sizeof(PL_HOMING_KODOMO_DATA));
				s->data 			= data;
			//	s->data 			= NULL;
				data->target_obj	= s_old;
				data->time_out		= 8;
			}
		}
	}
}


static void player_move_add_bomber_hlaser(SPRITE *src)
{

//	if (0==(((int)pd_bomber_time)&0x3f))
	if (0==(((int)pd_bomber_time)&0x0f))
	{
		/* �V�i���I���ɂ͗U���e��ǉ����Ȃ� */
		if (0==(pd_state_flag & STATE_FLAG_06_IS_SCRIPT))
		{
			player_add_hlaser(player);
		}
	}
	#if 0
	src->an im_frame	= (((int)pd_bomber_time>>4)&0x03);/*"bomber_slow.png"*/
	src->x256			= (player->x256)+((player->w128-src->w128));
	src->y256			= (player->y256)+((player->h128-src->h128));
	#endif
//
	if (0 >= pd_bomber_time) {	src->type = SP_DELETE;}/* ���Ԑ����ł����܂� */
}

/* �ᑬ�{���̐e */
static void player_create_bomber_homing_parrent(SPRITE *src)	/* player_move_add_bomber_hlaser */
{
	pd_state_flag		|= STATE_FLAG_02_BOMB_AUTO_GET_ITEM;	/* �{���ɂ�鎩�����W�\ */
//
	SPRITE *h;
	h					= sprite_add_gu(JIKI_ATARI_ITEM_16);
//	h->flags			|= (SP_FLAG_VISIBLE/*|SP_FLAG_TIME_OVER*/);
	h->flags			&= (~(SP_FLAG_VISIBLE));	/*��\��*/
	h->data 			= 0/*d*/;
//	h->anim_frame		= 0;
	h->color32			= 0xdcffffff;	/*	c->alpha			= 0x80 0xdc;*/
	h->callback_mover	= player_move_add_bomber_hlaser;
//	h->type 			= (SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_SPECIAL)/*�{�X�ɗL��*/; /* �ᑬ�{�����{�X�ɗL���Ƃ��� */	/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) �{�X�ɖ���*/
	h->type 			= (/*�\�����Ȃ�*/SP_GROUP_ETC_DUMMY_SLOW_BOMB/*SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_SPECIAL*/)/*�{�X�ɗL��*/;	/* �ᑬ�{�����{�X�ɗL���Ƃ��� */	/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) �{�X�ɖ���*/
	#if 1/*Gu(���S���W)*/
	h->x256 			= src->x256;
	h->y256 			= src->y256;
	#endif
//
	#if 1
	/* �{�������������A�L���ɂ���ƁA�d������B */
	/* ����@�\ */
	bullets_to_hosi();/* �e�S���A���A�C�e���ɂ��� */
	#endif
}

/*---------------------------------------------------------
	�얲��p �ᑬ�{��
---------------------------------------------------------*/

/*---------------------------------------------------------
	�얲��p �ᑬ�{��
---------------------------------------------------------*/

/*---------------------------------------------------------
	�얲��p �ᑬ�{��
---------------------------------------------------------*/

static void reimu_add_hlaser(SPRITE *src)
{
//	int j;
//	j = (ra_nd()&(2-1));//for (j=0;j<2;j++)
	{
		//#define hlaser_NUM_OF_ENEMIES (24)
//		#define hlaser_NUM_OF_ENEMIES (12)		/* [***090128		�����ɂ��Ă݂� */
		int i;
	//	for (i=0; i<(12)/*hlaser_NUM_OF_ENEMIES*/; i++)
		for (i=0; i<(16)/*hlaser_NUM_OF_ENEMIES*/; i++)
		{
			SPRITE *h;
			h				= sprite_add_gu(JIKI_ATARI_ITEM_16);
//			h->type 		= (SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_SPECIAL)/*�{�X�ɗL��*/ /*SP_PL_HLASER*/;
			h->type 		= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_01|SP_GROUP_SHOT_SPECIAL)/*�{�X�ɗL��*/ /*SP_PL_HLASER*/;
			#if 0/*SDL(�������W)*/
			h->x256 		= (src->x256) + ((src->w128)) - t256(5);
			h->y256 		= (src->y256) + ((src->h128)) + t256(15);
			#else/*Gu(���S���W)*/
		//	h->x256 		= src->x256 - t256(5);
		//	h->y256 		= src->y256 + t256(15);
			h->x256 		= src->x256;
			h->y256 		= src->y256;
			#endif
		//	h->flags		|= (SP_FLAG_VISIBLE/*|SP_FLAG_TIME_OVER*/);
			h->flags			&= (~(SP_FLAG_VISIBLE));	/*��\��*/
//			if (0==i)
			{
				h->callback_mover	= move_reimu_musou_fuuin;
				PL_HOMING_DATA *data;
				data				= mmalloc(sizeof(PL_HOMING_DATA));
				h->data 			= data;
//				h->m_angleCCW512/*data->angleCCW512*/	= deg_360_to_512CCW(0);
//				h->m_angleCCW512/*data->angleCCW512*/	= (0/*j&1*/)?(deg_360_to_512CCW(  0+10)):(deg_360_to_512CCW(360-10));
				h->m_angleCCW512/*data->angleCCW512*/	= (i<<5);
				data->speed256		= t256(1.2);
				/*data->*/h->base_weapon_strength		= player_fix_status[BASE_LOW_BOMB_STRENGTH+select_player]/*6*/;/*�ᑬ�{���̋���*/
			//	data->time_out/*range*/ 				= (100);	/*(200)*/
				data->reimu_musou_id					= (i);	/*(200)*/
				data->state 		= 0;
				data->target_obj	= player;	//b->target_obj/*target_id*/	= /*-1*/NULL;
			}
		}
	}
}


/*---------------------------------------------------------
	�얲��p �ᑬ�{��
---------------------------------------------------------*/

/* �ᑬ�{���̐e */
static void reimu_create_bomber_homing_parrent(SPRITE *src) 	/* reimu_move_add_bomber_hlaser */
{
	pd_state_flag		|= STATE_FLAG_02_BOMB_AUTO_GET_ITEM;	/* �{���ɂ�鎩�����W�\ */
//
	reimu_add_hlaser(player);
//
	#if 1
	/* �{�������������A�L���ɂ���ƁA�d������B */
	/* ����@�\ */
	bullets_to_hosi();/* �e�S���A���A�C�e���ɂ��� */
	#endif
}
	#if 0
	SPRITE *h;
	h					= sprite_add_gu(JIKI_ATARI_ITEM_16);
//	h->flags			|= (SP_FLAG_VISIBLE/*|SP_FLAG_TIME_OVER*/);
	h->flags			&= (~(SP_FLAG_VISIBLE));	/*��\��*/
	h->data 			= 0/*d*/;
//	h->anim_frame		= 0;
	h->color32			= 0xdcffffff;	/*	c->alpha			= 0x80 0xdc;*/
	h->callback_mover	= reimu_move_add_bomber_hlaser;
//	h->type 			= (SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_SPECIAL)/*�{�X�ɗL��*/; /* �ᑬ�{�����{�X�ɗL���Ƃ��� */	/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) �{�X�ɖ���*/
	h->type 			= (/*�\�����Ȃ�*/SP_GROUP_ETC_DUMMY_SLOW_BOMB/*SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_SPECIAL*/)/*�{�X�ɗL��*/;	/* �ᑬ�{�����{�X�ɗL���Ƃ��� */	/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) �{�X�ɖ���*/
	#if 1/*Gu(���S���W)*/
	h->x256 			= src->x256;
	h->y256 			= src->y256;
	#endif

	src->x256			= (player->x256)+((player->w128-src->w128));
	src->y256			= (player->y256)+((player->h128-src->h128));
	#endif
//


/*---------------------------------------------------------
	�v���C���[�L�[����
---------------------------------------------------------*/
	#if 0
	/*
		���u(��炢�{����t���Ԓ���)�{�X�Ƒ��ł�����ƃn���O�A�b�v�v�o�O(�`r29)�΍�
	*/
	/* �����ςȎd�l(KETM)���ȁA����Ȃ̗v��Ȃ��C������B
	�o�O�ɋ߂��d�l(����Ȃ��d�l�o�O)���ˁB
	�����Ɛ݌v�𕪂��Ȃ����炱���Ȃ�񂾂�Ȃ��B
	KETM�̓v���C�I���e�B�[�ł����蔻�������Ƃ��F�X������������B
	(����Ȏ�������A����KETM�ł��v���C�I���e�B�[�̏�������A�ς����Ȃ��Ȃ����Ⴄ(�d�l�ύX�ł��Ȃ�)�ł���H
	���ʂ́A����Ȗ����Ȑ݌v�͂��Ȃ��B)
	*/
	/* GAME OVER ���� pd_state_flag �� STATE_FLAG_16_NOT_ALLOW_KEY_CONTROL �őΏ�����B */
	//if player is invisible (state gameover) allow no keycontrol
	if ((0==(s1->flags&SP_FLAG_VISIBLE)))
	{
		return;/* �\�����Ȃ��ꍇ�͑��얳���B */
	}
	#endif

//static int /*bomb_wait*/d->bomber_time;		/* �{���̗L������ */	//���̃{�����o����܂ł̎���

extern /*global*/short my_analog_x; /* �A�i���O�ʁA�␳�ς� */
extern /*global*/short my_analog_y; /* �A�i���O�ʁA�␳�ς� */
extern void set_bg_alpha(int set_bg_alpha);

static void player_keycontrol(SPRITE *s1)
{
	/*---------------------------------------------------------
		�v���C���[�{���֘A����
	---------------------------------------------------------*/
//	if (STATE_FLAG_06_IS_SCRIPT==(pd_state_flag & STATE_FLAG_06_IS_SCRIPT))
//	{
//		;	/* ��b�C�x���g���̓{���֘A�̏����͂��Ȃ� */
//	}
//	else
		/* �V�i���I���ɂ͗U���e��ǉ����Ȃ� */
	if (0==(pd_state_flag & STATE_FLAG_06_IS_SCRIPT))
	{
	//	if (d->bomber_time>0)
		if (0 < /*bomb_wait*/pd_bomber_time)		/* �{���E�F�C�g���� */
		{
			//	d->bomber_time-=fps_fa_ctor;
			/*bomb_wait*/pd_bomber_time--/*-=fps_fa_ctor*/;/**/
			if (2 > /*bomb_wait*/pd_bomber_time)
			{
				set_bg_alpha(255);/* ��ʂ𖾂邭���� */
		//	}
		//	else
		//	if (/*bomb_wait*/pd_bomber_time < 190)
		//	{
				pd_state_flag &= (~STATE_FLAG_02_BOMB_AUTO_GET_ITEM);	/* �{���ɂ�鎩�����W�͏I��� */
			}
		}
		else
		{
			pd_bomber_time = 0;
		//}
		//if (/*bomb_wait*/d->bomber_time<=0)
		//{
			if (my_pad & PSP_KEY_BOMB_CANCEL)
			{
				if (0 < pd_bombs)
				{
					pd_bombs--;
					pd_used_bomber++;	/* �{���g�p�� */
					voice_play(VOICE07_BOMB, TRACK02_ALEART_IVENT);/*�e�L�g�[*/
					set_bg_alpha(50);/* ��ʂ��Â����� */
//					set_bg_alpha(100);/* ��ʂ��Â����� */
//					set_bg_alpha(127);/* ��ʂ��Â����� */
					/* �����G */
					player_create_bomber_tachie(player);
					//
					static /*const*/ void (*regist_call_table[/*16*/(PLAYERS8*2)])(SPRITE *src) =
					{
						player_create_bomber_kekkai_parrent,			reimu_create_bomber_homing_parrent, 		/* �얲 A */
						player_create_bomber_kekkai_parrent,			player_create_bomber_homing_parrent,		/* �얲 B */
						player_create_bomber_levarie_parrent,			player_create_bomber_homing_parrent,		/* ������ A */
						player_create_bomber_levarie_parrent,			player_create_bomber_homing_parrent,		/* ������ B */
						player_create_bomber_cross_red_parrent, 		player_create_bomber_homing_parrent,		/* ���~���A */
						player_create_bomber_levarie_parrent,			player_create_bomber_cross_red_parrent, 	/* �H�X�q */
						player_create_bomber_levarie_parrent,			player_create_bomber_homing_parrent,		/* �`���m A */
						player_create_bomber_levarie_parrent,			player_create_bomber_homing_parrent,		/* �`���m Q */
					};
						int index_aaa;
						index_aaa = ((select_player)+(select_player)+((my_pad & PSP_KEY_SLOW)?1:0));
					{
						(regist_call_table[(index_aaa)])(player);
					}
//	/* �ᑬ�{�� */			pd_bomber_time = 200/*320*/ /*400*/ /*800*/;	/* �{���̗L������(�ݒ薳�G����)[���P�d�l���ς�����̂Ŕ����ɂ���] */
//	/* �얲 */				pd_bomber_time = 180/*200*/;	/* �{���̗L������(�ݒ薳�G����) */	/* ������̓{�X�L��(�U���^) */
//	/* ������	�`���m */	pd_bomber_time = 100/*32*/ /*100*/; 		/* �{���̗L������(�ݒ薳�G����) */
//	/* �� �L������ */		pd_bomber_time = 255/*255==((16*4*2*2)-1)*/;			/* �{���̗L������(�ݒ薳�G����) */
//	���~���A�{��			pd_bomber_time = 0x7e	/* 255==0xff==8��	254==0xfe==7��==0xbf=192 4��==0x7f 150==0x96*/;

					#if 0
					static /*const*/ u8 pd_bomber_time_table[/*16*/(PLAYERS8*2)] =
					{
						180,		180,		/* 180 200 �얲 A */
						180,		200,		/* 180 200 �얲 B */
						100,		180,		/* 100 200 ������ A */
						100,		200,		/* 100 200 ������ B */
						0x7e,		255,		/* 0x7e 200���~���A */
						180,		180,		/* 255 200 �H�X�q */
						100,		180,		/* 100 200 �`���m A */
						100,		255,		/* 100 200 �`���m Q */
					};
					#endif
					pd_bomber_time		= 180;//pd_bomber_time_table[(index_aaa)];
					//	#define USER_BOMOUT_WAIT (30)
					pd_save_timer		= pd_bomber_time + USER_BOMOUT_WAIT;/*�������G����*/
					/*	�������G���� == (�ݒ薳�G���� + USER_BOMOUT_WAIT)�B 	�������G���� == �ݒ薳�G���� ���ƁA�킩��ɂ����̂Ŏ኱�̗]�T���K�v�B */
					s1->color32 		= 0x50ffffff;	/*	s1->alpha			= 0x50;*/	/* ������ */
					pd_core->color32	= 0x50ffffff;	/*	pd_core->alpha		= 0x50;*/	/* ������ */
					if (PLAYER_STATE_01_HIT_BOMB==pd_player_status)/* ��炢�{����t�� */
					{
						pd_use_kurai_bomb++;/* ��炢�{������ */
					}
					pd_player_status	= PLAYER_STATE_03_SAVE_02;
				//
					script_message_window_clear();/*�X�N���v�g���b�Z�[�W��ʂ�����*/
					{
						msg_time = pd_bomber_time;	/*(60*5)*/
						typedef struct
						{
							const char *spell_str_name; 	/* �X�y�J���� */
						} aaa_RESOURCE;
						static const aaa_RESOURCE my_aaa_resource[/*16*/(PLAYERS8*2)/*(PLAYERS8*2)*/] =
						{
//	 "feeddccbbaa99887766554433221100",'\0' 	 "feeddccbbaa99887766554433221100",'\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
	{"�아�u���z���E�E�~�v" 			"\n"},	{"�P���u���z����E���v" 			"\n"},//�아�u���z���E�E�~�v
	{"�����u���z���E�E�a�v" 			"\n"},	{"�����u���쐬�U���v"				"\n"},//�����u���z���E�E�a�v
	{"�����u�X�^�[�_�X�g�����@���G�v"	"\n"},	{"�M���u���쐬�U���v"				"\n"},
	{"�����u�}�X�^�[�X�p�[�N�v" 		"\n"},	{"�����u���쐬�U���v"				"\n"},
	{"�g���u�s���f�r���v" 			"\n"},	{"�����u�A�N�}�N���X�v" 			"\n"},
	{"�����u�M���X�g���h���[���v"		"\n"},	{"�����u���쐬�U���v"				"\n"},	/* dream cast. ghastly dream(�d�X��). ghastly dream(�얲). */
	{"�ŕ��u�A�C�X�r�����b�g�v" 		"\n"},	{"�X���u�I���U���b�N�v" 			"\n"},	/* ��(�����): �J����������A�n���������Ⴊ�Ăѓ����č~��B */
	{"���u�A�^�C�T�C�L���[�l�v"		"\n"},	{"�����u�t�~�t�~�L�b�N�v"			"\n"},	/* ��(�݂���): �J�ƐႪ�������č~��B���u */
						};
						print_kanji000(/*SDL_Surface *drawmap,*/ /*SDL_Rect *rect*/ /*0,*/
							my_aaa_resource[index_aaa].spell_str_name, //	aaa_str[(spell_card_number&15)],
							/*int color_type*/7, /*int wait*/0
						);
					}
				}
			}
		}
	}

//
	if (PLAYER_STATE_01_HIT_BOMB==pd_player_status) return;/* ��炢�{����t�� */
//


	/*---------------------------------------------------------
		�v���C���[�ړ��֘A����
	---------------------------------------------------------*/
	/*
		256�Œ菬���_�Ŏ��@�̈ړ���
		1.0dot	 == 256 == t256(1.0),			�c���̏ꍇ
		��0.7dot == 181 == t256(1.0/��2.0)		�΂߂̏ꍇ
	*/
	static const s16 jiki_move_length[16][2] =
	{
	/*LDRU*/ /* y x */
	// �΂߈ړ������߂���̂��C���B
	/*0000*/ {	  0,	0},
	/*0001*/ {	  0, -256},/*0 U*/
	/*0010*/ {	256,	0},/*2 R*/
	/*0011*/ {	181, -181},/*1 UR*/
	/*0100*/ {	  0,  256},/*4 D*/
	/*0101*/ {	  0,	0},
	/*0110*/ {	181,  181},/*3 DR*/
	/*0111*/ {	  0,	0},
	/*1000*/ { -256,	0},/*6 L*/
	/*1001*/ { -181, -181},/*7 UL*/
	/*1010*/ {	  0,	0},
	/*1011*/ {	  0,	0},
	/*1100*/ { -181,  181},/*5 DL*/
	/*1101*/ {	  0,	0},
	/*1110*/ {	  0,	0},
	/*1111*/ {	  0,	0},
	};
/* �ԉf�ˉ�b������T�C�g http://ambriel.hp.infoseek.co.jp/th9/  ��{���\��r �ړ����x(�������[�h)
														[51200/xxxF]
200/50		50F ��										0x0400[1024.000000000000000000000000000000]3.63636363636363636363636363636364
200/55		55F [������] �~�X�e�B�A (���f�B , �l�G) 	0x03a3[ 930.909090909090909090909090909091]3.63636363636363636363636363636364
200/58		58F [�`���m]								0x0373[ 882.758620689655172413793103448276]3.44827586206896551724137931034483
200/61		61F (��� , ���i�T) �Ă�					0x0347[ 839.344262295081967213114754098361]3.27868852459016393442622950819672		[REMILIA]
200/65		65F �����J									0x0314[ 787.692307692307692307692307692308]3.07692307692307692307692307692308
200/67		67F ����									0x02fc[ 764.179104477611940298507462686567]2.98507462686567164179104477611940
200/68		68F [�얲] ��� 							0x02f1[ 752.941176470588235294117647058824]2.94117647058823529411764705882353
200/72		72F ��������								0x02c7[ 711.111111111111111111111111111111]2.77777777777777777777777777777778		[YUYUKO]
200/91		91F �H��									0x0233[ 562.637362637362637362637362637363]2.19780219780219780219780219780220

�z��ړ����x(�ᑬ���[�h)
200/79		79F 	��									0x0288[ 648.101265822784810126582278481013]2.53164556962025316455696202531646
200/80		80F 	�~�X�e�B�A							0x0280[ 640.000000000000000000000000000000]2.50000000000000000000000000000000
200/83		83F 	[�`���m]							0x0269[ 616.867469879518072289156626506024]2.40963855421686746987951807228916
200/85		85F 	���								0x025a[ 602.352941176470588235294117647059]2.35294117647058823529411764705882
200/91		91F 	[������]							0x0233[ 562.637362637362637362637362637363]2.19780219780219780219780219780220
200/109 	109F	(��� , ���i�T) 					0x01d6[ 469.724770642201834862385321100917]1.83486238532110091743119266055046		[REMILIA]
200/114 	114F	(���f�B , �l�G) 					0x01c1[ 449.122807017543859649122807017544]1.75438596491228070175438596491228
200/124 	124F	�����J ���� 						0x019d[ 412.903225806451612903225806451613]1.61290322580645161290322580645161
200/130 	130F	�d��								0x018a[ 393.846153846153846153846153846154]1.53846153846153846153846153846154
200/136 	136F	�������� [�얲] 					0x0178[ 376.470588235294117647058823529412]1.47058823529411764705882352941176
200/145 	145F	�Ă�								0x0161[ 353.103448275862068965517241379310]1.37931034482758620689655172413793		[YUYUKO]
200/148 	148F	�H��								0x015a[ 345.945945945945945945945945945946]1.35135135135135135135135135135135

*/
//#define BASE_SPEED_MINIMUM		(PLAYERS8*4)
//#define BASE_SPEED_MAXIMUM		(PLAYERS8*5)
//	 2,  3,  4,  7,  3, 	/* �Œᑬ�x player_speed_minimum */
//	 4,  5,  4,  7,  3, 	/* �ō����x player_speed_maximum */
	static const signed /*int*/short player_speed256[(PLAYERS8*4)] =
	{
	/* �������[�h(�ʏ펞) */								/* �������[�h(�{��������) */
	/* REIMU_A */	0x02f1,/* t256(2.94) 2.94140625 		  t256(3.0), */
	/* REIMU_B */	0x02f1,/* t256(2.94) 2.94140625 		  t256(3.0), */
	/* MARISA_A */	0x03a3,/* t256(3.64) 3.63671875 		  t256(2.0), */
	/* MARISA_B */	0x03a3,/* t256(3.64) 3.63671875 		  t256(2.0), */
	/* REMILIA */	0x0347,/* t256(3.28) 3.27734375 		  t256(5.0), */
	/* YUYUKO */	0x02c7,/* t256(2.78) 2.77734375 		  t256(5.0), */
	/* CIRNO_A */	0x0373,/* t256(3.45) 3.44921875 		  t256(4.5), */ 			/* �H������ᑬ�̕������� */
	/* CIRNO_Q */	0x0373,/* t256(3.45) 3.44921875 		  t256(4.5), */ 			/* �H������ᑬ�̕������� */
	/* �ᑬ���[�h(�ʏ펞) */								/* �ᑬ���[�h(�{��������) */
	/* REIMU_A */	0x0178,/* t256(1.47) 1.46875000 t256(2.0) t256(2.0), */
	/* REIMU_B */	0x0178,/* t256(1.47) 1.46875000 t256(2.0) t256(2.0), */
	/* MARISA_A */	0x0233,/* t256(2.20) 2.19921875 t256(2.5) t256(2.0), */
	/* MARISA_B */	0x0233,/* t256(2.20) 2.19921875 t256(2.5) t256(2.0), */
	/* REMILIA */	0x01d6,/* t256(1.84) 1.83593750 t256(2.5) t256(4.0), */
	/* YUYUKO */	0x0161,/* t256(1.38) 1.37890625 t256(2.0) t256(4.5), */
	/* CIRNO_A */	0x0269,/* t256(2.41) 2.41015625 t256(7.0) t256(5.0), */ /* 0x0700 �H������ᑬ�̕������� 0x0400==��������==���z���ő��H */
	/* CIRNO_Q */	0x0700,/* t256(2.41) 2.41015625 t256(7.0) t256(5.0), */ /* 0x0700 �H������ᑬ�̕������� 0x0400==��������==���z���ő��H */
	};
	/* ���@���x�����߂�B */
	int is_slow;
	is_slow = (my_pad & PSP_KEY_SLOW);
	/*const*/ signed int my_speed = player_speed256[select_player + ((is_slow)?(PLAYERS8):(0))];
	/* �ړ��ʂ����߂�B(�ړ��� = ���@���x x �A�i���O�L�[�A�f�W�^���̏ꍇ�͗\�߃A�i���O�L�[�ړ��ʂɕϊ����Ă���) */
	short	aaa_my_analog_x = (((my_speed)*(my_analog_x))>>8);
	short	aaa_my_analog_y = (((my_speed)*(my_analog_y))>>8);
	/* �΂߂��l�����Ĉړ�����B */
	s1->x256 += ((((signed int)(jiki_move_length[((my_pad&0xf0)>>4)][0]))*(aaa_my_analog_x))>>8);	/*fps_factor*/
	s1->y256 += ((((signed int)(jiki_move_length[((my_pad&0xf0)>>4)][1]))*(aaa_my_analog_y))>>8);	/*fps_factor*/
	/* ��ʊO�ɁA�݂͂�������C���B */
	#if 0/*SDL(�������W)*/
		 if (s1->x256 < t256(0))									{	s1->x256 = t256(0); 									}/* ���`�F�b�N */
	else if (s1->x256 > t256(GAME_WIDTH) -((s1->w128+s1->w128)))	{	s1->x256 = t256(GAME_WIDTH) -((s1->w128+s1->w128)); 	}/* �E�`�F�b�N */
		 if (s1->y256 < t256(0))									{	s1->y256 = t256(0); 									}/* ��`�F�b�N */
	else if (s1->y256 > t256(GAME_HEIGHT)-((s1->h128+s1->h128)))	{	s1->y256 = t256(GAME_HEIGHT)-((s1->h128+s1->h128)); 	}/* ���`�F�b�N */
	#else/*Gu(���S���W)*/
//		 if (s1->x256 < t256(0))				{	s1->x256 = t256(0); 			}/* ���`�F�b�N */
//	else if (s1->x256 > t256(GAME_WIDTH)  ) 	{	s1->x256 = t256(GAME_WIDTH) ;	}/* �E�`�F�b�N */
//		 if (s1->y256 < t256(0))				{	s1->y256 = t256(0); 			}/* ��`�F�b�N */
//	else if (s1->y256 > t256(GAME_HEIGHT) ) 	{	s1->y256 = t256(GAME_HEIGHT);	}/* ���`�F�b�N */
		 if (s1->x256 < t256(0))				{	s1->x256 = t256(0); 			}/* ���`�F�b�N */
	else if (s1->x256 > t256(GAME_WIDTH-31)  )	{	s1->x256 = t256(GAME_WIDTH-31); 	}/* �E�`�F�b�N */
		 if (s1->y256 < t256(0/*+16*/)) 		{	s1->y256 = t256(0/*+16*/);			}/* ��`�F�b�N */
	else if (s1->y256 > t256(GAME_HEIGHT-16) )	{	s1->y256 = t256(GAME_HEIGHT-16);	}/* ���`�F�b�N */
	#endif
	/* �R�A�ړ� */
	{
		SPRITE *s2;
		s2 = pd_core;
		if (is_slow)	{	s2->type		&= (~(0x08));/*s2->flags		|= ( (SP_FLAG_VISIBLE));*/	}	/* ���A�\�� */
		else			{	s2->type		|= ( (0x08));/*s2->flags		&= (~(SP_FLAG_VISIBLE));*/	}	/* ���A��\�� */
		#if 0/*SDL(�������W)*/
		s2->x256 = s1->x256+((s1->w128-s2->w128))	-(t256( 1));	/* -(t256( 1)) �Ƃ肠���� */
		s2->y256 = s1->y256-((s2->h128+s2->h128))	+(t256(20));	/* +(t256(20)) �Ƃ肠���� */
		#else/*Gu(���S���W)*/
	//	s2->x256 = s1->x256 -(t256( 1));	/* -(t256( 1)) �Ƃ肠���� */
	//	s2->y256 = s1->y256 +(t256(20));	/* +(t256(20)) �Ƃ肠���� */
		s2->x256 = s1->x256 +(t256((31-5)/2));	/* -(t256( 1)) �Ƃ肠���� */	/* �R�A�I�t�Z�b�g */
		s2->y256 = s1->y256 +(t256((31-5)/2));	/* +(t256(20)) �Ƃ肠���� */	/* �R�A�I�t�Z�b�g */
		#endif
	}
	/* MAX���̃A�C�e���������W */
	if (my_pad & PSP_KEY_UP/*PSP_CTRL_UP*/) 	/* ���F�΂ߏ�ł�����\ */ /*&& (s1->y>0)*/
	{
		#if 1
			/* �S�L����: �A�C�e���㕔���W�����ł��\ */
		#else
		if ((pd_weapon_power==MAX_POWER_IS_128)  //128[***090123 �ύX /*max==MAX_POWER_IS_128==�u129�i�K�v*/
			/* ������ ����\�́F�A�C�e���㕔���W�����ł��\ */
			|| (MARISA_A==select_player)
			|| (MARISA_B==select_player)
		//	|| (MARISA_C==select_player)
		)	/* �������͏�ɏ㕔�������W���\ */
		#endif
		{
			if (s1->y256 < PLAYER_SPLIT_LINE256/*t256(50)*/)/* [FULL�p���[���̃A�C�e�������񂹃��C��] */
			{
				pd_state_flag |= STATE_FLAG_01_PLAYER_UP_AUTO_GET_ITEM; /* �㕔�������W�\ */
			}
		}
	}
	else if (my_pad & PSP_KEY_DOWN/*PSP_CTRL_DOWN*/)	/* ���F�΂߉��ł���߂� */ /*&& (s1->y<screen->h*-s1->h)*/
	{
		//if (pd_state_flag & (STATE_FLAG_01_PLAYER_UP_AUTO_GET_ITEM /*| STATE_FLAG_02_BOMB_AUTO_GET_ITEM*/) )/*???���Ԃ�*/
		{
			pd_state_flag &= (~STATE_FLAG_01_PLAYER_UP_AUTO_GET_ITEM);	/* �㕔�������W�s�� */
		}
	}
//
		/*---------------------------------------------------------
			�v���C���[�V���b�g�֘A����
		---------------------------------------------------------*/
	/* �V�i���I�X�N���v�g���[�h�ł̓V���b�g�{�^�����͖��� */
	if (0==(pd_state_flag & STATE_FLAG_06_IS_SCRIPT))
	{
		if (my_pad & PSP_KEY_SHOT_OK)
		{
			pd_state_flag |= STATE_FLAG_15_KEY_SHOT;	/* on */
			//
			if (
				(MARISA_B==select_player) &&	/* ������B(����) */
				(0<pd_bomber_time)				/* �}�X�^�[�X�p�[�N�� */
			)
			{
				;/* �}�X�^�[�X�p�[�N���̓V���b�g�����ĂȂ��炵�� */
			}
			else
			{
				{static int weapon_interval=0;
					/*pd_*/weapon_interval--/*=fps_fa_ctor*/;
					if (/*pd_*/weapon_interval < 1 )
					{
						static const u8 weapon_tbl[(WEAPON_L_MAX)*(PLAYERS8)] =
						{/* REIMU(A/B) MARISA(A/B) REMILIA YUYUKO CIRNO(A/Q) */    /* ���~���A��������̂Œ���(+5) */
								 5, 5,	6, 6,  7+5,  9+8,  9,  9,	/* WEAPON_L0(P000-P008) */
								 5, 5,	6, 6,  6+5,  8+8,  9,  9,	/* WEAPON_L1(P008-P015) */
								 5, 5,	5, 5,  7+5,  7+8,  9,  9,	/* WEAPON_L2(P016-P031) */
								 5, 5,	5, 5,  6+5,  6+8,  9,  9,	/* WEAPON_L3(P032-P063) */
								 5, 5,	5, 5,  5+5,  5+8,  9,  9,	/* WEAPON_L4(P064-P127) */
								 5, 5,	5, 5,  5+5,  5+8,  9,  9,	/* WEAPON_L5(P128)		*/
						};
						/*pd_*/weapon_interval = weapon_tbl[(weapon_level_offset)];
//						/*pd_*/weapon_interval = weapon_tbl[(weapon_List<<3)+select_player];
//
						voice_play(VOICE00_SHOT, TRACK00_BULLETS);
						static /*const*/ void (*bbb[(WEAPON_L_MAX)*(PLAYERS8)])(SPRITE *sss) =
						{
	/*REIMU_A*/ 	/*REIMU_B*/ 	/*MARISA_A*/	/*MARISA_B*/	/*REMILIA*/ 	/*YUYUKO ���i�K���� */	/*CIRNO_A*/ 	/*CIRNO_Q*/
	shot_regist_1,	shot_regist_1,	shot_regist_1,	shot_regist_1,	shot_regist_1,	shot_regist_chou,		shot_regist_2,	shot_regist_2,	/* WEAPON_L0:(P000-P008) */
	shot_regist_1,	shot_regist_1,	shot_regist_2,	shot_regist_2,	shot_regist_1,	shot_regist_chou,		shot_regist_2,	shot_regist_2,	/* WEAPON_L1:(P008-P015) */
	shot_regist_2,	shot_regist_2,	shot_regist_2,	shot_regist_3,	shot_regist_2,	shot_regist_chou,		shot_regist_2,	shot_regist_3,	/* WEAPON_L2:(P016-P031) */
	shot_regist_3,	shot_regist_3,	shot_regist_3,	shot_regist_4,	shot_regist_2,	shot_regist_chou,		shot_regist_3,	shot_regist_3,	/* WEAPON_L3:(P032-P063) */
	shot_regist_3,	shot_regist_4,	shot_regist_3,	shot_regist_4,	shot_regist_2,	shot_regist_chou,		shot_regist_3,	shot_regist_3,	/* WEAPON_L4:(P064-P127) */
	shot_regist_4,	shot_regist_5,	shot_regist_3,	shot_regist_5,	shot_regist_2,	shot_regist_chou,		shot_regist_3,	shot_regist_3,	/* WEAPON_L5:(P128) 	 */
						};
						(bbb[(weapon_level_offset)])(s1);
					}
				}
			}
		}
	}
	#if 0/*�f�o�b�O�p*/
	pd_my_score 	= (weapon_level_offset);/*�f�o�b�O*/
	pd_graze_point	= (weapon_level_offset);/*�f�o�b�O*/
	#endif

	/*---------------------------------------------------------
		�v���C���[�A�j���[�V�����֘A����
	---------------------------------------------------------*/
	{static int anime_delay;
		/*pd_*/anime_delay -= 1/*fps_fa_ctor*/;
		if (0 > /*pd_*/anime_delay)
		{
			/*pd_*/anime_delay = player_fix_status[BASE_SPEED_ANIME+select_player]/*2*/;
			//
			static int auto_anime_frame = 4/*5*/;
				 if (my_pad & PSP_KEY_LEFT/*PSP_CTRL_LEFT*/)	{	if (auto_anime_frame>0	)	 auto_anime_frame--; }
			else if (my_pad & PSP_KEY_RIGHT/*PSP_CTRL_RIGHT*/)	{	if (auto_anime_frame</*7*/8/*10*/) auto_anime_frame++; }
			else
			{
				if (auto_anime_frame>4/*5*/) auto_anime_frame--;
				if (auto_anime_frame<4/*5*/) auto_anime_frame++;
			}
			{
				int now_anime_frame;
				static int auto_aniime;
				auto_aniime++;
				auto_aniime&=3;
					 if (4==auto_anime_frame)	{	now_anime_frame = (auto_anime_frame + auto_aniime); }
				else if (4<auto_anime_frame)	{	now_anime_frame = (auto_anime_frame + 3);			}
				else							{	now_anime_frame = (auto_anime_frame );				}
			//	s1->an im_frame = (now_anime_frame);
				s1->type = (SP_GROUP_JIKI_GET_ITEM)|(now_anime_frame);
			}
		}
	}
/*
0 1 2 3 4 5 6  7  8
0 1 2 3 4 8 9 10 11
		5
		6
		7
*/
}

/*---------------------------------------------------------
	���탌�x�����ύX���ꂽ�ꍇ�ɁA�����I�v�V�����̏�Ԃ��`�F�b�N���ĕύX
---------------------------------------------------------*/
/* [pd_weapon_power���ύX���ꂽ�ꍇ�ɕK���s����`�F�b�N] */
static void check_weapon_level(void)
{
	/*---------------------------------------------------------
		����̒i�K�����߂�
	---------------------------------------------------------*/
	//weapon�̒i�K���獡�̑��������߂�		//���X�g�����đ����̕ύX
	// [***090123	�ő�128�ցB
	// [***100123	�ő�129�ցB
	// (0-128��129�i�K�ɏC��)
	#if 0//1968661
	/* ���낢��撣���Ă݂����A���̒���(6,11,61,86)�͒N��(?)�Ȃ̂ŕς���(2�{�𒴂���l�̒����͈Ӗ������C��������) */
	u8 pd_weapon;	pd_weapon = pd_weapon_power;
		 if (pd_weapon < (	8)) {	weapon_level_offset = (WEAPON_L0<<3);	}	/* WEAPON_L0(P000-P008) */	/* 6*/
	else if (pd_weapon < ( 16)) {	weapon_level_offset = (WEAPON_L1<<3);	}	/* WEAPON_L1(P008-P015) */	/*11*/
	else if (pd_weapon < ( 32)) {	weapon_level_offset = (WEAPON_L2<<3);	}	/* WEAPON_L2(P016-P031) */	/*61*/
	else if (pd_weapon < ( 64)) {	weapon_level_offset = (WEAPON_L3<<3);	}	/* WEAPON_L3(P032-P063) */	/*86*/
	else if (pd_weapon < (128)) {	weapon_level_offset = (WEAPON_L4<<3);	}	/* WEAPON_L4(P064-P127) */
	else						{	weapon_level_offset = (WEAPON_L5<<3);	}	/* WEAPON_L5(P128)		*/	/*max==P128==�u129�i�K�v*/
	weapon_level_offset += select_player;
	#endif
	#if 1//1968181
	u8 pd_weapon;	pd_weapon = pd_weapon_power;
	/* 0000 0001 1111 1--- */
	/* �g�A���ׂ��璚�x2�{���ۂ��B */
	{	/* ���x2�{���Ƃ���Ȋ����Ŋȗ����ł���B */
		weapon_level_offset = (select_player/*0*/);
		weapon_level_offset += (WEAPON_L5<<3);
		int jj;
		for (jj=(0); jj<(5); jj++)
		{
			if (pd_weapon & ((0x80)>>jj))
			{
				goto loop_end;//break;
			}
			weapon_level_offset -= (0x08);
		}
		loop_end:;
	}/*
0000 0000 0000 0000
		  5432 1 0
		  0123 4
*/
	#endif
//
	/*---------------------------------------------------------
		�v���C���[�I�v�V�����`�F�b�N(�K�v�ł���΃I�v�V�������Z�b�g)
	---------------------------------------------------------*/
	#if 1
	{
		/* ���낢��撣���Ă݂����A���̒���(35,55,80,108)�͒N��(?)�Ȃ̂ŕς���(2�{�𒴂���l�̒����͈Ӗ������C��������) */
	//	static const u8 jjj_tbl[4] =
	//	{
	//		( 8),		//(35-1),
	//		(16),		//(55-1),
	//		(32),		//(80-1),
	//		(64)		//(108-1),
	//	};
		int jj;
		for (jj=(0); jj<(4); jj++)/* �I�v�V������4�� */
		{
			if ( (pd_weapon > ((0x08)<<jj)/*jjj_tbl[jj]*/)) 	/* ���x2�{���Ƃ���Ȋ����Ŋȗ����ł���B */
					{	option[jj]->flags	|= ( (SP_FLAG_VISIBLE));	}	// ���t���O��On(��)
			else	{	option[jj]->flags	&= (~(SP_FLAG_VISIBLE));	option[jj]->y256=t256(GAME_HEIGHT+1);	}	// ���t���O��OFF(�s��)
		}
	}
	#endif
}
/*
����
;-------------------
�얲 �아	�V���b�g�i�z�[�~���O�A�~�����b�g�j		�{�� �i���z����j

L0	1way(�܂�������)								P000-P007			(���i1way)
�I�v�V����2�ǉ�									P008-P015
L1	2way(�E��A����)								P016-	28			(�Ԋu������2way)

L3	3way(�܂��������A�E�Ώ�A���Ώ�)				P032-	43			(�Ԋu���L��3way)

Lx	3way(�܂��������A�E�Ώ�A���Ώ�)				P064-	113 	124 (�Ԋu���L��3way)

124 �I�v�V����3����

Lx	4way(�܂������E�A�܂��������A�E�Ώ�A���Ώ�)			MAX(128)	(�Ԋu���L��4way)

MAX(128)->����->(112)->����->(96)->����->(80)->����->(64)
;-------------------
�얲 ����	�V���b�g�i�p�X�E�F�C�W�����j�[�h���j	�{�� �i�����w�j

;-------------------
������ ���� �V���b�g�i�}�W�b�N�~�T�C���j			�{�� �i�X�^�[�_�X�g�����@���G�j

;-------------------
������ ���� �V���b�g�i�C�����[�W�������[�U�[�j		�{�� �i�}�X�^�[�X�p�[�N�j

;-------------------
*/

/*---------------------------------------------------------
	�v���C���[�ƃA�C�e���̂����蔻��1(�A�C�e����p)
	-------------------------------------------------------
		src 	�v���C���[
		ttt 	�A�C�e��
---------------------------------------------------------*/
static void player_add_power(SPRITE *src, SPRITE *ttt, int add_power)
{
	int prev_power; prev_power = pd_weapon_power;		/* �A�C�e���擾���钼�O�̃p���[�l */
	pd_weapon_power += add_power;	/* �p���[�l�A���Z */
	/* �p���[�l�A�����Ă���C�� */
	if ((MAX_POWER_IS_128-1) < pd_weapon_power )/*127==128�i�K��*/
	{
		pd_weapon_power = MAX_POWER_IS_128;/* �ő�l */
	}
	int add_score_point;
	if (MAX_POWER_IS_128 == pd_weapon_power/*prev_power*/) /* max�̏ꍇ */
	{
		pd_chain_point += add_power;	/* �`�F�C���A���Z */
		/* �`�F�C���A�����Ă���C�� */
		if (31 < pd_chain_point)
		{
			pd_chain_point = 31;/* �ő�l */
		}
		/* (�m�[�~�X�{�[�i�X)�E�F�|���{�[�i�X���_�v�Z */
		add_score_point = (/*score_type*/(pd_chain_point));
	}
	else	/* max�łȂ��ꍇ�A���_�v�Z */
	{
		/* (MAX���ȊO��) [P]��10�_ */
		/* (MAX���ȊO��) [P��]��80�_ */
		add_score_point = ((add_power/*-1*/)/*SCORE_10*/);
	}
	bonus_info_score_nodel(ttt, add_score_point);/* */ttt->type = SP_DELETE;/* �����܂� */
	check_weapon_level();/* [pd_weapon_power���ύX���ꂽ�ꍇ�ɕK���s����`�F�b�N] */
//
	voice_play(VOICE05_BONUS, TRACK07_GRAZE);/*�e�L�g�[*/
}

static void player_colision_check_item(SPRITE *src/*, int mask*/)
{
	/* ���@���A�C�e���ɂ��������ꍇ */
	SPRITE *ttt;	//�Ώ�
	ttt = sprite_collision_check_444(src, SP_GROUP_ITEMS);	/* �e����p(�A�C�e��) */	/*mask*/
	if (NULL != ttt)
	{

		switch (ttt->type)
		{
		case SP_ITEM_00_P001:	player_add_power(src, ttt, 1);		break;	// �E�F�|���A�C�e��(��P)
		case SP_ITEM_01_P008:	player_add_power(src, ttt, 8);		break;	// �E�F�|���A�C�e��(��P)	// [***090123		�ǉ�
		case SP_ITEM_02_P128:	player_add_power(src, ttt, 127);	break;	// �E�F�|���A�C�e��(F)		// [***090123		�ǉ�
		//
		case SP_ITEM_04_BOMB:
			//player_add_bomb(t);		// [*****�{���̓R�����g�A�E�g���Ȃ���
			if ((8-1) < pd_bombs)	{	goto add_10000pts;	}/* ���ɍő�l(8)�Ȃ�΁A10000+ [pts] */
			pd_bombs++;
			#if 1/*�o�Ofix?*/
			ttt->type = SP_DELETE;/* �����܂� */
			#endif
			voice_play(VOICE05_BONUS, TRACK07_GRAZE);/*�e�L�g�[*/
			break;

		case SP_ITEM_03_1UP:
			if ((8-1) < pd_zanki)	{	goto add_10000pts;	}/* ���ɍő�l(8)�Ȃ�΁A10000+ [pts] */
			pd_zanki++;
			#if 1/*�o�Ofix?*/
			ttt->type = SP_DELETE;/* �����܂� */
			#endif
			/*effect_sound_number=*/voice_play(VOICE06_EXTEND, TRACK03_SHORT_MUSIC);/*�e�L�g�[*/
			break;

		case SP_ITEM_07_SPECIAL:
			break;
		#if 0
		/* ���_�́A�����蔻��Ȃ� */
		case S P_ITEM_05_HOSI:		/*not_break;*/
			ttt->type = SP_DELETE;/* ���_�̂ݓ��ʏ��� */
//
			voice_play(VOICE05_BONUS, TRACK07_GRAZE);/*�e�L�g�[*/
			break;
		#endif
		case SP_ITEM_05_TENSU:
			{
				/* PLAYER_SPLIT_LINE256 ����Ŏ��� 10000pts. ... ���Ŏ���(?)��100pts. */
				/* (���90pts�A���ɓ�����A����΂��(�����钼�O��3���C��)70pts�܂ŉ\�炵��) */
			//
				int add_score_point;
				if (PLAYER_SPLIT_LINE256 < src->y256)	/* PLAYER_SPLIT_LINE256 �����̏ꍇ�́APLAYER_SPLIT_LINE256�܂ł̋����ɂ������Č��_ */
				{
				//	add_score_point -= ((ttt->y256-PLAYER_SPLIT_LINE256)>>(4+8));
				//	add_score_point = ((/*0x1f-(3)*/(SCORE_9000)-((ttt->y256-PLAYER_SPLIT_LINE256)>>(3+8)))/*&0x1f*/);
					add_score_point = (SCORE_9000) - ((ttt->y256-PLAYER_SPLIT_LINE256)>>(3+8));
				}
				else
				{
					add_10000pts:/* �����ɓ��_10000+ [pts] */
				//	add_score_point = (SCORE_10000);	/* ��{�_ 10000[pts] */
					add_score_point = (SCORE_10000+difficulty); 	/* ��{�_ 10000[pts](easy), 11000(normal), 12000(hard), 512000(lunatic). */
				}
				//player_add_score(score(1000));		// [***090123		�ύX
			//
				bonus_info_score_nodel(ttt, add_score_point);/* */ttt->type = SP_DELETE;/* �����܂� */
//
			}
			voice_play(VOICE05_BONUS, TRACK07_GRAZE);/*�e�L�g�[*/
			break;
		}
	}
}


/*---------------------------------------------------------
	�҂���[�񏈗�(�T�u)
	-------------------------------------------------------
	�v���C���[�� �G�O���[�v �܂��� �G�e �ɓ��������ꍇ�A
	��炢�{���J�n�ׂ̈́A�҂���[�񏈗����s���B
---------------------------------------------------------*/
static void player_set_pichuun(void)
{
	pd_player_status	= PLAYER_STATE_01_HIT_BOMB;
	pd_save_timer		= player_fix_status[BASE_HIT_BOMB_WAIT+select_player];/*0*/
	voice_play(VOICE04_SHIP_HAKAI, TRACK03_SHORT_MUSIC/*TRACK01_EXPLODE*/);/* ���@���ɉ��́A�Ȃ�ׂ��d�˂Ȃ� */
}


/*---------------------------------------------------------
	�v���C���[�ƓG�̂����蔻��3(�G��p)
	-------------------------------------------------------
		src 	�v���C���[
		ttt 	�G
	-------------------------------------------------------
	���@���G(�{�X/���^/�U�R)�ɑ̓����肳�ꂽ�ꍇ
---------------------------------------------------------*/

static void player_colision_check_enemy(SPRITE *src)//, int player_hit_enemy_group_mask)
{
	SPRITE *ttt;	/* �G����(�{�X/���^/�U�R) */
	ttt = sprite_collision_check_SDL_teki(src);//, (SP_GROUP_TEKI)/*player_hit_enemy_group_mask*/ /*SP_GROUP_ENEMYS*/);/*�G��p*/
	if (NULL != ttt)	{	player_set_pichuun();	}
}


/*---------------------------------------------------------
	�v���C���[�ƓG�e�̂����蔻��2(�O���C�Y��p)
	-------------------------------------------------------
		src 	�v���C���[
		ttt 	�G�e
---------------------------------------------------------*/

static void player_colision_check_graze(SPRITE *src)
{
	SPRITE *ttt;	/* �G�e */
	ttt = sprite_collision_check_444(src, SP_GROUP_BULLETS);/* �e����p(�e��) */
	if (NULL != ttt)
	{
		/* �v���C���[�ɓG�e�����������ꍇ�̓O���C�Y���� */
		if (0==(ttt->flags & SP_FLAG_GRAZE)) /* �O���C�Y��? */
		{
			ttt->flags |= SP_FLAG_GRAZE;/*�O���C�Y��*/
			pd_graze_point++;/*�O���C�Y��*/
			player_add_score((score(100)+score(200)*difficulty));/*score(500)*/
			voice_play(VOICE09_GRAZE, TRACK07_GRAZE);/*�e�L�g�[*/
		}
		/* �v���C���[�X�v���C�g���e�ɐG��Ă��邩 */
		{

			SPRITE *s2;
			s2 = pd_core;
			/* (�O���C�Y���Ȃ�R�A�ƓG�e�̂����蔻�������) */
			ttt = sprite_collision_check_444(s2, SP_GROUP_BULLETS);/* �e����p(�e��) */
			if (NULL != ttt)	{	player_set_pichuun();	}
		}
	}
}


/*---------------------------------------------------------
	�v���C���[�ړ�
---------------------------------------------------------*/

/*---------------------------------------------------------
	�v���C���[���[�v�𔲂��鏈��
---------------------------------------------------------*/
void player_loop_quit(void)
{
	pd_state_flag		|= STATE_FLAG_14_GAME_LOOP_QUIT;
	#if (0000)
	pd_state_flag		&= (~STATE_FLAG_08_OPTION_HIDE);	/* �I�v�V���������� */
	#endif/*0000*/
//
	pd_save_timer		= (150);	/* 150	 120�ł͎኱�Z�� 100 */
	pd_player_status	= PLAYER_STATE_04_GAME_OUT;/* GAME_OUT�� */
	#if (00)
	player->flags			&= (~(SP_FLAG_VISIBLE));	/* ���@�A��\�� */
	pd_core->flags	&= (~(SP_FLAG_VISIBLE));	/* ���A��\�� */
//	pd_core->alpha	= 0x00; 		// �����������߂ɓ��ꂽ���ǈӖ�����������������Ȃ��B	// �Q�[���I�[�o�[���́��̕\������
	#endif
}

/*---------------------------------------------------------
	�v���C���[��������
---------------------------------------------------------*/
void player_few_muteki(void)
{
	pd_save_timer		= (40); 					// ���G���� 		/*120*/ /*150-120*/
	pd_player_status	= PLAYER_STATE_02_SAVE_01;	// ���G��ԁH
}

static void player_fukkatsu(SPRITE *s1)
{
	player_few_muteki();/* �X�e�[�W�J�n���̂ݎ኱�̖��G��ԂɃZ�b�g */
	#if (00)
	player->flags			|= ( (SP_FLAG_VISIBLE));	/* ���@�A�\�� */
	#endif
	player->color32 		= 0x50ffffff;				/* ���@�A������ */	/*	s1->alpha			= 0x50;*/
//


//	pd_core->flags	|= ( (SP_FLAG_VISIBLE));	/* ���A�\�� */
	pd_core->color32	= 0x50ffffff;				/* ���A������ */	/*	pd_core->alpha	= 0x50;*/
	pd_bombs			= option_config[OPTION_CONFIG_01_BOMB]; 	/* �{���������̏����� */	/*player_fix_status[BASE_BOMBS+select_player]*/  /*3*/
	pd_chain_point	= (0);										/* �m�[�~�X�{�[�i�X�̏����� */	// pd_chain_point(�m�[�~�X�{�[�i�X)�̏����� // �ǂꂾ���A��(�m�[�~�X)��wepon_up���������
//
	check_weapon_level();/* [pd_weapon_power���ύX���ꂽ�ꍇ�ɕK���s����`�F�b�N] */
}

/*---------------------------------------------------------
	�v���C���[���ꏈ��
---------------------------------------------------------*/

static void player_explode(SPRITE *s1)
{
	/* �A�C�e����f���o�� */	//	[***090124������ƒǉ�
	{
		if ((0==pd_zanki)
			/*&&(0==difficulty)*/	/* (�g�A���ׂ����Փx�Ɋ֌W�Ȃ�[F]���o��)	*/
		)
		{
			/* �R���e�B�j���[�̏ꍇ(GAME_OUT)easy �̏ꍇ */
			/* ���ʂ�8�f������  */
			/* �R���e�B�j���[�̏ꍇeasy �̏ꍇ�F (�L���b�`�o���鐔�œ_���͈Ⴄ���ǁA�ǂꂩ�P�L���b�`����� POWER �͓���������) */
		//	item_create(s1, (0==difficulty)?(SP_ITEM_02_P128):(SP_ITEM_01_P008), 8, ITEM_MOVE_FLAG_06_RAND_XY);
			item_create(s1, (SP_ITEM_02_P128), 8, ITEM_MOVE_FLAG_06_RAND_XY);
		}
		else
		{
			/* �R���e�B�j���[�̏ꍇnormal�ȏ�̏ꍇ�F FIRE_POWER_G (8) x (8) == 64/128 (�S���W�߂�50%) �B */
			/* �R���e�B�j���[�̏ꍇ(GAME_OUT)normal�ȏ�̏ꍇ */
			/* �R���e�B�j���[�ȊO�̏ꍇ */
			/* �ɒ[�Ɏキ�Ď��񂾏ꍇ�̋~�ύ�:
				  ��肢�l��(���)���Ղ� FULLPOWER �ɂȂ�Ǝv�����A
				���ɖ}�~�X�Ŏ��񂾂Ƃ��Ă��{������Ȃǂ� POWER ���ێ�����̂ŁA
				�ɒ[�Ɏキ�Ď��ʎ��͂قڂȂ��B
				  �ɒ[�Ɏキ�Ď��ʂ̂͌��\���肾�Ǝv���̂ŁA�኱�̋~�ύ􂪕K�v���Ǝv���B
				���̏ꍇ FIRE_POWER_G (8) x (7+1) == 64/128 (�S���W�߂�50%) �ɂ����B
			*/
			#if (0)
			/*
				�p���[�A�b�v�V�X�e�����኱���������̂ŁA(30-1) -> (16) �֊�������������B
 WEAPON_L0(P000-P008)
 WEAPON_L1(P008-P015)
 WEAPON_L2(P016-P031)
 WEAPON_L3(P032-P063)
 WEAPON_L4(P064-P127)
 WEAPON_L5(P128)
					WEAPON_L1�ȉ��Ŏ��񂾏ꍇ�̂݋~�ύ􂪂���A
					�S������WEAPON_L3�ɂȂ�̂ŁA�Ó����Ǝv���B���A������������?
					WEAPON_L1[P15] -> ���� -> �S�����[P8]x6[��] -> [P15+P8x6]==WEAPON_L3[P63]
					WEAPON_L1[P15] -> ���� -> �P���[P8]x1[��] -> [P15+P8x1]==WEAPON_L2[P23]
					WEAPON_L0[P00] -> ���� -> �S�����[P8]x6[��] -> [P00+P8x6]==WEAPON_L3[P48]
					WEAPON_L0[P00] -> ���� -> �P���[P8]x1[��] -> [P00+P8x1]==WEAPON_L1[P08]
					WEAPON_L0[P00] -> ���� -> �R���[P8]x3[��] -> [P00+P8x3]==WEAPON_L2[P24]
					WEAPON_L2[P16] -> ���� -> �S�����[P1]x6[��] -> [P16+P1x6]==WEAPON_L2[P22]
					����Ŕ���(3��)���ƕK��WEAPON_L2�ȏ�ɂȂ�B
			*/
		//	item_create(s1, (((30-1) > pd_weapon_power)?(SP_ITEM_01_P008):(SP_ITEM_00_P001)), (7)/*5*/, ITEM_MOVE_FLAG_06_RAND_XY);
		//	item_create(s1, (((16) > pd_weapon_power)?(SP_ITEM_01_P008):(SP_ITEM_00_P001)), (6), ITEM_MOVE_FLAG_06_RAND_XY);/* �����牺��ł��A������������? */
		//	item_create(s1, SP_ITEM_01_P008, 1, ITEM_MOVE_FLAG_06_RAND_XY);
			#else
			/* �����C�����Ă݂� */
			item_create(s1, (((16) > pd_weapon_power)?(SP_ITEM_01_P008):(SP_ITEM_00_P001)), (3), ITEM_MOVE_FLAG_06_RAND_XY);/* ����Ȑl������΂��3���鎖���O�� */
			item_create(s1, SP_ITEM_00_P001, (5), ITEM_MOVE_FLAG_06_RAND_XY);/* P001���΂�܂�(���܂��A����Ȑl�́A����΂��Ă������ɂ͎��Ȃ��Ƃ��������O��) */
			#endif
		}
		//
		/* ���~���A ����\�́F������������ƕK���{�����o�� */
		if (REMILIA==select_player) 	/* ���~���A�̏ꍇ */
		{
			/* ������������� */
			if (0 != pd_bombs)
			{
				/* �g���ĂȂ����̎����Ă�{����f���o�� */
				item_create(s1, SP_ITEM_04_BOMB, pd_bombs, ITEM_MOVE_FLAG_06_RAND_XY);
			}
		}
	}
//		pd_explode		= 0;
//	if (0==pd_explode)
	{

	//	bakuhatsu_add_type(t->x256+t256(5),t->y256+t256(5),/*0,*/BAKUHATSU_MINI00);
	//	t->type = SP_DELETE;/* �����܂� */			/* ���������G���� */
	//	pd_explode=0;
	//	pd_bonus=0;
		//	voice_play(VOICE04_SHIP_HAKAI, TRACK03_SHORT_MUSIC/*TRACK01_EXPLODE*/);/* ���@���ɉ��́A�Ȃ�ׂ��d�˂Ȃ� */ 	// [***090127	�ꏊ���ړ�����B// �ύX��
		/* �����G�t�F�N�g */

		#if 0/* ���������G�͓|���Ȃ��G��������Ȃ��̂ŁA�����ŕK�������̂͂܂����C������ */
		/* ���������G���� */
		bakuhatsu_add_type(pd_enemy->x256+t256( 5), pd_enemy->y256+t256( 5), /*0,*/ BAKUHATSU_MINI00);
		pd_enemy->type	= SP_DELETE;/*???*/
		#endif
		/* �������� */
		#if 1
		{
			int jj;
			for (jj=0; jj<(4); jj++)
			{
				bakuhatsu_add_type_ddd(s1, BAKUHATSU_MINI00);
			}
		}
		#endif
		/* �v���C���[�ʒu�̏����� */
		#if 0/*SDL(�������W)*/
		s1->x256 = t256(GAME_WIDTH/2)-((s1->w128));
		s1->y256 = t256(220/*GAME_HEIGHT*/);/*240*/
		#else/*Gu(���S���W)*/
		s1->x256 = t256(GAME_WIDTH/2);
		s1->y256 = t256(220/*GAME_HEIGHT*/);/*240*/
		#endif
		/*	*/
		pd_bomber_time	= 0;			/* �����������������Ŗ�����(&�`���m)�{�����ĂȂ��Ȃ��Ă� */
		#if 1
	//	pd_bomber_time = 0;/*�s����*/
		set_bg_alpha(255);/* ��ʂ𖾂邭���� */
		#endif

	//	pd_state_flag		= STATE_FLAG_00_NONE;/*???*/
		pd_state_flag		&= (~(	STATE_FLAG_01_PLAYER_UP_AUTO_GET_ITEM | 	/* �I��� */
									STATE_FLAG_02_BOMB_AUTO_GET_ITEM |			/* �I��� */
									STATE_FLAG_03_SCORE_AUTO_GET_ITEM));		/* �I��� */

		/* ���݃I�v�V�������������āA�㔼�d���Ȃ�o�O������B */
		#if (0000)
		pd_state_flag		&= (~STATE_FLAG_08_OPTION_HIDE);	/* �I�v�V���������� */
		#endif/*0000*/
		pd_count_miss++;	/* �~�X�� */
		/* �c�`�����X���炷 */
		pd_zanki--;
		if (0 > pd_zanki)	/* �c�`�����X���Ȃ���� */
		{
			player_loop_quit();
		}
		else
		{
			//
		//	if (pd_player_speed > player_fix_status[BASE_SPEED_MINIMUM+select_player]/*pd_player_speed_minimum*/)			//player_speed_minimum�ȉ��ɂȂ�Ȃ��悤��
		//	{	pd_player_speed--;}
		//	pd_weapon_power -= (difficulty*4)+12;		/* ���탌�x���̒ቺ */
		//	pd_weapon_power -= ((difficulty+1)<<4); /* ���탌�x���̒ቺ(easy=16,32,48,64) */
			pd_weapon_power -= (16);	/* ���탌�x���̒ቺ(�g�A���ׂ����Փx�Ɋ֌W�Ȃ�P16��������) */
			if (0 > pd_weapon_power)	{	pd_weapon_power = 0;	}
			player_fukkatsu(s1);
		}
	}
}

/*---------------------------------------------------------
	�v���C���[�ړ��A���ꏈ��
	-------------------------------------------------------
	���ꒆ�A�������A�{�����AGAME_OUT���Ȃǂ̓��ꏈ��
---------------------------------------------------------*/
//void render_continue(void/*int now_max_continue*/);
/*static*/ int	now_max_continue;
static void player_move_other(SPRITE *s1)
{
	pd_save_timer--;
	if (2 > pd_save_timer)/* ���̏�ԂɈڍs */
	{

		switch (pd_player_status)
		{
		case PLAYER_STATE_01_HIT_BOMB:/* ��炢�{����t�� */			// [***090125		�ǉ�
			/* ���S�m�� */
				pd_state_flag |= STATE_FLAG_16_NOT_ALLOW_KEY_CONTROL;		/* �L�[���͖���(1) */
			player_explode(s1);/*�v���C���[����*/
			break;

		case PLAYER_STATE_02_SAVE_01:/*�v���C���[�������P(��ʊO���畜���n�_�֓_�ł��Ȃ���ړ���)*/
		//������Ƃ����҂�����
		//	if (s1->y256 < t256(220)/*240*/)	//������ƑO�ɏo�Ă���
			{
				pd_state_flag		&= (~STATE_FLAG_16_NOT_ALLOW_KEY_CONTROL);	/* �L�[���͗L��(0) */
				pd_save_timer		= (120);
				pd_player_status	= PLAYER_STATE_03_SAVE_02;
			}
		//	else
		//	{
		//		s1->y256 -= t256(1)/*fps_fa_ctor*/;
		//	}
			break;/*??? [***090210 �ǉ� */

		case PLAYER_STATE_03_SAVE_02:/*�v���C���[�������Q*/
			pd_player_status	= PLAYER_STATE_00_NORMAL;
			s1->color32 		= 0xffffffff;					/*	s1->alpha			= 0xff;*/
			pd_core->color32	= 0xffffffff;		/* ���A */	/*	pd_core->alpha	= 0xff;*/
			break;

		case PLAYER_STATE_04_GAME_OUT:/* GAME_OUT�� */
			now_max_continue--;
			if (
					(0 < now_max_continue)
				#if (0)/* (0==DEBUG) */
				/* �܂����� */
				&& ( (6) != player_now_stage)	/* �ŏI�ʂ̓R���e�B�j���[�o���Ȃ��B */
				#endif
				)
			{
				pd_use_continue++;	/* �R���e�B�j���[�� */
			//	render_continue(/*now_max_continue*/);
				psp_loop = (ST_INIT_MENU|ST_MENU_SUB_ASK_CONTINUE);
			}
			else
			{
				psp_loop = (ST_WORK_GAME_OVER|0);
			}
			break;
		}
	}
}


/*---------------------------------------------------------
	�v���C���[�ړ��A���C������
---------------------------------------------------------*/
#define USE_BOSS_NOT_BOMBER 0/* ���앗(�{�X�Ń{�������G)(�܂�����ĂȂ��̂�0 �ɏo���Ȃ�) */
//#define USE_BOSS_NOT_BOMBER 1/* �{�X�Ń{���Q�֎~(�{�X�Ń{��������) */
/*
	���� 0 �ɂ���ƁA�Q�[���I�Ɏx�Ⴊ����̂ŏo���Ȃ��B(�܂�����ĂȂ��̂�0 �ɏo���Ȃ�)
	�x��Ƃ����̂́A���݂̕����ł̓{������������ꍇ�ɁA�d�Ȃ�{���ňُ�ȍU���͂ɂȂ��Ă��܂��ׁB
//
	�������������ɂ́B
	�{�X�ɍU�������ꍇ�́A���ڃ��C�t�����������Ȃ��B
	�{�X�̍ő�U�������l���t���[���P�ʂŊǗ�����B
//
	�܂�����ĂȂ��B
*/

static void any_player_move(SPRITE *s1)
{
	pd_state_flag &= (~STATE_FLAG_15_KEY_SHOT); /* off */
	if (0==(pd_state_flag & STATE_FLAG_16_NOT_ALLOW_KEY_CONTROL))	/* �L�[���͗L��(0) */
	{
		player_keycontrol(s1);
	}
	#if (1==USE_BOSS_NOT_BOMBER)
	int player_hit_enemy_group;
	//player_hit_enemy_group = (SP_GROUP_TEKI);
	//player_hit_enemy_group = (SP_GROUP_BOSS);
	player_hit_enemy_group = (SP_GROUP_TEKI);
	#endif
	if (0 != pd_save_timer)/* ���ꏈ�� */
	{
//		bullets_to_hosi();/* �e�S���A���A�C�e���ɂ��� */
		player_move_other(s1);
	//
		#if (1==USE_BOSS_NOT_BOMBER)
		if (0==(pd_state_flag & (STATE_FLAG_05_IS_BOSS)))
		{	/* ���� */
			/* �{�����́A �U�R�G �ɂ������Ď��ȂȂ� */
			player_hit_enemy_group = (0);
		}
	//	else
	//	{	/* �{�X�퓬�� */
	//		/* �{�����́A�{�X �ɂ������Ď��� */
	//		player_hit_enemy_group = (SP_GROUP_TEKI/*SP_GROUP_BOSS*/);
	//	}
		#endif
	}
	else		/*�ʏ폈��*/
	{
//	case PLAYER_STATE_00_NORMAL:
		player_colision_check_graze(s1);
//		player_hit_enemy_group = (SP_GROUP_TEKI/*|SP_GROUP_BOSS*/);/* �G�����{�X */
	}
	/* �{�����́A�{�X / ���U�R�G �ɂ������Ď��� */
	/* �X�N���v�g���쒆 �́A�G�����蔻��͂Ȃ� */
	/* �L�[���͖�����(==������) �́A�G�����蔻��͂Ȃ� */
	if (0==(pd_state_flag & (STATE_FLAG_06_IS_SCRIPT|STATE_FLAG_16_NOT_ALLOW_KEY_CONTROL)))
	{
		/* ��炢�{����t�� �́A�G�����蔻��͂Ȃ� */
		if (PLAYER_STATE_01_HIT_BOMB != pd_player_status)
		{
			#if (1==USE_BOSS_NOT_BOMBER)
			if (0 != player_hit_enemy_group)/*(???)*/			/* �{�����́A �U�R�G �ɂ������Ď��ȂȂ� */
			#else /*(0==USE_BOSS_NOT_BOMBER)*/
			if (0 != pd_save_timer){;}else/*(???)*/ 			/* �{�����́A �{�X/�U�R�G �ɂ������Ď��ȂȂ� */
			#endif
			{
				player_colision_check_enemy(s1);/*SP_GROUP_ENEMYS*/  // 	/, /*(SP_GROUP_TEKI)*/player_hit_enemy_group
			}
		}
	}
	/* �A�C�e���͏�Ɏ��� */
	player_colision_check_item(s1/*,SP_GROUP_ITEMS*/);
}


/*---------------------------------------------------------
	�v���C���[������()
---------------------------------------------------------*/
#if (1==USE_EXTEND_CHECK)
extern void player_init_extend_score(void);
#endif
//extern int zanki;
void player_continue_value(void)
{
	pd_zanki				= (1+option_config[OPTION_CONFIG_00_PLAYER]);	/*pd_base_zanki*/	/*((zanki&0x03)+2)*/ /*player_fix_status[BASE_LIVES+select_player]*/
	pd_my_score 		= score(0);
	pd_state_flag			&= (~STATE_FLAG_14_GAME_LOOP_QUIT); 	/* ���A */
	#if (1==USE_EXTEND_CHECK)
	player_init_extend_score();
	#endif
	player_fukkatsu(player);/* �I�v�V������ǉ����� */
}

/*---------------------------------------------------------
	�v���C���[�A�I�v�V�����̒ǉ�
	�얲	������	�H�X�q
---------------------------------------------------------*/

static void option_create_re_ma_yu(SPRITE *src)
{
	PL_OPTION_DATA *data;		/* �얲�̂𗬗p���Ă܂��B */
//
	int jj;
	for (jj=0; jj<4; jj++)
	{
		SPRITE *s;
		s						= sprite_add_gu( TAMA_TYPE_BULLET_DUMMY );	/*+ REIMU MARISA YUYUKO */
		option[(OPTION_C1+jj)]	= s;
	//	s->flags				|= (SP_FLAG_VISIBLE);
		s->flags				&= (~(SP_FLAG_VISIBLE));			/* ���t���O��OFF(�s��) */
		s->flags				&= (~(SP_FLAG_COLISION_CHECK)); 	/* �����蔻���OFF(���G) */

		s->type 				= (JIKI_OPTION_00_00/*|S P_MUTEKI*/);
	//	s->anim_frame			= 0;
	//
		s->callback_mover		= re_ma_yu_move_option;
		data					= mmalloc(sizeof(PL_OPTION_DATA));
		s->data 				= data;
/*???*/ 		/*data->*/s->base_weapon_strength	= (1/*8*5*/);/* [***090214 �ǉ� */
	//	data->length			= 11;
		#if 0/*SDL(�������W)*/
		s->x256 				= src->x256+((src->w128-s->w128))-t256(11)/*data->length*/;
		s->y256 				= src->y256+((src->h128-s->h128));
		#else/*Gu(���S���W)*/
	//	s->x256 				= src->x256-t256(11)/*data->length*/;
	//	s->y256 				= src->y256;
		s->x256 				= src->x256/*(???)*/;
		s->y256 				= src->y256/*(???)*/;
		#endif
		data->anime_wait		= 0;
		static const int iii_tbl[4] =
		{
		/*data->opt_anime_add_id*/ /*pos*/		/*=*/ OPTION_ID_02_LM,	/*-1*/ /*;*/ /*0*/
		/*data->opt_anime_add_id*/ /*pos*/		/*=*/ OPTION_ID_03_RM,	/* 1*/ /*;*/
		/*data->opt_anime_add_id*/ /*pos*/		/*=*/ OPTION_ID_01_LL,	/*-1*/ /*;*/ /*0*/
		/*data->opt_anime_add_id*/ /*pos*/		/*=*/ OPTION_ID_04_RR	/* 1*/ /*;*/
		};
		data->opt_anime_add_id/*pos*/			= iii_tbl[jj]/*-1*/;/*0*/
		static const /*int*/s16 ccc_tbl[4] =
		{
		/*data->angleCCW512=*/	deg_360_to_512CCW((270)),	/*deg_360_to_512(180)*/ /*;*/ /*-ra d2deg512(M_PI)*/
		/*data->angleCCW512=*/	deg_360_to_512CCW(( 90)),	/*deg_360_to_512(  0)*/ /*;*/
		/*data->angleCCW512=*/	deg_360_to_512CCW((270)),	/*deg_360_to_512(180)*/ /*;*/ /*-ra d2deg512(M_PI)*/
		/*data->angleCCW512=*/	deg_360_to_512CCW(( 90))	/*deg_360_to_512(  0)*/ /*;*/
		};
		data->angleCCW512						= ccc_tbl[jj];
	//	data->state 							= 0;
		data->opt_shot_interval 				= 0;
	}
}

static void option_create_oz_ci(SPRITE *src)	/* ���~���A �� �`���m [***090220 �ǉ� */
{
	/*REMILIA_OPTION_DATA*/PL_OPTION_DATA *data;
	int jj;
	int kk;
	kk=0;
	for (jj=0; jj<4; jj++)
	{
		SPRITE *s;
		s						= sprite_add_gu( TAMA_TYPE_BULLET_DUMMY );
		option[(OPTION_C1+jj)]	= s;
	//	s->flags				|= (SP_FLAG_VISIBLE);
		s->flags				&= (~(SP_FLAG_VISIBLE));			/* ���t���O��OFF(�s��) */
		s->flags				&= (~(SP_FLAG_COLISION_CHECK)); 	/* �����蔻���OFF(���G) */

		s->type 				= (JIKI_OPTION_00_00/*|S P_MUTEKI*/)+kk; kk+=8/*PLAYERS8*/;
	//	s->anim_frame			= 0;
	//
		s->callback_mover		= oz_ci_move_option;
		/* ���~���A�̃I�v�V�����͔��������ۂ� */
		if (REMILIA==select_player)
		{
		//	s->color32			= 0x96ffffff;	/*	s->alpha			= 0x96; */	/* ����߂��� */
			s->color32			= 0x80ffffff;	/*	s->alpha			= 0x80;*/	/* ������ */
		//	s->color32			= 0x50ffffff;	/*	s->alpha			= 0x50;*/	/* �Â�߂��� */
		}
		data					= mmalloc(sizeof(/*REMILIA_OPTION_DATA*/PL_OPTION_DATA));
		s->data 				= data;
		static const /*int*/s16 aaa_tbl[4] =
		{
		/*data->REMILIA_angle512=*/ deg_360_to_512CCW(360-(45*5)),	/*deg_360_to_512(45*1)*/ /*M_PI*2*1/8*/ /*;*/ /*�`���m*/
		/*data->REMILIA_angle512=*/ deg_360_to_512CCW(360-(45*7)),	/*deg_360_to_512(45*5)*/ /*M_PI*2*5/8*/ /*;*/ /*�`���m*/
		/*data->REMILIA_angle512=*/ deg_360_to_512CCW(360-(45*1)),	/*deg_360_to_512(45*3)*/ /*M_PI*2*3/8*/ /*;*/ /*�`���m*/
		/*data->REMILIA_angle512=*/ deg_360_to_512CCW(360-(45*3))	/*deg_360_to_512(45*7)*/ /*M_PI*2*7/8*/ /*;*/ /*�`���m*/
		};
		data->angleCCW512/*REMILIA_angle512*/	= aaa_tbl[jj];
	//	data->next					= player;
	//	data->st ate1				= 0;///
		data->state2				= 0;
		data->offset_x256			= t256(0);
		data->offset_y256			= t256(0);
/*???*/ 		/*data->*/s->base_weapon_strength	= (1/*8*5*/);
		data->anime_wait			= 0;
		data->opt_shot_interval 	= 0;
		data->opt_anime_add_id		= (OPTION_C1+jj);
	//	data->ccc_angle512			= deg_360_to_512((0));
		data->slow_count			= 0;
		#if 0/*SDL(�������W)*/
		s->x256 					= src->x256+((src->w128-s->w128));
		s->y256 					= src->y256+((src->h128-s->h128))+t256(20);
		#else/*Gu(���S���W)*/
	//	s->x256 					= src->x256;
	//	s->y256 					= src->y256+t256(20);
		s->x256 					= src->x256/*+t256((31-5)/2)*/; 	/* �_�~�[�I�t�Z�b�g */
		s->y256 					= src->y256/*+t256((31-5)/2)*/; 	/* �_�~�[�I�t�Z�b�g */
		#endif
	}
}

/*---------------------------------------------------------
	�v���C���[�A��������p�R�A�̒ǉ�
	���̒ǉ�
---------------------------------------------------------*/

static SPRITE *player_add_maru_core(SPRITE *s1)
{
	SPRITE *s2;
	s2						= sprite_add_gu(BASE_CORE_ATARI_0a+select_player);
	s2->type				= (JIKI_CORE_00_REIMU_A+select_player);
	#if 1/*Gu(���S���W)*/
	s2->x256				= (s1->x256)+t256((31-5)/2);	/* �R�A�I�t�Z�b�g */
	s2->y256				= (s1->y256)+t256((31-5)/2);	/* �R�A�I�t�Z�b�g */
	#endif

//	s2->callback_mover		= NULL/*player_move_core*/;
	#if (00)
//	s2->flags				|= ( (SP_FLAG_VISIBLE));/* ���\�� */
	s2->flags				&= (~(SP_FLAG_VISIBLE));/* ����\�� */
	#endif
	return (s2);
}

/*---------------------------------------------------------
	�v���C���[������
---------------------------------------------------------*/
extern int boss_hp_dec_by_frame;/*�{�X�U�������l�A�t���[���P��*/
extern int practice_mode;
extern void select_jiki_load_surface(void);
void player_init(void)
{
	select_jiki_load_surface();
	player					= sprite_add_gu( JIKI_ATARI_ITEM_16 );		/* [***090220 �ǉ� */
	player->type			= (SP_GROUP_JIKI_GET_ITEM);
	player->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
	player->callback_mover	= any_player_move;
	#if 1/*Gu(���S���W)*/
	player->x256			= (t256(GAME_WIDTH/2));
	player->y256			= (t256(GAME_HEIGHT));
	#endif
//
	pd_use_continue 	= 0;	/* �R���e�B�j���[�� */
	pd_count_miss		= 0;	/* �~�X�� */
	pd_used_bomber		= 0;	/* �{���g�p�� */
	pd_use_kurai_bomb	= 0; 	/* ��炢�{���g�p�� */
//	pd_count_bonus		= 0;	/* �X�y���J�[�h�{�[�i�X�� */
//
//
	pd_bomber_time		= 0;	/*==bomb_wait*/
	pd_graze_point		= 0;
//

	pd_state_flag			= STATE_FLAG_00_NONE;
//	if (MARISA==select_player)		{	pd_state_flag		|= STATE_FLAG_04_IS_MARISA; 	}	/* �������͏�Ɏ������W */
//
	boss_hp_dec_by_frame = 0;/*�{�X�U�������l�A�t���[���P��*/
//
	#if (0==USE_BOSS_COMMON_MALLOC)
	pd_boss 			= NULL;
	#else
//----[BOSS]
	{SPRITE *sakuya;
		sakuya								= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
	//	pd_boss = sakuya;/*�P��{�l*/
		pd_boss 						= sakuya;/*�P��{�l*/
	}
	#endif
	pd_core 			= player_add_maru_core(player); 	/* ���̒ǉ� */

	weapon_level_offset 	= select_player + (/*0==���탌�x��*/0<<3);
	/* ���K���[�h�̏ꍇ�̓t���p���[�Ŏn�߂�(���̑���N���A��̃C�x���g������Ȃ�) */
	pd_weapon_power 	= (0==practice_mode)?(0):(MAX_POWER_IS_128);
//
	/* �I�v�V������ǉ� */
	switch (select_player)
	{
	case CIRNO_A:/*�Ƃ肠����*/
	case CIRNO_Q:/*�Ƃ肠����*/
	case REMILIA:		option_create_oz_ci(player);		break;/* [***090220 �ǉ� */
	default:
	#if 0
	case YUYUKO:/*�Ƃ肠����*/
	case REIMU_A:/**/
	case REIMU_B:/**/
	case MARISA_A:
	case MARISA_B:
	#endif
						option_create_re_ma_yu(player); 	break;
	}
//
	now_max_continue		= DEFAULT_MAX_CONTINUE;/*(3)*/
//
	#if 1
	player_continue_value();
	#else
//	pd_zanki				= /*pd_base_zanki*/ player_fix_status[BASE_LIVES+select_player];
//	pd_my_score 		= score(0);
//	player_fukkatsu(player);/* �I�v�V������ǉ����� */
	#endif



}

/*---------------------------------------------------------
	�G����
	�{�X�|�����ꍇ�̏���
	(�v���C���[���܂�)
	-------------------------------------------------------
???��R�����g???	�{�X��|�����炷���Ă΂��(�v���C���[�𖳓G�ɂ����)
???��R�����g???	(�t���O�͎d�l�㎞�ԑ҂�������ׁA���݂��̗p�r�ɂ͎g���Ȃ�)
---------------------------------------------------------*/
//extern void player_loop_quit(void);
extern int	now_max_continue;

/*static*/ void lose_boss(SPRITE *src)
{
	bakuhatsu_add_circle(src, 1);	/* pl ayer_set_destoroy_boss(); B02_BOSS_DESTROY ���K�v */
//
	{


	//	pd_bo ssmode=B02_BOSS_DESTROY;
//		pd_state_flag |= ST ATE_FLAG_11_IS_BOSS_DESTROY;


		#if 1
		/* �o�O�ŃA�C�e���o�Ȃ�(???) */
	//	if (pd_state_flag&STATE_FLAG_09_IS_WIN_BOSS)		/* �U���œ|�����ꍇ�̂� */
		#endif
		{
		//	pd_state_flag		&= (~STATE_FLAG_09_IS_WIN_BOSS);	/* �I��� */
			/* ���ł��̏ꍇ�A������炢�����A�L�[���͗L��(0) */
			pd_state_flag		&= (~(STATE_FLAG_09_IS_WIN_BOSS|STATE_FLAG_16_NOT_ALLOW_KEY_CONTROL));	/* �I��� */

			if (0 >= spell_card_boss_timer)
			{
				;/* ���Ԑ؂�̏ꍇ�̓{�[�i�X�A�C�e���Ɠ��_�Ȃ��B */
			}
			else
			{
				/* �{�[�i�X�A�C�e�����o�� */
// ��� 		item_create(src, S P_ITEM_05_HOSI/*SP_ITEM_05_TENSU*/, 15, IT EM_MOVE_FLAG_01_COLLECT);/*���_���o��*/
// �P�� 		item_create_for_boss(src, ITEM_CREATE_MODE_01);// �P�� �� �p�`�F �A���X
				item_create_for_boss(src, ITEM_CREATE_MODE_01);
			//
				#if 1
				/* �{�X�̓��_���Z */
				player_add_score(src->base_score);/* �P��̃X�R�A */
				#endif
			}
		}
	}
	//		data->boss_base.boss_health = 0;/* �Ȃ񂩃o�O��̂Œǉ� */
	/* �R�[���o�b�N�o�^ */
	src->callback_hit_enemy 	= NULL; 	/* �_�~�[�R�[���o�b�N�o�^ */
	#if 0
//	src->callback_mover 		= NULL; 		���ʃ{�X�ޔ�(���j��ɉ�ʊO�Ƀ{�X��������)
//	src->type					= SP_DELETE;	���ʃ{�X�ޔ�(���j��ɉ�ʊO�Ƀ{�X��������)
	#else
	src->callback_mover 		= common_99_keitai;/* ���ʃ{�X�ޔ�(���j��ɉ�ʊO�Ƀ{�X��������) */
	#endif
//
	/* �P��/�A���X��|���ƊF�j�󂳂��B */
	#if 1
	common_boss_flags		= (0)/*FLG_ALL_CAST*/;
	#endif
//
	#if (0)
	pd_save_timer		= /*pd_bomber_time*/255 + USER_BOMOUT_WAIT/*(30)*/;/*�������G����*/
	#else
//	pd_save_timer		= (6);/* 6[�t���[��] �{�X�|���Ă��玟(�V�i���I)�ɐi�ނ܂ł̑҂����� */
//	pd_save_timer		= (60);/* 60[�t���[��] �{�X�|���Ă���A�{�X����ʊO�ɑޔ�����܂ł̑҂����� */
	pd_save_timer		= (9999);/* �ޔ���ɑޔ���Ŏw��B	60[�t���[��] �{�X�|���Ă���A�{�X����ʊO�ɑޔ�����܂ł̑҂����� */
	#endif
//	pd_save_timer		= 40/*120*/ /*150-120*/;									// ���G����
	pd_player_status	= PLAYER_STATE_02_SAVE_01;	// ���G��ԁH
//	pd_player_status	= PLAYER_STATE_03_SAVE_02;	/* �H�ɁA���܂������Ȃ� */
//
	pd_bomber_time		= 0;
	#if 1
//	pd_bomber_time = 0;/*�s����*/
	set_bg_alpha(255);/* ��ʂ𖾂邭���� */
	#endif

	#if 1
	draw_boss_hp_value	= 0;/* �悭�킩��Ȃ� */
	#endif
//	s1->color32 		= 0x50ffffff;	//	s1->alpha			= 0x50; 	/* ������ */
//	pd_core->color32	= 0x50ffffff;	//	pd_core->alpha	= 0x50; 	/* ������ */
	boss_effect_term();

	/*---------------------------------------------------------
		�{�X���ꏈ��
	---------------------------------------------------------*/
	//	if (B02_BOSS_DESTROY==pd_bo ssmode) //�{�X��|�����Ƃ��̏���
	//	if (/*ST ATE_FLAG_11_IS_BOSS_DESTROY==*/ (pd_state_flag & ST ATE_FLAG_11_IS_BOSS_DESTROY))
	//void boss_destroy_aaa(void)	/* �{�X��|��������A�u�{�X��C�x���g�v�O�̏��� */
	{
		bullets_to_hosi();/* �e�S���A���A�C�e���ɂ��� */
		// TIME_20_DBWAIT�t���[���҂��Ă�����s�B�{�X��|�������ɉ�ʂɕ\������Ă���e��S�ď��������̂��߂ɕK�v�B
		play_music_num(BGM_00_stop);
//
		voice_play(VOICE03_BOSS_HAKAI, TRACK03_SHORT_MUSIC/*TRACK02_ALEART_IVENT*/);		// [***090313		�ǉ�
//		voice_play(VOICE03_BOSS_HAKAI, TRACK01_EXPLODE);/*�\��(���邳��)*/
//
		{


			pd_state_flag &= (~(STATE_FLAG_13_DRAW_BOSS_GAUGE));/*off*/
//			pd_state_flag		&= (~(STATE_FLAG_03_SCORE_AUTO_GET_ITEM));		/* �I��� */
			if (0==practice_mode)/* ���K���[�h�ł̓{�X��C�x���g�͌���Ȃ���B */
			{
			//	pd_bo ssmode	= B07_AFTER_LOAD;
				pd_state_flag |= STATE_FLAG_10_IS_LOAD_SCRIPT;
			}
			else/* ���K���[�h�̏ꍇ�A�I������ */
			{
				#if 1/* ���̂Q�̃Z�b�g�Ŏ����I�ɏI��(GAME OVER)���� */
				now_max_continue = 1;	/* �R���e�B�j���[�����Ȃ� */
				player_loop_quit();
				#endif
			}
		}
	}
//
}
