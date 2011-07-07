
#include "game_main.h"
#include "spell_card_value.h"/*�X�y�J���ꍇ�ɕK�v*/

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�p�`�F�l�`
	-------------------------------------------------------

---------------------------------------------------------*/
#if 0/*����*/
/* �{�X���ʋK�i */
	#define target_x256 		user_data00 	/* �ڕWx���W */
	#define target_y256 		user_data01 	/* �ڕWy���W */
	#define vvv256				user_data02 	/* �ڕW���W�ւ̓��B���� */
	#define boss_time_out		user_data03 	/* �������� */
#endif
//	�{�X���ʋK�i�Ɠ���(boss.h�C���N���[�h���Ă����Ȃ��Ă��Ή�)
#ifndef boss_time_out
	#define boss_time_out		user_data03 	/* �������� */
#endif
//
//----[ZAKO]
#define DOLL_DATA_fix_angle1024 	user_data07 	/* �Ă��ƌŒ�l */

//------------ doll02��p

#define DOLL_DATA_state222			user_data04 	/* �`�� */
#define DOLL_DATA_bwait 			user_data05 	/* �e���˂܂ł̑҂����ԁB */
#define shot_angle1024				user_data06 	/* �e�̊p�x */

/*---------------------------------------------------------
	�G����
---------------------------------------------------------*/

static void lose_doll(SPRITE *src)
{
	item_create_for_boss(src, ITEM_CREATE_MODE_02);
//
	/* ���i�̏����𑬂�(R4000�n�����ӂ�0��r�𑽗p����)���āA������(�ő��ɗ��Ȃ��̂�)�x�����܂� */
//	com mon_boss_flags &= (~(src->DOLL_DATA_identity_bit));
}

/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static int bb_angle1024;
static int cc1024;
static void move_doll01(SPRITE *src)
{
	check_boss_option_time_out(src);	/* ���Ԍo�߂ŏI���B�{�X��|���ƊF�j�󂳂��B */
//
	#if 1
	{
		bb_angle1024++;
	int aa_angle1024;
		aa_angle1024 = bb_angle1024;
	//int jj;
		/* �g�����̐l�`(�l�`) �����O�̐l�` ���E�O�̐l�` */
	//	for (jj=2; jj<8; jj++)/*2 3 4 5 6 7*/
		{
//			aa_angle1024 += (85*2)/*171*/;/* 85.33*2==1024/6 170.66*2== 1024/3 */
			aa_angle1024 += src->DOLL_DATA_fix_angle1024;
			mask1024(aa_angle1024);
			int radius;
		//	radius = (90)/*45*/;
			{
				radius = ((sin1024(cc1024))>>2)+16; /* 80==16+64 */
			}
		/*CCW*/
//			SPRITE *obj_boss;
//			obj_boss = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_08_BOSS];
			src->cx256 = (global_obj_boss)->cx256 + (/*(short)*/((sin1024((aa_angle1024))*radius)));
			src->cy256 = (global_obj_boss)->cy256 + (/*(short)*/((cos1024((aa_angle1024))*radius)));
		}
	}
	#if 0
//	if ((SPELL_CARD_00E_pache_000+3) < spell_card_number)/*+3��normal,hard,lunatic�� */
	#else
//???	if ((SPELL_CARD_11E_pache_bbb-1) < spell_card_number)/*-1�͒��O*/
	#endif
	{
		#if 1//0/*�Ƃ肠����*/
		if (0==(ra_nd()&(0xff)))
		{
			/* �V���b�g */
		//	b05_fire_flags &= (~(doll_data->identity_bit));/* off */
			obj_send1->cx256					= (src->cx256);
			obj_send1->cy256					= (src->cy256);
			br.BULLET_REGIST_00_speed256				= (t256(1.5)+((cg_game_difficulty)<<6));
			br.BULLET_REGIST_02_VECTOR_angle1024		= ANGLE_JIKI_NERAI_DAN;
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KOME_04_MIZU_IRO;	//BULLET_CAP16_04_KOME_SIROI;			/* [���Ēe] */	/* �e�O�� */
			br.BULLET_REGIST_05_regist_type 			= REGIST_TYPE_00_MULTI_VECTOR;
			br.BULLET_REGIST_06_n_way					= (4+(cg_game_difficulty)); 				/*(4-difficulty)*/
			br.BULLET_REGIST_07_VECTOR_div_angle1024	= (int)(1024/64);
			bullet_regist_vector();
		}
		#endif
	}
	#endif
}

/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_doll02(SPRITE *src)
{
	check_boss_option_time_out(src);	/* ���Ԍo�߂ŏI���B�{�X��|���ƊF�j�󂳂��B */
//
	cc1024 += (2);

	if (0==src->DOLL_DATA_state222)
	{
		{
//			SPRITE *obj_boss;
//			obj_boss = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_08_BOSS];
			src->cx256 = (global_obj_boss)->cx256 + (((src->DOLL_DATA_fix_angle1024))<<6);
			src->cy256 = (global_obj_boss)->cy256 - t256(16);
		}
	//	if (0==(com mon_boss_flags & FLG_MINI_DOLL))
		{
			src->DOLL_DATA_state222++;
		}
	}
	else
	if (1==src->DOLL_DATA_state222)
	{
		/* ���߂�(���߂���Əオ�󂫂����邵�A���ɋ߂�����) */
		if (t256(28)/*offset*/ < src->cy256)
		{
			src->cy256 -= t256(1.414);/*fps_factor*/
		}
		/* �K���͈͓�(t256(25)���炢)�A���� */
		if (t256(22)/*offset*/ < src->cy256)
		{
			src->DOLL_DATA_state222++;
		}
		/* ��߂�(��߂���ƒe�������Ă��Ȃ�) */
		else
		{
			src->cy256 += t256(1.414);/*fps_factor*/
		}
	}
	else
//	if (2==src->DOLL_DATA_state222)
	{
		{	src->DOLL_DATA_bwait--;}
		if (src->DOLL_DATA_bwait <= 0)
		{
			src->DOLL_DATA_bwait = 5+(3-(cg_game_difficulty))*5;
			src->shot_angle1024 -= (16);		/* cv1024r(10)*/
//
				obj_send1->cx256						= (src->cx256);
				obj_send1->cy256						= (src->cy256);
			//
				const unsigned char u8_ra_nd03 = (ra_nd()&0x03);
				br.BULLET_REGIST_00_speed256				= t256(1.0);//t256(1+difficulty)/*(3+difficulty)*/ /*(4+difficulty)*/;
//				br.BULLET_REGIST_02_VECTOR_angle1024				= (src->shot_angle1024&(256-1))+512+128;// /*deg512_2rad*/( (doll_data->br_angle512&(256-1) )+deg_360_to_512(90) );
				br.BULLET_REGIST_02_VECTOR_angle1024				= (src->shot_angle1024&(256-1))-512-128;// /*deg512_2rad*/( (doll_data->br_angle512&(256-1) )+deg_360_to_512(90) );
			//	br.BULLET_REGIST_07_VECTOR_jyuryoku_dan_delta256	= (u8_ra_nd03+1);//t256(0.04)/*10*/
				br.BULLET_REGIST_07_VECTOR_jyuryoku_dan_delta256	= (u8_ra_nd03+2);//t256(0.04)
				br.BULLET_REGIST_04_bullet_obj_type 	= (BULLET_MINI8_02_YUKARI+u8_ra_nd03);	/* �e�O�� */
				br.BULLET_REGIST_05_regist_type 		= REGIST_TYPE_02_GRAVITY02;
				bullet_regist_vector();
		}
	}
}

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

global void add_zako_pache_dolls(SPRITE *src)
{
//----[ZAKO]
	bb_angle1024		= (0);

//----[ZAKO]
	/* 85.33*2==512*2/6 170.66*2== 512*2/3 */	/*171*2*/
//	#define AA_OFS85 (85*2)
	#define AA_OFS85 (100*2)
	int jj_angle1024;
	jj_angle1024= -(int)(AA_OFS85/2);	/*0*/
	int i;
	for (i=0; i<8; i++)
	{
		SPRITE *h;	/* �l�`�B */
		h						= obj_add_01_teki_error();
		if (NULL!=h)/* �o�^�ł����ꍇ�̂� */	/* �����o�^ */
		{
			h->m_Hit256R			= ZAKO_ATARI16_PNG;
			h->flags				|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/);
			h->type 				= BOSS_16_YOUSEI11; 	/*S P_ZA KO*/	/*S P_BO SS05*/
//
			h->callback_loser		= lose_doll;
			h->callback_hit_teki	= callback_hit_zako;
//
			h->DOLL_DATA_bwait			= 0;
	//		h->shot_angle1024	= cv1024r(0);/*CW*/
			h->shot_angle1024	= (0);/*CCW*/
		//	h->base_score				= score( 500)*(difficulty+1);
			if (3/*1*/<i)
			{
				h->callback_mover		= move_doll01;
				h->type 				= /*0x20*/8+BOSS_16_YOUSEI11+((i&1)<<2/*4*/);		/* �~�j�l�` / �g�����̐l�` */
				h->base_score			= adjust_score_by_difficulty(score(  50000));		/*	 5�� x 4�l */
				h->base_hp				= (((128*8)-1));/*��������*/
			}
			else
			{
				h->callback_mover		= move_doll02;
				h->type 				= BOSS_16_YOUSEI11+(i<<2/*4*/); 			/*	0:�������̐l�`(������) / 1:�ɂ̖��̐l�`(���E��) */
				h->base_score			= adjust_score_by_difficulty(score( 300000));		/*	30�� x 4�l */
				h->base_hp				= (((1024*8)-1)*5);/*�d��(�������Ɖ󂹂�)*/
			}
			h->DOLL_DATA_fix_angle1024	= jj_angle1024;
			jj_angle1024 += (AA_OFS85);
			h->DOLL_DATA_state222		= (0);
			h->boss_time_out				= (0x01ff); 	/* �������� */
		}
	}
}

//�V�X�R�A�z��: 	�������ƕɂ̖��͂Ȃ�ׂ��|�����B�{�ȏ�X�R�A���Ⴄ��B
//		data->boss_base.score				= adjust_score_by_difficulty(score( 100000));	/*	10�� x 1�l (�v100��==(1�lx10��)+(6�lx5��)+(2�lx30��)) */
//		data->base.score					= adjust_score_by_difficulty(score( 300000));	/*	30�� x 2�l */
//		data->base.score					= adjust_score_by_difficulty(score(  50000));	/*	 5�� x 6�l */

//���X�R�A�z��
//		data->boss_base.score				= adjust_score_by_difficulty(score( 500000));	/*	50�� x 1�l (�v100��==(1�lx50��)+(5�lx10��)) */
//		data->base.score					= adjust_score_by_difficulty(score( 100000));	/*	10�� x 5�l */
