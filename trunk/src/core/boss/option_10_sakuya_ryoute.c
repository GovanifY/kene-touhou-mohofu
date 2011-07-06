
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�e��
	-------------------------------------------------------
---------------------------------------------------------*/

//typedef struct
//{
#define EVEN_KNIFE_DATA_x256						user_data00
#define EVEN_KNIFE_DATA_y256						user_data01
//
#define EVEN_KNIFE_DATA_d_angle1024 				user_data02
#define EVEN_KNIFE_DATA_angle1024					user_data03 	/*union BULLET_ANGLE_DATA*/
#define EVEN_KNIFE_DATA_speed256					user_data04 	/*union BULLET_ANGLE_DATA*/
//
#define EVEN_KNIFE_DATA_state						user_data05
#define EVEN_KNIFE_DATA_wait1						user_data06 	/* fire */

#define EVEN_KNIFE_DATA_l_or_r						user_data07 	/* -1==l, 1==r */
	//int time_out; // act
//	int length256;	t256(128)
//} EVEN_KNIFE_DATA;

/*---------------------------------------------------------

	-------------------------------------------------------
	s->cx256 	�i�C�t��x���W
	data->cx256	�m�[�h��x���W
	s->w/2		�i�C�t�̉����̔���
	�i�C�t��x���W == �m�[�h��x���W + co_s1024((data->angle1024)) x length - �i�C�t�̉����̔���
---------------------------------------------------------*/
#define ryoute_knife_length256 t256(128)
//static void enemy_evenr_knife_move(SPRITE *src)
static void enemy_even_knife_move(SPRITE *src)
{
	if (0== src->EVEN_KNIFE_DATA_state)
	{
		if (100 < src->EVEN_KNIFE_DATA_wait1)
		{
			src->EVEN_KNIFE_DATA_wait1 = 0;
			src->EVEN_KNIFE_DATA_state = 1;
		}
		src->EVEN_KNIFE_DATA_angle1024 -= (src->EVEN_KNIFE_DATA_d_angle1024*src->EVEN_KNIFE_DATA_l_or_r);	/*fps_factor*/
		mask1024(src->EVEN_KNIFE_DATA_angle1024);
		/* �`��p�p�x(����0�x�ō����(�����v���)) */
		src->rotationCCW1024 = (src->EVEN_KNIFE_DATA_angle1024+((int)(1024*1/4)*src->EVEN_KNIFE_DATA_l_or_r));
		mask1024(src->rotationCCW1024);/*�K���v��*/
	//	src->cx256		= data->cx256-((cos512((data->angle512))*data->length256)>>8);
	//	src->cy256		= data->cy256-((sin512((data->angle512))*data->length256)>>8);
	//	src->cx256		= data->cx256-((sin512((data->angle512))*ryoute_knife_length256/*data->length256*/)>>8);
	//	src->cy256		= data->cy256-((cos512((data->angle512))*ryoute_knife_length256/*data->length256*/)>>8);
		#if 1/*Gu(���S���W)*/
		src->cx256		= src->EVEN_KNIFE_DATA_x256-((sin1024((src->EVEN_KNIFE_DATA_angle1024)))<<7 );
		src->cy256		= src->EVEN_KNIFE_DATA_y256-((cos1024((src->EVEN_KNIFE_DATA_angle1024)))<<7 );
		#endif
	}
	else
//	if (1== src->EVEN_KNIFE_DATA_state)
	{
		if (50 < src->EVEN_KNIFE_DATA_wait1)
		{
			src->EVEN_KNIFE_DATA_wait1 = 0;
			src->EVEN_KNIFE_DATA_state = 0;
		}
		//#if 1
		//data->speed=20/*speed*/;/*�悭�킩��Ȃ����~�܂����Ⴄ*/
		//#endif
	//	mono_angle_move(src,(BULLET_ANGLE_DATA *)data);
		src->vx256 = ((sin1024((src->EVEN_KNIFE_DATA_angle1024))*src->EVEN_KNIFE_DATA_speed256)>>8);/*fps_factor*/
		src->vy256 = ((cos1024((src->EVEN_KNIFE_DATA_angle1024))*src->EVEN_KNIFE_DATA_speed256)>>8);/*fps_factor*/
		src->cx256 += (src->vx256);/*fps_factor*/
		src->cy256 += (src->vy256);/*fps_factor*/
	}
	if (10 < src->EVEN_KNIFE_DATA_wait1)
	{
		src->EVEN_KNIFE_DATA_wait1 = 0;
		#if 1
		obj_send1->cx256 = src->cx256;
		obj_send1->cy256 = src->cy256;
//
	//	br.BULLET_REGIST_div_angle1024		= (int)(1024/(18)); 		/* �_�~�[�����p�x(1way�Ȃ̂Ŏg�p���Ă��Ȃ�) */
		br.BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_04_AOI;		/* [�i�C�t�e] */
		br.BULLET_REGIST_n_way				= (1);	/* [1way] */
		br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
//
		br.BULLET_REGIST_speed256			= (t256(2.5)/*t256(5.0)*/);
		br.BULLET_REGIST_angle1024			= (src->EVEN_KNIFE_DATA_angle1024+((int)(1024*3/12)*src->EVEN_KNIFE_DATA_l_or_r));
		bullet_regist_vector();
		//
		br.BULLET_REGIST_speed256			= (t256(2.2)/*t256(4.0)*/);
		br.BULLET_REGIST_angle1024			= (src->EVEN_KNIFE_DATA_angle1024+((int)(1024*4/12)*src->EVEN_KNIFE_DATA_l_or_r));
		bullet_regist_vector();
//
		br.BULLET_REGIST_speed256			= (t256(2.0)/*t256(3.0)*/);
		br.BULLET_REGIST_angle1024			= (src->EVEN_KNIFE_DATA_angle1024+((int)(1024*5/12)*src->EVEN_KNIFE_DATA_l_or_r));
		bullet_regist_vector();
//
		if (1<difficulty)
		{
			br.BULLET_REGIST_speed256		= (t256(1.8)/*t256(2.0)*/);
			br.BULLET_REGIST_angle1024		= (src->EVEN_KNIFE_DATA_angle1024+((int)(1024*6/12)*src->EVEN_KNIFE_DATA_l_or_r));
			bullet_regist_vector();
		}
		#endif
	}
	src->EVEN_KNIFE_DATA_wait1++;
	/*data->*/src->base_time_out--;
	if ( /*data->*/src->base_time_out < 0 )
	{
		src->jyumyou = JYUMYOU_NASI;
	}
}


/*---------------------------------------------------------
	����i�C�t(��)
	-------------------------------------------------------
	length	enemy-obj_player�Ԃ̋���/��2
	r_or_l	0==�E	1==��
---------------------------------------------------------*/

global void bullet_create_sakuya_ryoute_knife(SPRITE *src)
//, int speed256/*, int dummy_length256*/, int r_or_l)/*0==�E*/ 	/*1==��*/
{
	int speed256;
	speed256	= t256(10);
	int r_or_l;
//	for (r_or_l=0; r_or_l<2; r_or_l++)
	for (r_or_l=-1; r_or_l<2; r_or_l+=2)
	{
		SPRITE *h;
		h					= sprite_add_444only_bullet_error();/*�S�����i�C�t(��)*/ /*h->use_alpha=0 ????*/
		if (NULL != h)
		{
			h->type 			= BULLET_KNIFE20_04_AOI;/*S P_BULLET*/ /*SP_LASER*/
			reflect_sprite_spec444(h, OBJ_BANK_SIZE_00_TAMA);
			h->m_Hit256R		= TAMA_ATARI_KNIFE18_PNG;
		//
			#if 1/*Gu(���S���W)*/
			h->cx256 			= src->cx256;
			h->cy256 			= src->cy256;
			#endif
			/*data->*/h->base_time_out		= (150+difficulty*50);
		//
			tmp_angleCCW1024_jiki_nerai(src);/*???(original)*/ /* ��e�̏ꍇ�Ɏ��@�_�� */
			int tmp1024;
			tmp1024 = src->tmp_angleCCW1024;
			h->callback_mover	= enemy_even_knife_move;
			#if 0
		//	if (1==r_or_l)	/* psp��0���W�X�^������̂�0�Ɣ�r�����ق������� */
			if (0!=r_or_l)
			{
		//		h->callback_mover	= enemy_evenl_knife_move;
				tmp1024 += (128)/*1024*1/8*/;
				data->l_or_r			= (-1);
			}
			else
		//	if (0==r_or_l)
			{
		//		h->callback_mover	= enemy_evenr_knife_move;
				tmp1024 -= (128)/*1024*1/8*/;
				data->l_or_r			= (1);
			}
			#else
			h->EVEN_KNIFE_DATA_l_or_r			= r_or_l;
			tmp1024 -= (128*r_or_l)/*1024*1/8*/;
			#endif
			mask1024(tmp1024);
		//	if (0==length256)	{length256=1*256;}
		//	data->length256 	= length256;
		//	data->cx256			= /*s->cx256*/h->cx256+((cos1024((tmp1024))*length256)>>8);
		//	data->cy256			= /*s->cy256*/h->cy256+((sin1024((tmp1024))*length256)>>8);
		//	data->cx256			= /*s->cx256*/h->cx256+((sin1024((tmp1024))*ryoute_knife_length256/*length256*/)>>8);
		//	data->cy256			= /*s->cy256*/h->cy256+((cos1024((tmp1024))*ryoute_knife_length256/*length256*/)>>8);
			h->EVEN_KNIFE_DATA_x256 		= /*s->cx256*/h->cx256+((sin1024((tmp1024)) )<<7 );
			h->EVEN_KNIFE_DATA_y256 		= /*s->cy256*/h->cy256+((cos1024((tmp1024)) )<<7 );
			h->EVEN_KNIFE_DATA_angle1024	= ((tmp1024));
		//	h->EVEN_KNIFE_DATA_d_angle1024	= ra d2deg1024(speed/(M_PI*2*length));	//��angle���C��/(2�΁~length/��2)
		//	int tmp_1024;
		//	tmp_1024 = speed/(/*1024*/1024*length); 	// ��angle �� �C��/(2�΁~length/��2)
			h->EVEN_KNIFE_DATA_d_angle1024	= (1+1);//	  /*ra d2deg1024*/(/*deg1024_2rad*/(tmp_1024));
		//
			h->EVEN_KNIFE_DATA_speed256 	= /*10*/t256(10)/*8+difficulty*/ /*speed*/;/*�悭�킩��Ȃ����~�܂����Ⴄ*/
			h->EVEN_KNIFE_DATA_state		= 0;
			h->EVEN_KNIFE_DATA_wait1		= 0;
		}
	}
}
