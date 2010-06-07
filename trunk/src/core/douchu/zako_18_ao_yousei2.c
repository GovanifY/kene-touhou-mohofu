
#include "douchu.h"

/*---------------------------------------------------------
	�����͕핗  �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
		"�d��2",		"SPLASH",
	-------------------------------------------------------

---------------------------------------------------------*/
#if 0/* �߂� */
/* �{�X���ʋK�i */
	#define target_x256 		user_data00 	/* �ڕWx���W */
	#define target_y256 		user_data01 	/* �ڕWy���W */
	#define vvv256				user_data02 	/* �ڕW���W�ւ̓��B���� */
	#define time_out			user_data03 	/* �������� */
#endif
#define time_out		user_data03 	/* �o�ߎ��� */		/* ��ԑJ�ڗp�̎��Ԑ؂� */
#define nnn 			user_data04 	/* �e������ */
#define state			user_data05 	/* �d���̏�� */


//	int ani_turn;		/* �A�j���[�V��������(�H�̏㉺) */
//	int anime_frame;	/* �A�j���[�V���� */


/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/
enum
{
	STATE_00 = 0,
	STATE_01,
	STATE_02,
	STATE_03,
};


//#define SSS00 ( 0<<2)/*�E*/
//#define SSS04 ( 4<<2)/*�E�΂߉�*/
//#define SSS08 ( 8<<2)/*���P*/
//#define SSS12 (12<<2)/*���Q*/
//#define SSS16 (16<<2)/*���΂߉�*/
//#define SSS20 (20<<2)/*��*/
static void move_ao_yousei2(SPRITE *src)
{
	src->time_out--;	/* ��Ɏ��Ԍo�� */
	switch (src->state)
	{
	case STATE_00:	/* �o�� */
		if (0 > src->time_out)
		{
			src->time_out = 10;
			src->state++;// = STATE_01;/*����*/
		}
		else
		{
			/* �o��̓��� */
			src->x256 += src->vx256;/*fps_factor*/
			src->y256 += src->vy256;/*fps_factor*/
		}
		break;
	case STATE_01:	/* �����҂� */
		if (0 > src->time_out)
		{
			src->time_out = 50;
			src->state++;// = STATE_02;/*����*/
		}
		else
		{
			tmp_angleCCW1024_jikinerai(obj_player, src);
			mask1024(src->tmp_angleCCW1024);
		//	const u8 aa_offs[8] = { OF_16, OF_12, OF_08, OF_04, OF_00, OF_04, OF_08, OF_12 };
//			const u8 aa_offs[8] = { SSS00, SSS04, SSS08, SSS16, SSS20, SSS16, SSS08, SSS04 };
//			src->anime_frame = (src->anime_frame&(4-1))+aa_offs[(src->tmp_angleCCW1024>>7)];	/* 64  32== 512/16 */
			src->type				= /*SP_ZAKO*/TEKI_12_YOUSEI1_1+(src->tmp_angleCCW1024>>7)/*_17_AO_YOUSEI2*/;
		}
		break;
	case STATE_02:	/* �e������ */
		{
			int k;
			for (k=0; k<6/*4*/; k++)
			{
				const static u16 bk1024_tbl[4] =
				{((int)(1024*1/8)), ((int)(1024*1/12)), ((int)(1024*1/16)), ((int)(1024*1/24))};
				obj_send1->x256 					= (src->x256);
				obj_send1->y256 					= (src->y256);
				br.BULLET_REGIST_speed256			= (t256(2.5)+(k<<6));					/* �e�� */
				br.BULLET_REGIST_angle1024			= ANGLE_JIKI_NERAI_DAN; 				/* */
				br.BULLET_REGIST_div_angle1024		= (bk1024_tbl[(difficulty)]);			/* �����p�x */
				br.BULLET_REGIST_bullet_obj_type	= (BULLET_MARU8_00_AKA+(/*0*/6-k/*�Ƃ肠����*/)); /* [�Ԓe] */
				br.BULLET_REGIST_n_way				= (7);									/* [7way] */
				br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
				bullet_regist_vector();
			}
		}
//				if (1 < difficulty)
/*(1+(((k+difficulty)>>1)))*/  /*(2+((k>>1)))*/
/*(2+(((k+difficulty)>>1)))*/  /*(3+((k>>1)))*/

/*
  3+((k>>1))
0 4 3
1 4 4
2 4 4
3 4 5
*/
		src->nnn--;
		if (0 > src->nnn)
		{
			/* �ޏꏀ�� */
//				 if (src->enemy_rank<3) 	{	src->anime_frame=(src->anime_frame&(4-1))+SSS00;}
//			else if (src->enemy_rank<7) 	{	src->anime_frame=(src->anime_frame&(4-1))+SSS08;}
//			else							{	src->anime_frame=(src->anime_frame&(4-1))+SSS00;}
			src->state++;// = STATE_03;/*����*/
			src->time_out = 50;
		}
		else
		{
			src->state--;// = STATE_01;/*�߂�*/
		}
		break;
	case STATE_03:	/* �ޏ� */
		if (0 > src->time_out)
		{
			src->type = SP_DELETE;	/* �����܂� */
		}
		else
		{
			/* �ޏ�̓��� */
			src->x256 -= src->vx256;/*fps_factor*/
			src->y256 -= src->vy256;/*fps_factor*/
		}
		break;
	}
}


//
//	if (0==src->ani_turn)
//	{
//		src->anime_frame++;
//		if (3==(src->anime_frame&(4-1)))
//		{
//			src->ani_turn=1;
//		}
//	}
//	else
//	{
//		src->anime_frame--;
//		if (0==(src->anime_frame&(4-1)))
//		{
//			src->ani_turn=0;
//		}
//	}

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/
	#if 0
//
//	if ( (enemy_rank)>9) 	{enemy_rank=9;}
	signed short spr_tbl[10][4] =
	{
{/* case 0: 	s->x=*/372 GAME_WIDTH352+20-80-12(280), /*s->y=*/100,		1,/*-s->w;*/		/*s->yx_an im_frame=*/SSS20},	//�E��
{/* case 1: 	s->x=*/392 GAME_WIDTH352+40-80-12(300), /*s->y=*/70,		1,/*-s->w;*/		/*s->yx_an im_frame=*/SSS20},	//�E��
{/* case 2: 	s->x=*/412 GAME_WIDTH352+60-80-12(320), /*s->y=*/40,		1,/*-s->w;*/		/*s->yx_an im_frame=*/SSS20},	//�E��
//
{/* case 3: 	s->y=*/-30+80(50),			/*s->x=*/-300,		3,/*-s->w/2;*/		/*s->yx_an im_frame=*/SSS08},	//��E�E
{/* case 4: 	s->y=*/-50+80(30),			/*s->x=*/-220,		3,/*-s->w/2;*/		/*s->yx_an im_frame=*/SSS08},	//��E
{/* case 5: 	s->y=*/-50+80(30),			/*s->x=*/-160,		3,/*-s->w/2;*/		/*s->yx_an im_frame=*/SSS08},	//�㍶
{/* case 6: 	s->y=*/-30+80(50),			/*s->x=*/ -80,		3,/*-s->w/2;*/		/*s->yx_an im_frame=*/SSS08},	//�㍶��
//
{/* case 7: 	s->x=*/-20+80(60),			/*s->y=*/40,		0,/*		*/		/*s->yx_an im_frame=*/SSS00},	//����
{/* case 8: 	s->x=*/-40+80(40),			/*s->y=*/70,		0,/*		*/		/*s->yx_an im_frame=*/SSS00},	//����
{/* case 9: 	s->x=*/-60+80(20),			/*s->y=*/100,		0,/*		*/		/*s->yx_an im_frame=*/SSS00},	//����
	};
//	s->y256 				= (spr_tbl[enemy_rank][1]<<8);
//	s->x256 				= (spr_tbl[enemy_rank][0]<<8);
//		 if (1==spr_tbl[enemy_rank][2]) 	{	s->x256 -= ((s->w128+s->w128));}
//	else if (3==spr_tbl[enemy_rank][2]) 	{	s->x256 -= ((s->w128));}
	#endif

global void add_zako_ao_yousei2(STAGE_DATA *l)
{
	SPRITE *h;
	h							= sprite_add_gu_error();
	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
	{
		add_zako_common(l, h);
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
		h->type 				= TEKI_12_YOUSEI1_1;
	//
		h->callback_mover		= move_ao_yousei2;
	//	h->yx_an im_frame		= 0;// spr_tbl[enemy_rank][3];
		h->x256 				= ((l->user_x)<<8);
		h->y256 				= ((l->user_y)<<8);
	//
		/* �o��̓��� / �ޏ�̓��� */
	//		 if (/*h->*/enemy_rank<3)	{	h->vx256 = -t256(2.0);	}
	//	else if (/*h->*/enemy_rank<7)	{	h->vx256 =	t256(2.0);	}
	//	else								{	h->vx256 =	t256(2.0);	}
			 if (t256(64)			 > h->x256) 	{	h->vx256 =	t256(2.0);	}	/* �o���ʒu�����Ȃ�A�E�ֈړ� */
		else if (t256(GAME_WIDTH-64) < h->x256) 	{	;	}						/* ���S�t�߂Ȃ獶�E�Ɉړ����Ȃ� */
		else										{	h->vx256 = -t256(2.0);	}	/* �o���ʒu���E�Ȃ�A���ֈړ� */

		h->vy256 = (0);

		#if 1//(1==USE_X_HOUKOU)
		if (0 < (h->y256))
		{	;	}
		else
		{
			/* y���W���������̏ꍇ�́Ax���W�w��Ȃ̂�(x���W y���W ���t�ɂ���) */
			{
				int s_sss;
				s_sss				=  (h->x256);
				h->x256 			= -(h->y256);
				h->y256 			= s_sss;
			}
			h->vy256			= (h->vx256);
			h->vx256			= (0);
	//		h->AO_YOUSEI3_anime_houkou		= ((0x20)>>2);
		}
		#endif

		h->state			= STATE_00;
	//	h->enemy_rank		= enemy_rank;
		h->time_out 		= 40;
		h->nnn				= 3;	/* 3�񌂂� */
	//
	//	h->ani_turn 		= 0;
	//	h->anime_frame		= 0;// spr_tbl[enemy_rank][3];
	}
}