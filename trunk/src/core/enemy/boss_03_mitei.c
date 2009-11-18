
#include "bullet_object.h"

/*---------------------------------------------------------
	3�� ����(���A�P��A������)
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

typedef struct /*_boss04_data*/
{
	BOSS_BASE boss_base;
//------------ �ړ��֘A
	int state1; 		/* �`�ԃX�e�[�g */
	int state8; 		/* �e���X�e�[�g */
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

//	int state;
//	int health_flag;
//	int level;

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

enum
{
/*0*/	ALICE_01_KEITAI = 0,	/* ���`��: �P��o�� */
/*0*/	ALICE_02_KEITAI,		/* ���`��: �S���ōU�� */
/*1*/	ALICE_03_KEITAI,		/* ��O�`��: �t�H�[���V����2 */
/*2*/	ALICE_04_KEITAI,		/* ��l�`��: �P��P�� */
};
enum
{
	ST_00 = 0,
	ST_01,
	ST_02,
	ST_03,
	ST_04,
};

/*---------------------------------------------------------

---------------------------------------------------------*/

//static int rd4_zero;



//----[ZAKO]

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
	[�X�y�J�V�X�e�����Ɉړ��\��]	�X�y�J���j��A�C�e���o��
---------------------------------------------------------*/
	#if (0)
static void mitei_put_items(SPRITE *src)
{
	common_boss_put_items(src);
}
	#else
		#define mitei_put_items common_boss_put_items
	#endif

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
	play_voice_auto_track(VOICE14_YOKAI1_E_SHOT01);
	#else
	voice_play(VOICE14_YOKAI1_E_SHOT01, TRACK04_TEKIDAN);/*�e�L�g�[*/
	#endif
//
	int hari_no_iro;
	hari_no_iro = BULLET_HARI32_00_AOI;
#if (0)
	/* 4�ʂ̏ꍇ---- */
	if (0==rd4_zero/*0==data->level*/)
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
	//	hari_no_iro = iro_tbl[(((data->boss_base.boss_health)&(0xfc00) )>>10)];
		hari_no_iro = iro_tbl[(data->boss_base.boss_life)];
	}
#endif
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

static void enemy_boss04_LR_dole_fire(void)
{
	if (0!=(common_boss_flags&FLG_DOLL1)/*obj_doll[3]->flags&SP_FLAG_VISIBLE*/)	/*LEFT 0*/
	{
		bullet_create_aka_maru_jikinerai(obj_doll[1], t256(difficulty+1)/*5*/);
	//	bullet_create_n_way_dan_sa_type(obj_doll[1], t256(difficulty+1)/*5*/, ANGLE_JIKI_NERAI_DAN, (int)(512/24), BULLET_MARU8_00_AKA, 1);
	}
	if (0!=(common_boss_flags&FLG_DOLL2)/*obj_doll[5]->flags&SP_FLAG_VISIBLE*/)	/*RIGHT 1*/
	{
		bullet_create_aka_maru_jikinerai(obj_doll[2], t256(difficulty+1)/*5*/);
	//	bullet_create_n_way_dan_sa_type(obj_doll[2], t256(difficulty+1)/*5*/, ANGLE_JIKI_NERAI_DAN, (int)(512/24), BULLET_MARU8_00_AKA, 1);
	}
}

/*---------------------------------------------------------
	���E�V���b�g
---------------------------------------------------------*/

static void kaguya_sayuu_shot(SPRITE *src)
{
	BOSS04_DATA *data = (BOSS04_DATA *)src->data;
//
//	if (ST_03 < data->state1)	/*((BOSS04_DATA *)sakuya->data)*/ /*data->state*/
//	if (ST_03 < ((BOSS04_DATA *)src->data)->state1) /*((BOSS04_DATA *)sakuya->data)*/ /*data->state*/
	if (ST_03 < (data->state1)) /*((BOSS04_DATA *)sakuya->data)*/ /*data->state*/
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
			else
			{
				/* 3�ʂ̏ꍇ���E������ */
#if (0)
				if (0!=rd4_zero/*0!=data->level*/)
#endif
				{
					enemy_boss04_LR_dole_fire();/*enemy_boss04_fire(0);enemy_boss04_fire(1);*/
				}
			}
		}
	}
}


/*---------------------------------------------------------

---------------------------------------------------------*/

// [***090114		���낢��ƒǉ����܂��B
//3�ʂ�4�ʂ̍��ʉ��B
//4�ʂɂ͐V�����ǉ������e�O����g�ݍ��ށB
static void kaguya_01_keitai(SPRITE *src)
{
	static int my_wait;
//	static int firewait1;
//	static int firewait2;
//	static int firewait3;
//	static int bomb_n;
//	int i;
//
	my_wait -= (1)/*fps_fa ctor*/;
//
	BOSS04_DATA *data = (BOSS04_DATA *)src->data;
	switch (/*data->state*/data->state1)
	{
	case ST_00:
	//	src->x256	= (GAME_WIDTH/2)-((obj_doll[1]->w128+sakuya->w128+obj_doll[2]->w128)>>8); //�E�B���h�E���̕ύX
	//	src->x256	= t256((GAME_WIDTH/2)-((34+54+34)/2));	// �E�B���h�E���̕ύX
		src->x256	= t256( 146)+t256(34/2);		// �E�B���h�E���̕ύX
		src->y256	= t256(-100);
		src->vx256	= t256( 0);
		src->vy256	= t256( 2)/**fps_fa ctor*/;
		data->state1++/* = ST_01*/;
	//	firewait1	= 45;
	//	firewait2	= 4;
	//	firewait3	= 0;
	//	bomb_n		= 0;
		break;
	case ST_01:/*���ֈړ�*/
		if (src->y256 >= t256(40/*30*/) )
		{
			src->vy256 = t256( 0);
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
	case ST_04:
		if (my_wait<= 0)
		{
			data->state8++;
			data->state8 &= (4-1);
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
					{( 2),(-1),(100),( 0),},	/*�E���*/
				//	{( 0),( 0),( 50),( 2),},	/*wait*/
					{( 2),( 1),(100),( 0),},	/*�E����*/
				//	{( 0),( 0),( 10),( 0),},	/*wait*/
					{(-2),(-1),(100),( 1),},	/*�����*/
				//	{( 0),( 0),( 50),( 2),},	/*wait*/
					{(-2),( 1),(100),( 0),},	/*������*/
				//	{( 0),( 0),( 10),( 0),},	/*wait*/
				};
			//						src->vx256 = t256(-2); src->vy256 = t256(-1);
			//	center_shot(src);	src->vx256 = t256(-2); src->vy256 = t256( 1);
			//						src->vx256 = t256( 2); src->vy256 = t256(-1);
			//	center_shot(src);	src->vx256 = t256( 2); src->vy256 = t256( 1);
			//
				src->vx256	= ((ppp[data->state8][PPP_00_VX])<<8);
				src->vy256	= ((ppp[data->state8][PPP_01_VY])<<8);
				my_wait=60;
				/* 4�ʂ̏ꍇ---- */
				#if (0)
			//	if (0!=ppp[data->state8][PPP_03_IS_RESET_ANIME])
			//	{
			//		center_shot(src);
			//	}
				#endif
			}
		}
		break;
	}
}


/*---------------------------------------------------------
	[�X�y�J�V�X�e�����Ɉړ��\��]	�X�y�J�o�^
---------------------------------------------------------*/

static void regist_spell_card(SPRITE *src)
{
	spell_card_limit_health -= 1000/*500*/;
	if (0 >= spell_card_limit_health)
	{
		spell_card_limit_health = 0;
		spell_card_mode 		= 0/*off*/;
	}
	else
	{
	//	BOSS06_DATA *data = (BOSS06_DATA *)src->data;
		spell_card_mode 		= 1/*on*/;
//		spell_card_number++;
//		if (SAKUYA_10_KEITAI < spell_card_number)
//		{
//			spell_card_number--;
//		}
		mitei_put_items(src);/**/
	}
}


/*---------------------------------------------------------
	[�X�y�J�V�X�e�����Ɉړ��\��]	�X�y�J����
---------------------------------------------------------*/

static void spell_card_generator(SPRITE *src)
{
//	BOSS06_DATA *data = (BOSS06_DATA *)src->data;
//	switch (spell_card_number)
	{
		kaguya_01_keitai(src);
	}
	/*---------------------------------------------------------
		�p�`�F�ړ�����
	---------------------------------------------------------*/
	//
	//	enemy_boss04_setpos(src, xxx,yyy);
	//
	boss_move96(src);

	/*---------------------------------------------------------
		�X�y�J�`�F�b�N
	---------------------------------------------------------*/
	{
		if (0/*off*/==spell_card_mode)
		{
			if (0/*off*/==spell_card_boss_timer)
			{
				spell_card_limit_health = 0;
				spell_card_mode 		= 1/*on*/;
			}
		}
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void move_mitei(SPRITE *src)
{
	/* �X�y�J�o�^ */
			if (0/*off*/==spell_card_mode)
			{
				regist_spell_card(src);
			}
	spell_card_generator(src);	/* �X�y�J���� */
//
	kaguya_sayuu_shot(src); 	/* �e������ */
//	kaguya_pong_boll(src);		/* �e������ */	/* 4�ʂ̏ꍇ���˂�� */
//
}

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

void add_boss_mitei(STAGE_DATA *l)/*int lv*/
{
//----[ZAKO]
	bb_angle512 	= 0;

	common_boss_flags	= (FLG_ALL_CAST);

//----[BOSS]
		SPRITE *sakuya;
	{
		sakuya									= sprite_add_res(BASE_BOSS_KAGUYA_PNG);
		sakuya->flags							|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
	//	sakuya->anim_speed						= 0;
		sakuya->yx_anim_frame					= 0/*0*/+(1/*rd4_zero*/<<4);
		sakuya->type							= SP_BOSS/*SP_BOSS03*/;
		sakuya->callback_mover					= move_mitei;
		sakuya->callback_loser					= lose_boss;
	//	sakuya->callback_hit_enemy				= callback_hit_boss;
		sakuya->callback_hit_enemy				= NULL; /* �_�~�[�R�[���o�b�N�o�^ */
	//
		BOSS04_DATA *data;
		data									= mmalloc(sizeof(BOSS04_DATA));
		sakuya->data							= data;
//		data->identity							= 0/*i*/;		// �ŗL�ԍ�
//		data->health_flag						= 0;
		//
		/* 3�ʂ̏ꍇ---- */ 	/* �S���ꏏ */
	//	/*data->boss_base.boss_*/sakuya->base_health	= (difficulty<<6/*[x64]*/)+ 200+1024;
	//	/*data->boss_base.boss_*/sakuya->base_health	= (1024-1);
	//	data->boss_base.boss_life						= (((difficulty<<6/*[x64]*/)+ 200+1024)>>10);
		/*data->boss_base.boss_*/sakuya->base_health	= (((difficulty<<6/*[x64]*/)+ 200+1024) );/*test*/
	//	/*data->boss_base.*/sakuya->base_score			= score(500)*(difficulty+1);
		/*data->boss_base.*/sakuya->base_score			= adjust_score_by_difficulty(score( 500000));	/* 50�� (�v300��==6x50��) */
		//
		data->state1						= ST_00;
		data->state8						= (0);
		/*data->boss_base.*/spell_card_boss_timer		= (40*64);			/* 40*64==40[count] 	��40[�b(64/60)](�P�ʂ͕b�ł͂Ȃ�) */
	//	/*data->boss_base.*/spell_card_boss_timer		= ((20)*64);		/* 75*64==75[count] 	��20[�b(64/60)](�P�ʂ͕b�ł͂Ȃ�) */
		// 60 [�b] ???
		#if 1
	//------------ �e���֘A
		#endif
	//------------ �X�y�J�֘A
		#if 1
		/* [�X�y�J�V�X�e�����Ɉړ��\��] */
		/* ����̓o�^����ĂȂ��̂Ŏ蓮 */
		create_spell_card_init_dummy();
		spell_card_mode 		= 1/*on*/;
		spell_card_limit_health = (sakuya->base_health)-500/*1000*/;/* �ʏ�U��(����U��)�̍U����(�蓮�ݒ�) */
		#endif
//
		((PLAYER_DATA *)player->data)->boss = sakuya;/*�P��{�l*/
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

		obj_doll[i]->yx_anim_frame			= i/*0*/+(1/*rd4_zero*/<<4);
//
		DOLL_DATA *data;
		data								= mmalloc(sizeof(BOSS04_DATA));
		#if (1==USE_KEISYOU)
		data->sakuya_obj					= sakuya;	/* �p�������� */
		#endif
		obj_doll[i]->data					= data;
		data->identity_bit					= (FLG_DOLL0<<i);		/* �ŗL�r�b�g�ԍ� */
//		data->health_flag					= (0);
		/*data->base.*/obj_doll[i]->base_health 				= (1024-1);
//
		obj_doll[i]->callback_mover 		= move_doll01;
#if (0)
		/* 4�ʂ̏ꍇ---- */
		if (0==rd4_zero)		// [***090114		�ǉ�
		{
		//	data->base.health				= (difficulty<<8/*[x256]*/)+ 200+1024;
//			data->base.life 				= (((difficulty<<8/*[x256]*/)+ 200+1024)>>10);
		//	data->base.score				= score(600)*(difficulty+1);
			/*data->base.*/obj_doll[i]->base_score				= adjust_score_by_difficulty(score( 400000));	/* 40�� */
		}
		else/* �S���ꏏ */
#endif
		/* 3�ʂ̏ꍇ---- */ 	/* �S���ꏏ */
		{
		//	data->base.health				= (difficulty<<6/*[x64]*/)+ 200+1024;
//			data->base.life 				= (((difficulty<<6/*[x64]*/)+ 200+1024)>>10);
		//	/*data->base.*/obj_doll[i]->base_score				= score(500)*(difficulty+1);
			/*data->base.*/obj_doll[i]->base_score				= adjust_score_by_difficulty(score( 500000));	/* 50�� (�v300��==6x50��) */
		}
		data->fix_angle512					= jj_angle512;
		jj_angle512 += (AA_OFS85);
	//
	}
}
