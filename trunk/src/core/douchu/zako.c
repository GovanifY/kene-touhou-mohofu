
#include "douchu.h"

/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�U�R�͓Z�߂�\��B
---------------------------------------------------------*/
//extern void zako_anime_type01(SPRITE *src, int type_base);
//extern void zako_anime_type02(SPRITE *src, int type_base);

#if 0/* �߂� */
/* �{�X���ʋK�i */
	#define target_x256 		user_data00 	/* �ڕWx���W */
	#define target_y256 		user_data01 	/* �ڕWy���W */
	#define vvv256				user_data02 	/* �ڕW���W�ւ̓��B���� */
	#define boss_time_out		user_data03 	/* �������� */
#endif


/*---------------------------------------------------------
	�U�R�ړ�����
---------------------------------------------------------*/
	#define target_x256 			user_data00
	#define target_y256 			user_data01
	#define radius256				user_data02
static void zako_move_type11_radius(SPRITE *src)
{
	src->cx256 = ((sin1024((src->tmp_angleCCW1024))*(src->radius256))>>8)+(src->target_x256);/* CCW�̏ꍇ */
	src->cy256 = ((cos1024((src->tmp_angleCCW1024))*(src->radius256))>>8)+(src->target_y256);
}
	#define speed256				user_data04 	/* ���x */
	#define kaisu_nnn				user_data05 	/* �J��Ԃ����񐔂̃J�E���^ */

/*---------------------------------------------------------
	�U�R�A�j������
---------------------------------------------------------*/
static void zako_anime_type01(SPRITE *src, int type_base)
{
	src->type				= (type_base)+((((src->tmp_angleCCW1024)&(1024-1)))>>7);
}
static void zako_anime_type02(SPRITE *src, int type_base)
{
	src->type				= (type_base)+((src->jyumyou>>2)&(4-1));
}


/*---------------------------------------------------------

---------------------------------------------------------*/
	#define MAHOU_TEKI_wait1		user_data04
	#define MAHOU_TEKI_wait2		user_data05
	#define start_y256				user_data06
#include "zako_11_mahoujin.h"			/* �����w[A/B] */

	#define limit_y256				user_data05 	/* ���z�n�� */
#include "zako_10_inyou1.h" 			/* ���� �A�z�� */

//	#define speed256				user_data04 	/* ���x */
	#define vx_r					user_data05 	/* ��]����(��]��) */
	#define px256					user_data06 	/* ���zx�ʒu(���]�p) */
#include "zako_11_tatsumaki1.h" 		/* ���� �A�z�� */

//	#define target_x256 			user_data00
//	#define target_y256 			user_data01
//	#define radius256				user_data02
	/* �G�̌��� tmp_angleCCW1024  */
#include "zako_12_kakomi1.h"			/* �d�� */
#include "zako_04_aka_kedama.h" 		/* �d�� */


//	#define speed256				user_data04
	#define max_y256				user_data05
#include "zako_03_yukari2.h"			/* �ы� */
	#define target_x256 			user_data00
	#define target_y256 			user_data01
#include "zako_06_midori_kedama1.h" 	/* �ы� */
	#define wait1					user_data04 	/* �㉺�ɓ������J�E���^ */
//	#define kaisu_nnn				user_data05 	/* �J��Ԃ����񐔂̃J�E���^ */
#include "zako_07_midori_kedama2.h" 	/* �ы� */

	#define target_x256 			user_data00
	#define target_y256 			user_data01
//	#define speed256				user_data04
#include "zako_08_kedama1.h"			/* �ы� */
	#define kougeki_type			user_data05
	#define USE_X_HOUKOU (1)
#include "zako_09_kedama2.h"			/* �ы� */


	/* �G�̌��� tmp_angleCCW1024  */
	#define gra_anime_type			user_data04 	/* �O���^�C�v */
#include "zako_01_obake1.h" 			/* [C�d��]���̑��U�R */
//	#define target_x256 			user_data00
//	#define target_y256 			user_data01
//	#define radius256				user_data02
	#define flag1					user_data05
#include "zako_02_obake2.h" 			/* [C�d��]���̑��U�R */
	#define USE_X_HOUKOU_karasu (0)
	#define KARASU3_houkou			user_data05
#include "zako_05_karasu1.h"			/* [C�d��]���̑��U�R */


//	#define speed256				user_data04
#include "zako_13_meido1.h" 			/* [B�d��]���d�� */
//	#define speed256				user_data04
#include "zako_14_meido2.h" 			/* [B�d��]���d�� */
	#define clip_left256			user_data00
	#define clip_right256			user_data01
	#define turnspeed1024			user_data03
//	#define speed256				user_data04
//	#define kaisu_nnn				user_data05 	/* �^�[������� */
#include "zako_15_meido3.h" 			/* [B�d��]���d�� */
//	#define speed256				user_data04
#include "zako_16_meido4.h" 			/* [B�d��]���d�� */

	/* �G�̌��� tmp_angleCCW1024  */
//	#define speed256				user_data04 	/* ���x */
#include "zako_17_ao_yousei1.h" 		/* [A�d��]���d�� */
//	#define kaisu_nnn				user_data05 	/* �e������ */
#include "zako_18_ao_yousei2.h" 		/* [A�d��]���d�� */
	#define USE_X_HOUKOU_ao3 (0)
	#define KARASU3_houkou			user_data05
#include "zako_19_ao_yousei3.h" 		/* [A�d��]���d�� */
	/* �G�̌��� tmp_angleCCW1024  */
//	#define speed256				user_data04 	/* ���x */
#include "zako_20_ao_yousei4.h" 		/* [A�d��]���d�� */


/*
src->jyumyou		 ---a bc--
src->yx_an im_frame  yyyy xxxx
src->yx_an im_frame  --1a --bc
*/
