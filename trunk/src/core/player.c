#include "player.h"

#define rand_percent(aaa) ((unsigned char)(rand())<=(unsigned char)( (aaa*256)/100 ) ) /* aa%�̊m���� */

extern SPRITE *sprite;
extern int select_player;


int player_now_stage;		//
SPRITE *player;

enum _player_state
{
	PL_NORMAL,
	PL_HIT_BOMB,		// [***090125		�ǉ�:PL_HIT_BOMB
	PL_EXPLODE,
	PL_RESET,
	PL_SAVE
};

//typedef struct
//{
//	int strength;/*union WEAPON_BASE*/
//	int angle512;
//	double speed;
//} PLAYER_BASE_STRUCT;


typedef struct
{
	int strength;/*union WEAPON_BASE*/
	/*double*/double/*int*/ vx; 	/* vector x */
	/*double*/double/*int*/ vy; 	/* vector y */
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
	double speed;
	double distance;	/*����(distance)*/
} PL_KILLRAY_DATA;
#endif //(1==USE_PLAYER_WAVE)

typedef struct
{
	int strength;/*union WEAPON_BASE*/
	/*double*/int angle512;
	double speed;
//
	int nr;
	int state;
	int time_out;	/*double range;*/		/* �ݒ莞�ԓ��Ɍ������Ȃ��ꍇ�͂����܂� */
	SPRITE *target_obj/*int target_id*/;
//	SPRITE *tg_sprite;/* �^�[�Q�b�g�ڕW�\���}�[�J�[ */	/*PL_HOMING_DATA�̂�*/
} PL_HOMING_DATA;/*==PL_HLASER_DATA*/

typedef struct
{
	int strength;/*union WEAPON_BASE*/	/* [***090214 �ǉ� */
	/*double*/int angle512;
	double speed;
	int radius; 	/*���a(radius)*/
} PL_SHIELD_DATA;

typedef struct
{
	int strength;/*union WEAPON_BASE*/	/* [***090214 �ǉ� */
	/*double*/int angle512;
	int ani_speed;
	int option_id/*pos*/;/*option_id*/
	int w_wait;
//
//	int length;
//	int state;
} PL_OPTION_DATA;

#define OPTION_ID_99_LEFT	(-1)/*��-1 0*/
#define OPTION_ID_01_RIGHT	( 1)/*��+1 1*/

typedef struct
{
	int strength;/*union WEAPON_BASE*/	/* [***090214 �ǉ� */
	/*double*/int angle512;
	int ani_speed;
	int option_id;
	int w_wait;
//
//	int state1;//���ʂȂ̂ŁA�����ɂ͗v��Ȃ�
	int state2;
	int offset_x;		/* �~�̔��a�ƌ��p */
	int offset_y;
	/*double*/int f_angle512;	/* ���~�̉������ꍇ�́A��]�p�x */	/* FORMATION_01: ���~�̎�������� */

	int slow_count;
//	/*double*/int m_angle512;	/* �p�~ */
//	struct _sprite *next;
} REMILIA_OPTION_DATA;

	/*---------------------------------------------------------
		�e�v���C���[�̐��\��
	---------------------------------------------------------*/

#define BASE_BOMBS			(4*0)
#define BASE_HIT_BOMB_WAIT	(4*1)
#define BASE_LIVES			(4*2)
#define BASE_SPEED_ANIME	(4*3)
#define BASE_SPEED_MINIMUM	(4*4)
#define BASE_SPEED_MAXIMUM	(4*5)
#define BASE_MAX			(4*6)
static Uint8 player_status[BASE_MAX]=
{/* REIMU MARISA REMILIA CIRNO */
	 3,  3,  3,  3, 	/* �������̃{���� */
	16,  8,  4,  9, 	/* ��炢�{���̎�t���� hit_bomb_wait */
	 3,  4,  2,  9, 	/* �J�n���̎c��`�����X */
	 4,  2,  3,  1, 	/* �A�j���[�V�������x */

	 2,  3,  4,  9, 	/* �Œᑬ�x player_speed_minimum */
	 4,  6,  4,  9, 	/* �ō����x player_speed_maximum */
};
//	 3,  2,  1,  9, 	/* �������̃{���� */

//	 3,  4,  4,  9, 	/* �Œᑬ�x player_speed_minimum */
//	 6,  8,  7,  9, 	/* �ō����x player_speed_maximum */
//	 3,  4,  4,  9, 	/* �Œᑬ�x player_speed_minimum */
//	 6,  8,  4,  9, 	/* �ō����x player_speed_maximum */

/*---------------------------------------------------------
	�v���C���[�A��������p�R�A�A��������
---------------------------------------------------------*/

static int is_graze;	//player1���e���ɐG�ꂽ���H

static void player_move_core(SPRITE *s2)
{
	s2->x=player->x+((player->w-s2->w)>>1)-1;
	s2->y=player->y-s2->h+20;
	if (is_graze)	//�v���C���[�X�v���C�g���e�ɐG��Ă��邩
	{
		if (((PLAYER_DATA *)player->data)->state==PL_NORMAL)
		{
			SPRITE *c;
			if ((c=sprite_colcheck(s2,SP_SHOW_ENEMY_WEAPONS))!=NULL)
			{
				/* �K�v�̂Ȃ�����e�̎�ނ𑝂₳�Ȃ��ŉ������B�x���Ȃ�܂��B */
				/* ����ł͕\���v���C�I���e�B���炢�����K�v�ȃP�[�X�������̂ŁB */
				switch (c->type)	//�e�e�̏����B�e�̎�ނ�ǉ������ꍇ�B�����ɂ��ǉ�����K�v����B
				{
				case SP_EN_BULLET:				/*not_break;*/
			//	case SP_EN_LASER:				/*not_break;*/
			//	case SP_EN_BIGBULLET:			/*not_break;*/
			//	case SP_EN_BOSS02ICE:			/*not_break;*/
					playChunk(4);		// [***090127	�ύX��
					((PLAYER_DATA *)player->data)->state=PL_HIT_BOMB;
					((PLAYER_DATA *)player->data)->enemy=c;/* ���������G���o���Ƃ� */
					break;
				}
			}
		}
	}
	is_graze=0;
}


/*---------------------------------------------------------
	�v���C���[����̂����蔻��
---------------------------------------------------------*/

static void weapon_colcheck(SPRITE *s, /*int angle,*/ int destroy, int check_bullets)
{
	//s 	��l���̒e
	SPRITE *c;		//�G�X�v���C�g
//	SDL_Surface *spimg;
	ENEMY_BASE *d;
	WEAPON_BASE *b=(WEAPON_BASE *)s->data;
//	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;

	/* wenn flag check_bullets gesetzt ist, werden auch gegnerische schuesse getroffen */
	if (check_bullets)
	{
		if ((c=sprite_colcheck(s,SP_SHOW_ENEMY_WEAPONS))!=NULL) 	//�G�̒e�Ȃ̂��H
		{
			playChunk(2);
			//spimg=sprite_getcurrimg(c);
			//parsys_add(spimg, 2,2, c->x,c->y, 10, angle, 10, 30, EXPLODE|DIFFSIZE, NULL);
			//SDL_FreeSurface(spimg);
			c->type=SP_DELETE;
		}
	}

	if ((c=sprite_colcheck(s,SP_SHOW_ENEMYS))!=NULL)		//�G���̂��̂Ȃ̂��H
	{
	//	if (s->type==SP_PL_HOMING)
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
		// c->flags&=~SP_FLAG_PARANOIA;
		// ???

		switch (c->type)
		{
		//�{�X�ɓ��������̂��V�[���h�������ꍇ�̓_���[�W�������s��Ȃ�
		#if 0
		case SP_EN_BOSS01:	if (s->type!=SP_PL_SHIELD)	{	enemy_boss01_hitbyweapon(c,s/*,angle*/);	}	break;
		case SP_EN_BOSS02:	if (s->type!=SP_PL_SHIELD)	{	enemy_boss02_hitbyweapon(c,s/*,angle*/);	}	break;
		case SP_EN_BOSS03:	if (s->type!=SP_PL_SHIELD)	{	enemy_boss03_hitbyweapon(c,s/*,angle*/);	}	break;
		case SP_EN_BOSS04:	if (s->type!=SP_PL_SHIELD)	{	enemy_boss04_hitbyweapon(c,s/*,angle*/);	}	break;	// [***090127		�ǉ�
		#else
		case SP_EN_BOSS:	if (s->type!=SP_PL_SHIELD)	{	(((PLAYER_DATA *)player->data)->callback_boss_hitbyweapon)(c,s/*,angle*/);	}	break;
		#endif
		case SP_EN_GFAIRY:	if (s->type!=SP_PL_SHIELD)	{	enemy_nonshield_hitbyweapon(c,s/*,angle*/); 	}	break;

		default:
			d=(ENEMY_BASE *)c->data;
			/* �ɂƂ�o�O�킩���̂ŁA�Ȃ������Ȓl�̏ꍇ���Ȃ� */
			if (0<b->strength)	/* ���l�� */
			{
				if (20>b->strength) /* �Q�O�����̏ꍇ */
				{
					d->health -= b->strength;	/* ���������� */
				}
			}
			//spimg=sprite_getcurrimg(c);
			if (d->health>0)
			{
			//	playChunk(2);
			//	parsys_add(spimg, 2,2, c->x,c->y, 10, 0, 0, 30, EXPLODE|DIFFSIZE, NULL);
			}
			else
			{
				playChunk(8);
				explosion_add(c->x,c->y+5,0,EXPLOSION_ZAKO04/*EXPLOSION_MINI00*/);/*�U�R���Ŕ���*/
				//parsys_add(spimg, 2,2, c->x,c->y, 10, angle, 10, 50, EXPLODE|DIFFSIZE, NULL);
				int put_item;
				int up_flags;
				put_item=99;
				up_flags=(s->type==SP_PL_SHIELD)?(BONUS_FLAG_COLLECT|BONUS_FLAG_RAND_XY):(0);
				switch (c->type)	// [***090123		�ǉ�
				{
				case SP_EN_GREETER: 	if (rand_percent(30))	{
											if (rand_percent(50))	{	put_item=(SP_BONUS_00_FIREPOWER&0xff);}
											else					{	put_item=(SP_BONUS_06_COIN&0xff);}
										}	break;
				case SP_EN_MING:		if (rand_percent(30))	{
											if (rand_percent(50))	{	put_item=(SP_BONUS_00_FIREPOWER&0xff);}
											else					{	put_item=(SP_BONUS_06_COIN&0xff);}
										}	break;
				case SP_EN_GROUNDER:	if (rand_percent(30))		{	put_item=(SP_BONUS_00_FIREPOWER&0xff);}
										if (rand_percent(30))		{	bonus_multi_add(c,/*->x+ 5, c->y+5,*/ SP_BONUS_06_COIN, 	 1, (up_flags) );}
										if (rand_percent(30))		{	bonus_multi_add(c,/*->x+10, c->y-5,*/ SP_BONUS_00_FIREPOWER, 1, (up_flags) );}
											break;
				case SP_EN_BADGUY:		if (rand_percent(10))		{	put_item=(SP_BONUS_07_ADDSPEED&0xff);}		break;
				case SP_EN_SPLASH:									{	put_item=(SP_BONUS_06_COIN&0xff);}			break;
				case SP_EN_FAIRY:	//	if (rand_percent(30))		{	put_item=(c->x, c->y, SP_BONUS_00_FIREPOWER&0xff);}
										put_item=(rand()&0x07);
											break;
				}
				if (99!=put_item)
				{
					bonus_multi_add(c, (put_item|SP_BONUS_00_FIREPOWER), 1, (up_flags) );
				}
				c->type=SP_DELETE;
				bonus_info_any_score(c, ((d->score)*(/*pd->now_stage*/player_now_stage/*level*/)) );
			}
			//SDL_FreeSurface(spimg);
			break;
		}
	}
}

/*---------------------------------------------------------
	�v���C���[�A�j�e�̈ړ�
---------------------------------------------------------*/

static void player_move_needle(SPRITE *s) // �얲 �� ������ �� �`���m �� ������
{
	PL_SHOT_DATA *data=(PL_SHOT_DATA *)s->data;
	s->x += data->vx;//co_s512((data->angle512))*data->speed/**fps_fa_ctor*/;
	s->y += data->vy;//si_n512((data->angle512))*data->speed/**fps_fa_ctor*/;
	if (REMILIA==select_player)
	{	int rand_int;
		rand_int=rand();
		s->x += (rand_int&1);
		s->x -= ((rand_int&2)>>1);
	}
	if (s->y < -10)
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
//static void player_move_shot(SPRITE *s)	// �얲 �� ������ �� �`���m �� ������
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
	s->x+=co_s512((data->angle512))*data->speed*fps_fa_ctor;
	s->y+=si_n512((data->angle512))*data->speed*fps_fa_ctor;
	data->distance+=data->speed*fps_fa_ctor;
	if ((data->distance>20)&&(s->aktframe<11))
	{
		data->distance=0;
		s->aktframe++;
	}
	if (s->y<-10)
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
	s->x+=co_s512((data->angle512))*data->speed*fps_fa_ctor;
	s->y+=si_n512((data->angle512))*data->speed*fps_fa_ctor;
	if ((s->x<0-100)||(s->x>100+GAME_WIDTH)||
		(s->y<0-100)||(s->y>100+GAME_HEIGHT))
	{
		s->type=SP_DELETE;
	}
	else
	{
		weapon_colcheck(s,/*data->angle512,*/1,0);
	}
	if (k>0)	{k--;}else{data->speed++;k=5;}
}
#endif //(1==USE_PLAYER_BOMBER)

/*---------------------------------------------------------
	�G��T���q�֐�
---------------------------------------------------------*/
static /*int*/SPRITE *search_enemy_by_sprite(void)
{
	/* Suche Enemy-Sprite, das noch nicht verfolgt wird, liefert SPRITE-ID oder -1 */
	SPRITE *s=sprite;/* �X�v���C�g ���X�g�̐擪 ����T�� */
	while (NULL != s)/* �X�v���C�g ���X�g�̍Ō�܂Œ��ׂ� */
	{
		if ((s->type&SP_SHOW_ENEMYS) && 		/* �v���C���[�ɂƂ��Ă̓G(�U�R��{�X) */
			(!(s->flags&SP_FLAG_PARANOIA)) &&	/* ����t���O�������G(�ʂ̗U���e�������Ă��Ȃ��G) */
			(s->flags&SP_FLAG_VISIBLE)) 		/* �g�p���̓G */
		{
			if ((s->x>0) && (s->x<GAME_WIDTH-s->w) &&
				(s->y>0) && (s->y<GAME_HEIGHT-s->h))	/* �\����ʓ��̓G */
			{
				return s/*->id*/;/*��������*/
			}
		}
		s=s->next;
	}
	return NULL/*-1*/;/*������Ȃ�����*/
}

/*---------------------------------------------------------
	�v���C���[�A�U���e�̈ړ�
---------------------------------------------------------*/

#if (1==USE_PLAYER_HOMING)
//static void player_move_homing(SPRITE *s) {}
#endif //(1==USE_PLAYER_HOMING)
static void player_move_hlaser(SPRITE *s)
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
		else			{	data->angle512-=deg_360_to_512(8)/**fps_fa_ctor*/; }	/* �����ɉ�]������B/ rechte Rekete: rot. gegen UZ */
		//
		#if 0
		if (data->angle360<0)		{	data->angle360+=360;	}
		if (data->angle360>360) {	data->angle360-=360;	}
		#else
		mask512(data->angle512);
		#endif
		//
		if ( (1.0) < data->speed )
		{
			data->speed -= /*(int)*/(0.5);/*����*/
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
		if (data->target_obj/*target_id*/ != NULL/*-1*/)
		{
			target=data->target_obj/*sprite_get_by_id (data->target_id)*/;
			if (target!=NULL)
			{
				data->state=3;						/* ���� */
				target->flags|=SP_FLAG_PARANOIA;	/* �u������Ȃ��ł���v�t���OON */
			//	tg->flags|= SP_FLAG_VISIBLE;	/* �^�[�Q�b�g�ڕW�\���}�[�J�[ */
			}
		//	else
		//	{
		//	//	tg->flags&= ~SP_FLAG_VISIBLE;	/* �^�[�Q�b�g�ڕW�\���}�[�J�[ */
		//	}
		}
		break;
	case 3: /* Ziel verfolgen */
		if (/*data->target_id*/data->target_obj == NULL/*-1*/)	/* ������Ȃ� */
		{
		//	/* ...orz ���肦�˂� */ error(ERR_WARN,"homing: target_id==-1&&state==3 ->back to state 2 (this shouldn't happen)");
			data->state=2;	/* �d���Ȃ��߂� */
		}
		else
		{
			if ( (12.0) > data->speed)
			{
				data->speed += /*(int)*/(0.5);/*����*/
			}
			target=data->target_obj/*sprite_get_by_id (data->target_id)*/;
			if (//(target!=NULL) && 	/* �d�����Ă�̂Ŕr�� */
				(target->flags&SP_FLAG_VISIBLE) &&
				(target->x>0) &&
				(target->x<GAME_WIDTH-target->w) &&
				(target->y>0) &&
				(target->y<GAME_HEIGHT-target->h))
			{
				{int ta512;
					ta512=atan_512(target->y-s->y,target->x-s->x);
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
				if (data->angle360<0)		{	data->angle360+=360;}
				if (data->angle360>360) {	data->angle360-=360;}
				#else
				mask512(data->angle512);
				#endif
			//	tg->x=target->x+((target->w-tg->w)>>1)+rand()%6-3;	/* �^�[�Q�b�g�ڕW�\���}�[�J�[ */
			//	tg->y=target->y+((target->h-tg->h)>>1)+rand()%6-3;	/* �^�[�Q�b�g�ڕW�\���}�[�J�[ */
			}
			else
			{
				/* Ziel wurde anderweitig vernichtet, neues Ziel suchen */
				data->target_obj=NULL/*data->target_id=-1*/;		/* ��ʊO�ɓ�����ꂽ�� */
				data->state=2;	/* �߂� */
			//	tg->flags&= ~SP_FLAG_VISIBLE;	/* �^�[�Q�b�g�ڕW�\���}�[�J�[ */
			}
		}
		break;
	}
	#if 1
	/* (�����K�v�Ȃ�����)�O�̈׃}�X�N */
	mask512(data->angle512);
	#endif
	s->x+=co_s512((data->angle512))*data->speed/**fps_fa_ctor*/;
	s->y+=si_n512((data->angle512))*data->speed/**fps_fa_ctor*/;
	#if 0
	/* homing */
	//	{	double dangle;
	//		dangle=data->angle+180;
	//		if (dangle>360) {	dangle-=360;}
	//		//parsys_add(NULL,20,0,s->x+((s->w)>>1),s->y+((s->h)>>1),20,dangle,30,10,PIXELATE,NULL);
	//	}
		#if 0
		s->aktframe=(((int)( data->angle360*20))/360) % 20;
		#else
		s->aktframe=(((int)( data->angle512*10))>>8);
		#endif
	#endif
	weapon_colcheck(s,/*data->angle,*/1,0);

	data->time_out--/*range-=fps_fa_ctor*/;
	if (data->time_out/*range*/<=0) /* ���Ԑ؂�H / max. Reichweite erreicht? */
	{
		s->type=SP_DELETE;		/* ���Ԑ؂�Ȃ̂ł����܂� */
	//	tg->type=SP_DELETE; 	/* �^�[�Q�b�g�ڕW�\���}�[�J�[*/
		if (target!=NULL)		/* �^�[�Q�b�g���܂������Ă���A Target freigeben */
		{
			target->flags &= ~SP_FLAG_PARANOIA; /* �M�����t���O�߂��Ƃ����B */
		}
	}
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
static void player_move_shield(SPRITE *s)	//�얲
{
	PL_SHIELD_DATA *data	=(PL_SHIELD_DATA *)s->data;
/// PLAYER_DATA *pd 	=(PLAYER_DATA *)player->data;
	data->angle512 += (data->speed/**fps_fa_ctor*/);/*...*/
	mask512(data->angle512);// if (data->angle>360) {	data->angle-=360;}

//	s->aktframe = (((int)(data->angle360))/10)%36;
	s->aktframe = ((((data->angle512)/*&(512-1)*/)*(36/2))>>8);

	s->x=player->x+co_s512((data->angle512))*data->radius+((player->w-s->w)>>1);
	s->y=player->y+si_n512((data->angle512))*data->radius+((player->h-s->h)>>1);
	if (data->radius<255)	{	data->radius+=(rand()&1);}	/* �������L����悤�ɂ��� */
	else	{	s->type=SP_DELETE;}/* ��]���L�������炨���܂� */
	s->alpha			= ((int)(255-data->radius))&(256-1);				/* ���񂾂������悤�ɂ��� */

	weapon_colcheck(s,/*d->angle,*/0,1);/* �{���œG��|�������蔻�� */

//	if (pd->bomber_time<=0) {	s->type=SP_DELETE;}/* ���Ԑ����ł����܂� */
}

/*---------------------------------------------------------
	�v���C���[�A�������{���̈ړ�(�������A�`���m)
---------------------------------------------------------*/

static void player_move_levarie(SPRITE *s)
{
	PL_LEVARIE_DATA *data=(PL_LEVARIE_DATA *)s->data;
	s->x += data->vx;//co_s512((data->angle512))*d->speed/**fps_fa_ctor*/;
	s->y += data->vy;//si_n512((data->angle512))*d->speed/**fps_fa_ctor*/;
	if (CIRNO==select_player)
	{	/*1.5*(d->speed)*//**fps_fa_ctor*/;/*�`���m�p(�b��I)*/
		s->y -= (abs((data->vx*1.5)));
		s->y -= (abs((data->vy*1.5)));
		s->y--;
	}
	weapon_colcheck(s,/*data->angle,*/0,1);/* �{���œG��|�������蔻�� */

	if ((s->x+s->w<0)||(s->x>GAME_WIDTH)||
		(s->y+s->h<0)||(s->y>GAME_HEIGHT))
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
	data->vx *= 1.1;//co_s512((data->angle512))*p->speed/**fps_fa_ctor*/;
	data->vy *= 1.1;//si_n512((data->angle512))*p->speed/**fps_fa_ctor*/;
	//
	s->x += data->vx;//co_s512((data->angle512))*p->speed/**fps_fa_ctor*/;
	s->y += data->vy;//si_n512((data->angle512))*p->speed/**fps_fa_ctor*/;
	if ((s->x+s->w<0)||(s->x>GAME_WIDTH)||
		(s->y+s->h<0))
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
	for (ggg=0;ggg<3;ggg++)
	{
		/*const*/ char *bbb_name[3]=
		{	// ������
			"fire_wind_l.png",	/*move left*/
			"fire_wind_r.png",	/*move right*/
			"fire_wind_u.png",
		};
		SPRITE *c;
		c=sprite_add_file0((char *)bbb_name[(ggg)],4,PR_PLAYER,0);
		//
		c->flags			|= SP_FLAG_VISIBLE;
		c->type 			= SP_PL_PLASMA/*�{�X�ɗL��*/;/*SP_PL_SHIELD �{�X�ɖ���*/
		c->anim_speed		= 3;
		c->aktframe 		= 0;
		c->mover			= remilia_move_burn_fire;
		c->alpha			= 100;
		c->x				= s->x+((s->w-c->w)>>1);
	//		 if (0==ggg/*2==r_or_l*/)	{	c->y			= s->y+15;}
	//	else if (1==ggg/*0==r_or_l*/)	{	c->y			= s->y+15;}
	//	else if (2==ggg/*3==r_or_l*/)	{	c->y			= s->y+15;	c->y -= s->h;}
	//
		c->y			= s->y+15;
		if (2==ggg/*3==r_or_l*/)	{	c->y -= s->h;}
	//
		OZ_BURN_FIRE_DATA *data;
		data				= mmalloc(sizeof(OZ_BURN_FIRE_DATA));
		c->data 			= data;
	//
	//	data->speed 		= 3/*12*/;/*����������ƒx��(�x�����������c��̂ŋ���)*/
		data->strength		= 9/*10*/;/* ����̃{���͓��ĂȂ���Ό���Ȃ��̂�(�O���ƌ���Ȃ�)�኱�������邭�炢�ł��� */
		/* �����A���������蔻�肪�ݒ�ł���悤�ɂȂ�����A�����蔻��͑����������������� */
		int aaa512[4] =
		{
			deg_360_to_512(180),	/*l=2*/ 	/*move left*/
			deg_360_to_512(  0),	/*r=0*/ 	/*move right*/
			deg_360_to_512(270),	/*u=3*/
			deg_360_to_512( 90),	/*d=1*/
		};
	//	data->angle512		= ((double)r_or_l/2)*deg_360_to_512(180)/*M_PI*2*1/2*/;
	//	data->angle512		= aaa512[ggg];
		data->vx = co_s512((/*data->angle512*/aaa512[ggg]))*/*data->speed*/(1)/**fps_fa_ctor*/;
		data->vy = si_n512((/*data->angle512*/aaa512[ggg]))*/*data->speed*/(1)/**fps_fa_ctor*/;
	}
}

static void player_move_add_cross_red(SPRITE *s)	/* [***090220 �ǉ� */
{
//	PL_CROSS_FIRE_DATA *data=(PL_CROSS_FIRE_DATA *)s->data;

//	data->angle512 += rad2deg512(1)/*fps_fa_ctor*/;
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
	//		s->aktframe++;
	//		s->aktframe &= 3;	//if (s->aktframe==4)	{	s->aktframe=0;	}
	//	}
		s->aktframe = ((((int)pd->bomber_time)>>2)&0x03);
	}
	else
	{
	//	if (0==(((int)pd->bomber_time)&3/*%4*/))
	//	{
	//			 if (s->aktframe<4) 	{	s->aktframe=4;}
	//		else if (s->aktframe==4)	{	s->aktframe=5;}
	//		else if (s->aktframe==5)	{	s->aktframe=6;}
	//		else if (s->aktframe==6)	{	s->aktframe=7;}
	//		else if (s->aktframe==7)	{	s->aktframe=8;}
	//		else						{	s->aktframe=9;}
	//	}
		static Uint8 ddd_tbl[8]=
		{	/* �{���̏I��肪�킩��₷���悤�P�\����(9�̃p�^�[��)�𒷂�(����)�A���߂Ɍx������ */
			 9,  9,  9,  8, 	 7,  6,  5,  4,
		};
		s->aktframe = ddd_tbl[((((int)pd->bomber_time)>>3)&0x07)];
	}
	s->x = player->x+((player->w-s->w)>>1);
	s->y = player->y+((player->h-s->h)>>1);

	weapon_colcheck(s,/*data->angle,*/0,1);/* �\�����{�̂ŏĂ��E���ꍇ */

	if (pd->bomber_time <= 0) { s->type=SP_DELETE;}/* ���Ԑ����ł����܂� */
}


/*---------------------------------------------------------
	�v���C���[�A�j�e�̒ǉ�(�얲)[�j�e]
---------------------------------------------------------*/

static void reimu_add_needle(SPRITE *s, int x_offs )	//�얲
{
	SPRITE *shot;
	shot				= sprite_add_res( BASE_NEEDLE_PNG+REIMU/*select_player*/ ); //sprite_add_file0("fireball1.png",2,PR_PLAYER,1);	shot->anim_speed=1;
	shot->type			= SP_PL_FIREBALL;
	shot->x 			= s->x+((s->w-shot->w)>>1) + x_offs;
	shot->y 			= s->y-(7);
	shot->mover 		= player_move_needle;
	shot->flags 		|= SP_FLAG_VISIBLE;
	PL_SHOT_DATA *data;
	data				= mmalloc(sizeof(PL_SHOT_DATA));
	shot->data			= data;
//	data->angle512		= deg_360_to_512(270);
//	data->speed 		= 18;
	data->vx = co_s512((/*angle512*/deg_360_to_512(270)))*/*p->speed*/(18)/**fps_fa_ctor*/;
	data->vy = si_n512((/*angle512*/deg_360_to_512(270)))*/*p->speed*/(18)/**fps_fa_ctor*/;
	data->strength		= /*3*/2;	/*��������*/
}

/*---------------------------------------------------------
	�v���C���[�A�j�e�̒ǉ�(������)[�X�e]
---------------------------------------------------------*/

static void marisa_add_needle(SPRITE *s, int x_offs, int y_offs)	//������
{
	SPRITE *shot;
	shot				= sprite_add_res( BASE_NEEDLE_PNG+select_player ); //sprite_add_file0((char *)aaa_name[(k)],1,PR_PLAYER,0); shot->anim_speed=0;
	shot->type			= SP_PL_FIREBALL;
	shot->x 			= s->x+((s->w-shot->w)>>1) + x_offs;
	shot->y 			= s->y-(7)+y_offs;
	shot->alpha 		= 150;
	shot->mover 		= player_move_needle;
	shot->flags 		|= SP_FLAG_VISIBLE;
	PL_SHOT_DATA *data;
	data				= mmalloc(sizeof(PL_SHOT_DATA));
	shot->data			= data;
	data->strength		= 4;
//
//	data->angle512		= deg_360_to_512(270);
//	data->speed 		= 10;
	data->vx = co_s512((/*angle512*/deg_360_to_512(270)))*/*p->speed*/(10)/**fps_fa_ctor*/;
	data->vy = si_n512((/*angle512*/deg_360_to_512(270)))*/*p->speed*/(10)/**fps_fa_ctor*/;
}

/*---------------------------------------------------------
	�v���C���[�A�j�e�̒ǉ�(���~���A�A���`���m)[�R�E�����e]
---------------------------------------------------------*/
enum
{
	REMILIA_NEEDLE_TYPE270=0,
	REMILIA_NEEDLE_TYPE263,
	REMILIA_NEEDLE_TYPE277,
	REMILIA_NEEDLE_TYPE260,
};
static void remilia_add_needle(SPRITE *s, int needle_type ) //������	/* [***090220 �ǉ� */
{
	SPRITE *shot;
	shot				= sprite_add_res( BASE_NEEDLE_PNG+select_player/*REMILIA*/ ); //sprite_add_file0("bat.png",5,PR_PLAYER,0); shot->anim_speed=3;
	shot->type			= SP_PL_FIREBALL;
	shot->x 			= s->x+((s->w-shot->w)>>1);
	shot->y 			= s->y-(7);
	shot->mover 		= player_move_needle/*player_move_koumori*/;
	shot->flags 		|= SP_FLAG_VISIBLE;
	PL_SHOT_DATA *data;
	data				= mmalloc(sizeof(PL_SHOT_DATA));
	shot->data			= data;
	data->strength		= 6/*5*/;	/*��������*/
//
//	data->angle512		= bbb[needle_type]/*deg_360_to_512(270)*/;
//	data->speed 		= 3/*8*/;	/*��������*/
	const signed short bbb[4] =
	{
		deg_360_to_512(270),
		deg_360_to_512(270-25),/*263=270-7*/
		deg_360_to_512(270+25),/*277=270+7*/
		deg_360_to_512(260),
	};
	data->vx = co_s512((/*angle512*/bbb[needle_type]))*/*p->speed*/(3)/**fps_fa_ctor*/;
	data->vy = si_n512((/*angle512*/bbb[needle_type]))*/*p->speed*/(3)/**fps_fa_ctor*/;
}

/*---------------------------------------------------------
	�v���C���[�A�I�v�V�����̈ړ�����
---------------------------------------------------------*/
static int option_flag;
static void reimu_marisa_control_option(SPRITE *s, PL_OPTION_DATA *data, PLAYER_DATA *pd) //�얲�A������
{
	int test_angle512;
	test_angle512 = data->angle512 - 128;/* ����p�p�x�F�X�O�x��]�����ĉ����O�Ƃ��� */ /*�[���P�ʌn�ϊ�*/
	mask512(test_angle512);
	if (my_pad & PSP_KEY_SLOW/*keybo ard[KEY_SLOW]*/)	/* �����ƑO���Ɉړ� */
	{
		if (OPTION_ID_01_RIGHT/*1*/==data->option_id/*pos*/)	/* 1: �E���̃I�v�V���� */
		{
			if (test_angle512 > /*(224)*/(256-16)/*240 (256-16)*/ )//if ((data->angle>-M_PI/2-0.05)&&(data->angle<-M_PI/2+0.05))
			{
				data->angle512 -= rad2deg512(0.1);
			}
		}
		else	/* 0: �����̃I�v�V���� */
		{
			if (test_angle512 < /*(288)*/(256+16)/*272 (256+16)*/ )//if ((data->angle>-M_PI/2-0.05)&&(data->angle<-M_PI/2+0.05))
			{
				data->angle512 += rad2deg512(0.1);
			}
		}
	}
	else	/* �����ƌ���Ɉړ� */
	{
		if (OPTION_ID_01_RIGHT/*1*/==data->option_id/*pos*/)	/* 1: �E���̃I�v�V���� */
		{
			if (test_angle512 < 448-16 )//if ((data->angle>-M_PI/2-0.05)&&(data->angle<-M_PI/2+0.05))
			{
				data->angle512 += rad2deg512(0.1);
			}
		}
		else	/* 0: �����̃I�v�V���� */
		{
			if (test_angle512 > 64+16 )//if ((data->angle>-M_PI/2-0.05)&&(data->angle<-M_PI/2+0.05))
			{
				data->angle512 -= rad2deg512(0.1);
			}
		}
	}
	if (
	//	(0==(pd->bonus_flag&BONUS_FLAG_08_OPTION_HIDE))/*0==pd->option_flag*/ ||	/* �I�v�V�����������Ă�� */
		(0==option_flag)/*0==pd->option_flag*/ ||	/* �I�v�V�����������Ă�� */
		pd->lives<0)			/* �c�`�����X���Ȃ���� */
	{	s->type=SP_DELETE;}/*�����܂�*/
	#if 1
	mask512(data->angle512);/*�˂�̂���*/
	#endif
//	s->x = player->x+co_s512((data->angle512))*(16)/*data->length*/-((s->w)>>1)+((player->w-12-1)*data->option_id)+6;
//	s->x = player->x+co_s512((data->angle512))*(16)/*data->length*/+((/*player->w*/24-12-1)*data->option_id)+6-8;
//	s->x = player->x+co_s512((data->angle512))*(16)/*data->length*/+((/*player->w*/24+11+11)*data->option_id)-11-8;
// -1 : 		 -11 -8
//	1 : +24+11+11-11 -8
//	s->x = player->x+co_s512((data->angle512))*(16)/*data->length*/+((/*player->w*/24-6-6)*data->option_id)+6-8;
// -1 : 		 +6 -8
//	1 : +24 -6 -6 +6 -8
//	s->x = player->x+co_s512((data->angle512))*(16)/*data->length*/+((/*player->w*/(24)-6-6)*data->option_id)+6-((s->w)>>1)/*(8)*/;
//
	//	s->x = player->x+co_s512((data->angle512))*(16)/*data->length*/+(((int)(data->pos+1)*player->w-s->w)/2)-data->pos*6;
	//	s->x = player->x+co_s512((data->angle512))*(16)/*data->length*/+(((int)(data->option_id+1)*/*player->w*/(24)-s->w)/2)-data->option_id*6;			//2598187
	//	s->x = player->x+co_s512((data->angle512))*(16)/*data->length*/+ (((24/*player->w*/ /2)-6)*(data->option_id)) -((s->w)>>1) +(24/*player->w*/ /2);	//2598219
		s->x = player->x+co_s512((data->angle512))*(16)/*data->length*/+ ((6)*(2+data->option_id)) -((s->w)>>1) ;	//2598187
		s->y = player->y+si_n512((data->angle512))*(16)/*data->length*/+((player->h-s->h)>>1)-5;
}

/*---------------------------------------------------------
	�v���C���[�A�I�v�V�����̈ړ�(�얲)
---------------------------------------------------------*/

static void re_move_option(SPRITE *s)	//�얲
{
	PL_OPTION_DATA *data=(PL_OPTION_DATA *)s->data;
	data->ani_speed--;
	if (data->ani_speed==0)
	{
		data->ani_speed=12/*2*/;/*�A�z���܂͂���������*/
	//	s->aktframe=((s->aktframe+(data->option_id)+(data->option_id)-1/*data->pos*/)&(8-1)/*%8*/);
		s->aktframe=((s->aktframe+(data->option_id))&(8-1)/*%8*/);
	//	if (s->aktframe<0)		{	s->aktframe=7;}
	}
	//
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	if (my_pad & PSP_KEY_SHOT/*keybo ard[KEY_SHOT]*/)
	{
		if ( (pd->bossmode!=B03_BEFORE_EVENT) && (pd->bossmode!=B06_AFTER_EVENT))
		{
			if (pd->state==PL_SAVE||pd->state==PL_NORMAL)
			{
				data->w_wait--;
				if (data->w_wait<0)
				{
					data->w_wait=4;
					if ( pd->weapon <= 90-1)
					{
						reimu_add_needle(s,  0);
					}
					else
					{
						reimu_add_needle(s, -5);
						reimu_add_needle(s,  5);
					}
				}
			}
		}
	}
	reimu_marisa_control_option(s, data, pd);
}

/*---------------------------------------------------------
	�v���C���[�A�I�v�V�����̈ړ�(������)
---------------------------------------------------------*/

static void ma_move_option(SPRITE *s)	//������
{
	PL_OPTION_DATA *data=(PL_OPTION_DATA *)s->data;
	data->ani_speed--;
	if (data->ani_speed==0)
	{
		data->ani_speed=6;
	//	s->aktframe=((s->aktframe+(data->option_id)+(data->option_id)-1/*data->pos*/)&(8-1)/*%8*/);
		s->aktframe=((s->aktframe+(data->option_id))&(8-1)/*%8*/);
	//	if (s->aktframe<0)		{	s->aktframe=7;}
	}
	//
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	if (my_pad & PSP_KEY_SHOT/*keybo ard[KEY_SHOT]*/)
	{
		if ( (pd->bossmode!=B03_BEFORE_EVENT) && (pd->bossmode!=B06_AFTER_EVENT))
		{
			if (pd->state==PL_SAVE||pd->state==PL_NORMAL)
			{
				data->w_wait--;
				if (data->w_wait<0)
				{
					data->w_wait=8;
					if (pd->weapon <= 70-1)
					{
						marisa_add_needle(s,  0,   0);
					}
					else if (pd->weapon <= 100-1)
					{
						marisa_add_needle(s, -7,   0);
						marisa_add_needle(s,  7,   0);
					}
					else
					{
						marisa_add_needle(s, -7,   0);
						marisa_add_needle(s,  0, -38);/*-38-7==-45*/
						marisa_add_needle(s,  7,   0);
					}
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

static void oz_move_option(SPRITE *s)	//������	/* [***090220 �ǉ� */
{
	static int state1=0;

	REMILIA_OPTION_DATA *data=(REMILIA_OPTION_DATA *)s->data;
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
//
	switch (data->option_id)
	{
	case OPTION_ID_01_NUM:
		break;
	case OPTION_ID_02_NUM:
		if (  (pd->weapon<=55-1))			{	s->flags &= ~SP_FLAG_VISIBLE;	}
		else /*if (  (pd->weapon>55-1))*/	{	s->flags |=  SP_FLAG_VISIBLE;	}
		break;
	case OPTION_ID_03_NUM:
		if (  (pd->weapon<=80-1))			{	s->flags &= ~SP_FLAG_VISIBLE;	}
		else /*if (  (pd->weapon>80-1))*/	{	s->flags |=  SP_FLAG_VISIBLE;	}
		break;
	case OPTION_ID_04_NUM:
		if (  (pd->weapon<=108-1))			{	s->flags &= ~SP_FLAG_VISIBLE;	}
		else /*if (  (pd->weapon>108-1))*/	{	s->flags |=  SP_FLAG_VISIBLE;	}
		break;
	}
//
	if (s->flags&SP_FLAG_VISIBLE)	//�\������Ă���Ώ�L�̏����������B
	{
		// �A�j���[�V����
		data->ani_speed--;
		if (data->ani_speed==0)
		{
			data->ani_speed=3;
			s->aktframe++;
			s->aktframe &= (8-1);//if (s->aktframe==8)	{s->aktframe=0;}
		}
		// �U���x��
		if (my_pad & PSP_KEY_SHOT/*keybo ard[KEY_SHOT]*/)
		{
			if (pd->state==PL_SAVE||pd->state==PL_NORMAL)	//�U��
			{
				if ( (pd->bossmode!=B03_BEFORE_EVENT) && (pd->bossmode!=B06_AFTER_EVENT))
				{
					data->w_wait--;
					if (data->w_wait<0)
					{
						switch (data->option_id)
						{
						case OPTION_ID_01_NUM:
						case OPTION_ID_02_NUM:
							if (pd->weapon > 100-1)
							{
								remilia_add_needle(s, REMILIA_NEEDLE_TYPE277 );
								remilia_add_needle(s, REMILIA_NEEDLE_TYPE263 );
								data->w_wait=7+3;
							}
							else
							{
								remilia_add_needle(s, REMILIA_NEEDLE_TYPE270 );
								data->w_wait=5+3;
							}
							break;
						case OPTION_ID_03_NUM:
						case OPTION_ID_04_NUM:
							remilia_add_needle(s, REMILIA_NEEDLE_TYPE270 );
							data->w_wait=5+3;
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
		if (my_pad & PSP_KEY_SLOW/*keybo ard[KEY_SLOW]*/)
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
		int player_offs_x;
		int player_offs_y;
		player_offs_x = player->x+((player->w-s->w)>>1);
		player_offs_y = player->y+((player->h-s->h)>>1);
		if (1==slow_flag)	// ���Ȃ��Ƃ��ᑬ�ړ��ŃI�v�V�������~�߂���̂̓��~�̂�(�`���m�ɂ͍���Ȃ�)
		{
			/*���~�p*/
			data->slow_count++;
			if ((/*data->*/state1==FORMATION_03) && /* FORMATION_03: �������p(�D���Ȉʒu�ɒu�����) */
				(data->slow_count < ((data->option_id+1)<<6)/* *30 64*/))
			{
				s->x = player_offs_x;
				s->y = player_offs_y;
			}
			else if (/*data->*/state1==FORMATION_02)	/* FORMATION_02: ��ʌ������x������� */
			{
				switch (data->option_id)
				{
				case OPTION_ID_01_NUM: if (s->x<170-s->w)	{s->x+=2/**fps_fa_ctor*/;}	break;
				case OPTION_ID_02_NUM: if (s->x>210)		{s->x-=2/**fps_fa_ctor*/;}	break;
				case OPTION_ID_03_NUM: if (s->x<100-s->w)	{s->x+=4/**fps_fa_ctor*/;}	break;
				case OPTION_ID_04_NUM: if (s->x>280)		{s->x-=4/**fps_fa_ctor*/;}	break;
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
			if (OPTION_ID_01_NUM==data->option_id) //option_id==1�ɂ��flag�Ǘ�
			{
			//	if (my_pad & PSP_KEY_CANCEL/*keybo ard[KEY_CANCEL]*/)
			/* (�L�����Z���L�[�𗣂����u�ԂȂ�) */
				if ((0==(my_pad & PSP_KEY_CANCEL))) 	/* �L�����Z���L�[��������Ă��� */
				{
					if ( (my_pad^my_pad_alter) & PSP_KEY_CANCEL)	/* �L�����Z���L�[�̏�Ԃ��ς����(�g�O���A�������܂��͗������ꍇ) */
					{
						/*data->*/state1++;
						if (/*data->*/state1==(FORMATION_03+1) )
						{	/*data->*/state1=FORMATION_00;}
					//
						data->state2=0;
						{
						((REMILIA_OPTION_DATA *)c4->data)->state2=0;
						((REMILIA_OPTION_DATA *)c3->data)->state2=0;
						((REMILIA_OPTION_DATA *)c2->data)->state2=0;
						}
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
						FORMATION_MAX,		  /* 6*/
					};
					const signed short fff[FORMATION_MAX][4]=
					{
						{  15, -15,  25, -25 },
						{  15,	15,  20,  20 },
						{  20,	30,  40,  50 }, 	/* FORMATION_01:��]���a */
						{	1,	 2,  -4,  -8 }, 	/* FORMATION_01:��]�����A�p�x���Z�l */
						{	1*115+10,  2*115+10,  0*115+10,  3*115+10 },
						{ 240, 240, 220, 220 },
					};
					switch (/*data->*/state1)
					{
					case FORMATION_00:	/* FORMATION_00: ���~���A�̒���Ɏl�� */
						data->offset_x=fff[FORMATION_00_LOCATE_X][(data->option_id)];
						data->offset_y=fff[FORMATION_00_LOCATE_Y][(data->option_id)];
						break;
					case FORMATION_01:	/* FORMATION_01: ���~�̎�������� */
						data->offset_x=fff[FORMATION_01_RADIUS_R][(data->option_id)];/*40*/ 	/* �~�̔��a�ƌ��p */
						data->offset_y=fff[FORMATION_01_ADD_R	][(data->option_id)];			/* �~�̉�]�����A�p�x���Z�l�ƌ��p */
						data->f_angle512=0/*l_angle512*/;
						break;
					case FORMATION_02:	/* FORMATION_02: ��ʌ������x������� */
						data->offset_x=fff[FORMATION_02_LOCATE_X][(data->option_id)];
						data->offset_y=fff[FORMATION_02_LOCATE_Y][(data->option_id)];/*240*/
						break;
					case FORMATION_03:	/* FORMATION_03: �������p(�D���Ȉʒu�ɒu�����) */
						break;
					}
				}
				data->state2=1; 	/* ���� */
				break;
			case 1: 	// �ڕW�n�_�ֈړ���
				#define oz_offset_r 	(data->offset_x)		/* �~�̔��a�ƌ��p */
				#define oz_offset_add_r (data->offset_y)		/* �~�̉�]�����A�p�x���Z�l�ƌ��p */
				{
				int delta_y=0;				/* delta_y : y���W�ɂ�����ڕW�n�_�ƌ��ݒn�̍� */
				int delta_x=0;				/* delta_x : x���W�ɂ�����ڕW�n�_�ƌ��ݒn�̍� */
					switch (/*data->*/state1)
					{
					case FORMATION_00:	/* FORMATION_00: ���~���A�̒���Ɏl�� */
						delta_y = player_offs_y-s->y + data->offset_y;
						delta_x = player_offs_x-s->x + data->offset_x;
						break;
					case FORMATION_01:	/* FORMATION_01: ���~�̎�������� */						//static /*double*/int l_angle512;
						delta_y = player_offs_y-s->y + si_n512((data->angle512/*+l_angle512*/))*oz_offset_r;
						delta_x = player_offs_x-s->x + co_s512((data->angle512/*+l_angle512*/))*oz_offset_r;
						break;
					case FORMATION_02:	/* FORMATION_02: ��ʌ������x������� */
						delta_y = (-((s->h)>>1)-s->y+data->offset_y);/*240*/
						delta_x = (-((s->w)>>1)-s->x+data->offset_x);
						break;
					case FORMATION_03:	/* FORMATION_03: �������p(�D���Ȉʒu�ɒu�����) */
						delta_y = player_offs_y-s->y;
						delta_x = player_offs_x-s->x;
						break;
					}
					{	/* �ڕW�n�_�Ɉړ����� */
						int m_angle512;
						/*data->*/m_angle512=atan_512(delta_y, delta_x);
						s->x+=co_s512((/*data->*/m_angle512))*/*10*/((REMILIA==select_player)?8.0:1.0)/**fps_fa_ctor*/;
						s->y+=si_n512((/*data->*/m_angle512))*/*10*/((REMILIA==select_player)?8.0:1.0)/**fps_fa_ctor*/;
					}
#if 1
					/*���~�p*/
					if (REMILIA==select_player)
					{
						/* �ڕW�t�߂܂ł��ǂ蒅������ */
						if ( (-5 < delta_y) && (delta_y < 5) )
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
					s->x = player_offs_x + data->offset_x;
					s->y = player_offs_y + data->offset_y;
					break;
				case FORMATION_01:	/* FORMATION_01: ���~�̎�������� */
					/*l_angle512*/data->f_angle512 += oz_offset_add_r/*data->offset_y*/ /*rad2deg512(0.05)*/ /**fps_fa_ctor*/;
					mask512(/*l_angle512*/data->f_angle512);	//if (l_angle>M_PI) 	{	l_angle-=2*M_PI;	}
					s->x = player_offs_x + si_n512((data->angle512+data->f_angle512/*l_angle512*/))*oz_offset_r;
					s->y = player_offs_y + co_s512((data->angle512+data->f_angle512/*l_angle512*/))*oz_offset_r;
					break;
				case FORMATION_02:	/* FORMATION_02: ��ʌ������x������� */
					s->x = data->offset_x;
					s->y = data->offset_y;
					break;
				case FORMATION_03:	/* FORMATION_03: �������p(�D���Ȉʒu�ɒu�����) */
					s->x = player_offs_x;
					s->y = player_offs_y;
					break;
				}
#endif
				break;
			}
		}
	}
//
	if (
	//	(0==(pd->bonus_flag&BONUS_FLAG_08_OPTION_HIDE))/*0==pd->option_flag*/ ||	/* �I�v�V�����������Ă�� */
		(0==option_flag)/*0==pd->option_flag*/ ||	/* �I�v�V�����������Ă�� */
		pd->lives<0)			/* �c�`�����X���Ȃ���� */
	{	s->type=SP_DELETE;}/*�����܂�*/
}

/*---------------------------------------------------------
	�v���C���[�A�I�v�V�����̒ǉ�
---------------------------------------------------------*/

static void re_add_option(SPRITE *s)	//�얲
{
	PL_OPTION_DATA *d1, *d2;
	c1				= sprite_add_res( BASE_OPTION1_PNG/*+REIMU*/ );
	c1->flags		|= SP_FLAG_VISIBLE;
	c1->mover		= re_move_option;
	d1				= mmalloc(sizeof(PL_OPTION_DATA));
	c1->data		= d1;
	d1->strength	= 5;/* [***090214 �ǉ� */
//	d1->length		= 11;
	c1->x			= s->x+((s->w-c1->w)>>1)-(11)/*d1->length*/;
	c1->y			= s->y+((s->h-c1->h)>>1);
	d1->ani_speed	= 2;
	d1->option_id/*pos*/		= OPTION_ID_99_LEFT/*-1*/;
	d1->angle512	= deg_360_to_512(180)/*-rad2deg512(M_PI)*/;
//	d1->state		= 0;
	d1->w_wait		= 0;
	c1->aktframe	= 0;
	c1->type		= SP_PLAYER2;

	c2				= sprite_add_res( BASE_OPTION1_PNG/*+REIMU*/ );
	c2->flags		|= SP_FLAG_VISIBLE;
	c2->mover		= re_move_option;
	d2				= mmalloc(sizeof(PL_OPTION_DATA));
	c2->data		= d2;
	d2->strength	= 5;/* [***090214 �ǉ� */
//	d2->length		= 11;
	c2->x			= s->x+((s->w-c2->w)>>1)-(11)/*d2->length*/;
	c2->y			= s->y+((s->h-c2->h)>>1);
	d2->ani_speed	= 2;
	d2->option_id/*pos*/		= OPTION_ID_01_RIGHT/*1*/;
	d2->angle512	= deg_360_to_512(0);
//	d2->state		= 0;
	d2->w_wait		= 0;
	c2->aktframe	= 0;
	c2->type		= SP_PLAYER2;
}

static void ma_add_option(SPRITE *s)	//������
{
	PL_OPTION_DATA *d1, *d2;		//�얲�̂𗬗p���Ă܂��B
	c1				= sprite_add_res( BASE_OPTION1_PNG+MARISA );
	c1->flags		|= SP_FLAG_VISIBLE;
	c1->mover		= ma_move_option;
	d1				= mmalloc(sizeof(PL_OPTION_DATA));
	c1->data		= d1;
	d1->strength	= 5;/* [***090214 �ǉ� */
//	d1->length		= 11;
	c1->x			= s->x+((s->w-c1->w)>>1)-(11)/*d1->length*/;
	c1->y			= s->y+((s->h-c1->h)>>1);
	d1->ani_speed	= 6;
	d1->option_id/*pos*/		= OPTION_ID_99_LEFT/*0*/ /*-1*/;
	d1->angle512	= deg_360_to_512(180)/*-rad2deg512(M_PI)*/;
//	d1->state		= 0;
	d1->w_wait		= 0;
	c1->aktframe	= 0;
	c1->type		= SP_PLAYER2;

	c2				= sprite_add_res( BASE_OPTION1_PNG+MARISA );
	c2->flags		|= SP_FLAG_VISIBLE;
	c2->mover		= ma_move_option;
	d2				= mmalloc(sizeof(PL_OPTION_DATA));
	c2->data		= d2;
	d2->strength	= 5;/* [***090214 �ǉ� */
//	d2->length		= 11;
	c2->x			= s->x+((s->w-c2->w)>>1)-(11)/*d2->length*/;
	c2->y			= s->y+((s->h-c2->h)>>1);
	d2->ani_speed	= 6;
	d2->option_id/*pos*/		= OPTION_ID_01_RIGHT/*1*/;
	d2->angle512	= deg_360_to_512(0);
//	d2->state		= 0;
	d2->w_wait		= 0;
	c2->aktframe	= 0;
	c2->type		= SP_PLAYER2;
}

static void oz_add_option(SPRITE *s)	//������	/* [***090220 �ǉ� */
{
	REMILIA_OPTION_DATA *d;
	c1				= sprite_add_res( BASE_OPTION1_PNG+select_player/*REMILIA*/ );
	c1->flags		|= SP_FLAG_VISIBLE;
	c1->type		= SP_PLAYER2;
	c1->mover		= oz_move_option;
	d				= mmalloc(sizeof(REMILIA_OPTION_DATA));
	c1->data		= d;
	c1->aktframe	= 0;
	d->angle512 	= deg_360_to_512(45*1)/*M_PI*2*1/8*/;
//	d->next 		= player;
//	d->state1		= 0;///
	d->state2		= 0;
	d->offset_x 	= 0;
	d->offset_y 	= 0;
	d->strength 	= 5;
	d->ani_speed	= 3;
	d->w_wait		= 0;
	d->option_id	= OPTION_ID_01_NUM;
//	d->m_angle512	= deg_360_to_512(0);
	d->slow_count	= 0;
	c1->x			= s->x+((s->w-c1->w)>>1);
	c1->y			= s->y+((s->h-c1->h)>>1)+20;

	c2				= sprite_add_res( BASE_OPTION2_PNG+select_player/*REMILIA*/ );
	c2->flags		&= ~SP_FLAG_VISIBLE;
	c2->type		= SP_PLAYER2;
	c2->mover		= oz_move_option;
	d				= mmalloc(sizeof(REMILIA_OPTION_DATA));
	c2->data		= d;
	c2->aktframe	= 0;
	d->angle512 	= deg_360_to_512(45*5)/*M_PI*2*5/8*/;
//	d->next 		= c1;
//	d->state1		= ((REMILIA_OPTION_DATA *)c1->data)->state1;///
	d->state2		= 0;
	d->offset_x 	= 0;
	d->offset_y 	= 0;
	d->strength 	= 5;
	d->ani_speed	= 3;
	d->w_wait		= 0;
	d->option_id	= OPTION_ID_02_NUM;
//	d->m_angle512	= deg_360_to_512(0);
	d->slow_count	= 0;
	c2->x			= s->x+((s->w-c2->w)>>1);
	c2->y			= s->y+((s->h-c2->h)>>1)+20;

	c3				= sprite_add_res( BASE_OPTION3_PNG+select_player/*REMILIA*/ );
	c3->flags		&= ~SP_FLAG_VISIBLE;
	c3->type		= SP_PLAYER2;
	c3->mover		= oz_move_option;
	d				= mmalloc(sizeof(REMILIA_OPTION_DATA));
	c3->data		= d;
	c3->aktframe	= 0;
	d->angle512 	= deg_360_to_512(45*3)/*M_PI*2*3/8*/;
//	d->next 		= c2;
//	d->state1		= ((REMILIA_OPTION_DATA *)c1->data)->state1;///
	d->state2		= 0;
	d->offset_x 	= 0;
	d->offset_y 	= 0;
	d->strength 	= 5;
	d->ani_speed	= 3;
	d->w_wait		= 0;
	d->option_id	= OPTION_ID_03_NUM;
//	d->m_angle512	= deg_360_to_512(0);
	d->slow_count	= 0;
	c3->x			= s->x+((s->w-c3->w)>>1);
	c3->y			= s->y+((s->h-c3->h)>>1)+20;

	c4				= sprite_add_res( BASE_OPTION4_PNG+select_player/*REMILIA*/ );
	c4->flags		&= ~SP_FLAG_VISIBLE;
	c4->type		= SP_PLAYER2;
	c4->mover		= oz_move_option;
	d				= mmalloc(sizeof(REMILIA_OPTION_DATA));
	c4->data		= d;
	c4->aktframe	= 0;
	d->angle512 	= deg_360_to_512(45*7)/*M_PI*2*7/8*/;
//	d->next 		= c3;
//	d->state1		= ((REMILIA_OPTION_DATA *)c1->data)->state1;///
	d->state2		= 0;
	d->offset_x 	= 0;
	d->offset_y 	= 0;
	d->strength 	= 5;
	d->ani_speed	= 3;
	d->w_wait		= 0;
	d->option_id	= OPTION_ID_04_NUM;
//	d->m_angle512	= deg_360_to_512(0);
	d->slow_count	= 0;
	c4->x			= s->x+((s->w-c4->w)>>1);
	c4->y			= s->y+((s->h-c4->h)>>1)+20;
}

/*---new add-----*/


/*---------------------------------------------------------
	�v���C���[�A��������p�R�A�̒ǉ�
---------------------------------------------------------*/

static SPRITE *player_add_core(SPRITE *s1)		//���̒ǉ�
{
	SPRITE *s2;
	s2				= NULL;
	s2				= sprite_add_res( BASE_CORE_PNG+select_player );
	s2->anim_speed	= 0;
	s2->type		= SP_PLAYER2;
	s2->x			= s1->x+((s1->w-s2->w)>>1);
	s2->y			= s1->y-s2->h+20;
	s2->mover		= player_move_core;
	s2->flags		|= SP_FLAG_VISIBLE;
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
	MARISA_SHOT_TYPE_03,
	MARISA_SHOT_TYPE_04,
	MARISA_SHOT_TYPE_05,
	MARISA_SHOT_TYPE_06,
	MARISA_SHOT_TYPE_07,
	MARISA_SHOT_TYPE_08,
//
	REIMU_SHOT_TYPE_MAX
};

enum
{
	REI00_x_offset=0,
	REI01_y_offset,
	REI02_angle512,
	REI03_yudo,
	REI04_MAX,
};

static void player_add_shot(SPRITE *player, int shot_type) //�얲 �� ������ �� �`���m �� ������ /* [***090220 �ǉ� */
{
	const Sint16 r_tbl[REIMU_SHOT_TYPE_MAX][REI04_MAX] =
	{	/*x_offset	y_offset  angle512 yudo */
/*00*/	{  0, 20, deg_360_to_512(270),	  0},	/*player_add_fuda*/ 		/*player_add_star*/
/*01*/	{ -5, 20, deg_360_to_512(270),	  0},	/*player_add_double_fuda*/	/*player_add_double_star*/
/*02*/	{ +5, 20, deg_360_to_512(270),	  0},	/*player_add_double_fuda*/	/*player_add_double_star*/
//
/*03*/	{-15,+25, deg_360_to_512(270-6),  1},	/*player_add_triple_fuda*/
/*04*/	{+15,+25, deg_360_to_512(270+6),  1},	/*player_add_triple_fuda*/
/*05*/	{-15,+25, deg_360_to_512(270-10), 1},	/*player_add_quad_fuda*/
/*06*/	{+15,+25, deg_360_to_512(270+10), 1},	/*player_add_quad_fuda*/
//
/*07*/	{-15,+25, deg_360_to_512(270-15), 0},	/*player_add_five_fuda*/
/*08*/	{+15,+25, deg_360_to_512(270+15), 0},	/*player_add_five_fuda*/
/*09*/	{ -5,+20, deg_360_to_512(270-8),  1},	/*player_add_five_fuda*/
/*0a*/	{ +5,+20, deg_360_to_512(270+8),  1},	/*player_add_five_fuda*/
//
/*03*/	{ -5,+20, deg_360_to_512(270-4),  0},	/*player_add_triple_star*/
/*04*/	{ +5,+20, deg_360_to_512(270+4),  0},	/*player_add_triple_star*/
/*05*/	{ -6,+20, deg_360_to_512(270-3),  0},	/*player_add_quad_star*/
/*06*/	{ +6,+20, deg_360_to_512(270+3),  0},	/*player_add_quad_star*/
/*07*/	{-10,+25, deg_360_to_512(270-7),  0},	/*player_add_quad_star*/
/*08*/	{+10,+25, deg_360_to_512(270+7),  0},	/*player_add_quad_star*/
	};
	SPRITE *shot;
	shot			= sprite_add_res( BASE_PLAZMA_PNG+select_player );
	shot->type		= SP_PL_PLASMA;
	shot->x 		= player->x+((player->w-shot->w)>>1)+r_tbl[shot_type][REI00_x_offset];
	shot->y 		= player->y-(shot->h)+r_tbl[shot_type][REI01_y_offset]/*20*/;
	shot->mover 	= player_move_shot;
	shot->flags 	|= SP_FLAG_VISIBLE;
	shot->alpha 	= 150;/*player_add_triplestar*/
	PL_SHOT_DATA *data;
	data			= mmalloc(sizeof(PL_SHOT_DATA));
	shot->data		= data;
//
	int int_angle512;
	int ok;
	ok=0;
	{
		if (1==r_tbl[shot_type][REI03_yudo])/* �����V���b�g���H */
		{
			SPRITE *target;
			target=search_enemy_by_sprite();
			if (target != NULL)
			{
				/*data->*/int_angle512	= atan_512(target->y-player->y,target->x-player->x);
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
	#define CCC_STRENGTH		(4*0)
	#define CCC_SPEED			(4*1)
	#define CCC_MAX 			(4*2)
	static Uint8 ccc_tbl[CCC_MAX] =
	{/* REIMU MARISA REMILIA CIRNO */
		 2, 3, 4, 5,	/* strength �V���b�g�̋��� */
		16,10, 9, 8,	/* speed	�V���b�g�̑��� (�x��������ʏ�̒e����������̂ŋ����B�������d���Ȃ�) */
	};
//		 2, 3, 4, 3,	/* strength �V���b�g�̋��� */
//		15,12,18,12,	/* speed	�V���b�g�̑���	*/
/* �ɒ[�ɋ@�̂̐��\���΂�̂͗ǂ��Ȃ��B(�����đ����̂͂��邢) */
	data->strength	= ccc_tbl[CCC_STRENGTH+select_player];
//	data->speed 	= ccc_tbl[CCC_SPEED+select_player];
//
	data->vx = co_s512((/*angle512*/int_angle512))*/*p->speed*/(ccc_tbl[CCC_SPEED+select_player])/**fps_fa_ctor*/;
	data->vy = si_n512((/*angle512*/int_angle512))*/*p->speed*/(ccc_tbl[CCC_SPEED+select_player])/**fps_fa_ctor*/;
}

/*---------------------------------------------------------
	�v���C���[�e(�V���b�g�̂�)����
---------------------------------------------------------*/

static void player_add_dual_shot(SPRITE *player)	//�얲 �� ������ �� �`���m �� ������	/* [***090220 �ǉ� */
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
	player_add_shot(player,MARISA_SHOT_TYPE_03);
	player_add_shot(player,REIMU_SHOT_TYPE_00);
	player_add_shot(player,MARISA_SHOT_TYPE_04);
}

static void player_add_quad_star(SPRITE *player)	//������ �� �`���m
{
	player_add_shot(player,MARISA_SHOT_TYPE_05);
	player_add_shot(player,MARISA_SHOT_TYPE_06);
	player_add_shot(player,MARISA_SHOT_TYPE_07);
	player_add_shot(player,MARISA_SHOT_TYPE_08);
}


#if (1==USE_PLAYER_WAVE)
static void player_add_killray(SPRITE *player)		//�g��Ȃ�
{
	SPRITE *shot;
	PL_KILLRAY_DATA *data;
	static int last=0;
	if (last)
	{
		shot=sprite_add_file0("killray-r.png",12,PR_PLAYER,0);
		last=0;
	}
	else
	{
		shot=sprite_add_file0("killray-b.png",12,PR_PLAYER,0);
		last=1;
	}
	shot->type		= SP_PL_KILLRAY;
	shot->x 		= player->x+((player->w-shot->w)>>1);
	shot->y 		= player->y-shot->h+20;
	shot->mover 	= player_move_killray;
	shot->flags 	|= SP_FLAG_VISIBLE;
	data			= mmalloc(sizeof(PL_KILLRAY_DATA));
	shot->data		= data;
	data->angle 	= 270;
	data->speed 	= 25;
	data->distance	= 0;
	data->strength	= 6;
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
		c				= sprite_add_file0("fireball.png",1,PR_PLAYER,0);
		c->type 		= SP_PL_BOMB;
		c->x			= player->x+((player->w)>>1);
		c->y			= player->y+((player->h)>>1);
		c->mover		= player_move_bomb;
		c->flags		|= SP_FLAG_VISIBLE;
		PL_BOMB_DATA *data;
		data			= mmalloc(sizeof(PL_BOMB_DATA));
		c->data 		= b;
		data->angle 	= a;
		data->speed 	= 1;
		data->strength	= 2;
	}
}
#endif //(1==USE_PLAYER_BOMBER)

/*---------------------------------------------------------
	�v���C���[�V�[���h����
---------------------------------------------------------*/

static void player_add_shield(SPRITE *s)		//�V�[���h�̒ǉ�	//�얲
{
	PLAYER_DATA *pd=(PLAYER_DATA *)s->data;
	pd->bonus_flag	|= BONUS_FLAG_02_BOMB_AUTO_GET_ITEM;	/*�{���ɂ�鎩�����W�\*/
	int ii;//	int i;	/* ���ی����� */
	for (ii=0;ii<(8);ii++)//	for (i=0;i<360/*i<=359*/;i+=45)
	{
		SPRITE *c;
		PL_SHIELD_DATA *d;
		if ( (ii&1) == 0)// if (i%90 == 0)
		{
			c			= sprite_add_file0("cshoot1.png",36,PR_PLAYER,1);
			d			= mmalloc(sizeof(PL_SHIELD_DATA));
			c->data 	= d;
			d->speed	= 23;						//���v���
			d->radius	= 5/*48*/;	//d->radius=38; 	/*���a�����l*/	/* �������L����悤�ɂ��� */
		}
		else		//1���ƕs���Ȃ̂�2������B
		{
			c			= sprite_add_file0("cshoot2r.png"/*"cshoot2.png"*/,36,PR_PLAYER,1);
			d			= mmalloc(sizeof(PL_SHIELD_DATA));
			c->data 	= d;
			d->speed	= -17;							//�����v���
			d->radius	= 0/*45*/;	//d->radius=35; 	/*���a�����l*/	/* �������L����悤�ɂ��� */
		}
			c->mover	= player_move_shield;		//���v��聕�����v���
		//	c->mover	= player_move_shield/*2*/;		//�����v���
		c->flags		|= SP_FLAG_VISIBLE;
		c->type 		= SP_PL_SHIELD;
		d->angle512 	= (ii*deg_360_to_512(45))/*i*/;
		/* �o�n�߂̓v���C���[���W */
		c->x			= player->x;//+co_s512(((ii*deg_360_to_512(45))/*i*/))*d->radius+((player->w-s->w)>>1);
		c->y			= player->y;//+si_n512(((ii*deg_360_to_512(45))/*i*/))*d->radius+((player->h-s->h)>>1); //���v���
		d->strength 	= 5;/* [***090214 �ǉ� */
	}
}

static void player_add_levarie(SPRITE *s)	//������
{
	SPRITE *c;
	c=NULL;
	PL_LEVARIE_DATA *d;
	PLAYER_DATA *pd=(PLAYER_DATA *)s->data;
	pd->bonus_flag	|= BONUS_FLAG_02_BOMB_AUTO_GET_ITEM;	/*�{���ɂ�鎩�����W�\*/
	int i;
	int j;
	int k;
	if (MARISA==select_player)	{k=0;}else{k=6;}
	/*const*/ char *bbb_name[12]=
	{	// ������
		"star_shield_blue.png",
		"star_shields_red.png",
		"star_shield_green.png",
		"star_shields_blue.png",
		"star_shield_red.png",
		"star_shields_green.png",
		// �`���m
		"cirno_shield_blue.png",
		"cirno_shields_red.png",
		"cirno_shield_green.png",
		"cirno_shields_blue.png",
		"cirno_shield_red.png",
		"cirno_shields_green.png",
	};

	j=0;
	/*double*/int ang512;
//	ang512 = rad2deg512(((M_PI*2)/32))/*0*/;/*�`���m�p(�b��I)*/
	ang512 = (0)/*0*/;/*�`���m�p(�b��I)*/
	for (i=0;i<16;i++)
	{
		c				= sprite_add_file0((char *)bbb_name[(k+j)],3,PR_PLAYER,0);
		j++;if (6==j)	{j=0;}
		c->anim_speed	= 5;
		c->flags		|= SP_FLAG_VISIBLE;
		c->mover		= player_move_levarie;

		d				= mmalloc(sizeof(PL_LEVARIE_DATA));
		c->data 		= d;
		d->strength 	= 5;/* [***090214 �ǉ� */
//		d->angle512 	= ang512;
		double d_speed;
		if ((i&(2-1)/*%2*/)==0)
		{
			/*d->*/d_speed	= 1.0;
			c->alpha	= 150;
		}
		else
		{
			/*d->*/d_speed	= 1.2;
			c->alpha	= 180;
		}
		d->vx = co_s512((/*angle512*/ang512))*/*p->speed*/(d_speed)/**fps_fa_ctor*/;
		d->vy = si_n512((/*angle512*/ang512))*/*p->speed*/(d_speed)/**fps_fa_ctor*/;
//
		c->type 		= SP_PL_SHIELD;
		c->x			= s->x+((s->w-c->w)>>1);
		c->y			= s->y+((s->h-c->h)>>1);
		ang512 += rad2deg512((M_PI*2)/16) ; // 22.5�x/360, ��/8 /*0.39269908169872415481*/
	}
}

/* ����{���̏\�����{�� */
static void player_add_cross_red_parrent(SPRITE *s) /* ������ */ /* [***090220 �ǉ� */
{
	PLAYER_DATA *pd = (PLAYER_DATA *)s->data;
	pd->bonus_flag	|= BONUS_FLAG_02_BOMB_AUTO_GET_ITEM;	/*�{���ɂ�鎩�����W�\*/
	SPRITE *c;
	c				= sprite_add_file0("cross_red.png",10,PR_PLAYER,0);
	c->flags		|= SP_FLAG_VISIBLE;
	c->anim_speed	= 0;
	c->aktframe 	= 0;
	c->alpha		= 220;
	c->mover		= player_move_add_cross_red;
	c->type 		= SP_PL_SHIELD;
	c->x			= s->x+((s->w-c->w)>>1);
	c->y			= s->y+((s->h-c->h)>>1);
//
	PL_CROSS_FIRE_DATA *data;
	data			= mmalloc(sizeof(PL_CROSS_FIRE_DATA));
	c->data 		= data;
	data->strength	= 3;		/* �\�����{�� �̋��� */
//	data->angle512	= (0);
//	data->vx = player->x+((player->w-s->w)>>1);
//	data->vy = player->y+((player->h-s->h)>>1);
}


/*---------------------------------------------------------
	�v���C���[�A�U���e�̈ړ�����
---------------------------------------------------------*/

//#define hlaser_NUM_OF_ENEMIES (24)
#define hlaser_NUM_OF_ENEMIES (12)		/* [***090128		�����ɂ��Ă݂� */
static void player_controller_hlaser(CONTROLLER *c)
{
	int *id_array=c->e;
	SPRITE *s;
	int i;
	if (NULL==sprite_get_by_id(id_array[0]))
	{
		for (i=1;i<hlaser_NUM_OF_ENEMIES;i++)
		{
			s=sprite_get_by_id(id_array[i]);
			if (NULL!=s)
			{	s->type=SP_DELETE;}/* ID����X�v���C�g�̎��̂��݂���Ȃ���΁A�����܂� */
		}
		controller_remove(c);
	}
	else
	{
		for (i=hlaser_NUM_OF_ENEMIES-1;i>=1;i--)/*��납�璲�ׂ�*/
		{
			SPRITE *l/*=NULL*/;
			s=sprite_get_by_id(id_array[i]);
			l=sprite_get_by_id(id_array[i-1]);
			if ((NULL!=s)&&(NULL!=l))
			{
				s->x=l->x;
				s->y=l->y;
				// draw_line(screen,s->x,s->y,l->x,l->y,
				// SDL_MapRGB(screen->format,0xff,0xff,0xff),
				// SDL_MapRGB(screen->format,0xa0,0xff,0xa0));
			}
		}
	}
}

/*---------------------------------------------------------
	�v���C���[�U���e����
---------------------------------------------------------*/

#if (1==USE_PLAYER_HOMING)
#endif //(1==USE_PLAYER_HOMING)
static void player_add_hlaser(SPRITE *src)
{
	int j;
	j=(rand()&(2-1));//for (j=0;j<2;j++)
	{
		CONTROLLER *c;
		c=controller_add();
	//	hlaser_NUM_OF_ENEMIES=12;		// [***090128		�����ɂ��Ă݂�
		int *id_array;
		id_array	= mmalloc(sizeof(int)*hlaser_NUM_OF_ENEMIES);
		c->e		= id_array;
		c->con		= player_controller_hlaser;
//
//		/*int*/ SPRITE **id_array_bbb= (SPRITE **)c->e;
		int i;
		for (i=0;i<hlaser_NUM_OF_ENEMIES;i++)
		{
			SPRITE *s;	//	s=NULL;
			s				= sprite_add_res( BASE_TSHOOT_PNG+select_player );
			id_array[i] 	= s->id;
			s->type 		= SP_PL_HLASER;
			s->x			= ((((int)src->x)+src->w)>>1)-5;
			s->y			= ((((int)src->y)+src->h)>>1)+15;
			s->flags		|= SP_FLAG_VISIBLE;
			s->aktframe 	= 5-((double)6.0/hlaser_NUM_OF_ENEMIES)*i;
			if (i==0)
			{
				s->mover			= player_move_hlaser;
				PL_HOMING_DATA *data;
				data				= mmalloc(sizeof(PL_HOMING_DATA));
				s->data 			= data;
				data->nr			= j;
				data->angle512		= deg_360_to_512(100);
				data->speed 		= 12;
				data->strength		= 6;
				data->time_out/*range*/ 	= 100/*200*/;
				data->state 		= 0;
				data->target_obj	= NULL; 	//b->target_obj/*target_id*/	= /*-1*/NULL;
			}
			else
			{
				s->mover		= NULL;
				s->data 		= NULL;
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
	s->aktframe 	= (((int)pd->bomber_time>>4)&0x03);/*"bomber_slow.png"*/
	s->x			= player->x+((player->w-s->w)>>1);
	s->y			= player->y+((player->h-s->h)>>1);
	if (pd->bomber_time<=0) {	s->type=SP_DELETE;}/* ���Ԑ����ł����܂� */
}
static void player_add_hlaser_parrent(SPRITE *s)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)s->data;
	pd->bonus_flag	|= BONUS_FLAG_02_BOMB_AUTO_GET_ITEM;	/*�{���ɂ�鎩�����W�\*/
	SPRITE *c;
	c				= sprite_add_file0("bomber_slow.png",4,PR_BACK1,1);
	c->flags		|= SP_FLAG_VISIBLE;
	c->data 		= 0/*d*/;
	c->anim_speed	= 0;
	c->aktframe 	= 0;
	c->alpha		= 128/*220*/;
	c->mover		= player_move_add_bomber_hlaser;
	c->type 		= SP_PL_SHIELD;
	c->x			= s->x+((s->w-c->w)>>1);
	c->y			= s->y+((s->h-c->h)>>1);
}


/*---------------------------------------------------------
	�v���C���[�L�[����
---------------------------------------------------------*/

//static int /*bomb_wait*/d->bomber_time;		/* �{���̗L������ */	//���̃{�����o����܂ł̎���
static int weapon_List; 	//�ǂ̕���𑕔����Ă��邩

enum _weapon_type
{
	WEAPON_L1=0,
	WEAPON_L2,
	WEAPON_L3,
	WEAPON_L4,
	WEAPON_L5,
//	WEAPON_L6,
	WP_MAX		/* �ő吔 */
};
static void player_keycontrol(SPRITE *s, int hit_bomb_mode)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)s->data;
	//if player is invisible (state gameover) allow no keycontrol
	if ((!(s->flags&SP_FLAG_VISIBLE))||(pd->state==PL_RESET))
	{	return;}

	/*---------------------------------------------------------
		�v���C���[�{���֘A����
	---------------------------------------------------------*/
	if ((pd->bossmode==B03_BEFORE_EVENT) || (pd->bossmode==B06_AFTER_EVENT))
	{
		;	/* ��b�C�x���g���̓{���֘A�̏����͂��Ȃ� */
	}
	else
	{
	//	if (d->bomber_time>0)
		if (/*bomb_wait*/pd->bomber_time>0) 				//�{���E�F�C�g����
		{
			//	d->bomber_time-=fps_fa_ctor;
			/*bomb_wait*/pd->bomber_time--/*-=fps_fa_ctor*/;/**/
			if (/*bomb_wait*/pd->bomber_time<190)
			{
				pd->bonus_flag &= (~BONUS_FLAG_02_BOMB_AUTO_GET_ITEM);	/* �{���ɂ�鎩�����W�͏I��� */
			}
		}
		else
		{
			pd->bomber_time=0;
			pd->extra_type=PLX_NONE;
		//}
		//if (/*bomb_wait*/d->bomber_time<=0)
		//{
			if (pd->bombs>0)
			{
				if (my_pad & PSP_KEY_BOMB/*keybo ard[KEY_BOMB]*/)
				{
					#define KOUSOKU_MODE (0)
					#define TEISOKU_MODE (1)
					playChunk(7);
					int low=0;
					if (my_pad & PSP_KEY_SLOW/*keybo ard[KEY_SLOW]*/)
					{
						low=1;
					}
					switch ((select_player+select_player)+low)
					{
					case (REIMU*2+KOUSOKU_MODE):
					//	/*bomb_wait*/d->bomber_time 	= 200;
						pd->bomber_time = 200;			/* �{���̗L������(�ݒ薳�G����) */
					//	pd->save_delay	= 200+30;		//�ݒ薳�G���ԁB���܂ɃV�[���h�����蔲����҂������̂�
					//	pd->extra_type	= PLX_BOMB;
						player_add_shield(player);		//�V�[���h�̒ǉ�
						break;
					case (CIRNO*2+KOUSOKU_MODE):/*�Ƃ肠����*/
					case (MARISA*2+KOUSOKU_MODE):
					//	/*bomb_wait*/d->bomber_time 	= 100;
						pd->bomber_time = 100;			/* �{���̗L������(�ݒ薳�G����) */
					//	pd->save_delay	= 100+30/*200 5*/;	//�ݒ薳�G���ԁB�[���������Ƀ{�����Ԓ����G����Ȃ��̂͂���
					//	pd->extra_type	= PLX_BOMB; 	/*��������������(extra_tem�L������)�{���������o���Ȃ���*/
						player_add_levarie(player);
						break;

					case (REMILIA*2+KOUSOKU_MODE):	/* [***090220 �ǉ� */
					//	/*bomb_wait*/d->bomber_time 	= 150/*300*/ /*400*/;	/*400(150+250)�͖��炩�ɒ�������*/
						/* ���~�{���͋�������̂ŁA4������3���Ɍ��炵�A�����{���ɂ��ē��ē�����B */
						/* ���������~�{���̓{�X�ɗL���Ƃ��A������΋����B */
						pd->bomber_time = 254/* 255==0xff==4��	254==0xfe==3��==0xbf=192 2��==0x7f 150==0x96*/;
					//	pd->save_delay	= 150+30;		//�ݒ薳�G���ԁB
					//	pd->extra_type	= PLX_BOMB;
						player_add_cross_red_parrent(player);
						break;

					case (REMILIA*2+TEISOKU_MODE):/*�Ƃ肠����*/
					case (REIMU*2+TEISOKU_MODE):/*�Ƃ肠����*/
					case (MARISA*2+TEISOKU_MODE):/*�Ƃ肠����*/
					case (CIRNO*2+TEISOKU_MODE):/*�Ƃ肠����*/
						pd->bomber_time = 400/*800*/;	/* �{���̗L������(�ݒ薳�G����)[���P�d�l���ς�����̂Ŕ����ɂ���] */
						/*player_move_add_bomber_hlaser*/player_add_hlaser_parrent(player);
						break;
					#if (1==USE_PLAYER_HOMING)
					//	d->bomber_time	= 500/*1000*/;	/* �{���̗L������(�ݒ薳�G����)[���P�d�l���ς�����̂Ŕ����ɂ���] */
					//	/*player_move_add_bomber_homing*/player_add_homing_cross_red(player);
					//	break;
					#endif //(1==USE_PLAYER_HOMING)
					}
					/*	[���P] homing / hlaser �͒ᑬ�{���ɂȂ蕐��g�p���͖��G�ƂȂ����̂ŁA
						�]���̗L�����Ԃł͒�������B�����ŗL�����Ԃ𔼕��ɂ����B*/
					#define USER_BOMOUT_WAIT (30)
					/*	[���Q] �������G���� == (�ݒ薳�G���� + USER_BOMOUT_WAIT)
						�t���[���X�L�b�v�������Ă����菈�����������肷��֌W��A
						�O�����K�������`���I����Ă���Ƃ͌���Ȃ��B
						�����`���I����Ă��Ȃ��ꍇ�́A�������G���� == �ݒ薳�G���� ���ƁA
						��ʂɃ{�����\������Ă���̂ɔ���Ŏ��ʁB	*/
					/*	[���R]���Q�̗]�T�������Ă��A���݂̏�Ԃł̓{���̏I��肪
						���ɂ킩��ɂ����B�Ƃ����������킩��Ȃ��B
						���炩�̌x����g�ݓ���鎖����ΕK�v�B(�����܂�����ĂȂ�)
						(�{�Ƃł͐F�X�x��������) */
					pd->save_delay		= pd->bomber_time + USER_BOMOUT_WAIT;/*�������G����*/
					pd->extra_type		= PLX_BOMB;
					pd->hit_bomb_wait	= 0;
					pd->state			= PL_SAVE;
					pd->bombs--;
				}
			}
		}
	}
//
	if (1==hit_bomb_mode) return;
//
	/*
	if (keybo ard[KEY_LEFT]) {		direction=-1;
		if (s->x>0) {
			if (keybo ard[KEY_SLOW])	s->x-=(pd->player_speed/2)*fps_fa_ctor;
			else						s->x-=pd->player_speed*fps_fa_ctor;
		}
	}
	if (keybo ard[KEY_RIGHT]) { 	direction=1;
		if (s->x<GAME_WIDTH-s->w) { 	//�v���C���[��x���W < �X�N���[���̉���-�v���C���[�̉���
			if (keybo ard[KEY_SLOW])
				s->x+=(pd->player_speed/2)*fps_fa_ctor;
			else
				s->x+=pd->player_speed*fps_fa_ctor;
		}
	}
	if ((keybo ard[KEY_UP]) && (s->y>0)) {
		if (keybo ard[KEY_SLOW])	s->y-=(pd->player_speed/2)*fps_fa_ctor;
		else						s->y-=pd->player_speed*fps_fa_ctor;
		if (pd->weapon==127)	//128[***090126 	�ύX
			if (s->y<50)				pd->bonus=0x01;
		//parsys_add(NULL,50,0,s->x+((s->w)>>1),s->y+s->h,10,90,10,100,PIXELATE,NULL);
	}
	if ((keybo ard[KEY_DOWN]) && (s->y<screen->h-s->h)) {
		if (keybo ard[KEY_SLOW])	s->y+=(pd->player_speed/2)*fps_fa_ctor;
		else						s->y+=pd->player_speed*fps_fa_ctor;
		if (s->y>50)			pd->bonus&=~0x01;
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
	/*0000*/ {0,0},
	/*0001*/ { 0, -256},/*0 U*/
	/*0010*/ { 256, 0},/*2 R*/
	/*0011*/ { 181, -181},/*1 UR*/
	/*0100*/ { 0, 256},/*4 D*/
	/*0101*/ {0,0},
	/*0110*/ { 181, 181},/*3 DR*/
	/*0111*/ {0,0},
	/*1000*/ {-256, 0},/*6 L*/
	/*1001*/ {-181, -181},/*7 UL*/
	/*1010*/ {0,0},
	/*1011*/ {0,0},
	/*1100*/ {-181, 181},/*5 DL*/
	/*1101*/ {0,0},
	/*1110*/ {0,0},
	/*1111*/ {0,0},
	};

	const unsigned int my_slow =(my_pad & PSP_KEY_SLOW/*keybo ard[KEY_SLOW]*/)?(1):(0); 	//�l���V�t�g���Ĕ����ɂ���H
	s->x += ((pd->player_speed*(shipMv[((my_pad&0xf0)>>4)][0] >>my_slow))>>8) /**fps_fa_ctor*/;
	s->y += ((pd->player_speed*(shipMv[((my_pad&0xf0)>>4)][1] >>my_slow))>>8) /**fps_fa_ctor*/;
	// �݂͂�������C���B
		 if (s->x < 0)					{	s->x = 0;					}
	else if (s->x > GAME_WIDTH-s->w)	{	s->x = GAME_WIDTH-s->w; 	}
		 if (s->y < 0)					{	s->y = 0;					}
	else if (s->y > GAME_HEIGHT-s->h)	{	s->y = GAME_HEIGHT-s->h;	}

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
#define SPLIT_LINE (80)
	/* MAX���̃A�C�e���������W */
	if (my_pad & PSP_KEY_UP/*PSP_CTRL_UP*/) 	/* ���F�΂ߏ�ł�����\ */ /*&& (s->y>0)*/
	{
		if (pd->weapon==127) //128[***090123 �ύX /*max==127==�u128�i�K�v*/
		{
			if (s->y < SPLIT_LINE/*50*/)/**/
			{
				pd->bonus_flag |= BONUS_FLAG_01_PLAYER_UP_AUTO_GET_ITEM;	/*�㕔�������W�\*/
			}
		}
	}
	else if (my_pad & PSP_KEY_DOWN/*PSP_CTRL_DOWN*/)	/* ���F�΂߉��ł���߂� */ /*&& (s->y<screen->h*-s->h)*/
	{
		//if (pd->bonus_flag & (BONUS_FLAG_01_PLAYER_UP_AUTO_GET_ITEM /*| BONUS_FLAG_02_BOMB_AUTO_GET_ITEM*/) )/*???���Ԃ�*/
		{
			pd->bonus_flag &= (~BONUS_FLAG_01_PLAYER_UP_AUTO_GET_ITEM); 	/*�㕔�������W�s��*/
		}
	}
//
	/*---------------------------------------------------------
		�v���C���[�V���b�g�֘A����
	---------------------------------------------------------*/
	if ( (pd->bossmode!=B03_BEFORE_EVENT) && (pd->bossmode!=B06_AFTER_EVENT))
	{
		if (my_pad & PSP_KEY_SHOT/*keybo ard[KEY_SHOT]*/)
		{
			{static int weapon_interval=0;
				if (/*pd->*/weapon_interval>0)
				{	/*pd->*/weapon_interval--/*=fps_fa_ctor*/;}
				else //if (/*pd->*/weapon_interval<=0)
				{
					playChunk(0);
					switch (select_player)
					{
					case REIMU:
						switch (weapon_List)
						{
						case WEAPON_L1: 	player_add_shot(s,REIMU_SHOT_TYPE_00);	break;
						case WEAPON_L2: 	player_add_dual_shot(s);				break;
						case WEAPON_L3: 	player_add_triple_fuda(s);				break;
						case WEAPON_L4: 	player_add_quad_fuda(s);				break;
						case WEAPON_L5: 	player_add_five_fuda(s);				break;
						}
						break;
					case CIRNO:/*�Ƃ肠����*/
					case MARISA:
						switch (weapon_List)
						{
						case WEAPON_L1: 	player_add_shot(s,REIMU_SHOT_TYPE_00);	break;
						case WEAPON_L2: 	player_add_dual_shot(s);				break;
						case WEAPON_L3: 	player_add_dual_shot(s);				break;
						case WEAPON_L4: 	player_add_triple_star(s);				break;
						case WEAPON_L5: 	player_add_quad_star(s);				break;
						}
						break;
					case REMILIA:
						switch (weapon_List)
						{
						case WEAPON_L1: 	player_add_shot(s,REIMU_SHOT_TYPE_00);		break;
						case WEAPON_L2: 	player_add_shot(s,REIMU_SHOT_TYPE_00);		break;
						case WEAPON_L3: 	player_add_dual_shot(s);					break;
						case WEAPON_L4: 	player_add_dual_shot(s);					break;
						case WEAPON_L5: 	player_add_dual_shot(s);					break;
						#if (1==USE_PLAYER_WAVE)
						case WEAPON_L6: 	player_add_killray(s);						break;
						#endif //(1==USE_PLAYER_WAVE)
						}
						break;
					}
					#define BASE_WEAPON_L1		(4*0)
					#define BASE_WEAPON_L2		(4*1)
					#define BASE_WEAPON_L3		(4*2)
					#define BASE_WEAPON_L4		(4*3)
					#define BASE_WEAPON_L5		(4*4)
					#define BASE_WEAPON_MAX 	(4*5)
					static Uint8 www_tbl[BASE_WEAPON_MAX] =
					{/* REIMU MARISA REMILIA CIRNO */	/* ���~��������̂Œ���(+5) */
						 5,  6,  7+5,  9,	/* WEAPON_L1 */
						 5,  6,  6+5,  9,	/* WEAPON_L2 */
						 5,  5,  7+5,  9,	/* WEAPON_L3 */
						 5,  5,  6+5,  9,	/* WEAPON_L4 */
						 5,  5,  5+5,  9,	/* WEAPON_L5 */
					};
					/*pd->*/weapon_interval = www_tbl[(4*weapon_List)+select_player];
				}
			}
			#if 0
//			{static int extra_interval = 0;
//				if (/*pd->*/extra_interval>0)
//				{	/*pd->*/extra_interval--/*=fps_fa_ctor*/;}
//				else //if (pd->extra_interval<=0)
//				{
//					switch (pd->extra_type)
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
			/*pd->*/anim_delay = player_status[BASE_SPEED_ANIME+select_player]/*2*/;
			//
			static int auto_aktframe=4/*5*/;
				 if (my_pad & PSP_KEY_LEFT/*PSP_CTRL_LEFT*/)	{	if (auto_aktframe>0   )    auto_aktframe--; }
			else if (my_pad & PSP_KEY_RIGHT/*PSP_CTRL_RIGHT*/)	{	if (auto_aktframe</*7*/8/*10*/) auto_aktframe++; }
			else
			{
				if (auto_aktframe>4/*5*/) auto_aktframe--;
				if (auto_aktframe<4/*5*/) auto_aktframe++;
			}
			{
				static int auto_aniime;
				auto_aniime++;
				auto_aniime&=3;
					 if (4==auto_aktframe)	{	s->aktframe = (auto_aktframe + auto_aniime);	}
				else if (4<auto_aktframe)	{	s->aktframe = (auto_aktframe + 3);				}
				else						{	s->aktframe = (auto_aktframe ); 				}
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
/* [pd->weapon���ύX���ꂽ�ꍇ�ɕK���s����`�F�b�N] */
static void check_weapon_level(PLAYER_DATA *pd)
{
	int pd_weapon;
	pd_weapon=pd->weapon;
	/*---------------------------------------------------------
		����̒i�K�����߂�
	---------------------------------------------------------*/
	//weapon�̒i�K���獡�̑��������߂�		//���X�g�����đ����̕ύX
	// [***090123	�ő�128�ցB
	// (0-128��129�i�K���������A0-127��128�i�K�ɏC��)
		 if (pd_weapon <= ( 5-1)) 	{	weapon_List=WEAPON_L1;	}
	else if (pd_weapon <= (10-1)) 	{	weapon_List=WEAPON_L2;	}
	else if (pd_weapon <= (60-1)) 	{	weapon_List=WEAPON_L3;	}
	else if (pd_weapon <= (85-1)) 	{	weapon_List=WEAPON_L4;	}
	else /*if (pd_weapon<=(128-1))*/{	weapon_List=WEAPON_L5;	}	/*max==127==�u128�i�K�v*/
	//else							{	weapon_List=WEAPON_L6;	}
	/*---------------------------------------------------------
		�v���C���[�I�v�V�����`�F�b�N(�K�v�ł���΃I�v�V�������Z�b�g)
	---------------------------------------------------------*/
	if (pd_weapon > (35-1))
	{
	//	if (0==(pd->bonus_flag&BONUS_FLAG_08_OPTION_HIDE))/*(0==pd->option_flag)*/ /* �I�v�V�����������Ă�� */
		if (0==option_flag) 	/* �I�v�V�����������Ă�� */
		{
		//	pd->bonus_flag |= BONUS_FLAG_08_OPTION_HIDE;/*pd->option_flag=1;*/ /* �I�v�V������ǉ� */
			option_flag=1;	/* �I�v�V������ǉ� */
			switch (select_player)
			{
			case REIMU: 		re_add_option(player);		break;
			case MARISA:		ma_add_option(player);		break;
			case CIRNO:/*�Ƃ肠����*/
			case REMILIA:		oz_add_option(player);		break;/* [***090220 �ǉ� */
			}
		}
	}
	else
	{
	//	pd->bonus_flag &= (~BONUS_FLAG_08_OPTION_HIDE);//pd->option_flag=0;/* �I�v�V���������� */
		option_flag=0;	/* �I�v�V���������� */
	}
}

/*---------------------------------------------------------
	�v���C���[�����蔻��
---------------------------------------------------------*/

//�O���C�Y�p�A�eid�i�[�ꏊ�B
//1�ɂ��Ȃ������͕̂����̒e�𓯎��ɃO���C�Y�����Ƃ��̂��߁B
//static int graze_check[5]={-1,-1,-1,-1,-1};

static int weapon_chain;		//�ǂꂾ���A����wepon_up���������

static void player_colcheck(SPRITE *s, int mask)
{
	//SDL_Surface *spimg;
	//is_graze=0;
	/*
		s		�v���C���[
		mask	���ׂ�ΏہH
				�G���G�̒e���A�C�e����
	*/
	/* Kollision Player <> Feind, Feindwaffe oder Bonusitem */
	SPRITE *c;	//�Ώ�
	if (NULL!=(c=sprite_colcheck(s,mask)))
	{
	//	int effect_sound_number;/* ���ʉ� */
	//	effect_sound_number = 99;/* ���ʉ��Ȃ� */
		//
		PLAYER_DATA *pd=(PLAYER_DATA *)s->data;
		switch (c->type)
		{
		case SP_BONUS_00_FIREPOWER:
			if (pd->weapon<127) 	//128max�łȂ��Ƃ� /*max==127==�u128�i�K�v*/
			{
				pd->weapon++;
				bonus_info_add(c, SP_BONUS_00_FIREPOWER/*"weapon.png"*/);
			}
			else					//max�̎�
			{
				weapon_chain++;
				//�E�F�|���{�[�i�X���_�̕\��
				bonus_info_any_score(c, (weapon_chain));
			}
			check_weapon_level(pd);/* [pd->weapon���ύX���ꂽ�ꍇ�ɕK���s����`�F�b�N] */
			c->type=SP_DELETE;/* �����܂� */
			/*effect_sound_number=*/playChunk(5);
			break;

		case SP_BONUS_04_FIREPOWER_128: 	//�E�F�|���A�C�e��(F)		// [***090123		�ǉ�
			goto my_128;
		case SP_BONUS_01_FIREPOWER_G:		//�E�F�|���A�C�e��(��)		// [***090123		�ǉ�
			if (pd->weapon<(120-1))
			{
				pd->weapon+=8;
				bonus_info_add(c, SP_BONUS_00_FIREPOWER/*"weapon.png"*/);
			}
			else
			{
my_128:
				if (pd->weapon==127)/*127==128�i�K��*/
				{
					weapon_chain+=8;
					//�E�F�|���{�[�i�X���_�̕\��
					bonus_info_any_score(c, (weapon_chain));
				}
				else
				{
					pd->weapon=127;/*max==127==�u128�i�K�v*/
				}
			}
			check_weapon_level(pd);/* [pd->weapon���ύX���ꂽ�ꍇ�ɕK���s����`�F�b�N] */
			c->type=SP_DELETE;/* �����܂� */
			/*effect_sound_number=*/playChunk(5);
			break;

		case SP_BONUS_02_BOMB:
			//player_add_bomb(c);		// [*****�{���̓R�����g�A�E�g���Ȃ���
			if (8 < pd->bombs)	{	goto add_1000pts;	}
			pd->bombs++;
			bonus_info_add(c, SP_BONUS_02_BOMB/*"firebomb.png"*/);
			c->type=SP_DELETE;/* �����܂� */
			/*effect_sound_number=*/playChunk(5);
			break;

		case SP_BONUS_03_HEALTH:
			if (8 < pd->lives)	{	goto add_1000pts;	}
			pd->lives++;
			bonus_info_add(c, SP_BONUS_03_HEALTH/*"energy.png"*/);
			c->type=SP_DELETE;/* �����܂� */
			/*effect_sound_number=*/playChunk(6);
			break;

		case SP_BONUS_07_ADDSPEED:
			if (pd->player_speed < player_status[BASE_SPEED_MAXIMUM+select_player]/*pd->player_speed_maximum*/)
			{	;	}	else	{	goto add_1000pts;	}
			pd->player_speed++;
			bonus_info_add(c, SP_BONUS_07_ADDSPEED/*"speed.png"*/);
			c->type=SP_DELETE;/* �����܂� */
			/*effect_sound_number=*/playChunk(5);
			break;

add_1000pts:
		case SP_BONUS_05_HOSI:		/*not_break;*/
		case SP_BONUS_06_COIN:
			if ( (GAME_HEIGHT/2) > c->y )	/* ��Ŏ��� 1000pts. */
			{
				pd->score+=score(1000);
				bonus_info_add(c, SP_BONUS_05_HOSI/*"plus1000.png"*/);
			}
			else	/* ���Ŏ��� 100pts. */
			{
				pd->score+=score(100);		// [***090123		�ύX
				bonus_info_add(c, SP_BONUS_06_COIN/*"plus100.png"*/);		// [***090123		�ύX
			}
			c->type=SP_DELETE;/* �����܂� */
			/*effect_sound_number=*/playChunk(5);
			break;


//add_1000pts:
//			pd->score+=score(1000);
//			bonus_info_add(c, SP_BONUS_05_HOSI/*"plus1000.png"*/);
//			c->type=SP_DELETE;/* �����܂� */
//			break;

			//spimg=sprite_getcurrimg(c);
			//parsys_add(spimg, c->w,1, c->x,c->y, 5, 0, 0, 80, LINESPLIT, NULL);
			//SDL_FreeSurface(spimg);

		#if 0
		case SP_BONUS_EXTRA_SHIELD: 	//�E�F�|���A�C�e��(��)
			if (pd->bomber_time > 0)	{	goto add_1000pts;	} //�{�����͂��������Ă�1000�|�C���g���邾������
			//
		//	sprite_remove_all_type(SP_PL_SHIELD);
		//	pd->extra_type=PLX_NONE/*PLX_SHIELD*/;		//�E�F�|���A�C�e��(��)
		//	pd->bomber_time=30;
			if (pd->weapon<(112-1)) 	// [***090123		�ύX
			{
				pd->weapon+=16;
				bonus_info_add(c, SP_BONUS_00_FIREPOWER/*"weapon.png"*/);
			}
			else
			if (pd->weapon==127)	// [***090123		�ύX/*max==127==�u128�i�K�v*/
			{
				weapon_chain+=16;
				goto add_1000pts;
			}
			else
			{
				pd->weapon=127; 		// [***090123		�ύX/*max==127==�u128�i�K�v*/
			}
			check_weapon_level(pd);/* [pd->weapon���ύX���ꂽ�ꍇ�ɕK���s����`�F�b�N] */
			c->type=SP_DELETE;/* �����܂� */
			/*effect_sound_number=*/playChunk(5);
			break;
		#endif

		#if 0
//		case SP_BONUS_EXTRA_HOMING:
//			if (pd->bomber_time > 0)	{	goto add_1000pts;	} //�{�����͂��������Ă�1000�|�C���g���邾������
//			//
//		//	sprite_remove_all_type(SP_PL_SHIELD);
//			pd->extra_type		= PLX_BOMB/*PLX_HOMING*/;
//			pd->bomber_time 	= 800;
//			bonus_info_s_text(c, "homing missiles",FONT04);
//			bonus_info_add(c, /*"extra.png"*/);
//			//spimg=sprite_getcurrimg(c);
//			//parsys_add(spimg, c->w,1, c->x,c->y, 5, 0, 0, 80, LINESPLIT, NULL);
//			//SDL_FreeSurface(spimg);
//			c->type=SP_DELETE;/* �����܂� */
//			/*effect_sound_number=*/playChunk(5);
//			break;
//
//		case SP_BONUS_EXTRA_HLASER:
//			if (pd->bomber_time > 0)	{	goto add_1000pts;	} //�{�����͂��������Ă�1000�|�C���g���邾������
//			//
//		//	sprite_remove_all_type(SP_PL_SHIELD);
//			pd->extra_type		= PLX_BOMB/*PLX_HLASER*/;
//			pd->bomber_time 	= 1000;
//			bonus_info_s_text(c, "homing laser",FONT04);
//			bonus_info_add(c, /*"extra.png"*/);
//			//spimg=sprite_getcurrimg(c);
//			//parsys_add(spimg, c->w,1, c->x,c->y, 5, 0, 0, 80, LINESPLIT, NULL);
//			//SDL_FreeSurface(spimg);
//			c->type=SP_DELETE;/* �����܂� */
//			/*effect_sound_number=*/playChunk(5);
//			break;
		#endif


		#if 0
		case SP_EN_BOSS01:		enemy_boss01_hitbyplayer(c);	break;
		case SP_EN_BOSS02:		enemy_boss02_hitbyplayer(c);	break;
		case SP_EN_BOSS03:		enemy_boss03_hitbyplayer(c);	break;
		case SP_EN_BOSS04:		enemy_boss04_hitbyplayer(c);	break;
		#else
		case SP_EN_BOSS:		(((PLAYER_DATA *)player->data)->callback_boss_hitbyplayer)(c);	break;
		#endif
		case SP_EN_BULLET:		/*not_break;*/
	//	case SP_EN_LASER:		/*not_break;*/
	//	case SP_EN_BIGBULLET:	/*not_break;*/
	//	case SP_EN_BOSS02ICE:	/*not_break;*/
			is_graze=1;
			if (0==(c->flags & SP_FLAG_GRAZE)) /* �O���C�Y��? */ /* if (l==5) ����ID�̒e������������*/
			{
				c->flags |= SP_FLAG_GRAZE;/*�O���C�Y��*/
				pd->graze++;/*�O���C�Y��*/
				pd->score += (score(100)+score(200)*difficulty);
				/*effect_sound_number=*/playChunk(9);
			}
			break;

		case SP_EN_BIGBULLET_DUMMY: 	//2�ڂ̃X�v���C�g�͓����蔻�肪����
			break;

		default:			//�G���̂��̂ɓ��������ꍇ
		//	spimg=sprite_getcurrimg(c);
		//	parsys_add(spimg, 2,2, c->x,c->y, 10, 270, 10, 30, EXPLODE|DIFFSIZE, NULL);
			//SDL_FreeSurface(spimg);
		//	spimg=NULL;
			pd->state=PL_HIT_BOMB;		// [***090125		�ǉ�
			pd->enemy=c;/* ���������G���o���Ƃ� */
			//explosion_add(c->x+5,c->y+5,0,EXPLOSION_MINI00);		// [***090125		�ȉ��R�����g�A�E�g
			//pd->explode=0;

			//spimg=sprite_getcurrimg(s);
			//parsys_add(spimg, 2,2, c->x,c->y, 10, 90, 10, 30, EXPLODE|DIFFSIZE, &pd->explode);
			//SDL_FreeSurface(spimg);
			//spimg=NULL;
			//c->type=SP_DELETE;/* �����܂� */
			//s->flags&= ~SP_FLAG_VISIBLE;
			//pd->core->flags&= ~SP_FLAG_VISIBLE;	//��������
			//pd->lives--;
			//pd->bonus=0;
			/*effect_sound_number=*/playChunk(4);		// [***090127	�ύX��
		}
	//	if (99 != effect_sound_number)/* ���ʉ������ */
	//	{
	//		playChunk(effect_sound_number);/* ���ʉ��炷 */
	//	}
	}
}

/*---------------------------------------------------------
	�v���C���[�ړ�
---------------------------------------------------------*/

static void player_move(SPRITE *s1)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)s1->data;
	switch (pd->state)
	{
	case PL_NORMAL:/*�ʏ폈��*/
		player_keycontrol(s1, 0);
		player_colcheck(s1,SP_SHOW_ENEMYS|SP_SHOW_ENEMY_WEAPONS|SP_SHOW_BONUS);
		break;

	case PL_HIT_BOMB:/* ��炢�{����t�� */ 				// [***090125		�ǉ�
		if (player_status[BASE_HIT_BOMB_WAIT+select_player] < pd->hit_bomb_wait) /*<6*/
		{
			pd->hit_bomb_wait=0;
			/* ���S�m�� */
			pd->state			= PL_EXPLODE;
		}
		else
		{
			pd->hit_bomb_wait++;
			/* ��炢�{����t�� */
			player_keycontrol(s1, 1);
		}
		break;

	case PL_EXPLODE:/*�v���C���[������*/
	//	if (pd->lives<0) {	;	}else
		{
			//�����ŃA�C�e����f���o���悤�ɂ���΂�����ۂ��Ȃ邩��
			/* GAMEOUT�ł��A�C�e���f���ׂ�(�Ōゾ���f���Ȃ��͈̂�a��������) */
			bonus_multi_add(s1, SP_BONUS_00_FIREPOWER, 5, BONUS_FLAG_RAND_XY);//�Ƃ����킯�Ŏ���
			// [***090124������ƒǉ�
			bonus_multi_add(s1, ((pd->weapon>(30-1))?(SP_BONUS_01_FIREPOWER_G):(SP_BONUS_04_FIREPOWER_128)), 1, BONUS_FLAG_RAND_XY);
			/* �ɒ[�Ɏキ�Ď��񂾏ꍇ�̋~�ύ�: ��肢�l�͏��Ղ� FULLPOWER �ɂȂ�A����ł��{������Ȃǂ� POWER ���ێ�����̂ŁA */
			/* �ɒ[�Ɏキ�Ă͎��ȂȂ��B �ɒ[�Ɏキ�Ď��ʂ̂͌��\���肾�Ǝv���̂ŁA���̏ꍇ FULLPOWERUP �ɂ����B */
		}
//			pd->explode 		= 0;
//		if (0==pd->explode)
		{
			//playChunk(4); 	// [***090127	�ꏊ���ړ�����B
			/* �����G�t�F�N�g */

			/* ���������G���� */
			explosion_add(pd->enemy->x+5,pd->enemy->y+5,0,EXPLOSION_MINI00);
			#if 0/* ���������G�͓|���Ȃ��G��������Ȃ��̂ŁA�����ŕK�������̂͂܂����C������ */
			pd->enemy->type 	= SP_DELETE;/*???*/
			#endif
			/* �������� */
			explosion_add(s1->x+5, s1->y+5, 0,EXPLOSION_MINI00);
			explosion_add(s1->x+5, s1->y+20,0,EXPLOSION_MINI00);
			explosion_add(s1->x+20,s1->y+5, 0,EXPLOSION_MINI00);
			explosion_add(s1->x+20,s1->y+20,0,EXPLOSION_MINI00);

			/*	*/
			s1->flags			&= ~SP_FLAG_VISIBLE;		// ���t���O��OFF(�s��)
			pd->core->flags 	&= ~SP_FLAG_VISIBLE;		// ������
			pd->extra_type		= PLX_NONE;
			pd->bomber_time 	= 0;			/* �����������������Ŗ�����(&�`���m)�{�����ĂȂ��Ȃ��Ă� */
			pd->bonus_flag		= BONUS_FLAG_00_NONE;

			pd->save_delay		= 150;						// ���G����
			pd->state			= PL_RESET; 				// ���G��ԁH

			/* �c�`�����X���炷 */
			pd->lives--;
			if (pd->lives<0)	/* �c�`�����X���Ȃ���� */
			{
				;/* GAMEOUT�� */
			}
			else
			{
				s1->flags			|= SP_FLAG_VISIBLE; 		// ���t���O��ON(��)
				pd->core->flags 	|= SP_FLAG_VISIBLE; 		// ���\��
				//
				pd->bombs = player_status[BASE_BOMBS+select_player]/*3*/;			// �{���������̏�����
				if (pd->player_speed > player_status[BASE_SPEED_MINIMUM+select_player]/*pd->player_speed_minimum*/) 			//player_speed_minimum�ȉ��ɂȂ�Ȃ��悤��
				{	pd->player_speed--;}
				pd->weapon -= (difficulty*4)+12;	// ���탌�x���̒ቺ
				if ( pd->weapon<0)	{	pd->weapon=0;}
				weapon_chain=0; 					// weapon_chain�̏�����
			}
			check_weapon_level(pd);/* [pd->weapon���ύX���ꂽ�ꍇ�ɕK���s����`�F�b�N] */
			s1->x=GAME_WIDTH/2-((s1->w)>>1);			// �v���C���[�ʒu�̏�����
			s1->y=GAME_HEIGHT;
		}
		break;

	case PL_RESET:/*�v���C���[�������P(��ʊO���畜���n�_�֓_�ł��Ȃ���ړ���)*/
		if (pd->save_delay>120) //������Ƃ����҂�����
		{	pd->save_delay-=1/*fps_fa_ctor*/;}
		else
		{
			if (s1->y<220/*240*/)	//������ƑO�ɏo�Ă���
			{
				pd->state=PL_SAVE;
			}
			else
			{
				s1->y-=1/*fps_fa_ctor*/;
			//	if (((int)(s1->y/10)&(2-1)/*%2*/)==0)		// �_�ł��邩�ȂƎv���ē���Ă݂����ǂ��Ȃ������恨�����������Ă��B
				if (((unsigned int)(s1->y)&((2*8)-1))==0)	// �_�ł��Ă��
				{
					s1->alpha		= 80;
					pd->core->alpha = 80;
				}
				else
				{
					s1->alpha		= 0/*255*/;
					pd->core->alpha = 0/*255*/;
				}
			}
		}
		break;/*??? [***090210 �ǉ� */

	case PL_SAVE:/*�v���C���[�������Q*/
		player_keycontrol(s1, 0);
		player_colcheck(s1,SP_SHOW_BONUS);
		if (pd->save_delay>0)
		{
			pd->save_delay-=1/*fps_fa_ctor*/;
			s1->alpha=80;
			if (pd->lives<0)	// �Q�[���I�[�o�[���́��̕\������
			{	pd->core->alpha=0;}
			else
			{	pd->core->alpha=80;}
		}
		else
		{
			pd->state		= PL_NORMAL;
			s1->alpha		= 255;
			pd->core->alpha = 255;
		}
		break;
	}
}

/*---------------------------------------------------------
	�v���C���[������
---------------------------------------------------------*/

void player_init(void)
{
	player				= sprite_add_res( BASE_SHIP_MED_PNG+select_player );		/* [***090220 �ǉ� */
	player->type		= SP_PLAYER;
	player->flags		|= (SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	player->mover		= player_move;
	player->x			= GAME_WIDTH/2;
	player->y			= GAME_HEIGHT-player->h;
	player->aktframe	= 5;
	PLAYER_DATA *pd;
	pd					= mmalloc(sizeof(PLAYER_DATA));
	player->data		= pd;
//	pd->player_speed_maximum	= player_status[BASE_SPEED_MAXIMUM+select_player];
//	pd->player_speed_minimum	= player_status[BASE_SPEED_MINIMUM+select_player];
	pd->player_speed			= player_status[BASE_SPEED_MINIMUM+select_player]/*pd->player_speed_minimum*/;

	pd->bossmode		= B00_NONE;
	pd->state			= PL_RESET/*PL_SAVE*/;
	pd->save_delay		= 120;
//	pd->anim_delay		= 0;
	pd->weapon			= 0;
	pd->extra_type		= PLX_NONE;
	pd->bomber_time 	= 0;	/*==bomb_wait*/
//	pd->explode 		= 0;
//	pd->weapon_interval = 0;
//	pd->extra_interval	= 0;

	pd->graze			= 0;
	pd->lives			= player_status[BASE_LIVES+select_player];
	pd->bombs			= player_status[BASE_BOMBS+select_player];
	pd->score			= score(0);
	pd->bonus_flag		= BONUS_FLAG_00_NONE;	// �z�� pd->option_flag 	= 0;/* �I�v�V���������� */
	option_flag 		= 0;/* �I�v�V���������� */
	pd->hit_bomb_wait	= 0;	// [***090125		�ǉ�
	pd->boss			= NULL;
	pd->core			= player_add_core(player); // ���̒ǉ�

	weapon_List 		= 0;
	weapon_chain		= 0;
}
