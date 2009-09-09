
#include "bullet_object.h"

/*---------------------------------------------------------
	�A���X
	-------------------------------------------------------
	�{�X�^�C�}�[���Ή���
---------------------------------------------------------*/

typedef struct _boss01_data
{
	BOSS_BASE boss_base;
	int state1; 		//�`��
	int health_flag;
//
	int identity;		// �ŗL�ԍ�
	int bwait;			// �e���˂܂ł̑҂����ԁB
	int br_angle512;	// �e�̊p�x
} BOSS01_DATA;

//static int bwait1;	// �e���˂܂ł̑҂����ԁB
//static int bwait2;
//static int br1_angle512;		// �e�̊p�x
//static int br2_angle512;


enum
{
/*0*/	ALICE_01_KEITAI = 0,	// ���`��: �S���ōU��
/*1*/	ALICE_02_KEITAI,		// ���`��: �t�H�[���V����2
/*2*/	ALICE_03_KEITAI,		// ��O�`��: �A���X�P��
};

// FLG_FIRE1 �e�t���O�B�g�����̐l�`�����Ă��āA���w��ʒu�ɕt�������Bon�ɂȂ�B
#define FLG_FIRE1 0x01/* �������̐l�`(������)���w��ʒu�ɕt������ */
#define FLG_FIRE2 0x02/* �ɂ̖��̐l�`(���E��)���w��ʒu�ɕt������ */
//
#define FLG_DOLL0 0x04/* �A���X�{�l*/
#define FLG_DOLL1 0x08/* �������̐l�`(������) */
#define FLG_DOLL2 0x10/* �ɂ̖��̐l�`(���E��) */
#define FLG_DOLL3 0x20/* �g�����̐l�`(�l�`) */	/*key*/
#define FLG_DOLL4 0x40/* �����O�̐l�` */
#define FLG_DOLL5 0x80/* ���E�O�̐l�` */

#define FLG_ALL_DOLL   (FLG_DOLL1|FLG_DOLL2|FLG_DOLL3|FLG_DOLL4|FLG_DOLL5)/*�A���X�ȊO*/
#define FLG_ALL_CAST   (FLG_DOLL0|FLG_ALL_DOLL)/*�S��*/

/*---------------------------------------------------------

---------------------------------------------------------*/

static unsigned int b01flags/*=0*/;
static int b01level;
static unsigned int aaa_count;
static int b01state;

static int bb_angle512;

static SPRITE *sb01[6];

/*---------------------------------------------------------
���l���̔z�u����
[1][0][2]
[4][3][5]
---------------------------------------------------------*/

/* boss wurde von player-weapon ber??rt */
static void callback_enemy_boss01_hitbyweapon_dummy(SPRITE *s, SPRITE *t/*, int angle*/)
{	/* ���S�Ɏp�������܂ōU���́A���ׂĖ����Ƃ���B */
}
/*	s = �{�X�{�� boss sprite
	t = �v���C���[�̒e player's weapon */
static void callback_enemy_boss01_hitbyweapon(SPRITE *s, SPRITE *t/*, int angle*/)
{
	explosion_add_type(t->x256,t->y256,/*0,*/EXPLOSION_MINI00);/*��Ɏ��s������������*/
	//parsys_add(NULL,100,0,s->x,s->y,30,0,0,50,PIXELATE,NULL);
//
	WEAPON_BASE *w		= (WEAPON_BASE *)t->data;
	BOSS01_DATA *data	= (BOSS01_DATA *)s->data;
//
	data->boss_base.health -= w->strength;
//
	if (data->boss_base.health <= 15)
	{
		if (0==data->health_flag)
		{
			data->health_flag = 1;
			explosion_add_type(s->x256+((s->w128)),s->y256+((s->h128)),/*0,*/EXPLOSION_FIRE08);
		}
		if (data->boss_base.health <= 0)/* �Y���҂̗̑͐؂�Ȃ� */
		{
			data->boss_base.health = 0;
			//
			explosion_add_type(s->x256+((s->w128)),s->y256+((s->h128)),/*0,*/EXPLOSION_ZAKO04/*EXPLOSION_FIRE08*/);
			s->flags			&= (~(SP_FLAG_VISIBLE));
			item_create_for_boss(s, ITEM_CREATE_MODE_02);
			player_add_score(data->boss_base.score);
			{
				int i_id;
				i_id = data->identity;
				b01flags			|= (FLG_DOLL0<<i_id);/* �|�����l��ON�ɂ��� */
				if (0==i_id)	// �A���X���ǂ����`�F�b�N
				{
					b01flags |= FLG_ALL_CAST;	// �A���X��|���ƊF�j�󂳂��B
				}
			}
		}
	}
//
	if (FLG_ALL_CAST==(b01flags&FLG_ALL_CAST))/*�S���|�����H*/		/* boss komplett zerst??t */
	{
		item_create_for_boss(s, ITEM_CREATE_MODE_01);
//
		explosion_add_type(sb01[1]->x256+t256((/*sb01[1]->w*/62/(2*2))),sb01[1]->y256+t256(/*sb01[1]->h*/49/2),/*0,*/EXPLOSION_ZAKO04/*EXPLOSION_FIRE08*/);
		explosion_add_type(sb01[2]->x256+t256((/*sb01[2]->w*/53/(2*2))),sb01[2]->y256+t256(/*sb01[2]->h*/42/2),/*0,*/EXPLOSION_ZAKO04/*EXPLOSION_FIRE08*/);
		sb01[1]->flags	&= (~(SP_FLAG_VISIBLE));
		sb01[2]->flags	&= (~(SP_FLAG_VISIBLE));
//
		player_set_destoroy_boss();
		int i;
		for (i=0; i<6; i++)
		{
		//	explosion_add_type(sb01[i]->x256+((sb01[i]->w)<<7),sb01[i]->y256+((sb01[i]->h)<<7),/*(ra_nd()&(16-1)/*%20*/),*/EXPLOSION_FIRE08);
			explosion_add_circle(sb01[i], 1);	/* player_set_destoroy_boss();B02_BOSS_DESTROY ���K�v */
			sb01[i]->type = SP_DELETE;
		}
		/* �R�[���o�b�N�o�^ */
		((PLAYER_DATA *)player->data)->callback_boss_hitbyweapon = callback_enemy_boss01_hitbyweapon_dummy; /* �_�~�[�R�[���o�b�N�o�^ */
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void enemy_boss01_setpos(int x256, int y256)
{
//	int x = t256_floor(x256);
//	int y = t256_floor(y256);
	/*
		�g�����̐l�`���j�󂳂��ƁA�������̐l�`�ƕɂ̖��̐l�`�̍s���p�^�[�����ς��B
	*/
	if (0==(b01flags&FLG_DOLL3)/*sb01[3]->flags&SP_FLAG_VISIBLE*/)	// �g�����̐l�`�̐����m�F */
	{
		sb01[1]->x256 = x256+t256(3-4+4)/*offset*/;
		sb01[2]->x256 = x256+t256(1-4+4)/*offset*/+/*sb01[0]->w*/t256(60/2)+/*sb01[1]->w*/t256(62/2);
		sb01[1]->y256 = y256+t256(15)/*offset*/;
		sb01[2]->y256 = y256+t256(15)/*offset*/;
	}
	else	/* �g�����̐l�`���j�󂳂��� */
	{
		if (0==(b01flags&FLG_FIRE1))	// fire1�����˂ł��邩�ǂ��� */
		{
			#if 0
			/* �l�����͂���Ő������BVisual C++�Ȃ炱��Ŗ��Ȃ��B */
			/* ������ GCC �͒��z�֐��� const�ɂ��Ă���Ȃ��̂ŁA */
			/* ����ł͒x������B(�����������Ă��ꂽ�肵�Ă�) */
			sb01[1]->x+=co s((M_PI/4)*5)*3/**fps_fa ctor*/; 	// ���΂ߌ��Ɉړ� */
			sb01[1]->y+=si n((M_PI/4)*5)*3/**fps_fa ctor*/;
			#else
			/* GCC �͂��炩���ߌv�Z���Ƃ��� */
			sb01[1]->x256 -= t256(1.414)/**fps_fa ctor*/;		// ���΂ߌ��Ɉړ� */
			sb01[1]->y256 -= t256(1.414)/**fps_fa ctor*/;
			#endif
			if ((sb01[1]->x256 < t256(10)+t256(3-4+4)/*offset*/)||
				(sb01[1]->y256 < t256(10)+t256(15)/*offset*/))
			{	b01flags |= FLG_FIRE1;	}		//fire1�ړ����� */
		}
		if (0==(b01flags&FLG_FIRE2))	// fire2�����˂ł��邩�ǂ��� */
		{
			#if 0
			/* GCC �͒��z�֐��� const�ɂ��Ă���Ȃ� */
			sb01[2]->x+=co s((M_PI/4)*7)*3/**fps_fa ctor*/; 	// �E�΂ߌ��Ɉړ� */
			sb01[2]->y+=si n((M_PI/4)*7)*3/**fps_fa ctor*/;
			#else
			/* GCC �͂��炩���ߌv�Z���Ƃ��� */
			sb01[2]->x256 += t256(1.414)/**fps_fa ctor*/;		// �E�΂ߌ��Ɉړ� */
			sb01[2]->y256 -= t256(1.414)/**fps_fa ctor*/;
			#endif
			if ((sb01[2]->x256 > t256(GAME_WIDTH-50)+t256(1-4+4)/*offset*/)||
				(sb01[2]->y256 < t256(10)+t256(15)/*offset*/))
			{	b01flags |= FLG_FIRE2;	}		// fire2�ړ����� */
		}
	}
	sb01[0]->x256 = x256+t256(62/2)/*sb01[1]->w*/;			// �ʏ폈�� */
	sb01[0]->y256 = y256;

	#if 0
	sb01[4]->x256 = x256+t256(9-3)/*offset*/;
	sb01[3]->x256 = x256/*sb01[4]->x*/+t256(62/2)/*sb01[4]->w*/+t256(7-4)/*offset*/;
	sb01[5]->x256 = x256/*sb01[3]->x*/+t256(62/2)/*sb01[4]->w*/+t256(60/2)/*sb01[3]->w*/+t256(8-3)/*offset*/;

	sb01[4]->y256 = y256+ t256(49)/*sb01[1]->h*/+t256(3)/*offset*/;
	sb01[3]->y256 = y256+ t256(42)/*sb01[0]->h*/+t256(8)/*offset*/;
	sb01[5]->y256 = y256+ t256(42)/*sb01[2]->h*/+t256(10)/*offset*/;
	#else
	{
		bb_angle512++;
	int aa_angle512;
		aa_angle512 = bb_angle512;
	int jj;
		/* �g�����̐l�`(�l�`) �����O�̐l�` ���E�O�̐l�` */
		for (jj=3; jj<6; jj++)/*3 4 5*/
		{
			aa_angle512 += 171;/* 170.66== 512/3 */
			mask512(aa_angle512);
			sb01[jj]->x256 = sb01[0]->x256 + ((cos512((aa_angle512))*45));
			sb01[jj]->y256 = sb01[0]->y256 + ((sin512((aa_angle512))*45));
		}
	}
	#endif
//	/* �������̐l�`(������) */
	BOSS01_DATA *bdata;
	bdata = (BOSS01_DATA *)sb01[1]->data;
	if ((FLG_FIRE1==(b01flags&FLG_FIRE1))&&(bdata->boss_base.health>0)) //�j��ς݂Ȃ̂ɍU�����Ă��Ȃ��悤��
	{	/* �������ۂ������̂́A�e�����������邩�炾�Ǝv���̂ŏC��(3+b01level) */
		{	bdata->bwait--;}
		if (bdata->bwait <= 0)
		{
			bdata->bwait = 5+(3-difficulty)*5;
			bdata->br_angle512 += deg_360_to_512(10);
			bullet_create_jyuryoku_dan(
				sb01[1],
				256,//t256(1+b01level)/*(3+b01level)*/ /*(4+bdata->level)*/,
				(bdata->br_angle512&127)+256+64,// /*deg512_2rad*/( (bdata->br_angle512&255/*%180*/)-deg_360_to_512(90) ),
				((ra_nd()&0x03)+1)//t256(0.04)/*10*/
			);
		}
		//else
	}
//	/* �ɂ̖��̐l�`(���E��) */
	bdata=(BOSS01_DATA *)sb01[2]->data;
	if ((FLG_FIRE2==(b01flags&FLG_FIRE2))&&(bdata->boss_base.health>0))
	{
		{	bdata->bwait--;}
		if (bdata->bwait <= 0)
		{
			bdata->bwait = 5+(3-difficulty)*5;
		//	bdata->br_angle512 += deg_360_to_512(10);
			bdata->br_angle512 += 8;
			bullet_create_jyuryoku_dan(
				sb01[2],
				256,//t256(1+b01level)/*(3+b01level)*/ /*(4+bdata->level)*/,
				(bdata->br_angle512&127)+256+64,// /*deg512_2rad*/( (bdata->br_angle512&255/*%180*/)+deg_360_to_512(90) ),
				((ra_nd()&0x03)+2)//t256(0.04)
			);
		}
	//	else
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

/* where / 0: left, 1: right, 2: bombenhagel */
static void enemy_boss01_LR_dole_fire(void)
{
	//bullet_way(sb01[3],100, 0, M_PI*2/100,2);
	if (/*((BOSS01_DATA *)sb01[0]->data)->state*/b01state > 2)
	{
		//	const short spd_tbl_00[4] = { t256(1.5), t256(1.75), t256(2.0), t256(2.25) };
	//case 0://left 	/* �����O�̐l�` */
		if (0==(b01flags&FLG_DOLL4)/*sb01[4]->flags&SP_FLAG_VISIBLE*/)
		{
		//	const short spd_tbl_11[4] = { t256(4), t256(3), t256(2), t256(1) };
		//	const short spd_tbl_11[4] = { t256(1.5), t256(2.5), t256(2.0), t256(1.0) };
		//	bullet_create_aka_maru_jikinerai(sb01[4], (t256(1.5)+(difficulty<<6))/*spd_tbl_11[difficulty]*/ /*t256(4-difficulty)*/ /*5*/);
			bullet_create_n_way_dan_sa_type(sb01[4],
				(t256(1.5)+(difficulty<<6)),
				ANGLE_JIKINERAI_KISUDAN,
				(int)(512/64),
				BULLET_CAP16_04_SIROI,
				(4-difficulty));
		}
	//	break;
	//case 1://right	/* ���E�O�̐l�` */
		if (0==(b01flags&FLG_DOLL5)/*sb01[5]->flags&SP_FLAG_VISIBLE*/)
		{
		//	const short spd_tbl_22[4] = { t256(4), t256(5), t256(6), t256(7) };
		//	const short spd_tbl_22[4] = { t256(1.5), t256(2.5), t256(3.0), t256(5.0) };
		//	bullet_create_aka_maru_jikinerai(sb01[5], (t256(1.5)+(difficulty<<6))/*spd_tbl_22[difficulty]*/ /*t256(4+difficulty)*/ /*5*/);
			bullet_create_n_way_dan_sa_type(sb01[4],
				(t256(1.5)+(difficulty<<6)),
				ANGLE_JIKINERAI_KISUDAN,
				(int)(512/64),
				BULLET_CAP16_04_SIROI,
				(4-difficulty));
		}
	//	break;
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void enemy_boss01_nway_fire(void/*SPRITE *src, int x_offset256*/)
{
	//bullet_way(sb01[3],100, 0, M_PI*2/100,2);
	{
	//case 2://bombenhagel
		if (/*((BOSS01_DATA *)sb01[0]->data)->state*/b01state > 2)
		{
			play_voice_auto_track(VOICE14_YOKAI1_E_SHOT01);
			aaa_count = 16/*8*/;
		//
		//	int angle512;
		//	for (angle512=deg_360_to_512(0);angle512<=deg_360_to_512(180);angle512+=deg_360_to_512(20))
			{
				const short spd_tbl_33[4] = { t256(3.0), t256(3.0), t256(2.0), t256(1.0) };/* �x��������� */
		//		bullet_create_offset_dan_type(src,
		//			spd_tbl1[difficulty]/*(4-difficulty)*/ /*4*/,
		//			angle512,
		//			/*offsx*/x_offset256/*(src->w/2)*/,
		//			/*offsy*/t256(0)/*(src->h/2)*/,
		//			BULLET_HARI32_00_AOI
		//		);
				bullet_create_n_way_dan_sa_type(sb01[0],
					spd_tbl_33[difficulty]/*(4-difficulty)*/ /*4*/,
					deg_360_to_512(90),
					(int)(512/24),
					BULLET_HARI32_00_AOI,
					12);
			}
		}
		if (FLG_DOLL3==(b01flags&FLG_DOLL3)/*(0==(sb01[3]->flags&SP_FLAG_VISIBLE))*/)	/* �g�����̐l�`���j�󂳂��� */
		{
			/* �������ۂ������̂́A�e�����������邩�炾�Ǝv���̂ŏC�� */
		//	const short spd_tbl2[4] = { t256(3), t256(3), t256(4), t256(4) };/*spd_tbl2[difficulty]*/ /*(3+difficulty)*/
			bullet_create_n_way_dan_sa_type(sb01[0],
				(t256(1.5/*3.0*/)+((difficulty)<<6)),
				ANGLE_JIKINERAI_KISUDAN,
				(int)(512/24),
				BULLET_UROKO14_04_MIZUIRO,
				12);	/* [A]��[B]�ɕύX����(�w�Ǔ���) */
			// [A]	angle512=atan_512(player->y-s->y+((player->h-s->h)/2),player->x-s->x-((player->w	   )/2));
			// [B]	angle512=atan_512(player->y-s->y+((player->h-s->h)/2),player->x-s->x-((player->w+s->w)/2));/*???(original)*/	/* ��e�̏ꍇ�Ɏ��@�_�� */
			// [C]	angle512=atan_512(player->y-s->y+((player->h-s->h)/2),player->x-s->x+((player->w-s->w)/2));/*???*/			/* �����e�̏ꍇ�Ɏ��@�_�� */
		}
	//	break;
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/
static int vvv256;
static void enemy_vvv(void)
{
	vvv256++/*vvv256+=vvv256*/;
	if (vvv256> t256(1) )
	{	vvv256= t256(1);}
//
	if (aaa_count)
	{
		aaa_count--;
		sb01[0]->anim_frame=4;
	}
	else
	{
		if (t256(0.3)>vvv256)		{	sb01[0]->anim_frame=3;}
		else
		if (t256(0.5)>vvv256)		{	sb01[0]->anim_frame=2;}
		else
		if (t256(0.7)>vvv256)		{	sb01[0]->anim_frame=1;}
		else						{	sb01[0]->anim_frame=0;}
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void enemy_boss01_move(SPRITE *c)
{
//	BOSS01_DATA *b=(BOSS01_DATA *)c->data;
	static int x256;
	static int y256;
	static int my_wait;
	static int firewait1;
	static int firewait2;
	static int firewait3;

	switch (/*data->state*/b01state)
	{
	case 0:
	//	x256		= (GAME_WIDTH*256/2); // /*t256*/256*(GAME_WIDTH/2-(/*sb01[1]->w*/62/2+/*sb01[0]->w*/60/2+/*sb01[2]->w*/53/2)/2);/*???*/
	//	x256		= ( 256*( (GAME_WIDTH)-(62/2)-(60/2)-(53/2) ) )/(2);/*???*/  /*sb01[1]->w*/ /*sb01[0]->w*/ /*sb01[2]->w*/
		x256		= t256( 146);
		y256		= t256(-100);
		enemy_boss01_setpos(x256,y256);
		/*data->state*/b01state = 1;
		firewait1	= 45;
		firewait2	= 4;
		firewait3	= 0;
		break;
	case 1:
		y256 += t256(2)/**fps_fa ctor*/;
		enemy_boss01_setpos(x256,y256);
		if (y256 >= t256(20) )
		{
			/*data->state*/b01state=2;
			my_wait=0;
		//	((PLAYER_DATA *)player->data)->bo ssmode=B05_BE FORE_LOAD;
			((PLAYER_DATA *)player->data)->state_flag |= STATE_FLAG_10_IS_LOAD_SCRIPT;
		}
		break;
	case 2:
	//	if (B00_NONE/*B01_BA TTLE*/==(((PLAYER_DATA *)player->data)->bo ssmode))
		if ( ((((PLAYER_DATA *)player->data)->state_flag) & STATE_FLAG_05_IS_BOSS) )
		{
			my_wait += (1)/*fps_fa ctor*/;
			if (my_wait >= 40)
			{
				/*data->state*/b01state=3;
			}
		}
		break;
	case 3:/*���ֈړ�*/
		y256 += t256(1)/*fps_fa ctor*/;
		enemy_boss01_setpos(x256,y256);
		if (y256 >= t256(50) )
		{
			/*data->state*/b01state=4;
			my_wait=0;
			/* �v���C���[�e�󂯕t���A�R�[���o�b�N��o�^ */
			((PLAYER_DATA *)player->data)->callback_boss_hitbyweapon=callback_enemy_boss01_hitbyweapon;
		}
		break;
/*���S�Ɏp������*/
	case 4: 																																																	my_wait+=1/*fps_fa ctor*/;		if (my_wait>=30)	{	sb01[0]->anim_frame=0;			/*data->state*/b01state=5; vvv256=1;			my_wait=/*60*/50*4; }		break;
//
	case 5:/*�E��*/ y256 += vvv256; if (y256 > t256(64) )	{y256 = t256(64); } x256-=2*vvv256; enemy_vvv();	if (x256< t256(5/*10*/) )		{	x256=t256(5/*10*/);}	enemy_boss01_setpos(x256,y256); 	my_wait-=1/*fps_fa ctor*/;		if (my_wait<=0) 	{	/*data->state*/b01state=6; vvv256=1;	my_wait=/*60*/50*4; 	}		break;
	case 6: 		y256 -= vvv256; if (y256 < t256(0)	)	{y256 = t256(0);  } x256+=2*vvv256; enemy_vvv();																enemy_boss01_setpos(x256,y256); 	my_wait-=1/*fps_fa ctor*/;		if (my_wait<=0) 	{	/*data->state*/b01state=7; vvv256=1;	my_wait=/*60*/50*4; 	}		break;
	case 7:/*����*/ y256 += vvv256; if (y256 > t256(64) )	{y256 = t256(64); } x256+=2*vvv256; enemy_vvv();	if (x256> t256(290/*350*/) )	{	x256=t256(290/*350*/);} enemy_boss01_setpos(x256,y256); 	my_wait-=1/*fps_fa ctor*/;		if (my_wait<=0) 	{	/*data->state*/b01state=8; vvv256=1;	my_wait=/*60*/50*4; 	}		break;
	case 8: 		y256 -= vvv256; if (y256 < t256(0)	)	{y256 = t256(0);  } x256-=2*vvv256; enemy_vvv();																enemy_boss01_setpos(x256,y256); 	my_wait-=1/*fps_fa ctor*/;		if (my_wait<=0) 	{	/*data->state*/b01state=4; vvv256=1;	my_wait=/*60*/50*4; 	}		break;
	}
	firewait1 -= /*fps_fa ctor**/(b01level/*data->level+1*/);
	if (firewait1<=0)
	{
		firewait1=45;
		firewait2--;
		if (0==firewait2)
		{
			firewait2=4;
			firewait3=1+difficulty;
			if (FLG_ALL_DOLL==(b01flags&FLG_ALL_CAST))/*�A���X�ȊO�S���|�����H*/
			{
				firewait3++;	/* �Ōオ�ȒP������̂ŏ����苭������ */
			}
		}
		else
		{
			enemy_boss01_LR_dole_fire();/*enemy_boss01_fire(0);enemy_boss01_fire(1);*/
		}
		if (firewait3)
		{
			firewait3--;
			enemy_boss01_nway_fire(/*sb01[3], (firewait3<<(3+8))*/ /*x_offset256*/);/*enemy_boss01_fire(2);*/
		}
	}
	BOSS01_DATA *data;
	data=(BOSS01_DATA *)sb01[0]->data;
//	���ƂŃR�[���o�b�N�ɂ��ċ��ʉ�����B
	{
		data->boss_base.boss_timer -= 1/*fps_fa ctor*/;
		#if 1/*���Ԑ؂�(�F�X��肠��̂łƂ肠��������)*/
		if ((data->boss_base.boss_timer < 1))
		{
			data->boss_base.boss_timer = (60*64);
		//	if ((data->state1 < SAKUYA_05_KEITAI/*4*/))
		//	{
		//		//���~���A��̊J�n�C�x���g������Ȃ��Ȃ����Ⴄ�̂ł����͖���
		//		#if (0==USE_REMILIA)
		//		/* vs REMILIA */
		//		if (2==select_player)
		//		{data->boss_base.health=0/*1*/;}
		//		else
		//		#endif
		//		{data->boss_base.health 	= ((/*4*/SAKUYA_05_KEITAI*1024)-1)-(data->state1<<10/**1024*/);}
		//		sakuya_put_items(/*c,*/s);
		//	}
		}
		#endif
	}
//

}

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

void add_boss_alice(STAGE_DATA *l)/*int lv*/
{
	int lv;
	lv	= l->user_y;
//
	b01level	= lv+1;
	b01flags	= 0;
	aaa_count	= 0;
//	vvv256=/*fps_fa ctor**/t256(b01level/*data->level+1*/)/4;
//	sb01[0] = spr ite_add_file 0("boss/boss01_1.png", 8, PRIORITY_03_ENEMY, 0); sb01[0]->anim_speed=0;/*"boss01-mo.png"60x42, offset*/
//	sb01[1] = spr ite_add_file 0("boss/boss01_0.png", 4, PRIORITY_03_ENEMY, 0); sb01[1]->anim_speed=8;/*"boss01-lo.png"62x49, offset(3-4+4)x(15)*/
//	sb01[2] = spr ite_add_file 0("boss/boss01_2.png", 4, PRIORITY_03_ENEMY, 0); sb01[2]->anim_speed=8;/*"boss01-ro.png"53x42, offset(1-4+4)x(15)*/
//	sb01[4] = spr ite_add_file 0("boss/boss01_3.png", 2, PRIORITY_03_ENEMY, 0); sb01[4]->anim_speed=8;/*"boss01-lu.png"62x67, offset(9-3)x(3)*/
//	sb01[3] = spr ite_add_file 0("boss/boss01_4.png", 2, PRIORITY_03_ENEMY, 0); sb01[3]->anim_speed=8;/*"boss01-mu.png"60x57, offset(7-4)x(8)*/ //key
//	sb01[5] = spr ite_add_file 0("boss/boss01_3.png", 2, PRIORITY_03_ENEMY, 0); sb01[5]->anim_speed=8;/*"boss01-ru.png"53x57, offset(8-3)x(10)*/
//
	bb_angle512=43;/*512/12==42.66*/

	sb01[0] = sprite_add_res(BASE_BOSS_ALICE_1_PNG); /*"boss01-mo.png"60x42, offset*/
	sb01[1] = sprite_add_res(BASE_BOSS_ALICE_0_PNG); /*"boss01-lo.png"62x49, offset(3-4+4)x(15)*/
	sb01[2] = sprite_add_res(BASE_BOSS_ALICE_2_PNG); /*"boss01-ro.png"53x42, offset(1-4+4)x(15)*/
	sb01[3] = sprite_add_res(BASE_BOSS_ALICE_4_PNG); /*"boss01-mu.png"60x57, offset(7-4)x(8)*/ //key
	sb01[4] = sprite_add_res(BASE_BOSS_ALICE_3_PNG); /*"boss01-lu.png"62x67, offset(9-3)x(3)*/
	sb01[5] = sprite_add_res(BASE_BOSS_ALICE_3_PNG); /*"boss01-ru.png"53x57, offset(8-3)x(10)*/
//
	/*data->state*/b01state = 0;
	int i;
	for (i=0; i<6; i++)
	{
		const unsigned short b01_health[6] =
		{
		/*	   if (0==i)	{	data->boss_base.health=*/1023,//;	}	// �A���X�{�̂�HP�B�������������Ă����������B// [***090305	�ύX
		/*else if (1==i)	{	data->boss_base.health=*/ 400,//;	}	// [***090114		�ύX(+50)
		/*else if (2==i)	{	data->boss_base.health=*/ 400,//;	}	// [***090114		�ύX(+50)
		/*else if (3==i)	{	data->boss_base.health=*/  20,//;	}	// �g�����̐l�` ((RANK_MAX-difficulty)*20)
		/*else if (4==i)	{	data->boss_base.health=*/ 200,//;	}	// [***090114	�ύX(+20)
		/*else if (5==i)	{	data->boss_base.health=*/ 200 //;	}	// [***090114	�ύX(+20)
		};
		// 3==�g�����̐l�`(�ō������N�̏ꍇ20�A�ȉ������N�����邲�Ƃ�20��������) /* ((RANK_MAX-difficulty)*20) */
		const unsigned short b01_04[4] = {	200,	100,	50, 	20, 	};
		sb01[i]->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
//		sb01[i]->anim_speed 	= 0;
		sb01[i]->anim_frame 	= 0;
		sb01[i]->type			= SP_BOSS/*SP_BOSS01*/;
		BOSS01_DATA *data;
		data					= mmalloc(sizeof(BOSS01_DATA));
		sb01[i]->data			= data;
		data->identity			= i;		// �ŗL�ԍ�
		data->health_flag		= 0;
		data->bwait 			= 0;
	//	br1_angle512			= deg_360_to_512(0);
	//	br2_angle512			= deg_360_to_512(0);
		data->br_angle512		= deg_360_to_512(0);
		data->boss_base.health	= ((3==i)?(b01_04[difficulty]):(b01_health[i]));
		//
		if (0==i)/* �A���X�{�l */
		{	sb01[i]->callback_mover 	= enemy_boss01_move;
			data->boss_base.score		= adjust_score_by_difficulty(score( 500000));	/*	50�� (�v100��==(50��)+(5x10��)) */
		//	data->boss_base.score		= score(1000)*(difficulty+1);
		}
		else/* �l�`�B */
		{
			data->boss_base.score		= adjust_score_by_difficulty(score( 100000));	/*	10�� */
		//	data->boss_base.score		= score( 500)*(difficulty+1);
		}
		data->boss_base.boss_timer		= 60*64;	/*	[] */
	}
	//
	((PLAYER_DATA *)player->data)->boss = sb01[0];
	/* �R�[���o�b�N�o�^ */
	((PLAYER_DATA *)player->data)->callback_boss_hitbyweapon = callback_enemy_boss01_hitbyweapon_dummy; /* �_�~�[�R�[���o�b�N�o�^ */
}
//	((PLAYER_DATA *)player->data)->bo ssmode = B00_NONE/*B01_BA TTLE*/;
