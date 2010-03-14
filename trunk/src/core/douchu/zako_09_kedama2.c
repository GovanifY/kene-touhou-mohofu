
#include "douchu.h"

/*---------------------------------------------------------
		"�ы�2",	"BADGUY",	(r27����V�K�ǉ�)
	-------------------------------------------------------
	�{�Ɩы�
---------------------------------------------------------*/

//#define tx256		user_data00
//#define ty256		user_data01
#define time_out	user_data02
//#define speed256	user_data03
#define state		user_data04

#define kougeki_type		user_data05

#define USE_X_HOUKOU (1)

/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/
enum
{
	ST00 = 0,
	ST02,
	ST03,
};

static void move_kedama2(SPRITE *src)
{
	src->time_out--;/*fps_factor*/
//
	switch (src->state)
	{
	case ST00:	/* �΂�΂�ɓo�ꂳ���� */
		if (
			(0 >= src->x256)
			|| (t256(GAME_WIDTH) < src->x256)
			#if (1==USE_X_HOUKOU)
			|| (0 >= src->y256)
			|| (t256(GAME_HEIGHT) < src->y256)
			#endif
		)
		{
			/*��ʊO*/;
		}
		else
		{
			src->time_out	= 64;
			src->state++;	/* �o�ꂷ�� */
		}
		break;
	case ST02:	/* �U�� */
		{			enum
			{
				K00_KOUGEKI_KANKAKU_MASK = 0,
				K01_SUKIMA_KAKUDO,
				K02_TAMA_KAZU,
				K99_MAX/* �ő吔 */
			};
			const u32 kougeki_tbl[(4*2)][K99_MAX] =
			{
				/* 0:�g���ۂ��ы�(���p�x ��e) */
				{0x3f, (u32)(512/12), (5/*+difficulty+difficulty*/)},	/* (u32)(512/12):�L�p�Ȃ̂ŊȒP */
				{0x1f, (u32)(512/16), (3/*+difficulty+difficulty*/)},	/* 0x0f:���ʂɔ������Ԋu */
				{0x0f, (u32)(512/20), (3/*+difficulty+difficulty*/)},	/* 0x0f:���ʂɔ������Ԋu */
				{0x07, (u32)(512/24), (5/*+difficulty+difficulty*/)},	/* (u32)(512/24):���ʂ̋��p */
				/* 1:�d���ۂ��ы�(90�x 4����) (���ʒe�Ȃ̂œ�Փx�͒Ⴂ��) */
				{0x3f, (u32)(512/8),  (8)}, 	/* 8:�����e(���ʒe�Ȃ̂œ�Փx�͒Ⴂ��) */
				{0x1f, (u32)(512/8),  (8)}, 	/* 0x1f:���ʂɔ������Ԋu */
				{0x07, (u32)(512/4),  (4)},
				{0x03, (u32)(512/4),  (4)}, 	/* 0x03:�A���e���ۂ� */
			};
			if (0==(src->time_out & kougeki_tbl[src->kougeki_type][K00_KOUGEKI_KANKAKU_MASK]))/*0x0f*/
			{
				send1_obj->x256 = src->x256;
				send1_obj->y256 = src->y256;
				send1_obj->BULLET_REGIST_speed256			=	(t256(2.5)+((difficulty)<<6));/* [2.5 - 3.25] */		//	(t256(2.0)+((difficulty)<<6)),/* [2.0 - 2.75] */
				send1_obj->BULLET_REGIST_angle512			=	ANGLE_JIKI_NERAI_DAN;
				send1_obj->BULLET_REGIST_div_angle512		=	kougeki_tbl[src->kougeki_type][K01_SUKIMA_KAKUDO]; 	//	(int)(512/24),
				send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_KOME_01_AOI+(src->x256&0x07);
				send1_obj->BULLET_REGIST_n_way				=	kougeki_tbl[src->kougeki_type][K02_TAMA_KAZU] ;		//	(3/*+difficulty+difficulty*/)
				bullet_regist_basic();
			}
		}
		if (0 > src->time_out)
		{
			src->state++;
		}
		break;
	case ST03:
		gamen_gai_nara_osimai(src);/* ��ʊO�Ȃ炨���܂� */
		break;
	}

	/* �ړ����� */
	src->x256 += (src->vx256);
	src->y256 += (src->vy256);
//
	src->m_angleCCW512 += 5;
	mask512(src->m_angleCCW512);
}

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

void add_zako_kedama2(STAGE_DATA *l)
{
	SPRITE *h;
	h						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
	h->type 				= TEKI_59_HAI_KEDAMA;
	add_zako_common(l, h);
//	h->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	h->color32				= 0xaaffffff;		/*�����ۂ� */
	h->callback_mover		= move_kedama2;
//	h->callback_loser		= lose_random_item;
//	h->callback_hit_enemy	= callback_hit_zako;
	//
	h->y256 				= ((l->user_y)<<8);
	h->vx256				= ((l->user_x));
	h->vy256				= (0);
	//
	{
		if (0 < (h->vx256))
		{/* [���� user_y ���������̏ꍇ�A�E�ֈړ�(��)] [����y���������̏ꍇ�A���ֈړ�(��)] */
			h->x256 			= t256(-30)/*-(i<<(5+8))*/;
		}
		else
		{/* [���� user_y ���������̏ꍇ�A���ֈړ�(��)] [����y���������̏ꍇ�A��ֈړ�(��)] */
			h->x256 			= t256(360)/*+(i<<(5+8))*/;/* 360 > 352(x_max) > 272(y_max) */
		}
	}
	//
	if (0 < (h->y256))
	{	;	}
	else
	{
		/* y���W���������̏ꍇ�́Ax���W�w��Ȃ̂�(x���W y���W ���t�ɂ���) */
		{
			int s_sss;
			s_sss				= h->x256;
			h->x256 			= -(h->y256);
			h->y256 			= s_sss;
		}
		h->vy256			= (h->vx256);
		h->vx256			= (0);
	}
	//
//	/*src->base.*/h->base_score		= score(/*50*/5*2*4)/**(enemy_rank+1)*/;
//	/*src->base.*/h->base_hp		= (8*(8/**8*/))/*+(di fficulty<<(2+3))*/ /*(1+(difficulty<<2))*/;/*���炩����*/
	h->state 			= ST00;
	h->time_out			= 64/*+(i<<4)*/;
	h->kougeki_type		= (((l->user_1_moji)&1)<<2)|(difficulty/*&0x03*/);
}



	//	s->x256 				= t256(200)+	   (ra_nd()&((64*256)-1))/*ra_nd()%40*/;
	//	s->y256 				= ((ra_nd()&((32*256)-1))-t256(80));//	(ra_nd()%40-90);
	//	s->vx256			= (0);
	//	s->vy256			= (0);
//	/*src->*/enemy_rank	= l->user_y;

//		src->tx256 			= player->x256;
//		src->ty256 			= player->y256;

