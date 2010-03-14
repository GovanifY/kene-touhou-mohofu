
#include "douchu.h"

/*---------------------------------------------------------
	"���΂�2"(�΂��΂�) 	"���ы�1",		"EYEFO",
	-------------------------------------------------------
	�o�O����H�H�H
---------------------------------------------------------*/

#define xcenter256			user_data00
#define ycenter256			user_data01
#define time_out			user_data02

#define radius256			user_data04
#define flag1				user_data05

/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_obake2(SPRITE *src)
{
	src->time_out--;/*fps_factor*/
	if (0 > src->time_out)
	{
		src->type = SP_DELETE;	/* �����܂� */
	}
	else
	if (600 > src->time_out)	/* ��֋A�� */
	{
		src->ycenter256 -= t256(1.0);/*fps_factor*/
	}
	else
	if (900 < src->time_out)	/* ���֗��� */
	{
	//	if (src->ycenter256 < t256(GAME_HEIGHT))
	//	if (src->ycenter256 < t256(difficulty<<6) )/* ��Փx�ɂ�艺�����Ă���ʒu���Ⴄ�B�ő�192(3*64) */
		if (src->ycenter256 < t256(difficulty<<5) )/* ��Փx�ɂ�艺�����Ă���ʒu���Ⴄ�B�ő�96(3*32) */
		{
			src->ycenter256 += t256(1.0);/*fps_factor*/
		}
	}
//
	if (0 == src->flag1)
	{
		src->radius256 += t256(1.0);	/*fps_factor*/
		if (src->radius256 > t256(64.0)/*150*/) /* �ő唼�a */
		{
			src->flag1=1;
		}
	}
	else
	{
		src->radius256 -= t256(2.0);	/*fps_factor*/	/*1*/
		if (src->radius256 < t256(8.0)) /* �ŏ����a */	/*<=10*/
		{
			src->flag1=0;
		}
	}
	src->tmp_angleCCW512 += (1);/*fps_factor*/	/*deg_360_to_512(5)*/	/* deg_360_to_512(5) �������� */
	mask512(src->tmp_angleCCW512);// if (src->angle>=360)	{	src->angle-=360;}
/* CCW�̏ꍇ */
	src->x256 = ((sin512((src->tmp_angleCCW512))*(src->radius256))>>8)+src->xcenter256;
	src->y256 = ((cos512((src->tmp_angleCCW512))*(src->radius256))>>8)+src->ycenter256;

	if (0 != difficulty)/* easy �̓z�[�~���O�����Ȃ� */
	{
	//	if (src->enemy_rank)
		{
		//	if (0==(ra_nd()%(1000-(src->enemy_rank)*100)))
		//	if (0==(ra_nd()%((10-(src->enemy_rank))*100)))
	//		if (0==(ra_nd()&((1<<(10-(src->enemy_rank)))-1)))/*�e�L�g�[*/	/* [10]�m��(���p�x)��������C������ */
//			if (0==(ra_nd()&((1<<(12-(src->enemy_rank)))-1)))/*�e�L�g�[*/
			if (0==(ra_nd()&((1<<(11-(3/*src->enemy_rank*/)))-1)))/*�e�L�g�[*/
			{
				/* ��ʊO�Ȃ�Ό����Ȃ� */
				if (0 < src->y256)	/* ��ʓ��Ȃ猂�� */
				{
					bullet_create_enemy_homing(src);	/* �G���̗U���~�T�C�����쐬���� */
				}
			}
		}
	}

	if (SP_DELETE != src->type)
	{
		src->type			= TEKI_28_YOUSEI2_5+(((src->y256>>8)&0x03));
	}
}

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

void add_zako_obake2(STAGE_DATA *l)
{
	SPRITE *h;
	h						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
	h->type 				= TEKI_28_YOUSEI2_5;
	add_zako_common(l, h);
	h->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	h->callback_mover		= move_obake2;
//	h->callback_loser		= lose_random_item;
//	h->callback_hit_enemy	= callback_hit_zako;

//	/*h->base.*/h->base_score	= score(30*2);
//	/*h->base.*/h->base_hp		= /*50*/ /*48*/(8*32)+(1/*di fficulty*/<<(/*2+*/3));/*10+(di fficulty<<2)*/ 	/* ���������|���鋭���ɂ��Ƃ�(�U���~�T�C���������̂�) */
	h->radius256		= t256(10);
	h->flag1			= 0;
	h->tmp_angleCCW512	= (ra_nd()&0x1ff);//(/*360*/512/N UM_OF_ENEMIES)*i;
	h->time_out 		= (1200);
	h->xcenter256		= t256( 62) + ((ra_nd()&((256*256)-1)))/*t256*/  /*(GAME_WIDTH/2)*/;/* GAME_WIDTH 380 ~= 380 == 62+256+62 */
	h->ycenter256		= t256(-32);/*-100*/
}
