
#include "bullet_object.h"

/*---------------------------------------------------------
	�˖��� ��
	�V��
	-------------------------------------------------------
	ToDo:
	�{�X�^�C�}�[�Ή���
	-------------------------------------------------------
	�����Ŏv����������2�`�Ԃ̍U���p�^�[�����J�I�X�ɂȂ��Ă��܂����B
	�����ƒ��J�ɍ�蒼�����������������B
	[*** 090114 	�V��̑��x���Փx���Ƃɕς��Ă݂��B
---------------------------------------------------------*/

typedef struct /*_boss02_data*/
{
	BOSS_BASE boss_base;
	/*static*/ int angle512/*angle_radian*/;/*radian�Ŏ����int�Ŏ����Ďg���x�ɕϊ�������������*/
	int state1;
	int wait1;			/* �ړ��҂� */
	int wait2;			/* ���[�V�����ύX�p */
	int xp256;
	int n_wait3;
//	int level;
	int aya_speed;		/*	��x�ʑ��x */
	int danmaku_type;	/*	�e���^�C�v */
	int move_type;		/*	�ړ��^�C�v */
	int ice_dan;		/*	ice_dan�ύX�p */
} BOSS02_DATA;

/* �e���^�C�v */
enum
{
	DT00 = 0,
	DT01,
	DT02,
	DT03,
	DT04,
	DT05,
	DT06,
	DT07,
};


/*---------------------------------------------------------

	-------------------------------------------------------
	s = �{�X�{�� boss sprite
	t = �v���C���[�̒e player's weapon
---------------------------------------------------------*/
		#if 0
static void callback_enemy_boss02_hitbyweapon_dummy(SPRITE *s, SPRITE *t/*, int angle*/)
{	/* ���S�Ɏp�������܂ōU���́A���ׂĖ����Ƃ���B */
}
		#endif

static void callback_enemy_boss02_hitbyweapon(SPRITE *s, SPRITE *t/*, int angle*/)
{
	explosion_add_type(t->x256,t->y256,/*0,*/EXPLOSION_MINI00);/*��Ɏ��s������������*/
//
	BOSS02_DATA *data	= (BOSS02_DATA *)s->data;
	WEAPON_BASE *w		= (WEAPON_BASE *)t->data;
//
	{
		data->boss_base.boss_health -= w->strength;/* �{�X�U�����đ̗͌��炷 */
		if (0 > data->boss_base.boss_health)
		{
			data->boss_base.boss_health = 0;
		}
	}
}
//	parsys_add(NULL,100,0,t->x,t->y,30,0,0,50,PIXELATE,NULL);

/*---------------------------------------------------------

---------------------------------------------------------*/

static void aya_put_items(SPRITE *s/*, SPRITE *t*/)
{
	play_voice_auto_track(VOICE07_BOMB);
	item_from_bullets(SP_ITEM_05_HOSI);
	item_create_for_boss(s, ITEM_CREATE_MODE_02);
	explosion_add_circle(/*d*/s, 1);
//
	BOSS02_DATA *data=(BOSS02_DATA *)s->data;
	data->boss_base.boss_health 	= (1024-1); 	/* life_point����Q�[�W��[ */
	data->boss_base.boss_timer		= (40*64);		/* 40*64==40[count] 	��40[�b(64/60)](�P�ʂ͕b�ł͂Ȃ�) */		/* �{�X�^�C�}�[������ */
	data->danmaku_type++;
//	if (DT02 < data->danmaku_type)	{	data->danmaku_type = DT02;	}
	//
	{
		const unsigned char aaa_bbb[16] =
		{
			0,0,1,0, 1,0,1,1,
			1,0,0,1, 0,1,0,0,
		};
		data->move_type 	= aaa_bbb[(data->danmaku_type&7)];
		data->ice_dan		= aaa_bbb[(data->danmaku_type&7)+8];
	}
	#if 0/*�΍�ς�*/
	{
		/* ���ɂ��������Ⴄ�ꍇ������̂ŁA�Ƃ肠�������΍� */
		#define MAX_AYA_LOW_Y256 (t256(200))
		if (MAX_AYA_LOW_Y256 < s->y256)
		{
			s->y256 = MAX_AYA_LOW_Y256;/* ���[�v */
		}
		#undef MAX_AYA_LOW_Y256
	}
	#endif
	//
	//if (0 != data->move_type)
	{
	//	data->danmaku_type	= DT02;
		data->state1	= 0;
		data->wait1 	= 40;
	//	player_add_score(data->boss_base.score);
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void sakuya_wait_state(SPRITE *s/*, int next_set_state*/)
{
	BOSS02_DATA *data = (BOSS02_DATA *)s->data;
	if (data->wait1 > 0)
	{
		data->wait1 -= 1/*fps_fa ctor*/;
		data->wait2 += 1/*fps_fa ctor*/;
		if (data->wait2 >= 3)
		{
			data->wait2 = 0;
			if (s->anim_frame<4)
			{
				s->anim_frame++;
			}
			else if (s->anim_frame>4)
			{
				s->anim_frame--;
			}
		}
	}
	else
	{
		data->state1++/* = next_set_state*/;
		bullet_create_oodama0(s, t256(4)+(difficulty<<7), ANGLE_JIKI_NERAI_DAN, 10, 0, ((s->w128)), ((s->h128)));
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/
enum
{
	ST_02 = 0,
	ST_03,
	ST_04,
	ST_04bbb,
	ST_05,
	ST_06,
	ST_07,
	ST_08,
	ST_09,
	ST_10,
	ST_11,
	ST_12,
	ST_12bbb,
	ST_00,
	ST_01,

};
enum
{
	SS00 = 0,
	SS01,
	SS02,
	SS03,
	SS04,
	SS05,
	SS06,
	SS07,
	SS08,
	SS09,
	SS10,
	SS11,
	SS12,
};

/*---------------------------------------------------------

---------------------------------------------------------*/

static void bullet_create_aya_momiji(SPRITE *s)
{
#if 0
/* CW�̏ꍇ */
	bullet_create_momiji_dan(s, t256(3.5), deg_360_to_512((30*(3-2))) );
	bullet_create_momiji_dan(s, t256(4.0), deg_360_to_512((30*(3+0))) );
	bullet_create_momiji_dan(s, t256(3.5), deg_360_to_512((30*(3+2))) );
#else
/* CCW�̏ꍇ */
//	bullet_create_momiji_dan(s, t256(3.5), -deg_360_to_512CCW((30*(3-2))) );
//	bullet_create_momiji_dan(s, t256(4.0), -deg_360_to_512CCW((30*(3+0))) );
//	bullet_create_momiji_dan(s, t256(3.5), -deg_360_to_512CCW((30*(3+2))) );
//2016201
	bullet_create_momiji_dan(s, t256(3.5), deg_360_to_512CCW(360+90-(30*(3-2))) );
	bullet_create_momiji_dan(s, t256(4.0), deg_360_to_512CCW(360+90-(30*(3+0))) );
	bullet_create_momiji_dan(s, t256(3.5), deg_360_to_512CCW(360+90-(30*(3+2))) );
#endif
}

/*---------------------------------------------------------

---------------------------------------------------------*/

extern void bullet_create_aya_ice(SPRITE *s);

static int bullet_create_oodama3(SPRITE *s, int speed256, int ransu512)
{	int angle_jikinerai_512;
	angle_jikinerai_512=atan_512(player->y256-s->y256,player->x256-s->x256);
#if 0
/* CW�̏ꍇ */
	bullet_create_oodama0(s, speed256, angle_jikinerai_512-deg_360_to_512((30)), ransu512, 0, ((s->w128*86*2)>>8), (s->h128)); /*t256(s->w/3), t256(s->h/2)*/
	bullet_create_oodama0(s, speed256, angle_jikinerai_512					   , ransu512, 0, ((s->w128*86*2)>>8), (s->h128)); /*t256(s->w/3), t256(s->h/2)*/
	bullet_create_oodama0(s, speed256, angle_jikinerai_512+deg_360_to_512((30)), ransu512, 0, ((s->w128*86*2)>>8), (s->h128)); /*t256(s->w/3), t256(s->h/2)*/
#else
/* CCW�̏ꍇ */
//	bullet_create_oodama0(s, speed256, angle_jikinerai_512+deg_360_to_512CCW((30)), ransu512, 0, ((s->w128*86*2)>>8), (s->h128)); /*t256(s->w/3), t256(s->h/2)*/
//	bullet_create_oodama0(s, speed256, angle_jikinerai_512						  , ransu512, 0, ((s->w128*86*2)>>8), (s->h128)); /*t256(s->w/3), t256(s->h/2)*/
//	bullet_create_oodama0(s, speed256, angle_jikinerai_512-deg_360_to_512CCW((30)), ransu512, 0, ((s->w128*86*2)>>8), (s->h128)); /*t256(s->w/3), t256(s->h/2)*/
	bullet_create_oodama0(s, speed256, angle_jikinerai_512-deg_360_to_512CCW(360-(30)), ransu512, 0, ((s->w128*86*2)>>8), (s->h128)); /*t256(s->w/3), t256(s->h/2)*/
	bullet_create_oodama0(s, speed256, angle_jikinerai_512							  , ransu512, 0, ((s->w128*86*2)>>8), (s->h128)); /*t256(s->w/3), t256(s->h/2)*/
	bullet_create_oodama0(s, speed256, angle_jikinerai_512+deg_360_to_512CCW(360-(30)), ransu512, 0, ((s->w128*86*2)>>8), (s->h128)); /*t256(s->w/3), t256(s->h/2)*/
#endif
	return (angle_jikinerai_512);
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void bullet_create_kougeki_01(SPRITE *s)
{	BOSS02_DATA *data=(BOSS02_DATA *)s->data;
//	if (DT00==data->danmaku_type)
//	if (2 < ((data->danmaku_type+1) & 0x03/*DT00*/))
	if (0!=data->ice_dan)
	{	bullet_create_aya_ice(s);}
};
static void bullet_create_kougeki_02(SPRITE *s)
{	BOSS02_DATA *data=(BOSS02_DATA *)s->data;
//	if (DT01==data->danmaku_type)
	if (0!=(data->danmaku_type & 1/*DT01*/))
	{	bullet_create_aya_momiji(s);}
};
/*
000 0 --
001 1
010 2
011 3 --
100 4

*/

/*---------------------------------------------------------

---------------------------------------------------------*/

static void move_aya(SPRITE *s)
{
	BOSS02_DATA *data=(BOSS02_DATA *)s->data;

	if (0==data->move_type)
	{		/* �ړ��p�^�[��1(�ᑬ�ړ�) */
		switch (data->state1)
		{
		case ST_02: /* y-pos erreicht: rundumschuss */
			#if (1==USE_CONTROLLER)
			enemy_boss02_sr_add1(s);
			#endif //(1==USE_CONTROLLER)
			bullet_create_kougeki_01(s);
			bullet_create_kougeki_02(s);
			data->wait1 = 20;
			data->state1++/* = ST_03*/;
			break;

		case ST_03: /* waiting */
			sakuya_wait_state(s/*,ST_04*/);
			break;

		case ST_04: /* nach links bis zum rand */
			if (s->x256 >= t256(0))
			{
				s->x256 -= t256(2)/**fps_fa ctor*/;
				if (s->anim_frame>0)
				{
					data->wait2 += 1/*fps_fa ctor*/;
					if (data->wait2>=3)
					{
						s->anim_frame--;
						data->wait2=0;
					}
				}
			}
			else
			{
				data->state1++/* = ST_05*/;
			}
			break;
		case ST_04bbb: /*  */
			{
				data->state1++/* = ST_05*/;
				data->wait1 = 20;
				bullet_create_kougeki_02(s);
			}
			break;


		case ST_05: /* waiting */
			sakuya_wait_state(s/*,ST_06*/);
			break;

		case ST_06: /* nach rechts bis zur mitte */
			if (s->x256 < data->xp256)
			{
				s->x256 += t256(2)/**fps_fa ctor*/;
				if (s->anim_frame<8)
				{
					data->wait2 += 1/*fps_fa ctor*/;
					if (data->wait2>=3)
					{
						s->anim_frame++;
						data->wait2=0;
					}
				}
			}
			else
			{
				#if (1==USE_CONTROLLER)
				enemy_boss02_sr_add1(s);
				#endif //(1==USE_CONTROLLER)
				data->wait1 = 100;
				data->state1++/* = ST_07*/;
				bullet_create_kougeki_02(s);
			}
			break;

		case ST_07: /* waiting */
			sakuya_wait_state(s/*,ST_08*/);
			break;

		case ST_08:
			bullet_create_kougeki_01(s);
			data->wait1 = 100;
			data->state1++/* = ST_09*/;
			break;

		case ST_09: /* waiting */
			sakuya_wait_state(s/*,ST_10*/);
			break;

		case ST_10: /* nach rechts bis zum rand */
			if (s->x256 < (t256(GAME_WIDTH)-((s->w128+s->w128))))
			{
				s->x256 += t256(2)/**fps_fa ctor*/;
				if (s->anim_frame<8)
				{
					data->wait2 += 1/*fps_fa ctor*/;
					if (data->wait2>=3)
					{
						s->anim_frame++;
						data->wait2=0;
					}
				}
			}
			else
			{
				data->wait1 = 20;
				data->state1++/* = ST_11*/;
				bullet_create_kougeki_02(s);
			}
			break;

		case ST_11: /* waiting */
			sakuya_wait_state(s/*,ST_12*/);
			break;

		case ST_12: /* nach links bis zur mitte */
			if (s->x256 > data->xp256)
			{
				s->x256 -= t256(2)/**fps_fa ctor*/;
				if (s->anim_frame>0)
				{
					data->wait2 += 1/*fps_fa ctor*/;
					if (data->wait2>=3)
					{
						s->anim_frame--;
						data->wait2=0;
					}
				}
			}
			else
			{	data->state1++/* = ST_02*/; }
			break;
		case ST_12bbb: /*  */
			{
				data->state1 = ST_02;
			}
			break;

////////////////////////
		case ST_00: 	/* ������ */
			s->y256 += t256(2) /**fps_fa ctor*/;
			if (s->y256 >= t256(30))
			{
				data->state1++/* = ST_01*/;
				data->wait1 = 0;
			//	((PLAYER_DATA *)player->data)->bo ssmode=B05_BE FORE_LOAD;
				((PLAYER_DATA *)player->data)->state_flag |= STATE_FLAG_10_IS_LOAD_SCRIPT;
			}
			break;
		/*�p������*/
		case ST_01:
		//	if (((PLAYER_DATA *)player->data)->bo ssmode==B01_BA TTLE)
		//	if ((STATE_FLAG_05_IS_BOSS|0) == (((PLAYER_DATA *)player->data)->state_flag&(STATE_FLAG_05_IS_BOSS|STATE_FLAG_06_IS_SCRIPT)))
			if ( ((((PLAYER_DATA *)player->data)->state_flag) & STATE_FLAG_05_IS_BOSS) )
			{
				/* �v���C���[�e�󂯕t���A�R�[���o�b�N��o�^ */
			//	data->wait1 += (1)/*fps_fa ctor*/;
			//	if (data->wait1 >= 40)
			//	{
					data->state1 = ST_02;
			//	}
			}
			break;
		}
	}
	else
	{	/* �ړ��U���p�^�[��2(�����ړ�) */
		switch (data->state1)
		{
		case SS00:	/* �s��:�����ʒu���̎擾->SS01�� */
			data->angle512/*_radian*/=atan_512(t256(20)-s->y256,t256(GAME_WIDTH/2)-s->x256-((s->w128+s->w128)));
			data->state1++/* = SS01*/;
			break;

		case SS01:	/* �s��:�����ʒu�֖߂�->SS02�� */
			if (s->y256 < t256(30))
			{
				data->state1++/* = SS02*/;
				data->wait1 += 10+(3-difficulty)*10;
/*
[0] 1  2 [0] 1	2 [0] 1  2 [0] 1  2 				/ 3��Ɉ��
<0> 1  2  3 [0] 1  2  3 [0] 1  2  3 				/ 4��Ɉ��
<0> 1  2  3  4	5  6  7  8	9 10 11 				/ 12��Ɉ��ȊO
[-][-][-][-][+][+][+][+][-][-][-][-][+][+][+][+]
[0] 1  2 [0] 1	2 [0] 1 [0] 1  2 [0] 1	2 [0] 1 	/ 3��Ɉ��
 0 [1] 2  3  0 [1] 2  3  0 [1] 2  3  0 [1] 2  3 	/ 4��Ɉ��
 0	1  2  3  4	5  6  7  8	9 10 11 12 13 14 15 	/ 16��
*/
				{	static const Uint8 n_wait3_tbl[8/*16*/] =
					{ 1,2,0,1, 0,2,1,0, /*1,2,0,1, 0,2,1,0,*/ };
					/* 3��Ɉ��   (0 == ((data->n_wait3)%3)) */
					data->n_wait3++;
					data->n_wait3 &= 0x07;
					if (n_wait3_tbl[data->n_wait3]&0x01)
					{
						/* �e�ɓœh���Ă���ݒ� */
						bullet_create_n_way_dan_sa_type(s, t256(5), ANGLE_JIKI_NERAI_DAN, (int)(512/24), BULLET_KOME_01_AOI+(ra_nd()&0x0f),	 8);	/*�Ȃ�ׂ����ʉ�*/
					}
					/* 4��Ɉ��   (0 == ((data->n_wait3)%4)) && */
					/* �A��12��Ɉ��ȊO  (0 != ((data->n_wait3)%12)) */
					if (n_wait3_tbl[data->n_wait3]&0x02)
					{
						/* �e�ɓœh���Ă���ݒ� */
						bullet_create_n_way_dan_sa_type(s, t256(4), ANGLE_JIKI_NERAI_DAN, (int)(512/24), BULLET_KOME_01_AOI+(ra_nd()&0x0f)/*BULLET_KUNAI12_06_AOI*/, 11);	/*�Ȃ�ׂ����ʉ�*/
						bullet_create_n_way_dan_sa_type(s, t256(3), ANGLE_JIKI_NERAI_DAN, (int)(512/24), BULLET_KOME_01_AOI+(ra_nd()&0x0f)/*BULLET_KUNAI12_07_MIDORI*/, 11); /*�Ȃ�ׂ����ʉ�*/
					}
				}
			}
			else
			{
#if 0
/* CW�̏ꍇ */
				s->x256 += ((cos512((data->angle512/*_radian*/))*t256(6))>>8)/**fps_fa ctor*/;
				s->y256 += ((sin512((data->angle512/*_radian*/))*t256(6))>>8)/**fps_fa ctor*/;
#else
/* CCW�̏ꍇ */
				s->x256 += ((sin512((data->angle512/*_radian*/))*t256(6))>>8)/**fps_fa ctor*/;
				s->y256 += ((cos512((data->angle512/*_radian*/))*t256(6))>>8)/**fps_fa ctor*/;
#endif
				data->state1--/* = SS00*/;
				#if 1/*�������݊�*/
				data->n_wait3--;
				#endif
			}
			break;

		case SS02:	/* �����ʒu:��e3��->SS03, SS04, SS05 */
			if (data->wait1<0)
			{	int angle_jikinerai_512;
				angle_jikinerai_512=bullet_create_oodama3(s, t256(5.0), 1/*ransu512*/);
			//	data->state1 = (ra_nd()%3)+SS03;
				data->state1 = (((ra_nd()&(256-1))*3)>>8)+SS03;
				if (data->state1 == SS05)
				{
					data->angle512/*_radian*/=angle_jikinerai_512;
				}
				else
				{
					s->y256 += t256(6)/**fps_fa ctor*/;
				}
			}
			else
			{
				data->wait1 -= 1/*fps_fa ctor*/;
			}
			break;

		case SS03:	/* ���ړ���->SS06�� */
			if (s->x256 >= 0)
			{
				s->x256 -= /*(t256(4)+(difficulty<<8))*/(data->aya_speed)/**fps_fa ctor*/;
				if (s->anim_frame>0)
				{
					data->wait2 += 1/*fps_fa ctor*/;
					if (data->wait2>=3)
					{
						s->anim_frame--;
						data->wait2=0;
					}
				}
			}
			else
			{
				s->x256 += t256(1); 		// [***090114		�ǉ�
				data->wait1 = 10;
				data->state1 = SS06;
			}
			break;

		case SS04:	/* �E�ړ���->SS08�� */
			if (s->x256 < (t256(GAME_WIDTH)-((s->w128+s->w128))))
			{
				s->x256 += /*(t256(4)+(difficulty<<8))*/(data->aya_speed)/**fps_fa ctor*/;
				if (s->anim_frame<8)
				{
					data->wait2 += 1/*fps_fa ctor*/;
					if (data->wait2>=3)
					{
						s->anim_frame++;
						data->wait2=0;
					}
				}
			}
			else
			{
				s->x256 -= t256(1); 		// [***090114		�ǉ�
				data->wait1 = 10;
				data->state1 = SS08;
			}
			break;

		case SS05:	/* �v���C���[�ʒu�t�߈ړ���->SS10�� */
			/* aya�̉ғ��͈� */
			#if 0
			/* ��ʂ���ݏo���ꍇ�ɖ߂�(�{�X�����肠��ɂ����̂ŕ������ɓ��܂�č���) */
			if (((s->x256 < t256(0))||(s->y256 < t256(0)))||
				((s->x256+((s->w128+s->w128)) > t256(GAME_WIDTH))||
			//	 (s->y256+((s->h128+s->h128)) > t256(GAME_HEIGHT))))
				 (s->y256+((s->h128+s->h128)) > t256(220))))/*(���܂�Ȃ�)*/
			#else
			/* �ғ��͈͂���ݏo���ꍇ�ɖ߂�(�{�X�����肠��ɂ����̂ŕ������ɓ��܂�č���) */
			if (((s->x256 < t256(0))||(s->y256 < t256(0)))||
				((s->x256+((s->w128+s->w128)) > t256(GAME_WIDTH))||
			//	 (s->y256+((s->h128+s->h128)) > t256(GAME_HEIGHT))))
				 (s->y256+((s->h128+s->h128)) > (t256(128)+(difficulty<<(8+5)))  )))	/* difficulty x 32 */
			/*
				easy:	128(���܂�Ȃ�)
				normal: 160(���܂�Ȃ�)
				hard:	192(���܂�Ȃ�)
				luna:	224(���܂��)
			*/
			#endif
			{
				data->state1 = SS10;
			}
			else
			{
#if 0
/* CW�̏ꍇ */
				s->x256 += ((cos512((data->angle512/*_radian*/))*(data->aya_speed)/*(t256(4)+(difficulty<<8))*/)>>8)/**fps_fa ctor*/;
				s->y256 += ((sin512((data->angle512/*_radian*/))*(data->aya_speed)/*(t256(4)+(difficulty<<8))*/)>>8)/**fps_fa ctor*/;
#else
/* CCW�̏ꍇ */
				s->x256 += ((sin512((data->angle512/*_radian*/))*(data->aya_speed)/*(t256(4)+(difficulty<<8))*/)>>8)/**fps_fa ctor*/;
				s->y256 += ((cos512((data->angle512/*_radian*/))*(data->aya_speed)/*(t256(4)+(difficulty<<8))*/)>>8)/**fps_fa ctor*/;
#endif
			}
			break;
//--------
		case SS06:	/* ��:�ҋ@->SS07�� */
			sakuya_wait_state(s/*,SS07*/);
			break;

		case SS07:	/* ��:�E��]�U��->SS00, SS11 */
			#if (1==USE_CONTROLLER)
			enemy_boss02_sr_add1(s);
			#endif //(1==USE_CONTROLLER)
			if (0==(ra_nd()&(2-1)))
			{
				data->state1 = SS00;/*	data->n_wait3++;*/
			}
			else
			{	data->state1 = SS11;}
			break;
//--------

		case SS08:	/* �E:�ҋ@->SS09�� */
			sakuya_wait_state(s/*,SS09*/);
			break;

		case SS09:	/* �E:����]�U��->�ЂƂ܂�SS00�� */
			#if (1==USE_CONTROLLER)
			enemy_boss02_sr_add2(s);
			#endif //(1==USE_CONTROLLER)
			data->state1 = SS00;/*	data->n_wait3++;*/
			break;
//--------

		case SS10:	/* �v���C���[�ʒu�t��:��e3��->�ЂƂ܂�SS00�� */
			bullet_create_oodama3(s, t256(3.0), 20/*ransu512*/);	/* ��e�̒ǉ� */
			data->state1 = SS00;/*	data->n_wait3++;*/
			break;

		case SS11:	/* ��:�O��->SS12 */
			if (s->y256 > t256(GAME_HEIGHT/3) )
			{	data->state1++/* = SS12*/;}
			else
			{	s->y256 += (t256(1)+(data->aya_speed)/*(t256(4)+(difficulty<<8))*/)/**fps_fa ctor*/;}
			break;

		case SS12:
			{
				/* �e�ɓœh���Ă���ݒ� */
#if 0
/* CW�̏ꍇ */
//				bullet_create_n_way_dan_sa_type(s, t256(5), ANGLE_JIKI_NERAI_DAN, deg_360_to_512((30)), BULLET_KOME_01_AOI+(ra_nd()&0x0f),	 5);	/*�Ȃ�ׂ����ʉ�*/
#else
/* CCW�̏ꍇ */
//				bullet_create_n_way_dan_sa_type(s, t256(5), ANGLE_JIKI_NERAI_DAN, -deg_360_to_512CCW((30)), BULLET_KOME_01_AOI+(ra_nd()&0x0f),	 5);	/*�Ȃ�ׂ����ʉ�*/
				bullet_create_n_way_dan_sa_type(s, t256(5), ANGLE_JIKI_NERAI_DAN, deg_360_to_512CCW(360-(30)), BULLET_KOME_01_AOI+(ra_nd()&0x0f),	 5);	/*�Ȃ�ׂ����ʉ�*/
#endif
			//	int angle_jikinerai_512;
			//	angle_jikinerai_512=atan_512(player->y256-s->y256,player->x256-s->x256);
			//	bullet_create_gg_dan(s, t256(5), angle_jikinerai_512+deg_360_to_512(60),	2/*state_hi*/, 0/*state_lo*/);		//����Ȃ��e�B�B������������������
			//	bullet_create_gg_dan(s, t256(5), angle_jikinerai_512+deg_360_to_512(30),	1/*state_hi*/, 0/*state_lo*/);
			//	bullet_create_aka_maru_jikinerai(s, t256(5));
			//	bullet_create_gg_dan(s, t256(5), angle_jikinerai_512-deg_360_to_512(30), -1/*state_hi*/, 0/*state_lo*/);
			//	bullet_create_gg_dan(s, t256(5), angle_jikinerai_512-deg_360_to_512(60), -2/*state_hi*/, 0/*state_lo*/);
			}
			data->state1 = SS00;/*	data->n_wait3++;*/
			break;
		}
	}
	/*---------------------------------------------------------
		�_���[�W�`�F�b�N
	---------------------------------------------------------*/
//
	{
	//	if (b_health_alter_low1024 < (data->boss_base.boss_health & (1024-1)))/* �`�ԕύX������ */
		if (0==data->boss_base.boss_health)
		{
			if (0 != (data->boss_base.boss_life))/* �`�ԕύX������ */
			{
				data->boss_base.boss_life--;
				aya_put_items(s/*,t*/);
			}
		}
	}
//
	{
		if (0 == data->boss_base.boss_life)
		{
			if (data->boss_base.boss_health < 1)/*�S���|�����H*/
			{
				item_create_for_boss(s, ITEM_CREATE_MODE_01);
				player_add_score(data->boss_base.score);
			//
				player_set_destoroy_boss();
				explosion_add_circle(s, 1); 	/* player_set_destoroy_boss();B02_BOSS_DESTROY ���K�v */
				s->type 	= SP_DELETE;
			}
		}
	}
}
				/* �R�[���o�b�N�o�^ */
			//	((PLAYER_DATA *)player->data)->callback_boss_hitbyweapon = callback_enemy_boss02_hitbyweapon_dummy; /* �_�~�[�R�[���o�b�N�o�^ */

/*d*/
/*d*/
	//	HP���c��1024�ɂȂ������B�U���p�^�[���ύX
	//	else if ((data->boss_base.boss_health<=(1024*2/*1024*1*/)) ) {	}
	//	if (data->boss_base.boss_health <= 0)/*�S���|�����H*/

			#if 0
			data->boss_base.boss_health = 0;/* �Ȃ񂩃o�O��̂Œǉ� */
		/* �Ȃ񂩃o�O��̂Œǉ� */
		//	((PLAYER_DATA *)player->data)->callback_boss_hitbyweapon = callback_enemy_boss00_hitbyweapon_dummy; /* �_�~�[�R�[���o�b�N�o�^ */
			((PLAYER_DATA *)player->data)->callback_boss_hitbyweapon = callback_enemy_boss02_hitbyweapon_dummy; /* �_�~�[�R�[���o�b�N�o�^ */
			((PLAYER_DATA *)player->data)->boss->type = SP_DELETE;
			((PLAYER_DATA *)player->data)->boss->callback_mover = NULL;
			#endif
			#if 0
			/* �R�[���o�b�N�o�^ */
			((PLAYER_DATA *)player->data)->callback_boss_hitbyweapon = callback_enemy_boss02_hitbyweapon_dummy; /* �_�~�[�R�[���o�b�N�o�^ */
			#endif


/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

void add_boss_aya(STAGE_DATA *l)/*int lv*/
{
	SPRITE *s;
//	s								= spr ite_add_file 0("boss/aya.png"/*"boss02_v2.png"*/,9,PRIORITY_03_ENEMY, 0); s->anim_speed=0;
	s								= sprite_add_res(BASE_BOSS_AYA_PNG);
	s->flags						|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
	s->anim_frame					= 4;
	s->type 						= SP_BOSS/*SP_BOSS02*/;
	BOSS02_DATA *data;
	data							= mmalloc(sizeof(BOSS02_DATA));
	s->data 						= data;
//	data->boss_base.boss_health 	= (difficulty<<10/**1024*/)+3071+1024;
	data->boss_base.boss_health 	= 1024-1;
	data->boss_base.boss_life		= (difficulty)+3;
//	data->boss_base.score			= score(2000)+score(1500)*difficulty;
	data->boss_base.score			= adjust_score_by_difficulty(score(2000000));	/* 200�� */
	data->state1					= ST_00;/*0*/
	data->wait1 					= 50;
	data->wait2 					= 0;
//	data->level 					= (l->user_y);
	data->n_wait3					= 0/*1*/;
	data->aya_speed 				= t256(4)+(difficulty<<8);
	data->danmaku_type				= DT00;
	data->move_type 				= 0;
	data->ice_dan					= 1;
	s->callback_mover				= move_aya;
	s->x256 						= t256(GAME_WIDTH/2)-(s->w128);
	data->xp256 					= s->x256;
	s->y256 						= -(s->h128+s->h128);
	//
	data->boss_base.boss_timer		= 40*64;	/* 40*64==40[count] 	��40[�b(64/60)](�P�ʂ͕b�ł͂Ȃ�) */
//	((PLAYER_DATA *)player->data)->bo ssmode=B00_NONE/*B01_BA TTLE*/;
	((PLAYER_DATA *)player->data)->boss=s;
	/* �R�[���o�b�N�o�^ */
	((PLAYER_DATA *)player->data)->callback_boss_hitbyweapon=callback_enemy_boss02_hitbyweapon;
}
