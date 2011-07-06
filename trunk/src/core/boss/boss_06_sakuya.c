
#include "boss.h"

/*static*/extern void add_zako_sakuya_doll_01_laser(SPRITE *src);/* ���[�U�[ */
/*static*/extern void add_zako_sakuya_doll_02_star(SPRITE *src);/* �q�������w(���^��`��) */
/*static*/extern void add_zako_sakuya_doll_03_360knife(SPRITE *src);/* �q�������w(360�i�C�t������) */
/*static*/extern void add_zako_sakuya_baramaki(SPRITE *src);/* ��� �΂�T���e �I�v�V���� */

/*static*/extern void bullet_create_sakuya_ryoute_knife(SPRITE *src);

/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
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


#define move_angle1024		user_data04 	/* ���̊p�x */
#define wait1				user_data05 	/* ���̊p�x */
#define wait2_256			user_data06 	/* ���̊p�x */

//	int length_s_p256;	/* ���ƃv���C���[�Ƃ̋��� */

//
/*---------------------------------------------------------

---------------------------------------------------------*/

enum
{
	SG00 = 0,
	SG01,
	SG02,
	SG03,
	SG04,
};

enum
{
	SE00 = 0,
	SE01,
	SE02,
};

enum
{
	SD00 = 0,
	SD01,
	SD02,
};
enum
{
	SC00 = 0,
	SC01,
	SC02,
	SC03,
	SC04,
	SC05,
	SC06,
	SC07,
};



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
	src->wait1		= 80;
	src->wait2_256	= t256(-100);

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
#define SAKUYA_POINT_X_MID			(t256(GAME_WIDTH*2/4))/*-src->w/2*/
#define SAKUYA_POINT_X_RIGHT_MID	(t256(GAME_WIDTH*3/4))
#define SAKUYA_POINT_X_RIGHT_OUT	(t256(GAME_WIDTH*4/4))/*-src->w  */
#define SAKUYA_POINT_X_MARGIN		(t256(16.0))

#define SAKUYA_POINT_Y_CENTER		(t256(50.0))




/*---------------------------------------------------------
	SPELL_CARD_11_sakuya_kkk	��10�`��: �ŏI�`��(����3)
---------------------------------------------------------*/

global void sakuya_11_keitai(SPRITE *src)
{
	{
		static int seisei_count;
		if(0==seisei_count)
		{
			add_zako_sakuya_doll_01_laser(src);/* �q�������w�ǉ� */
			/* hard or lunatic */
			if (1 < difficulty)
			{
				add_zako_sakuya_doll_02_star(src);/* �q�������w�ǉ� */
				/* lunatic only */
				if (2 < difficulty)
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
	if (DANMAKU_00==src->boss_base_danmaku_type)		/* �e�������I���Ȃ�e������ */
	{
//		alice_anime_count = 48;
		spell_card_get_danmaku_number(src); 	/* �X�y�J�V�X�e������A���݂̌`�ԂŌ��e���ԍ�����ɓ���A�W����boss_base_danmaku_time_out���Ԃ�ݒ�B */
		src->vvv256 = t256(1.0);
	}
//
	{
		static int seisei_count;
		if(0==seisei_count)
		{
			add_zako_sakuya_doll_02_star(src);/* �q�������w�ǉ� */
		//	add_zako_sakuya_doll_03_360knife(src);/* �q�������w�ǉ� */
		}
		seisei_count++;
		seisei_count &= 0x00ff; 	/* 256 == 0x100 == ( 4 x 64). [ 4�b���炢] */
	}
//
	if (src->wait2_256 < t256(0.0) )
	{
		src->wait2_256	= t256(0.0);
		src->wait1		= 120;
		#if 0
		/* �X�y���J�[�h�V�X�e���ł͑Ή��ł��Ȃ��B */
		sp ell_card_number = SPELL_CARD_09_sakuya_iii/*4*/;
		#endif
		#if 0/*???????????*/
		sp ell_card_number += (4);/*�悭�킩��Ȃ��H�H�H*/
		#endif
		src->boss_base_state777 = SG02;
		//	[SAKUYA_ANIME_03_CENTER_A;
	}
	else
	{
		src->wait2_256 -= t256(0.03);
		if (src->color32 > 0xf9000000)	/*	if (src->alpha > 0xf9)	*/	/* (src->alpha>0xff)*/
		{
			src->color32 = 0xffffffff;	/*	src->alpha = 0xff;*/
		}
		else /*if (src->alpha<0xff)*/
		{
			src->color32 += 0x05000000; /*	src->alpha += 0x05;*/
		}
		src->cx256 += ((sin1024((src->tmp_angleCCW1024))*(src->wait2_256))>>8);/*fps_factor*/		/* CCW�̏ꍇ */
		src->cy256 += ((cos1024((src->tmp_angleCCW1024))*(src->wait2_256))>>8);/*fps_factor*/
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
#if 1
/* �v���C���[�̎���ɎU��΂�A���Ԃ��o�ƃv���C���[�_���e */
static void enemy_boss06_knifes3(SPRITE *src)	/*,*/ /*dou ble speed,*/	/*int length222*/
{
	int len256;
	int tmp1_256;
	int tmp2_256;
	int tmp888;
	tmp888			= 0;
	/* ������x�����ɏo�Ȃ��ƔF�����Â炢 */
	/* �L����(�R�A)���ł����Ɣ�����O�Ɏ��ʁB(�i�C�t���L����O�ɂ�����) */
	len256			= t256(/*70.0*/80.0) -((difficulty)<<8);	/*-length222*/	/*/2*/
	src->wait2_256	= t256(0.0);
				SPRITE *zzz_player;
				zzz_player = &obj00[FIX_OBJ_00_PLAYER];
	while (1)
	{
		tmp888		= (((int)ra_nd()+(t256_floor(src->wait2_256)) )&(8-1)/*%8*/);
		src->wait2_256	+= t256(1.0);
		len256			+= t256(0.1);
	/* CCW�̏ꍇ */
		#if 1/*Gu(���S���W)*/
		tmp1_256 = zzz_player->cx256 + ((sin1024((tmp888<<7))*len256)>>8);
		tmp2_256 = zzz_player->cy256 + ((cos1024((tmp888<<7))*len256)>>8);
		#endif
		if ((( t256(30) < tmp1_256) && (tmp1_256 < t256(GAME_WIDTH-30))) &&
			(( t256(30) < tmp2_256) && (tmp2_256 < t256(GAME_HEIGHT-72))))
		{	break;	}
	}
	int tmp_x256;
	int tmp_y256;
	#if 1/*Gu(���S���W)*/
	tmp_x256	= zzz_player->cx256;
	tmp_y256	= zzz_player->cy256;
	#endif
	int ii;
	for (ii=0; ii<8; ii++)
	{
		if (tmp888==ii)
		{
			src->cx256				= tmp1_256;
			src->cy256				= tmp2_256;
//			src->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x00);			/*	src->alpha			= 0x00;*/
			src->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x80);			/*	src->alpha			= 0x00;*/	/* ������ */
//			//	[SAKUYA_ANIME_15_HATUDAN_END;//?????
			src->tmp_angleCCW1024	= (ii<<7);
			src->wait2_256			= /*speed256*/t256(1.0)+(difficulty<<8);
			#if 0/*???*/
			/* �X�y���J�[�h�V�X�e���ł͑Ή��ł��Ȃ��B */
			sp ell_card_number		= SPELL_CARD_10_sakuya_jjj;/*5*/ /*???*/
			#endif
		}
		else
		{
			/* �v���C���[�_���e */
			#if 1
			/* CCW�̏ꍇ */
			obj_send1->cx256 	= (src->cx256)	= tmp_x256 + ((sin1024((ii<<7))*len256)>>8);
			obj_send1->cy256 	= (src->cy256)	= tmp_y256 + ((cos1024((ii<<7))*len256)>>8);
			#endif
			br.BULLET_REGIST_speed256						= t256(1.0/*2.0*/); 				/* ���x */
			br.BULLET_REGIST_angle1024						= (ii<<7);/*i*(1024)*1/8*/			/* �p�x */
//			br.BU LLET_REGIST_sakuya_kurukurku_knife_height = t256(32)-(ii&(2-1)/*%2*/)*64*256; 		/* -32 or 32 */
			br.BULLET_REGIST_regist_type					= REGIST_TYPE_04_KURU_KURU;
			bullet_regist_vector();
		}
	}
}
#endif


/*---------------------------------------------------------
	SPELL_CARD_09_sakuya_iii	���`��: �ŏI�`��(����1)
---------------------------------------------------------*/

global void sakuya_09_keitai(SPRITE *src)
{
	{
		static int seisei_count;
		if(0==seisei_count)
		{
		/*�Ă���*/	add_zako_sakuya_doll_02_star(src);/* �q�������w�ǉ� */
		//	add_zako_sakuya_doll_03_360knife(src);/* �q�������w�ǉ� */
		}
		seisei_count++;
		seisei_count &= 0x01ff; 	/* 512 == 0x200 == ( 8 x 64). [ 8�b���炢] */
	}
//
	switch (src->boss_base_state777)
	{
	case SG00:
		src->wait1--;/*fps_factor*/
		if (0 > src->wait1) /* �ҋ@���Ԃ��I������� */
		{
			src->boss_base_state777++;/* = SG01*/
//++		pd->bo ssmode = B00_NONE/*B01_BA TTLE*/;
			obj_send1->cx256 	= (t256(GAME_WIDTH/2)); 		/* �e��x256 */
			obj_send1->cy256 	= (t256(30));					/* �e��y256 */
			tmp_angleCCW1024_src_nerai(obj_send1, src);
			src->move_angle1024 = src->tmp_angleCCW1024;
		}
		break;
	case SG01:
		if ( (src->cx256 < t256(GAME_WIDTH/2+30) ) &&
			 (src->cx256 > t256(GAME_WIDTH/2-10) ) &&
			 (src->cy256 < t256(50.0)) ) /* �^�񒆂ɗ����� */
		{
			src->boss_base_state777++;/* = SG02*/
			src->wait1		= (20);
			src->wait2_256	= t256(0.0);
			//	[SAKUYA_ANIME_03_CENTER_A;
		}
		else
		{
			src->cx256 += ((sin1024((src->move_angle1024))*t256(3.0))>>8);/*fps_factor*/ 	/* CCW�̏ꍇ */
			src->cy256 += ((cos1024((src->move_angle1024))*t256(3.0))>>8);/*fps_factor*/
		}
		break;
	case SG02:
		{
				src->wait1--;/*fps_factor*/
			if (0 > src->wait1) 		{	src->boss_base_state777++;/* = SG03*/
											//	[SAKUYA_ANIME_12_HATUDAN_START;
			}		/* �ҋ@���Ԃ��I������� */
		}
		break;
	case SG03:
		//	[SAKUYA_ANIME_15_HATUDAN_END)
		{
			{
			//	int rrr256; 	//	/*src->length_s_p256*/rrr256 = sakuya_kyori(src);
		//		bullet_create_sakuya_even_knife(src, t256(10), /*src->length_s_p256*/ /*rrr256,*/ 0/*0==�E*/);
		//		bullet_create_sakuya_even_knife(src, t256(10), /*src->length_s_p256*/ /*rrr256,*/ 1/*1==��*/);
				bullet_create_sakuya_ryoute_knife(src);
			}
			src->wait1=40;
			if (src->cy256 > t256(150))
		//	{	src->move_angle1024 = cv1024r(	  210) + (((ra_nd()&(4096-1))*21)>>8) /*(ra_nd()%(deg_360_to_512(120)))*/;} 	/* CW�̏ꍇ */
		//	{	src->move_angle1024 = cv1024r(360-210) + (((ra_nd()&(4096-1))*21)>>8) /*(ra_nd()%(deg_360_to_512(120)))*/;} 	/* CCW�̏ꍇ */
			{	src->move_angle1024 = cv1024r(360-210) + (((ra_nd()&(4096-1))*21)>>8) /*(ra_nd()%(deg_360_to_512(120)))*/;}
			else
			{	src->move_angle1024 = ((ra_nd()&(1024-1)));}
			#if 1
			/*�O�̈�*/
			mask1024(src->move_angle1024);
			#endif
			//	[(SAKUYA_ANIME_12_HATUDAN_START+1);/*16???*/
			src->boss_base_state777++;/* = SG04*/
		}
		break;
	case SG04:
			src->wait1--;/*fps_factor*/
		if (0 > src->wait1)
		{
			src->wait1	= 120+(3-difficulty)*50;
			src->boss_base_state777 = SG02;
		}
		else
		{
			/* CCW�̏ꍇ */
			src->cx256 += ((sin1024((src->move_angle1024))*t256(3.5))>>8);/*fps_factor*/
			src->cy256 += ((cos1024((src->move_angle1024))*t256(3.5))>>8);/*fps_factor*/
		}
		break;
	}
				SPRITE *zzz_player;
				zzz_player = &obj00[FIX_OBJ_00_PLAYER];
	#if 1/*Gu(���S���W)*/
	if ((zzz_player->cx256 < src->cx256 + t256(25)) &&  /* ???????????? */
		(zzz_player->cx256 > src->cx256 - t256(25)))
	#endif
	{	src->wait2_256 += t256(1.0);/*fps_factor*/}
	if (src->wait2_256 > t256(20.0)+((3-difficulty)*t256(30)))
	{
		//	[(SAKUYA_ANIME_12_HATUDAN_START+1);/*16???*/
	//	src->length_s_p256 = sakuya_kyori(src);
		enemy_boss06_knifes3(src/*,*/ /*1+difficulty,*/ /*(src->length_s_p256)*/ );
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
/*static*/extern void bullet_crate_sakuya_linear_yuudou(SPRITE *src);	/* �����U���i�C�t */

global void sakuya_06_keitai(SPRITE *src)
{
	if (0==src->boss_base_state777)
	{
//	{
//		static int seisei_count;
//		if(0==seisei_count)
		{
			add_zako_sakuya_doll_01_laser(src);/* ���[�U�[�ǉ� */
		}
//		seisei_count++;
//		seisei_count &= 0x01ff; 	/* 512 == 0x200 == ( 8 x 64). [ 8�b���炢] */
//	}
		src->wait1		= (3);	/* 3�񌂂����� */
		src->wait2_256	= t256(50.0);
		src->boss_base_state777 = 1;
	}
	else
	{
		src->wait2_256 -= t256(1.0);/*fps_factor*/
		if (src->wait2_256 < t256(0.0))
		{
			src->wait2_256	= t256(30.0);
			/* (���F�����e) */
			#if 1
			tmp_angleCCW1024_jiki_nerai(src);/* ���@�˂炢�p�쐬 */
			#endif
			/* CCW�̏ꍇ */
			bullet_crate_sakuya_linear_yuudou(src); 	/* �����U���i�C�t */
			//bullet_create_oodama0(src, t256(2), ANGLE_JIKINERAI_DAN, 40, 0, t256(src->w/2), t256(src->h/2));
			src->wait1--;/*fps_factor*/
			if (0 > src->wait1) 	/* 3�񌂂����� */
			{
				src->wait1 = (6-difficulty);	/* 5�񌂂����� */	/*(20) 20�񂾂ƌ��Ȃ������Ɏ��̌`�ԂɂȂ��Ă� */
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
	��l�`��: ���� �����w����(�������邾��)
---------------------------------------------------------*/

global void sakuya_04_keitai(SPRITE *src)
{
	{
		static int seisei_count;
		if(0==seisei_count)
		{
		//	add_zako_sakuya_doll_02_star(src);/* �q�������w�ǉ� */
			add_zako_sakuya_doll_03_360knife(src);/* �q�������w�ǉ� */
		}
		seisei_count++;
		seisei_count &= 0x01ff; 	/* 512 == 0x200 == ( 8 x 64). [ 8�b���炢] */
	}
//
	switch (src->boss_base_state777)
	{
	case SD00:
		add_zako_sakuya_baramaki(src);/* �q�������w�ǉ� */
		//
		src->boss_base_state777++;
		src->target_x256 = SAKUYA_POINT_X_MID;/*SAKUYA_POINT_00_LEFT*/
		src->target_y256 = SAKUYA_POINT_Y_CENTER;/*SAKUYA_POINT_00_LEFT*/
		src->vvv256 = t256(1.00);
		break;
#if 0/* �X�y���J�[�h�V�X�e���ł͑Ή��ł��Ȃ��B */
	case SD01:
		if (/*150*/200 > src->vvv256 )	/* ���Ԃ� */	/* �^�񒆂ɗ����� */
		{
			src->boss_base_state777++;/* = SE01*/ /*dummy*/
		//	src->boss_base_state777 	=	0/* = SE01*/;
		//	src->wait1		= (10);
//			src->wait1		= (5);/*???*/
		//	src->wait2_256	= t256(0.0);
//			src->wait2_256	= t256(10.0);/*???*/
				//	[SAKUYA_ANIME_08_HIKARU_START;
		//	[SAKUYA_ANIME_11_HIKARU_END)
			/* �q�������w����(2�񐶐������Ⴄ�o�O����) */
			#if 0/*???*/
			/* �X�y���J�[�h�V�X�e���ł͑Ή��ł��Ȃ��B */
			sp ell_card_number += (4);		/* ���̌`�Ԃ� */
			#endif
			//	[SAKUYA_ANIME_11_HIKARU_END)
		}
		break;
#endif
	}
	#if 1
	alice_yuudou_move_only(src);
	alice_yuudou_calc(src);
	#endif
	bullet_angle_all_gamen_gai_nara_kesu();/*�p�x�e�̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�)*/
}


/*---------------------------------------------------------
	SPELL_CARD_03_sakuya_ccc
	���J�u�o�[�e�B�J���i�C�t�v
	��O�`��: �����i�C�t�������Ă����
---------------------------------------------------------*/
static void sakuya_shot_111(SPRITE *src)
{
	{
	//	const short sss[(4)] =
	//	{
	//	t256(2*1.0),	//	t256(2*0.48),	/*	easy:�܂΂�Ȃ̂ŊȒP*/ 						/* 1.2 */
	//	t256(2*1.0),	//	t256(2*0.75),	/*normal:�܂΂�Ȃ̂ŊȒP*/ 						//	t256(0.48/*0.5*/ /*1.0*/ /*1.5*/),/*normal:��Ȃ̂ő傫��������ΊȒP*/
	//	t256(2*18.5),	//	t256(2*1.0),	/*	hard:���܂ɓ����������*/ 					/*2.0*/
	//	t256(2*18.5)	//	t256(2*18.5)	/*	luna:���ւ����̃X�L�}�������ł������H*/		/*6.5*/ /*12.5*/ /*8.5*/
	//		/*(r32)luna:�R�c���v�邯�ǂ������ȒP�ɂȂ����A���̑�(hard�ȉ�)�͘_�O�ɊȒP */
	//		/*(r31)(luna:)�{�����������������ǁA���I�ɂ�(���̗�10%�ȉ���)�C�������\�B�ł��m��5-10%(10����Ɉ��)���炢�͎��ʁB���肵�Ȃ��ȁB */
	//		/*(-r30)(luna:�{����Ȃ��Ă��C�������łȂ�Ƃ��Ȃ���x�ɓ�Փx�}�������̂����܂����������)*/
	//	};
		src->wait2_256 += t256(2*18.5);//	/*t256(4)*/sss[difficulty];/*t256(3*1.42)*/
	}
	if (0==(((int)src->wait2_256)&(32-1-1)/*%21 t256(15*1.42)*/))/* 0x1e == (32-1-1) */
	{
		obj_send1->cx256 					= (src->cx256);
		obj_send1->cy256 					= (src->cy256);
		br.BULLET_REGIST_speed256			= t256(1.75);/*(2.0)*/ /*(0.7)*/
		br.BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_05_AKA;
		br.BULLET_REGIST_regist_type		= REGIST_TYPE_02_GRAVITY02;
		#if 0
		/* CW�̏ꍇ */
		br.BULLET_REGIST_angle1024	= ( cv1024r(360)-((t256_floor(src->wait2_256))&((1024/2)-1)/*%(180*1.42)*/));	br.BULLET_REGIST_jyuryoku_dan_delta256	= t256(0.03);	bullet_regist_vector();
		br.BULLET_REGIST_angle1024	= ( cv1024r(270)-((t256_floor(src->wait2_256))&((1024/2)-1)/*%(180*1.42)*/));	br.BULLET_REGIST_jyuryoku_dan_delta256	= t256(0.04);	bullet_regist_vector();
		br.BULLET_REGIST_angle1024	= ( cv1024r(180)-((t256_floor(src->wait2_256))&((1024/2)-1)/*%(180*1.42)*/));	br.BULLET_REGIST_jyuryoku_dan_delta256	= t256(0.02);	bullet_regist_vector();
		#else
		/* CCW�̏ꍇ */
		br.BULLET_REGIST_angle1024	= (-cv1024r(360)-((t256_floor(src->wait2_256))&((1024/2)-1)/*%(180*1.42)*/));	br.BULLET_REGIST_jyuryoku_dan_delta256	= t256(0.03);	bullet_regist_vector();
		br.BULLET_REGIST_angle1024	= (-cv1024r(270)-((t256_floor(src->wait2_256))&((1024/2)-1)/*%(180*1.42)*/));	br.BULLET_REGIST_jyuryoku_dan_delta256	= t256(0.04);	bullet_regist_vector();
		br.BULLET_REGIST_angle1024	= (-cv1024r(180)-((t256_floor(src->wait2_256))&((1024/2)-1)/*%(180*1.42)*/));	br.BULLET_REGIST_jyuryoku_dan_delta256	= t256(0.02);	bullet_regist_vector();
		#endif
		voice_play(VOICE12_SAKUYA_B2_SHOT, TRACK04_TEKIDAN);
	}
}
static void sakuya_shot_222(SPRITE *src)
{
	src->wait2_256 += t256(4.0);/*(3*1.42)*/
	if (0x0000==((src->wait2_256)&(0x0f00)))/*%(12*1.42)*/
	{
		obj_send1->cx256 					= (src->cx256);
		obj_send1->cy256 					= (src->cy256);
		br.BULLET_REGIST_speed256			= t256(3.00);
		br.BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_05_AKA;
		br.BULLET_REGIST_regist_type		= REGIST_TYPE_02_GRAVITY02;
		#if 0
		/* CW�̏ꍇ */
				br.BULLET_REGIST_angle1024	= (((t256_floor(src->wait2_256))&(256-1)/*%(180*1.42)*/)+cv1024r(180)); 		br.BULLET_REGIST_jyuryoku_dan_delta256	= t256(0.03);	bullet_regist_vector();
		if (difficulty)
		{
				br.BULLET_REGIST_angle1024	= (((t256_floor(src->wait2_256))&(256-1)/*%(180*1.42)*/)+cv1024r(90));			br.BULLET_REGIST_jyuryoku_dan_delta256	= t256(0.04);	bullet_regist_vector();
			if (1<difficulty)
			{	br.BULLET_REGIST_angle1024	= (((t256_floor(src->wait2_256))&(256-1)/*%(180*1.42)*/));						br.BULLET_REGIST_jyuryoku_dan_delta256	= t256(0.02);	bullet_regist_vector();}
		}
		#else
		/* CCW�̏ꍇ */
				br.BULLET_REGIST_angle1024	= (((t256_floor(src->wait2_256))&(256-1)/*%(180*1.42)*/)+cv1024r(360-180)); 	br.BULLET_REGIST_jyuryoku_dan_delta256	= t256(0.03);	bullet_regist_vector();
		if (difficulty)
		{
				br.BULLET_REGIST_angle1024	= (((t256_floor(src->wait2_256))&(256-1)/*%(180*1.42)*/)+cv1024r(360-90));		br.BULLET_REGIST_jyuryoku_dan_delta256	= t256(0.04);	bullet_regist_vector();
			if (1<difficulty)
			{	br.BULLET_REGIST_angle1024	= (((t256_floor(src->wait2_256))&(256-1)/*%(180*1.42)*/));						br.BULLET_REGIST_jyuryoku_dan_delta256	= t256(0.02);	bullet_regist_vector();}
		}
		#endif
		voice_play(VOICE12_SAKUYA_B2_SHOT, TRACK04_TEKIDAN);
	}
}
static void sakuya_shot_333(SPRITE *src)
{
//	src->move_angle1024 = at an_1024_SAKUYA_MID;
//	if (0 == ((t256_floor(src->wait2_256))&(32-1)/*%20*/) )
	if (0 == (( (src->vvv256))&(32-1)/*%20*/) )
	{
//		int b_wait2_high=(((int)(( (src->vvv256))>>5/*/20*/)));
		{
//			obj_send1->cx256 	=	(src->cx256);
//			obj_send1->cy256 	=	(src->cy256);
			br.BULLET_REGIST_regist_type					= REGIST_TYPE_04_KURU_KURU;
			/* CCW�̏ꍇ */
			unsigned int ii;
		//	for (ii=0; ii<((4+difficulty)<<3); ii++)
		//	for (ii=0; ii<(32<<difficulty); ii++)
			for (ii=0; ii<(32*8); ii++)
			{
				const u32 ra_nd32 = ra_nd();
			//	obj_send1->cx256 	= (src->cx256)+((ra_nd32   )&0xffff)-t256(128);/*�Ƃ肠�������(br.BULLET_REGIST_angle512)*/
				obj_send1->cx256 	= (src->cx256)+((((ra_nd32	)&0xffff)-t256(128))>>(3-difficulty));/*�Ƃ肠�������(br.BULLET_REGIST_angle512)*/
				obj_send1->cy256 	= (src->cy256)+((ra_nd32<<8)&0x3fff)-t256(32);/*�Ƃ肠�������(br.BULLET_REGIST_angle512)*/
//				br.BULLET_REGIST_angle512	= cv512r(/*360+*/90-30)+(b_wait2_high)*cv512r(20);/*�p�~*/
//				br.BULLET_REGIST_angle512	= cv512r(/*360+*/90-30)+(b_wait2_high)*cv512r(40);/*�p�~*/
//				br.BULLET_REGIST_angle512	= cv512r(/*360+*/90-30)+(b_wait2_high)*cv512r(30);/*�p�~*/
//
			//	br.BULLET_REGIST_speed256	= (b_wait2_high<<7/*8*/)+t256(2.0)-(ii<<5);//t256(2.0/*4.0*/) t256(1.5/*3.0*/) t256(1.0/*2.0*/)
				br.BULLET_REGIST_speed256	= t256(2.0)-(ii);//t256(2.0/*4.0*/) t256(1.5/*3.0*/) t256(1.0/*2.0*/)
//				br.BU LLET_REGIST_sakuya_kurukurku_knife_height = t256(32)+(ii<<4);//t256(40) t256(60) t256(80);
				bullet_regist_vector();
			}
		}
	}
}
			#if 0
			/* CW�̏ꍇ */
				br.BULLET_REGIST_angle512 = (deg_360_to_512((180-30))+((b_wait2_high)*deg_360_to_512(40)));
				br.BULLET_REGIST_angle512 = (deg_360_to_512((180-30))+((b_wait2_high)*deg_360_to_512(30)));
			if (1<difficulty)
			{
				br.BULLET_REGIST_angle512 = (deg_360_to_512((180-30))+((b_wait2_high)*deg_360_to_512(20)));
			}
			#endif
global void sakuya_03_keitai(SPRITE *src)
{
	switch (src->boss_base_state777)
	{
	case SC00:
	//	if (src->cx256 < (SAKUYA_LIMIT_X_LEFT)/*5*/ )	/* �����ɗ����� */	/* �� || (src->y<5)������Ɩ������[�v�ɂȂ�*/
		{
			src->boss_base_state777++;
			src->target_x256 = SAKUYA_POINT_X_LEFT_OUT;/*SAKUYA_POINT_00_LEFT*/
			src->vvv256 = t256(1.00);
		}
		break;
	case SC01:
		if (/*150*/200 > src->vvv256 )	/* ���Ԃ� */	/* �����ɗ����� */
		{
			src->boss_base_state777++;
			src->target_x256 = SAKUYA_POINT_X_RIGHT_OUT;/*SAKUYA_POINT_03_RIGHT*/
			src->vvv256 = t256(1.00);
			src->wait2_256	= t256(0.0);/*(sakuya_shot_111�p)�e�������*/
		}
		break;
	case SC02:
		if (/*150*/200 > src->vvv256 )	/* ���Ԃ� */	/* �E�[�ɗ����� */
		{
			src->boss_base_state777++;
			src->target_x256 = SAKUYA_POINT_X_MID;/*SAKUYA_POINT_01_MID_UP*/
			src->vvv256 = t256(1.00);
		}
		else
		{
			sakuya_shot_111(src);
		}
		break;
	case SC03:
		if (/*150*/200 > src->vvv256 )	/* ���Ԃ� */	/* �^�񒆂ɗ����� */
		{
			src->boss_base_state777++;
			src->target_x256 = SAKUYA_POINT_X_RIGHT_OUT;/*SAKUYA_POINT_03_RIGHT*/
			src->vvv256 = t256(1.00);
		}
		break;
	case SC04:
		if (/*150*/200 > src->vvv256 )	/* ���Ԃ� */	/* �E�[�ɗ����� */
		{
			src->boss_base_state777++;
			src->target_x256 = SAKUYA_POINT_X_LEFT_OUT;/*SAKUYA_POINT_00_LEFT*/
			src->vvv256 = t256(1.00);
	//		src->wait1		= (10);/*???�p�~*/
			src->wait2_256	= t256(0.0);/*(sakuya_shot_222�p)�e�������*/
		}
		break;
	case SC05:
		if (/*150*/200 > src->vvv256 )	/* ���Ԃ� */	/* ���[�ɗ����� */
		{
			src->boss_base_state777++;
			src->target_x256 = SAKUYA_POINT_X_MID;/*SAKUYA_POINT_01_MID_UP*/
			src->vvv256 = t256(1.00);
		}
		else
		{
			sakuya_shot_222(src);
		}
		break;
	case SC06:
		if (/*150*/200 > src->vvv256 )	/* ���Ԃ� */	/* �^�񒆂ɗ����� */
		{
			src->boss_base_state777++;
			src->vvv256 = t256(1.00);
//			src->wait2_256	= t256(60.0);
		}
		break;
	case SC07:
//		src->wait2_256 -= t256(1.0);
//		if (src->wait2_256 < t256(0.0))
		if (/*150*/200 > src->vvv256 )	/* ���Ԃ� */
		{
		//	src->boss_base_state777++/* = SC00*/;
			src->boss_base_state777 = SC00;
		}
		else
		{
			sakuya_shot_333(src);
		}
		break;
	}
	#if 1
	alice_yuudou_move_only(src);
	alice_yuudou_calc(src);
	#endif
	bullet_angle_all_gamen_gai_nara_kesu();/*�p�x�e�̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�)*/
}


/*---------------------------------------------------------
	SPELL_CARD_02_sakuya_bbb
	���`��: ��p�u�~�X�f�B���N�V�����v(�S���ʁA���܂�)
---------------------------------------------------------*/

/* ���A���`�Ԃł͎g���Ė����݂��������痘�p���� */
#define sayuu_mode wait2_256
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
*/
global void sakuya_02_keitai(SPRITE *src)
{
//	src->boss_base_state777++;
//	src->boss_base_state777 &= (256-1);
	/* �e���������ĂȂ��ꍇ�ɒǉ� */
	if (DANMAKU_00==src->boss_base_danmaku_type)		/* �e�������I���Ȃ�e������ */
	{
		spell_card_get_danmaku_number(src); 	/* �X�y�J�V�X�e������A���݂̌`�ԂŌ��e���ԍ�����ɓ���A�W����boss_base_danmaku_time_out���Ԃ�ݒ�B */
	//
		src->target_x256 = src->cx256 = t256(GAME_WIDTH/2); /* �^���փ��[�v */
		src->vvv256 = t256(1.00);
	}
	if ((256-64) == src->boss_base_danmaku_time_out) /* ���]�ړ������Z�b�g */
	{
		{
			{
				src->sayuu_mode ^= (2-1);/* ���] */
				if (/*0==*/src->sayuu_mode & (2-1))
						{	src->target_x256 = SAKUYA_POINT_X_MID-(t256(128));	}//96==128*0.75
				else	{	src->target_x256 = SAKUYA_POINT_X_MID+(t256(128));	}//
			}
			src->vvv256 = t256(1.00);
		}
		/* 24�e�����Z�b�g */
		//	enemy_boss06_mamemaki_state(src, SB05, SAKUYA_ANIME_15_HATUDAN_END);
//		src->boss_base_danmaku_type 	= DANMAKU_02_sakuya;	/* 24�e�����Z�b�g */
//		danmaku_set_time_out(src);		/* �e���̎��Ԑ؂��ݒ� */	//	#define DANMAKU_0048_TIME	(48+1)
	}
	else
	if ((256-192) == src->boss_base_danmaku_time_out) /* �^���փ��[�v */
//	if ((256-180) == src->boss_base_danmaku_time_out) /* �^���փ��[�v */
	{
		src->target_x256 = src->cx256 = t256(GAME_WIDTH)-src->cx256;			/* ���Α��փ��[�v */
		src->vvv256 = t256(1.0);
	}

	#if 1
	alice_yuudou_move_only(src);
	alice_yuudou_calc(src);
	#endif
	/*�p�x�e�̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�)*/
	if (RANK_NORMAL < difficulty)/* hard or lunatic */
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
//boss_base_danmaku_time_out
//
//	if (58/*264-206*/ > src->boss_base_state777)
//	if (58/*264-206*/ == src->boss_base_state777)
//	if (0 == src->boss_base_state777)
//	{
//		/* 11�e�����Z�b�g */
//	//	src->boss_base_danmaku_type 	= DANMAKU_03_sakuya;		/* 11�e�����Z�b�g */
//		src->boss_base_danmaku_type 	= DANMAKU_1c_sakuya;		/* ��p�u�~�X�f�B���N�V�����v���Z�b�g */
//		danmaku_set_time_out(src);		/* �e���̎��Ԑ؂��ݒ� */	//#define DANMAKU_0064_TIME (64+1)
//	}
//	else
//	if (64 == src->boss_base_state777) /* �^���փ��[�v */
//	if (207-206 > src->boss_base_state777)	/* �^���փ��[�v�A�ړ������Z�b�g */
//	if (1 == src->boss_base_state777)		/* �^���փ��[�v�A�ړ������Z�b�g */
//	if (192 == src->boss_base_state777) 	/* �^���փ��[�v�A�ړ������Z�b�g */


/*---------------------------------------------------------
	SPELL_CARD_01_sakuya_aaa
	���`��: �ʏ�U��(���E�ɓ������S�e����)
	�ʏ�U��(�S�e���A����Ƃ͂����Ď኱�Ⴄ)
---------------------------------------------------------*/


/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/
#if 1
global void boss_init_sakuya(SPRITE *h)
{
	h->callback_loser				= sakuya_put_items;

//	h->wait1						= (0);/*����sakuya_02_keitai()�ŏ��������邩�炽�Ԃ�s�v*/
//	h->wait2_256					= t256(0.0);/*���� sakuya_02_keitai()�Ńr�b�g���g���Asakuya_03_keitai()�ŏ��������邩�炽�Ԃ�s�v*/
//	h->move_angle1024				= (0);/*����sakuya_09_keitai()�ŏ��������邩��s�v*/
	//-------------------------
	obj_send1->type 				= SP_DUMMY_MUTEKI;
	obj_send1->cx256 				= (0);
	obj_send1->cy256 				= (0);
}
#endif
//	h->callback_mover				= move_alice;

//	obj_send1						= sprite_add_res(BASE_BOSS_SAKUYA_PNG);
//	obj_send1->flags				&= (~(SP_FLAG_VISIBLE));
//	obj_send1->callback_mover		= dummy_move;
