
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗  �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�X�v���C�g �}�l�[�W��
	-------------------------------------------------------
	�͕핗�ł́A���X�g����(�`r30)�ł� 500�e�𒴂��������肩��A
	(�Q�[�����~�܂����̂��Ǝv�����炢)���������������������n�܂�܂��B
	�����ŁA���X�g�����͔p�~���܂��B
	//
	�z������Ȃ�A����z���S�������Ă� 1000�e ���炢�܂łȂ�A
	�Ƃ肠���������������܂���ł����B
	//
	r31����:�e�������z������ŃU�R�́A���X�g�����Ȃ̂ŐF�X��������������܂��B
	�U�R�ƒe�����I�������������ł��B(�����蔻�肪����̂ŁA������Ƃ����Ӗ��ł͂Ȃ��B
	���ʂ��Ďg����d�g�݂ɂ���Ƃ�����)
--------------------------------------------------------- */

/*---------------------------------------------------------
	�����蔻�� collision
--------------------------------------------------------- */

/*---------------------------------------------------------
	��`/�~ �����蔻��
	-------------------------------------------------------
	�I�u�W�F�N�g���m�̓����蔻��
	��܂��ɋ�`�Ŕ��ʂ�����A�߂����Ȃ�~�̏Փ˔���
--------------------------------------------------------- */

#if (1)
static /* BOOL */int collision_hit( /* obj_t */SPRITE *obj1, /* obj_t */SPRITE *obj2 )
{
	/*--- ����L���ȃI�u�W�F�N�g�łȂ�������I�� */
//	if (obj2->m_Hit256R == 0/* .0 */ /* FALSE */)
	if (256 > obj2->m_Hit256R /* FALSE */)
	{
		return (0/* FALSE */);	/* not hit. �������ĂȂ� */
	}
//	if (obj1->m_Hit256R == 0/* .0 */ /* FALSE */)
	if (256 > obj1->m_Hit256R /* FALSE */)
	{
		return (0/* FALSE */);	/* not hit. �������ĂȂ� */
	}
//
	/* flo at */int dr, dx, dy;
	dr = (obj2->m_Hit256R + obj1->m_Hit256R);
	#if 0/*SDL(�������W)*/
	dx = ((obj2->x256+obj2->w128) - (obj1->x256+obj1->w128));
	dy = ((obj2->y256+obj2->h128) - (obj1->y256+obj1->h128));
	#else/*Gu(���S���W)*/
	dx = ((obj2->x256) - (obj1->x256));
	dy = ((obj2->y256) - (obj1->y256));
	#endif
	/* check collision, delta x position. ��`����i�w���j */
	if (dx < 0/* .0 */) 	{	dx = (-dx); }	/* dx = abs(dx); */
	if (dx > dr)			{	return (0/* FALSE */);	}	/* not hit, because not near the dx. �w�̍������傫���̂ŁA�������ĂȂ� */

	/* check collision, delta y position. ��`����i�x���j */
	if (dy < 0/* .0 */) 	{	dy = (-dy); }	/* dy = abs(dy); */
	if (dy > dr)			{	return (0/* FALSE */);	}	/*	not hit, because not near the dy. �x�̍������傫���̂ŁA�������ĂȂ� */

	/* check collision, circle delta round distance.  �~�̔��a�Ŕ��� */
	dr *= dr;
	dx *= dx;
	dy *= dy;
	dx += dy;
	// �����蔻��O�Ȃ�
	if (dr < dx)/* if ( (dr^2) < ( (dx^2) + (dy^2) )) */
	{
		return (0/* FALSE */);	/* not hit. �������ĂȂ� */
	}
	return (1/* TRUE */); /* hit! �������� */
}
#endif


/*---------------------------------------------------------
	�X�v���C�g�̂����蔻��
	-------------------------------------------------------
	���͒l: type
		SP_GROUP_BULLETS	(�e��)
		SP_GROUP_ITEMS		(�A�C�e��)
--------------------------------------------------------- */

//	/*static*/global SPRITE *sp rite_list444_head = NULL;	/* �X�v���C�g�̃��X�g�\�� */

//global SPRITE sprite_pool[SPRITE_POOL_MAX];	/* �X�v���C�g�̃��X�g�\�� */


global SPRITE sprite_444pool[SPRITE_444POOL_MAX];	/* �X�v���C�g�̃��X�g�\�� */

/*static*/global SPRITE *sprite_collision_check_444(SPRITE *tocheck, int type)
{
//	SPRITE *s = sp rite_list444_head;/* �X�v���C�g ���X�g�̐擪 ����T�� */
//	while (NULL != s)
	int ii;
	for (ii=0; ii<SPRITE_444POOL_MAX; ii++ )/* �S�����ׂ�B */
	{
		SPRITE *s;
		s = &sprite_444pool[ii];
	//
		if (SP_DELETE != s->type)/* �����ς݂͔�΂��B */
		{
			/* 1. �������g�`�F�b�N */
			/* ����Ώ�(�������g)�͔�΂�(�S���������̓��m�̂����蔻�������Ă��Ӗ����Ȃ��̂Ŏ��Ȃ�) / zu testendes Sprite ueberspringen */
			if (s != tocheck)	/* Is sprites my self?(not need self collisions.) */
			{
				/* 2. �����ރ`�F�b�N */
				if (
					//		(s->type != SP_DELETE ) && /* �폜�ς݂͔�΂� */	/* SP_DELETE�� 0 �ɂȂ����̂ŗv��Ȃ��Ȃ����D */
							(s->type & type)			 /* type����v����ꍇ�̂� */	/* Passender Sprite-Type? */
					)	/* do collision only same type. */
				{
					/* 3. ����t���O�`�F�b�N */
					if (
							( (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/) == (s->flags&(SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/)))
							/* �����蔻�肪����A���A�\���\�Ȃ��̂̂݃`�F�b�N */
						)	/* do collision only visible and, use collision check type. */
					{
						#if 1
						/* 4. ��`/�~�����蔻��`�F�b�N   ��܂��ɋ�`�Ŕ��ʂ�����A�߂����Ȃ�~�̏Փ˔��� */
						if (collision_hit(s,tocheck))	/* ��`/�~�����蔻�� */ 	/* hit collision rectangle to circle check. */
						{
								return (s);/* �������� */	/* hit collisioning! */
					}
						#endif
					}
				}
			}
//			s = s->next;/* �� */
		}
	}
	return (NULL);/* �������ĂȂ� */	/* no hit collisioning. */
}


/*---------------------------------------------------------
	�X�v���C�g ���X�g�ɓo�^���ꂽ�X�v���C�g�������B
--------------------------------------------------------- */
#include <malloc.h>/* free(); */

#if 0
//extern SPRITE *sp rite_list444_head;
static void sprite_remove_one_444(SPRITE *src)
{
	SPRITE *s = sp rite_list444_head;/* �X�v���C�g ���X�g�̐擪 ����T�� */
	SPRITE *p = NULL;
	SPRITE *n = NULL;
	while (NULL != s)/* �X�v���C�g ���X�g�̏I���܂ŒT�� */
	{
		n = s->next;
		if (s == src)/* ���������H */
		{
			if (NULL == p)
			{
				sp rite_list444_head = n;	/* �X�v���C�g���X�g�̐擪������������B */
			}
			else
			{
				p->next = n;
			}
			free (s);
			#if 0
			s = NULL;/* �O�̈חv��H */
			#endif
			return;/* ���� */
		}
		p = s;
		s = n;
	}
	CHECKPOINT;/* �ُ� */
	error(ERR_WARN, (char*)"sprite not found in list");
}
#endif

/*---------------------------------------------------------
	�X�v���C�g ���X�g�ɓo�^���ꂽ�X�v���C�g��S�������B
--------------------------------------------------------- */

global void sprite_remove_all_444_void(void)/*int ty pe*/
{
	#if 0
	SPRITE *s = sp rite_list444_head;/* �X�v���C�g ���X�g�̐擪 ����T�� */
	SPRITE *n = NULL;
	while (NULL != s)
	{
		n = s->next;
		if (s->type & (SP_GROUP_ALL_GAME_OBJS)/*type*/) /* type����v����ꍇ�̂� */
		{
			sprite_remove_one_444(s);
		}
		s = n;
	}
	#else
	int ii;
	for (ii=0; ii<SPRITE_444POOL_MAX; ii++ )/* �S�����ׂ�B */
	{
		sprite_444pool[ii].type 	= SP_DELETE;
	}
	#endif
}


/*---------------------------------------------------------
	�X�v���C�g����t���[�������삳����B
--------------------------------------------------------- */

global void sprite_move_main_Gu_444(void)/*int ty pe*/
{
//	SPRITE *s;
//	SPRITE *n;
//	s = sp rite_list444_head;/* �X�v���C�g ���X�g�̐擪 ����T�� */
//	while (NULL != s)
	int ii;
	for (ii=0; ii<SPRITE_444POOL_MAX; ii++ )/* �S�����ׂ�B */
	{
		SPRITE *s;
		s = &sprite_444pool[ii];
	//
		#if 1/* �v��Ȃ��C������(�Ă��Ƃ���) */
		if (s->type != SP_DELETE ) /* �폜�ς݂͔�΂� */
		#endif
		{
//			n = s->next;
			#if 0
			if (s->type != SP_DELETE ) /* �폜�ς݂͔�΂� */	/* SP_DELETE�� 0 �ɂȂ����̂ŗv��Ȃ��Ȃ����D */
			#endif
			{
				if (s->type & (SP_GROUP_ALL_SDL_CORE_TYPE)/*type*/) /* type����v����ꍇ�̂� */
				{
					if (NULL != s->callback_mover)
					{
						(s->callback_mover)(s);
					}
				}
			}
//			s = n;
		}
	}
	/* �����̏ꍇ����ɐH�ݏo���`�F�b�N���s���e������(�b��I) */
	if (0==(pd_state_flag & STATE_FLAG_05_IS_BOSS))
	{
		bullet_angle_all_hamidasi_check();/*�p�x�e�̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�)*/
	}
}









#if 1
/*---------------------------------------------------------
	��ʊO�Ȃ炨���܂�(�ʁA��ɔėpobj�Ŏg�p)
---------------------------------------------------------*/
global void gamen_gai_nara_zako_osimai(SPRITE *src)
{
	if (
		(src->x256 < t256(0)) ||
		(src->x256 > t256(GAME_WIDTH)) ||
		(src->y256 < t256(0)) ||
		(src->y256 > t256(GAME_HEIGHT)) )		/* ��ʊO�ɏo���� */
	{
		src->type = SP_DELETE;		/* �����܂� */
	}
}
//	if ((src->x256 < t256(0))||(src->x256 > t256(GAME_WIDTH))||
//		(src->y256 < t256(0))||(src->y256 > t256(GAME_HEIGHT)))
//	{
//		src->type = SP_DELETE;/*��ʊO�ɂł��炨���܂�*/
//	}
#endif



#if 1
/*---------------------------------------------------------
	��ʊO�Ȃ炨���܂�(�e��)
---------------------------------------------------------*/

/* ������ */

/*extern*/global POINT256 bullet_clip_min;		/*  �͈�(�ŏ��l) */
/*extern*/global POINT256 bullet_clip_max;		/*  �͈�(�ő�l) */

static void s_check_hamidasi_bullet_angle_one(SPRITE *src)
{
	#if 1/* ������ */
//	if ((src->x256 < t256(-100))||(src->x256 > t256(100)+t256(GAME_WIDTH))||
//		(src->y256 < t256(-256))||(src->y256 > t256(100)+t256(GAME_HEIGHT) ) )
	if (
		(src->x256 < bullet_clip_min.x256)||
		(src->x256 > bullet_clip_max.x256)||
		(src->y256 < bullet_clip_min.y256)||
		(src->y256 > bullet_clip_max.y256) )
	{
		src->type = SP_DELETE;
	}
	#endif
}

/*---------------------------------------------------------
	�e�̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�)
---------------------------------------------------------*/

global void bullet_angle_all_hamidasi_check(void)
{
	int ii;
	for (ii=0; ii<SPRITE_444POOL_MAX; ii++ )/* �S�����ׂ�B */
	{
		SPRITE *s;
		s = &sprite_444pool[ii];
		s_check_hamidasi_bullet_angle_one(s);
	}
}

#endif



/*---------------------------------------------------------
	���ʃ{�X�A������off
	�X�v���C�g���m�ۂ����܂ܑޔ�������B
---------------------------------------------------------*/
global void sprite_initialize_position(SPRITE *h)
{
		h->m_Hit256R					= ZAKO_ATARI16_PNG;
		h->flags				&= (~(SP_FLAG_OPTION_VISIBLE)); 	/* ���t���O��OFF(�s��) */
		h->flags				&= (~(SP_FLAG_COLISION_CHECK)); 	/* �����蔻���OFF(���G) */
		h->type 						= BOSS_00_BOSS11;
		h->callback_mover				= NULL;
		h->callback_loser				= NULL;
		h->callback_hit_enemy			= NULL; 	/* �_�~�[�R�[���o�b�N�o�^ */
		h->x256			= (t256(GAME_WIDTH/2));
		h->y256			= (t256(-256));

}

/*---------------------------------------------------------
	�X�v���C�g�̏�����������B
	-------------------------------------------------------
	���X�g�����łȂ��X�v���C�g������������ׁA
	�O��������Ă΂��̂Œ��ӁB
--------------------------------------------------------- */

global void sprite_initialize_gu(SPRITE *obj)
{
	#if (0==USE_MEM_CLEAR)/* [�������[���N���A�[�@�\]���g��Ȃ��ꍇ�́A���������K�v�B */
	#else
	memset(obj, 0, sizeof(SPRITE));
	#endif
//
	obj->flags		= 0;				/*set_flags*/
	obj->color32	= MAKE32RGBA(0xff, 0xff, 0xff, 0xff);		/*	obj->alpha		= 0xff;*/

	obj->m_zoom_x256			= 256;	/* �\���g�嗦 256 == [ x 1.00 ] */
	#if (0==USE_MEM_CLEAR)/* [�������[���N���A�[�@�\]���g��Ȃ��ꍇ�́A���������K�v�B */
	obj->m_angleCCW1024 		= 0;	/* �`��p�p�x(����0�x�ō����(�����v���)) */	/* 0 == �X���Ȃ��B����0�x */
//
	#if 000/* �����I�ɏ�����@�\ */
	obj->timeover_ticks 		= 0;
	#endif
//
	obj->vx256					= t256(0);
	obj->vy256					= t256(0);
//
	obj->callback_mover 		= NULL;
	obj->callback_loser 		= NULL;
	obj->callback_hit_enemy 	= NULL;
	#endif	/* (0==USE_MEM_CLEAR) */
}


/*---------------------------------------------------------
	gu�ڍs���A�e��p
	�X�v���C�g�����X�g�ɒǉ�����B

	�o�^�ł��Ȃ��ꍇ�ANULL��Ԃ��B

--------------------------------------------------------- */

global SPRITE *sprite_add_only_bullet_error(void)/*int image_resource_num*/
{
	#if 1
	/* private (���̊֐���)�v���C�x�[�g�ȕϐ� */
	static int register_num;	/* �o�^�ł���\�����������Ȉʒu��ێ� */
	#endif
//	if (�o�^�ł��Ȃ��ꍇ)	{	return(NULL);	}
//
//	int priority;		priority		= image_resource_ptr->priority;
//
	SPRITE *obj;/* �V�K�쐬����X�v���C�g */
	int search_count;
	search_count = 0;
my_retry:
		search_count++;
		if ( (SPRITE_444POOL_MAX-1) < search_count)
		{	return(NULL);	}	/* (�o�^�ł��Ȃ��ꍇ) */

		register_num++; 	/* �o�^�ł���\�����������Ȉʒu */
		register_num &= (SPRITE_444POOL_MAX-1);
		obj = &sprite_444pool[register_num];
	//
		if (obj->type != SP_DELETE )/* �g�p�� */
		{
			goto my_retry;	/* �T���B */
		}
//




//	SPRITE *s;
//	SPRITE *tail_obj/* =NULL */;		/* �ǉ����郊�X�g�̌��ʒu�B�ؒf����ꍇ�͐ؒf�ʒu�B */
//	obj 		= my_calloc(sizeof(SPRITE));
//	memset(obj, 0, sizeof(SPRITE));
//	s			= sp rite_list444_head; /* �X�v���C�g���X�g�̐擪�B�Ăяo�� */
//	tail_obj	= NULL;
	/*---------------------------------------------------------
		�X�v���C�g�̕\���v���C�I���e�B�[�𖞂����ʒu�𒲂ׂāA�����ɑ}������B
	--------------------------------------------------------- */
//	while (
//		(NULL != s) /*&&*/				/* ���X�g�̓��܂ł���ׂ��Ȃ�I��� */
//		(s->priority < priority)		/* �\���v���C�I���e�B�[�𖞂����ʒu�܂ŗ�����I��� */
//		)
//	{
//		tail_obj	= s;
//		s			= s->next;
//	}
//	obj->next		= s;				/* �V�K�쐬�����X�v���C�g�̍Ō�ɘA�� */
//	if (NULL == tail_obj)				/* �擪�Ȃ�(���X�g�̐ؒf���Ȃ����) */
//	{
//		/* �擪�̏ꍇ��->next�������̂� */
//		sp rite_list444_head = obj; 	/* �X�v���C�g���X�g�̐擪������������B */
//	}
//	else	//if (NULL != tail_obj)
//	{
//		tail_obj->next		= obj;		/* ���X�g�ɑ}�� */
//	}
	sprite_initialize_gu(obj);			/* �V�K�쐬�����X�v���C�g�������� */
	#if 1
	/* �W�������� */
	obj->flags			|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER*/);	/* �����蔻�肠�� */
	#endif
	return (obj);
}






/*---------------------------------------------------------
	�X�v���C�g �}�l�[�W��(old)
--------------------------------------------------------- */



/*---------------------------------------------------------
	(�G��p)�X�v���C�g�̂����蔻��
--------------------------------------------------------- */

/*static*/global  SPRITE *sprite_list000_head = NULL;	/* �X�v���C�g�̃��X�g�\�� */

//global SPRITE sprite_pool[SPRITE_POOL_MAX];	/* �X�v���C�g�̃��X�g�\�� */

/*static*/global  SPRITE *sprite_collision_check_SDL_teki(SPRITE *tocheck)/*, (SP_GROUP_TEKI)int ty pe*/
{
	SPRITE *s = sprite_list000_head;/* �X�v���C�g ���X�g�̐擪 ����T�� */
	while (NULL != s)
	{
		/* 1. �������g�`�F�b�N */
		/* ����Ώ�(�������g)�͔�΂�(�S���������̓��m�̂����蔻�������Ă��Ӗ����Ȃ��̂Ŏ��Ȃ�) / zu testendes Sprite ueberspringen */
		if (s != tocheck)	/* Is sprites my self?(not need self collisions.) */
		{
			/* 2. �����ރ`�F�b�N */
			if (
				//		(s->type != SP_DELETE ) && /* �폜�ς݂͔�΂� */	/* SP_DELETE�� 0 �ɂȂ����̂ŗv��Ȃ��Ȃ����D */
						(s->type & (SP_GROUP_TEKI)/*type*/) 		 /* type����v����ꍇ�̂� */	/* Passender Sprite-Type? */
				)	/* do collision only same type. */
			{
				/* 3. ����t���O�`�F�b�N */
				if (
						( (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/) == (s->flags&(SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/)))
						/* �����蔻�肪����A���A�\���\�Ȃ��̂̂݃`�F�b�N */
					)	/* do collision only visible and, use collision check type. */
				{
					#if 1
					/* 4. ��`/�~�����蔻��`�F�b�N   ��܂��ɋ�`�Ŕ��ʂ�����A�߂����Ȃ�~�̏Փ˔��� */
					if (collision_hit(s,tocheck))	/* ��`/�~�����蔻�� */ 	/* hit collision rectangle to circle check. */
					{
						return (s);/* �������� */	/* hit collisioning! */
					}
					#endif
				}
			}
		}
		s = s->next;/* �� */
	}
	return (NULL);/* �������ĂȂ� */	/* no hit collisioning. */
}


/*---------------------------------------------------------
	�X�v���C�g ���X�g�ɓo�^���ꂽ�X�v���C�g�������B
--------------------------------------------------------- */
#include <malloc.h>/* free(); */

//extern SPRITE *sprite_list000_head;
static void sprite_remove_one_SDL(SPRITE *src)
{
	SPRITE *s = sprite_list000_head;/* �X�v���C�g ���X�g�̐擪 ����T�� */
	SPRITE *p = NULL;
	SPRITE *n = NULL;
	while (NULL != s)/* �X�v���C�g ���X�g�̏I���܂ŒT�� */
	{
		n = s->next;
		if (s == src)/* ���������H */
		{
			if (NULL == p)
			{
				sprite_list000_head = n;	/* �X�v���C�g���X�g�̐擪������������B */
			}
			else
			{
				p->next = n;
			}
			free (s);
			#if 0
			s = NULL;/* �O�̈חv��H */
			#endif
			return;/* ���� */
		}
		p = s;
		s = n;
	}
	CHECKPOINT;/* �ُ� */
	error(ERR_WARN, (char*)"sprite not found in list");
}

/*---------------------------------------------------------
	�X�v���C�g ���X�g�ɓo�^���ꂽ�X�v���C�g��S�������B
--------------------------------------------------------- */

global void sprite_remove_all_SDL_void(void)/*int ty pe*/
{
	SPRITE *s = sprite_list000_head;/* �X�v���C�g ���X�g�̐擪 ����T�� */
	SPRITE *n = NULL;
	while (NULL != s)
	{
		n = s->next;
		if (s->type & (SP_GROUP_ALL_GAME_OBJS)/*type*/) /* type����v����ꍇ�̂� */
		{
			sprite_remove_one_SDL(s);
		}
		s = n;
	}
}

/*---------------------------------------------------------
	�X�v���C�g����t���[�������삳����B
--------------------------------------------------------- */

global void sprite_move_main_SDL_222(void)/*int ty pe*/
{
	SPRITE *s;
	SPRITE *n;
	s = sprite_list000_head;/* �X�v���C�g ���X�g�̐擪 ����T�� */
	while (NULL != s)
	{
		n = s->next;
		#if 0
		if (s->type != SP_DELETE ) /* �폜�ς݂͔�΂� */	/* SP_DELETE�� 0 �ɂȂ����̂ŗv��Ȃ��Ȃ����D */
		#endif
		{
			if (s->type & (SP_GROUP_ALL_SDL_CORE_TYPE)/*type*/) /* type����v����ꍇ�̂� */
			{
				if (NULL != s->callback_mover)
				{
					(s->callback_mover)(s);
				}
			}
		}
		s = n;
	}
	#if 1
	/* �g�p�ς݃X�v���C�g��T���āA�g�p�ς݃X�v���C�g������Ώ��� */
	#if 00/* �Ƃ肠�������ݖ��g�p�����A���j���[��obj�g���ꍇ�͒��� */
//	if ( S T_WORK_MENU != (ps p_loop&0xff00) )/* ���j���[�ȊO�̏ꍇ�A(�|�[�Y���j���[�Ŏ��Ԃ��o�ꍇ�ɏ�����ƍ���̂�) */
	if ( S T_INIT_MENU_ST_MENU_SUB_MAIN_MENU > (ps p_loop&0xff00) )/* ���j���[�ȊO�̏ꍇ�A(�|�[�Y���j���[�Ŏ��Ԃ��o�ꍇ�ɏ�����ƍ���̂�) */
/* �Ƃ肠���� */
	#endif
	{
		s = sprite_list000_head;/* �X�v���C�g ���X�g�̐擪 ����T�� */
		while (NULL != s)
		{
			n = s->next;
			#if 000/* �����I�ɏ�����@�\ */
			/* ��莞�Ԃł̎��������A���t���OON�̏ꍇ�A */
			if ( (SP_FLAG_TIME_OVER == (s->flags&SP_FLAG_TIME_OVER)) )
			{
				s->timeover_ticks++;
				if ( 1000 < (s->timeover_ticks) )/* 16�b */ 	/* (1024-1) < */ /* (0x0400 & (s->ticks)) */
				{
				//	s->timeover_ticks = 0;
					s->type = SP_DELETE;	/* ���������ɂ���B */
				}
			}
			#endif
			if (SP_DELETE == s->type)	/* �����H */
			{
				sprite_remove_one_SDL(s);	/* �폜�ς݂����� */
			}
			s = n;
		}
	}
	#endif
}

/*---------------------------------------------------------
	gu�ڍs���A�ėp�X�v���C�g�����X�g�ɒǉ�����B

	�o�^�ł��Ȃ��ꍇ�ANULL��Ԃ��B

--------------------------------------------------------- */
extern void sprite_initialize_gu(SPRITE *obj);
global SPRITE *sprite_add_gu_error(void)
{
//	if (�o�^�ł��Ȃ��ꍇ)	{	return(NULL);	}
//
//	int priority;		priority		= image_resource_ptr->priority;
//
	SPRITE *obj;/* �V�K�쐬����X�v���C�g */
	SPRITE *s;
	SPRITE *tail_obj/* =NULL */;		/* �ǉ����郊�X�g�̌��ʒu�B�ؒf����ꍇ�͐ؒf�ʒu�B */
	obj 		= my_calloc(sizeof(SPRITE));
//	memset(obj, 0, sizeof(SPRITE));
	s			= sprite_list000_head;	/* �X�v���C�g���X�g�̐擪�B�Ăяo�� */
	tail_obj	= NULL;
	/*---------------------------------------------------------
		�X�v���C�g�̕\���v���C�I���e�B�[�𖞂����ʒu�𒲂ׂāA�����ɑ}������B
	--------------------------------------------------------- */
	while (
		(NULL != s) /*&&*/				/* ���X�g�̓��܂ł���ׂ��Ȃ�I��� */
//		(s->priority < priority)		/* �\���v���C�I���e�B�[�𖞂����ʒu�܂ŗ�����I��� */
		)
	{
		tail_obj	= s;
		s			= s->next;
	}
	obj->next		= s;				/* �V�K�쐬�����X�v���C�g�̍Ō�ɘA�� */
	if (NULL == tail_obj)				/* �擪�Ȃ�(���X�g�̐ؒf���Ȃ����) */
	{
		/* �擪�̏ꍇ��->next�������̂� */
		sprite_list000_head = obj;		/* �X�v���C�g���X�g�̐擪������������B */
	}
	else	//if (NULL != tail_obj)
	{
		tail_obj->next		= obj;		/* ���X�g�ɑ}�� */
	}
	sprite_initialize_gu(obj);			/* �V�K�쐬�����X�v���C�g�������� */
	return (obj);
}

