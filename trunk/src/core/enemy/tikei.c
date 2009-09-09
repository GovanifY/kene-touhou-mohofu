
#include "bullet_object.h"

/*---------------------------------------------------------
	�n�`�Ɋ֘A����G
	-------------------------------------------------------

---------------------------------------------------------*/

#if 0
typedef struct
{
	ENEMY_BASE b;
	int speed256;
//	int state;
//	int tx256;
//	int ty256;
//	int level;
} BGPANEL_DATA;

typedef struct
{
	ENEMY_BASE b;
	int speed256;
	int state;
	int wait1;		// [***090124		�ǉ�
	int wait2;
//	int tx256;
//	int ty256;
//	int level;
} GROUNDER_DATA;
#endif

typedef struct
{
#if 1
	ENEMY_BASE b;		// [***090410		???�ǉ�???
#endif
	int speed256;
	int state;
	int wait1;
	int wait2;
//	int level;
} MAGICF_DATA;

#define GROUNDER_DATA MAGICF_DATA
#define BGPANEL_DATA  MAGICF_DATA

/*---------------------------------------------------------
	�����閂���w(�����̂��)
---------------------------------------------------------*/

static void move_magic_formation(SPRITE *s)
{
	MAGICF_DATA *data = (MAGICF_DATA *)s->data;
	switch (data->state)
	{
	case 0:
		data->wait1 -= 1/*fps_fa ctor*/;
		if (data->wait1 < 1/*0*/)
		{
		//	data->wait1=8/*5*/;
		//	s->alpha	= 0;
			data->state=1;
		}
		break;
	case 1: 		/* �o�� */
		s->alpha += 3/*4*/ /**fps_fa ctor*/;
		if (180 < (unsigned int)(s->alpha))
		{
			s->alpha = 180;
			data->state=2;
		}
		break;
	case 2:
		if (0 < data->wait2)		// [***090128		�ύX
		{
			data->wait1--;
			if (data->wait1 < 1/*0*/)
			{
				data->wait1=8/*5*/;/*�v���O�����������Ȃ��Ă镪�x������*/
				data->wait2--;
				play_voice_auto_track(VOICE10_MAGICF_E_SHOT);
#if 0
				int sp;
				#if 0
				/* sp �� �w��2��3�ŁA��1/80�̊m���ŋɋH��1���o�� */
				sp=ra_nd()%3/*+1-1*/;	/* 0 1 2 */
				if (0==sp/*<2-1*/)
				{
					sp=ra_nd()%3/*+1-1*/;
					if (0==sp/*<2-1*/)
					{
						sp=ra_nd()%3/*+1-1*/;
						if (0==sp/*<2-1*/)
						{
							sp=ra_nd()%3/*+1-1*/;
						}
					}
				}
				#else
			//	sp=ra_nd()%80;
				sp=(ra_nd()&(64-1));/* �m��(1/64) �ɕύX */
				if (0!=sp)
				{
					sp = (sp&1)+1;
				}
				#endif
				sp++;	/* sp = 1 or 2 or 3 */
				/* ���x�� (1.0�܂�) �� 2.0 �� 3.0 */
#endif
				/* ���x�� 1.0-3.0�܂ł̖��i�K�����ɕύX */
				{	int angle512;
					angle512=(ra_nd()&(512-1))/*%360*/;
					bullet_create_tomari2_dan(s, /*t256(sp)*/((ra_nd()&(512-1))+256)/*speed256*/, /*degtorad*/(angle512), t256(0.04), ANGLE_NO_SHOT_DAN);
				}
			}
		}
		else
		{	data->state=3;}
		break;
	case 3: 		/* ������ */
		s->alpha -= 1/*2*/ /**fps_fa ctor*/;
		if ( 4/*0*/ > (unsigned int)(s->alpha))
		{
			s->alpha = 0;
			s->type = SP_DELETE;
		}
		break;
	}
}

/*---------------------------------------------------------
	�΂�܂��U�������w
---------------------------------------------------------*/

static void lose_grounder(SPRITE *s)
{
//	case SP_GROUNDER:
	if (rand_percent(30))	{	item_create(s,					  SP_ITEM_00_P001,	1, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY)/*(up_flags)*/ );}
	if (rand_percent(30))	{	item_create(s,/*->x+ 5, c->y+5,*/ SP_ITEM_06_TENSU, 1, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY)/*(up_flags)*/ );}
	if (rand_percent(30))	{	item_create(s,/*->x+10, c->y-5,*/ SP_ITEM_00_P001,	1, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY)/*(up_flags)*/ );}
}

/*---------------------------------------------------------
	�΂�܂��U�������w
---------------------------------------------------------*/

static void move_grounder(SPRITE *s)	// [***090124 �U���p�^�[����ς���
{
	GROUNDER_DATA *data = (GROUNDER_DATA *)s->data;
	switch (data->state)
	{
	case 0:
		if ((s->y256 >= player->y256) ||
			(s->y256 > t256(100)) )
		{
			data->state=1;
			bullet_create_aka_maru_jikinerai(s, t256(1) );
		}
		break;/*??? [***090210 �ǉ� */
	case 1:
		if (s->y256 > t256(GAME_HEIGHT+16+2))/* GAME_HEIGHT+16+2==290 */
		{
			s->type=SP_DELETE;
		}
	}
	if (data->wait2<(difficulty*7)+10) // [***090128 �ύX
	{
		data->wait1--;
		if (data->wait1 < 1/*0*/)
		{
			data->wait1=25-(difficulty*5);
			play_voice_auto_track(VOICE10_MAGICF_E_SHOT);
#if 0
			int sp;
			#if 0
			/* sp �� �w��4�ŁA��42%�̊m���� 1 2 3 ���o�� */
			sp=((ra_nd()&(4-1))/*+1-1*/);/* 0 1 2 3 */
			if (sp<(4-1))
			{
				sp=((ra_nd()&(4-1))/*+1-1*/);/* 0 1 2 3 */
				if (sp<(4-1))
				{
					sp=((ra_nd()&(4-1))/*+1-1*/);/* 0 1 2 3 */
					/* ���̎��_�� 0 1 2 ���o��m����(0.75*0.75*0.75==0.421875%) */
				}
			}
			sp++;
			#else
			if (rand_percent(56))	/*56%�̊m����*/
			{
				sp=ra_nd()&3;
				/* ���̎��_�� 1 2 3 ���o��m����(��42%==0.56*3/4) */
				if (0==sp)
				{
					sp = 4;/*�c���4*/
				}
			}else
			{
				sp = 4;/*�c���4*/
			}
			#endif
			/* sp == 1 or 2 or 3 or 4 */
#endif
			/* ���x�� 1.0-4.0�܂ł̗���(�����������ɕ΍�)�ɕύX */
			bullet_create_tomari2_dan(s, ((ra_nd()&(1024-1))|(0x100))/*t256(sp)*/, /*deg512_2rad*/((ra_nd()&(512-1)))/*de gtorad(ra_nd()%360)*/, t256(0.03), ANGLE_NO_SHOT_DAN);
		}
	}
	s->y256 += data->speed256/**fps_fa ctor*/;
}

/*---------------------------------------------------------
	�x�[�X�p�l��
---------------------------------------------------------*/

static void move_bg_panel(SPRITE *s)
{
	BGPANEL_DATA *data = (BGPANEL_DATA *)s->data;
	if (s->y256 > t256(GAME_HEIGHT+2)) /* 272+2 == 274 */
	{
		s->type = SP_DELETE;
	}
	s->y256 += data->speed256/**fps_fa ctor*/;
}

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/


//#define OLD_PANEL (0/*1*/)
//#if (1==OLD_PANEL)
//extern void enemy_bgpanel_add_xy( 		const short xxx, const short yyy, char speed);/*������*/
//extern void enemy_bgpanel2_add_xy(		const short xxx, const short yyy, char speed);/*������*/
//extern void enemy_grounder_add_xy(		const short xxx, const short yyy, const short speed256);/*������*/
//extern void enemy_magicformation_add_xy(	const short xxx, const short yyy, const short speed256);/*������*/
//#endif


//	l->user_x, l->user_y, l->para3, /*type*/l->command/*1-1*/
//actually lv is the x coord
//id enemy_magicformation_add_xy(const short xxx, const short yyy, const short speed256){}/*������*/
//id enemy_grounder_add_xy(const short xxx, const short yyy, const short speed256) {}/*������*/
void add_enemy_panels(STAGE_DATA *l
//const short xxx,
//const short yyy,
//const short speed256,
//int type
)
{
short xxx;
short yyy;
short speed256;
int type;
//
xxx=l->user_x;
yyy=l->user_y;
speed256 = l->scroll_speed256;
type = /*type*/l->user_command/*1-1*/;
//
	short yy2;
	yy2 = yyy;
	BGPANEL_DATA *data;
//	GROUNDER_DATA *data;
//	MAGICF_DATA *data;
	data			= mmalloc(sizeof(BGPANEL_DATA));
//	data			= mmalloc(sizeof(GROUNDER_DATA));
//	data			= mmalloc(sizeof(MAGICF_DATA));
	data->state 	= 0;
//	data->tx256 	= player->x256;
//	data->ty256 	= player->y256;
//	data->level 	= 0;
//
	SPRITE *s;
	s = NULL;
//
	switch (type)
	{
	case BTYPE_01_BGPANEL1:  /*(1-1)*/		/* �p�l���� */		//		"��",
		s=sprite_add_res(BASE_TIKEI_BGPANEL1_PNG);	//	s->anim_speed	= 0/*6*/;/*�Ƃ肠�����Ȃ�*/ 	/*P R_BACK0*/
		goto BTYPE_BGPANEL;
		/*not_break;*/
	case BTYPE_02_BGPANEL2:  /*(2-1)*/		/* �p�l���_ */		//		"�_",
		s=sprite_add_res(BASE_TIKEI_BGPANEL2_PNG);	//	s->anim_speed	= 0;/* 0 ���X���Ȃ�, 6==KETM�̃o�O */	/*P R_BACK0*/
		/*goto BTYPE_BGPANEL;*/
		/*not_break;*/
	BTYPE_BGPANEL:
	//
		s->type 			= SP_ZAKO/*SP_TIKEI*/ /*SP_BGPANEL*/;
	//	s->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);
		s->callback_mover	= move_bg_panel;
/**/	//s->alpha			= 255;
		s->anim_speed		= 0/*6*/;
		s->data 			= data;
		data->b.score		= score(10*2);
		data->b.health		= 2;/*???*/
		data->speed256		= (speed256);
		break;

	case BTYPE_03_GROUNDER: 		/* �΂�܂��U�������w */	//	"�����w1",	/*	"GROUNDER",*/
		/*
		anim_speed: 1 ����
		anim_speed: 3 �x����
		*/
		s					= sprite_add_res(BASE_TIKEI_GROUNDER08_PNG);	//s->anim_speed =  2/*3*/ /*(5-difficulty)*/ /*1*/;/*9"grounder.png"*/
		s->type 			= SP_ZAKO/*SP_TIKEI*/ /*SP_GROUNDER*/;
		s->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s->callback_mover	= move_grounder;
		s->callback_loser	= lose_grounder;
/**/	//s->alpha			= 255;
		s->data 			= data;
		data->b.score		= score(200*2);
		data->b.health		= 25+(difficulty<<3/*4*/);//+(difficulty*15);
		data->wait1 		= 100;	// [***090124		�ǉ�
		data->wait2 		= 0;
		data->speed256		= (speed256);
		break;
	case BTYPE_04_MAGIC_FORMATION:		/* �����閂���w */	//	"�����w2",	/*	"MAGICF",*/
		s					= sprite_add_res(BASE_TIKEI_GROUNDER08_PNG);	s->anim_speed	=  3 /*(5-difficulty)*/ /*2*/;/*9"grounder.png"*/
		s->type 			= SP_PLAYER2;
		s->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s->callback_mover	= move_magic_formation;
		s->alpha			= 2/*0*/;
		s->data 			= data;
	#if 1
		data->b.score		= score(200*2); 		// [***090410		???�ǉ�???
		data->b.health		= 25+(difficulty<<2);	// [***090410		???�ǉ�???
	#endif
		data->wait1 		= 10/*+90*/;/* */
		data->wait2 		= 16+(difficulty<<3)/*10+(7*difficulty)*/;
		yy2 = -yyy; 		/* KETM�̑��΍��W�w�肩��AMAGICF�`���̐�΍��W�w��ɕϊ����� */
		break;
	}
//
		s->anim_frame		= 0;
		s->x256 			=  ((xxx)<<8);
	//	s->y256 			=  ((yyy)<<8)-((s->h128+s->h128));
		s->y256 			= -((yy2)<<8)-((s->h128+s->h128));
}