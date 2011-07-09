
#include "boss.h"

/*static*/extern void add_zako_sakuya_doll_01_laser(SPRITE *src);/* ���[�U�[ */
/*static*/extern void add_zako_sakuya_doll_02_tama_oki(SPRITE *src);/* ���u�� */
/*static*/extern void add_zako_sakuya_doll_03_star(SPRITE *src);/* �q�������w(���^��`��) */
/*static*/extern void add_zako_sakuya_doll_04_360knife(SPRITE *src);/* �q�������w(360�i�C�t������) */
/*static*/extern void add_zako_sakuya_baramaki1(SPRITE *src);/* ��� �΂�T��1�e �I�v�V���� */
/*static*/extern void add_zako_sakuya_baramaki2(SPRITE *src);/* ��� �΂�T��2�e �I�v�V���� */

/*static*/extern void bullet_create_sakuya_ryoute_knife(SPRITE *src);	/* ��� ����i�C�t�e�� */
/*static*/extern void bullet_crate_sakuya_linear_yuudou(SPRITE *src);	/* ��� �����U���i�C�t�e�� */
/*static*/extern void bullet_crate_sakuya_kurukuru(SPRITE *src);		/* ��� ���邭��i�C�t�e�� */

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�\�Z�� ���
	Izayoi Sakuya.
	-------------------------------------------------------
	ToDo:
	�ړ��ƒe���𕪗����悤�B�łȂ��Ɩ�킩���B
	�{�X�^�C�}�[�Ή���
	-------------------------------------------------------
	���{�X�^�C�}�[�̓V�X�e���ŃT�|�[�g�\��ł��B(��ƒ�)
	���e���̓V�X�e���ŃT�|�[�g����\�z������܂��B(�\�z��)
	�V�X�e���ŃT�|�[�g�����ƁA�ǂ̃{�X���A�ǂ̒e���ł����Ă�悤�ɂȂ�܂��B
---------------------------------------------------------*/
#if 0/* �߂��F���́A���ʕ����Ŏg���Ă�B */
/* �{�X���ʋK�i */
	#define target_x256 			user_data00 	/* �ڕWx���W */
	#define target_y256 			user_data01 	/* �ڕWy���W */
	#define toutatu_wariai256		user_data02 	/* �ڕW���W�ւ̓��B���� */
	#define kougeki_anime_count 	user_data03 	/* �U���A�j���[�V�����p�J�E���^ */
	#define boss_time_out			user_data04 	/* �������� */
	#define boss_base_state777		user_data04 	/* ��������(boss_time_out�Ɠ���) */
//
	#define boss_spell_timer		user_data05 	/* �X�y������ */
#endif

#define sakuya_data_move_angle1024		user_data06 	/* ���̊p�x */
#define sakuya_data_common_wait256		user_data07 	/* ���� */


/*---------------------------------------------------------
	���w��̖����w�ɂ��ă���
	-------------------------------------------------------
	5�ʒ�-�{�X�ŏo�Ă��鎞�́A�傫��60x60(���Ԃ�64x64obj)���炢�B
	5�ʃ{�X�ŏo�Ă��鎞�́A�傫��125x125(���Ԃ�128x128obj)���炢�B
	�͕핗�͊�{�I�ɏk����75%�Ȃ̂ŁA���w��̖����w��
	48x48[dot](2�{�g�厞��96x96[dot])���炢���Ó����ȁH
---------------------------------------------------------*/

/*---------------------------------------------------------
	�����Ȃ��ŃX�y�J���B
---------------------------------------------------------*/

/*---------------------------------------------------------
	SPELL_CARD_11_sakuya_kkk	��11�`��: �ŏI�`��(����3)
---------------------------------------------------------*/

global void sakuya_11_keitai(SPRITE *src)
{
	if (SPELL_00==spell_card.spell_type)		/* �e�������I���Ȃ�e������ */
	{
		spell_card_get_spell_number(src);	/* �X�y�J�V�X�e������A���݂̌`�ԂŌ��e���ԍ�����ɓ���A�W����boss_base_spell_time_out���Ԃ�ݒ�B */
		src->toutatu_wariai256 = t256(1.0);
	}
//
	int iii;
	iii = ra_nd();
	if (0==(iii&0x07))
	{
		src->vx256 = (((signed int)((iii>> 8)&0xff))-127);
		src->vy256 = (((signed int)((iii>>16)&0xff))-127);
	}
	bullet_angle_all_gamen_gai_nara_kesu();/*�p�x�e�̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�)*/
}


/*---------------------------------------------------------
	SPELL_CARD_10_sakuya_jjj	��10�`��: �ŏI�`��(����2)
---------------------------------------------------------*/

global void sakuya_10_keitai(SPRITE *src)
{
	if (SPELL_00==spell_card.spell_type)		/* �e�������I���Ȃ�e������ */
	{
		spell_card_get_spell_number(src);	/* �X�y�J�V�X�e������A���݂̌`�ԂŌ��e���ԍ�����ɓ���A�W����boss_base_spell_time_out���Ԃ�ݒ�B */
		src->toutatu_wariai256 = t256(1.0);
	}
//
	{
		static int seisei_count;
		if (0==seisei_count)
		{
			add_zako_sakuya_doll_03_star(src);/* �q�������w�ǉ� */
		//	add_zako_sakuya_doll_04_360knife(src);/* �q�������w�ǉ� */
		}
		seisei_count++;
		seisei_count &= 0x00ff; 	/* 256 == 0x100 == ( 4 x 64). [ 4�b���炢] */
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
		src->cx256 -= ((sin1024((src->tmp_angleCCW1024))*(t256(0.03)))>>8);/*fps_factor*/		/* CCW�̏ꍇ */
		src->cy256 -= ((cos1024((src->tmp_angleCCW1024))*(t256(0.03)))>>8);/*fps_factor*/
	}
	//	[SAKUYA_ANIME_12_HATUDAN_START)
	bullet_angle_all_gamen_gai_nara_kesu();/*�p�x�e�̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�)*/
}


/*---------------------------------------------------------
	SPELL_CARD_09_sakuya_iii	���`��: �����u�t�F�X�e�B�o���i�C�t�v �ŏI�`��(����1)
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
	src->boss_base_state777++;
	if (80 > src->boss_base_state777) /* [A] �������Ȃ�(�ҋ@����) */
	{
		;/* �������Ȃ�(�ҋ@����) */
	}
	else
	if (81 > src->boss_base_state777) /* [B] �ҋ@���Ԃ��I�������ړ����� */
	{
		//	src->boss_base_state777++;/* = SG01*/
//++		pd->bo ssmode = B00_NONE/*B01_BA TTLE*/;
		//	send1_xy(src);	/* �e��x256 y256 ���S���甭�e�B */
			obj_send1->cx256	= (t256(GAME_WIDTH/2)); 		/* �e��x256 */
			obj_send1->cy256	= (t256(30));					/* �e��y256 */
			tmp_angleCCW65536_src_nerai(obj_send1, src);
			src->sakuya_data_move_angle1024 = ((src->tmp_angleCCW65536)>>6);		/* �u1����65536�����v����u1����1024�����v�֕ϊ�����B */
	}
	else
	if (1024 > src->boss_base_state777) /* [C] �^�񒆂ɗ���܂ňړ� */
	{
	//case SG01:
		if ( (src->cx256 < t256((GAME_WIDTH/2)+20) ) &&
			 (src->cx256 > t256((GAME_WIDTH/2)-20) ) &&
			 (src->cy256 < t256(50.0)) ) /* �^�񒆂ɗ����� */
		{
			src->boss_base_state777 = (1024+1024-1)-(20);// src->boss_base_state777++;/* = SG02*/
			src->sakuya_data_common_wait256 	= t256(0.0);
			//	[SAKUYA_ANIME_03_CENTER_A;
		}
		else
		{
			src->cx256 += ((sin1024((src->sakuya_data_move_angle1024))*t256(3.0))>>8);/*fps_factor*/	/* CCW�̏ꍇ */
			src->cy256 += ((cos1024((src->sakuya_data_move_angle1024))*t256(3.0))>>8);/*fps_factor*/
		}
	//	break;
	}
	else
	if ((1024+1024 ) > src->boss_base_state777) /* [D] �������Ȃ�(�ҋ@����) */
	{
		;/* �������Ȃ�(�ҋ@����) */
	}
	else
	if ((1024+1024 +1) > src->boss_base_state777) /* [E] ����i�C�t�ōU�� */
	{
//	case SG03:
		//	[SAKUYA_ANIME_15_HATUDAN_END)
		{
			bullet_create_sakuya_ryoute_knife(src);
			if (src->cy256 > t256(150))
		//	{	src->sakuya_data_move_angle1024 = cv1024r(	  210) + (((ra_nd()&(4096-1))*21)>>8) /*(ra_nd()%(deg_360_to_512(120)))*/;} 	/* CW�̏ꍇ */
		//	{	src->sakuya_data_move_angle1024 = cv1024r(360-210) + (((ra_nd()&(4096-1))*21)>>8) /*(ra_nd()%(deg_360_to_512(120)))*/;} 	/* CCW�̏ꍇ */
			{	src->sakuya_data_move_angle1024 = cv1024r(360-210) + (((ra_nd()&(4096-1))*21)>>8) /*(ra_nd()%(deg_360_to_512(120)))*/;}
			else
			{	src->sakuya_data_move_angle1024 = ((ra_nd()&(1024-1)));}
			#if 1
			/*�O�̈�*/
			mask1024(src->sakuya_data_move_angle1024);
			#endif
			//	[(SAKUYA_ANIME_12_HATUDAN_START+1);/*16???*/
		//	src->boss_base_state777++;/* = SG04*/
		}
	//	break;
	}
	else
	if ((1024+1024 +1+40) > src->boss_base_state777) /* [F]�ړ� */
	{
		/* CCW�̏ꍇ */
		src->cx256 += ((sin1024((src->sakuya_data_move_angle1024))*t256(3.5))>>8);/*fps_factor*/
		src->cy256 += ((cos1024((src->sakuya_data_move_angle1024))*t256(3.5))>>8);/*fps_factor*/
	}
	else /* [G] �ҋ@���ԍ쐬���A[D]�ɖ߂�B */
	{
	//case SG04:
		//	src->boss_base_state777 = SG02;
		//	src->boss_base_state777 = (1024+1024-1)-(120)-((3-(cg_game_difficulty))*50);
		// difficulty(0) = (1927)-((3)*50) == 1777 == (271)c
		// difficulty(1) = (1927)-((2)*50) == 1827 == (221)c
		// difficulty(2) = (1927)-((1)*50) == 1877 == (171)c
		// difficulty(3) = (1927)-((0)*50) == 1927 == (121)c
		//	src->boss_base_state777 = (1024+1024-1)-(120)-((3-(cg_game_difficulty))*64);
		//	src->boss_base_state777 = (1024+1024-1)-(120)-(3*64)+(cg_game_difficulty*64);
			src->boss_base_state777 = (1024+1024-1)-(120)-(3*64)+(cg_game_difficulty<<6);
	//	break;
	}
				SPRITE *zzz_player;
				zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
	#if 1/*Gu(���S���W)*/
	if ((zzz_player->cx256 < src->cx256 + t256(25)) &&	/* ???????????? */
		(zzz_player->cx256 > src->cx256 - t256(25)))
	#endif
	{	src->sakuya_data_common_wait256 += (1+2+cg_game_difficulty);/*fps_factor*/}
		// difficulty(0) = 20+((3)*(30)) == 110(r33)  42(r34)==(128/3)
		// difficulty(1) = 20+((2)*(30)) ==  80(r33)  32(r34)==(128/4)
		// difficulty(2) = 20+((1)*(30)) ==  50(r33)  25(r34)==(128/5)
		// difficulty(3) = 20+((0)*(30)) ==  20(r33)  21(r34)==(128/6)
//	if (src->sakuya_data_common_wait256 > 20+((3-(cg_game_difficulty))*(30)))
	if (src->sakuya_data_common_wait256 > 128)
	{
		src->sakuya_data_common_wait256 		= 0;
	//	src->sakuya_data_common_wait256 		= 0;/*speed256*/(2) + ((cg_game_difficulty));
		//	[(SAKUYA_ANIME_12_HATUDAN_START+1);/*16???*/
		{
	//		src->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x00);			/*	src->alpha			= 0x00;*/
			src->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x80);			/*	src->alpha			= 0x00;*/	/* ������ */
	//		//	[SAKUYA_ANIME_15_HATUDAN_END;//?????
			src->tmp_angleCCW1024	= 0;//(ii<<7);
		}
		bullet_crate_sakuya_kurukuru(src);/*,*/ /*1+difficulty,*/
	}
	bullet_angle_all_gamen_gai_nara_kesu();/*�p�x�e�̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�)*/
}

/*---------------------------------------------------------
	SPELL_CARD_08_sakuya_hhh	�攪�`��: (���~�߃i�C�t)�ǉ��v�撆
	SPELL_CARD_07_sakuya_ggg	�掵�`��: (���U�����w)�ǉ��v�撆
//
	�掵�`��: ���H�u�W���b�N�E�U�E���h�r���v�ɂȂ邩������Ȃ��e�X�g
	<<<�Ă���>>>
	�����͕W���񓯊��ړ����g���B(r32)�B
---------------------------------------------------------*/

/*---------------------------------------------------------
	SPELL_CARD_06_sakuya_fff	��Z�`��: (���F�����e)
	SPELL_CARD_05_sakuya_eee	��܌`��: (���F�����e)
---------------------------------------------------------*/

/*---------------------------------------------------------
	��08�`��: ���� �����w����(�������邾��)
---------------------------------------------------------*/

extern void core_boss_move_05_xy_douki_differential64(SPRITE *src);
extern void core_boss_move_04_xy_douki_differential32(SPRITE *src);
global void sakuya_08_keitai(SPRITE *src)
{
	{
		static int seisei_count;
		if (0==seisei_count)
		{
		//	add_zako_sakuya_doll_03_star(src);/* �q�������w�ǉ� */
	//	/* �s���N�n�[�g */	add_zako_sakuya_doll_04_360knife(src);/* �q�������w�ǉ� */
			add_zako_sakuya_baramaki2(src);/* �q�������w�ǉ� */
		}
		seisei_count++;
		seisei_count &= 0x01ff; 	/* 512 == 0x200 == ( 8 x 64). [ 8�b���炢] */
	//
	}
//
	core_boss_move_04_xy_douki_differential32(src);
	bullet_angle_all_gamen_gai_nara_kesu();/*�p�x�e�̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�)*/
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
		//	add_zako_sakuya_doll_03_star(src);/* �q�������w�ǉ� */
	//	/* �s���N�n�[�g */	add_zako_sakuya_doll_04_360knife(src);/* �q�������w�ǉ� */
			add_zako_sakuya_baramaki1(src);/* �q�������w�ǉ� */
		}
		seisei_count++;
		seisei_count &= 0x01ff; 	/* 512 == 0x200 == ( 8 x 64). [ 8�b���炢] */
	}
//
	core_boss_move_04_xy_douki_differential32(src);
	bullet_angle_all_gamen_gai_nara_kesu();/*�p�x�e�̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�)*/
}


/*---------------------------------------------------------
	SPELL_CARD_03_sakuya_ccc
	���J�u�o�[�e�B�J���i�C�t�v
	��03�`��: �����i�C�t�������Ă����
	�ʏ�U��3: �ԃi�C�t
---------------------------------------------------------*/

global void sakuya_03_keitai(SPRITE *src)
{
	core_boss_move_04_xy_douki_differential32(src);
//	bullet_angle_all_gamen_gai_nara_kesu();/*�p�x�e�̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�)*/
	/* ���˒e�̂Ă��� */
//	bullet_angle_all_gamen_gai_nara_hansya();
	bullet_angle_all_gamen_gai_nara_hansya_gensoku();
}

/*---------------------------------------------------------
	SPELL_CARD_02_sakuya_bbb
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
	/* �e���������ĂȂ��ꍇ�ɒǉ� */
	if (SPELL_00==spell_card.spell_type)		/* �e�������I���Ȃ�e������ */
	{
		spell_card_get_spell_number(src);	/* �X�y�J�V�X�e������A���݂̌`�ԂŌ��e���ԍ�����ɓ���A�W����boss_base_spell_time_out���Ԃ�ݒ�B */
	//
	}
	if (/*(256-64)*/(255/*192+64*/) == src->boss_spell_timer) /* ���]�ړ������Z�b�g */
	{
		{
			/* ���`�Ԃł͎g���Ė������痘�p(���p)���� */
			#define sakuya_data_sayuu_mode sakuya_data_common_wait256
			{	/* ��邳��̗U���|�C���g���W */
				#define SAKUYA_POINT_X_MID			(t256(GAME_WIDTH*2/4))
				src->sakuya_data_sayuu_mode ^= (2-1);/* ���] */
				if (/*0==*/src->sakuya_data_sayuu_mode & (2-1))
						{	src->target_x256 = SAKUYA_POINT_X_MID-(t256(128));	}//96==128*0.75
				else	{	src->target_x256 = SAKUYA_POINT_X_MID+(t256(128));	}//
			}
			src->toutatu_wariai256 = t256(1.00);
		}
	}
	//
	else
//	if (/*(256-256)*/(59)/*((251+64)-256)*/ == src->boss_spell_timer) /* �^���փ��[�v */
	if (/*(256-256)*/(59)/*((251+64)-256)*/ == src->boss_spell_timer) /* �^���փ��[�v */
	{
		src->target_x256 = src->cx256 = t256(GAME_WIDTH/2); /* �^���փ��[�v */
		src->toutatu_wariai256 = t256(1.00);
	}
	else
	if (/*(256-256)*/(58)/*((250+64)-256)*/ == src->boss_spell_timer) /* �^���փ��[�v */
//	if (/*(256-256)*/(0) == src->boss_spell_timer) /* �^���փ��[�v */
//	if (/*(256-192)*/(64) == src->boss_spell_timer) /* �^���փ��[�v */
//	if (/*(256-180)*/(76) == src->boss_spell_timer) /* �^���փ��[�v */
	{
		src->target_x256 = src->cx256 = t256(GAME_WIDTH)-src->cx256;			/* ���Α��փ��[�v */
		src->toutatu_wariai256 = t256(1.00);
	}

	#if 1
	boss_yuudou_idou_nomi(src);
	boss_yuudou_hiritu_keisan(src);
	#endif
//
	/*�p�x�e�̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�)*/
	if (RANK_NORMAL < (cg_game_difficulty))/* hard or lunatic */
	{
		#if 0
		/* ���˒e�̂Ă��� */
		/*�������*/bullet_angle_all_gamen_gai_nara_hansya();
		#else/* ����Ă݂����ǁA������Ɠ������̂ŋ}篊ȒP�ɂ��� */
		/*�����������B���ƈꉟ���ȒP�ɂ�������(r32���_)*/bullet_angle_all_gamen_gai_nara_hansya_gensoku();
		#endif
	}
	else
	{
		bullet_angle_all_gamen_gai_nara_kesu();
	}
}


/*---------------------------------------------------------
	�X���u�A�C�V�N���t�H�[���v�����Ȃ��ŃX�y�J���B
---------------------------------------------------------*/

global void cirno_02_keitai(SPRITE *src)
{
	if (SPELL_00==spell_card.spell_type)		/* �e�������I���Ȃ�e������ */
	{
		spell_card_get_spell_number(src);	/* �X�y�J�V�X�e������A���݂̌`�ԂŌ��e���ԍ�����ɓ���A�W����boss_base_spell_time_out���Ԃ�ݒ�B */
		src->toutatu_wariai256 = t256(1.0);
	}
//
//	bullet_angle_all_gamen_gai_nara_kesu();/*�p�x�e�̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�)*/
		#if 0
		/* ���˒e�̂Ă��� */
		/*�������*/bullet_angle_all_gamen_gai_nara_hansya();
		#else/* ����Ă݂����ǁA������Ɠ������̂ŋ}篊ȒP�ɂ��� */
		/*�����������B���ƈꉟ���ȒP�ɂ�������(r32���_)*/bullet_angle_all_gamen_gai_nara_hansya_gensoku();
		#endif
}

/*---------------------------------------------------------
	SPELL_CARD_01_sakuya_aaa
	���`��: �ʏ�U��(���E�ɓ������S�e����)
	�ʏ�U��(�S�e���A����Ƃ͂����Ď኱�Ⴄ)
---------------------------------------------------------*/
/*
	boss_move.c ��
*/


/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/
#if 1
global void boss_init_sakuya(SPRITE *h)
{
	h->callback_loser				= common_boss_put_items;

//	h->sakuya_data_common_wait256					= t256(0.0);/*���� sakuya_02_keitai()�Ńr�b�g���g���Asakuya_03_keitai()�ŏ��������邩�炽�Ԃ�s�v*/
//	h->sakuya_data_move_angle1024				= (0);/*����sakuya_09_keitai()�ŏ��������邩��s�v*/
	//-------------------------
	obj_send1->type 				= SP_DUMMY_MUTEKI;
//	send1_xy(src);	/* �e��x256 y256 ���S���甭�e�B */
	obj_send1->cx256				= (0);
	obj_send1->cy256				= (0);
}
#endif

