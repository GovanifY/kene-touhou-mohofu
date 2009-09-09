
#include "bullet_object.h"

/*---------------------------------------------------------
	�P��
	������
	-------------------------------------------------------
	ToDo:
	�{�X�^�C�}�[�Ή���
---------------------------------------------------------*/

typedef struct /*_boss03_data*/
{
	BOSS_BASE boss_base;
//	int state;
	int health_flag;
	int identity;		// �ŗL�ԍ�
//	int level;

} BOSS03_DATA;

// FLG_FIRE1 �e�t���O�B�^�񒆂̕󕨂����Ă��āA���w��ʒu�ɕt�������Bon�ɂȂ�B
//#define FLG_FIRE1 0x01/*����̗�����(��)���w��ʒu�ɕt������*/
//#define FLG_FIRE2 0x02/*�E��̕ɂ̖�(��)���w��ʒu�ɕt������*/
//
#define FLG_DOLL0 0x04/*�P��{�l*/
#define FLG_DOLL1 0x08/*����̗�����(��)*/
#define FLG_DOLL2 0x10/*�E��̕ɂ̖�(��)*/
#define FLG_DOLL3 0x20/*���O�̕�*/
#define FLG_DOLL4 0x40/*�^���̍g����(��)*/
#define FLG_DOLL5 0x80/*�E�O�̕�*/

#define FLG_ALL_DOLL	(FLG_DOLL1|FLG_DOLL2|FLG_DOLL3|FLG_DOLL4|FLG_DOLL5)/*�P��ȊO*/
#define FLG_ALL_CAST	(FLG_DOLL0|FLG_ALL_DOLL)/*�S��*/

/*---------------------------------------------------------

---------------------------------------------------------*/

static unsigned int b01flags/*=0*/;
static int rd4_zero;

static int b01state;
static int bb_angle512;

static SPRITE *sb03[6];

/*---------------------------------------------------------
�l���̔z�u����
[1][0][2]
[3][4][5]
---------------------------------------------------------*/

static void callback_enemy_boss03_hitbyweapon_dummy(SPRITE *s, SPRITE *t/*, int angle*/)
{	/* ���S�Ɏp�������܂ōU���́A���ׂĖ����Ƃ���B */
}
/*	s = �{�X�{�� boss sprite
	t = �v���C���[�̒e player's weapon */
static void callback_enemy_boss03_hitbyweapon(SPRITE *s, SPRITE *t/*, int angle*/)
{
	explosion_add_type(t->x256,t->y256,/*0,*/EXPLOSION_MINI00);/*��Ɏ��s������������*/
//
	WEAPON_BASE *w		= (WEAPON_BASE *)t->data;
	BOSS03_DATA *data	= (BOSS03_DATA *)s->data;
//
	{
		data->boss_base.boss_health -= w->strength;/* �{�X�U�����đ̗͌��炷 */
		if (0 > data->boss_base.boss_health)
		{
			data->boss_base.boss_health = 0;
		}
	}
//
	int i_id;
	i_id = data->identity;
//
	if (0==i_id)	/* �P�邩�ǂ����`�F�b�N */
	{
		return; 	/* �P��̏ꍇ�����ŏ������Ȃ� */
	}
//
/* ----[�����ȍ~�́A������p����]---- */
	{
	//	if (b_health_alter_low1024 < (data->boss_base.boss_health & (1024-1)))/* �`�ԕύX������ */
		if (0==data->boss_base.boss_health)
		{
			if (0 != (data->boss_base.boss_life))/* �`�ԕύX������ */
			{
				data->boss_base.boss_life--;
				data->boss_base.boss_health 	= (1024-1); 	/* life_point����Q�[�W��[ */
			//	if (0==i_id)	// �P�邩�ǂ����`�F�b�N
			//	{
			//		data->boss_base.boss_timer		= (40*64);	/* 40*64==40[count] 	��40[�b(64/60)](�P�ʂ͕b�ł͂Ȃ�) */		/* �{�X�^�C�}�[������ */
			//		item_create_for_boss(s, ITEM_CREATE_MODE_02);
			//	}
			}
		}
	}
//

	if (0 == data->boss_base.boss_life)
	{
		if (data->boss_base.boss_health <= 15)
		{
			if (0==data->health_flag)
			{
				data->health_flag = 1;
				explosion_add_type(s->x256+((s->w128)),s->y256+((s->h128)),/*0,*/EXPLOSION_FIRE08);
			}
		//	if (data->boss_base.boss_health <= 0)/* �Y���҂̗̑͐؂�Ȃ� */
			if (0 == data->boss_base.boss_health)/* �Y���҂̗̑͐؂�Ȃ� */
			{
		//		data->boss_base.boss_health = 0;
				//
				explosion_add_type(s->x256+((s->w128)),s->y256+((s->h128)),/*0,*/EXPLOSION_ZAKO04/*EXPLOSION_FIRE08*/);
				s->flags			&= (~(SP_FLAG_VISIBLE));
				item_create_for_boss(s, ITEM_CREATE_MODE_02);
				player_add_score(data->boss_base.score);/*�P��ȊO�̏����̃X�R�A*/
				{
					b01flags			|= (FLG_DOLL0<<i_id);/* �|�����l��ON�ɂ��� */
//					if (0==i_id)	// �P�邩�ǂ����`�F�b�N
//					{
//						b01flags |= FLG_ALL_CAST;	/* �P���|���ƊF�j�󂳂��B */
//					}
				}
			}
		}
	}

}

/*---------------------------------------------------------

---------------------------------------------------------*/
//	if (0==i_id)	// �P�邩�ǂ����`�F�b�N

static void kaguya_put_items(SPRITE *s/*, SPRITE *t*/)
{
	play_voice_auto_track(VOICE07_BOMB);
//
	item_from_bullets(SP_ITEM_05_HOSI);
	item_create_for_boss(s, ITEM_CREATE_MODE_02);
//
	BOSS03_DATA *data = (BOSS03_DATA *)s->data;
	data->boss_base.boss_health 	= (1024-1); 	/* life_point����Q�[�W��[ */
	data->boss_base.boss_timer		= (40*64);		/* 40*64==40[count] 	��40[�b(64/60)](�P�ʂ͕b�ł͂Ȃ�) */
	/* �{�X�^�C�}�[������ */
}

	//parsys_add(NULL,100,0,t->x,t->y,30,0,0,50,PIXELATE,NULL);
	//	int b_health_alter_low1024;
	//	b_health_alter_low1024 = (data->boss_base.boss_health & (1024-1));/* ���O�̉���1024 */

/*---------------------------------------------------------

---------------------------------------------------------*/

static void enemy_boss03_setpos(int xxx, int yyy)
{
int x256;
int y256;
	x256 = (xxx<<8);
	y256 = (yyy<<8);
//
	sb03[0]->x256 = x256 + (t256(34));
	sb03[0]->y256 = y256;
//
	bb_angle512++;
int aa_angle512;
	aa_angle512 = bb_angle512;
int jj;
	for (jj=1; jj<6; jj++)
	{
		aa_angle512 += 102;/* 102.4== 512/5 */
		mask512(aa_angle512);
		/*CCW*/
		sb03[jj]->x256 = sb03[0]->x256 + ((sin512((aa_angle512))*45));
		sb03[jj]->y256 = sb03[0]->y256 + ((cos512((aa_angle512))*45));
	}
}
/*---------------------------------------------------------

---------------------------------------------------------*/

/*int where / 0: left, 1: right, 2: bombenhagel */
//	switch (where)//	case 2:
static void enemy_boss01_nway_fire(BOSS03_DATA *data)
{
	static int hari=0;
	hari--;
	hari &= 32;
	play_voice_auto_track(VOICE14_YOKAI1_E_SHOT01);
//
	int hari_no_iro;
	hari_no_iro = BULLET_HARI32_00_AOI;
	/* 4�ʂ̏ꍇ---- */
	if (0==rd4_zero/*0==data->level*/)
	{
		unsigned short iro_tbl[8] =
		{
/**/		BULLET_HARI32_01_AKA,
/*1*/		BULLET_HARI32_01_AKA,
/*2*/		BULLET_HARI32_03_DAIDAI,
/*3*/		BULLET_HARI32_02_KIIRO,
/*4*/		BULLET_HARI32_00_AOI,
/*5*/		BULLET_HARI32_00_AOI,
/**/		BULLET_HARI32_00_AOI,
/**/		BULLET_HARI32_00_AOI,
		};
	//	hari_no_iro = iro_tbl[(((data->boss_base.boss_health)&(0xfc00) )>>10)];
		hari_no_iro = iro_tbl[(data->boss_base.boss_life)];
	}
//
	int angle512;
//	for (angle512=deg_360_to_512(0); angle512</*=*/deg_360_to_512(360/*180*/);
	for (angle512=deg_360_to_512CCW(0); angle512</*=*/deg_360_to_512CCW(360/*180*/);
	//	angle512+=/*22*/21+hari-difficulty/*deg_360_to_512(20)*/		/* 23�{(360) �� 8�{(180) */
		angle512+=(/*22*/18+hari-difficulty-difficulty/*deg_360_to_512(20)*/)		/* �Œ� == 10 == 48�{(360)	*/
		)
	{
		bullet_create_offset_dan_type(sb03[4],
			(int)(/*6*/ (0x100)+(ra_nd()&0x2ff)/*t256(2.5)*/),
			angle512,
			/*offsx*/t256(0)/*(src->w/2)*/,
			/*offsy*/t256(0)/*(src->h/2)*/,
			hari_no_iro
		);
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void enemy_boss03_LR_dole_fire(void)
{
//	case 0:/*LEFT*/
	if (0==(b01flags&FLG_DOLL1)/*sb03[3]->flags&SP_FLAG_VISIBLE*/)
	{	bullet_create_aka_maru_jikinerai(sb03[1], t256(difficulty+1)/*5*/);}
//	case 1:/*RIGHT*/
	if (0==(b01flags&FLG_DOLL2)/*sb03[5]->flags&SP_FLAG_VISIBLE*/)
	{	bullet_create_aka_maru_jikinerai(sb03[2], t256(difficulty+1)/*5*/);}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void center_shot(BOSS03_DATA *data)
{
	/* 4�ʂ̏ꍇ---- */
	if (0==rd4_zero/*0==data->level*/)
	{
		int kakudo;
		kakudo = (127-16);
		kakudo += ((ra_nd())&(32-1));
	//	switch ((data->boss_base.boss_health)&(0xfc00) )
		switch (data->boss_base.boss_life)
		{
		case (3/*<<10*/):/*not_break;*/
			bullet_create_n_way_dan_sa_type(sb03[0], t256(0.6), kakudo, (int)(512/24), BULLET_UROKO14_03_MIDORI,	24);
			bullet_create_n_way_dan_sa_type(sb03[0], t256(1.4), kakudo, (int)(512/24), BULLET_UROKO14_05_KIIRO, 	24);
		case (2/*<<10*/):/*not_break;*/
		case (4/*<<10*/):/*not_break;*/
			bullet_create_n_way_dan_sa_type(sb03[0], t256(1.0), kakudo, (int)(512/24), BULLET_UROKO14_00_AOI,		24);
			bullet_create_n_way_dan_sa_type(sb03[0], t256(1.8), kakudo, (int)(512/24), BULLET_UROKO14_04_MIZUIRO,	24);
		case (5/*<<10*/):/*not_break;*/
		case (1/*<<10*/):/*not_break;*/
			bullet_create_n_way_dan_sa_type(sb03[0], t256(1.2), kakudo, (int)(512/24), BULLET_UROKO14_02_YUKARI,	24);
			bullet_create_n_way_dan_sa_type(sb03[0], t256(0.8), kakudo, (int)(512/24), BULLET_UROKO14_01_AKA,		24);
			bullet_create_n_way_dan_sa_type(sb03[0], t256(2.0), kakudo, (int)(512/24), BULLET_UROKO14_01_AKA,		24);
			break;
		}
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

// [***090114		���낢��ƒǉ����܂��B
//3�ʂ�4�ʂ̍��ʉ��B
//4�ʂɂ͐V�����ǉ������e�O����g�ݍ��ށB
static void enemy_boss03_move(SPRITE *s)
{
	BOSS03_DATA *data = (BOSS03_DATA *)s->data;
	static int xxx;
	static int yyy;
	static int www;
	static int firewait3;
	static int bomb_n;
	int i;
	switch (/*data->state*/b01state)
	{
	case 0:
	//	firewait1	=45;
	//	firewait2	=4;
		firewait3	=0;
		bomb_n		=0;
			//	xxx 	= GAME_WIDTH/2-((sb03[1]->w128+sb03[0]->w128+sb03[2]->w128)>>8);	//�E�B���h�E���̕ύX
				xxx 	= GAME_WIDTH/2-((34+54+34)/2);	//�E�B���h�E���̕ύX
				yyy 	= -100; 			enemy_boss03_setpos(xxx,yyy);					{	/*data->state*/b01state=1;						}	break;
	case 1: 	yyy+=1; 					enemy_boss03_setpos(xxx,yyy);	if (yyy>= 0)	{	/*data->state*/b01state=2; www=0;
			//	((PLAYER_DATA *)player->data)->bo ssmode=B05_BE FORE_LOAD;
				((PLAYER_DATA *)player->data)->state_flag |= STATE_FLAG_10_IS_LOAD_SCRIPT;	}	break;/*��*/
	case 2:
			//	if (((PLAYER_DATA *)player->data)->bo ssmode==B00_NONE/*B01_BA TTLE*/)
			if ( ((((PLAYER_DATA *)player->data)->state_flag) & STATE_FLAG_05_IS_BOSS) )	{
								www+=1; 									if (www>=40)	{	/*data->state*/b01state=3;					}	}	break;
	case 3: 	yyy+=1; 					enemy_boss03_setpos(xxx,yyy);	if (yyy>=30)	{	/*data->state*/b01state=4; www=0;
			/* �v���C���[�e�󂯕t���A�R�[���o�b�N��o�^ */
			((PLAYER_DATA *)player->data)->callback_boss_hitbyweapon=callback_enemy_boss03_hitbyweapon; 										}	break;/*��*/
/*���S�Ɏp������*/
	case 4: 					www+=1; 									if (www>=30)	{	/*data->state*/b01state=5; www=0;				}	break;
	case 5: 	yyy+=1; xxx-=2; www+=1; 	enemy_boss03_setpos(xxx,yyy);	if (www>=60)	{	/*data->state*/b01state=6;						}	break;/*����*/
	case 6: 	yyy-=1; xxx+=2; www-=1; 	enemy_boss03_setpos(xxx,yyy);	if (www<= 0)	{	/*data->state*/b01state=7; center_shot(data);	}	break;/*�E��*/
	case 7: 	yyy+=1; xxx+=2; www+=1; 	enemy_boss03_setpos(xxx,yyy);	if (www>=60)	{	/*data->state*/b01state=8;						}	break;/*�E��*/
	case 8: 	yyy-=1; xxx-=2; www-=1; 	enemy_boss03_setpos(xxx,yyy);	if (www<= 0)	{	/*data->state*/b01state=4; center_shot(data);	}	break;/*����*/
	}
	if (/*((BOSS03_DATA *)sb03[0]->data)*/ /*data->state*/b01state > 2)
	{
	static int firewait1=45;
		firewait1-=1;
		if (firewait1<=0)
		{
			firewait1=45;
			static unsigned int firewait2=0;
			firewait2--;
			firewait2 &= (4-1);
			if (0==firewait2)
			{
				enemy_boss01_nway_fire(data);/*enemy_boss03_fire(2);*/
			}
			else
			{
				/* 3�ʂ̏ꍇ���E������ */
				if (0!=rd4_zero/*0!=data->level*/)
				{
					enemy_boss03_LR_dole_fire();/*enemy_boss03_fire(0);enemy_boss03_fire(1);*/
				}
			}
		}
	}
	/* 4�ʂ̏ꍇ���˂�� */
	if (0==rd4_zero/*0==data->level*/)
	{
		//switch ( (data->boss_base.boss_health)&0xfc00 )
		//if (data->boss_base.boss_health <= 1000+1024)
		if (data->boss_base.boss_life < 2)
		{
		//case (0<<10):
			{	firewait3--;}
			if (firewait3 < 0)
			{
			//	if (data->boss_base.boss_health > 200+1024)
				if (0 < data->boss_base.boss_life)
				{	firewait3 = ((data->boss_base.boss_health)>>2);}
				else
				{	firewait3 = 50;}
			//
				if (bomb_n < 32)
				{	bomb_n++;}
				for (i=0; i<=(int)(bomb_n/3); i++)
				{
					bullet_create_hazumi_dan(s,
						t256(0.05)+(difficulty<<6), 		/* speed256 t256(5.0) */
						(/*(M_PI*2)*/512*3*i/(bomb_n+1))	/* angle512 */
															/*+ra d2deg512(1)*/, /*���������ʏ����Ȃ������~�X*/
						(t256(0.07)),						/* delta256 */	/*17.92==t256(0.07)*/
						difficulty/*2*/ 					/* bound_counts */
					);		/* [***090116		������ */
				}
			}
		//	else
		//	{	firewait3--;}
		//	break;
		}
	}
//
	/*---------------------------------------------------------
		�_���[�W�`�F�b�N
	---------------------------------------------------------*/
//


/* ----[�P���p����]---- */
	{
	//	if (b_health_alter_low1024 < (data->boss_base.boss_health & (1024-1)))/* �`�ԕύX������ */
		if (0==data->boss_base.boss_health)
		{
			if (0 != (data->boss_base.boss_life))/* �`�ԕύX������ */
			{
				data->boss_base.boss_life--;
				kaguya_put_items(s/*,t*/);
			}
		}
	}
//
	{
		if (0 == data->boss_base.boss_life)
		{
		//	if (FLG_ALL_CAST==(b01flags&FLG_ALL_CAST))/*�S���|�����H*/
			/* �P���|���ƊF�j�󂳂��B */
			if (data->boss_base.boss_health < 1)/*�S���|�����H*/
			{
				item_create_for_boss(s, ITEM_CREATE_MODE_01);
				player_add_score(data->boss_base.score);/*�P��̃X�R�A*/
			//
				player_set_destoroy_boss();
				int i;
				for (i=0; i<6; i++)
				{
				//	explosion_add_type(sb03[i]->x256+((sb03[i]->w)<<7),sb03[i]->y256+((sb03[i]->h)<<7),/*(ra_nd()&(16-1)/*%20*/),*/EXPLOSION_FIRE08);
					explosion_add_circle(sb03[i], 1);	/* player_set_destoroy_boss(); B02_BOSS_DESTROY ���K�v */
					sb03[i]->type = SP_DELETE;
				}
			}
		}
	}
}
	/* �R�[���o�b�N�o�^ */
//	((PLAYER_DATA *)player->data)->callback_boss_hitbyweapon = callback_enemy_boss03_hitbyweapon_dummy; /* �_�~�[�R�[���o�b�N�o�^ */

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

void add_boss_kaguya(STAGE_DATA *l)/*int lv*/
{
	int lv;
	lv	= l->user_y;
//
	/*
	4�ʂ̏ꍇ0
	3��:	rd4_zero	= 0�ȊO�̐���(����1);
	4��:	rd4_zero	= 0;
	*/
	rd4_zero	= lv/*+1*/;
	b01flags	= 0;
	bb_angle512 = 0;
//
	/*data->state*/b01state=0;
	int i;
	for (i=0; i<6; i++)
	{
		sb03[i] 				= sprite_add_res(BASE_BOSS_KAGUYA_PNG);
		sb03[i]->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
		sb03[i]->anim_speed 	= 0;
		sb03[i]->yx_anim_frame	= i/*0*/+(rd4_zero<<4);
		sb03[i]->type			= SP_BOSS/*SP_BOSS03*/;
		BOSS03_DATA *data;
		data					= mmalloc(sizeof(BOSS03_DATA));
		sb03[i]->data			= data;
		data->identity			= i;		// �ŗL�ԍ�
		data->health_flag		= 0;
//
		/* 4�ʂ̏ꍇ---- */
		if (0==rd4_zero)		// [***090114		�ǉ�
		{
			if (0==i)/* �P��{�l */
			{
			//	data->boss_base.boss_health 	= (difficulty<<10/*[x1024]*/)+2047+1024;
				data->boss_base.boss_health 	= 1024-1;
				data->boss_base.boss_life		= (difficulty)+2;

			//	data->boss_base.score			= score(3000)+score(2000)*difficulty;
				data->boss_base.score			= adjust_score_by_difficulty(score(2000000));	/* 200�� (�v400��==(200��)+(5x40��)) */
			}
			else/* ������ */
			{
			//	data->boss_base.boss_health 	= (difficulty<<8/*[x256]*/)+ 200+1024;
				data->boss_base.boss_health 	= 1024-1;
				data->boss_base.boss_life		= (((difficulty<<8/*[x256]*/)+ 200+1024)>>10);
			//	data->boss_base.score			= score(600)*(difficulty+1);
				data->boss_base.score			= adjust_score_by_difficulty(score( 400000));	/* 40�� */
			}
		}
		/* 3�ʂ̏ꍇ---- */
		else/* �S���ꏏ */
		{
		//	data->boss_base.boss_health 	= (difficulty<<6/*[x64]*/)+ 200+1024;
			data->boss_base.boss_health 	= 1024-1;
			data->boss_base.boss_life		= (((difficulty<<6/*[x64]*/)+ 200+1024)>>10);
		//	data->boss_base.score			= score(500)*(difficulty+1);
			data->boss_base.score			= adjust_score_by_difficulty(score( 500000));	/* 50�� (�v300��==6x50��) */
		}
		if (0==i)/* �P��{�l */
		{
			sb03[i]->callback_mover 	= enemy_boss03_move;
		}
		data->boss_base.boss_timer		= (40*64);	/* 40*64==40[count] 	��40[�b(64/60)](�P�ʂ͕b�ł͂Ȃ�) */
	}
	//
	((PLAYER_DATA *)player->data)->boss=sb03[0];/*�P��{�l*/
	/* �R�[���o�b�N�o�^ */
	((PLAYER_DATA *)player->data)->callback_boss_hitbyweapon=callback_enemy_boss03_hitbyweapon_dummy;	/* �_�~�[�R�[���o�b�N�o�^ */
}
//	((PLAYER_DATA *)player->data)->bo ssmode=B00_NONE/*B01_BA TTLE*/;
