
/*---------------------------------------------------------
 東方模倣風  ～ Toho Imitation Style.
  プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	アリスの弾幕を生成します。
---------------------------------------------------------*/

/*---------------------------------------------------------
	アリス(仮)	洗濯機弾幕
	-------------------------------------------------------
	テキトー
---------------------------------------------------------*/
static void danmaku_create_09_alice_sentakki(SPRITE *src)
{
	if (0==((src->boss_base_danmaku_time_out)&0x03))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		src->tmp_angleCCW1024 -= (int)(1024/24);
		mask1024(src->tmp_angleCCW1024);
	//
			obj_send1->x256 					= (src->x256);							/* 弾源x256 */
			obj_send1->y256 					= (src->y256);							/* 弾源y256 */
			br.BULLET_REGIST_div_angle1024		= (int)(1024/24);						/* 分割角度 */
			br.BULLET_REGIST_n_way				= (8);									/* [8way] */
			br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
	//
		{
			br.BULLET_REGIST_speed256			= (t256(2.0)+((src->boss_base_danmaku_time_out)<<2));	/* 弾速 */
			br.BULLET_REGIST_angle1024			= (src->tmp_angleCCW1024);								/* 角度 */
			br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_00_AOI;				/* [青鱗弾] */
			bullet_regist_vector();
		//
			br.BULLET_REGIST_speed256			= (t256(2.0)+((src->boss_base_danmaku_time_out)<<2));	/* 弾速 */
			br.BULLET_REGIST_angle1024			= ((-src->tmp_angleCCW1024)&(1024-1));					/* 角度 */
			br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_04_MIZUIRO;			/* [水鱗弾] */
			bullet_regist_vector();
		}
	}
}

/*---------------------------------------------------------
	アリス(仮)	アリス人形弾幕
	-------------------------------------------------------
	テキトー
---------------------------------------------------------*/
extern void add_zako_alice_doll(SPRITE *src);/* アリス人形弾幕 */
static void danmaku_create_0b_alice_doll(SPRITE *src)
{
	if (50==((src->boss_base_danmaku_time_out) ))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		add_zako_alice_doll(src);
	}
}
