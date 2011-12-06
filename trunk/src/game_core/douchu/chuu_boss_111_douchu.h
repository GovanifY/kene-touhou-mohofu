
//#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�����̒��{�X
	ToDo:
		(r39u1)�w��̖����w�́A���݂̌`�Ԃ͔p�~���A
		�g�����V�X�e���Ɉړ�����K�v������B
		-------------------------------------------------------
		���{�X->�g����
		�{�X->�g����
	-------------------------------------------------------
		"���[�~�A", 				"GFAIRY",
		"�d��2"(��),				"GFAIRY",
		"�d��1"(��),(�b��I)		"GFAIRY",
		"�d��3"(��),(�b��I)		"GFAIRY",
	-------------------------------------------------------
	; (72==64[������]+8[���[�~�A�e])
	; (79==64[������]+15[�V��e])		(�������̗d���݊�)�B
	-------------------------------------------------------
	�g�p���W�X�^
//	REG_08_REG0 	�����B
---------------------------------------------------------*/

//#define YOKAI1_DATA_repeat			user_data05 	/* �J��Ԃ��� */
#define YOKAI1_DATA_start_limit_y256	user_data06 	/* �o��y���W */
//#define yokai_type					user_data07 	/* �d���̎��(0 ... 3) */
	/* �J�n����J�[�h�ԍ� */

//
#define YOKAI1_DATA_s2					target_obj		/* OBJ *s2;*/

//enum /* _yokai_type_ */
//{
//	YOKAI_TYPE_00_RUMIA = 0,	/* 0:00 ���[�~�A */
//	YOKAI_TYPE_01_YOUKAI02, 	/* 1:32 "�d��2"(��) */
//	YOKAI_TYPE_02_YOUKAI01, 	/* 2:64 "�d��1"(��) */
//	YOKAI_TYPE_03_YOUKAI03		/* 3:96 "�d��3"(��) */
//};

#define OSIMAI_JIKAN_256	(256)


/*---------------------------------------------------------
	OBJ *src;	��-�{�X�G����
	OBJ *tama;	���e
---------------------------------------------------------*/

static void callback_hit_chuu_boss(OBJ/**/ *src, OBJ/**/ *tama)
{
	/* �{�X & ��-�{�X�Ɏ��e�����������ꍇ�̉ΉԃG�t�F�N�g��o�^(����Gu�������Ȃ��̂ŕ`�悵�ĂȂ�) */
	set_REG_DEST_XY(tama);/* ���e�ʒu ���Wxy */
	bakuhatsu_add_type_ccc(BAKUHATSU_MOVE12);/* ��Ɏ��s������������ */ /*BAKUHATSU_MINI00*/
//
	/* ��ƕ������������R�[�h������������B */
	{
		src->base_hp -= tama->kougeki_ti;	/* �U�����đ̗͌��炷(����������) */
		if (0 >= src->base_hp)			/* �O�����l�Ȃ�A�|�����B */
		{
			#if 1/*�v��́H*/
			src->base_hp	= (9999);/* �Q��͓|���Ȃ� */
			#endif
			card.spell_used_number		= (SPELL_00);/*(�J�[�h���s��~�ɂ���)*/
			bullets_to_hosi();/* �e�S���A���A�C�e���ɂ��� */
		//
			/* (�U�R��)�J�X�^�����ꏈ�� */
/*[�X�y�J�R�[���o�b�N���p�Ȃ̂ŕs�\]*/	if (NULL != src->callback_loser)	/* �J�X�^�����ꏈ��������Ύ��s�B */
/*[�X�y�J�R�[���o�b�N���p�Ȃ̂ŕs�\]*/	{
/*[�X�y�J�R�[���o�b�N���p�Ȃ̂ŕs�\]*/		src->callback_loser(src);	/* �J�X�^�����ꏈ�������s����B(�W���́u�����_���e�[�u������A�C�e�������v) */
/*[�X�y�J�R�[���o�b�N���p�Ȃ̂ŕs�\]*/		src->callback_loser = NULL; 	/* ���s�����̂�(�O�̈�)�����B */
/*[�X�y�J�R�[���o�b�N���p�Ȃ̂ŕs�\]*/		/* (���ꏈ����1�x�������s���Ȃ�) */
/*[�X�y�J�R�[���o�b�N���p�Ȃ̂ŕs�\]*/	}
			player_dummy_add_score(src->base_score);
			//
			bakuhatsu_add_circle(src, 0);
			/* �|�����ꍇ�w��̖����w�������Ă��邪�A���������ꍇ�w��̖����w������B */
			src->YOKAI1_DATA_s2->jyumyou		= JYUMYOU_NASI; /* �|�����ꍇ�A�w��̖����w�������B */
			src->jyumyou						= (OSIMAI_JIKAN_256);/* [���Ԑ؂�̈ʒu]�܂Ŏ��Ԃ�i�߂�B */
			/* �R�[���o�b�N�o�^ */
			src->callback_hit_teki = NULL;	/* �_�~�[�R�[���o�b�N�o�^ */
		}
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

#define LIMIT_TIME_512	(1536)
static OBJ_CALL_FUNC(move_chuu_boss)
{
	if ((OSIMAI_JIKAN_256) > src->jyumyou)	/* �u��֑ޏ�v(256[count]==��4�b) */
	{
		#if 1/* ���݂̎��������́A����ܗǂ��Ȃ��C������B */
		/* �|�����ꍇ�w��̖����w�������Ă��邪�A���������ꍇ�w��̖����w������B */
		if (JYUMYOU_NASI <= src->YOKAI1_DATA_s2->jyumyou)/* �w��̖����w������Έړ��B */
		{
			src->YOKAI1_DATA_s2->center.y256	-= t256(1.5);	/*fps_factor*/
		}
		#endif
		src->center.y256						-= t256(1.5);	/*fps_factor*/
		if (0 > (src->center.y256+t256(55.0)))/* 55�h�b�g�Ƃ���(�����_�A�c�̃T�C�Y) */
	//	if (0 > src->center.y256)/* �ȗ��� */
		{
			src->jyumyou						= JYUMYOU_NASI;
			if (JYUMYOU_NASI <= src->YOKAI1_DATA_s2->jyumyou)/* �w��̖����w������Ώ����B */
			{
				src->YOKAI1_DATA_s2->jyumyou	= JYUMYOU_NASI;
			}
			/* �ޏꏈ�� */
			cg.chuu_boss_mode = (0);
			//hold_game_mode_off();
		}
	}
	else
	if ((OSIMAI_JIKAN_256) == src->jyumyou) 	/* �u�J�[�h���s��~�ɂ���v */
	{
			card.spell_used_number			= (SPELL_00);/*(�J�[�h���s��~�ɂ���)*/
	}
	/*([���Ԑ؂�̈ʒu]���Ԑ؂�̏ꍇ�A�u�J�[�h���s��~�ɂ���v)*/
	/*(�ő��8�b(512[count])�ŃX�y�J�������f)*/
	else
	if ((256+LIMIT_TIME_512) > src->jyumyou)	/* �u�J�[�h���I���܂őҋ@�v(512[count]==��8�b) */
	{
		/*(�J�[�h�ݒ莞�Ԃ�512�����̏ꍇ�͎��R�ɏI������B���̏ꍇ�A[���Ԑ؂�̈ʒu]�܂Ŏ��Ԃ�i�߂�B)*/
		if (SPELL_00 == card.spell_used_number)
		{
			src->jyumyou = (OSIMAI_JIKAN_256);
		}
	//	if (SPELL_00 != card.card_number)
	//	{
	//		src->jyumyou = (OSIMAI_JIKAN_256+1);/* �u�J�[�h���s��~�ɂ���v�֐ݒ�B */
	//	}
	}
	else
//	if ((512) > src->jyumyou)	/* �u�ォ��o��v(128[count]==��2�b) */
	{
		src->YOKAI1_DATA_s2->center.y256	+= t256(2.0);	/*fps_factor*/
		src->center.y256					+= t256(2.0);	/*fps_factor*/
		if ((src->YOKAI1_DATA_start_limit_y256) < src->center.y256)
		{
			src->jyumyou			= ((256+LIMIT_TIME_512)-1);/* �u�J�[�h���I���܂őҋ@�v�֐ݒ�B */
			card.spell_used_number	= card.address_temporaly;	/* �J�[�h���Z�b�g */		/* "�d��2"&"���[�~�A"��p */
			card_maikai_init(src);/*(r35-, �J�[�h�̏������B�J�[�h���ς��Ɩ���s���K�v������)*/
		}
	}

	if (JYUMYOU_NASI < src->jyumyou)/* ����� */
	/* �A�j���[�V���� */
	{
		zako_anime_type05(src);
	}
	if (JYUMYOU_NASI < src->YOKAI1_DATA_s2->jyumyou)/* ����� */
	{
		src->YOKAI1_DATA_s2->rotationCCW1024 += (3);
		mask1024(src->YOKAI1_DATA_s2->rotationCCW1024);
	}
//
	if ((OSIMAI_JIKAN_256) > src->jyumyou)
	{
		;	/* ��֑ޏ� */
	}
	else
	{
		card_generate(src); 	/* �X�y����CPU���s���A�J�[�h���P�t���[����������B */
	}
}


/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/
//extern void regist_settei_common(GAME_COMMAND *l, OBJ *src);
global void game_command_01_regist_chuu_boss(GAME_COMMAND *l)
{
	if (0==cg.chuu_boss_mode)
	{
		cg.chuu_boss_mode = (1);
		//hold_game_mode_on();
		/* �v���C�I���e�B�[(�\���D�揇��)�����邩��A�w��ɕ\��������ׂɁA���߂Ɍ��̖����w���m�ہB */
		OBJ *s2;		/* ���̖����w */
		s2								= obj_regist_teki();
		if (NULL != s2)/* �o�^�ł����ꍇ�̂� */
		{
			/* ���̖����w���m�ۏo������{�̂��m�ہB */
			OBJ *s1;	/* �{�� */
			s1									= obj_regist_teki();
			if (NULL != s1)/* �o�^�ł����ꍇ�̂� */
			{
				/* 0ttd dddd
					'0': force only zero.
					't': yokai type. (0-3)
					'd': dan maku type. (0-31)
				*/
				unsigned int my_haikei_iro;
			//	my_haikei_iro = (((l->user_255_code)>>5)&(4-1));
				my_haikei_iro = (((l->user_255_code)   )&(4-1));
			//
				/* �����w���� */
				s2->m_Hit256R				= ZAKO_ATARI02_PNG;
				s2->obj_type_set			= ZAKO_28_MAHOJIN;
				s2->atari_hantei			= (1/*�X�R�A���p*/);
				{
					const u32 color_table[4] =
					{	/*	s2->alpha			= 0x00;*/
						MAKE32RGBA(0xff, 0xff, 0xff, 0x7f), 	/* �D���ۂ� */
						MAKE32RGBA(0xff, 0x3f, 0x3f, 0x7f), 	/* �Ԃ��ۂ� */
						MAKE32RGBA(0x3f, 0x00, 0xff, 0x7f), 	/* ���ۂ� */
						MAKE32RGBA(0x5f, 0xff, 0xaf, 0x7f), 	/* �����ۂ�(���F�ɂȂ���) */
					};
					s2->color32 				= color_table[my_haikei_iro];
				}
			//
				s1->callback_mover			= move_chuu_boss;
			//
				regist_settei_common(l, s1);/* ���{�X�Ƌ��p����K�v������B */
				//	s1->type		 /* �ݒ�t�@�C�����猈�߂� */
				/* easy�ł����݊�����ۂÂ���ׂ� 200 �͕K�v */ 	// 50+150*di fficulty;
			//	s1->atari_hantei				= (1/*�X�R�A���p*/);
			//	s1->m_Hit256R					= ZAKO_ATARI16_PNG;
			//	s1->base_hp 					= ((l->user_hp));		/* �ݒ�t�@�C������̗͂����߂�B */
			//	s1->base_score					= ((l->user_score));	/* �ݒ�t�@�C������l���X�R�A�����߂�B */
			//	s1->center.x256 				= ((l->user_locate_x)<<(8));
			//	s1->center.y256 				= ((l->user_locate_y)<<(8));
				s1->center.y256 				= t256(-30.0);		/* (���{�X�p�A����C��) */
			//	s1->center.x256 				= (s1->center.x256);
			//	s1->center.y256 				= (s1->center.y256);
				s2->center.x256 				= (s1->center.x256);
				s2->center.y256 				= (s1->center.y256);
				// ���{�X�̏ꍇ�Ay���W�͓o��\����W�B
				s1->YOKAI1_DATA_start_limit_y256	= ((l->user_locate_y)<<(8));/* t256(50.0) */
				//
				s1->callback_hit_teki		= callback_hit_chuu_boss;	/* �R�[���o�b�N�o�^ */
			//
//				s1->YOKAI1_DATA_repeat		= (2+2+1);/*�p�~*/
				s1->YOKAI1_DATA_s2			= s2;
				#if 1
				//------------ �J�[�h�֘A
				card.spell_used_number		= (SPELL_00);/*(�J�[�h���s��~�ɂ���)*/
				card.address_temporaly		= ((l->user_kougeki_type)&0x3f);
				s1->jyumyou = ((256+LIMIT_TIME_512+256)-1);/* �u�ォ��o��v�֐ݒ�B */
				#endif
			}
			else
			{
				/* �{��(s1)���o�^�ł��Ȃ������ꍇ�A�w��̖����w(s2)�̓o�^��������߂�B */
				s2->jyumyou 				= JYUMYOU_NASI;/* �o�^�ł��Ȃ������ꍇ */
			}
		}
	}
}
