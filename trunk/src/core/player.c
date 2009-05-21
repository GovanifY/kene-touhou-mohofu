
/*---------------------------------------------------------
		�e�v���C���[
---------------------------------------------------------*/

#include "support.h"
#include "player.h"

extern int select_player;


int player_now_stage;		//
SPRITE *player;

enum /*_player_state_*/
{
	PLAYER_STATE_00_NORMAL,
	PLAYER_STATE_01_HIT_BOMB,		// [***090125		�ǉ�:PLAYER_STATE_01_HIT_BOMB
	PLAYER_STATE_03_SAVE_01,
	PLAYER_STATE_04_SAVE_02,
	PLAYER_STATE_05_GAME_OUT,
};

//typedef struct
//{
//	int strength;/*union WEAPON_BASE*/
//	int angle512;
//	/*dou ble*/int speed256;
//} PLAYER_BASE_STRUCT;


typedef struct
{
	int strength;/*union WEAPON_BASE*/
	/*dou ble*/int/*int*/ vx256;	/* vector x */
	/*dou ble*/int/*int*/ vy256;	/* vector y */
} PL_SHOT_DATA;

#define PL_LEVARIE_DATA    PL_SHOT_DATA/*PLAYER_BASE_STRUCT*/
#define OZ_BURN_FIRE_DATA  PL_SHOT_DATA/*PLAYER_BASE_STRUCT*/

/* �\�����{�� �̋����ׂ̈����ɕK�v */
#define PL_CROSS_FIRE_DATA	/*PL_SHOT_DATA*/WEAPON_BASE/*PLAYER_BASE_STRUCT*/

/*
���ӁF�\�����B�ʂ̖��̂ŌĂт����Ȃ邩���m��܂��񂪁A�^�@���c�̂����邳���̂ŁA���̖��̂ł��ł��������B
�������A�Ȃ�ēǂނ񂾂�H
*/


#if (1==USE_PLAYER_BOMBER)
#define PL_BOMB_DATA	  PLAYER_BASE_STRUCT
#endif //(1==USE_PLAYER_BOMBER)

#if (1==USE_PLAYER_WAVE)
typedef struct
{
	int strength;/*union WEAPON_BASE*/
	int angle512;
	/*dou ble*/int speed256;
	dou ble distance;	/*����(distance)*/
} PL_KILLRAY_DATA;
#endif //(1==USE_PLAYER_WAVE)

typedef struct
{
	int strength;/*union WEAPON_BASE*/
	/*dou ble*/int angle512;
	/*dou ble*/int speed256;
//
	int nr;
	int state;
	int time_out;	/*dou ble range;*/		/* �ݒ莞�ԓ��Ɍ������Ȃ��ꍇ�͂����܂� */
	SPRITE *target_obj/*int target_id*/;
//	SPRITE *tg_sprite;/* �^�[�Q�b�g�ڕW�\���}�[�J�[ */	/*PL_HOMING_DATA�̂�*/
} PL_HOMING_DATA;/*==PL_HLASER_DATA*/

typedef struct
{
	int strength;/*union WEAPON_BASE*/
//	/*dou ble*/int angle512;
//	/*dou ble*/int speed256;
//
//	int nr;
//	int state;
	int check_x256;
	int check_y256;

	int time_out;	/*dou ble range;*/		/* �ݒ莞�ԓ��Ɍ������Ȃ��ꍇ�͂����܂� */
	SPRITE *target_obj/*int target_id*/;
//	SPRITE *tg_sprite;/* �^�[�Q�b�g�ڕW�\���}�[�J�[ */	/*PL_HOMING_DATA�̂�*/
} PL_HOMING_KODOMO_DATA;/*==PL_HLASER_DATA*/

typedef struct
{
	int strength;/*union WEAPON_BASE*/	/* [***090214 �ǉ� */
	/*dou ble*/int angle512;
	/*dou ble*/int add_r512;
	int radius; 	/*���a(radius)*/
} PL_KEKKAI_DATA;

typedef struct
{
	int strength;/*union WEAPON_BASE*/	/* [***090214 �ǉ� */
	/*dou ble*/int angleL512;		/* �����O�Ƃ��č����(�t���)�̊p�x */
	int anime_wait; 				/* �A�j���[�V�������x�̃J�E���^ */
	int opt_anime_add_id/*pos*/;	/* �A�j���[�V���������̉��Z�l / �I�v�V����ID */
	int opt_shot_interval;			/* �I�v�V���������˂���V���b�g�̍X�V�Ԋu */
//
//	int length;
//	int state;
//	int anime_speed;				/* �p�~ */	/* �A�j���[�V�������x�̐ݒ�l */

} PL_OPTION_DATA;

//efine OPTION_ID_01_LL 	(0x01ff)/*��-1 0  0001 (0x01ff) 0001 1111 1111 */
//efine OPTION_ID_02_LM 	(0x04ff)/*��-1 0  0100 (0x02ff) 0010 1111 1111 */
//efine OPTION_ID_03_RM 	(0x0801)/*��+1 1  1000 (0x0401) 0100 0000 0001 */
//efine OPTION_ID_04_RR 	(0x0d01)/*��+1 1  1101 (0x0701) 0111 0000 0001 */

//efine OPTION_ID_01_LL 	(0x05ff)/*��-1 0  4 0101 (0x01ff) 0001 1111 1111 */
//efine OPTION_ID_02_LM 	(0x04ff)/*��-1 0  4 0100 (0x02ff) 0010 1111 1111 */
//efine OPTION_ID_03_RM 	(0x0c01)/*��+1 1  c 1100 (0x0401) 0100 0000 0001 */
//efine OPTION_ID_04_RR 	(0x0d01)/*��+1 1  c 1101 (0x0701) 0111 0000 0001 */

#define OPTION_ID_01_LL 	(0x0dff)/*��-1 0  c 1101 (0x01ff) 0001 1111 1111 */
#define OPTION_ID_02_LM 	(0x0cff)/*��-1 0  c 1100 (0x02ff) 0010 1111 1111 */
#define OPTION_ID_03_RM 	(0x0401)/*��+1 1  4 0100 (0x0401) 0100 0000 0001 */
#define OPTION_ID_04_RR 	(0x0501)/*��+1 1  4 0101 (0x0701) 0111 0000 0001 */

typedef struct
{
	int strength;/*union WEAPON_BASE*/	/* [***090214 �ǉ� */
	/*dou ble*/int angle512;
	int anime_wait; 				/* �A�j���[�V�������x�̃J�E���^ */
	int opt_anime_add_id;			/* �A�j���[�V���������̉��Z�l / �I�v�V����ID / x offset */
	int opt_shot_interval;			/* �I�v�V���������˂���V���b�g�̍X�V�Ԋu */
//
//	int state1;//���ʂȂ̂ŁA�����ɂ͗v��Ȃ�
	int state2;
	int offset_x256;		/* �~�̔��a�ƌ��p */
	int offset_y256;
//	/*dou ble*/int f_angle512;	/* ���~�̉������ꍇ�́A��]�p�x */	/* FORMATION_01: ���~�̎�������� */

	int slow_count; 			/*	*/				/* FORMATION_03: �������p(�D���Ȉʒu�ɒu�����) */
//	/*dou ble*/int m_angle512;	/* �p�~ */
//	struct _sprite *next;
//	int anime_speed;				/* �p�~ */	/* �A�j���[�V�������x�̐ݒ�l */
} REMILIA_OPTION_DATA;

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
#define BASE_BOMBS				(PLAYERS5*6)
#define BASE_LIVES				(PLAYERS5*7)
#define BASE_MAX				(PLAYERS5*8)		/* �ő吔 */
static Uint8 player_fix_status[BASE_MAX] =
{/* REIMU MARISA REMILIA CIRNO YUYUKO */
	 8,  2,  3,  1,  4, 	/* �v���C���[�̃A�j���[�V�������x */
	16,  8,  4,  9, 12, 	/* ��炢�{���̎�t���� hit_bomb_wait */
	 4,  8,  9,  9, 16, 	/* �I�v�V�����V���b�g�̍X�V�Ԋu option shot interval */
	12,  6,  3,  3, 12, 	/* �I�v�V�����V���b�g�̃A�j���[�V�������x */
//
	 1,  3,  3,  3,  3, 	/* �ʏ�{���̋��� standard bomb strength */
	10, 16,  4,  4,  8, 	/* �ᑬ�{���̋��� lower bomb strength */
	 3,  3,  3,  3,  4, 	/* �������̃{���� [�����{����](option���[�������疳���Ȃ邩��) */
	 4,  4,  3,  9,  5, 	/* �J�n���̎c��`�����X [�����v���C���[��](option���[�������疳���Ȃ邩��) */
};

//	 3,  2,  1,  9,  9, 	/* �������̃{���� */

//	 3,  4,  4,  9,  9, 	/* �Œᑬ�x player_speed_minimum */
//	 6,  8,  7,  9,  9, 	/* �ō����x player_speed_maximum */
//	 3,  4,  4,  9,  9, 	/* �Œᑬ�x player_speed_minimum */
//	 6,  8,  4,  9,  9, 	/* �ō����x player_speed_maximum */

/*---------------------------------------------------------
	�v���C���[�A��������p�R�A�A��������
---------------------------------------------------------*/

static int is_graze;	// player1���e���ɐG�ꂽ���H(�O���C�Y�����ۂ�)
static int pd_player_status;
static int pd_save_timer;

static void player_move_core(SPRITE *s2)
{
	s2->x256 = player->x256+((player->w-s2->w)<<7) -(t256(1));
	s2->y256 = player->y256-((s2->h)<<8)		   +(t256(20));
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
				play_voice_auto_track(VOICE04_SHIP_HAKAI);		// [***090127	�ύX��
				pd_player_status = PLAYER_STATE_01_HIT_BOMB;
				pd_save_timer		= player_fix_status[BASE_HIT_BOMB_WAIT+select_player]/*0*/;
			//	((PLAYER_DATA *)player->data)->enemy = t;/* ���������G���o���Ƃ� */
				/* ���������G�́A�K�����ʖ�ł͖����̂ŁA���������G�̗̑͂����炷�ׂ� */
				/**/
			}
		}
	}
	is_graze=0;/* �O���C�Y������� */
}

		/* �K�v�̂Ȃ�����e�̎�ނ𑝂₳�Ȃ��ŉ������B�x���Ȃ�܂��B */
		/* ����ł͕\���v���C�I���e�B���炢�����K�v�ȃP�[�X�������̂ŁB */
//		switch (t->type)	//�e�e�̏����B�e�̎�ނ�ǉ������ꍇ�B�����ɂ��ǉ�����K�v����B
//		{
//		case SP_BULLET: 			/*not_break;*/
	//	case SP_LASER:				/*not_break;*/
	//	case SP_BIGBULLET:			/*not_break;*/
	//	case SP_BOSS02ICE:			/*not_break;*/

//			break;
	//	case S P_EN_BIGBULLET_DUMMY:				break;
//		}

/*---------------------------------------------------------
	�v���C���[����̂����蔻��
---------------------------------------------------------*/

static void hit_work_zako(SPRITE *player_tama, SPRITE *s)
{
	ENEMY_BASE *zako_data;
	zako_data	= (ENEMY_BASE *)s->data;
	/* �ɂƂ�o�O�킩���̂ŁA�Ȃ������Ȓl�̏ꍇ���Ȃ� */
	WEAPON_BASE *player_tama_data = (WEAPON_BASE *)player_tama->data;
	if (0 < player_tama_data->strength) /* ���l�� */
	{
		if (20 > player_tama_data->strength) /* �Q�O�����̏ꍇ */
		{
			zako_data->health -= player_tama_data->strength;	/* ���������� */
			if (0 >= zako_data->health) 	/* �O�����l�Ȃ�A�|�����B */
			{
				play_voice_auto_track(VOICE08_DEATH);
				explosion_add_type(s->x256,s->y256+t256(5),/*0,*/EXPLOSION_ZAKO04/*EXPLOSION_MINI00*/);/*�U�R���Ŕ���*/
				//parsys_add(spimg, 2,2, c->x,c->y, 10, angle, 10, 50, EXPLODE|DIFFSIZE, NULL);
//				int up_flags;	up_flags=(player_tama->type==SP_SHOT_ZAKO)?(ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY):(ITEM_MOVE_FLAG_06_RAND_XY);
				if (NULL != s->callback_loser)
				{
					(s->callback_loser)(s);
					s->callback_loser=NULL;
				}
				bonus_info_any_score_nodel(s, ((zako_data->score)*(/*pd->now_stage*/player_now_stage/*level*/)) );/*���������֎d�l�ύX*/s->type=SP_DELETE;
			}
		}
	}
	//spimg=sprite_getcurrimg(c);
	//SDL_FreeSurface(spimg);
}
	//	play_voice_auto_track(VOICE02_ZAKO_HAKAI);
	//	parsys_add(spimg, 2,2, c->x,c->y, 10, 0, 0, 30, EXPLODE|DIFFSIZE, NULL);

extern void enemy_zako_yokai1_nonshield_hitbyweapon(SPRITE *s, SPRITE *t/*, int angle*/);
static void weapon_colcheck(SPRITE *s, /*int angle,*/ int destroy, int check_bullets)
{
	//s 	��l���̒e
	SPRITE *c;		//�G�X�v���C�g
//	SDL_Surface *spimg;
//	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;

	/* wenn flag check_bullets gesetzt ist, werden auch gegnerische schuesse getroffen */
	if (check_bullets)
	{
		if (NULL != (c=sprite_collision_check(s,SP_GROUP_BULLETS))) //�G�̒e�Ȃ̂��H
		{
			play_voice_auto_track(VOICE02_ZAKO_HAKAI);
			//spimg=sprite_getcurrimg(c);
			//parsys_add(spimg, 2,2, c->x,c->y, 10, angle, 10, 30, EXPLODE|DIFFSIZE, NULL);
			//SDL_FreeSurface(spimg);
			c->type=SP_DELETE;
		}
	}

	if (NULL != (c=sprite_collision_check(s,SP_GROUP_ENEMYS)))	//�G���̂��̂Ȃ̂��H
	{
	//	if (s->type==SP_SHOT_BOSS/*SP_PL_HOMING*/)
	//	{
	//		/* Gegner von Homing-Missile getroffen? */
	//		/* Paranoia-Flag und Target-Fadenkreuz entfernen */
	//		/* �z�[�~���O�e�����������Ɠ����ɁA�^�[�Q�b�g�ڕW�\���}�[�J�[���폜����B */
	//		PL_HOMING_DATA *h;	h=(PL_HOMING_DATA *)s->data;
	//		h->tg_sprite->type=SP_DELETE;/*�^�[�Q�b�g�ڕW�\���}�[�J�[�̍폜*/
	//	}
		/* wenn flag destroy gesetzt ist, wird der Schuss vernichtet */
		if (destroy)
		{	s->type=SP_DELETE;}

		// ??? why did i do this ???
		// c->flags &= (~(SP_FLAG_LOCK_TARGET));
		// ???

		switch (c->type)
		{
		//�{�X�ɓ��������̂��V�[���h�������ꍇ�̓_���[�W�������s��Ȃ�
		case SP_BOSS:	if (s->type!=SP_SHOT_ZAKO)	{	(((PLAYER_DATA *)player->data)->callback_boss_hitbyweapon)(c,s/*,angle*/);	}	break;
		case SP_CHUU:	if (s->type!=SP_SHOT_ZAKO)	{	enemy_zako_yokai1_nonshield_hitbyweapon(c,s/*,angle*/); 	}	break;

		default:
			/* ���������͖̂{���ɃU�R�ށH */
			if (SP_GROUP_ENEMYS==(c->type&SP_GROUP_ENEMYS))
			{
				hit_work_zako(s, c);/*�{���ɃU�R��*/
			}
			break;
		}
	}
}
		#if 0
		case SP_BOSS01: 	if (s->type!=SP_SHOT_ZAKO)	{	enemy_boss01_hitbyweapon(c,s/*,angle*/);	}	break;
		case SP_BOSS02: 	if (s->type!=SP_SHOT_ZAKO)	{	enemy_boss02_hitbyweapon(c,s/*,angle*/);	}	break;
		case SP_BOSS03: 	if (s->type!=SP_SHOT_ZAKO)	{	enemy_boss03_hitbyweapon(c,s/*,angle*/);	}	break;
		case SP_BOSS04: 	if (s->type!=SP_SHOT_ZAKO)	{	enemy_boss04_hitbyweapon(c,s/*,angle*/);	}	break;	// [***090127		�ǉ�
		#endif

/*---------------------------------------------------------
	�v���C���[�A�j�e�̈ړ�
---------------------------------------------------------*/

static void player_move_needle(SPRITE *s) // �얲 �� ������ �� �`���m �� ���~
{
	PL_SHOT_DATA *data=(PL_SHOT_DATA *)s->data;
	s->x256 += data->vx256;//co_s512((data->angle512))*data->speed/**fps_fa_ctor*/;
	s->y256 += data->vy256;//si_n512((data->angle512))*data->speed/**fps_fa_ctor*/;
	if (REMILIA==select_player) 	/* ���~�͂���� */
	{	int rand_int;
		rand_int=ra_nd();
		s->x256 += (rand_int&0x0100);
		s->x256 -= ((rand_int&0x0200)>>1);
	}
	else
	if (YUYUKO==select_player)		/* �H�X�q�͂���� */
	{
		PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
		data->vx256 += ((data->vx256*pd->weapon_power)>>11);
	}
	if (s->y256 < t256(-10))
	{
		s->type=SP_DELETE;
	}
	else
	{
		weapon_colcheck(s,/*data->angle512,*/1,0);
	}
}

/*---------------------------------------------------------
	�v���C���[�A�V���b�g�e�̈ړ�
---------------------------------------------------------*/
#define player_move_shot player_move_needle
//static void player_move_shot(SPRITE *s)	// �얲 �� ������ �� �`���m �� ���~
//{
//	PL_SHOT_DATA *data=(PL_SHOT_DATA *)s->data;
//	s->x += data->vx;//co_s512((data->angle512))*data->speed/**fps_fa_ctor*/;
//	s->y += data->vy;//si_n512((data->angle512))*data->speed/**fps_fa_ctor*/;
//	if (s->y < -10)
//	{
//		s->type=SP_DELETE;
//	}
//	else
//	{
//		weapon_colcheck(s,/*data->angle512,*/1,0);
//	}
//}


#if (1==USE_PLAYER_WAVE)
static void player_move_killray(SPRITE *s)		//�g��Ȃ�
{
	PL_KILLRAY_DATA *data=(PL_KILLRAY_DATA *)s->data;
	s->x256+=((cos512((data->angle512))*data->speed256)>>8)/**fps_fa_ctor*/;
	s->y256+=((sin512((data->angle512))*data->speed256)>>8)/**fps_fa_ctor*/;
	data->distance+=data->speed*fps_fa_ctor;
	if ((data->distance>20) && (s->anim_frame<11))
	{
		data->distance=0;
		s->anim_frame++;
	}
	if (s->y256<t256(-10))
	{
		s->type=SP_DELETE;
	}
	else
	{
		weapon_colcheck(s,/*data->angle512,*/1,0);
	}
}
#endif //(1==USE_PLAYER_WAVE)

#if (1==USE_PLAYER_BOMBER)
/* �����͏����X�y�J�ɂȂ�\�� */
static void player_move_bomb(SPRITE *s)
{
	static int k=5;
	PL_BOMB_DATA *data=(PL_BOMB_DATA *)s->data;
	s->x256 += ((cos512((data->angle512))*data->speed256)>>8)/**fps_fa_ctor*/;
	s->y256 += ((sin512((data->angle512))*data->speed256)>>8)/**fps_fa_ctor*/;
	if ((s->x256< t256(0-100))||(s->x256> t256(100+GAME_WIDTH ) )||
		(s->y256< t256(0-100))||(s->y256> t256(100+GAME_HEIGHT) ))
	{
		s->type=SP_DELETE;
	}
	else
	{
		weapon_colcheck(s,/*data->angle512,*/1,0);
	}
	if (k>0)	{	k--;	}
	else		{	data->speed++;	k=5;	}
}
#endif //(1==USE_PLAYER_BOMBER)

/*---------------------------------------------------------
	�G��T���q�֐�
---------------------------------------------------------*/
extern SPRITE *sprite_list000_head;
static /*int*/SPRITE *search_enemy_by_sprite(void)
{
	/* Suche Enemy-Sprite, das noch nicht verfolgt wird, liefert SPRITE-ID oder -1 */
	SPRITE *s=sprite_list000_head;/* �X�v���C�g ���X�g�̐擪 ����T�� */
	while (NULL != s)/* �X�v���C�g ���X�g�̍Ō�܂Œ��ׂ� */
	{
		if (
			#if 1
			(s->type != SP_DELETE ) && /* �폜�ς݂͔�΂� */
			#endif
			(s->type & SP_GROUP_ENEMYS) &&			/* �v���C���[�ɂƂ��Ă̓G(�U�R��{�X) */
			(0==(s->flags & SP_FLAG_LOCK_TARGET)) &&	/* ����t���O�������G(�ʂ̗U���e�������Ă��Ȃ��G) */
			(s->flags & SP_FLAG_VISIBLE))			/* �g�p���̓G */
		{
			if ((s->x256 > 0) && (s->x256 < t256(GAME_WIDTH )-((s->w)<<8)) &&
				(s->y256 > 0) && (s->y256 < t256(GAME_HEIGHT)-((s->h)<<8))) 	/* �\����ʓ��̓G */
			{
				return (s)/*->id*/;/*��������*/
			}
		}
		s = s->next;/*��*/
	}
	return (NULL)/*-1*/;/*������Ȃ�����*/
}

/*---------------------------------------------------------
	�v���C���[�A�U���e�̈ړ�
---------------------------------------------------------*/

#if (1==USE_PLAYER_HOMING)
//static void player_move_homing(SPRITE *s) {}
#endif //(1==USE_PLAYER_HOMING)
static void player_move_parrent_hlaser(SPRITE *s)
{
	PL_HOMING_DATA *data;
	data=(PL_HOMING_DATA *)s->data;
//
	SPRITE *target;
	target=NULL;	/* Target */
//	SPRITE *tg;
//	tg=(SPRITE *)data->tg_sprite; /* Target-Fadenkreuz */	/* �^�[�Q�b�g�ڕW�\���}�[�J�[ */

	switch (data->state)
	{
	case 0: /* ��]���Ȃ��琳�ʂ������B / Raketen in Ausgangspos. bringen (Sprengkopf nach Norden) */
		if (data->nr==0)	{	data->angle512+=deg_360_to_512(8)/**fps_fa_ctor*/; }	/* �E���ɉ�]������B/ linke Rakete: rot. im UZ */
		else				{	data->angle512-=deg_360_to_512(8)/**fps_fa_ctor*/; }	/* �����ɉ�]������B/ rechte Rekete: rot. gegen UZ */
		//
		#if 0
		if (data->angle360<0)		{	data->angle360+=360;	}
		if (data->angle360>360) 	{	data->angle360-=360;	}
		#else
		mask512(data->angle512);
		#endif
		//
		if ( t256(1.0) < data->speed256 )
		{
			data->speed256 -= /*(int)*/t256(0.5);/*����*/
		}
		//
		if ((data->angle512>=deg_360_to_512(270-4/*266*/))&&
			(data->angle512<=deg_360_to_512(270+4/*274*/))) /* �قڐ���(270)���������� */
		{
			/* Sprengkopf in Ausgangspos? */
			data->state=2;						/* ���� */
			data->angle512=deg_360_to_512(270); /* �����𐳖�(270)�ɌŒ肷�� */
		}
		break;
	case 2: /* �G��T���B / Ziel suchen */
		data->target_obj/*target_id*/=search_enemy_by_sprite();
		if (NULL/*-1*/ != data->target_obj/*target_id*/)
		{
			target=data->target_obj/*sp rite_get_by_id (data->target_id)*/;
			if (NULL != target)
			{
				data->state=3;						/* ���� */
				target->flags|=SP_FLAG_LOCK_TARGET; /* �u������Ȃ��ł���v�t���OON */
			//	tg->flags|= SP_FLAG_VISIBLE;	/* �^�[�Q�b�g�ڕW�\���}�[�J�[ */
			}
		//	else
		//	{
		//	//	tg->flags&= (~(SP_FLAG_VISIBLE));	/* �^�[�Q�b�g�ڕW�\���}�[�J�[ */
		//	}
		}
		break;
	case 3: /* Ziel verfolgen */
		if (NULL/*-1*/ == /*data->target_id*/data->target_obj)	/* ������Ȃ� */
		{
		//	/* ...orz ���肦�˂� */ error(ERR_WARN,"homing: target_id==-1&&state==3 ->back to state 2 (this shouldn't happen)");
			data->state=2;	/* �d���Ȃ��߂� */
		}
		else
		{
			if ( t256(12.0) > data->speed256)
			{
				data->speed256 += /*(int)*/t256(0.5);/*����*/
			}
			target=data->target_obj/*sp rite_get_by_id (data->target_id)*/;
			if (//(NULL != target) &&	/* �d�����Ă�̂Ŕr�� */
				(target->flags&SP_FLAG_VISIBLE) &&
				(target->x256 > t256(0)) &&
				(target->x256 < (t256(GAME_WIDTH))-((target->w)<<8)) &&
				(target->y256 > t256(0)) &&
				(target->y256 < (t256(GAME_HEIGHT))-((target->h)<<8)))
			{
				{int ta512;
					ta512=atan_512(target->y256-s->y256,target->x256-s->x256);
					if (data->angle512>ta512)
					{
							 if (data->angle512-ta512<deg_360_to_512(175))	{	data->angle512-=deg_360_to_512(8)/**fps_fa_ctor*/;} /* hlaser:8, homing:9 */
						else if (data->angle512-ta512>deg_360_to_512(185))	{	data->angle512+=deg_360_to_512(8)/**fps_fa_ctor*/;} /* hlaser:8, homing:9 */
					}
					else if (data->angle512<ta512)
					{
							 if (ta512-data->angle512<deg_360_to_512(175))	{	data->angle512+=deg_360_to_512(8)/**fps_fa_ctor*/;} /* hlaser:8, homing:9 */
						else if (ta512-data->angle512>deg_360_to_512(185))	{	data->angle512-=deg_360_to_512(8)/**fps_fa_ctor*/;} /* hlaser:8, homing:9 */
					}
				}
				#if 0
				if (data->angle360<0)	{	data->angle360+=360;}
				if (data->angle360>360) {	data->angle360-=360;}
				#else
				mask512(data->angle512);
				#endif
			//	tg->x=target->x+((target->w-tg->w)>>1)+ra_nd()%6-3; /* �^�[�Q�b�g�ڕW�\���}�[�J�[ */
			//	tg->y=target->y+((target->h-tg->h)>>1)+ra_nd()%6-3; /* �^�[�Q�b�g�ڕW�\���}�[�J�[ */
			}
			else
			{
				/* Ziel wurde anderweitig vernichtet, neues Ziel suchen */
				data->target_obj=NULL/*data->target_id=-1*/;		/* ��ʊO�ɓ�����ꂽ�� */
				data->state=2;	/* �߂� */
			//	tg->flags&= (~(SP_FLAG_VISIBLE));	/* �^�[�Q�b�g�ڕW�\���}�[�J�[ */
			}
		}
		break;
	}
	#if 1
	/* (�����K�v�Ȃ�����)�O�̈׃}�X�N */
	mask512(data->angle512);
	#endif
	s->x256 += ((cos512((data->angle512))*data->speed256)>>8)/**fps_fa_ctor*/;
	s->y256 += ((sin512((data->angle512))*data->speed256)>>8)/**fps_fa_ctor*/;
	#if 0
	/* homing */
	//	{	dou ble dangle;
	//		dangle=data->angle+180;
	//		if (dangle>360) {	dangle-=360;}
	//		//parsys_add(NULL,20,0,s->x+((s->w)>>1),s->y+((s->h)>>1),20,dangle,30,10,PIXELATE,NULL);
	//	}
		#if 0
		s->anim_frame=((data->angle360*20)/360) % 20;
		#else
		s->anim_frame=((data->angle512*10)>>8);
		#endif
	#endif
	weapon_colcheck(s,/*data->angle,*/1,0);

	data->time_out--/*range-=fps_fa_ctor*/;
	if (data->time_out/*range*/<=0) /* ���Ԑ؂�H / max. Reichweite erreicht? */
	{
		s->type=SP_DELETE;		/* ���Ԑ؂�Ȃ̂ł����܂� */
	//	tg->type=SP_DELETE; 	/* �^�[�Q�b�g�ڕW�\���}�[�J�[*/
		if (NULL != target) 	/* �^�[�Q�b�g���܂������Ă���A Target freigeben */
		{
			target->flags &= (~(SP_FLAG_LOCK_TARGET)); /* �M�����t���O�߂��Ƃ����B */
		}
	}
}

static void player_move_kodomo_hlaser(SPRITE *s)
{
	#if 1
	PL_HOMING_KODOMO_DATA *data=(PL_HOMING_KODOMO_DATA *)s->data;
	SPRITE *oya;
	oya = data->target_obj;
	if (	(SP_DELETE == oya->type) || 	/* �e�������ς݂Ȃ�Ύq������ */
			(SP_SHOT_BOSS != oya->type) 	/* SP_SHOT_BOSS�ȊO�͐e�łȂ��̂ŏ��� */
		)
	{
		s->type=SP_DELETE;/*�����܂�*/
	}
//
	s->x256 = oya->x256;
	s->y256 = oya->y256;

	/* ���t���[�������Ȃ��ꍇ�́A������������ */
	if ((data->check_x256 != s->x256) ||
		(data->check_y256 != s->y256))
	{
		data->time_out	= 8;/* �������玩���������Ȃ� */
	}
	data->time_out--;
	if ( 1 > data->time_out)
	{
		s->type=SP_DELETE;/*�����܂�*/
	}
	data->check_x256 = s->x256;
	data->check_y256 = s->y256;
	#endif
}

/*---------------------------------------------------------
	�v���C���[�A�������{���̈ړ�(�얲)
---------------------------------------------------------*/

/*
	s->x		�V�[���h��x���W
	player->x	�v���C���[��x���W
	player->w/2 �v���C���[�̉����̔���
	co_s512((data->angle512))	angle�̊p�x���΂ɒ���������cos�B���̏ꍇ��40���̔{���{�P�O*fps��cos�B
	data->radius	�~�̔��a40
	s->w/2		�V�[���h�̉����̔���
	�v���C���[��x���W�{�v���C���[�̉����̔������v���C���[�̒��S���W
	�Ȃ̂�
	�V�[���h��x���W���v���C���[��x���W�{�v���C���[�̉����̔����{co s(radian((�S�O��*x)�{speed*fps))�~40�|�V�[���h�̉����̔���
				   ���v���C���[�̒��S���W�{co s(radian((�S�O��*x)�{speed*fps))�~40�|�V�[���h�̉����̔���
*/
// ���v��� �� �����v��� ���p
static void player_move_kekkai(SPRITE *s)	//�얲
{
	PL_KEKKAI_DATA *data	= (PL_KEKKAI_DATA *)s->data;
	data->angle512 += (data->add_r512/**fps_fa_ctor*/);/*...*/
	mask512(data->angle512);// if (data->angle>360) {	data->angle-=360;}

//	s->anim_frame = (((data->angle360))/10)%36;
//	s->anim_frame = ((((data->angle512)/*&(512-1)*/)*(36/2))>>8);
//	s->anim_frame = ((((data->angle512)/*&(512-1)*/)*(32/2))>>8);
	s->anim_frame = ((((data->angle512)/*&(512-1)*/))>>4);

	s->x256 = player->x256 + cos512((data->angle512))*data->radius+((player->w-s->w)<<7);
	s->y256 = player->y256 + sin512((data->angle512))*data->radius+((player->h-s->h)<<7);
//	if (REIMU==select_player)
	{
		if (data->radius<255)	{	data->radius+=(ra_nd()&1);} /* �������L����悤�ɂ��� */
		else	{	s->type=SP_DELETE;}/* ��]���L�������炨���܂� */
		s->alpha			= ((int)(255-data->radius))&(256-1);				/* ���񂾂������悤�ɂ��� */
	}
//	else //if (YUYUKO==select_player)
//	{
//		PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
//		if (pd->bomber_time<=0) {	s->type=SP_DELETE;}/* ���Ԑ����ł����܂� */
//	}
	weapon_colcheck(s,/*d->angle,*/0,1);/* �{���œG��|�������蔻�� */
}

/*---------------------------------------------------------
	�v���C���[�A�������{���̈ړ�(�������A�`���m�A���H�X�q(�Ⴄ�����ɂ�����) )
---------------------------------------------------------*/

static void player_move_levarie(SPRITE *s)
{
	PL_LEVARIE_DATA *data=(PL_LEVARIE_DATA *)s->data;
	s->x256 += data->vx256;//co_s512((data->angle512))*d->speed/**fps_fa_ctor*/;
	s->y256 += data->vy256;//si_n512((data->angle512))*d->speed/**fps_fa_ctor*/;
	if (CIRNO==select_player)
	{	/*1.5*(d->speed)*//**fps_fa_ctor*/;/*�`���m�p(�b��I)*/
		s->y256 -= (abs((data->vx256+(data->vx256>>1) )));
		s->y256 -= (abs((data->vy256+(data->vy256>>1) )));
		s->y256 -= t256(1);
	}
	weapon_colcheck(s,/*data->angle,*/0,1);/* �{���œG��|�������蔻�� */

	if ((s->x256+((s->w)<<8) < t256(0))||(s->x256 > t256(GAME_WIDTH))||
		(s->y256+((s->h)<<8) < t256(0))||(s->y256 > t256(GAME_HEIGHT)))
	{
		s->type=SP_DELETE;/* ��ʊO�ɏo���炨���܂� */
	}
}

/*---------------------------------------------------------
	�v���C���[�A�������{���̈ړ�(���~���A)
---------------------------------------------------------*/

static void remilia_move_burn_fire(SPRITE *s)
{
	OZ_BURN_FIRE_DATA *data=(OZ_BURN_FIRE_DATA *)s->data;
	// ����
	// x1.10
//	data->vx256 = ((data->vx256 * t256(1.1))>>8);//co_s512((data->angle512))*p->speed/**fps_fa_ctor*/;
//	data->vy256 = ((data->vx256 * t256(1.1))>>8);//si_n512((data->angle512))*p->speed/**fps_fa_ctor*/;
	// x1.125
//	data->vx256 += ((data->vx256)>>3);//co_s512((data->angle512))*p->speed/**fps_fa_ctor*/;
//	data->vy256 += ((data->vy256)>>3);//si_n512((data->angle512))*p->speed/**fps_fa_ctor*/;
	// x1.0625
	data->vx256 += ((data->vx256)>>4);//co_s512((data->angle512))*p->speed/**fps_fa_ctor*/;
	data->vy256 += ((data->vy256)>>4);//si_n512((data->angle512))*p->speed/**fps_fa_ctor*/;
	//
	s->x256 += data->vx256;//co_s512((data->angle512))*p->speed/**fps_fa_ctor*/;
	s->y256 += data->vy256;//si_n512((data->angle512))*p->speed/**fps_fa_ctor*/;
	if ((s->x256+((s->w)<<8) < t256(0))||(s->x256 > t256(GAME_WIDTH))||
		(s->y256+((s->h)<<8) < t256(0)))
	{
		s->type=SP_DELETE;/* ��ʊO�ɏo���炨���܂� */
	}
	else
	{
		weapon_colcheck(s,/*data->angle,*/0,1);/* �{���œG��|�������蔻�� */
	}
}

/* �\�����{���̉��̕��� */
/*		r_or_l	[xxx_r] l=2, [xxx_l] r=0 ,u=3 ,d=1	*/
static void remilia_add_burn_fire(SPRITE *s/*, int ggg*/ /*r_or_l*/)	/* [***090221 �ǉ� */
{
	int ggg;
	int hhh;
	hhh = BASE_BOMBER2_PNG_oz;
	for (ggg=0;ggg<3;ggg++)
	{
		SPRITE *c;
		#if 0
		/*const*/ char *bbb_name[3] =
		{
			"bomber2_oz.png"/*"fire_wind_l.png"*/,	/*move left*/
			"bomber3_oz.png"/*"fire_wind_r.png"*/,	/*move right*/
			"bomber4_oz.png"/*"fire_wind_u.png"*/,
		};
		c = sprite_add_file 0((char *)bbb_name[(ggg)], 4, PRIORITY_01_SHOT/*PRIORITY_02_PLAYER*/, 0);
		#endif
		c = sprite_add_res(hhh);hhh += PLAYERS5;
		//
		c->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);
		c->type 			= SP_SHOT_BOSS/*�{�X�ɗL��*/;/*SP_SHOT_ZAKO �{�X�ɖ���*/
		c->anim_speed		= 3;
		c->anim_frame		= 0;
		c->callback_mover	= remilia_move_burn_fire;
		c->alpha			= 100;
		c->x256 			= s->x256+((s->w-c->w)<<7);
	//		 if (0==ggg/*2==r_or_l*/)	{	c->y			= s->y+15;}
	//	else if (1==ggg/*0==r_or_l*/)	{	c->y			= s->y+15;}
	//	else if (2==ggg/*3==r_or_l*/)	{	c->y			= s->y+15;	c->y -= s->h;}
	//
		c->y256 			= s->y256+t256(15);
		if (2==ggg/*3==r_or_l*/)	{	c->y256 -= ((s->h)<<8);}
	//
		OZ_BURN_FIRE_DATA *data;
		data				= mmalloc(sizeof(OZ_BURN_FIRE_DATA));
		c->data 			= data;
	//
	//	data->speed 		= 3/*12*/;/*����������ƒx��(�x�����������c��̂ŋ���)*/
		data->strength		= 16/*9*/ /*10*/;/* ����ύ��\���ۂ����炢�����Ă�����B���Ă�̓���� */
		/* ����̃{���͓��ĂȂ���Ό���Ȃ��̂�(�O���ƌ���Ȃ�)�኱�������邭�炢�ł��� */
		/* �����A���������蔻�肪�ݒ�ł���悤�ɂȂ�����A�����蔻��͑����������������� */
		{
			const signed short aaa512[4] =
			{
				deg_360_to_512(180),	/*l=2*/ 	/*move left*/
				deg_360_to_512(  0),	/*r=0*/ 	/*move right*/
				deg_360_to_512(270),	/*u=3*/
				deg_360_to_512( 90),	/*d=1*/ 	/* ���݃_�~�[(short�̃f�[�^�[��2�P�ʂ������̂�) */
			};
		//	data->angle512	= ((dou ble)r_or_l/2)*deg_360_to_512(180)/*M_PI*2*1/2*/;
		//	data->angle512	= aaa512[ggg];
			data->vx256 	= cos512((/*data->angle512*/aaa512[ggg]))*/*data->speed*/(1)/**fps_fa_ctor*/;
			data->vy256 	= sin512((/*data->angle512*/aaa512[ggg]))*/*data->speed*/(1)/**fps_fa_ctor*/;
		}
	}
}

static void player_move_add_cross_red(SPRITE *s)	/* [***090220 �ǉ� */
{
//	PL_CROSS_FIRE_DATA *data=(PL_CROSS_FIRE_DATA *)s->data;

//	data->angle512 += ra d2deg512(1)/*fps_fa_ctor*/;
//	mask512(data->angle512);	//if (data->angle_>M_PI)	{	data->angle_ -= 2*M_PI; }

	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	//
	if (0x3f==(((int)pd->bomber_time)&0x3f))
	{
		remilia_add_burn_fire(s/*, 0*/ /*2*/);
	//	remilia_add_burn_fire(s, 1/*0*/);
	//	remilia_add_burn_fire(s, 2/*3*/);
	}
	//
	if (pd->bomber_time > 0x3f/*31 20*/ )
	{
		/*else */
	//	if (0==(((int)pd->bomber_time)&3/*%4*/))
	//	{	/* 0 1 2 3 */
	//		s->anim_frame++;
	//		s->anim_frame &= 3; //if (s->anim_frame==4) {	s->anim_frame=0;	}
	//	}
		s->anim_frame = ((((int)pd->bomber_time)>>2)&0x03);
	}
	else
	{
	//	if (0==(((int)pd->bomber_time)&3/*%4*/))
	//	{
	//			 if (s->anim_frame<4)	{	s->anim_frame=4;}
	//		else if (s->anim_frame==4)	{	s->anim_frame=5;}
	//		else if (s->anim_frame==5)	{	s->anim_frame=6;}
	//		else if (s->anim_frame==6)	{	s->anim_frame=7;}
	//		else if (s->anim_frame==7)	{	s->anim_frame=8;}
	//		else						{	s->anim_frame=9;}
	//	}
		static Uint8 ddd_tbl[8] =
		{	/* �{���̏I��肪�킩��₷���悤�P�\����(9�̃p�^�[��)�𒷂�(����)�A���߂Ɍx������ */
			 9,  9,  9,  8, 	 7,  6,  5,  4,
		};
		s->anim_frame = ddd_tbl[((((int)pd->bomber_time)>>3)&0x07)];
	}
	s->x256 = player->x256+((player->w-s->w)<<7);
	s->y256 = player->y256+((player->h-s->h)<<7);

	weapon_colcheck(s,/*data->angle,*/0,1);/* �\�����{�̂ŏĂ��E���ꍇ */

	if (1 > pd->bomber_time) { s->type=SP_DELETE;}/* ���Ԑ����ł����܂� */
}


/*---------------------------------------------------------
	�v���C���[�A�j�e�̒ǉ�(�얲)[�j�e]�A(������)[�X�e]�A�H�X�q[���e] (���~���A�A���`���m)[�R�E�����e]
---------------------------------------------------------*/
enum
{
	NEEDLE_ANGLE_270=0,
	NEEDLE_ANGLE_263,	/* ���~���A �p */
	NEEDLE_ANGLE_277,	/* ���~���A �p */
	NEEDLE_ANGLE_271,	/* �H�X�q �p */
};
static void player_add_needle(SPRITE *s, int x_offs, int y_offs, int needle_type )	//�얲�A�������A�H�X�q //���~	/* [***090220 �ǉ� */
{
	SPRITE *shot; //sprite_add_file 0("fireball1.png",2,PRIORITY_02_PLAYER,1);	shot->anim_speed=1;
	shot					= sprite_add_res( BASE_NEEDLE_PNG+select_player );
	//sprite_add_file 0((char *)aaa_name[(k)],1,PRIORITY_02_PLAYER,0); shot->anim_speed=0;
	/*REMILIA*/  //sprite_add_file 0("bat.png",5,PRIORITY_02_PLAYER,0); shot->anim_speed=3;
	shot->type				= SP_SHOT_BOSS/*SP_PL_FIREBALL*/;
	shot->x256				= s->x256+((s->w-shot->w)<<7) + x_offs;
	shot->y256				= s->y256-t256(7) + y_offs;
//	if (REIMU != select_player) //�얲�̓A���t�@�Ȃ�	�얲���A���t�@����(�{�ƌ���������A���t�@����������)
	{
		shot->alpha 		= 150;
	}
	shot->callback_mover	= player_move_needle;/*player_move_koumori*/
	shot->flags 			|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);
	PL_SHOT_DATA *data;
	data					= mmalloc(sizeof(PL_SHOT_DATA));
	shot->data				= data;
//
	#define DDD_STRENGTH		(PLAYERS5*0)
	#define DDD_SPEED			(PLAYERS5*1)
	#define DDD_MAX 			(PLAYERS5*2)		/* �ő吔 */
	static Uint8 ddd_tbl[DDD_MAX] =
	{/* REIMU MARISA REMILIA CIRNO YUYUKO */
		 2, 4, 6, 6, 8, 	/* strength �V���b�g�̋��� */	/*REMILIA, CIRNO,  6 5 ��������*/
		18,10, 3, 3, 4, 	/* speed	�V���b�g�̑��� (�x��������ʏ�̒e����������̂ŋ����B�������d���Ȃ�) */
	};
	data->strength			= ddd_tbl[DDD_STRENGTH+select_player];
//	p_speed 				= ddd_tbl[DDD_SPEED+select_player];
//	data->strength			= (4);MARISA	data->strength		= /*3*/2;REIMU	/*��������*/
//	data->angle512			= bbb[needle_type]/*deg_360_to_512(270)*/;
//	data->speed 			= 10;	/*REMILIA, CIRNO,  3 8 ��������*/
//	data->vx256 			= cos512((/*angle512*/deg_360_to_512(270)))*/*p->speed*/(ddd_tbl[DDD_SPEED+select_player])/**fps_fa_ctor*/;
//	data->vy256 			= sin512((/*angle512*/deg_360_to_512(270)))*/*p->speed*/(ddd_tbl[DDD_SPEED+select_player])/**fps_fa_ctor*/;
	{
		const signed short bbb512[4] =
		{
			deg_360_to_512(270),
			deg_360_to_512(270-25),/*263=270-7*/	/* ���~���A �p */
			deg_360_to_512(270+25),/*277=270+7*/	/* ���~���A �p */
			deg_360_to_512(271),					/* �H�X�q �p */
		};
		data->vx256 		= cos512((/*angle512*/bbb512[needle_type]))*/*p->speed*/(ddd_tbl[DDD_SPEED+select_player])/**fps_fa_ctor*/;
		data->vy256 		= sin512((/*angle512*/bbb512[needle_type]))*/*p->speed*/(ddd_tbl[DDD_SPEED+select_player])/**fps_fa_ctor*/;
	}
}

/*---------------------------------------------------------
	�v���C���[�A�I�v�V�����̈ړ�����
---------------------------------------------------------*/
static void reimu_marisa_control_option(SPRITE *s, PL_OPTION_DATA *data, PLAYER_DATA *pd) //�얲�A������
{
	mask512(data->angleL512);

	{int vv512;
		vv512 = (YUYUKO==select_player)?(3):(7);	/*ra d2deg512(0.1) == 8.1*/
		if (my_pad & PSP_KEY_SLOW)		/* �����ƑO���Ɉړ� */
		{
			if ((OPTION_ID_03_RM/*1*/&0xff)==(data->opt_anime_add_id&0xff)/*pos*/) /* 1: �E���n�̃I�v�V����(OPTION_ID_03_RM, OPTION_ID_04_RR ) */
			{
				if (data->angleL512 < /*(288)*/(256+16	-16-64+(7<<4)+((data->opt_anime_add_id&0x100)>>2)-(vv512<<4) )/*272 (256+16)*/ )/*����(�E)�ʒu*/
				{
					data->angleL512 += vv512;
				}
			}
			else	/* 0: �����n�̃I�v�V���� */
			{
				if (data->angleL512 > /*(224)*/(256-16	+16+64-(7<<4)-((data->opt_anime_add_id&0x100)>>2)+(vv512<<4) )/*240 (256-16)*/ )/*����(��)�ʒu*/
				{
					data->angleL512 -= vv512;
				}
			}
		}
		else	/* �����ƌ���Ɉړ� */
		{
			if ( (OPTION_ID_03_RM/*1*/&0xff)==(data->opt_anime_add_id&0xff)/*pos*/) /* 1: �E���n�̃I�v�V���� */
			{
				if (data->angleL512 > 64+16 +8+32-64+((data->opt_anime_add_id&0x100)>>2) )/*�ŏ�(�E)�ʒu*/
				{
					data->angleL512 -= vv512;
				}
			}
			else	/* 0: �����n�̃I�v�V���� */
			{
				if (data->angleL512 < 448-16 -8-32+64-((data->opt_anime_add_id&0x100)>>2) )/*�ő�(��)�ʒu*/
				{
					data->angleL512 += vv512;
				}
			}
		}
	}
	if (0==(pd->state_flag&STATE_FLAG_08_OPTION_HIDE))	/* �I�v�V�����������Ă�� */
	{	s->type=SP_DELETE;}/*�����܂�*/
	#if 1
	mask512(data->angleL512);/*�˂�̂���*/
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
	//	s->x256 = player->x256+cos512((data->angle512))*(16)/*data->length*/+ (((6)*(2+data->opt_anime_add_id))<<8) -((s->w)<<7) ; //2598187 2079061

	{int hankei;
		hankei = (YUYUKO==select_player)?(32):(16);
	//	s->x256 = player->x256+cos512((data->angle512))*(hankei)/*data->length*/+ (18*256)-(((data->opt_anime_add_id&2)<<8)*6) -((s->w)<<7) ; // 2079061 (4�I�v�V�����Ή�)
	//	s->y256 = player->y256+sin512((data->angle512))*(hankei)/*data->length*/+((player->h-s->h)<<7)-t256(5);
		s->x256 = player->x256+sin512((data->angleL512))*(hankei)/*data->length*/+ (18*256)-(((data->opt_anime_add_id&2)<<8)*6) -((s->w)<<7) ; // 2079061 (4�I�v�V�����Ή�)
		s->y256 = player->y256+cos512((data->angleL512))*(hankei)/*data->length*/+((player->h-s->h)<<7)-t256(5);
	}
}
	#if 0
	if ( pd_weapon <= 90-1)
	{
		player_add_needle(s, t256( 0), t256( 0), NEEDLE_ANGLE_270);
	}
	else
	{
		player_add_needle(s, t256(-5), t256( 0), NEEDLE_ANGLE_270);
		player_add_needle(s, t256( 5), t256( 0), NEEDLE_ANGLE_270);
	}
	#endif
	#if 0
	if (0==(data->opt_anime_add_id&0x080))
	{
		player_add_needle(s, t256(-7), t256( 0), NEEDLE_ANGLE_270);
	}
	else
	{
		player_add_needle(s, t256( 7), t256( 0), NEEDLE_ANGLE_270);
	}
	#endif
/*---------------------------------------------------------
	�v���C���[�A�I�v�V�����̈ړ�(�얲�A�������A���H�X�q)
---------------------------------------------------------*/

static void re_gggg(SPRITE *s, int pd_weapon)	// �얲
{
	PL_OPTION_DATA *data=(PL_OPTION_DATA *)s->data;
	if (0==(data->opt_anime_add_id&0x100))
	{
		if ( pd_weapon < 64)		{	s->flags &= (~(SP_FLAG_VISIBLE));	return; }
		else						{	s->flags |=   (SP_FLAG_VISIBLE);	}
	}
//	player_add_needle(s, t256( 0), t256( 0), NEEDLE_ANGLE_270);
	/* �얲�̏ꍇ�O���W��������Ƌ����B */
	mask512(data->angleL512);
	{signed int diff256;
		diff256 = (data->angleL512);
		diff256 -= 256;
	//	player_add_needle(s, ((diff256)<<6)+(((data->opt_anime_add_id)&0x0c00))-((0x0c00)>>1), t256( 0), NEEDLE_ANGLE_270);
		player_add_needle(s, ((diff256)<<6)+(((data->opt_anime_add_id)&0x1e00))-((0x0800)), t256( 0), NEEDLE_ANGLE_270);
	}
}

static void ma_gggg(SPRITE *s, int pd_weapon)	// ������
{
	PL_OPTION_DATA *data=(PL_OPTION_DATA *)s->data;
	if (0==(data->opt_anime_add_id&0x100))
	{
		if ( pd_weapon < 64)		{	s->flags &= (~(SP_FLAG_VISIBLE));	return; }
		else						{	s->flags |=   (SP_FLAG_VISIBLE);	}
	}
	player_add_needle(s, t256( 0), t256( 0), NEEDLE_ANGLE_270);
}

	#if 0
	if (pd_weapon <= 70-1)
	{
		player_add_needle(s, t256( 0), t256(  0), NEEDLE_ANGLE_270);
	}
	else if (pd_weapon <= 100-1)
	{
		player_add_needle(s, t256(-7), t256(  0), NEEDLE_ANGLE_270);
		player_add_needle(s, t256( 7), t256(  0), NEEDLE_ANGLE_270);
	}
	else
	{
		player_add_needle(s, t256(-7), t256(  0), NEEDLE_ANGLE_270);
		player_add_needle(s, t256( 0), t256(-38), NEEDLE_ANGLE_270);/*-38-7==-45*/
		player_add_needle(s, t256( 7), t256(  0), NEEDLE_ANGLE_270);
	}
	#endif

static void yu_gggg(SPRITE *s, int pd_weapon)	// �H�X�q
{
		player_add_needle(s, t256(	0), t256(-40), NEEDLE_ANGLE_271);/*-48*/
}

static void re_ma_move_option(SPRITE *s)	//�얲	//������	// ���H�X�q
{
	PL_OPTION_DATA *data=(PL_OPTION_DATA *)s->data;
	// �A�j���[�V����
	data->anime_wait--;
	if (data->anime_wait<1)
	{
		data->anime_wait=/*data->anime_speed;*/player_fix_status[BASE_OPT_SHOT_ANIME+select_player];/*12*/ /*2*/ /*�A�z���܂͂���������*/
	//	s->anim_frame=((s->anim_frame+(data->opt_anime_add_id)+(data->opt_anime_add_id)-1/*data->pos*/)&(8-1)/*%8*/);
		s->anim_frame=((s->anim_frame+(data->opt_anime_add_id))&(8-1)/*%8*/);
	//	if (s->anim_frame<0)		{	s->anim_frame=7;}
	}
	//
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
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
					(ggg[select_player])(s, pd->weapon_power);
					#else//2082433
					switch (select_player)
					{
					case REIMU: 	re_gggg(s, pd->weapon_power); break;	// �얲
					case MARISA:	ma_gggg(s, pd->weapon_power); break;	// ������
					case YUYUKO:	yu_gggg(s, pd->weapon_power); break;	// �H�X�q
					}
					#endif
				}
			}
		}
	}
	reimu_marisa_control_option(s, data, pd);
}

/*---------------------------------------------------------
	�v���C���[�A�I�v�V�����̈ړ�(���~���A)
---------------------------------------------------------*/

/* ���~���A�p */
static SPRITE *c1, *c2, *c3, *c4; // [***090128.090220	�ǉ�:�I�v�V�����p.c3,c4
/*
	�ڕW:state1
	FORMATION_00: ���~���A�̒���Ɏl��
	FORMATION_01: ���~���A�̎��������
	FORMATION_02: ��ʌ������x������� (y=250�̈ʒu�ɎU��΂�)
	FORMATION_03: �������p(�D���Ȉʒu�ɒu�����) (���~���A�ɏd�Ȃ�悤�ɁBslow�������ƒi�K�I��option��~)
*/
#define FORMATION_00 (0)
#define FORMATION_01 (1)
#define FORMATION_02 (2)
#define FORMATION_03 (3)


#define OPTION_ID_01_NUM (0)
#define OPTION_ID_02_NUM (1)
#define OPTION_ID_03_NUM (2)
#define OPTION_ID_04_NUM (3)

static void oz_move_option(SPRITE *s)	//���~	/* [***090220 �ǉ� */
{
	static int state1=0;

	REMILIA_OPTION_DATA *data=(REMILIA_OPTION_DATA *)s->data;
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
//
	switch (data->opt_anime_add_id)
	{
	case OPTION_ID_01_NUM:
		break;
	case OPTION_ID_02_NUM:
		if (  (pd->weapon_power<=55-1)) 		{	s->flags &= (~(SP_FLAG_VISIBLE));	}
		else /*if (  (pd->weapon_power>55-1))*/ {	s->flags |=   (SP_FLAG_VISIBLE);	}
		break;
	case OPTION_ID_03_NUM:
		if (  (pd->weapon_power<=80-1)) 		{	s->flags &= (~(SP_FLAG_VISIBLE));	}
		else /*if (  (pd->weapon_power>80-1))*/ {	s->flags |=   (SP_FLAG_VISIBLE);	}
		break;
	case OPTION_ID_04_NUM:
		if (  (pd->weapon_power<=108-1))			{	s->flags &= (~(SP_FLAG_VISIBLE));	}
		else /*if (  (pd->weapon_power>108-1))*/	{	s->flags |=   (SP_FLAG_VISIBLE);	}
		break;
	}
//
	if (s->flags&SP_FLAG_VISIBLE)	//�\������Ă���Ώ�L�̏����������B
	{
		// �A�j���[�V����
		data->anime_wait--;
		if (data->anime_wait<1)
		{
			data->anime_wait=/*data->anime_speed;*/player_fix_status[BASE_OPT_SHOT_ANIME+select_player];/*12*/ /*2*/ /*�A�z���܂͂���������*/
			s->anim_frame++;
			s->anim_frame &= (8-1);//if (s->anim_frame==8)	{s->anim_frame=0;}
		}
		// �U���x��
		if (pd->state_flag & STATE_FLAG_15_KEY_SHOT)
		{
			{
				{
					data->opt_shot_interval--;
					if (data->opt_shot_interval<0)
					{
						switch (data->opt_anime_add_id)
						{
						case OPTION_ID_01_NUM:
						case OPTION_ID_02_NUM:
							if (pd->weapon_power > 100-1)
							{
								player_add_needle(s, t256( 0), t256( 0), NEEDLE_ANGLE_277 );
								player_add_needle(s, t256( 0), t256( 0), NEEDLE_ANGLE_263 );
								data->opt_shot_interval=7+3;
							}
							else
							{
								player_add_needle(s, t256( 0), t256( 0), NEEDLE_ANGLE_270 );
								data->opt_shot_interval=5+3;
							}
							break;
						case OPTION_ID_03_NUM:
						case OPTION_ID_04_NUM:
							player_add_needle(s, t256( 0), t256( 0), NEEDLE_ANGLE_270 );
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
		// �ᑬ�ړ�
		if (my_pad & PSP_KEY_SLOW)
		{
			if (REMILIA==select_player)
			{
				slow_flag=1;
			}
		//	else
		//	{
		//		/*�`���m*/
		//	}
		}
		int player_offs_x256;
		int player_offs_y256;
		player_offs_x256 = player->x256+((player->w-s->w)<<7);
		player_offs_y256 = player->y256+((player->h-s->h)<<7);
		if (1==slow_flag)	// ���Ȃ��Ƃ��ᑬ�ړ��ŃI�v�V�������~�߂���̂̓��~�̂�(�`���m�ɂ͍���Ȃ�)
		{
			/*���~�p*/
			data->slow_count += (1<<2);
			if ((/*data->*/state1==FORMATION_03) && /* FORMATION_03: �������p(�D���Ȉʒu�ɒu�����) */
				(data->slow_count < ((data->opt_anime_add_id+1)<<(6+2))/* *30 64*/))
			{
				s->x256 = player_offs_x256;
				s->y256 = player_offs_y256;
			}
			else if (/*data->*/state1==FORMATION_02)	/* FORMATION_02: ��ʌ������x������� */
			{
				switch (data->opt_anime_add_id)
				{
				case OPTION_ID_01_NUM: if (s->x256 < t256(170)-((s->w)<<8)) {s->x256 += t256(2)/**fps_fa_ctor*/;}	break;
				case OPTION_ID_02_NUM: if (s->x256 > t256(210)) 			{s->x256 -= t256(2)/**fps_fa_ctor*/;}	break;
				case OPTION_ID_03_NUM: if (s->x256 < t256(100)-((s->w)<<8)) {s->x256 += t256(4)/**fps_fa_ctor*/;}	break;
				case OPTION_ID_04_NUM: if (s->x256 > t256(280)) 			{s->x256 -= t256(4)/**fps_fa_ctor*/;}	break;
				}
			}
		}
		else
		{
			/*���~�p*/
			if (data->slow_count)
			{
				data->slow_count=0;
				data->state2=0;
			}
		//
			// �t�H�[���[�V�����ύX
			/* OPTION_ID_01_NUM �̏ꍇ�̂݃L�[���͎󂯕t�� */
			if (OPTION_ID_01_NUM==data->opt_anime_add_id) //opt_anime_add_id==1�ɂ��flag�Ǘ�
			{
			//	if (my_pad & PSP_KEY_OPTION)
			/* (�I�v�V�����L�[�L�[�𗣂����u�ԂȂ�) */
				if ((0==(my_pad & PSP_KEY_OPTION))) 	/* �I�v�V�����L�[��������Ă��� */
				{
					if ( (my_pad^my_pad_alter) & PSP_KEY_OPTION)	/* �I�v�V�����L�[�L�[�̏�Ԃ��ς����(�g�O���A�������܂��͗������ꍇ) */
					{
						/*data->*/state1++;
						if (/*data->*/state1==(FORMATION_03+1) )
						{	/*data->*/state1=FORMATION_00;}
					//
						data->state2=0; 								/* OPTION_ID_01_NUM �� state2 */
						((REMILIA_OPTION_DATA *)c2->data)->state2=0;	/* OPTION_ID_02_NUM �� state2 */
						((REMILIA_OPTION_DATA *)c3->data)->state2=0;	/* OPTION_ID_03_NUM �� state2 */
						((REMILIA_OPTION_DATA *)c4->data)->state2=0;	/* OPTION_ID_04_NUM �� state2 */
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
						{	( 1*115+10),  ( 2*115+10),	( 0*115+10),  ( 3*115+10) },	/* FORMATION_02: ��ʌ������x������� */
						{  (240),  (240),  (220),  (220) }, 							/* FORMATION_02: ��ʌ������x������� */
					};
					switch (/*data->*/state1)
					{
					case FORMATION_00:	/* FORMATION_00: ���~���A�̒���Ɏl�� */
						data->offset_x256=((fff[FORMATION_00_LOCATE_X][(data->opt_anime_add_id)]));
						data->offset_y256=((fff[FORMATION_00_LOCATE_Y][(data->opt_anime_add_id)]));
						break;
					case FORMATION_01:	/* FORMATION_01: ���~�̎�������� */
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
			case 1: 	// �ڕW�n�_�ֈړ���
				#define oz_offset_r 	(data->offset_x256) 	/* FORMATION_01: �~�̔��a�ƌ��p */
				#define oz_offset_add_r (data->offset_y256) 	/* FORMATION_01: �~�̉�]�����A�p�x���Z�l�ƌ��p */
				{
				int delta_y256=0;				/* delta_y : y���W�ɂ�����ڕW�n�_�ƌ��ݒn�̍� */
				int delta_x256=0;				/* delta_x : x���W�ɂ�����ڕW�n�_�ƌ��ݒn�̍� */
					switch (/*data->*/state1)
					{
					case FORMATION_00:	/* FORMATION_00: ���~���A�̒���Ɏl�� */
						delta_y256 = player_offs_y256-s->y256 + data->offset_y256;
						delta_x256 = player_offs_x256-s->x256 + data->offset_x256;
						break;
					case FORMATION_01:	/* FORMATION_01: ���~�̎�������� */						//static /*dou ble*/int l_angle512;
#if 0
						/*���~�p*/
						if (REMILIA==select_player)
						{
							/*l_angle512*/data->/*f_*/angle512 += oz_offset_add_r/*data->offset_y*/ /*5==ra d2deg512(0.05)*/ /**fps_fa_ctor*/;
							mask512(/*l_angle512*/data->/*f_*/angle512);	//if (l_angle>M_PI) 	{	l_angle-=2*M_PI;	}
						}
#endif
						delta_y256 = player_offs_y256-s->y256 + (sin512((data->angle512/*+l_angle512*/))*oz_offset_r);
						delta_x256 = player_offs_x256-s->x256 + (cos512((data->angle512/*+l_angle512*/))*oz_offset_r);
						break;
					case FORMATION_02:	/* FORMATION_02: ��ʌ������x������� */
						delta_y256 = (-((s->h)<<7)-s->y256+data->offset_y256);/*240*/
						delta_x256 = (-((s->w)<<7)-s->x256+data->offset_x256);
						break;
					case FORMATION_03:	/* FORMATION_03: �������p(�D���Ȉʒu�ɒu�����) */
						delta_y256 = player_offs_y256-s->y256;
						delta_x256 = player_offs_x256-s->x256;
						break;
					}
					{	/* �ڕW�n�_�Ɉړ����� */
						int m_angle512;
						/*data->*/m_angle512=atan_512(((delta_y256)>>8), ((delta_x256)>>8));
						s->x256 += ((cos512((/*data->*/m_angle512))*/*10*/((REMILIA==select_player)?t256(8.0):t256(1.0)))>>8)/**fps_fa_ctor*/;
						s->y256 += ((sin512((/*data->*/m_angle512))*/*10*/((REMILIA==select_player)?t256(8.0):t256(1.0)))>>8)/**fps_fa_ctor*/;
					}
#if 1
					/*���~�p*/
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
			case 2: 	// �ʒu�̈ێ� & �z�u����flag
				switch (/*data->*/state1)
				{
				case FORMATION_00:	/* FORMATION_00: ���~���A�̒���Ɏl�� */
					s->x256 = player_offs_x256 + data->offset_x256;
					s->y256 = player_offs_y256 + data->offset_y256;
					break;
				case FORMATION_01:	/* FORMATION_01: ���~�̎�������� */
					/*l_angle512*/data->/*f_*/angle512 += oz_offset_add_r/*data->offset_y*/ /*5==ra d2deg512(0.05)*/ /**fps_fa_ctor*/;
					mask512(/*l_angle512*/data->/*f_*/angle512);	//if (l_angle>M_PI) 	{	l_angle-=2*M_PI;	}
					s->x256 = player_offs_x256 + sin512((data->angle512/*+data->f_angle512*/ /*l_angle512*/))*oz_offset_r;
					s->y256 = player_offs_y256 + cos512((data->angle512/*+data->f_angle512*/ /*l_angle512*/))*oz_offset_r;
					break;
				case FORMATION_02:	/* FORMATION_02: ��ʌ������x������� */
					s->x256 = data->offset_x256;
					s->y256 = data->offset_y256;
					break;
				case FORMATION_03:	/* FORMATION_03: �������p(�D���Ȉʒu�ɒu�����) */
					s->x256 = player_offs_x256;
					s->y256 = player_offs_y256;
					break;
				}
#endif
				break;
			}
		}
	}
//
	if (0==(pd->state_flag&STATE_FLAG_08_OPTION_HIDE))	/* �I�v�V�����������Ă�� */
	{	s->type=SP_DELETE;}/*�����܂�*/
}

/*---------------------------------------------------------
	�v���C���[�A�I�v�V�����̒ǉ�
---------------------------------------------------------*/

static void re_ma_yu_add_option(SPRITE *s) //�얲	//������	// �H�X�q
{
	PL_OPTION_DATA *data;		//�얲�̂𗬗p���Ă܂��B
//
	c1					= sprite_add_res( BASE_OPTION1_PNG+select_player/*+ REIMU MARISA YUYUKO */ );
	c1->flags			|= (SP_FLAG_VISIBLE);
	c1->callback_mover			= re_ma_move_option;
	data				= mmalloc(sizeof(PL_OPTION_DATA));
	c1->data			= data;
	data->strength		= 5;/* [***090214 �ǉ� */
//	data->length		= 11;
	c1->x256			= s->x256+((s->w-c1->w)<<7)-t256(11)/*data->length*/;
	c1->y256			= s->y256+((s->h-c1->h)<<7);
	data->anime_wait	= 0;
//	data->anime_speed	= player_fix_status[BASE_OPT_SHOT_ANIME+select_player];/*12*/ /*2*/ /*�A�z���܂͂���������*/
	data->opt_anime_add_id/*pos*/	= OPTION_ID_02_LM/*-1*/;/*0*/
	data->angleL512 	= deg_360_to_512(270)/*deg_360_to_512(180)*/;/*-ra d2deg512(M_PI)*/
//	data->state 		= 0;
	data->opt_shot_interval 	= 0;
	c1->anim_frame		= 0;
	c1->type			= SP_PLAYER2;
//
	c2					= sprite_add_res( BASE_OPTION1_PNG+select_player/*+ REIMU MARISA YUYUKO */ );
	c2->flags			|= (SP_FLAG_VISIBLE);
	c2->callback_mover			= re_ma_move_option;
	data				= mmalloc(sizeof(PL_OPTION_DATA));
	c2->data			= data;
	data->strength		= 5;/* [***090214 �ǉ� */
//	data->length		= 11;
	c2->x256			= s->x256+((s->w-c2->w)<<7)-t256(11)/*data->length*/;
	c2->y256			= s->y256+((s->h-c2->h)<<7);
	data->anime_wait	= 0;
//	data->anime_speed	= player_fix_status[BASE_OPT_SHOT_ANIME+select_player];/*12*/ /*2*/;/*�A�z���܂͂���������*/
	data->opt_anime_add_id/*pos*/	= OPTION_ID_03_RM/*1*/;
	data->angleL512 	= deg_360_to_512(90)/*deg_360_to_512(0)*/;
//	data->state 		= 0;
	data->opt_shot_interval 	= 0;
	c2->anim_frame		= 0;
	c2->type			= SP_PLAYER2;
//
//
	c3					= sprite_add_res( BASE_OPTION1_PNG+select_player/*+ REIMU MARISA YUYUKO */ );
	c3->flags			|= (SP_FLAG_VISIBLE);
	c3->callback_mover			= re_ma_move_option;
	data				= mmalloc(sizeof(PL_OPTION_DATA));
	c3->data			= data;
	data->strength		= 5;/* [***090214 �ǉ� */
//	data->length		= 11;
	c3->x256			= s->x256+((s->w-c3->w)<<7)-t256(11)/*data->length*/;
	c3->y256			= s->y256+((s->h-c3->h)<<7);
	data->anime_wait	= 0;
//	data->anime_speed	= player_fix_status[BASE_OPT_SHOT_ANIME+select_player];/*12*/ /*2*/ /*�A�z���܂͂���������*/
	data->opt_anime_add_id/*pos*/	= OPTION_ID_01_LL/*-1*/;/*0*/
	data->angleL512 	= deg_360_to_512(270)/*deg_360_to_512(180)*/;/*-ra d2deg512(M_PI)*/
//	data->state 		= 0;
	data->opt_shot_interval 	= 0;
	c3->anim_frame		= 0;
	c3->type			= SP_PLAYER2;
//
	c4					= sprite_add_res( BASE_OPTION1_PNG+select_player/*+ REIMU MARISA YUYUKO */ );
	c4->flags			|= (SP_FLAG_VISIBLE);
	c4->callback_mover			= re_ma_move_option;
	data				= mmalloc(sizeof(PL_OPTION_DATA));
	c4->data			= data;
	data->strength		= 5;/* [***090214 �ǉ� */
//	data->length		= 11;
	c4->x256			= s->x256+((s->w-c4->w)<<7)-t256(11)/*data->length*/;
	c4->y256			= s->y256+((s->h-c4->h)<<7);
	data->anime_wait	= 0;
//	data->anime_speed	= player_fix_status[BASE_OPT_SHOT_ANIME+select_player];/*12*/ /*2*/;/*�A�z���܂͂���������*/
	data->opt_anime_add_id/*pos*/	= OPTION_ID_04_RR/*1*/;
	data->angleL512 	= deg_360_to_512(90)/*deg_360_to_512(0)*/;
//	data->state 		= 0;
	data->opt_shot_interval 	= 0;
	c4->anim_frame		= 0;
	c4->type			= SP_PLAYER2;
//
}

static void oz_add_option(SPRITE *s)	//���~	/* [***090220 �ǉ� */
{
	REMILIA_OPTION_DATA *data;
	c1					= sprite_add_res( BASE_OPTION1_PNG+select_player/*REMILIA*/ );
	c1->flags			|= (SP_FLAG_VISIBLE);
	c1->type			= SP_PLAYER2;
	c1->callback_mover			= oz_move_option;
	data				= mmalloc(sizeof(REMILIA_OPTION_DATA));
	c1->data			= data;
	c1->anim_frame		= 0;
	data->angle512		= deg_360_to_512(45*5)/*deg_360_to_512(45*1)*/ /*M_PI*2*1/8*/;/*�`���m*/
//	data->next			= player;
//	data->state1		= 0;///
	data->state2		= 0;
	data->offset_x256	= t256(0);
	data->offset_y256	= t256(0);
	data->strength		= 5;
	data->anime_wait	= 0;
//	data->anime_speed	= 3;
	data->opt_shot_interval 	= 0;
	data->opt_anime_add_id = OPTION_ID_01_NUM;
//	data->m_angle512	= deg_360_to_512(0);
	data->slow_count	= 0;
	c1->x256			= s->x256+((s->w-c1->w)<<7);
	c1->y256			= s->y256+((s->h-c1->h)<<7)+t256(20);
//
	c2					= sprite_add_res( BASE_OPTION2_PNG+select_player/*REMILIA*/ );
	c2->flags			&= (~(SP_FLAG_VISIBLE));
	c2->type			= SP_PLAYER2;
	c2->callback_mover			= oz_move_option;
	data				= mmalloc(sizeof(REMILIA_OPTION_DATA));
	c2->data			= data;
	c2->anim_frame		= 0;
	data->angle512		= deg_360_to_512(45*7)/*deg_360_to_512(45*5)*/ /*M_PI*2*5/8*/;/*�`���m*/
//	data->next			= c1;
//	data->state1		= ((REMILIA_OPTION_DATA *)c1->data)->state1;///
	data->state2		= 0;
	data->offset_x256	= t256(0);
	data->offset_y256	= t256(0);
	data->strength		= 5;
	data->anime_wait	= 0;
//	data->anime_speed	= 3;
	data->opt_shot_interval 	= 0;
	data->opt_anime_add_id = OPTION_ID_02_NUM;
//	data->m_angle512	= deg_360_to_512(0);
	data->slow_count	= 0;
	c2->x256			= s->x256+((s->w-c2->w)<<7);
	c2->y256			= s->y256+((s->h-c2->h)<<7)+t256(20);
//
	c3					= sprite_add_res( BASE_OPTION3_PNG+select_player/*REMILIA*/ );
	c3->flags			&= (~(SP_FLAG_VISIBLE));
	c3->type			= SP_PLAYER2;
	c3->callback_mover	= oz_move_option;
	data				= mmalloc(sizeof(REMILIA_OPTION_DATA));
	c3->data			= data;
	c3->anim_frame		= 0;
	data->angle512		= deg_360_to_512(45*1)/*deg_360_to_512(45*3)*/ /*M_PI*2*3/8*/;/*�`���m*/
//	data->next			= c2;
//	data->state1		= ((REMILIA_OPTION_DATA *)c1->data)->state1;///
	data->state2		= 0;
	data->offset_x256	= t256(0);
	data->offset_y256	= t256(0);
	data->strength		= 5;
	data->anime_wait	= 0;
//	data->anime_speed	= 3;
	data->opt_shot_interval 	= 0;
	data->opt_anime_add_id = OPTION_ID_03_NUM;
//	data->m_angle512	= deg_360_to_512(0);
	data->slow_count	= 0;
	c3->x256			= s->x256+((s->w-c3->w)<<7);
	c3->y256			= s->y256+((s->h-c3->h)<<7)+t256(20);
//
	c4					= sprite_add_res( BASE_OPTION4_PNG+select_player/*REMILIA*/ );
	c4->flags			&= (~(SP_FLAG_VISIBLE));
	c4->type			= SP_PLAYER2;
	c4->callback_mover	= oz_move_option;
	data				= mmalloc(sizeof(REMILIA_OPTION_DATA));
	c4->data			= data;
	c4->anim_frame		= 0;
	data->angle512		= deg_360_to_512(45*3)/*deg_360_to_512(45*7)*/ /*M_PI*2*7/8*/;/*�`���m*/
//	data->next			= c3;
//	data->state1		= ((REMILIA_OPTION_DATA *)c1->data)->state1;///
	data->state2		= 0;
	data->offset_x256	= t256(0);
	data->offset_y256	= t256(0);
	data->strength		= 5;
	data->anime_wait	= 0;
//	data->anime_speed	= 3;
	data->opt_shot_interval 	= 0;
	data->opt_anime_add_id = OPTION_ID_04_NUM;
//	data->m_angle512	= deg_360_to_512(0);
	data->slow_count	= 0;
	c4->x256			= s->x256+((s->w-c4->w)<<7);
	c4->y256			= s->y256+((s->h-c4->h)<<7)+t256(20);
}

/*---new add-----*/


/*---------------------------------------------------------
	�v���C���[�A��������p�R�A�̒ǉ�
---------------------------------------------------------*/

static SPRITE *player_add_core(SPRITE *s1)		// ���̒ǉ�
{
	SPRITE *s2;
	s2						= NULL;
	s2						= sprite_add_res( BASE_CORE_PNG+select_player );
	s2->anim_speed			= 0;
	s2->type				= SP_PLAYER2;
	s2->x256				= (s1->x256)+((s1->w-s2->w)<<7);
	s2->y256				= (s1->y256)-((s2->h)<<8)+t256(20);
	s2->callback_mover		= player_move_core;
	s2->flags				|= (SP_FLAG_VISIBLE);
	return s2;
}

/*---------------------------------------------------------
	�v���C���[�e(�V���b�g�̂�)�����A�q�֐�
---------------------------------------------------------*/

enum
{
	REIMU_SHOT_TYPE_00=0,
	REIMU_SHOT_TYPE_01,
	REIMU_SHOT_TYPE_02,
//
	REIMU_SHOT_TYPE_03,
	REIMU_SHOT_TYPE_04,
	REIMU_SHOT_TYPE_05,
	REIMU_SHOT_TYPE_06,
	REIMU_SHOT_TYPE_07,
	REIMU_SHOT_TYPE_08,
	REIMU_SHOT_TYPE_09,
	REIMU_SHOT_TYPE_0a,
//
	//MARISA_SHOT_TYPE_03,
	//MARISA_SHOT_TYPE_04,
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
	REI02_angle512,
	REI03_yudo,
	REI04_MAX		/* �ő吔 */
};

static void player_add_shot(SPRITE *player, int shot_type) //�얲 �� ������ �� �`���m �� ���~ /* [***090220 �ǉ� */
{
	const Sint16 r_tbl[REIMU_SHOT_TYPE_MAX][REI04_MAX] =
	{	/*x_offset	y_offset  angle512 yudo */
/*00*/	{t256(	0), t256( 20), deg_360_to_512(270),   0},	/*player_add_fuda*/ 		/*player_add_star*/
/*01*/	{t256( -5), t256( 20), deg_360_to_512(270),   0},	/*player_add_dou ble_fuda*/ /*player_add_dou ble_star*/
/*02*/	{t256( +5), t256( 20), deg_360_to_512(270),   0},	/*player_add_dou ble_fuda*/ /*player_add_dou ble_star*/
//
/*03*/	{t256(-15), t256(+25), deg_360_to_512(270-6),  1},	/*player_add_triple_fuda*/
/*04*/	{t256(+15), t256(+25), deg_360_to_512(270+6),  1},	/*player_add_triple_fuda*/
/*05*/	{t256(-15), t256(+25), deg_360_to_512(270-10), 1},	/*player_add_quad_fuda*/
/*06*/	{t256(+15), t256(+25), deg_360_to_512(270+10), 1},	/*player_add_quad_fuda*/
//
/*07*/	{t256(-15), t256(+25), deg_360_to_512(270-15), 0},	/*player_add_five_fuda*/
/*08*/	{t256(+15), t256(+25), deg_360_to_512(270+15), 0},	/*player_add_five_fuda*/
/*09*/	{t256( -5), t256(+20), deg_360_to_512(270-8),  1},	/*player_add_five_fuda*/
/*0a*/	{t256( +5), t256(+20), deg_360_to_512(270+8),  1},	/*player_add_five_fuda*/
//
/*03*/	//{t256( -5), t256(+20), deg_360_to_512(270/*-4*/),  0},	/*player_add_triple_star*/	/* ������ �܂������̕����悭�Ȃ��H */
/*04*/	//{t256( +5), t256(+20), deg_360_to_512(270/*+4*/),  0},	/*player_add_triple_star*/	/* ������ �܂������̕����悭�Ȃ��H */
/*05*/	//256( -6), t256(+20), deg_360_to_512(270-1/*3*/),	0}, /*player_add_quad_star*/
/*06*/	//256( +6), t256(+20), deg_360_to_512(270+1/*3*/),	0}, /*player_add_quad_star*/
/*07*/	{t256(-15/*-10*/), t256(+25), deg_360_to_512(270/*-7*/),  0},	/*player_add_quad_star*/	/* ������ �܂������̕����悭�Ȃ��H */
/*08*/	{t256(+15/*+10*/), t256(+25), deg_360_to_512(270/*+7*/),  0},	/*player_add_quad_star*/	/* ������ �܂������̕����悭�Ȃ��H */
//
/*01*/	{t256(-15), t256(-25), deg_360_to_512(270)-8, 0},	/*�H�X�q*/
/*02*/	{t256(+15), t256(-25), deg_360_to_512(270)+8, 0},	/*�H�X�q*/
/*03*/	{t256(-25), t256(-25), deg_360_to_512(270)-16, 0},	/*�H�X�q*/
/*04*/	{t256(+25), t256(-25), deg_360_to_512(270)+16, 0},	/*�H�X�q*/
	};
	SPRITE *shot;
	shot					= sprite_add_res( BASE_PLAZMA_PNG+select_player );
	shot->type				= SP_SHOT_BOSS;
	shot->x256				= (player->x256)+((player->w-shot->w)<<7) + r_tbl[shot_type][REI00_x_offset];
	shot->y256				= (player->y256)-((shot->h)<<8) 		  + r_tbl[shot_type][REI01_y_offset]/*20*/;
	shot->callback_mover	= player_move_shot;
	shot->flags 			|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);
	shot->alpha 			= 150;/*player_add_triplestar*/
	PL_SHOT_DATA *data;
	data					= mmalloc(sizeof(PL_SHOT_DATA));
	shot->data				= data;
//
	int int_angle512;
	int ok;
	ok=0;
	{
		if (1==r_tbl[shot_type][REI03_yudo])/* �����V���b�g���H */
		{
			SPRITE *target;
			target = search_enemy_by_sprite();
			if (target != NULL)
			{
				/*data->*/int_angle512	= atan_512(target->y256-player->y256,target->x256-player->x256);
			//	if (255 < /*data->*/int_angle512 )/* ���180-360�x[/360�x]�̗̈�̂ݒǔ����� */
				if ( /*(384)*/(128*3) < ((/*data->*/int_angle512+64)&(512-1)) )/* ���225-315�x[/360�x]�̗̈�̂ݒǔ����� */
				{
					ok=1;/* �����ǔ��\ */
				}
			}
		}
	}
//
	if (0==ok)/* �����ǔ��s�H */
	{
		/*data->*/int_angle512	= r_tbl[shot_type][REI02_angle512]/*deg_360_to_512(270)*/;/*�ŗL�̐ݒ�p�x���g��*/
	}
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
	data->strength	= ccc_tbl[CCC_STRENGTH+select_player];
//	data->speed 	= ccc_tbl[CCC_SPEED+select_player];
//
	data->vx256 = cos512((/*angle512*/int_angle512))*/*p->speed*/(ccc_tbl[CCC_SPEED+select_player])/**fps_fa_ctor*/;
	data->vy256 = sin512((/*angle512*/int_angle512))*/*p->speed*/(ccc_tbl[CCC_SPEED+select_player])/**fps_fa_ctor*/;
//	if (YUYUKO==select_player)
//	{
//		;
//	}
}

/*---------------------------------------------------------
	�v���C���[�e(�V���b�g�̂�)����
---------------------------------------------------------*/

static void player_add_single_shot(SPRITE *player)	//�얲 �� ������ �� �`���m �� ���~	/* [***090220 �ǉ� */
{
	player_add_shot(player,REIMU_SHOT_TYPE_00);
}
static void player_add_dual_shot(SPRITE *player)	//�얲 �� ������ �� �`���m �� ���~	/* [***090220 �ǉ� */
{
	player_add_shot(player,REIMU_SHOT_TYPE_01);
	player_add_shot(player,REIMU_SHOT_TYPE_02);
}

static void player_add_triple_fuda(SPRITE *player)	//�얲
{
	player_add_shot(player,REIMU_SHOT_TYPE_03);/* �����V���b�g */
	player_add_shot(player,REIMU_SHOT_TYPE_00);
	player_add_shot(player,REIMU_SHOT_TYPE_04);/* �����V���b�g */
}

static void player_add_quad_fuda(SPRITE *player) //�얲
{
	player_add_shot(player,REIMU_SHOT_TYPE_05);/* �����V���b�g */
	player_add_shot(player,REIMU_SHOT_TYPE_01);
	player_add_shot(player,REIMU_SHOT_TYPE_02);
	player_add_shot(player,REIMU_SHOT_TYPE_06);/* �����V���b�g */
}

static void player_add_five_fuda(SPRITE *player)	//�얲
{
	player_add_shot(player,REIMU_SHOT_TYPE_07);
	player_add_shot(player,REIMU_SHOT_TYPE_09);/* �����V���b�g */
	player_add_shot(player,REIMU_SHOT_TYPE_00);
	player_add_shot(player,REIMU_SHOT_TYPE_0a);/* �����V���b�g */
	player_add_shot(player,REIMU_SHOT_TYPE_08);
}

static void player_add_triple_star(SPRITE *player)	//������ �� �`���m
{
	player_add_shot(player,REIMU_SHOT_TYPE_01/*MARISA_SHOT_TYPE_03*/);	/* ������ �܂������̕����悭�Ȃ��H */
	player_add_shot(player,REIMU_SHOT_TYPE_00);
	player_add_shot(player,REIMU_SHOT_TYPE_02/*MARISA_SHOT_TYPE_04*/);	/* ������ �܂������̕����悭�Ȃ��H */
}

static void player_add_quad_star(SPRITE *player)	//������
{
	player_add_shot(player,REIMU_SHOT_TYPE_01/*MARISA_SHOT_TYPE_05*/);	/* ������ �܂������̕����悭�Ȃ��H */
	player_add_shot(player,REIMU_SHOT_TYPE_02/*MARISA_SHOT_TYPE_06*/);	/* ������ �܂������̕����悭�Ȃ��H */
	player_add_shot(player,MARISA_SHOT_TYPE_07);
	player_add_shot(player,MARISA_SHOT_TYPE_08);
}

static void player_add_chou(SPRITE *player) 	//�H�X�q
{
	player_add_shot(player,YUYUKO_SHOT_TYPE_01);
	player_add_shot(player,YUYUKO_SHOT_TYPE_02);
	player_add_shot(player,YUYUKO_SHOT_TYPE_03);
	player_add_shot(player,YUYUKO_SHOT_TYPE_04);
}


#if (1==USE_PLAYER_WAVE)
static void player_add_killray(SPRITE *player)		//�g��Ȃ�
{
	SPRITE *shot;
	static int last=0;
	if (last)	{	shot	= sprite_add_file 0("killray-r.png",12,PRIORITY_01_SHOT/*PRIORITY_02_PLAYER*/,0);	last=0; }
	else		{	shot	= sprite_add_file 0("killray-b.png",12,PRIORITY_01_SHOT/*PRIORITY_02_PLAYER*/,0);	last=1; }
	shot->type				= SP_SHOT_BOSS/*SP_PL_KILLRAY*/;
	shot->x256				= player->x256+((player->w-shot->w)<<7);
	shot->y256				= player->y256-((shot->h)<<8)+t256(20);
	shot->callback_mover	= player_move_killray;
	shot->flags 			|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);
	PL_KILLRAY_DATA *data;
	data					= mmalloc(sizeof(PL_KILLRAY_DATA));
	shot->data				= data;
	data->angle512			= deg_360_to_512(270);
	data->speed256			= t256(25);
	data->distance			= 0;
	data->strength			= 6;
}
#endif //(1==USE_PLAYER_WAVE)

#if (1==USE_PLAYER_BOMBER)
/* �����͏����X�y�J�ɂȂ�\�� */
static void player_add_bomb(SPRITE *s)
{
	int a;
	for (a=0;a<355;a+=5)
	{
		SPRITE *c;
		c					= sprite_add_file 0("fireball.png",1,PRIORITY_01_SHOT/*PRIORITY_02_PLAYER*/,0);
		c->type 			= SP_SHOT_BOSS/*SP_PL_BOMB*/;
		c->x256 			= player->x256+((player->w)<<7);
		c->y256 			= player->y256+((player->h)<<7);
		c->callback_mover	= player_move_bomb;
		c->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);
		PL_BOMB_DATA *data;
		data				= mmalloc(sizeof(PL_BOMB_DATA));
		c->data 			= b;
		data->angle 		= a;
		data->speed256		= t256(1);
		data->strength		= 2;
	}
}
#endif //(1==USE_PLAYER_BOMBER)

/*---------------------------------------------------------
	�v���C���[�V�[���h����(�얲)
---------------------------------------------------------*/

static void player_add_kekkai(SPRITE *s)		//�V�[���h�̒ǉ�	//�얲
{
	PLAYER_DATA *pd=(PLAYER_DATA *)s->data;
	pd->state_flag	|= STATE_FLAG_02_BOMB_AUTO_GET_ITEM;	/*�{���ɂ�鎩�����W�\*/
	int ii;//	int i;	/* ���ی����� */
	for (ii=0;ii<(8);ii++)//	for (i=0;i<360/*i<=359*/;i+=45)
	{
		SPRITE *c;
		PL_KEKKAI_DATA *d;
		if ( (ii&1) == 0)// if (i%90 == 0)
		{
		//	c				= sprite_add_file 0("bomber1_re.png", 32/*36*/, PRIORITY_03_ENEMY/*PRIORITY_02_PLAYER*/, 1);/*"cshoot1.png"*/
			c				= sprite_add_res(BASE_BOMBER1_PNG);
			d				= mmalloc(sizeof(PL_KEKKAI_DATA));
			c->data 		= d;
			d->add_r512 	= (23); 						//���v���
			d->radius		= 5/*48*/;	//d->radius=38; 	/*���a�����l*/	/* �������L����悤�ɂ��� */
		}
		else		//1���ƕs���Ȃ̂�2������B
		{
		//	c				= sprite_add_file 0("bomber2_re.png", 32/*36*/, PRIORITY_03_ENEMY/*PRIORITY_02_PLAYER*/, 1);/*"cshoot2r.png""cshoot2.png"*/
			c				= sprite_add_res(BASE_BOMBER2_PNG);
			d				= mmalloc(sizeof(PL_KEKKAI_DATA));
			c->data 		= d;
			d->add_r512 	= (-17);							//�����v���
			d->radius		= 0/*45*/;	//d->radius=35; 	/*���a�����l*/	/* �������L����悤�ɂ��� */
		}
//		if (YUYUKO==select_player)
//		{	/* �H�X�q */
//			d->radius		+= 45/*45*/;		/*���a�����l*/
//			c->type 		= SP_SHOT_ZAKO/*�{�X�ɖ���*/;
//		}
//		else //if (REIMU==select_player)
		{	/* �얲 */
			c->type 		= SP_SHOT_BOSS/*�{�X�ɗL��*/;/*SP_SHOT_ZAKO �{�X�ɖ���*/
		}
		c->callback_mover	= player_move_kekkai;			//���v��聕�����v���
	//	c->callback_mover	= player_move_kekkai/*2*/;		//�����v���
		c->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);
		d->angle512 		= (ii*deg_360_to_512(45))/*i*/;
		/* �o�n�߂̓v���C���[���W */
		c->x256 			= player->x256;//+co_s512(((ii*deg_360_to_512(45))/*i*/))*d->radius+((player->w-s->w)>>1);
		c->y256 			= player->y256;//+si_n512(((ii*deg_360_to_512(45))/*i*/))*d->radius+((player->h-s->h)>>1); //���v���
		d->strength 		= 1/*5*/;/* [***090214 �ǉ� */
	}
}
/*---------------------------------------------------------
	�v���C���[�V�[���h����(������ �� �`���m)
---------------------------------------------------------*/

static void player_add_levarie(SPRITE *s)	//������
{
	SPRITE *c;
	c=NULL;
	PL_LEVARIE_DATA *d;
	PLAYER_DATA *pd=(PLAYER_DATA *)s->data;
	pd->state_flag	|= STATE_FLAG_02_BOMB_AUTO_GET_ITEM;	/*�{���ɂ�鎩�����W�\*/
	int i;
	int j;
	#if 0
	int k;
//	int k_strength;
		 if (MARISA==select_player) 		{	k=	0;	/*k_strength=15;*/	}
	else if (CIRNO==select_player)			{	k=	6;	/*k_strength= 3;*/	}
	else /*if (YUYUKO==select_player)*/ 	{	k=2*6;	/*k_strength= 8;*/	}
	/*const*/ char *bbb_name[3*6] =
	{	// ������
		"bomber1_ma.png",	//	"star_shield_blue.png",
		"bomber2_ma.png",	//	"star_shields_red.png",
		"bomber3_ma.png",	//	"star_shield_green.png",
		"bomber4_ma.png",	//	"star_shields_blue.png",
		"bomber5_ma.png",	//	"star_shield_red.png",
		"bomber6_ma.png",	//	"star_shields_green.png",
			// �`���m
		"bomber1_ci.png",	//	"cirno_shield_blue.png",
		"bomber2_ci.png",	//	"cirno_shields_red.png",
		"bomber3_ci.png",	//	"cirno_shield_green.png",
		"bomber4_ci.png",	//	"cirno_shields_blue.png",
		"bomber5_ci.png",	//	"cirno_shield_red.png",
		"bomber6_ci.png",	//	"cirno_shields_green.png",
			// �H�X�q
		"bomber1_yu.png",	//	"yuyuko_shield_blue.png",
		"bomber2_yu.png",	//	"yuyuko_shields_red.png",
		"bomber3_yu.png",	//	"yuyuko_shield_green.png",
		"bomber4_yu.png",	//	"yuyuko_shields_blue.png",
		"bomber3_yu.png",	//	"yuyuko_shield_red.png",
		"bomber4_yu.png",	//	"yuyuko_shields_green.png",
	};
	#endif
	int hhh;
	j=0;	hhh = BASE_BOMBER1_PNG+select_player;
	/*dou ble*/int ang512;
//	ang512 = ra d2deg512(((M_PI*2)/32))/*0*/;/*�`���m�p(�b��I)*/
	ang512 = (0)/*0*/;/*�`���m�p(�b��I)*/
	for (i=0;i<16;i++)
	{
	//	c					= sprite_add_file 0((char *)bbb_name[(k+j)], 3, PRIORITY_01_SHOT/*PRIORITY_02_PLAYER*/, 0);
		c					= sprite_add_res(hhh);hhh += PLAYERS5;
		j++;if (6==j)	{j=0;	hhh = BASE_BOMBER1_PNG+select_player;}
		c->anim_speed		= 5;
		c->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);
		c->callback_mover	= player_move_levarie;

		d					= mmalloc(sizeof(PL_LEVARIE_DATA));
		c->data 			= d;
		d->strength 		= player_fix_status[BASE_STD_BOMB_STRENGTH+select_player] /*k_strength*/ /*5*/ /*15-k-k*/;/* [***090214 �ǉ� */
		/* MARISA�͋������g�U���Ă��܂��̂ł�����Ȃ��B  */
		/* �`���m�͓��Ă₷���̂ł��̕��キ���Ȃ��Ƌ��������Ⴄ�B  */
//		d->angle512 		= ang512;
		/*dou ble*/int d_speed256;
		if ((i&(2-1)/*%2*/)==0)
		{			/*d->*/d_speed256	= t256(1.0);	c->alpha	= 150;	}
		else	{	/*d->*/d_speed256	= t256(1.2);	c->alpha	= 180;	}
		d->vx256			= ((cos512((/*angle512*/ang512))*/*p->speed*/(d_speed256))>>8)/**fps_fa_ctor*/;
		d->vy256			= ((sin512((/*angle512*/ang512))*/*p->speed*/(d_speed256))>>8)/**fps_fa_ctor*/;
//
		c->type 			= SP_SHOT_BOSS/*�{�X�ɗL��*/;/*SP_SHOT_ZAKO �{�X�ɖ���*/
		c->x256 			= (s->x256)+((s->w-c->w)<<7);
		c->y256 			= (s->y256)+((s->h-c->h)<<7);
		/* �M���X�g�h���[��(���ǂ�)�B�X�^�[�_�X�g�����@���G�ƈႢ�ߐڐ��\�͖����B */
		if (YUYUKO==select_player)	/* �H�X�q */
		{
			c->x256 		+= ((d->vx256)<<5);
			c->y256 		+= ((d->vy256)<<5);
		}
		ang512 += ((512)/16) ; // 22.5�x/360, ��/8 /*0.39269908169872415481*/
	}
}

/*---------------------------------------------------------
	�v���C���[�V�[���h����(���~���A)[����{���̏\�����{��]
---------------------------------------------------------*/

static void player_add_cross_red_parrent(SPRITE *s) /* ���~ */ /* [***090220 �ǉ� */
{
	PLAYER_DATA *pd 	= (PLAYER_DATA *)s->data;
	pd->state_flag		|= STATE_FLAG_02_BOMB_AUTO_GET_ITEM;	/*�{���ɂ�鎩�����W�\*/
	SPRITE *c;
//	c					= sprite_add_file 0("bomber1_oz.png", 10, PRIORITY_01_SHOT/*PRIORITY_02_PLAYER*/, 0);/*"cross_red.png"*/
	c					= sprite_add_res(BASE_BOMBER1_PNG_oz);
	c->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);
	c->anim_speed		= 0;
	c->anim_frame		= 0;
	c->alpha			= 220;
	c->callback_mover	= player_move_add_cross_red;
	c->type 			= SP_SHOT_ZAKO;/* ���ڍU���͋֎~ */ 		/*SP_SHOT_ZAKO �{�X�ɖ���*/
	c->x256 			= (s->x256)+((s->w-c->w)<<7);
	c->y256 			= (s->y256)+((s->h-c->h)<<7);
//
	PL_CROSS_FIRE_DATA *data;
	data				= mmalloc(sizeof(PL_CROSS_FIRE_DATA));
	c->data 			= data;
	data->strength		= 3;		/* �\�����{�� �̋��� */
//	data->angle512		= (0);
//	data->vx			= player->x+((player->w-s->w)>>1);
//	data->vy			= player->y+((player->h-s->h)>>1);
}


/*---------------------------------------------------------
	�v���C���[�A�U���e�̈ړ�����
---------------------------------------------------------*/

//#define hlaser_NUM_OF_ENEMIES (24)
#define hlaser_NUM_OF_ENEMIES (12)		/* [***090128		�����ɂ��Ă݂� */

#if (1==USE_CONTROLLER)
static void player_controller_hlaser(CONTROLLER *c)
{
	int *id_array=c->e;
	SPRITE *s;
	int i;
	if (NULL == sprite_get_by_id(id_array[0]))
	{
		for (i=1;i<hlaser_NUM_OF_ENEMIES;i++)
		{
			s=sprite_get_by_id(id_array[i]);
			if (NULL != s)
			{	s->type=SP_DELETE;}/* ID����X�v���C�g�̎��̂��݂���Ȃ���΁A�����܂� */
		}
		controller_remove_one(c);
	}
	else
	{
		for (i=hlaser_NUM_OF_ENEMIES-1; i>=1; i--)/*��납�璲�ׂ�*/
		{
			SPRITE *l/*=NULL*/;
			s=sprite_get_by_id(id_array[i]);
			l=sprite_get_by_id(id_array[i-1]);
			if ((NULL != s) && (NULL != l))
			{
				s->x256 = l->x256;
				s->y256 = l->y256;
				// draw_line(screen,s->x,s->y,l->x,l->y,
				// SDL_MapRGB(screen->format,0xff,0xff,0xff),
				// SDL_MapRGB(screen->format,0xa0,0xff,0xa0));
			}
		}
	}
}
#endif //(1==USE_CONTROLLER)

/*---------------------------------------------------------
	�v���C���[�U���e����
---------------------------------------------------------*/

#if (1==USE_PLAYER_HOMING)
#endif //(1==USE_PLAYER_HOMING)
static void player_add_hlaser(SPRITE *src)
{
	int j;
	j=(ra_nd()&(2-1));//for (j=0;j<2;j++)
	{
		#if (1==USE_CONTROLLER)
		CONTROLLER *c;
		c=controller_add();
	//	hlaser_NUM_OF_ENEMIES=12;		// [***090128		�����ɂ��Ă݂�
		int *id_array;
		id_array	= mmalloc(sizeof(int)*hlaser_NUM_OF_ENEMIES);
		c->e		= id_array;
		c->con		= player_controller_hlaser;
		#endif //(1==USE_CONTROLLER)
//
//		/*int*/ SPRITE **id_array_bbb= (SPRITE **)c->e;
		SPRITE *s_old;
		s_old = NULL;
		int i;
		SPRITE *s;
		s=NULL;
		for (i=0;i<hlaser_NUM_OF_ENEMIES;i++)
		{
			s_old = s;
			s				= sprite_add_res( BASE_TSHOOT_PNG+select_player );

		#if (1==USE_CONTROLLER)
			id_array[i] 	= s->id;
		#endif //(1==USE_CONTROLLER)

			s->type 		= SP_SHOT_BOSS/*�{�X�ɗL��*/ /*SP_PL_HLASER*/;
			s->x256 		= (src->x256) + ((src->w)<<7) - t256(5);
			s->y256 		= (src->y256) + ((src->h)<<7) + t256(15);
			s->flags		|= (SP_FLAG_VISIBLE/*|SP_FLAG_TIME_OVER*/);
			s->anim_frame	= 5-((double)6.0/hlaser_NUM_OF_ENEMIES)*i;
			if (i==0)
			{
				s->callback_mover	= player_move_parrent_hlaser;
				PL_HOMING_DATA *data;
				data				= mmalloc(sizeof(PL_HOMING_DATA));
				s->data 			= data;
				data->nr			= j;
				data->angle512		= deg_360_to_512(100);
				data->speed256		= t256(12);
				data->strength		= player_fix_status[BASE_LOW_BOMB_STRENGTH+select_player]/*6*/;/*�ᑬ�{���̋���*/
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

static void player_move_add_bomber_hlaser(SPRITE *s)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	if (0==(((int)pd->bomber_time)&0x3f))
	{
		player_add_hlaser(player);
	}
	s->anim_frame		= (((int)pd->bomber_time>>4)&0x03);/*"bomber_slow.png"*/
	s->x256 			= (player->x256)+((player->w-s->w)<<7);
	s->y256 			= (player->y256)+((player->h-s->h)<<7);
	if (1 > pd->bomber_time) {	s->type=SP_DELETE;}/* ���Ԑ����ł����܂� */
}
static void player_add_hlaser_parrent(SPRITE *s)
{
	PLAYER_DATA *pd 	= (PLAYER_DATA *)s->data;
	pd->state_flag		|= STATE_FLAG_02_BOMB_AUTO_GET_ITEM;	/*�{���ɂ�鎩�����W�\*/
	SPRITE *c;
//	c					= sprite_add_file 0("bomber_slow.png", 4, PRIORITY_01_SHOT/*P R_BACK1*/, 1);	c->anim_speed		= 0;
	c					= sprite_add_res(BASE_BOMBER_SLOW_PNG);
	c->flags			|= (SP_FLAG_VISIBLE/*|SP_FLAG_TIME_OVER*/);
	c->data 			= 0/*d*/;
	c->anim_frame		= 0;
	c->alpha			= 128/*220*/;
	c->callback_mover	= player_move_add_bomber_hlaser;
	c->type 			= SP_SHOT_BOSS/*�{�X�ɗL��*/;	/* �ᑬ�{�����{�X�ɗL���Ƃ��� */	/*SP_SHOT_ZAKO �{�X�ɖ���*/
	c->x256 			= (s->x256)+((s->w-c->w)<<7);
	c->y256 			= (s->y256)+((s->h-c->h)<<7);
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


extern void set_bg_alpha(int set_bg_alpha);
static void player_keycontrol(SPRITE *s1)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)s1->data;
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
		if (0 < /*bomb_wait*/pd->bomber_time)				//�{���E�F�C�g����
		{
			//	d->bomber_time-=fps_fa_ctor;
			/*bomb_wait*/pd->bomber_time--/*-=fps_fa_ctor*/;/**/
			if (2 > /*bomb_wait*/pd->bomber_time)
			{
				set_bg_alpha(255);/* ��ʂ𖾂邭���� */	/* ���\���������������� */
		//	}
		//	else
		//	if (/*bomb_wait*/pd->bomber_time < 190)
		//	{
				pd->state_flag &= (~STATE_FLAG_02_BOMB_AUTO_GET_ITEM);	/* �{���ɂ�鎩�����W�͏I��� */
			}
		}
		else
		{
			pd->bomber_time=0;
		//	pd->ex tra_type=PLX_NONE;
		//}
		//if (/*bomb_wait*/d->bomber_time<=0)
		//{
			if (0 < pd->bombs)
			{
				if (my_pad & PSP_KEY_BOMB_CANCEL)
				{
					play_voice_auto_track(VOICE07_BOMB);
//					set_bg_alpha(100);/* ��ʂ��Â����� */	/* ���\���������������� */
					set_bg_alpha(127);/* ��ʂ��Â����� */	/* ���\���������������� */
					/* �ᑬ�{�� */
					if (my_pad & PSP_KEY_SLOW)
					{
				//	case (REIMU*2+TEISOKU_MODE):/*�Ƃ肠����*/
				//	case (MARISA*2+TEISOKU_MODE):/*�Ƃ肠����*/
				//	case (REMILIA*2+TEISOKU_MODE):/*�Ƃ肠����*/
				//	case (CIRNO*2+TEISOKU_MODE):/*�Ƃ肠����*/
				//	case (YUYUKO*2+TEISOKU_MODE):/*�Ƃ肠����*/
						pd->bomber_time = 200/*320*/ /*400*/ /*800*/;	/* �{���̗L������(�ݒ薳�G����)[���P�d�l���ς�����̂Ŕ����ɂ���] */
						item_from_bullets(SP_ITEM_05_HOSI);/* �e�S������ */
						/*player_move_add_bomber_hlaser*/player_add_hlaser_parrent(player);
				//		break;
					}
					/* �ʏ�{�� */
					else
					{
						switch ((select_player))
						{
					//	case (YUYUKO*2+TEISOKU_MODE):/*�Ƃ肠����*/
					//		pd->bomber_time = 300;			/* �{���̗L������(�ݒ薳�G����) */	/* ������̓{�X����(�h��^) */
					//		goto common_kekkai;
						case (REIMU):
						//	/*bomb_wait*/d->bomber_time 	= 200;
							pd->bomber_time = 180/*200*/;			/* �{���̗L������(�ݒ薳�G����) */	/* ������̓{�X�L��(�U���^) */
						//	pd_save_timer	= 200+30;		//�ݒ薳�G���ԁB���܂ɃV�[���h�����蔲����҂������̂�
						//	pd->ex tra_type = PLX_BOMB;
					//	common_kekkai:
							player_add_kekkai(player);		//�V�[���h�̒ǉ�
							break;
						case (YUYUKO):/*�Ƃ肠����(�����ƈႤ�����ɂ�����)*/
						case (CIRNO):/*�Ƃ肠����*/
						case (MARISA):
						//	/*bomb_wait*/d->bomber_time 	= 100;
							pd->bomber_time = 32/*100*/;			/* �{���̗L������(�ݒ薳�G����) */
						//	pd_save_timer	= 100+30/*200 5*/;	// �ݒ薳�G���ԁB�[���������Ƀ{�����Ԓ����G����Ȃ��̂͂���
						//	pd->ex tra_type = PLX_BOMB; 	/*��������������(extra_tem�L������)�{���������o���Ȃ���*/
							player_add_levarie(player);
							break;
						case (REMILIA): /* [***090220 �ǉ� */
						//	/*bomb_wait*/d->bomber_time 	= 150/*300*/ /*400*/;	/*400(150+250)�͖��炩�ɒ�������*/
							/* ���~�{���͋�������̂ŁA4������3���Ɍ��炵�A�����{���ɂ��ē��ē�����B */
							/* ���������~�{���̓{�X�ɗL���Ƃ��A������΋����B */
							pd->bomber_time = 254/* 255==0xff==4��	254==0xfe==3��==0xbf=192 2��==0x7f 150==0x96*/;
						//	pd_save_timer	= 150+30;		//�ݒ薳�G���ԁB
						//	pd->ex tra_type = PLX_BOMB;
							player_add_cross_red_parrent(player);
							break;
						}
					}
					pd_save_timer		= pd->bomber_time + USER_BOMOUT_WAIT;/*�������G����*/
				//	pd->ex tra_type 	= PLX_BOMB;
					s1->alpha			= 80;	/*������*/
					pd->core->alpha 	= 80;	/*������*/
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

					#if (1==USE_PLAYER_HOMING)
					//	d->bomber_time	= 500/*1000*/;	/* �{���̗L������(�ݒ薳�G����)[���P�d�l���ς�����̂Ŕ����ɂ���] */
					//	/*player_move_add_bomber_homing*/player_add_homing_cross_red(player);
					//	break;
					#endif //(1==USE_PLAYER_HOMING)



//
	if (PLAYER_STATE_01_HIT_BOMB==pd_player_status) return;/* ��炢�{����t�� */
//
	/*
	if (my_pad_alter & PSP_KEY_LEFT) {		direction=-1;
		if (s1->x>0) {
			if (my_pad_alter & PSP_KEY_SLOW)	s1->x-=(pd->player_speed/2);
			else								s1->x-=pd->player_speed;
		}
	}
	if (my_pad_alter & PSP_KEY_RIGHT) { 	direction=1;
		if (s1->x<GAME_WIDTH-s1->w) {	//�v���C���[��x���W < �X�N���[���̉���-�v���C���[�̉���
			if (my_pad_alter & PSP_KEY_SLOW)
				s1->x+=(pd->player_speed/2);
			else
				s1->x+=pd->player_speed*fps_fa_ctor;
		}
	}
	if ((my_pad_alter & PSP_KEY_UP) && (s1->y>0)) {
		if (my_pad_alter & PSP_KEY_SLOW)	s1->y-=(pd->player_speed/2);
		else						s1->y-=pd->player_speed;
		if (pd->weapon_power==127)	//128[***090126 	�ύX
			if (s1->y<50)				pd->bonus=0x01;
		//parsys_add(NULL,50,0,s1->x+((s1->w)>>1),s1->y+s1->h,10,90,10,100,PIXELATE,NULL);
	}
	if ((my_pad_alter & PSP_KEY_DOWN) && (s1->y<screen->h-s1->h)) {
		if (my_pad_alter & PSP_KEY_SLOW)	s1->y+=(pd->player_speed/2);
		else						s1->y+=pd->player_speed;
		if (s1->y>50)			pd->bonus&=~0x01;
	}
	*/
	//int direction=0;


	/*---------------------------------------------------------
		�v���C���[�ړ��֘A����
	---------------------------------------------------------*/
	static const signed /*int*/short shipMv[16][2] =
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
//#define BASE_SPEED_MINIMUM		(PLAYERS5*4)
//#define BASE_SPEED_MAXIMUM		(PLAYERS5*5)
//	 2,  3,  4,  7,  3, 	/* �Œᑬ�x player_speed_minimum */
//	 4,  5,  4,  7,  3, 	/* �ō����x player_speed_maximum */
	static const signed /*int*/short player_speed256[(PLAYERS5*4)] =
	{
	/* �������[�h(�ʏ펞) */
	/* REIMU */ 	t256(3.0),
	/* MARISA */	t256(3.5),
	/* REMILIA */	t256(3.5),
	/* CIRNO */ 	t256(3.5),/*�H������ᑬ�̕�������*/
	/* YUYUKO */	t256(3.0),
	/* �ᑬ���[�h(�ʏ펞) */
	/* REIMU */ 	t256(2.0),
	/* MARISA */	t256(2.5),
	/* REMILIA */	t256(2.5),
	/* CIRNO */ 	t256(7.0),/*�H������ᑬ�̕�������*/
	/* YUYUKO */	t256(2.0),
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
	};
	const signed int my_speed = player_speed256[
		select_player+
		((my_pad & PSP_KEY_SLOW)?(PLAYERS5):(0))+
		((1 > pd->bomber_time)?(0):(PLAYERS5+PLAYERS5))
		];
	s1->x256 += ((((signed int)(shipMv[((my_pad&0xf0)>>4)][0]))*(my_speed))>>8); /**fps_fa_ctor*/
	s1->y256 += ((((signed int)(shipMv[((my_pad&0xf0)>>4)][1]))*(my_speed))>>8); /**fps_fa_ctor*/
	// �݂͂�������C���B
		 if (s1->x256 < t256(0))						{	s1->x256 = t256(0); 						}
	else if (s1->x256 > t256(GAME_WIDTH) -((s1->w)<<8)) {	s1->x256 = t256(GAME_WIDTH) -((s1->w)<<8);	}
		 if (s1->y256 < t256(0))						{	s1->y256 = t256(0); 						}
	else if (s1->y256 > t256(GAME_HEIGHT)-((s1->h)<<8)) {	s1->y256 = t256(GAME_HEIGHT)-((s1->h)<<8);	}

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
					{/* REIMU MARISA REMILIA CIRNO YUYUKO */	/* ���~��������̂Œ���(+5) */
						 5,  6,  7+5,  9,  9+8, 	/* WEAPON_L1 */
						 5,  6,  6+5,  9,  8+8, 	/* WEAPON_L2 */
						 5,  5,  7+5,  9,  7+8, 	/* WEAPON_L3 */
						 5,  5,  6+5,  9,  6+8, 	/* WEAPON_L4 */
						 5,  5,  5+5,  9,  5+8, 	/* WEAPON_L5 */
					};
					/*pd->*/weapon_interval = www_tbl[(PLAYERS5*weapon_List)+select_player];
//
					play_voice_auto_track(VOICE00_SHOT);
					/*const*/static void (*bbb[PLAYERS5][WP_MAX])(SPRITE *sss) =
					{
						{	/*REIMU*/
						/* WEAPON_L1: */	player_add_single_shot,
						/* WEAPON_L2: */	player_add_dual_shot,
						/* WEAPON_L3: */	player_add_triple_fuda,
						/* WEAPON_L4: */	player_add_quad_fuda,
						/* WEAPON_L5: */	player_add_five_fuda,
						},
						{	/*MARISA*/		/* �܂������ȊO�������� */
						/* WEAPON_L1: */	player_add_single_shot,
						/* WEAPON_L2: */	player_add_dual_shot,
						/* WEAPON_L3: */	player_add_dual_shot,
						/* WEAPON_L4: */	player_add_triple_star,
						/* WEAPON_L5: */	player_add_quad_star,
						},
						{	/*REMILIA*/
						/* WEAPON_L1: */	player_add_single_shot,
						/* WEAPON_L2: */	player_add_single_shot,
						/* WEAPON_L3: */	player_add_dual_shot,
						/* WEAPON_L4: */	player_add_dual_shot,
						/* WEAPON_L5: */	player_add_dual_shot,
						},
						{	/*CIRNO*/		/*�Ƃ肠����*/
						/* WEAPON_L1: */	player_add_dual_shot,
						/* WEAPON_L2: */	player_add_dual_shot,
						/* WEAPON_L3: */	player_add_dual_shot,
						/* WEAPON_L4: */	player_add_triple_star,
						/* WEAPON_L5: */	player_add_triple_star,
						},
						{	/*YUYUKO*/		/* ���i�K���� */
						/* WEAPON_L1: */	player_add_chou,
						/* WEAPON_L2: */	player_add_chou,
						/* WEAPON_L3: */	player_add_chou,
						/* WEAPON_L4: */	player_add_chou,
						/* WEAPON_L5: */	player_add_chou,
						},
					};
					(bbb[select_player][weapon_List])(s1);
#if 0
					switch (select_player)
					{
					case REIMU:
						switch (weapon_List)
						{
						case WEAPON_L1: 	player_add_single_shot(s1); break;
						case WEAPON_L2: 	player_add_dual_shot(s1);	break;
						case WEAPON_L3: 	player_add_triple_fuda(s1); break;
						case WEAPON_L4: 	player_add_quad_fuda(s1);	break;
						case WEAPON_L5: 	player_add_five_fuda(s1);	break;
						}
						break;
					case CIRNO:/*�Ƃ肠����*/
					case MARISA:
						switch (weapon_List)
						{
						case WEAPON_L1: 	player_add_single_shot(s1); break;
						case WEAPON_L2: 	player_add_dual_shot(s1);	break;
						case WEAPON_L3: 	player_add_dual_shot(s1);	break;
						case WEAPON_L4: 	player_add_triple_star(s1); break;
						case WEAPON_L5: 	player_add_quad_star(s1);	break;
						}
						break;
					case REMILIA:
						switch (weapon_List)
						{
						case WEAPON_L1: 	player_add_single_shot(s1); break;
						case WEAPON_L2: 	player_add_single_shot(s1); break;
						case WEAPON_L3: 	player_add_dual_shot(s1);	break;
						case WEAPON_L4: 	player_add_dual_shot(s1);	break;
						case WEAPON_L5: 	player_add_dual_shot(s1);	break;
						#if (1==USE_PLAYER_WAVE)
						case WEAPON_L6: 	player_add_killray(s1); 	break;
						#endif //(1==USE_PLAYER_WAVE)
						}
						break;
					}
#endif
				}
			}
			#if 0
//			{static int extra_interval = 0;
//				if (/*pd->*/extra_interval>0)
//				{	/*pd->*/extra_interval--/*=fps_fa_ctor*/;}
//				else //if (pd->extra_interval<=0)
//				{
//					switch (pd->ex tra_type)
//					{
//					case PLX_HOMING:		player_add_homing(s);		/*pd->*/extra_interval=63/*60*/;			break;
//					case PLX_HLASER:		player_add_hlaser(s);		/*pd->*/extra_interval=63/*60*/;			break;
//					}
//				}
//			}
			#endif
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
				static int auto_aniime;
				auto_aniime++;
				auto_aniime&=3;
					 if (4==auto_anim_frame)	{	s1->anim_frame = (auto_anim_frame + auto_aniime);	}
				else if (4<auto_anim_frame) 	{	s1->anim_frame = (auto_anim_frame + 3); 			}
				else							{	s1->anim_frame = (auto_anim_frame );				}
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
	int pd_weapon;
	pd_weapon=pd->weapon_power;
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
	if (pd_weapon > (35-1))
	{
		if (0==(pd->state_flag&STATE_FLAG_08_OPTION_HIDE))	/* �I�v�V�����������Ă�� */
		{
			pd->state_flag |= STATE_FLAG_08_OPTION_HIDE;	/* �I�v�V������ǉ� */
			switch (select_player)
			{
			case YUYUKO:/*�Ƃ肠����*/
			case REIMU:/**/
			case MARISA:		re_ma_yu_add_option(player);	break;
			case CIRNO:/*�Ƃ肠����*/
			case REMILIA:		oz_add_option(player);		break;/* [***090220 �ǉ� */
			}
		}
	}
	else
	{
		pd->state_flag &= (~STATE_FLAG_08_OPTION_HIDE); 	/* �I�v�V���������� */
	}
}

/*---------------------------------------------------------
	�N���A�{�[�i�X �`�F�b�N
---------------------------------------------------------*/

void player_stage_clear(void)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
			player_add_score(adjust_score_by_difficulty((
			(player_now_stage * score(1000)) +	/* �X�e�[�W x 1000pts */
			(pd->weapon_power * score(100)) +	/* �p���[	x  100pts */
			(pd->graze_point)					/* �O���C�Y x	10pts */
		)));
	//
	pd->graze_point = 0;/* ���Z���ď����� */
}

/*---------------------------------------------------------
	�l�`�w���̃m�[�~�X�o�擾�`�F�b�N
---------------------------------------------------------*/

static void chain_check(SPRITE *s, SPRITE *t)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)s->data;
	if (30 < pd->chain_point) { pd->chain_point=30; }
	//(�m�[�~�X�{�[�i�X)�E�F�|���{�[�i�X���_�̕\��
	bonus_info_score_nodel(t, /*score_type*/(pd->chain_point));/*���������֎d�l�ύX*/t->type=SP_DELETE;
}




/*---------------------------------------------------------
	�v���C���[�����蔻��1(ITEM��p)
	-------------------------------------------------------
		s		�v���C���[
		mask	���ׂ�ΏہH
		t		�G���G�̒e���A�C�e����
---------------------------------------------------------*/
/*static*/ int effect_sound_hosi;
//�ςȖ��O�͏o���Ȃ�extern void bonus_info_add_name_label(SPRITE *src/*int x, int y*/, int info_type/*char *filename*/);

static void player_colision_check_item(SPRITE *s/*, int mask*/)
{
	/* Kollision Player <> Feind, Feindwaffe oder Bonusitem */
	SPRITE *t;	//�Ώ�
	if (NULL != (t=sprite_collision_check(s,SP_GROUP_ITEMS/*mask*/)))
	{
	//	int effect_sound_number;/* ���ʉ� */
	//	effect_sound_number = 99;/* ���ʉ��Ȃ� */
		//
		PLAYER_DATA *pd=(PLAYER_DATA *)s->data;
		switch (t->type)
		{
		/* --- �{�[�i�X�A�C�e�� --- */
		case SP_ITEM_00_P001:
			if (pd->weapon_power<127)	// 128max�łȂ��Ƃ� /*max==127==�u128�i�K�v*/
			{
				pd->weapon_power++;
				/* (MAX���ȊO��) [P]��10�_ */
				//player_add_score(score(10));
				bonus_info_score_nodel(t,SCORE_10);/*���������֎d�l�ύX*/t->type=SP_DELETE;
				//�ςȖ��O�͏o���Ȃ�bonus_info_add_name_label(t, SP_ITEM_00_P001/*"weapon.png"*/);/*���������֎d�l�ύXt->type=SP_DELETE;*/
			}
			else					// max�̎�
			{
				pd->chain_point++;
				chain_check(s, t);
			}
			check_weapon_level(pd);/* [pd->weapon_power���ύX���ꂽ�ꍇ�ɕK���s����`�F�b�N] */
			/*effect_sound_number=*/ effect_sound_hosi=1;/*play_voice_auto_track(VOICE05_BONUS);*/
			break;

		case SP_ITEM_04_P128:	// �E�F�|���A�C�e��(F)		// [***090123		�ǉ�
			goto my_128;
		case SP_ITEM_01_P008:	// �E�F�|���A�C�e��(��) 	// [***090123		�ǉ�
			if (pd->weapon_power<(120-1))
			{
				pd->weapon_power += 8;
				/* (MAX���ȊO��) [P��]��80�_ */
				bonus_info_score_nodel(t,SCORE_80);/*���������֎d�l�ύX*/t->type=SP_DELETE;
				//�ςȖ��O�͏o���Ȃ�bonus_info_add_name_label(t, SP_ITEM_00_P001/*"weapon.png"*/);/*���������֎d�l�ύXt->type=SP_DELETE;*/
			}
			else
			{
my_128:
				if (pd->weapon_power==127)/*127==128�i�K��*/
				{
					pd->chain_point+=8;
					chain_check(s, t);
				}
				else
				{
					pd->weapon_power=127;/*max==127==�u128�i�K�v*/
					t->type=SP_DELETE;/* �����܂� */
				}
			}
			check_weapon_level(pd);/* [pd->weapon_power���ύX���ꂽ�ꍇ�ɕK���s����`�F�b�N] */
			/*effect_sound_number=*/ effect_sound_hosi=1;/*play_voice_auto_track(VOICE05_BONUS);*/
			break;

		case SP_ITEM_02_BOMB:
			//player_add_bomb(t);		// [*****�{���̓R�����g�A�E�g���Ȃ���
			if (8 < pd->bombs)	{	goto add_1000pts;	}
			pd->bombs++;
			//�ςȖ��O�͏o���Ȃ�bonus_info_add_name_label(t, SP_ITEM_02_BOMB/*"firebomb.png"*/);/*���������֎d�l�ύXt->type=SP_DELETE;*/
			/*effect_sound_number=*/ effect_sound_hosi=1;/*play_voice_auto_track(VOICE05_BONUS);*/
			break;

		case SP_ITEM_03_1UP:
			if (8 < pd->zanki)	{	goto add_1000pts;	}
			pd->zanki++;
			//�ςȖ��O�͏o���Ȃ�bonus_info_add_name_label(t, SP_ITEM_03_1UP/*"energy.png"*/);/*���������֎d�l�ύXt->type=SP_DELETE;*/
			/*effect_sound_number=*/play_voice_auto_track(VOICE06_EXTEND);
			break;

		case SP_ITEM_07_SPECIAL:
			break;
		#if 0
		/* ���_�́A�����蔻��Ȃ� */
		case SP_ITEM_05_HOSI:		/*not_break;*/
			t->type=SP_DELETE;/* ���_�̂ݓ��ʏ��� */
			/*effect_sound_number=*/ effect_sound_hosi=1;/*play_voice_auto_track(VOICE05_BONUS);*/
			break;
		#endif
add_1000pts:
		case SP_ITEM_06_TENSU:
			if ( t256(GAME_HEIGHT/2) > t->y256 )	/* ��Ŏ��� 1000pts. */
			{
				//player_add_score(score(1000));
				bonus_info_score_nodel(t,SCORE_1000);/*���������֎d�l�ύX*/t->type=SP_DELETE;
				//�ςȖ��O�͏o���Ȃ�bonus_info_add_name_label(t, SP_ITEM_05_HOSI/*"plus1000.png"*/);/*���������֎d�l�ύXt->type=SP_DELETE;*/
			}
			else	/* ���Ŏ��� 100pts. */
			{
				//player_add_score(score(100));		// [***090123		�ύX
				bonus_info_score_nodel(t,SCORE_100);/*���������֎d�l�ύX*/t->type=SP_DELETE;
				//�ςȖ��O�͏o���Ȃ�bonus_info_add_name_label(t, SP_ITEM_06_TENSU/*"plus100.png"*/);/*���������֎d�l�ύXt->type=SP_DELETE;*/	// [***090123		�ύX
			}
			/*effect_sound_number=*/ effect_sound_hosi=1;/*play_voice_auto_track(VOICE05_BONUS);*/
			break;
		}
	//	if (99 != effect_sound_number)/* ���ʉ������ */
	//	{
	//		play_voice_auto_track(effect_sound_number);/* ���ʉ��炷 */
	//	}
	}
}






/*---------------------------------------------------------
	�v���C���[�����蔻��2
	-------------------------------------------------------
		s		�v���C���[
		mask	���ׂ�ΏہH
		t		�G���G�̒e���A�C�e����
---------------------------------------------------------*/
static void player_colision_check_graze(SPRITE *s/*, int mask*/)
{
	/* Kollision Player <> Feind, Feindwaffe oder Bonusitem */
	SPRITE *t;	//�Ώ�
	if (NULL != (t=sprite_collision_check(s,SP_GROUP_BULLETS/*mask*/)))
	{
	//	int effect_sound_number;/* ���ʉ� */
	//	effect_sound_number = 99;/* ���ʉ��Ȃ� */
		//
		PLAYER_DATA *pd=(PLAYER_DATA *)s->data;
		switch (t->type)
		{
		/* �v���C���[�ɓG�e�����������ꍇ�̓O���C�Y���� */
		/* --- �G�e --- */
		case SP_BULLET: 	/*not_break;*/
	//	case SP_LASER:		/*not_break;*/
	//	case SP_BIGBULLET:	/*not_break;*/
	//	case SP_BOSS02ICE:	/*not_break;*/
			is_graze = 1;/*�O���C�Y��*/
			if (0==(t->flags & SP_FLAG_GRAZE)) /* �O���C�Y��? */
			{
				t->flags |= SP_FLAG_GRAZE;/*�O���C�Y��*/
				pd->graze_point++;/*�O���C�Y��*/
				player_add_score((score(100)+score(200)*difficulty));/*score(500)*/
				/*effect_sound_number=*/play_voice_auto_track(VOICE09_GRAZE);
			}
			break;

	// (�n�߂��瓖���蔻�肪�����X�v���C�g�ɂ����̂ŗv��Ȃ��Ȃ���) 	// 2�ڂ̃X�v���C�g�͓����蔻�肪����
	//	case S P_EN_BIGBULLET_DUMMY:	break;
		}
	//	if (99 != effect_sound_number)/* ���ʉ������ */
	//	{
	//		play_voice_auto_track(effect_sound_number);/* ���ʉ��炷 */
	//	}
	}
}


/*---------------------------------------------------------
	�v���C���[�����蔻��3
	-------------------------------------------------------
		s		�v���C���[
		mask	���ׂ�ΏہH
		t		�G���G�̒e���A�C�e����
---------------------------------------------------------*/
static void player_colision_check_enemy(SPRITE *s/*, int mask*/)
{
	/* Kollision Player <> Feind, Feindwaffe oder Bonusitem */
	SPRITE *t;	//�Ώ�
	if (NULL != (t=sprite_collision_check(s,SP_GROUP_ENEMYS/*mask*/)))
	{
	//	int effect_sound_number;/* ���ʉ� */
	//	effect_sound_number = 99;/* ���ʉ��Ȃ� */
		//
		switch (t->type)
		{
		/* --- �G --- */
#if 1
		/* ���@���U�R�G�ɑ̓����肳�ꂽ�ꍇ */
	//	case SP_BOSS:		/* ���@���{�X�G�ɑ̓����肳�ꂽ�ꍇ */
	//	case SP_CHUU:		/* ���@�����^�G�ɑ̓����肳�ꂽ�ꍇ */
	//	case SP_ZAKO:		/* ���@���U�R�G�ɑ̓����肳�ꂽ�ꍇ */
		default:			//�G���̂��̂ɓ��������ꍇ
			#if 1
			if (0!=((t->type)&(SP_GROUP_ENEMYS)))		// �G���̂��̂ɓ��������ꍇ
			#endif
			{
				pd_player_status	= PLAYER_STATE_01_HIT_BOMB; 	// [***090125		�ǉ�
				pd_save_timer		= player_fix_status[BASE_HIT_BOMB_WAIT+select_player]/*0*/;
			//	pd->enemy			= t;/* ���������G���o���Ƃ� */
				/* ���������G�́A�K�����ʖ�ł͖����̂ŁA���������G�̗̑͂����炷�ׂ� */
				/**/
				/*effect_sound_number=*/play_voice_auto_track(VOICE04_SHIP_HAKAI);		// [***090127	�ύX��
			}
			#if 1
			/* �{�������͗v��Ȃ��͂��B�H�H�H�H���ݗǂ��킩��Ȃ��o�O������ */
			/* ���̂��u�ςȂ́v�ɂ������ăn���O�A�b�v���� */
			else
			{
				t->type=SP_ITEM_05_HOSI;/* �u�ςȂ́v�͋����I�ɓ��_�ɁA�����Ⴆ */
			}
			#endif
#endif
		}
	//	if (99 != effect_sound_number)/* ���ʉ������ */
	//	{
	//		play_voice_auto_track(effect_sound_number);/* ���ʉ��炷 */
	//	}
	}
}





	//SDL_Surface *spimg;
	//is_graze=0;

	//	case SP_ITEM_07_SPECIAL:
	//		if (pd->player_speed < player_fix_status[BASE_SPEED_MAXIMUM+select_player]/*pd->player_speed_maximum*/)
	//		{	;	}	else	{	goto add_1000pts;	}
	//		pd->player_speed++;
	//		//�ςȖ��O�͏o���Ȃ�bonus_info_add_name_label(t, SP_ITEM_07_SPECIAL/*"speed.png"*/);/*���������֎d�l�ύXt->type=SP_DELETE;*/
	//		/*effect_sound_number=*/play_voice_auto_track(VOICE05_BONUS);
	//		break;


//add_1000pts:
//			player_add_score(score(1000));
//			bonus_info_add_name_label(t, SP_ITEM_05_HOSI/*"plus1000.png"*/);/*���������֎d�l�ύXt->type=SP_DELETE;*/
//			break;

			//spimg=sprite_getcurrimg(c);
			//parsys_add(spimg, t->w,1, t->x,t->y, 5, 0, 0, 80, LINESPLIT, NULL);
			//SDL_FreeSurface(spimg);


		#if 0
		case SP_ITEM_EXTRA_SHIELD:	//�E�F�|���A�C�e��(��)
			if (pd->bomber_time > 0)	{	goto add_1000pts;	} //�{�����͂��������Ă�1000�|�C���g���邾������
			//
		//	sprite_remove_all_type(SP_SHOT_ZAKO);
		//	pd->ex tra_type=PLX_NONE/*PLX_SHIELD*/; 	//�E�F�|���A�C�e��(��)
		//	pd->bomber_time=30;
			if (pd->weapon_power<(112-1))	// [***090123		�ύX
			{
				pd->weapon_power+=16;
				bonus_info_add_name_label(t, SP_ITEM_00_P001/*"weapon.png"*/);/*���������֎d�l�ύXt->type=SP_DELETE;*/
			}
			else
			if (pd->weapon_power==127)	// [***090123		�ύX/*max==127==�u128�i�K�v*/
			{
				pd->chain_point+=16;//(�m�[�~�X�{�[�i�X)
				goto add_1000pts;
			}
			else
			{
				pd->weapon_power=127;		// [***090123		�ύX/*max==127==�u128�i�K�v*/
				t->type=SP_DELETE;/* �����܂� */
			}
			check_weapon_level(pd);/* [pd->weapon_power���ύX���ꂽ�ꍇ�ɕK���s����`�F�b�N] */
			/*effect_sound_number=*/play_voice_auto_track(VOICE05_BONUS);
			break;
		#endif


		#if 0
//		case SP_ITEM_EXTRA_HOMING:
//			if (pd->bomber_time > 0)	{	goto add_1000pts;	} //�{�����͂��������Ă�1000�|�C���g���邾������
//			//
//		//	sprite_remove_all_type(SP_SHOT_ZAKO);
//			pd->ex tra_type 	= PLX_BOMB/*PLX_HOMING*/;
//			pd->bomber_time 	= 800;
//			bonus_info_s_text(t, "homing missiles",FONT04);
//			bonus_info_add_name_label(t, /*"extra.png"*/);/*���������֎d�l�ύXt->type=SP_DELETE;*/
//			//spimg=sprite_getcurrimg(t);
//			//parsys_add(spimg, t->w,1, t->x,t->y, 5, 0, 0, 80, LINESPLIT, NULL);
//			//SDL_FreeSurface(spimg);
//			/*effect_sound_number=*/play_voice_auto_track(VOICE05_BONUS);
//			break;
//
//		case SP_ITEM_EXTRA_HLASER:
//			if (pd->bomber_time > 0)	{	goto add_1000pts;	} //�{�����͂��������Ă�1000�|�C���g���邾������
//			//
//		//	sprite_remove_all_type(SP_SHOT_ZAKO);
//			pd->ex tra_type 	= PLX_BOMB/*PLX_HLASER*/;
//			pd->bomber_time 	= 1000;
//			bonus_info_s_text(t, "homing laser",FONT04);
//			bonus_info_add_name_label(t, /*"extra.png"*/);/*���������֎d�l�ύXt->type=SP_DELETE;*/
//			//spimg=sprite_getcurrimg(t);
//			//parsys_add(spimg, t->w,1, t->x,t->y, 5, 0, 0, 80, LINESPLIT, NULL);
//			//SDL_FreeSurface(spimg);
//			/*effect_sound_number=*/play_voice_auto_track(VOICE05_BONUS);
//			break;
		#endif

/*---------------------------------------------------------
	�v���C���[�ړ�
---------------------------------------------------------*/
void player_loop_quit(void)
{
	SPRITE *s1;
	s1 = player;
	PLAYER_DATA *pd=(PLAYER_DATA *)s1->data;
	pd->state_flag |= STATE_FLAG_14_GAME_LOOP_QUIT;
	pd->state_flag &= (~STATE_FLAG_08_OPTION_HIDE); 	/* �I�v�V���������� */
//
	pd_save_timer		= 150;	/* 150	 120�ł͎኱�Z�� 100 */
	pd_player_status	= PLAYER_STATE_05_GAME_OUT;/* GAME_OUT�� */
	s1->flags			&= (~(SP_FLAG_VISIBLE));		// ���t���O��OFF(�s��)
	pd->core->flags 	&= (~(SP_FLAG_VISIBLE));		// ������// ��������
//	pd->core->alpha 	= 0;			// �����������߂ɓ��ꂽ���ǈӖ�����������������Ȃ��B	// �Q�[���I�[�o�[���́��̕\������
}

static void player_fukkatsu(SPRITE *s1)
{
	pd_save_timer		= 120/*150-120*/;									// ���G����
	pd_player_status	= PLAYER_STATE_03_SAVE_01;	// ���G��ԁH

	s1->flags			|= (SP_FLAG_VISIBLE);		// ���t���O��ON(��)
	s1->alpha			= 80;	/*������*/
//
	PLAYER_DATA *pd=(PLAYER_DATA *)s1->data;
	pd->core->flags 	|= (SP_FLAG_VISIBLE);		// ���\��
	pd->core->alpha 	= 80;	/*������*/
	pd->bombs			= player_fix_status[BASE_BOMBS+select_player]/*3*/; 		// �{���������̏�����
	pd->chain_point 	= 0;	// pd->chain_point(�m�[�~�X�{�[�i�X)�̏����� // �ǂꂾ���A��(�m�[�~�X)��wepon_up���������
//
	check_weapon_level(pd);/* [pd->weapon_power���ύX���ꂽ�ꍇ�ɕK���s����`�F�b�N] */
}

static void player_explode(SPRITE *s1)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)s1->data;
		{
			// �����ŃA�C�e����f���o���悤�ɂ���΂�����ۂ��Ȃ邩�� // �Ƃ����킯�Ŏ��� // [***090124������ƒǉ�
			/* GAME_OUT�ł��A�C�e���f���ׂ�(�Ōゾ���f���Ȃ��͈̂�a��������) */
		//
			/* �ɒ[�Ɏキ�Ď��񂾏ꍇ�̋~�ύ�: ��肢�l�͏��Ղ� FULLPOWER �ɂȂ�A����ł��{������Ȃǂ� POWER ���ێ�����̂ŁA */
			/* �ɒ[�Ɏキ�Ă͎��ȂȂ��B �ɒ[�Ɏキ�Ď��ʂ̂͌��\���肾�Ǝv���̂ŁA���̏ꍇ FIRE_POWER_G (8) x 6 == 48/128 (�S���W�߂�37%) �ɂ����B */
		//
			item_create(s1, (((30-1) > pd->weapon_power)?(SP_ITEM_01_P008):(SP_ITEM_00_P001)), 5, ITEM_MOVE_FLAG_06_RAND_XY);
			item_create(s1, SP_ITEM_01_P008, 1, ITEM_MOVE_FLAG_06_RAND_XY);
			//
			/* ���~���A ����\�́F������������ƕK���{�����o�� */
			if (REMILIA==select_player) 	/* ���~�̏ꍇ */
			{
				/* ������������� */
				if (0 != pd->bombs)
				{
					/* �g���ĂȂ����̎����Ă�{����f���o�� */
					item_create(s1, SP_ITEM_02_BOMB, pd->bombs, ITEM_MOVE_FLAG_06_RAND_XY);
				}
			}
		}
//			pd->explode 		= 0;
//		if (0==pd->explode)
		{
		//	spimg=sprite_getcurrimg(t); parsys_add(spimg, 2,2, t->x,t->y, 10, 270, 10, 30, EXPLODE|DIFFSIZE, NULL); 		SDL_FreeSurface(spimg);spimg=NULL;
		//	spimg=sprite_getcurrimg(s1);parsys_add(spimg, 2,2, t->x,t->y, 10,  90, 10, 30, EXPLODE|DIFFSIZE, &pd->explode); SDL_FreeSurface(spimg);spimg=NULL;

		//	explosion_add_type(t->x256+t256(5),t->y256+t256(5),/*0,*/EXPLOSION_MINI00);
		//	t->type=SP_DELETE;/* �����܂� */			/* ���������G���� */
		//	pd->explode=0;
		//	pd->bonus=0;

			//play_voice_auto_track(VOICE04_SHIP_HAKAI);	// [***090127	�ꏊ���ړ�����B
			/* �����G�t�F�N�g */

			#if 0/* ���������G�͓|���Ȃ��G��������Ȃ��̂ŁA�����ŕK�������̂͂܂����C������ */
			/* ���������G���� */
			explosion_add_type(pd->enemy->x256+t256( 5), pd->enemy->y256+t256( 5), /*0,*/ EXPLOSION_MINI00);
			pd->enemy->type 	= SP_DELETE;/*???*/
			#endif
			/* �������� */
			explosion_add_type(s1->x256+t256( 5), s1->y256+t256( 5), /*0,*/ EXPLOSION_MINI00);
			explosion_add_type(s1->x256+t256( 5), s1->y256+t256(20), /*0,*/ EXPLOSION_MINI00);
			explosion_add_type(s1->x256+t256(20), s1->y256+t256( 5), /*0,*/ EXPLOSION_MINI00);
			explosion_add_type(s1->x256+t256(20), s1->y256+t256(20), /*0,*/ EXPLOSION_MINI00);
			/* �v���C���[�ʒu�̏����� */
			s1->x256 = t256(GAME_WIDTH/2)-((s1->w)<<7);
			s1->y256 = t256(220/*GAME_HEIGHT*/);/*240*/
			/*	*/
		//	pd->ex tra_type 	= PLX_NONE; �p�~
			pd->bomber_time 	= 0;			/* �����������������Ŗ�����(&�`���m)�{�����ĂȂ��Ȃ��Ă� */
		//	pd->state_flag		= STATE_FLAG_00_NONE;/*???*/
			pd->state_flag		&= (~(	STATE_FLAG_01_PLAYER_UP_AUTO_GET_ITEM | 	/* �I��� */
										STATE_FLAG_02_BOMB_AUTO_GET_ITEM |			/* �I��� */
										STATE_FLAG_03_SCORE_AUTO_GET_ITEM));		/* �I��� */

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
				pd->weapon_power -= (difficulty*4)+12;	// ���탌�x���̒ቺ
				if ( pd->weapon_power<0)	{	pd->weapon_power=0;}
				player_fukkatsu(s1);
			}
		}

}
static void player_move_other(SPRITE *s1)
{
	pd_save_timer--;
	if (2 > pd_save_timer)/* ���̏�ԂɈڍs */
	{
		PLAYER_DATA *pd=(PLAYER_DATA *)s1->data;
		switch (pd_player_status)
		{
		case PLAYER_STATE_01_HIT_BOMB:/* ��炢�{����t�� */				// [***090125		�ǉ�
			/* ���S�m�� */
			pd->state_flag &= (~STATE_FLAG_16_KEY_CONTROL); 	/* �L�[���͖��� */
			player_explode(s1);/*�v���C���[����*/
			break;

		case PLAYER_STATE_03_SAVE_01:/*�v���C���[�������P(��ʊO���畜���n�_�֓_�ł��Ȃ���ړ���)*/
		//������Ƃ����҂�����
		//	if (s1->y256 < t256(220)/*240*/)	//������ƑO�ɏo�Ă���
			{
				pd->state_flag |= STATE_FLAG_16_KEY_CONTROL;	/* �L�[���͗L�� */
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
			s1->alpha			= 255;
		//
			pd->core->alpha 	= 255;
			break;

		case PLAYER_STATE_05_GAME_OUT:/* GAME_OUT�� */
			psp_loop=(ST_WORK_GAME_OVER|0);//newsta te(ST_GAME_OVER,0,1);
			break;
		}
	}
}
static void player_move(SPRITE *s1)
{
	{	static int frame_aaa;
		frame_aaa++;
		frame_aaa&=7;
		if (0==frame_aaa)
		{
			if (1==effect_sound_hosi)
			{
				effect_sound_hosi=0;
				play_voice_auto_track(VOICE05_BONUS);
			}
		}
	}
	PLAYER_DATA *pd=(PLAYER_DATA *)s1->data;
	pd->state_flag &= (~STATE_FLAG_15_KEY_SHOT);	/* off */
	if ((pd->state_flag & STATE_FLAG_16_KEY_CONTROL))
	{
		player_keycontrol(s1);
	}
	if (0 != pd_save_timer)
	{
		player_move_other(s1);
	}
	else
	{
//	case PLAYER_STATE_00_NORMAL:/*�ʏ폈��*/
		player_colision_check_graze(s1/*,SP_GROUP_BULLETS*/);
		//
		if (0==(pd->state_flag & STATE_FLAG_06_IS_SCRIPT))		/* �X�N���v�g���쒆�́A�G�����蔻��͂Ȃ� */
		{
			player_colision_check_enemy(s1/*,SP_GROUP_ENEMYS*/);
		}
	}
	/* �A�C�e���͏�Ɏ��� */
	player_colision_check_item(s1/*,SP_GROUP_ITEMS*/);
}
			#if 0/* �_�ŃE�U�C */
			//	if (0==((int)(s1->y/10)&(2-1)/*%2*/))		// �_�ł��邩�ȂƎv���ē���Ă݂����ǂ��Ȃ������恨�����������Ă��B
				if (0==((s1->y256)&((2*8*256)-1)))	// �_�ł��Ă��
				{
					s1->alpha		= 80;
					pd->core->alpha = 80;
				}
				else
				{
					s1->alpha		= 0/*255*/;
					pd->core->alpha = 0/*255*/;
				}
			#endif

/*---------------------------------------------------------
	�v���C���[������
---------------------------------------------------------*/

extern int practice_mode;
void player_init(void)
{
	player					= sprite_add_res( BASE_SHIP_MED_PNG+select_player );		/* [***090220 �ǉ� */
	player->type			= SP_PLAYER;
	player->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
	player->callback_mover	= player_move;
	player->x256			= (t256(GAME_WIDTH/2));
	player->y256			= (t256(GAME_HEIGHT))-((player->h)<<8);
	player->anim_frame		= 5;
	PLAYER_DATA *pd;
	pd						= mmalloc(sizeof(PLAYER_DATA));
	player->data			= pd;

	pd->bomber_time 		= 0;	/*==bomb_wait*/
	pd->graze_point 		= 0;
	pd->zanki				= /*pd_base_zanki*/ player_fix_status[BASE_LIVES+select_player];
	pd->my_score			= score(0);
	pd->state_flag			= STATE_FLAG_00_NONE;
//	if (MARISA==select_player)		{	pd->state_flag		|= STATE_FLAG_04_IS_MARISA; 	}	/* �������͏�Ɏ������W */
	pd->boss				= NULL;
	pd->core				= player_add_core(player); // ���̒ǉ�

	weapon_List 			= 0;
	/* ���K���[�h�̏ꍇ�̓t���p���[�Ŏn�߂�(���̑���N���A��̃C�x���g������Ȃ�) */
	pd->weapon_power		= (0==practice_mode)?(0):(127);
	player_fukkatsu(player);
}
