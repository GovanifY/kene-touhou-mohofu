
#include "bullet_object.h"

/*---------------------------------------------------------
		"�d��2",		"SPLASH",
	-------------------------------------------------------

---------------------------------------------------------*/

typedef struct
{
//	ENEMY_BASE base;
	int state;			/* �d���̏�� */
	int enemy_rank; 	/* �ݒ�t�@�C������̓G�̋��� */
	int time_out;		/* ��ԑJ�ڗp�̎��Ԑ؂� */
	int nnn;			/* �e������ */
//
	int ani_turn;		/* �A�j���[�V��������(�H�̏㉺) */
	int anime_frame;	/* �A�j���[�V���� */
} AO_YOUSEI2_DATA;

/*---------------------------------------------------------
	�G����
---------------------------------------------------------*/

static void lose_ao_yousei2(SPRITE *src)
{
//	case SP_ZAKO_17_AO_YOUSEI2:
	item_create(src, ((SP_ITEM_05_TENSU&0xff)|SP_ITEM_00_P001), 1, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY)/*(up_flags)*/ );
}

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

#define SSS00 ( 0<<2)/*�E*/
#define SSS04 ( 4<<2)/*�E�΂߉�*/
#define SSS08 ( 8<<2)/*���P*/
#define SSS12 (12<<2)/*���Q*/
#define SSS16 (16<<2)/*���΂߉�*/
#define SSS20 (20<<2)/*��*/
static void move_ao_yousei2(SPRITE *src)
{
	AO_YOUSEI2_DATA *data = (AO_YOUSEI2_DATA *)src->data;
	int p512;
//
	data->time_out--;	/* ��Ɏ��Ԍo�� */
	switch (data->state)
	{
	case STATE_00:	/* �o�� */
		if (data->time_out < 0)
		{
			data->time_out = 10;
			data->state++;// = STATE_01;/*����*/
		}
		else
		{
			/* �o��̓��� */
			src->x256 += src->vx256;/**fps_fa ctor*/
			src->y256 += src->vy256;/**fps_fa ctor*/
		}
		break;
	case STATE_01:	/* �����҂� */
		if (data->time_out < 0)
		{
			data->time_out = 50;
			data->state++;// = STATE_02;/*����*/
		}
		else
		{
			p512=atan_512(player->y256-src->y256,player->x256-src->x256);
			mask512(p512);
		//	const Uint8 aa_offs[8] = { OF_16, OF_12, OF_08, OF_04, OF_00, OF_04, OF_08, OF_12 };
			const Uint8 aa_offs[8] = { SSS00, SSS04, SSS08, SSS16, SSS20, SSS16, SSS08, SSS04 };
			data->anime_frame = (data->anime_frame&(4-1))+aa_offs[(p512>>6)];		/* 64  32== 512/16 */
		}
		break;
	case STATE_02:	/* �e������ */
		bullet_create_offset_dan_type000(src, /*((1+difficulty)<<7)*/t256(2.5)/*t256(3)*/, ANGLE_JIKI_NERAI_DAN/*, t256(0), t256(0)*/, BULLET_HARI32_00_AOI);
		bullet_create_offset_dan_type000(src, /*((1+difficulty)<<8)*/t256(3.5)/*t256(4)*/, ANGLE_JIKI_NERAI_DAN/*, t256(0), t256(0)*/, BULLET_HARI32_00_AOI);

		p512=atan_512(player->y256-src->y256+((player->h128)),player->x256-src->x256-((player->w128)));
		if (difficulty>0)
		{
		int k;
			for (k=0; k<4; k++)
			{
				/*bullet_create_maru8_frame*/bullet_create_offset_dan_type000(
					src,			/*	*/
					t256(2.5)+(k<<7),			/* �e�� */
					((p512))+(1-((k&(2-1)/*%2*/)*2))*((512*1/12)),			/* ���˒��S�p�x / ����@�\(���@�_��/��) */
					(BULLET_MARU8_00_AKA+(/*0*/7-k/*�Ƃ肠����*/))	/* �e�O�� */
				);
/*(2+(((k+difficulty)>>1)))*/  /*(3+((k>>1)))*/

/*
  3+((k>>1))
0 4 3
1 4 4
2 4 4
3 4 5
*/
				if (difficulty > 1)
				{
					/*bullet_create_maru8_frame*/bullet_create_offset_dan_type000(
						src,			/*	*/
						t256(1.5)+(k<<7),			/* �e�� */
						(/**/(p512))+(1-((k&(2-1)/*%2*/)*2))*((512*1/24)),			/* ���˒��S�p�x / ����@�\(���@�_��/��) */
						(BULLET_MARU8_00_AKA+(/*0*/3-k/*�Ƃ肠����*/))	/* �e�O�� */
					);
/*(1+(((k+difficulty)>>1)))*/  /*(2+((k>>1)))*/
				}
			}
		}

		data->nnn--;
		if (data->nnn < 0)
		{
			/* �ޏꏀ�� */
				 if (data->enemy_rank<3)	{	data->anime_frame=(data->anime_frame&(4-1))+SSS00;}
			else if (data->enemy_rank<7)	{	data->anime_frame=(data->anime_frame&(4-1))+SSS08;}
			else							{	data->anime_frame=(data->anime_frame&(4-1))+SSS00;}
			data->state++;// = STATE_03;/*����*/
			data->time_out = 50;
		}
		else
		{
			data->state--;// = STATE_01;/*�߂�*/
		}
		break;
	case STATE_03:	/* �ޏ� */
		if (data->time_out < 0)
		{
			src->type = SP_DELETE;	/* �����܂� */
		}
		else
		{
			/* �ޏ�̓��� */
			src->x256 -= src->vx256;/**fps_fa ctor*/
			src->y256 -= src->vy256;/**fps_fa ctor*/
		}
		break;
	}

	if (0==data->ani_turn)
	{
		data->anime_frame++;
		if (3==(data->anime_frame&(4-1)))
		{
			data->ani_turn=1;
		}
	}
	else
	{
		data->anime_frame--;
		if (0==(data->anime_frame&(4-1)))
		{
			data->ani_turn=0;
		}
	}
}

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/
	#if 0
//
//	if ( (enemy_rank)>9) {enemy_rank=9;}
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
//		 if (1==spr_tbl[enemy_rank][2]) {s->x256 -= ((s->w128+s->w128));}
//	else if (3==spr_tbl[enemy_rank][2]) {s->x256 -= ((s->w128));}
	#endif

void add_zako_ao_yousei2(STAGE_DATA *l)/*int lv*/
{
	int enemy_rank; 	enemy_rank	= (l->user_1_moji&0x0f)/*user_y*/;
	SPRITE *s;
//	s						= sp rite_add_res(BASE_AO_YOUSEI24_PNG); //s->anim_speed=0;/*20"sp lash.png"*/
	s						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
	s->type 				= /*SP_ZAKO*/TEKI_16_YOUSEI11/*_17_AO_YOUSEI2*/;
	s->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	s->callback_mover		= move_ao_yousei2;
	s->callback_loser		= lose_ao_yousei2;
	s->callback_hit_enemy	= callback_hit_zako;
//	s->yx_an im_frame		= 0;// spr_tbl[enemy_rank][3];
	s->x256 				= ((l->user_x)<<8);
	s->y256 				= ((l->user_y)<<8);
//
	/* �o��̓��� / �ޏ�̓��� */
		 if (/*data->*/enemy_rank<3)	{	s->vx256 = -t256(2.0);	}
//	else if (/*data->*/enemy_rank<7)	{	s->vx256 =	t256(2.0);	}
	else								{	s->vx256 =	t256(2.0);	}
	s->vy256 = (0);

	#if 1//(1==USE_X_HOUKOU)
	if (0 < (s->y256))
	{	;	}
	else
	{
		/* y���W���������̏ꍇ�́Ax���W�w��Ȃ̂�(x���W y���W ���t�ɂ���) */
		{
			int s_sss;
			s_sss				= s->x256;
			s->x256 			= -(s->y256);
			s->y256 			= s_sss;
		}
		s->vy256			= (s->vx256);
		s->vx256			= (0);
//		s->AO_YOUSEI3_anime_houkou		= ((0x20)>>2);
	}
	#endif

	AO_YOUSEI2_DATA *data;
	data					= mmalloc(sizeof(AO_YOUSEI2_DATA));
	s->data 				= data;
	/*data->base.*/s->base_score		= score(50*2);
	/*data->base.*/s->base_health		= 20+(difficulty<<2);
	data->state 			= STATE_00;
	data->enemy_rank		= enemy_rank;
	data->time_out			= 40;
	data->nnn				= 3;	/* 3�񌂂� */
//
	data->ani_turn			= 0;
	data->anime_frame		= 0;// spr_tbl[enemy_rank][3];

}
