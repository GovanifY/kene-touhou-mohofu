


/*---------------------------------------------------------
	�A�C�e���֘A
---------------------------------------------------------*/

#include "game_main.h"
#include "bonus.h"

/*---------------------------------------------------------
	�A�C�e���̈ړ�
---------------------------------------------------------*/

/* �X�̃A�C�e���̃X�e�[�^�X�f�[�^�[ */
//typedef struct
//{
#define ITEM_DATA_true_y256 	user_data00 	/* �{����y���W */
#define ITEM_DATA_y_sum256		user_data01 	/* �A�C�e�������o���p y�� �ώZ�l(y���A������}�C�i�X) */
#define ITEM_DATA_angle512		user_data02
#define ITEM_DATA_flags00		user_data03 	/* ���W�t���O	[***090116	�ύX */
/*	�������W */
#define ITEM_DATA_flag_first	user_data04 	/* first�t���O */
#define ITEM_DATA_x_sa256		user_data05 	/* ���� x */
#define ITEM_DATA_y_sa256		user_data06 	/* ���� y */
//} ITEM_DATA;

static void move_items(SPRITE *src)
{
//	ITEM_DATA *data = (ITEM_DATA *)src->data;
	/* �������W���[�h�̂ǂꂩ���쓮���Ă���A */
	if (0 != (
		(pd_state_flag) &
		(STATE_FLAG_01_PLAYER_UP_AUTO_GET_ITEM |		/* MAX���̏��� */
		 STATE_FLAG_02_BOMB_AUTO_GET_ITEM | 			/* �{���������̂݉�� */
		 STATE_FLAG_03_SCORE_AUTO_GET_ITEM				/* �{�X���j���̈ꎞ��� */
		) ))
	{
		src->ITEM_DATA_flags00 |= ITEM_MOVE_FLAG_01_COLLECT/*1*/;	/* ���̃A�C�e���͎������W�ɂ��� */
	}
	/* �������W�łȂ���΁A�P���ɕ�������`���ė����� */
	if (0 == ((src->ITEM_DATA_flags00)&ITEM_MOVE_FLAG_01_COLLECT))
	{
		/* [***090123 [***090220	�ύX5=>4=>3 t256(3.0) */
	//	if (src->ITEM_DATA_y_sum256 < t256(1.5) )	/* t256(2.2) �A�C�e�������A�ő呬�x�̒��� */
		if (src->ITEM_DATA_y_sum256 < t256(2.0) )	/* t256(2.2) �A�C�e�������A�ő呬�x�̒��� */
		/* t256(2.2) == �R���e�B�j���[��������[F]����ʂ̉���(�E���A����)�Ŏ��񂾏ꍇ�ŁA
			������Ԓx���A�H�X�q��(�������[�h�Ȃ��)�]�T�Ŏ��鑬�x�ɒ�������B
			(�ᑬ���[�h�ł͎��Ȃ��Ă��\��Ȃ�) */
		{
			src->ITEM_DATA_y_sum256 += src->ITEM_DATA_angle512; 	/* x1.5 */
		}
		src->ITEM_DATA_true_y256 += (src->ITEM_DATA_y_sum256);/*fps_factor*/
		if (src->ITEM_DATA_true_y256 > GAME_HEIGHT*256)
		{
			/* �E�F�|���A�C�e�� (��P) (��P) (F) �̂����ꂩ��������A�`�F�C���j�� */
			switch (src->type)
			{
			case SP_ITEM_00_P001:	/* �E�F�|���A�C�e��(��P) */
			case SP_ITEM_01_P008:	/* �E�F�|���A�C�e��(��P) */
			case SP_ITEM_02_P128:	/* �E�F�|���A�C�e��(F) */
				pd_chain_point = 0;
				break;
			/* [�_][��][B][1UP]�������Ă��A�`�F�C���ێ��B */
			}
			src->type = SP_DELETE;				/* ��ʊO�ɏo���炨���܂� */
		}
	}
	else	/* �������W�Ȃ�΁A�����ɏW�܂� */
	{
		src->ITEM_DATA_y_sum256--;
		if (1 > src->ITEM_DATA_y_sum256)
		{
			src->ITEM_DATA_y_sum256 = 0;
		}
		if (0 == src->ITEM_DATA_flag_first)
		{
			src->ITEM_DATA_flag_first = 1;
			src->ITEM_DATA_y_sum256 = t256(4.0/*1.0*/); 	/* psp�͉𑜓x���Ⴂ�̂ō׍H(x4) */ 	/* (2.5==5*0.5) */
		}
		src->ITEM_DATA_x_sa256 = (src->x256 - player->x256);
		src->ITEM_DATA_y_sa256 = (src->ITEM_DATA_true_y256 - player->y256);
		/* �����ɏW�܂� */
		int aaa_x256;
		int aaa_y256;
		aaa_x256 = ((src->ITEM_DATA_x_sa256 * src->ITEM_DATA_y_sum256)>>10/*8*/); /*fps_factor*/	/* psp�͉𑜓x���Ⴂ�̂ō׍H(/4) */
		aaa_y256 = ((src->ITEM_DATA_y_sa256 * src->ITEM_DATA_y_sum256)>>10/*8*/); /*fps_factor*/	/* psp�͉𑜓x���Ⴂ�̂ō׍H(/4) */
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
				src->type = SP_DELETE;/* ���_�̂ݓ��ʏ��� */				/* �����܂� */
//
				voice_play(VOICE05_BONUS, TRACK07_GRAZE);/*�e�L�g�[*/
			}
		}
		src->x256 = player->x256 + (aaa_x256);	/*fps_factor*/
		src->ITEM_DATA_true_y256 = player->y256 + (aaa_y256);	/*fps_factor*/
	}
	if (SP_DELETE != src->type)
	{
		/* ��ʓ��ɕϊ� */
		src->y256 = src->ITEM_DATA_true_y256;
		#if 0
		if (t256(0) >= src->y256)		/* ��ɂ���Ǝ��Ȃ����Ǒ��� */
		#else
		if (t256(-16) >= src->y256)		/* ���̕ӂ̎d�l�͂��Ԃ�ς��B(Gu�X�v���C�g���W���S�Ǘ��Ƃ���) */
		#endif
		{
			src->y256 = 0;
			src->type |= 0x08;
		}
		else
		{
			src->type &= (~0x08);
		}
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
	�A�C�e���o��������q�֐�
---------------------------------------------------------*/
/*int x, int y*/
static SPRITE *item_mono_create(SPRITE *src, int sel_type)
{
//	�A�C�e���̎�ނ�I��
	SPRITE *s;
	s			= sprite_add_only_bullet(ZAKO_TYPE_ATARI16_PNG);
	sel_type &= 0x07;
//	s->yx_an im_frame = ((/*SP_ITEM_FIRE_POWER-*/sel_type)/*&0x07*/);
//	�o���ʒu�����߂�
	#define OFFSET_X64		(64*256)/* 64�͂��炵�� 2��n��̕K�v�L�� */
	#define ITEM_WIDTH16	(16*256)/* 16�̓A�C�e���� */
	#define ITEM_X_LIMIT	(GAME_WIDTH*256+OFFSET_X64-ITEM_WIDTH16)
	int x256;
	x256 = src->x256;
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
	s->x256 			= (x256)-(OFFSET_X64);
	s->y256 			= src->y256;
//	sel_type &= 0x07;
	sel_type |= SP_ITEM_00_P001;
	s->type 			= sel_type;
	s->callback_mover	= move_items;
	if ( SP_ITEM_06_HOSI == sel_type )	/* ���_�̂ݓ��ʏ��� */
	{
		/* �g�́A�����炵�� */
	//	if ( USER_BOMOUT_WAIT > pd_bomber_time )
		if ( USER_BOMOUT_WAIT > pd_bomber_time )
		{	/* �{����(�ݒ薳�G���Ԓ�)��100pts.(�҂��Ȃ�) */
			bonus_info_score_nodel(s, SCORE_100);/*���������֎d�l�ύXs->type = SP_DELETE;*/
		}
		else/* ���_ */
		{	/* �{����̎������G���Ԓ��͂�����(�҂���) */
			/* ((graze/3)*10)+(500) pts */
			bonus_info_any_score_nodel(s, (score(500)+(((pd_graze_point*86)>>8)/*(pd->graze_point/3)*/)) );/*���������֎d�l�ύXc->type = SP_DELETE;*/
		}
		s->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);/*�����蔻��Ȃ�*/
	}
	else
	{
		s->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	}
	return (s);
	#undef OFFSET_X64
	#undef ITEM_WIDTH16
	#undef ITEM_X_LIMIT
}


/*---------------------------------------------------------
	�A�C�e����o�^���ďo��������
---------------------------------------------------------*/

static void s_item_convert_hosi(SPRITE *h)
{
	{
		h->callback_mover	= move_items;
	//	if ( SP_ITEM_06_HOSI == sel_type )	/* ���_�̂ݓ��ʏ��� */
		{
			/* �g�́A�����炵�� */
		//	if ( USER_BOMOUT_WAIT > pd_bomber_time )
			if ( USER_BOMOUT_WAIT > pd_bomber_time )
			{	/* �{����(�ݒ薳�G���Ԓ�)��100pts.(�҂��Ȃ�) */
				bonus_info_score_nodel(h, SCORE_100);/*���������֎d�l�ύXs->type = SP_DELETE;*/
			}
			else/* ���_ */
			{	/* �{����̎������G���Ԓ��͂�����(�҂���) */
				/* ((graze/3)*10)+(500) pts */
				bonus_info_any_score_nodel(h, (score(500)+(((pd_graze_point*86)>>8)/*(pd->graze_point/3)*/)) );/*���������֎d�l�ύXc->type = SP_DELETE;*/
			}
			h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);/*�����蔻��Ȃ�*/
		}
	}
	//	ITEM_DATA *data;
	//	data				= mmal loc(sizeof(ITEM_DATA));
	//	h->data 			= data;
		h->type = SP_ITEM_06_HOSI;
		h->ITEM_DATA_true_y256		= (/*src*/h->y256); /* ���z */

//		h->ITEM_DATA_angle512		= 6;	/* 6.51898646904403967309077986986488 ra d2deg512(0.08);*/
		h->ITEM_DATA_flags00		= ((ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY)&ITEM_MOVE_FLAG_01_COLLECT);
		h->ITEM_DATA_flag_first = 0;
//	}
	h->m_angleCCW512	= 0;			/* �`��p�p�x(����0�x�ō����(�����v���)) */	/* 0 == �X���Ȃ��B����0�x */
}


/*---------------------------------------------------------
	���Ă̓G�e���Ahosi�A�C�e���ɕς���
---------------------------------------------------------*/
extern SPRITE *sprite_list444_head;/* �e��p�X�v���C�g ���X�g */
//void item_from_bullets(int put_item_num)
extern void item_convert_hosi(SPRITE */*src*/h);
void bullets_to_hosi(void)
{
	SPRITE *s = sprite_list444_head;/* �e��p�X�v���C�g ���X�g�̐擪 ����T�� */
	while (NULL != s)/* �X�v���C�g ���X�g�̍Ō�܂Œ��ׂ� */
	{
		if (SP_GROUP_BULLETS & s->type)
		{
		//	item_create(s, /*put_item_num*/SP_IT EM_06_HOSI/*SP_IT EM_06_HOSI*/, 1, (IT EM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY) );
		//	s->type = SP_DELETE;
		//	s->type = SP_IT EM_06_HOSI;
			s_item_convert_hosi(s);//, /*put_item_num*/SP_IT EM_06_HOSI/*SP_IT EM_06_HOSI*/, 1, (IT EM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY) );
		}
		s = s->next;/*��*/
	}
}


/*---------------------------------------------------------
	�A�C�e����o�^���ďo��������
---------------------------------------------------------*/

/* [***090125	�ǉ�: up_flags �� ITEM_MOVE_FLAG_01_COLLECT �r�b�g���I���Ńv���C���[�ɏW�܂�܂��B */
void item_create(
	SPRITE *src/*int x, int y*/,
	int item_type,
	int num_of_creates,
	int up_flags
)
{
	int i;
	for (i=0; i<num_of_creates; i++)
	{
		SPRITE *h;
		h			= item_mono_create(src, item_type);
		if (NULL==h) return;
//
	//	ITEM_DATA *data;
	//	data				= mmal loc(sizeof(ITEM_DATA));
	//	h->data 			= data;
		h->ITEM_DATA_true_y256		= (src->y256);	/* ���z */

		#if 0
		/* ��{�I�ɉ�ʊO(��)�ɂ��܂��ݏo���Ȃ�(���^�C�v) */
		/* �A�C�e���}�[�J�[���쐬���Ȃ������O��̓��� */
		h->ITEM_DATA_y_sum256		= -(/*256*/((src->y256)>>7))-(ra_nd()&0xff);	/* �A�C�e�������o�������l(y���A������}�C�i�X) */
		#else
		/* ��ʊO(��)�ɋ�ݏo��(���A��ݏo���}�[�J�[����ĂȂ�) */
		/* �A�C�e���}�[�J�[���쐬���鎖���O��̓��� */
	//	h->ITEM_DATA_y_sum256		= -(/*256*/((200*256)>>7))-(ra_nd()&0xff);		/* �A�C�e�������o�������l(y���A������}�C�i�X) */
		h->ITEM_DATA_y_sum256		= -(/*256*/((512)))-(ra_nd()&0xff); 			/* �A�C�e�������o�������l(y���A������}�C�i�X) */
		#endif
		h->ITEM_DATA_angle512		= 6;	/* 6.51898646904403967309077986986488 ra d2deg512(0.08);*/
		h->ITEM_DATA_flags00		= (up_flags&ITEM_MOVE_FLAG_01_COLLECT);
		h->ITEM_DATA_flag_first = 0;
	}
}

/*---------------------------------------------------------
	�{�X�p�A�C�e���쐬
---------------------------------------------------------*/
//	#define ITEM_CREATE_MODE_01 	(12*0)
//	#define ITEM_CREATE_MODE_02 	(12*1)
void item_create_for_boss(SPRITE *src, int item_create_mode)
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
//	item_create(src, item_tbl[ITEM_01+difficulty+item_create_mode]/*SP_ITEM_01_P008 */, player_now_stage/*5*/, ITEM_MOVE_FLAG_06_RAND_XY);
	int i;
	for (i=0; i<(player_now_stage+2); i++)
	{
		item_create(src, SP_GROUP_ITEMS+u8_item_tbl[difficulty+item_create_mode+i]/*SP_ITEM_03_1UP	*/, 1, ITEM_MOVE_FLAG_06_RAND_XY);
	}
	item_create(src, SP_ITEM_05_TENSU/*SP_IT EM_06_HOSI*/, (16)/*(7)*/, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY) );/*���_���o��*/
}





/*-------------------------------------------------------*/
/*-------------------------------------------------------*/
/*-------------------------------------------------------*/

/*---------------------------------------------------------
	�����_���A�C�e���G�̏ꍇ�Ɏg���A���ʔ����e�[�u��
---------------------------------------------------------*/

static unsigned int random_item_seed/*=0*/;

void enemy_set_random_seed(void/*int set_seed*/)
{
	random_item_seed = (ra_nd()&(8-1))/*set_seed*/;
}
static int s_enemy_get_random_item(void)
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

/*static*/ void lose_random_item(SPRITE *src)
{
	item_create(src, s_enemy_get_random_item(), (1), (/*IT EM_MOVE_FLAG_01_COLLECT|*/ITEM_MOVE_FLAG_06_RAND_XY)/*(up_flags)*/ );
}



//	int put_item;	put_item=99;
//	case SP_ZAKO_03_YUKARI2:
//	if (rand_percent(30))
//	{
//		if (rand_percent(50))	{	put_item=(SP_ITEM_00_P001&0xff);}
//		else					{	put_item=(SP_ITEM_05_TENSU&0xff);}
//	}
//(put_item|SP_ITEM_00_P001)
//	if (99!=put_item)

//		static const u16/*int*/ item_table[8] =
//		{
//			SP_ITEM_07_SPECIAL,
//			SP_ITEM_04_BOMB,
//			SP_ITEM_04_BOMB/*�ᑬ�{���ɋz��==SP_ITEM_EXTRA_HOMING*/,
//			SP_ITEM_04_BOMB/*�ᑬ�{���ɋz��==SP_ITEM_EXTRA_HLASER*/,
//			//
//			SP_ITEM_01_P008/*�E�F�|���A�C�e��(��)==SP_ITEM_FIRE_POWER_G �E�F�|���A�C�e��(��)==SP_ITEM_EXTRA_SHIELD*/,
//			SP_ITEM_00_P001,
//			SP_ITEM_01_P008,/*�E�F�|���A�C�e��(��)==SP_ITEM_FIRE_POWER_G*/
//			SP_ITEM_05_TENSU,
//		};
//		static unsigned int drop_item=0;
//		drop_item++;
//		drop_item &= (8-1);//if (drop_item==8)	{	drop_item=0;}
/* [***20090223 �ǉ� �e�[�u���ɂ����� */
//					SP_ITEM_00_P001,		/*0x1000*/
//					SP_ITEM_04_BOMB,		/*0x1003*/
//					SP_ITEM_01_P008,		/*0x1007*/	//	SP_ITEM_00_P001/*0x1000*/,
//					SP_ITEM_00_P001,		/*0x1002*/	/*SP_ITEM_07_SPECIAL*/
//					SP_ITEM_00_P001,		/*0x1000*/
//					SP_ITEM_01_P008,		/*0x1004*/	//	SP_ITEM_01_P008/*0x1007*/,
//					//
//					SP_ITEM_04_BOMB,		/*0x1003*/
//					SP_ITEM_00_P001,		/*0x1002*/		/*SP_ITEM_07_SPECIAL*/
//					SP_ITEM_05_TENSU,		/*0x1001*/
//					SP_ITEM_03_1UP			/*0x1005*/




/*-------------------------------------------------------*/
/*-------------------------------------------------------*/
/*-------------------------------------------------------*/


/*---------------------------------------------------------
	sdl_screen[SDL_00_VIEW_SCREEN]�T�[�t�F�C�X�ɁA�X�R�A�������_�����O
---------------------------------------------------------*/

#if 0
//	   void font07_render_scorenum_xy( int time_out, int number, int x256, int y256)
//atic void font07_render_scorenum_xy(SCORE_CHACHE_DATA *aaa){}
void draw_SDL_score_chache(void)
{
//				SDL_Rect s,d;
//				s.w=(8); d.w=(8);
//				s.h=(8); d.h=(8);
	if (SDL_MUSTLOCK(sdl_screen[SDL_00_VIEW_SCREEN]))	{	SDL_LockSurface(sdl_screen[SDL_00_VIEW_SCREEN]); }/*���b�N����*/
	int i;
	for (i=0; i<MAX_SCORE_CHACHE; i++)
	{
		SCORE_CHACHE_DATA *aaa;
		aaa = &(score_number_chache[i]);
		if (SCORE_DELETE < aaa->time_out)
		{
			aaa->time_out -= 2;
			{
			//	font07_render_scorenum_xy(
				//	(aaa->time_out),
				//	(aaa->number),
				//	(aaa->x256)/*dx*/,
				//	(aaa->y256)/*dy*/
			//		&(score_number_chache[i])
			//		);
//				s.x = ((aaa->number)<<3)+(8);
//				s.y = 0;
//				d.x = (t256_floor(aaa->x256));
//				d.y = (t256_floor(aaa->y256))/*+((aaa->time_out)>>5) ��Ɉړ�*/;
//				#if 0
//				/*�x��*/
//				SDL_SetAlpha(font07_img,SDL_SRCALPHA,(aaa->time_out));
//				#endif
//				SDL_BlitSurface(font07_img,&s,sdl_screen[SDL_00_VIEW_SCREEN],&d);
				u16 alpha256;	alpha256 = (aaa->time_out);

				u16 *pd;
				{
					u32 y2562 = ((aaa->y256)&0x1ff00);
					pd = (u16 *)sdl_screen[SDL_00_VIEW_SCREEN]->pixels + (y2562+y2562) + (t256_floor(aaa->x256));/*x 512/256 */
				}
				int iii;	iii = (aaa->number);
				int yyy;
				for (yyy=0; yyy<6; yyy++)
				{
					#define v (0)
					#define M (1)
					#define f8(a,b,c,d, e,f,g,h) ((a<<0)|(b<<1)|(c<<2)|(d<<3)|(e<<4)|(f<<5)|(g<<6)|(h<<7))
					static const u8 score_font_08x05[/*(6*10)*/(64)] =
					{
f8(v,v,v,M,M,v,v,v),f8(v,v,v,v,v,v,v,v),f8(v,v,v,v,v,v,v,v),f8(v,v,v,v,v,v,v,v),f8(v,v,v,v,v,v,v,v),f8(v,M,M,M,M,M,v,v),f8(v,v,v,M,v,v,v,v),f8(v,M,v,v,v,v,v,v),f8(v,v,M,M,v,v,v,v),f8(v,v,M,v,v,v,v,v),
f8(v,v,M,v,v,M,v,v),f8(v,v,v,v,v,v,v,v),f8(v,v,v,v,v,v,v,v),f8(v,M,M,M,M,v,v,v),f8(M,M,M,M,M,M,M,v),f8(v,v,v,M,v,v,v,v),f8(v,M,M,M,M,M,M,v),f8(v,M,v,v,M,M,M,v),f8(v,v,v,M,M,v,v,v),f8(v,v,M,M,M,M,v,v),
f8(v,M,v,v,v,v,M,v),f8(v,v,v,v,v,v,v,v),f8(v,v,M,M,M,v,v,v),f8(v,v,v,v,v,v,v,v),f8(M,v,M,v,M,v,M,v),f8(v,v,M,M,M,M,v,v),f8(v,v,v,v,v,v,v,v),f8(M,M,M,M,M,v,v,v),f8(v,v,v,v,M,v,v,v),f8(M,M,M,v,v,M,v,v),
f8(v,M,v,v,v,v,M,v),f8(v,M,M,M,M,M,M,v),f8(v,v,v,v,v,v,v,v),f8(v,v,M,M,v,v,v,v),f8(M,M,M,v,M,M,M,v),f8(v,v,M,v,v,M,v,v),f8(v,v,M,v,v,M,v,v),f8(v,M,v,v,v,v,v,v),f8(v,M,v,v,M,M,v,v),f8(v,v,M,v,M,M,v,v),
f8(v,v,M,v,v,M,v,v),f8(v,v,v,v,v,v,v,v),f8(v,v,v,v,v,v,v,v),f8(v,v,v,v,v,v,v,v),f8(M,v,v,v,v,v,M,v),f8(v,M,M,v,M,M,v,v),f8(v,M,M,v,v,M,M,v),f8(v,M,v,v,v,v,v,v),f8(v,M,v,v,v,M,M,v),f8(v,M,M,v,M,v,v,v),
f8(v,v,v,M,M,v,v,v),f8(v,v,v,v,v,v,v,v),f8(M,M,M,M,M,M,M,v),f8(M,M,M,M,M,M,M,v),f8(M,M,M,M,M,M,M,v),f8(M,M,M,M,M,M,M,v),f8(M,M,v,v,v,v,M,M),f8(v,v,M,M,M,M,M,v),f8(M,v,v,v,v,v,M,M),f8(M,M,v,v,v,M,M,M),
						/*dummy*/0,0,0,0
					};
					#undef v
					#undef M
					#undef f8
					{
						const u8 ddd = score_font_08x05[iii];
						int kkk;
						kkk = 1;
						int xxx;
						for (xxx=0; xxx<8; xxx++)
						{
							if (ddd & (kkk) )
							{	/* �ėp�]��(�A���t�@�C��) */
							#define bgRGB (*pd)
							#define fgRGB (0xffff)
							(*pd) = (u16)MAKECOL16(
								(((GETR16F(fgRGB) * (alpha256)) + (GETR16F(bgRGB) * (256 - alpha256))) >> 8),
								(((GETG16F(fgRGB) * (alpha256)) + (GETG16F(bgRGB) * (256 - alpha256))) >> 8),
								(((GETB16F(fgRGB) * (alpha256)) + (GETB16F(bgRGB) * (256 - alpha256))) >> 8)	);
							}
							pd++;
							kkk += kkk;
						}
					}
					pd += (512-8);
					iii += 10;
				}
			}
		}
	}
	if (SDL_MUSTLOCK(sdl_screen[SDL_00_VIEW_SCREEN]))	{	SDL_UnlockSurface(sdl_screen[SDL_00_VIEW_SCREEN]);	}/*���b�N����*/
}
#endif


/*-------------------------------------------------------*/
/*-------------------------------------------------------*/
/*-------------------------------------------------------*/

/*---------------------------------------------------------
	��Փx�X�R�A�␳
---------------------------------------------------------*/

u32 adjust_score_by_difficulty(u32 convert_score)
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
	return (((convert_score)*(mul_tbl256[difficulty]))>>8);
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

void player_add_score(u32 score_num_pts)
{
	pd_my_score += score_num_pts;
	/* �J���X�g(�X�R�A�J�E���^�[ �X�g�b�v)�`�F�b�N����1�b(60flame)��1���
		�����I�ɂ͖��Ȃ��񂾂��ǁA�\�����ςɂȂ�Ǝv����B */
	#if (1==USE_MAX_SCORE_COUNTER_STOP_CHECK)
	/* �J���X�g(�X�R�A�J�E���^�[ �X�g�b�v)�`�F�b�N */
	if ( PLAYER_MAX_SCORE < pd_my_score )	/* �J���X�g �`�F�b�N */
	{
		pd_my_score = PLAYER_MAX_SCORE;
	}
	#endif /* (1==USE_MAX_SCORE_COUNTER_STOP_CHECK) */
}

#if (1==USE_EXTEND_CHECK)
static int extend_check_score;
static int extend_check_counter;
#endif /* (1==USE_EXTEND_CHECK) */
#if (1==USE_EXTEND_CHECK)
/* score_panel.c: �G�N�X�e���h�`�F�b�N�͖�1�b(60flame)��1��Ŗ��Ȃ��Ǝv���B */
void player_check_extend_score(void)
{
	/* 1000���A2500���A5000���A10000��(1��)�ŃG�N�X�e���h */
	#if (1==USE_EXTEND_CHECK)
	if ( extend_check_score < pd_my_score ) 	/* �G�N�X�e���h �`�F�b�N */
	{
		pd_zanki++; 	/* �G�N�X�e���h */
		/*
			�X�R�A�ɂ��G�N�X�e���h���͓���Ȃ̂ŁA�ڗ��ׂ��B
			(�A�C�e���ɂ��G�N�X�e���h���ƈ���āA�ڂŊm�F���Ȃ�)
			���ۂ���Ă݂���Q�炵�āA���x�ǂ��B
			���ɂ��邳���͂Ȃ������B
		*/
		voice_play(VOICE06_EXTEND, TRACK03_SHORT_MUSIC/*TRACK02_ALEART_IVENT*/);		/* �G�N�X�e���h�� */
		voice_play(VOICE06_EXTEND, TRACK01_EXPLODE);/*�\��(�K�v)*/						/* �G�N�X�e���h�� */
		{
			static const unsigned int extend_score_tbl[4/*8*/] =
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
void player_init_extend_score(void)
{
	extend_check_score		= score(   10000000);
	extend_check_counter	= 0;
}
#endif /* (1==USE_EXTEND_CHECK) */
