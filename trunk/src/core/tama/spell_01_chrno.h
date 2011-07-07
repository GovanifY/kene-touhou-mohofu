
/*---------------------------------------------------------
 東方模倣風 ～ Toho Imitation Style.
  プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	チルノの弾幕を生成します。
---------------------------------------------------------*/

/*---------------------------------------------------------
	紅2面ボス チルノ	通常攻撃1(1/3)	自機狙い拡散弾(1-2-3-4-5-6-7-8弾)
	-------------------------------------------------------
	(1-2-3-4-5-6-7弾)かと思ってたけど、原作やり直したら、
	(1-2-3-4-5-6-7-8弾)だったので修正。
うーん
	原作やり直してよく見たら、
	(1-2-3-4-5-6-7弾)か 			(easyとか)
	(3-4-5-6-7-8弾)。				(hard)
	(3-4-5-6-7-8弾)。				(lunatic)

	チルノはどうも「内部ランク」で違う弾幕撃ってくる。ので良く判らない。


1100 0111 0xc7
0100 0000
--11 0111 0x37


	-------------------------------------------------------
0x38(56) --0
0x30(48) --1
0x28(40) --2 全方位青枠丸弾
0x20(32) --3 全方位青枠丸弾
0x18(24) --4
0x10(16) --5
0x08( 8) --6
0x00( 0) --7
	-------------------------------------------------------
	lunaticとかだと全方位青枠丸弾も同時に撃ってくる。(2回ぐらい)
---------------------------------------------------------*/
static void spell_create_10_cirno_misogi(SPRITE *src)
{
//	if ((64-1)==((src->boss_base_spell_time_out)))
	if ((SPELL_TIME_0060-1)==((src->boss_base_spell_time_out)))
	{
		SPRITE *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
		tmp_angleCCW65536_src_nerai(zzz_player, src);/* 自機狙い角作成 */
	}
//	else
//	if (0x40==((src->boss_base_spell_time_out)&0xc7))/* 8回(修正1-2-3-4-5-6-7弾:0wayは撃たないので撃つのは7回) */
	if (0x00==((src->boss_base_spell_time_out)&0x07))/* 8回(修正1-2-3-4-5-6-7弾:0wayは撃たないので撃つのは7回) */
	{
		#if (1)
	//	voice_play(VOICE13_SAKUYA_SHOT02, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE13_SAKUYA_SHOT02);
		#endif
	//
		int n_way;
		n_way = 0;
		n_way = ((/*16*/7-((src->boss_base_spell_time_out)>>3))&7)+(1)+((cg_game_difficulty)>>1);
		if (n_way)
		{
			obj_send1->cx256							= (src->cx256); 	/* 弾源x256 ボス中心から発弾。 */
			obj_send1->cy256							= (src->cy256); 	/* 弾源y256 ボス中心から発弾。 */
		//
			br.BULLET_REGIST_00_speed256				= (t256(2.0));				/* 弾速(2.5) */
			br.BULLET_REGIST_01_speed_offset			= t256(6);/*(テスト)*/

		//	br.BULLET_REGIST_02_angle65536				= (src->tmp_angleCCW65536)-(n_way*65536/128);
			br.BULLET_REGIST_02_angle65536				= (src->tmp_angleCCW65536);

			br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)標準弾 */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KOME_03_AOI;		/* [青米弾] になってるのは現在 氷弾 のグラが無いから */
		//未定br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* 標準弾に設定する(r33-) */

			br.BULLET_REGIST_06_n_way					= n_way;/* [nway] */
			br.BULLET_REGIST_07_div_angle65536			= (int)(65536/(64));		/* 分割角度(1024[360/360度]を 64 分割) */	/* 1周をn分割した角度 */
			tama_system_regist_n_way();/* (r33-) */
		}
	}
#if 1
	if (0x20==((src->boss_base_spell_time_out)&0x37))/* 2回 */
	{
		if (0 != spell_bunkatsu_nums_table[b_chrno_00_BLUE_NUMS+(cg_game_difficulty)])
		{
			// hard 18方向(1回)
			// Luna 36方向(2回)
			set_sub_jikinerai(src);/* 1:いちいち作成するっぽい。 */
		//

			br.BULLET_REGIST_00_speed256				= (t256(2.0));				/* 弾速(2.5) */
			br.BULLET_REGIST_01_speed_offset			= t256(1);/*(テスト)*/

			br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_8000_NON_TILT);/* (r33-)非傾き弾 */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_WAKU12_03_AOI; 	/* [枠付き青丸弾] */
		//未定br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_01_ANGLE_NON_TILT;/* (r33-)非傾き弾 */

			br.BULLET_REGIST_06_n_way					= spell_bunkatsu_nums_table[b_chrno_00_BLUE_NUMS+(cg_game_difficulty)];//(36);/* [nway] */
			br.BULLET_REGIST_07_div_angle65536			= spell_nan_ido_table[chrno_04_BLUE_DIV_ANGLE+(cg_game_difficulty)];//(int)(65536/(36));		/* 分割角度(1024[360/360度]を 64 分割) */	/* 1周をn分割した角度 */
			tama_system_regist_katayori_n_way();/* (r33-) */
		}
	}
#endif /*0000*/
}


/*---------------------------------------------------------
	紅2面ボス チルノ	氷符「アイシクルフォール」
	-------------------------------------------------------

---------------------------------------------------------*/
static void spell_create_1b_cirno_icecle_fall(SPRITE *src)
{
//	if ((64-1)==((src->boss_base_spell_time_out)))
	if ((SPELL_TIME_0060-1)==((src->boss_base_spell_time_out)))
	{
		SPRITE *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
		tmp_angleCCW65536_src_nerai(zzz_player, src);/* 自機狙い角作成 */
	}
//	else
//	if (0x40==((src->boss_base_spell_time_out)&0xc7))/* 8回(修正1-2-3-4-5-6-7弾:0wayは撃たないので撃つのは7回) */
	if (0x00==((src->boss_base_spell_time_out)&0x07))/* 8回(修正1-2-3-4-5-6-7弾:0wayは撃たないので撃つのは7回) */
	{
		#if (1)
	//	voice_play(VOICE13_SAKUYA_SHOT02, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE13_SAKUYA_SHOT02);
		#endif
	//
			obj_send1->cx256							= (src->cx256); 	/* 弾源x256 ボス中心から発弾。 */
			obj_send1->cy256							= (src->cy256); 	/* 弾源y256 ボス中心から発弾。 */
		//
		int jj;
		for (jj=0; jj<5; jj++ )/* 全部調べる。 */
		{
			br.BULLET_REGIST_00_speed256				= (t256(2.0)-(jj<<5));				/* 弾速(2.5) */
			br.BULLET_REGIST_01_speed_offset			= t256(6);/*(テスト)*/

			br.BULLET_REGIST_02_angle65536				= (0);	/* 下向き */
			br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)標準弾 */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KOME_03_AOI;		/* [青米弾] になってるのは現在 氷弾 のグラが無いから */
		//未定br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* 標準弾に設定する(r33-) */
			br.BULLET_REGIST_06_n_way					= (2);/* [nway] */
			br.BULLET_REGIST_07_div_angle65536			= (int)(65536/(5))+((src->boss_base_spell_time_out)<<7);		/* 分割角度(1024[360/360度]を 64 分割) */	/* 1周をn分割した角度 */
			tama_system_regist_n_way();/* (r33-) */
		}
	}
	if (0x20==((src->boss_base_spell_time_out)&0x37))/* 2回 */
	{
		if (0 < (cg_game_difficulty) )
		{
			set_sub_jikinerai(src);/* 1:いちいち作成するっぽい。 */
			br.BULLET_REGIST_00_speed256				= (t256(2.0));				/* 弾速(2.5) */
			br.BULLET_REGIST_01_speed_offset			= t256(1);/*(テスト)*/

			br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_8000_NON_TILT);/* (r33-)非傾き弾 */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_WAKU12_06_KI_IRO;		/* [枠付き黄丸弾] */
		//未定br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_01_ANGLE_NON_TILT;/* 非傾き弾に設定する(r33-) */
			br.BULLET_REGIST_06_n_way					= (5);/* [nway] */
			br.BULLET_REGIST_07_div_angle65536			= (int)(65536/(36));		/* 分割角度(1024[360/360度]を 64 分割) */	/* 1周をn分割した角度 */
			tama_system_regist_n_way();/* (r33-) */
		}
	}
}


/*---------------------------------------------------------
	紅2面ボス チルノ	通常攻撃1(2/3)	自機狙い全方位弾
	-------------------------------------------------------
---------------------------------------------------------*/
#if 0
#endif


/*---------------------------------------------------------
	紅2面ボス チルノ	通常攻撃1(3/3)	自機狙い収束弾
	-------------------------------------------------------
---------------------------------------------------------*/
#if 0
#endif


/*---------------------------------------------------------
	パーフェクトフリーズの研究
	-------------------------------------------------------
	本当はパーフェクトフリーズに乱数は一つも無いのでは？
	と個人的には考えている。
	-------------------------------------------------------
	とりあえず、自前の乱数生成関数(単に周期の長い関数)が、割と品質の良い乱数を出すので、
	何も小細工しなくても偏らないので特に問題なかった。
	でもどうせなら拘ってみる。
//
	まずチルノは毎フレーム弾をばら撒いているに決まっているので(この前提がおかしい可能性はあまり無いと思われる)。
	チルノがばら撒く弾量から、1秒間に60弾程度、1秒間に120弾程度、1秒間に180弾程度の中では、1秒間に120弾程度が近い。
	従って1フレーム2弾とする。
//
	パット見には判らないが、チルノの乱数弾はぐるぐる回りながら、偏って出てくる。らしいので。
	どれぐらいの速度で回るのなら、回って見えないかを実験してみる。(原作は言われなければ判らない程度)
	(16分割==)約1秒間(64フレーム)に4周(0.25秒で1周)だと、露骨に回っている感じ。なので、これでは明らかにおかしい。
	(8分割==)約1秒間(64フレーム)に8周(0.125秒で1周)だと、言われなければ判らない程度。
	(4分割==)約1秒間(64フレーム)に16周(0.0625秒で1周)だと、分割領域がおおざっぱすぎて、分割する意味がないのではないか？。
	1フレーム2弾と総弾量が決まっている為、周回数を増せばその分、分割領域が広くなり、分割する意味がなくなる。
	(分割数1は分割処理をしないのと同じだから)
	つまり、弾量が1フレーム2弾の場合、8分割に近いものしかありえないという事が判る。
//
	上記の実験から、1フレーム辺りは 8分の1程度の領域に 2弾が確定した。
	ただし、この領域は厳密に 8分の1 ではない。
	なぜなら、この領域が n分の1で割り切れると、回って見えやすい上にゲーム的に安全地帯が出来やすい。
	だから、この領域は厳密に n分の1 ではない。
	65536/8 == 8192。65536/10 == 6553.6。これらに近い値と思われる。
	8分の1(8192) > ? > 10分の1(6554) テキトーに KOTEITI_7777 にしてみる。
	7777の場合 8.42689983284042689983284042689983分の1。(==65536/7777)
//
	上記実験が 8分割なので、2弾目の角度は、16 分割離れた先の、16 分割程度の乱数値(0-4095)が良いと思われる。
	( 8分割 == 16分割(固定値) + 16分割(乱数値) )
	-------------------------------------------------------
	★ 要修正:
	パーフェクトフリーズの後追い弾(青丸)はやり直したら全然違った。
	普通は止まって避けるから判らなかった。(っていうかワザワザ動く人居ないかと)
	なんか動きながら避けると、構造がわかる。
	ランダムとかじゃなくて、(８方向弾を)「三セット」(同時っぽく)撃って来るみたい。
	(自機狙わない)自機狙い偶数弾。
	初回時に自機狙い角a, b, cを作成(?)。
	aとbとcは独立。
---------------------------------------------------------*/

//#define KOTEITI_7777	(7777<<6)
//#define KOTEITI_7777	(497728)
#define KOTEITI_7777	(7777)

/*---------------------------------------------------------
	パーフェクトフリーズ(枠つき12丸弾を全弾減速させる)
	-------------------------------------------------------

---------------------------------------------------------*/

static void exchange_damnaku_chiruno_gensoku(void)
{
//	int check_type;
//	check_type = br.BULLET_REGIST_04_bullet_obj_type;/* 調べるタイプを受け取る */
	int ii;
	for (ii=0; ii<OBJ_POOL_00_TAMA_MAX; ii++ )/* 全部調べる。 */
	{
		SPRITE *s;
		s = &obj99[OBJ_HEAD_00_TAMA+ii];
	//	if (check_type == (s->type) )	/* 矢印の青弾か赤弾なら */
		if (is_tama_grouip08(BULLET_WAKU12_00_SIRO) == is_tama_grouip08(s->type) )	/* 枠つき12丸弾なら */
		{
		//	s_change_meirin_yajirusi_one(s);
		//	s->tama_system_tra65536 					= t256(-6); 		/* (-5.12==98%)調整減速弾 x 1.0 */
		//	s->tama_system_tra65536 					= t256(-12);		/* (-5.12==98%)調整減速弾 x 2.0 */	/* -12==少し戻る */
			s->tama_system_tra65536 					= t256(-9); 		/* (-5.12==98%)調整減速弾 x 1.5 */
		}
	}
}


/*---------------------------------------------------------
	パーフェクトフリーズ(枠つき12丸弾を全弾停止。現在の座標を新基点座標に変更する。向きを変える)
	-------------------------------------------------------

---------------------------------------------------------*/
#if 1
static void exchange_damnaku_chiruno_tomeru(void)
{
//	int check_type;
//	check_type = br.BULLET_REGIST_04_bullet_obj_type;/* 調べるタイプを受け取る */
	int ii;
	for (ii=0; ii<OBJ_POOL_00_TAMA_MAX; ii++ )/* 全部調べる。 */
	{
		SPRITE *s;
		s = &obj99[OBJ_HEAD_00_TAMA+ii];
	//	if (check_type == (s->type) )	/* 矢印の青弾か赤弾なら */
		if (is_tama_grouip08(BULLET_WAKU12_00_SIRO) == is_tama_grouip08(s->type) )	/* 枠つき12丸弾なら */
		{
			s->tama_system_tra65536 					= t256(0);		/* 調整加速弾 */
			s->tama_system_speed65536					= t256(0);		/* 弾速 */
			s->type 						= (BULLET_WAKU12_00_SIRO);
			reflect_sprite_spec444(s, OBJ_BANK_SIZE_00_TAMA);	/* 弾グラと弾あたり判定を変更する。 */
		}
	}
}
#endif


/*---------------------------------------------------------
	パーフェクトフリーズ(枠つき12丸弾を全弾加速させる)
	-------------------------------------------------------
	加速弾(正確には等加速弾)なのに、加速弾に見えないで
	等速弾(正確には等速度弾、加速==0)に見えちゃう問題がある。

---------------------------------------------------------*/
#if 1
static void exchange_damnaku_chiruno_saikasoku(void)
{
//	int check_type;
//	check_type = br.BULLET_REGIST_04_bullet_obj_type;/* 調べるタイプを受け取る */
	int add_angle = 0;
	int jj = 0;
	int ii;
	for (ii=0; ii<OBJ_POOL_00_TAMA_MAX; ii++ )/* 全部調べる。 */
	{
		SPRITE *s;
		s = &obj99[OBJ_HEAD_00_TAMA+ii];
	//	if (check_type == (s->type) )	/* 矢印の青弾か赤弾なら */
		if (is_tama_grouip08(BULLET_WAKU12_00_SIRO) == is_tama_grouip08(s->type) )	/* 枠つき12丸弾なら */
		{
		//	s_change_meirin_yajirusi_one(s);
			/* 弾の現在座標を、新基点座標とする。 */
			s->tx256 = (s->cx256);		/* 基点座標にセット */
			s->ty256 = (s->cy256);		/* 基点座標にセット */
			s->tama_system_radius256 = t256(0); 	/* 基点座標になったので半径は 0 にする。 */
			/*	*/
			#if 0
		//	s->rotationCCW1024				= (ra_nd()&((1024)-1)); 			/* 向き */	/* 向きは乱数でない気がする */
			#else
			/* 少なくとも乱数よりは本物に近い。 が、全然違う。 */
			/* 加算固定値(KOTEITI_7777)以外の要因として、弾幕リストのサイズでループする為、弾幕リストのサイズを */
			/* 現在の 1024 から、本物と同じ 640 に減らせば、似る可能性はある。 */
			add_angle += (KOTEITI_7777);
			s->rotationCCW1024				= (add_angle>>6);			/* 向き */
			#endif
			jj++;
		//	jj &= 0x07; 	// 2 3 4 5	6 7 8 9
			jj &= 0x03; 	// 3 4 5 6
		//	s_change_meirin_yajirusi_one(s);
		//	s->tama_system_tra65536 					= t256(6);		/* 調整加速弾 */
		//	s->tama_system_tra65536 					= (jj<<8)+t256(2);	/* (2 ... 9) 調整加速弾 */
			s->tama_system_tra65536 					= (jj<<8)+t256(3);	/* (3 ... 6) 調整加速弾 */
			/* (1は遅すぎたので除外) 9 だと速過ぎるかも */
		}
	}
}
#endif
/*---------------------------------------------------------
	パーフェクトフリーズ
	1周期が約10[秒]とする。
0x280  0000 0010 1000 0000	>>=6  0x000a
	-------------------------------------------------------
64 x  0 	(640==0x0280) a
	ランダム発射
64 x  1 	(576==0x0240) 9
	ランダム発射
64 x  2 	(512==0x0200) 8
	ランダム発射
64 x  3 	(448==0x01c0) 7 減速開始
	//減速
64 x  4 	(384==0x0180) 6 停止
	//停止	// 自機依存による8方向発射
64 x  5 	(320==0x0140) 5 再加速
	//再加速
64 x  6 	(256==0x0100) 4
64 x  7 	(192==0x00c0) 3
64 x  8 	(128==0x0080) 2
64 x  9 	( 64==0x0040) 1
64 x 10 	(  0==0x0000) 0
---------------------------------------------------------*/
#if 1
static void spell_create_13_perfect_freeze(SPRITE *src)
{
//	int i;
	// 最初のランダム発射 (64*3フレーム==約3秒)ばら撒く。
	if (((640)-(64*3))<(src->boss_base_spell_time_out)) // 192== (64*3)
	{
		#if (1)
		if (0==((src->boss_base_spell_time_out)&0x07))
		{
		//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		}
		#endif
		{
			/*
				原作では弾にどの色を割り当てるかという事には拘りがある。
				チルノは 5種類の色の弾を撃つが、撃つ弾の色の出る確率が均等ではなく偏っている。
				この色の偏りは、プログラムの都合等で偶然偏ったものではなく、作為的な演出である。
				ここではチルノが青(aho)なので、ゲーム的にチルノを目立たせるために青弾を少なくするという演出意図がある。
				(青弾が多いとチルノが何処にいるか判らなくなるというゲーム上の都合がある。)
				この色の偏りが、チルノらしさを演出しているので、ここの再現は実は重要だと思う。
				(ahoとかいう設定は関係なくて)弾の色の偏りが原作と違うと、おそらく「ゲーム的にチルノらしくならない」。
				チルノらしさ: 青 -> あお -> ao -> aho -> アホ -> baka -> baka==⑨(花映塚公式説明書から)
				//
				そこで適当なスクショから弾の数を調べる。
				1(1).赤 27/141個	3/16個 == 3.06382978723404255319148936170213 = 27*16/141	(R+G+Bは白なので青が若干少ないがほぼ同値にする)
				2(3).青 21/141個	2/16個 == 2.38297872340425531914893617021277 = 21*16/141	(R+G+Bは白なので青が若干少ないがほぼ同値にする)
				3(5).緑 26/141個	3/16個 == 2.95035460992907801418439716312057 = 26*16/141	(R+G+Bは白なので青が若干少ないがほぼ同値にする)
				4(6).黄 34/141個	4/16個 == 3.85815602836879432624113475177305 = 34*16/141	黄すなわち(R+G)なので青が少ないという演出。
				5(7).橙 33/141個	4/16個 == 3.74468085106382978723404255319149 = 33*16/141	橙すなわち(R+R+G)なので青が少ないという演出。
				従って、下記の確率テーブルがある。
			*/
			const u8 p_tama_tbl[(16)] =
			{
				/* 確率を等しくする。 */
				/* 色が散らばるような配置を考える。 */
				 1, 3, 5, 6, 7,
				 1, 3, 5, 6, 7,
				 1, 6, 5, 6, 7, 7,
			};
			static int add_angle;
			add_angle += (KOTEITI_7777);	/* 固定値。8分の1 から 10分の1 程度の値で、n分の1で割り切れない値。 */
			const u16 ra_nd16 = (ra_nd()/*&0xffff*/);
		//	br.BULLET_REGIST_00_speed256				= (t256(1.9)+(ra_nd16>>7)); 		/* 弾速 */	/* (ra_nd16>>7) == 乱数(0-511) */
			br.BULLET_REGIST_00_speed256				= (t256(1.5)+(ra_nd16>>7)); 		/* 弾速 */	/* (ra_nd16>>7) == 乱数(0-511) */
		//	br.BULLET_REGIST_01_speed_offset			= t256(-5); 		/* (-5) (-3)調整減速弾 */	/* この方式になるか検討中 */
			br.BULLET_REGIST_01_speed_offset			= t256(0);/*(テスト)*/
			/*
				ここの弾速調整は、単純に速い方が難しかったり、遅い方が難しかったりという事は無い。
				あんまり速くし過ぎる(倍速ぐらい？)と、ただのエターナルミークになっちゃうのは判ると思うけど。
				//
				速い場合は、始めの弾を避けるのはその分難しくなるが、後半は弾数が減るので酷く簡単になる。
				遅い場合は、始めの弾を避けるのはその分簡単になるが、後半は弾数が増えるのでその分難しくなる。
			*/
		//	br.BULLET_REGIST_02_angle65536				= (0/65536);					/* 下向き */
		//	br.BULLET_REGIST_02_angle65536				= (ra_nd()&((32768)-1)) + ((src->boss_base_spell_time_out)<<(8+6)); 		/* 向き */
		//	br.BULLET_REGIST_02_angle65536				= (ra_nd()&((32768/32)-1)) + ((src->boss_base_spell_time_out)<<(8+4));		/* 向き */
		//	br.BULLET_REGIST_02_angle65536				= (ra_nd()&((65536)-1)) ;		/* 向き */
			br.BULLET_REGIST_02_angle65536				= (add_angle)/*8分割(0.125秒で1周)*/;		/* 向き */
			br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_8000_NON_TILT);/* (r33-)非傾き弾 */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_WAKU12_00_SIRO+(p_tama_tbl[((src->boss_base_spell_time_out)&0x0f)]);
		//未定br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_01_ANGLE_NON_TILT;		/* 非傾き弾 */
			#if 0
			br.BULLET_REGIST_06_n_way					= (2);										/* [2way] */	/* 1フレームに2弾 */
			#else
			br.BULLET_REGIST_06_n_way					= (1+(cg_game_difficulty)); /* 模倣風なので、難易度調整 */		/* [2way] */	/* 1フレームに2弾 */
			#endif
			br.BULLET_REGIST_07_div_angle65536			= (65536/16)+(ra_nd16>>4);				/* 分割角度([360/360]度を16分割) */ 	/* (ra_nd16>>4) == 乱数(0-4095) */
			{
				obj_send1->cx256					= (src->cx256); 	/* 弾源x256 ボス中心から発弾。 */
				obj_send1->cy256					= (src->cy256); 	/* 弾源y256 ボス中心から発弾。 */
				tama_system_regist_katayori_n_way();	/* (r33-) */
			}
		}
	}
	else
	if (((640)-(64*4))<(src->boss_base_spell_time_out)) // 192== (64*3)
	{
		;/*何もしない*/
	}
	else
	if (((640)-(64*5))<(src->boss_base_spell_time_out)) // 192== (64*3)
	{
		/*
		Normalで適当にやってみた。
		後追い弾は、速度の違う弾(三種)を同時に出す。のセットを6回だった。
		*/
		if (0x00==((src->boss_base_spell_time_out)&0x07)) /* 8[frame]に1回 */
		{
			if ((4+(cg_game_difficulty))>((src->boss_base_spell_time_out)&0x07))
			{
				set_sub_jikinerai(src);/* 1:いちいち作成するっぽい。 */
				int aaa_angle65536;
				aaa_angle65536 = (src->tmp_angleCCW65536);/* 自機狙い角 */
				int jj;
				for (jj=0; jj<3; jj++)
				{
				//	br.BULLET_REGIST_00_speed256				= (t256(2.5));				/* 弾速 */
				//	br.BULLET_REGIST_00_speed256				= (t256(1.75)+(jj<<7)); 	/* 弾速 */
					br.BULLET_REGIST_00_speed256				= (t256(2.00)+(jj<<8)); 	/* 弾速 */
					br.BULLET_REGIST_01_speed_offset			= t256(0);/*(テスト)*/
					br.BULLET_REGIST_02_angle65536				= (aaa_angle65536)+(jj<<8); 		/* 自機狙い角 */
					br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_8000_NON_TILT);/* (r33-)非傾き弾 */
					br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_MARU10_03_AOI;
				//未定br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_01_ANGLE_NON_TILT;		/* 非傾き弾 */
					br.BULLET_REGIST_06_n_way					= (8);								/* [8way] */
					br.BULLET_REGIST_07_div_angle65536			= (65536/16);						/* 分割角度([360/360]度を16分割) */
					{
						tama_system_regist_n_way(); 		/* (r33-) */
					}
				}
			}
		}
	}
//
	#if 0
	if (((640)-(64*3))==(src->boss_base_spell_time_out))	// 192== (64*3)
	{
		exchange_damnaku_chiruno_gensoku();/*減速開始*/
	}
	else
//	if (((640)-(64*3)-48)==(src->boss_base_spell_time_out)) // 192== (64*3-48)
	if (((640)-(64*4))==(src->boss_base_spell_time_out))		// 256== (64*4)
	{
//		// 48 フレーム経過後、全てストップさせ、白くし、カウントリセット
		exchange_damnaku_chiruno_tomeru();/*全てストップ*/
	}
	else
	if (((640)-(64*5))==(src->boss_base_spell_time_out))		// 320== (64*5)
	{
		exchange_damnaku_chiruno_saikasoku();/*再加速*/
	}
	#else
	if ((7<<6)==(src->boss_base_spell_time_out))	{	exchange_damnaku_chiruno_gensoku();/*減速開始*/ 	}
	else
	if ((6<<6)==(src->boss_base_spell_time_out))	{	exchange_damnaku_chiruno_tomeru();/*全てストップ*/	}
	else
	if ((5<<6)==(src->boss_base_spell_time_out))	{	exchange_damnaku_chiruno_saikasoku();/*再加速*/ 	}
	#endif

}
#endif


/*---------------------------------------------------------
	雪符「ダイアモンドブリザード」
	-------------------------------------------------------
	チルノの周りで氷弾をばら撒く
	微妙に減速弾っぽい。
		1.チルノの周り離れた位置から発弾。(ランダム?)
		2.ある程度まで広がる。
		3.微減速弾。
---------------------------------------------------------*/

static void spell_create_16_diamond_blizzard(SPRITE *src)
{
	if (0==((src->boss_base_spell_time_out)&0x1f))
	{
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE16_BOSS_KYUPIN);
	//	voice_play(VOICE16_BOSS_KYUPIN, TRACK04_TEKIDAN);
	}
//	if (0==((src->boss_base_spell_time_out)&0x07))
	{
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
	//	bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		{
			const u32 ra_nd32 = (ra_nd()/*&0xffff*/);
			obj_send1->cx256							= (src->cx256) -t256(64)+(ra_nd32&0x7f00);				/* 弾源x256 */
			obj_send1->cy256							= (src->cy256) -t256(32)+((ra_nd32>>8)&0x3f00); 		/* 弾源y256 */
		//
		//	br.BULLET_REGIST_00_speed256				= (t256(1.0));				/* 弾速(2.5) */
			br.BULLET_REGIST_00_speed256				= (t256(0.0));				/* 弾速(2.5) */
		//	br.BULLET_REGIST_01_speed_offset			= t256(6);/* (6)...つええ てすと*/
			br.BULLET_REGIST_01_speed_offset			= t256(2);/* てすと*/
			br.BULLET_REGIST_02_angle65536				= (ra_nd32);
			br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)標準弾 */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KOME_03_AOI;		/* [青米弾] になってるのは現在 氷弾 のグラが無いから */
		//未定br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)標準弾 */
		//	br.BULLET_REGIST_06_n_way					= (1);/* [nway] */
			br.BULLET_REGIST_06_n_way					= (3);/* [nway] */
		//	br.BULLET_REGIST_07_div_angle65536			= (int)(65536/(64));		/* 分割角度(1024[360/360度]を 64 分割) */	/* 1周をn分割した角度 */
			br.BULLET_REGIST_07_div_angle65536			= (ra_nd32);				/* 分割角度(1024[360/360度]を 64 分割) */	/* 1周をn分割した角度 */
			tama_system_regist_katayori_n_way();/* (r33-) */
		}
	}
}

/*
	最強「エターナルフォースブリザード」
	(Eternal Force Blizzard, EFB)

	エターナルフォースブリザード
		一瞬で相手の周囲の大気ごと氷結させる
		相手は死ぬ

	2ちゃんねるのニュース速報板のスレッド(通称VIP)で考え出された魔法(必殺技？)
	あまりのクォリティの高さに様々なAAが作られる。
	また、多くの閲覧者に自分も過去に同じような事を考えていたと言うトラウマを呼び覚まさせた。

*/


