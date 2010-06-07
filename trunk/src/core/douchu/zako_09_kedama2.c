
#include "douchu.h"

/*---------------------------------------------------------
	“Œ•û–Í•í•—  ` Toho Imitation Style.
	ƒvƒƒWƒFƒNƒgƒy[ƒW http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
		"–Ñ‹Ê2",	"BADGUY",	(r27‚©‚çV‹K’Ç‰Á)
	-------------------------------------------------------
	–{‰Æ–Ñ‹Ê
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
//#define speed256	user_data06

#define time_out	user_data03
#define state		user_data04

#define kougeki_type		user_data05

#define USE_X_HOUKOU (1)

/*---------------------------------------------------------
	“GˆÚ“®
---------------------------------------------------------*/
enum
{
	ST00 = 0,
	ST02,
	ST03,
};

static void move_kedama2(SPRITE *src)
{
	src->time_out--;/*fps_factor*/
//
	switch (src->state)
	{
	case ST00:	/* ‚Î‚ç‚Î‚ç‚É“oê‚³‚¹‚é */
		if (
			(0 >= src->x256)
			|| (t256(GAME_WIDTH) < src->x256)
			#if (1==USE_X_HOUKOU)
			|| (0 >= src->y256)
			|| (t256(GAME_HEIGHT) < src->y256)
			#endif
		)
		{
			/*‰æ–ÊŠO*/;
		}
		else
		{
			src->time_out	= 64;
			src->state++;	/* “oê‚·‚é */
		}
		break;
	case ST02:	/* UŒ‚ */
		{	enum
			{
				K00_KOUGEKI_KANKAKU_MASK = 0,
				K01_SUKIMA_KAKUDO,
				K02_TAMA_KAZU,
				K99_MAX/* Å‘å” */
			};
			const u16 kougeki_tbl[(4*2)][K99_MAX] =
			{
				/* 0:g‚Á‚Û‚¢–Ñ‹Ê(‹·Šp“x Šï”’e) */
				{0x3f, (u16)(1024/12), (5)},	/* (u32)(1024/12):LŠp‚È‚Ì‚ÅŠÈ’P */
				{0x1f, (u16)(1024/16), (3)},	/* 0x0f:•’Ê‚É”ð‚¯‚ê‚éŠÔŠu */
				{0x0f, (u16)(1024/20), (3)},	/* 0x0f:•’Ê‚É”ð‚¯‚ê‚éŠÔŠu */
				{0x07, (u16)(1024/24), (5)},	/* (u32)(1024/24):•’Ê‚Ì‹·Šp */
				/* 1:—d‚Á‚Û‚¢–Ñ‹Ê(90“x 4•ûŒü) (–³‘Ê’e‚È‚Ì‚Å“ïˆÕ“x‚Í’á‚¢”¤) */
				{0x3f, (u16)(1024/8),  (8)}, 	/* 8:Œ©‚¹’e(–³‘Ê’e‚È‚Ì‚Å“ïˆÕ“x‚Í’á‚¢”¤) */
				{0x1f, (u16)(1024/8),  (8)}, 	/* 0x1f:•’Ê‚É”ð‚¯‚ê‚éŠÔŠu */
				{0x07, (u16)(1024/4),  (4)},
				{0x03, (u16)(1024/4),  (4)}, 	/* 0x03:˜A‘±’e‚Á‚Û‚­ */
			};
			if (0==(src->time_out & kougeki_tbl[src->kougeki_type][K00_KOUGEKI_KANKAKU_MASK]))/*0x0f*/
			{
				obj_send1->x256 = src->x256;
				obj_send1->y256 = src->y256;
				br.BULLET_REGIST_speed256			= (t256(2.5)+((difficulty)<<6));/* [2.5 - 3.25] */		//	(t256(2.0)+((difficulty)<<6)),/* [2.0 - 2.75] */
				br.BULLET_REGIST_angle1024			= ANGLE_JIKI_NERAI_DAN;
				br.BULLET_REGIST_div_angle1024		= kougeki_tbl[src->kougeki_type][K01_SUKIMA_KAKUDO];	//	(int)(1024/24),
				br.BULLET_REGIST_bullet_obj_type	= BULLET_KOME_01_AOI+(src->x256&0x07);
				br.BULLET_REGIST_n_way				= kougeki_tbl[src->kougeki_type][K02_TAMA_KAZU]; 		//	(3/*+difficulty+difficulty*/)
				br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
				bullet_regist_vector();
			}
		}
		if (0 > src->time_out)
		{
			src->state++;
		}
		break;
	case ST03:
		gamen_gai_nara_zako_osimai(src);/* ‰æ–ÊŠO‚È‚ç‚¨‚µ‚Ü‚¢ */
		break;
	}

	/* ˆÚ“®‚·‚é */
	src->x256 += (src->vx256);
	src->y256 += (src->vy256);
//
	src->m_angleCCW1024 += 10;
	mask1024(src->m_angleCCW1024);
}

/*---------------------------------------------------------
	“G‚ð’Ç‰Á‚·‚é
---------------------------------------------------------*/

global void add_zako_kedama2(STAGE_DATA *l)
{
	SPRITE *h;
	h							= sprite_add_gu_error();
	if (NULL!=h)/* “o˜^‚Å‚«‚½ê‡‚Ì‚Ý */
	{
		add_zako_common(l, h);
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
		h->type 				= TEKI_59_HAI_KEDAMA;
		h->color32				= MAKE32RGBA(0xff, 0xff, 0xff, 0xaa);		/*”’‚Á‚Û‚­ */
		h->callback_mover		= move_kedama2;
		//
		h->y256 				= ((l->user_y)<<8);
		h->vx256				= ((l->user_x));
		h->vy256				= (0);
		//
		{
			if (0 < (h->vx256))
			{/* [ˆø” user_y ‚ª³•ûŒü‚Ìê‡A‰E‚ÖˆÚ“®(¨)] [ˆø”y‚ª•‰•ûŒü‚Ìê‡A‰º‚ÖˆÚ“®(«)] */
				h->x256 			= t256(-30)/*-(i<<(5+8))*/;
			}
			else
			{/* [ˆø” user_y ‚ª³•ûŒü‚Ìê‡A¶‚ÖˆÚ“®(©)] [ˆø”y‚ª•‰•ûŒü‚Ìê‡Aã‚ÖˆÚ“®(ª)] */
				h->x256 			= t256(360)/*+(i<<(5+8))*/;/* 360 > 352(x_max) > 272(y_max) */
			}
		}
		//
		if (0 < (h->y256))
		{	;	}
		else
		{
			/* yÀ•W‚ª•‰•ûŒü‚Ìê‡‚ÍAxÀ•WŽw’è‚È‚Ì‚Å(xÀ•W yÀ•W ‚ð‹t‚É‚·‚é) */
			{
				int s_sss;
				s_sss				= h->x256;
				h->x256 			= -(h->y256);
				h->y256 			= s_sss;
			}
			h->vy256			= (h->vx256);
			h->vx256			= (0);
		}
		h->state			= ST00;
		h->time_out 		= 64/*+(i<<4)*/;
		h->kougeki_type 	= (((l->user_255_code)&1)<<2)|(difficulty/*&0x03*/);
	}
}
