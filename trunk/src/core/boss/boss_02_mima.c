
#include "boss.h"

/*---------------------------------------------------------
	űÍí  ` Toho Imitation Style.
	vWFNgy[W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	2Ę Ł
	˘č iÔ? (łAPéAŠŽâ)
	Mitei Madakimattenai.
	-------------------------------------------------------
---------------------------------------------------------*/
#if 0/**/
/* {X¤ĘKi */
	#define target_x256 		user_data00 	/* ÚWxŔW */
	#define target_y256 		user_data01 	/* ÚWyŔW */
	#define vvv256				user_data02 	/* ÚWŔWÖĚB */
	#define time_out			user_data03 	/* §ŔÔ */
#endif

/*---------------------------------------------------------
	{XsŽ
---------------------------------------------------------*/

global void mima_01_keitai(SPRITE *src)
{
	/* eđÁÄČ˘ęÉÇÁ */
	if (DANMAKU_00==src->boss_base_danmaku_type)		/* eśŹIšČçeśŹ */
	{
		src->boss_base_state001++;
		src->boss_base_state001 &= (8-1);
		if (0==src->boss_base_state001)
		{
		//	if (1<difficulty)
			{
				/* ÚŽŔWđßé */
				src->target_x256 = (obj_player->x256);
				#if 0
			//	if (t256(16.0) > (src->target_y256))
				if ( (((obj_player->y256)-t256(16.0))>>16) )
				{
					src->target_y256 = t256(16.0);
				}
				else
				#endif
				{
					src->target_y256 = (obj_player->y256);
			//		src->target_y256 -= t256(8.0);	/* ??? ˝ĚŠI­˘ŠČ˘B */
				}
				/* eđßé */
				src->boss_base_danmaku_type 	= DANMAKU_0d_mima_sekkin;	/* eđZbg */
			}
		}
		else
		{
			static const POINT_u16 hosi_no_zahyou[(8)] = /* ÚWŔW */
			{	/* {(x),(y)}, */
				{(171),( 18)},	/* ělšŻÖ */
				{(202),( 92)},	/* klľŻ(5)Ö */
				{(257),(107)},	/* klľŻ(6)Ö */
				{(339),( 74)},	/* klľŻ(7)Ö */
				{(	5),(116)},	/* klľŻ(1)Ö */
				{(	6),( 46)},	/* klľŻ(2)Ö */
				{(107),( 26)},	/* klľŻ(3)Ö */
				{(133),( 78)},	/* klľŻ(4)Ö */
			};
			/* ÚŽŔWđßé */
			src->target_x256 = ((hosi_no_zahyou[src->boss_base_state001].x)<<8);
			src->target_y256 = ((hosi_no_zahyou[src->boss_base_state001].y)<<8);
			/* eđßé */
			src->boss_base_danmaku_type 	= DANMAKU_0c_hana_test; 	/* eđZbg */
		}
		danmaku_set_time_out(src);		/* eĚ§ŔÔđÝč(\ße˛ĆÉÝčłęÄ˘éWÔÉÝč) */
		/* */
		src->vvv256 = t256(1.0);
	}
	alice_yuudou_move_only(src);
	alice_yuudou_calc(src);
	bullet_angle_all_hamidasi_check();/*pxeĚňÝoľ`FbNđs¤(t[s¤KvÍČ˘)*/
}
