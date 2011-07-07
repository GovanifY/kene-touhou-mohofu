
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�A�C�e���֘A
---------------------------------------------------------*/

/*---------------------------------------------------------
	�A�C�e���̈ړ�
---------------------------------------------------------*/

/* �X�̃A�C�e���̃X�e�[�^�X�f�[�^�[ */
//typedef struct
//{
#define ITEM_DATA_true_y256 	user_data00 	/* �{����y���W */
#define ITEM_DATA_y_sum256		user_data01 	/* �A�C�e�������o���p y�� �ώZ�l(y���A������}�C�i�X) */
#define ITEM_DATA_angle1024 	user_data02
#define ITEM_DATA_flags00		user_data03 	/* ���W�t���O	[***090116	�ύX */
/*	�������W */
#define ITEM_DATA_time_in		user_data04 	/* �o������ */
#define ITEM_DATA_x_sa256		user_data05 	/* ���� x */
#define ITEM_DATA_y_sa256		user_data06 	/* ���� y */
//} ITEM_DATA;
/* ���p */
#define ITEM_DATA_flag_first	ITEM_DATA_time_in	/* first�t���O */


/*---------------------------------------------------------
	�A�C�e���̈ړ�(�������W�̏ꍇ)
---------------------------------------------------------*/

static void move_item_type02(SPRITE *src)	/* �������W�Ȃ�΁A�����ɏW�܂� */
{
	{
		src->ITEM_DATA_y_sum256--;
		if (0 <= src->ITEM_DATA_y_sum256)/*0���W�X�^�g��*/
		{
			;
		}
		else
	//	if (1 > src->ITEM_DATA_y_sum256)
		{
			src->ITEM_DATA_y_sum256 = 0;
		}
		if (0 == src->ITEM_DATA_flag_first)
		{
			src->ITEM_DATA_flag_first = 1;
			src->ITEM_DATA_y_sum256 = t256(2.0/*1.0*/); 	/* psp�͉𑜓x���Ⴂ�̂ō׍H(x2) */ 	/* (2.5==5*0.5) */
		}
		SPRITE *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
		src->ITEM_DATA_x_sa256 = (src->cx256 - zzz_player->cx256);
		src->ITEM_DATA_y_sa256 = (src->ITEM_DATA_true_y256 - zzz_player->cy256);
		/* �����ɏW�܂� */
		int aaa_x256;
		int aaa_y256;
		aaa_x256 = ((src->ITEM_DATA_x_sa256 * src->ITEM_DATA_y_sum256)>>9/*8*/);	/*fps_factor*/	/* psp�͉𑜓x���Ⴂ�̂ō׍H(/2) */
		aaa_y256 = ((src->ITEM_DATA_y_sa256 * src->ITEM_DATA_y_sum256)>>9/*8*/);	/*fps_factor*/	/* psp�͉𑜓x���Ⴂ�̂ō׍H(/2) */
		if ( (SP_ITEM_06_HOSI) == (src->type))	/* ���_�̂ݓ��ʏ��� */
		{
			if (
				//	(/*10*/64/*16*/ > data->y_sum256) ||
				(
					#if 1
					/* [��`����] �v���C���𒆐S�Ƃ��āA�c��15x15�̗̈�(���E7[dot], �㉺7[dot]) */
					(t256(8) > abs(aaa_x256)) &&
					(t256(8) > abs(aaa_y256))
					#else
					/* [�H�`����] �v���C���𒆐S�Ƃ��āA���a(?)10[dot]�̕H�`�̈�<>21x21 */
					(t256(11) > (abs(aaa_x256)+abs(aaa_y256)))
					#endif
				)
			)
			{
				src->jyumyou = JYUMYOU_NASI;/* ���_�̂ݓ��ʏ��� */				/* �����܂� */
//
				voice_play(VOICE05_BONUS, TRACK07_GRAZE);/*�e�L�g�[*/
			}
		}
		src->cx256 = zzz_player->cx256 + (aaa_x256);	/*fps_factor*/
		src->ITEM_DATA_true_y256 = zzz_player->cy256 + (aaa_y256);	/*fps_factor*/
	}
//	if (SP_DELETE != src->type)
	if (JYUMYOU_NASI < src->jyumyou)/* ����� */
	{
		#define FIX_LABEL_ITEM_HEIGHT_DIV2 (8/2)/* ��ʊO�\���A�C�e�������̔��� */
		/* ��ʓ��ɕϊ� */
		src->cy256 = src->ITEM_DATA_true_y256;
	//	if (t256(0) >= src->cy256)		/* ��ɂ���Ǝ��Ȃ����Ǒ��� */
		if (t256(-16) >= src->cy256)	/* ���̕ӂ̎d�l�͂��Ԃ�ς��B(Gu�X�v���C�g���W���S�Ǘ��Ƃ���) */
	//	if (t256(FIX_LABEL_ITEM_HEIGHT_DIV2) >= src->cy256) 	/* ���S���W�Ȃ̂� */
		{
			src->cy256 = t256(FIX_LABEL_ITEM_HEIGHT_DIV2);/* ��ʊO�\���A�C�e�������̔��� */
			src->type |= 0x08;
		}
		else
		{
			src->type &= (~0x08);
		}
		reflect_sprite_spec444(src, OBJ_BANK_SIZE_01_ITEM);
	}
}
/*
[��`����] �� + �� + �� �̗̈�ŃA�C�e�����
[�H�`����] �� + �� + �� �̗̈�ŃA�C�e�����
������������������������������������������
������������������������������������������
������������������������������������������
������������������������������������������
������������������������������������������
������������������������������������������
������������������������������������������
������������������������������������������
������������������������������������������
������������������������������������������
������������������������������������������
������������������������������������������
������������������������������������������
������������������������������������������
������������������������������������������
������������������������������������������
������������������������������������������
������������������������������������������
������������������������������������������
������������������������������������������
������������������������������������������
*/

/*---------------------------------------------------------
	�A�C�e���̈ړ�(�ʏ펞���̂Q)
	-------------------------------------------------------
	�A�C�e���������x�ɂ���:
	-------------------------------------------------------
	�A�C�e���������x�̓Q�[���o�����X�Ɋւ�邪�A
	�A�C�e�������m�����̒�������Ă��Ȃ��������邵�A
	�G���p�^�[��(dat)�������ƍ���ĂȂ�����A
	���󂠂܂蒲������Ă��Ȃ��B
	-------------------------------------------------------
	�A�C�e���������x�͌���ɔ�ׂĈُ�ɒx���ݒ肳��Ă���B(�̂ɂ͗��R������)
	(�͕핗��psp�̉𑜓x���Ⴂ�̂ŁA��{�I�Ɍ���� 75% �̑��x���C���[�W���āA
	����ɋ߂��Ȃ�悤�ɐݒ肵�Ă��邪�A������ɒ[�ɒx��(����ɔ������炢)�Ƃ����Ӗ��B)
	-------------------------------------------------------
	����́Apsp�̏c����(272�h�b�g�����Ȃ�)���ȉ߂���ׂ̏��u�B
	������݂̑��x�̖��_�͎�ɉ��L�̂Q�_�B
	�P�D������Ԓx���A�H�X�q�͂܂��������Ȃ��B(�̂Ŗʔ����Ȃ��C������)
	�Q�D���̃A�C�e�����o��������ɂ����B(psp�͎c���������̂œǂ߂Ȃ�)
	-------------------------------------------------------
	����ł́A�L�����ɂ���ė������x���Ⴄ����A�����ŕς��Ă��ǂ��񂾂��A
	���X���x�Ȃ̂ŁA����܌��ʂ��Ȃ��C������B(��������Ə�L�̖��_�������ɂȂ�)
---------------------------------------------------------*/
		#if (0)
		/* [***090123 [***090220	�ύX5=>4=>3 t256(3.0) */
	//	if (src->ITEM_DATA_y_sum256 < t256(1.5) )	/* t256(2.2) �A�C�e�������A�ő呬�x�̒��� */
		if (src->ITEM_DATA_y_sum256 < t256(2.0) )	/* t256(2.2) �A�C�e�������A�ő呬�x�̒��� */
		/* t256(2.2) == �R���e�B�j���[��������[F]����ʂ̉���(�E���A����)�Ŏ��񂾏ꍇ�ŁA
			������Ԓx���A�H�X�q��(�������[�h�Ȃ��)�]�T�Ŏ��鑬�x�ɒ�������B
			(�ᑬ���[�h�ł͎��Ȃ��Ă��\��Ȃ�) */
		#endif


static void move_item_type01(SPRITE *src)
{
	/* �������W���[�h�̂ǂꂩ���쓮���Ă���A */
	if (0 != (
		(cg.state_flag) &
		(STATE_FLAG_01_PLAYER_UP_AUTO_GET_ITEM |		/* MAX���̏��� */
		 STATE_FLAG_02_BOMB_AUTO_GET_ITEM | 			/* �{���������̂݉�� */
		 STATE_FLAG_03_SCORE_AUTO_GET_ITEM				/* �{�X���j���̈ꎞ��� */
		) ))
	{
		src->ITEM_DATA_flags00 |= (ITEM_MOVE_FLAG_01_COLLECT);	/*1*/	/* ���̃A�C�e���͎������W�ɂ��� */
	}
	/* �������W�łȂ���΁A�P���ɕ�������`���ė����� */
	if (0 == ((src->ITEM_DATA_flags00)&ITEM_MOVE_FLAG_01_COLLECT))
	{
		src->ITEM_DATA_y_sum256 += ((src->ITEM_DATA_angle1024)>>1); 	/* x1.5 */
		static const s32 rakka_sokudo[(8)] =
		/* ���܂葬���ƃA�C�e������肫��Ȃ��̂ŁA�~�X��̕������x���x���Ȃ�B
		(�����čH�v���Ȃ��ƑS���͎��Ȃ����炢�ɒ�������B
		�p�^�[�����H�v����Ɩw�ǎ���悤�ɂȂ��Ă�B
		�������A����͎�ɃQ�[������(�`3��)�̘b�ŁA�㔼(4�ʁ`)�͊܂܂Ȃ��B)
		*/
		{
			t256(3.0),	/* REIMU_A */
			t256(3.0),	/* REIMU_B */
			t256(2.0),	/* MARISA_A */	/* �������͎d�l��A�x������B */
			t256(2.0),	/* MARISA_B */	/* �������͎d�l��A�x������B */
			t256(3.0),	/* REMILIA */
			t256(2.0),	/* YUYUKO */	/* �H�X�q�͑����x���̂ŁA�x������B */
			t256(3.0),	/* CIRNO_A */
			t256(4.0),	/* CIRNO_Q */
		};
		int rakka_sokudo_maximum = rakka_sokudo[((cg_game_select_player))];
		/* ���~���A & �H�X�q �̏ꍇ�A�ᑬ�ړ��ŗ������x���ꎞ�I�ɉ�������B */
		if (0x04==((cg_game_select_player) & 0x06) )/* 4:REMILIA or 5:YUYUKO */
		{
			if (/*is_slow =*/ (cg_my_pad & PSP_KEY_SLOW))/* �ᑬ�ړ��̏ꍇ */
			{
				rakka_sokudo_maximum >>= 1;/* �ꎞ�I�ɔ��� */
			}
		}
		if (src->ITEM_DATA_y_sum256 > rakka_sokudo_maximum )	/* t256(2.2) �A�C�e�������A�ő呬�x�̒��� */
		{
			src->ITEM_DATA_y_sum256 = rakka_sokudo_maximum;
		}
		src->ITEM_DATA_true_y256 += (src->ITEM_DATA_y_sum256);/*fps_factor*/
		if (src->ITEM_DATA_true_y256 > GAME_HEIGHT*256)
		{
			/* �E�F�|���A�C�e�� (��P) (��P) (F) �̂����ꂩ��������A�`�F�C���j�� */
			if ((SP_ITEM_02_P128+1) > src->type)
			{
			// SP_ITEM_00_P001: /* �E�F�|���A�C�e��(��P) */
			// SP_ITEM_01_P008: /* �E�F�|���A�C�e��(��P) */
			// SP_ITEM_02_P128: /* �E�F�|���A�C�e��(F) */
				cg.chain_point = 0;/* �`�F�C���j�� */
				//;
			}
			// else {;} 	/* [�_][��][B][1UP]�������Ă��A�`�F�C���ێ��B */
			src->jyumyou = JYUMYOU_NASI;				/* ��ʊO�ɏo���炨���܂� */
		}
	}
	else	/* �������W�Ȃ�΁A�����ɏW�܂� */
	{
		src->callback_mover 		= move_item_type02;
		src->ITEM_DATA_flag_first	= 0;
	}
//	if (SP_DELETE != src->type)
	if (JYUMYOU_NASI < src->jyumyou)/* ����� */
	{
		/* ��ʓ��ɕϊ� */
		src->cy256 = src->ITEM_DATA_true_y256;
//		if (t256(0) >= src->cy256)		/* ��ɂ���Ǝ��Ȃ����Ǒ��� */
		if (t256(-16) >= src->cy256)	/* ���̕ӂ̎d�l�͂��Ԃ�ς��B(Gu�X�v���C�g���W���S�Ǘ��Ƃ���) */
//		if (t256(FIX_LABEL_ITEM_HEIGHT_DIV2) >= src->cy256) 	/* ���S���W�Ȃ̂� */
		{
			src->cy256 = t256(FIX_LABEL_ITEM_HEIGHT_DIV2);/* ��ʊO�\���A�C�e�������̔��� */
			src->type |= 0x08;
		}
		else
		{
			src->type &= (~0x08);
		}
		reflect_sprite_spec444(src, OBJ_BANK_SIZE_01_ITEM);
	}
}

/*---------------------------------------------------------
	�A�C�e���̈ړ�(�ʏ펞���̂P)
	���񎞂̓����o�������B
	�����o�����͂����蔻��Ȃ��B(����ł��Ȃ�)
---------------------------------------------------------*/
#if 0
static void move_item_type00(SPRITE *src)
{
}
#endif

/*---------------------------------------------------------
	�A�C�e���o��������q�֐�
---------------------------------------------------------*/
/*int x, int y*/
static SPRITE *item_mono_create(SPRITE *src, int sel_type)/* */
{
	//	�A�C�e���̎�ނ�I��
	SPRITE *h;
	h						= obj_add_00_tama_error();
	if (NULL != h)
	{
		sel_type &= 0x07;		/* �A�C�e���̎�� == �O���t�B�b�N�̎�� */
		sel_type |= SP_ITEM_00_P001;
		h->type 				= sel_type;
		reflect_sprite_spec444(h, OBJ_BANK_SIZE_01_ITEM);
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
	//
		{
		//	�o���ʒu�����߂�
			#define OFFSET_X64		(64*256)/* 64�͂��炵�� 2��n��̕K�v�L�� */
			#define ITEM_WIDTH16	(16*256)/* 16�̓A�C�e���� */
			#define ITEM_X_LIMIT	(GAME_WIDTH*256+OFFSET_X64-ITEM_WIDTH16)
			int x256;
			x256 = src->cx256;
			x256 += ((ra_nd()&((OFFSET_X64+OFFSET_X64)-1)));
			if (x256 < OFFSET_X64)
			{
				x256 = OFFSET_X64;
			}
			else
			if (x256 > (ITEM_X_LIMIT))
			{
				x256 = (ITEM_X_LIMIT);
			}
		//	�o�^����
			h->cx256				= (x256)-(OFFSET_X64);
		}
		h->cy256					= src->cy256;

		h->callback_mover			= move_item_type01;
	//
	//
		if ( SP_ITEM_06_HOSI == sel_type )	/* ���_�̂ݓ��ʏ��� */
		{
			/* �g�́A�����炵�� */
			if ( USER_BOMOUT_WAIT > cg.bomber_time )
			{	/* �{����(�ݒ薳�G���Ԓ�)��100pts.(�҂��Ȃ�) */
				bonus_info_score_nodel(h, SCORE_100);/*���������֎d�l�ύXs->jyumyou = JYUMYOU_NASI;*/
			}
			else/* ���_ */
			{	/* �{����̎������G���Ԓ��͂�����(�҂���) */
				/* ((graze/3)*10)+(500) pts */
				bonus_info_any_score_nodel(h, (score(500)+(((cg.graze_point*86)>>8)/*(pd->graze_point/3)*/)) );/*���������֎d�l�ύXc->jyumyou = JYUMYOU_NASI;*/
			}
			h->flags			= (0/*SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER*/);/*�����蔻��Ȃ�*/
		}
	//	else
	//	{
	//	}
	}
	#undef OFFSET_X64
	#undef ITEM_WIDTH16
	#undef ITEM_X_LIMIT
	return (h);
}


/*---------------------------------------------------------
	�A�C�e����o�^���ďo��������
---------------------------------------------------------*/

static void s_item_convert_hosi(SPRITE *h)
{
	{
		h->callback_mover	= move_item_type01;
	//	if ( SP_ITEM_06_HOSI == sel_type )	/* ���_�̂ݓ��ʏ��� */
		{
			/* �g�́A�����炵�� */
			if ( USER_BOMOUT_WAIT > cg.bomber_time )
			{	/* �{����(�ݒ薳�G���Ԓ�)��100pts.(�҂��Ȃ�) */
				bonus_info_score_nodel(h, SCORE_100);/*���������֎d�l�ύXs->jyumyou = JYUMYOU_NASI;*/
			}
			else/* ���_ */
			{	/* �{����̎������G���Ԓ��͂�����(�҂���) */
				/* ((graze/3)*10)+(500) pts */
				bonus_info_any_score_nodel(h, (score(500)+(((cg.graze_point*86)>>8)/*(pd->graze_point/3)*/)) );/*���������֎d�l�ύXc->jyumyou = JYUMYOU_NASI;*/
			}
//			h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);/*�����蔻��Ȃ�*/
		}
	}
		/* �G�t�F�N�g�̏ꍇ�����_�ɂȂ�B */
		h->type = SP_ITEM_06_HOSI;
		{
			/* �G�t�F�N�g�̏ꍇ�������Ȃ̂ŁA���ɖ߂��B */
			h->color32		= (0xffffffff);
			/* �G�t�F�N�g�̏ꍇ�������Ȃ̂ŁA�����ɖ߂��B */
			h->m_zoom_x256	= t256(1.0);
			h->m_zoom_y256	= t256(1.0);
		}
		reflect_sprite_spec444(h, OBJ_BANK_SIZE_01_ITEM);
		h->ITEM_DATA_true_y256		= (/*src*/h->cy256); /* ���z */

//		h->ITEM_DATA_angle1024		= (2*6);	/* 2*6.51898646904403967309077986986488 cv1024r(0.08);*/
		h->ITEM_DATA_flags00		= ((ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY)&ITEM_MOVE_FLAG_01_COLLECT);
//	}
	h->rotationCCW1024				= (0);			/* �`��p�p�x(����0�x�ō����(�����v���)) */	/* 0 == �X���Ȃ��B����0�x */
}


/*---------------------------------------------------------
	���Ă̓G�e���Ahosi�A�C�e���ɕς���
---------------------------------------------------------*/
//extern SPRITE *sprite_list444_head;/* �e��p�X�v���C�g ���X�g */
//void item_from_bullets(int put_item_num)
extern void item_convert_hosi(SPRITE */*src*/h);
global void bullets_to_hosi(void)
{
//	SPRITE *s = sprite_list444_head;/* �e��p�X�v���C�g ���X�g�̐擪 ����T�� */
//	while (NULL != s)/* �X�v���C�g ���X�g�̍Ō�܂Œ��ׂ� */

	int ii;
	for (ii=0; ii<OBJ_POOL_00_TAMA_MAX; ii++ )/* �S�����ׂ�B */
	{
		SPRITE *s;
		s = &obj99[OBJ_HEAD_00_TAMA+ii];
	//
	//	#if 1/* �v��Ȃ��C������(�Ă��Ƃ���) */
	//	if (s->type != SP_DELETE ) /* �폜�ς݂͔�΂� */
	//	#endif
		if (JYUMYOU_NASI < s->jyumyou)/* ����� */
		{
			if (SP_GROUP_BULLETS & s->type)
			{
			//	item_create(s, /*put_item_num*/SP_IT EM_06_HOSI/*SP_IT EM_06_HOSI*/, 1, (IT EM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY) );
			//	s->jyumyou = JYUMYOU_NASI;
			//	s->type = SP_IT EM_06_HOSI;
				s_item_convert_hosi(s);//, /*put_item_num*/SP_IT EM_06_HOSI/*SP_IT EM_06_HOSI*/, 1, (IT EM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY) );
			}
//			s = s->next;/*��*/
		}
	}
}


/*---------------------------------------------------------
	�A�C�e����o�^���ďo��������
---------------------------------------------------------*/

/* [***090125	�ǉ�: up_flags �� ITEM_MOVE_FLAG_01_COLLECT �r�b�g���I���Ńv���C���[�ɏW�܂�܂��B */
global void item_create(
	SPRITE *src,/*int x, int y*/
	int item_type,
	int num_of_creates,
	int up_flags
)
{
	#if 0
	if ( (CIRNO_A-1) < ((cg_game_select_player)) )/* 6:CIRNO_A or 7:CIRNO_Q */
	{
	//	if (item_type<SP_ITEM_02_P128)		// [��p][��P]	 �͏o�Ȃ��B
		if (item_type<(SP_ITEM_02_P128+1))	// [��p][��P][F] �͏o�Ȃ��B
		{
			item_type = SP_ITEM_05_TENSU;
		}
	}
	else
	// ���~���A�̏ꍇ[�_]�͏o���Ȃ�.
	if (REMILIA==(cg_game_select_player))
	{
		if (item_type==(SP_ITEM_05_TENSU))	// [�_] �͏o�Ȃ��B
		{
			item_type = SP_ITEM_00_P001;	// [��p]
		}
	}
	#endif
	{
		// �`���m�̏ꍇ�A�O���C�Y�Ńp���[�A�b�v����̂�[��p][��P][F]�͏o���Ȃ��B[�_](�X)�ɂȂ�B
		// ���~���A�̏ꍇ[P](��)�����o���Ȃ��B
		// �H�X�q�̏ꍇ[��p]����[�_]�A[��P]����[�{��]�A������ւ���Ă���B
		const u8 item_henkan[(8)*(8)] =
		{/* �얲(�아)		�얲(����)					������(����)				������(����)				���~���A(����)				�H�X�q(����)				�`���m(�X��)				�`���m(�H�n)			*/
(SP_ITEM_00_P001&0xff), 	(SP_ITEM_00_P001&0xff), 	(SP_ITEM_00_P001&0xff), 	(SP_ITEM_00_P001&0xff), 	(SP_ITEM_00_P001&0xff), 	(SP_ITEM_05_TENSU&0xff),	(SP_ITEM_05_TENSU&0xff),	(SP_ITEM_05_TENSU&0xff),
(SP_ITEM_01_P008&0xff), 	(SP_ITEM_01_P008&0xff), 	(SP_ITEM_01_P008&0xff), 	(SP_ITEM_01_P008&0xff), 	(SP_ITEM_01_P008&0xff), 	(SP_ITEM_01_P008&0xff), 	(SP_ITEM_05_TENSU&0xff),	(SP_ITEM_05_TENSU&0xff),
(SP_ITEM_02_P128&0xff), 	(SP_ITEM_02_P128&0xff), 	(SP_ITEM_02_P128&0xff), 	(SP_ITEM_02_P128&0xff), 	(SP_ITEM_01_P008&0xff), 	(SP_ITEM_04_BOMB&0xff), 	(SP_ITEM_05_TENSU&0xff),	(SP_ITEM_05_TENSU&0xff),
(SP_ITEM_03_1UP&0xff),		(SP_ITEM_03_1UP&0xff),		(SP_ITEM_03_1UP&0xff),		(SP_ITEM_03_1UP&0xff),		(SP_ITEM_03_1UP&0xff),		(SP_ITEM_03_1UP&0xff),		(SP_ITEM_03_1UP&0xff),		(SP_ITEM_03_1UP&0xff),
(SP_ITEM_04_BOMB&0xff), 	(SP_ITEM_04_BOMB&0xff)	,	(SP_ITEM_04_BOMB&0xff), 	(SP_ITEM_04_BOMB&0xff), 	(SP_ITEM_01_P008&0xff), 	(SP_ITEM_02_P128&0xff), 	(SP_ITEM_04_BOMB&0xff), 	(SP_ITEM_04_BOMB&0xff),
(SP_ITEM_05_TENSU&0xff),	(SP_ITEM_05_TENSU&0xff),	(SP_ITEM_05_TENSU&0xff),	(SP_ITEM_05_TENSU&0xff),	(SP_ITEM_00_P001&0xff), 	(SP_ITEM_00_P001&0xff), 	(SP_ITEM_05_TENSU&0xff),	(SP_ITEM_05_TENSU&0xff),
(SP_ITEM_06_HOSI&0xff), 	(SP_ITEM_06_HOSI&0xff), 	(SP_ITEM_06_HOSI&0xff), 	(SP_ITEM_06_HOSI&0xff), 	(SP_ITEM_06_HOSI&0xff), 	(SP_ITEM_06_HOSI&0xff), 	(SP_ITEM_06_HOSI&0xff), 	(SP_ITEM_06_HOSI&0xff),
(SP_ITEM_07_SPECIAL&0xff),	(SP_ITEM_07_SPECIAL&0xff),	(SP_ITEM_07_SPECIAL&0xff),	(SP_ITEM_07_SPECIAL&0xff),	(SP_ITEM_07_SPECIAL&0xff),	(SP_ITEM_07_SPECIAL&0xff),	(SP_ITEM_07_SPECIAL&0xff),	(SP_ITEM_07_SPECIAL&0xff),
		};
		item_type = (/*SP_ITEM_00_P001*/SP_GROUP_ITEMS | (item_henkan[((item_type&0x07)<<3)+(cg_game_select_player)]));
	}
	//
	int i;
	for (i=0; i<num_of_creates; i++)
	{
		SPRITE *h;
		h			= item_mono_create(src, item_type);
		#if 1
		if (NULL==h) return;
		#else
		if (NULL != h)
		#endif
		{
			h->ITEM_DATA_true_y256		= (src->cy256); /* ���z */
			#if 0
			/* ��{�I�ɉ�ʊO(��)�ɂ��܂��ݏo���Ȃ�(���^�C�v) */
			/* �A�C�e���}�[�J�[���쐬���Ȃ������O��̓��� */
			h->ITEM_DATA_y_sum256		= -(/*256*/((src->cy256)>>7))-(ra_nd()&0xff);	/* �A�C�e�������o�������l(y���A������}�C�i�X) */
			#else
			/* ��ʊO(��)�ɋ�ݏo��(��ݏo���}�[�J�[�K�{) */
			/* �A�C�e���}�[�J�[���쐬���鎖���O��̓��� */
		//	h->ITEM_DATA_y_sum256		= -(/*256*/((200*256)>>7))-(ra_nd()&0xff);		/* �A�C�e�������o�������l(y���A������}�C�i�X) */
			h->ITEM_DATA_y_sum256		= -(/*256*/((512)))-(ra_nd()&0xff); 			/* �A�C�e�������o�������l(y���A������}�C�i�X) */
			#endif
			h->ITEM_DATA_angle1024		= (6*2);	/* 2*6.51898646904403967309077986986488 cv1024r(0.08);*/
			h->ITEM_DATA_flags00		= (up_flags&ITEM_MOVE_FLAG_01_COLLECT);
		}
	}
}

/*---------------------------------------------------------
	�{�X�p�A�C�e���쐬
---------------------------------------------------------*/
//	#define ITEM_CREATE_MODE_01 	(12*0)
//	#define ITEM_CREATE_MODE_02 	(12*1)
global void item_create_for_boss(SPRITE *src, int item_create_mode)
{
	#define ITEM_03 	(4*0)
	#define ITEM_02 	(4*1)
	#define ITEM_01_1	(4*2)
	#define ITEM_01_2	(4*3)
	#define ITEM_01_3	(4*4)
	#define ITEM_01_4	(4*5)
	#define ITEM_01_5	(4*6)
	#define ITEM_01_6	(4*7)
	#define ITEM_MAX	(4*8)
	const u8 u8_item_tbl[ITEM_MAX*2] =
	{	/* easy, normal, hard, lunatic */
	#if 0
	/* �� */
	/*mode 0: ���j�� */
		(SP_ITEM_03_1UP&7), 	(SP_ITEM_00_P001&7),	(SP_ITEM_04_BOMB&7),	(SP_ITEM_04_BOMB&7),
		(SP_ITEM_02_P128&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_04_BOMB&7),	(SP_ITEM_01_P008&7),
		(SP_ITEM_00_P001&7),	(SP_ITEM_04_BOMB&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_05_TENSU&7),	/*stage1*/
		(SP_ITEM_00_P001&7),	(SP_ITEM_04_BOMB&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_05_TENSU&7),	/*stage2*/
		(SP_ITEM_00_P001&7),	(SP_ITEM_04_BOMB&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_05_TENSU&7),	/*stage3*/
		(SP_ITEM_00_P001&7),	(SP_ITEM_04_BOMB&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_05_TENSU&7),	/*stage4*/
		(SP_ITEM_00_P001&7),	(SP_ITEM_04_BOMB&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_05_TENSU&7),	/*stage5*/
		(SP_ITEM_00_P001&7),	(SP_ITEM_04_BOMB&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_05_TENSU&7),	/*stage6*/
	/*mode 1: ���j�� */
		(SP_ITEM_04_BOMB&7),	(SP_ITEM_00_P001&7),	(SP_ITEM_00_P001&7),	(SP_ITEM_00_P001&7),
		(SP_ITEM_00_P001&7),	(SP_ITEM_00_P001&7),	(SP_ITEM_00_P001&7),	(SP_ITEM_01_P008&7),
		(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_05_TENSU&7),	/*stage1*/
		(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_05_TENSU&7),	/*stage2*/
		(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_05_TENSU&7),	/*stage3*/
		(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_05_TENSU&7),	/*stage4*/
		(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_05_TENSU&7),	/*stage5*/
		(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_05_TENSU&7),	/*stage6*/
	#else
	/* �V(�e�X�g��) */
	/*mode 0: ���j�� */ 	/* easy, normal, hard, lunatic */
		(SP_ITEM_03_1UP&7), 	(SP_ITEM_03_1UP&7), 	(SP_ITEM_04_BOMB&7),	(SP_ITEM_04_BOMB&7),	/* (����stage7�܂ō��Ƃ�����stage7�N���A�[��(�N���A�[��Ȃ̂ŃQ�[���I�ɂ͈Ӗ�����)�����������p����) */
		(SP_ITEM_03_1UP&7), 	(SP_ITEM_04_BOMB&7),	(SP_ITEM_04_BOMB&7),	(SP_ITEM_01_P008&7),
		(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_05_TENSU&7),	/*stage1*/	/*alice*/
		(SP_ITEM_00_P001&7),	(SP_ITEM_04_BOMB&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_05_TENSU&7),	/*stage2*/	/*mima*/
		(SP_ITEM_00_P001&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_04_BOMB&7),	(SP_ITEM_01_P008&7),	/*stage3*/	/*kaguya*/
		(SP_ITEM_00_P001&7),	(SP_ITEM_04_BOMB&7),	(SP_ITEM_04_BOMB&7),	(SP_ITEM_01_P008&7),	/*stage4*/	/*aya*/
		(SP_ITEM_00_P001&7),	(SP_ITEM_04_BOMB&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_05_TENSU&7),	/*stage5*/	/*pache*/
		(SP_ITEM_00_P001&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_05_TENSU&7),	/*stage6*/	/*sakuya*/		/*---- sakuya���j��̓Q�[���I�ɂ͈Ӗ����� */
	/*mode 1: ���j�� */
		(SP_ITEM_04_BOMB&7),	(SP_ITEM_00_P001&7),	(SP_ITEM_00_P001&7),	(SP_ITEM_00_P001&7),
		(SP_ITEM_00_P001&7),	(SP_ITEM_00_P001&7),	(SP_ITEM_00_P001&7),	(SP_ITEM_01_P008&7),
		(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_05_TENSU&7),	/*stage1*/
		(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_05_TENSU&7),	/*stage2*/
		(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_05_TENSU&7),	/*stage3*/
		(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_05_TENSU&7),	/*stage4*/
		(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_05_TENSU&7),	/*stage5*/
		(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_05_TENSU&7),	/*stage6*/
	#endif
	};
//	item_create(src, item_tbl[ITEM_03+difficulty+item_create_mode]/*SP_ITEM_03_1UP	*/, 1, ITEM_MOVE_FLAG_06_RAND_XY);
//	item_create(src, item_tbl[ITEM_02+difficulty+item_create_mode]/*SP_ITEM_02_P128 */, 1, ITEM_MOVE_FLAG_06_RAND_XY);
//	item_create(src, item_tbl[ITEM_01+difficulty+item_create_mode]/*SP_ITEM_01_P008 */, cg.game_now_stage/*5*/, ITEM_MOVE_FLAG_06_RAND_XY);
	int i;
	for (i=0; i<1+(((cg.game_now_stage>>1))&0x0f); i++)
	{
		item_create(src, SP_GROUP_ITEMS+u8_item_tbl[item_create_mode+(((cg_game_difficulty)+i)&((ITEM_MAX)-1))]/*SP_ITEM_03_1UP */, 1, ITEM_MOVE_FLAG_06_RAND_XY);
	}
	item_create(src, SP_ITEM_05_TENSU/*SP_IT EM_06_HOSI*/, (16)/*(7)*/, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY) );/*���_���o��*/
}





/*-------------------------------------------------------*/
/*-------------------------------------------------------*/
/*-------------------------------------------------------*/

/*---------------------------------------------------------
	�����_���A�C�e���G�̏ꍇ�Ɏg���A���ʔ����e�[�u��
---------------------------------------------------------*/

static unsigned int random_item_seed;		/*=0*/

global void teki_random_item_table_initialize(void) 	/*int set_seed*/
{
	random_item_seed = (ra_nd()&(8-1)); 	/*set_seed*/
}
static int s_teki_get_random_item(void)
{
	static const u16/*int*/ item_table[32] =
	{
		SP_ITEM_01_P008,	SP_ITEM_00_P001,	SP_ITEM_00_P001,	SP_ITEM_05_TENSU,
		SP_ITEM_00_P001,	SP_ITEM_05_TENSU,	SP_ITEM_00_P001,	SP_ITEM_00_P001,
		SP_ITEM_05_TENSU,	SP_ITEM_05_TENSU,	SP_ITEM_05_TENSU,	SP_ITEM_00_P001,
		SP_ITEM_00_P001,	SP_ITEM_00_P001,	SP_ITEM_05_TENSU,	SP_ITEM_05_TENSU,
		SP_ITEM_00_P001,	SP_ITEM_05_TENSU,	SP_ITEM_00_P001,	SP_ITEM_05_TENSU,
		SP_ITEM_00_P001,	SP_ITEM_05_TENSU,	SP_ITEM_00_P001,	SP_ITEM_05_TENSU,
		SP_ITEM_00_P001,	SP_ITEM_05_TENSU,	SP_ITEM_00_P001,	SP_ITEM_00_P001,
		SP_ITEM_05_TENSU,	SP_ITEM_05_TENSU,	SP_ITEM_05_TENSU,	SP_ITEM_00_P001,
	};
	random_item_seed++; 	random_item_seed &= 0x1f;
	return (item_table[random_item_seed]);
}


/*---------------------------------------------------------
	�G����
---------------------------------------------------------*/

global /*static*/ void lose_random_item(SPRITE *src)
{
	item_create(src, s_teki_get_random_item(), (1), (/*IT EM_MOVE_FLAG_01_COLLECT|*/ITEM_MOVE_FLAG_06_RAND_XY)/*(up_flags)*/ );
}

/*-------------------------------------------------------*/
/*-------------------------------------------------------*/
/*-------------------------------------------------------*/

/*---------------------------------------------------------
	��Փx�X�R�A�␳
---------------------------------------------------------*/

global u32 adjust_score_by_difficulty(u32 convert_score)
{
	u32 mul_tbl256[4] = /* ��Փx�␳ */
	{
//	/* easy */		t256(0.5),	/*���앗*/
//	/* normal */	t256(1.0),	/*���앗*/
//	/* hard */		t256(1.2),	/*���앗*/
//	/* lunatic */	t256(1.5),	/*���앗*/
	/* easy */		t256(0.5),	/*�͕핗*/
	/* normal */	t256(1.0),	/*�͕핗*/
	/* hard */		t256(2.0),	/*�͕핗*/
	/* lunatic */	t256(5.0),	/*�͕핗*/
	};
	return (((convert_score)*(mul_tbl256[(cg_game_difficulty)]))>>8);
}

/*
���e��J�E���^�[�X�g�b�v
�@���_�̃J���X�g�@�@�X�X���X�X�X�X���X�X�X�O�_
�@������񐔁@�@�@�@�X�X�X�X�X��
�@�v���C���[���@�@�@�X�l
�@�X�y���J�[�h�񐔁@�X�X�X�X��
*/
/*---------------------------------------------------------
	�X�R�A���Z�����ƁA�K���������Ă΂��B
	�����ŃG�N�X�e���h�`�F�b�N(�c�@�����_�ő�����`�F�b�N)��
	����\��B(�����ǁA�܂�����Ă��Ȃ�)
	-------------------------------------------------------
	�G�N�X�e���h�`�F�b�N�͎��s���Ԃ��ǂ߂Ȃ��̂̓}�Y��
	��1�b���ɕK���`�F�b�N�ɂ��������B
---------------------------------------------------------*/
//#define PLAYER_MAX_SCORE	(score( 9999999990))
#define PLAYER_MAX_SCORE	(	  ( 999999999 ))

global void player_dummy_add_score(u32 score_num_pts)
{
	cg.game_score += score_num_pts;
	/* �J���X�g(�X�R�A�J�E���^�[ �X�g�b�v)�`�F�b�N����1�b(60flame)��1���
		�����I�ɂ͖��Ȃ��񂾂��ǁA�\�����ςɂȂ�Ǝv����B */
	#if (1==USE_MAX_SCORE_COUNTER_STOP_CHECK)
	/* �J���X�g(�X�R�A�J�E���^�[ �X�g�b�v)�`�F�b�N */
	if ( PLAYER_MAX_SCORE < cg.game_score ) /* �J���X�g �`�F�b�N */
	{
		cg.game_score = PLAYER_MAX_SCORE;
	}
	#endif /* (1==USE_MAX_SCORE_COUNTER_STOP_CHECK) */
}



#if (1==USE_EXTEND_CHECK)
static u32 extend_check_score;
static int extend_check_counter;
#endif /* (1==USE_EXTEND_CHECK) */
#if (1==USE_EXTEND_CHECK)
/* score_panel.c: �G�N�X�e���h�`�F�b�N�͖�1�b(60flame)��1��Ŗ��Ȃ��Ǝv���B */
global void player_check_extend_score(void)
{
	/* 1000���A2500���A5000���A10000��(1��)�ŃG�N�X�e���h */
	#if (1==USE_EXTEND_CHECK)
	if ( extend_check_score < cg.game_score )	/* �G�N�X�e���h �`�F�b�N */
	{
		cg.zanki++; 	/* �G�N�X�e���h */
		/*
			�X�R�A�ɂ��G�N�X�e���h���͓���Ȃ̂ŁA�ڗ��ׂ��B
			(�A�C�e���ɂ��G�N�X�e���h���ƈ���āA�ڂŊm�F���Ȃ�)
			���ۂ���Ă݂���Q�炵�āA���x�ǂ��B
			���ɂ��邳���͂Ȃ������B
		*/
		voice_play(VOICE06_EXTEND, TRACK03_SHORT_MUSIC/*TRACK02_ALEART_IVENT*/);		/* �G�N�X�e���h�� */
		voice_play(VOICE06_EXTEND, TRACK01_EXPLODE);/*�\��(�K�v)*/						/* �G�N�X�e���h�� */
		{
			static const u32 extend_score_tbl[4/*8*/] =
			{
				0x7fffffff, 			/* �G�N�X�e���h �X�g�b�p�[(����ȏ�G�N�X�e���h���Ȃ�) */
				score(	 25000000),
				score(	 50000000),
				score(	100000000),
			};
			//	score(	 10000000),
			//	score( 9999999990), 	/*==0x3b9ac9ff*/
			//	score(21474836470), 	/*==0x7fffffff*/
			//	score(42949672950), 	/*==0xffffffff*/
			extend_check_counter++;
			extend_check_counter &= (4-1);
			extend_check_score = extend_score_tbl[extend_check_counter];
		}
	}
	#endif /* (1==USE_EXTEND_CHECK) */
}
#endif
#if (1==USE_EXTEND_CHECK)


/*---------------------------------------------------------
	�v���C���[�J�n���^�v���C���[�R���e�B�j���[�J�n���ɌĂ΂��B
---------------------------------------------------------*/
global void player_init_extend_score(void)
{
	extend_check_score		= score(   10000000);
	extend_check_counter	= 0;
}
#endif /* (1==USE_EXTEND_CHECK) */
