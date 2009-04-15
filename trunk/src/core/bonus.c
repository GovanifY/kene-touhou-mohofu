
#include "bonus.h"

//extern double fps_factor;
extern SDL_Surface *screen;
extern SPRITE *player;		//[***090116		�ǉ�

/*---------------------------------------------------------
	�A�C�e���o��������q�֐�
---------------------------------------------------------*/

static SPRITE *bonus_sel(int x, int y, int type)
{
//	�A�C�e���̎�ނ�I��
	SPRITE *s;
	s=sprite_add_file("bonus_items.png",8,PR_BONUS);	s->anim_speed=0;
	s->anim_speed=0;
	s->aktframe = ((/*SP_BONUS_FIREPOWER-*/type)&0x07);
//	�o���ʒu�����߂�
	#define OFFSET_X64		(64)/* 64�͂��炵�� 2��n��̕K�v�L�� */
	#define ITEM_WIDTH16	(16)/* 16�̓A�C�e���� */
	#define ITEM_X_LIMIT	(GAME_WIDTH+OFFSET_X64-ITEM_WIDTH16)
	x += ((rand()&((OFFSET_X64+OFFSET_X64)-1)));
	if (x < OFFSET_X64)
	{
		x = OFFSET_X64;
	}
	else
	if (x > (ITEM_X_LIMIT))
	{
		x = (ITEM_X_LIMIT);
	}
//	�o�^����
	s->x		= x-OFFSET_X64;
	s->y		= y;
	s->flags	|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	s->type 	= type;
	s->mover	= bonus_move;
	return (s);
	#undef OFFSET_X64
	#undef ITEM_WIDTH16
	#undef ITEM_X_LIMIT
}

/*---------------------------------------------------------
	�{�[�i�X�A�C�e����o�^���ďo��������
---------------------------------------------------------*/

typedef struct
{
	/*double*/int y_sum256; 	/* �{�[�i�X�A�C�e�������o���p y�� �ώZ�l(y���A������}�C�i�X) */
	/*double*/int angle512;
	int pl_up;					// [***090116		�ύX
} BONUS_DATA;

/* [***090125	�ǉ�:up��1�Ńv���C���[�ɏW�܂�܂��B0�ŕ��ʂ̏�ԁB */
void bonus_multi_add(SPRITE *src/*int x, int y*/, int type, int num, int up_flags)
{
	int i;
	for (i=0;i<num;i++)
	{
		SPRITE *s;
//	int dx;
//	int dy;
	int x;
	int y;
	x=src->x/*+(rand()&(8-1))*/;
	y=src->y/*+(rand()&(8-1))*/;
//	dx=0;
//	dy=0;
		/*if (BONUS_FLAG_RAND_X&up_flags)*/	//{		dx=((rand()&(128-1))-64);}
		/*if (BONUS_FLAG_RAND_Y&up_flags)*/	//{		dy=((rand()&(64-1))-32);}
	//	s				= bonus_sel(x + ((rand()%100)-50), y + ((rand()%60)-30), type);
		s				= bonus_sel(x  /*+dx*/, y/*+dy*/, type);
		if (NULL==s) return;
//
		BONUS_DATA *data;
		data			= mmalloc(sizeof(BONUS_DATA));
		s->data 		= data;
		/* ��ʉ����ɋ�����A�����o�����x�𑬂����� */
		data->y_sum256	= -(/*256*/((int)(src->y))<<1)-(rand()&0xff);	/* �{�[�i�X�A�C�e�������o�������l(y���A������}�C�i�X) */
	//	data->y_sum256	= t256(-5*0.5); 	//������ƕύX
	//	data->y_sum256	= t256(-2*0.5); 	/* �{�[�i�X�A�C�e�������o�������l(y���A������}�C�i�X) */
		data->angle512	= rad2deg512(0.08);
		data->pl_up 	= (up_flags&BONUS_FLAG_COLLECT);
	//
	}
}

/*---------------------------------------------------------
	�{�[�i�X�A�C�e���̈ړ�
---------------------------------------------------------*/

void bonus_move(SPRITE *s)
{
	BONUS_DATA *d=(BONUS_DATA *)s->data;
	if ((((PLAYER_DATA *)player->data)->bonus_flag) & (/*BONUS_FLAG_01_aaaa |*/ BONUS_FLAG_02_AUTO_GET_ITEM) )/*???���Ԃ�*/
	{
		d->pl_up=1;
	}
	if (!d->pl_up)	/*�P���ɕ�������`���ė�����*/
	{
		if (d->y_sum256 < t256(3) ) 	//[***090123.0220		�ύX5=>4=>3
		{	d->y_sum256 += d->angle512;}/* x1.5 */
		s->y += (t256_to_double(d->y_sum256))/**fps_factor*/;
		if (s->y>GAME_HEIGHT) {s->type=SP_DELETE;} // denis 480
	}
	else			/*�����ɏW�܂�*/
	{
		d->y_sum256 -= t256(1/*5*0.5*/);
		if (d->y_sum256 < t256(0) )
		{
			d->y_sum256 = t256(/*4*/20/*5.6 14*/);		/* 5.6==(14/(5*0.5))*/
			d->angle512 = atan_512(player->y-s->y,player->x-s->x);
		}
		/*�����ɏW�܂�*/
		s->x+=co_s512((d->angle512))*/*3*/4/*12*/ /**fps_factor*/;
		s->y+=si_n512((d->angle512))*/*3*/4/*12*/ /**fps_factor*/;
	}
}

/*---------------------------------------------------------
	�{�[�i�X�A�C�e���̖��̂̕\��
---------------------------------------------------------*/

void bonus_info_add(int x, int y, char *filename)
{
	SPRITE *c;
	c			= sprite_add_file(filename,1,PR_BONUS);
	c->flags	= SP_FLAG_VISIBLE;
	c->type 	= SP_ETC;
	c->x		= x;
	c->y		= y;
	c->alpha	= 255;
	c->mover	= bonus_info_move;
}

/*---------------------------------------------------------
	������A�C�e���̎�ނ�����\��
---------------------------------------------------------*/

void bonus_info_move(SPRITE *c)
{
	if (c->alpha>=3/**fps_factor*/)
	{
		c->alpha-=3/**fps_factor*/;
		c->y -= /*1*/0.5/**fps_factor*/; /* ��Ɉړ�(ketm�Ɠ���) */
	}
	else
	{
		c->type = -1;
	}
}

/*---------------------------------------------------------
	������A�C�e���̓��_������\���̎q�֐�
---------------------------------------------------------*/

typedef struct
{
	/*double*/int distance;
} BIT_DATA;

static void bonus_info_s_text(SPRITE *src/*int x, int y*/, char *text, int font)
{
	SDL_Surface *t;
	SPRITE *s;
	t			= font_render(text,font);
	s			= sprite_add(t,1,PR_BONUS,1);
	s->flags	|= SP_FLAG_FREESURFACE|SP_FLAG_VISIBLE;
	s->type 	= SP_ETC;
	s->x		= src->x;
	s->y		= src->y;
	s->alpha	= 255;
	s->data 	= mmalloc(sizeof(BIT_DATA));
	((BIT_DATA *)s->data)->distance=255;
	s->mover	= bonus_infotext_move;
}

/*---------------------------------------------------------
	������A�C�e���̓��_������\��(�Œ蓾�_�̏ꍇ)
---------------------------------------------------------*/

void bonus_info_score(SPRITE *src/*int x, int y*/, int score_type)
{
	/*const*/ Uint8 score_tbl[4]=
	{
	/* 0==SCORE_30*/	score(30),
	/* 1==SCORE_50*/	score(50),
	/* 2==SCORE_100*/	score(100),
	/* 3==SCORE_1000*/	score(1000),
	};
	((PLAYER_DATA *)player->data)->score += score_tbl[score_type];
//
	/*const*/ char *score_name[4]=
	{
	/* 0==SCORE_30*/	"30",
	/* 1==SCORE_50*/	"50",
	/* 2==SCORE_100*/	"100",
	/* 3==SCORE_1000*/	"1000",
	};
	bonus_info_s_text(src/*int x, int y*/, (char *)score_name[(score_type)&(4-1)], FONT07);
}

/*---------------------------------------------------------
	������A�C�e���̓��_������\��(�C�ӓ��_�̏ꍇ)
---------------------------------------------------------*/

void bonus_info_any_score(SPRITE *src/*int x, int y*/, int score_num)
{
	((PLAYER_DATA *)player->data)->score += score_num;
//
	char buffer[16];
	sprintf(buffer,"%d0", score_num );/* ���x�̗v��ꏊ�ł� sp rintf() �͎g��Ȃ������ǂ� */ 		/* �����񏈗�(sp rintf)�͔��ɒx�� */
	bonus_info_s_text(src/*int x, int y*/, buffer, FONT07);
}

/*---------------------------------------------------------
	��������_�������Ȋ����Ő����\��
---------------------------------------------------------*/

void bonus_infotext_move(SPRITE *c)
{
	BIT_DATA *b=(BIT_DATA *)c->data;
//	SDL_Surface *s;
	/* c->y 	+= 2 *fps_factor;  ���Ɉړ�(ketm)�ړ����x������ */
	c->y		-= /*1*/0.5/**fps_factor*/;   /* ��Ɉړ� */
	b->distance -= 3/**fps_factor*/;
	c->alpha	= b->distance;
	if (b->distance <= 0)
	{
		//s=sprite_getcurrimg(c);
		//parsys_add(s,1,1,c->x,c->y,30,0,0,100,PIXELIZE,NULL);
		c->type = -1;
	}
}




///--------- �ȉ�����


	#if 0
	switch (type)
	{
	case SP_BONUS_FIREPOWER:	s=sprite_add_file("bonus_p.png",1,PR_BONUS);	s->anim_speed=0;	break;		//[***090123		�ύX
	case SP_BONUS_FIREPOWER_G:	s=sprite_add_file("bonus_p_.png",1,PR_BONUS);	s->anim_speed=0;	break;		//[***090124		�ǉ�
	case SP_BONUS_BOMB: 		s=sprite_add_file("bonus_f.png",1,PR_BONUS);	s->anim_speed=0;	break;		//[***090123		�ύX
	case SP_BONUS_HEALTH:		s=sprite_add_file("bonus_h.png",9,PR_BONUS);	s->anim_speed=2;	break;
//	case SP_BONUS_EXTRA_HOMING:
//	case SP_BONUS_EXTRA_HLASER:
//	case SP_BONUS_EXTRA_SHIELD:
//								s=sprite_add_file("bonus_x.png",16,PR_BONUS);	s->anim_speed=3;	break;
	case SP_BONUS_COIN: 		s=sprite_add_file("coin.png",1,PR_BONUS);		s->anim_speed=0;	break;		//[***090123		�ύX
	case SP_BONUS_ADDSPEED: 	s=sprite_add_file("bonus_s.png",16,PR_BONUS);	s->anim_speed=2;	break;
	default:
		CHECKPOINT;
		error(ERR_WARN,"cant add unknown bonus! debug me! please! now!");
		return NULL;
	}
	#else
//	s=sprite_add_file("bonus_items.png",9,PR_BONUS);	s->anim_speed=0;
//	s->aktframe = SP_BONUS_FIREPOWER-type;
	#endif

	#if 0
	s=sprite_add_file("bonus_items.png",8,PR_BONUS);	s->anim_speed=0;
	s->anim_speed=0;
	switch (type)
	{
	case SP_BONUS_00_FIREPOWER: 	s->aktframe =0; break;		//[***090123		�ύX
	case SP_BONUS_01_FIREPOWER_G:	s->aktframe =1; break;		//[***090124		�ǉ�
	case SP_BONUS_02_BOMB:			s->aktframe =2; break;		//[***090123		�ύX
	case SP_BONUS_03_HEALTH:		s->aktframe =3; break;
	case SP_BONUS_04_FIREPOWER_128: s->aktframe =4; break;		//[***090405		�ǉ�
	case SP_BONUS_05_HOSI:			s->aktframe =5; break;		//[***090405		�ǉ�
//	case SP_BONUS_EXTRA_HOMING:
//	case SP_BONUS_EXTRA_HLASER:
//	case SP_BONUS_EXTRA_SHIELD: 					break;
	case SP_BONUS_06_COIN:			s->aktframe =6; break;		//[***090123		�ύX
	case SP_BONUS_07_ADDSPEED:		s->aktframe =7; break;
	default:
		CHECKPOINT;
		error(ERR_FATAL,"cant add unknown bonus! debug me! please! now!");
		return NULL;
	}
	#endif

	#if 0
	if (x </*GAME_WIDTH-20*/600/*600*/) x += 20;		/*600�͌���PC��KETM�̉����C���Y��(640-40==600)*/
	else							x -= 10;
	#else
	//x += 20;		/*�d�Ȃ�ƌ��ɂ����̂ŉ���20dot���炷*/
	//if ( GAME_WIDTH < x ) {	x = ((GAME_WIDTH - x) & (512-1));	}	/* �͂ݏo����܂�Ԃ��A���łɃ}�X�N */
//	x += 16;		/*�d�Ȃ�ƌ��ɂ����̂ŉ���20dot���炷*/
//	if ( (GAME_WIDTH/*-16*/) < (unsigned int)(x) ) { x = (GAME_WIDTH-256-32)+((x) & (256-1));	}	/* �͂ݏo����A�Ȃ�ƂȂ��E���̕��ɂ��� */
	#endif
	//	#if 0
	//	if (/*s->*/x < /*GAME_WIDTH-20*/480/*480*/) x += 20;
	//	else							x -= 10;
	//	#else
	//	x += 20;		/*�d�Ȃ�ƌ��ɂ����̂ŉ���20dot���炷*/
	//	if ( GAME_WIDTH < x ) { x = (GAME_WIDTH - x);	}	/* �͂ݏo����܂�Ԃ� */
	//	#endif
