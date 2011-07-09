
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	��� ����i�C�t�e��
	-------------------------------------------------------
---------------------------------------------------------*/

//typedef struct
//{
#define target_x256 					user_data00
#define target_y256 					user_data01
//
#define RYOUTE_KNIFE_DATA_l_or_r		user_data02 	/* -1==l, 1==r */
#define RYOUTE_KNIFE_DATA_angle1024 	user_data03 	/*union BULLET_ANGLE_DATA*/
#define RYOUTE_KNIFE_DATA_wait1 		user_data04 	/* fire */
	//int time_out; // act
//	int length256;	t256(128)
//} RYOUTE_KNIFE_DATA;


/*---------------------------------------------------------
	1�񕪁A�i�C�t�e�����B
	-------------------------------------------------------
---------------------------------------------------------*/

static void knife_shot(SPRITE *src)
{
	send1_xy(src);	/* �e��x256 y256 ���S���甭�e�B */
//	br.BULLET_REGIST_03_VECTOR_regist_type		= VEC TOR_REGIST_TYPE_00_MULTI_VECTOR;
	br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KNIFE20_04_AOI;		/* [�i�C�t�e] */
	br.BULLET_REGIST_06_n_way					= (1);	/* [1way] */
//	br.BULLET_REGIST_07_VECTOR_div_angle1024	= (int)(1024/(18)); 			/* �_�~�[�����p�x(1way�Ȃ̂Ŏg�p���Ă��Ȃ�) */
	{
		int kk_speed256;
		kk_speed256 = (t256(2.6));
		int jj;
		for (jj=0; jj<4; jj++)
		{
			kk_speed256 -= (t256(0.2));
			br.BULLET_REGIST_00_speed256			= (kk_speed256);
			const int jj_bbb[4] =
			{
				(int)(1024*3/12),
				(int)(1024*4/12),
				(int)(1024*5/12),
				(int)(1024*6/12),
			};
			br.BULLET_REGIST_02_VECTOR_angle1024			= (src->RYOUTE_KNIFE_DATA_angle1024+((int)(jj_bbb[jj])*src->RYOUTE_KNIFE_DATA_l_or_r));
			bullet_regist_multi_vector_direct();
		}
	}
}
/*---------------------------------------------------------

	-------------------------------------------------------
	s->cx256	�i�C�t��x���W
	data->cx256 �m�[�h��x���W
	s->w/2		�i�C�t�̉����̔���
	�i�C�t��x���W == �m�[�h��x���W + co_s1024((data->angle1024)) x length - �i�C�t�̉����̔���
---------------------------------------------------------*/
#define ryoute_knife_length256 t256(128)

static void sakuya_ryoute_knife_move(SPRITE *src)
{
	{
		//	h->RYOUTE_KNIFE_DATA_d_angle1024	= ra d2deg1024(speed/(M_PI*2*length));	//��angle���C��/(2�΁~length/��2)
		//	int tmp_1024;
		//	tmp_1024 = speed/(/*1024*/1024*length); 	// ��angle �� �C��/(2�΁~length/��2)
		//	h->RYOUTE_KNIFE_DATA_d_angle1024	= (1+1);//	  /*ra d2deg1024*/(/*deg1024_2rad*/(tmp_1024));
		src->RYOUTE_KNIFE_DATA_angle1024 -= ((1+1)*src->RYOUTE_KNIFE_DATA_l_or_r);	/*fps_factor*/
		mask1024(src->RYOUTE_KNIFE_DATA_angle1024);
		/* �`��p�p�x(����0�x�ō����(�����v���)) */
		src->rotationCCW1024 = (src->RYOUTE_KNIFE_DATA_angle1024+((int)(1024*1/4)*src->RYOUTE_KNIFE_DATA_l_or_r));
		mask1024(src->rotationCCW1024);/*�K���v��*/
	//	src->cx256		= data->cx256-((cos512((data->angle512))*data->length256)>>8);
	//	src->cy256		= data->cy256-((sin512((data->angle512))*data->length256)>>8);
	//	src->cx256		= data->cx256-((sin512((data->angle512))*ryoute_knife_length256/*data->length256*/)>>8);
	//	src->cy256		= data->cy256-((cos512((data->angle512))*ryoute_knife_length256/*data->length256*/)>>8);
		#if 1/*Gu(���S���W)*/
		src->cx256		= src->target_x256 - ((sin1024((src->RYOUTE_KNIFE_DATA_angle1024)))<<7 );
		src->cy256		= src->target_y256 - ((cos1024((src->RYOUTE_KNIFE_DATA_angle1024)))<<7 );
		#endif
	}
	src->RYOUTE_KNIFE_DATA_wait1++;
	if (10 < src->RYOUTE_KNIFE_DATA_wait1)
	{
		src->RYOUTE_KNIFE_DATA_wait1 = 0;
		knife_shot(src);
	}
}


/*---------------------------------------------------------
	����i�C�t(��)
	-------------------------------------------------------
	length	teki-obj_player�Ԃ̋���/��2
	r_or_l	0==�E	1==��
---------------------------------------------------------*/

global void bullet_create_sakuya_ryoute_knife(SPRITE *src)
{
	int r_or_l;
//	for (r_or_l=0; r_or_l<2; r_or_l++)
	for (r_or_l=-1; r_or_l<2; r_or_l+=2)
	{
		SPRITE *h;
		h					= obj_add_00_tama_error();/*�S�����i�C�t(��)*/ /*h->use_alpha=0 ????*/
		if (NULL != h)
		{
			h->type 			= BULLET_KNIFE20_04_AOI;/*S P_BULLET*/ /*SP_LASER*/
			reflect_sprite_spec444(h, OBJ_BANK_SIZE_00_TAMA);
			h->m_Hit256R		= TAMA_ATARI_KNIFE18_PNG;
			h->callback_mover	= sakuya_ryoute_knife_move;
		//
			tmp_angleCCW65536_jiki_nerai(src);
			h->RYOUTE_KNIFE_DATA_l_or_r 		= r_or_l;
			int tmp1024;
			tmp1024 = ((src->tmp_angleCCW65536)>>6);
			tmp1024 -= (128*r_or_l)/*1024*1/8*/;
			mask1024(tmp1024);
		//	if (0==length256)	{length256=1*256;}
		//	data->length256 	= length256;
			h->cx256			= (src->cx256);
			h->cy256			= (src->cy256);
		//	data->cx256 		= h->cx256 + ((sin1024((tmp1024))*ryoute_knife_length256/*length256*/)>>8);
		//	data->cy256 		= h->cy256 + ((cos1024((tmp1024))*ryoute_knife_length256/*length256*/)>>8);
			h->target_x256		= h->cx256 + ((sin1024((tmp1024)) )<<7 );
			h->target_y256		= h->cy256 + ((cos1024((tmp1024)) )<<7 );
			h->RYOUTE_KNIFE_DATA_angle1024	= ((tmp1024));
		//
			h->RYOUTE_KNIFE_DATA_wait1		= 0;
			h->jyumyou						= (128+(cg_game_difficulty<<6));/*(r33)*/
		}
	}
}
