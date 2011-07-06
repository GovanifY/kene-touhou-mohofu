
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



/*
colision_check �����蔻�� -------------------------------------------------------------------------
*/

/*---------------------------------------------------------
	�v���C���[�A�������{���B��ʊO�͂ݏo���`�F�b�N�B�����蔻��`�F�b�N�B
---------------------------------------------------------*/
static void player_bomber_out_colision_check(SPRITE *src)
{
	#if 1/*Gu(���S���W)*/
	if ((src->cx256 < t256(0))||(src->cx256 > t256(GAME_WIDTH))||
		(src->cy256 < t256(0))||(src->cy256 > t256(GAME_HEIGHT)))
	#endif
	{
		src->jyumyou = JYUMYOU_NASI;/* ��ʊO�ɏo���炨���܂� */
	}
	else
	{
		player_weapon_colision_check(src, PLAYER_WEAPON_TYPE_01_BOMB);/* �{���œG��|�������蔻�� */
	}
}



/*
move �V���b�g/�{���ړ� -------------------------------------------------------------------------
*/

/*---------------------------------------------------------
	�v���C���[�A�������{���̈ړ�(�얲)

	���v��� �� �����v��� �� �X�N�G�A���E(�얲B) ���p
---------------------------------------------------------*/

static void reimu_move_rotate_kekkai(SPRITE *src) /* �얲 */
{
	src->PL_KEKKAI_DATA_angleCCW1024 += (src->PL_KEKKAI_DATA_add_r1024);	/*fps_factor*/	/*...*/
	mask1024(src->PL_KEKKAI_DATA_angleCCW1024);
	#if 1
	/* �`��p�p�x(����0�x�ō����(�����v���)) */
	src->rotationCCW1024 = (src->PL_KEKKAI_DATA_angleCCW1024);
	#endif
//
	SPRITE *zzz_player;
	zzz_player = &obj00[FIX_OBJ_00_PLAYER];
	#if 1/*Gu(���S���W)*/
	src->cx256 = zzz_player->cx256 + sin1024((src->PL_KEKKAI_DATA_angleCCW1024))*src->PL_KEKKAI_DATA_radius;	/* CCW�̏ꍇ */
	src->cy256 = zzz_player->cy256 + cos1024((src->PL_KEKKAI_DATA_angleCCW1024))*src->PL_KEKKAI_DATA_radius;
	#endif
	/* �X�N�G�A���E(�얲B) */
	if (REIMU_A != select_player)
	{
		if ( 0 < src->PL_KEKKAI_DATA_add_r1024)
		{
			if (zzz_player->cx256 < src->cx256)
					{	src->cx256 = zzz_player->cx256 + ((src->PL_KEKKAI_DATA_radius)<<8); 	}
			else	{	src->cx256 = zzz_player->cx256 - ((src->PL_KEKKAI_DATA_radius)<<8); 	}
		}
		else
		{
			if (zzz_player->cy256 < src->cy256)
					{	src->cy256 = zzz_player->cy256 + ((src->PL_KEKKAI_DATA_radius)<<8); 	}
			else	{	src->cy256 = zzz_player->cy256 - ((src->PL_KEKKAI_DATA_radius)<<8); 	}
		}
	}
//	if (REIMU==select_player)
	{
		if (src->PL_KEKKAI_DATA_radius<255) 	{	src->PL_KEKKAI_DATA_radius += (ra_nd()&1);	}	/* �������L����悤�ɂ��� */
		else									{	src->jyumyou = JYUMYOU_NASI;
													pd.state_flag &= (~STATE_FLAG_02_BOMB_AUTO_GET_ITEM);	/* �{���ɂ�鎩�����W�͏I��� */
												}	/* ��]���L�������炨���܂� */
	//	src->alpha				= ((int)(255-src->PL_KEKKAI_DATA_radius))&(256-1);	/* ���񂾂������悤�ɂ��� */
	//	src->color32			= ((((255-src->PL_KEKKAI_DATA_radius))&(0xff))<<24)|0x00ffffff; /* ���񂾂������悤�ɂ��� */
		src->color32			= ((((255-src->PL_KEKKAI_DATA_radius))&(0xff))<<24)|0x00ffffff; /* ���񂾂������悤�ɂ��� */
	}
//	else //if (YUYUKO==select_player)
//	{

//		if (pd_bomber_time<=0)	{	src->jyumyou = JYUMYOU_NASI;}/* ���Ԑ����ł����܂� */
//	}
	if ( (ra_nd()&0xff) < pd.bomber_time)
	{
		if(0==((src->PL_KEKKAI_DATA_radius)&7))
		{
			//player_weapon_colision_check_kekkai(src, src->PL_KEKKAI_DATA_radius);
			bullets_to_hosi();/* �e�S���A���A�C�e���ɂ��� */
		}
	}
	player_weapon_colision_check(src, PLAYER_WEAPON_TYPE_01_BOMB);/* �{���œG��|�������蔻�� */
}


/*---------------------------------------------------------
	�v���C���[�A�������{���̈ړ�(������B �}�X�^�[�X�p�[�N��p)
---------------------------------------------------------*/
#define MASKER_SPARK_LENGTH144 (144)
static void marisa_move_master_spark(SPRITE *src)
{
	SPRITE *zzz_player;
	zzz_player = &obj00[FIX_OBJ_00_PLAYER];
//	src->cx256 += src->vx256;	/*fps_factor*/
//	src->cy256 += src->vy256;	/*fps_factor*/
	src->cx256 = zzz_player->cx256; 			/*fps_factor*/
	src->cy256 = zzz_player->cy256-t256(MASKER_SPARK_LENGTH144);	/*fps_factor*/
	{
		static const u16 maspa_kaiten[4] =
		{
			(18),
			(1024-24),
			(10),
			(1024-6),
			/* ��]���x */
		};
		/* �\���p�x(����0�x�ō����(�����v���)) */
		src->tmp_angleCCW1024	+= maspa_kaiten[((src->type)& (0x03))];
		mask1024((src->tmp_angleCCW1024));
		src->rotationCCW1024 = ((sin1024((src->tmp_angleCCW1024)))>>2);
	}
	/* ���Ԃł����܂� */
	if (0 >= pd.bomber_time)	{ src->jyumyou = JYUMYOU_NASI;}/* ���Ԑ����ł����܂� */
	else
	if (0x40 > pd.bomber_time)	/* 0x3f ... 0x01 */
	{
		src->color32			= ((((pd.bomber_time))/*&(0x3f)*/)<<(24+2))|0x00ffffff; /* ���񂾂������悤�ɂ��� */
		/* (���̒l�͈̔͂Ȃ�)���̓}�X�N�K�v�Ȃ��B */
	}
	player_weapon_colision_check(src, PLAYER_WEAPON_TYPE_01_BOMB);/* �{���œG��|�������蔻�� */
//	player_bomber_out_colision_check(src);/* ��ʊO�ɏo���炨���܂� & �{���œG��|�������蔻�� */
}

/*---------------------------------------------------------
	�v���C���[�A�������{���̈ړ�(�������A�`���m�A���H�X�q(�Ⴄ�����ɂ�����) )
---------------------------------------------------------*/

static void marisa_yuyuko_move_levarie_gyastry_dream(SPRITE *src)
{
	src->cx256 += src->vx256;	/*fps_factor*/
	src->cy256 += src->vy256;	/*fps_factor*/
	#if 1
	/* YUYUKO MARISA_A �ȊO */
	if (
	//	(CIRNO_A==select_player) || (CIRNO_Q==select_player) ||
		((CIRNO_A-1)<select_player) //||
	//	(MARISA_B==select_player)/* �}�X�p(���ǂ�) */
	)
	#endif
	{	/*�`���m�p(�b��I)*/	/*1.5*(d->speed);*/ 	/*fps_factor*/
		src->cy256 -= (abs((src->vx256+(src->vx256>>1) )));
		src->cy256 -= (abs((src->vy256+(src->vy256>>1) )));
		src->cy256 -= t256(1.0);
	}
	{
		static const u8 aaa_sss[16] =
		{/* REIMU(A/B) MARISA(A/B) REMILIA YUYUKO CIRNO(A/Q) */
//			0, 0,  4, 16, 0, 0, 18, 18, /* ��]���x */	/* YUYUKO			�ȊO�� ��] ����B */
			0, 0, 20, 16, 0, 0, 18, 18, /* ��]���x */	/* YUYUKO			�ȊO�� ��] ����B */
//			0, 0,  2,  0, 0, 0,  3,  3, /* �g�呬�x */	/* YUYUKO  MARISA_B �ȊO�� �g�� ����B */
			0, 0,  0,  0, 0, 0,  3,  3, /* �g�呬�x */	/* CIRNO �� �g�� ����B */
		};
		/* �\���p�x(����0�x�ō����(�����v���)) */
		src->rotationCCW1024	+= aaa_sss[select_player];
		mask1024((src->rotationCCW1024));
		#if (1==USE_ZOOM_XY)
		if ( t256(4.0) > src->m_zoom_x256)
		#else
		if ( t256(4.0) > src->m_zoom_xy256)
		#endif
		{
			#if (1==USE_ZOOM_XY)
			src->m_zoom_x256 += aaa_sss[select_player+(PLAYERS8)];
		//	src->m_zoom_y256 += aaa_sss[select_player+(PLAYERS8)];
			src->m_zoom_y256 = src->m_zoom_x256;
			#else
			src->m_zoom_xy256 += aaa_sss[select_player+(PLAYERS8)];
			#endif
		}
	}
	player_bomber_out_colision_check(src);
}

/*---------------------------------------------------------
	�v���C���[�A�������{���̈ړ�(���~���A)
---------------------------------------------------------*/

static void remilia_move_burn_fire(SPRITE *src)
{
	#if 1
	// ����
	// x1.10
//	src->vx256 = ((src->vx256 * t256(1.1))>>8);//co_s1024((data->angle1024))*p->speed;/*fps_factor*/
//	src->vy256 = ((src->vx256 * t256(1.1))>>8);//si_n1024((data->angle1024))*p->speed;/*fps_factor*/
	// x1.125
	src->vx256 += ((src->vx256)>>3);//co_s1024((data->angle1024))*p->speed;/*fps_factor*/
	src->vy256 += ((src->vy256)>>3);//si_n1024((data->angle1024))*p->speed;/*fps_factor*/
	// x1.0625
//	src->vx256 += ((src->vx256)>>4);//co_s1024((data->angle1024))*p->speed;/*fps_factor*/
//	src->vy256 += ((src->vy256)>>4);//si_n1024((data->angle1024))*p->speed;/*fps_factor*/
	/* �`�悪�d������̂ŉ������\�A�኱����( x1.0625�� x1.125)���� */
	//
	#endif
	src->cx256 += src->vx256;	//	src->vx256;//co_s1024((data->angle1024))*p->speed;/*fps_factor*/
	src->cy256 += src->vy256;	//	src->vy256;//si_n1024((data->angle1024))*p->speed;/*fps_factor*/
	//
	player_bomber_out_colision_check(src);
}

/*---------------------------------------------------------
	�U���e�̗U���ړ��T�u���[�`��
---------------------------------------------------------*/

static void yuudou_idou(SPRITE *src)
{
	SPRITE *target; 	/* �ڕW */
	target = src->target_obj;
	/* �ڕW����ʓ��Ȃ�ΖڕW�Ɍ����� */
	if (//(NULL != target) &&	/* �d�����Ă�̂Ŕr�� */
	//	(target->flags&SP_FLAG_VISIBLE) &&
		(target->cx256 > t256(0)) &&
		(target->cx256 < (t256(GAME_WIDTH)) ) &&
		(target->cy256 > t256(0)) &&
		(target->cy256 < (t256(GAME_HEIGHT)) )) 	/*Gu(���S���W)*/
	{
		#if (1)/*�U��(000)*/
		{int ta1024;
		#if 1
		/* CCW�̏ꍇ */
		//	ta1024 = at an_1024(target->cy256-src->cy256,target->cx256-src->cx256);
			ta1024 = atan_65536(target->cy256-src->cy256,target->cx256-src->cx256);
			ta1024 >>= (6);
			#if 1
			/* �x��U�� */
			/* KETM�͍̂l���������G�Ȃ̂Ŕj���A0�x����̍����ōl��180�x�����ɕ��򂷂�Γ���(1����360�x�����Ȃ��̂�) */
		//	const int sabun_aaa = (src->tmp_angleCCW1024-src->rotationCCW1024);
			const int sabun_aaa1024 = (ta1024-src->rotationCCW1024);
			if (0 > sabun_aaa1024/*(src->tmp_angleCCW1024-src->rotationCCW1024)*/ )/* 0���W�X�^+���ʍœK�� */
			{	/* �ڕW�p�x < ����p�x */
				if ( (1024/2) > sabun_aaa1024/*(src->tmp_angleCCW1024-src->rotationCCW1024)*/ )/* ���ʍœK�� */
						{	src->rotationCCW1024 -= 10; }	/* 180�x�ȓ��Ȃ�߂��ق������� */
				#if 0	/* �ȗ����Ė��Ȃ��H */
				else	{	src->rotationCCW1024 += 10; }	/* 180�x�ȏ�Ȃ甽�Ύ���̂ق����߂� */
				#endif
			}
			else	/* �ڕW�p�x > ����p�x */
			{
				if ( (1024/2) > -(sabun_aaa1024)/*(src->rotationCCW1024-src->tmp_angleCCW1024)*/ )
						{	src->rotationCCW1024 += 10; }	/* 180�x�ȓ��Ȃ�߂��ق������� */
				#if 0	/* �ȗ����Ė��Ȃ��H */
				else	{	src->rotationCCW1024 -= 10; }	/* 180�x�ȏ�Ȃ甽�Ύ���̂ق����߂� */
				#endif
			}
			#else
			/* ���ڗU�� */
			src->rotationCCW1024/*src->PL_HOMING_DATA_angleCCW1024*/ = ta1024;
			#endif
		#endif
		}
		mask1024(src->rotationCCW1024/*src->PL_HOMING_DATA_angleCCW1024*/);
		#endif/*�U��(000)*/
	}
	else	/* �ڕW����ʊO�Ȃ�ΐV�����ڕW��T���B */
	{
		/* Ziel wurde anderweitig vernichtet, neues Ziel suchen */
		src->target_obj = &obj00[FIX_OBJ_00_PLAYER];		/* ��ʊO�ɓ�����ꂽ */
	}
}

/* ------------------------------------------------------------------------ */

/*---------------------------------------------------------
	�v���C���[�A�U���e�̈ړ�(�e)����
---------------------------------------------------------*/

enum
{
	HOMING_096_TIME_OUT = 96,
	HOMING_128_TIME_OUT = 128,
};
static void marisa_move_parrent_hlaser(SPRITE *src)
{
	/* ���̗U���{�����A���ɓ|�������H */
	if (&obj00[FIX_OBJ_00_PLAYER] == src->target_obj)	/* ������Ȃ�(���̗U���{�����|���Ă��܂����ꍇ) */
	{
		/* �^�[�Q�b�g�����A�G��T���B */
		src->target_obj = search_enemy_by_sprite();
	}
	else	/* �܂��^�[�Q�b�g�������Ă� */
	{
		if (HOMING_096_TIME_OUT < src->PL_HOMING_DATA_time_out)
		{
			/* ��莞�Ԍ��� */
			src->PL_HOMING_DATA_speed256 -= t256(0.5);/*����*/
			// �~�܂�ƍ���
			if ( 0 > src->PL_HOMING_DATA_speed256 )
			{
				src->PL_HOMING_DATA_speed256 = t256(0.75);	/* �ŏ��l */
			}
		}
		else
		{
			/* ��莞�Ԃ𒴂�������� */
			src->PL_HOMING_DATA_speed256 += t256(0.5);/*����*/
			// ���߂���̂�����
		//	if ( t256(12.0) < src->PL_HOMING_DATA_speed256)
			if ( t256(16.0) < src->PL_HOMING_DATA_speed256)
			{
				src->PL_HOMING_DATA_speed256 = t256(16.0);	/* �ő�l */
			}
		}
		//
		yuudou_idou(src);
	}
	#if 1
	/* (�����K�v�Ȃ�����)�O�̈׃}�X�N */
	mask1024(src->rotationCCW1024/*src->PL_HOMING_DATA_angleCCW1024*/);
	#endif
	#if 1
	/* �\���p */
//	src->rotationCCW1024	= src->rotationCCW1024/*src->PL_HOMING_DATA_angleCCW1024*/;
	#endif
	src->cx256 += ((sin1024((src->rotationCCW1024/*src->PL_HOMING_DATA_angleCCW1024*/))*src->PL_HOMING_DATA_speed256)>>8);/*fps_factor*/	/* CCW�̏ꍇ */
	src->cy256 += ((cos1024((src->rotationCCW1024/*src->PL_HOMING_DATA_angleCCW1024*/))*src->PL_HOMING_DATA_speed256)>>8);/*fps_factor*/
	player_weapon_colision_check(src, PLAYER_WEAPON_TYPE_00_SHOT);
//
	src->PL_HOMING_DATA_time_out--;/*fps_factor*/
	if (0 > src->PL_HOMING_DATA_time_out ) /* ���Ԑ؂�H */
	{
		src->jyumyou = JYUMYOU_NASI;		/* ���Ԑ؂�Ȃ̂ł����܂� */
	}
}

/*---------------------------------------------------------
	�얲��p �ᑬ�{��
	�아�얲
---------------------------------------------------------*/

static void move_reimu_musou_fuuin(SPRITE *src)
{
	/* ���̗U���{�����A���ɓ|�������H */
	if (&obj00[FIX_OBJ_00_PLAYER] == src->target_obj)	/* ������Ȃ�(���̗U���{�����|���Ă��܂����ꍇ) */
	{
		/* �^�[�Q�b�g�����A�G��T���B */
		src->target_obj = search_enemy_by_sprite();
	}
	else	/* �܂��^�[�Q�b�g�������Ă� */
	{
		if ( t256(3.0) > src->PL_HOMING_DATA_speed256)
		{
			src->PL_HOMING_DATA_speed256 += /*(int)*/t256(0.5);/*����*/
		}
		//
		yuudou_idou(src);
	}
// �{���͍ő� 7 ��(8)
#define PL_HOMING_DATA_reimu_musou_id PL_HOMING_DATA_time_out
//	if((pd.bomber_time&0x0f)==(src->PL_HOMING_DATA_reimu_musou_id&0x0f))
	if((pd.bomber_time&0x07)==(src->PL_HOMING_DATA_reimu_musou_id&0x07))
	{
		src->vx256 = ((sin1024((src->rotationCCW1024/*src->PL_HOMING_DATA_angleCCW1024*/))*src->PL_HOMING_DATA_speed256)>>8);/*fps_factor*/ /* CCW�̏ꍇ */
		src->vy256 = ((cos1024((src->rotationCCW1024/*src->PL_HOMING_DATA_angleCCW1024*/))*src->PL_HOMING_DATA_speed256)>>8);/*fps_factor*/
	}
		src->cx256 += (src->vx256);/*fps_factor*/
		src->cy256 += (src->vy256);/*fps_factor*/
	player_weapon_colision_check(src, PLAYER_WEAPON_TYPE_01_BOMB/*PLAYER_WEAPON_TYPE_00_SHOT*/);
//
	if (0 >= pd.bomber_time)	/* ���Ԑ؂�̏ꍇ */
	{
		gamen_gai_nara_zako_osimai(src);/* ��ʊO�Ȃ炨���܂� */
		src->base_weapon_strength--;/* ���Ԑ؂�̏ꍇ�A�̗͂����炷 */
	}
//
	if (0 > src->base_weapon_strength)	{	src->jyumyou = JYUMYOU_NASI;	}	/* �̗͐؂�ł����܂� */
}
/*---------------------------------------------------------
	�v���C���[�A�U���e�̈ړ�(�q��)
---------------------------------------------------------*/

static void marisa_move_kodomo_hlaser(SPRITE *src)
{
	#if 1
	SPRITE *oya;
	oya = src->target_obj;
	if (
		//	(SP_DELETE == oya->type) || 			/* �e�������ς݂Ȃ�Ύq������ */
			(JYUMYOU_NASI > oya->jyumyou) ||			/* �e�������ς݂Ȃ�Ύq������ */
		//	((SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_SPECIAL) != oya->type)			/* (SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_SPECIAL)�ȊO�͐e�łȂ��̂ŏ��� */
			(0==(SP_GROUP_SHOT_SPECIAL & oya->type))	/* (SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_SPECIAL)�ȊO�͐e�łȂ��̂ŏ��� */
		)
	{
		src->jyumyou = JYUMYOU_NASI;/*�����܂�*/
	}
//
	#if 1
	/* �\���p */
	src->rotationCCW1024	= oya->rotationCCW1024;/*src->PL_HOMING_KODOMO_DATA_angleCCW1024*/
	#endif
	src->cx256 = oya->cx256;
	src->cy256 = oya->cy256;
//	src->PL_HOMING_KODOMO_DATA_color256 = ((oya->PL_HOMING_KODOMO_DATA_color256*t256(0.96))>>8)&0xff;
//	src->PL_HOMING_KODOMO_DATA_color256 = ((oya->PL_HOMING_KODOMO_DATA_color256*t256(0.90))>>8)&0xff;
//	src->PL_HOMING_KODOMO_DATA_color256 = ((oya->PL_HOMING_KODOMO_DATA_color256 - 0x10)&0xff);
//	src->color32 = (src->PL_HOMING_KODOMO_DATA_color256<<24)|(0x00ffffff);
	#if (1)/*???*/
	/* ���t���[�������Ȃ��ꍇ�́A������������ */
	if ((src->PL_HOMING_KODOMO_DATA_check_x256 != src->cx256) ||
		(src->PL_HOMING_KODOMO_DATA_check_y256 != src->cy256))
	{
		src->PL_HOMING_KODOMO_DATA_time_out = 8;/* �������玩���������Ȃ� */
	}
	src->PL_HOMING_KODOMO_DATA_time_out--;
	if ( 0 > src->PL_HOMING_KODOMO_DATA_time_out)
	{
		src->jyumyou = JYUMYOU_NASI;/*�����܂�*/
	}
	#endif
	src->PL_HOMING_KODOMO_DATA_check_x256 = src->cx256;
	src->PL_HOMING_KODOMO_DATA_check_y256 = src->cy256;
	#endif
}





/*
add1-------------------------------------------------------------------------
*/
/* (�H�X�q�ᑬ)�\�����{���̉��̕��� */
/*		r_or_l	[xxx_r] l=2, [xxx_l] r=0, u=3, d=1	*/
static void yuyuko_add_meifu(SPRITE *src/*, int ggg*/ /*r_or_l*/)	/* [***090221 �ǉ� */
{
	int angCCW1024;
	for (angCCW1024=0; angCCW1024<(1024); angCCW1024+=(256)/*(20)*/)
	{
		int i;
		for (i=0; i<10; i+=1)
		{
			SPRITE *h;
			h = sprite_add_gu_error();
			if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
			{
				//
				h->m_Hit256R		= JIKI_ATARI_ITEM_16;
//				h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);
				h->type 			= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_04|SP_GROUP_SHOT_SPECIAL)/*�{�X�ɗL��*/;/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) �{�X�ɖ���*/
				h->callback_mover	= remilia_move_burn_fire;
				h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x64);		/*	h->alpha			= 0x64 100;*/
				h->base_weapon_strength 	= (8*(5));/* �d�l�ύX���� */	/*9*/ /*10*/
				{
			u32 my_rand;
				my_rand = ra_nd();
					if (YUYUKO==select_player)
					{
						h->cx256			= (u16)((my_rand	)) + t256((GAME_WIDTH -256)/2);
						h->cy256			= (u16)((my_rand>>16)) + t256((GAME_HEIGHT-256)/2);
					}
					else
					{
						#if 1/*Gu(���S���W)*/
						h->cx256			= src->cx256;
						h->cy256			= src->cy256+t256(15);
						#endif
					}
				//	int rnd_spd 		= (ra_nd() & 0xff/*angCCW512*/)+256+((angCCW512+32+64) & 0x40);
				//	int aaa_spd 		= (((angCCW512+32+64) & 0x40));
				//	int rnd_spd 		= (ra_nd() & 0xff/*angCCW512*/)+256+(aaa_spd+aaa_spd);
			int rnd_spd 		= (my_rand & 0xff/*angCCW512*/)+256+(0x40+0x40);
			int ang_aaa_1024;
				ang_aaa_1024 = angCCW1024;
				ang_aaa_1024 += ((my_rand>>8) & (0x7f));
				ang_aaa_1024 += ((my_rand	) & (0x3f));
			//	ang_aaa_1024 -= (0x3f);
			//	ang_aaa_1024 -= (0x1f);
				ang_aaa_1024 -= (0x5f);
				mask1024(ang_aaa_1024);
				#if 1
				/* �`��p�p�x(����0�x�ō����(�����v���)) */
				h->rotationCCW1024		= ang_aaa_1024;
				#endif
				h->vx256		= ((sin1024((ang_aaa_1024))*(rnd_spd))>>8);/*fps_factor*/ /* CCW�̏ꍇ */
				h->vy256		= ((cos1024((ang_aaa_1024))*(rnd_spd))>>8);/*fps_factor*/
				}
			}
		}
	}
}
/* �\���V���b�g�{���̉��̕��� */
/*		r_or_l	[xxx_r] l=2, [xxx_l] r=0, u=3, d=1	*/
static void remilia_add_burn_shot(SPRITE *src/*, int ggg*/ /*r_or_l*/)	/* [***090221 �ǉ� */
{
	{
		int i;
		for (i=0; i<((4*3)-1); i++)
		{
			SPRITE *h;
			h = sprite_add_gu_error();
			if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
			{
				//
				h->m_Hit256R		= JIKI_ATARI_ITEM_16;
//				h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);
//				h->type 			= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_04|SP_GROUP_SHOT_SPECIAL)/*�{�X�ɗL��*/;/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) �{�X�ɖ���*/
				h->type 			= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_01|SP_GROUP_SHOT_SPECIAL)/*�{�X�ɗL��*/ /*SP_PL_HLASER*/;
				h->callback_mover	= remilia_move_burn_fire;
//				h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x64);		/*	h->alpha			= 0x64 100;*/
				h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0xff);		/*	h->alpha			= 0x64 100;*/
				h->base_weapon_strength 	= (8*(5));/* �d�l�ύX���� */	/*9*/ /*10*/
				#if 1/*Gu(���S���W)*/
				h->cx256			= src->cx256;
				h->cy256			= src->cy256+t256(15);
				#endif
				{
				//	int rnd_spd 		= (ra_nd() & 0xff/*angCCW512*/)+256+((angCCW512+32+64) & 0x40);
				//	int aaa_spd 		= (((angCCW512+32+64) & 0x40));
				//	int rnd_spd 		= (ra_nd() & 0xff/*angCCW512*/)+256+(aaa_spd+aaa_spd);
			//	u32 rnd_spd 		= (ra_nd() & 0xff/*angCCW512*/) | 0x0100;
				#if 1
				/* �`��p�p�x(����0�x�ō����(�����v���)) */
//				h->rotationCCW1024		= ang_aaa_1024;
				h->rotationCCW1024		= (0);
				#endif
			//	h->vx256		= (( ((src->vx256))*(rnd_spd))>>8);/*fps_factor*/ /* CCW�̏ꍇ */
			//	h->vy256		= (( ((src->vy256))*(rnd_spd))>>8);/*fps_factor*/
				h->vx256		= (src->vx256) ;/*fps_factor*/ /* CCW�̏ꍇ */
				h->vy256		= (src->vy256) ;/*fps_factor*/
				u32 rnd_spd 		= (ra_nd() ) ;
				if (rnd_spd&0x01)	{	h->vx256 = - h->vx256;	}
				if (rnd_spd&0x02)	{	h->vy256 = - h->vy256;	}
				}
			}
		}
	}
}

/*---------------------------------------------------------
	�얲 A ��p �ᑬ�{���ǉ�
	�아�얲
	�{��:���z����
	�z�[�~���O���e��7���o���B�G�����Ȃ����͂��΂炭���̏�ɒ�؁B
	�З͂͗����ɂ��ō����A���G���Ԃ������B���e�̋O�ՁA�y��ɒe�����B
---------------------------------------------------------*/

static void reimu_add_reifu_musofuuin(SPRITE *src)
{
//	int j;
//	j = (ra_nd()&(2-1));//for (j=0;j<2;j++)
	{
		//#define hlaser_NUM_OF_ENEMIES (24)
//		#define hlaser_NUM_OF_ENEMIES (12)		/* [***090128		�����ɂ��Ă݂� */
	//	int ii;
	//	for (i=0; i<(12)/*hlaser_NUM_OF_ENEMIES*/; i++)
	//	for (i=0; i<(16)/*hlaser_NUM_OF_ENEMIES*/; i++)
	//	for (ii=0; ii<(1)/*hlaser_NUM_OF_ENEMIES*/; ii++)
		static int musou_id;
		musou_id++;
		musou_id &= 0x07;
		{
			SPRITE *h;
			h						= sprite_add_gu_error();
			if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
			{
				h->m_Hit256R		= JIKI_ATARI_ITEM_16;
	//			h->type 			= (SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_SPECIAL)/*�{�X�ɗL��*/ /*SP_PL_HLASER*/;
				h->type 			= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_01|SP_GROUP_SHOT_SPECIAL)/*�{�X�ɗL��*/ /*SP_PL_HLASER*/;
				#if 1/*Gu(���S���W)*/
			//	h->cx256			= src->cx256 - t256(5);
			//	h->cy256			= src->cy256 + t256(15);
				h->cx256			= src->cx256;
				h->cy256			= src->cy256;
				#endif
			//	h->flags			|= (SP_FLAG_VISIBLE/*|SP_FLAG_TIME_OVER*/);
//				h->flags			&= (~(SP_FLAG_VISIBLE));	/*��\��*/
	//			if (0==i)
				{
					h->callback_mover	= move_reimu_musou_fuuin;
	//				h->rotationCCW1024	= cv1024r(0);
	//				h->rotationCCW1024	= (0/*j&1*/)?(cv1024r(	0+10)):(cv1024r(360-10));
	//				h->rotationCCW1024	= (i<<6);
					/* 180 ... 0 (1024/128==8) */
					h->rotationCCW1024	= ( (((int)pd.bomber_time)<<3)+(512));/* �ˏo������� */
					#if 0
					/* (�����K�v�Ȃ�����)�O�̈׃}�X�N */
					mask1024(src->rotationCCW1024/*src->PL_HOMING_DATA_angleCCW1024*/);
					#endif
					h->PL_HOMING_DATA_speed256				= t256(1.2);
					h->base_weapon_strength 				= player_fix_status[BASE_LOW_BOMB_STRENGTH+select_player]/*6*/;/*�ᑬ�{���̋���*/
				//	h->PL_HOMING_DATA_time_out/*range*/ 	= (100);	/*(200)*/
					h->PL_HOMING_DATA_reimu_musou_id		= (musou_id);	/*(ii)(200)*/
					h->target_obj							= &obj00[FIX_OBJ_00_PLAYER];			/* h->target_obj == obj00[FIX_OBJ_00_PLAYER];�݂���Ȃ� */
				}
			}
		}
	}
}

/*
add2-------------------------------------------------------------------------
*/
/*---------------------------------------------------------
	�v���C���[�U���e����
---------------------------------------------------------*/

static void marisa_add_teisoku_yuudoudan(SPRITE *src)
{
	int j;	j=(ra_nd()&(2-1));//for (j=0;j<2;j++)
	{
		SPRITE *h_old;	h_old = NULL;
		SPRITE *h;		h = NULL;
		//#define hlaser_NUM_OF_ENEMIES (24)
		//#define hlaser_NUM_OF_ENEMIES (12)		/* [***090128		�����ɂ��Ă݂� */
		#define hlaser_NUM_OF_ENEMIES (8)			/* 8���ʂŏ\������(?) */
		unsigned int i;
		unsigned int tmp_color32;
		tmp_color32 = 0xffffffff;
		for (i=0; i<hlaser_NUM_OF_ENEMIES; i++)
		#undef hlaser_NUM_OF_ENEMIES
		{
			h_old = h;
			h					= sprite_add_gu_error();
	//	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */	/* �����o�^ */
		if (NULL==h)return;/* �o�^�ł����ꍇ�̂� */
/* ���[��... */
			h->m_Hit256R		= JIKI_ATARI_ITEM_16;
//			h->type 			= (SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_SPECIAL)/*�{�X�ɗL��*/ /*SP_PL_HLASER*/;
			h->type 			= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_01|SP_GROUP_SHOT_SPECIAL)/*�{�X�ɗL��*/; /*SP_PL_HLASER*/
			#if 1/*Gu(���S���W)*/
		//	h->cx256			= src->cx256 - t256(5);
		//	h->cy256			= src->cy256 + t256(15);
			h->cx256			= src->cx256;
			h->cy256			= src->cy256;
			#endif
		//	h->flags			|= (SP_FLAG_VISIBLE/*|SP_FLAG_TIME_OVER*/);
//			h->flags			&= (~(SP_FLAG_VISIBLE));	/*��\��*/
//				h->PL_HOMING_KODOMO_DATA_color256		= (t256(1.0));	/* ���邳(�A���t�@�����l) */
			h->color32			= tmp_color32;
//			tmp_color32 -= 0x10000000;
//			tmp_color32 -= 0x20000000;
			tmp_color32 -= 0x20202020;
			if (0==i)
			{
				h->callback_mover	= marisa_move_parrent_hlaser;
//				h->rotationCCW1024						= cv1024r(0);
				h->rotationCCW1024						= (j&1)?(cv1024r(  0+10)):(cv1024r(360-10));
				h->PL_HOMING_DATA_speed256				= t256(4.0);//t256(12.0);
				h->base_weapon_strength 				= player_fix_status[BASE_LOW_BOMB_STRENGTH+select_player];/*6*/ /*�ᑬ�{���̋���*/
				h->PL_HOMING_DATA_time_out/*range*/ 	= (HOMING_128_TIME_OUT);	/*100(200)*/
				h->target_obj							= &obj00[FIX_OBJ_00_PLAYER];	/* (h->target_obj==obj00[FIX_OBJ_00_PLAYER])==������Ȃ� */
			}
			else
			{
				h->callback_mover	= marisa_move_kodomo_hlaser/*NULL*/;
				h->target_obj		= h_old;
				h->PL_HOMING_KODOMO_DATA_time_out		= 8;
			}
		}
	}
}

//(r31)
//	 0	1  2  3  4	5  6  7  8	9 10 11 [�ړ���������]
//	�� �� �� �� �� �� �� �� �� �� �� ��
//	 p	0  1  2  3	4  5  6  7	8  9 10
// ���ʓI�� �S���d�Ȃ�o�O�B

//(r32)
//	11	10	9  8  7  6	5  4  3  2	1  0 [�ړ���������]
//	��	�� �� �� �� �� �� �� �� �� �� ��
//	10	 9	8  7  6  5	4  3  2  1	0  p

/* tuika �ǉ��n ------------------------------------------------------------------------ */

/*---------------------------------------------------------
	(�얲)���z����̒ǉ�[�{���ǉ��{��]
---------------------------------------------------------*/

static void reimu_tuika_musofuuin_shot(SPRITE *src) /* [***090220 �ǉ� */
{
	SPRITE *zzz_player;
	zzz_player = &obj00[FIX_OBJ_00_PLAYER];
	//
//	if (0x3f==(((int)pd_bomber_time)&0x3f))
//	if (0x0f==(((int)pd_bomber_time)&0x0f))
	if (0x0f==((src->jyumyou)&0x0f))/* �g�������^�C�~���O */
	{
		/* �V�i���I���ɂ͗U���e��ǉ����Ȃ� */
		if (0==(pd.state_flag & STATE_FLAG_06_IS_SCRIPT))
		{
			reimu_add_reifu_musofuuin(zzz_player);/* �g������(�������t?) */
		}
	}
	#if 1/*Gu(���S���W)*/
	src->cx256 = zzz_player->cx256;
	src->cy256 = zzz_player->cy256;
	#endif
/* ���Ԑ����ł����܂� */	//if (0 >= pd.bomber_time)	{ src->jyumyou = JYUMYOU_NASI;}/* ���Ԑ����ł����܂� */
}

/*---------------------------------------------------------
	�������A�`���m���A�ᑬ�{���̒ǉ�[�{���ǉ��{��]
---------------------------------------------------------*/
static void marisa_tuika_bomber_teisoku_yuudoudan(SPRITE *src)
{
	SPRITE *zzz_player;
	zzz_player = &obj00[FIX_OBJ_00_PLAYER];
	//
//	if (0==(((int)pd_bomber_time)&0x3f))
//	if (0==(((int)pd_bomber_time)&0x0f))/*(r31)*/
	if (0==((src->jyumyou)&0x07))/*(r32)�ǉ��ʂ�2�{�ɂ��Ă݂�B*/
	{
		/* �V�i���I���ɂ͗U���e��ǉ����Ȃ� */
		if (0==(pd.state_flag & STATE_FLAG_06_IS_SCRIPT))
		{
			marisa_add_teisoku_yuudoudan(zzz_player);
		}
	}
/* ���Ԑ����ł����܂� */	//	if (0 >= pd.bomber_time)	{	src->jyumyou = JYUMYOU_NASI;}/* ���Ԑ����ł����܂� */
}

/*---------------------------------------------------------
	���~���A�{���̏\���V���b�g�̒ǉ�[�{���ǉ��{��]
---------------------------------------------------------*/

static void remilia_tuika_cross_shot(SPRITE *src) /* [***090220 �ǉ� */
{
	//
//	if (0x3f==(((int)pd_bomber_time)&0x3f))
//	if (0x0f==(((int)pd_bomber_time)&0x0f))
//	if (0x0f==(((int)pd_bomber_time)&0x0f))/* �g�������^�C�~���O */
	if (0x07==((src->jyumyou)&0x07))/* �g�������^�C�~���O */
	{
		/* �V�i���I���ɂ͗U���e��ǉ����Ȃ� */
		if (0==(pd.state_flag & STATE_FLAG_06_IS_SCRIPT))
		{
			remilia_add_burn_shot(src);/* �g������(�������t?) */
		}
	}
	#if 1/*Gu(���S���W)*/
	/* �e�ړ� */
	src->cx256 += src->vx256;
	src->cy256 += src->vy256;
	#endif
/* ���Ԑ����ł����܂� */	//	if (0 >= pd.bomber_time)	{ src->jyumyou = JYUMYOU_NASI;}
}

/*---------------------------------------------------------
	(�H�X�q�ᑬ)���~���A�{���̏\�����̒ǉ�[�{���ǉ��{��]
---------------------------------------------------------*/

static void yuyuko_tuika_meifu(SPRITE *src) /* [***090220 �ǉ� */
{
	SPRITE *zzz_player;
	zzz_player = &obj00[FIX_OBJ_00_PLAYER];
	//
//	if (0x3f==(((int)pd_bomber_time)&0x3f))
//	if (0x0f==(((int)pd_bomber_time)&0x0f))
	if (0x0f==((src->jyumyou)&0x0f))/* �g�������^�C�~���O */
	{
		/* �V�i���I���ɂ͗U���e��ǉ����Ȃ� */
		if (0==(pd.state_flag & STATE_FLAG_06_IS_SCRIPT))
		{
			yuyuko_add_meifu(zzz_player);/* �g������(�������t?) */
		}
	}
	#if 1/*Gu(���S���W)*/
	src->cx256 = zzz_player->cx256;
	src->cy256 = zzz_player->cy256;
	#endif
/* ���Ԑ����ł����܂� */	//	if (0 >= pd.bomber_time)	{ src->jyumyou = JYUMYOU_NASI;}/* ���Ԑ����ł����܂� */
}



/*
parrent �e�n -------------------------------------------------------------------------
*/
/*---------------------------------------------------------
	�v���C���[�V�[���h�����̐e(�얲)
---------------------------------------------------------*/

/*static*/global void reimu_create_bomber_kekkai_parrent(SPRITE *src)		/* �얲 */		//�V�[���h�̒ǉ�
{
	pd.state_flag	|= STATE_FLAG_02_BOMB_AUTO_GET_ITEM;	/*�{���ɂ�鎩�����W�\*/
	int ii;//	int i;	/* ���ی����� */
	for (ii=0; ii<(8); ii++)//	for (i=0;i<360/*i<=359*/;i+=45)
	{
		SPRITE *h;
		h				= sprite_add_gu_error();
	//	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
		if (NULL==h)return;/* �o�^�ł����ꍇ�̂� */
		{
			h->m_Hit256R			= TAMA_ATARI_04;
			if (0 == (ii&1))// if (i%90 == 0)
			{
				h->PL_KEKKAI_DATA_add_r1024 	= ( 46);							/* ���v��� */
				h->PL_KEKKAI_DATA_radius		= (5);		/*(48)*/	//d->radius=38; 	/*���a�����l*/	/* �������L����悤�ɂ��� */
			}
			else		/* 1���ƕs���Ȃ̂�2������B */
			{
				h->PL_KEKKAI_DATA_add_r1024 	= (-34);							/* �����v��� */
				h->PL_KEKKAI_DATA_radius		= (0);		/*(45)*/	//d->radius=35; 	/*���a�����l*/	/* �������L����悤�ɂ��� */
			}
	//		if (YUYUKO==select_player)
	//		{	/* �H�X�q */
	//			h->PL_KEKKAI_DATA_radius		+= 45/*45*/;		/*���a�����l*/
	//			h->type 		= (SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO)/*�{�X�ɖ���*/;
	//		}
	//		else // if (REIMU==select_player)
			{	/* �얲 */
				h->type 				= (ii&(4-1))+(/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_04|SP_GROUP_SHOT_SPECIAL)/*�{�X�ɗL��*/;/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) �{�X�ɖ���*/
			}
			h->callback_mover			= reimu_move_rotate_kekkai; 		/*	���v��� �� �����v��� �� �X�N�G�A���E(�얲B) ���p */
		//	h->callback_mover			= reimu_move_rotate_kekkai/*2*/;		/* �����v��� */
//			h->flags					|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);
			h->PL_KEKKAI_DATA_angleCCW1024		= (ii*cv1024r((45)));	/*i*/
			/* �o�n�߂̓v���C���[���W */
			{
				SPRITE *zzz_player;
				zzz_player = &obj00[FIX_OBJ_00_PLAYER];
				h->cx256					= zzz_player->cx256;
				h->cy256					= zzz_player->cy256;
			}
			h->base_weapon_strength 	= (8*1);	/*5*/	/* [***090214 �ǉ� */
		}
	}
}
/*---------------------------------------------------------
	�}�X�^�[�X�p�[�N�A�{�������̐e
	(������B ��p)
---------------------------------------------------------*/

/*static*/global void marisa_create_bomber_master_spark_parrent(SPRITE *src)
{
	pd.state_flag	|= STATE_FLAG_02_BOMB_AUTO_GET_ITEM;	/*�{���ɂ�鎩�����W�\*/
	int iii;
	for (iii=0; iii<(3)/*16*/; iii++)
	{
		SPRITE *h;
		h					= sprite_add_gu_error();
	//	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
		if (NULL==h)return;/* �o�^�ł����ꍇ�̂� */ 	/*���[��H�H�H*/
		{
			const static u32 color_mask32[(4)] =
			{	//		   RED	 GREEN BLUE  ALPHA
				MAKE32RGBA(0xff, 0x00, 0x00, 0x3f), 	// (��)
				MAKE32RGBA(0x00, 0xff, 0x00, 0x3f), 	// (��)
				MAKE32RGBA(0x00, 0x00, 0xff, 0x3f), 	// (��)
				MAKE32RGBA(0xff, 0xff, 0xff, 0x3f), 	// (��)
			};
			h->color32					= (color_mask32[iii]);
			h->m_Hit256R				= ((128)<<8);
	//		h->type 					= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_04|SP_GROUP_SHOT_ZAKO)/*�{�X�ɖ���*/;/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_SPECIAL)*/ /*�{�X�ɗL��*/
			h->type 					= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_04|SP_GROUP_SHOT_SPECIAL)/*�{�X�ɗL��*/ /*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) �{�X�ɖ���*/;
//			h->flags					|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);
			h->callback_mover			= marisa_move_master_spark;
			h->tmp_angleCCW1024 		= ((iii)<<8);
			#if (1==USE_ZOOM_XY)
			h->m_zoom_x256				= (256*4);	/* �`��p�{���̏����T�C�Y / size of bomb at first. */
			h->m_zoom_y256				= (256*4);	/* �`��p�{���̏����T�C�Y / size of bomb at first. */
			#else
			h->m_zoom_xy256 			= (256*4);	/* �`��p�{���̏����T�C�Y / size of bomb at first. */
			#endif
			h->base_weapon_strength 	= player_fix_status[BASE_STD_BOMB_STRENGTH+select_player]; /*k_strength*/ /*5*/ /*15-k-k*/ /* [***090214 �ǉ� */

			h->vx256					= (0);	/*fps_factor*/	/* CCW�̏ꍇ */
			h->vy256					= (0);	/*fps_factor*/
			#if 1/*�ȗ��\�����H*/
			h->cx256					= src->cx256;
			h->cy256					= src->cy256-t256(MASKER_SPARK_LENGTH144);
			#endif
//			/* �`��p�p�x(����0�x�ō����(�����v���)) */
//			h->rotationCCW1024		= (0);
		}
	}
}

/*---------------------------------------------------------
	�M���X�g���h���[��
	�v���C���[�{�������̐e
	(�H�X�q �� �`���m)
---------------------------------------------------------*/

/*static*/global void yuyuko_create_bomber_gyastry_dream_parrent(SPRITE *src)
{
	pd.state_flag	|= STATE_FLAG_02_BOMB_AUTO_GET_ITEM;	/*�{���ɂ�鎩�����W�\*/
	int jjj;		jjj=0;
	int angCCW1024; angCCW1024 = (0);/* 0 �`���m�p(�b��I)*/
	int iii;
	for (iii=0; iii<(18*3)/*16*/; iii++)
	{
		SPRITE *h;
		h					= sprite_add_gu_error();
	//
	//	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
		if (NULL==h)return;/* �o�^�ł����ꍇ�̂� */ 	/*���[��H�H�H*/
		{
			const static u8 base_bomber_atari[8] =
			{
				  (2),	//	BASE_BOMBER_ATARI_0a,/*4*/		/* �얲A */ 	/*	5 */	/*0,*/	 /* re a */  //  BASE_BOMBER_ATARI_0a,/*4*/
				  (2),	//	BASE_BOMBER_ATARI_0b,/*4*/		/* �얲B */ 	/*	5 */	/*0,*/	 /* re b */  //  BASE_BOMBER_ATARI_0a,/*4*/
				 (16),	//	BASE_BOMBER_ATARI_0c,/*16*/ 	/* ������A */	/*	6 */	/*0,*/	 /* ma a */  //  BASE_BOMBER_ATARI_0b,/*16*/
				 (16),	//	BASE_BOMBER_ATARI_0d,/*16*/ 	/* ������B */	/*	6 */	/*0,*/	 /* ma b */  //  BASE_BOMBER_ATARI_0b,/*16*/
				 (16),	//	BASE_BOMBER_ATARI_0e,/*16*/ 	/* ������C */	/*	7 */	/*0,*/	 /* oz / */  //  BASE_BOMBER_ATARI_0c,/*16*/
				  (4),	//	BASE_BOMBER_ATARI_0f,/*16*/ 	/* ���~���A */	/*	9 */	/*0,*/	 /* yu / */  //  BASE_BOMBER_ATARI_0e,/*4*/
				 (32),	//	BASE_BOMBER_ATARI_0g,/*32*/ 	/* �`���m */	/*	8 */	/*0,*/	 /* ci a */  //  BASE_BOMBER_ATARI_0d,/*32*/
				 (32),	//	BASE_BOMBER_ATARI_0h,/*4*/		/* �H�X�q */	/*	6 */	/*0,*/	 /* ci q */  //  BASE_BOMBER_ATARI_0b,/*16*/
			};
			h->m_Hit256R			= ((base_bomber_atari[select_player])<<8);
			//
			jjj++;
			jjj &= 0x03;	//	if (4==jjj) 	{	jjj=0;	}
	//		h->type 		= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_04|SP_GROUP_SHOT_ZAKO)/*�{�X�ɖ���*/;/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_SPECIAL)*/ /*�{�X�ɗL��*/
			h->type 		= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_04|SP_GROUP_SHOT_SPECIAL)/*�{�X�ɗL��*/ /*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) �{�X�ɖ���*/;
			h->type += jjj;
	//

//			h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);
			h->callback_mover	= marisa_yuyuko_move_levarie_gyastry_dream;
			#if 0
			{
				static const u16 player_fix_status_ggg[8] =
				{/* REIMU(A/B) MARISA(A/B) REMILIA YUYUKO CIRNO(A/Q) */
					256, 256, 256, 256, 32, 256, 32, 32,	/* �`��p�{���̏����T�C�Y / size of bomb at first. */
				};
				h->m_zoom_x256		= player_fix_status_ggg[select_player]/*8*/;/* 64 == (1/4) */
			}
			#else
			if (YUYUKO!=select_player)
			{
				#if (1==USE_ZOOM_XY)
				h->m_zoom_x256		= 1+(ra_nd()&(64-1));	/* �`��p�{���̏����T�C�Y / size of bomb at first. */
				h->m_zoom_y256		= 1+(ra_nd()&(64-1));	/* �`��p�{���̏����T�C�Y / size of bomb at first. */
				#else
				h->m_zoom_xy256 	= 1+(ra_nd()&(64-1));	/* �`��p�{���̏����T�C�Y / size of bomb at first. */
				#endif
			}
			#endif

			h->base_weapon_strength 	= player_fix_status[BASE_STD_BOMB_STRENGTH+select_player] /*k_strength*/ /*5*/ /*15-k-k*/;/* [***090214 �ǉ� */
			/* MARISA�͋������g�U���Ă��܂��̂ł�����Ȃ��B  */
			/* �`���m�͓��Ă₷���̂ł��̕��キ���Ȃ��Ƌ��������Ⴄ�B  */
			int d_speed256;
			#if 0
			if (0==(iii&(2-1)/*%2*/))
			{			/*d->*/d_speed256	= t256(1.0);	c->alpha	= 150;	}
			else	{	/*d->*/d_speed256	= t256(1.2);	c->alpha	= 180;	}
			#else
			/*d->*/d_speed256	= t256(1.0)+(iii<<3);
			#endif
			h->vx256			= ((sin1024((angCCW1024))*(d_speed256))>>8);/*fps_factor*/	/* CCW�̏ꍇ */
			h->vy256			= ((cos1024((angCCW1024))*(d_speed256))>>8);/*fps_factor*/
	//
			#if 1/*Gu(���S���W)*/
			h->cx256			= src->cx256;
			h->cy256			= src->cy256;
			#endif
			/* �M���X�g���h���[��(���ǂ�)�B�X�^�[�_�X�g�����@���G�ƈႢ�ߐڐ��\�͖����B */
			if (YUYUKO==select_player)	/* �H�X�q */
			{
				h->cx256		+= ((h->vx256)<<5);
				h->cy256		+= ((h->vy256)<<5);
	//			/* �H�X�q �������邩��ȁ`(���ʎア�����ʂ���̂��o����悤�ɃV�X�e�����������ق�����������) */
			}
			#if 1
			/* �`��p�p�x(����0�x�ō����(�����v���)) */
			h->rotationCCW1024		= angCCW1024;
			#endif
		//	angCCW1024 += (int)((1024)/16); // 22.5�x/360, 2x��/16 /* 0.392699081698724154810 */
			angCCW1024 += (int)((1024)/18); // 20.0�x/360, 2x��/18 /* 0.349065850398865915384 */
		}
	}
}


/*---------------------------------------------------------
	�����u�X�^�[�_�X�g�����@���G�v
	-------------------------------------------------------
	[�v���C���[�V�[���h]�{�������̐e(������ A)
	-------------------------------------------------------
	�X�^�[�_�X�g�����@���G(3x8==�v24�����o��)
	�͂W�����ɐ����o��B���̐F�́A���܂��Ă���B

		����	��		�E��
		(��0)	(��2)	(��1)

		��				�E
		(��1)			(��2)

		����	��		�E��
		(��2)	(��0)	(��1)

	��==0
	��==1
	��==2

	-------------------------------------------------------
	������ł́A�R�����o��B�摜�̍�����W��(x,y)�Ƃ���ƁA

	�启=�������A���W(x,y)
	����=�������A���W(x,y)
	����=�s�����A���W(x+64,y+64)	// 64 �� �e�L�g�[�Ȉ��l�B
									// psp�̏ꍇ�́A��ʂ������̂�48���Ó��H
	---------------------------------------------------------*/

/*static*/global void marisa_create_bomber_star_dust_levarie_parrent(SPRITE *src)
{
	pd.state_flag	|= STATE_FLAG_02_BOMB_AUTO_GET_ITEM;	/*�{���ɂ�鎩�����W�\*/
//
	int angCCW1024; angCCW1024 = (0);/* 0 �`���m�p(�b��I)*/
	int iii;
	for (iii=0; iii<(8)/*16*/; iii++)/*�W����*/
	{
		int kkk;
		for (kkk=0; kkk<(3)/*16*/; kkk++)/*3�T�C�Y*/
		{
			SPRITE *h;
			h					= sprite_add_gu_error();
		//	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
			if (NULL==h)return;/* �o�^�ł����ꍇ�̂� */ 	/*���[��H�H�H*/
			{
			//	h->m_Hit256R			= ((16)<<8);//((base_bomber_atari[select_player])<<8);
				h->m_Hit256R			= ((32)<<8);//((base_bomber_atari[select_player])<<8);
				const static u32 color_mask32[8] =
				{	//		   RED	 GREEN BLUE  ALPHA
					MAKE32RGBA(0xff, 0x00, 0x00, 0x7f), 	//	  ��(��0)
					MAKE32RGBA(0x00, 0xff, 0x00, 0x7f), 	//	�E��(��1)
					MAKE32RGBA(0x00, 0x00, 0xff, 0x7f), 	//	�E	(��2)
					MAKE32RGBA(0x00, 0xff, 0x00, 0x7f), 	//	�E��(��1)
					MAKE32RGBA(0x00, 0x00, 0xff, 0x7f), 	//	  ��(��2)
					MAKE32RGBA(0xff, 0x00, 0x00, 0x7f), 	//	����(��0)
					MAKE32RGBA(0x00, 0xff, 0x00, 0x7f), 	//	��	(��1)
					MAKE32RGBA(0x00, 0x00, 0xff, 0x7f), 	//	����(��2)
				};
				h->color32					= (color_mask32[iii]);
		//		h->type 					= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_04|SP_GROUP_SHOT_ZAKO)/*�{�X�ɖ���*/;/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_SPECIAL)*/ /*�{�X�ɗL��*/
				h->type 					= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_04|SP_GROUP_SHOT_SPECIAL)/*�{�X�ɗL��*/ /*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) �{�X�ɖ���*/;
		//
	//			h->flags					|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);
				h->callback_mover			= marisa_yuyuko_move_levarie_gyastry_dream;
				#if (1==USE_ZOOM_XY)
				h->m_zoom_x256				= (0x100<<(kkk));	/* �`��p�{���̏����T�C�Y / size of bomb at first. */
				h->m_zoom_y256				= (0x100<<(kkk));	/* �`��p�{���̏����T�C�Y / size of bomb at first. */
				#else
				h->m_zoom_xy256 			= (0x100<<(kkk));	/* �`��p�{���̏����T�C�Y / size of bomb at first. */
				#endif
				h->base_weapon_strength 	= player_fix_status[BASE_STD_BOMB_STRENGTH+select_player] /*k_strength*/ /*5*/ /*15-k-k*/;/* [***090214 �ǉ� */
				{
//					const int d_speed256	= t256(1.5);	/* ���x��� */
					const int d_speed256	= t256(1.5);	/* ���x��� */
					h->vx256			= ((sin1024((angCCW1024))*(d_speed256))>>8);/*fps_factor*/	/* CCW�̏ꍇ */
					h->vy256			= ((cos1024((angCCW1024))*(d_speed256))>>8);/*fps_factor*/
				}//
				#if 1/*Gu(���S���W)*/
				{
				//	const int aaa111	= ((0==kkk)?(t256(48)):(0));	/* ���x��� */
					h->cx256			= src->cx256;//+(aaa111);
					h->cy256			= src->cy256;//+(aaa111);
				}
				if (0==kkk)
				{
					h->cx256			+= (t256(48));
					h->cy256			+= (t256(48));
					h->color32			|= (MAKE32RGBA(0, 0, 0, 0xff)); 	/*	h->alpha			= 0xff 255;*/
				}
				#endif
				#if 1
				/* �`��p�p�x(����0�x�ō����(�����v���)) */
				h->rotationCCW1024		= angCCW1024;
				#endif
			}
		}
		angCCW1024 += (int)((1024)/8);	// 45.0�x/360, 64
	}
}

/*---------------------------------------------------------
	�v���C���[�V�[���h����(���~���A)�̐e[���~���A�{���̏\���V���b�g�{��]
---------------------------------------------------------*/

/*static*/global void remilia_create_bomber_cross_shot_parrent(SPRITE *src) /* ���~���A */ /* [***090220 �ǉ� */
{
	pd.state_flag		|= STATE_FLAG_02_BOMB_AUTO_GET_ITEM;	/*�{���ɂ�鎩�����W�\*/
	SPRITE *h;
	h						= sprite_add_gu_error();
	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */	/* ���[��... */
	{
		h->jyumyou			= (PD_BOMBER_JYUMYOU180);
//		h->m_Hit256R		= JIKI_ATARI_ITEM_16;
//		h->flags			|= (/*SP_FLAG_VISIBLE|*/SP_FLAG_TIME_OVER);
//		h->flags			&= (~(SP_FLAG_VISIBLE));	/*��\��*/
//		h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0xdc);		/*	h->alpha			= 0xdc;*/	/*��\��*/
		h->callback_mover	= remilia_tuika_cross_shot;
	//	h->type 			= (SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO);/* �{�X�̒��ڍU���͋֎~ */		/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) �{�X�ɖ���*/
//		h->type 			= (/*�\�����Ȃ�*/S P_GROUP_ETC_DUMMY_REMILIA/*SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO*/);/* �{�X�̒��ڍU���͋֎~ */		/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) �{�X�ɖ���*/
		h->type 			= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_01|SP_GROUP_SHOT_SPECIAL)/*�{�X�ɗL��*/; /*SP_PL_HLASER*/
	//	#if 1/*Gu(���S���W)*/
	//	h->cx256			= t256(GAME_NOT_VISIBLE480);//(src->cx256); /*��\��*/
//	//	h->cy256			= (src->cy256); /*��\��*/
	//	#endif
		SPRITE *zzz_player;
		zzz_player = &obj00[FIX_OBJ_00_PLAYER];
		#if 1/*Gu(���S���W)*/
		/*�J�n���̍��W*/
		h->cx256 = zzz_player->cx256;
		h->cy256 = zzz_player->cy256;
		#endif
		{
//			SPRITE *obj_boss;
//			obj_boss = &obj00[FIX_OBJ_08_BOSS];
			int int_angle1024;
			int int_aaa;
		//	int_angle1024 = at an_1024(global_obj_boss->cy256-zzz_player->cy256, global_obj_boss->cx256-zzz_player->cx256);
			int_angle1024 = atan_65536(global_obj_boss->cy256-zzz_player->cy256, global_obj_boss->cx256-zzz_player->cx256);
			int_angle1024 >>= (6);
			int_aaa = sin1024((int_angle1024)); 	h->vx256	 = int_aaa+int_aaa;/*fps_factor*/ /* CCW�̏ꍇ */
			int_aaa = cos1024((int_angle1024)); 	h->vy256	 = int_aaa+int_aaa;/*fps_factor*/
			#if 1
			/* �`��p�O����] */
		//	h->rotationCCW1024	= int_angle1024;
			/* �`��p�O����](�V���b�g�͏オ���ʂȂ̂ŁA��]�p���v�Z)[180/360�x]�񂷁B */
			int_angle1024 += cv1024r((180));
			mask1024(int_angle1024);
			h->rotationCCW1024	= int_angle1024;/* �������ʂ̊p�x���`��p�p�x */
			#endif
		}
	}
}

/*---------------------------------------------------------
	�������A�`���m���A�ᑬ�{���̐e
---------------------------------------------------------*/
/*static*/global void marisa_create_bomber_homing_parrent(SPRITE *src)
{
	pd.state_flag		|= STATE_FLAG_02_BOMB_AUTO_GET_ITEM;	/* �{���ɂ�鎩�����W�\ */
//
	SPRITE *h;
	h						= sprite_add_gu_error();
	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */	/* ���[��... */
	{
		h->jyumyou			= (PD_BOMBER_JYUMYOU180);
//		h->m_Hit256R		= JIKI_ATARI_ITEM_16;
	//	h->flags			|= (SP_FLAG_VISIBLE/*|SP_FLAG_TIME_OVER*/);
//		h->flags			&= (~(SP_FLAG_VISIBLE));	/*��\��*/
//		h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0xdc);	/*	c->alpha			= 0x80 0xdc;*/	/*��\��*/
		h->callback_mover	= marisa_tuika_bomber_teisoku_yuudoudan;
	//	h->type 			= (SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_SPECIAL)/*�{�X�ɗL��*/; /* �ᑬ�{�����{�X�ɗL���Ƃ��� */	/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) �{�X�ɖ���*/
		h->type 			= (/*�\�����Ȃ�*/SP_DUMMY_MUTEKI/*S P_GROUP_ETC_DUMMY_SLOW_BOMB*/ /*SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_SPECIAL*/)/*�{�X�ɗL��*/;	/* �ᑬ�{�����{�X�ɗL���Ƃ��� */	/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) �{�X�ɖ���*/
		#if 1/*Gu(���S���W)*/
		h->cx256			= t256(GAME_NOT_VISIBLE480);//src->cx256;	/*��\��*/
//		h->cy256			= src->cy256;	/*��\��*/
		#endif
//
		#if 1
		/* �{�������������A�L���ɂ���ƁA�d������B */
		/* ����@�\ */
		bullets_to_hosi();/* �e�S���A���A�C�e���ɂ��� */
		#endif
	}
}
/*---------------------------------------------------------
	�얲 A ��p �ᑬ�{���̐e
---------------------------------------------------------*/
/*static*/global void reimu_create_bomber_homing_shot_parrent(SPRITE *src)	/* reimu_move_add_bomber_hlaser */
{
	pd.state_flag		|= STATE_FLAG_02_BOMB_AUTO_GET_ITEM;	/* �{���ɂ�鎩�����W�\ */
	SPRITE *h;
	h						= sprite_add_gu_error();
	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */	/* ���[��... */
	{
		h->jyumyou			= (PD_BOMBER_JYUMYOU180);
//		h->m_Hit256R		= JIKI_ATARI_ITEM_16;
//		h->flags			|= (/*SP_FLAG_VISIBLE|*/SP_FLAG_TIME_OVER);
//		h->flags			&= (~(SP_FLAG_VISIBLE));	/*��\��*/
//		h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0xdc);		/*	h->alpha			= 0xdc;*/	/*��\��*/
		h->callback_mover	= reimu_tuika_musofuuin_shot;
	//	h->type 			= (SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO);/* �{�X�̒��ڍU���͋֎~ */		/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) �{�X�ɖ���*/
//		h->type 			= (/*�\�����Ȃ�*/S P_GROUP_ETC_DUMMY_REMILIA/*SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO*/);/* �{�X�̒��ڍU���͋֎~ */		/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) �{�X�ɖ���*/
		h->type 			= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_01|SP_GROUP_SHOT_SPECIAL)/*�{�X�ɗL��*/; /*SP_PL_HLASER*/
	//	#if 1/*Gu(���S���W)*/
	//	h->cx256			= t256(GAME_NOT_VISIBLE480);//(src->cx256); /*��\��*/
//	//	h->cy256			= (src->cy256); /*��\��*/
	//	#endif
		SPRITE *zzz_player;
		zzz_player = &obj00[FIX_OBJ_00_PLAYER];
		#if 1/*Gu(���S���W)*/
		/*�J�n���̍��W*/
		h->cx256 = zzz_player->cx256;
		h->cy256 = zzz_player->cy256;
		#endif
		{
//			SPRITE *obj_boss;
//			obj_boss = &obj00[FIX_OBJ_08_BOSS];
			#if 1
			/* �`��p�O����] */
		//	h->rotationCCW1024	= int_angle1024;
			/* �`��p�O����](�V���b�g�͏オ���ʂȂ̂ŁA��]�p���v�Z)[180/360�x]�񂷁B */
			h->rotationCCW1024	= 0;/* �������ʂ̊p�x���`��p�p�x */
			#endif
		}
	}
	#if 0
	/* �{�������������A�L���ɂ���ƁA�d������B */
	/* ����@�\ */
	bullets_to_hosi();/* �e�S���A���A�C�e���ɂ��� */
	#endif
}
/*---------------------------------------------------------
	�H�X�q��p �ᑬ�{���̐e 	[(�H�X�q�ᑬ)�{���̏\�����{��]
---------------------------------------------------------*/
/*static*/global void yuyuko_create_bomber_meifu_parrent(SPRITE *src) /* (�H�X�q�ᑬ) */ /* [***090220 �ǉ� */
{
	pd.state_flag		|= STATE_FLAG_02_BOMB_AUTO_GET_ITEM;	/*�{���ɂ�鎩�����W�\*/
	SPRITE *h;
	h						= sprite_add_gu_error();
	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */	/* ���[��... */
	{
		h->jyumyou			= (PD_BOMBER_JYUMYOU180);
//		h->m_Hit256R		= JIKI_ATARI_ITEM_16;
//		h->flags			|= (/*SP_FLAG_VISIBLE|*/SP_FLAG_TIME_OVER);
//		h->flags			&= (~(SP_FLAG_VISIBLE));	/*��\��*/
//		h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0xdc);		/*	h->alpha			= 0xdc;*/	/*��\��*/
		h->callback_mover	= yuyuko_tuika_meifu;
	//	h->type 			= (SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO);/* �{�X�̒��ڍU���͋֎~ */		/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) �{�X�ɖ���*/
		h->type 			= (/*�\�����Ȃ�*/SP_DUMMY_MUTEKI/*S P_GROUP_ETC_DUMMY_REMILIA*/ /*SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO*/);/* �{�X�̒��ڍU���͋֎~ */		/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) �{�X�ɖ���*/
		#if 1/*Gu(���S���W)*/
		h->cx256			= t256(GAME_NOT_VISIBLE480);//(src->cx256); /*��\��*/
//		h->cy256			= (src->cy256); /*��\��*/
		#endif
	}
}
