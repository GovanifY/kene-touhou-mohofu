
#include "douchu.h"

/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	"���ґ�2",		"GREETER",
	-------------------------------------------------------
	�T�@���ɑ�����A�ґ���s���Ă���U�R�G
	-------------------------------------------------------
	ming_greeter
	rwingx_curver(ming)�Ɏ��Ă�
---------------------------------------------------------*/
#if 0/* �߂� */
/* �{�X���ʋK�i */
	#define target_x256 		user_data00 	/* �ڕWx���W */
	#define target_y256 		user_data01 	/* �ڕWy���W */
	#define vvv256				user_data02 	/* �ڕW���W�ւ̓��B���� */
	#define time_out			user_data03 	/* �������� */
#endif

//#define tx256 	user_data00
//#define ty256 	user_data01
#define time_out	user_data03
#define speed256	user_data04

#define max_y256	user_data05


/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_yukari2(SPRITE *src)
{
	src->time_out--;
	if (0 > src->time_out)
	{
		gamen_gai_nara_zako_osimai(src);/* ��ʊO�Ȃ炨���܂� */
	}
	else
	{
		if (src->y256 >= src->max_y256)
		{
			src->time_out = (0);
			if (t256(1.5/*2.0*/) < src->speed256)
			{	/*����*/
				src->speed256 -= t256(0.2);
			//	src->speed256 = t256(2)/*3+difficulty*/;						// [***090201		�ύX
			}
		//
			obj_send1->x256 	= (t256(GAME_WIDTH/2)); 		/* �e��x256 */
			obj_send1->y256 	= (t256(0));					/* �e��y256 */
			tmp_angleCCW1024_jikinerai(obj_send1, src);
			src->vx256=((sin1024((src->tmp_angleCCW1024))*src->speed256)>>8);		/* CCW�̏ꍇ */
			src->vy256=((cos1024((src->tmp_angleCCW1024))*src->speed256)>>8);
		}
		if (ENEMY_LAST_SHOT_LINE256 > src->y256)	/* ���̃��C����艺����͓G�������Ȃ� */
		{
			//if (2 src->enemy_rank)
			{
			//	if (0==(ra_nd()%(105-(src->enemy_rank-2+difficulty)*10)))		// [***090126		�኱�ύX
			//	if (0==(ra_nd()%(11-(src->enemy_rank-2+difficulty) )))			// [***090126		�኱�ύX
			//	if (0==(ra_nd()%(16-(src->enemy_rank-2+difficulty) )))			// [***090126		�኱�ύX
			//	if ((src->enemy_rank-2+difficulty) >= (ra_nd()&(16-1))) 		// [***090126		�኱�ύX
				if ((difficulty) >= (ra_nd()&(64-1)))	// [***090126		�኱�ύX
				{
					obj_send1->x256 					= src->x256;
					obj_send1->y256 					= src->y256;
					br.BULLET_REGIST_speed256			= t256(difficulty)+t256(1.5);				/* �e�� */
				//	br.BULLET_REGIST_speed256			= speed256; 				/* �e�� */
					br.BULLET_REGIST_angle1024			= ANGLE_JIKI_NERAI_DAN; 	/* ���@�_���e */	/* ���˒��S�p�x / ����@�\(���@�_��/��) */
				//	br.BULLET_REGIST_div_angle1024		= (0);						/* �_�~�[�p�x(���g�p) */
					br.BULLET_REGIST_bullet_obj_type	= BULLET_MARU8_00_AKA;		/* [�Ԓe] */ /* �e�O�� */
					br.BULLET_REGIST_n_way				= (1);						/* [1way] */
					br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
					bullet_regist_vector();
				}
			}
		}
	}
	/*���Ă邪������ƈႤ--�ȉ�rwingx.c�Ɠ���*/
	src->x256 += (src->vx256);/*fps_factor*/
	src->y256 += (src->vy256);/*fps_factor*/
//
	src->m_angleCCW1024 += (10);/*�O����]*/
	mask1024(src->m_angleCCW1024);
}


/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

global void add_zako_yukari2(STAGE_DATA *l)
{
	SPRITE *h;
	h							= sprite_add_gu_error();
	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
	{
		add_zako_common(l, h);
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
		h->type 				= TEKI_61_NIJI_HOSI;
		h->callback_mover		= move_yukari2;
	//
		h->max_y256 			= (t256(GAME_HEIGHT)-((t256(24)/*s->h128+s->h128*/))-t256(60));
		tmp_angleCCW1024_jikinerai(obj_player, h);
		h->speed256 			= (t256(2.5/*3.0*/)+((3/*di fficulty*/)<<4) ); /*4*/ /*�n�߂����͑���*/
		/* CCW�̏ꍇ */
		h->vx256=((sin1024((h->tmp_angleCCW1024))*h->speed256)>>8);
		h->vy256=((cos1024((h->tmp_angleCCW1024))*h->speed256)>>8);
	//
		h->time_out 			= (60*99);/* �e�L�g�[ */
	}
}
