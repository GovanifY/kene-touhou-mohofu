
#include "douchu.h"
/*static*/extern void bullet_create_enemy_homing(SPRITE *src);

/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	"���΂�2"(�΂��΂�) 	"���ы�1",		"EYEFO",
	-------------------------------------------------------
	�o�O����H�H�H
---------------------------------------------------------*/
#if 0/* �߂� */
/* �{�X���ʋK�i */
	#define target_x256 		user_data00 	/* �ڕWx���W */
	#define target_y256 		user_data01 	/* �ڕWy���W */
	#define vvv256				user_data02 	/* �ڕW���W�ւ̓��B���� */
	#define time_out			user_data03 	/* �������� */
#endif

#define xcenter256			user_data00
#define ycenter256			user_data01
#define time_out			user_data03

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
	src->tmp_angleCCW1024 += (2);	/*fps_factor*/	/* cv1024r(5) �������� */
	mask1024(src->tmp_angleCCW1024);
/* CCW�̏ꍇ */
	src->x256 = ((sin1024((src->tmp_angleCCW1024))*(src->radius256))>>8)+src->xcenter256;
	src->y256 = ((cos1024((src->tmp_angleCCW1024))*(src->radius256))>>8)+src->ycenter256;

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

global void add_zako_obake2(STAGE_DATA *l)
{
	SPRITE *h;
	h						= sprite_add_gu_error();
	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
	{
		add_zako_common(l, h);
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
		h->type 				= TEKI_28_YOUSEI2_5;
		h->callback_mover		= move_obake2;

		h->radius256			= t256(10);
		h->flag1				= 0;
		h->tmp_angleCCW1024 	= (ra_nd()&(1024-1));
		h->time_out 			= (1200);
		h->xcenter256			= t256( 62) + ((ra_nd()&((256*256)-1)))/*t256*/  /*(GAME_WIDTH/2)*/;/* GAME_WIDTH 380 ~= 380 == 62+256+62 */
		h->ycenter256			= t256(-32);/*-100*/
	}
}
