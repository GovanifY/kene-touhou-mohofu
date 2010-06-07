
/*---------------------------------------------------------
 東方模倣風  〜 Toho Imitation Style.
  プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	その他の弾幕を生成します。
---------------------------------------------------------*/

/*---------------------------------------------------------
	差分氏の妖怪1(天狗様?)が撃つ弾幕(予定)
	-------------------------------------------------------
	未作成
---------------------------------------------------------*/
static void danmaku_create_11_tengu_shot(SPRITE *src)
{
	static int aaa_angle1024;
	static int data_wait2_start;/* 発弾間隔start */
	static int data_wait2;/* 発弾間隔 */

//	if (128==((src->boss_base_danmaku_time_out)))
	if (0x10==((src->boss_base_danmaku_time_out)&0x10))
	{
		data_wait2_start	= (20-((difficulty)<<2));	/*8*/ /*10*/
		data_wait2			= 10;
		tmp_angleCCW1024_jikinerai(obj_player, src);/*自機ねらい角作成*/
		aaa_angle1024		= src->tmp_angleCCW1024;/*自機ねらい角*/
		aaa_angle1024		-= (int)(1024*(5)/(32));/* 5弾目が自機狙い */
		/* 角度(1024[360/360度]を 32分割) */
	}
	else
//	if (0x00==((src->boss_base_danmaku_time_out)&0x10))/* 16回 */
	{
		data_wait2--;
		if (1 > data_wait2)
		{
			data_wait2 = data_wait2_start;
			#if (1)
		//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE13_SAKUYA_SHOT02/*VOICE14_BOSS_KOUGEKI_01*/);
			#endif
			obj_send1->y256 						= (src->y256);								/* 弾源y256 */	/* 発弾位置の offset 用 */
			{
			int iii;
				iii = (src->boss_base_danmaku_time_out);
			int tama_color;/* 弾色 */
			int bbb;/* 方向 */
				if (0x00==((src->boss_base_danmaku_time_out)&0x20))
						{	bbb=-1;tama_color=0;/* 枠つき青弾 */}
				else	{	bbb= 1;tama_color=1;/* 枠つき緑弾 */}
			//	obj_send1->x256 = src->x256+((16-(iii&0x0f))<<(3+8))*bbb;/* 本物は発弾位置の x offset あり */
				obj_send1->x256 = src->x256+((16-(iii&0x0f))<<(3+7))*bbb;/* 本物は発弾位置の x offset あり */
				br.BULLET_REGIST_speed256			= (t256(1.5));								/* 弾速 */
				br.BULLET_REGIST_angle1024			= ((/*0+*/(aaa_angle1024)*bbb)&(1024-1));	/* 弾源角度1024 */
		//		br.BULLET_REGIST_div_angle1024		= (0);										/* ダミー分割角度(未使用) */
				br.BULLET_REGIST_bullet_obj_type	= BULLET_MARU12_02_AOI+tama_color;			/* [枠つき青弾] [枠つき緑弾] */ 	/*BULLET_MARU12_03_AOI*/
				br.BULLET_REGIST_n_way				= (1);										/* [1way] */
				br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
				bullet_regist_vector();
			}
			/* 次の弾の角度 */
			aaa_angle1024 += (int)(1024/(32));/* 角度(1024[360/360度]を 32分割) */
		}
	}
}
