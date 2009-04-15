#include "enemy.h"

/*rwingx_cir*/

typedef struct
{
	ENEMY_BASE b;
	/*double*/int angle512;
	double speed;
	int state;
	int level;
//
} CIR_DATA;
/*
右側から二列縦隊で現れ、下に向かい。下で旋回。
中央から上昇し、右側に向かい。上で旋回。
右側から下に向かい。画面外へ退場。
*/
#define NUM_OF_ENEMIES (24)
static void enemy_cir_move(SPRITE *s)
{
	CIR_DATA *d=(CIR_DATA *)s->data;
	switch (d->state)
	{
	case 0: /* down */
		if (s->y>200)
		{
			d->state=1;
			d->speed=3;
		}
		break;
	case 1: /* turn */
		d->angle512 -= deg_360_to_512(2)/**fps_factor*/;/*簡略化の仕様上少し位置がずれる(※１)*/
		if (d->angle512 < deg_360_to_512(0))
		{	d->angle512 += deg_360_to_512(360);}
		if ((d->angle512 >= deg_360_to_512(267)) &&
			(d->angle512 <= deg_360_to_512(273)))
		{
			d->angle512= deg_360_to_512(270);
			d->state=2;
			d->speed=8;
		}
		break;
	case 2: /* up */
		if (s->y<50)
		{
			d->state=3;
			d->speed=3;
		}
		break;
	case 3: /* turn */
		d->angle512 -= deg_360_to_512(2)/**fps_factor*/;/*簡略化の仕様上少し位置がずれる(※１)*/
		if (d->angle512 < deg_360_to_512(0))
		{	d->angle512 += deg_360_to_512(360);}
		if ((d->angle512 >= deg_360_to_512(87)) &&
			(d->angle512 <= deg_360_to_512(93)))
		{
			d->angle512 = deg_360_to_512(90);
			d->state=4;
			d->speed=6;
		}
		break;
	case 4: /* down */
		if (s->y>GAME_HEIGHT) //denis 480
		{	s->type=SP_DELETE;}
		break;
	}
	/*以下rwingx.cと同じ*/
	s->x+=co_s512((d->angle512))*d->speed*fps_factor;
	s->y+=si_n512((d->angle512))*d->speed*fps_factor;
//	s->aktframe=(deg_512_to_360(d->angle512+deg_360_to_512(270))/10)%36;
//	s->aktframe = ((((d->angle512+deg_360_to_512(270))&(512-1))*(36/2))>>8);
	s->aktframe = ((((d->angle512)&(512-1)))>>6);/*"rwingx8.png"*/
}
/*
(※１):大昔のversionからズレまくってるが、fps_factorが一定しないから、
fps_factorを入れようがズレるのはKETM自体の仕様。
//
 easy や normal では、ほぼ変わらない。
lunatic だと多少(数ドット)左にずれる。
//
lunatic は現状処理落ちしてるからなのかも???
*/


void enemy_cir_add(int lv)
{
	int i;
	for (i=0;i<NUM_OF_ENEMIES;i++)
	{
		SPRITE *s;
		s				= sprite_add_file(/*"rwing x.png"*/"rwingx8.png",8/*37*/,PR_ENEMY);		s->anim_speed=0;
		s->type 		= SP_EN_CIR;
		s->flags		|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
		s->mover		= enemy_cir_move;
//		s->aktframe 	= 0;
		CIR_DATA *data;
		data			= mmalloc(sizeof(CIR_DATA));
		s->data 		= data;
		data->b.score	= score(/*25*/15*2);
		data->b.health	= 5+(difficulty<<2);
		data->angle512	= deg_360_to_512(90);
		data->speed 	= 6;
		data->state 	= 0;
		data->level 	= lv;
		s->x			= ((i&(2-1)/*%2*/)==0)?(30):(50);
		s->y			= -(i*20);
	}
}
#undef NUM_OF_ENEMIES
