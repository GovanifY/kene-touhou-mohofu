
/*---------------------------------------------------------
	�X�v���C�g �}�l�[�W��
--------------------------------------------------------- */

#include "game_main.h"

/*---------------------------------------------------------
	���\�[�X resource
--------------------------------------------------------- */
static u8 my_bullet_resource_atari_hankei[] =
{
#if 1
	/* [�L�閼�O��] */
	//#define DU MMY_IMG "te ki/bu llet_maru16.png"
//	#define DU MMY_IMG	"ji ki/co re_re.png"

	#if 0
	/* 77 */	//	   "teki/kugel.png",						   /*0,*/	 2 ,
	/* 78 */	//	   "teki/kugel2.png",						   /*0,*/	 2 ,
	/* 79 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/	 2 , /* �j�e (�G�e) */
	/* 80 */		   /* "teki/bullet_maru16.png"*/			   /*0,*/	 2 , /* ����(��) (�G�e) */	  /* �Ԋ�(��) (�G�e) */
//
	/* 81 */		   /* "teki/bullet_ming32.png"*/			   /*0,*/	 2 , /* �䂩���e (�G�e) */
	/* 82 */		   /* "teki/jippou32.png"*/ 				   /*0,*/	 2 , /* ���͋[�e (�G�e) */
//
					  /* �ǉ��\�� */											 /* �N�i�C�e (�G�e) */
//
	/* 83 */		   /* "teki/oodama08.png"*/ 				   /*1,*/	 4 ,/* ��e(����...)	  ����(��) PRIORITY_03_ENEMY �́A�����蔻�蕔�� */
	/* 83 */	//	   "teki/bigkugel2.png",					   /*0,*/	 2 ,/* ����(��)   �����蔻�蕔�� */
	/* 84 */	//	   "teki/bigkugel1.png",					   /*1,*/	 2 ,/* ��e(��) �\������ */
	/* 85 */	//	   "teki/new_bigkugel.png", 				   /*1,*/	 2 ,/* ��e(��) �\������ */
//
	/* 86 */		   /* "teki/knife.png"*/					   /*1,*/	 2 ,/* �����~���i�C�t(��) */
	/* 87 */		   /* "teki/knife_core16.png"*/ 			   /*1,*/	 2 ,/* �S�����i�C�t(��) */
	#else
	/*	0 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (2), /* 2 */
	/*	1 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (4), /* 4 */
	#endif
	/* 103 */		   /*"teki/homing16.png"*/					   /*0,*/  (2), /* �U�R �U���e */
//
	/*	3 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/ (16), /* 4 */ 	  //  JIKI_ATARI_ITEM_16,
	/*	4 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/ (80), /* 4 */ 	  //  JIKI_ATARI_ITEM_80,/* ��{�� */
//
	/*	5 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (2),   /* re a */  //  BASE_BOMBER_ATARI_0a,/*4*/
	/*	5 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (2),   /* re b */  //  BASE_BOMBER_ATARI_0a,/*4*/
	/*	6 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/ (16),   /* ma a */  //  BASE_BOMBER_ATARI_0b,/*16*/
	/*	6 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/ (16),   /* ma b */  //  BASE_BOMBER_ATARI_0b,/*16*/
	/*	7 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/ (16),   /* oz / */  //  BASE_BOMBER_ATARI_0c,/*16*/
	/*	9 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (4),   /* yu / */  //  BASE_BOMBER_ATARI_0e,/*4*/
	/*	8 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/ (32),   /* ci a */  //  BASE_BOMBER_ATARI_0d,/*32*/
	/*	6 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/ (32),   /* ci q */  //  BASE_BOMBER_ATARI_0b,/*16*/
//
	/*	5 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (4),  /* re a */  //  BASE_SHOT_ATARI_0a,/*4*/
	/*	5 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (4),  /* re b */  //  BASE_SHOT_ATARI_0a,/*4*/
	/*	6 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (4),  /* ma a */  //  BASE_SHOT_ATARI_0b,/*4*/
	/*	6 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (4),  /* ma b */  //  BASE_SHOT_ATARI_0b,/*4*/
	/*	7 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (4),  /* oz / */  //  BASE_SHOT_ATARI_0c,/*4*/
	/*	9 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (8),  /* yu / */  //  BASE_SHOT_ATARI_0e,/*8*/
	/*	8 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (6),  /* ci a */  //  BASE_SHOT_ATARI_0d,/*6*/
	/*	6 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (6),  /* ci q */  //  BASE_SHOT_ATARI_0b,/*4*/
//
	/*	5 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (6),   /* re a */  //  BASE_NEEDLE_ATARI_0a,/*6*/
	/*	5 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (6),   /* re b */  //  BASE_NEEDLE_ATARI_0a,/*6*/
	/*	6 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (8),   /* ma a */  //  BASE_NEEDLE_ATARI_0b,/*8*/
	/*	6 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (8),   /* ma b */  //  BASE_NEEDLE_ATARI_0b,/*8*/
	/*	7 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/ (12),   /* oz / */  //  BASE_NEEDLE_ATARI_0c,/*12*/
	/*	9 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (8),   /* yu / */  //  BASE_NEEDLE_ATARI_0e,/*8*/
	/*	8 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/ (16),   /* ci a */  //  BASE_NEEDLE_ATARI_0d,/*16*/
	/*	6 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/ (16),   /* ci q */  //  BASE_NEEDLE_ATARI_0b,/*8*/
//
	/*	5 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (1),  /* re a */  //  BASE_CORE_ATARI_0a,/*6*/
	/*	5 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (1),  /* re b */  //  BASE_CORE_ATARI_0a,/*6*/
	/*	6 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (2),  /* ma a */  //  BASE_CORE_ATARI_0b,/*8*/
	/*	6 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (2),  /* ma b */  //  BASE_CORE_ATARI_0b,/*8*/
	/*	7 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (2),  /* oz / */  //  BASE_CORE_ATARI_0c,/*12*/
	/*	9 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (2),  /* yu / */  //  BASE_CORE_ATARI_0e,/*8*/
	/*	8 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (1),  /* ci a */  //  BASE_CORE_ATARI_0d,/*16*/
	/*	6 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (1),  /* ci q */  //  BASE_CORE_ATARI_0b,/*8*/
#endif
};

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
	if (dx < 0/* .0 */) {	dx = (-dx); }	/* dx = abs(dx); */
	if (dx > dr)	{	return (0/* FALSE */);	}	/* not hit, because not near the dx. �w�̍������傫���̂ŁA�������ĂȂ� */

	/* check collision, delta y position. ��`����i�x���j */
	if (dy < 0/* .0 */) {	dy = (-dy); }	/* dy = abs(dy); */
	if (dy > dr)	{	return (0/* FALSE */);	}	/*	not hit, because not near the dy. �x�̍������傫���̂ŁA�������ĂȂ� */

	/* check collision, circle delta round distance.  �~�̔��a�Ŕ��� */
	dr *= dr;
	dx *= dx;
	dy *= dy;
	dx += dy;
	if (dr < dx)/* if ( (dr^2) < ( (dx^2) + (dy^2) )) */
	{
		return (0/* FALSE */);	/* not hit. �������ĂȂ� */
	}
	return (1/* TRUE */); /* hit! �������� */
}
#endif


/*---------------------------------------------------------
	(�G��p)�X�v���C�g�̂����蔻��
--------------------------------------------------------- */

SPRITE *sprite_list000_head = NULL; 	/* �X�v���C�g�̃��X�g�\�� */

//SPRITE sprite_pool[SPRITE_POOL_MAX];	/* �X�v���C�g�̃��X�g�\�� */

SPRITE *sprite_collision_check_SDL_teki(SPRITE *tocheck)/*, (SP_GROUP_TEKI)int type*/
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
						( (SP_FLAG_COLISION_CHECK|SP_FLAG_VISIBLE) == (s->flags&(SP_FLAG_COLISION_CHECK|SP_FLAG_VISIBLE)))
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
extern SPRITE *sprite_list000_head;
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
//			if (0==(s->flags&SP_FLAG_NOT_CACHE))
//			{
//				unloadbmp_by_surface(s->sprite_bmp);
//			}
			#if 0
		//	if ((s->flags&SP_FLAG_FREE_SURFACE))
			if ((s->flags&(SP_FLAG_FREE_SURFACE|SP_FLAG_NOT_CACHE)))
			{
				/* KETM�ł����͖����ɂȂ��Ă邪�A
				�����������ɂȂ��Ă����(�_���\���p�t�H���g��)
				���X���C����������������Ⴄ�̂ŁA
				�J�����Ă݂�e�X�g */
				/* �����Ńn���O�A�b�v */
				SDL_FreeSurface(s->sprite_bmp);
			}
			#endif
		//	#if (1==USE_ZUKEI_ATARI_HANTEI)
		//	sprite_remove_colision_map(s->colision_bmp);
		//	#endif /* (1==USE_ZUKEI_ATARI_HANTEI) */
			if (NULL != s->data)
			{
				free (s->data);
			}
			free (s);
			return;/* ���� */
		}
		p = s;
		s = n;
	}
	CHECKPOINT;/* �ُ� */
	error(ERR_WARN, "sprite not found in list");
}

/*---------------------------------------------------------
	�X�v���C�g ���X�g�ɓo�^���ꂽ�X�v���C�g��S�������B
--------------------------------------------------------- */

//void sprite_remove_all222(int type){}
void sprite_remove_all_SDL(int type)
{
	SPRITE *s = sprite_list000_head;/* �X�v���C�g ���X�g�̐擪 ����T�� */
	SPRITE *n = NULL;
	while (NULL != s)
	{
		n = s->next;
		if (s->type & type) /* type����v����ꍇ�̂� */
		{
			sprite_remove_one_SDL(s);
		}
		s = n;
	}
}

//void sprite_remove_all_type(int type)
//{
//	SPRITE *s = sprite;
//	SPRITE *n = NULL;
//	while (NULL != s)
//	{
//		n = s->next;
//		if (s->type == type)
//		{
//			sprite_remove(s);
//		}
//		s = n;
//	}
//}

/*---------------------------------------------------------
	�X�v���C�g����t���[�������삳����B
--------------------------------------------------------- */

//void sprite_work222(int type){}
void sprite_work_SDL(int type)
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
			if (s->type & type) /* type����v����ꍇ�̂� */
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
	if ( ST_WORK_MENU != (psp_loop&0xff00) )/* ���j���[�ȊO�̏ꍇ�A(�|�[�Y���j���[�Ŏ��Ԃ��o�ꍇ�ɏ�����ƍ���̂�) */
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

--------------------------------------------------------- */

SPRITE *sprite_add_gu(int image_resource_num)
{
	int m_Hit256R;		m_Hit256R		= ((my_bullet_resource_atari_hankei[image_resource_num])<<8);
//
	//int priority; 		priority		= image_resource_ptr->priority;
//
	SPRITE *obj;/* �V�K�쐬����X�v���C�g */
	SPRITE *s;
	SPRITE *tail_obj/* =NULL */; /* �ǉ����郊�X�g�̌��ʒu�B�ؒf����ꍇ�͐ؒf�ʒu�B */
	obj 		= mmalloc(sizeof(SPRITE));
//	memset(obj, 0, sizeof(SPRITE));
	s			= sprite_list000_head;	/* �X�v���C�g���X�g�̐擪�B�Ăяo�� */
	tail_obj	= NULL;
	/*---------------------------------------------------------
		�X�v���C�g�̕\���v���C�I���e�B�[�𖞂����ʒu�𒲂ׂāA�����ɑ}������B
	--------------------------------------------------------- */
	while (
		(NULL != s) /*&&*/				/* ���X�g�̓��܂ł���ׂ��Ȃ�I��� */
	//	(s->priority < priority)	/* �\���v���C�I���e�B�[�𖞂����ʒu�܂ŗ�����I��� */
		)
	{
		tail_obj	= s;
		s			= s->next;
	}
	obj->next		= s;	/* �V�K�쐬�����X�v���C�g�̍Ō�ɘA�� */
	if (NULL == tail_obj)/* �擪�Ȃ�(���X�g�̐ؒf���Ȃ����) */
	{
		/* �擪�̏ꍇ��->next�������̂� */
		sprite_list000_head = obj;		/* �X�v���C�g���X�g�̐擪������������B */
	}
	else	//if (NULL != tail_obj)
	{
		tail_obj->next		= obj;	/* ���X�g�ɑ}�� */
	}
	/*---------------------------------------------------------
		�X�v���C�g�̐ݒ������B
	--------------------------------------------------------- */
	#if 0
	{	/* ���̋@�\(�ʂ̃X�v���C�g�ɔԍ������蓖�Ĕԍ��Ŕ��ʂ���)�͑債���Ӗ����Ȃ��̂Ŗ��������B
			(�ʂ̃X�v���C�g�𔻕ʂ������ꍇ�́A�|�C���^�Ŕ��ʂ���΂��̋@�\�͕s�v�Ȃ̂�) */
		static int current_id	= 0;
		obj->id 				= current_id;
		current_id++;
	}
	#endif

	/*set_atari_hankei*/ //image_resource_ptr->atari_hankei;
	obj->m_Hit256R	= m_Hit256R;

//	obj->frames 	= total_frames;
//	obj->priority	= priority;
	obj->flags		= 0;//set_flags; /* 0;	if (nocache) s->flags |= SP_FLAG_NOT_CACHE; */
	obj->color32	= 0xffffffff;		/*	obj->alpha		= 0xff;*/

	obj->m_zoom_x256			= 256;	/* �\���g�嗦 256 == [ x 1.00 ] */
	#if (0==USE_MEM_CLEAR)/* [�������[���N���A�[�@�\]���g��Ȃ��ꍇ�́A���������K�v�B */
//	obj->yx_an im_frame = 0x00;
//	obj->anim_count 	= 0;
	obj->m_angleCCW512	= 0;			/* �`��p�p�x(����0�x�ō����(�����v���)) */	/* 0 == �X���Ȃ��B����0�x */
//
	#if 000/* �����I�ɏ�����@�\ */
	obj->timeover_ticks 		= 0;
	#endif
//
	obj->vx256					= t256(0);
	obj->vy256					= t256(0);
//
	obj->data					= NULL;
	obj->callback_mover 		= NULL;
	obj->callback_loser 		= NULL;
	obj->callback_hit_enemy 	= NULL;
	#endif	/* (0==USE_MEM_CLEAR) */
	return (obj);
}

