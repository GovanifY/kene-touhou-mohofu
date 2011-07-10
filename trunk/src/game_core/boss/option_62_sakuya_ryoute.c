
/*-------------------------------------------------------
	s->cx256	ナイフのx座標
	data->cx256 ノードのx座標
	s->w/2		ナイフの横幅の半分
	ナイフのx座標 == ノードのx座標 + co_s1024((data->angle1024)) x length - ナイフの横幅の半分
	#define ryoute_knife_length256 t256(128)
	⊿angle ≒ 任意/(2π×ryoute_knife_length/√2)
---------------------------------------------------------*/

#include "boss.h"//#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	咲夜 両手ナイフカード
	-------------------------------------------------------
	(r35)ナイフの絶対座標位置を一度で求める考え方から、
	相対座標(ナイフの移動ベクトル量、現在位置からの差分)を求める考え方に移行し、
	プログラムをシンプルに修正。
	-------------------------------------------------------
	使用レジスタ
//	REG_08_REG0 	カウンタ。
	REG_09_REG1 	発弾用カウンタ。(左右で共用)
//	REG_0a_REG2 	aaa
//	REG_0b_REG3 	bbb
	-------------------------------------------------------
	ボス共通規格使用データー:
---------------------------------------------------------*/

/*---------------------------------------------------------
	両手ナイフ親(紫)
	-------------------------------------------------------
	length	teki-obj_player間の距離/√2
	r_or_l	0==右	1==左
	-------------------------------------------------------
	ToDo:
	弾システムに移行する為、廃止予定。
	カードスクリプトシステムが稼動する為には、発弾を一ヶ所に統合しないと
	速度的な面でこのままの形態では無理があるので、
	(ここの発弾処理のせいで他の弾が処理落ちする)
	ここの発弾処理を hatudan_system_regist_single に置きかえる必要がある。
---------------------------------------------------------*/


// バグ対策。 user_data02 良くワカンナイけど空けとく。()
	/*-r34旧仕様メモ*/ /* r(-1)==+128, l(+1)==-128 */	/* -1==l, +1==r */
	/*(==TAMA_SPEC_AKA_AO_KUNAI_BIT; tama_systemの HATSUDAN_04_tama_spec に移行予定)*/
	#define RYOUTE_KNIFE_DATA_l_or_r		user_data03


#define KNIFE_TYPE_RIGHT (0)

/*---------------------------------------------------------
	[弾幕グループ(1)セクション]
	-------------------------------------------------------
---------------------------------------------------------*/

static void sakuya_ryoute_knife_danmaku_01_callback(SPRITE *src)
{
	/*(ナイフが進行する時に曲がる角度[量])*/
	{
		int aaa;
		aaa = (1);
	//	if (KNIFE_TYPE_RIGHT!=src->RYOUTE_KNIFE_DATA_l_or_r)
		if (KNIFE_TYPE_RIGHT==src->RYOUTE_KNIFE_DATA_l_or_r)
		{
			aaa = -aaa;
		}
		/* 描画用角度(下が0度で左回り(反時計回り)) */
		src->rotationCCW1024 += (aaa);			/*fps_factor*/	/* R(0)== +2, L(1)== -2 */		/*(r35-)*/
	}
//不要	mask1024(src->rotationCCW1024);//(sincosの場合不要)
	mask1024(src->rotationCCW1024);/*必ず要る*/
	/*(ナイフの相対移動量。ベクトル。)*/
	#if (0)//
	src->cx256		+= ((si n1024((src->rotationCCW1024)))<<1);/*fps_factor*/	/* CCWの場合 */
	src->cy256		+= ((co s1024((src->rotationCCW1024)))<<1);/*fps_factor*/
	#else
	{
		int sin_value_t256; 	//	sin_value_t256 = 0;
		int cos_value_t256; 	//	cos_value_t256 = 0;
		int256_sincos1024( (src->rotationCCW1024), &sin_value_t256, &cos_value_t256);
		/*(ナイフの進行移動量)*/
	//	src->cx256 += ((sin_value_t256)<<1);/*fps_factor*/
	//	src->cy256 += ((cos_value_t256)<<1);/*fps_factor*/
		src->cx256 += ((sin_value_t256) );/*fps_factor*/
		src->cy256 += ((cos_value_t256) );/*fps_factor*/
	}
	#endif
	/*(子ナイフの発弾間隔)*/
	/*(このカウンタは左右で共用なので2倍進む)*/
	REG_09_REG1++;
	if (((10)+(10)) < REG_09_REG1)
	{
		REG_09_REG1 = (0);
		/* 親ナイフ(紫)が、1回分の子ナイフ弾(青)を4弾、撃つ。 */
		/*(子ナイフ弾を4弾、撃つ)*/
		set_REG_DEST_XY(src);	/* 子ナイフ弾は親ナイフ(紫)弾の位置から発弾。 */
		//
		HATSUDAN_02_speed_offset		= t256(0);/*(テスト)*/
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_3000_EFFECT_NONE)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
		HATSUDAN_05_bullet_obj_type 	= BULLET_KNIFE20_04_AOI;	/* [青ナイフ弾] */
		{
			int kk_speed256;
			kk_speed256 = (t256(1.6));
			unsigned int jj;
			for (jj=0; jj<4; jj++)/*(1回で4弾、撃つ)*/
			{
				kk_speed256 += (t256(0.2));
				HATSUDAN_01_speed256		= (kk_speed256);
				const /*int*/ /*u16*/s32/*(s16で可能)*/ jj_bbb[4] =
				{
					(int)((65536*6/12)-(65536*6/12)),	//	kk_speed256 = (t256(1.8));
					(int)((65536*6/12)-(65536*5/12)),	//	kk_speed256 = (t256(2.0));
					(int)((65536*6/12)-(65536*4/12)),	//	kk_speed256 = (t256(2.2));
					(int)((65536*6/12)-(65536*3/12)),	//	kk_speed256 = (t256(2.4));
				};
				#if (1)/*r35-*/
				{	int aaa 	= (((int)(jj_bbb[jj]) ));
					/*(差分は本来負数で持つべきだが、とりあえず正数で持っている。そこで左右の判断を逆にする)*/
					if (KNIFE_TYPE_RIGHT!=src->RYOUTE_KNIFE_DATA_l_or_r)	{	aaa = -aaa; 	}
				//	HATSUDAN_03_angle65536 = (aaa) + (deg1024to65536(src->rotationCCW1024)); /* 「1周が1024分割」から「1周が65536分割」へ変換する。 */
					HATSUDAN_03_angle65536 = (aaa) + (deg1024to65536(src->rotationCCW1024)); /* 「1周が1024分割」から「1周が65536分割」へ変換する。 */
				}
				#endif
				hatudan_system_regist_single();
			}
		}
	}
}
/*---------------------------------------------------------
	[初期化セクション]
	-------------------------------------------------------
---------------------------------------------------------*/
#if (0)/*(弾幕システムに移行する場合)*/
local void bullet_init_sakuya_ryoute_knife(SPRITE *src)
{
//	card.danmaku_callback[0] = danmaku_00_standard_angle_mover;/*(通常弾用)*/
	card.danmaku_callback[1] = sakuya_ryoute_knife_danmaku_01_callback;/*(親ナイフ。)*/
//	card.danmaku_callback[2] = NULL;/*(未使用)*/
//	card.danmaku_callback[3] = NULL;/*(未使用)*/
}
#endif
/*---------------------------------------------------------
	[発弾セクション]
	-------------------------------------------------------
---------------------------------------------------------*/
global void bullet_create_sakuya_ryoute_knife(SPRITE *src)
{
	REG_09_REG1 	= (0);/*(左右共用発弾カウンタ)*/
	//
	/* 自機狙い角を HATSUDAN_03_angle65536 に作成 */
//	REG_02_DEST_X	= (src->cx256);/*(咲夜の現在位置座標)*/
//	REG_03_DEST_Y	= (src->cy256);/*(咲夜の現在位置座標)*/
	set_REG_DEST_XY(src);	/* ナイフ(紫)弾は咲夜の位置から発弾。 */
	calculate_jikinerai();
	/* 「1周が65536分割」から「1周が1024分割」へ変換する。 */
	HATSUDAN_03_angle1024 = (deg65536to1024(HATSUDAN_03_angle65536));
	//
	#if (0)
	/*(弾幕システムに移行する場合)*/
//	set_REG_DEST_XY(src);	/* ナイフ(紫)弾は咲夜の位置から発弾。 */	/*(既に自機狙いの時に設定済み)*/
	HATSUDAN_01_speed256			= (0);				/* 弾速(0) */
	HATSUDAN_02_speed_offset		= t256(0);/*(てすと)*/
	HATSUDAN_05_bullet_obj_type 	= (BULLET_KNIFE20_06_YUKARI);	/* ナイフ親(紫) */
	#endif
	int r_or_l;
	for (r_or_l=0; r_or_l<2; r_or_l++)/*r35-*/
	{
		SPRITE *h;
		h					= obj_add_00_tama_error();/*全方向ナイフ(青)*/
		if (NULL != h)
		{
			#if (1)
			h->cx256			= (src->cx256); 	/* ナイフ(紫)弾は咲夜の位置から発弾。 */
			h->cy256			= (src->cy256); 	/* ナイフ(紫)弾は咲夜の位置から発弾。 */
		//	h->type 			= BULLET_KNIFE20_04_AOI;/* ナイフ親(青) */
			h->type 			= BULLET_KNIFE20_06_YUKARI;/* ナイフ親(紫) */
			reflect_sprite_spec444(h, OBJ_BANK_SIZE_00_TAMA);
			h->callback_mover	= sakuya_ryoute_knife_danmaku_01_callback;
			h->m_Hit256R		= TAMA_ATARI_KNIFE18_PNG;
			#else
			/*(弾幕システムに移行する場合)*/
			#endif
		//
			#if (1)/*(特別処理)*/
			/*(ナイフの左右別フラグを決定)*/
			h->RYOUTE_KNIFE_DATA_l_or_r 		= r_or_l;
			/*(ナイフの進行向きを決定)*/
			{
				int tmp1024;
				tmp1024 = (128);
				if (KNIFE_TYPE_RIGHT!=r_or_l)	{tmp1024 = -tmp1024;	}
				tmp1024 += (HATSUDAN_03_angle1024); 	/* 「1周が1024分割」 */
			//	mask1024(tmp1024);
				#if (1)
				h->rotationCCW1024	= (tmp1024);	/*(ナイフの進行向きは表示向きと同じなので、表示向きで保持する)*/
				#else
				/*(弾幕システムに移行する場合)*/
				HATSUDAN_03_angle65536			= (tmp1024);
				#endif
			}
			/*(ナイフの発弾座標を決定)*/
			#if (0)//
			h->cx256	= (h->cx256) + ((si n1024((h->rotationCCW1024)))<<7);/*fps_factor*/ /* CCWの場合 */
			h->cy256	= (h->cy256) + ((co s1024((h->rotationCCW1024)))<<7);/*fps_factor*/
			#else
			{
				int sin_value_t256; 		//	sin_value_t256 = 0;
				int cos_value_t256; 		//	cos_value_t256 = 0;
				int256_sincos1024( (h->rotationCCW1024), &sin_value_t256, &cos_value_t256);
		//		h->cx256		= (h->cx256) + ((sin_value_t256)<<7);/*fps_factor*/
		//		h->cy256		= (h->cy256) + ((cos_value_t256)<<7);/*fps_factor*/
				h->cx256		= (h->cx256) + ((sin_value_t256)<<3);/*fps_factor*/
				h->cy256		= (h->cy256) + ((cos_value_t256)<<3);/*fps_factor*/
			}
			#endif
			/*(ナイフの寿命を決定)*/
			h->jyumyou						= (128+(REG_0f_GAME_DIFFICULTY<<6));/*(r33)*/	/*意味無い(?)*/
			#endif
		}
		#if (0)
		/*(弾幕システムに移行する場合)*/
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_01)|(r_or_l<<8)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
		hatudan_system_regist_single();
		#endif
	}
}
