
/*---------------------------------------------------------
	�X�v���C�g �}�l�[�W��
--------------------------------------------------------- */

#include "game_main.h"

/*---------------------------------------------------------
	���\�[�X resource
--------------------------------------------------------- */
static IMAGE_RESOURCE my_bullet_resource[] =
{
#if 1
	/* [�L�閼�O��] */
	//#define DUMMY_IMG "teki/bullet_maru16.png"
	#define DUMMY_IMG	"jiki/core_re.png"

	#if 0
	/* 77 */	//	{	"teki/kugel.png",							0,	 1,  /*iyx( 1,   1),*/ PRIORITY_05_BULLETS,    2 },
	/* 78 */	//	{	"teki/kugel2.png",							0,	 1,  /*iyx( 1,   1),*/ PRIORITY_05_BULLETS,    2 },
	/* 79 */		{	/* "teki/bullet_beam16.png"*/DUMMY_IMG, 	0,	16,  /*iyx(16,   1),*/ PRIORITY_05_BULLETS,    2 },	/* �j�e (�G�e) */
	/* 80 */		{	/* "teki/bullet_maru16.png"*/DUMMY_IMG, 	0,	16,  /*iyx(16,   1),*/ PRIORITY_05_BULLETS,    2 },	/* ����(��) (�G�e) */	/* �Ԋ�(��) (�G�e) */
//
	/* 81 */		{	/* "teki/bullet_ming32.png"*/DUMMY_IMG, 	0,	32,  /*iyx(32,   1),*/ PRIORITY_05_BULLETS,    2 },	/* �䂩���e (�G�e) */
	/* 82 */		{	/* "teki/jippou32.png"*/DUMMY_IMG,			0,	32,  /*iyx(32,   1),*/ PRIORITY_05_BULLETS,    2 },	/* ���͋[�e (�G�e) */
//
					/* �ǉ��\�� */																				/* �N�i�C�e (�G�e) */
//
	/* 83 */		{	/* "teki/oodama08.png"*/DUMMY_IMG,			1,	 8,  /*iyx( 8,   1),*/ PRIORITY_05_BULLETS,    4 },/* ��e(����...)	����(��) PRIORITY_03_ENEMY �́A�����蔻�蕔�� */
	/* 83 */	//	{	"teki/bigkugel2.png",						0,	 1,  /*iyx( 1,   1),*/ PRIORITY_03_ENEMY,	   2 },/* ����(��)  �����蔻�蕔�� */
	/* 84 */	//	{	"teki/bigkugel1.png",						1,	 1,  /*iyx( 1,   1),*/ PRIORITY_05_BULLETS,    2 },/* ��e(��) �\������ */
	/* 85 */	//	{	"teki/new_bigkugel.png",					1,	 1,  /*iyx( 1,   1),*/ PRIORITY_05_BULLETS,    2 },/* ��e(��) �\������ */
//
	/* 86 */		{	/* "teki/knife.png"*/DUMMY_IMG, 			1,	 1,  /*iyx( 1,   1),*/ PRIORITY_05_BULLETS,    2 },/* �����~���i�C�t(��) */
	/* 87 */		{	/* "teki/knife_core16.png"*/DUMMY_IMG,		1,	16,  /*iyx(16,   1),*/ PRIORITY_05_BULLETS,    2 },/* �S�����i�C�t(��) */
	#else
	/*	0 */		{	/* "teki/bullet_beam16.png"*/DUMMY_IMG, 	0,	 1,  /*iyx( 1,   1),*/ PRIORITY_05_BULLETS,     (2) }, /* 2 */
	/*	1 */		{	/* "teki/bullet_beam16.png"*/DUMMY_IMG, 	0,	 1,  /*iyx( 1,   1),*/ PRIORITY_05_BULLETS,     (4) }, /* 4 */
	#endif
	/* 103 */		{	/*"teki/homing16.png"*/DUMMY_IMG,			0,	16,  /*iyx(16,   1),*/ PRIORITY_05_BULLETS,     (2) }, /* �U�R �U���e */
//
	/*	3 */		{	/* "teki/bullet_beam16.png"*/DUMMY_IMG, 	0,	 1,  /*iyx( 1,   1),*/ PRIORITY_05_BULLETS,    (16) }, /* 4 */ 		//	JIKI_ATARI_ITEM_16,
	/*	4 */		{	/* "teki/bullet_beam16.png"*/DUMMY_IMG, 	0,	 1,  /*iyx( 1,   1),*/ PRIORITY_05_BULLETS,    (80) }, /* 4 */ 		//	JIKI_ATARI_ITEM_80,/* ��{�� */
//
	/*	5 */		{	/* "teki/bullet_beam16.png"*/DUMMY_IMG, 	0,	 1,  /*iyx( 1,   1),*/ PRIORITY_05_BULLETS,     (2) }, 	/* re a */	//	BASE_BOMBER_ATARI_0a,/*4*/
	/*	5 */		{	/* "teki/bullet_beam16.png"*/DUMMY_IMG, 	0,	 1,  /*iyx( 1,   1),*/ PRIORITY_05_BULLETS,     (2) }, 	/* re b */	//	BASE_BOMBER_ATARI_0a,/*4*/
	/*	6 */		{	/* "teki/bullet_beam16.png"*/DUMMY_IMG, 	0,	 1,  /*iyx( 1,   1),*/ PRIORITY_05_BULLETS,    (16) }, 	/* ma a */	//	BASE_BOMBER_ATARI_0b,/*16*/
	/*	6 */		{	/* "teki/bullet_beam16.png"*/DUMMY_IMG, 	0,	 1,  /*iyx( 1,   1),*/ PRIORITY_05_BULLETS,    (16) }, 	/* ma b */	//	BASE_BOMBER_ATARI_0b,/*16*/
	/*	7 */		{	/* "teki/bullet_beam16.png"*/DUMMY_IMG, 	0,	 1,  /*iyx( 1,   1),*/ PRIORITY_05_BULLETS,    (16) }, 	/* oz / */	//	BASE_BOMBER_ATARI_0c,/*16*/
	/*	9 */		{	/* "teki/bullet_beam16.png"*/DUMMY_IMG, 	0,	 1,  /*iyx( 1,   1),*/ PRIORITY_05_BULLETS,     (4) }, 	/* yu / */	//	BASE_BOMBER_ATARI_0e,/*4*/
	/*	8 */		{	/* "teki/bullet_beam16.png"*/DUMMY_IMG, 	0,	 1,  /*iyx( 1,   1),*/ PRIORITY_05_BULLETS,    (32) }, 	/* ci a */	//	BASE_BOMBER_ATARI_0d,/*32*/
	/*	6 */		{	/* "teki/bullet_beam16.png"*/DUMMY_IMG, 	0,	 1,  /*iyx( 1,   1),*/ PRIORITY_05_BULLETS,    (32) }, 	/* ci q */	//	BASE_BOMBER_ATARI_0b,/*16*/
//
	/*	5 */		{	/* "teki/bullet_beam16.png"*/DUMMY_IMG, 	0,	 1,  /*iyx( 1,   1),*/ PRIORITY_05_BULLETS,     (4) }, 	/* re a */	//	BASE_SHOT_ATARI_0a,/*4*/
	/*	5 */		{	/* "teki/bullet_beam16.png"*/DUMMY_IMG, 	0,	 1,  /*iyx( 1,   1),*/ PRIORITY_05_BULLETS,     (4) }, 	/* re b */	//	BASE_SHOT_ATARI_0a,/*4*/
	/*	6 */		{	/* "teki/bullet_beam16.png"*/DUMMY_IMG, 	0,	 1,  /*iyx( 1,   1),*/ PRIORITY_05_BULLETS,     (4) }, 	/* ma a */	//	BASE_SHOT_ATARI_0b,/*4*/
	/*	6 */		{	/* "teki/bullet_beam16.png"*/DUMMY_IMG, 	0,	 1,  /*iyx( 1,   1),*/ PRIORITY_05_BULLETS,     (4) }, 	/* ma b */	//	BASE_SHOT_ATARI_0b,/*4*/
	/*	7 */		{	/* "teki/bullet_beam16.png"*/DUMMY_IMG, 	0,	 1,  /*iyx( 1,   1),*/ PRIORITY_05_BULLETS,     (4) }, 	/* oz / */	//	BASE_SHOT_ATARI_0c,/*4*/
	/*	9 */		{	/* "teki/bullet_beam16.png"*/DUMMY_IMG, 	0,	 1,  /*iyx( 1,   1),*/ PRIORITY_05_BULLETS,     (8) }, 	/* yu / */	//	BASE_SHOT_ATARI_0e,/*8*/
	/*	8 */		{	/* "teki/bullet_beam16.png"*/DUMMY_IMG, 	0,	 1,  /*iyx( 1,   1),*/ PRIORITY_05_BULLETS,     (6) }, 	/* ci a */	//	BASE_SHOT_ATARI_0d,/*6*/
	/*	6 */		{	/* "teki/bullet_beam16.png"*/DUMMY_IMG, 	0,	 1,  /*iyx( 1,   1),*/ PRIORITY_05_BULLETS,     (6) }, 	/* ci q */	//	BASE_SHOT_ATARI_0b,/*4*/
//
	/*	5 */		{	/* "teki/bullet_beam16.png"*/DUMMY_IMG, 	0,	 1,  /*iyx( 1,   1),*/ PRIORITY_05_BULLETS,     (6) }, 	/* re a */	//	BASE_NEEDLE_ATARI_0a,/*6*/
	/*	5 */		{	/* "teki/bullet_beam16.png"*/DUMMY_IMG, 	0,	 1,  /*iyx( 1,   1),*/ PRIORITY_05_BULLETS,     (6) }, 	/* re b */	//	BASE_NEEDLE_ATARI_0a,/*6*/
	/*	6 */		{	/* "teki/bullet_beam16.png"*/DUMMY_IMG, 	0,	 1,  /*iyx( 1,   1),*/ PRIORITY_05_BULLETS,     (8) }, 	/* ma a */	//	BASE_NEEDLE_ATARI_0b,/*8*/
	/*	6 */		{	/* "teki/bullet_beam16.png"*/DUMMY_IMG, 	0,	 1,  /*iyx( 1,   1),*/ PRIORITY_05_BULLETS,     (8) }, 	/* ma b */	//	BASE_NEEDLE_ATARI_0b,/*8*/
	/*	7 */		{	/* "teki/bullet_beam16.png"*/DUMMY_IMG, 	0,	 1,  /*iyx( 1,   1),*/ PRIORITY_05_BULLETS,    (12) }, 	/* oz / */	//	BASE_NEEDLE_ATARI_0c,/*12*/
	/*	9 */		{	/* "teki/bullet_beam16.png"*/DUMMY_IMG, 	0,	 1,  /*iyx( 1,   1),*/ PRIORITY_05_BULLETS,     (8) }, 	/* yu / */	//	BASE_NEEDLE_ATARI_0e,/*8*/
	/*	8 */		{	/* "teki/bullet_beam16.png"*/DUMMY_IMG, 	0,	 1,  /*iyx( 1,   1),*/ PRIORITY_05_BULLETS,    (16) }, 	/* ci a */	//	BASE_NEEDLE_ATARI_0d,/*16*/
	/*	6 */		{	/* "teki/bullet_beam16.png"*/DUMMY_IMG, 	0,	 1,  /*iyx( 1,   1),*/ PRIORITY_05_BULLETS,    (16) }, 	/* ci q */	//	BASE_NEEDLE_ATARI_0b,/*8*/
//
	/*	5 */		{	/* "teki/bullet_beam16.png"*/DUMMY_IMG, 	0,	 1,  /*iyx( 1,   1),*/ PRIORITY_05_BULLETS,     (1) }, 	/* re a */	//	BASE_CORE_ATARI_0a,/*6*/
	/*	5 */		{	/* "teki/bullet_beam16.png"*/DUMMY_IMG, 	0,	 1,  /*iyx( 1,   1),*/ PRIORITY_05_BULLETS,     (1) }, 	/* re b */	//	BASE_CORE_ATARI_0a,/*6*/
	/*	6 */		{	/* "teki/bullet_beam16.png"*/DUMMY_IMG, 	0,	 1,  /*iyx( 1,   1),*/ PRIORITY_05_BULLETS,     (2) }, 	/* ma a */	//	BASE_CORE_ATARI_0b,/*8*/
	/*	6 */		{	/* "teki/bullet_beam16.png"*/DUMMY_IMG, 	0,	 1,  /*iyx( 1,   1),*/ PRIORITY_05_BULLETS,     (2) }, 	/* ma b */	//	BASE_CORE_ATARI_0b,/*8*/
	/*	7 */		{	/* "teki/bullet_beam16.png"*/DUMMY_IMG, 	0,	 1,  /*iyx( 1,   1),*/ PRIORITY_05_BULLETS,     (2) }, 	/* oz / */	//	BASE_CORE_ATARI_0c,/*12*/
	/*	9 */		{	/* "teki/bullet_beam16.png"*/DUMMY_IMG, 	0,	 1,  /*iyx( 1,   1),*/ PRIORITY_05_BULLETS,     (2) }, 	/* yu / */	//	BASE_CORE_ATARI_0e,/*8*/
	/*	8 */		{	/* "teki/bullet_beam16.png"*/DUMMY_IMG, 	0,	 1,  /*iyx( 1,   1),*/ PRIORITY_05_BULLETS,     (1) }, 	/* ci a */	//	BASE_CORE_ATARI_0d,/*16*/
	/*	6 */		{	/* "teki/bullet_beam16.png"*/DUMMY_IMG, 	0,	 1,  /*iyx( 1,   1),*/ PRIORITY_05_BULLETS,     (1) }, 	/* ci q */	//	BASE_CORE_ATARI_0b,/*8*/
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
	dx = ((obj2->x256+obj2->w128) - (obj1->x256+obj1->w128));
	dy = ((obj2->y256+obj2->h128) - (obj1->y256+obj1->h128));
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
	�X�v���C�g�̂����蔻��
--------------------------------------------------------- */

SPRITE *sprite_list000_head = NULL; 	/* �X�v���C�g�̃��X�g�\�� */

//SPRITE sprite_pool[SPRITE_POOL_MAX];	/* �X�v���C�g�̃��X�g�\�� */

SPRITE *sprite_collision_check(SPRITE *tocheck, int type)
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
						(s->type & type)			 /* type����v����ꍇ�̂� */	/* Passender Sprite-Type? */
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
/* �e�����X�g�͕�������\�� */
SPRITE *sprite_collision_check_bullets(SPRITE *tocheck)
{
	return (sprite_collision_check(tocheck, SP_GROUP_BULLETS));/*�Ƃ肠����*/
}

/*---------------------------------------------------------
	���Ă̓G�e���Ahosi�A�C�e���ɕς���
---------------------------------------------------------*/
extern SPRITE *sprite_list000_head;
//void item_from_bullets(int put_item_num)
extern void item_convert_hosi(SPRITE */*src*/h);
void bullets_to_hosi(void)
{
	SPRITE *s = sprite_list000_head;/* �X�v���C�g ���X�g�̐擪 ����T�� */
	while (NULL != s)/* �X�v���C�g ���X�g�̍Ō�܂Œ��ׂ� */
	{
		if (SP_GROUP_BULLETS & s->type)
		{
		//	item_create(s, /*put_item_num*/SP_IT EM_06_HOSI/*SP_IT EM_06_HOSI*/, 1, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY) );
		//	s->type = SP_DELETE;
		//	s->type = SP_IT EM_06_HOSI;
			item_convert_hosi(s);//, /*put_item_num*/SP_IT EM_06_HOSI/*SP_IT EM_06_HOSI*/, 1, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY) );
		}
		s = s->next;/*��*/
	}
}

/*---------------------------------------------------------
	�X�v���C�g ���X�g�ɓo�^���ꂽ�X�v���C�g�������B
--------------------------------------------------------- */
extern SPRITE *sprite_list000_head;
static void sprite_remove_one(SPRITE *src)
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
			if (0==(s->flags&SP_FLAG_NOT_CACHE))
			{
				unloadbmp_by_surface(s->sprite_bmp);
			}
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
void sprite_remove_all000(int type)
{
	SPRITE *s = sprite_list000_head;/* �X�v���C�g ���X�g�̐擪 ����T�� */
	SPRITE *n = NULL;
	while (NULL != s)
	{
		n = s->next;
		if (s->type & type) /* type����v����ꍇ�̂� */
		{
			sprite_remove_one(s);
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
void sprite_work000(int type)
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
			/* animate */
			//s->ticks++;
			if (s->type & type) /* type����v����ꍇ�̂� */
			{
				#if 0000/* �A�j���p�~ */
				/* �t�]�A�j���֎~�ɕύX */
			//	if (s->anim_speed != 0)
				if (0 < s->anim_speed)
				{
					s->anim_count++;			//		s->anim_count++/* =fps_fa ctor */;
					// if (s->anim_count==abs(s->anim_speed))
					if (s->anim_count >= /* abs */(s->anim_speed))
					{
						s->anim_count = 0;
					//	if (s->anim_speed > 0)
						{
							s->yx_an im_frame += 0x01;
							if (((s->yx_an im_frame)&YX_FRAME_LOW_X) >= ((s->yx_frames)&YX_FRAME_LOW_X))
							{
								#if 0
							//	s->yx_an im_frame -= 0x01;
							//	s->yx_an im_frame += 0x10;
								#else
								s->yx_an im_frame += 0x0f;
								#endif
							}
								if (s->yx_an im_frame >= s->yx_frames)
								{	s->yx_an im_frame = 0;}
						}
					//	else		/* �t�]�A�j���֎~�ɕύX */
					//	{
					//		s->an im_frame--;
					//		if (s->an im_frame < 0)
					//		{	s->an im_frame = s->frames-1;}
					//	}
					}
				}
				#endif
				/* move */
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
				sprite_remove_one(s);	/* �폜�ς݂����� */
			}
			s = n;
		}
	}
	#endif
}

/*---------------------------------------------------------
	�X�v���C�g��`�悷��B
--------------------------------------------------------- */
static void sprite_draw000(SPRITE *src)
{
	SDL_Rect rect_dest;
	SDL_Rect rect_src;
	rect_dest.x = (t256_floor(src->x256));
	rect_dest.y = (t256_floor(src->y256));
	rect_dest.w = ((src->w128)>>7);
	rect_dest.h = ((src->h128)>>7);
//
	rect_src.w = (rect_dest.w)/* src->w*/;	rect_src.x = /* src->w*/0/*(rect_dest.w)*/;//*(src->yx_an im_frame&YX_FRAME_LOW_X);
	rect_src.h = (rect_dest.h)/* src->h*/;	rect_src.y = /* src->w*/0/*(rect_dest.h)*/;//*((src->yx_an im_frame&YX_FRAME_HIGH_Y)>>4)/* 0*/;

//
//	if (src->flags&SP_FLAG_CHEAP_ALPHA)
//	{
//		blit_cheap_alpha(src->sprite_bmp, &rect_src, sdl_screen[SDL_00_SCREEN], &rect_dest);
//	}
//	else
//	{
		#if 0
		/* �A���t�@�l���ς�����ꍇ�̂� */
		if (src->alpha_chache != src->alpha)
		{
			src->alpha_chache = src->alpha;
			SDL_SetAlpha(src->sprite_bmp, SDL_SRCALPHA, src->alpha_chache);
		}
		#else
		//	SDL_SetAlpha(src->sprite_bmp,SDL_SRCALPHA,src->alpha);
	//	if ( (u8)0xff != (u8)(src->color32>>24))
		{
			SDL_SetAlpha(src->sprite_bmp, SDL_SRCALPHA, (u8)(src->color32>>24) );
		}
		#endif
		SDL_BlitSurface(src->sprite_bmp, &rect_src, sdl_screen[SDL_00_SCREEN], &rect_dest);
//	}
}
/*---------------------------------------------------------
	�X�v���C�g��\������B
--------------------------------------------------------- */
void pause_sprite_display(void)
{
	SPRITE *s;
	s = sprite_list000_head;/* �X�v���C�g ���X�g�̐擪 ����T�� */
	while (NULL != s)
	{
		#if 0
		if (s->type != SP_DELETE ) /* �폜�ς݂͔�΂� */	/* SP_DELETE�� 0 �ɂȂ����̂ŗv��Ȃ��Ȃ����D */
		#endif
		{
			if (s->type & SP_GROUP_PAUSE_SP_MENU_TEXT/*type*/) /* type����v����ꍇ�̂� */
			{
//					if ((s->flags&SP_FLAG_VISIBLE)) /* �\���\�ȏꍇ�̂� */
					{
						sprite_draw000(s);
					}
			}
		}
		s=s->next;/* �� */
	}
//
}

/*---------------------------------------------------------
	�X�v���C�g�����X�g�ɒǉ�����B
--------------------------------------------------------- */

#if (1)
SPRITE *sprite_add_res_list(
	SDL_Surface *surface,
	int set_flags/* nocache */,
	IMAGE_RESOURCE *image_resource_ptr
	)
#endif
{
	int priority;			priority		= image_resource_ptr->priority;
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
		(NULL != s) &&				/* ���X�g�̓��܂ł���ׂ��Ȃ�I��� */
		(s->priority < priority)	/* �\���v���C�I���e�B�[�𖞂����ʒu�܂ŗ�����I��� */
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
//
	obj->sprite_bmp 	= surface;
//
//	int x_divide_frames_m1 = image_resource_ptr->x_divide_frames_m1;
//	int y_divide_frames_m1 = image_resource_ptr->y_divide_frames_m1;
//
//	obj->yx_frames	= (((y_divide_frames_m1)<<4)|((x_divide_frames_m1)));
//
//	x_divide_frames_m1++;
//	y_divide_frames_m1++;
//
	obj->w128				= ((surface->w/*/(x_divide_frames_m1)*/)<<7);
	obj->h128				= ((surface->h/*/(y_divide_frames_m1)*/)<<7);
//
//	int total_frames;		total_frames	= image_resource_ptr->total_frames;
//	int anime_speed;		anime_speed 	= image_resource_ptr->anime_speed;
	int m_Hit256R;			m_Hit256R		= image_resource_ptr->atari_hankei;
//

	obj->m_Hit256R	= (m_Hit256R<<8);

//	#if (1==USE_ZUKEI_ATARI_HANTEI)
//	obj->colision_bmp		= sprite_add_colision_map(surface, total_frames);
//	#endif /* (1==USE_ZUKEI_ATARI_HANTEI) */

//	obj->frames 	= total_frames;
	obj->priority	= priority;
	obj->flags		= set_flags; /* 0;	if (nocache) s->flags |= SP_FLAG_NOT_CACHE; */
	obj->color32	= 0xffffffff;		/*	obj->alpha		= 0xff;*/

//	obj->anim_speed 	= anime_speed/* 0 */;

	obj->m_zoom_x256			= 256;	/* �\���g�嗦 256 == [ x 1.00 ] */
	#if (0==USE_MEM_CLEAR)/* [�������[���N���A�[�@�\]���g��Ȃ��ꍇ�́A���������K�v�B */
//	obj->yx_an im_frame	= 0x00;
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

/* clouds�G�̏ꍇ�A�t�@�C���� */
/* static */static SPRITE *bbb_sprite_add_internal_res(IMAGE_RESOURCE *image_resource_ptr)
{
	char *filename; 		filename		= (char *)image_resource_ptr->file_name;
	int use_alpha;			use_alpha		= image_resource_ptr->use_alpha;
//
	SDL_Surface *s;
	SPRITE *sp;
	s		= loadbmp0(filename, use_alpha, 1);
	if (0 == use_alpha)
	{
		SDL_SetColorKey(s,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
	}
	sp	= sprite_add_res_list( s, 0/* flags */, image_resource_ptr);
	return (sp);
}

/*---------------------------------------------------------
	gu�ڍs���A�ėp�X�v���C�g
--------------------------------------------------------- */

SPRITE *sprite_add_gu(int image_resource_num)
{
	//image_resource_num=BASE_YUKARI32_PNG;
//
	IMAGE_RESOURCE *image_resource_ptr;
	image_resource_ptr = &my_bullet_resource[image_resource_num];
	return (bbb_sprite_add_internal_res( image_resource_ptr ));
}


/*---------------------------------------------------------
	gu�ڍs���A�e��p
--------------------------------------------------------- */
extern int bullet_system_regist_object(int image_resource_num);
SPRITE *sprite_add_only_bullet(int image_resource_num)
{
	//image_resource_num=BASE_YUKARI32_PNG;
//
	IMAGE_RESOURCE *image_resource_ptr;
	image_resource_ptr = &my_bullet_resource[image_resource_num];
	#if 1
	if (0==bullet_system_regist_object(image_resource_num))
	{
		/* �e�o�^�o���Ȃ����� */
//		return (NULL);
	}
	#endif
	return (bbb_sprite_add_internal_res( image_resource_ptr ));
}
