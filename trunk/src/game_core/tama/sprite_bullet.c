
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�X�v���C�g �}�l�[�W��
	-------------------------------------------------------
	GAME_X_OFFSET�Ή�
--------------------------------------------------------- */

/*---------------------------------------------------------
	�����蔻�� collision
	-------------------------------------------------------
	�u�����蔻��v�Ƃ́Aobj���������Ă��邩�ǂ����̔���B
	�u�����蔻��v�Ƃ́A���΂���ŁA���́u�������Ă���ꍇ�v�𔻒肷��ƒx���B
	-------------------------------------------------------
	�u�������Ă���ꍇ�v�𔻒肷��̂ł͂Ȃ��A
	�u�������Ă��Ȃ��ꍇ�v�𔻒肷������������ǂ�(����)�B
	-------------------------------------------------------
	�Ȃ��������ǂ����ƌ����ƁA����̈悪���G�ȏꍇ�A�u���������v�����Ĕ��肷��Ηǂ��B
	�u�������Ă���ꍇ�v�𔻒肷��ꍇ�u�������������G�v�����A
	�u�������Ă��Ȃ��ꍇ�v�𔻒肷��ꍇ�́u�����������ȒP�v�B
	-------------------------------------------------------
	�Ō�܂Łu�������Ă��Ȃ��ꍇ�v�ȊO�Ȃ�΁A�܂肠�����Ă���Ƃ������B
--------------------------------------------------------- */

/*---------------------------------------------------------
	���[�U�[ �����蔻��
	-------------------------------------------------------
	-------------------------------------------------------
	���������P�D����L���ȃI�u�W�F�N�g
	���������Q�D�̈�召����(�����O�͂�����Ȃ��B)
	���������R�D�����Ŕ���(�~���͂�����Ȃ��B)
	-------------------------------------------------------
	���[�U�[�̎d�l:
	//	��1. ���[�U�[�����������苗��(laser1->ar256)�͓�����Ȃ��B//��3.�ɓ���
		��2. ���[�U�[�͈��̑���(laser1->aw256)������B�����̊O���͓�����Ȃ��B
		��3. ���[�U�[����������A���Ε����͂�����Ȃ��B
		��4. ���[�U�[�͔���������(laser1->angle65536)[n/65536�x]�X���Ă���B
.	-------------------------------------------------------
  .     �����[�U�[������
.   .  ---
 .    .     .
   . |   x   |���a(laser1->ar256)�̉~�B�~���͂�����Ȃ��B(��1.)
      .     .
   /    ---   .������̗̈�͔��Α��Ȃ̂ł�����Ȃ��B(��3.)
  /    / .     .
 /    /    .     .
     /���̑���(laser1->aw256)�̒����B�����O�͂�����Ȃ��B(��2.)
    /         .
   /            .
  /               .������̗̈�͔��Α�(���[�U�[�����������苗��)�Ȃ̂ł�����Ȃ��B(��1. �� ��3.)
 /


	 �������蔻��̈����]������B

--------------------------------------------------------- */
#if 0/* �߂� */
int collision_laser( /* obj_t */OBJ *obj1, LASER *laser1)
{
//	���������P�D����L���ȃI�u�W�F�N�g

//	����̈�� -(laser1->angle65536) ��]������B(�X���Ƌt�����ɉ�]������)
	int sin_angle;
	int cos_angle;
	int center_x256;
	int center_y256;
	int sabun_x;
	int sabun_y;
	int rotation_angle1024;
	int hantei_x;
//	int hantei_y;
	/* ���[�U�[�����������_�Ƃ���B */
	center_x256 = (laser1->cx256);
	center_y256 = (laser1->cy256);
	/* ���_����̍����x�N�g�������B */
	sabun_x = (obj1->cx256)-(center_x256);
	sabun_y = (obj1->cy256)-(center_y256);
	/* (�X���Ƌt�����ɉ�]������) */
	rotation_angle1024 = -(deg65536to1024(laser1->angle65536));
	/* �Œ�l */
//	sin_angle = (int)(int256_si n1024(/*rot_sin*/((/*OFFS_SIN1024+*/		  rotation_angle1024)&(1024-1)))/*<<8*/);
//	cos_angle = (int)(int256_si n1024(/*rot_cos*/(( OFFS_COS1024+			  rotation_angle1024)&(1024-1)))/*<<8*/);
	int256_sincos1024( (rotation_angle1024), &sin_angle, &cos_angle);
	#if 1
	/* [��CCW]���� 0�x�Ŕ����v���̊p�x�n */
	hantei_x = (((sabun_x256 * sin_angle) + (sabun_y256 * cos_angle))>>8);
	hantei_y = (((sabun_x256 * cos_angle) - (sabun_y256 * sin_angle))>>8);
	#endif
//	���������Q�D�̈�召����(�����O�͂�����Ȃ��B)
//	if ((0) > hantei_y)/* ���[�U�[����������A���Ε����͂�����Ȃ��B */
	if ((laser1->ar256) > hantei_y)/* ���[�U�[����������A���Ε����͂�����Ȃ��B */
	{
		return (0);/* FALSE */	/* not hit. �������ĂȂ� */
	}
	if ((0+(laser1->aw256)) > hantei_x)/* ���̑���(laser1->aw256)�̒����B�����O�͂�����Ȃ��B */
	{
		return (0);/* FALSE */	/* not hit. �������ĂȂ� */
	}
	if ((0-(laser1->aw256)) < hantei_x)/* ���̑���(laser1->aw256)�̒����B�����O�͂�����Ȃ��B */
	{
		return (0);/* FALSE */	/* not hit. �������ĂȂ� */
	}

//	���������R�D�����Ŕ���(�~���͂�����Ȃ��B)
//	if (laser1->ar256 > ra256)
//	{
//		return (0);/* FALSE */	/* not hit. �������ĂȂ� */
//	}
	return (1);/* TRUE */	/* hit! �������� */
}

#endif

/*---------------------------------------------------------
	��`/�~ �����蔻��
	-------------------------------------------------------
	�I�u�W�F�N�g���m�̓����蔻��
	��܂��ɋ�`�Ŕ��ʂ�����A�߂����Ȃ�~�̏Փ˔���
	-------------------------------------------------------
	���������P�D����L���ȃI�u�W�F�N�g
	���������Q�D��`����
	���������R�D�~�̔��a�Ŕ���
--------------------------------------------------------- */

#if (1)
static /* BOOL */int collision_hit( /* obj_t */OBJ *obj1, /* obj_t */OBJ *obj2 )
{
	#if (0)/* ���̋@�\�͗v��Ȃ�(?)�����B */
	/*--- ����L���ȃI�u�W�F�N�g�łȂ�������I�� */
//	if (obj2->m_Hit256R == 0/* .0 */ /* FALSE */)
	if ((16) > obj2->m_Hit256R) 	/*t256(1.00)t256(0.10)*/
	{
		return (0);/* FALSE */	/* not hit. �������ĂȂ� */
	}
//	if (obj1->m_Hit256R == 0/* .0 */ /* FALSE */)
	if ((16) > obj1->m_Hit256R) 	/*t256(1.00)t256(0.10)*/
	{
		return (0);/* FALSE */	/* not hit. �������ĂȂ� */
	}
	#endif
//
	int dr; 	/* ���a(==radius) */
	int dx; 	/* x����(����x����, delta x) */
	int dy; 	/* y����(����y����, delta x) */
	dr = (obj2->m_Hit256R + obj1->m_Hit256R);
	/* ���S���m�ō��B�܂莲���Ƃ̋��������߂�B */
	dx = ((obj2->cx256) - (obj1->cx256));/* ���S���m�ō������߂�B */
	dy = ((obj2->cy256) - (obj1->cy256));/* ���S���m�ō������߂�B */

	/* check collision, delta x position. ��`����i�w���j */
	if (dx < 0) 	{	dx = (-dx); 			}	/* dx = abs(dx); */
	if (dx > dr)	{	return (0);/* FALSE */	}	/* not hit, because not near the dx. �w�̍������傫���̂ŁA�������ĂȂ� */

	/* check collision, delta y position. ��`����i�x���j */
	if (dy < 0) 	{	dy = (-dy); 			}	/* dy = abs(dy); */
	if (dy > dr)	{	return (0);/* FALSE */	}	/*	not hit, because not near the dy. �x�̍������傫���̂ŁA�������ĂȂ� */

	/* check collision, circle delta round distance.  �~�̔��a�Ŕ��� */
	dr *= dr;
	dx *= dx;
	dy *= dy;
	dx += dy;
	/* �����蔻��O�Ȃ� */
	if (dr < dx)/* if ((dr^2) < ( (dx^2) + (dy^2) )) */
	{
		return (0);/* FALSE */	/* not hit. �������ĂȂ� */
	}
	return (1);/* TRUE */	/* hit! �������� */
}
#endif


/*---------------------------------------------------------
	�X�v���C�g�̂����蔻��
	-------------------------------------------------------
	���͒l: type
		OBJ_Z04_TAMA		(�e)
		OBJ_Z03_ITEM		(�A�C�e��)
--------------------------------------------------------- */
/*---------------------------------------------------------
	(�G��p)�X�v���C�g�̂����蔻��
--------------------------------------------------------- */

global OBJ obj99[OBJ_LAST_99_0x0948_MAX]; 	/* �S�X�v���C�g */

static OBJ *sprite_collision_check(OBJ *tocheck, OBJ *s, int set_obj_type, unsigned int length)
{
	unsigned int ii;
	for (ii=0; ii<length; ii++)/* �S�����ׂ�B */
	{
		if (JYUMYOU_NASI < s->jyumyou)/* �����ς݂͔�΂��B */
		{
			/* 1. �������g�`�F�b�N */
			/* ����Ώ�(�������g)�͔�΂�(�S���������̓��m�̂����蔻�������Ă��Ӗ����Ȃ��̂Ŏ��Ȃ�) */
			if (s != tocheck)	/* Is sprites my self?(not need self collisions.) */
			{
				/* 2. �����ރ`�F�b�N */
				if (
							(s->obj_type_set & set_obj_type)			/* obj_type_set ����v����ꍇ�̂� */
					)	/* do collision only mach type. */
				{
					/* 3. ����t���O�`�F�b�N */
					if (
							( (ATARI_HANTEI_OFF) != (s->atari_hantei))/*(�����蔻�肪������̂̂݃`�F�b�N)*/
						)	/* do collision only, use collision check type. */
					{
						/* 4. ��`/�~�����蔻��`�F�b�N   ��܂��ɋ�`�Ŕ��ʂ�����A�߂����Ȃ�~�̏Փ˔��� */
						if (collision_hit(s, tocheck))	/* ��`/�~�����蔻�� */ 	/* hit collision rectangle to circle check. */
						{
							return (s);/*(��������)*/	/* hit collisioning! */
						}
					}
				}
			}
		}
		s++;
	}
	return (NULL);/*(�������ĂȂ�)*/	/* no hit collisioning. */
}
/*static*/global OBJ *obj_collision_check_00_tama(OBJ *tocheck, int type)
{
	OBJ *s;
	s = &obj99[OBJ_HEAD_00_0x0000_TAMA+0];
	return sprite_collision_check(tocheck, s, type, OBJ_POOL_00_TAMA_1024_MAX);
}
//	/*static*/global OBJ *obj_collision_check_01_teki(OBJ *tocheck, int type)
/*static*/global OBJ *obj_collision_check_01_teki(OBJ *tocheck)/*, (OBJ_Z02_TEKI)int ty pe*/
{
	OBJ *s;
	s = &obj99[OBJ_HEAD_01_0x0800_TEKI+0];
	return sprite_collision_check(tocheck, s, /*type*/(OBJ_Z02_TEKI), OBJ_POOL_01_TEKI_0256_MAX);
}


/*---------------------------------------------------------
	�X�v���C�g ���X�g�ɓo�^���ꂽ�X�v���C�g��S�������B
--------------------------------------------------------- */
static void obj_remove_all(OBJ *s, unsigned int length)
{
	unsigned int ii;
	for (ii=0; ii<length; ii++)/* �S�����ׂ�B */
	{
		s->jyumyou	= JYUMYOU_NASI;
		#if (1)/* �f�o�b�O�Ă��� */
		s->cx256		= (-1);
		#endif
		s++;
	}
}

#if 0
static void s_obj_remove_all_00_tama(void)	/* �e��p */
{
	OBJ *s;
	s = &obj99[OBJ_HEAD_00_0x0000_TAMA+0];
	obj_remove_all(s, OBJ_POOL_00_TAMA_1024_MAX);
}
static void s_obj_remove_all_01_teki(void)	/* gu�ėp(��SDL) */
{
	OBJ *s;
	s = &obj99[OBJ_HEAD_01_0x0800_TEKI+0];
	obj_remove_all(s, OBJ_POOL_01_TEKI_0256_MAX);
}
global void obj_cleanup_all(void)
{
	s_obj_remove_all_01_teki();/* gu�ėp */
	s_obj_remove_all_00_tama();/* �e��p */
}
#endif

#if 1/*(r36)*/

/* [A00�e�̈�]��[A01�G�̈�]��[A02�Œ�̈�]��[A03�p�l���̈�]��OBJ��S�����B */
global void obj_cleanup_all(void)
{
	OBJ *s;
	s = &obj99[OBJ_HEAD_00_0x0000_TAMA+0];
	obj_remove_all(s, (OBJ_POOL_00_TAMA_1024_MAX+OBJ_POOL_01_TEKI_0256_MAX+OBJ_POOL_02_KOTEI_0016_MAX+OBJ_POOL_03_PANEL_0056_MAX));
}
#endif

#if 0
global void obj_cleanup_A02_A03_kotei_panel(void)
{
}
#endif
/*---------------------------------------------------------
	�X�v���C�g����t���[�������삳����B
--------------------------------------------------------- */
static void sprite_move_main(OBJ *s, unsigned int length)
{
	unsigned int iii;
//	for (iii=0; iii<length; iii++)/* �S�����ׂ�B */
	iii = length;/* ���[�U�[�̎������ȒP�ɂ���ׂɁA�t���ɂ���B */
	{
		my_loop:
		if (JYUMYOU_NASI < s->jyumyou)/* �����ς݂͔�΂��B */
		{
			s->jyumyou--;/* �����o�� */
			{
				#if 1/*???*/
				if (s->obj_type_set & (SP_GROUP_ALL_SDL_CORE_TYPE)) /* obj_type_set����v����ꍇ�̂� */
				#endif
				{
					if (NULL != s->callback_mover)
					{
						(s->callback_mover)(s);
					}
				}
			}
		}
		s--;//	s++;/* ���[�U�[�̎������ȒP�ɂ���ׂɁA�t���ɂ���B */
		iii--;
		if (0 != iii)
		{
			goto my_loop;
		}
	}
}
	#if(0)/*(�_��)*/
extern void hatudan_effect_move_all(void);
global void obj_area_move_A00_A01_A02(void)
{
	hatudan_effect_move_all();
	/*
	[�񓮍�]	�e��p�X�v���C�g(1024)
	[����]		�G��p�X�v���C�g(256)
	[����]		���@���Œ�X�v���C�g(16)
	[�񓮍�]	�p�l���p�X�v���C�g(56)
	*/
	OBJ *s;
	s = &obj99[OBJ_HEAD_03_0x0910_PANEL];
	sprite_move_main(s, (OBJ_POOL_01_TEKI_0256_MAX+OBJ_POOL_02_KOTEI_0016_MAX));
}
/*(r35, �����I�ɒe�̈ړ��͖���)*/
	#endif
	//
	#if(1)/*(�߂�r33)*/
global void obj_area_move_A00_A01_A02(void)
{
	/*
	[����]		�e��p�X�v���C�g(1024)[�A�C�e���␯�_�┭�e�G�t�F�N�g�̓���]
	[����]		�G��p�X�v���C�g(256)
	[����]		���@���Œ�X�v���C�g(16)
	[�񓮍�]	�p�l���p�X�v���C�g(56)
	*/
	OBJ *s;
	s = &obj99[OBJ_HEAD_03_0x0910_PANEL];
	sprite_move_main(s, (OBJ_POOL_00_TAMA_1024_MAX+OBJ_POOL_01_TEKI_0256_MAX+OBJ_POOL_02_KOTEI_0016_MAX));
}
/*(r35, �����I�ɒe�̈ړ��͖����ɂȂ�\���������B���̏ꍇ effect �͕ʃV�X�e���ɂȂ�)*/
	#endif
	//
	#if 0/*(�߂�r32)*/
static void obj_move_main_00_tama(void) 	/* �e��p�X�v���C�g(1024) gu�e��p */
{
	OBJ *s;
	s = &obj99[OBJ_HEAD_00_0x0000_TAMA+OBJ_POOL_00_TAMA_1024_MAX+0];
	sprite_move_main(s, OBJ_POOL_00_TAMA_1024_MAX);
}

static void obj_move_main_01_teki(void) /* �G��p�X�v���C�g(256) gu�ėp(��SDL) */
{
	OBJ *s;
	s = &obj99[OBJ_HEAD_01_0x0800_TEKI+OBJ_POOL_01_TEKI_0256_MAX+0];
	sprite_move_main(s, OBJ_POOL_01_TEKI_0256_MAX);
}
static void obj_move_main_02_kotei(void)/* ���@���Œ�X�v���C�g(16) */
{
	OBJ *s;
	s = &obj99[OBJ_HEAD_02_0x0900_KOTEI+OBJ_POOL_02_KOTEI_0016_MAX+0];
	sprite_move_main(s, OBJ_POOL_02_KOTEI_0016_MAX);
}
global void obj_area_move_A00_A01_A02(void)
{
	obj_move_main_02_kotei();/* ���@���Œ�X�v���C�g(16) (���@���Œ�I�u�W�F�N�g) */
	obj_move_main_01_teki();/* �G��p�X�v���C�g(256) (gu�ėp) */
	obj_move_main_00_tama();/* �e��p�X�v���C�g(1024) (�e��p) */
}
	#endif

#if 1
/*---------------------------------------------------------
	��ʊO�Ȃ炨���܂�(�ʁA��ɔėpobj�Ŏg�p)
---------------------------------------------------------*/
global void gamen_gai_nara_zako_osimai(OBJ *src)
{
	if (
		(src->cx256 < t256(GAME_X_OFFSET)) ||
		(src->cx256 > t256(GAME_WIDTH)) ||
		(src->cy256 < t256(0)) ||
		(src->cy256 > t256(GAME_HEIGHT)) )		/* ��ʊO�ɏo���� */
	{
		src->jyumyou = JYUMYOU_NASI;		/* �����܂� */
	}
}
//	if ((src->cx256 < t256(0))||(src->cx256 > t256(GAME_WIDTH))||
//		(src->cy256 < t256(0))||(src->cy256 > t256(GAME_HEIGHT)))
//	{
//		src->jyumyou = JYUMYOU_NASI;/*��ʊO�ɂł��炨���܂�*/
//	}
/*---------------------------------------------------------
	��ʊO�Ȃ炨���܂�(�ʁA��ɔėpobj�Ŏg�p)
---------------------------------------------------------*/
//global void gamen_gai_limit_30_nara_zako_osimai(OBJ *src)
//{
//	if ((JYUMYOU_ZAKO_CLIP) > src->jyumyou) 		/* ���E */
//	{
//		gamen_gai_nara_zako_osimai(src);/* ��ʊO�Ȃ炨���܂� */
//	}
//}
#endif



/*---------------------------------------------------------
	�g�p���ł��邪�ޔ��������ꍇ�B��ʊO�ɃX�v���C�g���ړ������A�����ɂ���B
	-------------------------------------------------------
	�X�v���C�g���m�ۂ����܂ܑޔ�������B
	�{�Xobj, effect obj �Ŏg�p���B
---------------------------------------------------------*/
global void sprite_kotei_obj_r36_taihi(OBJ *h)
{
	h->jyumyou						= (JYUMYOU_MUGEN);/* ���ԂŎ����������Ȃ� */	/*(�Œ�m�ۂŔ��ʂ����t���O)*/
	h->m_Hit256R					= ZAKO_ATARI16_PNG;
	h->atari_hantei 				= (ATARI_HANTEI_OFF/*�X�R�A���p*/); 	/* �����蔻���OFF(���G) */
	h->callback_mover				= NULL;
	h->callback_loser				= NULL;
	h->callback_hit_teki			= NULL; 	/* �_�~�[�R�[���o�b�N�o�^ */
	h->cx256		= (t256( GAME_X_OFFSET + ((GAME_320_WIDTH)/2) ));
	h->cy256		= (t256(-256));
}
//???	h->obj_type_set 				= BOSS_00_11;

/*---------------------------------------------------------
	�X�v���C�g�̏�����������B
	-------------------------------------------------------
	���X�g�����łȂ��X�v���C�g������������ׁA
	�O��������Ă΂��̂Œ��ӁB
--------------------------------------------------------- */
//tern void sprite_initialize_gu(OBJ *obj);
global void sprite_initialize_gu(OBJ *obj)
{
	#if (0==USE_MEM_CLEAR)/* [�������[���N���A�[�@�\]���g��Ȃ��ꍇ�́A���������K�v�B */
	#else
	memset(obj, 0, sizeof(OBJ));
	#endif
//
	obj->color32	= MAKE32RGBA(0xff, 0xff, 0xff, 0xff);		/*	obj->alpha		= 0xff;*/
	#if 1/* �����I�ɏ�����@�\ */
	obj->jyumyou	= JYUMYOU_1MIN; 		/* 1�������珟��Ɏ��������B */
//	obj->timeover_ticks 		= 0;
	#endif
	obj->m_zoom_x256			= t256(1.00);	/* �\���g�嗦 256 == [ x 1.00 ] */
	obj->m_zoom_y256			= t256(1.00);	/* �\���g�嗦 256 == [ x 1.00 ] */
	#if (0==USE_MEM_CLEAR)/* [�������[���N���A�[�@�\]���g��Ȃ��ꍇ�́A���������K�v�B */
	obj->rotationCCW1024		= 0;	/* �`��p�p�x(����0�x�ō����(�����v���)) */	/* 0 == �X���Ȃ��B����0�x */
//
	obj->vx256					= t256(0);
	obj->vy256					= t256(0);
//
	obj->callback_mover 		= NULL;
	obj->callback_loser 		= NULL;
	obj->callback_hit_teki		= NULL;
	#endif	/* (0==USE_MEM_CLEAR) */
}


/*---------------------------------------------------------
	gu�ڍs���A�e��p�X�v���C�g�����X�g�ɒǉ�����B
	gu�ڍs���A�ėp�X�v���C�g�����X�g�ɒǉ�����B
	�o�^�ł��Ȃ��ꍇ�ANULL��Ԃ��B
--------------------------------------------------------- */

global OBJ *obj_add_Ann_direct(unsigned int direct_register_number)/*OBJ_HEAD_02_0x0900_KOTEI+*/
{
//	int priority;		priority		= image_resource_ptr->priority;
	OBJ *obj;/* �V�K�쐬����X�v���C�g */
	obj = &obj99[direct_register_number];
	sprite_initialize_gu(obj);			/* �V�K�쐬�����X�v���C�g�������� */
	return (obj);
}

global OBJ *obj_add_A00_tama_error(void)
{
	#if 1
	/* private (���̊֐���)�v���C�x�[�g�ȕϐ� */
	static u16 register_num;	/* �o�^�ł���\�����������Ȉʒu��ێ� */
	#endif
	OBJ *obj;/* �V�K�쐬����X�v���C�g */
	{
		u16 search_count;
		search_count = 0;
my_retry:
		search_count++;
		if ((OBJ_POOL_00_TAMA_1024_MAX-1) <= search_count)
		{
			#if (0)/*(�f�o�b�O)*/
			/*(�A���X�͂Ƃ��������ŏo��!!���ł���H)*/
			psp_fatal_error( (char*)
			//	"0123456789012345678901234567890123456789"	// ���p40��"�ő�\��������"
				"regist tama: %d �o�^�\\�ő�e�����A" "\\n"
				"�����Ă��܂��B", search_count);
			#endif
			return (NULL);
		}	/* (�o�^�ł��Ȃ��ꍇ) */
		register_num++; 	/* �o�^�ł���\�����������Ȉʒu */
		register_num &= (OBJ_POOL_00_TAMA_1024_MAX-1);
		obj = &obj99[OBJ_HEAD_00_0x0000_TAMA+register_num];
		if (JYUMYOU_NASI < obj->jyumyou)/* �g�p���Ȃ������x�T���B */
		{
			goto my_retry;	/* �T���B */
		}
	}
	sprite_initialize_gu(obj);			/* �V�K�쐬�����X�v���C�g�������� */
	#if 1
	/* �W�������� */
	obj->atari_hantei			|= (1/*(�X�R�A���p)*/); 	/* �����蔻�肠�� */
	#endif
	return (obj);
}

/*
(r36)boss obj�͓G�ėpobj����Œ�m�ۂ���悤�Ɏd�l�ύX�����B
*/

/*boss���Œ�m�ۂ��镪*/
#define BOSS_OBJ_KOTEI_KAKUHO_BUN (1)
global OBJ *obj_add_A01_teki_error(void)
{
	#if 1
	/* private (���̊֐���)�v���C�x�[�g�ȕϐ� */
	static u32 register_num;	/* �o�^�ł���\�����������Ȉʒu��ێ� */
	#endif
	OBJ *obj;/* �V�K�쐬����X�v���C�g */
	{
		u32 search_count;
		search_count = 0;
my_retry:
		search_count++;
		if ((OBJ_POOL_01_TEKI_0256_MAX-1-(BOSS_OBJ_KOTEI_KAKUHO_BUN)) < search_count)
		{	return (NULL);	}	/* (�o�^�ł��Ȃ��ꍇ) */
		register_num++; 	/* �o�^�ł���\�����������Ȉʒu */
		register_num &= (OBJ_POOL_01_TEKI_0256_MAX-1);
		obj = &obj99[OBJ_HEAD_01_0x0800_TEKI+register_num];
		if (JYUMYOU_NASI < obj->jyumyou)/* �g�p���Ȃ������x�T���B */
		{
			goto my_retry;	/* �T���B */
		}
	}
	sprite_initialize_gu(obj);			/* �V�K�쐬�����X�v���C�g�������� */
	return (obj);
}


#define PPP (GAME_WIDTH)
global void sprite_panel_init(void)
{
	unsigned int ii;
	for (ii=0; ii<OBJ_POOL_03_PANEL_0056_MAX; ii++)/* �S�� */
	{
		OBJ *obj;/* ����������X�v���C�g */
		obj = &obj99[OBJ_HEAD_03_0x0910_PANEL+ii];
		sprite_initialize_gu(obj);			/* �X�v���C�g�������� */
	}
	unsigned int kk;
	kk = ((PPP+5*8+4)<<8);
//	unsigned int jj;
//	for (jj=0; jj<(2); jj++)/* */
	for (ii=0; ii<(10); ii++)/* */
	{
		OBJ *obj;/* ����������X�v���C�g */
		/* �X�R�A */
		obj = &obj99[OBJ_HEAD_03_0x0910_PANEL+ii+PANEL_OBJ_00_S00];
		obj->cx256 = (kk);
		obj->cy256 = ((6*8+7)<<8);
		/* �n�C�X�R�A */
		obj = &obj99[OBJ_HEAD_03_0x0910_PANEL+ii+PANEL_OBJ_10_H00];
		obj->cx256 = (kk);
		obj->cy256 = ((3*8+2)<<8);
		//
		kk += (8*256);
	}
}
