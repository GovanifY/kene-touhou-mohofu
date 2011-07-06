
#include "douchu.h"

/*---------------------------------------------------------
	“Œ•û–Í•í•—	` Toho Imitation Style.
	ƒvƒƒWƒFƒNƒgƒy[ƒW http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	ƒUƒR‚Í“Z‚ß‚é—\’èB
---------------------------------------------------------*/
//extern void zako_anime_type01(SPRITE *src, int type_base);
//extern void zako_anime_type02(SPRITE *src, int type_base);

#if 0/* ‚ß‚à */
/* ƒ{ƒX‹¤’Ê‹KŠi */
	#define target_x256 		user_data00 	/* –Ú•WxÀ•W */
	#define target_y256 		user_data01 	/* –Ú•WyÀ•W */
	#define vvv256				user_data02 	/* –Ú•WÀ•W‚Ö‚Ì“ž’BŠ„‡ */
	#define boss_time_out		user_data03 	/* §ŒÀŽžŠÔ */
#endif


/*---------------------------------------------------------
	ƒUƒRˆÚ“®‹¤’Ê
---------------------------------------------------------*/
	#define target_x256 			user_data00
	#define target_y256 			user_data01
	#define radius256				user_data02
static void zako_move_type11_radius(SPRITE *src)
{
	src->cx256 = ((sin1024((src->tmp_angleCCW1024))*(src->radius256))>>8)+(src->target_x256);/* CCW‚Ìê‡ */
	src->cy256 = ((cos1024((src->tmp_angleCCW1024))*(src->radius256))>>8)+(src->target_y256);
}
	#define speed256				user_data04 	/* ‘¬“x */
	#define kaisu_nnn				user_data05 	/* ŒJ‚è•Ô‚µ‚½‰ñ”‚ÌƒJƒEƒ“ƒ^ */

/*---------------------------------------------------------
	ƒUƒRƒAƒjƒ‹¤’Ê
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
#include "zako_11_mahoujin.h"			/* –‚•ûw[A/B] */

	#define limit_y256				user_data05 	/* ‰¼‘z’n–Ê */
#include "zako_10_inyou1.h" 			/* —³Šª ‰A—z‹Ê */

//	#define speed256				user_data04 	/* ‘¬“x */
	#define vx_r					user_data05 	/* ‰ñ“]•ûŒü(‰ñ“]—Ê) */
	#define px256					user_data06 	/* ‰¼‘zxˆÊ’u(”½“]—p) */
#include "zako_11_tatsumaki1.h" 		/* —³Šª ‰A—z‹Ê */

//	#define target_x256 			user_data00
//	#define target_y256 			user_data01
//	#define radius256				user_data02
	/* “G‚ÌŒü‚« tmp_angleCCW1024  */
#include "zako_12_kakomi1.h"			/* —d‰ö */
#include "zako_04_aka_kedama.h" 		/* —d‰ö */


//	#define speed256				user_data04
	#define max_y256				user_data05
#include "zako_03_yukari2.h"			/* –Ñ‹Ê */
	#define target_x256 			user_data00
	#define target_y256 			user_data01
#include "zako_06_midori_kedama1.h" 	/* –Ñ‹Ê */
	#define wait1					user_data04 	/* ã‰º‚É“®‚¢‚½ƒJƒEƒ“ƒ^ */
//	#define kaisu_nnn				user_data05 	/* ŒJ‚è•Ô‚µ‚½‰ñ”‚ÌƒJƒEƒ“ƒ^ */
#include "zako_07_midori_kedama2.h" 	/* –Ñ‹Ê */

	#define target_x256 			user_data00
	#define target_y256 			user_data01
//	#define speed256				user_data04
#include "zako_08_kedama1.h"			/* –Ñ‹Ê */
	#define kougeki_type			user_data05
	#define USE_X_HOUKOU (1)
#include "zako_09_kedama2.h"			/* –Ñ‹Ê */


	/* “G‚ÌŒü‚« tmp_angleCCW1024  */
	#define gra_anime_type			user_data04 	/* ƒOƒ‰ƒ^ƒCƒv */
#include "zako_01_obake1.h" 			/* [C—d¸]‚»‚Ì‘¼ƒUƒR */
//	#define target_x256 			user_data00
//	#define target_y256 			user_data01
//	#define radius256				user_data02
	#define flag1					user_data05
#include "zako_02_obake2.h" 			/* [C—d¸]‚»‚Ì‘¼ƒUƒR */
	#define USE_X_HOUKOU_karasu (0)
	#define KARASU3_houkou			user_data05
#include "zako_05_karasu1.h"			/* [C—d¸]‚»‚Ì‘¼ƒUƒR */


//	#define speed256				user_data04
#include "zako_13_meido1.h" 			/* [B—d¸]’†—d¸ */
//	#define speed256				user_data04
#include "zako_14_meido2.h" 			/* [B—d¸]’†—d¸ */
	#define clip_left256			user_data00
	#define clip_right256			user_data01
	#define turnspeed1024			user_data03
//	#define speed256				user_data04
//	#define kaisu_nnn				user_data05 	/* ƒ^[ƒ“‚·‚é‰ñ” */
#include "zako_15_meido3.h" 			/* [B—d¸]’†—d¸ */
//	#define speed256				user_data04
#include "zako_16_meido4.h" 			/* [B—d¸]’†—d¸ */

	/* “G‚ÌŒü‚« tmp_angleCCW1024  */
//	#define speed256				user_data04 	/* ‘¬“x */
#include "zako_17_ao_yousei1.h" 		/* [A—d¸]¬—d¸ */
//	#define kaisu_nnn				user_data05 	/* ’e‚ðŒ‚‚Â‰ñ” */
#include "zako_18_ao_yousei2.h" 		/* [A—d¸]¬—d¸ */
	#define USE_X_HOUKOU_ao3 (0)
	#define KARASU3_houkou			user_data05
#include "zako_19_ao_yousei3.h" 		/* [A—d¸]¬—d¸ */
	/* “G‚ÌŒü‚« tmp_angleCCW1024  */
//	#define speed256				user_data04 	/* ‘¬“x */
#include "zako_20_ao_yousei4.h" 		/* [A—d¸]¬—d¸ */


/*
src->jyumyou		 ---a bc--
src->yx_an im_frame  yyyy xxxx
src->yx_an im_frame  --1a --bc
*/
