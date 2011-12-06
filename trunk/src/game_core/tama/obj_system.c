
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�X�v���C�g�I�u�W�F�N�g �V�X�e��
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
.   -------------------------------------------------------
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
int obj_collision_laser( /* obj_t */OBJ *obj1, LASER *laser1)
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
	center_x256 = (laser1->center.x256);
	center_y256 = (laser1->center.y256);
	/* ���_����̍����x�N�g�������B */
	sabun_x = (obj1->center.x256)-(center_x256);
	sabun_y = (obj1->center.y256)-(center_y256);
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
static /* BOOL */int obj_collision_tama( /* obj_t */OBJ *obj1, /* obj_t */OBJ *obj2 )
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
	dx = ((obj2->center.x256) - (obj1->center.x256));/* ���S���m�ō������߂�B */
	dy = ((obj2->center.y256) - (obj1->center.y256));/* ���S���m�ō������߂�B */

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
	�X�v���C�g�I�u�W�F�N�g�̂����蔻��
	-------------------------------------------------------
	���͒l: type
		OBJ_Z04_TAMA		(�e)
		OBJ_Z03_ITEM		(�A�C�e��)
--------------------------------------------------------- */
/*---------------------------------------------------------
	(�G��p)�X�v���C�g�I�u�W�F�N�g�̂����蔻��
--------------------------------------------------------- */

global OBJ obj99[OBJ_LAST_99_0x0a48_MAX];	/* �S�X�v���C�g�I�u�W�F�N�g */

static OBJ *obj_collision_check(OBJ/**/ *tocheck, OBJ/**/ *s, int set_obj_type, unsigned int length)
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
						if (obj_collision_tama(s, tocheck)) 	/* ��`/�~�����蔻�� */ 	/* hit collision rectangle to circle check. */
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
/*static*/global OBJ *obj_collision_check_00_tama(OBJ/**/ *tocheck, int type)
{
	OBJ *s;
	s = &obj99[OBJ_HEAD_00_0x0000_TAMA+0];
	return obj_collision_check(tocheck, s, type, OBJ_POOL_00_TAMA_1024_MAX);
}
//	/*static*/global OBJ *obj_collision_check_A01_A02_teki(OBJ/**/ *tocheck, int type)
/*static*/global OBJ *obj_collision_check_A01_A02_teki(OBJ/**/ *tocheck)/*, (OBJ_Z02_TEKI)int ty pe*/
{
	OBJ *s;
//	s = &obj99[OBJ_HEAD_02_0x0900_TEKI_FIX+0];
//	return obj_collision_check(tocheck, s, /*type*/(OBJ_Z02_TEKI), (/*OBJ_POOL_01_TOUROKU_0256_MAX+*/OBJ_POOL_02_TEKI_FIX_0256_MAX));
	s = &obj99[OBJ_HEAD_01_0x0800_TOUROKU+0];
	return obj_collision_check(tocheck, s, /*type*/(OBJ_Z02_TEKI), (OBJ_POOL_01_TOUROKU_0256_MAX + OBJ_POOL_02_TEKI_FIX_0256_MAX));
}


/*---------------------------------------------------------
	�X�v���C�g�I�u�W�F�N�g����t���[�����A�ړ�����B
--------------------------------------------------------- */
//	for (iii=0; iii<length; iii++)/* �S�����ׂ�B */
	//
	#if(1)/*(�߂�r33)*/
	/*
	[�ړ�]		�e��p�X�v���C�g�I�u�W�F�N�g(1024)[�A�C�e���␯�_�┭�e�G�t�F�N�g�̈ړ�]
	[�ړ�]		�Œ�G��p�X�v���C�g�I�u�W�F�N�g(256)
	[�ړ�]		�G��p�X�v���C�g�I�u�W�F�N�g(256)
	[�ړ�]		���@���Œ�X�v���C�g�I�u�W�F�N�g(16)
	[��ړ�]	�p�l���p�X�v���C�g�I�u�W�F�N�g(56)
	*/
global void obj_system_move_all(void)
{
	OBJ *s; 	s = &obj99[OBJ_HEAD_04_0x0a10_PANEL];
	unsigned int length;	length = (
			OBJ_POOL_00_TAMA_1024_MAX +
			OBJ_POOL_01_TOUROKU_0256_MAX +
			OBJ_POOL_02_TEKI_FIX_0256_MAX +
			OBJ_POOL_03_KOTEI_0016_MAX);
	unsigned int iii;
	iii = length;/* ���[�U�[�̎������ȒP�ɂ���ׂɁA�t���ɂ���B */
	{
		my_loop:
		if (JYUMYOU_NASI < s->jyumyou)/* �����ς݂͔�΂��B */
		{
			s->jyumyou--;/* �����o�� */
			/*(�ړ�����������Έړ�����B)*/
			if (NULL != s->callback_mover)
			{
				(s->callback_mover)(s);/*(�ړ�����)*/
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
/*(r35, �����I�ɒe�̈ړ��͖����ɂȂ�\���������B���̏ꍇ effect �͕ʃV�X�e���ɂȂ�)*/
	#endif
	//
	#if(0)/*(�_��)*/
extern void hatudan_effect_move_all(void);
global void obj_system_move_all(void)
{
	hatudan_effect_move_all();
	/*
	[�񓮍�]	�e��p�X�v���C�g�I�u�W�F�N�g(1024)
	[����]		�Œ�G��p�X�v���C�g�I�u�W�F�N�g(256)
	[����]		�G��p�X�v���C�g�I�u�W�F�N�g(256)
	[����]		���@���Œ�X�v���C�g�I�u�W�F�N�g(16)
	[�񓮍�]	�p�l���p�X�v���C�g�I�u�W�F�N�g(56)
	*/
	OBJ *s;
	s = &obj99[OBJ_HEAD_04_0x0a10_PANEL];
	obj_system_move_all(s, (
		OBJ_POOL_01_TOUROKU_0256_MAX +
		OBJ_POOL_02_TEKI_FIX_0256_MAX +
		OBJ_POOL_03_KOTEI_0016_MAX));
}
/*(r35, �����I�ɒe�̈ړ��͖���)*/
	#endif

#if 1
/*---------------------------------------------------------
	��ʊO�Ȃ炨���܂�(�ʁA��ɔėpobj�Ŏg�p)
---------------------------------------------------------*/
global OBJ_CALL_FUNC(gamen_gai_nara_zako_osimai)
{
	if (
		(src->center.x256 < t256(GAME_X_OFFSET)) ||
		(src->center.x256 > t256(GAME_WIDTH)) ||
		(src->center.y256 < t256(0)) ||
		(src->center.y256 > t256(GAME_HEIGHT)) )		/* ��ʊO�ɏo���� */
	{
		src->jyumyou = JYUMYOU_NASI;		/* �����܂� */
	}
}
//	if ((src->center.x256 < t256(0))||(src->center.x256 > t256(GAME_WIDTH))||
//		(src->center.y256 < t256(0))||(src->center.y256 > t256(GAME_HEIGHT)))
//	{
//		src->jyumyou = JYUMYOU_NASI;/*��ʊO�ɂł��炨���܂�*/
//	}
/*---------------------------------------------------------
	��ʊO�Ȃ炨���܂�(�ʁA��ɔėpobj�Ŏg�p)
---------------------------------------------------------*/
//global OBJ_CALL_FUNC(gamen_gai_limit_30_nara_zako_osimai)
//{
//	if ((JYUMYOU_ZAKO_CLIP) > src->jyumyou) 		/* ���E */
//	{
//		gamen_gai_nara_zako_osimai(src);/* ��ʊO�Ȃ炨���܂� */
//	}
//}
#endif



/*---------------------------------------------------------
	�g�p���ł��邪�ޔ��������ꍇ�B��ʊO�ɃX�v���C�g�I�u�W�F�N�g���ړ������A�����ɂ���B
	-------------------------------------------------------
	�X�v���C�g�I�u�W�F�N�g���m�ۂ����܂ܑޔ�������B
	�{�Xobj, effect obj �Ŏg�p���B
---------------------------------------------------------*/
global OBJ_CALL_FUNC(sprite_kotei_obj_r36_taihi)
{
	src->jyumyou				= (JYUMYOU_MUGEN);/* ���ԂŎ����������Ȃ� */	/*(�Œ�m�ۂŔ��ʂ����t���O)*/
	src->m_Hit256R				= ZAKO_ATARI16_PNG;
	src->atari_hantei 			= (ATARI_HANTEI_OFF/*�X�R�A���p*/); 	/* �����蔻���OFF(���G) */
	src->callback_mover			= NULL; 	/*(�ړ�����)*/
	src->callback_kougeki 		= NULL; 	/*(�U������)*/
	src->callback_hit_teki		= NULL; 	/*(��e������) �_�~�[�R�[���o�b�N�o�^ */
	src->callback_loser			= NULL; 	/*(���S������)*/
	src->center.x256			= (t256( GAME_X_OFFSET + ((GAME_320_WIDTH)/2) ));
	src->center.y256			= (t256(-256));
}
//???	h->obj_type_set 	= BOSS_00_11;

/*---------------------------------------------------------
	�X�v���C�g�I�u�W�F�N�g�̏�����������B
	-------------------------------------------------------
	���X�g�����łȂ��X�v���C�g�I�u�W�F�N�g������������ׁA
	�O��������Ă΂��̂Œ��ӁB
--------------------------------------------------------- */
//tern OBJ_CALL_FUNC(obj_initialize_one);
global OBJ_CALL_FUNC(obj_initialize_one)
{
	#if (0==USE_MEM_CLEAR)/* [�������[���N���A�[�@�\]���g��Ȃ��ꍇ�́A���������K�v�B */
	#else
	memset(src, 0, sizeof(OBJ));
	#endif
//
	src->color32				= MAKE32RGBA(0xff, 0xff, 0xff, 0xff);		/*	obj->alpha		= 0xff;*/
	#if 1/* �����I�ɏ�����@�\ */
	src->jyumyou				= JYUMYOU_1MIN; 		/* 1�������珟��Ɏ��������B */
//	src->timeover_ticks 		= 0;
	#endif
	src->m_zoom.x256			= t256(1.00);	/* �\���g�嗦 256 == [ x 1.00 ] */
	src->m_zoom.y256			= t256(1.00);	/* �\���g�嗦 256 == [ x 1.00 ] */
//	src->m_zoom.y256			= M_ZOOM_Y256_NO_TILT;/* ����@�\�ŌX���Ȃ��悤�V�X�e���g��(r33)�B */
	/*([r39]m_zoom�̏����l��ύX�e�X�g�A[��]1.00�{���[�h������]�A����1.00�{���[�h��])*/
	#if (0==USE_MEM_CLEAR)/* [�������[���N���A�[�@�\]���g��Ȃ��ꍇ�́A���������K�v�B */
	src->rotationCCW1024		= 0;	/* �`��p�p�x(����0�x�ō����(�����v���)) */	/* 0 == �X���Ȃ��B����0�x */
//
	src->math_vector.x256		= t256(0);
	src->math_vector.y256		= t256(0);
//
	src->callback_mover 		= NULL;
	src->callback_kougeki		= NULL; 	/*(�U������)*/
	src->callback_hit_teki		= NULL;
	src->callback_loser 		= NULL;
	#endif	/* (0==USE_MEM_CLEAR) */
}


/*---------------------------------------------------------
	gu�ڍs���A�e��p�X�v���C�g�I�u�W�F�N�g�����X�g�ɒǉ�����B
	gu�ڍs���A�ėp�X�v���C�g�I�u�W�F�N�g�����X�g�ɒǉ�����B
	�o�^�ł��Ȃ��ꍇ�ANULL��Ԃ��B
--------------------------------------------------------- */

/*---------------------------------------------------------
	�e�̈�ɓG�e��o�^����B
--------------------------------------------------------- */

global OBJ *obj_regist_tama(void)
{
	#if 1
	/* private (���̊֐���)�v���C�x�[�g�ȕϐ� */
	static u32 register_num;/*(�o�^�ł���\�����������Ȉʒu��ێ�)*/
	#endif
	OBJ *obj;/*(�V�K�쐬����X�v���C�g�I�u�W�F�N�g)*/
	{
		u32 remain_count;/*(�o�^�o����c�萔)*/
		remain_count = (OBJ_POOL_00_TAMA_1024_MAX-1);
my_retry:
		remain_count--;
		if (0 == remain_count)
		{
			#if (0)/*(�f�o�b�O)*/
			/*(�A���X�͂Ƃ��������ŏo��!!���ł���H)*/
			psp_fatal_error( (char*)
			//	"0123456789012345678901234567890123456789"	// ���p40��"�ő�\��������"
				"regist tama: %d �o�^�\\�ő�e�����A" "\\n"
				"�����Ă��܂��B", remain_count);
			#endif
			return (NULL);/*(�o�^�ł��Ȃ�����)*/
		}
		register_num++; 	/*(�o�^�ł���\�����������Ȉʒu)*/
		register_num &= (OBJ_POOL_00_TAMA_1024_MAX-1);
		obj = &obj99[OBJ_HEAD_00_0x0000_TAMA+register_num];
		if (JYUMYOU_NASI < (obj->jyumyou))/*(�g�p���Ȃ������x�T���B)*/
		{
			goto my_retry;/*(�T���B)*/
		}
	}
	obj_initialize_one(obj);			/* �V�K�쐬�����X�v���C�g�I�u�W�F�N�g�������� */
	#if 1
	/* �W�������� */
	obj->atari_hantei			|= (1/*(�X�R�A���p)*/); 	/* �����蔻�肠�� */
	#endif
	return (obj);/*(�o�^�ł���)*/
}
/*---------------------------------------------------------
�B
--------------------------------------------------------- */

/*
(r36)boss obj�͓G�ėpobj����Œ�m�ۂ���悤�Ɏd�l�ύX�����B
*/
static u32 register_kotei_first;	/* �o�^�G���A�o�^�ł���ʒu */
static u32 register_kotei_max;		/* �o�^�G���A�o�^�ł���ʒu */

/*(�Œ�G���m��)*/
global void obj_touroku_push(void)
{
	register_kotei_first++;
	register_kotei_max--;
}
/*(�Œ�G�����炷)*/
global void obj_touroku_pop(void)
{
	register_kotei_first--;
	register_kotei_max++;
}
/*---------------------------------------------------------
	�G�I�u�W�F�o�^�̈�ɓG�I�u�W�F��o�^����B
--------------------------------------------------------- */

/*(boss���Œ�m�ۂ��镪)*/
//#define BOSS_OBJ_KOTEI_KAKUHO_BUN (1)
global OBJ *obj_regist_teki(void)
{
	#if 1
	/* private (���̊֐���)�v���C�x�[�g�ȕϐ� */
	static u32 register_num;/*(�o�^�ł���\�����������Ȉʒu��ێ�)*/
	#endif
	OBJ *obj;/*(�V�K�쐬����X�v���C�g�I�u�W�F�N�g)*/
	{
		u32 remain_count;/*(�o�^�o����c�萔)*/
		remain_count = register_kotei_max;
my_retry:
		remain_count--;
		if (0 == remain_count)
		{
			return (NULL);/*(�o�^�ł��Ȃ�����)*/
		}
		register_num++; 	/*(�o�^�ł���\�����������Ȉʒu)*/
		if (register_num >= (OBJ_POOL_01_TOUROKU_0256_MAX-1))
		{
			register_num = register_kotei_first;
		}
		obj = &obj99[OBJ_HEAD_01_0x0800_TOUROKU+register_num];
		if (JYUMYOU_NASI < (obj->jyumyou))/*(�g�p���Ȃ������x�T���B)*/
		{
			goto my_retry;/*(�T���B)*/
		}
	}
	obj_initialize_one(obj);		/* �V�K�쐬�����X�v���C�g�I�u�W�F�N�g�������� */
	return (obj);/*(�o�^�ł���)*/
}
/*---------------------------------------------------------
	�ԍ����w�肵�Ē��ړo�^����B
--------------------------------------------------------- */

global OBJ *obj_regist_direct_number(unsigned int direct_register_number)
{
//	int priority;		priority		= image_resource_ptr->priority;
	OBJ *obj;/* �V�K�쐬����X�v���C�g�I�u�W�F�N�g */
	obj = &obj99[direct_register_number];
	obj_initialize_one(obj);			/* �V�K�쐬�����X�v���C�g�I�u�W�F�N�g�������� */
	return (obj);
}

/*---------------------------------------------------------
	�X�v���C�g�I�u�W�F�N�g ���X�g�ɓo�^���ꂽ�X�v���C�g�I�u�W�F�N�g��S�������B
--------------------------------------------------------- */

/* [A00�e�̈�]��[A01�Œ�G�̈�]��[A02�G�̈�]��[A03�Œ�̈�]��[A04�p�l���̈�]��OBJ��S�����B */
global void obj_system_cleanup_all(void)
{
	OBJ *sss;
	sss = &obj99[OBJ_HEAD_00_0x0000_TAMA+0];
	unsigned int length;
	length = (
		OBJ_POOL_00_TAMA_1024_MAX +/* �e��p */
		OBJ_POOL_01_TOUROKU_0256_MAX +/* gu�ėp(��SDL) */
		OBJ_POOL_02_TEKI_FIX_0256_MAX +/* gu�ėp(��SDL) */
		OBJ_POOL_03_KOTEI_0016_MAX +
		OBJ_POOL_04_PANEL_0056_MAX);
	{
	re_try_loop:
	//	obj_initialize_one(sss);
		sss->jyumyou		= JYUMYOU_NASI;
		#if (1)/* �f�o�b�O�Ă��� */
		sss->center.x256	= (-1);
		#endif
		sss++;
		//
		length--;
		if (0 != length) goto re_try_loop;
	}

}

/*---------------------------------------------------------
	�X�v���C�g�I�u�W�F�N�g �V�X�e���������B
--------------------------------------------------------- */

global void obj_system_init(void)
{
	/*(�o�^�G�X�v���C�g�̏�����)*/
	register_kotei_first	= 0;
	register_kotei_max		= (OBJ_POOL_02_TEKI_FIX_0256_MAX-1/*-(BOSS_OBJ_KOTEI_KAKUHO_BUN)*/);
}

/*---------------------------------------------------------
	�v���C���[�ɂƂ��Ă̓G��T��
---------------------------------------------------------*/

global OBJ *obj_teki_search(void)
{
	int ii;
	for (ii=0; ii<OBJ_POOL_02_TEKI_FIX_0256_MAX; ii++)/* �S�����ׂ�B */
	{
		OBJ *h;
		h = &obj99[OBJ_HEAD_02_0x0900_TEKI_FIX+ii];
	//
		if (
			#if 1
			(JYUMYOU_NASI <= h->jyumyou) && /* �폜�ς݂͔�΂� */
			#endif
			(0 != (h->obj_type_set & (OBJ_Z02_TEKI )) ) 	/* �v���C���[�ɂƂ��Ă̓G(�U�R��{�X) */
		)
		{
			{
				#if 1/* Gu(���S���W) */
				if ((h->center.x256 > 0) && (h->center.x256 < t256(GAME_WIDTH ) ) &&
					(h->center.y256 > 0) && (h->center.y256 < t256(GAME_HEIGHT) ))	/* �\����ʓ��̓G */
				#endif
				{
					return (h);/* �������� */
				}
			}
		}
	}
	return (&obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER]);/* ������Ȃ����� */
}
