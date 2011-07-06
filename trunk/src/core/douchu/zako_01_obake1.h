
/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
		"おばけ1",		"DRAGER",
	-------------------------------------------------------
	突撃おばけ、(かなりうろ覚え、おばけじゃなかったかも？)
	-------------------------------------------------------
仕様：
	■ datでscoreを指定する。
	■ datで体力を指定する。
	■ datで獲得アイテムを指定する。
	■ datで出現位置 x, y 座標を指定する。
	■ 雑魚の速度は難易度に関係なく固定。結構速め。(t256(2.0))
	■ 雑魚は自機狙いで突撃してくる。(途中で向きは変えません)
	■ 等間隔で弾を撃ってくる。(早く倒した方が有利)
		easy	撃たない。
		normal	1way弾。
		hard	3way弾。
		luna	5way弾。
	■ 弾の速度は、雑魚の速度より遅い。速度は難易度に関係なく固定。(t256(1.25))
	(画面に残るので早く倒した方が有利)
	-------------------------------------------------------
	つまりフラワーウイザラウェイの逆バージョンです。
	と言った方が分かりやすいかな？
---------------------------------------------------------*/

/*---------------------------------------------------------
	"隕石1", "紫 編 隊1",		"MING",
	-------------------------------------------------------
仕様：
	■ datでscoreを指定する。
	■ datで体力を指定する。
	■ datで獲得アイテムを指定する。
	■ datで出現位置 x, y 座標を指定する。



	単体でばらばらに、落ちてくるザコ敵
	自機狙い赤単弾を連射してくる
	-------------------------------------------------------
	道中に必要か悩んだが、元と似た仕様の編隊飛行敵は必要ない。
	単純に落ちてくる敵に変更する。
	-------------------------------------------------------
	10機三角錐状に続がり、編隊飛行してくるザコ敵
	ming
	rwingx_curverに似てる
---------------------------------------------------------*/


/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_obake1(SPRITE *src)
{
	/* ザコ発生から1[秒]以上経過していれば、チェック */
	if (( (JYUMYOU_ZAKO_HASSEI-60) > src->jyumyou ))
	{
		gamen_gai_nara_zako_osimai(src);/* 画面外ならおしまい */
	}
	if (0==((src->jyumyou)&0x3f))/* 等間隔(テキト－) */
	{
		if (ENEMY_LAST_SHOT_LINE256 > src->cy256)	/* このラインより下からは敵が撃たない */
		{
			if (0<difficulty)
			{
				obj_send1->cx256 = src->cx256;
				obj_send1->cy256 = src->cy256;
				br.BULLET_REGIST_speed256			= (t256(1.25));
				br.BULLET_REGIST_angle1024			= (src->tmp_angleCCW1024);		/*(ANGLE_JIKI_NERAI_DAN)*/
				br.BULLET_REGIST_div_angle1024		= (int)(1024/(18)); 			/* 分割角度 */
				br.BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_04_AOI;		/* [青ナイフ弾] */
				br.BULLET_REGIST_n_way				= (difficulty+difficulty-1);	/* [1 3 5way] */
				br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
				bullet_regist_vector();
			}
		}
	}
//
	{
		src->cx256 += (src->vx256);/*fps_factor*/
		src->cy256 += (src->vy256);/*fps_factor*/
	}
	/* アニメーション */
	{
		if (0==src->gra_anime_type)
		{
			/* offset無しは回転 */
			src->rotationCCW1024 += (10);/*グラ回転*/
			mask1024(src->rotationCCW1024);
		}
		else
		{
		//	src->type			= TEKI_28_YOUSEI2_5+(((src->cy256>>8)&(4-1)));
			zako_anime_type02(src, TEKI_28_YOUSEI2_5);
		}
	}
}


/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

global void add_zako_obake1(STAGE_DATA *l)
{
	SPRITE *h;
	h							= sprite_add_gu_error();
	if (NULL!=h)/* 登録できた場合のみ */
	{
		add_zako_common(l, h);
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
		h->type 				= TEKI_28_YOUSEI2_5;
		h->callback_mover		= move_obake1;
//		h->jyumyou				= (60);
	//
		tmp_angleCCW1024_jiki_nerai(h);
		h->vx256 = ((sin1024((h->tmp_angleCCW1024))));
		h->vy256 = ((cos1024((h->tmp_angleCCW1024))));
		h->vx256 += h->vx256;
		h->vy256 += h->vy256;
		h->gra_anime_type		= (1);	/* 0==回転アニメ、グラタイプ */
	}
}


/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

global void add_zako_inseki1(STAGE_DATA *l)
{
	SPRITE *h;
	h							= sprite_add_gu_error();
	if (NULL!=h)/* 登録できた場合のみ */
	{
		add_zako_common(l, h);
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
		h->type 				= TEKI_61_NIJI_HOSI;
		h->callback_mover		= move_obake1;
//		h->jyumyou				= (0x3ff);
	//
		h->gra_anime_type		= (0);	/* 0==回転アニメ、グラタイプ */
		{
		//
		u32 rrrr;
			rrrr = (ra_nd());
			h->vx256 = ((rrrr)&0x01ff);/*右方向*/
			/* 画面右側の場合、左方向へ進む */
			if ( t256((GAME_WIDTH)/2) < h->cx256)
			{
				h->vx256 = (-(h->vx256));/*左方向*/
			}
			h->vy256 = ((rrrr>>16)&0x01ff) + t256(1.00);/*下方向*/
		}
	}
}
			/* 初期位置 */
//			h->cx256 = (((u32)l->user_x)<<8);//((rrrr)&0xffff) + t256((GAME_WIDTH-256)/2);
//			h->cy256 = -t256(30);
