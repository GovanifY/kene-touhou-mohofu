
#include "bullet_object.h"

/*---------------------------------------------------------
	�A���X
	-------------------------------------------------------
	ToDo:
	�{�X�^�C�}�[�Ή���
---------------------------------------------------------*/

typedef struct /*_boss01_data*/
{
	BOSS_BASE boss_base;

//------------ �ړ��֘A
	int state8; 		/*	*/
	int state1; 		/* �`�� */
	int move_type;		/*	�ړ��^�C�v */
	int vx; 		/*	*/
	int vy; 		/*	*/
} BOSS01_DATA;

typedef struct /*_boss01_data*/
{
	ENEMY_BASE base;
//
	SPRITE *sakuya_obj; 		/* �p�� */
	int bwait;			/* �e���˂܂ł̑҂����ԁB */
	int br_angle512;	/* �e�̊p�x */
	int identity_bit;	/* �ŗL�r�b�g�ԍ� */
//
	int fix_angle512;	/* �Ă��ƌŒ�l */
	int state222; 		/* �`�� */
} DOLL_DATA;


static int b01level;
static unsigned int alice_anime_count;

//----[ZAKO]

static unsigned int b01_live_flags;
static int bb_angle512;

static SPRITE *obj_doll[8]; 	/* �l�`�B */

// FLG_FIRE1 �e�t���O�B�g�����̐l�`�����Ă��āA���w��ʒu�ɒ��������Bon�ɂȂ�B
#define FLG_FIRE1 0x01/* �������̐l�`(������)���w��ʒu�ɕt������ */
#define FLG_FIRE2 0x02/* �ɂ̖��̐l�`(���E��)���w��ʒu�ɕt������ */
//
#define FLG_DOLL0 0x01/* �������̐l�`(������) */
#define FLG_DOLL1 0x02/* �ɂ̖��̐l�`(���E��) */
//
#define FLG_DOLL2 0x04/* �g�����̐l�`(�l�`) */	/*key*/
#define FLG_DOLL3 0x08/* �����O�̐l�` */
//
#define FLG_DOLL4 0x10/* ���E�O�̐l�` */
#define FLG_DOLL5 0x20/* ���E�O�̐l�` */
#define FLG_DOLL6 0x40/* ���E�O�̐l�` */
#define FLG_DOLL7 0x80/* ���E�O�̐l�` */

#define FLG_MINI_DOLL	(FLG_DOLL2|FLG_DOLL3|FLG_DOLL4|FLG_DOLL5|FLG_DOLL6|FLG_DOLL7)
#define FLG_FIRE_DOLL	(FLG_DOLL0|FLG_DOLL1)
#define FLG_ALL_CAST	(FLG_FIRE_DOLL|FLG_MINI_DOLL)/*�S��*/



/*---------------------------------------------------------

---------------------------------------------------------*/


enum
{
/*0*/	ALICE_01_KEITAI = 0,	/* ���`��: �A���X�o�� */
/*0*/	ALICE_02_KEITAI,		/* ���`��: �S���ōU�� */
/*1*/	ALICE_03_KEITAI,		/* ��O�`��: �t�H�[���V����2 */
/*2*/	ALICE_04_KEITAI,		/* ��l�`��: �A���X�P�� */
};
enum
{
	ST_00 = 0,
	ST_01,
	ST_02,
	ST_03,
	ST_04,
	ST_05,
	ST_06,
	ST_07,
	ST_08,
	ST_09,
	ST_10,
	ST_11,
	ST_12,
	ST_13,
};

/*---------------------------------------------------------
	�G����
---------------------------------------------------------*/

static void lose_doll(SPRITE *s)
{
	explosion_add_type(s->x256+((s->w128)),s->y256+((s->h128)),/*0,*/EXPLOSION_FIRE08);
//
	item_create_for_boss(s, ITEM_CREATE_MODE_02);
//
	DOLL_DATA *data = (DOLL_DATA *)s->data;
	/* ���i�̏����𑬂�(R4000�n�����ӂ�0��r�𑽗p����)���āA������(�ő��ɗ��Ȃ��̂�)�x�����܂� */
	b01_live_flags &= (~(data->identity_bit));
}
/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/
static int cc512;
static void move_doll01(SPRITE *s)
{
	DOLL_DATA *data = (DOLL_DATA *)s->data;
	#if 1
	{
		bb_angle512++;
	int aa_angle512;
		aa_angle512 = bb_angle512;
	//int jj;
		/* �g�����̐l�`(�l�`) �����O�̐l�` ���E�O�̐l�` */
	//	for (jj=2; jj<8; jj++)/*2 3 4 5 6 7*/
		{
//			aa_angle512 += (85)/*171*/;/* 85.33==512/6 170.66== 512/3 */
			aa_angle512 += data->fix_angle512;
			mask512(aa_angle512);
			int radius;
		//	radius = (90)/*45*/;
			{
				radius = ((sin512(cc512))>>2)+16;	/* 80==16+64 */
			}
		/*CCW*/
			/*obj_doll[jj]*/s->x256 = data->sakuya_obj->x256 + ((sin512((aa_angle512))*radius));
			/*obj_doll[jj]*/s->y256 = data->sakuya_obj->y256 + ((cos512((aa_angle512))*radius));
		}
	}
	if (ST_03< ((BOSS01_DATA *)data->sakuya_obj->data)->state1 )
	{
		if (0==(ra_nd()&(0xff)))
		{
			/* �V���b�g */
		//	b01_fire_flags &= (~(doll_data->identity_bit));/* off */
			bullet_create_n_way_dan_sa_type(s,
				(t256(1.5)+(difficulty<<6)),
				ANGLE_JIKI_NERAI_DAN,
				(int)(512/64),
				BULLET_CAP16_04_SIROI,
				(4+difficulty)/*(4-difficulty)*/);
		}
	}
	#endif
	#if 1
	/* �A���X��|���ƊF�j�󂳂��B */
	if (0==b01_live_flags)
	{
		s->type = SP_DELETE;
	}
	#endif
}
/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_doll02(SPRITE *s)
{
	cc512++;
	DOLL_DATA *data = (DOLL_DATA *)s->data;
	switch (data->state222)
	{
	case 0:
		/*obj_doll[jj]*/s->x256 = data->sakuya_obj->x256 + (((data->fix_angle512))<<7);
		/*obj_doll[jj]*/s->y256 = data->sakuya_obj->y256 - t256(16);
		if (0==(b01_live_flags & FLG_MINI_DOLL))
		{
			data->state222++;
		}
		break;
	case 1:
		/* ���߂�(���߂���Əオ�󂫂����邵�A���ɋ߂�����) */
		if (t256(28)/*offset*/ < s->y256)
		{
			s->y256 -= t256(1.414)/**fps_fa ctor*/;
		}
		/* �K���͈͓�(t256(25)���炢)�A���� */
		if (t256(22)/*offset*/ < s->y256)
		{
			data->state222++;
		}
		/* ��߂�(��߂���ƒe�������Ă��Ȃ�) */
		else
		{
			s->y256 += t256(1.414)/**fps_fa ctor*/;
		}
		break;
	case 2:
		{	data->bwait--;}
		if (data->bwait <= 0)
		{
			data->bwait = 5+(3-difficulty)*5;
//			data->br_angle512 += deg_360_to_512(10);
//			data->br_angle512 -= deg_360_to_512CCW(10);
//			data->br_angle512 += 8;
			data->br_angle512 -= 8;
			bullet_create_jyuryoku_dan(
				s,
				256,//t256(1+b01level)/*(3+b01level)*/ /*(4+doll_data->level)*/,
//				(data->br_angle512&127)+256+64,// /*deg512_2rad*/( (doll_data->br_angle512&255/*%180*/)+deg_360_to_512(90) ),
				(data->br_angle512&127)-256-64,// /*deg512_2rad*/( (doll_data->br_angle512&255/*%180*/)+deg_360_to_512(90) ),
			//	((ra_nd()&0x03)+1)//t256(0.04)/*10*/
				((ra_nd()&0x03)+2)//t256(0.04)
			);
		}
		break;
	}
	#if 1
	/* �A���X��|���ƊF�j�󂳂��B */
	if (0==b01_live_flags)
	{
		s->type = SP_DELETE;
	}
	#endif
}



/*---------------------------------------------------------

---------------------------------------------------------*/

/*---------------------------------------------------------
	���l���̔z�u����
	[1][0][2]
	[4][3][5]
	-------------------------------------------------------
	s = �{�X�{�� boss sprite
	t = �v���C���[�̒e player's weapon
---------------------------------------------------------*/

static void callback_enemy_boss01_hitbyweapon_dummy(SPRITE *s, SPRITE *t/*, int angle*/)
{	/* ���S�Ɏp�������܂ōU���́A���ׂĖ����Ƃ���B */
}

static void callback_enemy_boss01_hitbyweapon(SPRITE *s, SPRITE *t/*, int angle*/)
{
	explosion_add_type(t->x256,t->y256,/*0,*/EXPLOSION_MINI00);/*��Ɏ��s������������*/
//
	BOSS01_DATA *data	= (BOSS01_DATA *)s->data;
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
	//parsys_add(NULL,100,0,s->x,s->y,30,0,0,50,PIXELATE,NULL);


/*---------------------------------------------------------

---------------------------------------------------------*/



/*---------------------------------------------------------

---------------------------------------------------------*/
static int vvv256;
static void alice_animation(SPRITE *s)
{
	vvv256++/*vvv256+=vvv256*/;
	if (vvv256> t256(1) )
	{	vvv256= t256(1);}
//
	if (alice_anime_count)	/* �U���A�j���[�V���� */
	{
		alice_anime_count--;
		if ((32)>alice_anime_count) 	{	s->anim_frame = 0x23;}
		else
		if ((40)>alice_anime_count) 	{	s->anim_frame = 0x22;}
		else
										{	s->anim_frame = 0x21;}
	}
	else	/* �ړ��A�j���[�V���� */
	{
		BOSS01_DATA *data=(BOSS01_DATA *)s->data;
		int aaa;
		aaa = (data->vx>0)?(0x00):(0x10);	/*���E*/
		if ((16)>vvv256)		{	aaa+=2;}	/* 25.6==t256(0.1)*/
		else
		if ((200)>vvv256)		{	aaa+=3;}	/* 76.8==t256(0.3)*/
		else
		if ((224)>vvv256)		{	aaa+=2;}	/*128.0==t256(0.5)*/
		else
		if ((240)>vvv256)		{	aaa+=1;}	/*179.2==t256(0.7)*/
	//	else						{	aaa+=0;}
		s->anim_frame = aaa;
	}
}

/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_alice(SPRITE *s)
{
	static int my_wait;
	static int firewait1;
	static int firewait2;
	static int firewait3;
//
	my_wait -= (1)/*fps_fa ctor*/;
//
	BOSS01_DATA *data=(BOSS01_DATA *)s->data;
	switch (data->state1)
	{
	case ST_00:
	//	x256		= (GAME_WIDTH*256/2); // /*t256*/256*(GAME_WIDTH/2-(/*obj_doll[0]->w*/62/2+/*obj_doll[-1]->w*/60/2+/*obj_doll[1]->w*/53/2)/2);/*???*/
	//	x256		= ( 256*( (GAME_WIDTH)-(62/2)-(60/2)-(53/2) ) )/(2);/*???*/  /*obj_doll[0]->w*/ /*obj_doll[-1]->w*/ /*obj_doll[1]->w*/
		s->x256 	= t256( 146)+t256(62/2);
		s->y256 	= t256(-100);
		data->state1++/* = ST_01*/;
		firewait1	= 45;
		firewait2	= 4;
		firewait3	= 0;
		break;
	case ST_01:/*���ֈړ�*/
		s->y256 += t256(2)/**fps_fa ctor*/;
		if (s->y256 >= t256(40/*40*/) )
		{
			data->state1++/* = ST_02*/;
		//	((PLAYER_DATA *)player->data)->bo ssmode=B05_BE FORE_LOAD;
			((PLAYER_DATA *)player->data)->state_flag |= STATE_FLAG_10_IS_LOAD_SCRIPT;
		}
		break;
		/*�p������*/
	case ST_02:
	//	if (B00_NONE/*B01_BA TTLE*/==(((PLAYER_DATA *)player->data)->bo ssmode))
		if ( ((((PLAYER_DATA *)player->data)->state_flag) & STATE_FLAG_05_IS_BOSS) )
		{
			my_wait = 40;
			data->state1++/* = ST_03*/;
		}
		break;
	case ST_03: 	/*wait*/
		if (my_wait <= 0)
		{
			/* �v���C���[�e�󂯕t���A�R�[���o�b�N��o�^ */
			((PLAYER_DATA *)player->data)->callback_boss_hitbyweapon=callback_enemy_boss01_hitbyweapon;
			data->state1++/* = ST_03*/;
		}
		break;
/*���S�Ɏp������*/
	case ST_04: 	/* �ړ��� */
		if (my_wait<=0)
		{	/* �ړ����������߂� */
			data->state8++;
			data->state8 &= (8-1);
			{
				enum
				{
					PPP_00_VX = 0,			/* x �x�N�g���ړ��� */
					PPP_01_VY,				/* y �x�N�g���ړ��� */
					PPP_02_WAIT_DIV_2,		/* �E�F�C�g�J�E���^�̔����� */
					PPP_03_IS_RESET_ANIME,	/* �A�j���[�V�������Z�b�g 0:���Ȃ� 1:���� 2:����(�e������) */
				};
				s8 ppp[8][4] =
				{
					{( 2),(-1),(100),( 1),},	/*�E���*/
					{( 0),( 0),( 50),( 2),},	/*wait*/
					{( 2),( 1),(100),( 1),},	/*�E����*/
					{( 0),( 0),( 10),( 0),},	/*wait*/
					{(-2),(-1),(100),( 1),},	/*�����*/
					{( 0),( 0),( 50),( 2),},	/*wait*/
					{(-2),( 1),(100),( 1),},	/*������*/
					{( 0),( 0),( 10),( 0),},	/*wait*/
				};
				data->vx	= ppp[data->state8][PPP_00_VX];
				data->vy	= ppp[data->state8][PPP_01_VY];
				my_wait 	= ppp[data->state8][PPP_02_WAIT_DIV_2]; 	/* 50*4 60 �ړ��� */
				my_wait 	+= my_wait;
				if (0!=ppp[data->state8][PPP_03_IS_RESET_ANIME])
				{
					if (2==ppp[data->state8][PPP_03_IS_RESET_ANIME])	/* �U���A�j���[�V���� */
					{
						alice_anime_count = 48;
						data->boss_base.danmaku_test++;
						data->boss_base.danmaku_type		= data->boss_base.danmaku_test/*DANMAKU_01*/;	/* �S�e�����Z�b�g */
						data->boss_base.danmaku_time_out	= (DANMAKU_01_SET_TIME+DANMAKU_01_SET_TIME);	/* �S�e���̔������� x 2 */
					}
					vvv256=1;
				}
			}
		}
		break;
	}
	/* alice move */
	{
		s->x256 += (data->vx)*vvv256;
			 if (s->x256< t256( 		0)+t256(24) )	{	s->x256=t256(		  0)+t256(24);}
		else if (s->x256> t256(GAME_WIDTH)-t256(24) )	{	s->x256=t256(GAME_WIDTH)-t256(24);}
	//
		s->y256 += (data->vy)*vvv256;
			 if (s->y256 < t256(0)	)	{	s->y256 = t256(0);	}
		else if (s->y256 > t256(96) )	{	s->y256 = t256(96); }
	}
	alice_animation(s);
//	move_all_doll(s);
//
	danmaku_generator(s);	/* �e������ */
//
//
	/*---------------------------------------------------------
		�_���[�W�`�F�b�N
	---------------------------------------------------------*/
//
	{
	}
//
	{
	//	if (0 == data->boss_base.boss_life)
		{
		//	if (FLG_ALL_CAST==(b01_live_flags&FLG_ALL_CAST))/*�S���|�����H*/
			if (data->boss_base.boss_health < 1)/*�S���|�����H*/
			{
				item_create_for_boss(s, ITEM_CREATE_MODE_01);
			//
				/* �A���X��|���ƊF�j�󂳂��B */
				#if 1
				b01_live_flags		= 0/*FLG_ALL_CAST*/;
				#endif
			//
				player_set_destoroy_boss();
				explosion_add_circle(s, 1); /* player_set_destoroy_boss(); B02_BOSS_DESTROY ���K�v */
				s->type 	= SP_DELETE;
			}
		}
	}
}
	/* �R�[���o�b�N�o�^ */
//	((PLAYER_DATA *)player->data)->callback_boss_hitbyweapon = callback_enemy_boss01_hitbyweapon_dummy; /* �_�~�[�R�[���o�b�N�o�^ */

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

void add_boss_alice(STAGE_DATA *l)/*int lv*/
{
	int lv;
	lv	= l->user_y;
//
	b01level			= lv+1;
	alice_anime_count	= 0;
//

//----[ZAKO]
//	bb_angle512 	= 128-43/*CCW*/ /*CW 43*/;/*512/12==42.66*/
	bb_angle512 	= 128/*CCW*/ /*CW 43*/;/*512/12==42.66*/

	b01_live_flags	= (FLG_ALL_CAST);
//
//----[BOSS]

	SPRITE *sakuya;
	sakuya							= sprite_add_res(BASE_BOSS_ALICE_PNG); /*"boss01-mo.png"60x42, offset*/
	sakuya->flags					|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
	sakuya->anim_frame				= 0;
	sakuya->type 					= SP_BOSS/*SP_BOSS01*/;
	sakuya->callback_mover			= move_alice;

	{
		BOSS01_DATA *data;
		data								= mmalloc(sizeof(BOSS01_DATA));
		sakuya->data 						= data;
		//
		data->boss_base.boss_health 		= (1023);	// �A���X�{�̂�HP�B�������������Ă����������B// [***090305	�ύX
		data->boss_base.boss_life			= (0);
		data->boss_base.score				= adjust_score_by_difficulty(score( 100000));	/*	10�� x 1�l (�v100��==(1�lx10��)+(6�lx5��)+(2�lx30��)) */
//		data->boss_base.score				= score(1000)*(difficulty+1);
		data->state1						= ST_00;
		data->state8						= 0;
		data->move_type 					= ALICE_01_KEITAI;
		data->vx = ( 0);
		data->vy = ( 0);

//		data->boss_base.boss_timer			= 40*64;		/* 40*64==40[count] 	��40[�b(64/60)](�P�ʂ͕b�ł͂Ȃ�) */
		data->boss_base.boss_timer			= (99-24)*64;	/* 75*64==75[count] 	��75[�b(64/60)](�P�ʂ͕b�ł͂Ȃ�) */
		//
		#if 1
//------------ �e���֘A
		data->boss_base.danmaku_type		= 0;
		data->boss_base.danmaku_time_out	= 0;
		data->boss_base.danmaku_test		= (DANMAKU_08-1)/*0*/;
		#endif
//
		((PLAYER_DATA *)player->data)->boss = sakuya;
		/* �R�[���o�b�N�o�^ */
		((PLAYER_DATA *)player->data)->callback_boss_hitbyweapon = callback_enemy_boss01_hitbyweapon_dummy; /* �_�~�[�R�[���o�b�N�o�^ */
	}

//----[ZAKO]
	int i;
	/* 85.33==512/6 170.66== 512/3 */	/*171*/
	#define AA_OFS85 (85)
	int jj_angle512;
	jj_angle512= -(int)(AA_OFS85/2);	/*0*/
	for (i=0; i<8; i++)
	{
		obj_doll[i] 						= sprite_add_res(BASE_BOSS_ALICE_DOLLS_PNG/*doll_res_num[i]*/);
//		obj_doll[i]->anim_speed 			= 0;
		obj_doll[i]->type					= SP_ZAKO/*SP_BOSS01*/;
		obj_doll[i]->flags					|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);

		obj_doll[i]->callback_loser 		= lose_doll;

		DOLL_DATA *data;
		data								= mmalloc(sizeof(DOLL_DATA));
		data->sakuya_obj					= sakuya;
		obj_doll[i]->data					= data;
		data->identity_bit					= (FLG_DOLL0<<i);		/* �ŗL�r�b�g�ԍ� */
//		data->health_flag					= 0;
		data->bwait 						= 0;
	//	br1_angle512						= deg_360_to_512(0);
	//	br2_angle512						= deg_360_to_512(0);
//		data->br_angle512					= deg_360_to_512(0);/*CW*/
		data->br_angle512					= (0);/*CCW*/
		data->base.health 					= (1023)/*(200)*/ /*((2==i)?(b01_04[difficulty]):(b01_health[i]))*/;
	//	data->base.score					= score( 500)*(difficulty+1);
		if (1<i)
		{
			obj_doll[i]->callback_mover 	= move_doll01;
			obj_doll[i]->anim_frame 		= 0x20+((i&1)<<4);		/* �~�j�l�` / �g�����̐l�` */
			data->base.score				= adjust_score_by_difficulty(score(  50000));	/*	 5�� x 6�l */
		}
		else
		{
			obj_doll[i]->callback_mover 	= move_doll02;
			obj_doll[i]->anim_frame 		= (i<<4);				/*  0:�������̐l�`(������) / 1:�ɂ̖��̐l�`(���E��) */
			data->base.score				= adjust_score_by_difficulty(score( 300000));	/*	30�� x 2�l */
		}
		data->fix_angle512					= jj_angle512;
		jj_angle512 += (AA_OFS85);
		data->state222						= 0;
	}
}

//�V�X�R�A�z��: 	�������ƕɂ̖��͂Ȃ�ׂ��|�����B�{�ȏ�X�R�A���Ⴄ��B
//		data->boss_base.score				= adjust_score_by_difficulty(score( 100000));	/*	10�� x 1�l (�v100��==(1�lx10��)+(6�lx5��)+(2�lx30��)) */
//		data->base.score					= adjust_score_by_difficulty(score( 300000));	/*	30�� x 2�l */
//		data->base.score					= adjust_score_by_difficulty(score(  50000));	/*	 5�� x 6�l */


//���X�R�A�z��
//		data->boss_base.score				= adjust_score_by_difficulty(score( 500000));	/*	50�� x 1�l (�v100��==(1�lx50��)+(5�lx10��)) */
//		data->base.score					= adjust_score_by_difficulty(score( 100000));	/*	10�� x 5�l */


//	((PLAYER_DATA *)player->data)->bo ssmode = B00_NONE/*B01_BA TTLE*/;
