
#include "bullet_object.h"

/*---------------------------------------------------------
	�p�`�����[�E�m�[���b�W
	Patchouli Knowledge.
	-------------------------------------------------------
	�p�`�F(�\��)	(r27����V�K�ǉ�)
	ToDo:
	�{�X�^�C�}�[�Ή���
---------------------------------------------------------*/

//	int st ate1;	/* �`�ԃX�e�[�g */	/* �e���X�e�[�g */
#define vx			user_data00 	/*	*/
#define vy			user_data01 	/*	*/
//


typedef struct /*_boss01_data*/
{
//	ENEMY_BASE base;
//
	int identity_bit;	/* �ŗL�r�b�g�ԍ� */
	int fix_angle512;	/* �Ă��ƌŒ�l */
//------------ doll02��p
	int state222;		/* �`�� */
	int br_angle512;	/* �e�̊p�x */
	int bwait;			/* �e���˂܂ł̑҂����ԁB */
} DOLL_DATA;

static unsigned int alice_anime_count;

//----[ZAKO]

static int bb_angle512;


// FLG_FIRE1 �e�t���O�B�g�����̐l�`�����Ă��āA���w��ʒu�ɒ��������Bon�ɂȂ�B
#define FLG_FIRE1 0x01/* �������̐l�`(������)���w��ʒu�ɕt������ */
#define FLG_FIRE2 0x02/* �ɂ̖��̐l�`(���E��)���w��ʒu�ɕt������ */
//
#define FLG_DOLL0 0x01/* �������̐l�`(������) */
#define FLG_DOLL1 0x02/* �ɂ̖��̐l�`(���E��) */
//
#define FLG_DOLL2 0x04/* �g�����̐l�`(�l�`) */	/*key*/
#define FLG_DOLL3 0x08/* �����O�̐l�` */
//
#define FLG_DOLL4 0x10/* ���E�O�̐l�` */
#define FLG_DOLL5 0x20/* ���E�O�̐l�` */
#define FLG_DOLL6 0x40/* ���E�O�̐l�` */
#define FLG_DOLL7 0x80/* ���E�O�̐l�` */

#define FLG_MINI_DOLL	(FLG_DOLL4|FLG_DOLL5|FLG_DOLL6|FLG_DOLL7)
#define FLG_FIRE_DOLL	(FLG_DOLL0|FLG_DOLL1|FLG_DOLL2|FLG_DOLL3)
#define FLG_ALL_CAST	(FLG_FIRE_DOLL|FLG_MINI_DOLL)/*�S��*/

/*---------------------------------------------------------
	�G����
---------------------------------------------------------*/

static void lose_doll(SPRITE *src)
{
	bakuhatsu_add_type_ddd(src, BAKUHATSU_FIRE08);
	item_create_for_boss(src, ITEM_CREATE_MODE_02);
//
	DOLL_DATA *data = (DOLL_DATA *)src->data;
	/* ���i�̏����𑬂�(R4000�n�����ӂ�0��r�𑽗p����)���āA������(�ő��ɗ��Ȃ��̂�)�x�����܂� */
	common_boss_flags &= (~(data->identity_bit));
}

/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static int cc512;
static void move_doll01(SPRITE *src)
{
	DOLL_DATA *data = (DOLL_DATA *)src->data;
	#if 1
	{
		bb_angle512++;
	int aa_angle512;
		aa_angle512 = bb_angle512;
	//int jj;
		/* �g�����̐l�`(�l�`) �����O�̐l�` ���E�O�̐l�` */
	//	for (jj=2; jj<8; jj++)/*2 3 4 5 6 7*/
		{
//			aa_angle512 += (85)/*171*/;/* 85.33==512/6 170.66== 512/3 */
			aa_angle512 += data->fix_angle512;
			mask512(aa_angle512);
			int radius;
		//	radius = (90)/*45*/;
			{
				radius = ((sin512(cc512))>>2)+16;	/* 80==16+64 */
			}
		/*CCW*/
			src->x256 = (pd_boss)->x256 + (/*(short)*/((sin512((aa_angle512))*radius)));
			src->y256 = (pd_boss)->y256 + (/*(short)*/((cos512((aa_angle512))*radius)));
		}
	}
	if (SPELL_CARD_00_pache_000 < spell_card_number)
	{
		if (0==(ra_nd()&(0xff)))
		{
			/* �V���b�g */
		//	b05_fire_flags &= (~(doll_data->identity_bit));/* off */
			send1_obj->x256 = src->x256;
			send1_obj->y256 = src->y256;
			send1_obj->BULLET_REGIST_speed256			=	(t256(1.5)+(difficulty<<6));
			send1_obj->BULLET_REGIST_angle512			=	ANGLE_JIKI_NERAI_DAN;
			send1_obj->BULLET_REGIST_div_angle512		=	(int)(512/64);
			send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_CAP16_04_SIROI;
			send1_obj->BULLET_REGIST_n_way				=	(4+difficulty); 					/*(4-difficulty)*/
			bullet_regist_basic();
		}
	}
	#endif
	#if 1
	/* �p�`�F��|���ƊF�j�󂳂��B */
	if (0==common_boss_flags)
	{
		src->type = SP_DELETE;
	}
	#endif
}

/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_doll02(SPRITE *src)
{
	cc512++;
	DOLL_DATA *data = (DOLL_DATA *)src->data;
	switch (data->state222)
	{
	case 0:
		src->x256 = (pd_boss)->x256 + (((data->fix_angle512))<<7);
		src->y256 = (pd_boss)->y256 - t256(16);
		if (0==(common_boss_flags & FLG_MINI_DOLL))
		{
			data->state222++;
		}
		break;
	case 1:
		/* ���߂�(���߂���Əオ�󂫂����邵�A���ɋ߂�����) */
		if (t256(28)/*offset*/ < src->y256)
		{
			src->y256 -= t256(1.414);/*fps_factor*/
		}
		/* �K���͈͓�(t256(25)���炢)�A���� */
		if (t256(22)/*offset*/ < src->y256)
		{
			data->state222++;
		}
		/* ��߂�(��߂���ƒe�������Ă��Ȃ�) */
		else
		{
			src->y256 += t256(1.414);/*fps_factor*/
		}
		break;
	case 2:
		{	data->bwait--;}
		if (data->bwait <= 0)
		{
			data->bwait = 5+(3-difficulty)*5;
//			data->br_angle512 += deg_360_to_512(10);
//			data->br_angle512 -= deg_360_to_512CCW(10);
//			data->br_angle512 += 8;
			data->br_angle512 -= 8;
			bullet_create_jyuryoku_dan000(
				src,
				t256(1.0),//t256(1+difficulty)/*(3+difficulty)*/ /*(4+difficulty)*/,
//				(data->br_angle512&127)+256+64,// /*deg512_2rad*/( (doll_data->br_angle512&255/*%180*/)+deg_360_to_512(90) ),
				(data->br_angle512&127)-256-64,// /*deg512_2rad*/( (doll_data->br_angle512&255/*%180*/)+deg_360_to_512(90) ),
			//	((ra_nd()&0x03)+1)//t256(0.04)/*10*/
				((ra_nd()&0x03)+2),//t256(0.04)
				(BULLET_MARU8_00_AKA+(7))	/* �e�O�� */
			);
		}
		break;
	}
	#if 1
	/* �p�`�F��|���ƊF�j�󂳂��B */
	if (0==common_boss_flags)
	{
		src->type = SP_DELETE;
	}
	#endif
}

/*---------------------------------------------------------
	���l���̔z�u����
	[1][0][2]
	[4][3][5]
---------------------------------------------------------*/

/*---------------------------------------------------------

---------------------------------------------------------*/

static int vvv256;
static void alice_animation(SPRITE *src)
{
	vvv256++/*vvv256+=vvv256*/;
	if (vvv256> t256(1.0) )
	{	vvv256= t256(1.0);}
//
	if (alice_anime_count)	/* �U���A�j���[�V���� */
	{
		alice_anime_count--;
		if ((32)>alice_anime_count) 	{	src->type = TEKI_09_BOSS32;/*src->an im_frame = 0x23;*/}
		else
		if ((40)>alice_anime_count) 	{	src->type = TEKI_10_BOSS33;/*src->an im_frame = 0x22;*/}
		else
										{	src->type = TEKI_09_BOSS32;/*src->an im_frame = 0x21;*/}
	}
	else	/* �ړ��A�j���[�V���� */
	{
		int aaa;
		aaa = (src->vx>0)?(TEKI_00_BOSS11/*0x00*/):(TEKI_04_BOSS21/*0x10*/);	/*���E*/
		if ((16)>vvv256)		{	aaa+=2;}	/* 25.6==t256(0.1)*/
		else
		if ((200)>vvv256)		{	aaa+=3;}	/* 76.8==t256(0.3)*/
		else
		if ((224)>vvv256)		{	aaa+=2;}	/*128.0==t256(0.5)*/
		else
		if ((240)>vvv256)		{	aaa+=1;}	/*179.2==t256(0.7)*/
	//	else					{	aaa+=0;}
		src->type = aaa;
	}
}


/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/
static const u8 pache_danmaku_table[16] =
{
	DANMAKU_07_sakuya,
	DANMAKU_08_rumia,
	DANMAKU_04_sakuya,
	DANMAKU_09_alice,
	DANMAKU_05_sakuya,
	DANMAKU_0a_houka_kenran/*DANMAKU_09_alice*/,
	DANMAKU_06_sakuya,
	DANMAKU_09_alice,
//
	DANMAKU_0c_hana_test,//DANMAKU_07_sakuya,
	0,
	DANMAKU_01_sakuya,
	DANMAKU_02_sakuya,
	DANMAKU_03_sakuya,
	DANMAKU_04_sakuya,
	DANMAKU_05_sakuya,
	DANMAKU_0b_alice_doll,//DANMAKU_06_sakuya,
};
//
/*static*/ void pache_01_keitai(SPRITE *src)
{
	static int my_wait;
	my_wait--;/*fps_factor*/
//
	{
		if (my_wait <= 0)
		{	/* �ړ����������߂� */
			src->boss_base_state001++;
			src->boss_base_state001 &= (8-1);
			{	u16 my_rand;
				my_rand = ra_nd();
//				data->vx	= ((my_rand&0x02)-1)/*ppp[src->boss_base_state001][PPP_00_VX]*/;
//				data->vy	= (((my_rand>>8)&0x03)-2)/*ppp[src->boss_base_state001][PPP_01_VY]*/;
//					 if (src->x256< t256(		  0)+t256(48) ) 	{	data->vx =	(2);}
//				else if (src->x256> t256(GAME_WIDTH)-t256(48) ) 	{	data->vx = -(2);}
			//	my_wait 	= ppp[src->boss_base_state001]/*[PPP_02_WAIT_DIV_2]*/;	/* 50*4 60 �ړ��� */
				my_wait 	= (my_rand & 0xff); 	/* 50*4 60 �ړ��� */	/*[PPP_02_WAIT_DIV_2]*/
			//	my_wait 	+= my_wait;
			//	if (0!=ppp[src->boss_base_state001][PPP_03_IS_RESET_ANIME])
				{
				//	if (2==ppp[src->boss_base_state001][PPP_03_IS_RESET_ANIME]) /* �U���A�j���[�V���� */
					if (0==(my_rand&0x04))
					{
//						alice_anime_count = 48;
						src->boss_base_danmaku_test++;
						src->boss_base_danmaku_test &= 0x0f;
						src->boss_base_danmaku_type 	= pache_danmaku_table[src->boss_base_danmaku_test]; /*DANMAKU_01*/	/* �S�e�����Z�b�g */
						src->boss_base_danmaku_time_out = (DANMAKU_01_SET_TIME+DANMAKU_01_SET_TIME);	/* �S�e���̔������� x 2 */
					}
//					vvv256=1;
				}
			}
		}
	}
//	src->vx256 = (data->vx)*vvv256;
//	src->vy256 = (data->vy)*vvv256;
}
/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/
/*static*/ void pache_04_keitai(SPRITE *src)
{
	static int my_wait;
	my_wait--;/*fps_factor*/
//
	{
		if (my_wait <= 0)
		{	/* �ړ����������߂� */
			src->boss_base_state001++;
			src->boss_base_state001 &= (8-1);
			{	u16 my_rand;
				my_rand = ra_nd();
				src->vx = ((my_rand&0x02)-1)/*ppp[src->boss_base_state001][PPP_00_VX]*/;
				src->vy = (((my_rand>>8)&0x03)-2)/*ppp[src->boss_base_state001][PPP_01_VY]*/;
					 if (src->x256< t256(		  0)+t256(48) ) 	{	src->vx =	(2);}
				else if (src->x256> t256(GAME_WIDTH)-t256(48) ) 	{	src->vx = -(2);}
			//	my_wait 	= ppp[data->boss_base.state001]/*[PPP_02_WAIT_DIV_2]*/; 	/* 50*4 60 �ړ��� */
				my_wait 	= (my_rand & 0xff); 	/* 50*4 60 �ړ��� */	/*[PPP_02_WAIT_DIV_2]*/
			//	my_wait 	+= my_wait;
			//	if (0!=ppp[src->boss_base_state001][PPP_03_IS_RESET_ANIME])
				{
				//	if (2==ppp[src->boss_base_state001][PPP_03_IS_RESET_ANIME]) /* �U���A�j���[�V���� */
					if (0==(my_rand&0x04))
					{
						alice_anime_count = 48;
						src->boss_base_danmaku_test++;
						src->boss_base_danmaku_test &= 0x0f;
						src->boss_base_danmaku_type 	= pache_danmaku_table[src->boss_base_danmaku_test]; /*DANMAKU_01*/	/* �S�e�����Z�b�g */
						src->boss_base_danmaku_time_out = (DANMAKU_01_SET_TIME+DANMAKU_01_SET_TIME);	/* �S�e���̔������� x 2 */
					}
					vvv256=1;
				}
			}
		}
	}
	src->vx256 = (src->vx)*vvv256;
	src->vy256 = (src->vy)*vvv256;
}

/*---------------------------------------------------------
	�{�X�s��
---------------------------------------------------------*/

static void move_alice(SPRITE *src)
{
	/* �X�y�J�o�^ */
			if (0/*off*/==spell_card_mode)
			{
				regist_spell_card222(src);
			}
	spell_card_generator222(src);	/* �X�y�J���� */
	#if 1/* [�X�y�J�V�X�e�����Ɉړ��\��] */
	/*---------------------------------------------------------
		�p�`�F�ړ�����
	---------------------------------------------------------*/
	//
	//	enemy_boss04_setpos(src, xxx,yyy);
	//
	boss_move_clip_rect(src);
	#endif
//
	/*---------------------------------------------------------
		�p�`�F�ړ�����
	---------------------------------------------------------*/
	alice_animation(src);
//	boss_move_clip_rect(src);
//	move_all_doll(src);
//
	boss_effect(src);			/* ��G�t�F�N�g */
	danmaku_generator(src); 	/* �e������ */
}

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

void add_boss_pache(STAGE_DATA *l)
{
	alice_anime_count	= 0;

//----[ZAKO]
//	bb_angle512 		= 128-43/*CCW*/ /*CW 43*/;/*512/12==42.66*/
	bb_angle512 		= 0;/*128*/ /*CCW*/ /*CW 43*/ /*512/12==42.66*/

	common_boss_flags	= (FLG_ALL_CAST);

	//----[BOSS]
	SPRITE *h;
	#if (0==USE_BOSS_COMMON_MALLOC)
	h								= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
	#else
	h								= pd_boss;/*�P��{�l*/
	#endif
	h->flags						|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
	h->type 						= BOSS_00_BOSS11;
	h->callback_mover				= move_alice;
	h->callback_loser				= common_boss_put_items;
	//	life	[2] + [bar]
	h->base_score					= adjust_score_by_difficulty(score( 100000));	/*	10�� x 1�l (�v100��==(1�lx10��)+(6�lx5��)+(2�lx30��)) */
//		h->base_score					= score(1000)*(difficulty+1);
//
	//------------ �X�y�J�֘A
	spell_card_number					= SPELL_CARD_00_pache_000;
	spell_card_max						= SPELL_CARD_17_pache_hhh;
	#if (0==USE_BOSS_COMMON_MALLOC)
	spell_card_boss_init_regist(h);
	#else
	spell_card_boss_init_regist_void();
	#endif
	{
	h->boss_base_state001			= 0/*ST_00*/;
	h->vx							= ( 0);
	h->vy							= ( 0);
	}

//----[ZAKO]
	/* 85.33==512/6 170.66== 512/3 */	/*171*/
//	#define AA_OFS85 (85)
	#define AA_OFS85 (100)
	int jj_angle512;
	jj_angle512= -(int)(AA_OFS85/2);	/*0*/
	int i;
	for (i=0; i<8; i++)
	{
		/*static*/ SPRITE *obj_doll;	/* �l�`�B */
	//	obj_doll						= sprite_add_res(BASE_BOSS_PACHE_DOLLS_PNG/*doll_res_num[i]*/);
		obj_doll						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
		obj_doll->type					= BOSS_16_YOUSEI11; 	/*S P_ZA KO*/	/*S P_BO SS05*/
		obj_doll->flags 				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
//
		obj_doll->callback_loser		= lose_doll;
		obj_doll->callback_hit_enemy	= callback_hit_zako;
//
		DOLL_DATA *data;
		data								= mmalloc(sizeof(DOLL_DATA));
		obj_doll->data						= data;
		data->identity_bit					= (FLG_DOLL0<<i);		/* �ŗL�r�b�g�ԍ� */
//		data->health_flag					= 0;
		data->bwait 						= 0;
	//	br1_angle512						= deg_360_to_512(0);
	//	br2_angle512						= deg_360_to_512(0);
//		data->br_angle512					= deg_360_to_512(0);/*CW*/
		data->br_angle512					= (0);/*CCW*/
	//	obj_doll->base_score				= score( 500)*(difficulty+1);
		if (3/*1*/<i)
		{
			obj_doll->callback_mover		= move_doll01;
			obj_doll->type					= /*0x20*/8+BOSS_16_YOUSEI11+((i&1)<<2/*4*/);		/* �~�j�l�` / �g�����̐l�` */
			obj_doll->base_score			= adjust_score_by_difficulty(score(  50000));	/*	 5�� x 4�l */
			obj_doll->base_hp				= (((128*8)-1));/*��������*/
		}
		else
		{
			obj_doll->callback_mover		= move_doll02;
			obj_doll->type					= BOSS_16_YOUSEI11+(i<<2/*4*/); 			/*	0:�������̐l�`(������) / 1:�ɂ̖��̐l�`(���E��) */
			obj_doll->base_score			= adjust_score_by_difficulty(score( 300000));	/*	30�� x 4�l */
			obj_doll->base_hp				= (((1024*8)-1)*5);/*�d��(�������Ɖ󂹂�)*/
		}
		data->fix_angle512					= jj_angle512;
		jj_angle512 += (AA_OFS85);
		data->state222						= 0;
	}
}

//�V�X�R�A�z��: 	�������ƕɂ̖��͂Ȃ�ׂ��|�����B�{�ȏ�X�R�A���Ⴄ��B
//		data->boss_base.score				= adjust_score_by_difficulty(score( 100000));	/*	10�� x 1�l (�v100��==(1�lx10��)+(6�lx5��)+(2�lx30��)) */
//		data->base.score					= adjust_score_by_difficulty(score( 300000));	/*	30�� x 2�l */
//		data->base.score					= adjust_score_by_difficulty(score(  50000));	/*	 5�� x 6�l */

//���X�R�A�z��
//		data->boss_base.score				= adjust_score_by_difficulty(score( 500000));	/*	50�� x 1�l (�v100��==(1�lx50��)+(5�lx10��)) */
//		data->base.score					= adjust_score_by_difficulty(score( 100000));	/*	10�� x 5�l */
