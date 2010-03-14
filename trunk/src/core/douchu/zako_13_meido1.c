
#include "douchu.h"

/*---------------------------------------------------------
	"���C�h1",		"RWINGX",	rwingx
	-------------------------------------------------------
	�ォ�牡�ɓ��Ԋu�Ő��񂵂Č���A�v���C���[�Ɍ�����
	������x�߂Â��ƁA�e�u���Ĕ��]���ē�����B
---------------------------------------------------------*/

#define time_out	user_data02
#define speed256	user_data03

/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_meido1(SPRITE *src)
{
	src->time_out--;/*fps_factor*/
	if (0 > src->time_out)
	{
		gamen_gai_nara_osimai(src);/* ��ʊO�Ȃ炨���܂� */
	}
	else
	if (1024 > src->time_out)
	{
		src->speed256	+= t256(0.1);/*�������Ȃ��� �v���C���[�ɓ˂�����ł���*/
		if (src->y256 >= t256(GAME_HEIGHT-150))
		{
		//	if (src->enemy_rank)
			{
				bullet_create_aka_maru_jikinerai(src,
					t256(2.8) // //t256(1.0)+t256(src->enemy_rank)/*t256(3)+t256(src->enemy_rank)*/
				);
			}/*���߂�*/
			src->time_out = 1023;
			src->tmp_angleCCW512 += deg_360_to_512CCW(180/*360-180*/);/* ���]���ē����� */
			mask512(src->tmp_angleCCW512);// src->angle %= deg_360_to_512(360);
		//	src->speed256 = /*t256(6+src->enemy_rank)*/;/*���߂�*/
		}
	}
	else
	if (2048 > src->time_out)
	{
		/* �v���C���[�ɓ˂�����ł��� / schnell richtung player */
		src->time_out = 2047;
		src->tmp_angleCCW512	= atan_512(player->y256-src->y256,player->x256-src->x256);
	//	src->speed256	= /*t256(3+(src->enemy_rank>>1))*/ /*(4+src->enemy_rank)*/;/*���߂�*/
	}
	else
	if (2049 > src->time_out)/* 2048+1 */
	{
		if (0 < src->speed256)
		{
			src->speed256 -= 5/*t256(0.1)*/;/*����*/
		}
	}
	else
	if (2109 > src->time_out)/* 2048+1+60 */
	{
		if ( src->speed256 < t256(1.0))
		{
			src->speed256 += 5/*t256(0.1)*/;/*����*/
		}
	}
//
	/*�ȉ�rwingx.c�Ɠ���*/
/* CCW�̏ꍇ */
	src->x256 += ((sin512((src->tmp_angleCCW512))*src->speed256)>>8);/*fps_factor*/
	src->y256 += ((cos512((src->tmp_angleCCW512))*src->speed256)>>8);/*fps_factor*/
//	src->an im_frame=(deg_512_to_360(src->angle512+deg_360_to_512(270))/10)%36;
//	src->an im_frame = ((((src->angle512+deg_360_to_512(270))&(512-1))*(36/2))>>8);
#if 00
/* [CCW�̏ꍇ(�V)] CW�̏ꍇ */
	src->an im_frame = ((((src->tmp_angleCCW512)&(512-1)))>>6);/*"rw ingx8.png"*/
	/* �� */
//#else
/* CCW�̏ꍇ */
	/* �V(�܂�����ĂȂ�) */
	//src->an im_frame = ((((src->tmp_angleCCW512)&(512-1)))>>6);/*"rw ingx8.png"*/
	/*��������݊�*/
	{int aaa512;
		aaa512 = 128+ 512 - src->tmp_angleCCW512;
		mask512(aaa512);
		src->yx_an im_frame = (((aaa512))>>(6));
	}
#endif
	if (SP_DELETE != src->type)
	{
		src->type			= TEKI_36_YOUSEI3_1+((((src->tmp_angleCCW512)&(512-1)))>>6);
	}
}

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

void add_zako_meido1(STAGE_DATA *l)
{
	SPRITE *h;
	h						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
	add_zako_common(l, h);
	h->type 				= TEKI_36_YOUSEI3_1;	/*SP_ZAKO*/ /*_12_MEIDO1*/
//	h->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	h->callback_mover		= move_meido1;
//	h->callback_loser		= lose_random_item;
//	h->callback_hit_enemy	= callback_hit_zako;
//
	h->tmp_angleCCW512		= deg_360_to_512CCW(0);/* ������ */ 	/* CCW�̏ꍇ */
//
	h->speed256 			= t256(0);//t256(0.5);
	h->time_out 			= (2209);//(100);//20;
}
//	/*h->base.*/h->base_score	= score(25*2)*(1+1/*enemy_rank*/);
//	/*h->base.*/h->base_hp		= (8*1)+/*(8*enemy_rank)+*/(1/*di fficulty*/<<(2+3));
//	h->x256 				= t256(GAME_WIDTH/8)*i+t256(10/*-20*/);
//	h->y256 				= t256(-32)/*-50*/;
