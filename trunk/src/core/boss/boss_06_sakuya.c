
#include "boss.h"

/*static*/extern void add_zako_sakuya_doll_01_laser(SPRITE *src);/* ���[�U�[ */
/*static*/extern void add_zako_sakuya_doll_02_star(SPRITE *src);/* �q�������w(���^��`��) */
/*static*/extern void add_zako_sakuya_doll_03_360knife(SPRITE *src);/* �q�������w(360�i�C�t������) */
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
	#define target_x256 		user_data00 	/* �ڕWx���W */
	#define target_y256 		user_data01 	/* �ڕWy���W */
	#define vvv256				user_data02 	/* �ڕW���W�ւ̓��B���� */
	#define boss_time_out		user_data03 	/* �������� */
	#define boss_base_state777	user_data03 	/* ��������(boss_time_out�Ɠ���) */
#endif

//	int st ate1;							/* �s�� */
//	int move_type;		/*	�ړ��^�C�v */	/* �`�� */
//


#define sakuya_data_move_angle1024		user_data04 	/* ���̊p�x */
#define sakuya_data_wait1				user_data05 	/* ���̊p�x */
#define sakuya_data_wait2_256			user_data06 	/* ���̊p�x */

//	int length_s_p256;	/* ���ƃv���C���[�Ƃ̋��� */

//
/*---------------------------------------------------------
	���w��̖����w�ɂ��ă���
	-------------------------------------------------------
	5�ʒ�-�{�X�ŏo�Ă��鎞�́A�傫��60x60(���Ԃ�64x64obj)���炢�B
	5�ʃ{�X�ŏo�Ă��鎞�́A�傫��125x125(���Ԃ�128x128obj)���炢�B
	�͕핗�͊�{�I�ɏk����75%�Ȃ̂ŁA���w��̖����w��
	48x48[dot](2�{�g�厞��96x96[dot])���炢���Ó����ȁH
---------------------------------------------------------*/


/*--------------------------------------*/
/*--------------------------------------*/
/*--------------------------------------*/

#if 1
/*---------------------------------------------------------
	[�X�y�J�V�X�e�����Ɉړ��\��]	�X�y�J���j��A�C�e���o��
---------------------------------------------------------*/

static void sakuya_put_items(SPRITE *src)
{
	common_boss_put_items(src);
//
//�ʂŐݒ�ς�	src->boss_base_state777 	= 0;/* ���ʂ��� �l�� 0 �ɂ��� */
//???	src->sakuya_data_wait1		= 80;/* �p�~ */
	src->sakuya_data_wait2_256	= t256(-100);

}
#endif



/*---------------------------------------------------------
	�����Ȃ񂾂�������Ȃ��Ȃ�̂ŁA���O�����悤
---------------------------------------------------------*/

#define SAKUYA_LIMIT_MIN_Y (t256(8.0))
#define SAKUYA_LIMIT_MAX_Y (t256(GAME_HEIGHT-32))

/* ��邳��̗U���|�C���g���W */

#define SAKUYA_POINT_X_LEFT_OUT 	(t256(GAME_WIDTH*0/4))
#define SAKUYA_POINT_X_LEFT_MID 	(t256(GAME_WIDTH*1/4))
#define SAKUYA_POINT_X_MID			(t256(GAME_WIDTH*2/4))/* -src->w/2 */
#define SAKUYA_POINT_X_RIGHT_MID	(t256(GAME_WIDTH*3/4))
#define SAKUYA_POINT_X_RIGHT_OUT	(t256(GAME_WIDTH*4/4))/* -src->w */
#define SAKUYA_POINT_X_MARGIN		(t256(16.0))

#define SAKUYA_POINT_Y_CENTER		(t256(50.0))




/*---------------------------------------------------------
	SPELL_CARD_11_sakuya_kkk	��11�`��: �ŏI�`��(����3)
---------------------------------------------------------*/

global void sakuya_11_keitai(SPRITE *src)
{
	{
		static int seisei_count;
		if (0==seisei_count)
		{
			add_zako_sakuya_doll_01_laser(src);/* �q�������w�ǉ� */
			/* hard or lunatic */
			if (1 < (cg_game_difficulty))
			{
				add_zako_sakuya_doll_02_star(src);/* �q�������w�ǉ� */
				/* lunatic only */
				if (2 < (cg_game_difficulty))
				{
					add_zako_sakuya_doll_03_360knife(src);/* �q�������w�ǉ� */
				}
			}
		}
		seisei_count++;
		seisei_count &= ((0x00ff) );	/* 256 == 0x100 == ( 4 x 64). [ 4�b���炢] */
	}
//
	src->color32	= MAKE32RGBA(0xff, 0xff, 0xff, 0xff);		/*	src->alpha = 0xff;*/
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
	if (SPELL_00==src->boss_base_spell_type)		/* �e�������I���Ȃ�e������ */
	{
//		alice_anime_count = 48;
		spell_card_get_spell_number(src);	/* �X�y�J�V�X�e������A���݂̌`�ԂŌ��e���ԍ�����ɓ���A�W����boss_base_spell_time_out���Ԃ�ݒ�B */
		src->vvv256 = t256(1.0);
	}
//
	{
		static int seisei_count;
		if (0==seisei_count)
		{
			add_zako_sakuya_doll_02_star(src);/* �q�������w�ǉ� */
		//	add_zako_sakuya_doll_03_360knife(src);/* �q�������w�ǉ� */
		}
		seisei_count++;
		seisei_count &= 0x00ff; 	/* 256 == 0x100 == ( 4 x 64). [ 4�b���炢] */
	}
//
	if (src->sakuya_data_wait2_256 < t256(0.0) )
	{
		src->sakuya_data_wait2_256	= t256(0.0);
//???		src->sakuya_data_wait1		= 120;
		#if 0
		/* �X�y���J�[�h�V�X�e���ł͑Ή��ł��Ȃ��B */
		sp ell_card_number = SPELL_CARD_09_sakuya_iii/*4*/;
		#endif
		#if 0/*???????????*/
		sp ell_card_number += (4);/* �悭�킩��Ȃ��H�H�H*/
		#endif
//???		src->boss_base_state777 = SG02;
		//	[SAKUYA_ANIME_03_CENTER_A;
	}
	else
	{
		src->sakuya_data_wait2_256 -= t256(0.03);
		if (src->color32 > 0xf9000000)	/*	if (src->alpha > 0xf9) */	/* (src->alpha>0xff) */
		{
			src->color32 = 0xffffffff;	/*	src->alpha = 0xff;*/
		}
		else /*if (src->alpha<0xff)*/
		{
			src->color32 += 0x05000000; /*	src->alpha += 0x05;*/
		}
		src->cx256 += ((sin1024((src->tmp_angleCCW1024))*(src->sakuya_data_wait2_256))>>8);/*fps_factor*/		/* CCW�̏ꍇ */
		src->cy256 += ((cos1024((src->tmp_angleCCW1024))*(src->sakuya_data_wait2_256))>>8);/*fps_factor*/
	}
	//	[SAKUYA_ANIME_12_HATUDAN_START)
	bullet_angle_all_gamen_gai_nara_kesu();/*�p�x�e�̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�)*/
}


/*---------------------------------------------------------

---------------------------------------------------------*/
#if 0
static int sakuya_kyori(SPRITE *src)
{
	int rrr256;
	//	���������߂�̂ɂQ�悵�ă��[�g�����̂̓��[�g���Z���x���̂ŁA�H�v���K�v�����B
	//	�\�߃e�[�u�������Ă������A��։��Z(CODIC)�ŋ��߂邩�A���ʊ֐��ɂ���CPU��float�ϊ����߂��ŏ��ɗ��߂邩�A�ǂꂩ�B
	//	src->length_s_p = sqrt((obj_player->x-src->x)*(obj_player->x-src->x)+(obj_player->y-src->y)*(obj_player->y-src->y));
	{	/* sqrt()�x���̂ŊȈՔŁB�Ƃ����������ɂ͍S��Ȃ����ɂ��� */
		int xxx256;
		int yyy256;
		xxx256 = abs((int)obj_player->cx256-(int)src->cx256);/*xxx256+=t256(10);*/ /* abs()�͓W�J���Ă���邩����v */
		yyy256 = abs((int)obj_player->cy256-(int)src->cy256);/*yyy256+=t256(10);*/
		if (xxx256>yyy256)	{rrr256 = (xxx256/*+(yyy256>>1)*/);}/*�����������*/
		else				{rrr256 = (yyy256/*+(xxx256>>1)*/);}/*�����������*/
	}
	if (t256(40.0) > rrr256)	{	rrr256 = t256(40.0);}	/* 32 10 */
	else
	if (t256(128.0) < rrr256)	{	rrr256 = t256(128.0);}	/* 128 80 */
	return (rrr256);
}
#endif


/*---------------------------------------------------------
	SPELL_CARD_09_sakuya_iii	���`��: �����u�t�F�X�e�B�o���i�C�t�v �ŏI�`��(����1)
---------------------------------------------------------*/

global void sakuya_09_keitai(SPRITE *src)
{
	{
		static int seisei_count;
		if (0==seisei_count)
		{
		/*(�e�X�g)*/	add_zako_sakuya_doll_02_star(src);/* �q�������w�ǉ� */
		//	add_zako_sakuya_doll_03_360knife(src);/* �q�������w�ǉ� */
		}
		seisei_count++;
		seisei_count &= 0x01ff; 	/* 512 == 0x200 == ( 8 x 64). [ 8�b���炢] */
	}
//
	src->boss_base_state777++;

	if (80 > src->boss_base_state777)
	{
	//case SG00:
	//	src->sakuya_data_wait1--;/*fps_factor*/
	//	break;
	}
	else
	//	if (0 > src->sakuya_data_wait1) /* �ҋ@���Ԃ��I������� */
	if (81 > src->boss_base_state777)
	{
		//	src->boss_base_state777++;/* = SG01*/
//++		pd->bo ssmode = B00_NONE/*B01_BA TTLE*/;
			obj_send1->cx256	= (t256(GAME_WIDTH/2)); 		/* �e��x256 */
			obj_send1->cy256	= (t256(30));					/* �e��y256 */
			tmp_angleCCW1024_src_nerai(obj_send1, src);
			src->sakuya_data_move_angle1024 = src->tmp_angleCCW1024;
	}
	else
	if (1024 > src->boss_base_state777)
	{
	//case SG01:
		if ( (src->cx256 < t256(GAME_WIDTH/2+30) ) &&
			 (src->cx256 > t256(GAME_WIDTH/2-10) ) &&
			 (src->cy256 < t256(50.0)) ) /* �^�񒆂ɗ����� */
		{
			src->boss_base_state777 = (1024+1024-1)-(20);// src->boss_base_state777++;/* = SG02*/
		//	src->sakuya_data_wait1		= (20);
			src->sakuya_data_wait2_256	= t256(0.0);
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
	if ((1024+1024 ) > src->boss_base_state777)
	{
		;/* �������Ȃ�(�ҋ@����) */
//	case SG02:
//		{
//				src->sakuya_data_wait1--;/*fps_factor*/
//			if (0 > src->sakuya_data_wait1) 		{	src->boss_base_state777++;/* = SG03*/
//											//	[SAKUYA_ANIME_12_HATUDAN_START;
//			}		/* �ҋ@���Ԃ��I������� */
//		}
//		break;
	}
	else
	if ((1024+1024 +1) > src->boss_base_state777)
	{
//	case SG03:
		//	[SAKUYA_ANIME_15_HATUDAN_END)
		{
				bullet_create_sakuya_ryoute_knife(src);
		//	src->sakuya_data_wait1 = 40;
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
	if ((1024+1024 +1+40) > src->boss_base_state777)
	{
	//	else
		{
			/* CCW�̏ꍇ */
			src->cx256 += ((sin1024((src->sakuya_data_move_angle1024))*t256(3.5))>>8);/*fps_factor*/
			src->cy256 += ((cos1024((src->sakuya_data_move_angle1024))*t256(3.5))>>8);/*fps_factor*/
		}
	}
	else
	{
	//case SG04:
	//		src->sakuya_data_wait1--;/*fps_factor*/
	//	if (0 > src->sakuya_data_wait1)
		{
		//	src->sakuya_data_wait1	= (1024+1024-1)-(120)-((3-difficulty)*50);
		//	src->boss_base_state777 = SG02;
			src->boss_base_state777 = (1024+1024-1)-(120)-((3-(cg_game_difficulty))*50);
		}
	//	break;
	}
				SPRITE *zzz_player;
				zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
	#if 1/*Gu(���S���W)*/
	if ((zzz_player->cx256 < src->cx256 + t256(25)) &&	/* ???????????? */
		(zzz_player->cx256 > src->cx256 - t256(25)))
	#endif
	{	src->sakuya_data_wait2_256 += t256(1.0);/*fps_factor*/}
	if (src->sakuya_data_wait2_256 > t256(20.0)+((3-(cg_game_difficulty))*t256(30)))
	{
		//	[(SAKUYA_ANIME_12_HATUDAN_START+1);/*16???*/
	//	src->length_s_p256 = sakuya_kyori(src);
		{
	//		src->cx256				= tmp1_256;/* ���̂����ړ��H�H�H */
	//		src->cy256				= tmp2_256;/* ���̂����ړ��H�H�H */
	//		src->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x00);			/*	src->alpha			= 0x00;*/
			src->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x80);			/*	src->alpha			= 0x00;*/	/* ������ */
	//		//	[SAKUYA_ANIME_15_HATUDAN_END;//?????
			src->tmp_angleCCW1024	= 0;//(ii<<7);
			src->sakuya_data_wait2_256			= /*speed256*/t256(2.0) + ((cg_game_difficulty)<<8);
			#if 0/*???*/
			/* �X�y���J�[�h�V�X�e���ł͑Ή��ł��Ȃ��B */
			sp ell_card_number		= SPELL_CARD_10_sakuya_jjj;/*5*/ /*???*/
			#endif
		}
		bullet_crate_sakuya_kurukuru(src);/*,*/ /*1+difficulty,*/ /*(src->length_s_p256)*/
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

global void sakuya_06_keitai(SPRITE *src)
{
	if (0==src->boss_base_state777)
	{
//	{
//		static int seisei_count;
//		if (0==seisei_count)
		{
			add_zako_sakuya_doll_01_laser(src);/* ���[�U�[�ǉ� */
		}
//		seisei_count++;
//		seisei_count &= 0x01ff; 	/* 512 == 0x200 == ( 8 x 64). [ 8�b���炢] */
//	}
		src->sakuya_data_wait1		= (3);	/* 3�񌂂����� */
		src->sakuya_data_wait2_256	= t256(50.0);
		src->boss_base_state777 = 1;
	}
	else
	{
		src->sakuya_data_wait2_256 -= t256(1.0);/*fps_factor*/
		if (src->sakuya_data_wait2_256 < t256(0.0))
		{
			src->sakuya_data_wait2_256	= t256(30.0);
			/* (���F�����e) */
			#if 1
			tmp_angleCCW1024_jiki_nerai(src);/* ���@�_���p�쐬 */
			#endif
			/* CCW�̏ꍇ */
			bullet_crate_sakuya_linear_yuudou(src); 	/* �����U���i�C�t */
			//bullet_create_oodama0(src, t256(2), ANGLE_JIKINERAI_DAN, 40, 0, t256(src->w/2), t256(src->h/2));
			src->sakuya_data_wait1--;/*fps_factor*/
			if (0 > src->sakuya_data_wait1) 	/* 3�񌂂����� */
			{
				src->sakuya_data_wait1 = (6-(cg_game_difficulty));	/* 5�񌂂����� */	/*(20) 20�񂾂ƌ��Ȃ������Ɏ��̌`�ԂɂȂ��Ă� */
				if (0==(ra_nd()&(2-1)/*%2*/))
						{	src->target_x256 = t256(100);				}
				else	{	src->target_x256 = t256(GAME_WIDTH-100);	}
				src->vvv256 = t256(1.00);
			}
		}
	}
	#if 1
	alice_yuudou_move_only(src);
	alice_yuudou_calc(src);
	#endif
	bullet_angle_all_gamen_gai_nara_kesu();/*�p�x�e�̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�)*/
}


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
		//	add_zako_sakuya_doll_02_star(src);/* �q�������w�ǉ� */
	//	/* �s���N�n�[�g */	add_zako_sakuya_doll_03_360knife(src);/* �q�������w�ǉ� */
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
		//	add_zako_sakuya_doll_02_star(src);/* �q�������w�ǉ� */
	//	/* �s���N�n�[�g */	add_zako_sakuya_doll_03_360knife(src);/* �q�������w�ǉ� */
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
	if (SPELL_00==src->boss_base_spell_type)		/* �e�������I���Ȃ�e������ */
	{
		spell_card_get_spell_number(src);	/* �X�y�J�V�X�e������A���݂̌`�ԂŌ��e���ԍ�����ɓ���A�W����boss_base_spell_time_out���Ԃ�ݒ�B */
	//
	}
	if (/*(256-64)*/(255/*192+64*/) == src->boss_base_spell_time_out) /* ���]�ړ������Z�b�g */
	{
		{
			/* ���`�Ԃł͎g���Ė������痘�p(���p)���� */
			#define sakuya_data_sayuu_mode sakuya_data_wait2_256
			{
				src->sakuya_data_sayuu_mode ^= (2-1);/* ���] */
				if (/*0==*/src->sakuya_data_sayuu_mode & (2-1))
						{	src->target_x256 = SAKUYA_POINT_X_MID-(t256(128));	}//96==128*0.75
				else	{	src->target_x256 = SAKUYA_POINT_X_MID+(t256(128));	}//
			}
			src->vvv256 = t256(1.00);
		}
	}
	//
	else
//	if (/*(256-256)*/(59)/*((251+64)-256)*/ == src->boss_base_spell_time_out) /* �^���փ��[�v */
	if (/*(256-256)*/(59)/*((251+64)-256)*/ == src->boss_base_spell_time_out) /* �^���փ��[�v */
	{
		src->target_x256 = src->cx256 = t256(GAME_WIDTH/2); /* �^���փ��[�v */
		src->vvv256 = t256(1.00);
	}
	else
	if (/*(256-256)*/(58)/*((250+64)-256)*/ == src->boss_base_spell_time_out) /* �^���փ��[�v */
//	if (/*(256-256)*/(0) == src->boss_base_spell_time_out) /* �^���փ��[�v */
//	if (/*(256-192)*/(64) == src->boss_base_spell_time_out) /* �^���փ��[�v */
//	if (/*(256-180)*/(76) == src->boss_base_spell_time_out) /* �^���փ��[�v */
	{
		src->target_x256 = src->cx256 = t256(GAME_WIDTH)-src->cx256;			/* ���Α��փ��[�v */
		src->vvv256 = t256(1.00);
	}

	#if 1
	alice_yuudou_move_only(src);
	alice_yuudou_calc(src);
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
	if (SPELL_00==src->boss_base_spell_type)		/* �e�������I���Ȃ�e������ */
	{
		spell_card_get_spell_number(src);	/* �X�y�J�V�X�e������A���݂̌`�ԂŌ��e���ԍ�����ɓ���A�W����boss_base_spell_time_out���Ԃ�ݒ�B */
		src->vvv256 = t256(1.0);
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
	h->callback_loser				= sakuya_put_items;

//	h->sakuya_data_wait1						= (0);/*����sakuya_02_keitai()�ŏ��������邩�炽�Ԃ�s�v*/
//	h->sakuya_data_wait2_256					= t256(0.0);/*���� sakuya_02_keitai()�Ńr�b�g���g���Asakuya_03_keitai()�ŏ��������邩�炽�Ԃ�s�v*/
//	h->sakuya_data_move_angle1024				= (0);/*����sakuya_09_keitai()�ŏ��������邩��s�v*/
	//-------------------------
	obj_send1->type 				= SP_DUMMY_MUTEKI;
	obj_send1->cx256				= (0);
	obj_send1->cy256				= (0);
}
#endif

