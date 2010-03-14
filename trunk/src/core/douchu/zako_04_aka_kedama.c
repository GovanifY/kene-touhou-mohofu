
#include "douchu.h"

/*---------------------------------------------------------
		"�Ԗы�1",		"MINE",
	-------------------------------------------------------
	���邮�邻�̏�œ����Ȃ��ŁA��]�A�j�����Ă�
	�ق��Ƃ���8�ɕ��􂵂�
	�Ō�ɍ����e�����ė�����(��)
---------------------------------------------------------*/

#define time_out	user_data02

/*---------------------------------------------------------
	�G�ړ�(�����)
---------------------------------------------------------*/

static void move_aka_kedama1_2nd(SPRITE *src)
{
	{
		/*
		0  0/100   0/65536
		1  1/90  728/65536
		2  1/80  819/65536
		3  1/70  936/65536
		*/
		static const u16 kakuritu_tbl[4] =
		{ 0, 728, 819, 936 };
	//	if (0==(ra_nd()%(100-/*data->*/enemy_rank*10)))
		if (kakuritu_tbl[/*data->*/(difficulty)/*(enemy_rank)*/] > (ra_nd()&(65536-1)))
		{
			bullet_create_aka_maru_jikinerai(src, t256(2.5)+(/*data->*/0/*(enemy_rank<<6)*/) );/*�����e*/	/*t256(3+data->enemy_rank)*/
		}
	}
/* CCW�̏ꍇ */
	src->x256 += (src->vx256);/*fps_factor*/
	src->y256 += (src->vy256);/*fps_factor*/
	gamen_gai_nara_osimai(src);/* ��ʊO�Ȃ炨���܂� */
//
	src->m_angleCCW512 += (8);	/*(10)*/
	mask512(src->m_angleCCW512);
}


/*---------------------------------------------------------
	�G��ǉ�����(�ϐg)
---------------------------------------------------------*/

static void add_local_aka_kedama1_2nd(SPRITE *src/*, int lv*/)
{
	int i;
	for (i=0; i<8; i++)/*����*/
	{
		SPRITE *h;
		h						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
		h->type 				= TEKI_56_CHEN;
		h->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		h->callback_mover		= move_aka_kedama1_2nd;
		h->callback_loser		= lose_random_item; 	/* �����������ق����A�C�e�����҂��� */
		h->callback_hit_enemy	= callback_hit_zako;
		h->x256 				= src->x256;
		h->y256 				= src->y256;
		/*data->base.*/h->base_score		= src->base_score;		/* �����������ق����X�R�A���҂��� */		//score(10*2)*(1+(2/*di fficulty*/));	/*data->*/	/*(enemy_rank)*/	/*9*/
		/*data->base.*/h->base_hp			= (8*(1+ 9))+(1/*di fficulty*/<<(4+3)); 			/*data->*/	/*(enemy_rank)*/
		{
		int data_zzz_angle512;	/* ����p�x */
			data_zzz_angle512		= (i<<6);//  (/*360*/512/8)*i;
		int data_zzz_speed; 	/* �������x */
			data_zzz_speed		= (t256(1.5)+(difficulty<<6));	/*(3.0)*/
		//
			h->vx256 = ((sin512((data_zzz_angle512))*(data_zzz_speed))>>8);/*fps_factor*/
			h->vy256 = ((cos512((data_zzz_angle512))*(data_zzz_speed))>>8);/*fps_factor*/
		}
	}
}


/*---------------------------------------------------------
	�G�ړ�(�{��)
---------------------------------------------------------*/

static void move_aka_kedama1_1st(SPRITE *src)
{
	src->time_out--;
	if (0 > src->time_out)	/* ���� */
	{
		add_local_aka_kedama1_2nd(src/*,data->enemy_rank*/);/* �ϐg ���� */
		src->type = SP_DELETE;	/* �{�̂͂����܂� */
	}
	else
	if (31 > src->time_out)
	{
		;						/* �~�܂� */
	}
	else
	{
								/* �ړ� move */
		src->x256 += (src->vx256);/*fps_factor*/
		src->y256 += (src->vy256);/*fps_factor*/
		//
		src->m_angleCCW512 += 5;	/* ��] */
		mask512(src->m_angleCCW512);
	}
}


/*---------------------------------------------------------
	�G��ǉ�����(�{��)
---------------------------------------------------------*/

void add_zako_aka_kedama1(STAGE_DATA *l)
{
	SPRITE *h;
	h						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
	h->type 				= TEKI_56_CHEN;
	add_zako_common(l, h);
//
	h->callback_mover		= move_aka_kedama1_1st;
//	h->callback_loser		= lose_random_item;
//	h->callback_hit_enemy	= callback_hit_zako;
//		/* �����ʒu */
	{
	int data_tx256;
	int data_ty256;
		data_tx256			= ((ra_nd()&((256*256)-1)))+t256(64);/*320?*/ /*t256(ra_nd()%270)+t256(50)*/
	//	data_ty256			= ((ra_nd()&((256*256)-1)))+t256(16);/*350?*/ /*t256(ra_nd()%300)+t256(50)*/
		data_ty256			= ((ra_nd()&((256*128)-1)))+t256(16);/*350?*/ /*t256(ra_nd()%300)+t256(50)*/
	int angle512;
		angle512			= atan_512(data_ty256-h->y256,data_tx256-h->x256);
/* CCW�̏ꍇ */
		h->vx256			= ((sin512(angle512)*t256(1.5))>>8);/*fps_factor*/
		h->vy256			= ((cos512(angle512)*t256(1.5))>>8);/*fps_factor*/
	}
//
	h->time_out 			= 80+(1*60);
}
