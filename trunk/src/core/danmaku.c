
#include "bullet_object.h"

/*---------------------------------------------------------
	’e–‹¶¬
	-------------------------------------------------------
	’e–‹‚ΝƒVƒXƒeƒ€‚ΕƒTƒ|[ƒg‚·‚ι\‘z‚ª‚ ‚θ‚ά‚·B(\‘z’†)
	ƒVƒXƒeƒ€‚ΕƒTƒ|[ƒg‚³‚κ‚ι‚ΖA‚Η‚Μƒ{ƒX‚ªA‚Η‚Μ’e–‹‚Ε‚ΰ‚‚Δ‚ι‚ζ‚¤‚Ι‚Θ‚θ‚ά‚·B
---------------------------------------------------------*/


/*---------------------------------------------------------
	©‹@‘_‚Ά’e‚Μp“x‚πvZ
---------------------------------------------------------*/

/*static*/ void tmp_angle_jikinerai512(SPRITE *p, SPRITE *t)
{
	/* Θ—ª”Ε(’e‚Ζ©•ª‚ª‘ε‚«‚³‚ª“―‚¶‚Θ‚η‚Έ‚κ‚Θ‚ΆAα‚¤‚Ζ‚»‚Μ•ªλ·‚Ι‚Θ‚ι) */
	t->tmp_angleCCW512 =  (atan_512(p->y256-t->y256, p->x256-t->x256));
}


/*---------------------------------------------------------
	g5–Κ’†ƒ{ƒX η–ι ‘ζκU‚‚Ι‚Ώ‚ε‚Α‚Ζ‚Ύ‚―—‚½’e–‹
	-------------------------------------------------------
	–{‰Ζ‚ΝΕ’θ’e–‹(?)‚Ύ‚―‚ΗA‚±‚κ‚Ν‘_‚ΆnB
	(‰Ό‚ΙΕ’θ’e–‹‚Ύ‚Α‚½‚Ζ‚µ‚Δ‚ΰ)‚»‚ΰ‚»‚ΰγ‚Ιs‚―‚Θ‚Ά‚©‚ηAγ”π‚―‚Νo—‚Θ‚ΆB
	(psp‚Μc‰π‘“x‚Ε‚Νγ‚Ιs‚­ƒXƒy[ƒX‚ª‚Θ‚Ά)
	‰Ί”π‚―‚ΰpsp‚Μc‰π‘“x‚ª‘S‘R‘«‚θ‚Θ‚Ά‚©‚ηA–{‰Ζ•—‚Μƒ`ƒ‡ƒ“”π‚―‚Ν–³—B
	‚ ‚ι’φ“x¶‰E‚Ι“®‚«‰ρ‚Α‚Δ”π‚―‚Δ–α‚¤–‚π‘z’θ‚µ‚Δ‚Ά‚ιB(‚Ύ‚©‚η’e‘¬’x‚Ά)
	‚»‚κ‚Ι•Ή‚Ή‚Δ–§’…’e‚Ν‚ ‚¦‚Δ‚Ί‚ρ‚Ί‚ρα‚¤B(•Κ•Ο”‚Ζ‚ι‚Μ‚ª–Κ“|‚Ύ‚Α‚½–‚ΰ‚ ‚ι)
---------------------------------------------------------*/

static void danmaku_create_01_sakuya_misogi(SPRITE *src)
{
	static int aaa_angle512;
//
	if (0==((src->boss_base_danmaku_time_out)&0x07))
	{
		#if (1)
	//	voice_play(VOICE13_SAKUYA_SHOT02, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE13_SAKUYA_SHOT02);
		#endif
		src->tmp_angleCCW512 -= (int)(128/5)+(5);	/*CCW*/
		mask512(src->tmp_angleCCW512);
		send1_obj->x256 							= (src->x256);						/* ’eΉx256 */
		send1_obj->y256 							= (src->y256);						/* ’eΉy256 */
		send1_obj->BULLET_REGIST_speed256			= (t256(2.5));						/* ’e‘¬ */
		send1_obj->BULLET_REGIST_angle512			= (src->tmp_angleCCW512);			/* ’eΉp“x512 */			/* [—†ωσ’e] */
		send1_obj->BULLET_REGIST_div_angle512		= (int)(128/5);
		send1_obj->BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_04_AOI;			/* [ΒƒiƒCƒt’e] */
		send1_obj->BULLET_REGIST_n_way				= (8);								/* [8way] */
		bullet_regist_basic();

		if (0==((src->boss_base_danmaku_time_out)&0x1f))/* reset (1/32) */
		{
			aaa_angle512=(src->tmp_angleCCW512)-(int)((512/2));
		}
	}
	else
//	if ((10-1)>((((u8)src->boss_base_danmaku_time_out)^0x20)&0x2f))/* x 10way */
	if ((10-1)>(((src->boss_base_danmaku_time_out)	  )&0x0f))/* x 10way */
	{
		aaa_angle512 -= (int)(128/5);
		mask512(aaa_angle512);
		send1_obj->x256 							= (src->x256);						/* ’eΉx256 */
		send1_obj->y256 							= (src->y256);						/* ’eΉy256 */
		send1_obj->BULLET_REGIST_speed256			= (t256(2.0));						/* ’e‘¬ */
		send1_obj->BULLET_REGIST_angle512			= (aaa_angle512);					/* ’eΉp“x512 */
		send1_obj->BULLET_REGIST_div_angle512		= (int)(difficulty+1/*3*/); 		/* [–§’…’e]‚Μp“x */
		send1_obj->BULLET_REGIST_bullet_obj_type	= BULLET_KUNAI12_01_AKA;			/* [ΤƒNƒiƒC’e] */
		send1_obj->BULLET_REGIST_n_way				= (4);								/* [4way] */
		bullet_regist_basic();
	}
}

/*---------------------------------------------------------
	g5–Κ’†ƒ{ƒX η–ιuοpFƒ~ƒXƒfƒBƒƒNƒVƒ‡ƒ“(1/2)v‚Ι‚Ώ‚ε‚Α‚Ζ‚Ύ‚―—‚½’e–‹
	-------------------------------------------------------
	©‹@‘_‚ΆA‘S•ϋό24•ª„’e
	-------------------------------------------------------
	[’F»έbu llet_create_n_way_dan_sa_type()‚Μ©‹@‘_‚Ά‚ΝƒƒUƒg‘_‚ν‚Θ‚Άο”’e‚Θ‚Μ‚ΕA“®‚©‚Θ‚―‚κ‚Ξ“–‚½‚θ‚ά‚Ή‚ρ‚ªA
	‚»‚Μ‚¤‚Ώ‚±‚±‚Ν‹τ”’e‚Ι‚·‚ι—\’θ‚Θ‚Μ‚ΕA“®‚©‚Θ‚―‚κ‚Ξ“–‚½‚ι‚ζ‚¤‚Ι‚Θ‚ι—\’θ‚Ε‚·]
---------------------------------------------------------*/
static void danmaku_create_02_24nerai(SPRITE *src)
{
	if (0==((src->boss_base_danmaku_time_out)&0x03))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
		tmp_angle_jikinerai512(player, src);/*©‹@‘_‚Άpμ¬*/
		send1_obj->x256 							= (src->x256);						/* ’eΉx256 */
		send1_obj->y256 							= (src->y256);						/* ’eΉy256 */
		send1_obj->BULLET_REGIST_speed256			= (t256(2.0)+((src->boss_base_danmaku_time_out)<<2));	/* ’e‘¬ */
		send1_obj->BULLET_REGIST_angle512			= (src->tmp_angleCCW512);			/* ’eΉp“x512 */	/* ©‹@‘_‚Ά’e */
		send1_obj->BULLET_REGIST_div_angle512		= (int)(512/24);					/* p“x */
		send1_obj->BULLET_REGIST_bullet_obj_type	= BULLET_KUNAI12_01_AKA;			/* [ΤƒNƒiƒC’e] */
		send1_obj->BULLET_REGIST_n_way				= (24); 							/* [24way] */
		bullet_regist_basic();
	}
}

/*---------------------------------------------------------
	g5–Κ’†ƒ{ƒX η–ι uοpFƒ~ƒXƒfƒBƒƒNƒVƒ‡ƒ“(2/2)v‚Ι‚Ώ‚ε‚Α‚Ζ‚Ύ‚―—‚½’e–‹
	-------------------------------------------------------
	©‹@‘_‚ΆA11’e(’†S‚ΖA¶‰E‚Ι5’e‚Έ‚Β‘_‚Ά)
	-------------------------------------------------------
	[’F»έbu llet_create_n_way_dan_sa_type()‚Μ©‹@‘_‚Ά‚ΝƒƒUƒg‘_‚ν‚Θ‚Άο”’e‚Θ‚Μ‚ΕA“®‚©‚Θ‚―‚κ‚Ξ“–‚½‚θ‚ά‚Ή‚ρ‚ªA
	‚»‚Μ‚¤‚Ώ‚±‚±‚Ν‹τ”’e‚Ι‚·‚ι—\’θ‚Θ‚Μ‚ΕA“®‚©‚Θ‚―‚κ‚Ξ“–‚½‚ι‚ζ‚¤‚Ι‚Θ‚ι—\’θ‚Ε‚·]
---------------------------------------------------------*/
static void danmaku_create_03_11nife(SPRITE *src)
{
	if (0==((src->boss_base_danmaku_time_out)&0x07))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
		tmp_angle_jikinerai512(player, src);/*©‹@‚Λ‚η‚Άpμ¬*/
		send1_obj->x256 							= (src->x256);						/* ’eΉx256 */
		send1_obj->y256 							= (src->y256);						/* ’eΉy256 */
		send1_obj->BULLET_REGIST_speed256			= (t256(2.0));						/* ’e‘¬ */
		send1_obj->BULLET_REGIST_angle512			= (src->tmp_angleCCW512);			/* ’eΉp“x512 */
		send1_obj->BULLET_REGIST_div_angle512		= (int)(512/(7*4)); 				/* p“x([90/360]“x‚π7•ª„) */
		send1_obj->BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_04_AOI;			/* [ΒƒiƒCƒt’e] */
		send1_obj->BULLET_REGIST_n_way				= (11); 							/* [11way] */
		bullet_regist_basic();
	}
}

/*---------------------------------------------------------
	g5–Κƒ{ƒX η–ι u’ΚνU‚1(1/2)v‚Ι‚Ώ‚ε‚Α‚Ζ‚Ύ‚―—‚½’e–‹(—\’θ)
	-------------------------------------------------------
	24way =(‰E12way)+(¶12way)
	-------------------------------------------------------

---------------------------------------------------------*/
static void danmaku_create_04_pink_hearts(SPRITE *src)
{

	if (0==((src->boss_base_danmaku_time_out)&0x0f))/* –{•¨‚Ν 8 ’e(128==0x80==danmaku_time_out) */
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		send1_obj->y256 = (src->y256);		/* ’eΉy256 */	/* –{•¨‚Ν”­’eΚ’u‚Μ offset —p */
//		send1_obj->tmp_angleCCW512 = (0);/* ‰Ίό‚« */
	//
		int i;
		for (i=0; i<(13*12)/*144*/; i+= 13 )/*12•ϋό*/		/* p“x(128[(90+a)/360“x]‚π 10 •ª„) */
		{
			send1_obj->x256 = src->x256+t256(4.0);/* –{•¨‚Ν”­’eΚ’u‚Μ x offset ‚ ‚θ */
			send1_obj->BULLET_REGIST_speed256			= ((160-i)<<(1+1+1));				/* ’e‘¬ */
			send1_obj->BULLET_REGIST_angle512			= (i/*(0+i)&(512-1)*/);
	//		send1_obj->BULLET_REGIST_div_angle512		= (0);								/* ƒ_ƒ~[p“x(–Άg—p) */
			send1_obj->BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_06_YUKARI; 		/* [ƒsƒ“ƒNƒiƒCƒt’e] */
			send1_obj->BULLET_REGIST_n_way				= (1);								/* [1way] */
			bullet_regist_basic();
//
			send1_obj->x256 = src->x256-t256(4.0);	/* –{•¨‚Ν”­’eΚ’u‚Μ x offset ‚ ‚θ */
			send1_obj->BULLET_REGIST_speed256			= ((160-i)<<(1+1+1));				/* ’e‘¬ */
			send1_obj->BULLET_REGIST_angle512			= ((0-i)&(512-1));
	//		send1_obj->BULLET_REGIST_div_angle512		= (0);								/* ƒ_ƒ~[p“x(–Άg—p) */
			send1_obj->BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_06_YUKARI; 		/* [ƒsƒ“ƒNƒiƒCƒt’e] */
			send1_obj->BULLET_REGIST_n_way				= (1);								/* [1way] */
			bullet_regist_basic();
		}
	}
}

/*---------------------------------------------------------
	g5–Κƒ{ƒX η–ι u’ΚνU‚1(2/2)v‚Ι‚Ώ‚ε‚Α‚Ζ‚Ύ‚―—‚½’e–‹(—\’θ)
	-------------------------------------------------------
	-------------------------------------------------------

---------------------------------------------------------*/
static void danmaku_create_05_32way_dual(SPRITE *src)
{

	if (0==((src->boss_base_danmaku_time_out)&0x3f))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
			send1_obj->x256 = (src->x256);		/* ’eΉx256 */
			send1_obj->y256 = (src->y256);		/* ’eΉy256 */
	//
		int i;
		for (i=0; i<(512); i+= 16 )/*32•ϋό*/	/* p“x(512[360/360“x]‚π 32 •ª„) */
		{
			send1_obj->BULLET_REGIST_speed256			= (t256(2.0)-((i&16)<<4));	/*ο”‚Ν‚‘¬’e*/
			send1_obj->BULLET_REGIST_angle512			= (i/*(0+i)&(512-1)*/); 	/* ’eΉp“x512 */
	//		send1_obj->BULLET_REGIST_div_angle512		= (0);						/* ƒ_ƒ~[p“x(–Άg—p) */
			send1_obj->BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_04_AOI;		/* [ΒƒiƒCƒt’e] */
			send1_obj->BULLET_REGIST_n_way				= (1);						/* [1way] */
			bullet_regist_basic();
		}
	}
}

/*---------------------------------------------------------
	¶έuƒNƒƒbƒNƒR[ƒvƒX(1/2)v
---------------------------------------------------------*/
#if 0
#endif

/*---------------------------------------------------------
	¶έuƒNƒƒbƒNƒR[ƒvƒX(2/2)v
---------------------------------------------------------*/
#if 0
#endif

/*---------------------------------------------------------
	¶Ϋuƒ‹ƒiƒNƒƒbƒN(1/2)v
	-------------------------------------------------------
	32way Ε’θA’e	x 4‰ρ
---------------------------------------------------------*/
static void danmaku_create_06_luna_clock_32way(SPRITE *src)
{

	if (0x40==((src->boss_base_danmaku_time_out)&0xcf))/* 4‰ρ */
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
		send1_obj->BULLET_REGIST_speed256			= (t256(2.0));				/* ’e‘¬ */
		send1_obj->BULLET_REGIST_angle512			= (0);						/* ”­Λ’†Sp“x / “Ακ‹@”\(©‹@‘_‚Ά/‘Ό) */
		send1_obj->BULLET_REGIST_div_angle512		= (int)(512/(32));			/* p“x(512[360/360“x]‚π 32 •ª„) */	/* 1ό‚πn•ª„‚µ‚½p“x */
		send1_obj->BULLET_REGIST_bullet_obj_type	= BULLET_CAP16_05_SIROI;	/* [Β”’•Δ’e] */	/* ’eƒOƒ‰ */
		send1_obj->BULLET_REGIST_n_way				= (32); 					/* [32way] */		/* ”­’e” */
		bullet_regist_basic();
	}
}

/*---------------------------------------------------------
	¶Ϋuƒ‹ƒiƒNƒƒbƒN(2/2)v
---------------------------------------------------------*/
#if 0
#endif

/*---------------------------------------------------------
	u’ΚνU‚3(1/2)v
	-------------------------------------------------------
	gU^‘S•ϋΚƒiƒCƒt’e
	80•ª„’e == (8way[hi to 0]+8way[slow to 0]) x 5way(5•ª„)
	-------------------------------------------------------
	–{•¨‚Ν‰ζ–Κγ‚Ζ‰ζ–Κ¶‚Ζ‰ζ–Κ‰E‚Μ’[‚ΕƒiƒCƒt‚ªκ“x‚Ύ‚―”½Λ‚·‚ιB
---------------------------------------------------------*/
static void danmaku_create_07_80way_dual_five(SPRITE *src)
{

	if (0==((src->boss_base_danmaku_time_out)&0x3f))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
			send1_obj->x256 = (src->x256);		/* ’eΉx256 */
			send1_obj->y256 = (src->y256);		/* ’eΉy256 */
	//
		int ii; 	/* 1ό‚π65536•ª„‚µ‚½p“x */
		int j;		/* ƒJƒEƒ“ƒ^ */
		j=0;
		for (ii=0; ii<(0x10000); ii += 819 )/* 819.2(65536/80)	6.4==(512/80) 80•ª„ [5•ª„] */ 	/* 102.4==(512/5) p“x(512[72/360“x]‚π 5 •ª„) */
		{
			j++;
			j &= (16-1);/* 16way(8way[hi to 0]+8way[slow to 0]) */
			send1_obj->BULLET_REGIST_speed256			= (t256(3.0)-((j)<<5)-((j&1)<<7));	/*ο”‚Ν‚‘¬’e*/
			send1_obj->BULLET_REGIST_angle512			= (((0-ii)>>7)&(512-1));				/* p“x */
	//		send1_obj->BULLET_REGIST_div_angle512		= (0);								/* ƒ_ƒ~[p“x(–Άg—p) */
			send1_obj->BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_04_AOI;				/* [ΒƒiƒCƒt’e] */
			send1_obj->BULLET_REGIST_n_way				= (1);								/* [1way] */
			bullet_regist_basic();
		}
	}
}

/*---------------------------------------------------------
	ƒƒCƒh”ι‹Zu‘€‚θƒh[ƒ‹v
---------------------------------------------------------*/
#if 0
#endif

/*---------------------------------------------------------
	ƒAƒƒX(‰Ό)	τ‘σ‹@’e–‹
	-------------------------------------------------------
	ƒeƒLƒg[
---------------------------------------------------------*/
static void danmaku_create_08_sentakki(SPRITE *src)
{

	if (0==((src->boss_base_danmaku_time_out)&0x03))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		src->tmp_angleCCW512 -= (int)(512/24);
		mask512(src->tmp_angleCCW512);
	//
			send1_obj->x256 = (src->x256);		/* ’eΉx256 */
			send1_obj->y256 = (src->y256);		/* ’eΉy256 */
	//
		{
			send1_obj->BULLET_REGIST_speed256			= (t256(2.0)+((src->boss_base_danmaku_time_out)<<2));	/* ’e‘¬ */
			send1_obj->BULLET_REGIST_angle512			= (src->tmp_angleCCW512);								/* p“x */
			send1_obj->BULLET_REGIST_div_angle512		= (int)(512/24);						/* •ª„p“x */
			send1_obj->BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_00_AOI;				/* [Β—Ψ’e] */
			send1_obj->BULLET_REGIST_n_way				= (8);									/* [8way] */
			bullet_regist_basic();
		//
			send1_obj->BULLET_REGIST_speed256			= (t256(2.0)+((src->boss_base_danmaku_time_out)<<2));	/* ’e‘¬ */
			send1_obj->BULLET_REGIST_angle512			= ((-src->tmp_angleCCW512)&(512-1));					/* p“x */
			send1_obj->BULLET_REGIST_div_angle512		= (int)(512/24);						/* •ª„p“x */
			send1_obj->BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_04_MIZUIRO;			/* […—Ψ’e] */
			send1_obj->BULLET_REGIST_n_way				= (8);									/* [8way] */
			bullet_regist_basic();
		}
	}
}


/*---------------------------------------------------------
	–£–‚A‰Τ‚Δ‚·‚Ζ’e–‹
---------------------------------------------------------*/

static void enemy_boss01_nway_fire(SPRITE *src)
{
	static int adj_hari;	/* Jn’n“_ */
//	adj_hari = angle_jikinerai512(player, src);
	tmp_angle_jikinerai512(player, src);/*©‹@‘_‚Άpμ¬*/
	adj_hari = (src->tmp_angleCCW512);
//
	int speed256;
	speed256 = (0x200)+(ra_nd()&0xff);
//
	const int src__x256 = (src->x256)+((0<src->vx256)?(t256(16)):(0));/* ‰EΪ“®’†‚Ν‰E‘¤‚©‚η‚‚Β */
//
	int ii;
	for (ii=(adj_hari/*0*/); ii<(adj_hari+512-10); ii+=(512/5) )
	{
		int jj;
		int kk;
		for (jj=(0),kk=(0); jj<(256); jj+=(16),kk+=((512/16)/5) )
		{
			send1_obj->x256 = (src__x256);/* ‰EΪ“®’†‚Ν‰E‘¤‚©‚η‚‚Β */
			send1_obj->y256 = (src->y256);		/* ’eΉy256 */
			send1_obj->BULLET_REGIST_speed256			= (int)(speed256 + sin512(jj)); 	/* ’e‘¬ */
			send1_obj->BULLET_REGIST_angle512			= ((ii)+(kk));						/* p“x */
		//	send1_obj->BULLET_REGIST_div_angle512		= (0);								/* ƒ_ƒ~[p“x(–Άg—p) */
			send1_obj->BULLET_REGIST_bullet_obj_type	= BULLET_HARI32_00_AOI; 			/* […Fj’e] */	/*hari_no_iro*/
			send1_obj->BULLET_REGIST_n_way				= (1);								/* [1way] */
			bullet_regist_basic();
		}
	}
}

/*---------------------------------------------------------
	–£–‚A‰Τ‚Δ‚·‚Ζ’e–‹
	-------------------------------------------------------
	••–‚\‚Ε‚Ν4•ϋόƒoƒ‰ƒ}ƒL‚Α‚Ϋ‚Ά‚Μ‚π‚‚Α‚Δ‚­‚ι‚ªA
	ƒAƒƒ“ƒW‚µ‚Δ5•ϋόB‰Τ‰f’Λ‚Α‚Ϋ‚Ά“z‚ΙB
	-------------------------------------------------------
	ƒeƒLƒg[
-dd 1 0000 0x10 == (xxx)&0x1f
-11 1 0000 luna
-10 1 0000 luna hard
-01 1 0000 luna hard norm
-00 1 0000 luna hard norm easy
---------------------------------------------------------*/
static void danmaku_create_12_hana_test(SPRITE *src)
{
//	if (0x40==((src->boss_base_danmaku_time_out)&0xcf))/* 4‰ρ */
//	if (0x10==((src->boss_base_danmaku_time_out)&0x1f))/* (16‰ρ‚Ι1‰ρ)(128‚Θ‚ηv8‰ρ) */
	if (0x10==((src->boss_base_danmaku_time_out)&0x1f))/* (16‰ρ‚Ι1‰ρ)(128‚Θ‚ηv4‰ρ) */
	{
		if (difficulty >= ((src->boss_base_danmaku_time_out)>>5))
		{
			#if (1)
		//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		//	bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
			bullet_play_04_auto(VOICE11_BOSS_KIRARIN);
		//	voice_play(VOICE11_BOSS_KIRARIN, TRACK04_TEKIDAN);/*ƒeƒLƒg[*/
			#endif
		//
			enemy_boss01_nway_fire(src);/*enemy_boss04_fire(2);*/
		}
	}
//	else
//	if (0x00==((src->boss_base_danmaku_time_out)&0x1f))/* (16‰ρ‚Ι1‰ρ)(128‚Θ‚ηv4‰ρ) */
	{
		/* ‚±‚±‚Ε‰Ί•ϋ‚¤‚Λ‚θ4•ϋόAd—Ν’e‚Α‚Ϋ‚Ά“z(—\’θ) */
	}
}


/*---------------------------------------------------------
	ƒAƒƒX(‰Ό)	ƒAƒƒXl`’e–‹
	-------------------------------------------------------
	ƒeƒLƒg[
---------------------------------------------------------*/
extern void add_zako_alice_doll(SPRITE *src);/* ƒAƒƒXl`’e–‹ */
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

/*---------------------------------------------------------
	g2–Κƒ{ƒX ƒ`ƒ‹ƒm	’ΚνU‚1(1/3)	©‹@‘_‚ΆgU’e(1-2-3-4-5-6-7-8’e)
	-------------------------------------------------------
	(1-2-3-4-5-6-7’e)‚©‚Ζv‚Α‚Δ‚½‚―‚ΗA΄μ‚β‚θ’Ό‚µ‚½‚ηA
	(1-2-3-4-5-6-7-8’e)‚Ύ‚Α‚½‚Μ‚ΕC³B
‚¤[‚ρ
	΄μ‚β‚θ’Ό‚µ‚Δ‚ζ‚­©‚½‚ηA
	(1-2-3-4-5-6-7’e)‚© 			(easy‚Ζ‚©)
	(2-3-4-5-6-7-8’e)‚Θ‚ρ‚Ε‚·‚ΛB	(luna‚Ζ‚©)
---------------------------------------------------------*/
static void danmaku_create_09_cirno_misogi(SPRITE *src)
{

	if (128==((src->boss_base_danmaku_time_out)))
	{
		tmp_angle_jikinerai512(player, src);/*©‹@‚Λ‚η‚Άpμ¬*/
	}
	else
	if (0x40==((src->boss_base_danmaku_time_out)&0xc7))/* 8‰ρ(C³1-2-3-4-5-6-7’e:0way‚Ν‚‚½‚Θ‚Ά‚Μ‚Ε‚‚Β‚Μ‚Ν7‰ρ) */
	{
		#if (1)
	//	voice_play(VOICE13_SAKUYA_SHOT02, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE13_SAKUYA_SHOT02);
		#endif
	//
		{
			send1_obj->x256 = (src->x256);		/* ’eΉx256 */
			send1_obj->y256 = (src->y256);		/* ’eΉy256 */
		//
			send1_obj->BULLET_REGIST_speed256			= (t256(2.5));				/* ’e‘¬ */
			send1_obj->BULLET_REGIST_angle512			= (src->tmp_angleCCW512);
			send1_obj->BULLET_REGIST_div_angle512		= (int)(512/(64));		/* p“x(512[360/360“x]‚π 64 •ª„) */	/* 1ό‚πn•ª„‚µ‚½p“x */
			send1_obj->BULLET_REGIST_bullet_obj_type	= BULLET_KOME_01_AOI;	/* [Β•Δ’e] ‚Ι‚Θ‚Α‚Δ‚ι‚Μ‚Ν»έ •X’e ‚ΜƒOƒ‰‚ª–³‚Ά‚©‚η */
			send1_obj->BULLET_REGIST_n_way				= ((/*16*/7-((src->boss_base_danmaku_time_out)>>3))&7)+(1);/* [nway] */
			bullet_regist_basic();
		}
	}
}

/*---------------------------------------------------------
	g2–Κƒ{ƒX ƒ`ƒ‹ƒm	’ΚνU‚1(2/3)	©‹@‘_‚Ά‘S•ϋΚ’e
	-------------------------------------------------------
---------------------------------------------------------*/
#if 0
#endif

/*---------------------------------------------------------
	g2–Κƒ{ƒX ƒ`ƒ‹ƒm	’ΚνU‚1(3/3)	©‹@‘_‚Άϋ‘©’e
	-------------------------------------------------------
---------------------------------------------------------*/
#if 0
#endif


/*---------------------------------------------------------
	g3–Κ’†ƒ{ƒX g”ό—ι
	’ΚνU‚1(1/2)
	16•ϋΚ•Ξ‚θ‘S•ϋΚ‚Ξ‚ηT‚«’e
	-------------------------------------------------------
---------------------------------------------------------*/
#if 0
#endif

/*---------------------------------------------------------
	g3–Κ’†ƒ{ƒX g”ό—ι
	‰Ψ•„u–F‰ΨΊΰ£v‚Ι‚Ώ‚ε‚Α‚Ζ‚Ύ‚―—‚½’e–‹(—\’θ)
	v‰ρ‚θA”½v‰ρ‚θ‚Μ6way—†ω’e(‰©F)		1‰ρ–‚Ι(512/(6*8))[512/512“x]‰ρ“]
	48•ϋΚ‘S•ϋΚ’e(ΤF)						8‰ρ–‚Ι”­’e
	-------------------------------------------------------
	–F‰ΨΊΰ£
	‰π‘“x‚Ζ‚©‘¬“x‚Ζ‚©‚Μ—vφ‚ΕA‚»‚Μ‚ά‚ά‚Ε‚Ν psp‚Ε“ο‚µ‚Ά‹C‚ΰ‚·‚ιB
	Lunatic ‚Ν‚±‚Μ‚ά‚ά‚ΕAHard Θ‰Ί‚ΝΧH‚µ‚ΔΥ‚µ‚­‚·‚ιB
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
		(4),				(8),				(5),				(6),					/* ’:3‚ζ‚θ4‚Μ•ϋ‚ªΘ’P */
		(short)(512/(4)),	(short)(512/(8)),	(short)(512/(5)),	(short)(512/(6)),		/* ’:3‚ζ‚θ4‚Μ•ϋ‚ªΘ’P */
		(short)(512/(4*8)), (short)(512/(8*8)), (short)(512/(5*8)), (short)(512/(6*8)), 	/* ’:3‚ζ‚θ4‚Μ•ϋ‚ªΘ’P */
//
		(4*8),				(4*8),				(5*8),				(6*8),					/* ’:3‚ζ‚θ4‚Μ•ϋ‚ªΘ’P */
		(short)(512/(4*8)), (short)(512/(4*8)), (short)(512/(5*8)), (short)(512/(6*8)), 	/* ’:3‚ζ‚θ4‚Μ•ϋ‚ªΘ’P */
	};
	static int aaa_angle512;

	//
			send1_obj->x256 = (src->x256);		/* ’eΉx256 */		/* ’eΉx256 */
			send1_obj->y256 = (src->y256);		/* ’eΉy256 */		/* ’eΉy256 */
//
//	if ((0x10)==((src->boss_base_danmaku_time_out)&0x1f))/* (16‰ρ‚Ι1‰ρ)(128‚Θ‚ηv8‰ρ) */
	if ((0x40)==((src->boss_base_danmaku_time_out)&0x7f))/* (16‰ρ‚Ι1‰ρ)(128‚Θ‚ηv8‰ρ) */
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		{
			int aaa_angle512;
			tmp_angle_jikinerai512(player, src);/*©‹@‘_‚Άpμ¬*/
			aaa_angle512=(src->tmp_angleCCW512);
			send1_obj->BULLET_REGIST_speed256			= (t256(3.5));										/* ’e‘¬ */	/*2.5 2.0*/
			send1_obj->BULLET_REGIST_angle512			= (aaa_angle512);									/* ©‹@‘_‚Ά’e */
			send1_obj->BULLET_REGIST_div_angle512		= houga_tbl[HOUGA_12_RED_DIV_ANGLE+difficulty]; 	/*(int)(512/(48))*/ 				/* p“x(512[360/360“x]‚π 48 •ª„) */	/* 1ό‚πn•ª„‚µ‚½p“x */
			send1_obj->BULLET_REGIST_bullet_obj_type	= BULLET_KOME_02_AKA;								/* [ΤF•Δ’e] */
			send1_obj->BULLET_REGIST_n_way				= houga_tbl[HOUGA_08_RED_NUMS+difficulty];			/*(48)*/								/* [48way] */	/* ”­’e” */
			bullet_regist_basic();
		}
	}
//	if ((0x02)==((src->boss_base_danmaku_time_out)&0x03))/* (2‰ρ‚Ι1‰ρ)(8‰ρ–‚Ι”­’e) */
	if ((0x08)==((src->boss_base_danmaku_time_out)&0x0f))/* (2‰ρ‚Ι1‰ρ)(8‰ρ–‚Ι”­’e) */
	{	// ‡‰ρ‚θ(‰ΊCCW‚Ύ‚©‚ηA¶‰ρ‚θCCW)
		send1_obj->BULLET_REGIST_speed256			= (t256(3.5));										/* ’e‘¬ */		/*2.5 2.0*/
		send1_obj->BULLET_REGIST_angle512			= ((/*0+*/(aaa_angle512))&(512-1)); 				/* ”­Λ’†Sp“x / “Ακ‹@”\(©‹@‘_‚Ά/‘Ό) */
		send1_obj->BULLET_REGIST_div_angle512		= houga_tbl[HOUGA_04_YEL_DIV_ANGLE+difficulty]; 	/*(int)(512/(6))*/					/* p“x(512[360/360“x]‚π 6 •ª„) */ 	/* 1ό‚πn•ª„‚µ‚½p“x */
		send1_obj->BULLET_REGIST_bullet_obj_type	= BULLET_KOME_05_KIIRO; 							/* [‰©F•Δ’e] */
		send1_obj->BULLET_REGIST_n_way				= houga_tbl[HOUGA_00_YEL_NUMS+difficulty];			/*(6)*/ 								/* [1way] */	/* ”­’e” */
		bullet_regist_basic();
		// ‹t‰ρ‚θ(‰ΊCCW‚Ύ‚©‚ηA‰E‰ρ‚θCW)
		send1_obj->BULLET_REGIST_speed256			= (t256(3.5));										/* ’e‘¬ */		/*2.5 2.0*/
		send1_obj->BULLET_REGIST_angle512			= ((512-(aaa_angle512))&(512-1));					/* ”­Λ’†Sp“x / “Ακ‹@”\(©‹@‘_‚Ά/‘Ό) */
		send1_obj->BULLET_REGIST_div_angle512		= houga_tbl[HOUGA_04_YEL_DIV_ANGLE+difficulty]; 	/*(int)(512/(6))*/					/* p“x(512[360/360“x]‚π 6 •ª„) */ 	/* 1ό‚πn•ª„‚µ‚½p“x */
		send1_obj->BULLET_REGIST_bullet_obj_type	= BULLET_KOME_05_KIIRO; 							/* [‰©F•Δ’e] */
		send1_obj->BULLET_REGIST_n_way				= houga_tbl[HOUGA_00_YEL_NUMS+difficulty];			/*(6)*/ 								/* [1way] */	/* ”­’e” */
		bullet_regist_basic();
		// ‰ρ“]—Κ
		aaa_angle512 += houga_tbl[HOUGA_16_YEL_ROTATE_ANGLE+difficulty];	/*(512/(6*8))*/ 				/* p“x(512[360/360“x]‚π 48•ª„) */
	}
}
#endif

/*---------------------------------------------------------
	g1–Κƒ{ƒX ƒ‹[ƒ~ƒA –ι•„uƒiƒCƒgƒo[ƒhv‚Ι‚Ώ‚ε‚Α‚Ζ‚Ύ‚―—‚½’e–‹(—\’θ)
	-------------------------------------------------------
	5’e–Ϊ‚ª©‹@‘_‚Ά‚Μ 32•ª„16•ϋό ‹|σ’eB
	-------------------------------------------------------
0000 0000 0x00==(time_out)&0x11 Β(0x00==((time_out)&0x20))
0001 0000						Β(0x00==((time_out)&0x20))
0010 0000 0x00==(time_out)&0x11 —Ξ
0011 0000						—Ξ
0100 0000 0x00==(time_out)&0x11 Β(0x00==((time_out)&0x20))
0101 0000						Β(0x00==((time_out)&0x20))
0110 0000 0x00==(time_out)&0x11 —Ξ
0111 0000						—Ξ
//
1000 0000 start
---------------------------------------------------------*/
static void danmaku_create_04_night_bird(SPRITE *src)
{
	static int aaa_angle512;

//	if (128==((src->boss_base_danmaku_time_out)))
	if (0x10==((src->boss_base_danmaku_time_out)&0x10))
	{
		tmp_angle_jikinerai512(player, src);/*©‹@‚Λ‚η‚Άpμ¬*/
		aaa_angle512 = src->tmp_angleCCW512;/*©‹@‚Λ‚η‚Άp*/
		aaa_angle512 -= (int)(512*(5)/(32));/* 5’e–Ϊ‚ª©‹@‘_‚Ά */
		/* p“x(512[360/360“x]‚π 32•ª„) */
	}
	else
	if (0x00==((src->boss_base_danmaku_time_out)&0x10))/* 16‰ρ */
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE13_SAKUYA_SHOT02/*VOICE14_BOSS_KOUGEKI_01*/);
		#endif
		send1_obj->y256 = (src->y256);		/* ’eΉy256 */	/* ”­’eΚ’u‚Μ offset —p */
		{
			int iii;
				iii = (src->boss_base_danmaku_time_out);
			int aaa;/* ’eF */
			int bbb;/* •ϋό */
			if (0x00==((src->boss_base_danmaku_time_out)&0x20))
					{	bbb=-1;aaa=1;/* g‚Β‚«Β’e */}
			else	{	bbb= 1;aaa=0;/* g‚Β‚«—Ξ’e */}
		//	send1_obj->x256 = src->x256+((16-(iii&0x0f))<<(3+8))*bbb;/* –{•¨‚Ν”­’eΚ’u‚Μ x offset ‚ ‚θ */
			send1_obj->x256 = src->x256+((16-(iii&0x0f))<<(3+7))*bbb;/* –{•¨‚Ν”­’eΚ’u‚Μ x offset ‚ ‚θ */
			send1_obj->BULLET_REGIST_speed256			= (t256(1.5));								/* ’e‘¬ */
			send1_obj->BULLET_REGIST_angle512			= ((/*0+*/(aaa_angle512)*bbb)&(512-1)); 	/* ”­Λ’†Sp“x / “Ακ‹@”\(©‹@‘_‚Ά/‘Ό) */
	//		send1_obj->BULLET_REGIST_div_angle512		= (0);										/* ƒ_ƒ~[p“x(–Άg—p) */
			send1_obj->BULLET_REGIST_bullet_obj_type	= BULLET_MARU12_02_MIDORI+aaa;				/* [g‚Β‚«—Ξ’e]  [g‚Β‚«Β’e] */	/*BULLET_MARU12_03_AOI*/
			send1_obj->BULLET_REGIST_n_way				= (1);										/* [1way] */
			bullet_regist_basic();
		}
		/* ‚Μ’e‚Μp“x */
		aaa_angle512 += (int)(512/(32));/* p“x(512[360/360“x]‚π 32•ª„) */
	}
}

/*---------------------------------------------------------
	·•ª‚Μ—d‰φ1(“V‹η—l?)‚ª‚‚Β’e–‹(—\’θ)
	-------------------------------------------------------
	–Άμ¬
---------------------------------------------------------*/
static void danmaku_create_10_tengu_shot(SPRITE *src)
{
	static int aaa_angle512;
	static int data_wait2_start;/* ”­’eΤustart */
	static int data_wait2;/* ”­’eΤu */

//	if (128==((src->boss_base_danmaku_time_out)))
	if (0x10==((src->boss_base_danmaku_time_out)&0x10))
	{
		data_wait2_start = (20-((difficulty)<<2))/*8*/ /*10*/;
		data_wait2 = 10;
		tmp_angle_jikinerai512(player, src);/*©‹@‚Λ‚η‚Άpμ¬*/
		aaa_angle512 = src->tmp_angleCCW512;/*©‹@‚Λ‚η‚Άp*/
		aaa_angle512 -= (int)(512*(5)/(32));/* 5’e–Ϊ‚ª©‹@‘_‚Ά */
		/* p“x(512[360/360“x]‚π 32•ª„) */
	}
	else
//	if (0x00==((src->boss_base_danmaku_time_out)&0x10))/* 16‰ρ */
	{
		data_wait2--;
		if (1 > data_wait2)
		{
			data_wait2 = data_wait2_start;
			#if (1)
		//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE13_SAKUYA_SHOT02/*VOICE14_BOSS_KOUGEKI_01*/);
			#endif
			send1_obj->y256 = (src->y256);		/* ’eΉy256 */	/* ”­’eΚ’u‚Μ offset —p */
			{
			int iii;
				iii = (src->boss_base_danmaku_time_out);
			int aaa;/* ’eF */
			int bbb;/* •ϋό */
				if (0x00==((src->boss_base_danmaku_time_out)&0x20))
						{	bbb=-1;aaa=1;/* g‚Β‚«Β’e */}
				else	{	bbb= 1;aaa=0;/* g‚Β‚«—Ξ’e */}
			//	send1_obj->x256 = src->x256+((16-(iii&0x0f))<<(3+8))*bbb;/* –{•¨‚Ν”­’eΚ’u‚Μ x offset ‚ ‚θ */
				send1_obj->x256 = src->x256+((16-(iii&0x0f))<<(3+7))*bbb;/* –{•¨‚Ν”­’eΚ’u‚Μ x offset ‚ ‚θ */
				send1_obj->BULLET_REGIST_speed256			= (t256(1.5));								/* ’e‘¬ */
				send1_obj->BULLET_REGIST_angle512			= ((/*0+*/(aaa_angle512)*bbb)&(512-1)); 	/* ’eΉp“x512 */
		//		send1_obj->BULLET_REGIST_div_angle512		= (0);										/* ƒ_ƒ~[p“x(–Άg—p) */
				send1_obj->BULLET_REGIST_bullet_obj_type	= BULLET_MARU12_02_MIDORI+aaa;				/* [g‚Β‚«—Ξ’e]  [g‚Β‚«Β’e] */	/*BULLET_MARU12_03_AOI*/
				send1_obj->BULLET_REGIST_n_way				= (1);										/* [1way] */
				bullet_regist_basic();
			}
			/* ‚Μ’e‚Μp“x */
			aaa_angle512 += (int)(512/(32));/* p“x(512[360/360“x]‚π 32•ª„) */
		}
	}
}

/*---------------------------------------------------------
	•¶‚Ι”ό—ι‚Α‚Ϋ‚Ά’e–‹‚‚½‚Ή‚Δ‚έ‚ιƒeƒXƒg(‚Μ”¤‚Ύ‚Α‚½‚ρ‚Ύ‚―‚ΗA
	‰½Μ‚©“–‰‚Μ•ϋό‚Ζ‚Να‚¤•ϋό‚Ιcc)
	-------------------------------------------------------
---------------------------------------------------------*/
#if 1
extern void bullet_create_aya_ice(SPRITE *src);

static void danmaku_create_12_aya_merin_test(SPRITE *src)
{
	if (0x10==((src->boss_base_danmaku_time_out)&0x1f))/* (16‰ρ‚Ι1‰ρ)(128‚Θ‚ηv8‰ρ) */
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		bullet_create_aya_ice(src);
	}
}
#endif

/*---------------------------------------------------------
	‹¤’Κ’e–‹¶¬•”
	-------------------------------------------------------

---------------------------------------------------------*/
/*static*/ void danmaku_generator(SPRITE *src)
{
	if (DANMAKU_00 != src->boss_base_danmaku_type)	/* ’e–‹¶¬‚Ν•K—vH */
	{
		src->boss_base_danmaku_time_out--;			/* Τo‰ί‚·‚ι  */ 	/*fps_factor*/
		if (0 < src->boss_base_danmaku_time_out)	/* ΤΨ‚κH */
		{
			/* ’e–‹¶¬’† */
			/*const*/static void (*danmaku_create_bbb[(DANMAKU_MAX/*16+1*/)])(SPRITE *sss) =
			{
				#define danmaku_create_99_mitei danmaku_create_09_cirno_misogi
				NULL,								/* 00 */	/* ’e–‹¶¬‚µ‚Θ‚Ά */
			//													/* [η–ι] */
				danmaku_create_01_sakuya_misogi,	/* 01 */	/* η–ιβS’e–‹ / g5–Κ’†ƒ{ƒX η–ι u’ΚνU‚v */
				danmaku_create_02_24nerai,			/* 02 */	/* η–ι24’e–‹ (οpuƒ~ƒXƒfƒBƒƒNƒVƒ‡ƒ“‚ΰ‚Η‚«(1/2)v) */
				danmaku_create_03_11nife,			/* 03 */	/* η–ι11’e–‹ (οpuƒ~ƒXƒfƒBƒƒNƒVƒ‡ƒ“‚ΰ‚Η‚«(2/2)v) */
				danmaku_create_04_pink_hearts,		/* 04 */	/* g5–Κƒ{ƒX η–ι u’ΚνU‚1(1/2)v‚Ι‚Ώ‚ε‚Α‚Ζ‚Ύ‚―—‚½’e–‹(—\’θ) */
				danmaku_create_05_32way_dual,		/* 05 */	/* g5–Κƒ{ƒX η–ι u’ΚνU‚1/2(2/2)v‚Ι‚Ώ‚ε‚Α‚Ζ‚Ύ‚―—‚½’e–‹(—\’θ) */
				danmaku_create_06_luna_clock_32way, /* 06 */	/* g5–Κƒ{ƒX η–ι ¶Ϋuƒ‹ƒiƒNƒƒbƒN(1/2)v‚Ι‚Ώ‚ε‚Α‚Ζ‚Ύ‚―—‚½’e–‹(—\’θ) */
				danmaku_create_07_80way_dual_five,	/* 07 */	/* g5–Κƒ{ƒX η–ι u’ΚνU‚3(1/2)v‚Ι‚Ώ‚ε‚Α‚Ζ‚Ύ‚―—‚½’e–‹(—\’θ) */
			//													/* [ƒAƒƒX(‰Ό)] */
				danmaku_create_04_night_bird,		/* 08 */	/* g1–Κƒ{ƒX ƒ‹[ƒ~ƒA –ι•„uƒiƒCƒgƒo[ƒhv‚Ι‚Ώ‚ε‚Α‚Ζ‚Ύ‚―—‚½’e–‹(—\’θ) */
				danmaku_create_08_sentakki, 		/* 09 */	/* τ‘σ‹@’e–‹ */
				danmaku_create_11_houka_kenran, 	/* 0a */	/* g”ό—ι ‰Ψ•„u–F‰ΨΊΰ£v‚Ι‚Ώ‚ε‚Α‚Ζ‚Ύ‚―—‚½’e–‹(—\’θ) */
				danmaku_create_0b_alice_doll,		/* 0b */	/* ƒAƒƒXl`’e–‹ */
				danmaku_create_12_hana_test,		/* 0c */	/* –£–‚A‰Τ‚Δ‚·‚Ζ’e–‹ */
				danmaku_create_99_mitei,			/* 0d */	/* –Ά’θ’e–‹ */
				danmaku_create_99_mitei,			/* 0e */	/* –Ά’θ’e–‹ */
				danmaku_create_99_mitei,			/* 0f */	/* –Ά’θ’e–‹ */
			//													/* [ƒ`ƒ‹ƒm(‰Ό)] */
				danmaku_create_09_cirno_misogi, 	/* 10 */	/* ƒ`ƒ‹ƒmβS’e–‹ */
				danmaku_create_10_tengu_shot,		/* 11 */	/* ·•ª‚Μ—d‰φ1(“V‹η—l?)‚ª‚‚Β’e–‹(—\’θ) */
				danmaku_create_12_aya_merin_test,	/* 12 */	/* •¶‚Ι”ό—ι‚Α‚Ϋ‚Ά’e–‹‚‚½‚Ή‚Δ‚έ‚ιƒeƒXƒg */
				danmaku_create_99_mitei,			/* 13 */	/* –Ά’θ’e–‹ */
				danmaku_create_99_mitei,			/* 14 */	/* –Ά’θ’e–‹ */
				danmaku_create_99_mitei,			/* 15 */	/* –Ά’θ’e–‹ */
				danmaku_create_99_mitei,			/* 16 */	/* –Ά’θ’e–‹ */
				danmaku_create_99_mitei,			/* 17 */	/* –Ά’θ’e–‹ */
			//
				danmaku_create_99_mitei,			/* 18 */	/* –Ά’θ’e–‹ */
				danmaku_create_99_mitei,			/* 19 */	/* –Ά’θ’e–‹ */
				danmaku_create_99_mitei,			/* 1a */	/* –Ά’θ’e–‹ */
				danmaku_create_99_mitei,			/* 1b */	/* –Ά’θ’e–‹ */
				danmaku_create_99_mitei,			/* 1c */	/* –Ά’θ’e–‹ */
				danmaku_create_99_mitei,			/* 1d */	/* –Ά’θ’e–‹ */
				danmaku_create_99_mitei,			/* 1e */	/* –Ά’θ’e–‹ */
				danmaku_create_99_mitei,			/* 1f */	/* –Ά’θ’e–‹ */
			};
			(danmaku_create_bbb[(src->boss_base_danmaku_type/*-1*/)])(src);
		}
		else
		{
			src->boss_base_danmaku_type = DANMAKU_00;			/* ’e–‹¶¬I—Ή */
		}
	}
}
