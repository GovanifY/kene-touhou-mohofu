
#include "bullet_object.h"

/*---------------------------------------------------------
	�\�Z�� ���
	PAD
	-------------------------------------------------------
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
//	int level;
	int move_angle512;
	int length_s_p256;	/* ���ƃv���C���[�Ƃ̋��� */

} BOSS06_DATA;
//	int aaa_type;		/* 0: normal, 1: destroyed */
#define move_vx256 move_angle512


#if 1
/* �\���̂̊O�ɂ���K�R�������� */
	#define FLG_NO_DOLL 	(0)
	#define FLG_ALL_CAST	(1)
static int mahoujin_num;
//static SPRITE *dummy_obj; /*dummy_obj*/
#endif

enum
{
//--- ���^�ȑO
	MH00 = 0,
	MH01,
	MH02,
	MH03,
//--- ���^����
	MH04,
//--- ���^�ȍ~
	MH05,
	MH06,
	MH07,
	MH08,
	MH09,
	MH10,
	MH11,
	MH12,
	MH13,
	MH14,
	MH15,
	MH16,
	MH17,
	MH18,
	MH19,
	MH20,
//
	MH21,
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
	SA06aaa,
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



enum
{
	SAKUYA_ANIME_00_LEFT = 0,
	SAKUYA_ANIME_01_,
	SAKUYA_ANIME_02_,
	SAKUYA_ANIME_03_,
	SAKUYA_ANIME_04_CENTER_A,
//
	SAKUYA_ANIME_05_CENTER_B,
	SAKUYA_ANIME_06_,
	SAKUYA_ANIME_07_,
	SAKUYA_ANIME_08_,
	SAKUYA_ANIME_09_RIGHT,
//
	SAKUYA_ANIME_09_ = 0x10,
	SAKUYA_ANIME_10_,/*0x11*/
	SAKUYA_ANIME_11_,/*0x12*/
	SAKUYA_ANIME_12_,/*0x13*/
	SAKUYA_ANIME_13_,/*0x14*/
	SAKUYA_ANIME_14_,/*0x15*/
//
	SAKUYA_ANIME_15_HATUDAN,/*0x16*/
	SAKUYA_ANIME_16_,/*0x17*/
	SAKUYA_ANIME_17_,/*0x18*/
	SAKUYA_ANIME_18_,/*0x19*/
};


/*--------------------------------------*/
/*--------------------------------------*/
/*--------------------------------------*/


/*---------------------------------------------------------

---------------------------------------------------------*/

typedef struct /*_boss06_maho_data*/
{
	#if (1==USE_KEISYOU)
	SPRITE *sakuya_obj; 		/* �p�� */
	#endif
	int state_m01;	/* �s�� */
	int state_m02;	/* �`�� */
	int nnn;
	int mmm;
	int wait;
	int angle512;
//
	int sss;
	int jjj;
	int maho_vx;
	int maho_vy;
	int maho_point_x;			/* �ڕW���W */
	int maho_point_y;			/* �ڕW���W */
	int maho_point_angle512;
//
//	int tx256;
//	int ty256;
} BOSS06_MAHO_DATA;


/*---------------------------------------------------------
	�����w�ړ�
---------------------------------------------------------*/

/*static*/extern void enemy_sp2_bullet_create(SPRITE *src, int angle512, int gra256, int *sd_nnn);
static void enemy_boss06_maho_move(SPRITE *src)
{
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
	BOSS06_MAHO_DATA *data = (BOSS06_MAHO_DATA *)src->data;
	if (data->state_m02 < MH05)
	{
		#if (1==USE_KEISYOU)
		src->x256 = data->sakuya_obj->x256+((data->sakuya_obj->w128-src->w128));
		src->y256 = data->sakuya_obj->y256+((data->sakuya_obj->h128-src->h128));
		#else
		src->x256 = ((((PLAYER_DATA *)player->data)->boss))->x256+((((((PLAYER_DATA *)player->data)->boss))->w128-src->w128));
		src->y256 = ((((PLAYER_DATA *)player->data)->boss))->y256+((((((PLAYER_DATA *)player->data)->boss))->h128-src->h128));
		#endif
	//
		#if (1==USE_KEISYOU)
	//	if (((BOSS06_DATA *)data->sakuya_obj->data)->move_type==SAKUYA_09_KEITAI/*4*/ /*���^��`���Ă�������*/)
		#else
	//	if (((BOSS06_DATA *)((((PLAYER_DATA *)player->data)->boss))->data)->move_type==SAKUYA_09_KEITAI/*4*/ /*���^��`���Ă�������*/)
		if (spell_card_number==SAKUYA_09_KEITAI/*4*/ /*���^��`���Ă�������*/)
		#endif
		{
			static int hosi_gata_kakuze_counter;
			hosi_gata_kakuze_counter--;
			if (hosi_gata_kakuze_counter < 0)	/*���^��`���Ă�������*/
			{
				hosi_gata_kakuze_counter = (60*10);
			//	data->state_m02 = MH05;/*���^��`����*/
				data->state_m02 = MH04;/*���^��`����*/
			}
		}
	}
	switch (data->state_m02)
	{
//------------------------------------------
	case MH00:
		src->color32 += 0x03000000; 		/*	src->alpha += 0x03;*/
		if (0x96000000 < src->color32)		/*	(0x96 < src->alpha) */
		{
			src->color32		= 0x96ffffff;	/*	src->alpha		= 0x96;*/
			data->state_m02++/* = MH01*/;
			data->wait		= 50;
		}
		break;

	case MH01:
		data->wait -= 1/*fps_fa ctor*/;
		if (data->wait < 1)
		{
			data->state_m02++/* = MH02*/;
			data->state_m01 = 0x100;
		}
		break;
	case MH02:
		if (0==(data->state_m01&0x0f/*%10*/))
		{
#if 0
/* CW�̏ꍇ */
			bullet_create_tomari2_dan(src, (data->state_m01/*>>4*/<<3/*1/10*/)/**256*/, data->angle512, 					  t256(0.02), data->angle512+(data->state_m01)-deg_360_to_512((180	)));	//ra d2deg512((data->state_m01)>>7/*/100*/)
			bullet_create_tomari2_dan(src, (data->state_m01/*>>4*/<<3/*1/10*/)/**256*/, data->angle512+deg_360_to_512((120)), t256(0.02), data->angle512+(data->state_m01)-deg_360_to_512((180+120)));	//ra d2deg512((data->state_m01)>>7/*/100*/)
			bullet_create_tomari2_dan(src, (data->state_m01/*>>4*/<<3/*1/10*/)/**256*/, data->angle512-deg_360_to_512((120)), t256(0.02), data->angle512+(data->state_m01)-deg_360_to_512((180-120)));	//ra d2deg512((data->state_m01)>>7/*/100*/)
#else
/* CCW�̏ꍇ */
			bullet_create_tomari2_dan(src, (data->state_m01/*>>4*/<<3/*1/10*/)/**256*/, data->angle512, 						 t256(0.02), data->angle512+(data->state_m01)+deg_360_to_512CCW((180	)));	//ra d2deg512((data->state_m01)>>7/*/100*/)
			bullet_create_tomari2_dan(src, (data->state_m01/*>>4*/<<3/*1/10*/)/**256*/, data->angle512-deg_360_to_512CCW((120)), t256(0.02), data->angle512+(data->state_m01)+deg_360_to_512CCW((180+120)));	//ra d2deg512((data->state_m01)>>7/*/100*/)
			bullet_create_tomari2_dan(src, (data->state_m01/*>>4*/<<3/*1/10*/)/**256*/, data->angle512+deg_360_to_512CCW((120)), t256(0.02), data->angle512+(data->state_m01)+deg_360_to_512CCW((180-120)));	//ra d2deg512((data->state_m01)>>7/*/100*/)
#endif
		}
		data->state_m01--;
		if (data->state_m01 < 1)
		{
			data->state_m02++/* = MH03*/;
			data->wait		= 15;
			data->state_m01 = 0x10;
		}
		break;
	case MH03:
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
			data->state_m01--;
		}
		if (data->state_m01<0x0)
		{
			data->state_m02--/* = MH02*/;
			data->state_m01 = 0x50;
		}
		break;

//------------------------------------------

	/*
	���^��1 190, 48
	���^��2 248, 217
	���^��3 100, 113
	���^��4 280, 113
	���^��5 138, 217
	*/
#define MAHO_JIN_WAIT50 		(16/*16*/)
#define MAHO_JIN_SPEED10		(4/*8*/)
#define MAHO_JIN_SET03			(1)
#define MAHO_JIN_ADD05			(1)

#define MAHO_JIN_POINT_CX00 	(t256(GAME_WIDTH/2))
#define MAHO_JIN_POINT_CY00 	(t256(/*GAME_HEIGHT/4*/64))
#define MAHO_JIN_RADIUS_SIZE	(/*t256*/(90))

//------------------------------------------

	case MH04:	/* ���^��`������ */
		data->state_m02++/* = MH05*/;
		data->maho_point_angle512=0;
		data->maho_vx = t256(1.0);
		data->maho_vy = t256(0.0);
		#if (1==USE_KEISYOU)
		src->x256 = (data->sakuya_obj->x256);		dummy_obj->x256 = src->x256;
		src->y256 = (data->sakuya_obj->y256);		dummy_obj->y256 = src->y256;
		#else
		src->x256 = (((((PLAYER_DATA *)player->data)->boss))->x256);		dummy_obj->x256 = src->x256;
		src->y256 = (((((PLAYER_DATA *)player->data)->boss))->y256);		dummy_obj->y256 = src->y256;
		#endif
		data->sss = 0;
		break;
//------------------------------------------
	case MH05:	/* ���������܂ő҂� */
		src->anim_frame = 0;
//			if (2==data->nnn/*%(4-difficulty)*/)/* %2 == (5-3:Lunatic) */
//			{	src->anim_frame = 1;}
//			else
//			{	src->anim_frame = 0;}
		#if (1==USE_KEISYOU)
		if (((BOSS06_DATA *)data->sakuya_obj->data)->state1==1)
		#else
		if (((BOSS06_DATA *)((((PLAYER_DATA *)player->data)->boss))->data)->state1==1)
		#endif
		{
			data->state_m02++/* = MH06*/;
			data->state_m01 = 0x0;
		}
		break;
//------------------------------------------
	case MH06:	/* ���^��`���� */
//		data->maho_point_x=MAHO_JIN_POINT_CX00+sin512((data->maho_point_angle512))*MAHO_JIN_RADIUS_SIZE;
//		data->maho_point_y=MAHO_JIN_POINT_CY00+cos512((data->maho_point_angle512))*MAHO_JIN_RADIUS_SIZE;

		/*	*/
		dummy_obj->x256 += (data->maho_vx)/**fps_fa ctor*/;
		dummy_obj->y256 += (data->maho_vy)/**fps_fa ctor*/;
		data->sss++;
		if (
	//		(t256(32) > abs(data->maho_point_x-src->x256)) &&
	//		(t256(32) > abs(data->maho_point_y-src->y256))
				(16 < data->sss)
		)
		{
			data->sss = 0;
			data->maho_point_angle512 += (512*2/5);
			mask512(data->maho_point_angle512);
			{
			/* CCW�̏ꍇ */
		//	int d_angle512=atan_512(data->maho_point_y-src->y256-((src->h128)), data->maho_point_x-src->x256-((src->w128)));/*��ʒ��S����*/
			data->maho_vx = ((sin512((data->maho_point_angle512/*d_angle512*/))*/*t256*/(/*3*/MAHO_JIN_SPEED10/*+difficulty*/))/*>>8*/)/**fps_fa ctor*/;
			data->maho_vy = ((cos512((data->maho_point_angle512/*d_angle512*/))*/*t256*/(/*3*/MAHO_JIN_SPEED10/*+difficulty*/))/*>>8*/)/**fps_fa ctor*/;
			}

		//
			data->state_m02++/* = MH05*/;
		//
			data->mmm++;
			if (5 < data->mmm)
			{
				data->mmm = 0;
			//	data->mmm &= 1;
				data->nnn++;	if (data->nnn>(/*4-0*/3/*difficulty*/) )	{data->nnn=0/*1*/ /*0*/;}
				#if (1==USE_KEISYOU)
				src->x256 = (data->sakuya_obj->x256);		dummy_obj->x256 = src->x256;
				src->y256 = (data->sakuya_obj->y256);		dummy_obj->y256 = src->y256;
				#else
				src->x256 = (((((PLAYER_DATA *)player->data)->boss))->x256);		dummy_obj->x256 = src->x256;
				src->y256 = (((((PLAYER_DATA *)player->data)->boss))->y256);		dummy_obj->y256 = src->y256;
				#endif
			//	data->tx256 = MAHO_JIN_POINT_CX00;
			//	data->ty256 = MAHO_JIN_POINT_CY00;
			//	dummy_obj->x256 = data->tx256;
			//	dummy_obj->y256 = data->ty256;
			}
		//
			data->wait		= MAHO_JIN_WAIT50;
			data->state_m01 = 0x0;
		}
		else
		{
			data->state_m01+=/*5*/MAHO_JIN_ADD05/*3*/;
			if (0/*1*/==data->nnn)
			{
				if (0==(data->state_m01&/*7*/MAHO_JIN_SET03/*%5*/))
				{
					/* CW�̏ꍇ */
				//	enemy_sp2_bullet_create(src, deg_360_to_512(3)+(( ((-data->state_m01*2/**data->mmm*/)) +data->state_m01)>>3/*/5*/), t256(0.015), &(((BOSS06_MAHO_DATA *)src->data)->nnn) );
/* CCW�̏ꍇ */
					enemy_sp2_bullet_create(dummy_obj,
						data->maho_point_angle512,	//	-deg_360_to_512CCW(3)+(( ((-data->state_m01*2/**data->mmm*/)) +data->state_m01)>>3/*/5*/),
						t256(0.015),
						&(((BOSS06_MAHO_DATA *)src->data)->nnn)
					);
				}
			}
		}
		break;
	case MH07:
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
			bullet_create_n_way_dan_sa_type(src,
				(t256(1.2)/*(speed256)*/),
				(0/*aaa_angle512*/),
				(int)(512/(18)),		/* �p�x([360/360]�x��18����) */
				BULLET_KNIFE20_06_YUKARI/*BULLET_KNIFE20_04_AOI*/,
				(18));/* [18way�s���N�i�C�t�e] */
			}
		}
		data->state_m02++/* = MH08*/;
		break;
	case MH08:	/* �ꎞ��~ */
	//	hosi_wait(src); static void hosi_wait(SPRITE *src)
		{
		//	BOSS06_MAHO_DATA *data = (BOSS06_MAHO_DATA *)src->data;
		//	if (data->wait<0)	{	data->state_m02++/* = MH09*/;}	else	{	data->wait--;}
			data->wait--;	if (data->wait<0)	{	data->state_m02++/* = MH09*/;}
		}
		break;
	case MH09:
		data->state_m02 = MH06;
		data->state_m01 = 0x00;
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
	/* �p�l���̃X�R�A����state_m02���A�O���C�Y����state_m01��\��������B���Ă����������������Ⴄ�B */
	((PLAYER_DATA *)player->data)->my_score 	= data->state_m02;
	((PLAYER_DATA *)player->data)->graze_point	= data->state_m01;
	#endif
}


/*---------------------------------------------------------
	�����w����		�����w�O���t�B�b�N����
---------------------------------------------------------*/

static void create_mahoujin(SPRITE *sakuya)
{
	SPRITE *s2; 		// �����w�̃X�v���C�g
	s2					= sprite_add_res(BASE_MAHOUJIN_0_PNG);	//s2->anim_speed	= 0;/*"boss04-lo.png"*/
	s2->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
	s2->anim_frame		= 0;
	s2->type			= SP_MUTEKI;
	BOSS06_MAHO_DATA *data;
	data				= mmalloc(sizeof(BOSS06_MAHO_DATA));
	s2->data			= data;
	data->state_m02 	= 0;
	data->state_m01 	= 0x00;
	data->wait			= 0;
	data->angle512		= (0);
	#if (1==USE_KEISYOU)
	data->sakuya_obj	= sakuya;	/* �p�������� */
	#endif
	data->nnn			= 0;
	data->mmm			= 0;
	s2->color32 		= 0x00ffffff;	/*	s2->alpha			= 0x00;*/
	s2->callback_mover	= enemy_boss06_maho_move;
	s2->x256			= sakuya->x256+((sakuya->w128-s2->w128));
	s2->y256			= sakuya->y256+((sakuya->h128-s2->h128));
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
	//tama->anim_frame	= SAKUYA_ANIME_04_CENTER_A;//�o�O(�����Ńv���C���[�V���b�g�̃A�j���p�^�[����M��̂ŁA���~�{���Ńn���O�A�b�v����)
	src->anim_frame 	= SAKUYA_ANIME_04_CENTER_A;//���̃A�j���p�^�[����M��
	BOSS06_DATA *data = (BOSS06_DATA *)src->data;
	data->state1		= 0;/* ���ʂ��� �l�� 0 �ɂ��� */
	data->wait1 		= 80;
	data->wait2_256 	= t256(-100);
	data->wait3 		= 0;//???

	#if (1)
	draw_spell_card_name();
	#endif
}
	#if (0)
	spell_card_number++;
	#endif
	#if (0)
//	/*data->boss_base.boss_*/src->base_health	= (1024-1); 	/* life_point����Q�[�W��[ */
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
		/*data->boss_base.*/spell_card_boss_timer		= aaa_timer[(data->boss_base.bo ss_life)];		/* 40*64==40[count] 	��40[�b(64/60)](�P�ʂ͕b�ł͂Ȃ�) */
	}	/* �{�X�^�C�}�[������ */
	// 40 60 [�b] ???	4096/*2500*/	/*3000*/;//???
	#endif

/*---------------------------------------------------------

---------------------------------------------------------*/

/*static*/extern void sakuya_sp1_bullet_create(SPRITE *src, int speed256, int angle512, int gra256, int r_or_l/*, int *sd_angle512*/);


/*---------------------------------------------------------

---------------------------------------------------------*/

static void sakuya_wait_state(SPRITE *src, /*int nextstate,*/ int anim_frame)
{
	BOSS06_DATA *data = (BOSS06_DATA *)src->data;
	data->wait1 -= 1/*fps_fa ctor*/;
	if (0 < data->wait1)
	{;}
	else
	{
		data->state1++/* = nextstate*/;
		src->anim_frame = anim_frame;
	}
}


/*---------------------------------------------------------
	�e�����I���܂ő҂B
	-------------------------------------------------------

---------------------------------------------------------*/

static void danmaku_state_check_holding(SPRITE *src)	/*, int nextstate*/ /*, int anim_frame*/
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
	s->anim_frame = SAKUYA_ANIME_04_CENTER_A;
	data->wait3 = 0;
	�ŏ���������Ă�K�v������B
---------------------------------------------------------*/

static void sakuya_anime_to_sayuu(SPRITE *src)
{
	BOSS06_DATA *data = (BOSS06_DATA *)src->data;
	if (0 < data->move_vx256)
	{
		/* �E�ֈړ� */
		if (src->anim_frame==SAKUYA_ANIME_09_RIGHT)
		{
			data->state1++/*	= SB06*/;/*�����܂�*/
		}
		else
		{
			data->wait3 += 1/*fps_fa ctor*/;
			if (data->wait3>=3)
			{
				data->wait3=0;
				src->anim_frame++;/*???*/
			}
		}
	}
	else
	{
		/* ���ֈړ� */
		if (src->anim_frame==SAKUYA_ANIME_00_LEFT)
		{
			data->state1++/*	= SB06*/;/*�����܂�*/
		}
		else
		{
			data->wait3 += 1/*fps_fa ctor*/;
			if (data->wait3>=3)
			{
				data->wait3=0;
				src->anim_frame--;/*???*/
			}
		}
	}
}


/*---------------------------------------------------------
	���	�e�����A�j���ҋ@
	-------------------------------------------------------
	s->anim_frame = SAKUYA_ANIME_15_HATUDAN;
	data->wait3 = 0;
	�ŏ���������Ă�K�v������B
---------------------------------------------------------*/

static void sakuya_tama_anime_wait3_0(SPRITE *src)
{
	BOSS06_DATA *data = (BOSS06_DATA *)src->data;
	if (src->anim_frame==SAKUYA_ANIME_18_)
	{
		data->state1++/*	= SB06*/;
	}
	else
	{
		data->wait3 += 1/*fps_fa ctor*/;
		if (data->wait3>=3)
		{
			data->wait3=0;
			src->anim_frame++;/*???*/
		}
	}
}


/*---------------------------------------------------------

---------------------------------------------------------*/

static void enemy_boss06_out(SPRITE *src)
{
	if (src->x256 < t256(50))												{	src->x256 = t256(50);}
	if (src->x256 > t256(GAME_WIDTH)-t256(50)-((src->w128+src->w128)))		{	src->x256 = t256(GAME_WIDTH)-t256(50)-((src->w128+src->w128));}
	if (src->y256 < t256(10))												{	src->y256 = t256(10);}
	if (src->y256 > t256(GAME_HEIGHT)-t256(120)+((src->h128+src->h128)))	{	src->y256 = t256(GAME_HEIGHT)-t256(120)+((src->h128+src->h128));}
}


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
			/*data->boss_base.*/src->tmp_angleCCW512	= (ii<<6);
			src->x256			= tmp1_256;
			src->y256			= tmp2_256;
			src->color32		= 0x00ffffff;			/*	src->alpha			= 0x00;*/
//			src->anim_frame 	= SAKUYA_ANIME_18_;//?????
			spell_card_number	= SAKUYA_10_KEITAI/*5*/;/*???*/
			data->wait2_256 	= /*speed256*/t256(1)+(difficulty<<8);
		}
		else
		{
		//	dou ble tmp_s_x;
		//	dou ble tmp_s_y;
		//	tmp_s_x = s->x;
		//	tmp_s_y = s->y;
			/* �v���C���[�_���e */
#if 0
/* CW�̏ꍇ */
			src->x256		=	tmp_x256 + ((cos512((ii<<6))*len256)>>8);
			src->y256		=	tmp_y256 + ((sin512((ii<<6))*len256)>>8);
#else
/* CCW�̏ꍇ */
			src->x256		=	tmp_x256 + ((sin512((ii<<6))*len256)>>8);
			src->y256		=	tmp_y256 + ((cos512((ii<<6))*len256)>>8);
#endif
			bullet_create_sakuya_follow_knife1/*2*/(src,
				/*speed256*/t256(2.0),
				(ii<<6)/*i*M_PI*2*1/8*/,			/*angle512*/
				-30+(ii&(2-1)/*%2*/)*60);			/* -30 or 30 */
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
	SAKUYA_01_KEITAI	�ʏ�U��(�S�e���A����Ƃ͂����Ď኱�Ⴄ)	���`��: ���E�ɓ����ď��e����
---------------------------------------------------------*/

static void sakuya_01_keitai(SPRITE *src)
{
	BOSS06_DATA *data = (BOSS06_DATA *)src->data;
	PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
	static int my_yudo_point;

/* ���`�Ԃł͎g���Ė����݂��������痘�p���� */
//#define _mode wait2_256
	switch (data->state1)
	{
	case SA00:	/* ������ */
		src->y256 += t256(1)/*fps_fa ctor*/;
		if (t256(16/*20*/) < src->y256) 	/* y �� t256(20) �܂ŗ����� */
		{
		/* �o�ꎞ�ɂ͒e�������Ȃ� */
		//	bullet_create_aka_lines(s);
			src->anim_frame = SAKUYA_ANIME_04_CENTER_A/*0*/;
		//	pd->bo ssmode=B05_BE FORE_LOAD;
			pd->state_flag |= STATE_FLAG_10_IS_LOAD_SCRIPT;
			data->state1++/*	= SA01*/;
		}
		break;
	/*���S�Ɏp������*/
	case SA01:	/* */
	//	if (pd->bo ssmode==B01_BA TTLE)
	//	if ((STATE_FLAG_05_IS_BOSS|0) == (pd->state_flag&(STATE_FLAG_05_IS_BOSS|STATE_FLAG_06_IS_SCRIPT)))
		if ( ((((PLAYER_DATA *)player->data)->state_flag) & STATE_FLAG_05_IS_BOSS) )
		{
			/* �v���C���[�e�󂯕t���A�R�[���o�b�N��o�^ */
			src->callback_hit_enemy = callback_hit_boss;	/* �R�[���o�b�N�o�^ */
		//	/*���Ԑ����J�E���g�L����*/
		//	data->state1++/*	= SA02*/;
			data->state1	= SA12;
		}
		break;
/*----------------*/
	case SA02:	/* ���ֈړ� */
		//if (pd->bo ssmode==B01_BA TTLE)
	//	{
		if (my_yudo_point < src->x256 )/*t256(50)*/
		{
			src->x256 -= t256(2.0/*3.0*/)/*fps_fa ctor*/;
			if (SAKUYA_ANIME_00_LEFT < src->anim_frame)
			{
				data->wait3 += 1/*fps_fa ctor*/;
				if (SAKUYA_ANIME_04_CENTER_A < data->wait3)
				{
					src->anim_frame--;
					data->wait3 = 0;
				}
			}
		}
		else	/* x �� 50 �ȉ��ɂȂ����� */
		{
			data->state1++/*	= SA03*/;
	#if 1/* �ϒe�� */
			tmp_angle_jikinerai512(player, src);/* ���@�˂炢�p�쐬 */
		#if 0
		/*CW*/
			/*data->boss_base.*/src->tmp_angleCCW512	-= 128;
		#else
		/*CCW*/
			/*data->boss_base.*/src->tmp_angleCCW512	+= (256-128);
		#endif
	#else/* �Œ�e�� */
		/*CCW*/
			/*data->boss_base.*/src->tmp_angleCCW512	= deg_360_to_512CCW((90));/* �E�����猂���n�߂� (128)*/
	#endif
			data->boss_base.danmaku_type		= DANMAKU_01_sakuya;	/* �S�e�����Z�b�g */
			data->boss_base.danmaku_time_out	= DANMAKU_01_SET_TIME;	/* �S�e���̔������� */
	//		src->anim_frame 		= SAKUYA_ANIME_04_CENTER_A;
			src->anim_frame 		= SAKUYA_ANIME_15_HATUDAN;
		}
	//	}
		break;
	case SA03:	/* �e�����A�j���ҋ@ */
		sakuya_tama_anime_wait3_0(src);
		break;
	case SA04:	/* �S�e�����I���܂őҋ@ */
		danmaku_state_check_holding(src/*, SA04*/ /*, SAKUYA_ANIME_04_CENTER_A*/);
		break;
	case SA05:
		src->anim_frame 		= SAKUYA_ANIME_04_CENTER_A;
		data->state1++/*	= SA06*/;
		break;
	case SA06:	/* �^�񒆂ɖ߂� */
		if (SAKUYA_POINT_X_MID > src->x256 )
		{
			src->x256 += t256(3.0/*3.0*/)/*fps_fa ctor*/;
			if (SAKUYA_ANIME_09_RIGHT > src->anim_frame)
			{
				data->wait3 += 1/*fps_fa ctor*/;
				if (SAKUYA_ANIME_04_CENTER_A < data->wait3)
				{
					src->anim_frame++;
					data->wait3 = 0;
				}
			}
		}
		else	/* �^�񒆂܂ŗ����� */
		{
			data->state1++/*	= SA05*/;
		//	if (RANK_NORMAL<difficulty) 	{	bullet_create_aka_lines(s); 	}
		}
		break;
/*----------------*/
	case SA06aaa:/* �ړ��ʒu�����߂� */
			data->state1++/*	= SA05*/;
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
			data->state1++/*	= SA06*/;
	#if 1/* �ϒe�� */
			tmp_angle_jikinerai512(player, src);/* ���@�˂炢�p�쐬 */
		#if 0
		/*CW*/
			/*data->boss_base.*/src->tmp_angleCCW512	-= 128;
		#else
		/*CCW*/
			/*data->boss_base.*/src->tmp_angleCCW512	+= (256-128);
		#endif
	#else/* �Œ�e�� */
		/*CCW*/
			/*data->boss_base.*/src->tmp_angleCCW512	= deg_360_to_512CCW((90));/* �E�����猂���n�߂� (128)*/
	#endif
			data->boss_base.danmaku_type		= DANMAKU_01_sakuya;	/* �S�e�����Z�b�g */
			data->boss_base.danmaku_time_out	= DANMAKU_01_SET_TIME;	/* �S�e���̔������� */
		//	src->anim_frame 		= SAKUYA_ANIME_04_CENTER_A;
			src->anim_frame 		= SAKUYA_ANIME_15_HATUDAN;
		}
		break;
	case SA08:	/* �e�����A�j���ҋ@ */
		sakuya_tama_anime_wait3_0(src);
		break;
	case SA09:	/* �S�e�����I���܂őҋ@ */
		danmaku_state_check_holding(src/*, SA07*/ /*, SAKUYA_ANIME_04_CENTER_A*/);
		break;
	case SA10:
		src->anim_frame 		= SAKUYA_ANIME_04_CENTER_A;
		data->state1++/*	= SA06*/;
		break;
	case SA11:	/* �^�񒆂ɖ߂� */
		if (SAKUYA_POINT_X_MID < src->x256 )
		{
			src->x256 -= t256(3.0/*3.0*/)/*fps_fa ctor*/;
			if (SAKUYA_ANIME_00_LEFT < src->anim_frame)
			{
				data->wait3 += 1/*fps_fa ctor*/;
				if (SAKUYA_ANIME_04_CENTER_A < data->wait3)
				{
					src->anim_frame--;
					data->wait3 = 0;
				}
			}
		}
		else	/* �^�񒆂ɖ߂� */
		{
			data->state1++/* = SA02*/;
		//	if (RANK_NORMAL<difficulty) 	{	bullet_create_aka_lines(s); 	}
		}
		break;
/*----------------*/
	case SA12:/* �ړ��ʒu�����߂� */
		data->state1	= SA02;
	//	my_yudo_point = SAKUYA_POINT_X_LEFT_MID;
		my_yudo_point = SAKUYA_POINT_X_MID-(ra_nd()&((128*256)-1));
		break;
	}
}


/*---------------------------------------------------------
	SAKUYA_02_KEITAI	��p�u�~�X�f�B���N�V�����v	���`��: �S���ʁA���܂�
---------------------------------------------------------*/

static void sakuya_02_keitai(SPRITE *src)
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
//		sakuya_wait_state(src, SAKUYA_ANIME_04_CENTER_A);
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
//				if (ang512>deg_360_to_512(90))		{	ang512+=deg_360_to_512(90); 	src->anim_frame=(SAKUYA_ANIME_04_CENTER_A+1); }
//				else								{									src->anim_frame=(SAKUYA_ANIME_04_CENTER_A-1); }
//				ang512 -= deg_360_to_512(45);
			}
			else
			{
				data->move_vx256 = -t256(1.0/*0.5*/);
//				if (src->x256 > t256(GAME_WIDTH/2) )	{	ang512=deg_360_to_512(180); 	src->anim_frame=(SAKUYA_ANIME_04_CENTER_A+2); }
//				else									{	ang512=deg_360_to_512(0);		src->anim_frame=(SAKUYA_ANIME_04_CENTER_A-2); }
			}
//			data->move_angle512=/*deg512_2rad*/(ang512);
		}
//		data->wait1 		= 20+(3-difficulty)*10;
		data->wait1 		= (35);/* [WAITSTATE] */	/* �e�����Ȃ��ł�����ƈړ� */
//		data->wait2_256 += t256(1);
		//
		src->anim_frame = SAKUYA_ANIME_04_CENTER_A;
		data->wait3=0;
		data->state1++/*	= SB02*/;
		break;

	case SB02:		/* �ړ� */
		src->x256 += ((data->move_vx256))/**fps_fa ctor*/;	/* �ړ� */	/* �e�����Ȃ��ł�����ƈړ� */
		sakuya_anime_to_sayuu(src);
		break;
	case SB03:
		src->x256 += ((data->move_vx256))/**fps_fa ctor*/;	/* �ړ� */	/* �e�����Ȃ��ł�����ƈړ� */
		sakuya_wait_state(src/*, SB05*/, SAKUYA_ANIME_15_HATUDAN);
		break;
	case SB04:
		src->anim_frame = SAKUYA_ANIME_15_HATUDAN;
		data->wait3 = 0;
		data->state1++/*	= SB02*/;
		break;
	case SB05:	/* �e�����A�j���ҋ@ */
		sakuya_tama_anime_wait3_0(src);
		break;
	case SB06:
		//	enemy_boss06_mamemaki_state(src, SB07, SAKUYA_ANIME_18_);
			data->boss_base.danmaku_type		= DANMAKU_02_sakuya;	/* 24�e�����Z�b�g */
			data->boss_base.danmaku_time_out	= DANMAKU_02_SET_TIME;	/* 24�e���̔������� */
			data->state1++/*	= SB07*/;
		break;
	case SB07:	/* 24�e�����I���܂ňړ� */
		src->x256 += ((data->move_vx256))/**fps_fa ctor*/;	/* �ړ� */	/* �e�����Ȃ���ړ� */
		danmaku_state_check_holding(src/*, SA07*/ /*, SAKUYA_ANIME_04_CENTER_A*/);
		break;
	case SB08:
			src->x256 = t256(GAME_WIDTH)-src->x256; 			/* ���Α��փ��[�v */
			src->anim_frame = SAKUYA_ANIME_15_HATUDAN;
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
			src->anim_frame 		= SAKUYA_ANIME_04_CENTER_A;
			data->state1++/*	= SB07*/;
		break;
	case SB11:	/* 11�e�����I���܂őҋ@ */
		danmaku_state_check_holding(src/*, SA07*/ /*, SAKUYA_ANIME_04_CENTER_A*/);
		break;
	case SB12:	/* (SB12��)�����ҋ@ */
		sakuya_wait_state(src/*, SB05*/, SAKUYA_ANIME_04_CENTER_A);
		break;
	case SB13:
		data->state1	= SB01;
		break;
	}
	enemy_boss06_out(src);
}


/*---------------------------------------------------------
	SAKUYA_03_KEITAI	���J�u�o�[�e�B�J���i�C�t�v		��O�`��: �����i�C�t�������Ă����
---------------------------------------------------------*/

static void sakuya_03_keitai(SPRITE *src)
{
	BOSS06_DATA *data = (BOSS06_DATA *)src->data;
	switch (data->state1)
	{
	case SC00:
		sakuya_wait_state(src, SAKUYA_ANIME_04_CENTER_A);
	//	data->wait1 -= 1/*fps_fa ctor*/;
	//	if (data->wait1<0)	/* �ҋ@���Ԃ��I������� */
	//	{
	//		data->state1++/*	= SC01*/;
//++//		pd->bossmode		= B00_NONE/*B01_BATTLE*/;
	//		src->anim_frame 	= SAKUYA_ANIME_04_CENTER_A;
	//	}
	//	else	{	}
		break;
	case SC01:
		if (src->x256 < (SAKUYA_LIMIT_X_LEFT)/*5*/ )	/* �����ɗ����� */	/* �� || (src->y<5)������Ɩ������[�v�ɂȂ�*/
		{
			data->state1++/*	= SC02*/;
			data->wait1 	= 10;
			data->wait2_256 = t256(0);
			src->anim_frame = SAKUYA_ANIME_00_LEFT;
		}
		else
		{
			sakuya_idou_sub(src, t256(3), SAKUYA_POINT_00_LEFT);
			if (src->anim_frame>SAKUYA_ANIME_00_LEFT)
			{
				data->wait3 += 1/*fps_fa ctor*/;
				if (data->wait3>=3)
				{
					src->anim_frame--;
					data->wait3=0;
				}
			}
		}
		break;
	case SC02:
		sakuya_wait_state(src/*, SC03*/, SAKUYA_ANIME_02_/*???*/);
		break;
	case SC03:
		if (src->x256 > (SAKUYA_LIMIT_X_RIGHT)/*GAME_WIDTH-src->w+3*/ ) /* �E�[�ɗ����� */
		{
			data->state1++/*	= SC04*/;
			data->wait2_256 = t256(0);
			src->anim_frame = SAKUYA_ANIME_09_RIGHT;
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
				//	if (difficulty>1)
					{	bullet_create_jyuryoku_dan000(src, /*(0.7)*/t256(2)/*(2)*/, (deg_360_to_512(180)-((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)), t256(0.02), BULLET_KNIFE20_05_AKA);}
				}
#else
/* CCW�̏ꍇ */
				bullet_create_jyuryoku_dan000(src, /*(0.7)*/t256(2)/*(2)*/, (-deg_360_to_512CCW(360)-((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)), t256(0.03), BULLET_KNIFE20_05_AKA);
				//if (difficulty)
				{
					bullet_create_jyuryoku_dan000(src, /*(0.7)*/t256(2)/*(2)*/, (-deg_360_to_512CCW(270)-((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)), t256(0.04), BULLET_KNIFE20_05_AKA);
				//	if (difficulty>1)
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
			if (src->anim_frame<SAKUYA_ANIME_09_RIGHT)
			{
				data->wait3 += 1/*fps_fa ctor*/;
				if (data->wait3>=7)
				{
					src->anim_frame++;
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
			src->anim_frame = SAKUYA_ANIME_04_CENTER_A;
		}
		else
		{
			sakuya_idou_sub(src, t256(3), SAKUYA_POINT_01_MID_UP);
			if (SAKUYA_ANIME_04_CENTER_A < src->anim_frame)
			{
				data->wait3 += 1/*fps_fa ctor*/;
				if (data->wait3>=3)
				{
					src->anim_frame--;
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
			src->anim_frame = SAKUYA_ANIME_09_RIGHT;
		}
		else
		{
			sakuya_idou_sub(src, t256(3), SAKUYA_POINT_03_RIGHT);
			if (src->anim_frame<SAKUYA_ANIME_09_RIGHT)
			{
				data->wait3 += 1/*fps_fa ctor*/;
				if (data->wait3>=3)
				{
					src->anim_frame++;
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
			src->anim_frame = SAKUYA_ANIME_00_LEFT;
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
					if (difficulty>1)
					{	bullet_create_jyuryoku_dan000(src, t256(3), (((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)), t256(0.02), BULLET_KNIFE20_05_AKA);}
				}
#else
/* CCW�̏ꍇ */
				bullet_create_jyuryoku_dan000(src, t256(3), (((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)+deg_360_to_512CCW(360-180)), t256(0.03), BULLET_KNIFE20_05_AKA);
				if (difficulty)
				{
					bullet_create_jyuryoku_dan000(src, t256(3), (((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)+deg_360_to_512CCW(360-90)), t256(0.04), BULLET_KNIFE20_05_AKA);
					if (difficulty>1)
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
			if (src->anim_frame>SAKUYA_ANIME_00_LEFT)
			{
				data->wait3 += 1/*fps_fa ctor*/;
				if (data->wait3>=7)
				{
					src->anim_frame--;
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
			src->anim_frame = SAKUYA_ANIME_04_CENTER_A;
		}
		else
		{
			sakuya_idou_sub(src, t256(3), SAKUYA_POINT_01_MID_UP);
			if (SAKUYA_ANIME_04_CENTER_A > src->anim_frame)
			{
				data->wait3 += 1/*fps_fa ctor*/;
				if (data->wait3>=3)
				{
					src->anim_frame++;
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
			src->anim_frame = SAKUYA_ANIME_04_CENTER_A;
		}
		else
		{
//			data->move_angle512=atan_512_SAKUYA_MID;
			if (0 == ((t256_floor(data->wait2_256))&(32-1)/*%20*/) )
			{
				int b_wait2_high;
				b_wait2_high=(((int)((t256_floor(data->wait2_256))>>5/*/20*/)));
			//	src->anim_frame++;	src->anim_frame %= 7;//??? 9???
				src->anim_frame++;	src->anim_frame &= (8-1);
				{
#if 0
/* CW�̏ꍇ */
					bullet_create_sakuya_follow_knife1(src, (b_wait2_high<<8)+t256(4.0), (deg_360_to_512((180-30))+((b_wait2_high)*deg_360_to_512(40))), -80);
					bullet_create_sakuya_follow_knife1(src, (b_wait2_high<<8)+t256(3.0), (deg_360_to_512((180-30))+((b_wait2_high)*deg_360_to_512(30))), -60);
					if (difficulty>1)
					{
						bullet_create_sakuya_follow_knife1(src, (b_wait2_high<<8)+t256(2.0), (deg_360_to_512((180-30))+((b_wait2_high)*deg_360_to_512(20))), -40);
					}
#else
/* CCW�̏ꍇ */
						bullet_create_sakuya_follow_knife1(src, (b_wait2_high<<8)+t256(4.0), deg_360_to_512CCW(/*360+*/90-30)+(b_wait2_high)*deg_360_to_512CCW(40), -80);
						bullet_create_sakuya_follow_knife1(src, (b_wait2_high<<8)+t256(3.0), deg_360_to_512CCW(/*360+*/90-30)+(b_wait2_high)*deg_360_to_512CCW(30), -60);
					if (difficulty>1)
					{
						bullet_create_sakuya_follow_knife1(src, (b_wait2_high<<8)+t256(2.0), deg_360_to_512CCW(/*360+*/90-30)+(b_wait2_high)*deg_360_to_512CCW(20), -40);
					}
#endif
				}
				src->anim_frame = SAKUYA_ANIME_18_;
			}
		}
		if (src->anim_frame>SAKUYA_ANIME_15_HATUDAN)
		{
			data->wait3 += 1/*fps_fa ctor*/;
			if (data->wait3>=4)
			{
				src->anim_frame--;
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

static void sakuya_04_keitai(SPRITE *src)
{
	BOSS06_DATA *data = (BOSS06_DATA *)src->data;
	switch (data->state1)
	{
	case SD00:
		sakuya_wait_state(src, SAKUYA_ANIME_15_HATUDAN);
	//	data->wait1 -= 1/*fps_fa ctor*/;
	//	if (data->wait1<0)	/* �ҋ@���Ԃ��I������� */
	//	{
	//		data->state1++/*	= SD01*/;
//++//		pd->bo ssmode=B00_NONE/*B01_BA TTLE*/;
//??//		data->boss_base.boss_timer += (20*64);			// + 20 [�b] ???	/* 16.666[�b] 1000 ???*/
	//		src->anim_frame=SAKUYA_ANIME_15_HATUDAN;
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
			src->anim_frame = SAKUYA_ANIME_09_;
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
			if (src->anim_frame<SAKUYA_ANIME_18_)
			{
				data->wait3 += 1/*fps_fa ctor*/;
				if (data->wait3>=3)
				{
					src->anim_frame++;
					data->wait3=0;
				}
			}
		}
		break;
	case SD02:
		if (src->anim_frame==SAKUYA_ANIME_14_)
		{
			mahoujin_num++;
			if (1==mahoujin_num)
			{
				create_mahoujin(src);		/* �����w����(2�񐶐������Ⴄ�o�O����) */
			}
		//	data->state1++/*	= SE00*/;
			data->state1		=	0/* = SE00*/;
			spell_card_number++;		/* ���̌`�Ԃ� */
			data->wait2_256 	= t256(10);
			data->wait1 		= 5;
		}
		else
		{
			if (src->anim_frame<SAKUYA_ANIME_14_)
			{
				data->wait3 += 1/*fps_fa ctor*/;
				if (data->wait3>=3)
				{
					src->anim_frame++;
					data->wait3=0;
				}
			}
		}
		break;
	}
}


/*---------------------------------------------------------
	enemy_boss06_knifes1
	�A���O���͊p�x�w���(-90)�`(90)
	height��"player����"�̍���
---------------------------------------------------------*/

#if 0
static void enemy_boss06_knifes1(SPRITE *src, int speed256, int angle512, int height)
{
	/*	*/
	/*	*/
#if 0
/* CW�̏ꍇ */
//	bullet_create_sakuya_follow_knife1(src, speed256, /*deg_360_to_512*/(deg_360_to_512((180))-angle512), height);
#else
/* CCW�̏ꍇ */
//	bullet_create_sakuya_follow_knife1(src, speed256, /*deg_360_to_512*/(-deg_360_to_512CCW((180))-angle512), height);
	bullet_create_sakuya_follow_knife1(src, speed256, /*deg_360_to_512*/(/*deg_360_to_512CCW(360+90-(180))-*/angle512), height);
#endif
//	bullet_create_sakuya_follow_knife1(src, speed256, /*deg_360_to_512*/(angle512), height);
}
#endif


/*---------------------------------------------------------
	SAKUYA_05_KEITAI_B	��܌`��: (���F�}�X�J�b�g�e)
	SAKUYA_06_KEITAI_C	��Z�`��: (���F�}�X�J�b�g�e)
---------------------------------------------------------*/

static void sakuya_06_keitai(SPRITE *src)
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
#if 0
/* CW�̏ꍇ */
			sakuya_sp1_bullet_create(src, (t256(0.70)), /*data->boss_base.*/src->tmp_angleCCW512-1*42/*deg_360_to_512(30)*/, (t256(0.030)),  1/* 1==R*/ /*, s_data_angle512*/ );/*0.72 == 1.2*0.6*/
			sakuya_sp1_bullet_create(src, (t256(0.70)), /*data->boss_base.*/src->tmp_angleCCW512+1*42/*deg_360_to_512(30)*/, (t256(0.030)), -1/*-1==L*/ /*, s_data_angle512*/ );/*0.72 == 1.2*0.6*/
			sakuya_sp1_bullet_create(src, (t256(0.85)), /*data->boss_base.*/src->tmp_angleCCW512-2*42/*deg_360_to_512(30)*/, (t256(0.033)),  1/* 1==R*/ /*, s_data_angle512*/ );/*0.9 == 1.5*0.6*/
			sakuya_sp1_bullet_create(src, (t256(0.85)), /*data->boss_base.*/src->tmp_angleCCW512+2*42/*deg_360_to_512(30)*/, (t256(0.033)), -1/*-1==L*/ /*, s_data_angle512*/ );/*0.9 == 1.5*0.6*/
			if (difficulty)
			{
				sakuya_sp1_bullet_create(src, (t256(1.0)), /*data->boss_base.*/src->tmp_angleCCW512-3*42/*deg_360_to_512(30)*/, (t256(0.036)),	1/* 1==R*/ /*, s_data_angle512*/ );/*1.08 == 1.8*0.6*/
				sakuya_sp1_bullet_create(src, (t256(1.0)), /*data->boss_base.*/src->tmp_angleCCW512+3*42/*deg_360_to_512(30)*/, (t256(0.036)), -1/*-1==L*/ /*, s_data_angle512*/ );/*1.08 == 1.8*0.6*/
				if (1<difficulty)
				{
					sakuya_sp1_bullet_create(src, (t256(1.15)), /*data->boss_base.*/src->tmp_angleCCW512-4*42/*deg_360_to_512(30)*/, (t256(0.04)),	1/* 1==R*/ /*, s_data_angle512*/ );/*1.26 == 2.1*0.6*/
					sakuya_sp1_bullet_create(src, (t256(1.15)), /*data->boss_base.*/src->tmp_angleCCW512+4*42/*deg_360_to_512(30)*/, (t256(0.04)), -1/*-1==L*/ /*, s_data_angle512*/ );/*1.26 == 2.1*0.6*/
				}
			}
#else
/* CCW�̏ꍇ */
			sakuya_sp1_bullet_create(src, (t256(0.70)), /*data->boss_base.*/src->tmp_angleCCW512+1*42/*deg_360_to_512(30)*/, (t256(0.030)),  1/* 1==R*/ /*, s_data_angle512*/ );/*0.72 == 1.2*0.6*/
			sakuya_sp1_bullet_create(src, (t256(0.70)), /*data->boss_base.*/src->tmp_angleCCW512-1*42/*deg_360_to_512(30)*/, (t256(0.030)), -1/*-1==L*/ /*, s_data_angle512*/ );/*0.72 == 1.2*0.6*/
			sakuya_sp1_bullet_create(src, (t256(0.85)), /*data->boss_base.*/src->tmp_angleCCW512+2*42/*deg_360_to_512(30)*/, (t256(0.033)),  1/* 1==R*/ /*, s_data_angle512*/ );/*0.9 == 1.5*0.6*/
			sakuya_sp1_bullet_create(src, (t256(0.85)), /*data->boss_base.*/src->tmp_angleCCW512-2*42/*deg_360_to_512(30)*/, (t256(0.033)), -1/*-1==L*/ /*, s_data_angle512*/ );/*0.9 == 1.5*0.6*/
			if (difficulty)
			{
				sakuya_sp1_bullet_create(src, (t256(1.0)), /*data->boss_base.*/src->tmp_angleCCW512+3*42/*deg_360_to_512(30)*/, (t256(0.036)),	1/* 1==R*/ /*, s_data_angle512*/ );/*1.08 == 1.8*0.6*/
				sakuya_sp1_bullet_create(src, (t256(1.0)), /*data->boss_base.*/src->tmp_angleCCW512-3*42/*deg_360_to_512(30)*/, (t256(0.036)), -1/*-1==L*/ /*, s_data_angle512*/ );/*1.08 == 1.8*0.6*/
				if (1<difficulty)
				{
					sakuya_sp1_bullet_create(src, (t256(1.15)), /*data->boss_base.*/src->tmp_angleCCW512+4*42/*deg_360_to_512(30)*/, (t256(0.04)),	1/* 1==R*/ /*, s_data_angle512*/ );/*1.26 == 2.1*0.6*/
					sakuya_sp1_bullet_create(src, (t256(1.15)), /*data->boss_base.*/src->tmp_angleCCW512-4*42/*deg_360_to_512(30)*/, (t256(0.04)), -1/*-1==L*/ /*, s_data_angle512*/ );/*1.26 == 2.1*0.6*/
				}
			}
#endif
			//bullet_create_oodama0(src, t256(2), ANGLE_JIKINERAI_DAN, 40, 0, t256(src->w/2), t256(src->h/2));
			data->wait1 -= 1/*fps_fa ctor*/;
			if (data->wait1<0)	/* 3�񌂂����� */
			{
				data->wait1=20;
				if (0==(ra_nd()&(2-1)/*%2*/))
				{
					data->state1++/*	= SE01*/;
					src->anim_frame=(SAKUYA_ANIME_04_CENTER_A-2);
				}
				else
				{
					data->state1/*+=2*/ = SE02;
					src->anim_frame=(SAKUYA_ANIME_04_CENTER_A+2);
				}
			}
		}
		if (src->anim_frame>SAKUYA_ANIME_09_)
		{
			data->wait3 += 1/*fps_fa ctor*/;
			if (data->wait3>=3)
			{
				data->wait3=0;
				src->anim_frame--;
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
			src->anim_frame=SAKUYA_ANIME_18_;
		}
		else
		{
			if (src->x256 < t256(100) )
			{
				data->state1++/* = SE02*/;
				src->anim_frame=SAKUYA_ANIME_09_RIGHT-1/*SAKUYA_ANIME_08_RIGHT-1*/;/*7 ???*/
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
			src->anim_frame = SAKUYA_ANIME_18_;
		}
		else
		{
			if (src->x256 > t256(GAME_WIDTH-100)-((src->w128+src->w128)) )
			{
				data->state1--/* = SE01*/;
				src->anim_frame=SAKUYA_ANIME_00_LEFT+1;/*1 ???*/
			}
			else
			{	src->x256 += t256(2)/**fps_fa ctor*/;}
		}
		break;
	}
}


/*---------------------------------------------------------
	SAKUYA_06_KEITAI	�掵�`��: (���U�����w)�ǉ��v�撆
	SAKUYA_07_KEITAI	�攪�`��: (���~�߃i�C�t)�ǉ��v�撆
---------------------------------------------------------*/

/*---------------------------------------------------------
	SAKUYA_09_KEITAI	���`��: �ŏI�`��(����1)
---------------------------------------------------------*/

static void sakuya_09_keitai(SPRITE *src)
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
		else
		{
		}
		break;
	case SG01:
		if ( (src->x256 < t256(GAME_WIDTH/2+30) ) &&
			 (src->x256 > t256(GAME_WIDTH/2-10) ) &&
			 (src->y256 < t256(50)) )	/* �^�񒆂ɗ����� */
		{
			data->state1++/*	= SG02*/;
			data->wait1 	= 20;
			data->wait2_256 = 0;
			src->anim_frame=SAKUYA_ANIME_04_CENTER_A;
		}
		else
		{
#if 0
/* CW�̏ꍇ */
			src->x256 += ((cos512((data->move_angle512))*t256(3))>>8);/**fps_fa ctor*/
			src->y256 += ((sin512((data->move_angle512))*t256(3))>>8);/**fps_fa ctor*/
#else
/* CCW�̏ꍇ */
			src->x256 += ((sin512((data->move_angle512))*t256(3))>>8);/**fps_fa ctor*/
			src->y256 += ((cos512((data->move_angle512))*t256(3))>>8);/**fps_fa ctor*/
#endif
		}
		break;
	case SG02:
		{
			const Uint8 aaa_tbl[40/*(6*6)*/] =
			{
				SAKUYA_ANIME_14_,	SAKUYA_ANIME_14_,	SAKUYA_ANIME_14_,	SAKUYA_ANIME_14_,	SAKUYA_ANIME_14_,	SAKUYA_ANIME_14_,
				SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,
				SAKUYA_ANIME_12_,	SAKUYA_ANIME_12_,	SAKUYA_ANIME_12_,	SAKUYA_ANIME_12_,	SAKUYA_ANIME_12_,	SAKUYA_ANIME_12_,
				SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,
				SAKUYA_ANIME_10_,	SAKUYA_ANIME_10_,	SAKUYA_ANIME_10_,	SAKUYA_ANIME_10_,	SAKUYA_ANIME_10_,	SAKUYA_ANIME_10_,
				SAKUYA_ANIME_09_,	SAKUYA_ANIME_09_,	SAKUYA_ANIME_09_,	SAKUYA_ANIME_09_,	SAKUYA_ANIME_09_,	SAKUYA_ANIME_09_,
				0,0,0,0/* �A���C���p�_�~�[�f�[�^�[�B dummy for align*/
			};
				data->wait1 -= 1/*fps_fa ctor*/;
			if (data->wait1<0)			{	data->state1++/* = SG03*/;
											src->anim_frame=SAKUYA_ANIME_15_HATUDAN;	}		/* �ҋ@���Ԃ��I������� */
		//	else if (data->wait1<6) 	{	src->anim_frame=SAKUYA_ANIME_14_;			}
		//	else if (data->wait1<12)	{	src->anim_frame=SAKUYA_ANIME_13_;			}
		//	else if (data->wait1<18)	{	src->anim_frame=SAKUYA_ANIME_12_;			}
		//	else if (data->wait1<24)	{	src->anim_frame=SAKUYA_ANIME_11_;			}
		//	else if (data->wait1<30)	{	src->anim_frame=SAKUYA_ANIME_10_;			}
		//	else if (data->wait1<36)	{	src->anim_frame=SAKUYA_ANIME_09_;			}
			else if (data->wait1<36)	{	src->anim_frame=aaa_tbl[data->wait1];		}
		}
		break;
	case SG03:
		if (src->anim_frame==SAKUYA_ANIME_18_)
		{
		//	���������߂�̂ɂQ�悵�ă��[�g�����̂̓��[�g���Z���x���̂ŁA�H�v���K�v�����B
		//	�\�߃e�[�u�������Ă������A��։��Z(CODIC)�ŋ��߂邩�A���ʊ֐��ɂ���CPU��float�ϊ����߂��ŏ��ɗ��߂邩�A�ǂꂩ�B
		//	data->length_s_p=sqrt((player->x-src->x)*(player->x-src->x)+(player->y-src->y)*(player->y-src->y));
			{	/* sqrt()�x���̂ŊȈՔŁB�Ƃ����������ɂ͍S��Ȃ����ɂ��� */
				int xxx256;
				int yyy256;
				xxx256=abs(player->x256-src->x256);xxx256+=256;/* abs()�͓W�J���Ă���邩����v */
				yyy256=abs(player->y256-src->y256);yyy256+=256;
				if (xxx256>yyy256)	{data->length_s_p256=(xxx256/*+(yyy256>>1)*/);}/*�����������*/
				else				{data->length_s_p256=(yyy256/*+(xxx256>>1)*/);}/*�����������*/
			}
#if 1
			bullet_create_sakuya_even_knife(src, t256(10), data->length_s_p256, 0/*0==�E*/);
			#if 1
			bullet_create_sakuya_even_knife(src, t256(10), data->length_s_p256, 1/*1==��*/);
			#endif
#endif
			data->wait1=40;
			if (src->y256 > t256(150))
#if 0
/* CW�̏ꍇ */
			{	data->move_angle512=deg_360_to_512(210) + (((ra_nd()&(2048-1))*21)>>8) /*(ra_nd()%(deg_360_to_512(120)))*/;}
#else
/* CCW�̏ꍇ */
			{	data->move_angle512=deg_360_to_512CCW(360-210) + (((ra_nd()&(2048-1))*21)>>8) /*(ra_nd()%(deg_360_to_512(120)))*/;}
#endif
			else
			{	data->move_angle512=/*deg512_2rad*/((ra_nd()&(512-1)))/*degtorad(ra_nd()%360)*/;}
			#if 1
			/*�O�̈�*/
			mask512(data->move_angle512);
			#endif
			src->anim_frame = SAKUYA_ANIME_16_;/*16???*/
			data->state1++/*	= SG04*/;
		}
		else
		{
			data->wait3 += 1/*fps_fa ctor*/;
			if (data->wait3>=3)
			{
				src->anim_frame++;
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
#if 0
/* CW�̏ꍇ */
			src->x256 += ((cos512((data->move_angle512))*t256(3.5))>>8)/**fps_fa ctor*/;
			src->y256 += ((sin512((data->move_angle512))*t256(3.5))>>8)/**fps_fa ctor*/;
#else
/* CCW�̏ꍇ */
			src->x256 += ((sin512((data->move_angle512))*t256(3.5))>>8)/**fps_fa ctor*/;
			src->y256 += ((cos512((data->move_angle512))*t256(3.5))>>8)/**fps_fa ctor*/;
#endif
		}
		break;
	}
	if ((player->x256+((player->w128)) < src->x256+((src->w128))+t256(25))&&
		(player->x256+((player->w128)) > src->x256+((src->w128))-t256(25)))
	{	data->wait2_256 += t256(1)/*fps_fa ctor*/;}
	if (data->wait2_256 > t256(20)+(3-difficulty)*t256(30))
	{
		src->anim_frame=SAKUYA_ANIME_16_;/*16???*/
	//	���������߂�̂ɂQ�悵�ă��[�g�����̂̓��[�g���Z���x���̂ŁA�H�v���K�v�����B
	//	�\�߃e�[�u�������Ă������A��։��Z(CODIC)�ŋ��߂邩�A���ʊ֐��ɂ���CPU��float�ϊ����߂��ŏ��ɗ��߂邩�A�ǂꂩ�B
	//	data->length_s_p=sqrt((player->x-src->x)*(player->x-src->x)+(player->y-src->y)*(player->y-src->y));
			{	/* sqrt()�x���̂ŊȈՔŁB�Ƃ����������ɂ͍S��Ȃ����ɂ��� */
				int xxx256;
				int yyy256;
				xxx256=abs((int)player->x256-(int)src->x256);xxx256+=256;/* abs()�͓W�J���Ă���邩����v */
				yyy256=abs((int)player->y256-(int)src->y256);yyy256+=256;
				if (xxx256>yyy256)	{data->length_s_p256=(xxx256/*+(yyy256>>1)*/);}/*�����������*/
				else				{data->length_s_p256=(yyy256/*+(xxx256>>1)*/);}/*�����������*/
			}
		if (data->length_s_p256>t256(80))
		{	data->length_s_p256=t256(80);}
		enemy_boss06_knifes3(src/*,*/ /*1+difficulty,*/ /*(data->length_s_p256)*/ );
	}
	enemy_boss06_out(src);
}


/*---------------------------------------------------------
	SAKUYA_10_KEITAI	��10�`��: �ŏI�`��(����2)
---------------------------------------------------------*/

static void sakuya_10_keitai(SPRITE *src)
{
	BOSS06_DATA *data = (BOSS06_DATA *)src->data;
	if (data->wait2_256 < t256(0) )
	{
		data->wait2_256 = t256(0);
		data->wait1 	= 120;
		spell_card_number = SAKUYA_09_KEITAI/*4*/;
		data->state1	= SG02;
		src->anim_frame = SAKUYA_ANIME_04_CENTER_A;
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
#if 0
/* CW�̏ꍇ */
		src->x256 += ((cos512((/*data->boss_base.*/src->tmp_angleCCW512))*(data->wait2_256))>>8)/**fps_fa ctor*/;
		src->y256 += ((sin512((/*data->boss_base.*/src->tmp_angleCCW512))*(data->wait2_256))>>8)/**fps_fa ctor*/;
#else
/* CCW�̏ꍇ */
		src->x256 += ((sin512((/*data->boss_base.*/src->tmp_angleCCW512))*(data->wait2_256))>>8)/**fps_fa ctor*/;
		src->y256 += ((cos512((/*data->boss_base.*/src->tmp_angleCCW512))*(data->wait2_256))>>8)/**fps_fa ctor*/;
#endif
	}
	if (src->anim_frame>SAKUYA_ANIME_15_HATUDAN)
	{
		data->wait3 += 1/*fps_fa ctor*/;
		if (data->wait3 >= 4)
		{
			src->anim_frame--;
			data->wait3 = 0;
		}
	}
	enemy_boss06_out(src);
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
		spell_card_number++;
		if (SAKUYA_10_KEITAI < spell_card_number)
		{
			spell_card_number--;
		}
		sakuya_put_items(src/*,t*/);/**/
	}
}


/*---------------------------------------------------------
	[�X�y�J�V�X�e�����Ɉړ��\��]	�X�y�J����
---------------------------------------------------------*/

static void spell_card_generator(SPRITE *src)
{
//	BOSS06_DATA *data = (BOSS06_DATA *)src->data;
	switch (spell_card_number)
	{
	case SAKUYA_01_KEITAI/*0*/: 		sakuya_01_keitai(src);		break;	/* ���`��: ���E�ɓ����ď��e���� */
	case SAKUYA_02_KEITAI/*1*/: 		sakuya_02_keitai(src);		break;	/* ���`��: �S���ʁA���܂� */
	case SAKUYA_03_KEITAI/*2*/: 		sakuya_03_keitai(src);		break;	/* ��O�`��: �����i�C�t�������Ă���� */
	case SAKUYA_04_KEITAI_A/*3*/:		sakuya_04_keitai(src);		break;	/* ��l�`��: �����w���� */
//------------------------
	//	���܂�ɒZ���̂ŏ�����������	/* �����w����(2�񐶐������Ⴄ�o�O����) */
	case SAKUYA_05_KEITAI_B/*3*/:											/* ��܌`��: (���F�}�X�J�b�g�e) */
	case SAKUYA_06_KEITAI_C/*4*/:		sakuya_06_keitai(src);		break;	/* ��Z�`��: (���F�}�X�J�b�g�e) */
	case SAKUYA_09_KEITAI/*5 4*/:		sakuya_09_keitai(src);		break;	/* ���`��: �ŏI�`��(����1) */
	case SAKUYA_10_KEITAI/*6 5*/:		sakuya_10_keitai(src);		break;	/* ��10�`��: �ŏI�`��(����2) */
	}
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
	�{�X�s��
---------------------------------------------------------*/

static void move_sakuya(SPRITE *src)
{
	/* �X�y�J�o�^ */
	/* �����w��������off */
	{
	//	BOSS06_DATA *data = (BOSS06_DATA *)src->data;
		if (SAKUYA_04_KEITAI_A != spell_card_number)	/* ��l�`��: �����w���� */
		{
			if (0/*off*/==spell_card_mode)
			{
				regist_spell_card(src);
			}
		}
	}
	spell_card_generator(src);	/* �X�y�J���� */
	danmaku_generator(src); 	/* �e������ */
//
	#if 0/*�\�Z��{�l�f�o�b�O�p*/
	/* �p�l���̃X�R�A����move_type���A�O���C�Y����state1��\��������B���Ă����������������Ⴄ�B */
	((PLAYER_DATA *)player->data)->my_score 	= spell_card_number;
	((PLAYER_DATA *)player->data)->graze_point	= data->state1;
	#endif
}


/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

void add_boss_sakuya(STAGE_DATA *l)/*int lv*/
{
//----[ZAKO]
	common_boss_flags	= (FLG_ALL_CAST);
//
	mahoujin_num	= 0;

//
//	dummy_obj						= sprite_add_res(BASE_BOSS_SAKUYA_PNG);
	dummy_obj->flags				&= (~(SP_FLAG_VISIBLE));
	dummy_obj->anim_frame			= SAKUYA_ANIME_04_CENTER_A;
	dummy_obj->type 				= SP_MUTEKI;
//	dummy_obj->callback_mover		= dummy_move;
	dummy_obj->x256 				= 0;
	dummy_obj->y256 				= 0;

//
//----[BOSS]
	{
		SPRITE *sakuya;
	//	sakuya							= spr ite_add_file 0("boss/sakuya.png"/*"boss04.png"*/, 19, PRIORITY_03_ENEMY, 0); sakuya->anim_speed=0;
		sakuya							= sprite_add_res(BASE_BOSS_SAKUYA_PNG);
		sakuya->flags					|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
		sakuya->anim_frame				= SAKUYA_ANIME_04_CENTER_A;
		sakuya->type					= SP_BOSS/*SP_BOSS06*/;
		sakuya->callback_mover			= move_sakuya;
		sakuya->callback_loser			= lose_boss;
		sakuya->callback_hit_enemy		= NULL; 	/* �_�~�[�R�[���o�b�N�o�^ */

		sakuya->x256					= t256(GAME_WIDTH/2)/*(t256(GAME_WIDTH/2))-((sakuya->w128))*/;
		sakuya->y256					= -((sakuya->h128+sakuya->h128));
	//
		BOSS06_DATA *data;
		data							= mmalloc(sizeof(BOSS06_DATA));
		sakuya->data					= data;
	//	/*data->boss_base.boss_*/sakuya->base_health	= ((/*5*/SAKUYA_10_KEITAI*1024)-1); 	/*5119==((5*1024)-1)*/
//		/*data->boss_base.boss_*/sakuya->base_health	= ((1024)-1);
		/*data->boss_base.boss_*/sakuya->base_health	= ((1024)-1)*9;/*test*/
//		data->boss_base.bo ss_life		= ((/*5*/SAKUYA_10_KEITAI));		/*5119==((5*1024)-1)*/

		/*score(5000)+score(4000)*difficulty*/
		/*data->boss_base.*/sakuya->base_score			= adjust_score_by_difficulty(score(5000000));	/* 500�� */
		spell_card_number				= SAKUYA_01_KEITAI/*0*/;
		data->state1					= SA00;
		data->wait1 					= 0;
		data->wait2_256 				= t256(0);
		data->wait3 					= 0;
	//	data->aaa_type					= 0;
	//	data->level 					= (l->user_y);	/*lv*/
		data->move_angle512 			= (0);
	//	/*data->boss_base.*/spell_card_boss_timer		= (20*64);		/*1000*/	// + 20 [�b] ???	/* 16.666[�b] 1000 ???*/
	//	/*data->boss_base.*/spell_card_boss_timer		= (40*64);			/* 40*64==40[count] 	��40[�b(64/60)](�P�ʂ͕b�ł͂Ȃ�) */
	//	/*data->boss_base.*/spell_card_boss_timer		= ((99-24)*64); 	/* 75*64==75[count] 	��75[�b(64/60)](�P�ʂ͕b�ł͂Ȃ�) */
		/*data->boss_base.*/spell_card_boss_timer		= ((160)*64);		/* 75*64==75[count] 	��160[�b(64/60)](�P�ʂ͕b�ł͂Ȃ�) */
		// 60 [�b] ???
		#if 1
	//------------ �e���֘A
		data->boss_base.danmaku_type				= 0;
		data->boss_base.danmaku_time_out			= 0;
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
}
