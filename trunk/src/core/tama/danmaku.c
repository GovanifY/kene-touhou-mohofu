
#include "game_main.h"

/*---------------------------------------------------------
 “•ϋ–Ν•ν•—  ` Toho Imitation Style.
  ƒvƒƒWƒFƒNƒgƒy[ƒW http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	’e–‹¶¬
	-------------------------------------------------------
	’e–‹‚ΝƒVƒXƒeƒ€‚ΕƒTƒ|[ƒg‚·‚ι\‘z‚ª‚ ‚θ‚ά‚·B(\‘z’†)
	ƒVƒXƒeƒ€‚ΕƒTƒ|[ƒg‚³‚κ‚ι‚ΖA‚Η‚Μƒ{ƒX‚ªA‚Η‚Μ’e–‹‚Ε‚ΰ‚‚Δ‚ι‚ζ‚¤‚Ι‚Θ‚θ‚ά‚·B
---------------------------------------------------------*/


/*---------------------------------------------------------
	©‹@‘_‚Ά’e‚Μp“x‚πvZ
---------------------------------------------------------*/

global /*static*/ void tmp_angleCCW65536_jikinerai(SPRITE *p, SPRITE *t)
{
	/* Θ—ª”Ε(’e‚Ζ©•ª‚ª‘ε‚«‚³‚ª“―‚¶‚Θ‚η‚Έ‚κ‚Θ‚ΆAα‚¤‚Ζ‚»‚Μ•ªλ·‚Ι‚Θ‚ι) */
	t->tmp_angleCCW65536		= (atan_65536(p->y256-t->y256, p->x256-t->x256));
}


/*---------------------------------------------------------
	©‹@‘_‚Ά’e‚Μp“x‚πvZ
---------------------------------------------------------*/

global /*static*/ void tmp_angleCCW1024_jikinerai(SPRITE *p, SPRITE *t)
{
	/* Θ—ª”Ε(’e‚Ζ©•ª‚ª‘ε‚«‚³‚ª“―‚¶‚Θ‚η‚Έ‚κ‚Θ‚ΆAα‚¤‚Ζ‚»‚Μ•ªλ·‚Ι‚Θ‚ι) */
	t->tmp_angleCCW1024 		= (atan_1024(p->y256-t->y256, p->x256-t->x256));
}

#include "danmaku_00_sonota.h"
//
#include "danmaku_01_sakuya.h"
#include "danmaku_01_mima.h"
#include "danmaku_01_alice.h"
#include "danmaku_01_rumia.h"
#include "danmaku_01_meirin.h"
#include "danmaku_01_chrno.h"
#include "danmaku_01_kaguya.h"
#include "danmaku_01_daiyousei.h"

/*---------------------------------------------------------
	‹¤’Κ’e–‹¶¬•”
	-------------------------------------------------------

---------------------------------------------------------*/

//#define DANMAKU_064_TIME	/*(150)*/
//#define DANMAKU_064_TIME	/*(ra_nd()&0xff)*/
//	src->boss_base_danmaku_time_out = (128+1);			/* βS’e–‹‚Μ”­¶Τ x 2 */
	#define DANMAKU_064_TIME	(64+1)
	#define DANMAKU_048_TIME	(48+1)
	#define DANMAKU_128_TIME	(128+1)
	#define DANMAKU_256_TIME	(256+1)
	#define DANMAKU_640_TIME	(640+1)

typedef struct
{
	void (*danmaku_callback)(SPRITE *sss);		/* ‰ϊ‰»Ϊ“®— */
	int danmaku_time;							/* Τ */
} DANMAKU_RESOURCE;
//	/*const*/static void (*danmaku_create_bbb[(DANMAKU_MAX/*16+1*/)])(SPRITE *sss) =
	/*const*/static DANMAKU_RESOURCE my_danmaku_resource[(DANMAKU_MAX/*16+1*/)] =
{
	#define danmaku_create_99_mitei danmaku_create_10_cirno_misogi
	{	NULL,									DANMAKU_128_TIME},	/* 00 */	/* ’e–‹¶¬‚µ‚Θ‚Ά */
	//													/* [η–ι] */
	{	danmaku_create_01_sakuya_misogi,		DANMAKU_064_TIME},	/* 01 */	/* η–ιβS’e–‹ / g5–Κ’†ƒ{ƒX η–ι u’ΚνU‚v */
	{	danmaku_create_02_24nerai,				DANMAKU_048_TIME},	/* 02 */	/* η–ι24’e–‹ (οpuƒ~ƒXƒfƒBƒƒNƒVƒ‡ƒ“‚ΰ‚Η‚«(1/2)v) */
	{	danmaku_create_03_11nife,				DANMAKU_064_TIME},	/* 03 */	/* η–ι11’e–‹ (οpuƒ~ƒXƒfƒBƒƒNƒVƒ‡ƒ“‚ΰ‚Η‚«(2/2)v) */
	{	danmaku_create_04_pink_hearts,			DANMAKU_128_TIME},	/* 04 */	/* g5–Κƒ{ƒX η–ι u’ΚνU‚1(1/2)v‚Ι‚Ώ‚ε‚Α‚Ζ‚Ύ‚―—‚½’e–‹(—\’θ) */
	{	danmaku_create_05_32way_dual,			DANMAKU_128_TIME},	/* 05 */	/* g5–Κƒ{ƒX η–ι u’ΚνU‚1/2(2/2)v‚Ι‚Ώ‚ε‚Α‚Ζ‚Ύ‚―—‚½’e–‹(—\’θ) */
	{	danmaku_create_06_luna_clock_32way, 	DANMAKU_128_TIME},	/* 06 */	/* g5–Κƒ{ƒX η–ι ¶Ϋuƒ‹ƒiƒNƒƒbƒN(1/2)v‚Ι‚Ώ‚ε‚Α‚Ζ‚Ύ‚―—‚½’e–‹(—\’θ) */
	{	danmaku_create_07_80way_dual_five,		DANMAKU_128_TIME},	/* 07 */	/* g5–Κƒ{ƒX η–ι u’ΚνU‚3(1/2)v‚Ι‚Ώ‚ε‚Α‚Ζ‚Ύ‚―—‚½’e–‹(—\’θ) */
	//													/* [ƒAƒƒX(‰Ό)] */
	{	danmaku_create_08_night_bird,			DANMAKU_128_TIME},	/* 08 */	/* g1–Κƒ{ƒX ƒ‹[ƒ~ƒA –ι•„uƒiƒCƒgƒo[ƒhv‚Ι‚Ώ‚ε‚Α‚Ζ‚Ύ‚―—‚½’e–‹(—\’θ) */
	{	danmaku_create_09_alice_sentakki,		DANMAKU_128_TIME},	/* 09 */	/* τ‘σ‹@’e–‹ */
	{	danmaku_create_0a_houka_kenran, 		DANMAKU_128_TIME},	/* 0a */	/* g”ό—ι ‰Ψ•„u–F‰ΨΊΰ£v‚Ι‚Ώ‚ε‚Α‚Ζ‚Ύ‚―—‚½’e–‹(—\’θ) */
	{	danmaku_create_0b_alice_doll,			DANMAKU_128_TIME},	/* 0b */	/* ƒAƒƒXl`’e–‹ */
	{	danmaku_create_0c_hana_test,			DANMAKU_256_TIME},	/* 0c */	/* –£–‚A‰Τ‚Δ‚·‚Ζ’e–‹ */
	{	danmaku_create_0d_mima_sekkin,			DANMAKU_256_TIME},	/* 0d */	/* –£–‚AΪ‹ί’e–‹ */
	{	danmaku_create_99_mitei,				DANMAKU_128_TIME},	/* 0e */	/* –Ά’θ’e–‹ */
	{	danmaku_create_99_mitei,				DANMAKU_128_TIME},	/* 0f */	/* –Ά’θ’e–‹ */
	//													/* [ƒ`ƒ‹ƒm(‰Ό)] */
	{	danmaku_create_10_cirno_misogi, 		DANMAKU_064_TIME},	/* 10 */	/* ƒ`ƒ‹ƒmβS’e–‹ */
	{	danmaku_create_11_tengu_shot,			DANMAKU_128_TIME},	/* 11 */	/* ·•ª‚Μ—d‰φ1(“V‹η—l?)‚ª‚‚Β’e–‹(—\’θ) */
	{	danmaku_create_12_aya_merin_test,		DANMAKU_128_TIME},	/* 12 */	/* •¶‚Ι”ό—ι‚Α‚Ϋ‚Ά’e–‹‚‚½‚Ή‚Δ‚έ‚ιƒeƒXƒg */
	{	danmaku_create_13_perfect_freeze,		DANMAKU_640_TIME},	/* 13 */	/* ƒp[ƒtƒFƒNƒgƒtƒ[ƒY */
	{	danmaku_create_99_mitei,				DANMAKU_128_TIME},	/* 14 */	/* –Ά’θ’e–‹ */
	{	danmaku_create_99_mitei,				DANMAKU_128_TIME},	/* 15 */	/* –Ά’θ’e–‹ */
	{	danmaku_create_99_mitei,				DANMAKU_128_TIME},	/* 16 */	/* –Ά’θ’e–‹ */
	{	danmaku_create_99_mitei,				DANMAKU_128_TIME},	/* 17 */	/* –Ά’θ’e–‹ */
	//
	{	danmaku_create_18_kaguya01, 			DANMAKU_064_TIME},	/* 18 */	/* ‚Ζ‚θ‚ ‚¦‚Έ‹­ψ‚Ι•Ο· */
	{	danmaku_create_19_kaguya04, 			DANMAKU_064_TIME},	/* 19 */	/* ‚Ζ‚θ‚ ‚¦‚Έ‹­ψ‚Ι•Ο· */
	{	danmaku_create_20_dai_yousei_midori,	DANMAKU_064_TIME},	/* 1a */	/* –Ά’θ’e–‹ */
	{	danmaku_create_21_dai_yousei_aka,		DANMAKU_064_TIME},	/* 1b */	/* –Ά’θ’e–‹ */
	{	danmaku_create_99_mitei,				DANMAKU_128_TIME},	/* 1c */	/* –Ά’θ’e–‹ */
	{	danmaku_create_99_mitei,				DANMAKU_128_TIME},	/* 1d */	/* –Ά’θ’e–‹ */
	{	danmaku_create_99_mitei,				DANMAKU_128_TIME},	/* 1e */	/* –Ά’θ’e–‹ */
	{	danmaku_create_99_mitei,				DANMAKU_128_TIME},	/* 1f */	/* –Ά’θ’e–‹ */
};
global void danmaku_set_time_out(SPRITE *src)
{
	src->boss_base_danmaku_time_out = my_danmaku_resource[(src->boss_base_danmaku_type/*-1*/)].danmaku_time;
}
global /*static*/ void danmaku_generator(SPRITE *src)
{
	if (DANMAKU_00 != src->boss_base_danmaku_type)	/* ’e–‹¶¬‚Ν•K—vH */
	{
		src->boss_base_danmaku_time_out--;			/* Τo‰ί‚·‚ι  */ 	/*fps_factor*/
		if (0 < src->boss_base_danmaku_time_out)	/* ΤΨ‚κH */
		{
			/* ’e–‹¶¬’† */
			(my_danmaku_resource[(src->boss_base_danmaku_type/*-1*/)].danmaku_callback)(src);
		}
		else
		{
			src->boss_base_danmaku_type = DANMAKU_00;			/* ’e–‹¶¬I—Ή */
		}
	}
}
