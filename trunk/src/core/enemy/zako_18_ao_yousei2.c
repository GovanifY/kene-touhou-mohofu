
#include "bullet_object.h"

/*---------------------------------------------------------
		"青妖精2",		"SPLASH",
	-------------------------------------------------------

---------------------------------------------------------*/

typedef struct
{
//	ENEMY_BASE base;
	int state;			/* 妖精の状態 */
	int enemy_rank; 	/* 設定ファイルからの敵の強さ */
	int time_out;		/* 状態遷移用の時間切れ */
	int nnn;			/* 弾を撃つ回数 */
//
	int ani_turn;		/* アニメーション方向(羽の上下) */
	int anime_frame;	/* アニメーション */
} AO_YOUSEI2_DATA;

/*---------------------------------------------------------
	敵やられ
---------------------------------------------------------*/

static void lose_ao_yousei2(SPRITE *src)
{
//	case SP_ZAKO_17_AO_YOUSEI2:
	item_create(src, ((SP_ITEM_05_TENSU&0xff)|SP_ITEM_00_P001), 1, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY)/*(up_flags)*/ );
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
static void move_ao_yousei2(SPRITE *src)
{
	AO_YOUSEI2_DATA *data = (AO_YOUSEI2_DATA *)src->data;
	int p512;
//
	data->time_out--;	/* 常に時間経過 */
	switch (data->state)
	{
	case STATE_00:	/* 登場 */
		if (data->time_out < 0)
		{
			data->time_out = 10;
			data->state++;// = STATE_01;/*次へ*/
		}
		else
		{
			/* 登場の動き */
			src->x256 += src->vx256;/**fps_fa ctor*/
			src->y256 += src->vy256;/**fps_fa ctor*/
		}
		break;
	case STATE_01:	/* 少し待つ */
		if (data->time_out < 0)
		{
			data->time_out = 50;
			data->state++;// = STATE_02;/*次へ*/
		}
		else
		{
			p512=atan_512(player->y256-src->y256,player->x256-src->x256);
			mask512(p512);
		//	const Uint8 aa_offs[8] = { OF_16, OF_12, OF_08, OF_04, OF_00, OF_04, OF_08, OF_12 };
			const Uint8 aa_offs[8] = { SSS00, SSS04, SSS08, SSS16, SSS20, SSS16, SSS08, SSS04 };
			data->anime_frame = (data->anime_frame&(4-1))+aa_offs[(p512>>6)];		/* 64  32== 512/16 */
		}
		break;
	case STATE_02:	/* 弾を撃つ */
		bullet_create_offset_dan_type000(src, /*((1+difficulty)<<7)*/t256(2.5)/*t256(3)*/, ANGLE_JIKI_NERAI_DAN/*, t256(0), t256(0)*/, BULLET_HARI32_00_AOI);
		bullet_create_offset_dan_type000(src, /*((1+difficulty)<<8)*/t256(3.5)/*t256(4)*/, ANGLE_JIKI_NERAI_DAN/*, t256(0), t256(0)*/, BULLET_HARI32_00_AOI);

		p512=atan_512(player->y256-src->y256+((player->h128)),player->x256-src->x256-((player->w128)));
		if (difficulty>0)
		{
		int k;
			for (k=0; k<4; k++)
			{
				/*bullet_create_maru8_frame*/bullet_create_offset_dan_type000(
					src,			/*	*/
					t256(2.5)+(k<<7),			/* 弾速 */
					((p512))+(1-((k&(2-1)/*%2*/)*2))*((512*1/12)),			/* 発射中心角度 / 特殊機能(自機狙い/他) */
					(BULLET_MARU8_00_AKA+(/*0*/7-k/*とりあえず*/))	/* 弾グラ */
				);
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
					/*bullet_create_maru8_frame*/bullet_create_offset_dan_type000(
						src,			/*	*/
						t256(1.5)+(k<<7),			/* 弾速 */
						(/**/(p512))+(1-((k&(2-1)/*%2*/)*2))*((512*1/24)),			/* 発射中心角度 / 特殊機能(自機狙い/他) */
						(BULLET_MARU8_00_AKA+(/*0*/3-k/*とりあえず*/))	/* 弾グラ */
					);
/*(1+(((k+difficulty)>>1)))*/  /*(2+((k>>1)))*/
				}
			}
		}

		data->nnn--;
		if (data->nnn < 0)
		{
			/* 退場準備 */
				 if (data->enemy_rank<3)	{	data->anime_frame=(data->anime_frame&(4-1))+SSS00;}
			else if (data->enemy_rank<7)	{	data->anime_frame=(data->anime_frame&(4-1))+SSS08;}
			else							{	data->anime_frame=(data->anime_frame&(4-1))+SSS00;}
			data->state++;// = STATE_03;/*次へ*/
			data->time_out = 50;
		}
		else
		{
			data->state--;// = STATE_01;/*戻る*/
		}
		break;
	case STATE_03:	/* 退場 */
		if (data->time_out < 0)
		{
			src->type = SP_DELETE;	/* おしまい */
		}
		else
		{
			/* 退場の動き */
			src->x256 -= src->vx256;/**fps_fa ctor*/
			src->y256 -= src->vy256;/**fps_fa ctor*/
		}
		break;
	}

	if (0==data->ani_turn)
	{
		data->anime_frame++;
		if (3==(data->anime_frame&(4-1)))
		{
			data->ani_turn=1;
		}
	}
	else
	{
		data->anime_frame--;
		if (0==(data->anime_frame&(4-1)))
		{
			data->ani_turn=0;
		}
	}
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/
	#if 0
//
//	if ( (enemy_rank)>9) {enemy_rank=9;}
	signed short spr_tbl[10][4] =
	{
{/* case 0: 	s->x=*/372 GAME_WIDTH352+20-80-12(280), /*s->y=*/100,		1,/*-s->w;*/		/*s->yx_an im_frame=*/SSS20},	//右下
{/* case 1: 	s->x=*/392 GAME_WIDTH352+40-80-12(300), /*s->y=*/70,		1,/*-s->w;*/		/*s->yx_an im_frame=*/SSS20},	//右中
{/* case 2: 	s->x=*/412 GAME_WIDTH352+60-80-12(320), /*s->y=*/40,		1,/*-s->w;*/		/*s->yx_an im_frame=*/SSS20},	//右上
//
{/* case 3: 	s->y=*/-30+80(50),			/*s->x=*/-300,		3,/*-s->w/2;*/		/*s->yx_an im_frame=*/SSS08},	//上右右
{/* case 4: 	s->y=*/-50+80(30),			/*s->x=*/-220,		3,/*-s->w/2;*/		/*s->yx_an im_frame=*/SSS08},	//上右
{/* case 5: 	s->y=*/-50+80(30),			/*s->x=*/-160,		3,/*-s->w/2;*/		/*s->yx_an im_frame=*/SSS08},	//上左
{/* case 6: 	s->y=*/-30+80(50),			/*s->x=*/ -80,		3,/*-s->w/2;*/		/*s->yx_an im_frame=*/SSS08},	//上左左
//
{/* case 7: 	s->x=*/-20+80(60),			/*s->y=*/40,		0,/*		*/		/*s->yx_an im_frame=*/SSS00},	//左上
{/* case 8: 	s->x=*/-40+80(40),			/*s->y=*/70,		0,/*		*/		/*s->yx_an im_frame=*/SSS00},	//左中
{/* case 9: 	s->x=*/-60+80(20),			/*s->y=*/100,		0,/*		*/		/*s->yx_an im_frame=*/SSS00},	//左下
	};
//	s->y256 				= (spr_tbl[enemy_rank][1]<<8);
//	s->x256 				= (spr_tbl[enemy_rank][0]<<8);
//		 if (1==spr_tbl[enemy_rank][2]) {s->x256 -= ((s->w128+s->w128));}
//	else if (3==spr_tbl[enemy_rank][2]) {s->x256 -= ((s->w128));}
	#endif

void add_zako_ao_yousei2(STAGE_DATA *l)/*int lv*/
{
	int enemy_rank; 	enemy_rank	= (l->user_1_moji&0x0f)/*user_y*/;
	SPRITE *s;
//	s						= sp rite_add_res(BASE_AO_YOUSEI24_PNG); //s->anim_speed=0;/*20"sp lash.png"*/
	s						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
	s->type 				= /*SP_ZAKO*/TEKI_16_YOUSEI11/*_17_AO_YOUSEI2*/;
	s->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	s->callback_mover		= move_ao_yousei2;
	s->callback_loser		= lose_ao_yousei2;
	s->callback_hit_enemy	= callback_hit_zako;
//	s->yx_an im_frame		= 0;// spr_tbl[enemy_rank][3];
	s->x256 				= ((l->user_x)<<8);
	s->y256 				= ((l->user_y)<<8);
//
	/* 登場の動き / 退場の動き */
		 if (/*data->*/enemy_rank<3)	{	s->vx256 = -t256(2.0);	}
//	else if (/*data->*/enemy_rank<7)	{	s->vx256 =	t256(2.0);	}
	else								{	s->vx256 =	t256(2.0);	}
	s->vy256 = (0);

	#if 1//(1==USE_X_HOUKOU)
	if (0 < (s->y256))
	{	;	}
	else
	{
		/* y座標が負方向の場合は、x座標指定なので(x座標 y座標 を逆にする) */
		{
			int s_sss;
			s_sss				= s->x256;
			s->x256 			= -(s->y256);
			s->y256 			= s_sss;
		}
		s->vy256			= (s->vx256);
		s->vx256			= (0);
//		s->AO_YOUSEI3_anime_houkou		= ((0x20)>>2);
	}
	#endif

	AO_YOUSEI2_DATA *data;
	data					= mmalloc(sizeof(AO_YOUSEI2_DATA));
	s->data 				= data;
	/*data->base.*/s->base_score		= score(50*2);
	/*data->base.*/s->base_health		= 20+(difficulty<<2);
	data->state 			= STATE_00;
	data->enemy_rank		= enemy_rank;
	data->time_out			= 40;
	data->nnn				= 3;	/* 3回撃つ */
//
	data->ani_turn			= 0;
	data->anime_frame		= 0;// spr_tbl[enemy_rank][3];

}
