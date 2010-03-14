
#include "douchu.h"

/*---------------------------------------------------------
	"�A�z��1",		"PLASMABALL",
	-------------------------------------------------------
	(��)���ыʁA(601��)�d��
	(4*6)��24�C���o�Ă��邪���܂�Ӗ��̂Ȃ��L����
	-------------------------------------------------------
	�ԉf�˕�(?)�ɕς����B
	���Ȃ�e�L�g�[�B
---------------------------------------------------------*/

#define limit_y256		user_data04 	/* ���z�n�� */


/*---------------------------------------------------------
	�G����
---------------------------------------------------------*/

static void lose_inyou1(SPRITE *src)
{
//	if ( 0==(ra_nd()&(16-1)) ) /*�m���グ���B[1/16]��[1/20]*/ /*%20*/
	if ( 0==(ra_nd()&( 8-1)) ) /*�m���グ���B[1/8]��[1/20]*/ /*%20*/
	{
		send1_obj->x256 = src->x256;
		send1_obj->y256 = src->y256;
		send1_obj->BULLET_REGIST_speed256			=	t256(5.0);				/* �e�� */	//speed256;
		send1_obj->BULLET_REGIST_angle512			=	ANGLE_JIKI_NERAI_DAN;	//angle512;
		send1_obj->BULLET_REGIST_div_angle512		=	(int)(512/(24));		/* �����p�x */
		send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_HARI32_01_AKA;	/* [�Ԑj�e] */	//bullet_obj_type;
		send1_obj->BULLET_REGIST_n_way				=	(5);					/* [5way] */
		bullet_regist_basic();
	}
	else
	{
		item_create(src, (SP_ITEM_00_P001+(ra_nd()&1)), 1, ITEM_MOVE_FLAG_06_RAND_XY);
	}
}
/*50%(SP_ITEM_00_P001 or SP_ITEM_01_P008)*/
			//66%==SP_ITEM_04_BOMB or 33%==SP_ITEM_01_P008	 (SP_ITEM_EXTRA_HOMING+(ra_nd()&3/*%3*/)),

/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_inyou1(SPRITE *src)
{
	src->x256 += src->vx256;
	if (
		(((0)*256) > (src->x256) ) ||
		(((GAME_WIDTH)*256) < (src->x256) )
		)
	{
		src->vx256 = -src->vx256;
	}
//
	if (0 < src->vx256)
	{
		src->m_angleCCW512 += 12; /*rad2 deg512(0.3)*/ /*24*/ /*fps_factor*/ /* rad2 deg512( 0.3)== 24.4461992589151487740904245119933 */
	}
	else
	{
		src->m_angleCCW512 -= 12; /*rad2 deg512(0.3)*/ /*24*/ /*fps_factor*/ /* rad2 deg512( 0.3)== 24.4461992589151487740904245119933 */
	}
//
	if ((t256(2.50)) > src->vy256)	/* �ő�d�͉����x */
	{
		src->vy256 += 8;	/* �d�͉����x */
	}
//
	src->y256 += src->vy256;
	if ( (src->limit_y256) < (src->y256) )
	{
	//	src->y256			= src->limit_y256;
		src->limit_y256 	= t256(300);
		src->vy256			= -src->vy256;	/* ���˂� */
	}
	if ((272*256) < (src->y256) )
	{
		src->type = SP_DELETE;	/* �����܂� */
	}
}

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

void add_zako_inyou1(STAGE_DATA *l)
{
	int yyy;
	int xxx;
	for (yyy=0; yyy<4; yyy++)
	{
		for (xxx=0; xxx<5/*6*/; xxx++)
		{
			SPRITE *h;
			h						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
			h->type 				= TEKI_53_INYOU_DAI;
	add_zako_common(l, h);
//			h->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
			h->callback_mover		= move_inyou1;
			h->callback_loser		= lose_inyou1;
			h->callback_hit_enemy	= callback_hit_zako;
//
//			/*src->base.*/h->base_score 	= score(25*2);
//			/*src->base.*/h->base_hp		= (8*(8*8))+(1/*di fficulty*/<<(2+3));	/*���炩����*/	/*(2+(di fficulty<<2))*/
//			h->state			= 0;
//			h->speed256 		= ((ra_nd()&(512-1)))+128;		/*((ra_nd()&(256-1)))*3*/		/*3*((dou ble)ra_nd()/RAND_MAX+1)*/
			h->limit_y256		= t256(128)-((yyy<<(5+8))); 	/*t256(200)-(j*t256(40))*/		/* 臒l */
		//	h->x256 				= t256( 32)+((xxx<<(6+8))); 	/*t256( 40)+(i*t256(35))*/
//			h->y256 				= (-((s->w128+s->w128))-(xxx*t256(16))-(yyy*t256(64)));
									/*(-((s->w128+s->w128))-(xxx*t256(20))-(yyy*t256(50)))*/
			#if 1/*Gu(���S���W)*/
			h->x256 				= t256( 32)+((ra_nd()&0xff));	/*t256( 40)+(i*t256(35))*/
			h->y256 				= ( -(yyy*t256(512)));
			#endif
			h->vx256				=  (ra_nd()&0x1ff);
			h->vy256				=  (ra_nd()&0x1ff);
		}
	}
}
