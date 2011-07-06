
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�e�v���C���[(�얲 �� ������ �� ���~���A �� �`���m �� �H�X�q)
	REIMU(A/B) MARISA(A/B/C) REMILIA CIRNO YUYUKO
	-------------------------------------------------------
---------------------------------------------------------*/

#include "jiki_local.h"

/*static*/extern int weapon_level_offset; /* �����������탌�x��(�ԍ�)�̂W�{�Ƀv���C���[�l�𑫂����l���I�t�Z�b�g�Ƃ��ĕێ� */

/*---------------------------------------------------------
	�v���C���[�A�j�e�A�V���b�g�e�̈ړ�
	�얲 �� ������ �� ���~���A �� �`���m �� �H�X�q
---------------------------------------------------------*/

static void player_move_shot(SPRITE *src)
{
	src->cx256 += src->vx256;		/*fps_factor*/
	src->cy256 += src->vy256;		/*fps_factor*/
	if (REMILIA==select_player) 	/* ���~���A�͂���� */
	{	u16 rand_int;
		rand_int = ra_nd();
		src->cx256 -= (rand_int&0x0200);/*0x0100*/
		src->cx256 += (rand_int&0x0100);/*0x0080*/
		src->cx256 += (rand_int&0x0100);/*0x0080*/
	}
	else
	if (YUYUKO==select_player)		/* �H�X�q�͂���� */
	{
		src->vx256 += ((src->vx256*pd.weapon_power)>>11);
	}
	else
//	if (REIMU==select_player)			/* �얲�̉�]�V���b�g */
	if ((REIMU_B+1) > select_player)	/* �얲�̉�]�V���b�g */
	{
		if (((JIKI_SHOT_01)|SP_GROUP_SHOT_SPECIAL)==src->type)
		{
			/* �`��p�O����] */
			src->rotationCCW1024 += (32);	/* cv1024r(10) */
			mask1024(src->rotationCCW1024);
		}
	}
	if (src->cy256 < t256(-10))
	{
		src->jyumyou = JYUMYOU_NASI;
	}
	else
	{
		player_weapon_colision_check(src, PLAYER_WEAPON_TYPE_00_SHOT);
	}
}


/*---------------------------------------------------------

---------------------------------------------------------*/

enum
{
	REI00_x_offset = 0,
	REI01_y_offset,
	REI02_vx256,
	REI03_status,	/* 2==�얲�̉�]�V���b�g */
	REI04_MAX		/* �ő吔 */
};


//global void player_register_shot_object(SPRITE *src, int /*set_*/shot_type);
/*static*/static/*global*/ void player_register_shot_object(SPRITE *src, int /*set_*/shot_type)
{
	SPRITE *h;/* shot */
	h					= sprite_add_gu_error();
	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
	{
		static const s8 r_tbl[REIMU_SHOT_TYPE_MAX][REI04_MAX] =
		{	/*x_offset256	y_offset256  vx256 status */
	//	/* �U���V���b�g(�얲 �� ������) */
	/*00*/	{ (  0),	(  0),	(	0), 	0x01},	 /*shot_regist_triple_fuda*/
	//	/* ��U���V���b�g(�얲) */
	/*01*/	{ (  0),	(  0),	(	0), 	0x00},	/* �� �܂����� */
	/*02*/	{ ( -5),	(  0),	(	0), 	0x00},	/* �� �܂����� */
	/*03*/	{ ( +5),	(  0),	(	0), 	0x00},	/* �E �܂����� */
	/*04*/	{ ( -5),	(  0),	( -20), 	0x00},	/* �� ���� */
	/*05*/	{ ( +5),	(  0),	(  20), 	0x00},	/* �E �E�� */
	/*06*/	{ ( -5),	(  0),	( -35), 	0x00},	/* �� ���Ώ� */
	/*07*/	{ ( +5),	(  0),	(  35), 	0x00},	/* �E �E�Ώ� */
	//	/* ��U���V���b�g(�H�X�q) */
	/*08*/	{ (-15),	(-25),	( -25), 	0x00},	 /* �H�X�q */					 /*(-32)==(4*(- 8))*/
	/*09*/	{ (+15),	(-25),	( +25), 	0x00},	 /* �H�X�q */					 /*( 32)==(4*(	8))*/
	/*0a*/	{ (-25),	(-25),	( -50), 	0x00},	 /* �H�X�q */					 /*(-64)==(4*(-16))*/
	/*0b*/	{ (+25),	(-25),	( +50), 	0x00},	 /* �H�X�q */					 /*( 64)==(4*( 16))*/
	//		/* CCW�̏ꍇ */
			{ (  0),	( -7),	(	0), 	0x01},//	cv1024r((180)), 				/* ������ A/B �얲 ���~���A �� �`���m */		/*t256( 0), t256( 0),*/
			{ (  0),	( -7),	(-108), 	0x01},//	cv1024r((180-25)),/*263=270-7*/ /* ���~���A �� �`���m */ /*t256( 0), t256( 0),*/
			{ (  0),	( -7),	( 108), 	0x01},//	cv1024r((180+25)),/*277=270+7*/ /* ���~���A �� �`���m */ /*t256( 0), t256( 0),*/
			{ (  0),	(-47),	(	5), 	0x01},//	cv1024r((181)), 				/* �H�X�q��p */	/*-48*/ /*t256( 0), t256(-40),*/
		};
	//	int shot_type;
		int shot_status;/*�j?*/
		shot_status = ((r_tbl[shot_type][REI03_status]));

		int is_needle;/*�j?*/
	//	is_needle = (shot_type <= NEEDLE_ANGLE_270)?(1/*�j*/):(0/*�e*/);
		is_needle = ((shot_status)&0x01);
	//
		#define OFFS_IS_NEEDLE (select_player+(is_needle<<3))
		const static u8 base_shot_atari[16] =
		{	/* ���C���V���b�g�̂����蔻�� */
		/*0,*/	(4),	/* re a */		//	BASE_SHOT_ATARI_0a,/*4*/		/* �얲A */ 	/*	5 */ BASE_SHOT_ATARI_0a,/*4*/
		/*0,*/	(4),	/* re b */		//	BASE_SHOT_ATARI_0b,/*4*/		/* �얲B */ 	/*	5 */ BASE_SHOT_ATARI_0a,/*4*/
		/*0,*/	(4),	/* ma a */		//	BASE_SHOT_ATARI_0c,/*4*/		/* ������A */	/*	6 */ BASE_SHOT_ATARI_0b,/*4*/
		/*0,*/	(4),	/* ma b */		//	BASE_SHOT_ATARI_0d,/*4*/		/* ������B */	/*	6 */ BASE_SHOT_ATARI_0b,/*4*/
		/*0,*/	(4),	/* oz / */		//	BASE_SHOT_ATARI_0e,/*4*/		/* ������C */	/*	7 */ BASE_SHOT_ATARI_0c,/*4*/
		/*0,*/	(8),	/* yu / */		//	BASE_SHOT_ATARI_0f,/*4*/		/* ���~���A */	/*	9 */ BASE_SHOT_ATARI_0e,/*8*/
		/*0,*/	(6),	/* ci a */		//	BASE_SHOT_ATARI_0g,/*6*/		/* �`���m */	/*	8 */ BASE_SHOT_ATARI_0d,/*6*/
		/*0,*/	(9),	/* ci q */		//	BASE_SHOT_ATARI_0h,/*8*/		/* �H�X�q */	/*	6 */ BASE_SHOT_ATARI_0b,/*4*/
			/* �T�u�V���b�g�̂����蔻�� */
		/*0,*/	(6),	/* re a */		//	BASE_NEEDLE_ATARI_0a,/*6*/		/* �얲A */ 	/*	5 */  BASE_NEEDLE_ATARI_0a,/*6*/
		/*0,*/	(6),	/* re b */		//	BASE_NEEDLE_ATARI_0b,/*6*/		/* �얲B */ 	/*	5 */  BASE_NEEDLE_ATARI_0a,/*6*/
		/*0,*/	(8),	/* ma a */		//	BASE_NEEDLE_ATARI_0c,/*8*/		/* ������A */	/*	6 */  BASE_NEEDLE_ATARI_0b,/*8*/
		/*0,*/	(8),	/* ma b */		//	BASE_NEEDLE_ATARI_0d,/*8*/		/* ������B */	/*	6 */  BASE_NEEDLE_ATARI_0b,/*8*/
		/*0,*/ (12),	/* oz / */		//	BASE_NEEDLE_ATARI_0e,/*8*/		/* ������C */	/*	7 */  BASE_NEEDLE_ATARI_0c,/*12*/
		/*0,*/	(8),	/* yu / */		//	BASE_NEEDLE_ATARI_0f,/*12*/ 	/* ���~���A */	/*	9 */  BASE_NEEDLE_ATARI_0e,/*8*/
		/*0,*/ (16),	/* ci a */		//	BASE_NEEDLE_ATARI_0g,/*16*/ 	/* �`���m */	/*	8 */  BASE_NEEDLE_ATARI_0d,/*16*/
		/*0,*/ (24),	/* ci q */		//	BASE_NEEDLE_ATARI_0h,/*8*/		/* �H�X�q */	/*	6 */  BASE_NEEDLE_ATARI_0b,/*8*/
		};
		h->m_Hit256R			= ((base_shot_atari[(OFFS_IS_NEEDLE)])<<8);/* �����蔻��̑傫�� */

	//	h->type 			= (/*SP_GROUP_JIKI_GET_ITEM*/JI KI_SHOT_02|SP_GROUP_SHOT_SPECIAL)-shot_status;
		#if 1
	//	else
	//	if(0x01== shot_status)	/* �j */
		{
			h->type 			= (JIKI_SHOT_07|SP_GROUP_SHOT_SPECIAL); 	/*SP_GROUP_JIKI_GET_ITEM*/	/*SP_PL_FIREBALL*/
		}
	//
		if (0x00 == shot_status)	/* �j�ȊO */
		{
			if (REIMU_B+1 > select_player)	/* �얲? */
			{
			//	if(0x02== shot_status)	/* �얲�̉�]�V���b�g */
				if (REIMU_SHOT_TYPE_00 != shot_type)/* �U���V���b�g�ȊO�H */
				{
					h->type 			= (JIKI_SHOT_01|SP_GROUP_SHOT_SPECIAL); 	/*SP_GROUP_JIKI_GET_ITEM*/
					/* �`��p�O����] */
					h->rotationCCW1024	= (ra_nd() & (1024-1));/* �o�����̊p�x�̓����_���łȂ���(���[�U�[�݂�����)�ςȉ�ʂɂȂ� */
				}
			/*	else	{	h->type = �U���V���b�g�́A�j�Ɠ���; 	}	*/
			}
			else
			{
				h->type 			= (JIKI_SHOT_02|SP_GROUP_SHOT_SPECIAL); 	/*SP_GROUP_JIKI_GET_ITEM*/
			}
		}
	//	else
		#endif
		#if 1/*Gu(���S���W)*/
		h->cx256			= (src->cx256) + ((r_tbl[shot_type][REI00_x_offset])<<8); /*+ x_offs*/
		h->cy256			= (src->cy256) + ((r_tbl[shot_type][REI01_y_offset])<<8);/*(20)*/
		#endif
		h->callback_mover	= player_move_shot;
//		h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);
		h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x96);	/*	h->alpha		= 0x96; 150 �������ɂ���B */	/*shot_regist_triplestar*/
	//
		#define CCC_STRENGTH_L1 	(PLAYERS8* 0)/* WEAPON_L0 */
		#define CCC_STRENGTH_L2 	(PLAYERS8* 1)/* WEAPON_L1 */
		#define CCC_STRENGTH_L3 	(PLAYERS8* 2)/* WEAPON_L2 */
		#define CCC_STRENGTH_L4 	(PLAYERS8* 3)/* WEAPON_L3 */
		#define CCC_STRENGTH_L5 	(PLAYERS8* 4)/* WEAPON_L4 */
		#define CCC_STRENGTH_L6 	(PLAYERS8* 5)/* WEAPON_L5 */
		#define CCC_SPEED			(PLAYERS8* 6)
		#define DDD_SPEED			(PLAYERS8* 7)
	//
		#define DDD_STRENGTH_L1 	(PLAYERS8* 8)/* WEAPON_L0 */
		#define DDD_STRENGTH_L2 	(PLAYERS8* 9)/* WEAPON_L1 */
		#define DDD_STRENGTH_L3 	(PLAYERS8*10)/* WEAPON_L2 */
		#define DDD_STRENGTH_L4 	(PLAYERS8*11)/* WEAPON_L3 */
		#define DDD_STRENGTH_L5 	(PLAYERS8*12)/* WEAPON_L4 */
		#define DDD_STRENGTH_L6 	(PLAYERS8*13)/* WEAPON_L5 */
	//	#define DDD_MAX 			(PLAYERS8*2)		/* �ő吔 */
		#define CCC_MAX 			(PLAYERS8*14)		/* �ő吔 */
		static const u8/*u8*/ ccc_tbl[CCC_MAX] =
		{/* REIMU(A/B) MARISA(A/B) REMILIA YUYUKO CIRNO(A/Q) */
	//		 40, 40,  40, 40,  40,128,	40, 40, 	/* WEAPON_L0  strength �V���b�g�̋��� */ /* (L1�V���b�g�́A�e�������Ȃ��̂� 40�ȉ��ɂ��Ȃ�) */
	//		 16, 24,  24, 16,  32,128,	40, 40, 	/* WEAPON_L1  strength �V���b�g�̋��� */
	//		 16, 24,  24,  8,  32,128,	40, 40, 	/* WEAPON_L2  strength �V���b�g�̋��� */
	//		 16, 24,  24,  8,  32,128,	40, 40, 	/* WEAPON_L3  strength �V���b�g�̋��� */
	//		 16, 24,  24,  8,  32,128,	40, 40, 	/* WEAPON_L4  strength �V���b�g�̋��� */
	//		 16, 24,  24,  8,  32,128,	40, 40, 	/* WEAPON_L5  strength �V���b�g�̋��� */
	//
			 40, 40,  40, 40,  40,128,	40, 40, 	/* WEAPON_L0  strength �V���b�g�̋��� */ /* (L1�V���b�g�́A�e�������Ȃ��̂� 40�ȉ��ɂ��Ȃ�) */
			 20, 24,  24, 24,  37,129,	40, 40, 	/* WEAPON_L1  strength �V���b�g�̋��� */
			 17, 24,  24, 24,  36,130,	40, 40, 	/* WEAPON_L2  strength �V���b�g�̋��� */
			 16, 24,  24, 24,  35,132,	40, 40, 	/* WEAPON_L3  strength �V���b�g�̋��� */
			 15, 25,  24, 24,  34,136,	48, 44, 	/* WEAPON_L4  strength �V���b�g�̋��� */
			 14, 26,  25, 25,  33,144,	64, 60, 	/* WEAPON_L5  strength �V���b�g�̋��� */

			 16, 14,  10, 12,	9,	4,	 8,  9, 	/* speed	�V���b�g�̑��� (�x��������ʏ�̒e����������̂ŋ����B�������d���Ȃ�) */
			 18, 18,  10, 10,	3,	4,	 3,  4, 	/* speed	�j�̑��� (�x��������ʏ�̒e����������̂ŋ����B�������d���Ȃ�) */
	//
			 16, 32,  32,160,  48, 64,	48, 48, 	/* WEAPON_L0  strength �j�̋��� */	/*REMILIA, CIRNO,  48 40 ��������*/
			 16, 32,  32,162,  48, 64,	48, 48, 	/* WEAPON_L1  strength �j�̋��� */	/*REMILIA, CIRNO,  48 40 ��������*/
			 16, 33,  32,163,  49, 64,	48, 48, 	/* WEAPON_L2  strength �j�̋��� */	/*REMILIA, CIRNO,  48 40 ��������*/
			 16, 34,  32,164,  49, 65,	48, 48, 	/* WEAPON_L3  strength �j�̋��� */	/*REMILIA, CIRNO,  48 40 ��������*/
			 15, 35,  32,165,  50, 65,	56, 52, 	/* WEAPON_L4  strength �j�̋��� */	/*REMILIA, CIRNO,  48 40 ��������*/
			 14, 36,  32,166,  52, 66,	80, 78, 	/* WEAPON_L5  strength �j�̋��� */	/*REMILIA, CIRNO,  48 40 ��������*/
	/* [***20090822 REIMU 24 <- 16 (�Ƃ肠�����j�シ����̂�) */
	/* r32 �얲A�̗U���V���b�g�B��������C������̂ŁA32 �� 16 �ɂ��Ă݂�B*/
		};
	//		 2, 3,	3, 2,  4,16, 5, 5,	/* strength �V���b�g�̋��� */
	//		16,14, 10,12,  9, 4, 8, 8,	/* speed	�V���b�g�̑��� (�x��������ʏ�̒e����������̂ŋ����B�������d���Ȃ�) */
		//};
	//		 2, 2, 3, 3, 4, 3, 3, 3,	/* strength �V���b�g�̋��� */
	//		15,15,12,12,18,12,12,12,	/* speed	�V���b�g�̑���	*/
	/* �ɒ[�ɋ@�̂̐��\���΂�̂͗ǂ��Ȃ��B(�����đ����̂͂��邢) */
		//static const u8 ddd_tbl[DDD_MAX] =
		//{/* REIMU(A/B) MARISA(A/B) REMILIA YUYUKO CIRNO(A/Q) */
	//		 3, 3, 4, 4, 6, 8, 6, 6,	/* strength �j�̋��� */ 	/*REMILIA, CIRNO,  6 5 ��������*/
	/* �얲(�������Ă݂� [***20090930 ) */
	//		 4, 4, 4,10, 6, 8, 6, 6,	/* strength �j�̋��� */ 	/*REMILIA, CIRNO,  6 5 ��������*/
	//		 4, 4, 4,25, 6, 8, 6, 6,	/* strength �j�̋��� */ 	/*REMILIA, CIRNO,  6 5 ��������*/
	//		18,18,10,15, 3, 4, 3, 3,	/* speed	�j�̑��� (�x��������ʏ�̒e����������̂ŋ����B�������d���Ȃ�) */


		h->base_weapon_strength = ccc_tbl[CCC_STRENGTH_L1+(weapon_level_offset)+(is_needle<<(4+2))];/* 64 == 8[players]*8[line] */
	//	h->base_weapon_strength = ddd_tbl[DDD_STRENGTH_L1+(weapon_level_offset)];

	//
	//	int ok;
	//	ok=0;
		{
			#if 0
		//	if (1==r_tbl[shot_type][REI03_yudo])	/* psp��0���W�X�^������̂�0�Ɣ�r�����ق������� */
			if (0 != r_tbl[shot_type][REI03_yudo])/* �U���V���b�g���H */
			#else
		//	if ( (REIMU_SHOT_TYPE_01) > shot_type)/* �U���V���b�g���H */
			if ( (REIMU_SHOT_TYPE_00) == shot_type)/* �U���V���b�g���H */
			#endif
			{
				SPRITE *zzz_player;
				zzz_player = &obj00[FIX_OBJ_00_PLAYER];
				SPRITE *target;
				target = search_enemy_by_sprite();
				if (target != zzz_player)/* ���������� */
				{
					int int_angle1024;
				//	int_angle1024	= at an_1024(target->cy256-zzz_player->cy256, target->cx256-zzz_player->cx256);
					int_angle1024	= atan_65536(target->cy256-zzz_player->cy256, target->cx256-zzz_player->cx256);
					int_angle1024 >>= (6);
				//	if (511 < int_angle1024 )/* ���180-360�x[/360�x]�̗̈�̂ݒǔ����� */
					if ( /*(384)*/(256*3) < (( int_angle1024+128+256)&(1024-1)) )/* ���225-315�x[/360�x]�̗̈�̂ݒǔ����� */	/* CCW�̏ꍇ */
					{
						h->vx256	 = sin1024((int_angle1024))*/*p->speed*/(16/*ccc_tbl[CCC_SPEED+select_player]*/);/*fps_factor*/ /* CCW�̏ꍇ */
						h->vy256	 = cos1024((int_angle1024))*/*p->speed*/(16/*ccc_tbl[CCC_SPEED+select_player]*/);/*fps_factor*/
						#if 1
						/* �`��p�O����] */
						if (MARISA_A==select_player)/* (������������)�}�W�b�N�~�T�C���͌X���Ȃ��B */
						{	int_angle1024 = (0);	}
						else/* (�얲�아��)�z�[�~���O�A�~�����b�g�͖͕핗�ł͌X���鎖�ɂ���B */
						{
						//	h->rotationCCW1024	= int_angle1024;
							/* �`��p�O����](�V���b�g�͏オ���ʂȂ̂ŁA��]�p���v�Z)[180/360�x]�񂷁B */
							int_angle1024 += cv1024r((180));
							mask1024(int_angle1024);
						}
						h->rotationCCW1024	= int_angle1024;/* �������ʂ̊p�x���`��p�p�x */
						#endif
					//	ok=1;/* �����ǔ��\ */
						return;
					}
				}
			}
		}
	//
	//	if (0==ok)/* �����ǔ��s�H */
		{	/*�ŗL�̐ݒ�p�x���g��*/
			h->vx256 = ((/*int_vx*/r_tbl[shot_type][REI02_vx256]))*/*p->speed*/(ccc_tbl[CCC_SPEED+OFFS_IS_NEEDLE]);/*fps_factor*/
			h->vy256 = -(((int)ccc_tbl[CCC_SPEED+OFFS_IS_NEEDLE])<<8);/*fps_factor*/ /*p->speed*/
		}
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

/*---------------------------------------------------------
	�v���C���[�e(�V���b�g�̂�)����
---------------------------------------------------------*/

/* �얲A (�U���ޏ�) �U���V���b�g */

static void shot_regist_re_yudou(SPRITE *s) /* �얲A ������A */
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_00);/* �U���V���b�g */
}

static void shot_regist_yuyuko(SPRITE *s)	/* �H�X�q */
{
	player_register_shot_object(s, NEEDLE_ANGLE_271);
}

/* �얲B (�j�ޏ�) / ������A/B / ���~���A �� �`��  */

static void shot_regist_re_hari(SPRITE *s)	/* ������A/B */ 	/* ��(���݃_�~�[�p) */
{
	player_register_shot_object(s, NEEDLE_ANGLE_270);
}

static void shot_regist_ci_gggg(SPRITE *s)	/* ���~���A �� �`���m */
{
	player_register_shot_object(s, NEEDLE_ANGLE_277 );
	player_register_shot_object(s, NEEDLE_ANGLE_263 );
}
static void shot_regist_ma_lazer(SPRITE *s) /* ������B ���[�U�[ */
{
	{
		static u8 count128;
		count128++;
	//	count128 &= 0x7f;
		/* -110 0000 */
		#if 0
	//	if (0x00==(count128&0x60))	/* ���͂����A1/4���Ԃ������ʂ��Ȃ��B */
		#else
		count128 &= 0x7f;
		if ((43)>(count128 ))	/* ���͂����A1/3���Ԃ������ʂ��Ȃ��B */
		#endif
		{
			shot_regist_re_hari(s);
		}
	}
}

/*---------------------------------------------------------
	�v���C���[�e(�V���b�g�̂�)����
---------------------------------------------------------*/

/* �얲A/B ��]�V���b�g */	/* �ʏ�V���b�g */
static void shot_regist_1(SPRITE *s)	/* �얲 */	/* �얲 �� ������ �� �`���m �� ���~���A */
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_01); 	/* �ʏ�V���b�g */
}
static void shot_regist_2(SPRITE *s)	/* �얲 */	/* �얲 �� ������ �� �`���m �� ���~���A */
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_02); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_03); 	/* �ʏ�V���b�g */
}
static void shot_regist_3(SPRITE *s)	/* �얲 */	/* ������ �� �`���m */
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_04); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_01); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_05); 	/* �ʏ�V���b�g */
}
static void shot_regist_4(SPRITE *s)	/* �얲 */	/* ������ */
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_04); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_02); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_03); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_05); 	/* �ʏ�V���b�g */
}

static void shot_regist_5(SPRITE *s)	/* �얲B */ /* ������ */
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_06); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_04); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_01); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_05); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_07); 	/* �ʏ�V���b�g */
}

/* �얲B (�j�ޏ�) */

/* ������A(�~�T�C��������) */
/* ������B(���[�U�[������) */


static void shot_regist_chou(SPRITE *s) 		/* �H�X�q */
{
	player_register_shot_object(s, YUYUKO_SHOT_TYPE_08);
	player_register_shot_object(s, YUYUKO_SHOT_TYPE_09);
	player_register_shot_object(s, YUYUKO_SHOT_TYPE_0a);
	player_register_shot_object(s, YUYUKO_SHOT_TYPE_0b);
}
/*---------------------------------------------------------
	�I�v�V���� �A�j���[�V����
---------------------------------------------------------*/

static void player_animate_option(SPRITE *src)
{
#if 1
	if (0==player_fix_status[BASE_OPT_ANIM_TYPE+select_player])
	{
		/* �`��p�O����] */
		/* �`��p�p�x(����0�x�ō����(�����v���)) */
		src->rotationCCW1024 += (2+2);/*...�Ƃ肠����*/
		mask1024(src->rotationCCW1024);
	}
	else
//	if (/*YUYUKO==select_player*/)
//	if ((CIRNO_A==select_player)||(CIRNO_Q==select_player))
	{
		src->PL_OPTION_DATA_anime_wait--;
		if (src->PL_OPTION_DATA_anime_wait<1)
		{
//			src->PL_OPTION_DATA_anime_wait=/*(anime_speed)*/(12);/*(12)*/ /*2*/ /*�A�z���܂͂���������*/	//yuyuko
//			src->PL_OPTION_DATA_anime_wait=/*(anime_speed)*/(3);/*12*/ /*2*/ /*�A�z���܂͂���������*/		//cirno
			src->PL_OPTION_DATA_anime_wait=/*(anime_speed)*/(player_fix_status[BASE_OPT_ANIM_SPEED+select_player]);/*12*/ /*2*/ /*�A�z���܂͂���������*/
		#if 0
		//	src->type=((src->type+(src->PL_OPTION_DATA_opt_anime_add_id)+(src->PL_OPTION_DATA_opt_anime_add_id)-1/*src->PL_OPTION_DATA_pos*/)&(8-1)/*%8*/);
		//	src->type=((src->type+(src->PL_OPTION_DATA_opt_anime_add_id))&(0xfff7)/*%8*/);
			src->type=((src->type+(1))&(0xfff7)/*%8*/);
		//	if (src->type<0)		{	src->type=7;}
		#endif
		#if 0
		//	src->an im_frame=((src->an im_frame+(src->PL_OPTION_DATA_opt_anime_add_id)+(src->PL_OPTION_DATA_opt_anime_add_id)-1/*src->PL_OPTION_DATA_pos*/)&(8-1)/*%8*/);
			src->an im_frame=((src->an im_frame+(src->PL_OPTION_DATA_opt_anime_add_id))&(8-1)/*%8*/);
		//	if (src->an im_frame<0) 	{	src->an im_frame=7;}
		#endif
			#if 0
			src->an im_frame++;
			src->an im_frame &= (8-1);//if (src->an im_frame==8)	{src->an im_frame=0;}
			#endif

		#if 1
		//	src->type = ((src->type) & (0xfff8))|((src->type+1) & (8-1));//if (src->an im_frame==8) 	{src->an im_frame=0;}
			src->type++;
			src->type &= (0xfff7);//if (src->type==8)	{src->type=0;}
		#endif
		}
	}
#endif
}


/*---------------------------------------------------------

---------------------------------------------------------*/
/*
 WEAPON_L0(P000-P008)
 WEAPON_L1(P008-P015)
 WEAPON_L2(P016-P031)
 WEAPON_L3(P032-P063)
 WEAPON_L4(P064-P127)
 WEAPON_L5(P128)
*/
/*static*/static  void shot_regist_remilia_cirno(SPRITE *src)
{	/* �U���x�� */
	switch (src->PL_OPTION_DATA_opt_anime_add_id)
	{
	case OPTION_C1:
	case OPTION_C2:
	//	if (pd.weapon_power > (100-1))	/* ���V�X�e���̒����l 100 */
		if (pd.weapon_power > (96)) 	/* �V�X�e�����ς�����̂ŏC�����Ă݂�(96==128-32) */
		{
			shot_regist_ci_gggg(src);	//	src->PL_OPTION_DATA_opt_shot_interval=(7+3);
		}
		else
		{
			shot_regist_re_hari(src);	//	src->PL_OPTION_DATA_opt_shot_interval=(5+3);
		}
		break;
	case OPTION_C3:
	case OPTION_C4:
		shot_regist_re_hari(src);		//	src->PL_OPTION_DATA_opt_shot_interval=(5+3);
		break;
	}
}


/*---------------------------------------------------------
	�v���C���[�A�I�v�V�����̈ړ�����
---------------------------------------------------------*/
/*static*/extern void player_control_option(SPRITE *src);	/* �S�� */
/*static*/global void player_move_option(SPRITE *src)	/* �S�� */
{
	/* �I�v�V��������\���̏ꍇ�A�������Ȃ��B */
	if (0==(src->flags & ( SP_FLAG_OPTION_VISIBLE)))	{	return; 	}
//
	/* �A�j���[�V���� */
	player_animate_option(src);
//
	if (pd.state_flag & STATE_FLAG_15_KEY_SHOT)
	{
		{
			{
				src->PL_OPTION_DATA_opt_shot_interval--;
				if (src->PL_OPTION_DATA_opt_shot_interval<0)
				{
					src->PL_OPTION_DATA_opt_shot_interval = player_fix_status[BASE_OPT_SHOT_INTERVAL0+(weapon_level_offset)];/* (�얲 ������	���H�X�q)�I�v�V�����̃V���b�g�Ԋu */
					#if 1//1975613
					static /*const*/ void (*ggg[PLAYERS8])(SPRITE *sss) =
					{
						/*REIMU_A*/ 	shot_regist_re_yudou,			/* �얲 A(�U���ԎD) */
						/*REIMU_B*/ 	shot_regist_re_hari,			/* �얲 B(�j) */	/*re_bbbb*/
						/*MARISA_A*/	shot_regist_re_yudou,			/* ������ A(�U���X�e) */
						/*MARISA_B*/	shot_regist_ma_lazer,			/* ������ B(���܂��ꃌ�[�U�[) */
						/*REMILIA*/ 	shot_regist_remilia_cirno,		/* ���~���A �� �`���m */
						/*YUYUKO*/		shot_regist_yuyuko, 			/* �H�X�q(��) */
						/*CIRNO_A*/ 	shot_regist_remilia_cirno,		/* ���~���A �� �`���m */
						/*CIRNO_Q*/ 	shot_regist_remilia_cirno,		/* ���~���A �� �`���m */
					};
					(ggg[select_player])(src);
					#endif
				}
			}
		}
	}
	player_control_option(src);
}


/*---------------------------------------------------------
	���@�̃V���b�g�{�^���������ςȂ��Ŏ����V���b�g�����ꍇ
---------------------------------------------------------*/

global void register_main_shot(SPRITE *s1)
{
	if (
		(MARISA_B==select_player) &&	/* ������B(����) */
		(0<pd.bomber_time)				/* �}�X�^�[�X�p�[�N�� */
	)
	{
		;/* �}�X�^�[�X�p�[�N���̓V���b�g�����ĂȂ��炵�� */
	}
	else
	{
		{static int weapon_interval=0;
			/*pd.*/weapon_interval--/*=fps_fa_ctor*/;
			if (/*pd.*/weapon_interval < 1 )
			{	/* ���@�̃V���b�g�{�^���������ςȂ��Ŏ����V���b�g�����ꍇ�́A���Ԋu[�t���[���P��]�̃e�[�u���B */
				static const u8 jiki_auto_shot_interval_table[(WEAPON_L_MAX)*(PLAYERS8)] =
				{/* REIMU(A/B) MARISA(A/B) REMILIA YUYUKO CIRNO(A/Q) */    /* ���~���A��������̂Œ���(+5) */
						 5, 5,	6, 6,  7+5,  9+8,  9,  9,	/* WEAPON_L0(P000-P008) */
						 5, 5,	6, 6,  6+5,  8+8,  9,  9,	/* WEAPON_L1(P008-P015) */
						 5, 5,	5, 5,  7+5,  7+8,  9,  9,	/* WEAPON_L2(P016-P031) */
						 5, 5,	5, 5,  6+5,  6+8,  9,  9,	/* WEAPON_L3(P032-P063) */
						 5, 5,	5, 5,  5+5,  5+8,  9,  9,	/* WEAPON_L4(P064-P127) */
						 5, 5,	5, 5,  5+5,  5+8,  9,  9,	/* WEAPON_L5(P128)		*/
				};
			//	/*pd.*/weapon_interval = jiki_auto_shot_interval_table[(weapon_List<<3)+select_player];
				/*pd.*/weapon_interval = jiki_auto_shot_interval_table[(weapon_level_offset)];
				voice_play(VOICE00_SHOT, TRACK00_BULLETS);
				static /*const*/ void (*bbb[(WEAPON_L_MAX)*(PLAYERS8)])(SPRITE *sss) =
				{
	/*REIMU_A*/ 	/*REIMU_B*/ 	/*MARISA_A*/	/*MARISA_B*/	/*REMILIA*/ 	/*YUYUKO ���i�K���� */	/*CIRNO_A*/ 	/*CIRNO_Q*/
	shot_regist_1,	shot_regist_1,	shot_regist_1,	shot_regist_1,	shot_regist_1,	shot_regist_chou,		shot_regist_2,	shot_regist_2,	/* WEAPON_L0:(P000-P008) */
	shot_regist_1,	shot_regist_1,	shot_regist_2,	shot_regist_2,	shot_regist_1,	shot_regist_chou,		shot_regist_2,	shot_regist_2,	/* WEAPON_L1:(P008-P015) */
	shot_regist_2,	shot_regist_2,	shot_regist_2,	shot_regist_3,	shot_regist_2,	shot_regist_chou,		shot_regist_2,	shot_regist_3,	/* WEAPON_L2:(P016-P031) */
	shot_regist_3,	shot_regist_3,	shot_regist_3,	shot_regist_4,	shot_regist_2,	shot_regist_chou,		shot_regist_3,	shot_regist_3,	/* WEAPON_L3:(P032-P063) */
	shot_regist_3,	shot_regist_4,	shot_regist_3,	shot_regist_4,	shot_regist_2,	shot_regist_chou,		shot_regist_3,	shot_regist_3,	/* WEAPON_L4:(P064-P127) */
	shot_regist_4,	shot_regist_5,	shot_regist_3,	shot_regist_5,	shot_regist_2,	shot_regist_chou,		shot_regist_3,	shot_regist_3,	/* WEAPON_L5:(P128) 	 */
				};
				(bbb[(weapon_level_offset)])(s1);
			}
		}
	}
}
