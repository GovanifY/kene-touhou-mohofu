
/*---------------------------------------------------------
	�e�v���C���[(�얲 �� ������ �� ���~���A �� �`���m �� �H�X�q)
	REIMU MARISA REMILIA CIRNO YUYUKO
	-------------------------------------------------------
---------------------------------------------------------*/

#include "game_main.h"
#include "player.h"

extern int select_player;

SPRITE *player;
SPRITE *dummy_obj;

int player_now_stage;		//
int pd_bomber_time; 		// Use Gu

enum /*_player_state_*/
{
	PLAYER_STATE_00_NORMAL,
	PLAYER_STATE_01_HIT_BOMB,		// [***090125		�ǉ�:PLAYER_STATE_01_HIT_BOMB
	PLAYER_STATE_03_SAVE_01,
	PLAYER_STATE_04_SAVE_02,
	PLAYER_STATE_05_GAME_OUT,
};

typedef struct
{
//	int strength;/*union WEAPON_BASE*/
	int angleCCW512;
	int speed256;
//
	int nr;
	int state;
	int time_out;	/* �ݒ莞�ԓ��Ɍ������Ȃ��ꍇ�͂����܂� */
	SPRITE *target_obj/*int target_id*/;
} PL_HOMING_DATA;/*==PL_HLASER_DATA*/

typedef struct
{
//	int strength;/*union WEAPON_BASE*/
	int check_x256;
	int check_y256;

	int time_out;	/* �ݒ莞�ԓ��Ɍ������Ȃ��ꍇ�͂����܂� */
	SPRITE *target_obj/*int target_id*/;
} PL_HOMING_KODOMO_DATA;/*==PL_HLASER_DATA*/

typedef struct
{
//	int strength;/*union WEAPON_BASE*/	/* [***090214 �ǉ� */
	int angleCCW512;
	int add_r512;
	int radius; 	/*���a(radius)*/
} PL_KEKKAI_DATA;

typedef struct
{
//	int strength;/*union WEAPON_BASE*/	/* [***090214 �ǉ� */
	int angleCCW512;				/* �����O�Ƃ��č����(�t���)�̊p�x */
	int anime_wait; 				/* �A�j���[�V�������x�̃J�E���^ */
	int opt_anime_add_id/*pos*/;	/* �A�j���[�V���������̉��Z�l / �I�v�V����ID / (���~���A & �`���m)x offset */
	int opt_shot_interval;			/* �I�v�V���������˂���V���b�g�̍X�V�Ԋu */
//
	#if 1
	/* ���~���A & �`���m */
//	int state1;//���ʂȂ̂ŁA�����ɂ͗v��Ȃ�
	int state2;
	int offset_x256;				/* �~�̔��a�ƌ��p */
	int offset_y256;
	int slow_count; 				/* FORMATION_03: �������p(�D���Ȉʒu�ɒu�����) */
	#endif
} PL_OPTION_DATA;

#define REMILIA_angle512		angleCCW512
#define REMILIA_OPTION_DATA 	PL_OPTION_DATA



//efine OPTION_ID_01_LL 	(0x01ff)/*��-1 0  0001 (0x01ff) 0001 1111 1111 */
//efine OPTION_ID_02_LM 	(0x04ff)/*��-1 0  0100 (0x02ff) 0010 1111 1111 */
//efine OPTION_ID_03_RM 	(0x0801)/*��+1 1  1000 (0x0401) 0100 0000 0001 */
//efine OPTION_ID_04_RR 	(0x0d01)/*��+1 1  1101 (0x0701) 0111 0000 0001 */

//efine OPTION_ID_01_LL 	(0x05ff)/*��-1 0  4 0101 (0x01ff) 0001 1111 1111 */
//efine OPTION_ID_02_LM 	(0x04ff)/*��-1 0  4 0100 (0x02ff) 0010 1111 1111 */
//efine OPTION_ID_03_RM 	(0x0c01)/*��+1 1  c 1100 (0x0401) 0100 0000 0001 */
//efine OPTION_ID_04_RR 	(0x0d01)/*��+1 1  c 1101 (0x0701) 0111 0000 0001 */

//#define OPTION_ID_01_LL	(0x0dff)/*��-1 0  c 1101 (0x01ff) 0001 1111 1111 */
//#define OPTION_ID_02_LM	(0x0cff)/*��-1 0  c 1100 (0x02ff) 0010 1111 1111 */
//#define OPTION_ID_03_RM	(0x0401)/*��+1 1  4 0100 (0x0401) 0100 0000 0001 */
//#define OPTION_ID_04_RR	(0x0501)/*��+1 1  4 0101 (0x0701) 0111 0000 0001 */

//#define OPTION_ID_01_LL	(0x1dff)/*��-1 0  c 1 1101 (0x01ff) 0001 1111 1111 */
//#define OPTION_ID_02_LM	(0x1cff)/*��-1 0  c 1 1100 (0x02ff) 0010 1111 1111 */
//#define OPTION_ID_03_RM	(0x1401)/*��+1 1  4 1 0100 (0x0401) 0100 0000 0001 */
//#define OPTION_ID_04_RR	(0x1501)/*��+1 1  4 1 0101 (0x0701) 0111 0000 0001 */

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

#define PLAYERS5				(5)

#define BASE_SPEED_ANIME		(PLAYERS5*0)
#define BASE_HIT_BOMB_WAIT		(PLAYERS5*1)
#define BASE_OPT_SHOT_INTERVAL	(PLAYERS5*2)
#define BASE_OPT_SHOT_ANIME 	(PLAYERS5*3)
#define BASE_STD_BOMB_STRENGTH	(PLAYERS5*4)
#define BASE_LOW_BOMB_STRENGTH	(PLAYERS5*5)
//
#define BASE_BOMBS				(PLAYERS5*6)
#define BASE_LIVES				(PLAYERS5*7)
#define BASE_MAX				(PLAYERS5*8)		/* �ő吔 */
static Uint8 player_fix_status[BASE_MAX] =
{/* REIMU MARISA REMILIA CIRNO YUYUKO */
	  8,   2,	3,	 1,   4,	/* �v���C���[�̃A�j���[�V�������x */
	 16,   8,	4,	 9,  12,	/* ��炢�{���̎�t���� / hit_bomb_wait. */
	  4,   8,	9,	 9,  16,	/* �I�v�V�����V���b�g�̍X�V�Ԋu / option shot interval. */
	 12,   6,	3,	 3,  12,	/* �I�v�V�����V���b�g�̃A�j���[�V�������x */
	  5,   5,	3,	 1,   6,	/* �ʏ�{���̋��� / standard bomb strength. */
	 30,  48,  12,	12,  24,	/* �ᑬ�{���̋��� / lower bomb strength. */
//
	 3,  3,  3,  3,  4, 	/* �������̃{���� [�����{����](option���[�������疳���Ȃ邩��) */
	 4,  4,  3,  9,  5, 	/* �J�n���̎c��`�����X [�����v���C���[��](option���[�������疳���Ȃ邩��) */
};
/*
���~���A�{��: �ʏ�{���̋���:[��==8] [�V==4]
�� 3��==((256/64)-1) [24==(8)*3]
�V 7��==((256/32)-1) [21==(3)*7]
REIMU		5,[�V==5] [��==3]
MARISA		5,
REMILIA 	3,[�V==3] [��==8]
CIRNO		1,[�V==1] [��==4]
YUYUKO		6,[�V==6] [��==4]
	�ʏ�{���̋��� / standard bomb strength.
*/


/*---------------------------------------------------------
	�v���C���[�A��������p�R�A�A��������
---------------------------------------------------------*/

static int is_graze;	// player1���e���ɐG�ꂽ���H(�O���C�Y�����ۂ�)
static int pd_player_status;
static int pd_save_timer;

static void player_move_core(SPRITE *s2)
{
	s2->x256 = player->x256+((player->w128-s2->w128))	-(t256(1));
	s2->y256 = player->y256-((s2->h128+s2->h128))		+(t256(20));
	// �v���C���[�X�v���C�g���e�ɐG��Ă��邩
	if (is_graze)	// �O���C�Y���H
	{
		#if 0
		/* �������̓A�C�e���ȊO�����蔻�肪�����̂ł����ɗ��Ȃ���(������o�O) */
		if (PLAYER_STATE_00_NORMAL == pd_player_status)
		#endif
		{
			SPRITE *t;
			// (�O���C�Y���Ȃ�R�A�ƓG�e�̂����蔻�������)
			if (NULL != (t=sprite_collision_check(s2,SP_GROUP_BULLETS)))
			{
				#if (0==USE_DESIGN_TRACK)
				play_voice_auto_track(VOICE04_SHIP_HAKAI);		// [***090127	�ύX��
				#else
				voice_play(VOICE04_SHIP_HAKAI, TRACK03_SHORT_MUSIC/*TRACK01_EXPLODE*/);/* ���@���ɉ��́A�Ȃ�ׂ��d�˂Ȃ� */
				#endif
				pd_player_status	= PLAYER_STATE_01_HIT_BOMB;
				pd_save_timer		= player_fix_status[BASE_HIT_BOMB_WAIT+select_player]/*0*/;
			//	((PLAYER_DATA *)player->data)->enemy = t;/* ���������G���o���Ƃ� */
				/* ���������G�́A�K�����ʖ�ł͖����̂ŁA���������G�̗̑͂����炷�ׂ� */
				/**/
			}
		}
	}
	is_graze=0;/* �O���C�Y������� */
}

/*---------------------------------------------------------
	�v���C���[����̂����蔻��
---------------------------------------------------------*/

//extern void enemy_zako_yokai1_nonshield_hitbyweapon(SPRITE *s, SPRITE *t/*, int angle*/);

	/* wenn flag erase_bullets gesetzt ist, werden auch gegnerische schuesse getroffen */
	/* wenn flag erase_player_tama gesetzt ist, wird der Schuss vernichtet */
//PLAYER_WEAPON_TYPE_00_SHOT /*int angle,*/ int 1==erase_player_tama, int 0==erase_bullets
//PLAYER_WEAPON_TYPE_01_BOMB /*int angle,*/ int 0==erase_player_tama, int 1==erase_bullets
enum /*_player_state_*/
{
	PLAYER_WEAPON_TYPE_00_SHOT = 0, 	/* �V���b�g�n */	/* ���e���G�ɂ��������ꍇ�Ɏ��e�����ł��� */	/* �G�e�ɂ�����ƓG�e�������Ȃ� */
	PLAYER_WEAPON_TYPE_01_BOMB, 		/* �{���n */		/* ���e���G�ɂ��������ꍇ�Ɏ��e�����ł��Ȃ� */	/* �G�e�ɂ�����ƓG�e������ */
};
//	SDL_Surface *spimg;
//	PLAYER_DATA *pd = (PLAYER_DATA *)player->data;

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
		tekidan_obj = sprite_collision_check(shot, SP_GROUP_BULLETS);
		if (NULL != tekidan_obj)		/* �G�e�ɓ��������� */
		{
			tekidan_obj->type = SP_DELETE;	/* �G�e������ */
			#if (0==USE_DESIGN_TRACK)
			play_voice_auto_track(VOICE02_ZAKO_HAKAI);
			#else
			voice_play(VOICE02_ZAKO_HAKAI, TRACK01_EXPLODE);
			#endif
			//spimg=sprite_getcurrimg(c);
			//parsys_add(spimg, 2,2, c->x,c->y, 10, angle, 10, 30, EXPLODE|DIFFSIZE, NULL);
			//SDL_FreeSurface(spimg);
		}
	}
	/* �G�ɂ��������ꍇ�ɓG������ */
	{
		/* ���e�ɂ��������͓̂G���̂Ȃ̂����ׂ�D */
		teki_obj = sprite_collision_check(shot, (SP_GROUP_ZAKO|SP_GROUP_BOSS)/*SP_GROUP_ENEMYS*/);
		if (NULL != teki_obj)			/* �G���̂ɓ��������� */
		{
			if (NULL != (teki_obj->callback_hit_enemy)) 	/*	*/
			{
				(teki_obj->callback_hit_enemy)(teki_obj/*�G����*/, shot/*���e*/);/*,angle*/
			}
			/* ���e���G�ɂ��������ꍇ�Ɏ��e�����ł��� */
			if (/*erase_player_tama*/PLAYER_WEAPON_TYPE_00_SHOT == erase_shot_type) 	/* �V���b�g�n�̂� */
			{
				shot->type = SP_DELETE; 	/* ���e������ */
			}
		}
	}
}
#if (0)
		switch (teki_obj->type)
		{
		//�{�X�ɓ��������̂��V�[���h�������ꍇ�̓_���[�W�������s��Ȃ�
		case SP_CHUU:	/*not_break;*/		// �{�X�Ƌ��p
		case SP_BOSS:
			#if (0)/*[debug]*/
			((PLAYER_DATA *)player->data)->my_score = shot->type;
			#endif /*(0000)*/
		//	if ((SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) != s->type)
			if (0==(SP_GROUP_SHOT_SPECIAL & shot->type))
			{
				;
			}
			else
			{
				(teki_obj->callback_hit_enemy)(teki_obj/*�G����*/, shot/*���e*/);/*,angle*/
			}
			break;
		default:
			/* ���������͖̂{���ɃU�R�ށH */
			if (SP_ZAKO==(teki_obj->type&(SP_GROUP_ZAKO|SP_GROUP_BOSS)/*SP_GROUP_ENEMYS*/))
			{
				callback_hit_zako(teki_obj/*�G����*/, shot/*���e*/);/*�{���ɃU�R��*/
			}
			break;
		}
#endif /*(0000)*/

/*---------------------------------------------------------
	�v���C���[�A�j�e�̈ړ�
	�얲 �� ������ �� ���~���A �� �`���m �� �H�X�q
---------------------------------------------------------*/

static void player_move_needle(SPRITE *src)
{
	src->x256 += src->vx256;			//co_s512((data->angle512))*data->speed/**fps_fa_ctor*/;
	src->y256 += src->vy256;			//si_n512((data->angle512))*data->speed/**fps_fa_ctor*/;
	if (REMILIA==select_player) 	/* ���~���A�͂���� */
	{	int rand_int;
		rand_int = ra_nd();
		src->x256 -= (rand_int&0x0100);
		src->x256 += (rand_int&0x0080);
		src->x256 += (rand_int&0x0080);
	}
	else
	if (YUYUKO==select_player)		/* �H�X�q�͂���� */
	{
		PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
		src->vx256 += ((src->vx256*pd->weapon_power)>>11);
	}
	else
	if (REIMU==select_player)		/* �얲�̉�]�V���b�g */
	{
		if (((JIKI_SHOT_01)|SP_GROUP_SHOT_SPECIAL)==src->type)
		{
			/* �`��p�O����] */
			src->m_angleCCW512 += 16/*deg_360_to_512CCW((10))*/;
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
	�v���C���[�A�V���b�g�e�̈ړ�
	�얲 �� ������ �� ���~���A �� �`���m �� �H�X�q
---------------------------------------------------------*/
#define player_move_shot player_move_needle

/*---------------------------------------------------------
	�G��T���q�֐�
---------------------------------------------------------*/
extern SPRITE *sprite_list000_head;
static /*int*/SPRITE *search_enemy_by_sprite(void)
{
	/* Suche Enemy-Sprite, das noch nicht verfolgt wird, liefert SPRITE-ID oder -1 */
	SPRITE *s = sprite_list000_head;/* �X�v���C�g ���X�g�̐擪 ����T�� */
	while (NULL != s)/* �X�v���C�g ���X�g�̍Ō�܂Œ��ׂ� */
	{
		if (
			#if 1
			(SP_DELETE != s->type ) && /* �폜�ς݂͔�΂� */
			#endif
			(0 != (s->type & (SP_GROUP_ZAKO|SP_GROUP_BOSS))/*SP_GROUP_ENEMYS*/) 	/* �v���C���[�ɂƂ��Ă̓G(�U�R��{�X) */
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
				if ((s->x256 > 0) && (s->x256 < t256(GAME_WIDTH )-((s->w128+s->w128))) &&
					(s->y256 > 0) && (s->y256 < t256(GAME_HEIGHT)-((s->h128+s->h128)))) 	/* �\����ʓ��̓G */
				{
					return (s)/*->id*/;/*��������*/
				}
			}
		}
		s = s->next;/*��*/
	}
	return (NULL)/*-1*/;/*������Ȃ�����*/
}

/*---------------------------------------------------------
	�v���C���[�A�U���e�̈ړ�
---------------------------------------------------------*/

enum
{
	SH_00 = 0,
//	SH_01,
	SH_02,
	SH_03,
};


static void player_move_parrent_hlaser(SPRITE *src)
{
	PL_HOMING_DATA *data;
	data=(PL_HOMING_DATA *)src->data;
//
	SPRITE *target;
	target = NULL;	/* Target */
//	SPRITE *tg;
//	tg=(SPRITE *)data->tg_sprite; /* Target-Fadenkreuz */	/* �^�[�Q�b�g�ڕW�\���}�[�J�[ */

	switch (data->state)
	{
	case SH_00: /* ��]���Ȃ��琳�ʂ������B / Raketen in Ausgangspos. bringen (Sprengkopf nach Norden) */
#if 1
/* CCW�̏ꍇ */
		if (0==data->nr)	{	data->angleCCW512-=deg_360_to_512CCW(/*360-*/(8))/**fps_fa_ctor*/; }	/* �E���ɉ�]������B/ linke Rakete: rot. im UZ */
		else				{	data->angleCCW512+=deg_360_to_512CCW(/*360-*/(8))/**fps_fa_ctor*/; }	/* �����ɉ�]������B/ rechte Rekete: rot. gegen UZ */
#endif
		//
		mask512(data->angleCCW512);
		//
		if ( t256(1.0) < data->speed256 )
		{
			data->speed256 -= /*(int)*/t256(0.5);/*����*/
		}
		//
#if 1
/* CCW�̏ꍇ */
		if ((data->angleCCW512>=deg_360_to_512CCW((180-4)/*266*/))&&
			(data->angleCCW512<=deg_360_to_512CCW((180+4)/*274*/))) /* �قڐ���(180)���������� */
		{
			/* Sprengkopf in Ausgangspos? */
			data->state = SH_02;						/* ���� */
			data->angleCCW512=deg_360_to_512CCW((180)); /* �����𐳖�(180)�ɌŒ肷�� */
		}
#endif
		break;
	case SH_02: /* �G��T���B / Ziel suchen */
		data->target_obj/*target_id*/ = search_enemy_by_sprite();
		if (NULL/*-1*/ != data->target_obj/*target_id*/)
		{
			target=data->target_obj/*sp rite_get_by_id (data->target_id)*/;
			if (NULL != target)
			{
				data->state = SH_03;						/* ���� */
#if 0
/* SP_FLAG_LOCK_TARGET ���̍l���͌��ׂ������ */
				target->flags|=SP_FLAG_LOCK_TARGET; /* �u������Ȃ��ł���v�t���OON */
#endif
			//	tg->flags|= SP_FLAG_VISIBLE;	/* �^�[�Q�b�g�ڕW�\���}�[�J�[ */
			}
		//	else
		//	{
		//	//	tg->flags&= (~(SP_FLAG_VISIBLE));	/* �^�[�Q�b�g�ڕW�\���}�[�J�[ */
		//	}
		}
		break;
	case SH_03: 			/* Ziel verfolgen */
		/* ���̗U���{�����A���ɓ|�������H */
		if (NULL/*-1*/ == /*data->target_id*/data->target_obj)	/* ������Ȃ�(���̗U���{�����|���Ă��܂����ꍇ) */
		{
			/* SP_FLAG_LOCK_TARGET���~�߂��̂ł����ɂ��� */
			data->state = SH_02;	/* �^�[�Q�b�g���������蒼���B */
		}
		else	/* �܂��^�[�Q�b�g�������Ă� */
		{
			if ( t256(12.0) > data->speed256)
			{
				data->speed256 += /*(int)*/t256(0.5);/*����*/
			}
			target=data->target_obj/*sp rite_get_by_id (data->target_id)*/;
			if (//(NULL != target) &&	/* �d�����Ă�̂Ŕr�� */
				(target->flags&SP_FLAG_VISIBLE) &&
				(target->x256 > t256(0)) &&
				(target->x256 < (t256(GAME_WIDTH))-((target->w128+target->w128))) &&
				(target->y256 > t256(0)) &&
				(target->y256 < (t256(GAME_HEIGHT))-((target->h128+target->h128))))
			{
#if (1)/*�U��(000)*/
				{int ta512;
#if 1
/* CCW�̏ꍇ */
//					ta512=atan_512(target->y256-src->y256,target->x256-src->x256);
					ta512=atan_512(target->y256-src->y256,target->x256-src->x256);
					#if 0
					/* �x��U�� */
					/* ����ĂȂ� */
					if (data->angleCCW512>ta512)
					{
							 if (data->angleCCW512-ta512<deg_360_to_512CCW((175)))	{	data->angleCCW512+=deg_360_to_512CCW((8))/**fps_fa_ctor*/;} /* hlaser:8, homing:9 */
						else if (data->angleCCW512-ta512>deg_360_to_512CCW((185)))	{	data->angleCCW512-=deg_360_to_512CCW((8))/**fps_fa_ctor*/;} /* hlaser:8, homing:9 */
					}
					else if (data->angleCCW512<ta512)
					{
							 if (ta512-data->angleCCW512<deg_360_to_512CCW((175)))	{	data->angleCCW512-=deg_360_to_512CCW((8))/**fps_fa_ctor*/;} /* hlaser:8, homing:9 */
						else if (ta512-data->angleCCW512>deg_360_to_512CCW((185)))	{	data->angleCCW512+=deg_360_to_512CCW((8))/**fps_fa_ctor*/;} /* hlaser:8, homing:9 */
					}
					#else
					/* ���ڗU�� */
					data->angleCCW512 = ta512;
					#endif
#endif
				}
				mask512(data->angleCCW512);
			//	tg->x=target->x+((target->w-tg->w)>>1)+ra_nd()%6-3; /* �^�[�Q�b�g�ڕW�\���}�[�J�[ */
			//	tg->y=target->y+((target->h-tg->h)>>1)+ra_nd()%6-3; /* �^�[�Q�b�g�ڕW�\���}�[�J�[ */
#endif/*�U��(000)*/
			}
			else
			{
				/* Ziel wurde anderweitig vernichtet, neues Ziel suchen */
				data->target_obj = NULL/*data->target_id=-1*/;		/* ��ʊO�ɓ�����ꂽ�� */
				data->state = SH_02;	/* �߂� */
			//	tg->flags&= (~(SP_FLAG_VISIBLE));	/* �^�[�Q�b�g�ڕW�\���}�[�J�[ */
			}
		}
		break;
	}
	#if 1
	/* (�����K�v�Ȃ�����)�O�̈׃}�X�N */
	mask512(data->angleCCW512);
	#endif
#if 1
/* CCW�̏ꍇ */
	src->x256 += ((sin512((data->angleCCW512))*data->speed256)>>8)/**fps_fa_ctor*/;
	src->y256 += ((cos512((data->angleCCW512))*data->speed256)>>8)/**fps_fa_ctor*/;
#endif
	#if 0
	/* homing */
	//	{	int ddd_angle512;
	//		ddd_angle512 = data->angle512+deg_360_to_512((180));
	//		mask512(ddd_angle512);
	//		//parsys_add(NULL,20,0,src->x+((src->w)>>1),src->y+((src->h)>>1),20,ddd_dangle512,30,10,PIXELATE,NULL);
	//	}
		#if 0
		src->anim_frame=((data->angle512*20)/512) % 20;
		#else
		src->anim_frame=((data->angle512*10)>>8);
		#endif
	#endif
	player_weapon_colision_check(src, PLAYER_WEAPON_TYPE_00_SHOT);

	data->time_out--/*range-=fps_fa_ctor*/;
	if (data->time_out/*range*/<=0) /* ���Ԑ؂�H / max. Reichweite erreicht? */
	{
		src->type = SP_DELETE;		/* ���Ԑ؂�Ȃ̂ł����܂� */
#if 0
/* SP_FLAG_LOCK_TARGET ���̍l���͌��ׂ������ */
	//	tg->type = SP_DELETE;	/* �^�[�Q�b�g�ڕW�\���}�[�J�[*/
		if (NULL != target) 	/* �^�[�Q�b�g���܂������Ă���A Target freigeben */
		{
			target->flags &= (~(SP_FLAG_LOCK_TARGET)); /* �M�����t���O�߂��Ƃ����B */
		}
#endif
	}
}
		//	/* ...orz ���肦�˂� */ error(ERR_WARN, "homing: target_id==-1&&state==3 ->back to state 2 (this shouldn't happen)");

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
	src->x256 = oya->x256;
	src->y256 = oya->y256;

	/* ���t���[�������Ȃ��ꍇ�́A������������ */
	if ((data->check_x256 != src->x256) ||
		(data->check_y256 != src->y256))
	{
		data->time_out	= 8;/* �������玩���������Ȃ� */
	}
	data->time_out--;
	if ( 1 > data->time_out)
	{
		src->type = SP_DELETE;/*�����܂�*/
	}
	data->check_x256 = src->x256;
	data->check_y256 = src->y256;
	#endif
}

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
	data->angleCCW512 += (data->add_r512/**fps_fa_ctor*/);/*...*/
	mask512(data->angleCCW512);// if (data->angle>360) {	data->angle-=360;}
	#if 1
	/* �`��p�p�x(����0�x�ō����(�����v���)) */
	src->m_angleCCW512	= (data->angleCCW512);
	#endif
//
#if 1
	/* CCW�̏ꍇ */
	src->x256 = player->x256 + sin512((data->angleCCW512))*data->radius+((player->w128-src->w128));
	src->y256 = player->y256 + cos512((data->angleCCW512))*data->radius+((player->h128-src->h128));
#endif

//	if (REIMU==select_player)
	{
		if (data->radius<255)	{	data->radius+=(ra_nd()&1);	}	/* �������L����悤�ɂ��� */
		else					{	src->type = SP_DELETE;		}	/* ��]���L�������炨���܂� */
	//	src->alpha				= ((int)(255-data->radius))&(256-1);	/* ���񂾂������悤�ɂ��� */
	//	src->color32			= ((((255-data->radius))&(0xff))<<24)|0x00ffffff;	/* ���񂾂������悤�ɂ��� */
		src->color32			= ((((255-data->radius))&(0xff))<<24)|0x00ffffff;	/* ���񂾂������悤�ɂ��� */
	}
//	else //if (YUYUKO==select_player)
//	{
//		PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
//		if (pd_bomber_time<=0) {	src->type = SP_DELETE;}/* ���Ԑ����ł����܂� */
//	}
	player_weapon_colision_check(src, PLAYER_WEAPON_TYPE_01_BOMB);/* �{���œG��|�������蔻�� */
}



/*---------------------------------------------------------
	�v���C���[�A�������{���B��ʊO�͂ݏo���`�F�b�N�B�����蔻��`�F�b�N�B
---------------------------------------------------------*/
static void player_bomber_out_colision_check(SPRITE *src)
{
	if ((src->x256+((src->w128+src->w128)) < t256(0))||(src->x256 > t256(GAME_WIDTH))||
		(src->y256+((src->h128+src->h128)) < t256(0))||(src->y256 > t256(GAME_HEIGHT)))
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
	src->x256 += src->vx256;	//co_s512((data->angle512))*d->speed/**fps_fa_ctor*/;
	src->y256 += src->vy256;	//si_n512((data->angle512))*d->speed/**fps_fa_ctor*/;
	if (CIRNO==select_player)
	{	/*1.5*(d->speed)*//**fps_fa_ctor*/;/*�`���m�p(�b��I)*/
		src->y256 -= (abs((src->vx256+(src->vx256>>1) )));
		src->y256 -= (abs((src->vy256+(src->vy256>>1) )));
		src->y256 -= t256(1);
	}
	/* YUYUKO �ȊO�͉�]/�g�傷��B */
	//if (YUYUKO != select_player)
	//if (0==(ra_nd()&0x40))/* ���܂����]/�g�� */
	{
		u8 aaa_sss[16] =
		{/* REIMU MARISA REMILIA CIRNO YUYUKO */
			0,	2,	0,	9, 0,	/* ��]���x */
			0,	2,	0,	3, 0,	/* �g�呬�x */
			0,	0,	0,	0, 0, 0,
		};
		/*(�b��I)*/
	//	if (0==(ra_nd()&0x40))/* ���܂����] */
		{	/* �\���p�x(����0�x�ō����(�����v���)) */
			src->m_angleCCW512		+= aaa_sss[select_player]/*10*/;
		}
		mask512((src->m_angleCCW512));
		if ( t256(4.0) > src->m_zoom_x256)
		{
		//	src->m_zoom_x256 += 1/*8*/;
			src->m_zoom_x256 += aaa_sss[select_player+(PLAYERS5)]/*8*/;
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
//	src->vx256 = ((src->vx256 * t256(1.1))>>8);//co_s512((data->angle512))*p->speed/**fps_fa_ctor*/;
//	src->vy256 = ((src->vx256 * t256(1.1))>>8);//si_n512((data->angle512))*p->speed/**fps_fa_ctor*/;
	// x1.125
	src->vx256 += ((src->vx256)>>3);//co_s512((data->angle512))*p->speed/**fps_fa_ctor*/;
	src->vy256 += ((src->vy256)>>3);//si_n512((data->angle512))*p->speed/**fps_fa_ctor*/;
	// x1.0625
//	src->vx256 += ((src->vx256)>>4);//co_s512((data->angle512))*p->speed/**fps_fa_ctor*/;
//	src->vy256 += ((src->vy256)>>4);//si_n512((data->angle512))*p->speed/**fps_fa_ctor*/;
	/* �`�悪�d������̂ŉ������\�A�኱����( x1.0625�� x1.125)���� */
	//
	#endif
	src->x256 += src->vx256;	//	src->vx256;//co_s512((data->angle512))*p->speed/**fps_fa_ctor*/;
	src->y256 += src->vy256;	//	src->vy256;//si_n512((data->angle512))*p->speed/**fps_fa_ctor*/;
//	if ((src->x256+((src->w128+src->w128)) < t256(0))||(src->x256 > t256(GAME_WIDTH))||
//		(src->y256+((src->h128+src->h128)) < t256(0)))
//
	player_bomber_out_colision_check(src);
}

		#if 0
	int hhh;
	hhh = BASE_BOMBER2_PNG_oz;
		#endif
		#if 0
		/*const*/ char *bbb_name[3] =
		{
			"bomber2_oz.png"/*"fire_wind_l.png"*/,	/*move left*/
			"bomber3_oz.png"/*"fire_wind_r.png"*/,	/*move right*/
			"bomber4_oz.png"/*"fire_wind_u.png"*/,
		};
		s = spr ite_add_file 0((char *)bbb_name[(ggg)], 4, PRIORITY_01_SHOT/*PRIORITY_02_PLAYER*/, 0);
		#endif


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
		//	s = sprite_add_res(hhh);hhh += PLAYERS5;
			s = sprite_add_bullet(JIKI_ATARI_ITEM_16);
			//
			s->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);
			s->type 			= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_02|SP_GROUP_SHOT_SPECIAL)/*�{�X�ɗL��*/;/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) �{�X�ɖ���*/
			s->anim_speed		= 3;
			s->anim_frame		= 0;
			s->callback_mover	= remilia_move_burn_fire;
			s->color32			= 0x64ffffff;		/*	s->alpha			= 0x64 100;*/
			s->x256 			= src->x256+((src->w128-s->w128));
			s->y256 			= src->y256+t256(15);
	u16 my_rand;
		my_rand = ra_nd();

			/*data->*/s->base_weapon_strength		= 5/*9*/ /*10*/;/* �d�l�ύX���� */
			{
			//	int rnd_spd 		= (ra_nd() & 0xff/*angCCW512*/)+256+((angCCW512+32+64) & 0x40);
			//	int aaa_spd 		= (((angCCW512+32+64) & 0x40));
			//	int rnd_spd 		= (ra_nd() & 0xff/*angCCW512*/)+256+(aaa_spd+aaa_spd);

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
			s->vx256		= ((sin512((/*data->angle512*/ang_aaa_512))*/*data->speed*/(rnd_spd)/**fps_fa_ctor*/)>>8);
			s->vy256		= ((cos512((/*data->angle512*/ang_aaa_512))*/*data->speed*/(rnd_spd)/**fps_fa_ctor*/)>>8);
	#endif
			}
		}
	}
}

/*---------------------------------------------------------
	�H�X�q�{���̐�
---------------------------------------------------------*/

static void player_move_add_oogi(SPRITE *src) /* [***090220 �ǉ� */
{
	//PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
	if ((255-32) < pd_bomber_time)
	{
	//	src->m_zoom_x256		= ((1+255-pd_bomber_time)<<3);
		src->m_zoom_x256		= ((1+255-pd_bomber_time)<<4);
	}
	else
	{
		src->m_zoom_x256		-= (1);
	}
	if ((32) > pd_bomber_time)/* �c�莞�Ԃ� 1/8 �����Ȃ�A�x������ */
	{
		src->m_zoom_x256		-= (8);
	}
	if ((0) > src->m_zoom_x256)
	{
	//	src->flags		&= (~SP_FLAG_VISIBLE);/*�\��off*/
		src->type = SP_DELETE;	/* ��k���ł����܂� */
	}

	#if 0
	data->oogi_x_offset256	= t256(11-100);
	src->x256 = player->x256+(data->oogi_x_offset256)/*+((player->w128-s->w128))*/;
	#else
	src->x256 = player->x256+(t256(11-100)/*oogi_x_offset256*/)/*+((player->w128-s->w128))*/;
	#endif
//	src->y256 = player->y256-t256(64)/*+((player->h128-src->h128))*/;		/* �H�X�q������(601���āA������鎞�ɕ�) */
	src->y256 = player->y256-t256((104/2))/*+((player->h128-src->h128))*/;	/* �H�X�q�����S(�d�X�����A�d�X���̗H�X�q�͐�̒��S�ɂ����) */
//
	#if 1/*�v��Ȃ������H*/
	if (1 > pd_bomber_time) { src->type = SP_DELETE;}/* ���Ԑ����ł����܂� */
	#endif
}

/*---------------------------------------------------------
	���~���A�{���̏\����
---------------------------------------------------------*/

static void player_move_add_cross_red(SPRITE *src)	/* [***090220 �ǉ� */
{
	//PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
	//
//	if (0x3f==(((int)pd_bomber_time)&0x3f))
//	if (0x0f==(((int)pd_bomber_time)&0x0f))
	if (0x0f==(((int)pd_bomber_time)&0x0f))/* �g�������^�C�~���O */
	{
		remilia_add_burn_fire(src);/* �g������ */
	}
	src->x256 = player->x256+((player->w128-src->w128));
	src->y256 = player->y256+((player->h128-src->h128));

	if (1 > pd_bomber_time) { src->type = SP_DELETE;}/* ���Ԑ����ł����܂� */
}


/*---------------------------------------------------------
	�v���C���[�A�j�e�̒ǉ�(�얲)
	[�j�e]�A(������)[�X�e]�A�H�X�q[���e] (���~���A�A���`���m)[�R�E�����e]
---------------------------------------------------------*/
enum
{
	NEEDLE_ANGLE_270=0,
	NEEDLE_ANGLE_263,	/* ���~���A �p */
	NEEDLE_ANGLE_277,	/* ���~���A �p */
	NEEDLE_ANGLE_271,	/* �H�X�q �p */
};
/* �얲�A�������A�H�X�q ���~���A ���`���m */	/* [***090220 �ǉ� */
static void player_add_needle(SPRITE *src, int x_offs, int y_offs, int needle_type )
{
	SPRITE *s;/* shot */
//	s					= sprite_add_file0("fireball1.png",2,PRIORITY_02_PLAYER,1); s->anim_speed=1;
//	s					= sprite_add_res( BASE_NEEDLE_PNG+select_player );
	s					= sprite_add_bullet( BASE_NEEDLE_ATARI_0a+select_player );
	//spr ite_add_file 0((char *)aaa_name[(k)],1,PRIORITY_02_PLAYER,0); s->anim_speed=0;
	/*REMILIA*/  //spr ite_add_file 0("bat.png",5,PRIORITY_02_PLAYER,0); s->anim_speed=3;
	s->type 			= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_SHOT_07|SP_GROUP_SHOT_SPECIAL)/*SP_PL_FIREBALL*/;
	s->x256 			= src->x256+((src->w128-s->w128)) + x_offs;
	s->y256 			= src->y256-t256(7) + y_offs;
//	if (REIMU != select_player) //�얲�̓A���t�@�Ȃ�	�얲���A���t�@����(�{�ƌ���������A���t�@����������)
	{
		s->color32		= 0x96ffffff;	/*	s->alpha		= 0x96; 150 */
	}
	s->callback_mover	= player_move_needle;/*player_move_koumori*/
	s->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);
//
	#define DDD_STRENGTH		(PLAYERS5*0)
	#define DDD_SPEED			(PLAYERS5*1)
	#define DDD_MAX 			(PLAYERS5*2)		/* �ő吔 */
	static Uint8 ddd_tbl[DDD_MAX] =
	{/* REIMU MARISA REMILIA CIRNO YUYUKO */
		 3, 4, 6, 6, 8, 	/* strength �V���b�g�̋��� */	/*REMILIA, CIRNO,  6 5 ��������*/
		18,10, 3, 3, 4, 	/* speed	�V���b�g�̑��� (�x��������ʏ�̒e����������̂ŋ����B�������d���Ȃ�) */
	};
/* [***20090822 REIMU 3 <- 2 (�Ƃ肠�����j�シ����̂�) */

	/*data->*/s->base_weapon_strength			= ddd_tbl[DDD_STRENGTH+select_player];
//	p_speed 				= ddd_tbl[DDD_SPEED+select_player];
//	data->strength			= (4);MARISA	data->strength		= /*3*/2;REIMU	/*��������*/
//	data->angle512			= bbb[needle_type]/*deg_360_to_512((270))*/;
//	data->speed 			= 10;	/*REMILIA, CIRNO,  3 8 ��������*/
//	s->vx256			= cos512((/*angle512*/deg_360_to_512((270))))*/*p->speed*/(ddd_tbl[DDD_SPEED+select_player])/**fps_fa_ctor*/;
//	s->vy256			= sin512((/*angle512*/deg_360_to_512((270))))*/*p->speed*/(ddd_tbl[DDD_SPEED+select_player])/**fps_fa_ctor*/;
#if 1
/* CCW�̏ꍇ */
	{
		const signed short bbb512[4] =
		{
			deg_360_to_512CCW((180)),
			deg_360_to_512CCW((180-25)),/*263=270-7*/	/* ���~���A �p */
			deg_360_to_512CCW((180+25)),/*277=270+7*/	/* ���~���A �p */
			deg_360_to_512CCW((181)),					/* �H�X�q �p */
		};
		 s->vx256	= sin512((/*angle512*/bbb512[needle_type]))*/*p->speed*/(ddd_tbl[DDD_SPEED+select_player])/**fps_fa_ctor*/;
		 s->vy256	= cos512((/*angle512*/bbb512[needle_type]))*/*p->speed*/(ddd_tbl[DDD_SPEED+select_player])/**fps_fa_ctor*/;
	}
#endif
}

/*---------------------------------------------------------
	�v���C���[�A�I�v�V�����̈ړ�����
---------------------------------------------------------*/
static void re_ma_yu_control_option(SPRITE *s, PL_OPTION_DATA *data, PLAYER_DATA *pd) /* �얲�A������	���H�X�q */
{
	mask512(data->angleCCW512);
	{int vv512;
		vv512 = (YUYUKO==select_player)?(3):(7);	/*ra d2deg512(0.1) == 8.1*/
		if (my_pad & PSP_KEY_SLOW)		/* �����ƑO���Ɉړ� */
		{
			if ((OPTION_ID_03_RM/*1*/&0xff)==(data->opt_anime_add_id&0xff)/*pos*/) /* 1: �E���n�̃I�v�V����(OPTION_ID_03_RM, OPTION_ID_04_RR ) */
			{
				if (data->angleCCW512 < /*(288)*/(256+16	-16-64+(7<<4)+((data->opt_anime_add_id&0x100)>>2)-(vv512<<4) )/*272 (256+16)*/ )/*����(�E)�ʒu*/
				{
					data->angleCCW512 += vv512;
				}
			}
			else	/* 0: �����n�̃I�v�V���� */
			{
				if (data->angleCCW512 > /*(224)*/(256-16	+16+64-(7<<4)-((data->opt_anime_add_id&0x100)>>2)+(vv512<<4) )/*240 (256-16)*/ )/*����(��)�ʒu*/
				{
					data->angleCCW512 -= vv512;
				}
			}
		}
		else	/* �����ƌ���Ɉړ� */
		{
			if ( (OPTION_ID_03_RM/*1*/&0xff)==(data->opt_anime_add_id&0xff)/*pos*/) /* 1: �E���n�̃I�v�V���� */
			{
				if (data->angleCCW512 > 64+16 +8+32-64+((data->opt_anime_add_id&0x100)>>2) )/*�ŏ�(�E)�ʒu*/
				{
					data->angleCCW512 -= vv512;
				}
			}
			else	/* 0: �����n�̃I�v�V���� */
			{
				if (data->angleCCW512 < 448-16 -8-32+64-((data->opt_anime_add_id&0x100)>>2) )/*�ő�(��)�ʒu*/
				{
					data->angleCCW512 += vv512;
				}
			}
		}
	}
	#if (0000)
	if (0==(pd->state_flag&STATE_FLAG_08_OPTION_HIDE))	/* �I�v�V�����������Ă�� */
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
		hankei = (YUYUKO==select_player)?(32):(16);
#if 1
/* CCW�̏ꍇ */
		s->x256 = player->x256+sin512((data->angleCCW512))*(hankei)/*data->length*/+ (18*256)-(((data->opt_anime_add_id&2)<<8)*6) -((s->w128)); // 2079061 (4�I�v�V�����Ή�)
		s->y256 = player->y256+cos512((data->angleCCW512))*(hankei)/*data->length*/+((player->h128-s->h128))-t256(5);
#endif
	}
}

/*---------------------------------------------------------
	�v���C���[�A�I�v�V�����̈ړ�(�얲�A�������A���H�X�q)
---------------------------------------------------------*/

static void re_gggg(SPRITE *s, int pd_weapon)	/* �얲 */
{
	PL_OPTION_DATA *data=(PL_OPTION_DATA *)s->data;

//	player_add_needle(s, t256( 0), t256( 0), NEEDLE_ANGLE_270);
	/* �얲�̏ꍇ�O���W��������Ƌ����B */
	mask512(data->angleCCW512);
	{signed int diff256;
		diff256 = (data->angleCCW512);
	//	diff256 = 512-diff256;
		diff256 -= 256;
		#if (0)/*[debug]*/
		((PLAYER_DATA *)player->data)->my_score = data->angleCCW512;
		#endif /*(0000)*/
	//	player_add_needle(s,  ((diff256)<<6)+(((data->opt_anime_add_id)&0x0c00))-((0x0c00)>>1), t256( 0), NEEDLE_ANGLE_270);
	//	player_add_needle(s,  ((diff256)<<6)+(((data->opt_anime_add_id)&0x1e00))-((0x0800)), t256( 0), NEEDLE_ANGLE_270);
	//	player_add_needle(s, -((diff256)<<6)+(((data->opt_anime_add_id)&0x1e00))-((0x0800)), t256( 0), NEEDLE_ANGLE_270);
	//	player_add_needle(s, -((diff256)<<6)+(((data->opt_anime_add_id)&0x1e00))-((0x1000)), t256( 0), NEEDLE_ANGLE_270);
		player_add_needle(s, -((diff256)<<4)+(((data->opt_anime_add_id)&0x1e00))-((0x1000)), t256( 0), NEEDLE_ANGLE_270);
	}
}
/*
����: [51]	[461=256+128+64+16-3]
����: [  ]	[314=256	+64   -6]
*/

static void ma_gggg(SPRITE *s, int pd_weapon)	/* ������ */
{
	player_add_needle(s, t256( 0), t256( 0), NEEDLE_ANGLE_270);
}

static void yu_gggg(SPRITE *s, int pd_weapon)	/* �H�X�q */
{
	player_add_needle(s, t256( 0), t256(-40), NEEDLE_ANGLE_271);/*-48*/
}

#if 0
	#if 1
	//	s->anim_frame=((s->anim_frame+(data->opt_anime_add_id)+(data->opt_anime_add_id)-1/*data->pos*/)&(8-1)/*%8*/);
		s->anim_frame=((s->anim_frame+(data->opt_anime_add_id))&(8-1)/*%8*/);
	//	if (s->anim_frame<0)		{	s->anim_frame=7;}
	#else
		s->anim_frame++;
		s->anim_frame &= (8-1);//if (s->anim_frame==8)	{s->anim_frame=0;}
	#endif
#endif

static void animate_option_re_ma_yu(SPRITE *src)
{
#if 1
	if (YUYUKO==select_player)
	{
		/*REMILIA_OPTION_DATA*/PL_OPTION_DATA *data=(/*REMILIA_OPTION_DATA*/PL_OPTION_DATA *)src->data;
		data->anime_wait--;
		if (data->anime_wait<1)
		{
			data->anime_wait=/*data->anime_speed;*/player_fix_status[BASE_OPT_SHOT_ANIME+select_player];/*12*/ /*2*/ /*�A�z���܂͂���������*/
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
	if (CIRNO==select_player)
	{
		/*REMILIA_OPTION_DATA*/PL_OPTION_DATA *data=(/*REMILIA_OPTION_DATA*/PL_OPTION_DATA *)src->data;
		data->anime_wait--;
		if (data->anime_wait<1)
		{
			data->anime_wait=/*data->anime_speed;*/player_fix_status[BASE_OPT_SHOT_ANIME+select_player];/*12*/ /*2*/ /*�A�z���܂͂���������*/
		#if 0
		//	src->anim_frame=((src->anim_frame+(data->opt_anime_add_id)+(data->opt_anime_add_id)-1/*data->pos*/)&(8-1)/*%8*/);
			src->anim_frame=((src->anim_frame+(data->opt_anime_add_id))&(8-1)/*%8*/);
		//	if (src->anim_frame<0)		{	src->anim_frame=7;}
		#else
			#if 0
			src->anim_frame++;
			src->anim_frame &= (8-1);//if (src->anim_frame==8)	{src->anim_frame=0;}
			#else
			src->type = ((src->type) & (0xfff8))|((src->type+1) & (8-1));//if (src->anim_frame==8)	{src->anim_frame=0;}
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

static void re_ma_yu_move_option(SPRITE *src)	/* �얲 	������	���H�X�q */
{
	/* �I�v�V��������\���̏ꍇ�A�������Ȃ��B */
	if (0==(src->flags & ( SP_FLAG_VISIBLE)))	{	return; 	}
//
	// �A�j���[�V����
	animate_option_re_ma_yu(src);
//
	/*REMILIA_OPTION_DATA*/PL_OPTION_DATA *data = (/*REMILIA_OPTION_DATA*/PL_OPTION_DATA *)src->data;
	PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
	if (pd->state_flag & STATE_FLAG_15_KEY_SHOT)
	{
		{
			{
				data->opt_shot_interval--;
				if (data->opt_shot_interval<0)
				{
					data->opt_shot_interval=player_fix_status[BASE_OPT_SHOT_INTERVAL+select_player];
					#if 0//2083393
					/*const*/static void (*ggg[PLAYERS5])(SPRITE *sss, int pd_weapon) =
					{
						/*REIMU*/		re_gggg,	// �얲
						/*MARISA*/		ma_gggg,	// ������
						/*REMILIA*/ 	re_gggg,	// ���݃_�~�[
						/*CIRNO*/		re_gggg,	// ���݃_�~�[
						/*YUYUKO*/		yu_gggg,	// �H�X�q
					};
					(ggg[select_player])(src, pd->weapon_power);
					#else//2082433
					switch (select_player)
					{
					case REIMU: 	re_gggg(src, pd->weapon_power); break;	// �얲
					case MARISA:	ma_gggg(src, pd->weapon_power); break;	// ������
					case YUYUKO:	yu_gggg(src, pd->weapon_power); break;	// �H�X�q
					}
					#endif
				}
			}
		}
	}
	re_ma_yu_control_option(src, data, pd);
}

/*---------------------------------------------------------
	�v���C���[�A�I�v�V�����̒�`
---------------------------------------------------------*/

/* [***090128 [***090220	�ǉ�:���~���A�p�I�v�V����.c3,c4 */

enum
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

enum
{
	FORMATION_00 = 0,
	FORMATION_01,
	FORMATION_02,
	FORMATION_03,
};
/*
	�ڕW:state1
	FORMATION_00: ���~���A�̒���Ɏl��
	FORMATION_01: ���~���A�̎��������
	FORMATION_02: ��ʌ������x������� (y=250�̈ʒu�ɎU��΂�)
	FORMATION_03: �������p(�D���Ȉʒu�ɒu�����) (���~���A�ɏd�Ȃ�悤�ɁBslow�������ƒi�K�I��option��~)
*/
static void oz_ci_move_option(SPRITE *src) /* ���~���A �� �`���m 	[***090220 �ǉ� */
{
	/* �I�v�V��������\���̏ꍇ�A�������Ȃ��B */
	if (0==(src->flags & ( SP_FLAG_VISIBLE)))	{	return; }
//
	// !--------------------
	REMILIA_OPTION_DATA *data=(REMILIA_OPTION_DATA *)src->data;
	PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
//
	static int state1=0;
	if (src->flags&SP_FLAG_VISIBLE) /* �\������Ă���Ώ�L�̏����������B */
	{
		/* �A�j���[�V���� */
		animate_option_oz_ti(src);
		/* �U���x�� */
		if (pd->state_flag & STATE_FLAG_15_KEY_SHOT)
		{
			{
				{
					data->opt_shot_interval--;
					if (data->opt_shot_interval<0)
					{
						switch (data->opt_anime_add_id)
						{
						case OPTION_C1:
						case OPTION_C2:
							if (pd->weapon_power > 100-1)
							{
								player_add_needle(src, t256( 0), t256( 0), NEEDLE_ANGLE_277 );
								player_add_needle(src, t256( 0), t256( 0), NEEDLE_ANGLE_263 );
								data->opt_shot_interval=7+3;
							}
							else
							{
								player_add_needle(src, t256( 0), t256( 0), NEEDLE_ANGLE_270 );
								data->opt_shot_interval=5+3;
							}
							break;
						case OPTION_C3:
						case OPTION_C4:
							player_add_needle(src, t256( 0), t256( 0), NEEDLE_ANGLE_270 );
							data->opt_shot_interval=5+3;
							break;
						}
					}
				}
			}
		}
		//
		int slow_flag;
		slow_flag=0;
		// �ᑬ�ړ� */
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
		player_offs_x256 = player->x256+((player->w128-src->w128));
		player_offs_y256 = player->y256+((player->h128-src->h128));
	//	if (1==slow_flag)	/* psp��0���W�X�^������̂�0�Ɣ�r�����ق������� */
		if (0!=slow_flag)	// ���Ȃ��Ƃ��ᑬ�ړ��ŃI�v�V�������~�߂���̂̓��~���A�̂�(�`���m�ɂ͍���Ȃ�)
		{
			/* ���~���A�p */
			data->slow_count += (1<<2);
			if ((/*data->*/state1==FORMATION_03) && /* FORMATION_03: �������p(�D���Ȉʒu�ɒu�����) */
				(data->slow_count < ((data->opt_anime_add_id+1)<<(6+2))/* *30 64*/))
			{
				src->x256 = player_offs_x256;
				src->y256 = player_offs_y256;
			}
			else if (/*data->*/state1==FORMATION_02)	/* FORMATION_02: ��ʌ������x������� */
			{
				switch (data->opt_anime_add_id)
				{
				case OPTION_C1: if (src->x256 < t256(170)-((src->w128+src->w128))) {src->x256 += t256(2)/**fps_fa_ctor*/;}	break;
				case OPTION_C2: if (src->x256 > t256(210))					{src->x256 -= t256(2)/**fps_fa_ctor*/;} break;
				case OPTION_C3: if (src->x256 < t256(100)-((src->w128+src->w128))) {src->x256 += t256(4)/**fps_fa_ctor*/;}	break;
				case OPTION_C4: if (src->x256 > t256(280))					{src->x256 -= t256(4)/**fps_fa_ctor*/;} break;
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
			// �t�H�[���[�V�����ύX
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
	/* �p�l���̃X�R�A����move_type���A�O���C�Y����state1��\��������B���Ă����������������Ⴄ�B */
	/*
	my_score0000460960 == 0xb410
	0x1
		STATE_FLAG_05_IS_BOSS						(0x0010)
	0x4
		STATE_FLAG_11_IS_BOSS_DESTROY				(0x0400)
	0xb
		STATE_FLAG_13_DRAW_BOSS_GAUGE				(0x1000)
		STATE_FLAG_14_GAME_LOOP_QUIT				(0x2000)
		STATE_FLAG_16_NOT_KEY_CONTROL				(0x8000)
	 */
	((PLAYER_DATA *)player->data)->my_score 	= ((PLAYER_DATA *)player->data)->state_flag/*spell_card_number*/;
	((PLAYER_DATA *)player->data)->graze_point	= /*data->*/state1;
	#endif
						/*data->*/state1++;
						if (/*data->*/state1==(FORMATION_03+1) )
						{	/*data->*/state1=FORMATION_00;}
					//
						data->state2=0; 								/* OPTION_C1 �� state2 */
						((REMILIA_OPTION_DATA *)option[OPTION_C2]->data)->state2=0; /* OPTION_C2 �� state2 */
						((REMILIA_OPTION_DATA *)option[OPTION_C3]->data)->state2=0; /* OPTION_C3 �� state2 */
						((REMILIA_OPTION_DATA *)option[OPTION_C4]->data)->state2=0; /* OPTION_C4 �� state2 */
					}
				}
			}
			switch (data->state2)
			{
			case 0: 	//������
				{
					enum
					{
						FORMATION_00_LOCATE_X=0,
						FORMATION_00_LOCATE_Y,/* 1*/
						FORMATION_01_RADIUS_R,/* 2*/
						FORMATION_01_ADD_R,   /* 3*/
						FORMATION_02_LOCATE_X,/* 4*/
						FORMATION_02_LOCATE_Y,/* 5*/
						FORMATION_MAX		  /* 6*/		/* �ő吔 */
					};
					const signed short fff[FORMATION_MAX][4] =
					{
						{	t256(15),  t256(-15),	t256(25),  t256(-25) }, 	/* FORMATION_00: ���~���A�̒���Ɏl�� */
						{	t256(15),  t256( 15),	t256(20),  t256( 20) }, 	/* FORMATION_00: ���~���A�̒���Ɏl�� */
						{	(20),  ( 30),	(40),  ( 50) }, 					/* FORMATION_01: ��]���a */
						{	( 1),  (  2),	(-3),  ( -6) }, 					/* FORMATION_01: ��]�����A�p�x���Z�l */
//					//	{	( 1*115+10),  ( 2*115+10),	( 0*115+10),  ( 3*115+10) },	/* FORMATION_02: ��ʌ������x�������(wideth380dot) */
						{	( 1*104+20),  ( 2*104+20),	( 0*104+20),  ( 3*104+20) },	/* FORMATION_02: ��ʌ������x�������(wideth352dot) */
						{  (240),  (240),  (220),  (220) }, 							/* FORMATION_02: ��ʌ������x������� */
					};
					switch (/*data->*/state1)
					{
					case FORMATION_00:	/* FORMATION_00: ���~���A�̒���Ɏl�� */
						data->offset_x256=((fff[FORMATION_00_LOCATE_X][(data->opt_anime_add_id)]));
						data->offset_y256=((fff[FORMATION_00_LOCATE_Y][(data->opt_anime_add_id)]));
						break;
					case FORMATION_01:	/* FORMATION_01: ���~���A�̎�������� */
						data->offset_x256 = fff[FORMATION_01_RADIUS_R][(data->opt_anime_add_id)];/*40*/ /* �~�̔��a�ƌ��p */
						data->offset_y256 = fff[FORMATION_01_ADD_R ][(data->opt_anime_add_id)]; 		/* �~�̉�]�����A�p�x���Z�l�ƌ��p */
/// 					data->f_angle512=0/*l_angle512*/;
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
			case 1: 	// �ڕW�n�_�ֈړ��� */
				#define oz_offset_r 	(data->offset_x256) 	/* FORMATION_01: �~�̔��a�ƌ��p */
				#define oz_offset_add_r (data->offset_y256) 	/* FORMATION_01: �~�̉�]�����A�p�x���Z�l�ƌ��p */
				{
				int delta_y256=0;				/* delta_y : y���W�ɂ�����ڕW�n�_�ƌ��ݒn�̍� */
				int delta_x256=0;				/* delta_x : x���W�ɂ�����ڕW�n�_�ƌ��ݒn�̍� */
					switch (/*data->*/state1)
					{
					case FORMATION_00:	/* FORMATION_00: ���~���A�̒���Ɏl�� */
						delta_y256 = player_offs_y256-src->y256 + data->offset_y256;
						delta_x256 = player_offs_x256-src->x256 + data->offset_x256;
						break;
					case FORMATION_01:	/* FORMATION_01: ���~���A�̎�������� */						//static int l_angle512;
#if 0
						/* ���~���A�p */
						if (REMILIA==select_player)
						{
							/*l_angle512*/data->/*f_*/angle512 += oz_offset_add_r/*data->offset_y*/ /*5==ra d2deg512(0.05)*/ /**fps_fa_ctor*/;
							mask512(/*l_angle512*/data->/*f_*/angle512);	//if (l_angle>M_PI) 	{	l_angle-=2*M_PI;	}
						}
#endif
#if 1
/* CCW�̏ꍇ */
						delta_x256 = player_offs_x256-src->x256 + (sin512((data->REMILIA_angle512/*+l_angle512*/))*oz_offset_r);
						delta_y256 = player_offs_y256-src->y256 + (cos512((data->REMILIA_angle512/*+l_angle512*/))*oz_offset_r);
#endif
						break;
					case FORMATION_02:	/* FORMATION_02: ��ʌ������x������� */
						delta_y256 = (-((src->h128))-src->y256+data->offset_y256);/*240*/
						delta_x256 = (-((src->w128))-src->x256+data->offset_x256);
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
						src->x256 += ((sin512((/*data->*/ccc_angle512))*/*10*/((REMILIA==select_player)?t256(8.0):t256(1.0)))>>8)/**fps_fa_ctor*/;
						src->y256 += ((cos512((/*data->*/ccc_angle512))*/*10*/((REMILIA==select_player)?t256(8.0):t256(1.0)))>>8)/**fps_fa_ctor*/;
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
			case 2: 	/* �ʒu�̈ێ� & �z�u����flag */
				switch (/*data->*/state1)
				{
				case FORMATION_00:	/* FORMATION_00: ���~���A�̒���Ɏl�� */
					src->x256 = player_offs_x256 + data->offset_x256;
					src->y256 = player_offs_y256 + data->offset_y256;
					break;
				case FORMATION_01:	/* FORMATION_01: ���~���A�̎�������� */
					/*l_angle512*/data->/*f_*/REMILIA_angle512 += oz_offset_add_r/*data->offset_y*/ /*5==ra d2deg512(0.05)*/ /**fps_fa_ctor*/;
					mask512(/*l_angle512*/data->/*f_*/REMILIA_angle512);	//if (l_angle>M_PI) 	{	l_angle-=2*M_PI;	}
#if 1
/* CCW�̏ꍇ */
					src->x256 = player_offs_x256 + sin512((data->REMILIA_angle512/*+data->f_angle512*/ /*l_angle512*/))*oz_offset_r;
					src->y256 = player_offs_y256 + cos512((data->REMILIA_angle512/*+data->f_angle512*/ /*l_angle512*/))*oz_offset_r;
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
//
	#if (0000)
	if (0==(pd->state_flag&STATE_FLAG_08_OPTION_HIDE))	/* �I�v�V�����������Ă�� */
	{
	//	src->type = SP_DELETE;/*�����܂�*/
	//	src->type = SP_DELETE;/*�����܂�*/
		src->flags			|= (SP_FLAG_VISIBLE);/*�����Ȃ�*/
	}
	#endif/*0000*/
}

/*---new add-----*/

/*---------------------------------------------------------
	�v���C���[�e(�V���b�g�̂�)�����A�q�֐�
---------------------------------------------------------*/

enum
{
/* �U���V���b�g */
	REIMU_SHOT_TYPE_91 = 0,
	REIMU_SHOT_TYPE_92,
	REIMU_SHOT_TYPE_93,
	REIMU_SHOT_TYPE_94,
	REIMU_SHOT_TYPE_95,
	REIMU_SHOT_TYPE_96,
/* ��U���V���b�g */
	REIMU_SHOT_TYPE_10,
	REIMU_SHOT_TYPE_11,
	REIMU_SHOT_TYPE_12,
//
	REIMU_SHOT_TYPE_13,
	REIMU_SHOT_TYPE_14,
//
	MARISA_SHOT_TYPE_03,
	MARISA_SHOT_TYPE_04,
	//MARISA_SHOT_TYPE_05,
	//MARISA_SHOT_TYPE_06,
	MARISA_SHOT_TYPE_07,
	MARISA_SHOT_TYPE_08,
//
	YUYUKO_SHOT_TYPE_01,
	YUYUKO_SHOT_TYPE_02,
	YUYUKO_SHOT_TYPE_03,
	YUYUKO_SHOT_TYPE_04,
//
	REIMU_SHOT_TYPE_MAX 	/* �ő吔 */
};

enum
{
	REI00_x_offset=0,
	REI01_y_offset,
	REI02_vx256,
	REI03_gra_type, 	/* �얲�̉�]�V���b�g */
	REI04_MAX			/* �ő吔 */
};

static void player_add_shot(SPRITE *player, int shot_type) //�얲 �� ������ �� �`���m �� ���~���A */	/* [***090220 �ǉ� */
{
	const Sint16 r_tbl[REIMU_SHOT_TYPE_MAX][REI04_MAX] =
	{	/*x_offset256	y_offset256  vx256 gra_type */
/* �U���V���b�g(�얲) */
//
/*03*/	{t256(-15), t256(+25),	(-20), 0},	/*player_add_triple_fuda*/	/*(-24)==(4*(- 6))*/
/*04*/	{t256(+15), t256(+25),	( 20), 0},	/*player_add_triple_fuda*/	/*( 24)==(4*(  6))*/
/*05*/	{t256(-15), t256(+25),	(-30), 0},	/*player_add_quad_fuda*/	/*(-40)==(4*(-10))*/
/*06*/	{t256(+15), t256(+25),	( 30), 0},	/*player_add_quad_fuda*/	/*( 40)==(4*( 10))*/
//
/*07*/	{t256(-15), t256(+25),	(-45), 0},	/*player_add_five_fuda*/	/*(-60)==(4*(-15))*/
/*08*/	{t256(+15), t256(+25),	( 45), 0},	/*player_add_five_fuda*/	/*( 60)==(4*( 15))*/

/* ��U���V���b�g(�얲) */

/*10*/	{t256(	0), t256( 20),	(  0), 0},	/*player_add_fuda*/ 		/*player_add_star*/
/*11*/	{t256( -5), t256( 20),	(  0), 0},	/*player_add_dou ble_fuda*/ /*player_add_dou ble_star*/
/*12*/	{t256( +5), t256( 20),	(  0), 0},	/*player_add_dou ble_fuda*/ /*player_add_dou ble_star*/
//
/*13*/	{t256( -5), t256(+20),	(-25), 1},	/*player_add_five_fuda*/	/*(-32)==(4*(- 8))*/
/*14*/	{t256( +5), t256(+20),	( 25), 1},	/*player_add_five_fuda*/	/*( 32)==(4*(  8))*/

/* ��U���V���b�g(������) */

/*03*/	//256( -5), t256(+20),	(  0), 0},	/*player_add_triple_star*/	/*(-16)==(4*(- 4))*/	/* ������ �܂������̕����悭�Ȃ��H */
/*04*/	//256( +5), t256(+20),	(  0), 0},	/*player_add_triple_star*/	/*( 16)==(4*(  4))*/	/* ������ �܂������̕����悭�Ȃ��H */
/*03*/	{t256(	0), t256( 30),	( -8), 0},	/*player_add_triple_star*/	/*( -8)==(4*(- 2))*/	/* ������ ���S������o�Ă��܂�g�U���Ȃ� */
/*04*/	{t256(	0), t256( 30),	(  8), 0},	/*player_add_triple_star*/	/*(  8)==(4*(  2))*/	/* ������ ���S������o�Ă��܂�g�U���Ȃ� */
/*05*/	//256( -6), t256(+20),	( -4), 0},	/*player_add_quad_star*/	/*( -4)==(4*(- 1))*/	/*(-12)==(4*(- 3))*/
/*06*/	//256( +6), t256(+20),	(  4), 0},	/*player_add_quad_star*/	/*(  4)==(4*(  1))*/	/*( 12)==(4*(  3))*/
/*07*/	{t256(-15), t256(+25),	(  0), 0},	/*player_add_quad_star*/	/*(-28)==(4*(- 7))*/	/* ������ �܂������̕����悭�Ȃ��H */	/*t256(-10)*/
/*08*/	{t256(+15), t256(+25),	(  0), 0},	/*player_add_quad_star*/	/*( 28)==(4*(  7))*/	/* ������ �܂������̕����悭�Ȃ��H */	/*t256(+10)*/

/* ��U���V���b�g(�H�X�q) */

/*01*/	{t256(-15), t256(-25),	(-25), 0},	/*�H�X�q*/					/*(-32)==(4*(- 8))*/
/*02*/	{t256(+15), t256(-25),	(+25), 0},	/*�H�X�q*/					/*( 32)==(4*(  8))*/
/*03*/	{t256(-25), t256(-25),	(-50), 0},	/*�H�X�q*/					/*(-64)==(4*(-16))*/
/*04*/	{t256(+25), t256(-25),	(+50), 0},	/*�H�X�q*/					/*( 64)==(4*( 16))*/
	};
	SPRITE *s;
//	s					= sprite_add_res( BASE_PLAZMA_PNG+select_player );
	s					= sprite_add_bullet( BASE_SHOT_ATARI_0a+select_player );
//	s->type 			= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_SHOT_02|SP_GROUP_SHOT_SPECIAL)-r_tbl[shot_type][REI03_gra_type];
	#if 1
	if (0 == r_tbl[shot_type][REI03_gra_type])
	{
		s->type 			= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_SHOT_02|SP_GROUP_SHOT_SPECIAL);
	}
	else	/* �얲�̉�]�V���b�g */
	{
		s->type 			= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_SHOT_01|SP_GROUP_SHOT_SPECIAL);
		/* �`��p�O����] */
		s->m_angleCCW512 =	(ra_nd() & 0x1ff);/* �o�����̊p�x�̓����_���łȂ���(���[�U�[�݂�����)�ςȉ�ʂɂȂ� */
	}
	#endif
	s->x256 			= (player->x256)+((player->w128-s->w128))	+ r_tbl[shot_type][REI00_x_offset];
	s->y256 			= (player->y256)-((s->h128+s->h128))		+ r_tbl[shot_type][REI01_y_offset]/*20*/;
	s->callback_mover	= player_move_shot;
	s->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);
	s->color32			= 0x96ffffff;	/*	s->alpha			= 0x96; */	/*player_add_triplestar*/
//
	#define CCC_STRENGTH		(PLAYERS5*0)
	#define CCC_SPEED			(PLAYERS5*1)
	#define CCC_MAX 			(PLAYERS5*2)		/* �ő吔 */
	static Uint8 ccc_tbl[CCC_MAX] =
	{/* REIMU MARISA REMILIA CIRNO YUYUKO */
		 2, 3, 4, 5,16, 	/* strength �V���b�g�̋��� */
		16,10, 9, 8, 4, 	/* speed	�V���b�g�̑��� (�x��������ʏ�̒e����������̂ŋ����B�������d���Ȃ�) */
	};
//		 2, 3, 4, 3, 3, 	/* strength �V���b�g�̋��� */
//		15,12,18,12,12, 	/* speed	�V���b�g�̑���	*/
/* �ɒ[�ɋ@�̂̐��\���΂�̂͗ǂ��Ȃ��B(�����đ����̂͂��邢) */
	/*data->*/s->base_weapon_strength	= ccc_tbl[CCC_STRENGTH+select_player];

//
	int int_angle512;
//	int ok;
//	ok=0;
	{
	//	if (1==r_tbl[shot_type][REI03_yudo])	/* psp��0���W�X�^������̂�0�Ɣ�r�����ق������� */
		#if 0
		if (0 != r_tbl[shot_type][REI03_yudo])/* �����V���b�g���H */
		#else
		if ( (REIMU_SHOT_TYPE_10) > shot_type)/* �����V���b�g���H */
		#endif
		{
			SPRITE *target;
			target = search_enemy_by_sprite();
			if (target != NULL)
			{
				/*data->*/int_angle512	= atan_512(target->y256-player->y256,target->x256-player->x256);
			//	if (255 < /*data->*/int_angle512 )/* ���180-360�x[/360�x]�̗̈�̂ݒǔ����� */
#if 1
/* CCW�̏ꍇ */
				if ( /*(384)*/(128*3) < ((/*data->*/int_angle512+64+128)&(512-1)) )/* ���225-315�x[/360�x]�̗̈�̂ݒǔ����� */
#endif
				{
#if 1
/* CCW�̏ꍇ */
					s->vx256	 = sin512((/*angle512*/int_angle512))*/*p->speed*/(16/*ccc_tbl[CCC_SPEED+select_player]*/)/**fps_fa_ctor*/;
					s->vy256	 = cos512((/*angle512*/int_angle512))*/*p->speed*/(16/*ccc_tbl[CCC_SPEED+select_player]*/)/**fps_fa_ctor*/;
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
		s->vx256 = ((/*int_vx*/r_tbl[shot_type][REI02_vx256]))*/*p->speed*/(ccc_tbl[CCC_SPEED+select_player])/**fps_fa_ctor*/;
		s->vy256 = -(((int)ccc_tbl[CCC_SPEED+select_player])<<8);/**fps_fa_ctor*/
	}
//	data->speed 	= ccc_tbl[CCC_SPEED+select_player];
}

/*---------------------------------------------------------
	�v���C���[�e(�V���b�g�̂�)����
---------------------------------------------------------*/

static void player_add_single_shot(SPRITE *player)	/* �얲 �� ������ �� �`���m �� ���~���A */	/* [***090220 �ǉ� */
{
	player_add_shot(player,REIMU_SHOT_TYPE_10);
}
static void player_add_dual_shot(SPRITE *player)	/* �얲 �� ������ �� �`���m �� ���~���A */	/* [***090220 �ǉ� */
{
	player_add_shot(player,REIMU_SHOT_TYPE_11);
	player_add_shot(player,REIMU_SHOT_TYPE_12);
}

static void player_add_triple_fuda(SPRITE *player)	/* �얲 */
{
	player_add_shot(player,REIMU_SHOT_TYPE_91);/* �����V���b�g */
	player_add_shot(player,REIMU_SHOT_TYPE_10);
	player_add_shot(player,REIMU_SHOT_TYPE_92);/* �����V���b�g */
}

static void player_add_quad_fuda(SPRITE *player)	/* �얲 */
{
	player_add_shot(player,REIMU_SHOT_TYPE_93);/* �����V���b�g */
	player_add_shot(player,REIMU_SHOT_TYPE_11);
	player_add_shot(player,REIMU_SHOT_TYPE_12);
	player_add_shot(player,REIMU_SHOT_TYPE_94);/* �����V���b�g */
}

static void player_add_five_fuda(SPRITE *player)	/* �얲 */
{
	player_add_shot(player,REIMU_SHOT_TYPE_95);/* �����V���b�g */
	player_add_shot(player,REIMU_SHOT_TYPE_13);
	player_add_shot(player,REIMU_SHOT_TYPE_10);
	player_add_shot(player,REIMU_SHOT_TYPE_14);
	player_add_shot(player,REIMU_SHOT_TYPE_96);/* �����V���b�g */
}

static void player_add_dual_star(SPRITE *player)	/* ������ */
{
	player_add_shot(player,MARISA_SHOT_TYPE_03);	/* ���g�U */
	player_add_shot(player,MARISA_SHOT_TYPE_04);	/* ���g�U */
}

static void player_add_triple_star(SPRITE *player)	/* ������ �� �`���m */
{
	player_add_shot(player,MARISA_SHOT_TYPE_03);	/* ���g�U */	/* REIMU_SHOT_TYPE_11 �܂����� */
	player_add_shot(player,REIMU_SHOT_TYPE_10);
	player_add_shot(player,MARISA_SHOT_TYPE_04);	/* ���g�U */	/* REIMU_SHOT_TYPE_12 �܂����� */
}

static void player_add_quad_star(SPRITE *player)	/* ������ */
{
	player_add_shot(player,REIMU_SHOT_TYPE_11); 	/* REIMU_SHOT_TYPE_11 �܂����� */	/*MARISA_SHOT_TYPE_05*/
	player_add_shot(player,REIMU_SHOT_TYPE_12); 	/* REIMU_SHOT_TYPE_12 �܂����� */	/*MARISA_SHOT_TYPE_06*/
	player_add_shot(player,MARISA_SHOT_TYPE_07);
	player_add_shot(player,MARISA_SHOT_TYPE_08);
}

static void player_add_chou(SPRITE *player) 		/* �H�X�q */
{
	player_add_shot(player,YUYUKO_SHOT_TYPE_01);
	player_add_shot(player,YUYUKO_SHOT_TYPE_02);
	player_add_shot(player,YUYUKO_SHOT_TYPE_03);
	player_add_shot(player,YUYUKO_SHOT_TYPE_04);
}

/*---------------------------------------------------------
	�v���C���[�V�[���h����(�얲)
---------------------------------------------------------*/

static void player_create_bomber_kekkai_parrent(SPRITE *src)		/* �얲 */
{
	PLAYER_DATA *pd = (PLAYER_DATA *)src->data;
	pd->state_flag	|= STATE_FLAG_02_BOMB_AUTO_GET_ITEM;	/*�{���ɂ�鎩�����W�\*/
	int ii;//	int i;	/* ���ی����� */
	for (ii=0; ii<(8); ii++)//	for (i=0;i<360/*i<=359*/;i+=45)
	{
		SPRITE *s;
		//	s				= spr ite_add_file 0("bomber1_re.png", 32/*36*/, PRIORITY_03_ENEMY/*PRIORITY_02_PLAYER*/, 1);/*"cshoot1.png"*/
		//	s				= spr ite_add_file 0("bomber2_re.png", 32/*36*/, PRIORITY_03_ENEMY/*PRIORITY_02_PLAYER*/, 1);/*"cshoot2r.png""cshoot2.png"*/
		//	s				= sprite_add_res(BASE_BOMBER1_PNG);
		//	s				= sprite_add_res(BASE_BOMBER2_PNG);
			s				= sprite_add_bullet(TAMA_ATARI_04);
		PL_KEKKAI_DATA *data;
			data			= mmalloc(sizeof(PL_KEKKAI_DATA));
			s->data 		= data;

		if (0 == (ii&1))// if (i%90 == 0)
		{
			data->add_r512	= (23); 							/* ���v��� */
			data->radius	= 5/*(48)*/;	//d->radius=38; 	/*���a�����l*/	/* �������L����悤�ɂ��� */
		}
		else		/* 1���ƕs���Ȃ̂�2������B */
		{
			data->add_r512	= (-17);							/* �����v��� */
			data->radius	= 0/*(45)*/;	//d->radius=35; 	/*���a�����l*/	/* �������L����悤�ɂ��� */
		}
//		if (YUYUKO==select_player)
//		{	/* �H�X�q */
//			data->radius		+= 45/*45*/;		/*���a�����l*/
//			s->type 		= (SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO)/*�{�X�ɖ���*/;
//		}
//		else // if (REIMU==select_player)
		{	/* �얲 */
			s->type 		= (ii&(4-1))+(/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_02|SP_GROUP_SHOT_SPECIAL)/*�{�X�ɗL��*/;/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) �{�X�ɖ���*/
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
		/*data->*/s->base_weapon_strength		= 1/*5*/;/* [***090214 �ǉ� */
	}
}

/*---------------------------------------------------------
	[�v���C���[�V�[���h]�{������
	(������ �� �`���m�� �H�X�q)
---------------------------------------------------------*/

static void player_create_bomber_levarie_parrent(SPRITE *src)
{
	PLAYER_DATA *pd = (PLAYER_DATA *)src->data;
	pd->state_flag	|= STATE_FLAG_02_BOMB_AUTO_GET_ITEM;	/*�{���ɂ�鎩�����W�\*/
	int j;		j=0;
		#if 0
	int hhh;	hhh = BASE_BOMBER1_PNG+select_player;
		#endif
	int angCCW512; angCCW512 = (0)/*0*/;/*�`���m�p(�b��I)*/	//angCCW512 = ra d2deg512(((M_PI*2)/32))/*0*/;/*�`���m�p(�b��I)*/
	int iii;
	for (iii=0; iii<(18*3)/*16*/; iii++)
	{
		SPRITE *s;
	//	s = NULL;
	//	s					= spr ite_add_file 0((char *)bbb_name[(k+j)], 3, PRIORITY_01_SHOT/*PRIORITY_02_PLAYER*/, 0);
	//	s					= sprite_add_res(hhh);hhh += PLAYERS5;
		s					= sprite_add_bullet(BASE_BOMBER_ATARI_0a+select_player);
		j++;if (6==j)
		{
			j=0;
		#if 0
			hhh = BASE_BOMBER1_PNG+select_player;
		#endif
		}
		s->anim_speed		= 5;
		s->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);
		s->callback_mover	= player_move_levarie;
		#if 0
		{
			Uint16 player_fix_status_ggg[8] =
			{/* REIMU MARISA REMILIA CIRNO YUYUKO */
				256, 256, 32, 32, 256, 0,0,0,	/* �`��p�{���̏����T�C�Y / size of bomb at first. */
			};
			s->m_zoom_x256		= player_fix_status_ggg[select_player]/*8*/;/* 64 == (1/4) */
		}
		#else
		if (YUYUKO!=select_player)
		{
			s->m_zoom_x256		= 1+(ra_nd()&(64-1));
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
		s->vx256		= ((sin512((/*angle512*/angCCW512))*/*p->speed*/(d_speed256))>>8)/**fps_fa_ctor*/;
		s->vy256		= ((cos512((/*angle512*/angCCW512))*/*p->speed*/(d_speed256))>>8)/**fps_fa_ctor*/;
#endif
//
		s->x256 			= (src->x256)+((src->w128-s->w128));
		s->y256 			= (src->y256)+((src->h128-s->h128));
		/* �M���X�g�h���[��(���ǂ�)�B�X�^�[�_�X�g�����@���G�ƈႢ�ߐڐ��\�͖����B */
		if (YUYUKO==select_player)	/* �H�X�q */
		{
			s->x256 		+= ((s->vx256)<<5);
			s->y256 		+= ((s->vy256)<<5);
		#if 0
			if (BASE_BOMBER5_PNG_yu==hhh)	{	hhh=BASE_BOMBER3_PNG_yu;}
		#endif
//			s->type 		= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_02|SP_GROUP_SHOT_ZAKO)/*�{�X�ɖ���*/;/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_SPECIAL)*/ /*�{�X�ɗL��*/
//			/* �H�X�q �������邩��ȁ`(���ʎア�����ʂ���̂��o����悤�ɃV�X�e�����������ق�����������) */
		}
//		else
		{
			s->type 		= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_02|SP_GROUP_SHOT_SPECIAL)/*�{�X�ɗL��*/ /*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) �{�X�ɖ���*/;
		}
		s->type += j;
		#if 1
		/* �`��p�p�x(����0�x�ō����(�����v���)) */
		s->m_angleCCW512		= angCCW512;
		#endif
	//	angCCW512 += (int)((512)/16); // 22.5�x/360, ��/8 /* 0.392699081698724154810 */
		angCCW512 += (int)((512)/18); // 20.0�x/360, ��/9 /* 0.349065850398865915384 */
	}
}

/*---------------------------------------------------------
	�v���C���[�V�[���h����(�H�X�q)[�H�X�q�{���̐�{��]
---------------------------------------------------------*/

static void player_create_bomber_oogi_parrent(SPRITE *src)
{
//	PLAYER_DATA *pd 	= (PLAYER_DATA *)s->data;
//	pd->state_flag		|= STATE_FLAG_02_BOMB_AUTO_GET_ITEM;	/*�{���ɂ�鎩�����W�\*/	/*��(player_create_bomber_levarie_parrent)�Őݒ�ς�*/
	SPRITE *s;
//	s					= spr ite_add_file 0("bomber1_oz.png", 10, PRIORITY_01_SHOT/*PRIORITY_02_PLAYER*/, 0);/*"cross_red.png"*/
//	s					= sprite_add_res(BASE_BOMBER5_PNG_yu);
	s					= sprite_add_bullet(JIKI_ATARI_ITEM_80);
	s->flags			|= (/*SP_FLAG_VISIBLE|*/SP_FLAG_TIME_OVER);
	s->anim_speed		= 0;
	s->anim_frame		= 0;
	s->color32			= 0xffffffff;		/*	s->alpha			= 0xff;*/
	s->callback_mover	= player_move_add_oogi;
	s->type 			= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_SHOT_00|SP_GROUP_SHOT_ZAKO);/* �{�X�̒��ڍU���͋֎~ */		/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) �{�X�ɖ���*/
	s->x256 			=	0/*(src->x256)+((src->w128-s->w128))*/;
	s->y256 			= 272/*(src->y256)+((src->h128-s->h128))*/;
//
	/* �`��p�p�x */
//	s->m_angleCCW512		= (128);
	s->m_angleCCW512		= (0);
//
	/*data->*/s->base_weapon_strength		= 1;		/* ��{�� �̋��� */
}

/*---------------------------------------------------------
	�v���C���[�V�[���h����(���~���A)[���~���A�{���̏\�����{��]
---------------------------------------------------------*/

static void player_create_bomber_cross_red_parrent(SPRITE *src) /* ���~���A */ /* [***090220 �ǉ� */
{
	PLAYER_DATA *pd 	= (PLAYER_DATA *)src->data;
	pd->state_flag		|= STATE_FLAG_02_BOMB_AUTO_GET_ITEM;	/*�{���ɂ�鎩�����W�\*/
	SPRITE *s;
//	s					= spr ite_add_file 0("bomber1_oz.png", 10, PRIORITY_01_SHOT/*PRIORITY_02_PLAYER*/, 0);/*"cross_red.png"*/
//	s					= sprite_add_res(BASE_BOMBER1_PNG_oz);
	s					= sprite_add_bullet(JIKI_ATARI_ITEM_16);
	s->flags			|= (/*SP_FLAG_VISIBLE|*/SP_FLAG_TIME_OVER);
	s->flags			&= (~(SP_FLAG_VISIBLE));	/*��\��*/
	s->anim_speed		= 0;
	s->anim_frame		= 0;
	s->color32			= 0xdcffffff;		/*	s->alpha			= 0xdc;*/
	s->callback_mover	= player_move_add_cross_red;
//	s->type 			= (SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO);/* �{�X�̒��ڍU���͋֎~ */		/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) �{�X�ɖ���*/
	s->type 			= (/*�\�����Ȃ�*/SP_GROUP_ETC/*SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO*/);/* �{�X�̒��ڍU���͋֎~ */		/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) �{�X�ɖ���*/
	s->x256 			= (src->x256)+((src->w128-s->w128));
	s->y256 			= (src->y256)+((src->h128-s->h128));
}


/*---------------------------------------------------------
	�v���C���[�A�U���e�̈ړ�����
---------------------------------------------------------*/

//#define hlaser_NUM_OF_ENEMIES (24)
#define hlaser_NUM_OF_ENEMIES (12)		/* [***090128		�����ɂ��Ă݂� */

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
		int i;
		for (i=0; i<hlaser_NUM_OF_ENEMIES; i++)
		{
			s_old = s;
		//	s				= sprite_add_res( BASE_TSHOOT_PNG+select_player );
			s				= sprite_add_bullet(JIKI_ATARI_ITEM_16);
			s->type 		= (SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_SPECIAL)/*�{�X�ɗL��*/ /*SP_PL_HLASER*/;
			s->x256 		= (src->x256) + ((src->w128)) - t256(5);
			s->y256 		= (src->y256) + ((src->h128)) + t256(15);
		//	s->flags		|= (SP_FLAG_VISIBLE/*|SP_FLAG_TIME_OVER*/);
			s->flags			&= (~(SP_FLAG_VISIBLE));	/*��\��*/
			s->anim_frame	= 5-((double)6.0/hlaser_NUM_OF_ENEMIES)*i;
			if (0==i)
			{
				s->callback_mover	= player_move_parrent_hlaser;
				PL_HOMING_DATA *data;
				data				= mmalloc(sizeof(PL_HOMING_DATA));
				s->data 			= data;
				data->nr			= j;
//				data->angle512		= deg_360_to_512((100));
//				data->angle512		= deg_360_to_512CCW(360-(100));
				data->angleCCW512	= deg_360_to_512CCW(0);
				data->speed256		= t256(12);
				/*data->*/s->base_weapon_strength		= player_fix_status[BASE_LOW_BOMB_STRENGTH+select_player]/*6*/;/*�ᑬ�{���̋���*/
				data->time_out/*range*/ 	= 100/*200*/;
				data->state 		= 0;
				data->target_obj	= NULL; 	//b->target_obj/*target_id*/	= /*-1*/NULL;
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
#undef hlaser_NUM_OF_ENEMIES


static void player_move_add_bomber_hlaser(SPRITE *src)
{
	PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
//	if (0==(((int)pd_bomber_time)&0x3f))
	if (0==(((int)pd_bomber_time)&0x0f))
	{
		/* �V�i���I���ɂ͗U���e��ǉ����Ȃ� */
		if (0==(pd->state_flag & STATE_FLAG_06_IS_SCRIPT))
		{
			player_add_hlaser(player);
		}
	}
	#if 0
	src->anim_frame 	= (((int)pd_bomber_time>>4)&0x03);/*"bomber_slow.png"*/
	src->x256			= (player->x256)+((player->w128-src->w128));
	src->y256			= (player->y256)+((player->h128-src->h128));
	#endif
//
	if (1 > pd_bomber_time) {	src->type = SP_DELETE;}/* ���Ԑ����ł����܂� */
}

/* �ᑬ�{���̐e */
static void player_create_bomber_homing_parrent(SPRITE *src)
{
	PLAYER_DATA *pd 	= (PLAYER_DATA *)src->data;
	pd->state_flag		|= STATE_FLAG_02_BOMB_AUTO_GET_ITEM;	/*�{���ɂ�鎩�����W�\*/
//
	SPRITE *c;
//	c					= spr ite_add_file 0("bomber_slow.png", 4, PRIORITY_01_SHOT/*P R_BACK1*/, 1);	c->anim_speed		= 0;
//	c					= sprite_add_res(BASE_BOMBER_SLOW_PNG);
	c					= sprite_add_bullet(JIKI_ATARI_ITEM_16);
//	c->flags			|= (SP_FLAG_VISIBLE/*|SP_FLAG_TIME_OVER*/);
	c->flags			&= (~(SP_FLAG_VISIBLE));	/*��\��*/
	c->data 			= 0/*d*/;
	c->anim_frame		= 0;
	c->color32			= 0xdcffffff;	/*	c->alpha			= 0x80 0xdc;*/
	c->callback_mover	= player_move_add_bomber_hlaser;
//	c->type 			= (SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_SPECIAL)/*�{�X�ɗL��*/; /* �ᑬ�{�����{�X�ɗL���Ƃ��� */	/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) �{�X�ɖ���*/
	c->type 			= (/*�\�����Ȃ�*/SP_GROUP_ETC/*SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_SPECIAL*/)/*�{�X�ɗL��*/;	/* �ᑬ�{�����{�X�ɗL���Ƃ��� */	/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) �{�X�ɖ���*/
	c->x256 			= (src->x256)+((src->w128-c->w128));
	c->y256 			= (src->y256)+((src->h128-c->h128));
}


/*---------------------------------------------------------
	�v���C���[�L�[����
---------------------------------------------------------*/

//static int /*bomb_wait*/d->bomber_time;		/* �{���̗L������ */	//���̃{�����o����܂ł̎���
static int weapon_List; 	//�ǂ̕���𑕔����Ă��邩

	enum /*_weapon_type_*/
	{
		WEAPON_L1=0,
		WEAPON_L2,
		WEAPON_L3,
		WEAPON_L4,
		WEAPON_L5,
	//	WEAPON_L6,
		WP_MAX		/* �ő吔 */
	};

extern /*global*/short my_analog_x; /* �A�i���O�ʁA�␳�ς� */
extern /*global*/short my_analog_y; /* �A�i���O�ʁA�␳�ς� */
extern void set_bg_alpha(int set_bg_alpha);
static void player_keycontrol(SPRITE *s1)
{
	PLAYER_DATA *pd = (PLAYER_DATA *)s1->data;
	//if player is invisible (state gameover) allow no keycontrol
	if ((0==(s1->flags&SP_FLAG_VISIBLE)))
	{
		return;
	}

	/*---------------------------------------------------------
		�v���C���[�{���֘A����
	---------------------------------------------------------*/
//	if (STATE_FLAG_06_IS_SCRIPT==(pd->state_flag & STATE_FLAG_06_IS_SCRIPT))
//	{
//		;	/* ��b�C�x���g���̓{���֘A�̏����͂��Ȃ� */
//	}
//	else
	if (0==(pd->state_flag & STATE_FLAG_06_IS_SCRIPT))
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
				pd->state_flag &= (~STATE_FLAG_02_BOMB_AUTO_GET_ITEM);	/* �{���ɂ�鎩�����W�͏I��� */
			}
		}
		else
		{
			pd_bomber_time = 0;
		//	pd->ex tra_type=PLX_NONE;
		//}
		//if (/*bomb_wait*/d->bomber_time<=0)
		//{
			if (0 < pd->bombs)
			{
				if (my_pad & PSP_KEY_BOMB_CANCEL)
				{
					#if (0==USE_DESIGN_TRACK)
					play_voice_auto_track(VOICE07_BOMB);
					#else
					voice_play(VOICE07_BOMB, TRACK02_ALEART_IVENT);/*�e�L�g�[*/
					#endif
					set_bg_alpha(50);/* ��ʂ��Â����� */
//					set_bg_alpha(100);/* ��ʂ��Â����� */
//					set_bg_alpha(127);/* ��ʂ��Â����� */
					/* �ᑬ�{�� */
					if (my_pad & PSP_KEY_SLOW)
					{
				//	case (REIMU*2+TEISOKU_MODE):/*�Ƃ肠����*/
				//	case (MARISA*2+TEISOKU_MODE):/*�Ƃ肠����*/
				//	case (REMILIA*2+TEISOKU_MODE):/*�Ƃ肠����*/
				//	case (CIRNO*2+TEISOKU_MODE):/*�Ƃ肠����*/
				//	case (YUYUKO*2+TEISOKU_MODE):/*�Ƃ肠����*/
						pd_bomber_time = 200/*320*/ /*400*/ /*800*/;	/* �{���̗L������(�ݒ薳�G����)[���P�d�l���ς�����̂Ŕ����ɂ���] */
						item_from_bullets(SP_ITEM_05_HOSI);/* �e�S������ */
						player_create_bomber_homing_parrent(player);	/*player_move_add_bomber_hlaser*/
				//		break;
					}
					/* �ʏ�{�� */
					else
					{
						switch ((select_player))
						{
					//	case (YUYUKO*2+TEISOKU_MODE):/*�Ƃ肠����*/
					//		pd_bomber_time = 300;			/* �{���̗L������(�ݒ薳�G����) */	/* ������̓{�X����(�h��^) */
					//		goto common_kekkai;
						case (REIMU):
						//	/*bomb_wait*/d->bomber_time 	= 200;
							pd_bomber_time = 180/*200*/;	/* �{���̗L������(�ݒ薳�G����) */	/* ������̓{�X�L��(�U���^) */
						//	pd_save_timer	= 200+30;		//�ݒ薳�G���ԁB���܂ɃV�[���h�����蔲����҂������̂�
						//	pd->ex tra_type = PLX_BOMB;
					//	common_kekkai:
							player_create_bomber_kekkai_parrent(player);		//�V�[���h�̒ǉ�
							break;
						case (CIRNO):/*�Ƃ肠����*/
						case (MARISA):
						case (YUYUKO):
						//	/*bomb_wait*/d->bomber_time 	= 100;
							pd_bomber_time = 100/*32*/ /*100*/; 		/* �{���̗L������(�ݒ薳�G����) */
						//	pd_save_timer	= 100+30/*200 5*/;	// �ݒ薳�G���ԁB�[���������Ƀ{�����Ԓ����G����Ȃ��̂͂���
						//	pd->ex tra_type = PLX_BOMB; 	/*��������������(extra_tem�L������)�{���������o���Ȃ���*/
							if (YUYUKO==select_player)
							{
								/* �� �L������ */
								pd_bomber_time = 255/*255==((16*4*2*2)-1)*/;			/* �{���̗L������(�ݒ薳�G����) */
								player_create_bomber_oogi_parrent(player);
							}
							player_create_bomber_levarie_parrent(player);
							break;
					//	case (YUYUKO):
					//		/* �� �L������ */
					//		pd_bomber_time = 255/*255==((16*4*2*2)-1)*/;			/* �{���̗L������(�ݒ薳�G����) */
					//		player_create_bomber_levarie_parrent(player);
					//		player_create_bomber_oogi_parrent(player);
					//		break;
						case (REMILIA): /* [***090220 �ǉ� */
						//	/*bomb_wait*/d->bomber_time 	= 150/*300*/ /*400*/;	/*400(150+250)�͖��炩�ɒ�������*/
							/* ���~���A�{���͋�������̂ŁA4������3���Ɍ��炵�A�����{���ɂ��ē��ē�����B */
							/* ���������~���A�{���̓{�X�ɗL���Ƃ��A������΋����B */
						//	pd_bomber_time = 0xfe	/* 255==0xff==4��	254==0xfe==3��==0xbf=192 2��==0x7f 150==0x96*/;
							pd_bomber_time = 0x7e	/* 255==0xff==8��	254==0xfe==7��==0xbf=192 4��==0x7f 150==0x96*/;
						//	pd_save_timer	= 150+30;		//�ݒ薳�G���ԁB
						//	pd->ex tra_type = PLX_BOMB;
							player_create_bomber_cross_red_parrent(player);
							break;
						}
					}
					pd_save_timer		= pd_bomber_time + USER_BOMOUT_WAIT;/*�������G����*/
				//	pd->ex tra_type 	= PLX_BOMB;
					s1->color32 		= 0x50ffffff;	/*	s1->alpha			= 0x50;*/	/*������*/
					pd->core->color32	= 0x50ffffff;	/*	pd->core->alpha 	= 0x50;*/	/*������*/
					pd_player_status	= PLAYER_STATE_04_SAVE_02;
					pd->bombs--;
				}
			}
		}
	}
					/*	[���P] homing / hlaser �͒ᑬ�{���ɂȂ蕐��g�p���͖��G�ƂȂ����̂ŁA
						�]���̗L�����Ԃł͒�������B�����ŗL�����Ԃ𔼕��ɂ����B*/
				//	#define USER_BOMOUT_WAIT (30)
					/*	[���Q] �������G���� == (�ݒ薳�G���� + USER_BOMOUT_WAIT)
						�t���[���X�L�b�v�������Ă����菈�����������肷��֌W��A
						�O�����K�������`���I����Ă���Ƃ͌���Ȃ��B
						�����`���I����Ă��Ȃ��ꍇ�́A�������G���� == �ݒ薳�G���� ���ƁA
						��ʂɃ{�����\������Ă���̂ɔ���Ŏ��ʁB	*/
					/*	[���R]���Q�̗]�T�������Ă��A���݂̏�Ԃł̓{���̏I��肪
						���ɂ킩��ɂ����B�Ƃ����������킩��Ȃ��B
						���炩�̌x����g�ݓ���鎖����ΕK�v�B(�����܂�����ĂȂ�)
						(�{�Ƃł͐F�X�x��������) */


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
	static const signed /*int*/short jiki_move_length[16][2] =
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
	#if 0
	/* �������[�h(�{��������) */
	/* REIMU */ 	t256(3.0),
	/* MARISA */	t256(2.0),
	/* REMILIA */	t256(5.0),
	/* CIRNO */ 	t256(4.5),/*�H������ᑬ�̕�������*/
	/* YUYUKO */	t256(5.0),
	/* �ᑬ���[�h(�{��������) */
	/* REIMU */ 	t256(2.0),
	/* MARISA */	t256(2.0),
	/* REMILIA */	t256(4.0),
	/* CIRNO */ 	t256(5.0),/*�H������ᑬ�̕�������*/
	/* YUYUKO */	t256(4.5),
		/*+*/
		/*((1 > pd_bomber_time)?(0):(PLAYERS5+PLAYERS5))*/
	#endif
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
//#define BASE_SPEED_MINIMUM		(PLAYERS5*4)
//#define BASE_SPEED_MAXIMUM		(PLAYERS5*5)
//	 2,  3,  4,  7,  3, 	/* �Œᑬ�x player_speed_minimum */
//	 4,  5,  4,  7,  3, 	/* �ō����x player_speed_maximum */
	static const signed /*int*/short player_speed256[(PLAYERS5*4)] =
	{
	/* �������[�h(�ʏ펞) */
	/* REIMU */ 	0x02f1,/* t256(2.94) 2.94140625 */
	/* MARISA */	0x03a3,/* t256(3.64) 3.63671875 */
	/* REMILIA */	0x0347,/* t256(3.28) 3.27734375 */
	/* CIRNO */ 	0x0373,/* t256(3.45) 3.44921875 */ /*�H������ᑬ�̕�������*/
	/* YUYUKO */	0x02c7,/* t256(2.78) 2.77734375 */
	/* �ᑬ���[�h(�ʏ펞) */
	/* REIMU */ 	0x0178,/* t256(1.47) 1.46875000  t256(2.0)*/
	/* MARISA */	0x0233,/* t256(2.20) 2.19921875  t256(2.5)*/
	/* REMILIA */	0x01d6,/* t256(1.84) 1.83593750  t256(2.5)*/
	/* CIRNO */ 	/*0x0700*/0x0269/*0x0269*/,/* t256(2.41) 2.41015625  t256(7.0)*/ /*�H������ᑬ�̕������� 0x0400==��������==���z���ő��H	*/
	/* YUYUKO */	0x0161,/* t256(1.38) 1.37890625  t256(2.0)*/
	};
	/*const*/ signed int my_speed = player_speed256[select_player+	((my_pad & PSP_KEY_SLOW)?(PLAYERS5):(0))];
	short	aaa_my_analog_x = (((my_speed)*(my_analog_x))>>8);
	short	aaa_my_analog_y = (((my_speed)*(my_analog_y))>>8);
	s1->x256 += ((((signed int)(jiki_move_length[((my_pad&0xf0)>>4)][0]))*(aaa_my_analog_x))>>8); /**fps_fa_ctor*/
	s1->y256 += ((((signed int)(jiki_move_length[((my_pad&0xf0)>>4)][1]))*(aaa_my_analog_y))>>8); /**fps_fa_ctor*/
	// �݂͂�������C���B
		 if (s1->x256 < t256(0))									{	s1->x256 = t256(0); 									}
	else if (s1->x256 > t256(GAME_WIDTH) -((s1->w128+s1->w128)))	{	s1->x256 = t256(GAME_WIDTH) -((s1->w128+s1->w128)); 	}
		 if (s1->y256 < t256(0))									{	s1->y256 = t256(0); 									}
	else if (s1->y256 > t256(GAME_HEIGHT)-((s1->h128+s1->h128)))	{	s1->y256 = t256(GAME_HEIGHT)-((s1->h128+s1->h128)); 	}

// > �yPSP�zPSP�œ�����肽���z8�y�e���z
// > 200 �F���������񁗂��������ς��B�F2009/01/27(��) 18:41:00 ID:xQgI7hCU
// > ���낻��S�A�C�e�������ė~�����ȁB
// > ����FULL�p���[���̃A�C�e�������񂹃��C���͂���������Ɖ��̕��������Ǝv���B
// > ��̓I�ɂ͂��̕ӂ聫
// > ttp://www4.uploader.jp/user/isisuke/images/isisuke_uljp00081.bmp
// ���̈ӌ��͂킽�������l�Ɏv�����̂ŁA�݂�Ȃ������v�����ɈႢ�Ȃ��B
// ���̉摜��484x283[dots]�ŁAy==63[line]�ɐԐ��������Ă������B
// psp�̉𑜓x�́A 480x272[dots]�Ȃ̂� x/272=63/283 , x=(63*272)/283 == 60.55[line]
// ����/���쎁	50(49)[line] �����シ����B
// �X��8��200�� ��60[line] (���o�I��)
// �����ŃL�����̍����������邵�A64(63)�������Ɖ�72(71)���炢�ł������̂ł́H�Ƃ������64�ɂ��Ď������Ă݂�B
// ���ʁF����ς����������������B8[dots]�Ƃ��킸16[dots]���炢���������B�Ăю������Ă݂�B
// ���ʁF����Ȃ��񂩂ȁH�Ƃ����킯��80(79)[dots]�Ɍ���B����8[dots]��88(87)�ł����������ˁB
// �{�Ƃ̊��o��厖�ɂ���Ȃ炱��Ȃ��񂾂Ǝv���B
#define SPLIT_LINE (t256(80))
// http://hossy.info/game/toho/k_score.php �����̉摜���t�Z���Čv�Z����� 77 dots ������B
//#define SPLIT_LINE (t256(77))

	/* MAX���̃A�C�e���������W */
	if (my_pad & PSP_KEY_UP/*PSP_CTRL_UP*/) 	/* ���F�΂ߏ�ł�����\ */ /*&& (s1->y>0)*/
	{
		if ((pd->weapon_power==127)  //128[***090123 �ύX /*max==127==�u128�i�K�v*/
			/* ������ ����\�́F�A�C�e���㕔���W�����ł��\ */
			|| (MARISA==select_player)) 	/* �������͏�ɏ㕔�������W���\ */
		{
			if (s1->y256 < SPLIT_LINE/*50*/)/**/
			{
				pd->state_flag |= STATE_FLAG_01_PLAYER_UP_AUTO_GET_ITEM;	/* �㕔�������W�\ */
			}
		}
	}
	else if (my_pad & PSP_KEY_DOWN/*PSP_CTRL_DOWN*/)	/* ���F�΂߉��ł���߂� */ /*&& (s1->y<screen->h*-s1->h)*/
	{
		//if (pd->state_flag & (STATE_FLAG_01_PLAYER_UP_AUTO_GET_ITEM /*| STATE_FLAG_02_BOMB_AUTO_GET_ITEM*/) )/*???���Ԃ�*/
		{
			pd->state_flag &= (~STATE_FLAG_01_PLAYER_UP_AUTO_GET_ITEM); 	/* �㕔�������W�s�� */
		}
	}
//
	/*---------------------------------------------------------
		�v���C���[�V���b�g�֘A����
	---------------------------------------------------------*/
	/* �V�i���I�X�N���v�g���[�h�ł̓V���b�g�{�^�����͖��� */
	if (0==(pd->state_flag & STATE_FLAG_06_IS_SCRIPT))
	{
		if (my_pad & PSP_KEY_SHOT_OK)
		{
			pd->state_flag |= STATE_FLAG_15_KEY_SHOT;	/* on */
			{static int weapon_interval=0;
				/*pd->*/weapon_interval--/*=fps_fa_ctor*/;
				if (/*pd->*/weapon_interval < 1 )
				{
					#define BASE_WEAPON_L1		(PLAYERS5*0)
					#define BASE_WEAPON_L2		(PLAYERS5*1)
					#define BASE_WEAPON_L3		(PLAYERS5*2)
					#define BASE_WEAPON_L4		(PLAYERS5*3)
					#define BASE_WEAPON_L5		(PLAYERS5*4)
					#define BASE_WEAPON_MAX 	(PLAYERS5*5)
					static Uint8 www_tbl[BASE_WEAPON_MAX] =
					{/* REIMU MARISA REMILIA CIRNO YUYUKO */	/* ���~���A��������̂Œ���(+5) */
						 5,  6,  7+5,  9,  9+8, 	/* WEAPON_L1 */
						 5,  6,  6+5,  9,  8+8, 	/* WEAPON_L2 */
						 5,  5,  7+5,  9,  7+8, 	/* WEAPON_L3 */
						 5,  5,  6+5,  9,  6+8, 	/* WEAPON_L4 */
						 5,  5,  5+5,  9,  5+8, 	/* WEAPON_L5 */
					};
					/*pd->*/weapon_interval = www_tbl[(PLAYERS5*weapon_List)+select_player];
//
					#if (0==USE_DESIGN_TRACK)
					play_voice_auto_track(VOICE00_SHOT);
					#else
					voice_play(VOICE00_SHOT, TRACK00_BULLETS);
					#endif
					/*const*/static void (*bbb[PLAYERS5][WP_MAX])(SPRITE *sss) =
					{
				/* WEAPON_L1: */			/* WEAPON_L2: */		/* WEAPON_L3: */			/* WEAPON_L4: */			/* WEAPON_L5: */
{	/*REIMU*/	player_add_single_shot, 	player_add_dual_shot,	player_add_triple_fuda, 	player_add_quad_fuda,		player_add_five_fuda,	},
{	/*MARISA*/	player_add_single_shot, 	player_add_dual_shot,	player_add_dual_star,		player_add_triple_star, 	player_add_quad_star,	},
{	/*REMILIA*/ player_add_single_shot, 	player_add_single_shot, player_add_dual_shot,		player_add_dual_shot,		player_add_dual_shot,	},
{	/*CIRNO*/	player_add_dual_shot,		player_add_dual_star,	player_add_dual_star,		player_add_triple_star, 	player_add_triple_star, },
{	/*YUYUKO*/	player_add_chou,			player_add_chou,		player_add_chou,			player_add_chou,			player_add_chou,		},	/* ���i�K���� */
					};
					(bbb[select_player][weapon_List])(s1);
				}
			}
		}
	}
	/*---------------------------------------------------------
		�v���C���[�A�j���[�V�����֘A����
	---------------------------------------------------------*/
	{static int anim_delay;
		/*pd->*/anim_delay -= 1/*fps_fa_ctor*/;
		if (0 > /*pd->*/anim_delay)
		{
			/*pd->*/anim_delay = player_fix_status[BASE_SPEED_ANIME+select_player]/*2*/;
			//
			static int auto_anim_frame = 4/*5*/;
				 if (my_pad & PSP_KEY_LEFT/*PSP_CTRL_LEFT*/)	{	if (auto_anim_frame>0	)	 auto_anim_frame--; }
			else if (my_pad & PSP_KEY_RIGHT/*PSP_CTRL_RIGHT*/)	{	if (auto_anim_frame</*7*/8/*10*/) auto_anim_frame++; }
			else
			{
				if (auto_anim_frame>4/*5*/) auto_anim_frame--;
				if (auto_anim_frame<4/*5*/) auto_anim_frame++;
			}
			{
				int now_anim_frame;
				static int auto_aniime;
				auto_aniime++;
				auto_aniime&=3;
					 if (4==auto_anim_frame)	{	now_anim_frame = (auto_anim_frame + auto_aniime);	}
				else if (4<auto_anim_frame) 	{	now_anim_frame = (auto_anim_frame + 3); 			}
				else							{	now_anim_frame = (auto_anim_frame );				}
				s1->anim_frame = (now_anim_frame);
				s1->type = (SP_GROUP_JIKI_GET_ITEM)|(now_anim_frame);
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
/* [pd->weapon_power���ύX���ꂽ�ꍇ�ɕK���s����`�F�b�N] */
static void check_weapon_level(PLAYER_DATA *pd)
{
	u8 pd_weapon;	pd_weapon = pd->weapon_power;
	/*---------------------------------------------------------
		����̒i�K�����߂�
	---------------------------------------------------------*/
	//weapon�̒i�K���獡�̑��������߂�		//���X�g�����đ����̕ύX
	// [***090123	�ő�128�ցB
	// (0-128��129�i�K���������A0-127��128�i�K�ɏC��)
		 if (pd_weapon <= ( 5-1))	{	weapon_List=WEAPON_L1;	}
	else if (pd_weapon <= (10-1))	{	weapon_List=WEAPON_L2;	}
	else if (pd_weapon <= (60-1))	{	weapon_List=WEAPON_L3;	}
	else if (pd_weapon <= (85-1))	{	weapon_List=WEAPON_L4;	}
	else /*if (pd_weapon<=(128-1))*/{	weapon_List=WEAPON_L5;	}	/*max==127==�u128�i�K�v*/
	//else							{	weapon_List=WEAPON_L6;	}
/*
[0	1  2  3  4][5  6  7  8	9]10 11 12 13 14 15 -- WEAPON_L1 -- WEAPON_L2 -- WEAPON_L3
16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31
32
48 49 50 51 52 53 54 55 56 57 58 59][60  --- WEAPON_L4
64
80 81 82 83 84][85 -- WEAPON_L5
96
112
--
128
//
[0	1  2  3  4	5  6  7][8	9 10 11 12 13 14 15]-- WEAPON_L1 -- WEAPON_L2
16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 -- WEAPON_L3 -- WEAPON_L3
32												-- WEAPON_L3 -- WEAPON_L3
48 49 50 51 52 53 54 55 56 57 58 59[60			-- WEAPON_L3 -- WEAPON_L4
64												-- WEAPON_L4 -- WEAPON_L4
80 81 82 83 84 85 86 87][						-- WEAPON_L4 -- WEAPON_L5
96												-- WEAPON_L5 -- WEAPON_L5
112 											-- WEAPON_L5 -- WEAPON_L5
*/
	/*---------------------------------------------------------
		�v���C���[�I�v�V�����`�F�b�N(�K�v�ł���΃I�v�V�������Z�b�g)
	---------------------------------------------------------*/
	#if 1
	{
		const u8 jjj_tbl[4] =
		{
			(35-1),
			(55-1),
			(80-1),
			(108-1),
		};
		int jj;
		for (jj=0; jj<4; jj++)
		{
			if ( (pd_weapon > jjj_tbl[jj]))
					{	option[jj]->flags	|= ( (SP_FLAG_VISIBLE));	}	// ���t���O��On(��)
			else	{	option[jj]->flags	&= (~(SP_FLAG_VISIBLE));	option[jj]->y256=t256(272);	}	// ���t���O��OFF(�s��)
		}
	}
	#endif
}

/*---------------------------------------------------------
	�N���A�{�[�i�X �`�F�b�N
---------------------------------------------------------*/

void player_stage_clear(void)
{
	pd_bomber_time = 0;/*�s����*/
//
	PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
			player_add_score(adjust_score_by_difficulty((
			(player_now_stage * score(1000)) +	/* �X�e�[�W x 1000pts */
			(pd->weapon_power * score(100)) +	/* �p���[	x  100pts */
			(pd->graze_point)					/* �O���C�Y x	10pts */
		)));
	//
	pd->graze_point = 0;/* ���Z���ď����� */
}




/*---------------------------------------------------------
	�v���C���[�ƃA�C�e���̂����蔻��1(�A�C�e����p)
	-------------------------------------------------------
		s1		�v���C���[
		tt		�A�C�e��
---------------------------------------------------------*/
static void player_add_power(SPRITE *s1, SPRITE *tt, int add_power)
{
	PLAYER_DATA *pd = (PLAYER_DATA *)s1->data;
	int prev_power; prev_power = pd->weapon_power;		/* �A�C�e���擾���钼�O�̃p���[�l */
	pd->weapon_power += add_power;	/* �p���[�l�A���Z */
	/* �p���[�l�A�����Ă���C�� */
	if ((127-1) < pd->weapon_power )/*127==128�i�K��*/
	{
		pd->weapon_power = 127;/* �ő�l */
	}
	int add_score_point;
	if (127 == pd->weapon_power/*prev_power*/)	/* max�̏ꍇ */
	{
		pd->chain_point += add_power;	/* �`�F�C���A���Z */
		/* �`�F�C���A�����Ă���C�� */
		if (31 < pd->chain_point)
		{
			pd->chain_point = 31;/* �ő�l */
		}
		/* (�m�[�~�X�{�[�i�X)�E�F�|���{�[�i�X���_�v�Z */
		add_score_point = (/*score_type*/(pd->chain_point));
	}
	else	/* max�łȂ��ꍇ�A���_�v�Z */
	{
		/* (MAX���ȊO��) [P]��10�_ */
		/* (MAX���ȊO��) [P��]��80�_ */
		add_score_point = ((add_power/*-1*/)/*SCORE_10*/);
	}
	bonus_info_score_nodel(tt, add_score_point);/* */tt->type = SP_DELETE;/* �����܂� */
	check_weapon_level(pd);/* [pd->weapon_power���ύX���ꂽ�ꍇ�ɕK���s����`�F�b�N] */

				#if (0==USE_DESIGN_TRACK)
				play_voice_auto_track(VOICE05_BONUS);
				#else
				voice_play(VOICE05_BONUS, TRACK07_GRAZE);/*�e�L�g�[*/
				#endif
}

static void player_colision_check_item(SPRITE *s1/*, int mask*/)
{
	/* ���@���A�C�e���ɂ��������ꍇ */
	SPRITE *tt; //�Ώ�
	if (NULL != (tt=sprite_collision_check(s1,SP_GROUP_ITEMS/*mask*/)))
	{
		PLAYER_DATA *pd = (PLAYER_DATA *)s1->data;
		switch (tt->type)
		{
		case SP_ITEM_00_P001:	player_add_power(s1, tt, 1);		break;	// �E�F�|���A�C�e��(��P)
		case SP_ITEM_01_P008:	player_add_power(s1, tt, 8);		break;	// �E�F�|���A�C�e��(��P)	// [***090123		�ǉ�
		case SP_ITEM_04_P128:	player_add_power(s1, tt, 127);		break;	// �E�F�|���A�C�e��(F)		// [***090123		�ǉ�
		//
		case SP_ITEM_02_BOMB:
			//player_add_bomb(t);		// [*****�{���̓R�����g�A�E�g���Ȃ���
			if (8 < pd->bombs)	{	goto add_1000pts;	}/* ���ɍő�l�Ȃ�΁A1000pts */
			pd->bombs++;
			#if 1/*�o�Ofix?*/
			tt->type = SP_DELETE;/* �����܂� */
			#endif
//
				#if (0==USE_DESIGN_TRACK)
				play_voice_auto_track(VOICE05_BONUS);
				#else
				voice_play(VOICE05_BONUS, TRACK07_GRAZE);/*�e�L�g�[*/
				#endif
			break;

		case SP_ITEM_03_1UP:
			if (8 < pd->zanki)	{	goto add_1000pts;	}/* ���ɍő�l�Ȃ�΁A1000pts */
			pd->zanki++;
			#if 1/*�o�Ofix?*/
			tt->type = SP_DELETE;/* �����܂� */
			#endif
			#if (0==USE_DESIGN_TRACK)
			/*effect_sound_number=*/play_voice_auto_track(VOICE06_EXTEND);
			#else
			voice_play(VOICE06_EXTEND, TRACK03_SHORT_MUSIC);/*�e�L�g�[*/
			#endif
			break;

		case SP_ITEM_07_SPECIAL:
			break;
		#if 0
		/* ���_�́A�����蔻��Ȃ� */
		case SP_ITEM_05_HOSI:		/*not_break;*/
			tt->type = SP_DELETE;/* ���_�̂ݓ��ʏ��� */
//
				#if (0==USE_DESIGN_TRACK)
				play_voice_auto_track(VOICE05_BONUS);
				#else
				voice_play(VOICE05_BONUS, TRACK07_GRAZE);/*�e�L�g�[*/
				#endif
			break;
		#endif
add_1000pts:
		case SP_ITEM_06_TENSU:
			{	/* SPLIT_LINE ����Ŏ��� 1000pts. ... ���Ŏ��Ɩ�100pts. */
				/* (���90pts�A���ɓ�����A����΂��(�����钼�O��3���C��)70pts�܂ŉ\�炵��) */
				int add_score_point;
				add_score_point = (SCORE_1000); 	/* ��{�_ 1000pts */
				if (SPLIT_LINE < s1->y256)	/* SPLIT_LINE �����̏ꍇ�́ASPLIT_LINE�܂ł̋����ɂ������Č��_ */
				{
					add_score_point -= ((tt->y256-SPLIT_LINE)>>(4+8));
				}
				//player_add_score(score(1000));		// [***090123		�ύX
				bonus_info_score_nodel(tt, add_score_point);/* */tt->type = SP_DELETE;/* �����܂� */
//
			}
				#if (0==USE_DESIGN_TRACK)
				play_voice_auto_track(VOICE05_BONUS);
				#else
				voice_play(VOICE05_BONUS, TRACK07_GRAZE);/*�e�L�g�[*/
				#endif
			break;
		}
	}
}


/*---------------------------------------------------------
	�v���C���[�ƓG�̂����蔻��3(�G��p)
	-------------------------------------------------------
		s1		�v���C���[
		tt		�G
---------------------------------------------------------*/

static void player_colision_check_enemy(SPRITE *s1, int player_hit_enemy_group_mask)
{
	/* ���@���G(�{�X/���^/�U�R)�ɑ̓����肳�ꂽ�ꍇ */
	/* --- �G --- */
	//	case SP_BOSS:		/* ���@���{�X�G�ɑ̓����肳�ꂽ�ꍇ */
	//	case SP_CHUU:		/* ���@�����^�G�ɑ̓����肳�ꂽ�ꍇ */
	//	case SP_ZAKO:		/* ���@���U�R�G�ɑ̓����肳�ꂽ�ꍇ */
	SPRITE *tt; //�Ώ�
	if (NULL != (tt=sprite_collision_check(s1, player_hit_enemy_group_mask/*SP_GROUP_ENEMYS*/)))	// �G�O���[�v�ɓ��������ꍇ
	{
		pd_player_status	= PLAYER_STATE_01_HIT_BOMB; 	// [***090125		�ǉ�
		pd_save_timer		= player_fix_status[BASE_HIT_BOMB_WAIT+select_player]/*0*/;
	//	pd->enemy			= t;/* ���������G���o���Ƃ� */
		/* ���������G�́A�K�����ʖ�ł͖����̂ŁA���������G�̗̑͂����炷�ׂ� */
		/**/
		#if (0==USE_DESIGN_TRACK)
		/*effect_sound_number=*/play_voice_auto_track(VOICE04_SHIP_HAKAI);		// [***090127	�ύX��
		#else
		voice_play(VOICE04_SHIP_HAKAI, TRACK03_SHORT_MUSIC/*TRACK01_EXPLODE*/);/* ���@���ɉ��́A�Ȃ�ׂ��d�˂Ȃ� */
		#endif
	}
}


/*---------------------------------------------------------
	�v���C���[�ƓG�e�̂����蔻��2(�O���C�Y��p)
	-------------------------------------------------------
		s1		�v���C���[
		tt		�G�e
---------------------------------------------------------*/
static void player_colision_check_graze(SPRITE *s1/*, int mask*/)
{
	/* --- �G�e --- */
	//	case SP_BULLET: 	/*not_break;*/
	//	case SP_LASER:		/*not_break;*/
	//	case SP_BIGBULLET:	/*not_break;*/
	//	case SP_BOSS02ICE:	/*not_break;*/
//
	SPRITE *tt; //�Ώ�
	if (NULL != (tt=sprite_collision_check(s1,SP_GROUP_BULLETS/*mask*/)))
	{
		/* �v���C���[�ɓG�e�����������ꍇ�̓O���C�Y���� */
		is_graze = 1;/*�O���C�Y��*/
		if (0==(tt->flags & SP_FLAG_GRAZE)) /* �O���C�Y��? */
		{
			tt->flags |= SP_FLAG_GRAZE;/*�O���C�Y��*/
			((PLAYER_DATA *)s1->data)->graze_point++;/*�O���C�Y��*/
			player_add_score((score(100)+score(200)*difficulty));/*score(500)*/
			#if (0==USE_DESIGN_TRACK)
			/*effect_sound_number=*/play_voice_auto_track(VOICE09_GRAZE);
			#else
			voice_play(VOICE09_GRAZE, TRACK07_GRAZE);/*�e�L�g�[*/
			#endif
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
	SPRITE *s1;
	s1 = player;
	PLAYER_DATA *pd = (PLAYER_DATA *)s1->data;
	pd->state_flag		|= STATE_FLAG_14_GAME_LOOP_QUIT;
	#if (0000)
	pd->state_flag		&= (~STATE_FLAG_08_OPTION_HIDE);	/* �I�v�V���������� */
	#endif/*0000*/
//
	pd_save_timer		= 150;	/* 150	 120�ł͎኱�Z�� 100 */
	pd_player_status	= PLAYER_STATE_05_GAME_OUT;/* GAME_OUT�� */
	s1->flags			&= (~(SP_FLAG_VISIBLE));		// ���t���O��OFF(�s��)
	pd->core->flags 	&= (~(SP_FLAG_VISIBLE));		// ������// ��������
//	pd->core->alpha 	= 0x00; 		// �����������߂ɓ��ꂽ���ǈӖ�����������������Ȃ��B	// �Q�[���I�[�o�[���́��̕\������
//
}

/*---------------------------------------------------------
	�v���C���[��������
---------------------------------------------------------*/

static void player_fukkatsu(SPRITE *s1)
{
	pd_save_timer		= 40/*120*/ /*150-120*/;									// ���G����
	pd_player_status	= PLAYER_STATE_03_SAVE_01;	// ���G��ԁH

	s1->flags			|= (SP_FLAG_VISIBLE);		// ���t���O��ON(��)
	s1->color32 		= 0x50ffffff;		/*	s1->alpha			= 0x50;*/	/*������*/
	s1->anim_frame		= 5;
//
	PLAYER_DATA *pd = (PLAYER_DATA *)s1->data;
	pd->core->flags 	|= (SP_FLAG_VISIBLE);		// ���\��
	pd->core->color32	= 0x50ffffff;		/*	pd->core->alpha 	= 0x50;*/	/*������*/
	pd->bombs			= player_fix_status[BASE_BOMBS+select_player]/*3*/; 		// �{���������̏�����
	pd->chain_point 	= 0;	// pd->chain_point(�m�[�~�X�{�[�i�X)�̏����� // �ǂꂾ���A��(�m�[�~�X)��wepon_up���������
//
	check_weapon_level(pd);/* [pd->weapon_power���ύX���ꂽ�ꍇ�ɕK���s����`�F�b�N] */
}

/*---------------------------------------------------------
	�v���C���[���ꏈ��
---------------------------------------------------------*/

static void player_explode(SPRITE *s1)
{
	PLAYER_DATA *pd = (PLAYER_DATA *)s1->data;
	/* �A�C�e����f���o�� */	//	[***090124������ƒǉ�
	{
		if ((0==pd->zanki)&&(0==difficulty))
		{
			/* �R���e�B�j���[�̏ꍇ(GAME_OUT)easy �̏ꍇ */
			/* ���ʂ�8�f������  */
			/* �R���e�B�j���[�̏ꍇeasy �̏ꍇ�F (�L���b�`�o���鐔�œ_���͈Ⴄ���ǁA�ǂꂩ�P�L���b�`����� POWER �͓���������) */
		//	item_create(s1, (0==difficulty)?(SP_ITEM_04_P128):(SP_ITEM_01_P008), 8, ITEM_MOVE_FLAG_06_RAND_XY);
			item_create(s1, (SP_ITEM_04_P128), 8, ITEM_MOVE_FLAG_06_RAND_XY);
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
			item_create(s1, (((30-1) > pd->weapon_power)?(SP_ITEM_01_P008):(SP_ITEM_00_P001)), 7/*5*/, ITEM_MOVE_FLAG_06_RAND_XY);
			item_create(s1, SP_ITEM_01_P008, 1, ITEM_MOVE_FLAG_06_RAND_XY);
		}
		//
		/* ���~���A ����\�́F������������ƕK���{�����o�� */
		if (REMILIA==select_player) 	/* ���~���A�̏ꍇ */
		{
			/* ������������� */
			if (0 != pd->bombs)
			{
				/* �g���ĂȂ����̎����Ă�{����f���o�� */
				item_create(s1, SP_ITEM_02_BOMB, pd->bombs, ITEM_MOVE_FLAG_06_RAND_XY);
			}
		}
	}
//		pd->explode 		= 0;
//	if (0==pd->explode)
	{
	//	spimg=sprite_getcurrimg(t); parsys_add(spimg, 2,2, t->x,t->y, 10, 270, 10, 30, EXPLODE|DIFFSIZE, NULL); 		SDL_FreeSurface(spimg);spimg = NULL;
	//	spimg=sprite_getcurrimg(s1);parsys_add(spimg, 2,2, t->x,t->y, 10,  90, 10, 30, EXPLODE|DIFFSIZE, &pd->explode); SDL_FreeSurface(spimg);spimg = NULL;

	//	bakuhatsu_add_type(t->x256+t256(5),t->y256+t256(5),/*0,*/BAKUHATSU_MINI00);
	//	t->type = SP_DELETE;/* �����܂� */			/* ���������G���� */
	//	pd->explode=0;
	//	pd->bonus=0;

		//play_voice_auto_track(VOICE04_SHIP_HAKAI);	// [***090127	�ꏊ���ړ�����B
		/* �����G�t�F�N�g */

		#if 0/* ���������G�͓|���Ȃ��G��������Ȃ��̂ŁA�����ŕK�������̂͂܂����C������ */
		/* ���������G���� */
		bakuhatsu_add_type(pd->enemy->x256+t256( 5), pd->enemy->y256+t256( 5), /*0,*/ BAKUHATSU_MINI00);
		pd->enemy->type 	= SP_DELETE;/*???*/
		#endif
		/* �������� */
		#if 0
		dummy_obj->x256 = s1->x256+t256( 5);						dummy_obj->y256 = s1->y256+t256( 5);						bakuhatsu_add_type_ddd(dummy_obj/*s1->x256+t256( 5), s1->y256+t256( 5)*/, /*0,*/ BAKUHATSU_MINI00);
	/*	dummy_obj->x256 = s1->x256+t256( 5);	*/					dummy_obj->y256 += t256(20-5)/*= s1->y256+t256(20);*/		bakuhatsu_add_type_ddd(dummy_obj/*s1->x256+t256( 5), s1->y256+t256(20)*/, /*0,*/ BAKUHATSU_MINI00);
		dummy_obj->x256 += t256(20-5)/*= s1->x256+t256(20)*/;	/*	dummy_obj->y256 = s1->y256+t256(20);	*/					bakuhatsu_add_type_ddd(dummy_obj/*s1->x256+t256(20), s1->y256+t256( 5)*/, /*0,*/ BAKUHATSU_MINI00);
	/*	dummy_obj->x256 = s1->x256+t256(20);	*/					dummy_obj->y256 -= t256(20-5)/*= s1->y256+t256( 5)*/;		bakuhatsu_add_type_ddd(dummy_obj/*s1->x256+t256(20), s1->y256+t256(20)*/, /*0,*/ BAKUHATSU_MINI00);
		#endif
		#if 0
		dummy_obj->x256 = s1->x256+t256( 5);	dummy_obj->y256 = s1->y256+t256( 5);		bakuhatsu_add_type_ddd(dummy_obj/*s1->x256+t256( 5), s1->y256+t256( 5)*/, /*0,*/ BAKUHATSU_MINI00);
		dummy_obj->x256 = s1->x256+t256( 5);	dummy_obj->y256 = s1->y256+t256(20);		bakuhatsu_add_type_ddd(dummy_obj/*s1->x256+t256( 5), s1->y256+t256(20)*/, /*0,*/ BAKUHATSU_MINI00);
		dummy_obj->x256 = s1->x256+t256(20);	dummy_obj->y256 = s1->y256+t256(20);		bakuhatsu_add_type_ddd(dummy_obj/*s1->x256+t256(20), s1->y256+t256( 5)*/, /*0,*/ BAKUHATSU_MINI00);
		dummy_obj->x256 = s1->x256+t256(20);	dummy_obj->y256 = s1->y256+t256( 5);		bakuhatsu_add_type_ddd(dummy_obj/*s1->x256+t256(20), s1->y256+t256(20)*/, /*0,*/ BAKUHATSU_MINI00);
		#endif
		#if 1
		{int jj;
			u8 jjaaa[8] =
			{/* x y */
				( 5),( 5),
				( 5),(20),
				(20),( 5),
				(20),(20),
			};
			for (jj=0; jj<(4); jj+=2)
			{
				dummy_obj->x256 = s1->x256+((jjaaa[jj  ])<<8);
				dummy_obj->y256 = s1->y256+((jjaaa[jj+1])<<8);
				bakuhatsu_add_type_ddd(dummy_obj/*s1->x256+t256( 5), s1->y256+t256( 5)*/, /*0,*/ BAKUHATSU_MINI00);
			}
		}
		#endif
		/* �v���C���[�ʒu�̏����� */
		s1->x256 = t256(GAME_WIDTH/2)-((s1->w128));
		s1->y256 = t256(220/*GAME_HEIGHT*/);/*240*/
		/*	*/
	//	pd->ex tra_type 	= PLX_NONE; �p�~
		pd_bomber_time	= 0;			/* �����������������Ŗ�����(&�`���m)�{�����ĂȂ��Ȃ��Ă� */
	//	pd->state_flag		= STATE_FLAG_00_NONE;/*???*/
		pd->state_flag		&= (~(	STATE_FLAG_01_PLAYER_UP_AUTO_GET_ITEM | 	/* �I��� */
									STATE_FLAG_02_BOMB_AUTO_GET_ITEM |			/* �I��� */
									STATE_FLAG_03_SCORE_AUTO_GET_ITEM));		/* �I��� */

		/* ���݃I�v�V�������������āA�㔼�d���Ȃ�o�O������B */
		#if (0000)
		pd->state_flag		&= (~STATE_FLAG_08_OPTION_HIDE);	/* �I�v�V���������� */
		#endif/*0000*/
		/* �c�`�����X���炷 */
		pd->zanki--;
		if (0 > pd->zanki)	/* �c�`�����X���Ȃ���� */
		{
			player_loop_quit();
		}
		else
		{
			//
		//	if (pd->player_speed > player_fix_status[BASE_SPEED_MINIMUM+select_player]/*pd->player_speed_minimum*/) 			//player_speed_minimum�ȉ��ɂȂ�Ȃ��悤��
		//	{	pd->player_speed--;}
			pd->weapon_power -= (difficulty*4)+12;	/* ���탌�x���̒ቺ */
			if ( pd->weapon_power<0)	{	pd->weapon_power=0;}
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
		PLAYER_DATA *pd = (PLAYER_DATA *)s1->data;
		switch (pd_player_status)
		{
		case PLAYER_STATE_01_HIT_BOMB:/* ��炢�{����t�� */			// [***090125		�ǉ�
			/* ���S�m�� */
				pd->state_flag |= STATE_FLAG_16_NOT_KEY_CONTROL;		/* �L�[���͖���(1) */
			player_explode(s1);/*�v���C���[����*/
			break;

		case PLAYER_STATE_03_SAVE_01:/*�v���C���[�������P(��ʊO���畜���n�_�֓_�ł��Ȃ���ړ���)*/
		//������Ƃ����҂�����
		//	if (s1->y256 < t256(220)/*240*/)	//������ƑO�ɏo�Ă���
			{
				pd->state_flag		&= (~STATE_FLAG_16_NOT_KEY_CONTROL);	/* �L�[���͗L��(0) */
				pd_save_timer		= 120;
				pd_player_status	= PLAYER_STATE_04_SAVE_02;
			}
		//	else
		//	{
		//		s1->y256 -= t256(1)/*fps_fa_ctor*/;
		//	}
			break;/*??? [***090210 �ǉ� */

		case PLAYER_STATE_04_SAVE_02:/*�v���C���[�������Q*/
			pd_player_status	= PLAYER_STATE_00_NORMAL;
			s1->color32 		= 0xffffffff;		/*	s1->alpha			= 0xff;*/
			pd->core->color32	= 0xffffffff;		/*	pd->core->alpha 	= 0xff;*/
			break;

		case PLAYER_STATE_05_GAME_OUT:/* GAME_OUT�� */
			now_max_continue--;
			if (0 < now_max_continue)
			{
			//	render_continue(/*now_max_continue*/);
				psp_loop=(ST_INIT_MENU|ST_MENU_SUB_ASK_CONTINUE);//newsta te(ST_GAME_OVER,0,1);
			}
			else
			{
				psp_loop=(ST_WORK_GAME_OVER|0);//newsta te(ST_GAME_OVER,0,1);
			}
			break;
		}
	}
}


/*---------------------------------------------------------
	�v���C���[�ړ��A���C������
---------------------------------------------------------*/
static void player_move(SPRITE *s1)
{
	PLAYER_DATA *pd = (PLAYER_DATA *)s1->data;
	pd->state_flag &= (~STATE_FLAG_15_KEY_SHOT);	/* off */
	if (0==(pd->state_flag & STATE_FLAG_16_NOT_KEY_CONTROL))	/* �L�[���͗L��(0) */
	{
		player_keycontrol(s1);
	}
	int player_hit_enemy_group;
	//player_hit_enemy_group = SP_GROUP_ZAKO;
	//player_hit_enemy_group = SP_GROUP_BOSS;
	if (0 != pd_save_timer)/* ���ꏈ�� */
	{
		player_move_other(s1);
		player_hit_enemy_group = (SP_GROUP_BOSS);	/* �{�����́A�{�X / ���U�R�G �ɂ������Ď��� */
	}
	else		/*�ʏ폈��*/
	{
//	case PLAYER_STATE_00_NORMAL:
		player_colision_check_graze(s1/*,SP_GROUP_BULLETS*/);
		player_hit_enemy_group = (SP_GROUP_BOSS|SP_GROUP_ZAKO);
	}
	/* �{�����́A�{�X / ���U�R�G �ɂ������Ď��� */
	/* �X�N���v�g���쒆 �́A�G�����蔻��͂Ȃ� */
	/* �L�[���͖�����(==������) �́A�G�����蔻��͂Ȃ� */
	if (0==(pd->state_flag & (STATE_FLAG_06_IS_SCRIPT|STATE_FLAG_16_NOT_KEY_CONTROL)))
	{
		/* ��炢�{����t�� �́A�G�����蔻��͂Ȃ� */
		if (PLAYER_STATE_01_HIT_BOMB != pd_player_status)
		{
			player_colision_check_enemy(s1, player_hit_enemy_group);/*SP_GROUP_ENEMYS*/
		}
	}
	/* �A�C�e���͏�Ɏ��� */
	player_colision_check_item(s1/*,SP_GROUP_ITEMS*/);
}


/*---------------------------------------------------------
	�v���C���[������()
---------------------------------------------------------*/
void player_continue_value(void)
{
	PLAYER_DATA *pd = (PLAYER_DATA *)player->data;

	pd->zanki				= /*pd_base_zanki*/ player_fix_status[BASE_LIVES+select_player];
	pd->my_score			= score(0);

	player_fukkatsu(player);
	pd->state_flag			&= (~STATE_FLAG_14_GAME_LOOP_QUIT); 	/* ���A */
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
	//	s						= sprite_add_res( BASE_OPTION1_PNG+select_player ); 	/*+ REIMU MARISA YUYUKO */
		s						= sprite_add_bullet( TAMA_TYPE_BULLET_DUMMY );	/*+ REIMU MARISA YUYUKO */
		option[(OPTION_C1+jj)]	= s;
	//	s->flags				|= (SP_FLAG_VISIBLE);
		s->flags				&= (~(SP_FLAG_VISIBLE));			/* ���t���O��OFF(�s��) */
		s->flags				&= (~(SP_FLAG_COLISION_CHECK)); 	/* �����蔻���OFF(���G) */

		s->type 				= (JIKI_OPTION_00_00/*|SP_MUTEKI*/);
		s->anim_frame			= 0;
	//
		s->callback_mover		= re_ma_yu_move_option;
		data					= mmalloc(sizeof(PL_OPTION_DATA));
		s->data 				= data;
		/*data->*/s->base_weapon_strength	= 5;/* [***090214 �ǉ� */
	//	data->length			= 11;
		s->x256 				= src->x256+((src->w128-s->w128))-t256(11)/*data->length*/;
		s->y256 				= src->y256+((src->h128-s->h128));
		data->anime_wait		= 0;
	//	data->anime_speed		= player_fix_status[BASE_OPT_SHOT_ANIME+select_player];/*12*/ /*2*/ /*�A�z���܂͂���������*/
		int iii_tbl[4] =
		{
		/*data->opt_anime_add_id*/ /*pos*/		/*=*/ OPTION_ID_02_LM, /*-1*/ /*;*/ /*0*/
		/*data->opt_anime_add_id*/ /*pos*/		/*=*/ OPTION_ID_03_RM, /* 1*/ /*;*/
		/*data->opt_anime_add_id*/ /*pos*/		/*=*/ OPTION_ID_01_LL, /*-1*/ /*;*/ /*0*/
		/*data->opt_anime_add_id*/ /*pos*/		/*=*/ OPTION_ID_04_RR  /* 1*/ /*;*/
		};
		data->opt_anime_add_id/*pos*/			= iii_tbl[jj]/*-1*/;/*0*/
		int ccc_tbl[4] =
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
	//	s						= sprite_add_res( BASE_OPTION1_PNG+select_player/*REMILIA*/+kk );	kk+=PLAYERS5;
		s						= sprite_add_bullet( TAMA_TYPE_BULLET_DUMMY );
		option[(OPTION_C1+jj)]	= s;
	//	s->flags				|= (SP_FLAG_VISIBLE);
		s->flags				&= (~(SP_FLAG_VISIBLE));		/* ���t���O��OFF(�s��) */
		s->flags				&= (~(SP_FLAG_COLISION_CHECK)); 	/* �����蔻���OFF(���G) */

		s->type 				= (JIKI_OPTION_00_00/*|SP_MUTEKI*/)+kk; kk+=8/*PLAYERS5*/;
		s->anim_frame			= 0;
	//
		s->callback_mover		= oz_ci_move_option;
		data					= mmalloc(sizeof(REMILIA_OPTION_DATA));
		s->data 				= data;
		int aaa_tbl[4] =
		{
		/*data->REMILIA_angle512=*/ deg_360_to_512CCW(360-(45*5)), /*deg_360_to_512(45*1)*/ /*M_PI*2*1/8*/ /*;*/ /*�`���m*/
		/*data->REMILIA_angle512=*/ deg_360_to_512CCW(360-(45*7)), /*deg_360_to_512(45*5)*/ /*M_PI*2*5/8*/ /*;*/ /*�`���m*/
		/*data->REMILIA_angle512=*/ deg_360_to_512CCW(360-(45*1)), /*deg_360_to_512(45*3)*/ /*M_PI*2*3/8*/ /*;*/ /*�`���m*/
		/*data->REMILIA_angle512=*/ deg_360_to_512CCW(360-(45*3))  /*deg_360_to_512(45*7)*/ /*M_PI*2*7/8*/ /*;*/ /*�`���m*/
		};
		data->REMILIA_angle512		= aaa_tbl[jj];
	//	data->next					= player;
	//	data->state1				= 0;///
		data->state2				= 0;
		data->offset_x256			= t256(0);
		data->offset_y256			= t256(0);
		/*data->*/s->base_weapon_strength	= 5;
		data->anime_wait			= 0;
	//	data->anime_speed			= 3;
		data->opt_shot_interval 	= 0;
		data->opt_anime_add_id		= (OPTION_C1+jj);
	//	data->ccc_angle512			= deg_360_to_512((0));
		data->slow_count			= 0;
		s->x256 					= src->x256+((src->w128-s->w128));
		s->y256 					= src->y256+((src->h128-s->h128))+t256(20);
	}
}

/*---------------------------------------------------------
	�v���C���[�A��������p�R�A�̒ǉ�
	���̒ǉ�
---------------------------------------------------------*/

static SPRITE *player_add_core(SPRITE *s1)
{
	SPRITE *s2;
	s2						= NULL;
	s2						= sprite_add_res( BASE_CORE_PNG+select_player );
	s2->anim_speed			= 0;
	s2->type				= SP_PLAYER_CORE;
	#if 0
	/* �Ƃ肠�����A���Ή��B�����I�ɐݒ肵���Ⴄ */
	s2->w128				= (t256(32/2));
	s2->h128				= (t256(16/2));
	#endif
	s2->x256				= (s1->x256)+((s1->w128-s2->w128));
	s2->y256				= (s1->y256)-((s2->h128+s2->h128))+t256(20);
	s2->callback_mover		= player_move_core;
	s2->flags				|= (SP_FLAG_VISIBLE);
	return (s2);
}

/*---------------------------------------------------------
	�v���C���[������
---------------------------------------------------------*/
extern int practice_mode;
extern void select_jiki_load_surface(void);
void player_init(void)
{
	select_jiki_load_surface();
//	player					= sprite_add_res( BASE_SHIP_MED_PNG+select_player );		/* [***090220 �ǉ� */
	player					= sprite_add_bullet( JIKI_ATARI_ITEM_16 );		/* [***090220 �ǉ� */
	player->type			= (SP_GROUP_JIKI_GET_ITEM);
	player->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
	player->callback_mover	= player_move;
	#if 1
	/* �Ƃ肠�����A���Ή��B�����I�ɐݒ肵���Ⴄ */
	player->w128			= (t256(32/2));
	player->h128			= (t256(16/2));
	#endif
	player->x256			= (t256(GAME_WIDTH/2));
	player->y256			= (t256(GAME_HEIGHT))-((player->h128+player->h128));
//	player->anim_frame		= 5;
	PLAYER_DATA *pd;
	pd						= mmalloc(sizeof(PLAYER_DATA));
	player->data			= pd;

	pd_bomber_time			= 0;	/*==bomb_wait*/
	pd->graze_point 		= 0;
	pd->zanki				= /*pd_base_zanki*/ player_fix_status[BASE_LIVES+select_player];
	pd->my_score			= score(0);
	pd->state_flag			= STATE_FLAG_00_NONE;
//	if (MARISA==select_player)		{	pd->state_flag		|= STATE_FLAG_04_IS_MARISA; 	}	/* �������͏�Ɏ������W */
	pd->boss				= NULL;
	pd->core				= player_add_core(player);		/* ���̒ǉ� */

	weapon_List 			= 0;
	/* ���K���[�h�̏ꍇ�̓t���p���[�Ŏn�߂�(���̑���N���A��̃C�x���g������Ȃ�) */
	pd->weapon_power		= (0==practice_mode)?(0):(127);
//
	/* �I�v�V������ǉ� */
	switch (select_player)
	{
	case YUYUKO:/*�Ƃ肠����*/
	case REIMU:/**/
	case MARISA:		option_create_re_ma_yu(player); 	break;
	case CIRNO:/*�Ƃ肠����*/
	case REMILIA:		option_create_oz_ci(player);		break;/* [***090220 �ǉ� */
	}
//
	player_fukkatsu(player);/* �I�v�V������ǉ����� */
	now_max_continue		= /*3*/DEFAULT_MAX_CONTINUE;
}

/*---------------------------------------------------------
	�G����
	�{�X�|�����ꍇ�̏���
	(�v���C���[���܂�)2018833
	-------------------------------------------------------
???��R�����g???	�{�X��|�����炷���Ă΂��(�v���C���[�𖳓G�ɂ����)
???��R�����g???	(�t���O�͎d�l�㎞�ԑ҂�������ׁA���݂��̗p�r�ɂ͎g���Ȃ�)
---------------------------------------------------------*/

/*static*/ void lose_boss(SPRITE *src)
{
	bakuhatsu_add_circle(src, 1);	/* pl ayer_set_destoroy_boss(); B02_BOSS_DESTROY ���K�v */
//
	{
		PLAYER_DATA *pd 	= (PLAYER_DATA *)player->data;
	//	pd->bo ssmode=B02_BOSS_DESTROY;
		pd->state_flag |= STATE_FLAG_11_IS_BOSS_DESTROY;
		#if 1
		/* �o�O�ŃA�C�e���o�Ȃ�(???) */
	//	if (pd->state_flag&STATE_FLAG_09_IS_WIN_BOSS)		/* �U���œ|�����ꍇ�̂� */
		#endif
		{
			pd->state_flag		&= (~STATE_FLAG_09_IS_WIN_BOSS);	/* �I��� */
			/* �{�[�i�X�A�C�e�����o�� */
// ��� 	item_create(src, SP_ITEM_05_HOSI/*SP_ITEM_06_TENSU*/, 15, ITEM_MOVE_FLAG_01_COLLECT);/*���_���o��*/
// �P�� 	item_create_for_boss(src, ITEM_CREATE_MODE_01);// �P�� �� �p�`�F �A���X
			item_create_for_boss(src, ITEM_CREATE_MODE_01);
		//
			#if 1
			/* �{�X�̓��_���Z */
			player_add_score(/*data->boss_base.*/src->base_score);/* �P��̃X�R�A */
			#endif
		}
	//	else
	//	{
	//		;/* ���Ԑ؂�̏ꍇ�̓{�[�i�X�A�C�e���Ɠ��_�Ȃ��B */
	//	}
	}
	//		data->boss_base.boss_health = 0;/* �Ȃ񂩃o�O��̂Œǉ� */
	/* �R�[���o�b�N�o�^ */
	src->callback_hit_enemy 	= NULL; 	/* �_�~�[�R�[���o�b�N�o�^ */
	src->callback_mover 		= NULL;
	src->type					= SP_DELETE;
//
	/* �P���|���ƊF�j�󂳂��B */
	/* �A���X��|���ƊF�j�󂳂��B */
	#if 1
	common_boss_flags		= 0/*FLG_ALL_CAST*/;
	#endif
//
	#if (0)
	pd_save_timer		= /*pd_bomber_time*/255 + USER_BOMOUT_WAIT/*(30)*/;/*�������G����*/
	#else
	pd_save_timer		= (6);/* 6[�t���[��] �{�X�|���Ă��玟(�V�i���I)�ɐi�ނ܂ł̑҂����� */
	#endif
//
	pd_bomber_time		= 0;
	#if 1
	draw_boss_hp_value	= 0;/* �悭�킩��Ȃ� */
	#endif
//	pd->ex tra_type 	= PLX_BOMB;
//	s1->alpha			= 80;	/*������*/
//	pd->core->alpha 	= 80;	/*������*/
	pd_player_status	= PLAYER_STATE_04_SAVE_02;
}

