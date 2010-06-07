
#include "douchu.h"

/*---------------------------------------------------------
	�����͕핗  �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	"�A�z��1",		"PLASMABALL",
	-------------------------------------------------------
	(��)���ыʁA(601��)�d��
	(4*6)��24�C���o�Ă��邪���܂�Ӗ��̂Ȃ��L����
	-------------------------------------------------------
	�ԉf�˕�(?)�ɕς����B
	���Ȃ�e�L�g�[�B
---------------------------------------------------------*/
#if 0/* �߂� */
/* �{�X���ʋK�i */
	#define target_x256 		user_data00 	/* �ڕWx���W */
	#define target_y256 		user_data01 	/* �ڕWy���W */
	#define vvv256				user_data02 	/* �ڕW���W�ւ̓��B���� */
	#define time_out			user_data03 	/* �������� */
#endif

#define limit_y256		user_data04 	/* ���z�n�� */


/*---------------------------------------------------------
	�G����
---------------------------------------------------------*/

static void lose_inyou1(SPRITE *src)
{
//	if ( 0==(ra_nd()&(16-1)) ) /*�m���グ���B[1/16]��[1/20]*/ /*%20*/
	if ( 0==(ra_nd()&( 8-1)) ) /*�m���グ���B[1/8]��[1/20]*/ /*%20*/
	{
		obj_send1->x256 = src->x256;
		obj_send1->y256 = src->y256;
		br.BULLET_REGIST_speed256			= t256(5.0);				/* �e�� */
		br.BULLET_REGIST_angle1024			= ANGLE_JIKI_NERAI_DAN; 	/* */
		br.BULLET_REGIST_div_angle1024		= (int)(1024/(24)); 		/* �����p�x */
		br.BULLET_REGIST_bullet_obj_type	= BULLET_HARI32_01_AKA; 	/* [�Ԑj�e] */
		br.BULLET_REGIST_n_way				= (5);						/* [5way] */
		br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
		bullet_regist_vector();
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
		src->m_angleCCW1024 += (24);	/*48*/ /*fps_factor*/ /* cv1024r(0.3) == 2*24.4461992589151487740904245119933 */
	}
	else
	{
		src->m_angleCCW1024 -= (24);	/*48*/ /*fps_factor*/ /* cv1024r(0.3) == 2*24.4461992589151487740904245119933 */
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
	(r30)�d�l�o�O	(�d�l:�����U�R�ǉ��͂��Ȃ�)
	r31�ŏC���ς�
---------------------------------------------------------*/

global void add_zako_inyou1(STAGE_DATA *l)
{
	SPRITE *h;
	h							= sprite_add_gu_error();
	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
	{
		add_zako_common(l, h);
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
		h->type 				= TEKI_53_INYOU_DAI;
		h->callback_mover		= move_inyou1;
		h->callback_loser		= lose_inyou1;
//		h->state				= 0;
//		h->speed256 			= ((ra_nd()&(512-1)))+128;		/*((ra_nd()&(256-1)))*3*/		/*3*((dou ble)ra_nd()/RAND_MAX+1)*/
		h->limit_y256			= (t256(128)-((/*yyy*/(l->user_255_code)&0x0f)<<(5+8)));	/*t256(200)-(j*t256(40))*/		/* 臒l */
								/*(-((s->w128+s->w128))-(xxx*t256(20))-(yyy*t256(50)))*/
		h->vx256				=  (ra_nd()&0x1ff);
		h->vy256				=  (ra_nd()&0x1ff);
	}
}
