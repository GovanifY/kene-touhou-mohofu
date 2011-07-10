
#include "boss.h"

/*static*/extern void add_zako_sakuya_doll_01_laser(SPRITE *src);/* ���[�U�[ */
 /* ���u�� */
/*static*/extern void add_zako_sakuya_doll_04_360knife(SPRITE *src);/* �q�������w(360�i�C�t������) */
/*static*/extern void add_zako_sakuya_baramaki1(SPRITE *src);/* ��� �΂�T��1�e �I�v�V���� */
/*static*/extern void add_zako_sakuya_baramaki2(SPRITE *src);/* ��� �΂�T��2�e �I�v�V���� */

/*static*/extern void bullet_create_sakuya_ryoute_knife(SPRITE *src);	/* ��� ����i�C�t�J�[�h */
/*static*/extern void bullet_crate_sakuya_linear_yuudou(SPRITE *src);	/* ��� �����U���i�C�t�J�[�h */
/*static*/extern void bullet_crate_sakuya_kurukuru(SPRITE *src);		/* ��� ���邭��i�C�t�J�[�h */

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�\�Z�� ���
	Izayoi Sakuya.
	-------------------------------------------------------
	ToDo:
	�ړ��ƒe�̏����𕪗����悤�B�łȂ��Ɩ�킩���B
	�{�X�^�C�}�[�Ή���
	-------------------------------------------------------
	���{�X�^�C�}�[�̓V�X�e���ŃT�|�[�g�\��ł��B(��ƒ�)
	-------------------------------------------------------
	GAME_X_OFFSET �Ή��B
---------------------------------------------------------*/

/*---------------------------------------------------------
	���w��̖����w�ɂ��ă���
	-------------------------------------------------------
	5�ʒ�-�{�X�ŏo�Ă��鎞�́A�傫��60x60(���Ԃ�64x64obj)���炢�B
	5�ʃ{�X�ŏo�Ă��鎞�́A�傫��125x125(���Ԃ�128x128obj)���炢�B
	�͕핗�͊�{�I�ɏk����75%�Ȃ̂ŁA���w��̖����w��
	48x48[dot](2�{�g�厞��96x96[dot])���炢���Ó����ȁH
---------------------------------------------------------*/

/*---------------------------------------------------------
	�����Ȃ��ŃJ�[�h�����B
---------------------------------------------------------*/

/*---------------------------------------------------------
	��11�`��: �ŏI�`��(����3)
---------------------------------------------------------*/

global void sakuya_11_keitai(SPRITE *src)
{
	if (SPELL_00==card.card_number) 	/* �J�[�h�����I���Ȃ�J�[�h���� */
	{
		card_maikai_init_and_get_spell_number(src); 	/* ���݂̌`�ԂŌ��Ă�J�[�h�ԍ����J�[�h�V�X�e���ɐݒ�B */
		src->BOSS_DATA_04_toutatu_wariai256 = t256(1.0);
	}
//
	int iii;
	iii = ra_nd();
	if (0==(iii&0x07))
	{
		src->vx256 = (((signed int)((iii>> 8)&0xff))-127);
		src->vy256 = (((signed int)((iii>>16)&0xff))-127);
	}
}


/*---------------------------------------------------------
	��10�`��: �ŏI�`��(����2)
---------------------------------------------------------*/

global void sakuya_10_keitai(SPRITE *src)
{
	if (SPELL_00==card.card_number) 	/* �J�[�h�����I���Ȃ�J�[�h���� */
	{
		card_maikai_init_and_get_spell_number(src); 	/* ���݂̌`�ԂŌ��Ă�J�[�h�ԍ����J�[�h�V�X�e���ɐݒ�B */
		src->BOSS_DATA_04_toutatu_wariai256 = t256(1.0);
	}
//
	{
		if (src->color32 > 0xf9000000)	/*	if (src->alpha > 0xf9) */	/* (src->alpha>0xff) */
		{
			src->color32 = 0xffffffff;	/*	src->alpha = 0xff;*/
		}
		else /*if (src->alpha<0xff)*/
		{
			src->color32 += 0x05000000; /*	src->alpha += 0x05;*/
		}
		#if (0)//
		src->cx256 -= ((si n1024((src->tmp_angleCCW1024))*(t256(0.03)))>>8);/*fps_factor*/		/* CCW�̏ꍇ */
		src->cy256 -= ((co s1024((src->tmp_angleCCW1024))*(t256(0.03)))>>8);/*fps_factor*/
		#else
		{
			int sin_value_t256; 	//	sin_value_t256 = 0;
			int cos_value_t256; 	//	cos_value_t256 = 0;
			int256_sincos1024( (src->tmp_angleCCW1024), &sin_value_t256, &cos_value_t256);
			src->cx256 -= ((sin_value_t256*(t256(0.03)))>>8);/*fps_factor*/
			src->cy256 -= ((cos_value_t256*(t256(0.03)))>>8);/*fps_factor*/
		}
		#endif
	}
	//	[SAKUYA_ANIME_12_HATUDAN_START)
}


/*---------------------------------------------------------
	���`��: �����u�t�F�X�e�B�o���i�C�t�v �ŏI�`��(����1)
---------------------------------------------------------*/
/*
0
	[A] �������Ȃ�(�ҋ@����)
80
	[B] �ҋ@���Ԃ��I�������ړ�����
81
	[C] �^�񒆂ɗ���܂ňړ� //case SG01:
1024
	[D] �������Ȃ�(�ҋ@����)
2048 ==(1024+1024 )
	[E] ����i�C�t�ōU��
2049 ==(1024+1024+1)
	[F]�ړ�
2089 ==(1024+10024+1+40)
	[G] �ҋ@���ԍ쐬���A�߂�B
*/
global void sakuya_09_keitai(SPRITE *src)
{
	src->BOSS_DATA_05_boss_base_state777++;
	if (80 > src->BOSS_DATA_05_boss_base_state777) /* [A] �������Ȃ�(�ҋ@����) */
	{
		;/* �������Ȃ�(�ҋ@����) */
	}
	else
	if (81 > src->BOSS_DATA_05_boss_base_state777) /* [B] �ҋ@���Ԃ��I�������ړ����� */
	{
		//	src->BOSS_DATA_05_boss_base_state777++;/* = SG01*/
//++		pd->bo ssmode = B00_NONE/*B01_BA TTLE*/;
		//(r35�ŃJ�[�h�̏ꍇ�̂ݏȗ��\)	REG_02_DEST_X	= ((src->cx256));
		//(r35�ŃJ�[�h�̏ꍇ�̂ݏȗ��\)	REG_03_DEST_Y	= ((src->cy256));
		//(r35�ŃJ�[�h�̏ꍇ�̂ݏȗ��\)	set_REG_DEST_XY(src);	/* �e��x256 y256 ���S���甭�e�B */
			REG_00_SRC_X	= (t256(GAME_X_OFFSET+(GAME_320_WIDTH/2))); 		/* �e��x256 */
			REG_01_SRC_Y	= (t256(30));					/* �e��y256 */
			tmp_angleCCW65536_src_nerai();
			src->BOSS_DATA_06_sakuya_data_move_angle1024 = (deg65536to1024(HATSUDAN_03_angle65536));	/* �u1����65536�����v����u1����1024�����v�֕ϊ�����B */
	}
	else
	if (1024 > src->BOSS_DATA_05_boss_base_state777) /* [C] �^�񒆂ɗ���܂ňړ� */
	{
	//case SG01:
		if ( (src->cx256 < t256(GAME_X_OFFSET+(GAME_320_WIDTH/2)+20) ) &&
			 (src->cx256 > t256(GAME_X_OFFSET+(GAME_320_WIDTH/2)-20) ) &&
			 (src->cy256 < t256(50.0)) ) /* �^�񒆂ɗ����� */
		{
			src->BOSS_DATA_05_boss_base_state777 = (1024+1024-1)-(20);// src->BOSS_DATA_05_boss_base_state777++;/* = SG02*/
			src->BOSS_DATA_07_sakuya_data_common_wait256	= t256(0.0);
			//	[SAKUYA_ANIME_03_CENTER_A;
		}
		else
		{
			#if (0)//
			src->cx256 += ((si n1024((src->BOSS_DATA_06_sakuya_data_move_angle1024))*(t256(3.0)))>>8);/*fps_factor*/	/* CCW�̏ꍇ */
			src->cx256 += ((si n1024((src->BOSS_DATA_06_sakuya_data_move_angle1024))*(t256(3.0)))>>8);/*fps_factor*/	/* CCW�̏ꍇ */
			src->cy256 += ((co s1024((src->BOSS_DATA_06_sakuya_data_move_angle1024))*(t256(3.0)))>>8);/*fps_factor*/
			src->cy256 += ((co s1024((src->BOSS_DATA_06_sakuya_data_move_angle1024))*(t256(3.0)))>>8);/*fps_factor*/
			#else
			{
				int sin_value_t256; 		//	sin_value_t256 = 0;
				int cos_value_t256; 		//	cos_value_t256 = 0;
				int256_sincos1024( (src->BOSS_DATA_06_sakuya_data_move_angle1024), &sin_value_t256, &cos_value_t256);
				src->cx256 += ((sin_value_t256*(t256(3.0)))>>8);/*fps_factor*/
				src->cy256 += ((cos_value_t256*(t256(3.0)))>>8);/*fps_factor*/
			}
			#endif
		}
	//	break;
	}
	else
	if ((1024+1024 ) > src->BOSS_DATA_05_boss_base_state777) /* [D] �������Ȃ�(�ҋ@����) */
	{
		;/* �������Ȃ�(�ҋ@����) */
	}
	else
	if ((1024+1024 +1) > src->BOSS_DATA_05_boss_base_state777) /* [E] ����i�C�t�ōU�� */
	{
//	case SG03:
		//	[SAKUYA_ANIME_15_HATUDAN_END)
		{
			bullet_create_sakuya_ryoute_knife(src);
			if (src->cy256 > t256(150))
		//	{	src->BOSS_DATA_06_sakuya_data_move_angle1024 = cv1024r(   210) + (((ra_nd()&(4096-1))*21)>>8) /*(ra_nd()%(deg_360_to_512(120)))*/;} 	/* CW�̏ꍇ */
		//	{	src->BOSS_DATA_06_sakuya_data_move_angle1024 = cv1024r(360-210) + (((ra_nd()&(4096-1))*21)>>8) /*(ra_nd()%(deg_360_to_512(120)))*/;}	/* CCW�̏ꍇ */
			{	src->BOSS_DATA_06_sakuya_data_move_angle1024 = cv1024r(360-210) + (((ra_nd()&(4096-1))*21)>>8) /*(ra_nd()%(deg_360_to_512(120)))*/;}
			else
			{	src->BOSS_DATA_06_sakuya_data_move_angle1024 = ((ra_nd()&(1024-1)));}
			#if 1
			/*�O�̈�*/
			mask1024(src->BOSS_DATA_06_sakuya_data_move_angle1024);
			#endif
			//	[(SAKUYA_ANIME_12_HATUDAN_START+1);/*16???*/
		//	src->BOSS_DATA_05_boss_base_state777++;/* = SG04*/
		}
	//	break;
	}
	else
	if ((1024+1024 +1+40) > src->BOSS_DATA_05_boss_base_state777) /* [F]�ړ� */
	{
		#if (0)//
		src->cx256 += ((si n1024((src->BOSS_DATA_06_sakuya_data_move_angle1024))*(t256(3.5)))>>8);/*fps_factor*/	/* CCW�̏ꍇ */
		src->cy256 += ((co s1024((src->BOSS_DATA_06_sakuya_data_move_angle1024))*(t256(3.5)))>>8);/*fps_factor*/
		#else
		{
			int sin_value_t256; 	//	sin_value_t256 = 0;
			int cos_value_t256; 	//	cos_value_t256 = 0;
			int256_sincos1024( (src->BOSS_DATA_06_sakuya_data_move_angle1024), &sin_value_t256, &cos_value_t256);
			src->cx256 += ((sin_value_t256*(t256(3.5)))>>8);/*fps_factor*/
			src->cy256 += ((cos_value_t256*(t256(3.5)))>>8);/*fps_factor*/
		}
		#endif
	}
	else /* [G] �ҋ@���ԍ쐬���A[D]�ɖ߂�B */
	{
	//case SG04:
		//	src->BOSS_DATA_05_boss_base_state777 = SG02;
		//	src->BOSS_DATA_05_boss_base_state777 = (1024+1024-1)-(120)-((3-(cg_game_di fficulty))*50);
		// difficulty(0) = (1927)-((3)*50) == 1777 == (271)c
		// difficulty(1) = (1927)-((2)*50) == 1827 == (221)c
		// difficulty(2) = (1927)-((1)*50) == 1877 == (171)c
		// difficulty(3) = (1927)-((0)*50) == 1927 == (121)c
		//	src->BOSS_DATA_05_boss_base_state777 = (1024+1024-1)-(120)-((3-(cg_game_di fficulty))*64);
		//	src->BOSS_DATA_05_boss_base_state777 = (1024+1024-1)-(120)-(3*64)+(cg_game_di fficulty*64);
		//	src->BOSS_DATA_05_boss_base_state777 = (1024+1024-1)-(120)-(3*64)+(cg_game_di fficulty<<6);
			src->BOSS_DATA_05_boss_base_state777 = (1024+1024-1)-(120)-(3*64)+(3<<6);
	//	break;
	}
				SPRITE *zzz_player;
				zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
	#if 1/*Gu(���S���W)*/
	if ((zzz_player->cx256 < src->cx256 + t256(25)) &&	/* ???????????? */
		(zzz_player->cx256 > src->cx256 - t256(25)))
	#endif
	{
	//	src->BOSS_DATA_07_sakuya_data_common_wait256 += (1+2+cg_game_di fficulty);/*fps_factor*/}
		src->BOSS_DATA_07_sakuya_data_common_wait256 += (1+2+3);/*fps_factor*/}
		// difficulty(0) = 20+((3)*(30)) == 110(r33)  42(r34)==(128/3)
		// difficulty(1) = 20+((2)*(30)) ==  80(r33)  32(r34)==(128/4)
		// difficulty(2) = 20+((1)*(30)) ==  50(r33)  25(r34)==(128/5)
		// difficulty(3) = 20+((0)*(30)) ==  20(r33)  21(r34)==(128/6)
//	if (src->BOSS_DATA_07_sakuya_data_common_wait256 > 20+((3-(cg_game_di fficulty))*(30)))
	if (src->BOSS_DATA_07_sakuya_data_common_wait256 > 128)
	{
		src->BOSS_DATA_07_sakuya_data_common_wait256		= 0;
	//	src->BOSS_DATA_07_sakuya_data_common_wait256		= 0;/*speed256*/(2) + ((cg_game_di fficulty));
		//	[(SAKUYA_ANIME_12_HATUDAN_START+1);/*16???*/
		{
	//		src->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x00);			/*	src->alpha			= 0x00;*/
			src->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x80);			/*	src->alpha			= 0x00;*/	/* ������ */
	//		//	[SAKUYA_ANIME_15_HATUDAN_END;//?????
			src->tmp_angleCCW1024	= 0;//(ii<<7);
		}
		bullet_crate_sakuya_kurukuru(src);/*,*/ /*1+difficulty,*/
	}
}

/*---------------------------------------------------------
	�攪�`��: (���~�߃i�C�t)�ǉ��v�撆
	�掵�`��: (���U�����w)�ǉ��v�撆
//
	�掵�`��: ���H�u�W���b�N�E�U�E���h�r���v�ɂȂ邩������Ȃ��e�X�g
	<<<�Ă���>>>
	�����͕W���񓯊��ړ����g���B(r32)�B
---------------------------------------------------------*/

/*---------------------------------------------------------
	��Z�`��: (���F�����e)
	��܌`��: (���F�����e)
---------------------------------------------------------*/
	//	/* �s���N�n�[�g */	add_zako_sakuya_doll_04_360knife(src);/* �q�������w�ǉ� */
	//	/* �s���N�n�[�g */	add_zako_sakuya_doll_04_360knife(src);/* �q�������w�ǉ� */

/*---------------------------------------------------------
	��08�`��: ���� �����w����(�������邾��)
---------------------------------------------------------*/

extern void core_boss_move_05_xy_douki_differential64(SPRITE *src);
extern void boss_move_04_xy_douki_differential32(SPRITE *src);
global void sakuya_08_keitai(SPRITE *src)
{
	{
		static int seisei_count;
		if (0==seisei_count)
		{
			add_zako_sakuya_baramaki2(src);/* �q�������w�ǉ� */
		}
		seisei_count++;
		seisei_count &= 0x01ff; 	/* 512 == 0x200 == ( 8 x 64). [ 8�b���炢] */
	//
	}
//
	boss_move_04_xy_douki_differential32(src);
}


/*---------------------------------------------------------
	��04�`��: ���� �����w����(�������邾��)
---------------------------------------------------------*/

global void sakuya_04_keitai(SPRITE *src)
{
	{
		static int seisei_count;
		if (0==seisei_count)
		{
			add_zako_sakuya_baramaki1(src);/* �q�������w�ǉ� */
		}
		seisei_count++;
		seisei_count &= 0x01ff; 	/* 512 == 0x200 == ( 8 x 64). [ 8�b���炢] */
	}
//
	boss_move_04_xy_douki_differential32(src);
}


/*---------------------------------------------------------
	���J�u�o�[�e�B�J���i�C�t�v
	��03�`��: �����i�C�t�������Ă����
	�ʏ�U��3: �ԃi�C�t
---------------------------------------------------------*/
/*
	boss_move.c ��
*/


/*---------------------------------------------------------
	��02�`��: ��p�u�~�X�f�B���N�V�����v(�S���ʁA���܂�)
---------------------------------------------------------*/

/*
		64		256-64		256-0
	//	�^���փ��[�v
		128 				256-64
	// �Ȃɂ����Ȃ�
		192 				256-128
	// ���]�ړ������Z�b�g
		256 				256-192
	// �Ȃɂ����Ȃ�
		0		256 		256-256
	//	���Α��փ��[�v
-------------------------

256 	�^���փ��[�v

250 	���Α��փ��[�v
*/
global void sakuya_02_keitai(SPRITE *src)
{
	/* �J�[�h�������ĂȂ��ꍇ�ɒǉ� */
	if (SPELL_00==card.card_number) 	/* �J�[�h�����I���Ȃ�J�[�h���� */
	{
		card_maikai_init_and_get_spell_number(src); 	/* ���݂̌`�ԂŌ��Ă�J�[�h�ԍ����J�[�h�V�X�e���ɐݒ�B */
	//
	}
	if (/*(256-64)*/(255/*192+64*/) == REG_10_BOSS_SPELL_TIMER) /* ���]�ړ������Z�b�g */
	{
		{
			/* ���`�Ԃł͎g���Ė������痘�p(���p)���� */
			#define sakuya_data_sayuu_mode BOSS_DATA_07_sakuya_data_common_wait256
			{	/* ��邳��̗U���|�C���g���W */
				#define SAKUYA_POINT_X_MID			(t256(GAME_X_OFFSET+(GAME_320_WIDTH/2)))
				src->sakuya_data_sayuu_mode ^= (2-1);/* ���] */
				if (/*0==*/src->sakuya_data_sayuu_mode & (2-1))
						{	src->BOSS_DATA_00_target_x256 = SAKUYA_POINT_X_MID-(t256(128)); }//96==128*0.75
				else	{	src->BOSS_DATA_00_target_x256 = SAKUYA_POINT_X_MID+(t256(128)); }//
			}
			src->BOSS_DATA_04_toutatu_wariai256 = t256(1.00);
		}
	}
	//
	else
//	if (/*(256-256)*/(59)/*((251+64)-256)*/ == REG_10_BOSS_SPELL_TIMER) /* �^���փ��[�v */
	if (/*(256-256)*/(59)/*((251+64)-256)*/ == REG_10_BOSS_SPELL_TIMER) /* �^���փ��[�v */
	{
		src->BOSS_DATA_00_target_x256 = src->cx256 = t256(GAME_X_OFFSET+(GAME_320_WIDTH/2)); /* �^���փ��[�v */
		src->BOSS_DATA_04_toutatu_wariai256 = t256(1.00);
	}
	else
	if (/*(256-256)*/(58)/*((250+64)-256)*/ == REG_10_BOSS_SPELL_TIMER) /* �^���փ��[�v */
//	if (/*(256-256)*/(0) == REG_10_BOSS_SPELL_TIMER) /* �^���փ��[�v */
//	if (/*(256-192)*/(64) == REG_10_BOSS_SPELL_TIMER) /* �^���փ��[�v */
//	if (/*(256-180)*/(76) == REG_10_BOSS_SPELL_TIMER) /* �^���փ��[�v */
	{
	//	src->BOSS_DATA_00_target_x256 = src->cx256 = t256(GAME_X_OFFSET) + ( t256(GAME_320_WIDTH) - ( (src->cx256) - t256(GAME_X_OFFSET) ) );	/* ���Α��փ��[�v */
		src->BOSS_DATA_00_target_x256 = src->cx256 = t256(GAME_X_OFFSET) + t256(GAME_320_WIDTH) - (src->cx256) + t256(GAME_X_OFFSET);			/* ���Α��փ��[�v */
		src->BOSS_DATA_04_toutatu_wariai256 = t256(1.00);
	}

	#if 1
	boss_yuudou_idou_nomi(src);
	boss_yuudou_hiritu_keisan(src);
	#endif
}




/*---------------------------------------------------------
	���`��: �ʏ�U��(���E�ɓ������S�e����)
	�ʏ�U��(�S�J�[�h�A����Ƃ͂����Ď኱�Ⴄ)
---------------------------------------------------------*/
/*
	boss_move.c ��
*/

