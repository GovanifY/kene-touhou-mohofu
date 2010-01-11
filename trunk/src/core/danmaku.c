
#include "bullet_object.h"

/*---------------------------------------------------------
	’e–‹¶¬
	-------------------------------------------------------
	š’e–‹‚ÍƒVƒXƒeƒ€‚ÅƒTƒ|[ƒg‚·‚é\‘z‚ª‚ ‚è‚Ü‚·B(\‘z’†)
	ƒVƒXƒeƒ€‚ÅƒTƒ|[ƒg‚³‚ê‚é‚ÆA‚Ç‚Ìƒ{ƒX‚ªA‚Ç‚Ì’e–‹‚Å‚àŒ‚‚Ä‚é‚æ‚¤‚É‚È‚è‚Ü‚·B
---------------------------------------------------------*/

/* BOSS_BASE */

/*---------------------------------------------------------
	©‹@‘_‚¢’e‚ÌŠp“x‚ğŒvZ
---------------------------------------------------------*/
/*static*/ void tmp_angle_jikinerai512(SPRITE *p, SPRITE *t)
{
//	BOSS99_DATA *data = (BOSS99_DATA *)t->data;
//
	#if 1
		/* Gu‰»Š®—¹‚·‚é‚Ü‚ÅA‚Ç‚¤‚¹ƒYƒŒ‚é‚Ì‚ÅŠÈ—ª”Å */
	//	data->boss_base.tmp_angleCCW512 =  (atan_512(p->y256-t->y256		  ,p->x256-t->x256				  ));						/* ŠÈ—ª”Å(’e‚Æ©•ª‚ª‘å‚«‚³‚ª“¯‚¶‚È‚ç‚¸‚ê‚È‚¢Aˆá‚¤‚Æ‚»‚Ì•ªŒë·‚É‚È‚é) */
		t->tmp_angleCCW512 =  (atan_512(p->y256-t->y256+(16*256)			  ,p->x256-t->x256+(16*256) 	  ));						/* ƒYƒ‹”Å(ƒeƒLƒg[AƒYƒŒ‚é) */
	#else
		/* Gu‰»Š®—¹‚µ‚½‚ç‚±‚Á‚¿ */
		t->tmp_angleCCW512 =  (atan_512(p->y256-t->y256+((p->h128-t->h128)), p->x256-t->x256+((p->w128-t->w128))));/**/ 				/* ‹ô”’e‚Ìê‡‚É©‹@‘_‚¢ */
	#endif
}
	#if 0
	//	t->tmp_angleCCW512 =  (atan_512(p->y256-t->y256 					  ,p->x256-t->x256				  ));//2082417				/* ŠÈ—ª”Å(’e‚Æ©•ª‚ª‘å‚«‚³‚ª“¯‚¶‚È‚ç‚¸‚ê‚È‚¢Aˆá‚¤‚Æ‚»‚Ì•ªŒë·‚É‚È‚é) */
	//	t->tmp_angleCCW512 =  (atan_512(p->y256-t->y256+((p->h128-t->h128	)),p->x256-t->x256-((p->w128+t->w128))));/*???(original)*/	/* Šï”’e‚Ìê‡‚É©‹@‘_‚¢(?) ‚È‚ñ‚©¶‚É‚¸‚ê‚é */
/// 	t->tmp_angleCCW512 =  (atan_512(p->y256-t->y256+((p->h128			)),p->x256-t->x256-((p->w128	 ))));
//		t->tmp_angleCCW512 =  (atan_512(p->y256-t->y256+((p->h128-t->h128	)),p->x256-t->x256-((p->w128-t->w128))));/**/				/* Šï”’e‚Ìê‡‚É©‹@‘_‚¢ */
		t->tmp_angleCCW512 =  (atan_512(p->y256-t->y256+((p->h128-t->h128	)),p->x256-t->x256+((p->w128-t->w128))));/**/				/* ‹ô”’e‚Ìê‡‚É©‹@‘_‚¢ */
	//	t->tmp_angleCCW512 =  (atan_512(p->y256-t->y256+((p->h128			)),p->x256-t->x256-((p->w128	 ))));
	//	t->tmp_angleCCW512 =  (atan_512(p->y256-t->y256-((p->h128			)),p->x256-t->x256-((p->w128	 ))));
	#endif

/*---------------------------------------------------------
	g5–Ê’†ƒ{ƒX ç–é ‘æˆêUŒ‚‚É‚¿‚å‚Á‚Æ‚¾‚¯—‚½’e–‹
	-------------------------------------------------------
	–{‰Æ‚ÍŒÅ’è’e–‹(?)‚¾‚¯‚ÇA‚±‚ê‚Í‘_‚¢ŒnB
	(‰¼‚ÉŒÅ’è’e–‹‚¾‚Á‚½‚Æ‚µ‚Ä‚à)‚»‚à‚»‚àã‚És‚¯‚È‚¢‚©‚çAã”ğ‚¯‚Ío—ˆ‚È‚¢B
	(psp‚Ìc‰ğ‘œ“x‚Å‚Íã‚És‚­ƒXƒy[ƒX‚ª‚È‚¢)
	‰º”ğ‚¯‚àpsp‚Ìc‰ğ‘œ“x‚ª‘S‘R‘«‚è‚È‚¢‚©‚çA–{‰Æ•—‚Ìƒ`ƒ‡ƒ“”ğ‚¯‚Í–³—B
	‚ ‚é’ö“x¶‰E‚É“®‚«‰ñ‚Á‚Ä”ğ‚¯‚Ä–á‚¤–‚ğ‘z’è‚µ‚Ä‚¢‚éB(‚¾‚©‚ç’e‘¬’x‚¢)
	‚»‚ê‚É•¹‚¹‚Ä–§’…’e‚Í‚ ‚¦‚Ä‚º‚ñ‚º‚ñˆá‚¤B(•Ê•Ï”‚Æ‚é‚Ì‚ª–Ê“|‚¾‚Á‚½–‚à‚ ‚é)
---------------------------------------------------------*/
static void danmaku_create_01_sakuya_misogi(SPRITE *src)	/*, int nextstate*/ /*, int an im_frame*/
{
	static int aaa_angle512;
	BOSS99_DATA *data = (BOSS99_DATA *)src->data;
//
	if (0==((data->boss_base.danmaku_time_out)&0x07))
	{
		#if (0==USE_DESIGN_TRACK)
		play_voice_auto_track(VOICE13_SAKUYA_SHOT02);
		#else
	//	voice_play(VOICE13_SAKUYA_SHOT02, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE13_SAKUYA_SHOT02);
		#endif

/*CW*/
//		data->boss_base.tmp_angleCCW512 += (int)(128/5)+/*2*/(5)/*(difficulty<<2)*/; // 	/*3*/2+(difficulty);
/*CCW*/
		src->tmp_angleCCW512 -= (int)(128/5)+/*2*/(5)/*(difficulty<<2)*/; //	/*3*/2+(difficulty);
		mask512(src->tmp_angleCCW512);
	//	bullet_create_aka_lines(s);
	//	bullet_create_aka_maru_jikinerai(src, t256(2.5));
	//	bullet_create_aka_maru_jikinerai(src, t256(2.0));
	//	bullet_create_aka_maru_jikinerai(src, t256(3.5));
		send1_obj->x256 = src->x256;
		send1_obj->y256 = src->y256;
		#if 1
		/* ‚ ‚Æ‚Å—v‚é */
//		send1_obj->h128 = src->h128;
//		send1_obj->w128 = src->w128;
		#endif
	//	bullet_create_n_way_dan_sa_type(src,
		send1_obj->BULLET_REGIST_speed256			=	(t256(2.5));						/* ’e‘¬ */
		send1_obj->BULLET_REGIST_angle512			=	(src->tmp_angleCCW512); 			/* [—†ùó’e] */
		send1_obj->BULLET_REGIST_div_angle512		=	(int)(128/5);
		send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_KNIFE20_04_AOI;				/* [ÂƒiƒCƒt’e] */
		send1_obj->BULLET_REGIST_n_way				=	(8);								/* [8way] */
		bullet_regist_basic();

		if (0==((data->boss_base.danmaku_time_out)&0x1f))/* reset (1/32) */
		{
			aaa_angle512=(src->tmp_angleCCW512)-(int)((512/2));
		}
	}
	else
//	if ((10-1)>((((u8)data->boss_base.danmaku_time_out)^0x20)&0x2f))/* x 10way */
	if ((10-1)>(((data->boss_base.danmaku_time_out)    )&0x0f))/* x 10way */
	{
	//	if (/*(64)*/(16)/*(64-(difficulty<<4))*/ < data->danmaku_time_out)
		{
			aaa_angle512 -= (int)(128/5);
			mask512(aaa_angle512);
			send1_obj->x256 = src->x256;
			send1_obj->y256 = src->y256;
			#if 1
			/* ‚ ‚Æ‚Å—v‚é */
//			send1_obj->h128 = src->h128;
//			send1_obj->w128 = src->w128;
			#endif
		//	bullet_create_n_way_dan_sa_type(src,
			send1_obj->BULLET_REGIST_speed256			=	(t256(2.0));					/* ’e‘¬ */
			send1_obj->BULLET_REGIST_angle512			=	(aaa_angle512);
			send1_obj->BULLET_REGIST_div_angle512		=	(int)(difficulty+1/*3*/);		/* [–§’…’e]‚ÌŠp“x */
			send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_KUNAI12_01_AKA;			/* [ÔƒNƒiƒC’e] */
			send1_obj->BULLET_REGIST_n_way				=	(4);							/* [4way] */
			bullet_regist_basic();
		}
	}
}

/*---------------------------------------------------------
	g5–Ê’†ƒ{ƒX ç–éuŠïpFƒ~ƒXƒfƒBƒŒƒNƒVƒ‡ƒ“(1/2)v‚É‚¿‚å‚Á‚Æ‚¾‚¯—‚½’e–‹
	-------------------------------------------------------
	©‹@‘_‚¢A‘S•ûŒü24•ªŠ„’e
	-------------------------------------------------------
	[’FŒ»İbullet_create_n_way_dan_sa_type()‚Ì©‹@‘_‚¢‚ÍƒƒUƒg‘_‚í‚È‚¢Šï”’e‚È‚Ì‚ÅA“®‚©‚È‚¯‚ê‚Î“–‚½‚è‚Ü‚¹‚ñ‚ªA
	‚»‚Ì‚¤‚¿‚±‚±‚Í‹ô”’e‚É‚·‚é—\’è‚È‚Ì‚ÅA“®‚©‚È‚¯‚ê‚Î“–‚½‚é‚æ‚¤‚É‚È‚é—\’è‚Å‚·]
---------------------------------------------------------*/
static void danmaku_create_02_24nerai(SPRITE *src)
{
	BOSS99_DATA *data = (BOSS99_DATA *)src->data;
	if (0==((data->boss_base.danmaku_time_out)&0x03))
	{
		#if (0==USE_DESIGN_TRACK)
		play_voice_auto_track(VOICE14_BOSS_KOUGEKI_01);
		#else
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
	int aaa_angle512;
			tmp_angle_jikinerai512(player, src);/*©‹@‘_‚¢Špì¬*/
			aaa_angle512=(src->tmp_angleCCW512);
		{
			send1_obj->x256 = src->x256;
			send1_obj->y256 = src->y256;
			#if 1
			/* ‚ ‚Æ‚Å—v‚é */
//			send1_obj->h128 = src->h128;
//			send1_obj->w128 = src->w128;
			#endif
		//	bullet_create_n_way_dan_sa_type(src,
			send1_obj->BULLET_REGIST_speed256			=	(t256(2.0)+((data->boss_base.danmaku_time_out)<<2));	/* ’e‘¬ */
			send1_obj->BULLET_REGIST_angle512			=	(aaa_angle512); 				/* ©‹@‘_‚¢’e */
			send1_obj->BULLET_REGIST_div_angle512		=	(int)(512/24);					/* Šp“x */
			send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_KUNAI12_01_AKA;			/* [ÔƒNƒiƒC’e] */
			send1_obj->BULLET_REGIST_n_way				=	(24);							/* [24way] */
			bullet_regist_basic();
		}
	}
}

/*---------------------------------------------------------
	g5–Ê’†ƒ{ƒX ç–é uŠïpFƒ~ƒXƒfƒBƒŒƒNƒVƒ‡ƒ“(2/2)v‚É‚¿‚å‚Á‚Æ‚¾‚¯—‚½’e–‹
	-------------------------------------------------------
	©‹@‘_‚¢A11’e(’†S‚ÆA¶‰E‚É5’e‚¸‚Â‘_‚¢)
	-------------------------------------------------------
	[’FŒ»İbullet_create_n_way_dan_sa_type()‚Ì©‹@‘_‚¢‚ÍƒƒUƒg‘_‚í‚È‚¢Šï”’e‚È‚Ì‚ÅA“®‚©‚È‚¯‚ê‚Î“–‚½‚è‚Ü‚¹‚ñ‚ªA
	‚»‚Ì‚¤‚¿‚±‚±‚Í‹ô”’e‚É‚·‚é—\’è‚È‚Ì‚ÅA“®‚©‚È‚¯‚ê‚Î“–‚½‚é‚æ‚¤‚É‚È‚é—\’è‚Å‚·]
---------------------------------------------------------*/
static void danmaku_create_03_11nife(SPRITE *src)
{
	BOSS99_DATA *data = (BOSS99_DATA *)src->data;
	if (0==((data->boss_base.danmaku_time_out)&0x07))
	{
		#if (0==USE_DESIGN_TRACK)
		play_voice_auto_track(VOICE14_BOSS_KOUGEKI_01);
		#else
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
	int aaa_angle512;
			tmp_angle_jikinerai512(player, src);/*©‹@‚Ë‚ç‚¢Špì¬*/
			aaa_angle512=(src->tmp_angleCCW512);
		{
			send1_obj->x256 = src->x256;
			send1_obj->y256 = src->y256;
			#if 1
			/* ‚ ‚Æ‚Å—v‚é */
//			send1_obj->h128 = src->h128;
//			send1_obj->w128 = src->w128;
			#endif
	//		bullet_create_n_way_dan_sa_type(src,
			send1_obj->BULLET_REGIST_speed256			=	(t256(2.0));				/* ’e‘¬ */
			send1_obj->BULLET_REGIST_angle512			=	(aaa_angle512);
			send1_obj->BULLET_REGIST_div_angle512		=	(int)(512/(7*4));			/* Šp“x([90/360]“x‚ğ7•ªŠ„) */
			send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_KNIFE20_04_AOI;		/* [ÂƒiƒCƒt’e] */
			send1_obj->BULLET_REGIST_n_way				=	(11);						/* [11way] */
			bullet_regist_basic();
		}
	}
}

/*---------------------------------------------------------
	g5–Êƒ{ƒX ç–é u’ÊíUŒ‚1(1/2)v‚É‚¿‚å‚Á‚Æ‚¾‚¯—‚½’e–‹(—\’è)
	-------------------------------------------------------
	24way =(‰E12way)+(¶12way)
	-------------------------------------------------------

---------------------------------------------------------*/
static void danmaku_create_04_pink_hearts(SPRITE *src)
{
	BOSS99_DATA *data = (BOSS99_DATA *)src->data;
	if (0==((data->boss_base.danmaku_time_out)&0x0f))/* –{•¨‚Í 8 ’e(128==0x80==danmaku_time_out) */
	{
		#if (0==USE_DESIGN_TRACK)
		play_voice_auto_track(VOICE14_BOSS_KOUGEKI_01);
		#else
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		send1_obj->w128 = src->w128;
		send1_obj->h128 = src->h128;
		send1_obj->y256 = src->y256;	/* –{•¨‚Í”­’eˆÊ’u‚Ì offset —p */
//		send1_obj->tmp_angleCCW512 = (0);/* ‰ºŒü‚« */
	//
		int i;
		for (i=0; i<(13*12)/*144*/; i+= 13 )/*12•ûŒü*/		/* Šp“x(128[(90+a)/360“x]‚ğ 10 •ªŠ„) */
		{
			send1_obj->x256 = src->x256+t256(4.0);/* –{•¨‚Í”­’eˆÊ’u‚Ì x offset ‚ ‚è */
	//		bullet_create_n_way_dan_sa_type(send1_obj/*src*/,
			send1_obj->BULLET_REGIST_speed256			=	((160-i)<<(1+1+1)); 				/* ’e‘¬ */
			send1_obj->BULLET_REGIST_angle512			=	(i/*(0+i)&(512-1)*/);
	//		send1_obj->BULLET_REGIST_div_angle512		=	(0);								/* ƒ_ƒ~[Šp“x(–¢g—p) */
			send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_KNIFE20_06_YUKARI;			/* [ƒsƒ“ƒNƒiƒCƒt’e] */
			send1_obj->BULLET_REGIST_n_way				=	(1);								/* [1way] */
			bullet_regist_basic();
//
			send1_obj->x256 = src->x256-t256(4.0);	/* –{•¨‚Í”­’eˆÊ’u‚Ì x offset ‚ ‚è */
	//		bullet_create_n_way_dan_sa_type(send1_obj/*src*/,
			send1_obj->BULLET_REGIST_speed256			=	((160-i)<<(1+1+1)); 				/* ’e‘¬ */
			send1_obj->BULLET_REGIST_angle512			=	((0-i)&(512-1));
	//		send1_obj->BULLET_REGIST_div_angle512		=	(0);								/* ƒ_ƒ~[Šp“x(–¢g—p) */
			send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_KNIFE20_06_YUKARI;			/* [ƒsƒ“ƒNƒiƒCƒt’e] */
			send1_obj->BULLET_REGIST_n_way				=	(1);								/* [1way] */
			bullet_regist_basic();
		}
	}
}

/*---------------------------------------------------------
	g5–Êƒ{ƒX ç–é u’ÊíUŒ‚1(2/2)v‚É‚¿‚å‚Á‚Æ‚¾‚¯—‚½’e–‹(—\’è)
	-------------------------------------------------------
	-------------------------------------------------------

---------------------------------------------------------*/
static void danmaku_create_05_32way_dual(SPRITE *src)
{
	BOSS99_DATA *data = (BOSS99_DATA *)src->data;
	if (0==((data->boss_base.danmaku_time_out)&0x3f))
	{
		#if (0==USE_DESIGN_TRACK)
		play_voice_auto_track(VOICE14_BOSS_KOUGEKI_01);
		#else
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
			send1_obj->x256 = src->x256;		/* ’eŒ¹x256 */
			send1_obj->y256 = src->y256;		/* ’eŒ¹y256 */
			#if 1
			/* ‚ ‚Æ‚Å—v‚é */
//			send1_obj->h128 = src->h128;		/* ’e‚ */
//			send1_obj->w128 = src->w128;		/* ’e• */
			#endif
//
		int i;
		for (i=0; i<(512); i+= 16 )/*32•ûŒü*/	/* Šp“x(512[360/360“x]‚ğ 32 •ªŠ„) */
		{
		//	bullet_create_n_way_dan_sa_type(src,
			send1_obj->BULLET_REGIST_speed256			=	(t256(2.0)-((i&16)<<4));	/*Šï”‚Í‚‘¬’e*/
			send1_obj->BULLET_REGIST_angle512			=	(i/*(0+i)&(512-1)*/);		/* ’eŒ¹Šp“x512 */
	//		send1_obj->BULLET_REGIST_div_angle512		=	(0);						/* ƒ_ƒ~[Šp“x(–¢g—p) */
			send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_KNIFE20_04_AOI;		/* [ÂƒiƒCƒt’e] */
			send1_obj->BULLET_REGIST_n_way				=	(1);						/* [1way] */
			bullet_regist_basic();
		}
	}
}

/*---------------------------------------------------------
	Œ¶İuƒNƒƒbƒNƒR[ƒvƒX(1/2)v
---------------------------------------------------------*/
#if 0
#endif

/*---------------------------------------------------------
	Œ¶İuƒNƒƒbƒNƒR[ƒvƒX(2/2)v
---------------------------------------------------------*/
#if 0
#endif

/*---------------------------------------------------------
	Œ¶Ûuƒ‹ƒiƒNƒƒbƒN(1/2)v
	-------------------------------------------------------
	32way ŒÅ’è˜A’e	x 4‰ñ
---------------------------------------------------------*/
static void danmaku_create_06_luna_clock_32way(SPRITE *src)
{
	BOSS99_DATA *data = (BOSS99_DATA *)src->data;
	if (0x40==((data->boss_base.danmaku_time_out)&0xcf))/* 4‰ñ */
	{
		#if (0==USE_DESIGN_TRACK)
		play_voice_auto_track(VOICE14_BOSS_KOUGEKI_01);
		#else
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
	//	bullet_create_n_way_dan_sa_type(src,
		send1_obj->BULLET_REGIST_speed256			=	(t256(2.0));			/* ’e‘¬ */
		send1_obj->BULLET_REGIST_angle512			=	(0);					/* ”­Ë’†SŠp“x / “Áê‹@”\(©‹@‘_‚¢/‘¼) */
		send1_obj->BULLET_REGIST_div_angle512		=	(int)(512/(32));		/* Šp“x(512[360/360“x]‚ğ 32 •ªŠ„) */	/* 1ü‚ğn•ªŠ„‚µ‚½Šp“x */
		send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_CAP16_05_SIROI;	/* [Â”’•Ä’e] */	/* ’eƒOƒ‰ */
		send1_obj->BULLET_REGIST_n_way				=	(32);					/* [32way] */		/* ”­’e” */
		bullet_regist_basic();
	}
}

/*---------------------------------------------------------
	Œ¶Ûuƒ‹ƒiƒNƒƒbƒN(2/2)v
---------------------------------------------------------*/
#if 0
#endif

/*---------------------------------------------------------
	u’ÊíUŒ‚3(1/2)v
	-------------------------------------------------------
	ŠgUŒ^‘S•ûˆÊƒiƒCƒt’e
	80•ªŠ„’e == (8way[hi to 0]+8way[slow to 0]) x 5way(5•ªŠ„)
	-------------------------------------------------------
	–{•¨‚Í‰æ–Êã‚Æ‰æ–Ê¶‚Æ‰æ–Ê‰E‚Ì’[‚ÅƒiƒCƒt‚ªˆê“x‚¾‚¯”½Ë‚·‚éB
---------------------------------------------------------*/
static void danmaku_create_07_80way_dual_five(SPRITE *src)
{
	BOSS99_DATA *data = (BOSS99_DATA *)src->data;
	if (0==((data->boss_base.danmaku_time_out)&0x3f))
	{
		#if (0==USE_DESIGN_TRACK)
		play_voice_auto_track(VOICE14_BOSS_KOUGEKI_01);
		#else
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
	//
			send1_obj->x256 = src->x256;		/* ’eŒ¹x256 */
			send1_obj->y256 = src->y256;		/* ’eŒ¹y256 */
			#if 1
			/* ‚ ‚Æ‚Å—v‚é */
//			send1_obj->h128 = src->h128;		/* ’e‚ */
//			send1_obj->w128 = src->w128;		/* ’e• */
			#endif
//
		int ii; 	/* 1ü‚ğ65536•ªŠ„‚µ‚½Šp“x */
		int j;		/* ƒJƒEƒ“ƒ^ */
		j=0;
		for (ii=0; ii<(0x10000); ii += 819 )/* 819.2(65536/80)	6.4==(512/80) 80•ªŠ„ [5•ªŠ„] */ 	/* 102.4==(512/5) Šp“x(512[72/360“x]‚ğ 5 •ªŠ„) */
		{
			j++;
			j &= (16-1);/* 16way(8way[hi to 0]+8way[slow to 0]) */
		//	bullet_create_n_way_dan_sa_type(src,
			send1_obj->BULLET_REGIST_speed256			=	(t256(3.0)-((j)<<5)-((j&1)<<7));	/*Šï”‚Í‚‘¬’e*/
			send1_obj->BULLET_REGIST_angle512			=	(((0-ii)>>7)&(512-1));				/* CW */
	//		send1_obj->BULLET_REGIST_div_angle512		=	(0);								/* ƒ_ƒ~[Šp“x(–¢g—p) */
			send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_KNIFE20_04_AOI;				/* [ÂƒiƒCƒt’e] */
			send1_obj->BULLET_REGIST_n_way				=	(1);								/* [1way] */
			bullet_regist_basic();
		}
	}
}

/*---------------------------------------------------------
	ƒƒCƒh”é‹Zu‘€‚èƒh[ƒ‹v
---------------------------------------------------------*/
#if 0
#endif

/*---------------------------------------------------------
	ƒAƒŠƒX(‰¼)	ô‘ó‹@’e–‹
	-------------------------------------------------------
	ƒeƒLƒg[
---------------------------------------------------------*/
static void danmaku_create_08_sentakki(SPRITE *src)
{
	BOSS99_DATA *data = (BOSS99_DATA *)src->data;
	if (0==((data->boss_base.danmaku_time_out)&0x03))
	{
		#if (0==USE_DESIGN_TRACK)
		play_voice_auto_track(VOICE14_BOSS_KOUGEKI_01);
		#else
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		src->tmp_angleCCW512 -= (int)(512/24);
		mask512(src->tmp_angleCCW512);
	//
			send1_obj->x256 = src->x256;		/* ’eŒ¹x256 */
			send1_obj->y256 = src->y256;		/* ’eŒ¹y256 */
			#if 1
			/* ‚ ‚Æ‚Å—v‚é */
//			send1_obj->h128 = src->h128;		/* ’e‚ */
//			send1_obj->w128 = src->w128;		/* ’e• */
			#endif
//
		{
		//	bullet_create_n_way_dan_sa_type(src,
			send1_obj->BULLET_REGIST_speed256			=	(t256(2.0)+((data->boss_base.danmaku_time_out)<<2));	/* ’e‘¬ */
			send1_obj->BULLET_REGIST_angle512			=	(src->tmp_angleCCW512);
			send1_obj->BULLET_REGIST_div_angle512		=	(int)(512/24);						/* Šp“x */
			send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_UROKO14_00_AOI;				/* [Â—Ø’e] */
			send1_obj->BULLET_REGIST_n_way				=	(8);								/* [8way] */
			bullet_regist_basic();
//
		//	bullet_create_n_way_dan_sa_type(src,
			send1_obj->BULLET_REGIST_speed256			=	(t256(2.0)+((data->boss_base.danmaku_time_out)<<2));	/* ’e‘¬ */
			send1_obj->BULLET_REGIST_angle512			=	((-src->tmp_angleCCW512)&(512-1));
			send1_obj->BULLET_REGIST_div_angle512		=	(int)(512/24);						/* Šp“x */
			send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_UROKO14_04_MIZUIRO;			/* […—Ø’e] */
			send1_obj->BULLET_REGIST_n_way				=	(8);								/* [8way] */
			bullet_regist_basic();
		}
	}
}

/*---------------------------------------------------------
	g2–Êƒ{ƒX ƒ`ƒ‹ƒm	’ÊíUŒ‚1(1/3)	©‹@‘_‚¢ŠgU’e(1-2-3-4-5-6-7-8’e)
	-------------------------------------------------------
	(1-2-3-4-5-6-7’e)‚©‚Æv‚Á‚Ä‚½‚¯‚ÇAŒ´ì‚â‚è’¼‚µ‚½‚çA
	(1-2-3-4-5-6-7-8’e)‚¾‚Á‚½‚Ì‚ÅC³B
‚¤[‚ñ
	Œ´ì‚â‚è’¼‚µ‚Ä‚æ‚­Œ©‚½‚çA
	(1-2-3-4-5-6-7’e)‚© 			(easy‚Æ‚©)
	(2-3-4-5-6-7-8’e)‚È‚ñ‚Å‚·‚ËB	(luna‚Æ‚©)
---------------------------------------------------------*/
static void danmaku_create_09_cirno_misogi(SPRITE *src)
{
	BOSS99_DATA *data = (BOSS99_DATA *)src->data;
	if (128==((data->boss_base.danmaku_time_out)))
	{
		tmp_angle_jikinerai512(player, src);/*©‹@‚Ë‚ç‚¢Špì¬*/
	}
	else
	if (0x40==((data->boss_base.danmaku_time_out)&0xc7))/* 8‰ñ(C³1-2-3-4-5-6-7’e:0way‚ÍŒ‚‚½‚È‚¢‚Ì‚ÅŒ‚‚Â‚Ì‚Í7‰ñ) */
	{
		#if (0==USE_DESIGN_TRACK)
		play_voice_auto_track(VOICE13_SAKUYA_SHOT02);
		#else
	//	voice_play(VOICE13_SAKUYA_SHOT02, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE13_SAKUYA_SHOT02);
		#endif
	//
		{
			send1_obj->x256 = src->x256;		/* ’eŒ¹x256 */
			send1_obj->y256 = src->y256;		/* ’eŒ¹y256 */
			#if 1
			/* ‚ ‚Æ‚Å—v‚é */
//			send1_obj->h128 = src->h128;		/* ’e‚ */
//			send1_obj->w128 = src->w128;		/* ’e• */
			#endif
//
		//	bullet_create_n_way_dan_sa_type(src,
			send1_obj->BULLET_REGIST_speed256			=	(t256(2.5));	/* ’e‘¬ */
			send1_obj->BULLET_REGIST_angle512			=	(src->tmp_angleCCW512);
			send1_obj->BULLET_REGIST_div_angle512		=	(int)(512/(64));		/* Šp“x(512[360/360“x]‚ğ 64 •ªŠ„) */	/* 1ü‚ğn•ªŠ„‚µ‚½Šp“x */
			send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_KOME_01_AOI; 	/* [Â•Ä’e] ‚É‚È‚Á‚Ä‚é‚Ì‚ÍŒ»İ •X’e ‚ÌƒOƒ‰‚ª–³‚¢‚©‚ç */
			send1_obj->BULLET_REGIST_n_way				=	((/*16*/7-((data->boss_base.danmaku_time_out)>>3))&7)+(1);/* [nway] */
			bullet_regist_basic();
		}
	}
}

/*---------------------------------------------------------
	g2–Êƒ{ƒX ƒ`ƒ‹ƒm	’ÊíUŒ‚1(2/3)	©‹@‘_‚¢‘S•ûˆÊ’e
	-------------------------------------------------------
---------------------------------------------------------*/
#if 0
#endif

/*---------------------------------------------------------
	g2–Êƒ{ƒX ƒ`ƒ‹ƒm	’ÊíUŒ‚1(3/3)	©‹@‘_‚¢û‘©’e
	-------------------------------------------------------
---------------------------------------------------------*/
#if 0
#endif


/*---------------------------------------------------------
	g3–Ê’†ƒ{ƒX g”ü—é
	’ÊíUŒ‚1(1/2)
	16•ûˆÊ•Î‚è‘S•ûˆÊ‚Î‚çT‚«’e
	-------------------------------------------------------
---------------------------------------------------------*/
#if 0
#endif

/*---------------------------------------------------------
	g3–Ê’†ƒ{ƒX g”ü—é
	‰Ø•„u–F‰Øˆºà£v‚É‚¿‚å‚Á‚Æ‚¾‚¯—‚½’e–‹(—\’è)
	Œv‰ñ‚èA”½Œv‰ñ‚è‚Ì6way—†ù’e(‰©F)		1‰ñ–ˆ‚É(512/(6*8))[512/512“x]‰ñ“]
	48•ûˆÊ‘S•ûˆÊ’e(ÔF)						8‰ñ–ˆ‚É”­’e
	-------------------------------------------------------
	–F‰Øˆºà£
	‰ğ‘œ“x‚Æ‚©‘¬“x‚Æ‚©‚Ì—vˆö‚ÅA‚»‚Ì‚Ü‚Ü‚Å‚Í psp‚Å“ï‚µ‚¢‹C‚à‚·‚éB
	Lunatic ‚Í‚±‚Ì‚Ü‚Ü‚ÅAHard ˆÈ‰º‚Í×H‚µ‚ÄˆÕ‚µ‚­‚·‚éB
---------------------------------------------------------*/
#if 1
static void danmaku_create_11_houka_kenran(SPRITE *src)
{
	enum
	{
		H0=0,H1,H2,H3,H4,H5
	};
	#define HOUGA_00_YEL_NUMS			(H0*4)
	#define HOUGA_04_YEL_DIV_ANGLE		(H1*4)
	#define HOUGA_16_YEL_ROTATE_ANGLE	(H2*4)
//
	#define HOUGA_08_RED_NUMS			(H3*4)
	#define HOUGA_12_RED_DIV_ANGLE		(H4*4)
	const static s16 houga_tbl[(H5*4)] =
	{/* easy				normal				hard				lunatic 			*/
		(4),				(8),				(5),				(6),					/* ’:3‚æ‚è4‚Ì•û‚ªŠÈ’P */
		(short)(512/(4)),	(short)(512/(8)),	(short)(512/(5)),	(short)(512/(6)),		/* ’:3‚æ‚è4‚Ì•û‚ªŠÈ’P */
		(short)(512/(4*8)), (short)(512/(8*8)), (short)(512/(5*8)), (short)(512/(6*8)), 	/* ’:3‚æ‚è4‚Ì•û‚ªŠÈ’P */
//
		(4*8),				(4*8),				(5*8),				(6*8),					/* ’:3‚æ‚è4‚Ì•û‚ªŠÈ’P */
		(short)(512/(4*8)), (short)(512/(4*8)), (short)(512/(5*8)), (short)(512/(6*8)), 	/* ’:3‚æ‚è4‚Ì•û‚ªŠÈ’P */
	};
	static int aaa_angle512;
	BOSS99_DATA *data = (BOSS99_DATA *)src->data;
	//
			send1_obj->x256 = src->x256;		/* ’eŒ¹x256 */
			send1_obj->y256 = src->y256;		/* ’eŒ¹y256 */
			#if 1
			/* ‚ ‚Æ‚Å—v‚é */
//			send1_obj->h128 = src->h128;		/* ’e‚ */
//			send1_obj->w128 = src->w128;		/* ’e• */
			#endif
//
//	if ((0x10)==((data->boss_base.danmaku_time_out)&0x1f))/* (16‰ñ‚É1‰ñ)(128‚È‚çŒv8‰ñ) */
	if ((0x40)==((data->boss_base.danmaku_time_out)&0x7f))/* (16‰ñ‚É1‰ñ)(128‚È‚çŒv8‰ñ) */
	{
		#if (0==USE_DESIGN_TRACK)
		play_voice_auto_track(VOICE14_BOSS_KOUGEKI_01);
		#else
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		{
			int aaa_angle512;
			tmp_angle_jikinerai512(player, src);/*©‹@‘_‚¢Špì¬*/
			aaa_angle512=(src->tmp_angleCCW512);
	//	bullet_create_n_way_dan_sa_type(src,
			send1_obj->BULLET_REGIST_speed256			=	(t256(2.0/*2.0*/)); 							/* ’e‘¬ */
			send1_obj->BULLET_REGIST_angle512			=	(aaa_angle512); 								/* ©‹@‘_‚¢’e */
			send1_obj->BULLET_REGIST_div_angle512		=	houga_tbl[HOUGA_12_RED_DIV_ANGLE+difficulty];	/*(int)(512/(48))*/ 				/* Šp“x(512[360/360“x]‚ğ 48 •ªŠ„) */	/* 1ü‚ğn•ªŠ„‚µ‚½Šp“x */
			send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_KOME_02_AKA; 							/* [ÔF•Ä’e] */
			send1_obj->BULLET_REGIST_n_way				=	houga_tbl[HOUGA_08_RED_NUMS+difficulty];		/*(48)*/								/* [48way] */	/* ”­’e” */
			bullet_regist_basic();
		}
	}
//	if ((0x02)==((data->boss_base.danmaku_time_out)&0x03))/* (2‰ñ‚É1‰ñ)(8‰ñ–ˆ‚É”­’e) */
	if ((0x08)==((data->boss_base.danmaku_time_out)&0x0f))/* (2‰ñ‚É1‰ñ)(8‰ñ–ˆ‚É”­’e) */
	{	// ‡‰ñ‚è(‰ºCCW‚¾‚©‚çA¶‰ñ‚èCCW)
	//	bullet_create_n_way_dan_sa_type(src,
		send1_obj->BULLET_REGIST_speed256			=	(t256(2.0/*2.0*/)); 							/* ’e‘¬ */
		send1_obj->BULLET_REGIST_angle512			=	((/*0+*/(aaa_angle512))&(512-1));				/* ”­Ë’†SŠp“x / “Áê‹@”\(©‹@‘_‚¢/‘¼) */
		send1_obj->BULLET_REGIST_div_angle512		=	houga_tbl[HOUGA_04_YEL_DIV_ANGLE+difficulty];	/*(int)(512/(6))*/					/* Šp“x(512[360/360“x]‚ğ 6 •ªŠ„) */ 	/* 1ü‚ğn•ªŠ„‚µ‚½Šp“x */
		send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_KOME_05_KIIRO;							/* [‰©F•Ä’e] */
		send1_obj->BULLET_REGIST_n_way				=	houga_tbl[HOUGA_00_YEL_NUMS+difficulty];		/*(6)*/ 								/* [1way] */	/* ”­’e” */
		bullet_regist_basic();
		// ‹t‰ñ‚è(‰ºCCW‚¾‚©‚çA‰E‰ñ‚èCW)
	//	bullet_create_n_way_dan_sa_type(src,
		send1_obj->BULLET_REGIST_speed256			=	(t256(2.0/*2.0*/)); 							/* ’e‘¬ */
		send1_obj->BULLET_REGIST_angle512			=	((512-(aaa_angle512))&(512-1)); 				/* ”­Ë’†SŠp“x / “Áê‹@”\(©‹@‘_‚¢/‘¼) */
		send1_obj->BULLET_REGIST_div_angle512		=	houga_tbl[HOUGA_04_YEL_DIV_ANGLE+difficulty];	/*(int)(512/(6))*/					/* Šp“x(512[360/360“x]‚ğ 6 •ªŠ„) */ 	/* 1ü‚ğn•ªŠ„‚µ‚½Šp“x */
		send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_KOME_05_KIIRO;							/* [‰©F•Ä’e] */
		send1_obj->BULLET_REGIST_n_way				=	houga_tbl[HOUGA_00_YEL_NUMS+difficulty];		/*(6)*/ 								/* [1way] */	/* ”­’e” */
		bullet_regist_basic();
		// ‰ñ“]—Ê
		aaa_angle512 += houga_tbl[HOUGA_16_YEL_ROTATE_ANGLE+difficulty];	/*(512/(6*8))*/ 				/* Šp“x(512[360/360“x]‚ğ 48•ªŠ„) */
	}
}
#endif

/*---------------------------------------------------------
	g1–Êƒ{ƒX ƒ‹[ƒ~ƒA –é•„uƒiƒCƒgƒo[ƒhv‚É‚¿‚å‚Á‚Æ‚¾‚¯—‚½’e–‹(—\’è)
	-------------------------------------------------------
	5’e–Ú‚ª©‹@‘_‚¢‚Ì 32•ªŠ„16•ûŒü ‹|ó’eB
	-------------------------------------------------------
0000 0000 0x00==(time_out)&0x11 Â(0x00==((time_out)&0x20))
0001 0000						Â(0x00==((time_out)&0x20))
0010 0000 0x00==(time_out)&0x11 —Î
0011 0000						—Î
0100 0000 0x00==(time_out)&0x11 Â(0x00==((time_out)&0x20))
0101 0000						Â(0x00==((time_out)&0x20))
0110 0000 0x00==(time_out)&0x11 —Î
0111 0000						—Î
//
1000 0000 start
---------------------------------------------------------*/
static void danmaku_create_04_night_bird(SPRITE *src)
{
	static int aaa_angle512;
	BOSS99_DATA *data = (BOSS99_DATA *)src->data;
//	if (128==((data->boss_base.danmaku_time_out)))
	if (0x10==((data->boss_base.danmaku_time_out)&0x10))
	{
		tmp_angle_jikinerai512(player, src);/*©‹@‚Ë‚ç‚¢Špì¬*/
		aaa_angle512 = src->tmp_angleCCW512;/*©‹@‚Ë‚ç‚¢Šp*/
		aaa_angle512 -= (int)(512*(5)/(32));/* 5’e–Ú‚ª©‹@‘_‚¢ */
		/* Šp“x(512[360/360“x]‚ğ 32•ªŠ„) */
	}
	else
	if (0x00==((data->boss_base.danmaku_time_out)&0x10))/* 16‰ñ */
	{
		#if (0==USE_DESIGN_TRACK)
		play_voice_auto_track(VOICE13_SAKUYA_SHOT02/*VOICE14_BOSS_KOUGEKI_01*/);
		#else
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE13_SAKUYA_SHOT02/*VOICE14_BOSS_KOUGEKI_01*/);
		#endif
		send1_obj->w128 = src->w128;	/* ”­’eˆÊ’u‚Ì offset —p */
		send1_obj->h128 = src->h128;	/* ”­’eˆÊ’u‚Ì offset —p */
		send1_obj->y256 = src->y256;	/* ”­’eˆÊ’u‚Ì offset —p */
		{
			int iii;
				iii = (data->boss_base.danmaku_time_out);
			int aaa;/* ’eF */
			int bbb;/* •ûŒü */
			if (0x00==((data->boss_base.danmaku_time_out)&0x20))
					{	bbb=-1;aaa=1;/* ˜g‚Â‚«Â’e */}
			else	{	bbb= 1;aaa=0;/* ˜g‚Â‚«—Î’e */}
		//	send1_obj->x256 = src->x256+((16-(iii&0x0f))<<(3+8))*bbb;/* –{•¨‚Í”­’eˆÊ’u‚Ì x offset ‚ ‚è */
			send1_obj->x256 = src->x256+((16-(iii&0x0f))<<(3+7))*bbb;/* –{•¨‚Í”­’eˆÊ’u‚Ì x offset ‚ ‚è */
	//		bullet_create_n_way_dan_sa_type(send1_obj/*src*/,
			send1_obj->BULLET_REGIST_speed256			=	(t256(1.5));							/* ’e‘¬ */
			send1_obj->BULLET_REGIST_angle512			=	((/*0+*/(aaa_angle512)*bbb)&(512-1));	/* ”­Ë’†SŠp“x / “Áê‹@”\(©‹@‘_‚¢/‘¼) */
	//		send1_obj->BULLET_REGIST_div_angle512		=	(0);									/* ƒ_ƒ~[Šp“x(–¢g—p) */
			send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_MARU12_02_MIDORI+aaa;			/* [˜g‚Â‚«—Î’e]  [˜g‚Â‚«Â’e] */	/*BULLET_MARU12_03_AOI*/
			send1_obj->BULLET_REGIST_n_way				=	(1);									/* [1way] */
			bullet_regist_basic();
		}
		/* Ÿ‚Ì’e‚ÌŠp“x */
		aaa_angle512 += (int)(512/(32));/* Šp“x(512[360/360“x]‚ğ 32•ªŠ„) */
	}
}

/*---------------------------------------------------------
	·•ª‚Ì—d‰ö1(“V‹ç—l?)‚ªŒ‚‚Â’e–‹(—\’è)
	-------------------------------------------------------
	–¢ì¬
---------------------------------------------------------*/
static void danmaku_create_10_tengu_shot(SPRITE *src)
{
	static int aaa_angle512;
	static int data_wait2_start;/* ”­’eŠÔŠustart */
	static int data_wait2;/* ”­’eŠÔŠu */
	BOSS99_DATA *data = (BOSS99_DATA *)src->data;
//	if (128==((data->boss_base.danmaku_time_out)))
	if (0x10==((data->boss_base.danmaku_time_out)&0x10))
	{
		data_wait2_start = (20-((difficulty)<<2))/*8*/ /*10*/;
		data_wait2 = 10;
		tmp_angle_jikinerai512(player, src);/*©‹@‚Ë‚ç‚¢Špì¬*/
		aaa_angle512 = src->tmp_angleCCW512;/*©‹@‚Ë‚ç‚¢Šp*/
		aaa_angle512 -= (int)(512*(5)/(32));/* 5’e–Ú‚ª©‹@‘_‚¢ */
		/* Šp“x(512[360/360“x]‚ğ 32•ªŠ„) */
	}
	else
//	if (0x00==((data->boss_base.danmaku_time_out)&0x10))/* 16‰ñ */
	{
		data_wait2--;
		if (1 > data_wait2)
		{
			data_wait2 = data_wait2_start;
			#if (0==USE_DESIGN_TRACK)
			play_voice_auto_track(VOICE13_SAKUYA_SHOT02/*VOICE14_BOSS_KOUGEKI_01*/);
			#else
		//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE13_SAKUYA_SHOT02/*VOICE14_BOSS_KOUGEKI_01*/);
			#endif
			send1_obj->w128 = src->w128;	/* ”­’eˆÊ’u‚Ì offset —p */
			send1_obj->h128 = src->h128;	/* ”­’eˆÊ’u‚Ì offset —p */
			send1_obj->y256 = src->y256;	/* ”­’eˆÊ’u‚Ì offset —p */
			{
			int iii;
				iii = (data->boss_base.danmaku_time_out);
			int aaa;/* ’eF */
			int bbb;/* •ûŒü */
				if (0x00==((data->boss_base.danmaku_time_out)&0x20))
						{	bbb=-1;aaa=1;/* ˜g‚Â‚«Â’e */}
				else	{	bbb= 1;aaa=0;/* ˜g‚Â‚«—Î’e */}
			//	send1_obj->x256 = src->x256+((16-(iii&0x0f))<<(3+8))*bbb;/* –{•¨‚Í”­’eˆÊ’u‚Ì x offset ‚ ‚è */
				send1_obj->x256 = src->x256+((16-(iii&0x0f))<<(3+7))*bbb;/* –{•¨‚Í”­’eˆÊ’u‚Ì x offset ‚ ‚è */
		//		bullet_create_n_way_dan_sa_type(send1_obj/*src*/,
				send1_obj->BULLET_REGIST_speed256			=	(t256(1.5));							/* ’e‘¬ */
				send1_obj->BULLET_REGIST_angle512			=	((/*0+*/(aaa_angle512)*bbb)&(512-1));	/* ’eŒ¹Šp“x512 */
		//		send1_obj->BULLET_REGIST_div_angle512		=	(0);									/* ƒ_ƒ~[Šp“x(–¢g—p) */
				send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_MARU12_02_MIDORI+aaa;			/* [˜g‚Â‚«—Î’e]  [˜g‚Â‚«Â’e] */	/*BULLET_MARU12_03_AOI*/
				send1_obj->BULLET_REGIST_n_way				=	(1);									/* [1way] */
				bullet_regist_basic();
			}
			/* Ÿ‚Ì’e‚ÌŠp“x */
		aaa_angle512 += (int)(512/(32));/* Šp“x(512[360/360“x]‚ğ 32•ªŠ„) */
		}
	}
}

/*---------------------------------------------------------
	•¶‚É”ü—é‚Á‚Û‚¢’e–‹Œ‚‚½‚¹‚Ä‚İ‚éƒeƒXƒg(‚Ì”¤‚¾‚Á‚½‚ñ‚¾‚¯‚ÇA
	‰½ŒÌ‚©“–‰‚Ì•ûŒü‚Æ‚Íˆá‚¤•ûŒü‚Écc)
	-------------------------------------------------------
---------------------------------------------------------*/
#if 1
extern void bullet_create_aya_ice(SPRITE *src);

static void danmaku_create_12_aya_merin_test(SPRITE *src)
{
	BOSS99_DATA *data = (BOSS99_DATA *)src->data;
	if (0x10==((data->boss_base.danmaku_time_out)&0x1f))/* (16‰ñ‚É1‰ñ)(128‚È‚çŒv8‰ñ) */
	{
		#if (0==USE_DESIGN_TRACK)
		play_voice_auto_track(VOICE14_BOSS_KOUGEKI_01);
		#else
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		bullet_create_aya_ice(src);
	}
}
#endif

/*---------------------------------------------------------
	‹¤’Ê’e–‹¶¬•”
	-------------------------------------------------------

---------------------------------------------------------*/
/*static*/ void danmaku_generator(SPRITE *src)
{
	BOSS99_DATA *data=(BOSS99_DATA *)src->data;
	if (DANMAKU_00 != data->boss_base.danmaku_type) 	/* ’e–‹¶¬‚Í•K—vH */
	{
		data->boss_base.danmaku_time_out -= 1/*fps_fa ctor*/;	/* ŠÔŒo‰ß‚·‚é  */
		if (0 < data->boss_base.danmaku_time_out)				/* ŠÔØ‚êH */
		{
			/* ’e–‹¶¬’† */
			/*const*/static void (*danmaku_create_bbb[(DANMAKU_MAX/*16+1*/)])(SPRITE *sss) =
			{
				#define danmaku_create_99_mitei danmaku_create_09_cirno_misogi
				NULL,								/* 00 */	/* ’e–‹¶¬‚µ‚È‚¢ */
			//													/* [ç–é] */
				danmaku_create_01_sakuya_misogi,	/* 01 */	/* ç–éâS’e–‹ / g5–Ê’†ƒ{ƒX ç–é u’ÊíUŒ‚v */
				danmaku_create_02_24nerai,			/* 02 */	/* ç–é24’e–‹ (Šïpuƒ~ƒXƒfƒBƒŒƒNƒVƒ‡ƒ“‚à‚Ç‚«(1/2)v) */
				danmaku_create_03_11nife,			/* 03 */	/* ç–é11’e–‹ (Šïpuƒ~ƒXƒfƒBƒŒƒNƒVƒ‡ƒ“‚à‚Ç‚«(2/2)v) */
				danmaku_create_04_pink_hearts,		/* 04 */	/* g5–Êƒ{ƒX ç–é u’ÊíUŒ‚1(1/2)v‚É‚¿‚å‚Á‚Æ‚¾‚¯—‚½’e–‹(—\’è) */
				danmaku_create_05_32way_dual,		/* 05 */	/* g5–Êƒ{ƒX ç–é u’ÊíUŒ‚1/2(2/2)v‚É‚¿‚å‚Á‚Æ‚¾‚¯—‚½’e–‹(—\’è) */
				danmaku_create_06_luna_clock_32way, /* 06 */	/* g5–Êƒ{ƒX ç–é Œ¶Ûuƒ‹ƒiƒNƒƒbƒN(1/2)v‚É‚¿‚å‚Á‚Æ‚¾‚¯—‚½’e–‹(—\’è) */
				danmaku_create_07_80way_dual_five,	/* 07 */	/* g5–Êƒ{ƒX ç–é u’ÊíUŒ‚3(1/2)v‚É‚¿‚å‚Á‚Æ‚¾‚¯—‚½’e–‹(—\’è) */
			//													/* [ƒAƒŠƒX(‰¼)] */
				danmaku_create_04_night_bird,		/* 08 */	/* g1–Êƒ{ƒX ƒ‹[ƒ~ƒA –é•„uƒiƒCƒgƒo[ƒhv‚É‚¿‚å‚Á‚Æ‚¾‚¯—‚½’e–‹(—\’è) */
				danmaku_create_08_sentakki, 		/* 09 */	/* ô‘ó‹@’e–‹ */
				danmaku_create_11_houka_kenran, 	/* 0a */	/* g”ü—é ‰Ø•„u–F‰Øˆºà£v‚É‚¿‚å‚Á‚Æ‚¾‚¯—‚½’e–‹(—\’è) */
				danmaku_create_99_mitei,			/* 0b */	/* –¢’è’e–‹ */
				danmaku_create_99_mitei,			/* 0c */	/* –¢’è’e–‹ */
				danmaku_create_99_mitei,			/* 0d */	/* –¢’è’e–‹ */
				danmaku_create_99_mitei,			/* 0e */	/* –¢’è’e–‹ */
				danmaku_create_99_mitei,			/* 0f */	/* –¢’è’e–‹ */
			//													/* [ƒ`ƒ‹ƒm(‰¼)] */
				danmaku_create_09_cirno_misogi, 	/* 10 */	/* ƒ`ƒ‹ƒmâS’e–‹ */
				danmaku_create_10_tengu_shot,		/* 11 */	/* ·•ª‚Ì—d‰ö1(“V‹ç—l?)‚ªŒ‚‚Â’e–‹(—\’è) */
				danmaku_create_12_aya_merin_test,	/* 12 */	/* •¶‚É”ü—é‚Á‚Û‚¢’e–‹Œ‚‚½‚¹‚Ä‚İ‚éƒeƒXƒg */
				danmaku_create_99_mitei,			/* 13 */	/* –¢’è’e–‹ */
				danmaku_create_99_mitei,			/* 14 */	/* –¢’è’e–‹ */
				danmaku_create_99_mitei,			/* 15 */	/* –¢’è’e–‹ */
				danmaku_create_99_mitei,			/* 16 */	/* –¢’è’e–‹ */
				danmaku_create_99_mitei,			/* 17 */	/* –¢’è’e–‹ */
			//
				danmaku_create_99_mitei,			/* 18 */	/* –¢’è’e–‹ */
				danmaku_create_99_mitei,			/* 19 */	/* –¢’è’e–‹ */
				danmaku_create_99_mitei,			/* 1a */	/* –¢’è’e–‹ */
				danmaku_create_99_mitei,			/* 1b */	/* –¢’è’e–‹ */
				danmaku_create_99_mitei,			/* 1c */	/* –¢’è’e–‹ */
				danmaku_create_99_mitei,			/* 1d */	/* –¢’è’e–‹ */
				danmaku_create_99_mitei,			/* 1e */	/* –¢’è’e–‹ */
				danmaku_create_99_mitei,			/* 1f */	/* –¢’è’e–‹ */
			};
			(danmaku_create_bbb[(data->boss_base.danmaku_type/*-1*/)])(src);
		}
		else
		{
			data->boss_base.danmaku_type = DANMAKU_00;			/* ’e–‹¶¬I—¹ */
		}
	}
}
