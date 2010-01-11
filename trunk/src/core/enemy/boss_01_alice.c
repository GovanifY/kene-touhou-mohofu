
#include "bullet_object.h"

/*---------------------------------------------------------
	�A���X�E�}�[�K�g���C�h
	Alice Margatroid.
	-------------------------------------------------------
	ToDo:
	�{�X�^�C�}�[�Ή���
---------------------------------------------------------*/

/*
C++�݂����� �p�� �����Ă݂�B
���A�悭�l�����猻��global��player�ɂ���̂�
�v��Ȃ������̂�OFF�B�ł����Ƃł��̍l���͎g�������B
 */
#define USE_KEISYOU 0

typedef struct /*_boss01_data*/
{
	BOSS_BASE boss_base;
//------------ �ړ��֘A
	int state1; 	/* �`�ԃX�e�[�g */	/* �e���X�e�[�g */
	int vx; 		/*	*/
	int vy; 		/*	*/
} BOSS01_DATA;

typedef struct /*_boss01_data*/
{
//	ENEMY_BASE base;
//
	#if (1==USE_KEISYOU)
	SPRITE *sakuya_obj; /* �p�� */
	#endif
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

#define FLG_MINI_DOLL	(FLG_DOLL2|FLG_DOLL3|FLG_DOLL4|FLG_DOLL5|FLG_DOLL6|FLG_DOLL7)
#define FLG_FIRE_DOLL	(FLG_DOLL0|FLG_DOLL1)
#define FLG_ALL_CAST	(FLG_FIRE_DOLL|FLG_MINI_DOLL)/*�S��*/


/*---------------------------------------------------------
	�G����
---------------------------------------------------------*/

static void lose_doll(SPRITE *src)
{
	send1_obj->x256 = src->x256+((src->w128));
	send1_obj->y256 = src->y256+((src->h128));
	bakuhatsu_add_type_ddd(send1_obj/*src->x256+((src->w128)),src->y256+((src->h128))*/,/*0,*/BAKUHATSU_FIRE08);
//
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
		#if (1==USE_KEISYOU)
		/*CCW*/
			src->x256 = data->sakuya_obj->x256 + ((sin512((aa_angle512))*radius));
			src->y256 = data->sakuya_obj->y256 + ((cos512((aa_angle512))*radius));
		#else
		/*CCW*/
			src->x256 = (((PLAYER_DATA *)player->data)->boss)->x256 + ((sin512((aa_angle512))*radius));
			src->y256 = (((PLAYER_DATA *)player->data)->boss)->y256 + ((cos512((aa_angle512))*radius));
		#endif
		}
	}
	if (SPELL_CARD_00_alice_000 < spell_card_number)
	{
		if (0==(ra_nd()&(0xff)))
		{
			/* �V���b�g */
		//	b01_fire_flags &= (~(doll_data->identity_bit));/* off */
			send1_obj->x256 = src->x256;
			send1_obj->y256 = src->y256;
			#if 1
			/* ���Ƃŗv�� */
	//		send1_obj->h128 = src->h128;
	//		send1_obj->w128 = src->w128;
			#endif
		//	bullet_create_n_way_dan_sa_type(src,
			send1_obj->BULLET_REGIST_speed256			=	(t256(1.5)+(difficulty<<6));
			send1_obj->BULLET_REGIST_angle512			=	ANGLE_JIKI_NERAI_DAN;
			send1_obj->BULLET_REGIST_div_angle512		=	(int)(512/64);
			send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_CAP16_04_SIROI;
			send1_obj->BULLET_REGIST_n_way				=	(4+difficulty)/*(4-difficulty)*/ ;
			bullet_regist_basic();
		}
	}
	#endif
	#if 1
	/* �A���X��|���ƊF�j�󂳂��B */
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
		#if (1==USE_KEISYOU)
		src->x256 = data->sakuya_obj->x256 + (((data->fix_angle512))<<7);
		src->y256 = data->sakuya_obj->y256 - t256(16);
		#else
		src->x256 = (((PLAYER_DATA *)player->data)->boss)->x256 + (((data->fix_angle512))<<7);
		src->y256 = (((PLAYER_DATA *)player->data)->boss)->y256 - t256(16);
		#endif
		if (0==(common_boss_flags & FLG_MINI_DOLL))
		{
			data->state222++;
		}
		break;
	case 1:
		/* ���߂�(���߂���Əオ�󂫂����邵�A���ɋ߂�����) */
		if (t256(28)/*offset*/ < src->y256)
		{
			src->y256 -= t256(1.414)/**fps_fa ctor*/;
		}
		/* �K���͈͓�(t256(25)���炢)�A���� */
		if (t256(22)/*offset*/ < src->y256)
		{
			data->state222++;
		}
		/* ��߂�(��߂���ƒe�������Ă��Ȃ�) */
		else
		{
			src->y256 += t256(1.414)/**fps_fa ctor*/;
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
	/* �A���X��|���ƊF�j�󂳂��B */
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
		if ((32)>alice_anime_count) 	{	src->type = BOSS_09_BOSS32;/*src->an im_frame = 0x23;*/}
		else
		if ((40)>alice_anime_count) 	{	src->type = BOSS_10_BOSS33;/*src->an im_frame = 0x22;*/}
		else
										{	src->type = BOSS_09_BOSS32;/*src->an im_frame = 0x21;*/}
	}
	else	/* �ړ��A�j���[�V���� */
	{
	//	BOSS01_DATA *data=(BOSS01_DATA *)src->data;
		int aaa;
		aaa = (src->vx256>0)?(BOSS_00_BOSS11/*0x00*/):(BOSS_04_BOSS21/*0x10*/); /*���E*/
		if ((16)>vvv256)		{	aaa+=2;}	/* 25.6==t256(0.1)*/
		else
		if ((200)>vvv256)		{	aaa+=3;}	/* 76.8==t256(0.3)*/
		else
		if ((224)>vvv256)		{	aaa+=2;}	/*128.0==t256(0.5)*/
		else
		if ((240)>vvv256)		{	aaa+=1;}	/*179.2==t256(0.7)*/
	//	else					{	aaa+=0;}
		/*src->an im_frame*/src->type = aaa;
	}
}


/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/
extern const u8 alice_danmaku_table[16];

//		firewait1	= 45;
//		firewait2	= 4;
//		firewait3	= 0;
//		break;
//	case ST_01:
//	static int firewait1;
//	static int firewait2;
//	static int firewait3;
//
/*static*/ void alice_01_keitai(SPRITE *src)
{
	static int my_wait;
	my_wait -= (1)/*fps_fa ctor*/;
//
	BOSS01_DATA *data = (BOSS01_DATA *)src->data;
	{
		if (my_wait <= 0)
		{	/* �ړ����������߂� */
			data->state1++;
			data->state1 &= (8-1);
			{
				enum
				{
					PPP_00_VX = 0,			/* x �x�N�g���ړ��� */
					PPP_01_VY,				/* y �x�N�g���ړ��� */
					PPP_02_WAIT_DIV_2,		/* �E�F�C�g�J�E���^�̔����� */
					PPP_03_IS_RESET_ANIME,	/* �A�j���[�V�������Z�b�g 0:���Ȃ� 1:���� 2:����(�e������) */
				};
				const s8 ppp[8][4] =
				{
					{( 2),(-1),(100),( 1),},	/*�E���*/
					{( 0),( 0),( 50),( 2),},	/*wait*/
					{( 2),( 1),(100),( 1),},	/*�E����*/
					{( 0),( 0),( 10),( 0),},	/*wait*/
					{(-2),(-1),(100),( 1),},	/*�����*/
					{( 0),( 0),( 50),( 2),},	/*wait*/
					{(-2),( 1),(100),( 1),},	/*������*/
					{( 0),( 0),( 10),( 0),},	/*wait*/
				};
				data->vx	= ppp[data->state1][PPP_00_VX];
				data->vy	= ppp[data->state1][PPP_01_VY];
				my_wait 	= ppp[data->state1][PPP_02_WAIT_DIV_2]; 	/* 50*4 60 �ړ��� */
				my_wait 	+= my_wait;
				if (0!=ppp[data->state1][PPP_03_IS_RESET_ANIME])
				{
					if (2==ppp[data->state1][PPP_03_IS_RESET_ANIME])	/* �U���A�j���[�V���� */
					{
						alice_anime_count = 48;
						data->boss_base.danmaku_test++;
						data->boss_base.danmaku_test &= 0x07;
						data->boss_base.danmaku_type		= alice_danmaku_table[data->boss_base.danmaku_test];	/*DANMAKU_01*/	/* �S�e�����Z�b�g */
						data->boss_base.danmaku_time_out	= (DANMAKU_01_SET_TIME+DANMAKU_01_SET_TIME);	/* �S�e���̔������� x 2 */
					}
					vvv256=1;
				}
			}
		}
	}
	src->vx256 = (data->vx)*vvv256;
	src->vy256 = (data->vy)*vvv256;
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
		�A���X�ړ�����
	---------------------------------------------------------*/
	alice_animation(src);
//	boss_move_clip_rect(src);
//	move_all_doll(src);
//
	boss_effect(src);			/* ��G�t�F�N�g */
	danmaku_generator(src); 	/* �e������ */
//
}

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/
//		sakuya->base_score			= score(1000)*(difficulty+1);

void add_boss_alice(STAGE_DATA *l)/*int lv*/
{
	boss_bgm_mode		= (l->user_y);
//
	alice_anime_count	= 0;
//

//----[ZAKO]
//	bb_angle512 		= 128-43/*CCW*/ /*CW 43*/;/*512/12==42.66*/
	bb_angle512 		= 128/*CCW*/ /*CW 43*/;/*512/12==42.66*/

	common_boss_flags	= (FLG_ALL_CAST);

//----[BOSS]
		SPRITE *sakuya;
	//	sakuya								= sprite_add_res(BASE_BOSS_ALICE_PNG); /*"boss01-mo.png"60x42, offset*/
		sakuya								= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
		sakuya->flags						|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
//		sakuya->an im_frame 				= 0;
		sakuya->type						= BOSS_00_BOSS11;	/*SP_BOSS*/ 	/*SP_BOSS01*/
		sakuya->callback_mover				= move_alice;
		sakuya->callback_loser				= common_boss_put_items;
	//	sakuya->callback_loser				= lose_boss/*common_boss_put_items*/;/* �X�y�J���� */
//
		sakuya->base_health 				= (1023);	// �A���X�{�̂�HP�B�������������Ă����������B// [***090305	�ύX
		sakuya->base_score					= adjust_score_by_difficulty(score( 100000));	/*	10�� x 1�l (�v100��==(1�lx10��)+(6�lx5��)+(2�lx30��)) */
	//------------ �X�y�J�֘A
		spell_card_number					= SPELL_CARD_00_alice_000;
		spell_card_max						= SPELL_CARD_19_alice_jjj;
		spell_card_boss_init_regist(sakuya);
	{
		BOSS01_DATA *data;
		data								= mmalloc(sizeof(BOSS01_DATA));
		sakuya->data						= data;
		//
		data->state1						= 0/*ST_00*/;
		data->vx = ( 0);
		data->vy = ( 0);
		//
		#if 1
	//------------ �e���֘A
		data->boss_base.danmaku_type		= 0;
		data->boss_base.danmaku_time_out	= 0;
		data->boss_base.danmaku_test		= 0;	/*(DANMAKU_08_rumia-1)*/ /*0*/
		#endif
	}

//----[ZAKO]
	/* 85.33==512/6 170.66== 512/3 */	/*171*/
	#define AA_OFS85 (85)
	int jj_angle512;
	jj_angle512= -(int)(AA_OFS85/2);	/*0*/

	int i;
	for (i=0; i<8; i++)
	{
		/*static*/ SPRITE *obj_doll;	/* �l�`�B */
	//	obj_doll						= sprite_add_res(BASE_BOSS_ALICE_DOLLS_PNG/*doll_res_num[i]*/);
		obj_doll						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
//		obj_doll->anim_speed			= 0;
//		obj_doll->type					= (BOSS_16_YOUSEI11+(i<<2));		/*SP_ZAKO*/ 	/*SP_BOSS01*/
		obj_doll->flags 				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
//
		obj_doll->callback_loser		= lose_doll;
		obj_doll->callback_hit_enemy	= callback_hit_zako;
//
		DOLL_DATA *data;
		data								= mmalloc(sizeof(DOLL_DATA));
		#if (1==USE_KEISYOU)
		data->sakuya_obj					= sakuya;	/* �p�������� */
		#endif
		obj_doll->data					= data;
		data->identity_bit					= (FLG_DOLL0<<i);		/* �ŗL�r�b�g�ԍ� */
//		data->health_flag					= 0;
		data->bwait 						= 0;
	//	br1_angle512						= deg_360_to_512(0);
	//	br2_angle512						= deg_360_to_512(0);
//		data->br_angle512					= deg_360_to_512(0);/*CW*/
		data->br_angle512					= (0);/*CCW*/
		obj_doll->base_health			= (1024-1)/*(200)*/ /*((2==i)?(b01_04[difficulty]):(b01_health[i]))*/;
	//	obj_doll->base_score			= score( 500)*(difficulty+1);
		if (1<i)
		{
			obj_doll->callback_mover	= move_doll01;
			obj_doll->type/*an im_frame*/		= /*0x20*/8+BOSS_16_YOUSEI11+((i&1)<<2/*4*/);		/* �~�j�l�` / �g�����̐l�` */
			obj_doll->base_score		= adjust_score_by_difficulty(score(  50000));	/*	 5�� x 6�l */
		}
		else
		{
			obj_doll->callback_mover	= move_doll02;
			obj_doll->type/*an im_frame*/		= BOSS_16_YOUSEI11+(i<<2/*4*/); 			/*	0:�������̐l�`(������) / 1:�ɂ̖��̐l�`(���E��) */
			obj_doll->base_score		= adjust_score_by_difficulty(score( 300000));	/*	30�� x 2�l */
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
