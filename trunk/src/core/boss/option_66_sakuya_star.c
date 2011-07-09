
#include "game_main.h"

/*---------------------------------------------------------
	“Œ•û–Í•í•— ` Toho Imitation Style.
	ƒvƒƒWƒFƒNƒgƒy[ƒW http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	Žq‹Ÿ–‚•ûw ’e–‹
	-------------------------------------------------------
	B.¯Œ^•`‚­‚º
	-------------------------------------------------------
	(–³‘Ê‚É•¡ŽG‚¾‚Á‚½‚Ì‚ÅA
		A.ƒŒ[ƒU[‚ðŒ‚‚Â‹@”\‚ÆA
		B.¯Œ^‚ð•`‚­‹@”\‚ÆA
		C.360ƒiƒCƒt‚ðŒ‚‚Â‹@”\‚ÆA
	‚ð•ª—£‚³‚¹‚½)
---------------------------------------------------------*/
#if 0/*ƒƒ‚*/
/* ƒ{ƒX‹¤’Ê‹KŠi */
	#define target_x256 			user_data00 	/* –Ú•WxÀ•W */
	#define target_y256 			user_data01 	/* –Ú•WyÀ•W */
	#define toutatu_wariai256		user_data02 	/* –Ú•WÀ•W‚Ö‚Ì“ž’BŠ„‡ */
	#define kougeki_anime_count 	user_data03 	/* UŒ‚ƒAƒjƒ[ƒVƒ‡ƒ“—pƒJƒEƒ“ƒ^ */
	#define boss_time_out			user_data04 	/* §ŒÀŽžŠÔ */
	#define boss_base_state777		user_data04 	/* §ŒÀŽžŠÔ(boss_time_out‚Æ“¯‚¶) */
//
	#define boss_spell_timer		user_data05 	/* ƒXƒyƒ‹ŽžŠÔ */
#endif
//	ƒ{ƒX‹¤’Ê‹KŠi‚Æ“¯‚¶(boss.hƒCƒ“ƒNƒ‹[ƒh‚µ‚Ä‚à‚µ‚È‚­‚Ä‚à‘Î‰ž)
#ifndef boss_time_out
	#define boss_time_out		user_data04 	/* §ŒÀŽžŠÔ */
#endif
//
#define my_angle1024		tmp_angleCCW1024	/* •ÛŽŠp“x[¯Œ^‚ð•`‚­ê‡‚ÉŽg‚¤Šp“x] */

#define doll_vx256			vx256
#define doll_vy256			vy256




//#include "game_main.h"

/*---------------------------------------------------------
	“Œ•û–Í•í•— ` Toho Imitation Style.
	ƒvƒƒWƒFƒNƒgƒy[ƒW http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	ç–é‚Ì		B.¯Œ^‚ð•`‚­–‚•ûw—p ’e
	-------------------------------------------------------
	’e–‹‘‚«Š·‚¦•ûŽ®‚ÉˆÚs‚·‚é‚Ì‚Å‚±‚±‚Í–³‚­‚È‚é—\’èB
	-------------------------------------------------------
	Todo:
	src->giji_jyumyou ‚ð”pŽ~‚µ‚Ä src->jyumyou ‚Å’u‚«Š·‚¦‚éB
---------------------------------------------------------*/


#define giji_jyumyou base_time_out

/* ‰ž—p’e */
#define BULLET_HOSI_GATA_DATA_angle1024 		user_data00
#define BULLET_HOSI_GATA_DATA_speed256			user_data01 		/* ‘¬“x */
#define BULLET_HOSI_GATA_DATA_add_delta256		user_data02 		/* ‰ÁŽZA‰Á‘¬“x */

#define HOSIGATA_DAN_LIMIT_01_512	(512+0)
static void move_bullet_sakuya_hosi_gata(SPRITE *src)
{
	src->giji_jyumyou--;/* ŽžŠÔŒo‰ß */
	if (HOSIGATA_DAN_LIMIT_01_512 > (src->giji_jyumyou))/* —\‚ßÝ’è‚³‚ê‚½ŽžŠÔ‚Å“®‚­ */
	{
		src->giji_jyumyou = HOSIGATA_DAN_LIMIT_01_512;
		src->BULLET_HOSI_GATA_DATA_speed256 += (src->BULLET_HOSI_GATA_DATA_add_delta256); //(4) (3.84) == t256(0.015);
		src->vx256 = ((sin1024((src->BULLET_HOSI_GATA_DATA_angle1024))*(src->BULLET_HOSI_GATA_DATA_speed256))>>8);/*fps_factor*/	/* CCW‚Ìê‡ */
		src->vy256 = ((cos1024((src->BULLET_HOSI_GATA_DATA_angle1024))*(src->BULLET_HOSI_GATA_DATA_speed256))>>8);/*fps_factor*/
	}
	//move_bullet_vector(src);
	src->cx256 += (src->vx256); 	/*fps_factor*/
	src->cy256 += (src->vy256); 	/*fps_factor*/
//	gamen_gai_nara_osimai(src);/* ‰æ–ÊŠO‚È‚ç‚¨‚µ‚Ü‚¢ */
}


static void bullet_crate_sakuya_hosi_gata(SPRITE *src)
{		/*ŠÛ’e‚W(Ô‚q‚f‚a—ÎŽá‰©Â)*/
	SPRITE *h;
	h					= obj_add_00_tama_error();
	if (NULL != h)
	{
		h->cx256			= src->cx256;
		h->cy256			= src->cy256;
		h->type 			= BULLET_MINI8_01_AKA+(0);/*S P_BULLET*/
		reflect_sprite_spec444(h, OBJ_BANK_SIZE_00_TAMA);
		h->m_Hit256R		= TAMA_ATARI_MARU16_PNG;
	//
		h->callback_mover	= move_bullet_sakuya_hosi_gata;
		h->vx256			= (0);
		h->vy256			= (0);
		h->giji_jyumyou 	= HOSIGATA_DAN_LIMIT_01_512 + (br.BULLET_REGIST_00_VECTOR_hosi_gata_time_out);
		h->BULLET_HOSI_GATA_DATA_angle1024			= (br.BULLET_REGIST_02_VECTOR_hosi_gata_angle1024);
		h->BULLET_HOSI_GATA_DATA_speed256			= t256(0);
		h->BULLET_HOSI_GATA_DATA_add_delta256		= (br.BULLET_REGIST_07_VECTOR_hosi_gata_add_speed256);
	}
}

/*---------------------------------------------------------
	Žq‹Ÿ–‚•ûw ˆÚ“®
---------------------------------------------------------*/

static SPRITE hosi_position_obj;/* ¯Œ^•`‚¢‚Ä‚éÀ•WˆÊ’u‚ð•ÛŽB */
static void move_sakuya_doll_03_star(SPRITE *src)
{
	check_boss_option_time_out(src);	/* ŽžŠÔŒo‰ß‚ÅI—¹Bƒ{ƒX‚ð“|‚·‚ÆŠF”j‰ó‚³‚ê‚éB */
//
	#if 1
	/* –‚•ûwƒAƒjƒ[ƒVƒ‡ƒ“ */
	src->rotationCCW1024--;/* ‰E‰ñ‚è */
	mask1024(src->rotationCCW1024);
	#endif
//
	if ((64*8) < src->boss_time_out)
	{
		/* ¯Œ^‚ð•`‚­‚æ */
		if (0==((src->boss_time_out)&0x0f))
		{
			src->my_angle1024 += (1024*2/5);
			mask1024(src->my_angle1024);
			#define DOLL_SPEED10		(4)/*8*/
			src->doll_vx256 = ((sin1024((src->my_angle1024))*/*t256*/(DOLL_SPEED10))/*>>8*/);/*fps_factor*/ 			/* CCW‚Ìê‡ */
			src->doll_vy256 = ((cos1024((src->my_angle1024))*/*t256*/(DOLL_SPEED10))/*>>8*/);/*fps_factor*/
			#undef DOLL_SPEED10
		}
		else
		{
			br.BULLET_REGIST_02_VECTOR_hosi_gata_angle1024		= ((src->my_angle1024))+(((src->boss_time_out)&0x0f)<<4/*3*/);
			br.BULLET_REGIST_00_VECTOR_hosi_gata_time_out		= ((src->boss_time_out)&0x1ff); //((src->boss_time_out)-(64*8))
			br.BULLET_REGIST_07_VECTOR_hosi_gata_add_speed256	= (((src->boss_time_out>>2)&0x3)+4);	/* ‰Á‰Á‘¬“x(‚©-‚©‚»‚­‚Ç) */
		//	br.BULLET_REGIST_07_VECTOR_hosi_gata_add_speed256	= (((src->boss_time_out )&0xf)|0x03);	/* ‰Á‰Á‘¬“x(‚©-‚©‚»‚­‚Ç) */
/*
0123 3333 4444	0+4
4567 7777 5555	1+4
89ab bbbb 6666	2+4
cdef ffff 7777	3+4
*/
			/* CCW‚Ìê‡ */
			bullet_crate_sakuya_hosi_gata(&hosi_position_obj);
		}
		// “®ì
		hosi_position_obj.cx256 += (src->doll_vx256);/*fps_factor*/
		hosi_position_obj.cy256 += (src->doll_vy256);/*fps_factor*/
	}
//
}


/*---------------------------------------------------------
	“G‚ð’Ç‰Á‚·‚é
---------------------------------------------------------*/

/*static*/ void add_zako_sakuya_doll_03_star(SPRITE *src)
{
	SPRITE *h;
	h								= obj_add_01_teki_error();
	if (NULL!=h)/* “o˜^‚Å‚«‚½ê‡‚Ì‚Ý */
	{
		h->m_Hit256R				= ZAKO_ATARI16_PNG;
	//	h->type 					= TEKI_51_MAHOJIN1;
		h->type 					= BOSS_16_YOUSEI11;
		h->base_hp					= (9999);/* “|‚¹‚È‚¢ */
		h->flags					= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/);
	//	h->color32					= MAKE32RGBA(0xff, 0xff, 0xff, 0x00);
	//	h->color32					= MAKE32RGBA(0x88, 0x33, 0xff, 0x80); /* g‚Á‚Û‚­ */
		h->color32					= MAKE32RGBA(0xff, 0x33, 0x66, 0x80); /* g‚Á‚Û‚­ */
		h->callback_mover			= move_sakuya_doll_03_star;
	//
		/* Žq‹Ÿ–‚•ûwA”z’uˆÊ’u */
		#if 1
		h->cx256					= (src->cx256);
		h->cy256					= (src->cy256)-t256(16);/*ç–é ã•û‚É”z’u*/
		#endif
		/* ’e‚ðŒ‚‚¿Žn‚ß‚éˆÊ’u(¯Œ^•`‚«Žn‚ß‚éˆÊ’u) */
		hosi_position_obj.cx256 	= (src->cx256)+(ra_nd()&0xfff);
		hosi_position_obj.cy256 	= (src->cy256)+(ra_nd()&0xfff);/* ç–é ‰º•û‚©‚ç•`‚­ */
	//
		h->boss_time_out			= ((64*8)+(5*16)+1);	/* §ŒÀŽžŠÔ */

		/* ¯Œ^‚ð•`‚­€”õ */
		h->my_angle1024 			= (0);
	//	h->doll_vx256				= t256(1.0);	/*-t256(0.125)*/ /*t256(1.0)*/
	//	h->doll_vy256				= t256(0.0);	/* t256(2.0)*/ /*t256(0.0)*/
	}
}
