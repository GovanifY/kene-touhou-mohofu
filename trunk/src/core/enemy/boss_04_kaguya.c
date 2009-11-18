
#include "bullet_object.h"

/*---------------------------------------------------------
	�H���R �P��
	������
	-------------------------------------------------------
	ToDo:
	4�ʂ̂�(3�ʂɂ͑Ή����܂���)
	�{�X�^�C�}�[�Ή���
---------------------------------------------------------*/

/*
C++�݂����� �p�� �����Ă݂�B
���A�悭�l�����猻��global��player�ɂ���̂�
�v��Ȃ������̂�OFF�B�ł����Ƃł��̍l���͎g�������B
 */
#define USE_KEISYOU 0

typedef struct /*_boss04_data*/
{
	BOSS_BASE boss_base;
//------------ �ړ��֘A
	int state1; 		/* �`�ԃX�e�[�g */		/* �e���X�e�[�g */
} BOSS04_DATA;

typedef struct /*_boss04_data*/
{
//	ENEMY_BASE base;
//
	#if (1==USE_KEISYOU)
	SPRITE *sakuya_obj; /* �p�� */
	#endif
	int identity_bit;	/* �ŗL�r�b�g�ԍ� */
	int fix_angle512;	/* �Ă��ƌŒ�l */
//------------ doll02��p
} DOLL_DATA;

// FLG_FIRE1 �e�t���O�B�^�񒆂̕󕨂����Ă��āA���w��ʒu�ɕt�������Bon�ɂȂ�B
//#define FLG_FIRE1 0x01/*����̗�����(��)���w��ʒu�ɕt������*/
//#define FLG_FIRE2 0x02/*�E��̕ɂ̖�(��)���w��ʒu�ɕt������*/
//
#define FLG_DOLL0 0x04/*�P��{�l*/
#define FLG_DOLL1 0x08/*����̗�����(��)*/
#define FLG_DOLL2 0x10/*�E��̕ɂ̖�(��)*/
#define FLG_DOLL3 0x20/*���O�̕�*/
#define FLG_DOLL4 0x40/*�^���̍g����(��)*/
#define FLG_DOLL5 0x80/*�E�O�̕�*/

#define FLG_ALL_DOLL	(FLG_DOLL1|FLG_DOLL2|FLG_DOLL3|FLG_DOLL4|FLG_DOLL5)/*�P��ȊO*/
#define FLG_ALL_CAST	(FLG_ALL_DOLL)/*�S��*/


//#define FLG_ALL_CAST	(FLG_DOLL0|FLG_ALL_DOLL)/*�S��*/

/*---------------------------------------------------------

---------------------------------------------------------*/


//----[ZAKO]
#if 1
/* �\���̂̊O�ɂ���K�R�������� */
#endif
static int bb_angle512;

static SPRITE *obj_doll[8]; 	/* �l�`�B */

/*---------------------------------------------------------
�l���̔z�u����
[1][0][2]
[3][4][5]
---------------------------------------------------------*/

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

	#if 0/*???*/
	bakuhatsu_add_zako04(src);
	#endif

	#if 0/*???*/
	player_add_score(data->base.score);/*�P��ȊO�̏����̃X�R�A*/
	#endif


/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_doll01(SPRITE *src)
{
	DOLL_DATA *data = (DOLL_DATA *)src->data;
	{
		bb_angle512++;
	int aa_angle512;
		aa_angle512 = bb_angle512;
		{
		//	aa_angle512 += (102);/* 102.4== 512/5 */
			aa_angle512 += data->fix_angle512;/* 102.4== 512/5 */
			mask512(aa_angle512);
			/*CCW*/
			src->x256 = (((PLAYER_DATA *)player->data)->boss)->x256 + ((sin512((aa_angle512))*45));
			src->y256 = (((PLAYER_DATA *)player->data)->boss)->y256 + ((cos512((aa_angle512))*45));
		}
	}
	#if 1
	/* �P���|���ƊF�j�󂳂��B */
	if (0==common_boss_flags)
	{
		src->type = SP_DELETE;
	}
	#endif
}


/*--------------------------------------*/
/*--------------------------------------*/
/*--------------------------------------*/

/*---------------------------------------------------------

---------------------------------------------------------*/

/*int where / 0: left, 1: right, 2: bombenhagel */
//	switch (where)//	case 2:
static void enemy_boss01_nway_fire(SPRITE *src)
{
//	BOSS04_DATA *data = (BOSS04_DATA *)src->data;
	static int hari=0;
	hari--;
	hari &= 32;
	#if (0==USE_DESIGN_TRACK)
	play_voice_auto_track(VOICE14_BOSS_KOUGEKI_01);
	#else
	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);/*�e�L�g�[*/
	#endif
//
	int hari_no_iro;
	hari_no_iro = BULLET_HARI32_00_AOI;
	/* 4�ʂ̏ꍇ---- */
#if (0)
	if (0==rd4_zero/*0==data->en emy_rank*/)
#endif
	{
		unsigned short iro_tbl[8] =
		{
/**/		BULLET_HARI32_01_AKA,
/*1*/		BULLET_HARI32_01_AKA,
/*2*/		BULLET_HARI32_03_DAIDAI,
/*3*/		BULLET_HARI32_02_KIIRO,
/*4*/		BULLET_HARI32_00_AOI,
/*5*/		BULLET_HARI32_00_AOI,
/**/		BULLET_HARI32_00_AOI,
/**/		BULLET_HARI32_00_AOI,
		};
		hari_no_iro = iro_tbl[(((src->base_health)&(0xfc00) )>>10)];
	//	hari_no_iro = iro_tbl[(data->boss_base.bo ss_life)];
	}
//
	int angle512;
//	for (angle512=deg_360_to_512(0); angle512</*=*/deg_360_to_512(360/*180*/);
	for (angle512=deg_360_to_512CCW(0); angle512</*=*/deg_360_to_512CCW(360/*180*/);
	//	angle512+=/*22*/21+hari-difficulty/*deg_360_to_512(20)*/		/* 23�{(360) �� 8�{(180) */
		angle512+=(/*22*/18+hari-difficulty-difficulty/*deg_360_to_512(20)*/)		/* �Œ� == 10 == 48�{(360)	*/
		)
	{
		bullet_create_offset_dan_type000(
			src,//(((PLAYER_DATA *)player->data)->boss)/*obj_doll[4]*/,
			(int)(/*6*/ (0x100)+(ra_nd()&0x2ff)/*t256(2.5)*/),
			angle512,
		//	/*offsx*/t256(0)/*(src->w/2)*/,
		//	/*offsy*/t256(0)/*(src->h/2)*/,
			hari_no_iro
		);
	}
}

/*---------------------------------------------------------
	3�ʐ�p
---------------------------------------------------------*/
#if (0)
static void enemy_boss04_LR_dole_fire(void)
{
	if (0!=(common_boss_flags&FLG_DOLL1)/*obj_doll[3]->flags&SP_FLAG_VISIBLE*/) /*LEFT 0*/
	{
		bullet_create_aka_maru_jikinerai(obj_doll[1], t256(difficulty+1)/*5*/);
	//	bullet_create_n_way_dan_sa_type(obj_doll[1], t256(difficulty+1)/*5*/, ANGLE_JIKI_NERAI_DAN, (int)(512/24), BULLET_MARU8_00_AKA, 1);
	}
	if (0!=(common_boss_flags&FLG_DOLL2)/*obj_doll[5]->flags&SP_FLAG_VISIBLE*/) /*RIGHT 1*/
	{
		bullet_create_aka_maru_jikinerai(obj_doll[2], t256(difficulty+1)/*5*/);
	//	bullet_create_n_way_dan_sa_type(obj_doll[2], t256(difficulty+1)/*5*/, ANGLE_JIKI_NERAI_DAN, (int)(512/24), BULLET_MARU8_00_AKA, 1);
	}
}
#endif

/*---------------------------------------------------------
	4�ʐ�p
---------------------------------------------------------*/

static void center_shot(SPRITE *src)
{
//	BOSS04_DATA *data = (BOSS04_DATA *)src->data;
	/* 4�ʂ̏ꍇ---- */
#if (0)
	if (0==rd4_zero/*0==data->en emy_rank*/)
#endif
	{
		int kakudo;
		kakudo = (127-16);
		kakudo += ((ra_nd())&(32-1));
		switch ((src->base_health)&(0xfc00) )
	//	switch (data->boss_base.bo ss_life)
		{
		case (3<<10):/*not_break;*/
			bullet_create_n_way_dan_sa_type(src, t256(0.6), kakudo, (int)(512/24), BULLET_UROKO14_03_MIDORI,	24);
			bullet_create_n_way_dan_sa_type(src, t256(1.4), kakudo, (int)(512/24), BULLET_UROKO14_05_KIIRO, 	24);
		case (2<<10):/*not_break;*/
		case (4<<10):/*not_break;*/
			bullet_create_n_way_dan_sa_type(src, t256(1.0), kakudo, (int)(512/24), BULLET_UROKO14_00_AOI,		24);
			bullet_create_n_way_dan_sa_type(src, t256(1.8), kakudo, (int)(512/24), BULLET_UROKO14_04_MIZUIRO,	24);
		case (5<<10):/*not_break;*/
		case (1<<10):/*not_break;*/
			bullet_create_n_way_dan_sa_type(src, t256(1.2), kakudo, (int)(512/24), BULLET_UROKO14_02_YUKARI,	24);
			bullet_create_n_way_dan_sa_type(src, t256(0.8), kakudo, (int)(512/24), BULLET_UROKO14_01_AKA,		24);
			bullet_create_n_way_dan_sa_type(src, t256(2.0), kakudo, (int)(512/24), BULLET_UROKO14_01_AKA,		24);
			break;
		}
	}
}

/*---------------------------------------------------------
	���E�V���b�g
---------------------------------------------------------*/

static void kaguya_sayuu_shot(SPRITE *src)
{
	if (SPELL_CARD_00_kaguya_000 < spell_card_number)
	{
	static int firewait1=45;
		firewait1-=1;
		if (firewait1<=0)
		{
			firewait1=45;
			static unsigned int firewait2=0;
			firewait2--;
			firewait2 &= (4-1);
			if (0==firewait2)
			{
				enemy_boss01_nway_fire(src);/*enemy_boss04_fire(2);*/
			}
#if (0)
			else
			{
				/* 3�ʂ̏ꍇ���E������ */
				if (0!=rd4_zero/*0!=data->en emy_rank*/)
				{
					enemy_boss04_LR_dole_fire();/*enemy_boss04_fire(0);enemy_boss04_fire(1);*/
				}
			}
#endif
		}
	}
}

/*---------------------------------------------------------
	4�ʂ̏ꍇ���˂��
---------------------------------------------------------*/

static int firewait3;
static int bomb_n;
static void kaguya_pong_boll(SPRITE *src)
{
//	BOSS04_DATA *data = (BOSS04_DATA *)src->data;
	/* 4�ʂ̏ꍇ���˂�� */
#if (0)
	if (0==rd4_zero/*0==data->en emy_rank*/)
#endif
	{
		//switch ( (src->base_health)&0xfc00 )
		if (src->base_health <= 1000+1024)
		//if (data->boss_base.bo ss_life < 2)
		{
		//case (0<<10):
			{	firewait3--;}
			if (firewait3 < 0)
			{
				if (200+1024 < src->base_health)
			//	if (0 < data->boss_base.bo ss_life)
				{	firewait3 = ((src->base_health)>>2);}
				else
				{	firewait3 = 50;}
			//
				if (bomb_n < 32)
				{	bomb_n++;}
				int i;
				for (i=0; i<=(int)(bomb_n/3); i++)
				{
					bullet_create_hazumi_dan(src,
						t256(0.05)+(difficulty<<6), 		/* speed256 t256(5.0) */
						(/*(M_PI*2)*/512*3*i/(bomb_n+1))	/* angle512 */
															/*+ra d2deg512(1)*/, /*���������ʏ����Ȃ������~�X*/
						(t256(0.07)),						/* delta256 */	/*17.92==t256(0.07)*/
						difficulty/*2*/ 					/* bound_counts */
					);		/* [***090116		������ */
				}
			}
		//	else
		//	{	firewait3--;}
		//	break;
		}
	}
}

/*---------------------------------------------------------
	���㉺�ړ�
---------------------------------------------------------*/

/*static*/ void kaguya_01_keitai(SPRITE *src)
{
	static int my_wait;
	my_wait -= (1)/*fps_fa ctor*/;
//
	BOSS04_DATA *data = (BOSS04_DATA *)src->data;
	{
		if (my_wait <= 0)
		{	/* �ړ����������߂� */
			data->state1++;
			data->state1 &= (4-1);
			{
				enum
				{
					dummy_PPP_00_VX = 0,			/* x �x�N�g���ړ��� */
					PPP_01_VY,				/* y �x�N�g���ړ��� */
					PPP_02_WAIT_DIV_2,		/* �E�F�C�g�J�E���^�̔����� */
					PPP_03_IS_RESET_ANIME,	/* �A�j���[�V�������Z�b�g 0:���Ȃ� 1:���� 2:����(�e������) */
				};
				s8 ppp[/*8*/4][4] =
				{
					{( 2),(-1),(100),( 1),},	/*�E���*/
				//	{( 0),( 0),( 50),( 2),},	/*wait*/
					{( 2),( 1),(100),( 3),},	/*�E����*/
				//	{( 0),( 0),( 10),( 0),},	/*wait*/
					{(-2),(-1),(100),( 0),},	/*�����*/	/* [�E��] */
				//	{( 0),( 0),( 50),( 2),},	/*wait*/
					{(-2),( 1),(100),( 2),},	/*������*/
				//	{( 0),( 0),( 10),( 0),},	/*wait*/
				};
			//						src->vx256 = t256(-2); src->vy256 = t256(-1);
			//	center_shot(src);	src->vx256 = t256(-2); src->vy256 = t256( 1);
			//						src->vx256 = t256( 2); src->vy256 = t256(-1);
			//	center_shot(src);	src->vx256 = t256( 2); src->vy256 = t256( 1);
			//
//				src->vx256	= ((ppp[data->state1][PPP_00_VX])<<8);
//				src->vy256	= ((ppp[data->state1][PPP_01_VY])<<8);
//				src->vx256	= (0);
				src->vy256	= ((ppp[data->state1][PPP_01_VY])<<4);
				my_wait=60;
				/* 4�ʂ̏ꍇ---- */
				#if (1)
			//	if (0!=ppp[data->state1][PPP_03_IS_RESET_ANIME])
				if (difficulty >= ppp[data->state1][PPP_03_IS_RESET_ANIME])
				{
					if (ra_nd()&1)		/* 50%�̊m���� */
					{
						center_shot(src);
					}
				}
				#endif
			}
		}
	}
}

/*---------------------------------------------------------
	�΂߈ړ�
---------------------------------------------------------*/

/*static*/ void kaguya_04_keitai(SPRITE *src)
{
	static int my_wait;
	my_wait -= (1)/*fps_fa ctor*/;
//
	BOSS04_DATA *data = (BOSS04_DATA *)src->data;
	{
		if (my_wait <= 0)
		{	/* �ړ����������߂� */
			data->state1++;
			data->state1 &= (4-1);
			{
				enum
				{
					PPP_00_VX = 0,			/* x �x�N�g���ړ��� */
					PPP_01_VY,				/* y �x�N�g���ړ��� */
					PPP_02_WAIT_DIV_2,		/* �E�F�C�g�J�E���^�̔����� */
					PPP_03_IS_RESET_ANIME,	/* �A�j���[�V�������Z�b�g 0:���Ȃ� 1:���� 2:����(�e������) */
				};
				s8 ppp[/*8*/4][4] =
				{
					{( 2),(-1),(100),( 1),},	/*�E���*/
				//	{( 0),( 0),( 50),( 2),},	/*wait*/
					{( 2),( 1),(100),( 3),},	/*�E����*/
				//	{( 0),( 0),( 10),( 0),},	/*wait*/
					{(-2),(-1),(100),( 0),},	/*�����*/	/* [�E��] */
				//	{( 0),( 0),( 50),( 2),},	/*wait*/
					{(-2),( 1),(100),( 2),},	/*������*/
				//	{( 0),( 0),( 10),( 0),},	/*wait*/
				};
			//						src->vx256 = t256(-2); src->vy256 = t256(-1);
			//	center_shot(src);	src->vx256 = t256(-2); src->vy256 = t256( 1);
			//						src->vx256 = t256( 2); src->vy256 = t256(-1);
			//	center_shot(src);	src->vx256 = t256( 2); src->vy256 = t256( 1);
			//
				src->vx256	= ((ppp[data->state1][PPP_00_VX])<<8);
				src->vy256	= ((ppp[data->state1][PPP_01_VY])<<8);
				my_wait=60;
				/* 4�ʂ̏ꍇ---- */
				#if (1)
			//	if (0!=ppp[data->state1][PPP_03_IS_RESET_ANIME])
				if (difficulty >= ppp[data->state1][PPP_03_IS_RESET_ANIME])
				{
					if (ra_nd()&1)		/* 50%�̊m���� */
					{
						center_shot(src);
					}
				}
				#endif
			}
		}
	}
}


/*---------------------------------------------------------
	�{�X�s��
---------------------------------------------------------*/

static void move_kaguya(SPRITE *src)
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
	boss_effect(src);			/* ��G�t�F�N�g */
	kaguya_sayuu_shot(src); 	/* �e������ */
	kaguya_pong_boll(src);		/* �e������ */
}


/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/
//		data->identity							= 0/*i*/;		// �ŗL�ԍ�
//		data->health_flag						= 0;

//		data->boss_base.bo ss_life						= (difficulty)+2;
	//	sakuya->base_health = (difficulty<<10/*[x1024]*/)+2047+1024;
//		sakuya->base_health = 1024-1;
//		sakuya->base_health = ((1024)-1)*9;/*test*/
	//	sakuya->base_health = ((1024)-1)*((difficulty)+2);/*test*/
	//	sakuya->base_score			= score(3000)+score(2000)*difficulty;

	//	spell_card_boss_timer		= (40*64);			/* 40*64==40[count] 	��40[�b(64/60)](�P�ʂ͕b�ł͂Ȃ�) */
	//	spell_card_boss_timer		= ((99-24)*64); 	/* 75*64==75[count] 	��75[�b(64/60)](�P�ʂ͕b�ł͂Ȃ�) */
	//	firewait1	= 45;
	//	firewait2	= 4;
	//	sakuya->anim_speed					= 0;

void add_boss_kaguya(STAGE_DATA *l)/*int lv*/
{
	boss_bgm_mode		= (l->user_y);
//
	firewait3			= 0;
	bomb_n				= 0;

//----[ZAKO]
	bb_angle512 		= 0;

	common_boss_flags	= (FLG_ALL_CAST);

//----[BOSS]
		SPRITE *sakuya;
		sakuya								= sprite_add_res(BASE_BOSS_KAGUYA_PNG);
		sakuya->flags						|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
		sakuya->yx_anim_frame				= 0/*0*/+(0/*rd4_zero*/<<4);
		sakuya->type						= SP_BOSS/*SP_BOSS03*/;
		sakuya->callback_mover				= move_kaguya;
		sakuya->callback_loser				= common_boss_put_items;
//
		#if 0/* �������ς݂̕K�v���� */
		sakuya->vx256						= t256( 0);
		sakuya->vy256						= t256( 0); 	/**fps_fa ctor*/
		#endif
		/* 4�ʂ̏ꍇ---- */ 	/* �P��{�l */
	//	sakuya->base_health 				= ((1024)-1)*4;/*test*/
		sakuya->base_health 				= ((1024)-1)*5;/*test*/
		sakuya->base_score					= adjust_score_by_difficulty(score(2000000));	/* 200�� (�v400��==(200��)+(5x40��)) */
	//------------ �X�y�J�֘A
		spell_card_number					= SPELL_CARD_00_kaguya_000;
		spell_card_max						= SPELL_CARD_17_kaguya_hhh;
		spell_card_boss_init_regist(sakuya);
	{
		BOSS04_DATA *data;
		data								= mmalloc(sizeof(BOSS04_DATA));
		sakuya->data						= data;
		data->state1						= (0)/*ST_00*/;
		#if 1
	//------------ �e���֘A
		#endif
	}

//----[ZAKO]
	//	aa_angle512 += (102);/* 102.4== 512/5 */
	/* 85.33==512/6 170.66== 512/3 */	/*171*/
//	#define AA_OFS85 (85)
	#define AA_OFS85 (102)
	int jj_angle512;
	jj_angle512= -(int)(AA_OFS85/2);	/*0*/
	int i;
	for (i=1/*0*/; i<6; i++)
	{
		obj_doll[i] 						= sprite_add_res(BASE_BOSS_KAGUYA_PNG);
//		obj_doll[i]->anim_speed 			= 0;
		obj_doll[i]->type					= SP_BOSS/*SP_BOSS03*/;
		obj_doll[i]->flags					|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
//
		obj_doll[i]->callback_loser 		= lose_doll;
		obj_doll[i]->callback_hit_enemy 	= callback_hit_zako;

		obj_doll[i]->yx_anim_frame			= i/*0*/+(0/*rd4_zero*/<<4);
//
		DOLL_DATA *data;
		data								= mmalloc(sizeof(BOSS04_DATA));
		#if (1==USE_KEISYOU)
		data->sakuya_obj					= sakuya;	/* �p�������� */
		#endif
		obj_doll[i]->data					= data;
		data->identity_bit					= (FLG_DOLL0<<i);		/* �ŗL�r�b�g�ԍ� */
//		data->health_flag					= (0);
		obj_doll[i]->base_health			= (1024-1);
//
		obj_doll[i]->callback_mover 		= move_doll01;
		/* 4�ʂ̏ꍇ---- */
#if (0)
		if (0==rd4_zero)		// [***090114		�ǉ�
#endif
		{
		//	obj_doll[i]->base_health		= (difficulty<<8/*[x256]*/)+ 200+1024;
//			data->base.life 				= (((difficulty<<8/*[x256]*/)+ 200+1024)>>10);
		//	obj_doll[i]->base_score 		= score(600)*(difficulty+1);
			obj_doll[i]->base_score 		= adjust_score_by_difficulty(score( 400000));	/* 40�� */
		}
#if (0)
		/* 3�ʂ̏ꍇ---- */
		else/* �S���ꏏ */
		{
		//	obj_doll[i]->base_health		= (difficulty<<6/*[x64]*/)+ 200+1024;
//			data->base.life 				= (((difficulty<<6/*[x64]*/)+ 200+1024)>>10);
		//	obj_doll[i]->base_score 		= score(500)*(difficulty+1);
			obj_doll[i]->base_score 		= adjust_score_by_difficulty(score( 500000));	/* 50�� (�v300��==6x50��) */
		}
#endif
		data->fix_angle512					= jj_angle512;
		jj_angle512 += (AA_OFS85);
	//
	}
}
