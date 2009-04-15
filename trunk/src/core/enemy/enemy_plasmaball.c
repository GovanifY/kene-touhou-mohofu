
#include "enemy.h"

typedef struct
{
	ENEMY_BASE b;
	/*double*/int angle512;
	double speed;
	int state;
	int level;
//
	int destx;
	int desty;
	int r;
	int c;
} PLASMABALL_DATA;


//#define NUM_OF_ENEMIES (4*6)
static void enemy_plasmaball_move(SPRITE *s)
{
	PLASMABALL_DATA *d=(PLASMABALL_DATA *)s->data;
	switch (d->state)
	{
	case 0:
		s->y+=d->speed*fps_factor;
		if (s->y>=d->desty)
		{
			s->y			= d->desty;
			d->state		= 1;
			d->angle512		= 0/*rad2deg512(0)*/;
			d->r			= 0;
			d->c			= 0;
		}
		break;
	case 1:
		if (d->r==0)
		{
			d->angle512 += /*rad2deg512(0.3)*/24/**fps_factor*/; /* rad2deg512( 0.3)== 24.4461992589151487740904245119933 */
			if (d->angle512 > (512)/*rad2deg512(2*M_PI)*/ )
			{
				d->r	= 1;
				d->c++;
			}
		}
		else
		{
			d->angle512 -= /*rad2deg512(0.3)*/24/**fps_factor*/; /* rad2deg512( 0.3)== 24.4461992589151487740904245119933 */
			if (d->angle512 < 0/*rad2deg512(0)*/ )
			{
				d->r	= 0;
			}
		}
		s->y = d->desty + co_s512((d->angle512))*(1+6-d->c);
		if ((d->angle512 > -57/*rad2deg512(-0.7)*/ ) && 	/* rad2deg512(-0.7)==-57.0411316041353471395443238613177 */
			(d->angle512 <  32/*rad2deg512( 0.4)*/ ) && 	/* rad2deg512( 0.4)== 32.5949323452201983654538993493244 */
			(d->c==5))
		{
			d->state=2;
			if (0==(rand()&(16-1)/*%20*/))/*確率上げた。1/16←1/20*/
			{
				bonus_multi_add(s,
					(SP_BONUS_01_FIREPOWER_G+(rand()&1))/*50%(SP_BONUS_01_FIREPOWER_G or SP_BONUS_02_BOMB)*/
					//66%==SP_BONUS_02_BOMB or 33%==SP_BONUS_01_FIREPOWER_G   (SP_BONUS_EXTRA_HOMING+(rand()&3/*%3*/)),
				, 1, BONUS_FLAG_RAND_XY);
			}
			if (0==(rand()&(16-1)/*%20*/))/*確率上げた。[1/16]←[1/20]*/
			{
				bullet_create_haridan180(s, ANGLE_JIKINERAI_DAN, t256(5), 0, 0);
			}
		}
		break;
	case 2:
		d->desty+=50;
		if (d->desty > GAME_HEIGHT) //denis 480
		{	s->type=SP_DELETE;}
		else
		{	d->state=0;}
		break;
	}
}

void enemy_plasmaball_add(int lv)
{
	int i,j;
	for (j=0;j<4;j++)
	{
		for (i=0;i<6;i++)
		{
			SPRITE *s;
			s				= sprite_add_file("plasmaball.png",11,PR_ENEMY); 		s->anim_speed=1;
			s->type 		= SP_EN_PLASMABALL;
			s->flags		|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
			s->mover		= enemy_plasmaball_move;
			s->aktframe 	= /*0*/(rand()&(8-1))/*rand()%s->frames*/;
			PLASMABALL_DATA *data;
			data			= mmalloc(sizeof(PLASMABALL_DATA));
			s->data 		= data;
			data->b.score	= score(25*2);
			data->b.health	= 2+(difficulty<<2);
			data->destx 	= i*35+40;
			data->desty 	= 200-(j*40);
			data->state 	= 0;
			data->speed 	= t256_to_double((rand()&(256-1)))*3/*3*((dou ble)rand()/RAND_MAX+1)*/;
			s->x			= data->destx;
			s->y			= -s->w-i*20-(j*50);
			data->level 	= lv;
		}
	}
}
//#undef NUM_OF_ENEMIES
