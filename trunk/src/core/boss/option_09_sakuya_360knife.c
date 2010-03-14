
#include "bullet_object.h"

/*---------------------------------------------------------
	Žq‹Ÿ–‚•ûw ’e–‹
	-------------------------------------------------------
	360ƒiƒCƒt‚ðŒ‚‚Â‚º
	-------------------------------------------------------
	(–³‘Ê‚É•¡ŽG‚¾‚Á‚½‚Ì‚ÅA
		ƒŒ[ƒU[‚ðŒ‚‚Â‹@”\‚ÆA
		¯Œ^‚ð•`‚­‹@”\‚ÆA
		360ƒiƒCƒt‚ðŒ‚‚Â‹@”\‚ÆA
	‚ð•ª—£‚³‚¹‚½)
---------------------------------------------------------*/

#define time_out	user_data02 		/* Œo‰ßŽžŠÔ */


/*---------------------------------------------------------
	Žq‹Ÿ–‚•ûw ˆÚ“®
---------------------------------------------------------*/

static void move_doll03(SPRITE *src)
{
	#if 1
	/* –‚•ûwƒAƒjƒ[ƒVƒ‡ƒ“ */
	src->m_angleCCW512--;/* ‰E‰ñ‚è */
	mask512(src->m_angleCCW512);
	#endif
//
	src->time_out--;
	if ((64*8) < src->time_out)
	{
		if (0==((src->time_out)&(64-1)))/* 64‰ñ‚É1‰ñ”­’e‚·‚é */
		{
			static int count_jjj;
			count_jjj++;
			if ((3-difficulty) < count_jjj)
			{
				count_jjj = 0;
				/*---------------------------------------------------------
					360ƒiƒCƒt
					–‚•ûw—p
					‘S•ûŒü‚ÉÂƒiƒCƒt’e‚ðŒ‚‚Â
				---------------------------------------------------------*/
				send1_obj->x256 = (src->x256)+t256(8.0);/* 8.0[dots]‰E */
				send1_obj->y256 = (src->y256)+t256(4.0);/* 4.0[dots]‰º */
				send1_obj->BULLET_REGIST_speed256			=	(t256(1.2));				/*(speed256)*/
				send1_obj->BULLET_REGIST_angle512			=	(0);						/* ’eŒ¹Šp“x512 */	/*aaa_angle512*/
				send1_obj->BULLET_REGIST_div_angle512		=	(int)(512/(18));			/* •ªŠ„Šp“x([360/360]“x‚ð18•ªŠ„) */
				send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_KNIFE20_06_YUKARI;	/* [ƒsƒ“ƒNƒiƒCƒt’e] */	/*BULLET_KNIFE20_04_AOI*/
				send1_obj->BULLET_REGIST_n_way				=	(18);						/* [18way] */
				bullet_regist_basic();
			}
		}
	}
	#if 1
	else
	if (0 > src->time_out)
	{
		src->base_hp		= (0);/* “|‚µ‚½ */
		src->type = SP_DELETE;/* ‚¨‚µ‚Ü‚¢ */
	}
	#endif
//
	#if 0
	//if (data->c->type==SP_DELETE) 		/*‚±‚ê‚Å‚Í‚¤‚Ü‚­‚¢‚©‚È‚¢ê‡‚ª‚ ‚é*/
	if (0/*FLG_NO_DOLL*/==common_boss_flags)	/* ‹P–é‚ð“|‚·‚ÆŠF”j‰ó‚³‚ê‚éB */
	{
		src->base_hp		= (0);/* “|‚µ‚½ */
		src->type = SP_DELETE;
	}
	#endif
}


/*---------------------------------------------------------
	“G‚ð’Ç‰Á‚·‚é
---------------------------------------------------------*/

/*static*/ void add_zako_sakuya_doll_03_360knife(SPRITE *src)
{
	SPRITE *h;
	h							= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
//	h->type 					= TEKI_51_MAHOJIN1;
//	h->type 					= SP_MUTEKI;
	h->type 					= BOSS_16_YOUSEI11;
	h->base_hp					= (9999);/* “|‚¹‚È‚¢ */
	h->flags					= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
//	h->color32					= 0x00ffffff;	/*	obj_doll->alpha 		= 0x00;*/
	h->color32					= 0x80ff3388;	/*	obj_doll->alpha 		= 0x00;*/ /* g‚Á‚Û‚­ */
	h->callback_mover			= move_doll03;
	#if 1
	h->x256 					= src->x256;
	h->y256 					= src->y256-t256(16);/*ç–é ã•û‚É”z’u*/
	#endif
	h->time_out 				= (64*8)+(16*64);/* 16[’e]Œ‚‚Â */
}
