
#include "douchu.h"

/*---------------------------------------------------------
	�����͕핗  �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
		"����1",		"PROBALL",
	-------------------------------------------------------
	-------------------------------------------------------
��	��	 ��
��. ��	/��
�� .�� / ��
��	��/  ��
����++����
   /  .
---------------------------------------------------------*/
#if 0/* �߂� */
/* �{�X���ʋK�i */
	#define target_x256 		user_data00 	/* �ڕWx���W */
	#define target_y256 		user_data01 	/* �ڕWy���W */
	#define vvv256				user_data02 	/* �ڕW���W�ւ̓��B���� */
	#define time_out			user_data03 	/* �������� */
#endif

#define state			user_data04 	/* ��� */
#define speed256		user_data05 	/* ���x */
#define kaiten_houkou	user_data06 	/* ��]���� */
#define px256			user_data07 	/* ���zx�ʒu(���]�p) */


/*---------------------------------------------------------
	�G����
---------------------------------------------------------*/

//static void lose_tatsumaki1(SPRITE *src)
//{
//	destoroy++;
//	if ( (N UM_OF_ENEMIES-1) < destoroy/*all_destoroy*/)
//	{
//		destoroy = 0;
//		if (rand_percent(50/*10*/))
//		{
//			item_create(/*zzz*/ src, (SP_ITEM_04_BOMB), 1, ITEM_MOVE_FLAG_06_RAND_XY);
//		}
//	}
//}
/*50%(SP_ITEM_01_P008 or SP_ITEM_04_BOMB) (SP_ITEM_01_P008+(ra_nd()&1)) */
//66%==SP_ITEM_04_BOMB or 33%==SP_ITEM_01_P008 (SP_ITEM_EXTRA_HOMING+(ra_nd()&3/*%3*/)),

/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/
enum
{
	SS00 = 0,
	SS01,
	SS02,
};
static void move_tatsumaki1(SPRITE *src)
{
	src->speed256++;	/* t256(0.1) t256(0.2)������ */
	switch (src->state)
	{
	case SS00:/* ���ֈړ� */
		src->y256 += t256(1.0);/*fps_factor*/
		if (t256(GAME_HEIGHT/3/*2*/) < src->y256 )	/*t256(250)*/
		{
			src->state++;/* = SS01;*/
		}
		break;
	case SS01:/* ��ֈړ� */
		src->y256 -= t256(1.125);/*fps_factor*/
		if (0 > src->y256 )
		{
		//	src->speed256 = (-(src->speed256));/* ���] */
			src->state++;/*  = SS02;*/
		}
		break;
	case SS02:/* ���ֈړ� */
		src->y256 += t256(1.25);/*fps_factor*/
		gamen_gai_nara_zako_osimai(src);/* ��ʊO�Ȃ炨���܂� */
		break;
	}
	src->px256 -= (src->speed256);/*fps_factor*/
	if ((0 > src->px256)||(t256(GAME_WIDTH-10) < src->px256))
	{
		src->speed256 = (-(src->speed256));/* ���] */
	}
//
	if (0 == src->kaiten_houkou)
			{	src->m_angleCCW1024 += 10;	src->x256	= (src->px256); 	}
	else	{	src->m_angleCCW1024 -= 10;	src->x256	= t256(GAME_WIDTH)-(src->px256); }
	mask1024(src->m_angleCCW1024);
}


/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

global void add_zako_tatsumaki1(STAGE_DATA *l)
{
	SPRITE *h;
	h							= sprite_add_gu_error();
	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
	{
		add_zako_common(l, h);
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
		h->type 				= TEKI_62_TATSUMAKI;
		h->callback_mover		= move_tatsumaki1;
	//	h->callback_loser		= lose_tatsumaki1;/*old*/
	//	h->m_angleCCW1024		= ((i&(16-1))<<6);
	//
		h->state				= SS00;
		h->speed256 			= t256(1.2);/* t256(2.0)������ */	/*3+difficulty+lv/3*/
		h->px256				= t256(GAME_WIDTH-10);	/*0*/	/*t256(GAME_WIDTH/2)-((s->w128))*/
		/* ��ʉE���̏ꍇ�A�������֐i�� */
		if ( t256((GAME_WIDTH)/2) < h->x256)
		{
			h->kaiten_houkou	= (1);	/*������*/
		}
		else
		{
			h->kaiten_houkou	= (0);
		}
	}
}
