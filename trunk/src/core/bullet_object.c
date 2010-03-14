
/*---------------------------------------------------------
	�e���쐬 / �����쐬
	-------------------------------------------------------
	��ŕω�����e����邩���B
	�{�X��|�������ɏ�����悤�ɂ��Ă���B
	�ǉ������e
	// [***090114	��e�̓�����ς��Ă݂�
	-------------------------------------------------------
	�������Ė{�ƌ������Ă݂�B
	�{�Ƃ̒e���Ɂu�ϐg�@�\�Ȃ�Ė����v�B
	�P�ɒe�����X�g���āA�e���ԍ��ŏ��������Ă�C�����Ă����B

	������������̕����ł͒e�����X�g���������́A
	�o���Ȃ������Ȃ����A���Ȃ�߂�ǂ������̂ŁA
	�e�����X�g���V���v���ȍ\���ɂ��������B(data�Amalloc���Ȃ�)
	-------------------------------------------------------

�� �e���V�X�e��(������)�B
 1.�e�̕ϐg:
 �e�̕ϐg�́A���܂Œe���ϐg����V�X�e�����Ǝv���Ă������A�{�Ƃ̒e�ɂ���ȋ@�\�͂Ȃ�(���Ԃ�)�B
�e�u���v�ϐg����񂶂�Ȃ��āA�e�u���v�ϐg������B���̈Ⴂ�͑傫���B
��������ϐg����e�͗v��Ȃ��B�e���V�X�e���ɒe�̕ϐg�@�\�͎������Ȃ��B(�O�����狭���I�ɕϐg������)

�Ⴆ�Ζ͕핗���́u���邭��i�C�t�v�Ő�������ƁA

���݂̖͕핗:
	��邪�u���邭��i�C�t�e�v�Ƃ�������e�𓊂���B
	�u���邭��i�C�t�e�v�͌����e�B
	���x�����ȉ��ɂȂ�Ǝ�����(�킴�Ƒ_��Ȃ�)���@�_���e�ɕϐg(�΃i�C�t)����B
	���@�_���e�Ƃ��Ĉړ�(���邪�����܂œ���e)�B

�ύX��:
	��邪�u�ʏ�e�v�𓊂���B(�Ⴆ�Βʏ�e�A�i�C�t�Ƃ܂������������́A
	���̂܂܉������Ȃ���Βʏ�e�A�i�C�t�Ƃ܂����������B������܂�������)
	�u�ʏ�e�v�͂����̌����e�B
	���t���[��(�e�Ƃ͊֌W�Ȃ��ꏊ�Ōv�������Ԃ�)�o�߂���ƁA��邪�u�e���`�F�b�N�C�x���g�����v������B
	��邪�u�e�����X�g�������v���A����(�Ⴆ�Α��x�����ȉ��̐i�C�t)�Ɉ�v�������̂�ϐg������B
	�ϐg���������(�킴�Ƒ_��Ȃ�)���@�_���u�ʏ�e�v�B
	(�ʏ�e�΃i�C�t�Ƃ܂�������������)
	-------------------------------------------------------
	�����͐F�X����B�Ⴆ�Δ��������莞�Ԍo�߂����e�Ƃ��B

���݂̖͕핗�͓���e����������B�{�Ƃ̒e�ɂ͓���e�͈������(���Ԃ�)�B
�������������łȂ��ƁA�d���̑�ʐ؂�݂����̂͒x���Ȃ肷���Ď����ł��Ȃ�(���Ԃ�)�B
(�e���{�X�����������Ă����̂��@�m���Ēe����̂ł́A��ʏ�̑S�e�ɖ��t���[�����̏���������̂ő��x���x���Ȃ肷����B
�{�X���u�e�����X�g�������v���A�����ɋ������߂��e�����؂�̂Ȃ�A���ōςނ̂ŏ\�������B
���̏���������ƂĂ��x�������ꍇ�ł�1�t���[���Ȃ̂őS�̓I�ɖ�肪�o�Ȃ�)

 2.�e�̊�_:
 ���Ă̒e�ɂ͊�_�Ɣ������Ԃ�����A�P���Ȋ֐��Ō��݈ʒu����x�Ŋ���o���Ă���C������B

---------------------------------------------------------*/

#include "game_main.h"
#include "bullet_object.h"

#define callback_2nd_regist 	callback_loser	/* �Qnd�o�^ */




/*---------------------------------------------------------
	�Ƃ肠�����ڍs�p(2)�ǉ��e�A��{�@�\
---------------------------------------------------------*/

#define BULLET_DATA_2nd_count	user_data10 	/* �Qnd �ϐg */
#define BULLET_DATA_2nd_speed	user_data11 	/* �Qnd ���x */
#define BULLET_DATA_2nd_vx256	user_data12 	/* �Qnd vx256 */
#define BULLET_DATA_2nd_vy256	user_data13 	/* �Qnd vy256 */



/*---------------------------------------------------------
	�Qnd �ϐg�o�^�A��{��
---------------------------------------------------------*/

static void callback_2nd_regist(SPRITE *src)
{
	src->BULLET_DATA_2nd_count = 9999;				/* 3���ϐg���Ȃ� */
	src->vx256	= (src->BULLET_DATA_2nd_vx256); 	/* �Qnd vx256 */
	src->vy256	= (src->BULLET_DATA_2nd_vy256); 	/* �Qnd vy256 */
}




/*---------------------------------------------------------
	�x�N�g���e(�p�x�ω����Ȃ����̂͂��̕����̕�������)
---------------------------------------------------------*/

/* ��{�e(���x�A�p�x�A�Ƃ��ɕω����Ȃ�) */
#define BASE_add_speed256	user_data07 	/* �����x */




/* ���p�e(���x�ω�����) */
//typedef struct
//{
#define TOMARI_DAN_DATA_speed256	user_data00 	/* ���x */
#define TOMARI_DAN_DATA_delta256	user_data01 	/* �����x */
//
#define TOMARI_DAN_DATA_init_vx256	user_data02 	/* �ݒ菉�����x */
#define TOMARI_DAN_DATA_init_vy256	user_data03 	/* �ݒ菉�����x */
//
	//int next_angle512;/*ST2*/ /* tmp_angleCCW512 */
	//int state;
//} TOMARI_DAN_DATA;
	//int time_out; 	/* timer */


/* ��{�e(���x�A�p�x�A�Ƃ��ɕω����Ȃ�) */
//typedef struct
//{
//	int speed256;			/* sum256 */		/* ���x */
#define GRAVITY_BULLET_DATA_delta256	user_data01 	/* �����x(y�������̂�) */		/* gra256 */
//} GRAVITY_BULLET_DATA;


//typedef struct
//{
//	int speed256;			/* sum256 */		/* ���x */
#define HAZUMI_DAN_DATA_delta256		user_data01 	/* �����x(y�������̂�) */		/* gra256 */
#define HAZUMI_DAN_DATA_bound_counts	user_data02 	/* ���˂�\��̉�(0�ŏI���) */
//} HAZUMI_DAN_DATA;

/*---------------------------------------------------------
	�x�N�g���W�J�e
	-------------------------------------------------------
	�w����W(���e����̑��Έʒu)�܂Ŕ䗦�œ����A
	��������̓x�N�g���e
---------------------------------------------------------*/



/*---------------------------------------------------------
	�p�x�e(�p�x�ω��������)
	-------------------------------------------------------
	�ǂ����A�{�Ƃɂ͊p�x�e�͖������ۂ��B
	(�{�Ƃ̒e���͂��̕����ȊO�Ŏ������Ă���ۂ�)
	�p�x�ω�������̂́A���t���[�����ƁA�p�x�ω��v�Z���Ȃ����Ă���ۂ��B
---------------------------------------------------------*/

/* ���p�e */

//typedef struct
//{
//	int aaa_angle512;	/*union BULLET_ANGLE_DATA*/
#define KURUKURU_KNIFE_DATA_aaa_speed256				user_data01 	/*union BULLET_ANGLE_DATA*/
//
	//int time_out;
#define KURUKURU_KNIFE_DATA_speed_2nd_256			user_data00
#define KURUKURU_KNIFE_DATA_height					user_data02
#define KURUKURU_KNIFE_DATA_target					user_data03
//	int p_angle512;/*�\���p�p�x*/
//} KURUKURU_KNIFE_DATA;

//typedef struct
//{
#define EVEN_KNIFE_DATA_angle512					user_data00 	/*union BULLET_ANGLE_DATA*/
#define EVEN_KNIFE_DATA_speed256					user_data01 	/*union BULLET_ANGLE_DATA*/
//
	//int time_out; // act

#define EVEN_KNIFE_DATA_d_angle512					user_data02
//	int length256;	t256(128)
#define EVEN_KNIFE_DATA_x256						user_data03
#define EVEN_KNIFE_DATA_y256						user_data04
#define EVEN_KNIFE_DATA_state						user_data05
#define EVEN_KNIFE_DATA_wait1						user_data06 	/* fire */
//	int p_angle512;/*�\���p�p�x*/
#define EVEN_KNIFE_DATA_l_or_r						user_data07 	/* -1==l, 1==r */
//} EVEN_KNIFE_DATA;


/* �����e ���� */
//typedef struct
//{
//	int angle512;
#define BOSS02_ICE_DATA_state						user_data00
#define BOSS02_ICE_DATA_speed256					user_data01
//
	//int time_out;
//	int add_angle512;
#define BOSS02_ICE_DATA_next_2nd_state				user_data02 /*������Ǝ���*/
//} BOSS02_ICE_DATA;

//typedef struct
//{
//	//int time_out; 	/* timer */
//} MOMIJI_DATA;


//typedef struct
//{
//	SPRITE *hyouji;
	//int time_out; 	/*OLD_BIG_BULLET_DATA*/ 	/* int wait_bg;*/
//#define OODAMA_OYA_DATA_ransu512					user_data00 	/*OLD_BIG_BULLET_DATA*/
//#define OODAMA_OYA_DATA_add_speed256				user_data01 	/*NEW_BIG_BULLET_DATA*/
//} OODAMA_OYA_DATA;

#if 0
typedef struct
{
	//int time_out;
	int check_x256;
	int check_y256;
//
//	int *sx256p;
//	int *sy256p;
//	int *type;
//	int sw;
//	int sh;
} OODAMA_KO_DATA;
#endif



//typedef struct
//{
#define BULLET_SP1_DATA_angle512				user_data00
#define BULLET_SP1_DATA_y_sum256				user_data01
#define BULLET_SP1_DATA_player_x256 			user_data02
#define BULLET_SP1_DATA_player_y256 			user_data03
//} BULLET_SP1_DATA;
//	int x_sa256;
//	int y_sa256;

//	int speed256;
//
//	int gra256; 	/* gra256 */
//	int sum256;
//	//int time_out; 	/* timer */

//	void/*BO SS99 _DATA*/ *sd;
//	int *sd_angle512;
//	int angle2_512;
//	int r_or_l; 	//	/*+1==R*/	/*-1==L*/

//typedef struct
//{
#define BULLET_SP2_DATA_angle512			user_data00
#define BULLET_SP2_DATA_speed256			user_data01 							/* ���x */
//	int delta256;		/* t256(0.015) gra256 */			/* �����x */
//
//	void/*BOSS05_MAHO_DATA *sd;
//	int  *sd_nnn;
//
//	int state;
#define BULLET_SP2_DATA_star_remain_time			user_data02
//} BULLET_SP2_DATA;


/*---------------------------------------------------------
	��ʊO�Ȃ炨���܂�
---------------------------------------------------------*/
void gamen_gai_nara_osimai(SPRITE *src)
{
		if (((src->x256 < t256(0))||(src->x256 > t256(GAME_WIDTH))||
			 (src->y256 < t256(0))||(src->y256 > t256(GAME_HEIGHT))))		/* ��ʊO�ɏo���� */
		{
			src->type = SP_DELETE;		/* �����܂� */
		}
}
//	if ((src->x256 < t256(0))||(src->x256 > t256(GAME_WIDTH))||
//		(src->y256 < t256(0))||(src->y256 > t256(GAME_HEIGHT)))
//	{
//		src->type = SP_DELETE;/*��ʊO�ɂł��炨���܂�*/
//	}
/*---------------------------------------------------------
	���x�l�A�p�x�l�A���x�N�g���ʂɕϊ�����
---------------------------------------------------------*/

static void callback_2nd_regist(SPRITE *src);
static void regist_vector(SPRITE *src, int speed256, int angle512)
{
	src->vx256	= ((sin512((angle512))*(speed256))>>8); 	/*fps_factor*/
	src->vy256	= ((cos512((angle512))*(speed256))>>8); 	/*fps_factor*/
//
	#if 0
	/* �e�����x������ꍇ�ɏC������ */
	if (( (8) > abs(src->vx256)) ) { src->vx256 = (0<src->vx256)?( (8)):( (-8)); }
	if (( (8) > abs(src->vy256)) ) { src->vy256 = (0<src->vy256)?( (8)):( (-8)); }
	#endif
//
	/* �Ƃ肠�����������ɂ��� */
	src->BULLET_DATA_2nd_vx256	= ((src->vx256)>>1);	/*fps_factor*/
	src->BULLET_DATA_2nd_vy256	= ((src->vy256)>>1);	/*fps_factor*/
//
	/* �Ƃ肠�����A��{�� */
	src->callback_2nd_regist	= callback_2nd_regist;
//
	/* �Ƃ肠�����A�e�L�g�[ */
	src->BULLET_DATA_2nd_count	= (30); 			/* 30�J�E���g�����甼���� */
	src->BASE_add_speed256		= (t256(1.00)); 	/* ����/�����A�Ȃ� */
}

/*---------------------------------------------------------
	��{�ʈړ��A��ʊO�͏�����
---------------------------------------------------------*/

static void move_bullet_vector(SPRITE *src)
{
	if (t256(1.00)!=(src->BASE_add_speed256))
	{
		src->vx256 = ((src->vx256*src->BASE_add_speed256)>>8);/* ����/���� */
		src->vy256 = ((src->vy256*src->BASE_add_speed256)>>8);/* ����/���� */
	}
	src->x256 += (src->vx256);		/*fps_factor*/
	src->y256 += (src->vy256);		/*fps_factor*/
//
	gamen_gai_nara_osimai(src);/* ��ʊO�Ȃ炨���܂� */

	src->BULLET_DATA_2nd_count--;		/*fps_factor*/
	if (0 > src->BULLET_DATA_2nd_count)
	{
		if (NULL != src->callback_2nd_regist)
		{
			(src->callback_2nd_regist)(src);
			src->callback_2nd_regist = NULL;
		}
	}
}


/*---------------------------------------------------------
	���p�̉��F�}�X�J�b�g�e(�i�C�t)
---------------------------------------------------------*/

#if 1
static int angle_jikinerai512_auto(SPRITE *p, SPRITE *t, int angle512);
static void move_bullet_sakuya_sp1(SPRITE *src)
{
//	BULLET_SP1_DATA *data = (BULLET_SP1_DATA *)src->data;
	src->base_time_out--;	//	data->timer++;
	if (0 > src->base_time_out)//	if (data->timer > 40)
	{
	//	else
		{
			src->vx256 = ((sin512((src->BULLET_SP1_DATA_angle512)) ) );/*fps_factor*/
			src->vy256 = ((cos512((src->BULLET_SP1_DATA_angle512)) ) );/*fps_factor*/
		}
	}
	else
	if (0+2048 > src->base_time_out)//	if (data->timer > 40)
	{
		const u8 m_tbl[4] =
		{
		//	0,1,3,7,	/* �x��������� */
			1,0,7,3,	/* �ق��̍U���Ƃ̌��ˍ����ŁA�^�C�~���O���Ƃ�Â炢������� */
		};
		/* �����ɏW�܂� */
	//	if (0==(src->base_time_out & (7>>difficulty) ))
	//	if (0==(src->base_time_out & (7>>(3-difficulty)) ))/* �x��������� */
		if (0==(src->base_time_out & (m_tbl[difficulty]) ))
		{
			src->BULLET_SP1_DATA_y_sum256--;
		}
	//	if (240/*1*/ < data->y_sum256)
		{
			int x_sa256 = (src->x256 - src->BULLET_SP1_DATA_player_x256);
			int y_sa256 = (src->y256 - src->BULLET_SP1_DATA_player_y256);
			if (t256(2.0) > abs(x_sa256))
			{
				if (t256(2.0) > abs(y_sa256))
				{
					src->base_time_out = 0;
				}
			}
			int aaa_x256 = ((x_sa256 * src->BULLET_SP1_DATA_y_sum256)>>8);	/*fps_factor*/
			int aaa_y256 = ((y_sa256 * src->BULLET_SP1_DATA_y_sum256)>>8);	/*fps_factor*/
			src->x256 = src->BULLET_SP1_DATA_player_x256 + (aaa_x256);		/*fps_factor*/
			src->y256 = src->BULLET_SP1_DATA_player_y256 + (aaa_y256);		/*fps_factor*/
		}
	}
	else
	if (0+2048==src->base_time_out) //else if (data->timer==40)
	{
	//	src->yx_an im_frame = (1);
	//	src->type			= BULLET_MARU8_00_AKA+(1);
		src->type			= BULLET_KNIFE20_07_MIDORI;/* (���΃i�C�t�ɕϐg) */
		src->BULLET_SP1_DATA_angle512 = ANGLE_JIKI_NERAI_DAN;
		src->BULLET_SP1_DATA_angle512 = angle_jikinerai512_auto(player, src, src->BULLET_SP1_DATA_angle512);
		mask512(src->BULLET_SP1_DATA_angle512);
		src->m_angleCCW512 = src->BULLET_SP1_DATA_angle512;
	//	src->BULLET_SP1_DATA_angle2_512 = (*(data->sd_angle512))/*(data->sdata->angle512)*/;
//		src->BULLET_SP1_DATA_angle2_512 = (s->tmp_angleCCW512)/*(data->sdata->angle512)*/;
		src->vx256 = (0);	/*fps_factor*/
		src->vy256 = (0);	/*fps_factor*/
		/* �Ƃ肠�����A���Ή��B�����I�ɐݒ肵���Ⴄ */
		src->BULLET_SP1_DATA_player_x256 = (player->x256+t256(32/2)/*���̔���*/) ;/*fps_factor*/
		src->BULLET_SP1_DATA_player_y256 = (player->y256+t256(16/2)/*�����̔���*/) ;/*fps_factor*/
//		src->BULLET_SP1_DATA_flag_first = 1;
		src->BULLET_SP1_DATA_y_sum256 = t256(1.0);	/* (2.5==5*0.5) */
	}
//	else
//	{
/* CCW�̏ꍇ */
//	}
//
	move_bullet_vector(src);
}
#endif



/*---------------------------------------------------------
	���p�̖����w�p
---------------------------------------------------------*/

static void move_bullet_sakuya_sp2(SPRITE *src)
{
	{
	//	BULLET_SP2_DATA *data = (BULLET_SP2_DATA *)src->data;
	//	if (2==data->sdata->nnn/*%(4-difficulty)*/)/* �X�e�[�g��1�̎��z�u���āA�X�e�[�g��2�ɂȂ����瓮�� */
	//	if (2==(*(data->sd_nnn))/*%(4-difficulty)*/)/* �X�e�[�g��1�̎��z�u���āA�X�e�[�g��2�ɂȂ����瓮�� */
	//	if (1==(*(data->sd_nnn))/*%(4-difficulty)*/)/* �X�e�[�g��0�̎��z�u���āA�X�e�[�g��1�ɂȂ����瓮�� */
		src->BULLET_SP2_DATA_star_remain_time--;
		if (0 > (src->BULLET_SP2_DATA_star_remain_time))/* ���� */
		{
			src->BULLET_SP2_DATA_star_remain_time = 0;
			src->BULLET_SP2_DATA_speed256 += 4; // (3.84) == t256(0.015);
			src->vx256 = ((sin512((src->BULLET_SP2_DATA_angle512))*(src->BULLET_SP2_DATA_speed256))>>8);/*fps_factor*/	/* CCW�̏ꍇ */
			src->vy256 = ((cos512((src->BULLET_SP2_DATA_angle512))*(src->BULLET_SP2_DATA_speed256))>>8);/*fps_factor*/
		}
	}
	//move_bullet_vector(src);
	src->x256 += (src->vx256);		/*fps_factor*/
	src->y256 += (src->vy256);		/*fps_factor*/
	gamen_gai_nara_osimai(src);/* ��ʊO�Ȃ炨���܂� */
}

/*---------------------------------------------------------
	���@�_���e�̊p�x���v�Z
---------------------------------------------------------*/
static int angle_jikinerai512(SPRITE *p, SPRITE *t)
{
	#if 1
	return (atan_512((p->y256-t->y256), (p->x256-t->x256)));
	/* �����e�̏ꍇ�Ɏ��@�_��(�ȗ���) �e�Ǝ������傫���������Ȃ炸��Ȃ��A�Ⴄ�Ƃ��̕��덷�ɂȂ� */
	#endif
}
static int angle_jikinerai512_auto(SPRITE *p, SPRITE *t, int angle512)
{
	if (ANGLE_JIKI_NERAI_DAN==angle512) 	{	angle512	= angle_jikinerai512(p,t);}
	return (angle512);
}


/*---------------------------------------------------------

---------------------------------------------------------*/
#define TOMARI_DAN_next_angle512 tmp_angleCCW512
#define TOMARI_DAN_LIMIT_00_640 (512+128)
#define TOMARI_DAN_LIMIT_00_576 (512+64)
#define TOMARI_DAN_LIMIT_01_512 (512+0)
#define TOMARI_DAN_LIMIT_02_000 (0+0)
static void bullet_create_oodama22(
	SPRITE *src//,
//	int speed256,
//	int angle512//,
//	int ransu512//,
//	int add_speed256/*, int xoffs256, int yoffs256*/
);
//atic void enemy_stop_bullet_move(SPRITE *src)
static void enemy_stop_bullet2_move(SPRITE *src)
{
//	TOMARI_DAN_DATA *data = (TOMARI_DAN_DATA *)src->data;
	/*data->*/src->base_time_out--;
	if (TOMARI_DAN_LIMIT_00_640 < /*data->*/src->base_time_out)
	{
//	case 0: 	/* �~�܂� */
		src->TOMARI_DAN_DATA_speed256 -= (src->TOMARI_DAN_DATA_delta256);
		/*data->v*/src->vx256 = (((src->TOMARI_DAN_DATA_init_vx256)*src->TOMARI_DAN_DATA_speed256)>>8);/*fps_factor*/
		/*data->v*/src->vy256 = (((src->TOMARI_DAN_DATA_init_vy256)*src->TOMARI_DAN_DATA_speed256)>>8);/*fps_factor*/
		if (src->TOMARI_DAN_DATA_speed256 < t256(0))
		{
			src->TOMARI_DAN_DATA_speed256 = t256(0);
		//	data->state = 1;
			/*data->*/src->base_time_out=(TOMARI_DAN_LIMIT_00_640);
		}
	}
	else
	if (TOMARI_DAN_LIMIT_00_576 < /*data->*/src->base_time_out)
	{
//		break;
//	case 1: 	/* ���΂���~ */
//		if (64 > /*data->*/src->base_time_out)
//		{
//			data->state = 2;
//		}
//		break;
//	case 2: 	/* ����(��e�ɕϐg) */
	//	src->an im_frame =						((/*data->*/src->base_time_out>>3)/*&0x07*/);	/*�x��(�ϐg����̂�)*/
		src->type		= BULLET_MARU8_00_AKA+	((/*data->*/src->base_time_out>>3)&0x07);		/*�x��(�ϐg����̂�)*/
	}
	else
	if (TOMARI_DAN_LIMIT_01_512 < /*data->*/src->base_time_out)
	{
	//	data->timer = 0;
	//	data->state = 3;
		if (ANGLE_NO_SHOT_DAN != src->TOMARI_DAN_next_angle512/*data->next_angle512*/)
		{
			bullet_create_oodama22(src//,
			//	t256(1.0/*0.0*/),
			//	(src->TOMARI_DAN_next_angle512)/*data->next_angle512*/ //,
				//0//,
				//(src->TOMARI_DAN_DATA_delta256/**7*/)/*, 0, 0*/
			);
			/*data->*/src->base_time_out=(TOMARI_DAN_LIMIT_02_000);
		}
		else
		{
			/*data->*/src->base_time_out=(TOMARI_DAN_LIMIT_01_512);
		}
	}
	else
	if (TOMARI_DAN_LIMIT_02_000 < /*data->*/src->base_time_out)
	{
		;	/* ���΂���~ */
	}
	else
	{
//		break;
//	case 3: 	/* ������ */
		src->color32 -= 0x10000000; 		/*	src->alpha -= 0x10; */
		if ( 0x20000000 > src->color32) 	/*	( 0x20 > src->alpha)	*/
		{
			src->color32 = 0x00ffffff;		/*	src->alpha = 0x00;	*/
			src->type = SP_DELETE;
		}
//		break;
	}
	/* ���ʂɌ����Ȃ��e�A�̏ꍇ */
	if (ANGLE_NO_SHOT_DAN == src->TOMARI_DAN_next_angle512/*data->next_angle512*/)
	{	/* �X�N���[������ */
		src->y256 += t256(1);	//	my_adjust_y(s,pd);
	}
	move_bullet_vector(src);
}

/*---------------------------------------------------------
	�P��A�ŏI�`�Ԃœ����Ă��邩�Ȃ薳���Ȓe�B
	-------------------------------------------------------
	�������̂ŁA�F�X��̉�www�B
	�ł��A�܂�����B
---------------------------------------------------------*/

/* 90/360�x��] */
//static void rot_90_angle512(HAZUMI_DAN_DATA *d)
//{
//	src->HAZUMI_DAN_DATA_angle512	= atan_512(sin512((src->HAZUMI_DAN_DATA_angle512)),-cos512((src->HAZUMI_DAN_DATA_angle512)));
//}
static void bullet_move_hazumi_dan(SPRITE *src)
{
//	HAZUMI_DAN_DATA *data = (HAZUMI_DAN_DATA *)src->data;
	if (/*src->HAZUMI_DAN_DATA_sum256*/src->vy256 < t256(15) )	/* �ő�d�͉����x */
	{
		/*src->HAZUMI_DAN_DATA_sum256*/src->vy256 += src->HAZUMI_DAN_DATA_delta256; /* �␳�l */
	}
//	s->y256 += src->HAZUMI_DAN_DATA_sum256; 		/* �ώZ�d�͉����x */
//	mono_angle_move(s,(BULLET_ANGLE_DATA *)d);
	src->y256 += src->vy256;
	src->x256 += src->vx256;

	/* ���̕� */
	if ((src->x256 < t256(0))||(src->x256 > t256(GAME_WIDTH)))
	{
		#if 1
		src->vx256 = -(src->vx256);
	//	rot_90_angle512(d); 	/*	���˕Ԃ� */
		#else
		src->type = SP_DELETE;		/*	������ */
		#endif
	}
	/* �� */
	else if (src->y256 > t256(GAME_HEIGHT))
	{
		/* �ݒ蒵�˂�� */
		if (0!=src->HAZUMI_DAN_DATA_bound_counts)
		{
			src->HAZUMI_DAN_DATA_bound_counts--;	/* ���˂�񐔂����炷 */
		//	src->an im_frame = 4+(src->HAZUMI_DAN_DATA_bound_counts);		// [***090116		�኱�ύX
			src->type		= BULLET_MARU8_00_AKA+4+(src->HAZUMI_DAN_DATA_bound_counts);		// [***090116		�኱�ύX
		//	src->HAZUMI_DAN_DATA_sum256 	= -src->HAZUMI_DAN_DATA_sum256; 	/* �d�͉����x */
			#if 0
			src->HAZUMI_DAN_DATA_speed256	= -src->HAZUMI_DAN_DATA_speed256;		/* ���x */
			src->HAZUMI_DAN_DATA_speed256	+= src->HAZUMI_DAN_DATA_speed256;		/* ����(���ēV��֓�����) */
		//	rot_90_angle512(d); 		// [***090116		�ǉ�
			#else
			src->vy256	= -(src->vy256);	/* ���x */
			src->vy256	+= (src->vy256>>2); /* 1.25�{�� ����(���ēV��֓�����) */
			src->vx256	+= src->vx256;		/* ���� */
			#endif
		}
		else
		{
			src->type = SP_DELETE;		/*	������ */
		}
	}
	#if 1
	/* �V�� */
	else if (src->y256 < 0)
	{
		src->type = SP_DELETE;		/*	������ */
	}
	#endif
}

/*---------------------------------------------------------
	�d�͒e�̈ړ�
	�����~���i�C�t(��)
---------------------------------------------------------*/

static void bullet_move_gravity(SPRITE *src)
{
//	GRAVITY_BULLET_DATA *data = (GRAVITY_BULLET_DATA *)src->data;
	src->vy256 += src->GRAVITY_BULLET_DATA_delta256;
//	data->speed256 += data->delta256;
//	src->y256 += data->speed256;
	move_bullet_vector(src);
}

/*---------------------------------------------------------
	���
	�v���C���[�̎�������邭�����Ă���A�����_���̃i�C�t�B
	���΂ɐF���ς��B
---------------------------------------------------------*/
#define knife_tmp_angle512 tmp_angleCCW512

static void bullet_move_kurukuru_knife(SPRITE *src)
{
//	KURUKURU_KNIFE_DATA *data = (KURUKURU_KNIFE_DATA *)src->data;
	if (0==src->KURUKURU_KNIFE_DATA_target)
	{
//		mono_angle_move(s,(BULLET_ANGLE_DATA *)d);

	//	src->KURUKURU_KNIFE_DATA_aaa_speed256 -= t256(0.02);
	//	src->KURUKURU_KNIFE_DATA_aaa_speed256 -= t256(0.02*2);/*2�{�ɂ����̂Œ��덇�킹*/
		src->KURUKURU_KNIFE_DATA_aaa_speed256 -= t256(0.04);	/* �i�C�t���x�A�����萔 */
	//	src->KURUKURU_KNIFE_DATA_aaa_speed256 -= 1/*t256(0.04)*/; /* �i�C�t���x�A�����萔 */
	//	if (s->an im_frame==17) {	src->an im_frame = 0;}
	//	else					{	src->an im_frame++;}
		/*data->p_angle512*/src->m_angleCCW512 += 32;
		mask512(src->m_angleCCW512/*data->p_angle512*/);/*�˂�̂���*/
		if (src->KURUKURU_KNIFE_DATA_aaa_speed256 <= t256(0) )
		{
			src->knife_tmp_angle512/*data->aaa_angle512*/ = atan_512(
				#if 1/*Gu(���S���W)*/
				(player->y256)-(src->y256)+(src->KURUKURU_KNIFE_DATA_height*256),
				(player->x256)-(src->x256)
				#endif
			);
			src->KURUKURU_KNIFE_DATA_aaa_speed256 = src->KURUKURU_KNIFE_DATA_speed_2nd_256;
			src->KURUKURU_KNIFE_DATA_target=1;
			src->type			= BULLET_KNIFE20_07_MIDORI;/* (���΃i�C�t�ɕϐg) */
		//	src->an im_frame=/*17-*/(deg_512_to_360(data->angle512)/20)%18;
		//	src->an im_frame=/*17-*/(deg_512_to_360(data->angle512)/20)%16;
			mask512(src->knife_tmp_angle512/*data->aaa_angle512*/);/*�˂�̂���*/
		//	src->an im_frame=/*17-*/((data->angle512)>>5);
			src->m_angleCCW512/*data->p_angle512*/=/*17-*/((src->knife_tmp_angle512/*data->aaa_angle512*/));
		}
		/*data->*/src->vx256 = ((sin512((src->knife_tmp_angle512/*data->aaa_angle512*/))*(src->KURUKURU_KNIFE_DATA_aaa_speed256))>>8);/*fps_factor*/
		/*data->*/src->vy256 = ((cos512((src->knife_tmp_angle512/*data->aaa_angle512*/))*(src->KURUKURU_KNIFE_DATA_aaa_speed256))>>8);/*fps_factor*/
	}
	src->x256 += (/*data->*/src->vx256);		/*fps_factor*/
	src->y256 += (/*data->*/src->vy256);		/*fps_factor*/
//
	/*data->*/src->base_time_out--;
	if (/*data->*/src->base_time_out<0)
	{
		if (((src->x256 < t256(0))||(src->x256 > t256(GAME_WIDTH))||
			 (src->y256 < t256(0))||(src->y256 > t256(GAME_HEIGHT))))
		{
			src->type = SP_DELETE;/* ��ʊO�ɏo���炨���܂� */
		}
	}
}

/*---------------------------------------------------------

	-------------------------------------------------------
	s->x256 	�i�C�t��x���W
	data->x256	�m�[�h��x���W
	s->w/2		�i�C�t�̉����̔���
	�i�C�t��x���W���m�[�h��x���W�{co_s512((data->angle512))�~length�|�i�C�t�̉����̔���
---------------------------------------------------------*/
#define ryoute_knife_length256 t256(128)
//static void enemy_evenr_knife_move(SPRITE *src)
static void enemy_even_knife_move(SPRITE *src)
{
//	EVEN_KNIFE_DATA *data = (EVEN_KNIFE_DATA *)src->data;
	switch (src->EVEN_KNIFE_DATA_state)
	{
	case 0:
		if (100 < src->EVEN_KNIFE_DATA_wait1)
		{
			src->EVEN_KNIFE_DATA_wait1 = 0;
			src->EVEN_KNIFE_DATA_state = 1;
		}
		src->EVEN_KNIFE_DATA_angle512 -= (src->EVEN_KNIFE_DATA_d_angle512*src->EVEN_KNIFE_DATA_l_or_r) ;/*fps_factor*/
		mask512(src->EVEN_KNIFE_DATA_angle512);
		/* �`��p�p�x(����0�x�ō����(�����v���)) */
		src->m_angleCCW512 = (src->EVEN_KNIFE_DATA_angle512+((int)(512*1/4)*src->EVEN_KNIFE_DATA_l_or_r));
		mask512(src->m_angleCCW512);/*�K���v��*/
	//	src->x256		= data->x256-((cos512((data->angle512))*data->length256)>>8)-((src->w128));
	//	src->y256		= data->y256-((sin512((data->angle512))*data->length256)>>8)-((src->h128));
	//	src->x256		= data->x256-((sin512((data->angle512))*ryoute_knife_length256/*data->length256*/)>>8)-((src->w128));
	//	src->y256		= data->y256-((cos512((data->angle512))*ryoute_knife_length256/*data->length256*/)>>8)-((src->h128));
		#if 1/*Gu(���S���W)*/
		src->x256		= src->EVEN_KNIFE_DATA_x256-((sin512((src->EVEN_KNIFE_DATA_angle512)))<<7 );
		src->y256		= src->EVEN_KNIFE_DATA_y256-((cos512((src->EVEN_KNIFE_DATA_angle512)))<<7 );
		#endif
		break;

	case 1:
		if (50 < src->EVEN_KNIFE_DATA_wait1)
		{
			src->EVEN_KNIFE_DATA_wait1 = 0;
			src->EVEN_KNIFE_DATA_state = 0;
		}
		//#if 1
		//data->speed=20/*speed*/;/*�悭�킩��Ȃ����~�܂����Ⴄ*/
		//#endif
	//	mono_angle_move(src,(BULLET_ANGLE_DATA *)data);
		src->vx256 = ((sin512((src->EVEN_KNIFE_DATA_angle512))*src->EVEN_KNIFE_DATA_speed256)>>8);/*fps_factor*/
		src->vy256 = ((cos512((src->EVEN_KNIFE_DATA_angle512))*src->EVEN_KNIFE_DATA_speed256)>>8);/*fps_factor*/
		src->x256 += (src->vx256);/*fps_factor*/
		src->y256 += (src->vy256);/*fps_factor*/
		break;
	}
	if (10 < src->EVEN_KNIFE_DATA_wait1)
	{
		src->EVEN_KNIFE_DATA_wait1 = 0;
		#if 1
		send1_obj->x256 = src->x256;
		send1_obj->y256 = src->y256;
		send1_obj->BULLET_REGIST_speed256			=	(t256(2.5)/*t256(5.0)*/);
		send1_obj->BULLET_REGIST_angle512			=	(src->EVEN_KNIFE_DATA_angle512+((int)(512*3/12)*src->EVEN_KNIFE_DATA_l_or_r));
	//	send1_obj->BULLET_REGIST_div_angle512		=	(int)(512/(18));			/* �_�~�[�����p�x(1way�Ȃ̂Ŏg�p���Ă��Ȃ�) */
		send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_KNIFE20_04_AOI;		/* [�i�C�t�e] */
		send1_obj->BULLET_REGIST_n_way				=	(1);	/* [1way] */
		bullet_regist_basic();
		//
		send1_obj->BULLET_REGIST_speed256			=	(t256(2.2)/*t256(4.0)*/);
		send1_obj->BULLET_REGIST_angle512			=	(src->EVEN_KNIFE_DATA_angle512+((int)(512*4/12)*src->EVEN_KNIFE_DATA_l_or_r));
	//	send1_obj->BULLET_REGIST_div_angle512		=	(int)(512/(18)),		/* �_�~�[�����p�x(1way�Ȃ̂Ŏg�p���Ă��Ȃ�) */
		send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_KNIFE20_04_AOI;		/* [�i�C�t�e] */
		send1_obj->BULLET_REGIST_n_way				=	(1);	/* [1way] */
		bullet_regist_basic();
//
		send1_obj->BULLET_REGIST_speed256			=	(t256(2.0)/*t256(3.0)*/);
		send1_obj->BULLET_REGIST_angle512			=	(src->EVEN_KNIFE_DATA_angle512+((int)(512*5/12)*src->EVEN_KNIFE_DATA_l_or_r));
	//	send1_obj->BULLET_REGIST_div_angle512		=	(int)(512/(18)),		/* �_�~�[�����p�x(1way�Ȃ̂Ŏg�p���Ă��Ȃ�) */
		send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_KNIFE20_04_AOI;		/* [�i�C�t�e] */
		send1_obj->BULLET_REGIST_n_way				=	(1);	/* [1way] */
		bullet_regist_basic();
//
		if (1<difficulty)
		{
		send1_obj->BULLET_REGIST_speed256			=	(t256(1.8)/*t256(2.0)*/);
		send1_obj->BULLET_REGIST_angle512			=	(src->EVEN_KNIFE_DATA_angle512+((int)(512*6/12)*src->EVEN_KNIFE_DATA_l_or_r));
	//	send1_obj->BULLET_REGIST_div_angle512		=	(int)(512/(18));		/* �_�~�[�����p�x(1way�Ȃ̂Ŏg�p���Ă��Ȃ�) */
		send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_KNIFE20_04_AOI;		/* [�i�C�t�e] */
		send1_obj->BULLET_REGIST_n_way				=	(1);	/* [1way] */
		bullet_regist_basic();
		}
		#else
		bullet_create_sakuya_knife(src, t256(5.0), src->EVEN_KNIFE_DATA_angle512+(int)(512*3/12), src->EVEN_KNIFE_DATA_p_angle512); //+M_PI/2
		bullet_create_sakuya_knife(src, t256(4.0), src->EVEN_KNIFE_DATA_angle512+(int)(512*4/12), src->EVEN_KNIFE_DATA_p_angle512);
		bullet_create_sakuya_knife(src, t256(3.0), src->EVEN_KNIFE_DATA_angle512+(int)(512*5/12), src->EVEN_KNIFE_DATA_p_angle512);
		if (1<difficulty)
		{	bullet_create_sakuya_knife(src, t256(2.0), src->EVEN_KNIFE_DATA_angle512+(int)(512*6/12), src->EVEN_KNIFE_DATA_p_angle512);}
		#endif
	}
	src->EVEN_KNIFE_DATA_wait1++;
	/*data->*/src->base_time_out--;
	if ( /*data->*/src->base_time_out < 0 )
	{
		src->type = SP_DELETE;
	}
}

enum
{
	KS00 = 0,
//	KS03_type2,
	KS01,
	KS02,
//	KS04,
};
/*---------------------------------------------------------

---------------------------------------------------------*/
			/* �`��p�p�x(����0�x�ō����(�����v���)) */
#define ice_angle512		m_angleCCW512
#define ice_add_angle512	tmp_angleCCW512

static void enemy_boss02_ice_move(SPRITE *src)
{
//	BOSS02_ICE_DATA *data = (BOSS02_ICE_DATA *)src->data;
	switch (src->BOSS02_ICE_DATA_state)
	{
	case KS00:	/* �~��^�� */
		/*data->*/src->base_time_out--;/*fps_factor*/
		if (/*data->*/src->base_time_out <= 0)
		{
			src->BOSS02_ICE_DATA_state = src->BOSS02_ICE_DATA_next_2nd_state;
		//	src->BOSS02_ICE_DATA_state++/* = KS01*/;
		}
		src->ice_angle512 += src->ice_add_angle512;/*fps_factor*/
//
		mask512(src->ice_angle512);
		src->vx256 = ((sin512((src->ice_angle512))*src->BOSS02_ICE_DATA_speed256)>>8);/*fps_factor*/
		src->vy256 = ((cos512((src->ice_angle512))*src->BOSS02_ICE_DATA_speed256)>>8);/*fps_factor*/
		break;
	case KS01:	/* �o���o���ړ� */
		src->BOSS02_ICE_DATA_state++/* = KS02*/;
		{
			#if 0
			const short sss[4] =
			{
				t256(2.5),		/* easy:   2 [�������ȒP�ɕύX]  2 ���ꂪ������Ȃ����Ď��͖����Ǝv���P�h */
				t256(3.0),		/* normal: 4 [�������ȒP�ɕύX]  3 */
				t256(3.5),		/* hard:   6 [�������ȒP�ɕύX]  5 ���������Ă悭����Δ�����܂� */
				t256(4.0)		/* luna:  12 [�������ȒP�ɕύX] 10 ���������Ă悭����Δ�����܂����A���܂Ɏ��s����www */
			};
			src->BOSS02_ICE_DATA_speed256 = sss[difficulty]/*t256(12)*/;
			#else
		//	src->BOSS02_ICE_DATA_speed256 = t256(2.5)+(difficulty<<7);	/* (difficulty x 128) */
			src->BOSS02_ICE_DATA_speed256 = t256(2.5)+(difficulty<<6);	/* (difficulty x 64) */ 	/* �e���������ē���Ȃ����̂Œx������ */
			#endif
		}
//		src->ice_angle512	= (ra_nd()&(128-1)/*%90*/)+deg_360_to_512(45);
		src->ice_angle512	= (ra_nd()&(128-1)/*%90*/)-deg_360_to_512CCW(45);
//
		mask512(src->ice_angle512);
		src->vx256 = ((sin512((src->ice_angle512))*src->BOSS02_ICE_DATA_speed256)>>8);/*fps_factor*/
		src->vy256 = ((cos512((src->ice_angle512))*src->BOSS02_ICE_DATA_speed256)>>8);/*fps_factor*/
		break;
//	case KS02:	/* �������Ȃ�(���������ړ�) */
//		break;
	}
//
	src->x256 += (src->vx256);/*fps_factor*/
	src->y256 += (src->vy256);/*fps_factor*/
//	move_bullet_vector(src);
	if ((src->x256 < t256(-100))||(src->x256 > t256(100)+t256(GAME_WIDTH))||
		(src->y256 < t256(-100))||(src->y256 > t256(100)+t256(GAME_HEIGHT) ) )
	{
		src->type = SP_DELETE;
	}
}


/*---------------------------------------------------------
	�P��A�ŏI�`�Ԃœ����Ă��邩�Ȃ薳���Ȓe�B
	-------------------------------------------------------
	delta256	�t���[�����ɉ������ɂǂꂾ���������邩
---------------------------------------------------------*/

// [***090114	�ǉ�
void bullet_create_hazumi_dan(SPRITE *src, int speed256, int angle512, int delta256, int bound_counts)
{
	SPRITE *h;			/*�ےe�W(�Ԃq�f�a�Ύቩ��)*/
	h					= sprite_add_only_bullet(TAMA_TYPE_BULLET_MARU16_PNG);/*�Ή��e*/
	h->type 			= BULLET_MARU8_00_AKA+(4+bound_counts);/*S P_BULLET*/ /*SP_LASER*/
	h->callback_mover	= bullet_move_hazumi_dan;
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//	h->an im_frame		= 0;//(4+bound_counts);/*�Ƃ肠����*/
	#if 1/*Gu(���S���W)*/
	h->x256 			= src->x256;
	h->y256 			= src->y256;
	#endif
//	HAZUMI_DAN_DATA *data;
//	data				= mmal loc(sizeof(HAZUMI_DAN_DATA));
//	h->data 			= data;
	/*angle512			= angle_jikinerai512_auto(player, src, angle512);*/
	mask512(angle512);
//	h->HAZUMI_DAN_DATA_angle512 	= (angle512);
//	h->HAZUMI_DAN_DATA_speed256 	= (speed256);
	regist_vector(h, speed256, angle512);

	h->HAZUMI_DAN_DATA_delta256 	= (delta256);
//	h->HAZUMI_DAN_DATA_sum256		= t256(0);
	h->HAZUMI_DAN_DATA_bound_counts = bound_counts;
}

/*---------------------------------------------------------
	�d�͒e�B�A���X�̐l�`�����e�B
	-------------------------------------------------------
	�����~���i�C�t(��)
	-------------------------------------------------------
	�������ɉ������Ă������e�B
	�t���[�����Ōv�Z���Ă���̂ł��Ȃ�d�͉����x�����������Ȃ��Ƒ�ςȂ��ƂɁB
	delta256	�t���[�����ɉ������ɂǂꂾ���������邩
---------------------------------------------------------*/
/*bullet_create_sakuya_no_rot_knife*/
void bullet_create_jyuryoku_dan000(
	SPRITE *src,
	int speed256,
	int angle512,
	int delta256,
	int bullet_obj_type 	/* �e�O�� */	/*BULLET_MARU8_00_AKA+(7)*/ 	/*(BULLET_KNIFE20_05_AKA)*/
)
{
	SPRITE *h;			/*�ےe�W(�Ԃq�f�a�Ύቩ��)*/
	h					= sprite_add_only_bullet(TAMA_TYPE_BULLET_MARU16_PNG);/*�e*/
	h->type 			= (bullet_obj_type);/*BULLET_MARU8_07_AOI*/  /*S P_BULLET*/ /*SP_LASER*/
	h->callback_mover	= bullet_move_gravity;	/*enemy_fall_knife_move*/
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//	h->an im_frame		= 0;//(7)/*0*/;/*�Ƃ肠����*/
	#if 1/*Gu(���S���W)*/
	h->x256 			= src->x256;
	h->y256 			= src->y256;
	#endif
//	/*FALL_KNIFE_DATA*/GRAVITY_BULLET_DATA *data;
//	data				= mmal loc(sizeof(GRAVITY_BULLET_DATA/*FALL_KNIFE_DATA*/));
//	h->data 			= data;
//
	/*angle512			= angle_jikinerai512_auto(player, src, angle512);*/
	mask512(angle512);
//	data->angle512		= (angle512);
//	data->speed256		= (speed256);
	regist_vector(h, speed256, angle512);
	h->GRAVITY_BULLET_DATA_delta256 	= (delta256);
//	data->speed256		= t256(0);
}


/*---------------------------------------------------------
	���p�̉��F�}�X�J�b�g�e
	-------------------------------------------------------
	���̒e�͑��̒e�������̂ł����Ēx������B
---------------------------------------------------------*/

/*static*/ //void enemy_sp1_bullet_create(SPRITE *src, int speed256, int angle512, int gra256, int r_or_l, int *sd_angle512)

void sakuya_sp1_bullet_create_bbb(SPRITE *src)
{
	int j;
	for (j=(0/*k*/); j<(8/*-k*/); j++)
	{
		//static void sakuya_sp1_bullet_create(/*-1==L*/ /*, s_data_angle512*/ );/*1.26 == 2.1*0.6*//*, int *sd_angle512*/)
		SPRITE *h;			/*�ےe�W(�Ԃq�f�a�Ύቩ��)*/
		h					= sprite_add_only_bullet(TAMA_TYPE_BULLET_MARU16_PNG);	//	h->priority 	= PRIORITY_03_ENEMY;
		const unsigned int aaa = ((ra_nd()&(4-1)));
	//	h->type 			= BULLET_MARU8_00_AKA+4+(aaa);/*S P_BULLET*/ /*SP_LASER*/
		h->type 			= BULLET_KNIFE20_04_AOI+(aaa);/*S P_BULLET*/ /*SP_LASER*/
		h->callback_mover	= move_bullet_sakuya_sp1;
		h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//		h->an im_frame		= 0;// /*4*/(aaa)/*0*/;/*�Ƃ肠����*/
//		h->an im_speed		= 0;
		#if 1/*Gu(���S���W)*/
		h->x256 			= src->x256;
		h->y256 			= src->y256;
		#endif
		h->base_time_out	= 2048+20+(40+1);		/*data->timer		= 0;*/

		int angle512		= src->tmp_angleCCW512+((j-4)*42)/*deg_360_to_512(30)*/;
		mask512(angle512);
		h->m_angleCCW512 = angle512;
		h->vx256 = ((sin512((angle512))*t256(1.0))>>8);/*fps_factor*/
		h->vy256 = ((cos512((angle512))*t256(1.0))>>8);/*fps_factor*/

	//	BULLET_SP1_DATA *data;
	//	data				= mmal loc(sizeof(BULLET_SP1_DATA));
	//	h->data 			= data;
		h->BULLET_SP1_DATA_y_sum256 	= t256(1.0);	/* (2.5==5*0.5) */


//
	#if 1
	/* �Ƃ肠�����A��{�� */
//	h->callback_2nd_regist	= callback_2nd_regist;
//
	/* �Ƃ肠�����A�e�L�g�[ */
	h->BULLET_DATA_2nd_count	= (999/*30*/);			/* 30�J�E���g�����甼���� */
	h->BASE_add_speed256		= (t256(1.00)); 	/* ����/�����A�Ȃ� */
	#endif
	}
}

#if 1/*�����w�p*/
/*---------------------------------------------------------
	���̖����w�p
	angle512	512�x�w��
---------------------------------------------------------*/

/*static*/ void enemy_sp2_bullet_create01(SPRITE *src, int angle512, int star_remain_time/**sd_nnn*/)
{		/*�ےe�W(�Ԃq�f�a�Ύቩ��)*/
	SPRITE *h;
	h					= sprite_add_only_bullet(TAMA_TYPE_BULLET_MARU16_PNG);	//	h->priority 	= PRIORITY_03_ENEMY;
	h->type 			= BULLET_MARU8_00_AKA+(0);/*S P_BULLET*/
	h->callback_mover	= move_bullet_sakuya_sp2;
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//	h->an im_frame		= 0;//(0);
//	h->an im_speed		= 0;
	#if 1/*Gu(���S���W)*/
	h->x256 			= src->x256;
	h->y256 			= src->y256;
	#endif
	h->vx256			= (0);
	h->vy256			= (0);

//	BULLET_SP2_DATA *data;
//	data				= mmal loc(sizeof(BULLET_SP2_DATA));
//	h->data 			= data;
//	data->sd			= (BOSS05_MAHO_DATA *)s->data;
	h->BULLET_SP2_DATA_star_remain_time 	= star_remain_time;
	h->BULLET_SP2_DATA_angle512 	= (angle512);
	h->BULLET_SP2_DATA_speed256 	= t256(0);

/*����*/
//
	#if 1
	/* �Ƃ肠�����A��{�� */
//	h->callback_2nd_regist	= callback_2nd_regist;
//
	/* �Ƃ肠�����A�e�L�g�[ */
	h->BULLET_DATA_2nd_count	= (999/*30*/);			/* 30�J�E���g�����甼���� */
	h->BASE_add_speed256		= (t256(1.00)); 	/* ����/�����A�Ȃ� */
	#endif
	/* �Ƃ肠�����A��{�� */
	h->callback_2nd_regist	= NULL;
}
//	data->state 		= 0;
//	data->delta256		= (delta256);

#endif



/*---------------------------------------------------------
	�΂�܂��U�������w(�ʏ̃}���z�[��)���΂�܂��A�~�܂�e
	-------------------------------------------------------
	��邪�΂�܂��A�~�܂�e�B��e�ɕω�����
	-------------------------------------------------------
	angle512	ANGLE_JIKINERAI_DAN (999) �Ńv���C���[�_��
	delta256	�t���[������speed���ǂꂾ���������邩
---------------------------------------------------------*/
// bullet_create_tomari_dan
void bullet_create_tomari2_dan(
	SPRITE *src,
	int speed256,	/* �e�� */
	int angle512,	/* �p�x */
	int delta256,	/* �����萔 */
	int next_angle512
)
{
	SPRITE *h;
//	h					= sprite_add_only_bullet(TAMA_TYPE_KUGEL_PNG);/*���Ԓe*/
	h					= sprite_add_only_bullet(TAMA_TYPE_BULLET_MARU16_PNG);		/*�ےe�W(�Ԃq�f�a�Ύቩ��)*/
	h->type 			= BULLET_MARU8_00_AKA+(0);/*S P_BULLET*/
	h->callback_mover	= enemy_stop_bullet2_move;
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//	h->an im_frame		= 0;//(0);
	#if 1/*Gu(���S���W)*/
	h->x256 			= src->x256;
	h->y256 			= src->y256;
	#endif
//	TOMARI_DAN_DATA *data;
//	data				= mmal loc(sizeof(TOMARI_DAN_DATA));
//	h->data 			= data;
	angle512			= angle_jikinerai512_auto(player, src, angle512);
//	data->angle512		= (angle512);
//	data->speed256		= (speed256);
	h->TOMARI_DAN_DATA_speed256 	= (speed256);
	regist_vector(h, t256(1.0)/*speed256*/, angle512);
	h->TOMARI_DAN_DATA_init_vx256	= /*data->*/h->vx256;
	h->TOMARI_DAN_DATA_init_vy256	= /*data->*/h->vy256;
//
	//data->state		= 0;
	h->TOMARI_DAN_DATA_delta256 	= (delta256);
	if (ANGLE_NO_SHOT_DAN==next_angle512)/* ���ʂɌ����Ȃ��e�A�̏ꍇ */
	{
;// 	/*data->*/h->base_time_out	= 800+64;
	}
	else
	{
;// 	/*data->*/h->base_time_out	= 100+64;
		mask512(next_angle512);
	}
	/*data->next_angle512*/h->TOMARI_DAN_next_angle512 = (next_angle512);
	/*data->*/h->base_time_out	= 1024;/* �K���ɑ傫��1024==(512+64+�ő��~����) */
}


/*---------------------------------------------------------
	���邭��i�C�t(����)
	-------------------------------------------------------
	�S�����i�C�t(��)
	�_���i�C�t(��)
---------------------------------------------------------*/

void bullet_create_sakuya_kurukuru_knife(void)
//	SPRITE *src,
//	int speed256,	/* �e�� */
//	int angle512,	/* �p�x */
//	int height		/* (?) */
//)
{/* x, y*/
	SPRITE *h;
	h					= sprite_add_only_bullet(TAMA_TYPE_BULLET_KNIFE18_PNG);/*�S�����i�C�t(��)*/
	h->type 			= /*BULLET_KNIFE20_07_MIDORI*/BULLET_KNIFE20_04_AOI;/*BULLET_KNIFE20_04_AOI*/	/* (���΃i�C�t�ɕϐg) */		/*S P_BULLET*/ /*SP_LASER*/
	h->callback_mover	= bullet_move_kurukuru_knife;
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//	h->an im_frame		= 0;//0;
//	h->x256 			= send1_obj->x256;
//	h->y256 			= send1_obj->y256/*x*/;
	#if 1/*Gu(���S���W)*/
	h->x256 			= send1_obj->x256;
	h->y256 			= send1_obj->y256;
	#endif
	/*data->*/h->base_time_out		= 300;
	h->knife_tmp_angle512/*data->aaa_angle512*/ 	= (send1_obj->BULLET_REGIST_angle512);
//
//	KURUKURU_KNIFE_DATA *data;
//	data				= mmal loc(sizeof(KURUKURU_KNIFE_DATA));
//	h->data 			= data;
	{
		int tmp_speed256;
		tmp_speed256 = (send1_obj->BULLET_REGIST_speed256);
		h->KURUKURU_KNIFE_DATA_aaa_speed256 	= (tmp_speed256/*+tmp_speed256*/);	/* �i�C�t�̓��������x */
		h->KURUKURU_KNIFE_DATA_speed_2nd_256	= (tmp_speed256+tmp_speed256);		/* �΃i�C�t��2nd�_�����x */
	}
	h->KURUKURU_KNIFE_DATA_target			= 0;
	h->KURUKURU_KNIFE_DATA_height			= (send1_obj->BULLET_REGIST_sakuya_kurukurku_knife_height);
}


/*---------------------------------------------------------
	����i�C�t(��)
	-------------------------------------------------------
	length	enemy-player�Ԃ̋���/��2
	r_or_l	0==�E	1==��
---------------------------------------------------------*/

void bullet_create_sakuya_ryoute_knife(SPRITE *src)
//, int speed256/*, int dummy_length256*/, int r_or_l)/*0==�E*/ 	/*1==��*/
{
int speed256;
	speed256	= t256(10);
	int r_or_l;
//	for (r_or_l=0; r_or_l<2; r_or_l++)
	for (r_or_l=-1; r_or_l<2; r_or_l+=2)
	{
		SPRITE *h;
		h					= sprite_add_only_bullet(TAMA_TYPE_BULLET_KNIFE18_PNG);/*�S�����i�C�t(��)*/ /*h->use_alpha=0 ????*/
		h->type 			= BULLET_KNIFE20_04_AOI;/*S P_BULLET*/ /*SP_LASER*/
	//
		h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	//	h->an im_frame		= 0;//0;
		#if 1/*Gu(���S���W)*/
		h->x256 			= src->x256;
		h->y256 			= src->y256;
		#endif
		/*data->*/h->base_time_out		= (150+difficulty*50);
	//
//		EVEN_KNIFE_DATA *data;
//		data				= mmal loc(sizeof(EVEN_KNIFE_DATA));
//		h->data 			= data;
	//
		int tmp512;
		tmp512 = angle_jikinerai512(player,src);/*???(original)*/ /* ��e�̏ꍇ�Ɏ��@�_�� */
		h->callback_mover	= enemy_even_knife_move;
		#if 0
	//	if (1==r_or_l)	/* psp��0���W�X�^������̂�0�Ɣ�r�����ق������� */
		if (0!=r_or_l)
		{
	//		h->callback_mover	= enemy_evenl_knife_move;
			tmp512 += (64)/*M_PI*2*1/8*/;
			data->l_or_r			= -1;
		}
		else
	//	if (0==r_or_l)
		{
	//		h->callback_mover	= enemy_evenr_knife_move;
			tmp512 -= (64)/*M_PI*2*1/8*/;
			data->l_or_r			= 1;
		}
		#else
		h->EVEN_KNIFE_DATA_l_or_r			= r_or_l;
		tmp512 -= (64*r_or_l)/*M_PI*2*1/8*/;
		#endif
		mask512(tmp512);
	//	if (0==length256)	{length256=1*256;}
	//	data->length256 	= length256;
	//	data->x256			= /*s->x256+((s->w128))*/h->x256+((cos512((tmp512))*length256)>>8);
	//	data->y256			= /*s->y256+((s->h128))*/h->y256+((sin512((tmp512))*length256)>>8);
	//	data->x256			= /*s->x256+((s->w128))*/h->x256+((sin512((tmp512))*ryoute_knife_length256/*length256*/)>>8);
	//	data->y256			= /*s->y256+((s->h128))*/h->y256+((cos512((tmp512))*ryoute_knife_length256/*length256*/)>>8);
		h->EVEN_KNIFE_DATA_x256 		= /*s->x256+((s->w128))*/h->x256+((sin512((tmp512)) )<<7 );
		h->EVEN_KNIFE_DATA_y256 		= /*s->y256+((s->h128))*/h->y256+((cos512((tmp512)) )<<7 );
		h->EVEN_KNIFE_DATA_angle512 	= /*ra d2deg512*/(/*deg512_2rad*/(tmp512));
	//	h->EVEN_KNIFE_DATA_d_angle512	= ra d2deg512(speed/(M_PI*2*length));	//��angle���C��/(2�΁~length/��2)
	//	int tmp_512;
	//	tmp_512=speed/(/*M_PI*2*/512*length);	//��angle���C��/(2�΁~length/��2)
		h->EVEN_KNIFE_DATA_d_angle512	= 1;//	  /*ra d2deg512*/(/*deg512_2rad*/(tmp_512));
	//
		h->EVEN_KNIFE_DATA_speed256 	= /*10*/t256(10)/*8+difficulty*/ /*speed*/;/*�悭�킩��Ȃ����~�܂����Ⴄ*/
		h->EVEN_KNIFE_DATA_state		= 0;
		h->EVEN_KNIFE_DATA_wait1		= 0;
	}
}


/*---------------------------------------------------------
	�������A�������i�e��ǉ�����
	-------------------------------------------------------

---------------------------------------------------------*/
/*(int)(512/24)*/
void bullet_regist_basic(void)
//	SPRITE *src,			/*	*/
//	int BULLET_REGIST_speed256, 		/* �e�� */
//	int BULLET_REGIST_angle512, 		/* ���˒��S�p�x / ����@�\(���@�_��/��) */
//	int BULLET_REGIST_div_angle512, 	/* 1����n���������p�x */
//	int BULLET_REGIST_bullet_obj_type,	/* �e�O�� */
//	int BULLET_REGIST_n_way 			/* ���e�� */
	/*
		�悭�l������AANGLE_JIKI_NERAI_KISUDAN �͕K�v�Ȃ��B
		ANGLE_JIKINERAI_DAN ��������Ηǂ��A
		���e���ɋ������w�肷��΋����e�B
		���e���Ɋ���w�肷��Ί�e�B
		������܂������ǁA�C���t���Ȃ�����ww�B
	*/
{
	send1_obj->BULLET_REGIST_angle512			= angle_jikinerai512_auto(player, send1_obj/*src*/, send1_obj->BULLET_REGIST_angle512); /*ANGLE_JIKI_NERAI_DAN???(original)*/ /* ��e�̏ꍇ�Ɏ��@�_�� */
	int i_angle512;
	i_angle512			= send1_obj->BULLET_REGIST_angle512-((send1_obj->BULLET_REGIST_n_way+2)*((send1_obj->BULLET_REGIST_div_angle512>>1)/*(int)(512/48)*/))/*ra d2deg512((M_PI*2)/48))*/; /* 48�����e */ // /* ��/24 0.13089969389957471827 */
	/* �O�̈׃}�X�N */
	mask512(i_angle512);
//
	int i;
	for (i=0; i<send1_obj->BULLET_REGIST_n_way; i++)
	{
		SPRITE *h;
		// frame�̓t���[����-1 /*char *filename, int frame,*/
		h					= sprite_add_only_bullet(TAMA_TYPE_BULLET_DUMMY/*_MING32_PNG*/);
		h->type 			= (send1_obj->BULLET_REGIST_bullet_obj_type);/*BULLET_UROKO14_01_AKA+*/ /*S P_BULLET*/
		h->callback_mover	= move_bullet_vector;
		h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		#if 1/*Gu(���S���W)*/
		h->x256 			= /*src*/send1_obj->x256;
		h->y256 			= /*src*/send1_obj->y256;
		#endif
//		BULLET_VECTOR_DATA *data;
//		data				= mmal loc(sizeof(BULLET_VECTOR_DATA));
//		h->data 			= data;
		i_angle512			+= send1_obj->BULLET_REGIST_div_angle512;	/*(2)*((int)(512/48))*/ /* ��/12 0.26179938779914943654 */
		mask512(i_angle512);
	//	data->angle512		= /*deg512_2rad*/(i_angle512);	//
	//	data->speed256		= speed256;
		#if 1
		/* �`��p�p�x(����0�x�ō����(�����v���)) */
	//	h->m_angleCCW512		= i_angle512;
	//	int aaa512; 	aaa512 = (/*127*/255-i_angle512);	mask512((aaa512));
//		int aaa512; 	aaa512 = (/*127*/127-i_angle512);	mask512((aaa512));
		int aaa512; 	aaa512 = (i_angle512);	mask512((aaa512));
		h->m_angleCCW512		= aaa512;
		#endif
		regist_vector(h, send1_obj->BULLET_REGIST_speed256, i_angle512);
	}
}

#if 0
/*---------------------------------------------------------
	���ׂĂ̒e�������ʉ�
	-------------------------------------------------------

---------------------------------------------------------*/

BULLET_STATUS bullet_resource[BULLET_RES_MAX] =
{
	{	&send1_obj, 	0,	0,	0,	0,	0,	0,	0,	NULL,	NULL,	NULL,	NULL,	},
	{	&send1_obj, 	0,	0,	0,	0,	0,	0,	0,	NULL,	NULL,	NULL,	NULL,	},
	{	&send1_obj, 	0,	0,	0,	0,	0,	0,	0,	NULL,	NULL,	NULL,	NULL,	},
	{	&send1_obj, 	0,	0,	0,	0,	0,	0,	0,	NULL,	NULL,	NULL,	NULL,	},
	{	&send1_obj, 	0,	0,	0,	0,	0,	0,	0,	NULL,	NULL,	NULL,	NULL,	},
	{	&send1_obj, 	0,	0,	0,	0,	0,	0,	0,	NULL,	NULL,	NULL,	NULL,	},
	{	&send1_obj, 	0,	0,	0,	0,	0,	0,	0,	NULL,	NULL,	NULL,	NULL,	},
	{	&send1_obj, 	0,	0,	0,	0,	0,	0,	0,	NULL,	NULL,	NULL,	NULL,	},
};
void bullet_create_resource(int type)
{

}
#endif

/*---------------------------------------------------------
	�m�[�}���e
---------------------------------------------------------*/
/*bullet_create_maru8_frame*/
/*---------------------------------------------------------
	�o���ʒu�I�t�Z�b�g�e���쐬����
	�j�e���쐬����
---------------------------------------------------------*/
void bullet_create_offset_dan_type000(
	SPRITE *src,			/*	*/
	int speed256,			/* �e�� */
	int angle512,			/* ���˒��S�p�x / ����@�\(���@�_��/��) */
	int bullet_obj_type 	/* �e�O�� */
)
	/*, int x_offset256, int y_offset256*/
{
		send1_obj->x256 = src->x256;
		send1_obj->y256 = src->y256;
		send1_obj->BULLET_REGIST_speed256			=	speed256;				/* �e�� */
		send1_obj->BULLET_REGIST_angle512			=	angle512;
	//	send1_obj->BULLET_REGIST_div_angle512		=	(0);				/* �_�~�[�p�x(���g�p) */
		send1_obj->BULLET_REGIST_bullet_obj_type	=	bullet_obj_type;	/* [�Ԓe] */
		send1_obj->BULLET_REGIST_n_way				=	(1);				/* [1way] */
		bullet_regist_basic();
}


/*---------------------------------------------------------
	�v���C���[�_���e(��)���쐬����
---------------------------------------------------------*/
void bullet_create_aka_maru_jikinerai(
	SPRITE *src,
	int speed256
)
{
	int bullet_obj_type;	bullet_obj_type 	=	BULLET_MARU8_00_AKA;	/* �e�O�� */
	int angle512;			angle512			=	ANGLE_JIKI_NERAI_DAN;	/* ���@�_���e */
//
	bullet_create_offset_dan_type000(src, speed256, angle512, bullet_obj_type);
}


/*---------------------------------------------------------
	���p�̑�e(��)�B
	-------------------------------------------------------
	�����ځ��������蔻��ł��B
---------------------------------------------------------*/

static void bullet_create_oodama22(
	SPRITE *src
//,
//	int speed256,
//	int angle512//,
//	int ransu512//,
	//int add_speed256
)/*, int xoffs256, int yoffs256*/
{
//	angle512			= angle_jikinerai512_auto(player, src, angle512);
//	mask512(angle512);
//
	int angle512;
	angle512			= (src->TOMARI_DAN_next_angle512);
//
	SPRITE *h;
	h					= sprite_add_only_bullet(TAMA_TYPE_OODAMA_08_PNG);	 /* ��e(��) �\������*/
//	h->priority 		= PRIORITY_03_ENEMY;
//	unsigned int aaa = (((angle512>>6)&(0x05))|(2));
	unsigned int aaa = (((angle512>>6)&(0x03)));
	h->type 			= BULLET_OODAMA32_00_SIROI+(aaa);/*S P_BULLET*/ /*SP_BIGBULLET*/
	h->callback_mover	= move_bullet_vector;	/* (�v�����@�\) */	/*move_bullet_oodama2_standard*/
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//	h->an im_frame		= 0;//(aaa);/*(2) ((rand()&(0x05))|(2))*/
	#if 1/*Gu(���S���W)*/
	h->x256 			= src->x256;
	h->y256 			= src->y256;
	#endif
//	OODAMA_OYA_DATA *data;
//	data				= mmal loc(sizeof(OODAMA_OYA_DATA));
//	h->data 			= data;
//	h->OODAMA_OYA_DATA_add_speed256 = add_speed256;
//
//	h->OODAMA_OYA_DATA_angle512 	= (angle512);
//	h->OODAMA_OYA_DATA_speed256 	= speed256;
	int speed256;
	speed256 = (t256(1.00));
	regist_vector(h, speed256, angle512);
	h->BULLET_DATA_2nd_count	= (300);			/* 30�J�E���g�����甼���� */
	h->BASE_add_speed256		= (t256(1.05) );	/* ���� */ /*add_speed256*/
}

/*---------------------------------------------------------
	�V��p
	ICE�e
	-------------------------------------------------------
---------------------------------------------------------*/

/*static*/ void bullet_create_aya_ice(SPRITE *src)
{
	int iii;
	int j;
	int rand_add_angle;
	rand_add_angle = (ra_nd()&(8-1));
	static const s8 step_tbl[(4)] =
	{
		(512/24),	/* easy */
		(512/28),	/* normal */
		(512/30),	/* hard */
		(512/32),	/* lunatic */
	};
	for (j=0; j</*2*/(1+difficulty); j++)
	{
		for (iii=(0); iii<(512/*-64*/); iii+=step_tbl[(difficulty)]/*(16)*/ )/* (512-32)(30<<4) (1<<4) ICE_NU M_OF_ENEMIES  */
		{
			SPRITE		*h;
			h					= sprite_add_only_bullet(TAMA_TYPE_BULLET_JIPPOU32_PNG);
			h->type 			= BULLET_KUNAI12_01_AKA;/*BULLET_CAP16_04_SIROI*/	/*S P_BULLET*/ /*S P_BO SS02ICE*/
			h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
			h->callback_mover	= enemy_boss02_ice_move;

		//	h->x256 			= src->x256 + ((0==j)?(t256(-19)):(t256(81)));
		//	h->x256 			= src->x256 + ((0==j)?(t256(-64)):(t256(64)));
		{	s16 aaa[4]			=	{t256(-64),t256(64),t256(-32),t256(32)};
			h->x256 			= src->x256 + aaa[(j)];
		}	h->y256 			= src->y256 /*+ t256(135 138)*/;
			/*data->*/h->base_time_out		= (120)/*100*/;
//
//			BOSS02_ICE_DATA *data;
//			data				= mmal loc(sizeof(BOSS02_ICE_DATA));
//			h->data 			= data;
//
		//	h->ice_add_angle512 = (j == 0)?(deg_360_to_512(+5)):(deg_360_to_512(-5));
			h->ice_add_angle512 = rand_add_angle-j;
			h->ice_angle512 	= (iii)/* (i<<4) deg_360_to_512(90) */;
//
			h->BOSS02_ICE_DATA_state		= 0;
		//	h->BOSS02_ICE_DATA_next_2nd_state	= KS01; 	/* �o���o���ړ� */
			h->BOSS02_ICE_DATA_next_2nd_state	= KS02; 	/* �������Ȃ�(���������ړ�) */
			h->BOSS02_ICE_DATA_speed256 	= t256(1.2);	/* (�Œ葬�x) */	/* (t256(2.5)+(difficulty<<6)) (t256(4.0)) */	/* (difficulty x 64) */
		}
		rand_add_angle = -rand_add_angle;
	}
}

