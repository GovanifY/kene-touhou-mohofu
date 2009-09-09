
#include "bullet_object.h"

/*---------------------------------------------------------
		"青妖精2",		"SPLASH",
	-------------------------------------------------------

---------------------------------------------------------*/

typedef struct
{
	ENEMY_BASE base;
	int state;		/* 妖精の状態 */
	int level;		/* 設定ファイルからの妖精の強さ */
	int time_out;	/* 状態遷移用の時間切れ */
	int ani_turn;	/* アニメーション方向(羽の上下) */
	int nnn;		/* 弾を撃つ回数 */
} AO_YOUSEI2_DATA;

/*---------------------------------------------------------
	敵やられ
---------------------------------------------------------*/

static void lose_ao_yousei2(SPRITE *s)
{
//	case SP_ZAKO_17_AO_YOUSEI2:
	item_create(s, ((SP_ITEM_06_TENSU&0xff)|SP_ITEM_00_P001), 1, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY)/*(up_flags)*/ );
}

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/
enum
{
	STATE_00 = 0,
	STATE_01,
	STATE_02,
	STATE_03,
};

#define SSS00 ( 0<<2)/*右*/
#define SSS04 ( 4<<2)/*右斜め下*/
#define SSS08 ( 8<<2)/*下１*/
#define SSS12 (12<<2)/*下２*/
#define SSS16 (16<<2)/*左斜め下*/
#define SSS20 (20<<2)/*左*/
static void move_ao_yousei2(SPRITE *s)
{
	AO_YOUSEI2_DATA *d = (AO_YOUSEI2_DATA *)s->data;
	int p512;
//
	d->time_out--;	/* 常に時間経過 */
	switch (d->state)
	{
	case STATE_00:	/* 登場 */
		if (d->time_out < 0)
		{
			d->time_out = 10;
			d->state++;// = STATE_01;/*次へ*/
		}
		else
		{	/* 登場の動き */
				 if (d->level<3)	{	s->x256 -= t256(2)/**fps_fa ctor*/; 	}
			else if (d->level<7)	{	s->y256 += t256(2)/**fps_fa ctor*/; 	}
			else					{	s->x256 += t256(2)/**fps_fa ctor*/; 	}
		}
		break;
	case STATE_01:	/* 少し待つ */
		if (d->time_out < 0)
		{
			d->time_out = 50;
			d->state++;// = STATE_02;/*次へ*/
		}
		else
		{
			p512=atan_512(player->y256-s->y256,player->x256-s->x256);
			mask512(p512);
		//	const Uint8 aa_offs[8] = { OF_16, OF_12, OF_08, OF_04, OF_00, OF_04, OF_08, OF_12 };
			const Uint8 aa_offs[8] = { SSS00, SSS04, SSS08, SSS16, SSS20, SSS16, SSS08, SSS04 };
			s->yx_anim_frame = (s->yx_anim_frame&(4-1))+aa_offs[(p512>>6)]; 		/* 64  32== 512/16 */
		}
		break;
	case STATE_02:	/* 弾を撃つ */
		bullet_create_offset_dan_type(s, /*((1+difficulty)<<7)*/t256(2.5)/*t256(3)*/, ANGLE_JIKI_NERAI_DAN, t256(0), t256(0), BULLET_HARI32_00_AOI);
		bullet_create_offset_dan_type(s, /*((1+difficulty)<<8)*/t256(3.5)/*t256(4)*/, ANGLE_JIKI_NERAI_DAN, t256(0), t256(0), BULLET_HARI32_00_AOI);

		p512=atan_512(player->y256-s->y256+((player->h128)),player->x256-s->x256-((player->w128)));
		if (difficulty>0)
		{
		int k;
			for (k=0; k<4; k++)
			{
				bullet_create_maru8_frame(s, t256(2.5)+(k<<7), ((p512))+(1-((k&(2-1)/*%2*/)*2))*((512*1/12)), (/*0*/7-k/*とりあえず*/) );
/*(2+(((k+difficulty)>>1)))*/  /*(3+((k>>1)))*/

/*
  3+((k>>1))
0 4 3
1 4 4
2 4 4
3 4 5
*/
				if (difficulty > 1)
				{
					bullet_create_maru8_frame(s, t256(1.5)+(k<<7), (/**/(p512))+(1-((k&(2-1)/*%2*/)*2))*((512*1/24)), (/*0*/3-k/*とりあえず*/) );
/*(1+(((k+difficulty)>>1)))*/  /*(2+((k>>1)))*/
				}
			}
		}

		d->nnn--;
		if (d->nnn < 0)
		{
			/* 退場準備 */
				 if (d->level<3)	{	s->yx_anim_frame=(s->yx_anim_frame&(4-1))+SSS00;}
			else if (d->level<7)	{	s->yx_anim_frame=(s->yx_anim_frame&(4-1))+SSS08;}
			else					{	s->yx_anim_frame=(s->yx_anim_frame&(4-1))+SSS00;}
			d->state++;// = STATE_03;/*次へ*/
			d->time_out = 50;
		}
		else
		{
			d->state--;// = STATE_01;/*戻る*/
		}
		break;
	case STATE_03:	/* 退場 */
		if (d->time_out < 0)
		{
			s->type = SP_DELETE;
		}
		else
		{	/* 退場の動き */
				 if (d->level<3)	{	s->x256 += t256(2)/**fps_fa ctor*/;}
			else if (d->level<7)	{	s->y256 -= t256(2)/**fps_fa ctor*/;}
			else					{	s->x256 -= t256(2)/**fps_fa ctor*/;}
		}
		break;
	}

	if (0==d->ani_turn)
	{
		s->yx_anim_frame++;
		if (3==(s->yx_anim_frame&(4-1)))
		{
			d->ani_turn=1;
		}
	}
	else
	{
		s->yx_anim_frame--;
		if (0==(s->yx_anim_frame&(4-1)))
		{
			d->ani_turn=0;
		}
	}
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

void add_zako_ao_yousei2(STAGE_DATA *l)/*int lv*/
{
	int lv;
	lv	= l->user_y;
//
	if ( (lv)>9) {lv=9;}
	SPRITE *s;
	s					= sprite_add_res(BASE_AO_YOUSEI24_PNG); //s->anim_speed=0;/*20"sp lash.png"*/
	s->type 			= SP_ZAKO/*_17_AO_YOUSEI2*/;
	s->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	s->callback_mover	= move_ao_yousei2;
	s->callback_loser	= lose_ao_yousei2;
	signed short spr_tbl[10][4] =
	{
{/* case 0: 	s->x=*/GAME_WIDTH+20,	1,/*-s->w;*/		/*s->y=*/100,		/*s->yx_anim_frame=*/SSS20},	//右下
{/* case 1: 	s->x=*/GAME_WIDTH+40,	1,/*-s->w;*/		/*s->y=*/70,		/*s->yx_anim_frame=*/SSS20},	//右中
{/* case 2: 	s->x=*/GAME_WIDTH+60,	1,/*-s->w;*/		/*s->y=*/40,		/*s->yx_anim_frame=*/SSS20},	//右上
{/* case 3: 	s->x=*/300, 			3,/*-s->w/2;*/		/*s->y=*/-30,		/*s->yx_anim_frame=*/SSS08},	//上右右
{/* case 4: 	s->x=*/220, 			3,/*-s->w/2;*/		/*s->y=*/-50,		/*s->yx_anim_frame=*/SSS08},	//上右
{/* case 5: 	s->x=*/160, 			3,/*-s->w/2;*/		/*s->y=*/-50,		/*s->yx_anim_frame=*/SSS08},	//上左
{/* case 6: 	s->x=*/ 80, 			3,/*-s->w/2;*/		/*s->y=*/-30,		/*s->yx_anim_frame=*/SSS08},	//上左左
{/* case 7: 	s->x=*/-20, 			0,/*		*/		/*s->y=*/40,		/*s->yx_anim_frame=*/SSS00},	//左上
{/* case 8: 	s->x=*/-40, 			0,/*		*/		/*s->y=*/70,		/*s->yx_anim_frame=*/SSS00},	//左中
{/* case 9: 	s->x=*/-60, 			0,/*		*/		/*s->y=*/100,		/*s->yx_anim_frame=*/SSS00},	//左下
	};
	s->yx_anim_frame	=  spr_tbl[lv][3];
	s->y256 			= (spr_tbl[lv][2]<<8);
	s->x256 			= (spr_tbl[lv][0]<<8);
		 if (1==spr_tbl[lv][1]) {s->x256 -= ((s->w128+s->w128));}
	else if (3==spr_tbl[lv][1]) {s->x256 -= ((s->w128));}

	AO_YOUSEI2_DATA *data;
	data				= mmalloc(sizeof(AO_YOUSEI2_DATA));
	s->data 			= data;
	data->base.score	= score(50*2);
	data->base.health	= 20+(difficulty<<2);
	data->state 		= STATE_00;
	data->level 		= lv;
	data->time_out		= 40;
	data->ani_turn		= 0;
	data->nnn			= 3;	/* 3回撃つ */
}
