
#include "game_main.h"

/*---------------------------------------------------------
  東方模倣風 ～ Toho Imitation Style.
  プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	レイヤーシステム
	-------------------------------------------------------
	レイヤーシステムは「弾幕」の文字通り、
	弾を単体としては認識せず、幕として認識するシステム。
	-------------------------------------------------------
---------------------------------------------------------*/

/*---------------------------------------------------------
	弾幕の有効範囲。有効範囲外は画面外と判断し、アクションが起きる。
	アクションの種類は、
		弾が消されるもの。
		弾が反射するもの。
	等がある。
---------------------------------------------------------*/


//	/*extern*/global POINT256 bullet_clip_min;		/*	範囲(最小値) */
//	/*extern*/global POINT256 bullet_clip_max;		/*	範囲(最大値) */

#if 0
/*---------------------------------------------------------
	弾幕レイヤーアクション#00
	-------------------------------------------------------
	弾幕の喰み出しチェックを行う(毎フレーム行う必要はない)。
	弾幕が画面外の場合は、弾を消す。
---------------------------------------------------------*/

global void danmaku_action_00_gamen_gai_nara_tama_wo_kesu(void)
{
	unsigned int ii;
	for (ii=0; ii<OBJ_POOL_00_TAMA_MAX; ii++)/* 全弾調べる。 */
	{
		SPRITE *obj;
		obj = &obj99[OBJ_HEAD_00_TAMA+ii];
		if (0!=(obj->flags&SP_FLAG_COLISION_CHECK))/* あたり判定の無い弾は消さない。(発弾エフェクト用) */
		{
			/* 画面外の場合は弾を消す。 */
			if (
			(0 < (rect_clip.bullet_clip_min.x256)-(obj->cx256) ) ||
			(0 > (rect_clip.bullet_clip_max.x256)-(obj->cx256) ) ||
			(0 < (rect_clip.bullet_clip_min.y256)-(obj->cy256) ) ||
			(0 > (rect_clip.bullet_clip_max.y256)-(obj->cy256) ) )
			{
				obj->jyumyou = JYUMYOU_NASI;
			}
		}
	}
}
#endif

#if 0
/*---------------------------------------------------------
	弾幕アクション#01
	-------------------------------------------------------
	弾の喰み出しチェックを行う(毎フレーム行う必要はない)。
	弾幕の左右が画面外の場合は反射する。
	弾幕の上下が画面外の場合は弾を消す。
---------------------------------------------------------*/

global void danmaku_action_01_gamen_gai_nara_hansya_nomi(void)
{
	unsigned int ii;
	for (ii=0; ii<OBJ_POOL_00_TAMA_MAX; ii++)/* 全弾調べる。 */
	{
		SPRITE *obj;
		obj = &obj99[OBJ_HEAD_00_TAMA+ii];
		if (0!=(obj->flags&SP_FLAG_COLISION_CHECK))/* あたり判定の無い弾は消さない。(発弾エフェクト用) */
		{
			#if 1/* 検討中 */
			int test_flag;
			test_flag = 0;
			if (
				(obj->cx256 < t256(GAME_X_OFFSET)/*rect_clip.bullet_clip_min.x256*/) )
			{
				if (0==(obj->hatudan_register_spec_data & TAMA_SPEC_KABE_SAYUU_HANSYA_BIT))
						{	test_flag = 1;			}
				else	{	obj->jyumyou = JYUMYOU_NASI;	}
			}
			else
			if (
				(obj->cx256 > t256(GAME_WIDTH)/*rect_clip.bullet_clip_max.x256*/) )
			{
				if (0==(obj->hatudan_register_spec_data & TAMA_SPEC_KABE_SAYUU_HANSYA_BIT))
						{	test_flag = 1;			}
				else	{	obj->jyumyou = JYUMYOU_NASI;	}
			}
			if (0 != test_flag)
			{
					obj->hatudan_register_spec_data |= TAMA_SPEC_KABE_SAYUU_HANSYA_BIT;
				//	obj->rotationCCW1024 += (1024/4);/* 90度回転 */ 		/* ダメ[※2] */
				//	obj->rotationCCW1024 += (1024*3/4);/* -90度回転 */		/* ダメ[※2] */
					obj->rotationCCW1024 = (1024)-(obj->rotationCCW1024);	/* 反転[※1] */
					mask1024(obj->rotationCCW1024);
			}
			if (
				(obj->cy256 < - t256(100)/*rect_clip.bullet_clip_min.y256*/)||
				(obj->cy256 >	t256(272)/*rect_clip.bullet_clip_max.y256*/)
			)
			{
				obj->jyumyou = JYUMYOU_NASI;
			}
			#endif
		}
	}
}

#endif




#if 0
/*---------------------------------------------------------
	弾幕アクション#02
	-------------------------------------------------------
	弾幕の喰み出しチェックを行う(毎フレーム行う必要はない)。
	弾幕の左右が画面外の場合は反射減速。
	弾幕の上下が画面外の場合は弾を消す。
---------------------------------------------------------*/

global void danmaku_action_02_gamen_gai_nara_hansya_gensoku(void)
{
	unsigned int ii;
	for (ii=0; ii<OBJ_POOL_00_TAMA_MAX; ii++)/* 全弾調べる。 */
	{
		SPRITE *obj;
		obj = &obj99[OBJ_HEAD_00_TAMA+ii];
		if (0!=(obj->flags&SP_FLAG_COLISION_CHECK))/* あたり判定の無い弾は消さない。(発弾エフェクト用) */
		{
			{
				/* [左の壁で反射する] 弾が弾幕設定領域画面より少し(4[dot])手前の位置に来た場合。 */
				if (0 < (rect_clip.bullet_clip_min.x256) + t256(4) - (obj->cx256) )
				{
					goto common_hansya;
				}
				else
				/* [右の壁で反射する] 弾が弾幕設定領域画面より少し(4[dot])手前の位置に来た場合。 */
				if (0 > (rect_clip.bullet_clip_max.x256) - t256(4) - (obj->cx256) )
				{
					goto common_hansya;
				}
				goto done_hansya;/*(反射しない)*/
			common_hansya:/*(反射する可能性あり)*/
				/* 既に反射している場合は反射しないで、弾を消す。 */
				if (0!=(obj->hatudan_register_spec_data & TAMA_SPEC_KABE_SAYUU_HANSYA_BIT))
				{	/*(反射処理を行わない)*/
					obj->jyumyou = JYUMYOU_NASI;/*(弾を消す)*/
				}
				else
			//	if (JYUMYOU_NASI != obj->jyumyou)
				{	/*(反射処理を行う)*/
					obj->hatudan_register_spec_data |= TAMA_SPEC_KABE_SAYUU_HANSYA_BIT;
				//	obj->rotationCCW1024 += (1024*3/4);/* -90度回転 */		/* ダメ[※2] */
				//	obj->rotationCCW1024 += (1024/4);/* 90度回転 */ 		/* ダメ[※2] */
					obj->rotationCCW1024 = (1024)-(obj->rotationCCW1024);	/* 反転[※1] */
				//	obj->hatudan_system_speed256					= ((obj->hatudan_system_speed256)>>1);/*減速*/
					obj->hatudan_register_speed65536				= ((obj->hatudan_register_speed65536)>>(1));/*減速*/
					/*(減速しても、速すぎる場合のリミット)*/
					#define mmm_MAX_SPEED_65536 	(t256(1.0)<<8)
				//	if (mmm_MAX_SPEED_65536 < (obj->hatudan_register_speed65536))
				//	{
				//		obj->hatudan_register_speed65536 = mmm_MAX_SPEED_65536;
				//	}
					obj->hatudan_register_speed65536 = psp_min(obj->hatudan_register_speed65536, mmm_MAX_SPEED_65536);
					mask1024(obj->rotationCCW1024);
				}
			done_hansya:/*(反射処理終わり)*/
				;
			}
			/* 上下の場合は弾を消す。 */
			if (
				(0 < (rect_clip.bullet_clip_min.y256) - (obj->cy256) )||
				(0 > (rect_clip.bullet_clip_max.y256) - (obj->cy256) )
			)
			{
				obj->jyumyou = JYUMYOU_NASI;
			}
		}
	}
}
#endif

/*---------------------------------------------------------
	弾幕コールバックシステム
	-------------------------------------------------------
	初弾時に弾幕コントロール(コールバック)を登録しておき、
	それを操作する。
---------------------------------------------------------*/

global void danmaku_system_callback(void)
{
	unsigned int jj;
	for (jj=0; jj<DANMAKU_LAYER_04_MAX; jj++)/*(登録を全部調べる。)*/
	{
		/*(登録されていない場合は何もしない。)*/
		if (NULL != card.danmaku_callback[jj])
		{
			unsigned int ii;
			for (ii=0; ii<OBJ_POOL_00_TAMA_MAX; ii++)/*(弾を全弾調べる。)*/
			{
				SPRITE *obj;
				obj = &obj99[OBJ_HEAD_00_TAMA+ii];
				if (0!=(obj->flags&SP_FLAG_COLISION_CHECK))/*(あたり判定の無い弾は対象外。発弾エフェクト用。)*/
				{
					if (jj == ((obj->hatudan_register_spec_data)&(0x03)) )	/* 弾幕[n]なら */
					{
						(card.danmaku_callback[jj])(obj);
					}
				}
			}
		}
	}
}
