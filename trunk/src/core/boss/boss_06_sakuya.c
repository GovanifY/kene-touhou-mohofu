
#include "bullet_object.h"
/*static*/extern void add_zako_sakuya_doll_01_laser(SPRITE *src);/* ���[�U�[ */
/*static*/extern void add_zako_sakuya_doll_02_star(SPRITE *src);/* �q�������w(���^��`��) */
/*static*/extern void add_zako_sakuya_doll_03_360knife(SPRITE *src);/* �q�������w(360�i�C�t������) */

/*static*/extern void add_zako_mima_dolls(SPRITE *src);/* ���� �I�v�V���� ��*/
/*---------------------------------------------------------
	�\�Z�� ���
	Izayoi Sakuya.
	-------------------------------------------------------
	PAD
	ToDo:
	�ړ��ƒe���𕪗����悤�B�łȂ��Ɩ�킩���B
	�{�X�^�C�}�[�Ή���
	-------------------------------------------------------
	���{�X�^�C�}�[�̓V�X�e���ŃT�|�[�g�\��ł��B(��ƒ�)
	���e���̓V�X�e���ŃT�|�[�g����\�z������܂��B(�\�z��)
	�V�X�e���ŃT�|�[�g�����ƁA�ǂ̃{�X���A�ǂ̒e���ł����Ă�悤�ɂȂ�܂��B
---------------------------------------------------------*/

//	int st ate1;							/* �s�� */
//	int move_type;		/*	�ړ��^�C�v */	/* �`�� */
//
#define wait1				user_data00 	/* ���̊p�x */
#define wait2_256			user_data01 	/* ���̊p�x */
//#define wa it3			user_data02 	/* ���̊p�x */
#define move_angle512		user_data03 	/* ���̊p�x */

#define move_vx256			move_angle512


//	int length_s_p256;	/* ���ƃv���C���[�Ƃ̋��� */

//
static unsigned int alice_anime_count;

/*---------------------------------------------------------

---------------------------------------------------------*/

static int vvv256;
static void alice_animation(SPRITE *src)
{
	vvv256++;	/*vvv256+=vvv256*/
	if (vvv256> t256(1.0) )
	{	vvv256= t256(1.0);}
//
	if (alice_anime_count)	/* �U���A�j���[�V���� */
	{
		alice_anime_count--;
		if ((32)>alice_anime_count) 	{	src->type = TEKI_09_BOSS32;}	/*src->an im_frame = 0x23;*/
		else
		if ((40)>alice_anime_count) 	{	src->type = TEKI_10_BOSS33;}	/*src->an im_frame = 0x22;*/
		else
										{	src->type = TEKI_09_BOSS32;}	/*src->an im_frame = 0x21;*/
	}
	else	/* �ړ��A�j���[�V���� */
	{
		int aaa;
		aaa = (src->vx256>0)?(TEKI_00_BOSS11/*0x00*/):(TEKI_04_BOSS21/*0x10*/); /*���E*/
		if ((16)>vvv256)		{	aaa+=2;}	/* 25.6==t256(0.1)*/
		else
		if ((200)>vvv256)		{	aaa+=3;}	/* 76.8==t256(0.3)*/
		else
		if ((224)>vvv256)		{	aaa+=2;}	/*128.0==t256(0.5)*/
		else
		if ((240)>vvv256)		{	aaa+=1;}	/*179.2==t256(0.7)*/
	//	else					{	aaa+=0;}
		src->type = aaa;
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/
/*
	SAKUYA_ANIME_xx_dummy
	�͒������p�^�[���Ȃ̂ŁA
	�v���O�����Ŏw�肵�Ȃ��B
	(�w�肷��ƈӖ�������Ȃ��Ȃ邩��w�肵�Ȃ��B)
*/
//enum
//{
//	SAKUYA_ANIME_00_LEFT = 0,
//	SAKUYA_ANIME_01_dummy,
//	SAKUYA_ANIME_02_dummy,							//	SAKUYA_ANIME_03_,
//	SAKUYA_ANIME_03_CENTER_A,
//
//	SAKUYA_ANIME_04_CENTER_B,
//	SAKUYA_ANIME_05_dummy,
//	SAKUYA_ANIME_06_dummy,							//	SAKUYA_ANIME_08_,
//	SAKUYA_ANIME_07_RIGHT,
//
//	SAKUYA_ANIME_08_HIKARU_START,	/*= 0x10*/		//	SAKUYA_ANIME_10_,/*0x11*/
//	SAKUYA_ANIME_09_dummy,			/*0x12*/		//	SAKUYA_ANIME_12_,/*0x13*/
//	SAKUYA_ANIME_10_dummy,			/*0x14*/
//	SAKUYA_ANIME_11_HIKARU_END, 	/*0x15*/
//
//	SAKUYA_ANIME_12_HATUDAN_START,	/*0x16*/
//	SAKUYA_ANIME_13_dummy,			/*0x17*/
//	SAKUYA_ANIME_14_dummy,			/*0x18*/
//	SAKUYA_ANIME_15_HATUDAN_END,	/*0x19*/
//};

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
	SE00_ss = 0,
	SE00,
	SE01,
	SE02,
};


enum
{
	SD00 = 0,
	SD01,
	SD02,
	SD03,
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
	SC08,
	SC09,
};
enum
{
	SB00 = 0,
	SB01,
	SB02,
	SB03,
	SB04,
	SB05,
	SB06,
	SB07,
	SB08,
	SB09,
};
enum
{
	SA00 = 0,
	SA01,
	SA02,
	SA03,
	SA04,
	SA05,
	SA06,
	SA07,
	SA08,
	SA09,
};

/*--------------------------------------*/
/*--------------------------------------*/
/*--------------------------------------*/


/*---------------------------------------------------------
	[�X�y�J�V�X�e�����Ɉړ��\��]	�X�y�J���j��A�C�e���o��
---------------------------------------------------------*/

static void sakuya_put_items(SPRITE *src)
{
	common_boss_put_items(src);
//
	src->boss_base_state001 	= 0;/* ���ʂ��� �l�� 0 �ɂ��� */
	src->wait1		= 80;
	src->wait2_256	= t256(-100);
//	src->wa it3 	= (0);//???

}


/*---------------------------------------------------------

---------------------------------------------------------*/

static void sakuya_wait_state(SPRITE *src)
{
	src->wait1--;/*fps_factor*/
	if (0 > src->wait1)
	{
		src->boss_base_state001++/* = nextstate*/;
	}
}


/*---------------------------------------------------------
	�e�����I���܂ő҂B
	-------------------------------------------------------

---------------------------------------------------------*/

static void danmaku_state_check_holding(SPRITE *src)	/*, int nextstate*/ /*, int an im_frame*/
{
	if (DANMAKU_00 == src->boss_base_danmaku_type)
	{
		src->boss_base_state001++/* = nextstate*/;
	}
}



/*---------------------------------------------------------

---------------------------------------------------------*/

#if 1
/* �v���C���[�̎���ɎU��΂�A���Ԃ��o�ƃv���C���[�_���e */
static void enemy_boss06_knifes3(SPRITE *src)	/*,*/ /*dou ble speed,*/	/*int length222*/
{
	int len256;
	int tmp1_256;
	int tmp2_256;
	int tmp888;
	tmp888=0;
	/* ������x�����ɏo�Ȃ��ƔF�����Â炢 */
	/* �L����(�R�A)���ł����Ɣ�����O�Ɏ��ʁB(�i�C�t���L����O�ɂ�����) */
	len256			= t256(/*70*/80) -((difficulty)<<8) /*-length222*/	/*/2*/;
	src->wait2_256	= t256(0);
	while (1)
	{
		tmp888 = (((int)ra_nd()+(t256_floor(src->wait2_256)) )&(8-1)/*%8*/);
		src->wait2_256	+= t256(1);
		len256			+= t256(0.1);
	/* CCW�̏ꍇ */
		#if 0/*SDL(�������W)*/
		tmp1_256 = player->x256 + ((player->w128-src->w128)) + ((sin512((tmp888<<6))*len256)>>8);
		tmp2_256 = player->y256 + ((player->h128-src->h128)) + ((cos512((tmp888<<6))*len256)>>8);
		#else/*Gu(���S���W)*/
		tmp1_256 = player->x256 + ((sin512((tmp888<<6))*len256)>>8);
		tmp2_256 = player->y256 + ((cos512((tmp888<<6))*len256)>>8);
		#endif
		if ((( t256(30) < tmp1_256) && (tmp1_256 < t256(GAME_WIDTH-30)))&&
			(( t256(30) < tmp2_256) && (tmp2_256 < t256(GAME_HEIGHT-72))))
		{	break;	}
	}
	int tmp_x256;
	int tmp_y256;
	#if 0/*SDL(�������W)*/
	tmp_x256 = player->x256 + ((player->w128));
	tmp_y256 = player->y256 + ((player->h128));
	#else/*Gu(���S���W)*/
	tmp_x256 = player->x256;
	tmp_y256 = player->y256;
	#endif
	int ii;
	for (ii=0; ii<8; ii++)
	{
		if (tmp888==ii)
		{
			src->tmp_angleCCW512	= (ii<<6);
			src->x256				= tmp1_256;
			src->y256				= tmp2_256;
//			src->color32			= 0x00ffffff;			/*	src->alpha			= 0x00;*/
			src->color32			= 0x80ffffff;			/*	src->alpha			= 0x00;*/	/* ������ */
//			//	[SAKUYA_ANIME_15_HATUDAN_END;//?????
			#if 0/*???*/
			/* �X�y���J�[�h�V�X�e���ł͑Ή��ł��Ȃ��B */
			sp ell_card_number	= SPELL_CARD_10_sakuya_jjj/*5*/;/*???*/
			#endif
			src->wait2_256	= /*speed256*/t256(1)+(difficulty<<8);
		}
		else
		{
		//	dou ble tmp_s_x;
		//	dou ble tmp_s_y;
		//	tmp_s_x = s->x;
		//	tmp_s_y = s->y;
			/* �v���C���[�_���e */
			#if 1
			/* CCW�̏ꍇ */
			send1_obj->x256 	=	src->x256	=	tmp_x256 + ((sin512((ii<<6))*len256)>>8);
			send1_obj->y256 	=	src->y256	=	tmp_y256 + ((cos512((ii<<6))*len256)>>8);
		//	send1_obj->w128 	=	src->w128;/*�e*/
		//	send1_obj->h128 	=	src->h128;/*�e*/
			#endif
		//	/*src,*/
			send1_obj->BULLET_REGIST_speed256 = 						/*speed256*/	t256(1.0/*2.0*/);
			send1_obj->BULLET_REGIST_angle512 = 						(ii<<6)/*i*M_PI*2*1/8*/;			/*angle512*/
			send1_obj->BULLET_REGIST_sakuya_kurukurku_knife_height =	-30+(ii&(2-1)/*%2*/)*60;			/* -30 or 30 */
			bullet_create_sakuya_kurukuru_knife/*2*/();
		//	s->x = tmp_s_x;
		//	s->y = tmp_s_y;
		}
	}
}
#endif


/*---------------------------------------------------------
	�����Ȃ񂾂�������Ȃ��Ȃ�̂ŁA���O�����悤
---------------------------------------------------------*/

#define SAKUYA_LIMIT_MIN_Y (t256(8))
#define SAKUYA_LIMIT_MAX_Y (t256(GAME_HEIGHT-32))

/* ��邳��̗U���|�C���g���W */

#define SAKUYA_POINT_X_LEFT_OUT 	(t256(GAME_WIDTH*0/4))
#define SAKUYA_POINT_X_LEFT_MID 	(t256(GAME_WIDTH*1/4))
#define SAKUYA_POINT_X_MID			(t256(GAME_WIDTH*2/4))/*-src->w/2*/
#define SAKUYA_POINT_X_RIGHT_MID	(t256(GAME_WIDTH*3/4))
#define SAKUYA_POINT_X_RIGHT_OUT	(t256(GAME_WIDTH*4/4))/*-src->w  */
#define SAKUYA_POINT_X_MARGIN		(t256(16))

#define SAKUYA_POINT_Y_LEFT 		(t256(50))
#define SAKUYA_POINT_Y_MID			(t256(10))
#define SAKUYA_POINT_Y_RIGHT		(t256(50))
#define SAKUYA_POINT_Y_CENTER		(t256(50))
#define SAKUYA_POINT_Y_MARGIN		(t256(16))

#define SAKUYA_LIMIT_X_LEFT 		(SAKUYA_POINT_X_LEFT_OUT +SAKUYA_POINT_X_MARGIN)
#define SAKUYA_LIMIT_X_MID_LEFT 	(SAKUYA_POINT_X_MID  +SAKUYA_POINT_X_MARGIN)
#define SAKUYA_LIMIT_X_MID_RIGHT	(SAKUYA_POINT_X_MID  -SAKUYA_POINT_X_MARGIN)
#define SAKUYA_LIMIT_X_RIGHT		(SAKUYA_POINT_X_RIGHT_OUT-SAKUYA_POINT_X_MARGIN)

#define atan_512_SAKUYA_LEFT		atan_512( (SAKUYA_POINT_Y_LEFT) 	-src->y256, (SAKUYA_POINT_X_LEFT_OUT) -src->x256)
#define atan_512_SAKUYA_MID 		atan_512( (SAKUYA_POINT_Y_MID)		-src->y256, (SAKUYA_POINT_X_MID)	  -src->x256)
#define atan_512_SAKUYA_RIGHT		atan_512( (SAKUYA_POINT_Y_RIGHT)	-src->y256, (SAKUYA_POINT_X_RIGHT_OUT)-src->x256)
#define atan_512_SAKUYA_CENTER		atan_512( (SAKUYA_POINT_Y_CENTER)	-src->y256, (SAKUYA_POINT_X_MID)	  -src->x256)

/* ��邳��̗U���|�C���g���� */
enum
{
	SAKUYA_POINT_00_LEFT=0, 	/*����*/
	SAKUYA_POINT_01_MID_UP, 	/*����*/
//	SAKUYA_POINT_02_CENTER, 	/*��ʒ��S*/
	SAKUYA_POINT_03_RIGHT,		/*�E��*/
};


/*---------------------------------------------------------
	move_sakuya�Ƃ������O�͊��Ɏg���Ă�̂Œ���
---------------------------------------------------------*/

static void sakuya_idou_sub(SPRITE *src, int speed256, int target_point_name)
{
	int b_move_angle512;
	switch (target_point_name)
	{
	case SAKUYA_POINT_00_LEFT:		b_move_angle512 = atan_512_SAKUYA_LEFT;/*50*/	break;
	case SAKUYA_POINT_01_MID_UP:	b_move_angle512 = atan_512_SAKUYA_MID;			break;
//	case SAKUYA_POINT_02_CENTER:	b_move_angle512 = atan_512_SAKUYA_CENTER;		break;
	case SAKUYA_POINT_03_RIGHT: 	b_move_angle512 = atan_512_SAKUYA_RIGHT;/**/	break;
	}
	src->x256 += ((sin512((b_move_angle512))*(speed256))>>8);/*fps_factor*/ 	/* CCW�̏ꍇ */
	src->y256 += ((cos512((b_move_angle512))*(speed256))>>8);/*fps_factor*/
	//
		 if (src->y256 < (SAKUYA_LIMIT_MIN_Y) ) {	src->y256 += t256(1.0); 	}/* �͂ݏo������C�� */
	else if (src->y256 > (SAKUYA_LIMIT_MAX_Y) ) {	src->y256 -= t256(1.0); 	}/* �͂ݏo������C�� */
}




	/*	256 == 0x100 == ( 4 x 64). [ 4�b���炢] */
	/*	512 == 0x200 == ( 8 x 64). [ 8�b���炢] */
	/*	768 == 0x300 == (12 x 64). [12�b���炢] */
	/* 1024 == 0x400 == (16 x 64). [16�b���炢] */


/*---------------------------------------------------------
	SPELL_CARD_11_sakuya_kkk	��10�`��: �ŏI�`��(����3)
---------------------------------------------------------*/

/*static*/ void sakuya_11_keitai(SPRITE *src)
{
	{
		static int seisei_count;
		if(0==seisei_count)
		{
			add_zako_sakuya_doll_01_laser(src);/* �q�������w�ǉ� */
		}
		seisei_count++;
		seisei_count &= ((0x00ff) );	/* 256 == 0x100 == ( 4 x 64). [ 4�b���炢] */
	}
//
	src->color32 = 0xffffffff;	/*	src->alpha = 0xff;*/
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
	SPELL_CARD_10_sakuya_jjj	��10�`��: �ŏI�`��(����2)
---------------------------------------------------------*/

/*static*/ void sakuya_10_keitai(SPRITE *src)
{
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
		spell_card_number++;
		src->boss_base_state001 = SG02;
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
		src->x256 += ((sin512((src->tmp_angleCCW512))*(src->wait2_256))>>8);/*fps_factor*/		/* CCW�̏ꍇ */
		src->y256 += ((cos512((src->tmp_angleCCW512))*(src->wait2_256))>>8);/*fps_factor*/
	}
	//	[SAKUYA_ANIME_12_HATUDAN_START)
}

/*---------------------------------------------------------
	SPELL_CARD_09_sakuya_iii	���`��: �ŏI�`��(����1)
---------------------------------------------------------*/
#if 0
static int sakuya_kyori(SPRITE *src)
{
	int rrr256;
	//	���������߂�̂ɂQ�悵�ă��[�g�����̂̓��[�g���Z���x���̂ŁA�H�v���K�v�����B
	//	�\�߃e�[�u�������Ă������A��։��Z(CODIC)�ŋ��߂邩�A���ʊ֐��ɂ���CPU��float�ϊ����߂��ŏ��ɗ��߂邩�A�ǂꂩ�B
	//	src->length_s_p = sqrt((player->x-src->x)*(player->x-src->x)+(player->y-src->y)*(player->y-src->y));
	{	/* sqrt()�x���̂ŊȈՔŁB�Ƃ����������ɂ͍S��Ȃ����ɂ��� */
		int xxx256;
		int yyy256;
		xxx256 = abs((int)player->x256-(int)src->x256);/*xxx256+=t256(10);*/ /* abs()�͓W�J���Ă���邩����v */
		yyy256 = abs((int)player->y256-(int)src->y256);/*yyy256+=t256(10);*/
		if (xxx256>yyy256)	{rrr256 = (xxx256/*+(yyy256>>1)*/);}/*�����������*/
		else				{rrr256 = (yyy256/*+(xxx256>>1)*/);}/*�����������*/
	}
	if (t256(40.0) > rrr256)	{	rrr256 = t256(40.0);}	/* 32 10 */
	else
	if (t256(128.0) < rrr256)	{	rrr256 = t256(128.0);}	/* 128 80 */
	return (rrr256);
}
#endif
/*static*/ void sakuya_09_keitai(SPRITE *src)
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
	switch (src->boss_base_state001)
	{
	case SG00:
			src->wait1--;/*fps_factor*/
		if (src->wait1<0)	/* �ҋ@���Ԃ��I������� */
		{
			src->boss_base_state001++/* = SG01*/;
//++		pd->bo ssmode=B00_NONE/*B01_BA TTLE*/;
			#if 0/*SDL(�������W)*/
			src->move_angle512=atan_512(t256(30)-(src->y256), t256(GAME_WIDTH/2)-((src->w128))-(src->x256));
			#else/*Gu(���S���W)*/
			src->move_angle512=atan_512(t256(30)-(src->y256), t256(GAME_WIDTH/2)-(src->x256));
			#endif
		}
	//	else		{		}
		break;
	case SG01:
		if ( (src->x256 < t256(GAME_WIDTH/2+30) ) &&
			 (src->x256 > t256(GAME_WIDTH/2-10) ) &&
			 (src->y256 < t256(50.0)) ) /* �^�񒆂ɗ����� */
		{
			src->boss_base_state001++/* = SG02*/;
			src->wait1		= (20);
			src->wait2_256	= t256(0.0);
			//	[SAKUYA_ANIME_03_CENTER_A;
		}
		else
		{
			src->x256 += ((sin512((src->move_angle512))*t256(3.0))>>8);/*fps_factor*/		/* CCW�̏ꍇ */
			src->y256 += ((cos512((src->move_angle512))*t256(3.0))>>8);/*fps_factor*/
		}
		break;
	case SG02:
		{
				src->wait1--;/*fps_factor*/
			if (src->wait1<0)			{	src->boss_base_state001++/* = SG03*/;
											//	[SAKUYA_ANIME_12_HATUDAN_START;
			}		/* �ҋ@���Ԃ��I������� */
		//	else if (src->wait1<6)	{	src->anxxime_frame=SAKUYA_ANIME_11_HIKARU_END;			}
		//	else if (src->wait1<12) {	src->anxxime_frame=SAKUYA_ANIME_13_;		}
		//	else if (src->wait1<18) {	src->anxxime_frame=SAKUYA_ANIME_12_;		}
		//	else if (src->wait1<24) {	src->anxxime_frame=SAKUYA_ANIME_11_;		}
		//	else if (src->wait1<30) {	src->anxxime_frame=SAKUYA_ANIME_10_;		}
		//	else if (src->wait1<36) {	src->anxxime_frame=SAKUYA_ANIME_08_HIKARU_START;		}
//			else if (src->wait1<36) {	src->anxxime_frame=aaa_tbl[src->wait1];	}
	//		else if (src->wait1>56) {	src->anxxime_frame=SAKUYA_ANIME_08_HIKARU_START;	}
//			else if (src->wait1<32) {	src->anxxime_frame=SAKUYA_ANIME_08_HIKARU_START+((src->wait1&0x38)>>3);	}
		}
		break;
	case SG03:
		//	[SAKUYA_ANIME_15_HATUDAN_END)
		{
			{
			//	int rrr256;
			//	/*src->length_s_p256*/rrr256 = sakuya_kyori(src);
		//		bullet_create_sakuya_even_knife(src, t256(10), /*src->length_s_p256*/ /*rrr256,*/ 0/*0==�E*/);
		//		bullet_create_sakuya_even_knife(src, t256(10), /*src->length_s_p256*/ /*rrr256,*/ 1/*1==��*/);
				bullet_create_sakuya_ryoute_knife(src);
			}
			src->wait1=40;
			if (src->y256 > t256(150))
			/* CW�̏ꍇ */
		//	{	src->move_angle512=deg_360_to_512(210) + (((ra_nd()&(2048-1))*21)>>8) /*(ra_nd()%(deg_360_to_512(120)))*/;}
			/* CCW�̏ꍇ */
			{	src->move_angle512=deg_360_to_512CCW(360-210) + (((ra_nd()&(2048-1))*21)>>8) /*(ra_nd()%(deg_360_to_512(120)))*/;}
			else
			{	src->move_angle512=/*deg512_2rad*/((ra_nd()&(512-1)))/*degtorad(ra_nd()%360)*/;}
			#if 1
			/*�O�̈�*/
			mask512(src->move_angle512);
			#endif
			//	[(SAKUYA_ANIME_12_HATUDAN_START+1);/*16???*/
			src->boss_base_state001++/* = SG04*/;
		}
		break;
	case SG04:
			src->wait1--;/*fps_factor*/
		if (src->wait1<0)
		{
			src->wait1	= 120+(3-difficulty)*50;
			src->boss_base_state001 = SG02;
		}
		else
		{
			/* CCW�̏ꍇ */
			src->x256 += ((sin512((src->move_angle512))*t256(3.5))>>8);/*fps_factor*/
			src->y256 += ((cos512((src->move_angle512))*t256(3.5))>>8);/*fps_factor*/
		}
		break;
	}
	#if 0/*SDL(�������W)*/
	if ((player->x256+((player->w128)) < src->x256+((src->w128))+t256(25))&&
		(player->x256+((player->w128)) > src->x256+((src->w128))-t256(25)))
	#else/*Gu(���S���W)*/
	if ((player->x256 < src->x256 +t256(25))&&
		(player->x256 > src->x256 -t256(25)))
	#endif
	{	src->wait2_256 += t256(1.0);/*fps_factor*/}
	if (src->wait2_256 > t256(20.0)+((3-difficulty)*t256(30)))
	{
		//	[(SAKUYA_ANIME_12_HATUDAN_START+1);/*16???*/
	//	src->length_s_p256 = sakuya_kyori(src);
		enemy_boss06_knifes3(src/*,*/ /*1+difficulty,*/ /*(src->length_s_p256)*/ );
	}
}
		//	static const u8 aaa_tbl[40/*(6*6)*/] =
		//	{
		//		SAKUYA_ANIME_11_HIKARU_END, SAKUYA_ANIME_11_HIKARU_END, SAKUYA_ANIME_11_HIKARU_END, SAKUYA_ANIME_11_HIKARU_END, SAKUYA_ANIME_11_HIKARU_END, SAKUYA_ANIME_11_HIKARU_END,
		//		SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,
		//		SAKUYA_ANIME_12_,	SAKUYA_ANIME_12_,	SAKUYA_ANIME_12_,	SAKUYA_ANIME_12_,	SAKUYA_ANIME_12_,	SAKUYA_ANIME_12_,
		//		SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,
		//		SAKUYA_ANIME_10_,	SAKUYA_ANIME_10_,	SAKUYA_ANIME_10_,	SAKUYA_ANIME_10_,	SAKUYA_ANIME_10_,	SAKUYA_ANIME_10_,
		//		SAKUYA_ANIME_08_HIKARU_START,	SAKUYA_ANIME_08_HIKARU_START,	SAKUYA_ANIME_08_HIKARU_START,	SAKUYA_ANIME_08_HIKARU_START,	SAKUYA_ANIME_08_HIKARU_START,	SAKUYA_ANIME_08_HIKARU_START,
		//		0,0,0,0/* �A���C���p�_�~�[�f�[�^�[�B dummy for align*/
		//	};
//			static const u8 aaa_tbl[40/*(6*6)*/] =
//			{
//	SAKUYA_ANIME_11_HIKARU_END, SAKUYA_ANIME_11_HIKARU_END, SAKUYA_ANIME_11_HIKARU_END, SAKUYA_ANIME_11_HIKARU_END,
//	SAKUYA_ANIME_11_HIKARU_END, SAKUYA_ANIME_11_HIKARU_END, SAKUYA_ANIME_11_HIKARU_END, SAKUYA_ANIME_11_HIKARU_END,
//	SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,
//	SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,
//	SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,
//	SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,
//	SAKUYA_ANIME_08_HIKARU_START,	SAKUYA_ANIME_08_HIKARU_START,	SAKUYA_ANIME_08_HIKARU_START,	SAKUYA_ANIME_08_HIKARU_START,
//	SAKUYA_ANIME_08_HIKARU_START,	SAKUYA_ANIME_08_HIKARU_START,	SAKUYA_ANIME_08_HIKARU_START,	SAKUYA_ANIME_08_HIKARU_START,//32
//	SAKUYA_ANIME_08_HIKARU_START,	SAKUYA_ANIME_08_HIKARU_START,	SAKUYA_ANIME_08_HIKARU_START,	SAKUYA_ANIME_08_HIKARU_START,
//	0,0,0,0/* �A���C���p�_�~�[�f�[�^�[�B dummy for align*/
//			};




/*---------------------------------------------------------
	SPELL_CARD_08_sakuya_hhh	�攪�`��: (���~�߃i�C�t)�ǉ��v�撆
	SPELL_CARD_07_sakuya_ggg	�掵�`��: (���U�����w)�ǉ��v�撆
---------------------------------------------------------*/


/*---------------------------------------------------------
	SPELL_CARD_06_sakuya_fff	��Z�`��: (���F�}�X�J�b�g�e)
	SPELL_CARD_05_sakuya_eee	��܌`��: (���F�}�X�J�b�g�e)
---------------------------------------------------------*/
/*static*/extern void sakuya_sp1_bullet_create_bbb(SPRITE *src);

/*static*/ void sakuya_06_keitai(SPRITE *src)
{
	switch (src->boss_base_state001)
	{
	case SE00_ss:
//	{
//		static int seisei_count;
//		if(0==seisei_count)
		{
			add_zako_sakuya_doll_01_laser(src);/* ���[�U�[�ǉ� */
		}
//		seisei_count++;
//		seisei_count &= 0x01ff; 	/* 512 == 0x200 == ( 8 x 64). [ 8�b���炢] */
//	}
		src->boss_base_state001++;
		break;
	case SE00:
		tmp_angle_jikinerai512(player, src);
		src->wait2_256 -= t256(1.0);/*fps_factor*/
		if (src->wait2_256 < t256(0.0))
		{
			src->wait2_256	= t256(50.0);
//			int *s_data_angle512;
		//	s_data_angle512 = &(((BO SS99 _DATA *)src->data)->boss_base.tmp_angleCCW512);
//			s_data_angle512 = &(src->tmp_angleCCW512);
			/* (���F�}�X�J�b�g�e) */

/* CCW�̏ꍇ */
			sakuya_sp1_bullet_create_bbb(src);
			//bullet_create_oodama0(src, t256(2), ANGLE_JIKINERAI_DAN, 40, 0, t256(src->w/2), t256(src->h/2));
			src->wait1--;/*fps_factor*/
			if (src->wait1<0)	/* 3�񌂂����� */
			{
				src->wait1=20;
				if (0==(ra_nd()&(2-1)/*%2*/))
				{
					src->boss_base_state001++/* = SE01*/;
					//	[(SAKUYA_ANIME_03_CENTER_A-2);
				}
				else
				{
					src->boss_base_state001/*+=2*/ = SE02;
					//	[(SAKUYA_ANIME_03_CENTER_A+2);
				}
			}
		}
	//	[SAKUYA_ANIME_08_HIKARU_START)

		break;
	case SE01:
		src->wait1--;/*fps_factor*/
		if (src->wait1<0)
		{
			src->wait1		= (3);
			src->boss_base_state001--/* = SE00*/;
			src->wait2_256	= t256(30.0);
			//	[SAKUYA_ANIME_15_HATUDAN_END;
		}
		else
		{
			if (src->x256 < t256(100) )
			{
				src->boss_base_state001++/* = SE02*/;
				//	[SAKUYA_ANIME_07_RIGHT-1/*SAKUYA_ANIME_07_RIGHT-1*/;/*7 ???*/
			}
			else
			{	src->x256 -= t256(2.0);/*fps_factor*/}
		}
		break;
	case SE02:
		src->wait1--;/*fps_factor*/
		if (src->wait1<0)
		{
			src->wait1		= (3);
			src->boss_base_state001 = SE00;
			src->wait2_256	= t256(30.0);
			//	[SAKUYA_ANIME_15_HATUDAN_END;
		}
		else
		{
			#if 0/*SDL(�������W)*/
			if (src->x256 > t256(GAME_WIDTH-100)-((src->w128+src->w128)) )
			#else/*Gu(���S���W)*/
			if (src->x256 > t256(GAME_WIDTH-100) )
			#endif
			{
				src->boss_base_state001--/* = SE01*/;
				//	[SAKUYA_ANIME_00_LEFT+1;/*1 ???*/
			}
			else
			{	src->x256 += t256(2.0);/*fps_factor*/}
		}
		break;
	}
}


/*---------------------------------------------------------
	��l�`��: ���� �����w����
---------------------------------------------------------*/

/*static*/ void sakuya_04_keitai(SPRITE *src)
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
	switch (src->boss_base_state001)
	{
	case SD00:
			add_zako_mima_dolls(src);/* �q�������w�ǉ� */
		{	/* �߂�? */
			boss_clip_min.x256	= t256( 		0)+t256(24);
			boss_clip_max.x256	= t256(GAME_WIDTH)-t256(24);
		}
		src->boss_base_state001++;
		break;
	case SD01:
		sakuya_wait_state(src);/*�A�j���Z�b�g[SAKUYA_ANIME_12_HATUDAN_START]*/
		break;
	case SD02:
	//	if ( ( src->y > 50) && (src->x<GAME_WIDTH/2+30 || src->x>GAME_WIDTH/2-30))		/* �^�񒆂ɗ����� */
		if (( src->y256 > (SAKUYA_POINT_Y_CENTER-SAKUYA_POINT_Y_MARGIN) ) &&
			( src->x256 < (SAKUYA_LIMIT_X_MID_LEFT)) &&
			( src->x256 > (SAKUYA_LIMIT_X_MID_RIGHT)))		/* �^�񒆂ɗ����� */
		{
			src->boss_base_state001++;
			src->wait1		= 10;
			src->wait2_256	= t256(0.0);
				//	[SAKUYA_ANIME_08_HIKARU_START;
		}
		else
		{
			src->move_angle512=atan_512_SAKUYA_CENTER/*atan_512(50-src->y,GAME_WIDTH/2-src->w-src->x)*/;
			src->x256 += ((sin512((src->move_angle512))*t256(3))>>8);/*fps_factor*/ 		/* CCW�̏ꍇ */
			src->y256 += ((cos512((src->move_angle512))*t256(3))>>8);/*fps_factor*/
			//	[SAKUYA_ANIME_15_HATUDAN_END)
		}
		break;
	case SD03:
		//	[SAKUYA_ANIME_11_HIKARU_END)
		{
			/* �q�������w����(2�񐶐������Ⴄ�o�O����) */
			src->boss_base_state001++/* = SE00*/;/*dummy*/
		//	src->boss_base_state001 	=	0/* = SE00*/;
#if 0/*???*/
/* �X�y���J�[�h�V�X�e���ł͑Ή��ł��Ȃ��B */
			sp ell_card_number++;		/* ���̌`�Ԃ� */
#endif
			src->wait2_256	= t256(10.0);
			src->wait1		= (5);
		}
			//	[SAKUYA_ANIME_11_HIKARU_END)
		break;
	}
}


/*---------------------------------------------------------
	SPELL_CARD_03_sakuya_ccc
	���J�u�o�[�e�B�J���i�C�t�v
	��O�`��: �����i�C�t�������Ă����
---------------------------------------------------------*/

/*static*/ void sakuya_03_keitai(SPRITE *src)
{
	switch (src->boss_base_state001)
	{
	case SC00:
			src->vx256 = t256(0.0); 	/* �ړ���~�w�� */
		{	/* ���̐�����ς��� */
			boss_clip_min.x256 = t256(0);			//t256(50);
			boss_clip_max.x256 = t256(GAME_WIDTH);	//t256(GAME_WIDTH)-t256(50)-((src->w128+src->w128));
		}
		src->boss_base_state001++;
		break;
	case SC01:
		sakuya_wait_state(src);/*�A�j���Z�b�g[SAKUYA_ANIME_03_CENTER_A]*/
		break;
	case SC02:
		if (src->x256 < (SAKUYA_LIMIT_X_LEFT)/*5*/ )	/* �����ɗ����� */	/* �� || (src->y<5)������Ɩ������[�v�ɂȂ�*/
		{
			src->boss_base_state001++;
			src->wait1		= (10);
			src->wait2_256	= t256(0.0);
				//	??? �A�j���Z�b�g[SAKUYA_ANIME_00_LEFT;
		}
		else
		{
			sakuya_idou_sub(src, t256(3.0), SAKUYA_POINT_00_LEFT);
		}
		break;
	case SC03:
		sakuya_wait_state(src);/*�A�j���Z�b�g[(SAKUYA_ANIME_03_CENTER_A-1)]*/ /*???*/
		break;
	case SC04:
		if (src->x256 > (SAKUYA_LIMIT_X_RIGHT)/*GAME_WIDTH-src->w+3*/ ) /* �E�[�ɗ����� */
		{
			src->boss_base_state001++;
			src->wait2_256	= t256(0.0);
				//	??? �A�j���Z�b�g[SAKUYA_ANIME_07_RIGHT;
		}
		else
		{
			{	const short sss[4] =
				{
					t256(0.48),/* 1.2  easy:�܂΂�Ȃ̂ŊȒP*/
					t256(0.75),/*normal:�܂΂�Ȃ̂ŊȒP*/	//	t256(0.48/*0.5*/ /*1.0*/ /*1.5*/),/*normal:��Ȃ̂ő傫��������ΊȒP*/
					t256(1.0/*2.0*/),/*hard:���܂ɓ����������*/
					t256(18.5/*6.5*/ /*12.5*/ /*8.5*/) /*luna:���ւ����̃X�L�}�������ł������H*/
					/*(luna:�{����Ȃ��Ă��C�������łȂ�Ƃ��Ȃ���x�ɓ�Փx�}�������̂����܂����������)*/
				};
				src->wait2_256 += /*t256(4)*/sss[difficulty]/*t256(3*1.42)*/;
			}
			if (0==(((int)src->wait2_256)&(16-1)/*%21 t256(15*1.42)*/))
			{
#if 0
/* CW�̏ꍇ */
				bullet_create_jyuryoku_dan000(src, /*(0.7)*/t256(1.75)/*(2.0)*/, (deg_360_to_512(360)-((t256_floor(src->wait2_256))&(256-1)/*%(180*1.42)*/)), t256(0.03), BULLET_KNIFE20_05_AKA);
				//if (difficulty)
				{
					bullet_create_jyuryoku_dan000(src, /*(0.7)*/t256(1.75)/*(2.0)*/, (deg_360_to_512(270)-((t256_floor(src->wait2_256))&(256-1)/*%(180*1.42)*/)), t256(0.04), BULLET_KNIFE20_05_AKA);
				//	if (1<difficulty)
					{	bullet_create_jyuryoku_dan000(src, /*(0.7)*/t256(1.75)/*(2.0)*/, (deg_360_to_512(180)-((t256_floor(src->wait2_256))&(256-1)/*%(180*1.42)*/)), t256(0.02), BULLET_KNIFE20_05_AKA);}
				}
#else
/* CCW�̏ꍇ */
				bullet_create_jyuryoku_dan000(src, /*(0.7)*/t256(1.75)/*(2.0)*/, (-deg_360_to_512CCW(360)-((t256_floor(src->wait2_256))&(256-1)/*%(180*1.42)*/)), t256(0.03), BULLET_KNIFE20_05_AKA);
				//if (difficulty)
				{
					bullet_create_jyuryoku_dan000(src, /*(0.7)*/t256(1.75)/*(2.0)*/, (-deg_360_to_512CCW(270)-((t256_floor(src->wait2_256))&(256-1)/*%(180*1.42)*/)), t256(0.04), BULLET_KNIFE20_05_AKA);
				//	if (1<difficulty)
					{	bullet_create_jyuryoku_dan000(src, /*(0.7)*/t256(1.75)/*(2.0)*/, (-deg_360_to_512CCW(180)-((t256_floor(src->wait2_256))&(256-1)/*%(180*1.42)*/)), t256(0.02), BULLET_KNIFE20_05_AKA);}
				}
#endif
				voice_play(VOICE12_SAKUYA_B2_SHOT, TRACK04_TEKIDAN);
			}
			sakuya_idou_sub(src, t256(5.0), SAKUYA_POINT_03_RIGHT);		// SAKUYA_ANIME_07_RIGHT)
		}
		break;
	case SC05:
	//	if (( src->x256 < (SAKUYA_LIMIT_X_MID)/*GAME_WIDTH/2-src->w/2*/ ))	/* �^�񒆂ɗ����� */
		if (( src->x256 < (SAKUYA_LIMIT_X_MID_LEFT)) &&
			( src->x256 > (SAKUYA_LIMIT_X_MID_RIGHT)))		/* �^�񒆂ɗ����� */
		{
			src->boss_base_state001++;
				//	??? �A�j���Z�b�g[SAKUYA_ANIME_03_CENTER_A;
		}
		else
		{
			sakuya_idou_sub(src, t256(3.0), SAKUYA_POINT_01_MID_UP);		// (SAKUYA_ANIME_03_CENTER_A )
		}
		break;
	case SC06:
		if (src->x256 > (SAKUYA_LIMIT_X_RIGHT)/*GAME_WIDTH-src->w-3*/ ) /* �E�[�ɗ����� */ /* �� || (src->y<5)������Ɩ������[�v�ɂȂ� */
		{
			src->boss_base_state001++;
			src->wait1		= (10);
			src->wait2_256	= t256(0.0);
				//	??? �A�j���Z�b�g[SAKUYA_ANIME_07_RIGHT;
		}
		else
		{
			sakuya_idou_sub(src, t256(3.0), SAKUYA_POINT_03_RIGHT);		// SAKUYA_ANIME_07_RIGHT)

		}
		break;
	case SC07:
		if (src->x256 < (SAKUYA_LIMIT_X_LEFT)/*5*/) 	/* ���[�ɗ����� */
		{
			src->boss_base_state001++;
			src->wait2_256	= t256(0.0);
				//	??? �A�j���Z�b�g[SAKUYA_ANIME_00_LEFT;
		}
		else
		{
			src->wait2_256 += t256(4.0)/*(3*1.42)*/;
			if (0x0000==((src->wait2_256)&(0x0f00))/*%(12*1.42)*/)
			{
#if 0
/* CW�̏ꍇ */
				bullet_create_jyuryoku_dan000(src, t256(3), (((t256_floor(src->wait2_256))&(256-1)/*%(180*1.42)*/)+deg_360_to_512(180)), t256(0.03), BULLET_KNIFE20_05_AKA);
				if (difficulty)
				{
					bullet_create_jyuryoku_dan000(src, t256(3), (((t256_floor(src->wait2_256))&(256-1)/*%(180*1.42)*/)+deg_360_to_512(90)), t256(0.04), BULLET_KNIFE20_05_AKA);
					if (1<difficulty)
					{	bullet_create_jyuryoku_dan000(src, t256(3), (((t256_floor(src->wait2_256))&(256-1)/*%(180*1.42)*/)), t256(0.02), BULLET_KNIFE20_05_AKA);}
				}
#else
/* CCW�̏ꍇ */
				bullet_create_jyuryoku_dan000(src, t256(3), (((t256_floor(src->wait2_256))&(256-1)/*%(180*1.42)*/)+deg_360_to_512CCW(360-180)), t256(0.03), BULLET_KNIFE20_05_AKA);
				if (difficulty)
				{
					bullet_create_jyuryoku_dan000(src, t256(3), (((t256_floor(src->wait2_256))&(256-1)/*%(180*1.42)*/)+deg_360_to_512CCW(360-90)), t256(0.04), BULLET_KNIFE20_05_AKA);
					if (1<difficulty)
					{	bullet_create_jyuryoku_dan000(src, t256(3), (((t256_floor(src->wait2_256))&(256-1)/*%(180*1.42)*/)), t256(0.02), BULLET_KNIFE20_05_AKA);}
				}
#endif
				voice_play(VOICE12_SAKUYA_B2_SHOT, TRACK04_TEKIDAN);
			}
			sakuya_idou_sub(src, t256(5.0), SAKUYA_POINT_00_LEFT);		// SAKUYA_ANIME_00_LEFT)
		}
		break;
	case SC08:
	//	if (( src->x256 > (SAKUYA_LIMIT_X_MID)/*(GAME_WIDTH/2-src->w/2)*/ ))	/* �^�񒆂ɗ����� */
		if (( src->x256 < (SAKUYA_LIMIT_X_MID_LEFT)) &&
			( src->x256 > (SAKUYA_LIMIT_X_MID_RIGHT)))		/* �^�񒆂ɗ����� */
		{
			src->boss_base_state001++;
			src->wait2_256	= t256(60.0);
				//	??? �A�j���Z�b�g[SAKUYA_ANIME_03_CENTER_A;
		}
		else
		{
			sakuya_idou_sub(src, t256(3.0), SAKUYA_POINT_01_MID_UP);		// (SAKUYA_ANIME_03_CENTER_A )
		}
		break;
	case SC09:
		src->wait2_256 -= t256(1.0);
		if (src->wait2_256 < t256(0.0))
		{
		//	src->boss_base_state001++/* = SC02*/;
			src->boss_base_state001 = SC02;
				//	??? �A�j���Z�b�g[SAKUYA_ANIME_03_CENTER_A;
		}
		else
		{
//			src->move_angle512=atan_512_SAKUYA_MID;
			if (0 == ((t256_floor(src->wait2_256))&(32-1)/*%20*/) )
			{
				int b_wait2_high;
				b_wait2_high=(((int)((t256_floor(src->wait2_256))>>5/*/20*/)));
			//	src->anxxime_frame++; src->anxxime_frame %= 7;//??? 9???
				//	??? �A�j���Z�b�g[		src->anxxime_frame++; src->anxxime_frame &= (8-1);
				{
					send1_obj->x256 	=	(src->x256);
					send1_obj->y256 	=	(src->y256);
//
					#if 0
					/* CW�̏ꍇ */
						send1_obj->BULLET_REGIST_angle512 = (deg_360_to_512((180-30))+((b_wait2_high)*deg_360_to_512(40)));
						send1_obj->BULLET_REGIST_angle512 = (deg_360_to_512((180-30))+((b_wait2_high)*deg_360_to_512(30)));
					if (1<difficulty)
					{
						send1_obj->BULLET_REGIST_angle512 = (deg_360_to_512((180-30))+((b_wait2_high)*deg_360_to_512(20)));
					}
					#endif
					/* CCW�̏ꍇ */
						/*src,*/
						send1_obj->BULLET_REGIST_speed256 = (b_wait2_high<<7/*8*/)+t256(2.0/*4.0*/);
						send1_obj->BULLET_REGIST_angle512 = deg_360_to_512CCW(/*360+*/90-30)+(b_wait2_high)*deg_360_to_512CCW(40);
						send1_obj->BULLET_REGIST_sakuya_kurukurku_knife_height = -80;
						bullet_create_sakuya_kurukuru_knife();
						/*src,*/
						send1_obj->BULLET_REGIST_speed256 = (b_wait2_high<<7/*8*/)+t256(1.5/*3.0*/);
						send1_obj->BULLET_REGIST_angle512 = deg_360_to_512CCW(/*360+*/90-30)+(b_wait2_high)*deg_360_to_512CCW(30);
						send1_obj->BULLET_REGIST_sakuya_kurukurku_knife_height = -60;
						bullet_create_sakuya_kurukuru_knife();
					if (1<difficulty)
					{
						/*src,*/
						send1_obj->BULLET_REGIST_speed256 = (b_wait2_high<<7/*8*/)+t256(1.0/*2.0*/);
						send1_obj->BULLET_REGIST_angle512 = deg_360_to_512CCW(/*360+*/90-30)+(b_wait2_high)*deg_360_to_512CCW(20);
						send1_obj->BULLET_REGIST_sakuya_kurukurku_knife_height = -40;
						bullet_create_sakuya_kurukuru_knife();
					}

				}
				//	??? �A�j���Z�b�g[SAKUYA_ANIME_15_HATUDAN_END;
			}
		}
	// [SAKUYA_ANIME_12_HATUDAN_START)
		break;
	}
}


/*---------------------------------------------------------
	SPELL_CARD_02_sakuya_bbb
	���`��: ��p�u�~�X�f�B���N�V�����v(�S���ʁA���܂�)
---------------------------------------------------------*/

/*static*/ void sakuya_02_keitai(SPRITE *src)
{
	/* ���`�Ԃł͎g���Ė����݂��������痘�p���� */
	#define sayuu_mode wait2_256
	switch (src->boss_base_state001)
	{
	case SB00:	/* �����l */
		src->wait1		= 0;/*�Ƃ肠����*/
		src->sayuu_mode = (ra_nd()&(2-1));
		src->boss_base_state001++;
//		src->vx256 = t256(0.0); 	/* �ړ���~�w�� */
		break;
/*----------------*/
	case SB01: /* �^���փ��[�v�A�ړ������Z�b�g */
		src->x256 = t256(GAME_WIDTH/2); /* �^���փ��[�v */
		{
			src->sayuu_mode ^= (2-1);/* ���] */
			if (0==src->sayuu_mode)
					{	src->vx256 = t256(1.0);/*0.5*/		}
			else	{	src->vx256 = -t256(1.0);/*0.5*/ 	}
		}
		src->wait1		= (35);/* �ړ���==�ҋ@����[WAITSTATE] */	/* �e�����Ȃ��ł�����ƈړ� */
		//	??? �A�j���Z�b�g[SAKUYA_ANIME_03_CENTER_A]
		src->boss_base_state001++;
		break;
	case SB02:	/* �e�����Ȃ��ł�����ƈړ� */
		sakuya_wait_state(src);/*�A�j���Z�b�g[SAKUYA_ANIME_12_HATUDAN_START]*/
		//	??? �A�j���Z�b�g[SAKUYA_ANIME_12_HATUDAN_START] //	/* �e�����A�j���ҋ@ */
		break;
	case SB03:	/* 24�e�����Z�b�g */
		//	enemy_boss06_mamemaki_state(src, SB07, SAKUYA_ANIME_15_HATUDAN_END);
			src->boss_base_danmaku_type 	= DANMAKU_02_sakuya;	/* 24�e�����Z�b�g */
			src->boss_base_danmaku_time_out = DANMAKU_02_SET_TIME;	/* 24�e���̔������� */
			src->boss_base_state001++;
		break;
	case SB04:	/* 24�e�����I���܂Œe�����Ȃ���ړ� */
		danmaku_state_check_holding(src); /*, SAKUYA_ANIME_03_CENTER_A*/
		break;
/*----------------*/
	case SB05:
			src->vx256 = t256(0.0); 	/* �ړ���~�w�� */
			src->x256 = t256(GAME_WIDTH)-src->x256; 			/* ���Α��փ��[�v */
		//	??? �A�j���Z�b�g[SAKUYA_ANIME_12_HATUDAN_START] //	/* �e�����A�j���ҋ@ */
			src->boss_base_state001++;
		break;
	case SB06:
			src->boss_base_danmaku_type 	= DANMAKU_03_sakuya;	/* 11�e�����Z�b�g */
			src->boss_base_danmaku_time_out = DANMAKU_03_SET_TIME;	/* 11�e���̔������� */
			src->wait1			= (35);/* (SB12��)�����ҋ@ */
		//	??? �A�j���Z�b�g[SAKUYA_ANIME_03_CENTER_A]
			src->boss_base_state001++;
		break;
	case SB07:	/* 11�e�����I���܂őҋ@ */
		danmaku_state_check_holding(src); /*, SAKUYA_ANIME_03_CENTER_A*/
		break;
	case SB08:	/* (SB12��)�����ҋ@ */
		sakuya_wait_state(src);/*�A�j���Z�b�g[SAKUYA_ANIME_03_CENTER_A]*/
		break;
	case SB09:
		src->boss_base_state001 = SB01;
		break;
	}
}


/*---------------------------------------------------------
	SPELL_CARD_01_sakuya_aaa
	���`��: �ʏ�U��(���E�ɓ������S�e����)
	�ʏ�U��(�S�e���A����Ƃ͂����Ď኱�Ⴄ)
---------------------------------------------------------*/

/*static*/ void sakuya_01_keitai(SPRITE *src)
{
	static int my_yudo_point;
	switch (src->boss_base_state001)
	{
	/* ���`�Ԃ͉�ʒ��S(��x�ʒu)����n�܂� */
/*----------------*/
	case SA00:/* ���̈ړ��ʒu(��ʍ���)�����߂� */
		src->boss_base_state001++;
		src->vx256 = -t256(2.0/*3.0*/);/*fps_factor*/	/* �������ֈړ��w�� */
	//	my_yudo_point = SAKUYA_POINT_X_LEFT_MID;
		my_yudo_point = SAKUYA_POINT_X_MID-(ra_nd()&((128*256)-1));/* ��ʍ����̓K���Ȉʒu */	/*��:x256==t256(50.0)*/
		break;
	case SA01:	/* ���ֈړ� */
		if (my_yudo_point > src->x256 ) 	/* ��ʍ����̓K���Ȉʒu�֓��B������ */
		{
			src->vx256 = t256(0.0); 	/* �ړ���~�w�� */
			src->boss_base_state001++;
			#if 1/* �ϒe�� */
			tmp_angle_jikinerai512(player, src);/* ���@�˂炢�p�쐬 */
				#if 0
					/*CW*/			src->tmp_angleCCW512	-= 128;
				#else
					/*CCW*/ 		src->tmp_angleCCW512	+= (256-128);
				#endif
			#else/* �Œ�e�� */
				/*CCW*/ 			src->tmp_angleCCW512	= deg_360_to_512CCW((90));/* �E�����猂���n�߂� (128)*/
			#endif
			src->boss_base_danmaku_type 	= DANMAKU_01_sakuya;	/* �S�e�����Z�b�g */
			src->boss_base_danmaku_time_out = DANMAKU_01_SET_TIME;	/* �S�e���̔������� */
			/*[���e�A�j���w��Z�b�g]*/		// [SAKUYA_ANIME_12_HATUDAN_START]
		}
		break;
	case SA02:	/* �S�e�����I���܂őҋ@ */	/* �e�����A�j���ҋ@ */
		danmaku_state_check_holding(src);/*, SA03*/ /* SAKUYA_ANIME_03_CENTER_A */
		break;
	case SA03:
		src->vx256 = t256(3.0/*3.0*/);/*fps_factor*/	/* �^�񒆂ֈړ� */
		src->boss_base_state001++;
		break;
	case SA04:	/* �^�񒆂ɖ߂� */
		if (SAKUYA_POINT_X_MID < src->x256 )	/* �^�񒆂܂ŗ����� */
		{
			src->vx256 = t256(0.0); 	/* �ړ���~�w�� */
			src->boss_base_state001++;
		}
		break;
/*----------------*/
	case SA05:/* ���̈ړ��ʒu(��ʉE��)�����߂� */
			src->boss_base_state001++;
			src->vx256 = t256(2.0/*3.0*/);/*fps_factor*/	/* �E�����ֈړ��w�� */
		//	my_yudo_point = SAKUYA_POINT_X_RIGHT_MID;
			my_yudo_point = SAKUYA_POINT_X_MID+(ra_nd()&((128*256)-1));
		break;
	case SA06:	/* �E�ֈړ� */
		if ( my_yudo_point < src->x256 )	/* ��ʉE���̓K���Ȉʒu�֓��B������ */
		{
			src->vx256 = t256(0.0); 	/* �ړ���~�w�� */
			src->boss_base_state001++;
			#if 1/* �ϒe�� */
			tmp_angle_jikinerai512(player, src);/* ���@�˂炢�p�쐬 */
				#if 0
					/*CW*/			src->tmp_angleCCW512	-= 128;
				#else
					/*CCW*/ 		src->tmp_angleCCW512	+= (256-128);
				#endif
			#else/* �Œ�e�� */
				/*CCW*/ 			src->tmp_angleCCW512	= deg_360_to_512CCW((90));/* �E�����猂���n�߂� (128)*/
			#endif
			src->boss_base_danmaku_type 	= DANMAKU_01_sakuya;	/* �S�e�����Z�b�g */
			src->boss_base_danmaku_time_out = DANMAKU_01_SET_TIME;	/* �S�e���̔������� */
			/*[���e�A�j���w��Z�b�g]*/		// [SAKUYA_ANIME_12_HATUDAN_START]
		}
		break;
	case SA07:	/* �S�e�����I���܂őҋ@ */	/* �e�����A�j���ҋ@ */
		danmaku_state_check_holding(src); /* [SAKUYA_ANIME_03_CENTER_A] */
		break;
	case SA08:
			/*[??? �A�j���w��Z�b�g]*/		// [SAKUYA_ANIME_03_CENTER_A]
		src->vx256 = -t256(3.0/*3.0*/);/*fps_factor*/	/* �^�񒆂ֈړ� */
		src->boss_base_state001++;
		break;
	case SA09:	/* �^�񒆂ɖ߂� */
		if (SAKUYA_POINT_X_MID > src->x256 )	/* �^�񒆂ɖ߂� */
		{
			src->vx256 = t256(0.0); 	/* �ړ���~�w�� */
			src->boss_base_state001 = SA00;
		}
		break;
	}
}


/*---------------------------------------------------------
	�{�X�s��
---------------------------------------------------------*/

	//	if (SPELL_CARD_04_sakuya_ddd != spell_card_number)	/* ��l�`��: �����w���� */
	/* �����w��������off */

static void move_sakuya(SPRITE *src)
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
}


/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

void add_boss_sakuya(STAGE_DATA *l)
{
	alice_anime_count	= 0;
	vvv256	= 0;
	//----[BOSS]
	SPRITE *h;
	#if (0==USE_BOSS_COMMON_MALLOC)
	h								= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
	#else
	h								= pd_boss;/*�P��{�l*/
	#endif
	h->flags						|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
	h->type 						= BOSS_00_BOSS11;
	h->callback_mover				= move_sakuya;
	h->callback_loser				= sakuya_put_items;
//
	#if 0/* �������ς݂̕K�v���� */
	h->vx256						= t256(0);
	h->vy256						= t256(0);	/*fps_factor*/
	#endif
	h->base_score					= adjust_score_by_difficulty(score(5000000));	/* 500�� */
	//------------ �X�y�J�֘A
	spell_card_number				= SPELL_CARD_00_sakuya_000;
	spell_card_max					= SPELL_CARD_11_sakuya_kkk;
	#if (0==USE_BOSS_COMMON_MALLOC)
	spell_card_boss_init_regist(h);
	#else
	spell_card_boss_init_regist_void();
	#endif
	h->boss_base_state001			= (0);	/*ST_00*/	/*SA00*/
	h->wait1						= (0);
	h->wait2_256					= t256(0.0);
	h->move_angle512				= (0);
//-------------------------
//	send1_obj						= sprite_add_res(BASE_BOSS_SAKUYA_PNG);
	send1_obj->flags				&= (~(SP_FLAG_VISIBLE));
//	send1_obj->an im_frame			= SAKUYA_ANIME_03_CENTER_A;
	send1_obj->type 				= SP_MUTEKI;
//	send1_obj->callback_mover		= dummy_move;
	send1_obj->x256 				= (0);
	send1_obj->y256 				= (0);
//
}
//	h->anxxime_frame					= SAKUYA_ANIME_03_CENTER_A;
//	sakuya->wa it3					= 0;
