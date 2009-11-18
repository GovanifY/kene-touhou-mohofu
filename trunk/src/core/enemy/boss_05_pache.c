
#include "bullet_object.h"

/*---------------------------------------------------------
	�p�`�����[�E�m�[���b�W
	�p�`�F(�\��)	(r27����V�K�ǉ�)
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
	int state1; 		/* �`�ԃX�e�[�g */
	int state8; 		/* �e���X�e�[�g */
//	int move_type;		/* �ړ��^�C�v */
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


//static int b01level;
static unsigned int alice_anime_count;

//----[ZAKO]

static int bb_angle512;

static SPRITE *obj_doll[8]; 	/* �l�`�B */

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

---------------------------------------------------------*/

enum
{
/*0*/	ALICE_01_KEITAI = 0,	/* ���`��: �p�`�F�o�� */
/*0*/	ALICE_02_KEITAI,		/* ���`��: �S���ōU�� */
/*1*/	ALICE_03_KEITAI,		/* ��O�`��: �t�H�[���V����2 */
/*2*/	ALICE_04_KEITAI,		/* ��l�`��: �p�`�F�P�� */
};
enum
{
	ST_00 = 0,
	ST_01,
	ST_02,
	ST_03,
	ST_04,
	ST_05,
	ST_06,
	ST_07,
	ST_08,
	ST_09,
	ST_10,
	ST_11,
	ST_12,
	ST_13,
};

/*---------------------------------------------------------
	�G����
---------------------------------------------------------*/

static void lose_doll(SPRITE *src)
{
	dummy_obj->x256 = src->x256+((src->w128));
	dummy_obj->y256 = src->y256+((src->h128));
	bakuhatsu_add_type_ddd(dummy_obj/*src->x256+((src->w128)),src->y256+((src->h128))*/,/*0,*/BAKUHATSU_FIRE08);
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
			/*obj_doll[jj]*/src->x256 = data->sakuya_obj->x256 + ((sin512((aa_angle512))*radius));
			/*obj_doll[jj]*/src->y256 = data->sakuya_obj->y256 + ((cos512((aa_angle512))*radius));
		#else
		/*CCW*/
			/*obj_doll[jj]*/src->x256 = (((PLAYER_DATA *)player->data)->boss)->x256 + ((sin512((aa_angle512))*radius));
			/*obj_doll[jj]*/src->y256 = (((PLAYER_DATA *)player->data)->boss)->y256 + ((cos512((aa_angle512))*radius));
		#endif
		}
	}
	#if (1==USE_KEISYOU)
	if (ST_03 < ((BOSS01_DATA *)data->sakuya_obj->data)->state1 )
	#else
	if (ST_03 < ((BOSS01_DATA *)(((PLAYER_DATA *)player->data)->boss)->data)->state1 )
	#endif
	{
		if (0==(ra_nd()&(0xff)))
		{
			/* �V���b�g */
		//	b05_fire_flags &= (~(doll_data->identity_bit));/* off */
			bullet_create_n_way_dan_sa_type(src,
				(t256(1.5)+(difficulty<<6)),
				ANGLE_JIKI_NERAI_DAN,
				(int)(512/64),
				BULLET_CAP16_04_SIROI,
				(4+difficulty)/*(4-difficulty)*/);
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
		#if (1==USE_KEISYOU)
		/*obj_doll[jj]*/src->x256 = data->sakuya_obj->x256 + (((data->fix_angle512))<<7);
		/*obj_doll[jj]*/src->y256 = data->sakuya_obj->y256 - t256(16);
		#else
		/*obj_doll[jj]*/src->x256 = (((PLAYER_DATA *)player->data)->boss)->x256 + (((data->fix_angle512))<<7);
		/*obj_doll[jj]*/src->y256 = (((PLAYER_DATA *)player->data)->boss)->y256 - t256(16);
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
				256,//t256(1+b01level)/*(3+b01level)*/ /*(4+doll_data->level)*/,
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
	if (vvv256> t256(1) )
	{	vvv256= t256(1);}
//
	if (alice_anime_count)	/* �U���A�j���[�V���� */
	{
		alice_anime_count--;
		if ((32)>alice_anime_count) 	{	src->anim_frame = 0x23;}
		else
		if ((40)>alice_anime_count) 	{	src->anim_frame = 0x22;}
		else
										{	src->anim_frame = 0x21;}
	}
	else	/* �ړ��A�j���[�V���� */
	{
		BOSS01_DATA *data=(BOSS01_DATA *)src->data;
		int aaa;
		aaa = (data->vx>0)?(0x00):(0x10);	/*���E*/
		if ((16)>vvv256)		{	aaa+=2;}	/* 25.6==t256(0.1)*/
		else
		if ((200)>vvv256)		{	aaa+=3;}	/* 76.8==t256(0.3)*/
		else
		if ((224)>vvv256)		{	aaa+=2;}	/*128.0==t256(0.5)*/
		else
		if ((240)>vvv256)		{	aaa+=1;}	/*179.2==t256(0.7)*/
	//	else						{	aaa+=0;}
		src->anim_frame = aaa;
	}
}

/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_alice(SPRITE *src)
{
	static int my_wait;
//	static int firewait1;
//	static int firewait2;
//	static int firewait3;
//
	my_wait -= (1)/*fps_fa ctor*/;
//
	BOSS01_DATA *data = (BOSS01_DATA *)src->data;
	switch (data->state1)
	{
	case ST_00:
	//	x256		= (GAME_WIDTH*256/2); // /*t256*/256*(GAME_WIDTH/2-(/*obj_doll[0]->w*/62/2+/*obj_doll[-1]->w*/60/2+/*obj_doll[1]->w*/53/2)/2);/*???*/
	//	x256		= ( 256*( (GAME_WIDTH)-(62/2)-(60/2)-(53/2) ) )/(2);/*???*/  /*obj_doll[0]->w*/ /*obj_doll[-1]->w*/ /*obj_doll[1]->w*/
		src->x256	= t256( 146)+t256(62/2);
		src->y256	= t256(-100);
		data->state1++/* = ST_01*/;
//		firewait1	= 45;
//		firewait2	= 4;
//		firewait3	= 0;
		break;
	case ST_01:/*���ֈړ�*/
		src->y256 += t256(2)/**fps_fa ctor*/;
		if (src->y256 >= t256(40/*40*/) )
		{
			data->state1++/* = ST_02*/;
		//	((PLAYER_DATA *)player->data)->bo ssmode=B05_BE FORE_LOAD;
			((PLAYER_DATA *)player->data)->state_flag |= STATE_FLAG_10_IS_LOAD_SCRIPT;
		}
		break;
		/*�p������*/
	case ST_02:
	//	if (B00_NONE/*B01_BA TTLE*/==(((PLAYER_DATA *)player->data)->bo ssmode))
		if ( ((((PLAYER_DATA *)player->data)->state_flag) & STATE_FLAG_05_IS_BOSS) )
		{
			my_wait = 40;
			data->state1++/* = ST_03*/;
		}
		break;
	case ST_03: 	/*wait*/
		if (my_wait <= 0)
		{
			/* �v���C���[�e�󂯕t���A�R�[���o�b�N��o�^ */
			src->callback_hit_enemy = callback_hit_boss; 	/* �R�[���o�b�N�o�^ */
			data->state1++/* = ST_04*/;
		}
		break;
/*���S�Ɏp������*/
	case ST_04: 	/* �ړ��� */
		if (my_wait<=0)
		{	/* �ړ����������߂� */
			data->state8++;
			data->state8 &= (8-1);
			{
				{	u16 my_rand;
					my_rand = ra_nd();
					data->vx	= ((my_rand&0x02)-1)/*ppp[data->state8][PPP_00_VX]*/;
					data->vy	= (((my_rand>>8)&0x03)-2)/*ppp[data->state8][PPP_01_VY]*/;
						 if (src->x256< t256(		  0)+t256(48) ) 	{	data->vx =  (2);}
					else if (src->x256> t256(GAME_WIDTH)-t256(48) ) 	{	data->vx = -(2);}
				//	my_wait 	= ppp[data->state8]/*[PPP_02_WAIT_DIV_2]*/; 	/* 50*4 60 �ړ��� */
					my_wait 	= (my_rand & 0xff); 	/* 50*4 60 �ړ��� */	/*[PPP_02_WAIT_DIV_2]*/
				//	my_wait 	+= my_wait;
				//	if (0!=ppp[data->state8][PPP_03_IS_RESET_ANIME])
					{
					//	if (2==ppp[data->state8][PPP_03_IS_RESET_ANIME])	/* �U���A�j���[�V���� */
						if (0==(my_rand&0x04))
						{
							alice_anime_count = 48;
							data->boss_base.danmaku_test++;
							data->boss_base.danmaku_test &= 0x0f; 		/*test*/
							data->boss_base.danmaku_type		= data->boss_base.danmaku_test/*DANMAKU_01*/;	/* �S�e�����Z�b�g */
							data->boss_base.danmaku_time_out	= (DANMAKU_01_SET_TIME+DANMAKU_01_SET_TIME);	/* �S�e���̔������� x 2 */
						}
						vvv256=1;
					}
				}
			}
		}
		break;
	}
	/*---------------------------------------------------------
		�p�`�F�ړ�����
	---------------------------------------------------------*/
	src->vx256 = (data->vx)*vvv256;
	src->vy256 = (data->vy)*vvv256;
	alice_animation(src);
	boss_move96(src);
//	move_all_doll(src);
//
	danmaku_generator(src); 	/* �e������ */
//
}

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

void add_boss_pache(STAGE_DATA *l)/*int lv*/
{
//	int lv;	lv	= l->user_y;
//	b01level			= lv+1;
	alice_anime_count	= 0;
//

//----[ZAKO]
//	bb_angle512 	= 128-43/*CCW*/ /*CW 43*/;/*512/12==42.66*/
	bb_angle512 	= 0;/*128*/ /*CCW*/ /*CW 43*/ /*512/12==42.66*/

	common_boss_flags	= (FLG_ALL_CAST);

//----[BOSS]
	SPRITE *sakuya;
	sakuya									= sprite_add_res(BASE_BOSS_PACHE_PNG); /*"boss05-mo.png"60x42, offset*/
	sakuya->flags							|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
	sakuya->anim_frame						= 0;
	sakuya->type							= SP_BOSS/*SP_BOSS01*/;
	sakuya->callback_mover					= move_alice;
	sakuya->callback_loser					= lose_boss;
//	sakuya->callback_hit_enemy				= callback_hit_boss;
	sakuya->callback_hit_enemy				= NULL; 	/* �_�~�[�R�[���o�b�N�o�^ */
//
	{
		BOSS01_DATA *data;
		data								= mmalloc(sizeof(BOSS01_DATA));
		sakuya->data						= data;
		//
		/*data->boss_base.boss_*/sakuya->base_health		= ((1024-1)*3);	// �p�`�F�{�̂�HP�B�������������Ă����������B// [***090305	�ύX
//		data->boss_base.boss_life			= (0);
		/*data->boss_base.*/sakuya->base_score				= adjust_score_by_difficulty(score( 100000));	/*	10�� x 1�l (�v100��==(1�lx10��)+(6�lx5��)+(2�lx30��)) */
//		/*data->boss_base.*/sakuya->base_score				= score(1000)*(difficulty+1);
		data->state1						= ST_00;
		data->state8						= 0;
//		data->move_type 					= ALICE_01_KEITAI;
		data->vx = ( 0);
		data->vy = ( 0);

	//	/*data->boss_base.*/spell_card_boss_timer		= (40*64);			/* 40*64==40[count] 	��40[�b(64/60)](�P�ʂ͕b�ł͂Ȃ�) */
	//	/*data->boss_base.*/spell_card_boss_timer		= ((99-24)*64);		/* 75*64==75[count] 	��75[�b(64/60)](�P�ʂ͕b�ł͂Ȃ�) */
		/*data->boss_base.*/spell_card_boss_timer		= ((120)*64);		/* 75*64==75[count] 	��120[�b(64/60)](�P�ʂ͕b�ł͂Ȃ�) */
		//
		#if 1
	//------------ �e���֘A
		data->boss_base.danmaku_type		= 0;
		data->boss_base.danmaku_time_out	= 0;
		data->boss_base.danmaku_test		= (DANMAKU_08_rumia-1)/*0*/;
		#endif
	//------------ �X�y�J�֘A
		#if 1
		/* [�X�y�J�V�X�e�����Ɉړ��\��] */
		/* ����̓o�^����ĂȂ��̂Ŏ蓮 */
		create_spell_card_init_dummy();
		spell_card_mode 		= 1/*on*/;
		#endif
//
		((PLAYER_DATA *)player->data)->boss = sakuya;
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
		obj_doll[i] 						= sprite_add_res(BASE_BOSS_PACHE_DOLLS_PNG/*doll_res_num[i]*/);
//		obj_doll[i]->anim_speed 			= 0;
		obj_doll[i]->type					= SP_ZAKO/*SP_BOSS05*/;
		obj_doll[i]->flags					|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
//
		obj_doll[i]->callback_loser 		= lose_doll;
		obj_doll[i]->callback_hit_enemy 	= callback_hit_zako;
//
		DOLL_DATA *data;
		data								= mmalloc(sizeof(DOLL_DATA));
		#if (1==USE_KEISYOU)
		data->sakuya_obj					= sakuya;	/* �p�������� */
		#endif
		obj_doll[i]->data					= data;
		data->identity_bit					= (FLG_DOLL0<<i);		/* �ŗL�r�b�g�ԍ� */
//		data->health_flag					= 0;
		data->bwait 						= 0;
	//	br1_angle512						= deg_360_to_512(0);
	//	br2_angle512						= deg_360_to_512(0);
//		data->br_angle512					= deg_360_to_512(0);/*CW*/
		data->br_angle512					= (0);/*CCW*/
	//	/*data->base.*/obj_doll[i]->base_score					= score( 500)*(difficulty+1);
		if (3/*1*/<i)
		{
			obj_doll[i]->callback_mover 	= move_doll01;
			obj_doll[i]->anim_frame 		= 0x20+((i&1)<<4);		/* �~�j�l�` / �g�����̐l�` */
			/*data->base.*/obj_doll[i]->base_score				= adjust_score_by_difficulty(score(  50000));	/*	 5�� x 4�l */
			/*data->base.*/obj_doll[i]->base_health 			= ((128-1));/*��������*/
		}
		else
		{
			obj_doll[i]->callback_mover 	= move_doll02;
			obj_doll[i]->anim_frame 		= (i<<4);				/*	0:�������̐l�`(������) / 1:�ɂ̖��̐l�`(���E��) */
			/*data->base.*/obj_doll[i]->base_score				= adjust_score_by_difficulty(score( 300000));	/*	30�� x 4�l */
			/*data->base.*/obj_doll[i]->base_health 			= ((1024-1)*5);/*�d��(�������Ɖ󂹂�)*/
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
