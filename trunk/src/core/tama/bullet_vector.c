
#include "game_main.h"

/*---------------------------------------------------------
  �����͕핗 �` Toho Imitation Style.
  �v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�ړ��ʒe
	-------------------------------------------------------
	Todo:
	���݂̖͕핗�͓���e����������B�u����e�̕�����psp�ł͒x������v�̂Œe���ɂ���̂͌����I�ɖ����B
	����e�ɂ��Ȃ��Ă��܂�������������(������)�ʏ�e(�̑g�ݍ��킹��)�Ŏ����ł���B
	�u�����̒ʏ�e�̑g�ݍ��킹�v�Ȃ猴���I�ɑ����̂Œe���ɂ���̂͌����I�ɉ\�B
	(�P����)�ʏ�e��100��ނ��炢�ɑ��₵�Ă����v��������Ȃ��B
	-------------------------------------------------------
	���x���l����Ɠ���e�̒���switch()case:������̂͘_�O(�ɒx��)�B
	GCC�͂ǂ� version / �œK�� �ł�switch()case:��R�n�̓W�����v�e�[�u���ɓW�J���Ȃ��B
	�Œ�ł�if ()���A�W�����v�e�[�u���ɂ���B
	VC++(MSC)�ł̓W�����v�e�[�u���𖾎��ł��Ȃ����AGCC�̓W�����v�e�[�u��������B
	VC++�͂��̕�switch()case:��dafault:�������ꍇ�A�W�����v�e�[�u���𐶐�����B(�œK���ɂ��)
	-------------------------------------------------------
	�v����Ɂu�e���ϐg(��ԕύX)�v����������͒x���̂Ŗ����B
	�u�{�X���ϐg(��ԕύX)�v����������͏\�������B
---------------------------------------------------------*/

/*---------------------------------------------------------
	�Ƃ肠�����ڍs�p(2)�ǉ��e�A��{�@�\
---------------------------------------------------------*/

#define BULLET_DATA_2nd_count	user_data10 	/* �Qnd �ϐg */
#define BULLET_DATA_2nd_speed	user_data11 	/* �Qnd ���x */
#define BULLET_DATA_2nd_vx256	user_data12 	/* �Qnd vx256 */
#define BULLET_DATA_2nd_vy256	user_data13 	/* �Qnd vy256 */

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
	�e�����X�g���V���v���ȍ\���ɂ��������B(data�Amal loc���Ȃ�)
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


/*---------------------------------------------------------
	���@(src)�_���e�̊p�x��(dest��)�v�Z
---------------------------------------------------------*/

static void s_tmp_angleCCW1024_src_nerai_auto(SPRITE *src, SPRITE *dest)
{
	if (ANGLE_JIKI_NERAI_DAN==dest->tmp_angleCCW1024)
	{
		/*s_*/tmp_angleCCW1024_src_nerai(src, dest);
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

global BULLET_REGIST br;

#define callback_2nd_regist 	callback_loser	/* �Qnd�o�^ */

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
#define BASE_add_speed256						user_data07 	/* �����x */


/* ���p�e(���x�ω�����) */
//typedef struct
//{
#define TOMARI_DAN_DATA_speed256				user_data00 	/* ���x */
#define TOMARI_DAN_DATA_delta256				user_data01 	/* �����x */
//
#define TOMARI_DAN_DATA_init_vx256				user_data02 	/* �ݒ菉�����x */
#define TOMARI_DAN_DATA_init_vy256				user_data03 	/* �ݒ菉�����x */
//} TOMARI_DAN_DATA;


/* ��{�e(���x�A�p�x�A�Ƃ��ɕω����Ȃ�) */
//typedef struct
//{
#define GRAVITY_BULLET_DATA_delta256			user_data01 	/* �����x(y�������̂�) */
//} GRAVITY_BULLET_DATA;


//typedef struct
//{
#define HAZUMI_DAN_DATA_delta256				user_data01 	/* �����x(y�������̂�) */
#define HAZUMI_DAN_DATA_bound_counts			user_data02 	/* ���˂�\��̉�(0�ŏI���) */
//} HAZUMI_DAN_DATA;


/*---------------------------------------------------------
	���x�l�A�p�x�l�A���x�N�g���ʂɕϊ�����
---------------------------------------------------------*/

static void callback_2nd_regist(SPRITE *src);
static void regist_vector(SPRITE *src, int speed256, int angle1024)
{
	src->vx256	= ((sin1024((angle1024))*(speed256))>>8);	/*fps_factor*/
	src->vy256	= ((cos1024((angle1024))*(speed256))>>8);	/*fps_factor*/
//
	#if 0
	/* �e�����x������ꍇ�ɏC������ */
	if (( (8) > abs(src->vx256)) )	{ src->vx256 = (0<src->vx256)?( (8)):( (-8)); }
	if (( (8) > abs(src->vy256)) )	{ src->vy256 = (0<src->vy256)?( (8)):( (-8)); }
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
//	src->BASE_add_speed256		= (t256(1.00)); 	/* ����/�����A�Ȃ� */
	src->BASE_add_speed256		= 1+(t256(1.00));	/* ���� �e */

}

/*---------------------------------------------------------
	��{�ʈړ��A��ʊO�͏�����
	��ݏo���`�F�b�N�@�\�͂Ȃ��̂ŕʂł�鎖
---------------------------------------------------------*/

static void move_bullet_vector(SPRITE *src)
{
	if (t256(1.00)!=(src->BASE_add_speed256))
	{
		#if (0)
	//	src->vx256 = ((src->vx256*src->BASE_add_speed256)>>8);/* ����/���� */	// ���̏ꍇ�_���B
		#else
		const int vx256_speed = (src->vx256*src->BASE_add_speed256);
		if (0 < vx256_speed) /* ���̏ꍇ */
		{
			src->vx256 = ((vx256_speed)>>8);/* ����/���� */
		}
		else /* ���̏ꍇ */
		{
			src->vx256 = -((-vx256_speed)>>8);/* ����/���� */
		}
		#endif
		//
		#if (1)/* ���e���������ĂȂ��̂�... */
		src->vy256 = ((src->vy256*src->BASE_add_speed256)>>8);/* ����/���� */	// ���̏ꍇ�_���B
		#else
		const int vy256_speed = (src->vy256*src->BASE_add_speed256);
		if (0 < vy256_speed) /* ���̏ꍇ */
		{
			src->vy256 = ((vy256_speed)>>8);/* ����/���� */
		}
		else /* ���̏ꍇ */
		{
			src->vy256 = -((-vy256_speed)>>8);/* ����/���� */
		}
		#endif
	}
	src->cx256 += (src->vx256); 	/*fps_factor*/
	src->cy256 += (src->vy256); 	/*fps_factor*/
//
//	gamen_gai_nara_osimai(src);/* ��ʊO�Ȃ炨���܂� */

	#if 1/* (�����Ȃ�\���������\��) */
	src->BULLET_DATA_2nd_count--;		/*fps_factor*/
	if (0 > src->BULLET_DATA_2nd_count)
	{
		if (NULL != src->callback_2nd_regist)
		{
			(src->callback_2nd_regist)(src);
			src->callback_2nd_regist = NULL;
		}
	}
	#endif
}






/*---------------------------------------------------------

	-------------------------------------------------------
	�e���������������Ɉڍs����̂ł����͖����Ȃ�\��B
---------------------------------------------------------*/
#define TOMARI_DAN_next_angle1024	user_data04/*tmp_angleCCW1024*/

#define TOMARI_DAN_LIMIT_00_MAX 	(512+512)	/* �K���ɑ傫��1024==(512+64+�ő��~����) */
#define TOMARI_DAN_LIMIT_00_640 	(512+128)
#define TOMARI_DAN_LIMIT_00_576 	(512+64)
#define TOMARI_DAN_LIMIT_01_512 	(512+0)
#define TOMARI_DAN_LIMIT_02_000 	(32+0+0)
//	int speed256,
//	int angle512//,
//	int ransu512//,
//	int add_speed256/*, int xoffs256, int yoffs256*/


static void bullet_create_oodama22(SPRITE *src);
static void bullet_move_tomari_dan(SPRITE *src)
{
	if (TOMARI_DAN_LIMIT_00_640 < src->jyumyou)
	{
		/* �~�܂� */
		src->TOMARI_DAN_DATA_speed256 -= (src->TOMARI_DAN_DATA_delta256);
		src->vx256 = (((src->TOMARI_DAN_DATA_init_vx256)*src->TOMARI_DAN_DATA_speed256)>>8);/*fps_factor*/
		src->vy256 = (((src->TOMARI_DAN_DATA_init_vy256)*src->TOMARI_DAN_DATA_speed256)>>8);/*fps_factor*/
		if (src->TOMARI_DAN_DATA_speed256 < t256(0))
		{
			src->TOMARI_DAN_DATA_speed256 = t256(0);
			src->jyumyou = (TOMARI_DAN_LIMIT_00_640);
		}
	}
	else
	if (TOMARI_DAN_LIMIT_00_576 < src->jyumyou)
	{
		/* ����(��e�ɕϐg) */
		src->type		= BULLET_MINI8_00_SIRO + (( src->jyumyou>>3)&0x07); 	/* �x��(�ϐg����̂�) */
	}
	else
	if (TOMARI_DAN_LIMIT_01_512 < src->jyumyou)
	{
		if (ANGLE_NO_SHOT_DAN != src->TOMARI_DAN_next_angle1024 )
		{
			bullet_create_oodama22(src);
			src->jyumyou = (TOMARI_DAN_LIMIT_02_000);
		}
		else
		{
			src->jyumyou = (TOMARI_DAN_LIMIT_01_512);
		}
	}
	else
	if (TOMARI_DAN_LIMIT_02_000 < src->jyumyou)
	{
		;	/* ���΂���~ */
	}
	else
	{
		/* ������ */
		src->color32 -= 0x10000000; 		/*	src->alpha -= 0x10; */
		if ( 0x20000000 > src->color32) 	/*	( 0x20 > src->alpha)	*/
		{
			src->color32 = 0x00ffffff;		/*	src->alpha = 0x00;	*/
			src->jyumyou = JYUMYOU_NASI;
		}
	}
	/* ���ʂɌ����Ȃ��e�A�̏ꍇ */
	if (ANGLE_NO_SHOT_DAN == src->TOMARI_DAN_next_angle1024 )
	{	/* �X�N���[������ */
		src->cy256 += t256(1.0);	//	my_adjust_y(s,pd);
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
//	src->HAZUMI_DAN_DATA_angle512	= at an_512(sin512((src->HAZUMI_DAN_DATA_angle512)),-cos512((src->HAZUMI_DAN_DATA_angle512)));
//}
static void bullet_move_hazumi_dan(SPRITE *src)
{
	if (/*src->HAZUMI_DAN_DATA_sum256*/src->vy256 < t256(15) )	/* �ő�d�͉����x */
	{
		/*src->HAZUMI_DAN_DATA_sum256*/src->vy256 += src->HAZUMI_DAN_DATA_delta256; /* �␳�l */
	}
//	s->cy256 += src->HAZUMI_DAN_DATA_sum256;		/* �ώZ�d�͉����x */
//	mono_angle_move(s,(BULLET_ANGLE_DATA *)d);
	src->cx256 += src->vx256;
	src->cy256 += src->vy256;

	/* ���̕� */
	if ((src->cx256 < t256(0))||(src->cx256 > t256(GAME_WIDTH)))
	{
		#if 1
		src->vx256 = -(src->vx256);
	//	rot_90_angle512(d); 	/*	���˕Ԃ� */
		#else
		src->jyumyou = JYUMYOU_NASI;		/*	������ */
		#endif
	}
	/* �� */
	else if (src->cy256 > t256(GAME_HEIGHT))
	{
		/* �ݒ蒵�˂�� */
		if (0!=src->HAZUMI_DAN_DATA_bound_counts)
		{
			src->HAZUMI_DAN_DATA_bound_counts--;	/* ���˂�񐔂����炷 */
			src->type		= BULLET_MINI8_00_SIRO+4+(src->HAZUMI_DAN_DATA_bound_counts);		// [***090116		�኱�ύX
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
			src->jyumyou = JYUMYOU_NASI;		/*	������ */
		}
	}
	#if 1
	/* �V�� */
	else if (src->cy256 < 0)
	{
		src->jyumyou = JYUMYOU_NASI;		/*	������ */
	}
	#endif
}

/*---------------------------------------------------------
	�d�͒e�̈ړ�
	�����~���i�C�t(��)
---------------------------------------------------------*/

static void bullet_move_gravity(SPRITE *src)
{
	src->vy256 += src->GRAVITY_BULLET_DATA_delta256;
//	data->speed256 += data->delta256;
//	src->cy256 += data->speed256;
	move_bullet_vector(src);
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
//SPRITE *src, int speed256, int angle512, int delta256, int bullet_obj_type);

//	SPRITE *src,
//	int br.BULLET_REGIST_00_speed256,
//	int br.BULLET_REGIST_02_VECTOR_angle1024,
//	int BULLET_REGIST_07_VECTOR_jyuryoku_dan_delta256,
//	int br.BULLET_REGIST_04_bullet_obj_type 	/* �e�O�� */	/*(BULLET_KNIFE20_05_AKA)*/

/*bullet_create_sakuya_no_rot_knife*/
static void s_bullet_create_jyuryoku_dan(void)
{
	SPRITE *h;			/*�ےe�W(�Ԃq�f�a�Ύቩ��)*/
	h					= obj_add_00_tama_error();/*�e*/
	if (NULL != h)
	{
		h->cx256			= obj_send1->cx256;
		h->cy256			= obj_send1->cy256;
		h->type 			= (br.BULLET_REGIST_04_bullet_obj_type);/*BULLET_MINI8_07_AOI*/  /*S P_BULLET*/ /*SP_LASER*/
		reflect_sprite_spec444(h, OBJ_BANK_SIZE_00_TAMA);
		h->m_Hit256R		= TAMA_ATARI_MARU16_PNG;
		//
		h->callback_mover	= bullet_move_gravity;
	//
		/*angle512			= angle_jikinerai512_auto(obj_player, src, angle512);*/
		mask1024(br.BULLET_REGIST_02_VECTOR_angle1024);
	//	data->angle512		= (angle512);
	//	data->speed256		= (speed256);
		regist_vector(h, br.BULLET_REGIST_00_speed256, br.BULLET_REGIST_02_VECTOR_angle1024);
		h->GRAVITY_BULLET_DATA_delta256 	= (br.BULLET_REGIST_07_VECTOR_jyuryoku_dan_delta256);
	//	data->speed256		= t256(0);
	}
}




/*---------------------------------------------------------
	�΂�܂��U�������w(�ʏ̃}���z�[��)���΂�܂��A�~�܂�e
	-------------------------------------------------------
	��邪�΂�܂��A�~�܂�e�B��e�ɕω�����
	-------------------------------------------------------
	angle512	ANGLE_JIKINERAI_DAN (999) �Ńv���C���[�_��
	delta256	�t���[������speed���ǂꂾ���������邩
	-------------------------------------------------------
	�e���������������Ɉڍs����̂ł����͖����Ȃ�\��B
---------------------------------------------------------*/
	//SPRITE *src, int speed256, int angle512, int a256, int next_angle512);
//SPRITE *src,
//	int br.BULLET_REGIST_00_speed256,				/* �e�� */
//	int br.BULLET_REGIST_02_VECTOR_angle1024,			/* �p�x */
//	int br.BULLET_REGIST_07_VECTOR_tomari_dan_delta256, /* �����萔 */
//	int br.BULLET_REGIST_06_VECTOR_tomari_dan_next_angle1024


// bullet_create_tomari_dan
static void s_bullet_create_tomari2_dan(void)
{
	SPRITE *h;
	h					= obj_add_00_tama_error();	/* �ےe�W(�Ԃq�f�a�Ύቩ��) */	/* ���Ԓe */
	if (NULL != h)
	{
		h->cx256			= obj_send1->cx256;
		h->cy256			= obj_send1->cy256;
		h->type 			= BULLET_MINI8_01_AKA+(0);/*S P_BULLET*/
		reflect_sprite_spec444(h, OBJ_BANK_SIZE_00_TAMA);
		h->m_Hit256R		= TAMA_ATARI_MARU16_PNG;
	//
		h->callback_mover	= bullet_move_tomari_dan;
		{
			/*src*/obj_send1->tmp_angleCCW1024	= br.BULLET_REGIST_02_VECTOR_angle1024;
			SPRITE *zzz_player;
			zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
			s_tmp_angleCCW1024_src_nerai_auto(zzz_player, /*src*/obj_send1);
			br.BULLET_REGIST_02_VECTOR_angle1024				= /*src*/obj_send1->tmp_angleCCW1024;
		}
	//	data->angle512		= (angle512);
	//	data->speed256		= (speed256);
		h->TOMARI_DAN_DATA_speed256 	= (br.BULLET_REGIST_00_speed256);
		regist_vector(h, t256(1.0)/*speed256*/, br.BULLET_REGIST_02_VECTOR_angle1024);
		h->TOMARI_DAN_DATA_init_vx256	= /*data->*/h->vx256;
		h->TOMARI_DAN_DATA_init_vy256	= /*data->*/h->vy256;
	//
		//data->state		= 0;
		h->TOMARI_DAN_DATA_delta256 	= (br.BULLET_REGIST_07_VECTOR_tomari_dan_delta256);
		if (ANGLE_NO_SHOT_DAN==br.BULLET_REGIST_06_VECTOR_tomari_dan_next_angle1024)/* ���ʂɌ����Ȃ��e�A�̏ꍇ */
		{
			;// 	h->jyumyou	= 800+64;
		}
		else
		{
			;// 	h->jyumyou	= 100+64;
			mask1024(br.BULLET_REGIST_06_VECTOR_tomari_dan_next_angle1024);
		}
		h->TOMARI_DAN_next_angle1024	= (br.BULLET_REGIST_06_VECTOR_tomari_dan_next_angle1024);
		h->jyumyou						= (TOMARI_DAN_LIMIT_00_MAX);
	}
}


/*---------------------------------------------------------
	���p�̑�e(��)�B
	-------------------------------------------------------
	�����ځ��������蔻��ł��B
	-------------------------------------------------------
	�e���������������Ɉڍs����̂ł����͖����Ȃ�\��B
---------------------------------------------------------*/

static void bullet_create_oodama22(SPRITE *src)
{
	SPRITE *h;
	h					= obj_add_00_tama_error();	/* ��e(��) �\������*/
	if (NULL != h)
	{
		h->cx256			= src->cx256;
		h->cy256			= src->cy256;
	//	angle512			= angle_jikinerai512_auto(obj_player, src, angle512);
	//	mask512(angle512);
	//
		int angle1024;
		angle1024			= (src->TOMARI_DAN_next_angle1024);
	//
		h->type 			= BULLET_OODAMA32_00_AOI+((((angle1024>>7)&(0x03))));
		reflect_sprite_spec444(h, OBJ_BANK_SIZE_00_TAMA);
		h->m_Hit256R		= TAMA_ATARI_OODAMA_08_PNG;
	//
		h->callback_mover	= move_bullet_vector;	/* (�v�����@�\) */
	//
		int speed256;
		speed256 = (t256(1.00));
		regist_vector(h, speed256, angle1024);
		h->BULLET_DATA_2nd_count	= (300);			/* 30�J�E���g�����甼���� */
		h->BASE_add_speed256		= (t256(1.05)); 	/* ���� */ /*add_speed256*/
	}
}

/*---------------------------------------------------------
	-------------------------------------------------------
	-------------------------------------------------------
	-------------------------------------------------------
	-------------------------------------------------------
---------------------------------------------------------*/


/*---------------------------------------------------------
	�P��A�ŏI�`�Ԃœ����Ă��邩�Ȃ薳���Ȓe�B
	-------------------------------------------------------
	delta256	�t���[�����ɉ������ɂǂꂾ���������邩
---------------------------------------------------------*/
//extern void bullet_create_hazumi_dan(void);
//SPRITE *src, int speed256, int angle512, int delta256, int bound_counts);

// [***090114	�ǉ�
//	SPRITE *src,
//	int br.BULLET_REGIST_00_speed256,
//	int br.BULLET_REGIST_02_VECTOR_angle1024,
//	int BULLET_REGIST_07_VECTOR_jyuryoku_dan_delta256,
//	int BULLET_REGIST_06_VECTOR_jyuryoku_dan_bound_counts

static void s_bullet_create_hazumi_dan(void)
{
	SPRITE *h;			/*�ےe�W(�Ԃq�f�a�Ύቩ��)*/
	h					= obj_add_00_tama_error();/*�Ή��e*/
	if (NULL != h)
	{
		h->cx256			= obj_send1->cx256;
		h->cy256			= obj_send1->cy256;
		h->type 			= BULLET_MINI8_00_SIRO+(4+br.BULLET_REGIST_06_VECTOR_jyuryoku_dan_bound_counts);/*S P_BULLET*/ /*SP_LASER*/
		reflect_sprite_spec444(h, OBJ_BANK_SIZE_00_TAMA);
		h->m_Hit256R		= TAMA_ATARI_MARU16_PNG;
	//
		h->callback_mover	= bullet_move_hazumi_dan;
		/*angle512			= angle_jikinerai512_auto(obj_player, src, angle512);*/
		mask1024(br.BULLET_REGIST_02_VECTOR_angle1024);
	//	h->HAZUMI_DAN_DATA_angle512 	= (angle512);
	//	h->HAZUMI_DAN_DATA_speed256 	= (speed256);
		regist_vector(h, br.BULLET_REGIST_00_speed256, br.BULLET_REGIST_02_VECTOR_angle1024);

		h->HAZUMI_DAN_DATA_delta256 	= (br.BULLET_REGIST_07_VECTOR_jyuryoku_dan_delta256);
	//	h->HAZUMI_DAN_DATA_sum256		= t256(0);
		h->HAZUMI_DAN_DATA_bound_counts = br.BULLET_REGIST_06_VECTOR_jyuryoku_dan_bound_counts;
	}
}



/*---------------------------------------------------------
	�������A�������i�e��ǉ�����
	-------------------------------------------------------

---------------------------------------------------------*/
/*(int)(512/24)*/

//	SPRITE *src,			/*	*/
//	int BULLET_REGIST_00_speed256,		/* �e�� */
//	int BULLET_REGIST_angle512, 		/* ���˒��S�p�x / ����@�\(���@�_��/��) */
//	int BULLET_REGIST_div_angle512, 	/* 1����n���������p�x */
//	int BULLET_REGIST_04_bullet_obj_type,	/* �e�O�� */
//	int BULLET_REGIST_06_n_way			/* ���e�� */
	/*
		�悭�l������AANGLE_JIKI_NERAI_KISUDAN �͕K�v�Ȃ��B
		ANGLE_JIKINERAI_DAN ��������Ηǂ��A
		���e���ɋ������w�肷��΋����e�B
		���e���Ɋ���w�肷��Ί�e�B
		������܂������ǁA�C���t���Ȃ�����ww�B
	*/
static void s_bullet_regist_multi_angle(void)
{
	{
		obj_send1->tmp_angleCCW1024 		= br.BULLET_REGIST_02_VECTOR_angle1024;
		SPRITE *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
		s_tmp_angleCCW1024_src_nerai_auto(zzz_player, obj_send1/*src*/ ); /*ANGLE_JIKI_NERAI_DAN???(original)*/ /* ��e�̏ꍇ�Ɏ��@�_�� */
		br.BULLET_REGIST_02_VECTOR_angle1024	= obj_send1->tmp_angleCCW1024;
	}
	int i_angle1024;
	i_angle1024 		= br.BULLET_REGIST_02_VECTOR_angle1024;
//	i_angle1024 		-= ((br.BULLET_REGIST_06_n_way+2)*((br.BULLET_REGIST_07_VECTOR_div_angle1024>>1)/*(int)(1024/48)*/)); /* 48�����e */ // /* ��/24 0.13089969389957471827 */
	i_angle1024 		-= ((br.BULLET_REGIST_06_n_way-1)*((br.BULLET_REGIST_07_VECTOR_div_angle1024>>1)/*(int)(1024/48)*/)); /* 48�����e */ // /* ��/24 0.13089969389957471827 */
/*
way 	�␳�p�x
1		-0/2
2		-1/2
3		-2/2
4		-3/2
5		-4/2
*/

	/* �O�̈׃}�X�N */
//	mask1024(i_angle1024);
//
	int i;
	for (i=0; i<br.BULLET_REGIST_06_n_way; i++)
	{
		SPRITE *h;
		// frame�̓t���[����-1 /*char *filename, int frame,*/
		h					= obj_add_00_tama_error(); /*_MING32_PNG*/
		if (NULL != h)
		{
			h->cx256			= obj_send1->cx256;
			h->cy256			= obj_send1->cy256;
			h->type 			= (br.BULLET_REGIST_04_bullet_obj_type);/*BULLET_UROKO14_01_AKA+*/ /*S P_BULLET*/
			reflect_sprite_spec444(h, OBJ_BANK_SIZE_00_TAMA);
			h->m_Hit256R		= TAMA_ATARI_COMMON16_PNG;
		//
			h->callback_mover	= move_bullet_vector;
			i_angle1024 		+= br.BULLET_REGIST_07_VECTOR_div_angle1024;	/*(2)*((int)(1024/48))*/ /* ��/12 0.26179938779914943654 */
			mask1024(i_angle1024);
			h->rotationCCW1024		= i_angle1024;		/* �`��p�p�x(����0�x�ō����(�����v���)) */
			regist_vector(h, br.BULLET_REGIST_00_speed256, i_angle1024);
		}
	}
}


/*---------------------------------------------------------
	�e��o�^����B
	-------------------------------------------------------
	�֐���(�N���X���ۂ�)�B���B
	�e�������̃G�t�F�N�g���̊֌W��A���e�����͑S���Z�߂�K�v������B
	-------------------------------------------------------
	�x�N�g���e�Ɗp�x�e�𓝍����邩������Ȃ��B
---------------------------------------------------------*/

global void bullet_regist_vector(void)
{
	#if 1
	switch (br.BULLET_REGIST_05_regist_type)
	{
	case REGIST_TYPE_00_MULTI_VECTOR:	s_bullet_regist_multi_angle();					break;
	case REGIST_TYPE_01_HAZUMI: 		s_bullet_create_hazumi_dan();					break;
	case REGIST_TYPE_02_GRAVITY02:		s_bullet_create_jyuryoku_dan(); 				break;
	case REGIST_TYPE_03_TOMARI: 		s_bullet_create_tomari2_dan();					break;
	}
	#else
	const void (*aaa_func[(4)])(void) =
	{
		s_bullet_regist_multi_angle,					/* REGIST_TYPE_00_MULTI_VECTOR	*/
		s_bullet_create_hazumi_dan, 					/* REGIST_TYPE_01_HAZUMI		*/
		s_bullet_create_jyuryoku_dan,					/* REGIST_TYPE_02_GRAVITY02 	*/
		s_bullet_create_tomari2_dan,					/* REGIST_TYPE_03_TOMARI		*/
	};
	(*aaa_func[ (br.BULLET_REGIST_05_regist_type/*&0x03*/) ])();	/* �e�֐��ɕ��򂷂� */
	#endif
}
