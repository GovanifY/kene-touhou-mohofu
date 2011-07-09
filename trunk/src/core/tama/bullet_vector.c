
#include "game_main.h"

/*---------------------------------------------------------
  �����͕핗 �` Toho Imitation Style.
  �v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�ړ��ʒe(�x�N�g���e)
	-------------------------------------------------------
	Todo:
	���݂̖͕핗�͓���e����������B�u����e�̕�����psp�ł͒x������v�̂Œe���ɂ���̂͌����I�ɖ����B
	����e�ɂ��Ȃ��Ă��܂�������������(������)�ʏ�e(�̑g�ݍ��킹��)�Ŏ����ł���B
	�u�����̒ʏ�e�̑g�ݍ��킹�v�Ȃ猴���I�ɑ����̂Œe���ɂ���̂͌����I�ɉ\�B
	(�P����)�ʏ�e��100��ނ��炢�ɑ��₵�Ă����v��������Ȃ��B
	-------------------------------------------------------
	���x���l����Ɠ���e�̒���sw itch()case:������̂͘_�O(�ɒx��)�B
	GCC�͂ǂ� version / �œK�� �ł�sw itch()case:��R�n�̓W�����v�e�[�u���ɓW�J���Ȃ��B
	�Œ�ł�if ()���A�W�����v�e�[�u���ɂ���B
	VC++(MSC)�ł̓W�����v�e�[�u���𖾎��ł��Ȃ����AGCC�̓W�����v�e�[�u��������B
	VC++�͂��̕�sw itch()case:��dafault:�������ꍇ�A�W�����v�e�[�u���𐶐�����B(�œK���ɂ��)
	-------------------------------------------------------
	�v����Ɂu�e���ϐg(��ԕύX)�v����������͒x���̂Ŗ����B
	�u�{�X���ϐg(��ԕύX)�v����������͏\�������B
---------------------------------------------------------*/

/*---------------------------------------------------------
	�e���쐬 / �����쐬
	-------------------------------------------------------
	��ŕω�����e����邩���B
	�{�X��|�������ɏ�����悤�ɂ��Ă���B
	-------------------------------------------------------
	�������Ė{�ƌ������Ă݂�B
	�{�Ƃ̒e���Ɂu�ϐg�@�\�Ȃ�Ė����v�B
	�P�ɒe�����X�g���āA�e���ԍ��ŏ��������Ă�C�����Ă����B
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

---------------------------------------------------------*/

global BULLET_REGIST br;

#define callback_2nd_regist 		callback_loser	/* �Qnd�o�^ */

/*---------------------------------------------------------
	�x�N�g���e(��{�I�ɂ́A�p�x�ω����Ȃ��e)
---------------------------------------------------------*/

/* ��{�e(���x�A�p�x�A�Ƃ��ɕω����Ȃ�) */
/* ���p�e(���x�ω�����) */

#define BASE_add_speed256			user_data00 	/* ������ */
#define TOKUSYU_DAN_DATA_delta256	user_data01 	/* ���������� */
#define TOMARI_DAN_next_angle1024	user_data02 	/* �ϐg��̊p�x */			/*tmp_angleCCW1024*/

/*---------------------------------------------------------
	�Ƃ肠�����ڍs�p(2)�ǉ��e�A��{�@�\
---------------------------------------------------------*/

#define BULLET_DATA_2nd_count		user_data04/*10*/	/* �Qnd �ϐg */
#define BULLET_DATA_2nd_speed		user_data05/*11*/	/* �Qnd ���x */
#define BULLET_DATA_2nd_vx256		user_data06/*12*/	/* �Qnd vx256 */
#define BULLET_DATA_2nd_vy256		user_data07/*13*/	/* �Qnd vy256 */

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

#define TOMARI_DAN_LIMIT_00_MAX 	(512+512)	/* �K���ɑ傫��1024==(512+64+�ő��~����) */
#define TOMARI_DAN_LIMIT_00_640 	(512+128)
#define TOMARI_DAN_LIMIT_00_576 	(512+64)
#define TOMARI_DAN_LIMIT_01_512 	(512+0)
#define TOMARI_DAN_LIMIT_02_000 	(32+0+0)

static void bullet_create_oodama22(SPRITE *src);
static void bullet_move_tomari_dan(SPRITE *src)
{
	if (TOMARI_DAN_LIMIT_00_640 < src->jyumyou)
	{
		/* �~�܂� */
	//	src->BASE_add_speed256 -= (1);
		src->BASE_add_speed256 -= (src->TOKUSYU_DAN_DATA_delta256);
		if (src->BASE_add_speed256 < t256(0))
		{
			src->BASE_add_speed256 = t256(0);
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
	�d�͒e�̈ړ�
	�����~���i�C�t(��)
---------------------------------------------------------*/

static void bullet_move_gravity(SPRITE *src)
{
	src->vy256 += src->TOKUSYU_DAN_DATA_delta256;
	move_bullet_vector(src);
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
		h->BASE_add_speed256		= (t256(1.05)); 	/* ���� */
	}
}


/*---------------------------------------------------------
	�d�͒e�B�A���X�̐l�`�����e�B
	�P��A�ŏI�`�Ԃœ����Ă��邩�Ȃ薳���Ȓe�Bbullet_create_sakuya_no_rot_knife
	�΂�܂��U�������w(�ʏ̃}���z�[��)���΂�܂��A�~�܂�e
	��邪�΂�܂��A�~�܂�e�B��e�ɕω�����
	-------------------------------------------------------
	�������ɉ������Ă������e�B
	�t���[�����Ōv�Z���Ă���̂ł��Ȃ�d�͉����x�����������Ȃ��Ƒ�ςȂ��ƂɁB
	delta256	�t���[�����ɉ������ɂǂꂾ���������邩
	-------------------------------------------------------
	angle512	ANGLE_JIKINERAI_DAN (999) �Ńv���C���[�_��
	delta256	�t���[������speed���ǂꂾ���������邩
	-------------------------------------------------------
	�e���������������Ɉڍs����̂ł����͖����Ȃ�\��B
---------------------------------------------------------*/
//	int br.BULLET_REGIST_00_speed256,					/* �e�� */
//	int br.BULLET_REGIST_02_VECTOR_angle1024,			/* �p�x */
//	int br.BULLET_REGIST_04_bullet_obj_type,				/* �e�O�� */	/*(BULLET_KNIFE20_05_AKA)*/
//	int br.BULLET_REGIST_06_VECTOR_HANERU_KAISUU  int bound_counts
//	int br.BULLET_REGIST_06_VECTOR_tomari_dan_next_angle1024
//	int br.BULLET_REGIST_07_VECTOR_legacy_dan_delta256, /* �����萔 */
static void s_bullet_create_haneru_dan(SPRITE *h)
{
	h->callback_mover	= bullet_move_gravity;
}

static void s_bullet_create_tomari_dan(SPRITE *h)// bullet_create_tomari_dan
{
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
	h->callback_mover				= bullet_move_tomari_dan;
}


static void s_bullet_regist_legacy_angle(void)
{
	SPRITE *h;
	h					= obj_add_00_tama_error();
	if (NULL != h)
	{
		h->cx256 = obj_send1->cx256;
		h->cy256 = obj_send1->cy256;
		//
		h->type 			= (br.BULLET_REGIST_04_bullet_obj_type);
		reflect_sprite_spec444(h, OBJ_BANK_SIZE_00_TAMA);
		h->m_Hit256R		= TAMA_ATARI_MARU16_PNG;
		//
		/*angle512			= angle_jikinerai512_auto(obj_player, src, angle512);*/
		mask1024(br.BULLET_REGIST_02_VECTOR_angle1024);
		regist_vector(h, br.BULLET_REGIST_00_speed256, br.BULLET_REGIST_02_VECTOR_angle1024);
		//
		h->TOKUSYU_DAN_DATA_delta256 	= (br.BULLET_REGIST_07_VECTOR_legacy_dan_delta256);
		if (LEGACY_REGIST_TYPE_00_HANERU==((br.BULLET_REGIST_03_VECTOR_regist_type)&0x03))
		{
			s_bullet_create_haneru_dan(h);		//	LEGACY_REGIST_TYPE_00_HANERU:
		}
		else
		{
			s_bullet_create_tomari_dan(h);		//	LEGACY_REGIST_TYPE_01_TOMARI:
		}
	}
}


/*---------------------------------------------------------
	�������A�������i�e��ǉ�����
	-------------------------------------------------------
	�悭�l������AANGLE_JIKI_NERAI_KISUDAN �͕K�v�Ȃ��B
	ANGLE_JIKINERAI_DAN ��������Ηǂ��A
	���e���ɋ������w�肷��΋����e�B
	���e���Ɋ���w�肷��Ί�e�B
	������܂������ǁA�C���t���Ȃ�����ww�B
	-------------------------------------------------------
way 	�␳�p�x
1		-0/2
2		-1/2
3		-2/2
4		-3/2
5		-4/2
---------------------------------------------------------*/
//	int BULLET_REGIST_00_speed256,			/* �e�� */
//	int BULLET_REGIST_02_angle512, 			/* ���˒��S�p�x / ����@�\(���@�_��/��) */
//	int BULLET_REGIST_04_bullet_obj_type,	/* �e�O�� */
//	int BULLET_REGIST_06_n_way				/* ���e�� */
//	int BULLET_REGIST_div_angle512, 		/* 1����n���������p�x */
static void s_bullet_regist_multi_angle(void)
{
	int i_angle1024;
	i_angle1024 		= br.BULLET_REGIST_02_VECTOR_angle1024;
//	i_angle1024 		-= ((br.BULLET_REGIST_06_n_way+2)*((br.BULLET_REGIST_07_VECTOR_div_angle1024>>1)/*(int)(1024/48)*/)); /* 48�����e */ // /* ��/24 0.13089969389957471827 */
	i_angle1024 		-= ((br.BULLET_REGIST_06_n_way-1)*((br.BULLET_REGIST_07_VECTOR_div_angle1024>>1)/*(int)(1024/48)*/)); /* 48�����e */ // /* ��/24 0.13089969389957471827 */
//	mask1024(i_angle1024);	/* �O�̈׃}�X�N */
	int i;
	for (i=0; i<br.BULLET_REGIST_06_n_way; i++)
	{
		SPRITE *h;
		h					= obj_add_00_tama_error();
		if (NULL != h)
		{
			h->cx256 = obj_send1->cx256;
			h->cy256 = obj_send1->cy256;
			h->type 			= (br.BULLET_REGIST_04_bullet_obj_type);
			reflect_sprite_spec444(h, OBJ_BANK_SIZE_00_TAMA);
			h->m_Hit256R		= TAMA_ATARI_COMMON16_PNG;
		//
			h->callback_mover	= move_bullet_vector;
			i_angle1024 		+= br.BULLET_REGIST_07_VECTOR_div_angle1024;
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


	/*---------------------------------------------------------
		���@(src)�_���e�w��̏ꍇ�A�p�x���v�Z
	---------------------------------------------------------*/
static void auto_jikinerai(void)
{
	if (ANGLE_JIKI_NERAI_DAN==br.BULLET_REGIST_02_VECTOR_angle1024)
	{
		tmp_angleCCW65536_jiki_nerai(obj_send1);
		br.BULLET_REGIST_02_VECTOR_angle1024	= ((obj_send1->tmp_angleCCW65536)>>6);
	}
}
global void bullet_regist_multi_vector_send1_xy_src(SPRITE *src)
{
	send1_xy(src);	/* �e��x256 y256 ���S���甭�e�B */
	auto_jikinerai();
	s_bullet_regist_multi_angle();
}

global void bullet_regist_multi_vector_direct(void)
{
	auto_jikinerai();
	s_bullet_regist_multi_angle();
}
global void bullet_regist_legacy_vector_send1_xy_src(SPRITE *src)
{
	send1_xy(src);	/* �e��x256 y256 ���S���甭�e�B */
	s_bullet_regist_legacy_angle();
}

global void bullet_regist_legacy_vector_direct(void)
{
	s_bullet_regist_legacy_angle();
}
