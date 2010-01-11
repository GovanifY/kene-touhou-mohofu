
#include "bullet_object.h"

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

/*
C++�݂����� �p�� �����Ă݂�B
���A�悭�l�����猻��global��player�ɂ���̂�
�v��Ȃ������̂�OFF�B�ł����Ƃł��̍l���͎g�������B
 */
#define USE_KEISYOU 0
//#define USE_KEISYOU 1
/*
�p�����Ȃ� 2019729
�p�������� 2019425(�p������������������)
*/

typedef struct /*_boss06_data*/
{
	BOSS_BASE boss_base;
//------------ �ړ��֘A
	int state1; 							/* �s�� */
//	int move_type;		/*	�ړ��^�C�v */	/* �`�� */
//
	int wait1;
	int wait2_256;
	int wait3;
	int move_angle512;

//	int length_s_p256;	/* ���ƃv���C���[�Ƃ̋��� */
	int anime_frame;

} BOSS06_DATA;
//	int aaa_type;		/* 0: normal, 1: destroyed */
#define move_vx256 move_angle512


typedef struct /*_boss06_doll_data*/
{
	#if (1==USE_KEISYOU)
	SPRITE *sakuya_obj; 		/* �p�� */
	#endif
	int state_d01;	/* �s�� */
	int state_d02;	/* �`�� */
	int nnn;
	int hen_no_kazu;	/* ���^�̕ӂ̐����J�E���g(0�ӂ���4�ӂ܂ł̌v5��==�܊p�`) */
	int wait;
	int angle512;
//
	int sss;
	int jjj;
	int doll_vx;
	int doll_vy;
	int doll_point_x;			/* �ڕW���W */
	int doll_point_y;			/* �ڕW���W */
	int doll_point_angle512;
	int star_remain_time;
//
//	int tx256;
//	int ty256;
} BOSS06_DOLL_DATA;


#if 1
/* �\���̂̊O�ɂ���K�R�������� */
	#define FLG_NO_DOLL 	(0)
	#define FLG_ALL_CAST	(1)
static int kodomo_num;
//static SPRITE *send1_obj; /*send1_obj*/
#endif


//----[ZAKO]


/*---------------------------------------------------------

---------------------------------------------------------*/


enum
{
//--- ���^�ȑO
	DS00 = 0,
	DS01,
	DS02,
	DS03,
};
enum
{
//--- ���^����
	DB00 = 0,
//--- ���^�ȍ~
	DB01,
	DB02,
	DB03,
	DB04,
	DB05,
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
	SA10,
	SA11,
	SA12,
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
	SB10,
	SB11,
	SB12,
	SB13,
};
enum
{
	SC00 = 0,
	SC0_0b,
	SC01,
	SC02,
	SC03,
	SC04,
	SC05,
	SC06,
	SC07,
	SC08,
};
enum
{
	SD00 = 0,
	SD0_0b,
	SD01,
	SD02,
};
enum
{
	SE00 = 0,
	SE01,
	SE02,
};
enum
{
	SG00 = 0,
	SG01,
	SG02,
	SG03,
	SG04,
};


/*
	SAKUYA_ANIME_xx_dummy
	�͒������p�^�[���Ȃ̂ŁA
	�v���O�����Ŏw�肵�Ȃ��B
	(�w�肷��ƈӖ�������Ȃ��Ȃ邩��w�肵�Ȃ��B)
*/
enum
{
	SAKUYA_ANIME_00_LEFT = 0,
	SAKUYA_ANIME_01_dummy,
	SAKUYA_ANIME_02_dummy,							//	SAKUYA_ANIME_03_,
	SAKUYA_ANIME_03_CENTER_A,
//
	SAKUYA_ANIME_04_CENTER_B,
	SAKUYA_ANIME_05_dummy,
	SAKUYA_ANIME_06_dummy,							//	SAKUYA_ANIME_08_,
	SAKUYA_ANIME_07_RIGHT,
//
	SAKUYA_ANIME_08_HIKARU_START,	/*= 0x10*/		//	SAKUYA_ANIME_10_,/*0x11*/
	SAKUYA_ANIME_09_dummy,			/*0x12*/		//	SAKUYA_ANIME_12_,/*0x13*/
	SAKUYA_ANIME_10_dummy,			/*0x14*/
	SAKUYA_ANIME_11_HIKARU_END, 	/*0x15*/
//
	SAKUYA_ANIME_12_HATUDAN_START,	/*0x16*/
	SAKUYA_ANIME_13_dummy,			/*0x17*/
	SAKUYA_ANIME_14_dummy,			/*0x18*/
	SAKUYA_ANIME_15_HATUDAN_END,	/*0x19*/
};

/*---------------------------------------------------------
	�q�������w �ړ�
---------------------------------------------------------*/

#define DOLL_WAIT50 		(16/*16*/)
#define DOLL_SPEED10		(4/*8*/)
#define DOLL_SET03			(1)
#define DOLL_ADD05			(1)

#define DOLL_POINT_CX00 	(t256(GAME_WIDTH/2))
#define DOLL_POINT_CY00 	(t256(/*GAME_HEIGHT/4*/64))
#define DOLL_RADIUS_SIZE	(/*t256*/(90))

#define DOLL_HEN_MAX		(5)

//------------------------------------------

	/*
	���^��1 190, 48
	���^��2 248, 217
	���^��3 100, 113
	���^��4 280, 113
	���^��5 138, 217
	*/
/*
#define pX 0
#define pY 1
	u16 ppp[5][2] =
	{
		{t256(190), t256( 48)},
		{t256(248), t256(217)},
		{t256(100), t256(113)},
		{t256(280), t256(113)},
		{t256(138), t256(217)},
	};
*/

/*static*/extern void enemy_sp2_bullet_create01(SPRITE *src, int angle512, int star_remain_time/**sd_nnn*/);
static void move_doll02(SPRITE *src)
{
	BOSS06_DOLL_DATA *data = (BOSS06_DOLL_DATA *)src->data;
	data->star_remain_time--;
//
	switch (data->state_d02)
	{
//------------------------------------------
	case DB00:	/* ���^��`������ */
		data->state_d02++/* = DB01*/;
		data->doll_point_angle512=0;
		data->doll_vx = t256(1.0);
		data->doll_vy = t256(0.0);
		#if (1==USE_KEISYOU)
		src->x256 = (data->sakuya_obj->x256);		send1_obj->x256 = src->x256;
		src->y256 = (data->sakuya_obj->y256);		send1_obj->y256 = src->y256;
		#else
		src->x256 = (((((PLAYER_DATA *)player->data)->boss))->x256);		send1_obj->x256 = src->x256;
		src->y256 = (((((PLAYER_DATA *)player->data)->boss))->y256);		send1_obj->y256 = src->y256;
		#endif
		data->sss = 0;
		break;
//------------------------------------------
	case DB01:	/* ���������܂ő҂� */
		#if 1
		/* �����w�A�j���[�V���� */
		src->m_angleCCW512++;
		mask512(src->m_angleCCW512);
		#else
//		src->an im_frame = 0;
//			if (2==data->nnn/*%(4-difficulty)*/)/* %2 == (5-3:Lunatic) */
//			{	src->an im_frame = 1;}
//			else
//			{	src->an im_frame = 0;}
		#endif
//
		#if (1==USE_KEISYOU)
		if (((BOSS06_DATA *)data->sakuya_obj->data)->state1==1)
		#else
		if (((BOSS06_DATA *)((((PLAYER_DATA *)player->data)->boss))->data)->state1==1)
		#endif
		{
			data->state_d02++ /*= DB02*/;
		}
		break;
//------------------------------------------
	case DB02:
		data->state_d02++ /*= DB03*/;
		data->state_d01 		= 0x00;
//		data->star_remain_time	= (64*5);
		data->wait				= DOLL_WAIT50;
		break;
	case DB03:	/* �ꎞ��~ */
	//	hosi_wait(src); static void hosi_wait(SPRITE *src)
		{
		//	BOSS06_DOLL_DATA *data = (BOSS06_DOLL_DATA *)src->data;
		//	if (data->wait<0)	{	data->state_d02++/* = DB04*/;}	else	{	data->wait--;}
			data->wait--;	if (data->wait<0)	{	data->state_d02++/* = DB04*/;}
		}
		break;
//------------------------------------------
	case DB04:	/* ���^��`���� */
//		data->doll_point_x=DOLL_POINT_CX00+sin512((data->doll_point_angle512))*DOLL_RADIUS_SIZE;
//		data->doll_point_y=DOLL_POINT_CY00+cos512((data->doll_point_angle512))*DOLL_RADIUS_SIZE;

		/*	*/
		send1_obj->x256 += (data->doll_vx)/**fps_fa ctor*/;
		send1_obj->y256 += (data->doll_vy)/**fps_fa ctor*/;
		data->sss++;
		if (
	//		(t256(32) > abs(data->doll_point_x-src->x256)) &&
	//		(t256(32) > abs(data->doll_point_y-src->y256))
				(16 < data->sss)
		)
		{
			data->sss = 0;
			data->doll_point_angle512 += (512*2/5);
			mask512(data->doll_point_angle512);
			{
			/* CCW�̏ꍇ */
		//	int d_angle512=atan_512(data->doll_point_y-src->y256-((src->h128)), data->doll_point_x-src->x256-((src->w128)));/*��ʒ��S����*/
			data->doll_vx = ((sin512((data->doll_point_angle512/*d_angle512*/))*/*t256*/(/*3*/DOLL_SPEED10/*+difficulty*/))/*>>8*/)/**fps_fa ctor*/;
			data->doll_vy = ((cos512((data->doll_point_angle512/*d_angle512*/))*/*t256*/(/*3*/DOLL_SPEED10/*+difficulty*/))/*>>8*/)/**fps_fa ctor*/;
			}

		//
			data->state_d02++/* = DB05*/;
		//
			data->hen_no_kazu--;
		//	if ((5-1) < data->hen_no_kazu)/* 5��==(5-1) */
			if (0 == data->hen_no_kazu)/* 5��==(5-1) */
			{
				data->hen_no_kazu = (DOLL_HEN_MAX);
			//	data->hen_no_kazu &= 1;
				data->star_remain_time	= (64*5);
				data->nnn++;	if (data->nnn>(/*4-0*/3/*difficulty*/) )	{data->nnn=0/*1*/ /*0*/;}
				#if (1==USE_KEISYOU)
				src->x256 = (data->sakuya_obj->x256);		send1_obj->x256 = src->x256;
				src->y256 = (data->sakuya_obj->y256);		send1_obj->y256 = src->y256;
				#else
				src->x256 = (((((PLAYER_DATA *)player->data)->boss))->x256);		send1_obj->x256 = src->x256;
				src->y256 = (((((PLAYER_DATA *)player->data)->boss))->y256);		send1_obj->y256 = src->y256;
				#endif
			//	data->tx256 = DOLL_POINT_CX00;
			//	data->ty256 = DOLL_POINT_CY00;
			//	send1_obj->x256 = data->tx256;
			//	send1_obj->y256 = data->ty256;
			}
		}
		else
		{
			data->state_d01+=/*5*/DOLL_ADD05/*3*/;
			if (0/*1*/==data->nnn)
			{
				if (0==(data->state_d01&/*7*/DOLL_SET03/*%5*/))
				{
					/* CW�̏ꍇ */
				//	enemy_sp2_bullet_create00(src, deg_360_to_512(3)+(( ((-data->state_d01*2/**data->hen_no_kazu*/)) +data->state_d01)>>3/*/5*/), t256(0.015), &(((BOSS06_DOLL_DATA *)src->data)->nnn) );
/* CCW�̏ꍇ */
					enemy_sp2_bullet_create01(send1_obj,
						data->doll_point_angle512,	//	-deg_360_to_512CCW(3)+(( ((-data->state_d01*2/**data->hen_no_kazu*/)) +data->state_d01)>>3/*/5*/),
					//	t256(0.015),
						data->star_remain_time//	&(((BOSS06_DOLL_DATA *)src->data)->nnn)
					);
				}
			}
		}
		break;
	case DB05:
		data->jjj++;
		if ((3-difficulty) < data->jjj)
		{
			data->jjj = 0;
		//	bullet_create_360_knifes(src, t256(1.2) );
			/*---------------------------------------------------------
				360�i�C�t
				�����w�p
				�S�����ɐi�C�t�e������
			---------------------------------------------------------*/
			//static void bullet_create_360_knifes(SPRITE *src, int speed256)
			{
			//	int angle512;
			//	for (angle512=0; angle512<512; angle512+=32/*29*/)
			//	{
			//		bullet_create_sakuya_knife(src, (speed256), (angle512), /*i*/(angle512)/*anime_pattern*/);
			//	}
			send1_obj->x256 = src->x256;
			send1_obj->y256 = src->y256;
			#if 1
			/* ���Ƃŗv�� */
	//		send1_obj->h128 = src->h128;
	//		send1_obj->w128 = src->w128;
			#endif
		//	bullet_create_n_way_dan_sa_type(src,
			send1_obj->BULLET_REGIST_speed256			=	(t256(1.2)/*(speed256)*/);
			send1_obj->BULLET_REGIST_angle512			=	(0/*aaa_angle512*/);	/* �e���p�x512 */
			send1_obj->BULLET_REGIST_div_angle512		=	(int)(512/(18));		/* �����p�x([360/360]�x��18����) */
			send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_KNIFE20_06_YUKARI;	/* [�s���N�i�C�t�e] */	/*BULLET_KNIFE20_04_AOI*/
			send1_obj->BULLET_REGIST_n_way				=	(18) ;/* [18way] */
			bullet_regist_basic();
			}
		}
		data->state_d02 = DB02;
		break;

//------------------------------------------
	}
	#if 1
	//if (data->c->type==SP_DELETE) 		/*����ł͂��܂������Ȃ��ꍇ������*/
	if (0/*FLG_NO_DOLL*/==common_boss_flags)	/* �P���|���ƊF�j�󂳂��B */
	{
		src->type = SP_DELETE;
	}
	#endif
//
	#if 0/*�����w�f�o�b�O�p*/
	/* �p�l���̃X�R�A����state_d02���A�O���C�Y����state_d01��\��������B���Ă����������������Ⴄ�B */
	((PLAYER_DATA *)player->data)->my_score 	= data->state_d02;
	((PLAYER_DATA *)player->data)->graze_point	= data->state_d01;
	#endif
}

static void move_doll01(SPRITE *src)
{
	BOSS06_DOLL_DATA *data = (BOSS06_DOLL_DATA *)src->data;
	//if (data->state_d02 < DB01)
	{
		#if (1==USE_KEISYOU)
		src->x256 = data->sakuya_obj->x256+((data->sakuya_obj->w128-src->w128));
		src->y256 = data->sakuya_obj->y256+((data->sakuya_obj->h128-src->h128));
		#else
		src->x256 = ((((PLAYER_DATA *)player->data)->boss))->x256+((((((PLAYER_DATA *)player->data)->boss))->w128-src->w128));
		src->y256 = ((((PLAYER_DATA *)player->data)->boss))->y256+((((((PLAYER_DATA *)player->data)->boss))->h128-src->h128))-t256(32);
		#endif
	//
		#if (1==USE_KEISYOU)
	//	if (((BOSS06_DATA *)data->sakuya_obj->data)->move_type==SPELL_CARD_09_sakuya_iii/*4*/ /*���^��`���Ă�������*/)
		#else
	//	if (((BOSS06_DATA *)((((PLAYER_DATA *)player->data)->boss))->data)->move_type==SPELL_CARD_09_sakuya_iii/*4*/ /*���^��`���Ă�������*/)
		if (spell_card_number==SPELL_CARD_09_sakuya_iii/*4*/ /*���^��`���Ă�������*/)
		#endif
		{
			static int hosi_gata_kakuze_counter;
			hosi_gata_kakuze_counter--;
			if (hosi_gata_kakuze_counter < 0)	/*���^��`���Ă�������*/
			{
				hosi_gata_kakuze_counter = (60*10);
			//	data->state_d02 = DB01;/*���^��`����*/
			//	data->state_d02 = DB00;/*���^��`����*/
				data->state_d02 = DB00;/*���^��`����*/
				src->callback_mover = move_doll02;
			}
		}
	}
	switch (data->state_d02)
	{
//------------------------------------------
	case DS00:
		src->color32 += 0x03000000; 		/*	src->alpha += 0x03;*/
		if (0x96000000 < src->color32)		/*	(0x96 < src->alpha) */
		{
			src->color32		= 0x96ffffff;	/*	src->alpha		= 0x96;*/
			data->state_d02++/* = DS01*/;
			data->wait		= 50;
		}
		break;

	case DS01:
		data->wait -= 1/*fps_fa ctor*/;
		if (data->wait < 1)
		{
			data->state_d02++/* = DS02*/;
			data->state_d01 = 0x100;
		}
		break;
	case DS02:
		if (0==(data->state_d01&0x0f/*%10*/))
		{
#if 0
/* CW�̏ꍇ */
			bullet_create_tomari2_dan(src, (data->state_d01/*>>4*/<<3/*1/10*/)/**256*/, data->angle512, 					  t256(0.02), data->angle512+(data->state_d01)-deg_360_to_512((180	)));	//ra d2deg512((data->state_d01)>>7/*/100*/)
			bullet_create_tomari2_dan(src, (data->state_d01/*>>4*/<<3/*1/10*/)/**256*/, data->angle512+deg_360_to_512((120)), t256(0.02), data->angle512+(data->state_d01)-deg_360_to_512((180+120)));	//ra d2deg512((data->state_d01)>>7/*/100*/)
			bullet_create_tomari2_dan(src, (data->state_d01/*>>4*/<<3/*1/10*/)/**256*/, data->angle512-deg_360_to_512((120)), t256(0.02), data->angle512+(data->state_d01)-deg_360_to_512((180-120)));	//ra d2deg512((data->state_d01)>>7/*/100*/)
#else
/* CCW�̏ꍇ */
			bullet_create_tomari2_dan(src, (data->state_d01/*>>4*/<<3/*1/10*/)/**256*/, data->angle512, 						 t256(0.02), data->angle512+(data->state_d01)+deg_360_to_512CCW((180	)));	//ra d2deg512((data->state_d01)>>7/*/100*/)
			bullet_create_tomari2_dan(src, (data->state_d01/*>>4*/<<3/*1/10*/)/**256*/, data->angle512-deg_360_to_512CCW((120)), t256(0.02), data->angle512+(data->state_d01)+deg_360_to_512CCW((180+120)));	//ra d2deg512((data->state_d01)>>7/*/100*/)
			bullet_create_tomari2_dan(src, (data->state_d01/*>>4*/<<3/*1/10*/)/**256*/, data->angle512+deg_360_to_512CCW((120)), t256(0.02), data->angle512+(data->state_d01)+deg_360_to_512CCW((180-120)));	//ra d2deg512((data->state_d01)>>7/*/100*/)
#endif
		}
		data->state_d01--;
		if (data->state_d01 < 1)
		{
			data->state_d02++/* = DS03*/;
			data->wait		= 15;
			data->state_d01 = 0x10;
		}
		break;
	case DS03:
		data->wait -= 1/*fps_fa ctor*/;
		if (data->wait < 1)
		{
			data->wait=12;
			#if (0==USE_DESIGN_TRACK)
			play_voice_auto_track(VOICE13_SAKUYA_SHOT02);
			#else
			voice_play(VOICE13_SAKUYA_SHOT02, TRACK04_TEKIDAN);
			#endif
			{int i;
				for (i=t256(5); i<t256(9); i+=t256(1))
				{
#if 0
/* CW�̏ꍇ */
					bullet_create_offset_dan_type000(src, i, data->angle512,					 /*t256(0), t256(0),*/ BULLET_HARI32_00_AOI);
					bullet_create_offset_dan_type000(src, i, data->angle512+deg_360_to_512(120), /*t256(0), t256(0),*/ BULLET_HARI32_00_AOI);
					bullet_create_offset_dan_type000(src, i, data->angle512-deg_360_to_512(120), /*t256(0), t256(0),*/ BULLET_HARI32_00_AOI);
#else
/* CCW�̏ꍇ */
					bullet_create_offset_dan_type000(src, i, data->angle512,						/*t256(0), t256(0),*/ BULLET_HARI32_00_AOI);
					bullet_create_offset_dan_type000(src, i, data->angle512-deg_360_to_512CCW(120), /*t256(0), t256(0),*/ BULLET_HARI32_00_AOI);
					bullet_create_offset_dan_type000(src, i, data->angle512+deg_360_to_512CCW(120), /*t256(0), t256(0),*/ BULLET_HARI32_00_AOI);
#endif
				}
			}
			/* deg_360_to_512(8.59) 8.59436717317284029518323968680208	PI*2/41.8879008478638666666666666666667 ra d2deg512(0.15)*/
			/* 12.2230999796235950864828297767852 == 512/41.8879008478638666666666666666667 */
#if 0
/* CW�̏ꍇ */
			data->angle512 += 12;
#else
/* CCW�̏ꍇ */
			data->angle512 -= 12;
#endif
			data->state_d01--;
		}
		if (data->state_d01<0x0)
		{
			data->state_d02--/* = DS02*/;
			data->state_d01 = 0x50;
		}
		break;

	}
	#if 1
	//if (data->c->type==SP_DELETE) 		/*����ł͂��܂������Ȃ��ꍇ������*/
	if (0/*FLG_NO_DOLL*/==common_boss_flags)	/* �P���|���ƊF�j�󂳂��B */
	{
		src->type = SP_DELETE;
	}
	#endif
//
	#if 0/*�����w�f�o�b�O�p*/
	/* �p�l���̃X�R�A����state_d02���A�O���C�Y����state_d01��\��������B���Ă����������������Ⴄ�B */
	((PLAYER_DATA *)player->data)->my_score 	= data->state_d02;
	((PLAYER_DATA *)player->data)->graze_point	= data->state_d01;
	#endif
}

static void move_doll00(SPRITE *src)
{
//	if (sp ell_card_number==SPELL_CARD_09_sakuya_iii/*4*/ /*���^��`���Ă�������*/)
	if (0 != kodomo_num)
	{
	//	BOSS06_DOLL_DATA *data = (BOSS06_DOLL_DATA *)src->data;
	//	data->state_d02 = DS00;
		src->callback_mover = move_doll01;
	}
}

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
	src->type			= BOSS_00_BOSS11;/* ���̃A�j���p�^�[�� */
//
	BOSS06_DATA *data = (BOSS06_DATA *)src->data;
	//tama->an im_frame = SAKUYA_ANIME_03_CENTER_A;//�o�O(�����Ńv���C���[�V���b�g�̃A�j���p�^�[����M��̂ŁA���~�{���Ńn���O�A�b�v����)
	data->anime_frame	= SAKUYA_ANIME_03_CENTER_A;//���̃A�j���p�^�[����M��
	data->state1		= 0;/* ���ʂ��� �l�� 0 �ɂ��� */
	data->wait1 		= 80;
	data->wait2_256 	= t256(-100);
	data->wait3 		= 0;//???

}
	#if (0)
	sp ell_card_number++;
	#endif
	#if (0)
//	src->base_health	= (1024-1); 	/* life_point����Q�[�W��[ */
	{
		u16 aaa_timer[10/*8*/] =
		{
			(1000*64),	/* �_�~�[ */
			(1000*64),	/* �ŏI�`�� */
			(40*64),
			(40*64),
			(40*64),
			(40*64),
			(40*64),
			(40*64),
			(40*64),
			(40*64),
		};
		spell_card_boss_timer		= aaa_timer[(data->boss_base.bo ss_life)];		/* 40*64==40[count] 	��40[�b(64/60)](�P�ʂ͕b�ł͂Ȃ�) */
	}	/* �{�X�^�C�}�[������ */
	// 40 60 [�b] ???	4096/*2500*/	/*3000*/;//???
	#endif

/*---------------------------------------------------------

---------------------------------------------------------*/



/*---------------------------------------------------------

---------------------------------------------------------*/

static void sakuya_wait_state(SPRITE *src, /*int nextstate,*/ int set_anime_frame)
{
	BOSS06_DATA *data = (BOSS06_DATA *)src->data;
	data->wait1 -= 1/*fps_fa ctor*/;
	if (0 < data->wait1)
	{;}
	else
	{
		data->state1++/* = nextstate*/;
		data->anime_frame = set_anime_frame;
	}
}


/*---------------------------------------------------------
	�e�����I���܂ő҂B
	-------------------------------------------------------

---------------------------------------------------------*/

static void danmaku_state_check_holding(SPRITE *src)	/*, int nextstate*/ /*, int an im_frame*/
{
	BOSS06_DATA *data = (BOSS06_DATA *)src->data;
	if (DANMAKU_00 == data->boss_base.danmaku_type)
	{
		data->state1++/* = nextstate*/;
	}
}


/*---------------------------------------------------------
	���	���S�����E�A�j���T�u
	-------------------------------------------------------
	data->anime_frame = SAKUYA_ANIME_03_CENTER_A;
	data->wait3 = 0;
	�ŏ���������Ă�K�v������B
---------------------------------------------------------*/

static void sakuya_anime_to_sayuu(SPRITE *src)
{
	BOSS06_DATA *data = (BOSS06_DATA *)src->data;
	if (0 < data->move_vx256)
	{
		/* �E�ֈړ� */
		if (data->anime_frame==SAKUYA_ANIME_07_RIGHT)
		{
			data->state1++/*	= SB06*/;/*�����܂�*/
		}
		else
		{
			data->wait3 += 1/*fps_fa ctor*/;
			if (data->wait3>=3)
			{
				data->wait3=0;
				data->anime_frame++;/*???*/
			}
		}
	}
	else
	{
		/* ���ֈړ� */
		if (data->anime_frame==SAKUYA_ANIME_00_LEFT)
		{
			data->state1++/*	= SB06*/;/*�����܂�*/
		}
		else
		{
			data->wait3 += 1/*fps_fa ctor*/;
			if (data->wait3>=3)
			{
				data->wait3=0;
				data->anime_frame--;/*???*/
			}
		}
	}
}


/*---------------------------------------------------------
	���	�e�����A�j���ҋ@
	-------------------------------------------------------
	data->anime_frame = SAKUYA_ANIME_12_HATUDAN_START;
	data->wait3 = 0;
	�ŏ���������Ă�K�v������B
---------------------------------------------------------*/

static void sakuya_tama_anime_wait3_0(SPRITE *src)
{
	BOSS06_DATA *data = (BOSS06_DATA *)src->data;
	if (data->anime_frame==SAKUYA_ANIME_15_HATUDAN_END)
	{
		data->state1++/*	= SB06*/;
	}
	else
	{
		data->wait3 += 1/*fps_fa ctor*/;
		if (data->wait3>=3)
		{
			data->wait3=0;
			data->anime_frame++;/*???*/
		}
	}
}


/*---------------------------------------------------------

---------------------------------------------------------*/

//static void enemy_boss06_out(SPRITE *src)
//{
//	boss_clip_min.x256 = t256(50);
//	boss_clip_max.x256 = t256(GAME_WIDTH)-t256(50)-((src->w128+src->w128));
//	boss_clip_min.y256 = t256(10);
//	boss_clip_max.y256 = t256(GAME_HEIGHT)-t256(120)+((src->h128+src->h128));
//
//		 if (src->x256 < boss_clip_min.x256 )	{	src->x256 = boss_clip_min.x256;}
//	else if (src->x256 > boss_clip_max.x256 )	{	src->x256 = boss_clip_max.x256;}
//		 if (src->y256 < boss_clip_min.y256 )	{	src->y256 = boss_clip_min.y256;}
//	else if (src->y256 > boss_clip_max.y256 )	{	src->y256 = boss_clip_max.y256;}
//}


/*---------------------------------------------------------

---------------------------------------------------------*/

#if 1
/* �v���C���[�̎���ɎU��΂�A���Ԃ��o�ƃv���C���[�_���e */
static void enemy_boss06_knifes3(SPRITE *src)	/*,*/ /*dou ble speed,*/	/*int length222*/
{
	BOSS06_DATA *data = (BOSS06_DATA *)src->data;
	int len256;
	int tmp1_256;
	int tmp2_256;
	int tmp888;
	tmp888=0;
	/* ������x�����ɏo�Ȃ��ƔF�����Â炢 */
	/* �L����(�R�A)���ł����Ɣ�����O�Ɏ��ʁB(�i�C�t���L����O�ɂ�����) */
	len256 = t256(/*70*/80) -((difficulty)<<8) /*-length222*/  /*/2*/;
	data->wait2_256 = t256(0);
	while (1)
	{
		tmp888 = (((int)ra_nd()+(t256_floor(data->wait2_256)) )&(8-1)/*%8*/);
		data->wait2_256 += t256(1);
		len256 += t256(0.1);
#if 0
/* CW�̏ꍇ */
		tmp1_256 = player->x256 + ((player->w128-src->w128)) + ((cos512((tmp888<<6))*len256)>>8);
		tmp2_256 = player->y256 + ((player->h128-src->h128)) + ((sin512((tmp888<<6))*len256)>>8);
#else
/* CCW�̏ꍇ */
		tmp1_256 = player->x256 + ((player->w128-src->w128)) + ((sin512((tmp888<<6))*len256)>>8);
		tmp2_256 = player->y256 + ((player->h128-src->h128)) + ((cos512((tmp888<<6))*len256)>>8);
#endif
		if ((( t256(30) < tmp1_256) && (tmp1_256 < t256(GAME_WIDTH-30)))&&
			(( t256(30) < tmp2_256) && (tmp2_256 < t256(GAME_HEIGHT-72))))
		{	break;}
	}
	int tmp_x256;
	int tmp_y256;
	tmp_x256 = player->x256 + ((player->w128));
	tmp_y256 = player->y256 + ((player->h128));
	int ii;
	for (ii=0; ii<8; ii++)
	{
		if (tmp888==ii)
		{
			src->tmp_angleCCW512	= (ii<<6);
			src->x256				= tmp1_256;
			src->y256				= tmp2_256;
			src->color32			= 0x00ffffff;			/*	src->alpha			= 0x00;*/
//			data->anime_frame		= SAKUYA_ANIME_15_HATUDAN_END;//?????
			#if 0/*???*/
			/* �X�y���J�[�h�V�X�e���ł͑Ή��ł��Ȃ��B */
			sp ell_card_number	= SPELL_CARD_10_sakuya_jjj/*5*/;/*???*/
			#endif
			data->wait2_256 	= /*speed256*/t256(1)+(difficulty<<8);
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
#if 0
/* CW�̏ꍇ */
	src->x256 += ((cos512((b_move_angle512))*(speed256))>>8)/**fps_fa ctor*/;
	src->y256 += ((sin512((b_move_angle512))*(speed256))>>8)/**fps_fa ctor*/;
#else
/* CCW�̏ꍇ */
	src->x256 += ((sin512((b_move_angle512))*(speed256))>>8)/**fps_fa ctor*/;
	src->y256 += ((cos512((b_move_angle512))*(speed256))>>8)/**fps_fa ctor*/;
#endif
		 if (src->y256 < (SAKUYA_LIMIT_MIN_Y) ) {	src->y256 += t256(1);	}/* �͂ݏo������C�� */
	else if (src->y256 > (SAKUYA_LIMIT_MAX_Y) ) {	src->y256 -= t256(1);	}/* �͂ݏo������C�� */
}


/*---------------------------------------------------------
	SPELL_CARD_01_sakuya_aaa	�ʏ�U��(�S�e���A����Ƃ͂����Ď኱�Ⴄ)	���`��: ���E�ɓ����ď��e����
---------------------------------------------------------*/

/*static*/ void sakuya_01_keitai(SPRITE *src)
{
	BOSS06_DATA *data = (BOSS06_DATA *)src->data;
//	PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
	static int my_yudo_point;

/* ���`�Ԃł͎g���Ė����݂��������痘�p���� */
//#define _mode wait2_256
	switch (data->state1)
	{
	case SA00:
			data->state1	= SA12; 	/* SA01*/
		break;

	/*���S�Ɏp������*/
/*----------------*/
	case SA01:	/* ���ֈړ� */
		//if (pd->bo ssmode==B01_BA TTLE)
	//	{
		if (my_yudo_point < src->x256 )/*t256(50)*/
		{
			src->x256 -= t256(2.0/*3.0*/)/*fps_fa ctor*/;
			if (SAKUYA_ANIME_00_LEFT < data->anime_frame)
			{
				data->wait3 += 1/*fps_fa ctor*/;
				if (SAKUYA_ANIME_03_CENTER_A < data->wait3)
				{
					data->anime_frame--;
					data->wait3 = 0;
				}
			}
		}
		else	/* x �� 50 �ȉ��ɂȂ����� */
		{
			data->state1++/*	= SA02*/;
	#if 1/* �ϒe�� */
			tmp_angle_jikinerai512(player, src);/* ���@�˂炢�p�쐬 */
		#if 0
		/*CW*/
			src->tmp_angleCCW512	-= 128;
		#else
		/*CCW*/
			src->tmp_angleCCW512	+= (256-128);
		#endif
	#else/* �Œ�e�� */
		/*CCW*/
			src->tmp_angleCCW512	= deg_360_to_512CCW((90));/* �E�����猂���n�߂� (128)*/
	#endif
			data->boss_base.danmaku_type		= DANMAKU_01_sakuya;	/* �S�e�����Z�b�g */
			data->boss_base.danmaku_time_out	= DANMAKU_01_SET_TIME;	/* �S�e���̔������� */
	//		data->anime_frame		= SAKUYA_ANIME_03_CENTER_A;
			data->anime_frame		= SAKUYA_ANIME_12_HATUDAN_START;
		}
	//	}
		break;
	case SA02:	/* �e�����A�j���ҋ@ */
		sakuya_tama_anime_wait3_0(src);
		break;
	case SA03:	/* �S�e�����I���܂őҋ@ */
		danmaku_state_check_holding(src/*, SA03*/ /*, SAKUYA_ANIME_03_CENTER_A*/);
		break;
	case SA04:
		data->anime_frame		= SAKUYA_ANIME_03_CENTER_A;
		data->state1++/*	= SA05*/;
		break;
	case SA05:	/* �^�񒆂ɖ߂� */
		if (SAKUYA_POINT_X_MID > src->x256 )
		{
			src->x256 += t256(3.0/*3.0*/)/*fps_fa ctor*/;
			if (SAKUYA_ANIME_07_RIGHT > data->anime_frame)
			{
				data->wait3 += 1/*fps_fa ctor*/;
				if (SAKUYA_ANIME_03_CENTER_A < data->wait3)
				{
					data->anime_frame++;
					data->wait3 = 0;
				}
			}
		}
		else	/* �^�񒆂܂ŗ����� */
		{
			data->state1++/*	= SA04*/;
		//	if (RANK_NORMAL<difficulty) 	{	bullet_create_aka_lines(s); 	}
		}
		break;
/*----------------*/
	case SA06:/* �ړ��ʒu�����߂� */
			data->state1++/*	= SA04*/;
		//	my_yudo_point = SAKUYA_POINT_X_RIGHT_MID;
			my_yudo_point = SAKUYA_POINT_X_MID+(ra_nd()&((128*256)-1));
		break;
/*----------------*/
	case SA07:	/* �E�ֈړ� */
		if ( my_yudo_point > src->x256/*+((src->w128+src->w128))*/ )/*t256(GAME_WIDTH-50)*/
		{
			src->x256 += t256(2.0/*3*/)/*fps_fa ctor*/;
		}
		else
		{
			data->state1++/*	= SA05*/;
	#if 1/* �ϒe�� */
			tmp_angle_jikinerai512(player, src);/* ���@�˂炢�p�쐬 */
		#if 0
		/*CW*/
			src->tmp_angleCCW512	-= 128;
		#else
		/*CCW*/
			src->tmp_angleCCW512	+= (256-128);
		#endif
	#else/* �Œ�e�� */
		/*CCW*/
			src->tmp_angleCCW512	= deg_360_to_512CCW((90));/* �E�����猂���n�߂� (128)*/
	#endif
			data->boss_base.danmaku_type		= DANMAKU_01_sakuya;	/* �S�e�����Z�b�g */
			data->boss_base.danmaku_time_out	= DANMAKU_01_SET_TIME;	/* �S�e���̔������� */
		//	data->anime_frame		= SAKUYA_ANIME_03_CENTER_A;
			data->anime_frame		= SAKUYA_ANIME_12_HATUDAN_START;
		}
		break;
	case SA08:	/* �e�����A�j���ҋ@ */
		sakuya_tama_anime_wait3_0(src);
		break;
	case SA09:	/* �S�e�����I���܂őҋ@ */
		danmaku_state_check_holding(src/*, SA07*/ /*, SAKUYA_ANIME_03_CENTER_A*/);
		break;
	case SA10:
		data->anime_frame		= SAKUYA_ANIME_03_CENTER_A;
		data->state1++/*	= SA05*/;
		break;
	case SA11:	/* �^�񒆂ɖ߂� */
		if (SAKUYA_POINT_X_MID < src->x256 )
		{
			src->x256 -= t256(3.0/*3.0*/)/*fps_fa ctor*/;
			if (SAKUYA_ANIME_00_LEFT < data->anime_frame)
			{
				data->wait3 += 1/*fps_fa ctor*/;
				if (SAKUYA_ANIME_03_CENTER_A < data->wait3)
				{
					data->anime_frame--;
					data->wait3 = 0;
				}
			}
		}
		else	/* �^�񒆂ɖ߂� */
		{
			data->state1++/* = SA01*/;
		//	if (RANK_NORMAL<difficulty) 	{	bullet_create_aka_lines(s); 	}
		}
		break;
/*----------------*/
	case SA12:/* �ړ��ʒu�����߂� */
		data->state1	= SA01;
	//	my_yudo_point = SAKUYA_POINT_X_LEFT_MID;
		my_yudo_point = SAKUYA_POINT_X_MID-(ra_nd()&((128*256)-1));
		break;
/*----------------*/

	}
}


/*---------------------------------------------------------
	SPELL_CARD_02_sakuya_bbb	��p�u�~�X�f�B���N�V�����v	���`��: �S���ʁA���܂�
---------------------------------------------------------*/

/*static*/ void sakuya_02_keitai(SPRITE *src)
{
	BOSS06_DATA *data = (BOSS06_DATA *)src->data;

/* ���`�Ԃł͎g���Ė����݂��������痘�p���� */
#define sayuu_mode wait2_256
	//data->wait2_256		���[�v��
	switch (data->state1)
	{
	case SB00:	/* �����l */
		data->wait1 		= 0;/*�Ƃ肠����*/
//		data->wait2_256 	= t256(0);
		data->sayuu_mode	= (ra_nd()&(2-1)/*%2*/);
		data->state1++/*	= SB01*/;
		break;

//	case SB00aaa:	/* �ҋ@ */
//		sakuya_wait_state(src, SAKUYA_ANIME_03_CENTER_A);
	//		data->wait1 -= 1/*fps_fa ctor*/;
	//	if (data->wait1<0)
	//	{
	//		data->state1++/*	= SB01*/;
//++//		pd->bo ssmode	= B00_NONE/*B01_BA TTLE*/;
	//	}	//		else	{	}
//		break;

//-----------

	case SB01:
		src->x256 = t256(GAME_WIDTH/2); /* �^���փ��[�v */
		{
//			int ang512;
			data->sayuu_mode ^= (2-1);/* ���] */
			if (0==data->sayuu_mode)
			{
				data->move_vx256 = t256(1.0/*0.5*/);
//				ang512=(ra_nd()&(256-1)/*%(180*1.42)*/);
//				if (ang512>deg_360_to_512(90))		{	ang512+=deg_360_to_512(90); 	data->anime_frame=(SAKUYA_ANIME_03_CENTER_A+1); }
//				else								{									data->anime_frame=(SAKUYA_ANIME_03_CENTER_A-1); }
//				ang512 -= deg_360_to_512(45);
			}
			else
			{
				data->move_vx256 = -t256(1.0/*0.5*/);
//				if (src->x256 > t256(GAME_WIDTH/2) )	{	ang512=deg_360_to_512(180); 	data->anime_frame=(SAKUYA_ANIME_03_CENTER_A+2); }
//				else									{	ang512=deg_360_to_512(0);		data->anime_frame=(SAKUYA_ANIME_03_CENTER_A-2); }
			}
//			data->move_angle512=/*deg512_2rad*/(ang512);
		}
//		data->wait1 		= 20+(3-difficulty)*10;
		data->wait1 		= (35);/* [WAITSTATE] */	/* �e�����Ȃ��ł�����ƈړ� */
//		data->wait2_256 += t256(1);
		//
		data->anime_frame = SAKUYA_ANIME_03_CENTER_A;
		data->wait3=0;
		data->state1++/*	= SB02*/;
		break;

	case SB02:		/* �ړ� */
		src->x256 += ((data->move_vx256))/**fps_fa ctor*/;	/* �ړ� */	/* �e�����Ȃ��ł�����ƈړ� */
		sakuya_anime_to_sayuu(src);
		break;
	case SB03:
		src->x256 += ((data->move_vx256))/**fps_fa ctor*/;	/* �ړ� */	/* �e�����Ȃ��ł�����ƈړ� */
		sakuya_wait_state(src/*, SB05*/, SAKUYA_ANIME_12_HATUDAN_START);
		break;
	case SB04:
		data->anime_frame = SAKUYA_ANIME_12_HATUDAN_START;
		data->wait3 = 0;
		data->state1++/*	= SB02*/;
		break;
	case SB05:	/* �e�����A�j���ҋ@ */
		sakuya_tama_anime_wait3_0(src);
		break;
	case SB06:
		//	enemy_boss06_mamemaki_state(src, SB07, SAKUYA_ANIME_15_HATUDAN_END);
			data->boss_base.danmaku_type		= DANMAKU_02_sakuya;	/* 24�e�����Z�b�g */
			data->boss_base.danmaku_time_out	= DANMAKU_02_SET_TIME;	/* 24�e���̔������� */
			data->state1++/*	= SB07*/;
		break;
	case SB07:	/* 24�e�����I���܂ňړ� */
		src->x256 += ((data->move_vx256))/**fps_fa ctor*/;	/* �ړ� */	/* �e�����Ȃ���ړ� */
		danmaku_state_check_holding(src/*, SA07*/ /*, SAKUYA_ANIME_03_CENTER_A*/);
		break;
	case SB08:
			src->x256 = t256(GAME_WIDTH)-src->x256; 			/* ���Α��փ��[�v */
			data->anime_frame = SAKUYA_ANIME_12_HATUDAN_START;
			data->wait3 = 0;
			data->state1++/*	= SB09*/;
		break;
	case SB09:	/* �e�����A�j���ҋ@ */
		sakuya_tama_anime_wait3_0(src);
		break;
	case SB10:
			data->boss_base.danmaku_type		= DANMAKU_03_sakuya;	/* 11�e�����Z�b�g */
			data->boss_base.danmaku_time_out	= DANMAKU_03_SET_TIME;	/* 11�e���̔������� */
			data->wait1 			= (35);/* (SB12��)�����ҋ@ */
			data->anime_frame		= SAKUYA_ANIME_03_CENTER_A;
			data->state1++/*	= SB07*/;
		break;
	case SB11:	/* 11�e�����I���܂őҋ@ */
		danmaku_state_check_holding(src/*, SA07*/ /*, SAKUYA_ANIME_03_CENTER_A*/);
		break;
	case SB12:	/* (SB12��)�����ҋ@ */
		sakuya_wait_state(src/*, SB05*/, SAKUYA_ANIME_03_CENTER_A);
		break;
	case SB13:
		data->state1	= SB01;
		break;
	}
//	enemy_boss06_out(src);
}


/*---------------------------------------------------------
	SPELL_CARD_03_sakuya_ccc	���J�u�o�[�e�B�J���i�C�t�v		��O�`��: �����i�C�t�������Ă����
---------------------------------------------------------*/

/*static*/ void sakuya_03_keitai(SPRITE *src)
{
	BOSS06_DATA *data = (BOSS06_DATA *)src->data;
	switch (data->state1)
	{
	case SC00:
		{	/* ���̐�����ς��� */
			boss_clip_min.x256 = t256(0);			//t256(50);
			boss_clip_max.x256 = t256(GAME_WIDTH);	//t256(GAME_WIDTH)-t256(50)-((src->w128+src->w128));
		}
		data->state1++;
		break;
	case SC0_0b:
		sakuya_wait_state(src, SAKUYA_ANIME_03_CENTER_A);
	//	data->wait1 -= 1/*fps_fa ctor*/;
	//	if (data->wait1<0)	/* �ҋ@���Ԃ��I������� */
	//	{
	//		data->state1++/*	= SC01*/;
//++//		pd->bossmode		= B00_NONE/*B01_BATTLE*/;
	//		data->anime_frame	= SAKUYA_ANIME_03_CENTER_A;
	//	}
	//	else	{	}
		break;
	case SC01:
		if (src->x256 < (SAKUYA_LIMIT_X_LEFT)/*5*/ )	/* �����ɗ����� */	/* �� || (src->y<5)������Ɩ������[�v�ɂȂ�*/
		{
			data->state1++/*	= SC02*/;
			data->wait1 	= 10;
			data->wait2_256 = t256(0);
			data->anime_frame = SAKUYA_ANIME_00_LEFT;
		}
		else
		{
			sakuya_idou_sub(src, t256(3), SAKUYA_POINT_00_LEFT);
			if (data->anime_frame > SAKUYA_ANIME_00_LEFT)
			{
				data->wait3 += 1/*fps_fa ctor*/;
				if (data->wait3>=3)
				{
					data->anime_frame--;
					data->wait3=0;
				}
			}
		}
		break;
	case SC02:
		sakuya_wait_state(src/*, SC03*/, (SAKUYA_ANIME_03_CENTER_A-1)/*???*/);
		break;
	case SC03:
		if (src->x256 > (SAKUYA_LIMIT_X_RIGHT)/*GAME_WIDTH-src->w+3*/ ) /* �E�[�ɗ����� */
		{
			data->state1++/*	= SC04*/;
			data->wait2_256 = t256(0);
			data->anime_frame = SAKUYA_ANIME_07_RIGHT;
		}
		else
		{
			{	const short sss[4] =
				{
					t256(1.2),/*easy:�܂΂�Ȃ̂ŊȒP*/
					t256(0.5/*1.0*/ /*1.5*/),/*normal:��Ȃ̂ő傫��������ΊȒP*/
					t256(1.0/*2.0*/),/*hard:���܂ɓ����������*/
					t256(18.5/*6.5*/ /*12.5*/ /*8.5*/) /*luna:���ւ����̃X�L�}�������ł������H*/
					/*(luna:�{����Ȃ��Ă��C�������łȂ�Ƃ��Ȃ���x�ɓ�Փx�}�������̂����܂����������)*/
				};
				data->wait2_256 += /*t256(4)*/sss[difficulty]/*t256(3*1.42)*/;
			}
			if (0==(((int)data->wait2_256)&(16-1)/*%21 t256(15*1.42)*/))
			{
#if 0
/* CW�̏ꍇ */
				bullet_create_jyuryoku_dan000(src, /*(0.7)*/t256(2)/*(2)*/, (deg_360_to_512(360)-((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)), t256(0.03), BULLET_KNIFE20_05_AKA);
				//if (difficulty)
				{
					bullet_create_jyuryoku_dan000(src, /*(0.7)*/t256(2)/*(2)*/, (deg_360_to_512(270)-((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)), t256(0.04), BULLET_KNIFE20_05_AKA);
				//	if (1<difficulty)
					{	bullet_create_jyuryoku_dan000(src, /*(0.7)*/t256(2)/*(2)*/, (deg_360_to_512(180)-((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)), t256(0.02), BULLET_KNIFE20_05_AKA);}
				}
#else
/* CCW�̏ꍇ */
				bullet_create_jyuryoku_dan000(src, /*(0.7)*/t256(2)/*(2)*/, (-deg_360_to_512CCW(360)-((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)), t256(0.03), BULLET_KNIFE20_05_AKA);
				//if (difficulty)
				{
					bullet_create_jyuryoku_dan000(src, /*(0.7)*/t256(2)/*(2)*/, (-deg_360_to_512CCW(270)-((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)), t256(0.04), BULLET_KNIFE20_05_AKA);
				//	if (1<difficulty)
					{	bullet_create_jyuryoku_dan000(src, /*(0.7)*/t256(2)/*(2)*/, (-deg_360_to_512CCW(180)-((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)), t256(0.02), BULLET_KNIFE20_05_AKA);}
				}
#endif
				#if (0==USE_DESIGN_TRACK)
				play_voice_auto_track(VOICE12_SAKUYA_B2_SHOT);
				#else
				voice_play(VOICE12_SAKUYA_B2_SHOT, TRACK04_TEKIDAN);
				#endif
			}
			sakuya_idou_sub(src, t256(5), SAKUYA_POINT_03_RIGHT);
			if (data->anime_frame < SAKUYA_ANIME_07_RIGHT)
			{
				data->wait3 += 1/*fps_fa ctor*/;
				if (data->wait3>=7)
				{
					data->anime_frame++;
					data->wait3=0;
				}
			}
		}
		break;
	case SC04:
	//	if (( src->x256 < (SAKUYA_LIMIT_X_MID)/*GAME_WIDTH/2-src->w/2*/ ))	/* �^�񒆂ɗ����� */
		if (( src->x256 < (SAKUYA_LIMIT_X_MID_LEFT)) &&
			( src->x256 > (SAKUYA_LIMIT_X_MID_RIGHT)))		/* �^�񒆂ɗ����� */
		{
			data->state1++/*	= SC05*/;
			data->anime_frame = SAKUYA_ANIME_03_CENTER_A;
		}
		else
		{
			sakuya_idou_sub(src, t256(3), SAKUYA_POINT_01_MID_UP);
			if (SAKUYA_ANIME_03_CENTER_A < data->anime_frame)
			{
				data->wait3 += 1/*fps_fa ctor*/;
				if (data->wait3>=3)
				{
					data->anime_frame--;
					data->wait3=0;
				}
			}
		}
		break;
	case SC05:
		if (src->x256 > (SAKUYA_LIMIT_X_RIGHT)/*GAME_WIDTH-src->w-3*/ ) /* �E�[�ɗ����� */ /* �� || (src->y<5)������Ɩ������[�v�ɂȂ� */
		{
			data->state1++/*	= SC06*/;
			data->wait1 	= 10;
			data->wait2_256 = 0;
			data->anime_frame = SAKUYA_ANIME_07_RIGHT;
		}
		else
		{
			sakuya_idou_sub(src, t256(3), SAKUYA_POINT_03_RIGHT);
			if (data->anime_frame < SAKUYA_ANIME_07_RIGHT)
			{
				data->wait3 += 1/*fps_fa ctor*/;
				if (data->wait3>=3)
				{
					data->anime_frame++;
					data->wait3=0;
				}
			}
		}
		break;
	case SC06:
		if (src->x256 < (SAKUYA_LIMIT_X_LEFT)/*5*/) 	/* ���[�ɗ����� */
		{
			data->state1++/*	= SC07*/;
			data->wait2_256 = 0;
			data->anime_frame = SAKUYA_ANIME_00_LEFT;
		}
		else
		{
			data->wait2_256 += t256(4)/*(3*1.42)*/;
			if (0x0000==((data->wait2_256)&(0x0f00))/*%(12*1.42)*/)
			{
#if 0
/* CW�̏ꍇ */
				bullet_create_jyuryoku_dan000(src, t256(3), (((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)+deg_360_to_512(180)), t256(0.03), BULLET_KNIFE20_05_AKA);
				if (difficulty)
				{
					bullet_create_jyuryoku_dan000(src, t256(3), (((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)+deg_360_to_512(90)), t256(0.04), BULLET_KNIFE20_05_AKA);
					if (1<difficulty)
					{	bullet_create_jyuryoku_dan000(src, t256(3), (((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)), t256(0.02), BULLET_KNIFE20_05_AKA);}
				}
#else
/* CCW�̏ꍇ */
				bullet_create_jyuryoku_dan000(src, t256(3), (((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)+deg_360_to_512CCW(360-180)), t256(0.03), BULLET_KNIFE20_05_AKA);
				if (difficulty)
				{
					bullet_create_jyuryoku_dan000(src, t256(3), (((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)+deg_360_to_512CCW(360-90)), t256(0.04), BULLET_KNIFE20_05_AKA);
					if (1<difficulty)
					{	bullet_create_jyuryoku_dan000(src, t256(3), (((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)), t256(0.02), BULLET_KNIFE20_05_AKA);}
				}
#endif
				#if (0==USE_DESIGN_TRACK)
				play_voice_auto_track(VOICE12_SAKUYA_B2_SHOT);
				#else
				voice_play(VOICE12_SAKUYA_B2_SHOT, TRACK04_TEKIDAN);
				#endif
			}
			sakuya_idou_sub(src, t256(5), SAKUYA_POINT_00_LEFT);
			if (data->anime_frame > SAKUYA_ANIME_00_LEFT)
			{
				data->wait3 += 1/*fps_fa ctor*/;
				if (data->wait3>=7)
				{
					data->anime_frame--;
					data->wait3=0;
				}
			}
		}
		break;
	case SC07:
	//	if (( src->x256 > (SAKUYA_LIMIT_X_MID)/*(GAME_WIDTH/2-src->w/2)*/ ))	/* �^�񒆂ɗ����� */
		if (( src->x256 < (SAKUYA_LIMIT_X_MID_LEFT)) &&
			( src->x256 > (SAKUYA_LIMIT_X_MID_RIGHT)))		/* �^�񒆂ɗ����� */
		{
			data->state1++/*	= SC08*/;
			data->wait2_256 = t256(60);
			data->anime_frame = SAKUYA_ANIME_03_CENTER_A;
		}
		else
		{
			sakuya_idou_sub(src, t256(3), SAKUYA_POINT_01_MID_UP);
			if (SAKUYA_ANIME_03_CENTER_A > data->anime_frame)
			{
				data->wait3 += 1/*fps_fa ctor*/;
				if (data->wait3>=3)
				{
					data->anime_frame++;
					data->wait3=0;
				}
			}
		}
		break;
	case SC08:
		data->wait2_256 -= t256(1);
		if (data->wait2_256 < 0)
		{
		//	data->state1++/*	= SC01*/;
			data->state1	= SC01;
			data->anime_frame = SAKUYA_ANIME_03_CENTER_A;
		}
		else
		{
//			data->move_angle512=atan_512_SAKUYA_MID;
			if (0 == ((t256_floor(data->wait2_256))&(32-1)/*%20*/) )
			{
				int b_wait2_high;
				b_wait2_high=(((int)((t256_floor(data->wait2_256))>>5/*/20*/)));
			//	data->anime_frame++;	data->anime_frame %= 7;//??? 9???
				data->anime_frame++;	data->anime_frame &= (8-1);
				{
					#if 1
					send1_obj->x256 	=	src->x256;
					send1_obj->y256 	=	src->y256;
				//	send1_obj->w128 	=	src->w128;/*�e*/
				//	send1_obj->h128 	=	src->h128;/*�e*/
					#endif
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
				data->anime_frame = SAKUYA_ANIME_15_HATUDAN_END;
			}
		}
		if (data->anime_frame > SAKUYA_ANIME_12_HATUDAN_START)
		{
			data->wait3 += 1/*fps_fa ctor*/;
			if (data->wait3>=4)
			{
				data->anime_frame--;
				data->wait3=0;
			}
		}
		break;
//	case SC09:
//			data->state1	= SC01;
//		break;
	}
}


/*---------------------------------------------------------
	��l�`��: �����w����
---------------------------------------------------------*/

/*static*/ void sakuya_04_keitai(SPRITE *src)
{
	BOSS06_DATA *data = (BOSS06_DATA *)src->data;
	switch (data->state1)
	{
	case SD00:
		{	/* �߂�? */
			boss_clip_min.x256	= t256( 		0)+t256(24);
			boss_clip_max.x256	= t256(GAME_WIDTH)-t256(24);
		}
		data->state1++;
		break;
	case SD0_0b:
		sakuya_wait_state(src, SAKUYA_ANIME_12_HATUDAN_START);
	//	data->wait1 -= 1/*fps_fa ctor*/;
	//	if (data->wait1<0)	/* �ҋ@���Ԃ��I������� */
	//	{
	//		data->state1++/*	= SD01*/;
//++//		pd->bo ssmode=B00_NONE/*B01_BA TTLE*/;
//??//		data->boss_base.boss_timer += (20*64);			// + 20 [�b] ???	/* 16.666[�b] 1000 ???*/
	//		data->anime_frame = SAKUYA_ANIME_12_HATUDAN_START;
	//	}
	//	else	{	}
		break;
	case SD01:
	//	if ( ( src->y > 50) && (src->x<GAME_WIDTH/2+30 || src->x>GAME_WIDTH/2-30))		/* �^�񒆂ɗ����� */
		if (( src->y256 > (SAKUYA_POINT_Y_CENTER-SAKUYA_POINT_Y_MARGIN) ) &&
			( src->x256 < (SAKUYA_LIMIT_X_MID_LEFT)) &&
			( src->x256 > (SAKUYA_LIMIT_X_MID_RIGHT)))		/* �^�񒆂ɗ����� */
		{
			data->state1++/*	= SD02*/;
			data->wait1 	= 10;
			data->wait2_256 = 0;
			data->anime_frame = SAKUYA_ANIME_08_HIKARU_START;
		}
		else
		{
			data->move_angle512=atan_512_SAKUYA_CENTER/*atan_512(50-src->y,GAME_WIDTH/2-src->w-src->x)*/;
#if 0
/* CW�̏ꍇ */
			src->x256 += ((cos512((data->move_angle512))*t256(3))>>8)/**fps_fa ctor*/;
			src->y256 += ((sin512((data->move_angle512))*t256(3))>>8)/**fps_fa ctor*/;
#else
/* CCW�̏ꍇ */
			src->x256 += ((sin512((data->move_angle512))*t256(3))>>8)/**fps_fa ctor*/;
			src->y256 += ((cos512((data->move_angle512))*t256(3))>>8)/**fps_fa ctor*/;
#endif
			if (data->anime_frame < SAKUYA_ANIME_15_HATUDAN_END)
			{
				data->wait3 += 1/*fps_fa ctor*/;
				if (data->wait3>=3)
				{
					data->anime_frame++;
					data->wait3=0;
				}
			}
		}
		break;
	case SD02:
		if (data->anime_frame==SAKUYA_ANIME_11_HIKARU_END)
		{	/* �q�������w����(2�񐶐������Ⴄ�o�O����) */
		//	kodomo_num++;
		//	if (1==kodomo_num)
		//	{
		//	}
			kodomo_num=1;
			data->state1++/*	= SE00*/;/*dummy*/
		//	data->state1		=	0/* = SE00*/;
#if 0/*???*/
/* �X�y���J�[�h�V�X�e���ł͑Ή��ł��Ȃ��B */
			sp ell_card_number++;		/* ���̌`�Ԃ� */
#endif
			data->wait2_256 	= t256(10);
			data->wait1 		= 5;
		}
		else
		{
			if (data->anime_frame < SAKUYA_ANIME_11_HIKARU_END)
			{
				data->wait3 += 1/*fps_fa ctor*/;
				if (data->wait3>=3)
				{
					data->anime_frame++;
					data->wait3=0;
				}
			}
		}
		break;
//	case SD03:/*dummy*/
//		break;
	}
}


/*---------------------------------------------------------
	SPELL_CARD_05_sakuya_eee	��܌`��: (���F�}�X�J�b�g�e)
	SPELL_CARD_06_sakuya_fff	��Z�`��: (���F�}�X�J�b�g�e)
---------------------------------------------------------*/
/*static*/extern void sakuya_sp1_bullet_create_bbb(SPRITE *src);

/*static*/ void sakuya_06_keitai(SPRITE *src)
{
	BOSS06_DATA *data = (BOSS06_DATA *)src->data;
	switch (data->state1)
	{
	case SE00:
		tmp_angle_jikinerai512(player, src);
		data->wait2_256 -= t256(1)/*fps_fa ctor*/;
		if (data->wait2_256<t256(0))
		{
			data->wait2_256=t256(50);
//			int *s_data_angle512;
		//	s_data_angle512 = &(((BOSS06_DATA *)src->data)->boss_base.tmp_angleCCW512);
//			s_data_angle512 = &(src->tmp_angleCCW512);
			/* (���F�}�X�J�b�g�e) */

/* CCW�̏ꍇ */
			sakuya_sp1_bullet_create_bbb(src);
			//bullet_create_oodama0(src, t256(2), ANGLE_JIKINERAI_DAN, 40, 0, t256(src->w/2), t256(src->h/2));
			data->wait1 -= 1/*fps_fa ctor*/;
			if (data->wait1<0)	/* 3�񌂂����� */
			{
				data->wait1=20;
				if (0==(ra_nd()&(2-1)/*%2*/))
				{
					data->state1++/*	= SE01*/;
					data->anime_frame = (SAKUYA_ANIME_03_CENTER_A-2);
				}
				else
				{
					data->state1/*+=2*/ = SE02;
					data->anime_frame = (SAKUYA_ANIME_03_CENTER_A+2);
				}
			}
		}
		if (data->anime_frame > SAKUYA_ANIME_08_HIKARU_START)
		{
			data->wait3 += 1/*fps_fa ctor*/;
			if (data->wait3>=3)
			{
				data->wait3=0;
				data->anime_frame--;
			}
		}
		break;
	case SE01:
		data->wait1 -= 1/*fps_fa ctor*/;
		if (data->wait1<0)
		{
			data->wait1 	= 3;
			data->state1--/*	= SE00*/;
			data->wait2_256=t256(30);
			data->anime_frame = SAKUYA_ANIME_15_HATUDAN_END;
		}
		else
		{
			if (src->x256 < t256(100) )
			{
				data->state1++/* = SE02*/;
				data->anime_frame = SAKUYA_ANIME_07_RIGHT-1/*SAKUYA_ANIME_07_RIGHT-1*/;/*7 ???*/
			}
			else
			{	src->x256 -= t256(2)/**fps_fa ctor*/;}
		}
		break;
	case SE02:
		data->wait1 -= 1/*fps_fa ctor*/;
		if (data->wait1<0)
		{
			data->wait1 	= 3;
			data->state1	= SE00;
			data->wait2_256 = t256(30);
			data->anime_frame = SAKUYA_ANIME_15_HATUDAN_END;
		}
		else
		{
			if (src->x256 > t256(GAME_WIDTH-100)-((src->w128+src->w128)) )
			{
				data->state1--/* = SE01*/;
				data->anime_frame = SAKUYA_ANIME_00_LEFT+1;/*1 ???*/
			}
			else
			{	src->x256 += t256(2)/**fps_fa ctor*/;}
		}
		break;
	}
}


/*---------------------------------------------------------
	SPELL_CARD_07_sakuya_ggg	�掵�`��: (���U�����w)�ǉ��v�撆
	SPELL_CARD_08_sakuya_hhh	�攪�`��: (���~�߃i�C�t)�ǉ��v�撆
---------------------------------------------------------*/

/*---------------------------------------------------------
	SPELL_CARD_09_sakuya_iii	���`��: �ŏI�`��(����1)
---------------------------------------------------------*/
#if 0
static int sakuya_kyori(SPRITE *src)
{
	int rrr256;
	//	���������߂�̂ɂQ�悵�ă��[�g�����̂̓��[�g���Z���x���̂ŁA�H�v���K�v�����B
	//	�\�߃e�[�u�������Ă������A��։��Z(CODIC)�ŋ��߂邩�A���ʊ֐��ɂ���CPU��float�ϊ����߂��ŏ��ɗ��߂邩�A�ǂꂩ�B
	//	data->length_s_p=sqrt((player->x-src->x)*(player->x-src->x)+(player->y-src->y)*(player->y-src->y));
	{	/* sqrt()�x���̂ŊȈՔŁB�Ƃ����������ɂ͍S��Ȃ����ɂ��� */
		int xxx256;
		int yyy256;
		xxx256=abs((int)player->x256-(int)src->x256);/*xxx256+=t256(10);*/ /* abs()�͓W�J���Ă���邩����v */
		yyy256=abs((int)player->y256-(int)src->y256);/*yyy256+=t256(10);*/
		if (xxx256>yyy256)	{rrr256=(xxx256/*+(yyy256>>1)*/);}/*�����������*/
		else				{rrr256=(yyy256/*+(xxx256>>1)*/);}/*�����������*/
	}
	if (t256(40) > rrr256)	{	rrr256=t256(40);}	/* 32 10 */
	else
	if (t256(128) < rrr256) {	rrr256=t256(128);}	/* 128 80 */
	return (rrr256);
}
#endif
/*static*/ void sakuya_09_keitai(SPRITE *src)
{
	BOSS06_DATA *data = (BOSS06_DATA *)src->data;
	switch (data->state1)
	{
	case SG00:
			data->wait1 -= 1/*fps_fa ctor*/;
		if (data->wait1<0)	/* �ҋ@���Ԃ��I������� */
		{
			data->state1++/* = SG01*/;
//++		pd->bo ssmode=B00_NONE/*B01_BA TTLE*/;
			data->move_angle512=atan_512(t256(30)-(src->y256),t256(GAME_WIDTH/2)-((src->w128))-(src->x256));
		}
	//	else		{		}
		break;
	case SG01:
		if ( (src->x256 < t256(GAME_WIDTH/2+30) ) &&
			 (src->x256 > t256(GAME_WIDTH/2-10) ) &&
			 (src->y256 < t256(50)) )	/* �^�񒆂ɗ����� */
		{
			data->state1++/*	= SG02*/;
			data->wait1 	= 20;
			data->wait2_256 = 0;
			data->anime_frame = SAKUYA_ANIME_03_CENTER_A;
		}
		else
		{
			/* CW�̏ꍇ */
			/* CCW�̏ꍇ */
			src->x256 += ((sin512((data->move_angle512))*t256(3))>>8);/**fps_fa ctor*/
			src->y256 += ((cos512((data->move_angle512))*t256(3))>>8);/**fps_fa ctor*/
		}
		break;
	case SG02:
		{
				data->wait1 -= 1/*fps_fa ctor*/;
			if (data->wait1<0)			{	data->state1++/* = SG03*/;
											data->anime_frame=SAKUYA_ANIME_12_HATUDAN_START;	}		/* �ҋ@���Ԃ��I������� */
		//	else if (data->wait1<6) 	{	data->anime_frame=SAKUYA_ANIME_11_HIKARU_END;			}
		//	else if (data->wait1<12)	{	data->anime_frame=SAKUYA_ANIME_13_; 		}
		//	else if (data->wait1<18)	{	data->anime_frame=SAKUYA_ANIME_12_; 		}
		//	else if (data->wait1<24)	{	data->anime_frame=SAKUYA_ANIME_11_; 		}
		//	else if (data->wait1<30)	{	data->anime_frame=SAKUYA_ANIME_10_; 		}
		//	else if (data->wait1<36)	{	data->anime_frame=SAKUYA_ANIME_08_HIKARU_START; 		}
//			else if (data->wait1<36)	{	data->anime_frame=aaa_tbl[data->wait1]; 	}
	//		else if (data->wait1>56)	{	data->anime_frame=SAKUYA_ANIME_08_HIKARU_START; 	}
			else if (data->wait1<32)	{	data->anime_frame=SAKUYA_ANIME_08_HIKARU_START+((data->wait1&0x38)>>3); 	}
		}
		break;
	case SG03:
		if (data->anime_frame==SAKUYA_ANIME_15_HATUDAN_END)
		{
			{
			//	int rrr256;
			//	/*data->length_s_p256*/rrr256 = sakuya_kyori(src);
		//		bullet_create_sakuya_even_knife(src, t256(10), /*data->length_s_p256*/ /*rrr256,*/ 0/*0==�E*/);
		//		bullet_create_sakuya_even_knife(src, t256(10), /*data->length_s_p256*/ /*rrr256,*/ 1/*1==��*/);
				bullet_create_sakuya_ryoute_knife(src);
			}
			data->wait1=40;
			if (src->y256 > t256(150))
			/* CW�̏ꍇ */
		//	{	data->move_angle512=deg_360_to_512(210) + (((ra_nd()&(2048-1))*21)>>8) /*(ra_nd()%(deg_360_to_512(120)))*/;}
			/* CCW�̏ꍇ */
			{	data->move_angle512=deg_360_to_512CCW(360-210) + (((ra_nd()&(2048-1))*21)>>8) /*(ra_nd()%(deg_360_to_512(120)))*/;}
			else
			{	data->move_angle512=/*deg512_2rad*/((ra_nd()&(512-1)))/*degtorad(ra_nd()%360)*/;}
			#if 1
			/*�O�̈�*/
			mask512(data->move_angle512);
			#endif
			data->anime_frame = (SAKUYA_ANIME_12_HATUDAN_START+1);/*16???*/
			data->state1++/*	= SG04*/;
		}
		else
		{
			data->wait3 += 1/*fps_fa ctor*/;
			if (data->wait3>=3)
			{
				data->anime_frame++;
				data->wait3=0;
			}
		}
		break;
	case SG04:
			data->wait1 -= 1/*fps_fa ctor*/;
		if (data->wait1<0)
		{
			data->wait1 	= 120+(3-difficulty)*50;
			data->state1	= SG02;
		}
		else
		{
			/* CCW�̏ꍇ */
			src->x256 += ((sin512((data->move_angle512))*t256(3.5))>>8)/**fps_fa ctor*/;
			src->y256 += ((cos512((data->move_angle512))*t256(3.5))>>8)/**fps_fa ctor*/;
		}
		break;
	}
	if ((player->x256+((player->w128)) < src->x256+((src->w128))+t256(25))&&
		(player->x256+((player->w128)) > src->x256+((src->w128))-t256(25)))
	{	data->wait2_256 += t256(1)/*fps_fa ctor*/;}
	if (data->wait2_256 > t256(20)+(3-difficulty)*t256(30))
	{
		data->anime_frame = (SAKUYA_ANIME_12_HATUDAN_START+1);/*16???*/
	//	data->length_s_p256 = sakuya_kyori(src);
		enemy_boss06_knifes3(src/*,*/ /*1+difficulty,*/ /*(data->length_s_p256)*/ );
	}
//	enemy_boss06_out(src);
}
		//	const Uint8 aaa_tbl[40/*(6*6)*/] =
		//	{
		//		SAKUYA_ANIME_11_HIKARU_END, SAKUYA_ANIME_11_HIKARU_END, SAKUYA_ANIME_11_HIKARU_END, SAKUYA_ANIME_11_HIKARU_END, SAKUYA_ANIME_11_HIKARU_END, SAKUYA_ANIME_11_HIKARU_END,
		//		SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,
		//		SAKUYA_ANIME_12_,	SAKUYA_ANIME_12_,	SAKUYA_ANIME_12_,	SAKUYA_ANIME_12_,	SAKUYA_ANIME_12_,	SAKUYA_ANIME_12_,
		//		SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,
		//		SAKUYA_ANIME_10_,	SAKUYA_ANIME_10_,	SAKUYA_ANIME_10_,	SAKUYA_ANIME_10_,	SAKUYA_ANIME_10_,	SAKUYA_ANIME_10_,
		//		SAKUYA_ANIME_08_HIKARU_START,	SAKUYA_ANIME_08_HIKARU_START,	SAKUYA_ANIME_08_HIKARU_START,	SAKUYA_ANIME_08_HIKARU_START,	SAKUYA_ANIME_08_HIKARU_START,	SAKUYA_ANIME_08_HIKARU_START,
		//		0,0,0,0/* �A���C���p�_�~�[�f�[�^�[�B dummy for align*/
		//	};
//			const Uint8 aaa_tbl[40/*(6*6)*/] =
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
	SPELL_CARD_10_sakuya_jjj	��10�`��: �ŏI�`��(����2)
---------------------------------------------------------*/

/*static*/ void sakuya_10_keitai(SPRITE *src)
{
	BOSS06_DATA *data = (BOSS06_DATA *)src->data;
	if (data->wait2_256 < t256(0) )
	{
		data->wait2_256 = t256(0);
		data->wait1 	= 120;
#if 0
/* �X�y���J�[�h�V�X�e���ł͑Ή��ł��Ȃ��B */
		sp ell_card_number = SPELL_CARD_09_sakuya_iii/*4*/;
#endif
		spell_card_number++;
		data->state1	= SG02;
		data->anime_frame = SAKUYA_ANIME_03_CENTER_A;
	}
	else
	{
		data->wait2_256 -= t256(0.03);
		if (src->color32 > 0xf9000000)	/*	if (src->alpha > 0xf9)	*/	/* (src->alpha>0xff)*/
		{
			src->color32 = 0xffffffff;	/*	src->alpha = 0xff;*/
		}
		else /*if (src->alpha<0xff)*/
		{
			src->color32 += 0x05000000; /*	src->alpha += 0x05;*/
		}
		/* CW�̏ꍇ */
		/* CCW�̏ꍇ */
		src->x256 += ((sin512((src->tmp_angleCCW512))*(data->wait2_256))>>8)/**fps_fa ctor*/;
		src->y256 += ((cos512((src->tmp_angleCCW512))*(data->wait2_256))>>8)/**fps_fa ctor*/;
	}
	if (data->anime_frame > SAKUYA_ANIME_12_HATUDAN_START)
	{
		data->wait3 += 1/*fps_fa ctor*/;
		if (data->wait3 >= 4)
		{
			data->anime_frame--;
			data->wait3 = 0;
		}
	}
//	enemy_boss06_out(src);
}

/*---------------------------------------------------------
	SPELL_CARD_11_sakuya_kkk	��10�`��: �ŏI�`��(����3)
---------------------------------------------------------*/

/*static*/ void sakuya_11_keitai(SPRITE *src)
{
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
	�{�X�s��
---------------------------------------------------------*/

static void move_sakuya(SPRITE *src)
{
	/* �X�y�J�o�^ */
	/* �����w��������off */
	{
	//	BOSS06_DATA *data = (BOSS06_DATA *)src->data;
	//	if (SPELL_CARD_04_sakuya_ddd != spell_card_number)	/* ��l�`��: �����w���� */
		{
			if (0/*off*/==spell_card_mode)
			{
				regist_spell_card222(src);
			}
		}
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
//
	boss_effect(src);			/* ��G�t�F�N�g */
	danmaku_generator(src); 	/* �e������ */
//
	#if 0/*�\�Z��{�l�f�o�b�O�p*/
	/* �p�l���̃X�R�A����move_type���A�O���C�Y����state1��\��������B���Ă����������������Ⴄ�B */
	((PLAYER_DATA *)player->data)->my_score 	= sp ell_card_number;
	((PLAYER_DATA *)player->data)->graze_point	= data->state1;
	#endif
}


/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

void add_boss_sakuya(STAGE_DATA *l)/*int lv*/
{
	boss_bgm_mode		= (l->user_y);
//
//----[ZAKO]
	common_boss_flags	= (FLG_ALL_CAST);
//
	kodomo_num	= 0;

//
//	send1_obj								= sprite_add_res(BASE_BOSS_SAKUYA_PNG);
	send1_obj->flags						&= (~(SP_FLAG_VISIBLE));
//	send1_obj->an im_frame					= SAKUYA_ANIME_03_CENTER_A;
	send1_obj->type 						= SP_MUTEKI;
//	send1_obj->callback_mover				= dummy_move;
	send1_obj->x256 						= 0;
	send1_obj->y256 						= 0;

//
//----[BOSS]
		SPRITE *sakuya;
	//	sakuya								= spr ite_add_file 0("boss/sakuya.png"/*"boss04.png"*/, 19, PRIORITY_03_ENEMY, 0); sakuya->anim_speed=0;
	//	sakuya								= sprite_add_res(BASE_BOSS_SAKUYA_PNG);
		sakuya								= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
		sakuya->flags						|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
	//	sakuya->yx_an im_frame				= SAKUYA_ANIME_03_CENTER_A;
		sakuya->type						= BOSS_00_BOSS11;	/*SP_BOSS*/ 	/*SP_BOSS06*/
		sakuya->callback_mover				= move_sakuya;
		sakuya->callback_loser				= sakuya_put_items;
//
		#if 0/* �������ς݂̕K�v���� */
		sakuya->vx256						= t256( 0);
		sakuya->vy256						= t256( 0); 	/**fps_fa ctor*/
		#endif
		sakuya->base_health 				= ((1024)-1)*9;/*test*/
		sakuya->base_score					= adjust_score_by_difficulty(score(5000000));	/* 500�� */
	//------------ �X�y�J�֘A
		spell_card_number					= SPELL_CARD_00_sakuya_000;
		spell_card_max						= SPELL_CARD_11_sakuya_kkk;
		spell_card_boss_init_regist(sakuya);
	{
		BOSS06_DATA *data;
		data								= mmalloc(sizeof(BOSS06_DATA));
		sakuya->data						= data;
		data->anime_frame					= SAKUYA_ANIME_03_CENTER_A;
		data->state1						= /*ST_00*/0/*SA00*/;
		data->wait1 						= 0;
		data->wait2_256 					= t256(0);
		data->wait3 						= 0;
	//	data->aaa_type						= 0;
		data->move_angle512 				= (0);
		#if 1
	//------------ �e���֘A
		data->boss_base.danmaku_type		= 0;
		data->boss_base.danmaku_time_out	= 0;
		#endif
	}
	//----[ZAKO]
	//	create_doll(src);		/* �q�������w����(2�񐶐������Ⴄ�o�O����) */

	/*---------------------------------------------------------
		�q�������w ���� 	�q�������w�O���t�B�b�N����
	---------------------------------------------------------*/

	//static void create_doll(SPRITE *sakuya)
	{
		/*static*/ SPRITE *obj_doll;		// �q�������w�̃X�v���C�g
//		obj_doll							= sprite_add_res(BASE_MAHOUJIN_0_PNG);	//obj_doll->anim_speed	= 0;/*"boss04-lo.png"*/
		obj_doll							= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG); //obj_doll->anim_speed	= 0;/*"boss04-lo.png"*/
	//	obj_doll->type						= TEKI_51_MAHOJIN1;
		obj_doll->type						= BOSS_16_YOUSEI11; 	/*SP_ZAKO*/ 	/*SP_BOSS01*/
//		obj_doll->type						= SP_MUTEKI;
		obj_doll->base_health				= 999999;/* �|���Ȃ� */
		obj_doll->flags 					|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
//		obj_doll->an im_frame				= 0;
		obj_doll->color32					= 0x00ffffff;	/*	obj_doll->alpha 		= 0x00;*/
		obj_doll->callback_mover			= move_doll00;
		obj_doll->x256						= sakuya->x256+((sakuya->w128-obj_doll->w128));
		obj_doll->y256						= sakuya->y256+((sakuya->h128-obj_doll->h128))-t256(32);

		{
			BOSS06_DOLL_DATA *data;
			data							= mmalloc(sizeof(BOSS06_DOLL_DATA));
			obj_doll->data					= data;
			data->state_d02 				= 0;
			data->state_d01 				= 0x00;
			data->wait						= 0;
			data->angle512					= (0);
			#if (1==USE_KEISYOU)
			data->sakuya_obj				= sakuya;	/* �p�������� */
			#endif
			data->nnn						= 0;
			data->hen_no_kazu				= (DOLL_HEN_MAX);
		}
	}

}
//		spell_card_boss_timer				= (20*64);			/*1000*/	// + 20 [�b] ???	/* 16.666[�b] 1000 ???*/
//		spell_card_boss_timer				= (40*64);			/* 40*64==40[count] 	��40[�b(64/60)](�P�ʂ͕b�ł͂Ȃ�) */
//		spell_card_boss_timer				= ((99-24)*64); 	/* 75*64==75[count] 	��75[�b(64/60)](�P�ʂ͕b�ł͂Ȃ�) */
	/*score(5000)+score(4000)*difficulty*/

//		data->boss_base.bo ss_life			= ((/*5*/SPELL_CARD_10_sakuya_jjj));			/*5119==((5*1024)-1)*/
//		sakuya->base_health 				= ((/*5*/SPELL_CARD_10_sakuya_jjj*1024)-1); 	/*5119==((5*1024)-1)*/
//		sakuya->base_health 				= ((1024)-1);
