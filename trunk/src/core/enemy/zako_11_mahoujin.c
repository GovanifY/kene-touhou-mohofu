
#include "bullet_object.h"

/*---------------------------------------------------------
	"�����w2"		�����閂���w
	"�����w1"		�΂�܂��U�������w
	-------------------------------------------------------
	�n�`�Ɋ֘A����G
---------------------------------------------------------*/

/* ����g�p */
#define MAHOU_TEKI_wait1		vx256
#define MAHOU_TEKI_wait2		vy256
#define MAHOU_TEKI_time_out 	tmp_angleCCW512

/* ���Ԑݒ� */

//#define MAHOU_TIME_LIMIT_00_MAX	(0x1ff)
//#define MAHOU_TIME_LIMIT_00_MAX	(0x17f)
#define MAHOU_TIME_LIMIT_00_MAX 	(0x13f)
#define MAHOU_TIME_LIMIT_01_ADJ 	((int)(0x0ff/3))
#define MAHOU_TIME_LIMIT_02_ADJ 	(MAHOU_TIME_LIMIT_00_MAX - MAHOU_TIME_LIMIT_01_ADJ)
#define MAHOU_TIME_LIMIT_03_EPS 	(0x04)

/*---------------------------------------------------------
	�G����
---------------------------------------------------------*/

static void lose_kougeki_mahoujin(SPRITE *src)
{
//	case SP_GROUNDER:
	if (rand_percent(30))	{	item_create(src,						SP_ITEM_00_P001,	1, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY)/*(up_flags)*/ );}
	if (rand_percent(30))	{	item_create(src,/*->x+10, c->y-5,*/ 	SP_ITEM_00_P001,	1, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY)/*(up_flags)*/ );}
	if (rand_percent(30))	{	item_create(src,/*->x+ 5, c->y+5,*/ 	SP_ITEM_06_TENSU,	1, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY)/*(up_flags)*/ );}
}

/*---------------------------------------------------------
	�G�U�����Ȃ�
---------------------------------------------------------*/

static void attack_kougeki_mahoujin00(SPRITE *src)
{
	/* �U�����Ȃ� */
}
/*---------------------------------------------------------
	�c�U���̃X�y�J
---------------------------------------------------------*/

static void attack_kougeki_mahoujin01(SPRITE *src)
{
	/* �U���J�n�t���[������16�t���[���o�ߌ�� 1�񂾂��U�� */
	if ((MAHOU_TIME_LIMIT_02_ADJ-16) == src->MAHOU_TEKI_time_out)
	{
		bullet_create_n_way_dan_sa_type(src,
			(t256(2.5)),			/* �e�� */
			(0),					/* ������(�㉺�e) */
			(int)(512/(2)), 		/* �p�x(512[360/360�x]�� 2 ����) */ 	/* 1����n���������p�x */
			BULLET_KOME_02_AKA, 	/* �ԕĒe */
			(2));/* [2way�ԕĒe] */
	}
}

/*---------------------------------------------------------
	�G�U��("�����w2")�̃X�y�J
---------------------------------------------------------*/

static void attack_kougeki_mahoujin02(SPRITE *src)
{
	if (0 < src->MAHOU_TEKI_wait2)		// [***090128		�ύX
	{
		src->MAHOU_TEKI_wait1--;			/* �΂�܂��Ԋu�J�E���^ */
		if (src->MAHOU_TEKI_wait1 <= 0) 	/* �΂�܂��Ԋu�J�E���^ */
		{
			src->MAHOU_TEKI_wait1 = 8/*5*/;/*�v���O�����������Ȃ��Ă镪�x������*/
			src->MAHOU_TEKI_wait2--;
			#if (0==USE_DESIGN_TRACK)
			play_voice_auto_track(VOICE10_MAGICF_E_SHOT);
			#else
			voice_play(VOICE10_MAGICF_E_SHOT, TRACK06_ALEART_IVENT_02);/*�e�L�g�[*/
			#endif
			/* ���x�� 1.0-3.0�܂ł̖��i�K�����ɕύX */
			{	int angle512;
				angle512=(ra_nd()&(512-1))/*%360*/;
				bullet_create_tomari2_dan(src,
					/*t256(sp)*/((ra_nd()&(512-1))+256)/*speed256*/,
					/*degtorad*/(angle512),
					t256(0.04), 	/* �����萔 */
					ANGLE_NO_SHOT_DAN);
			}
		}
	}
}

/*---------------------------------------------------------
	�G�U��("�����w1")�̃X�y�J
---------------------------------------------------------*/

static void attack_kougeki_mahoujin03(SPRITE *src)
{
	if (
			(src->y256 >= player->y256)
	 || 	(src->y256 > t256(100))
	)
	{
	//	src->MAHOU_TEKI_time_out++;/* = ST01;*/
		src->MAHOU_TEKI_time_out = MAHOU_TIME_LIMIT_01_ADJ; /* ������ */
		bullet_create_aka_maru_jikinerai(src, t256(1) );
	}
//	if (src->MAHOU_TEKI_wait2<(difficulty*7)+10) // [***090128 �ύX */
	{
		src->MAHOU_TEKI_wait1--;			/* �΂�܂��Ԋu�J�E���^ */
		if (src->MAHOU_TEKI_wait1 <= 0) 	/* �΂�܂��Ԋu�J�E���^ */
		{
			src->MAHOU_TEKI_wait1 = (25-(difficulty*5)); /* �΂�܂��Ԋu�J�E���^ */
			#if (0==USE_DESIGN_TRACK)
			play_voice_auto_track(VOICE10_MAGICF_E_SHOT);
			#else
			voice_play(VOICE10_MAGICF_E_SHOT, TRACK06_ALEART_IVENT_02);/*�e�L�g�[*/
			#endif
			/* ���x�� 1.0-4.0�܂ł̗���(�����������ɕ΍�)�ɕύX */
			bullet_create_tomari2_dan(src,
				((ra_nd()&(1024-1))|(0x100))/*t256(sp)*/,
				/*deg512_2rad*/((ra_nd()&(512-1)))/*de gtorad(ra_nd()%360)*/,
				t256(0.03), 	/* �����萔 */
				ANGLE_NO_SHOT_DAN);
		}
	}
}

/*---------------------------------------------------------
	�~��U���̃X�y�J
---------------------------------------------------------*/

static void attack_kougeki_mahoujin04(SPRITE *src)
{
	/* �U���J�n�t���[������16�t���[���o�ߌ�� 1�񂾂��U�� */
	//if ((MAHOU_TIME_LIMIT_02_ADJ-16) == src->MAHOU_TEKI_time_out)
	//if (0 == (src->MAHOU_TEKI_time_out&0x07))
	{
		bullet_create_tomari2_dan(src,
			(t256(2.5)),			/* �e�� */
			((src->MAHOU_TEKI_time_out)+((src->MAHOU_TEKI_time_out&0x07)<<6)),					/* ���� */
	//		(int)(512/(7)), 		/* �p�x(512[360/360�x]�� 7 ����) */ 	/* 1����n���������p�x */
	//		BULLET_KOME_02_AKA, 	/* �ԕĒe */
	//		(7));/* [7way�ԕĒe] */
				t256(0.03), 	/* �����萔 */
				ANGLE_NO_SHOT_DAN);
	}
}

/*---------------------------------------------------------
	�~��U���̃X�y�J
---------------------------------------------------------*/

static void attack_kougeki_mahoujin05(SPRITE *src)
{
	/* �U���J�n�t���[������16�t���[���o�ߌ�� 1�񂾂��U�� */
	//if ((MAHOU_TIME_LIMIT_02_ADJ-16) == src->MAHOU_TEKI_time_out)
	//if (0 == (src->MAHOU_TEKI_time_out&0x07))
	{
		bullet_create_tomari2_dan(src,
			(t256(2.5)),			/* �e�� */
			((-src->MAHOU_TEKI_time_out)+((src->MAHOU_TEKI_time_out&0x07)<<6)),					/* ���� */
	//		(int)(512/(7)), 		/* �p�x(512[360/360�x]�� 7 ����) */ 	/* 1����n���������p�x */
	//		BULLET_KOME_02_AKA, 	/* �ԕĒe */
	//		(7));/* [7way�ԕĒe] */
				t256(0.03), 	/* �����萔 */
				ANGLE_NO_SHOT_DAN);
	}
}


/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/
		#if 0
		if (SP_MUTEKI == src->type)
		{			attack_kougeki_mahoujin02(src); }	/* ("�����w2") �����閂���w */
		else
	//	if (SP_ZAKO == src->type)
		{			attack_kougeki_mahoujin03(src); }	/* ("�����w1") �΂�܂��U�������w */
		#endif

static void move_kougeki_mahoujin(SPRITE *src)
{
	src->MAHOU_TEKI_time_out--;
	/* �o�� */
	if (MAHOU_TIME_LIMIT_02_ADJ < src->MAHOU_TEKI_time_out)
	{
		src->color32 += 0x03000000; 		/*	src->alpha += 0x03;*/ /*4*/ /**fps_fa ctor*/
	}
	/* �U�� */
	else
	if (MAHOU_TIME_LIMIT_01_ADJ < src->MAHOU_TEKI_time_out)
	{
		/* �X�y�J�I�� */
		void (*aaa[8])(SPRITE *src) =
		{
			attack_kougeki_mahoujin00,		/* �U�����Ȃ� */
			attack_kougeki_mahoujin01,		/* �c�e */
			attack_kougeki_mahoujin02,		/* �΂�܂� #01(�������A��"�����w2") */
			attack_kougeki_mahoujin03,		/* �΂�܂� #02(�������A��"�����w1") */
			attack_kougeki_mahoujin04,		/* 8way�Ԓe */
			attack_kougeki_mahoujin05,		/* 8way�Ԓe */
			attack_kougeki_mahoujin04,		/* 8way�Ԓe */
			attack_kougeki_mahoujin04,		/* 8way�Ԓe */
		};
		(*aaa[ (int)((src->x256)&0x07) ])(src); 	/* �e�֐��ɕ��򂷂� */
	}
	/* ������ */
	else
	if ( MAHOU_TIME_LIMIT_03_EPS < src->MAHOU_TEKI_time_out )	/*	if ( 0x04 0x00 > (unsigned int)(src->alpha))*/
	{
		src->color32 -= 0x03000000; 	/*	src->alpha += 0x03;*/ /*4*/ /**fps_fa ctor*/
	}
	/* �����܂� */
	else
	{
	//	src->color32 = 0x00ffffff;		/*	src->alpha = 0x00;*/
		src->type = SP_DELETE;
	}
//
	src->m_angleCCW512++;
	mask512(src->m_angleCCW512);
}

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

void add_enemy_mahoujin(STAGE_DATA *l)
{
	short	xxx;				xxx 		= l->user_x;
	short	yyy;				yyy 		= l->user_y;
	char	i_code; 			i_code		= l->user_i_code;
	char	user_1_moji;	user_1_moji 	= l->user_1_moji;
//	short speed256; 	speed256	= l->scroll_speed256;
//	data->speed256					= (speed256);
	SPRITE *s;		//	s			= NULL;
	s = NULL;
//
	switch (i_code)
	{
		/* �΂�܂��U�������w
		anim_speed: 1 ����
		anim_speed: 3 �x����
		*/
	case CTYPE_11_MAHOUJIN_A:		/* �΂�܂��U�������w */	//	"�����w1",	/*	"GROUNDER",*/
		s							= sprite_add_res(BASE_TIKEI_GROUNDER08_PNG);	//s->anim_speed =  2/*3*/ /*(5-difficulty)*/ /*1*/;/*9"grounder.png"*/
//		s							= sprite_add_res(BASE_TIKEI_GROUNDER08_PNG);	s->anim_speed	=  3 /*(5-difficulty)*/ /*2*/;/*9"grounder.png"*/
		s->type 					= SP_ZAKO/*SP_TIKEI*/ /*SP_GROUNDER*/;
		s->callback_hit_enemy		= callback_hit_zako;
		s->callback_loser			= lose_kougeki_mahoujin;
//		s->MAHOU_TEKI_wait1 		= 100;	// [***090124		�ǉ�
	//	s->MAHOU_TEKI_wait2 		= 0;/* ���g�p  */
		break;
	case CTYPE_12_MAHOUJIN_B:		/* �����閂���w */	//	"�����w2",	/*	"MAGICF",*/
		s							= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);	//s->anim_speed =  2/*3*/ /*(5-difficulty)*/ /*1*/;/*9"grounder.png"*/
		s->type 					= TEKI_51_MAHOJIN1/*SP_TIKEI*/ /*SP_GROUNDER*/;
//		s->type 					= SP_MUTEKI;
		/*data->base.*/s->base_health	= 999999;/* �|���Ȃ� */
	//	s->callback_hit_enemy		= callback_hit_zako;
//		s->MAHOU_TEKI_wait1 		= 10/*+90*/;/* */
	//	s->MAHOU_TEKI_wait2 		= 5/*16+(difficulty<<3)*/ /*10+(7*difficulty)*/;/* �ő�܉� */
		break;
	}
	s->flags						|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	s->callback_mover				= move_kougeki_mahoujin;
	/* KETM�̑��΍��W�w��͔p�~�B��ʍ��W�w��ɂ��� */
	s->x256 						= ((xxx)<<8)|(user_1_moji&0x07);/* ��������C���^�[���[�u */
	s->y256 						= ((yyy)<<8);	/*-((s->h128+s->h128))*/
	/*data->base.*/s->base_score	= score(200*2);
	/*data->base.*/s->base_health	+= 25+(difficulty<<3/*4*/);//+(difficulty*15);
//
	s->MAHOU_TEKI_time_out			= MAHOU_TIME_LIMIT_00_MAX;/* SS00 �܂��� ST00 */

	s->MAHOU_TEKI_wait1 			= 10/*+90*/;/* */
	s->MAHOU_TEKI_wait2 			= 5/*16+(difficulty<<3)*/ /*10+(7*difficulty)*/;/* �ő�܉� */
	s->color32						= 0x00ffffff;	/*	s->alpha				= 0x02 0x00;*/
//	s->an im_frame					= 0;
}
