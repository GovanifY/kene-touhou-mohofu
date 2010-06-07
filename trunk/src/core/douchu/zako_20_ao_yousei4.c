
#include "douchu.h"

/*---------------------------------------------------------
	“Œ•û–Í•í•—  ` Toho Imitation Style.
	ƒvƒƒWƒFƒNƒgƒy[ƒW http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
		"Â—d¸4",(—\’è)		"FAIRY",	(r27‚©‚çV‹K’Ç‰Á)
	-------------------------------------------------------
	user_x: 	‘¬“x256[speed256]
	user_y: 	‰¡ˆÊ’u[dots]
---------------------------------------------------------*/
#if 0/* ‚ß‚à */
/* ƒ{ƒX‹¤’Ê‹KŠi */
	#define target_x256 		user_data00 	/* –Ú•WxÀ•W */
	#define target_y256 		user_data01 	/* –Ú•WyÀ•W */
	#define vvv256				user_data02 	/* –Ú•WÀ•W‚Ö‚Ì“ž’BŠ„‡ */
	#define time_out			user_data03 	/* §ŒÀŽžŠÔ */
#endif

//#define tx256 	user_data00
//#define ty256 	user_data01
#define time_out	user_data03
#define speed256	user_data04

/*---------------------------------------------------------
	“GˆÚ“®
---------------------------------------------------------*/

static void move_ao_yousei4(SPRITE *src)
{
	src->time_out--;
	if (0 > src->time_out)		/* —£’E */
	{
		src->speed256 += 2;/*(2+1)*/
		src->tmp_angleCCW1024 += (4);
		if (511 < src->tmp_angleCCW1024)
		{
			src->tmp_angleCCW1024 = (511);/*^ã*/
		}
		gamen_gai_nara_zako_osimai(src);/* ‰æ–ÊŠO‚È‚ç‚¨‚µ‚Ü‚¢ */
	}
	else	/* UŒ‚ */
	{
		if (50==src->time_out)
		{
			if (difficulty)
			{
				obj_send1->x256 					= src->x256;
				obj_send1->y256 					= src->y256;
				br.BULLET_REGIST_speed256			= (t256(2.0));								/* ’e‘¬ */	/*+((difficulty)<<6)*/
				br.BULLET_REGIST_angle1024			= ANGLE_JIKI_NERAI_DAN;
				br.BULLET_REGIST_div_angle1024		= (int)(1024/24);							/* •ªŠ„Šp“x */
				br.BULLET_REGIST_bullet_obj_type	= BULLET_KUNAI12_04_YUKARI+((ra_nd())&7);	/* [’eƒOƒ‰] */
				br.BULLET_REGIST_n_way				= (8);										/* 8way’e */
				br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
				bullet_regist_vector();
			}
		}
//		case 0x40:	/*not_break;*/	/*0x40==064 0x46==070*/
//		case 0x80:	/*not_break;*/	/*0x80==128 0x78==120*/
//		case 0xa0:	/*not_break;*/	/*0x  ==170 0xaa==170*/
//		case 0xc0:					/*0x  ==220 0xdc==220*/
		if (0x00==(src->time_out&(0x1f)))/* 8‰ñ */
		{
			if (difficulty)
			{
				obj_send1->x256 					= src->x256;
				obj_send1->y256 					= src->y256;
				br.BULLET_REGIST_speed256			= (t256(2.0));						/* ’e‘¬ */
				br.BULLET_REGIST_angle1024			= ANGLE_JIKI_NERAI_DAN;
				br.BULLET_REGIST_div_angle1024		= (int)(1024/24)-(difficulty);		/* •ªŠ„Šp“x */
				br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_00_AOI;			/* [Ô’e] */	//	BULLET_MARU8_00_AKA,
				br.BULLET_REGIST_n_way				= (difficulty); 					/* [1way] */	/*1*/
				br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
				bullet_regist_vector();
			}
		}
		src->speed256--;
		if (1 > src->speed256)
		{
			src->speed256 = 1;
		}
	}
	/*ˆÈ‰ºrwingx.c‚Æ“¯‚¶*/
	src->x256 += ((sin1024((src->tmp_angleCCW1024))*src->speed256)>>8);/*fps_factor*/ 	/* CCW‚Ìê‡ */
	src->y256 += ((cos1024((src->tmp_angleCCW1024))*src->speed256)>>8);/*fps_factor*/

//	src->yx_an im_frame = (8+((src->time_out>>2)&(8-1)));
//	src->yx_an im_frame = (0x20 | ((src->time_out)&(0x10)) | ((src->time_out>>2)&(4-1)));
/*
src->time_out		 ---a bc--
src->yx_an im_frame  yyyy xxxx
src->yx_an im_frame  --1a --bc
*/
	if (SP_DELETE != src->type)
	{
//		src->type				= (TEKI_12_YOUSEI1_1)+(0x08)+((src->time_out>>2)&(8-1));
		src->type				= (TEKI_12_YOUSEI1_1)+(0x04)+((src->time_out>>2)&(4-1));
	}
}

/*---------------------------------------------------------
	“G‚ð’Ç‰Á‚·‚é
---------------------------------------------------------*/

global void add_zako_ao_yousei4(STAGE_DATA *l)
{
	SPRITE *h;
	h							= sprite_add_gu_error();
	if (NULL!=h)/* “o˜^‚Å‚«‚½ê‡‚Ì‚Ý */
	{
		add_zako_common(l, h);
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
		h->type 				= TEKI_12_YOUSEI1_1;
		h->callback_mover		= move_ao_yousei4;
	//
		h->y256 				= t256(-30);
		h->x256 				= ((l->user_y)<<8);/*lv*t256(35)+t256(40)*/
		h->speed256 			= ((l->user_x));			/*t256(0.5)*/	/*+0.15*di fficulty*/ /*0.7+0.3*di fficulty*/

		h->tmp_angleCCW1024		= cv1024r(0);/* ^‰º */
		h->time_out 			= (320);
	}
}
