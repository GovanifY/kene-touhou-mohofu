
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�A�C�e���֘A
---------------------------------------------------------*/

#include "jiki_local.h"
/*---------------------------------------------------------
	�A�C�e���̈ړ�
---------------------------------------------------------*/

/* �X�̃A�C�e���̃X�e�[�^�X�f�[�^�[ */
//typedef struct
//{
/*hatudan_register_speed65536*/
//	/*hatudan_register_tra65536 ���p */#define ITEM_DATA_flag_first 	user_data01 	/* first�t���O */

//????#define ITEM_DATA_02_flags00		user_data02 	/* (user_data02==hatudan_register_spec_data)���W�t���O(hatudan_register_spec_data�ƍ��킹��K�v������) */
#define ITEM_DATA_02_flags00		user_data03 	/* (user_data02==hatudan_register_spec_data)���W�t���O(hatudan_register_spec_data�ƍ��킹��K�v������) */

#define ITEM_DATA_04_y_sum256		user_data04 				/* �A�C�e�������o���p y�� �ώZ�l(y���A������}�C�i�X) */
#define ITEM_DATA_04_count256		ITEM_DATA_04_y_sum256		/* �������E�p�J�E���^(���p) */
//efine ITEM_DA TA_time_in			user_data04 	/* �o������ */ /*	�������W */
//} ITEM_DATA;
#define ITEM_DATA_07_true_y256		user_data07 	/* �{����y���W */

/*---------------------------------------------------------
	�A�C�e���̈ړ�(�������W�̏ꍇ)
---------------------------------------------------------*/
	#if 0
	if (JYUMYOU_NASI < src->jyumyou)/* ����� */
	{
		/* ��ʓ��ɕϊ� */
		src->center.y256 = src->ITEM_DATA_07_true_y256;
		#if (0)/*(�������W�̏ꍇ��ʊO�ϊ����������Ȃ��B����Ƃ��������Ȃ�B)*/
		#endif
	//	if (t256(0) >= src->center.y256)		/* ��ɂ���Ǝ��Ȃ����Ǒ��� */
		if (t256(-16) >= src->center.y256)	/* ���̕ӂ̎d�l�͂��Ԃ�ς��B(Gu�X�v���C�g���W���S�Ǘ��Ƃ���) */
	//	if (t256(FIX_LABEL_ITEM_HEIGHT_DIV2) >= src->center.y256)	/* ���S���W�Ȃ̂� */
		{
			src->center.y256 = t256(FIX_LABEL_ITEM_HEIGHT_DIV2);/* ��ʊO�\���A�C�e�������̔��� */
			src->obj_type_set |= 0x08;
		}
		else
		{
			src->obj_type_set &= (~0x08);
		}
		reflect_sprite_spec(src, OBJ_BANK_SIZE_01_ITEM);
	}
	#endif
		#define FIX_LABEL_ITEM_HEIGHT_DIV2 (8/2)/* ��ʊO�\���A�C�e�������̔��� */
static OBJ_CALL_FUNC(gamen_gai_item_sub)	/* �������W�Ȃ�΁A�����ɏW�܂� */
{
	if (JYUMYOU_NASI < src->jyumyou)/* ����� */
	{
		/* ��ʓ��ɕϊ� */
		src->center.y256 = src->ITEM_DATA_07_true_y256;
//		if (t256(0) >= src->center.y256)		/* ��ɂ���Ǝ��Ȃ����Ǒ��� */
		if (t256(-16) >= src->center.y256)	/* ���̕ӂ̎d�l�͂��Ԃ�ς��B(Gu�X�v���C�g���W���S�Ǘ��Ƃ���) */
//		if (t256(FIX_LABEL_ITEM_HEIGHT_DIV2) >= src->center.y256)	/* ���S���W�Ȃ̂� */
		{	/* ��ʊO�̏ꍇ */
			src->obj_type_set |= (0x08);
			/* �����ɉ����ē����x���ς��B(�����������ƈÂ��Ȃ�) */
			int toumeido;
			toumeido = (src->center.y256);
		//	toumeido += t256(16);
		//	toumeido = ( (toumeido));
			/* 0 ... */
			toumeido >>= 7; 	/*(>>8���Ɩʔ����Ȃ�)*/
			#if 0/*(�f�o�b�O)*/
			cg.game_score = toumeido;/*[ �X�R�A���Ƀf�o�b�O�l�\�� ]*/
			#endif
			toumeido = 0xff + ((0x1000)>>7) + toumeido;
			toumeido = psp_max(0x10, toumeido);/*(0�ȉ��ɂȂ�̂ŕK���v��B)*/
		//	toumeido = psp_min(0xff, toumeido);/*(�����ėǂ��������A�O�̈�)*/
			src->color32		= (0x00ffffff) | ((toumeido)<<24);
		//
			src->center.y256 = t256(FIX_LABEL_ITEM_HEIGHT_DIV2);/* ��ʊO�\���A�C�e�������̔��� */
			src->atari_hantei	= (0);/*(���Ȃ�)*/
		}
		else
		{	/* ��ʓ��̏ꍇ */
			src->obj_type_set &= (~0x08);
			/* �O�ׁ̈A�������𔒂ɖ߂��B */
			src->color32		= (0xffffffff);
			#if (1)/*(�Ă���)*/
			src->atari_hantei	= (1);/*(����)*/
			#else
			src->atari_hantei	= (0);/*(���Ȃ�)*/
			#endif
		}
		reflect_sprite_spec(src, OBJ_BANK_SIZE_01_ITEM);
	}
}
/*---------------------------------------------------------
	�A�C�e���̈ړ�(�������W�̏ꍇ)
---------------------------------------------------------*/

static OBJ_CALL_FUNC(move_item_type02)	/* �������W�Ȃ�΁A�����ɏW�܂� */
{
	{
		src->ITEM_DATA_04_count256--;
		if (0 <= src->ITEM_DATA_04_count256)/*0���W�X�^�g��*/
		{
			;
		}
		else
	//	if (1 > src->ITEM_DATA_04_count256)
		{
			src->ITEM_DATA_04_count256 = (0);
		}
	//	if (0 == src->ITEM_DATA_flag_first)
	//	{
	//		src->ITEM_DATA_flag_first = (1);
	//		src->ITEM_DATA_04_count256 = t256(2.0/*1.0*/);	/* psp�͉𑜓x���Ⴂ�̂ō׍H(x2) */ 	/* (2.5==5*0.5) */
	//	}
		OBJ *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER];
		//#define ITEM_DATA_05_x_sa256		user_data05 	/* ���� x */ /* �������W */
		//#define ITEM_DATA_06_y_sa256		user_data06 	/* ���� y */ /* �������W */
		s32 src_ITEM_DATA_05_x_sa256 = (src->center.x256 - zzz_player->center.x256);
		s32 src_ITEM_DATA_06_y_sa256 = (src->ITEM_DATA_07_true_y256 - zzz_player->center.y256);
		/* ���@�_���p�� HATSUDAN_03_angle65536 �ɍ쐬 */
		REG_02_DEST_X	= ((src->center.x256));
		REG_03_DEST_Y	= ((src->ITEM_DATA_07_true_y256));
		calculate_jikinerai();
		/* �u1����65536�����v����u1����1024�����v�֕ϊ�����B */
	//	HATSUDAN_03_angle1024 = (deg65536to1024(HATSUDAN_03_angle65536));
		/* �����ɏW�܂� */
		int aaa_x256;
		int aaa_y256;
		aaa_x256 = ((src_ITEM_DATA_05_x_sa256 * src->ITEM_DATA_04_count256)>>9/*8*/);	/*fps_factor*/	/* psp�͉𑜓x���Ⴂ�̂ō׍H(/2) */
		aaa_y256 = ((src_ITEM_DATA_06_y_sa256 * src->ITEM_DATA_04_count256)>>9/*8*/);	/*fps_factor*/	/* psp�͉𑜓x���Ⴂ�̂ō׍H(/2) */
		if ((SP_ITEM_06_HOSI) == (src->obj_type_set))	/* ���_�̂ݓ��ʏ��� */
		{
			if (
				//	(/*10*/64/*16*/ > data->y_sum256) ||
				(
					#if 1
					/* [��`����] �v���C���𒆐S�Ƃ��āA�c��15x15�̗̈�(���E7[pixel], �㉺7[pixel]) */
					(t256(8) > abs(aaa_x256)) &&
					(t256(8) > abs(aaa_y256))
					#else
					/* [�H�`����] �v���C���𒆐S�Ƃ��āA���a(?)10[pixel]�̕H�`�̈�<>21x21 */
					(t256(11) > (abs(aaa_x256)+abs(aaa_y256)))
					#endif
				)
			)
			{
				src->jyumyou = JYUMYOU_NASI;/* ���_�̂ݓ��ʏ��� */	/* �����܂� */
				voice_play_menu(VOICE05_BONUS, TRACK06_ITEM);/* �e�L�g�[ */
			}
		}
//		src->center.x256					= zzz_player->center.x256 + (aaa_x256); /*fps_factor*/
//		src->ITEM_DATA_07_true_y256 = zzz_player->center.y256 + (aaa_y256); /*fps_factor*/
			#if (0)//
			src->math_vector.x256 = ((si n1024((deg65536to1024(HATSUDAN_03_angle65536)))*(src->speed256))>>8);/*fps_factor*/	/* CCW�̏ꍇ */
			src->math_vector.y256 = ((co s1024((deg65536to1024(HATSUDAN_03_angle65536)))*(src->speed256))>>8);/*fps_factor*/
			#else
			{
				int sin_value_t256; 	//	sin_value_t256 = 0;
				int cos_value_t256; 	//	cos_value_t256 = 0;
				int256_sincos1024( (deg65536to1024(HATSUDAN_03_angle65536)), &sin_value_t256, &cos_value_t256); /* �u1����65536�����v����u1����1024�����v�֕ϊ�����B */
				src->center.x256						+= ((sin_value_t256*(/*t256*/(5)))/*>>8*/);/*fps_factor*/
				src->ITEM_DATA_07_true_y256 	+= ((cos_value_t256*(/*t256*/(5)))/*>>8*/);/*fps_factor*/
			}
			#endif
	}
	gamen_gai_item_sub(src);
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
		/* �ύX5=>4=>3 t256(3.0) */
	//	if (src->ITEM_DATA_04_y_sum256 < t256(1.5) )	/* t256(2.2) �A�C�e�������A�ő呬�x�̒��� */
		if (src->ITEM_DATA_04_y_sum256 < t256(2.0) )	/* t256(2.2) �A�C�e�������A�ő呬�x�̒��� */
		/* t256(2.2) == �R���e�B�j���[��������[F]����ʂ̉���(�E���A����)�Ŏ��񂾏ꍇ�ŁA
			������Ԓx���A�H�X�q��(�������[�h�Ȃ��)�]�T�Ŏ��鑬�x�ɒ�������B
			(�ᑬ���[�h�ł͎��Ȃ��Ă��\��Ȃ�) */
		#endif

	/*
		�얲A�̑��x�ɂ��āB
		�g�ŗ얲A�̑��x�͈ӊO�Ƒ����̂ł��B
		--------------------
		�A�C�e������ʍŏ㕔�ŏo���܂��B
		(�d����������O�ɗd������ʊO�œ|���܂�)
		��������ʒ����őҋ@���đ҂��܂��B
		�A�C�e�����얲A�̐^���ɗ������Ă��܂��B
		���̎��A�C�e���̑��x�͍ő呬�x�ł��B
		�A�C�e���͈�葬�x�ȏ�������܂���B
		���̗����A�C�e���Ɨ얲A�ŁA�������Ă݂܂��B
		--------------------
		�g�𕁒ʂɗV��ł���ƃA�C�e���̍ő嗎�����x��
		�얲A�̈ړ����x�������ʂɊ�����̂ł����A
		�������Ă݂�ƁA�A�C�e���̍ő嗎�����x���
		�얲A�̈ړ����x�̕��������̂ł��B
		������x�I���l�łȂ��ƁA����ł��Ȃ���������܂��񂪁A
		�A�C�e�����ő嗎�����x�ɂȂ������_(��ʏ����㕔)���狣�����n�߂�ƁA
		��ʍŉ����̎��_�ŁB���1�L���������炢(16[pixel]���炢)�����ł��B
		--------------------
		�͕핗�ł͏c���ُ�ɋ����̂ŁA�ǂ��������������ׂ���
		�ۑ肪�������񂠂�܂��B
		--------------------
		�܂�A�g�ł́u��̃A�C�e���ɍi��ΕK������v
		�Ƃ�������������Ă���̂ɑ΂��A
		�͕핗�ł́A�c���ُ�ɋ����̂ŁA�����ڐ��������킹��ƁA
		�u��̃A�C�e���ɍi���Ă��K�����Ȃ��v
		�Ƃ��������ɂ����Ȃ�܂���B
		�u��̃A�C�e���ɍi��ΕK������v�Ƃ��������ɂ���ׂɂ́A
		�c���ُ�ɋ����̂ŁA�A�C�e���̌����ڐ������{�ȏ�x�����Ȃ��Ǝ��܂���B
	*/
static OBJ_CALL_FUNC(move_item_type01)
{
	/* �������W���[�h�̂ǂꂩ���쓮���Ă���A */
	if (0 != (
		(cg.state_flag) &
		(JIKI_FLAG_0x0010_JYOU_BU_SYUU_SYUU 			|		/* MAX���̏��� */
		 JIKI_FLAG_0x0020_BOMBER_SYUU_SYUU				|		/* �{���������̂݉�� */
		 JIKI_FLAG_0x0040_BOSS_GO_ITEM_JIDOU_SYUU_SYUU			/* �{�X���j��̎������W */
		) ))
	{
		src->ITEM_DATA_02_flags00 |= (ITEM_MOVE_FLAG_01_COLLECT);	/*1*/	/* ���̃A�C�e���͎������W�ɂ��� */
	}
	/* �������W�łȂ���΁A�P���ɕ�������`���ė����� */
	if (0 == ((src->ITEM_DATA_02_flags00)&ITEM_MOVE_FLAG_01_COLLECT))
	{
	//	#define ITEM_DATA_03_angle1024		user_data03
	//	obj->ITEM_DATA_03_angle1024 		= (6*2);	/* 2*6.51898646904403967309077986986488 cv1024r(0.08);*/
		#define src_ITEM_DATA_03_angle1024		(6*2)
		src->ITEM_DATA_04_y_sum256 += ((src_ITEM_DATA_03_angle1024)>>1);	/* x1.5 */
	#if 0
		static const s32 rakka_sokudo[(PLAYERS8)] =
		/* ���܂葬���ƃA�C�e������肫��Ȃ��̂ŁA�~�X��̕������x���x���Ȃ�B
		(�����čH�v���Ȃ��ƑS���͎��Ȃ����炢�ɒ�������B
		�p�^�[�����H�v����Ɩw�ǎ���悤�ɂȂ��Ă�B
		�������A����͎�ɃQ�[������(�`3��)�̘b�ŁA�㔼(4�ʁ`)�͊܂܂Ȃ��B)
		*/
	/* �������[�h(�ʏ펞) */					/* �������[�h(�{��������) */
//	#define SPEED_HIGH_REIMU_A		(0x02f1)	/* t256(2.94) 2.94140625		  t256(3.0), */
//	#define SPEED_HIGH_REIMU_B		(0x02f1)	/* t256(2.94) 2.94140625		  t256(3.0), */
//	#define SPEED_HIGH_MARISA_A 	(0x03a3)	/* t256(3.64) 3.63671875		  t256(2.0), */
//	#define SPEED_HIGH_MARISA_B 	(0x03a3)	/* t256(3.64) 3.63671875		  t256(2.0), */
//	#define SPEED_HIGH_REMILIA		(0x0347)	/* t256(3.28) 3.27734375		  t256(5.0), */
//	#define SPEED_HIGH_YUYUKO		(0x02c7)	/* t256(2.78) 2.77734375		  t256(5.0), */
//	#define SPEED_HIGH_CIRNO_A		(0x0373)	/* t256(3.45) 3.44921875		  t256(4.5), */ 	/* �H������ᑬ�̕������� */
//	#define SPEED_HIGH_CIRNO_Q		(0x0373)	/* t256(3.45) 3.44921875		  t256(4.5), */ 	/* �H������ᑬ�̕������� */
		// 0x02e6==t256(2.90) 2.8984375
		{//(r35)
			(0x02e6),		//(r35) t256(2.90), 	//(-r34)	t256(3.0),		/* REIMU_A */
			(0x02e6),		//(r35) t256(2.90), 	//(-r34)	t256(3.0),		/* REIMU_B */
			(0x02e6),		//(r35) t256(2.90), 	//(-r34)	t256(2.5),		/* MARISA_A */	/* �x������B �������͎d�l��A�x������B (r35- t256(2.75)) (-r34 t256(2.0) ) */
			(0x02e6),		//(r35) t256(2.90), 	//(-r34)	t256(2.75), 	/* MARISA_B */	/* ���C�������͑������悤�B (r35- t256(2.75)) (-r34 t256(2.0) ) */
			(0x02e6),		//(r35) t256(2.90), 	//(-r34)	t256(3.0),		/* REMILIA */
			t256(2.0),		//(r35) t256(2.0),		//(-r34)	t256(2.0),		/* YUYUKO */	/* �H�X�q�͑����x���̂ŁA�x������B */
			(0x02e6),		//(r35) t256(2.90), 	//(-r34)	t256(3.0),		/* CIRNO_A */
			t256(4.0),		//(r35) t256(4.0),		//(-r34)	t256(4.0),		/* CIRNO_Q */
		};
		int rakka_sokudo_maximum = rakka_sokudo[((cg_game_select_player))];
	#else
	//	int rakka_sokudo_maximum = (0x02e6);/*(psp�c�����̂ő����I�ɍl���āA�����C������)*/
	//	int rakka_sokudo_maximum = (0x02c0);/*(-r35u1 �H�X�q���x��)*/
		int rakka_sokudo_maximum = (0x0280);/*(r35u2-)*/
	#endif
		/* ���~���A & �H�X�q �̏ꍇ�A�ᑬ�ړ��ŗ������x���ꎞ�I�ɉ�������B */
		if (0x04==((cg_game_select_player) & 0x06) )/* 4:REMILIA or 5:YUYUKO */
		{
			if (/*is_slow =*/ (psp_pad.pad_data & PSP_KEY_SLOW))/* �ᑬ�ړ��̏ꍇ */
			{
				rakka_sokudo_maximum >>= 1;/* �ꎞ�I�ɔ��� */
			}
		}
		if (src->ITEM_DATA_04_y_sum256 > rakka_sokudo_maximum ) 	/* t256(2.2) �A�C�e�������A�ő呬�x�̒��� */
		{
			src->ITEM_DATA_04_y_sum256 = rakka_sokudo_maximum;
		}
		src->ITEM_DATA_07_true_y256 += (src->ITEM_DATA_04_y_sum256);/*fps_factor*/
		if (src->ITEM_DATA_07_true_y256 > GAME_HEIGHT*256)
		{
			/* �E�F�|���A�C�e�� (��P) (��P) (F) �̂����ꂩ��������A�`�F�C���j�� */
			if ((SP_ITEM_02_P128+1) > src->obj_type_set)
			{
			// SP_ITEM_00_P001: /* �E�F�|���A�C�e��(��P) */
			// SP_ITEM_01_P008: /* �E�F�|���A�C�e��(��P) */
			// SP_ITEM_02_P128: /* �E�F�|���A�C�e��(F) */
				cg.chain_point = (0);/* �`�F�C���j�� */
				//;
			}
			// else {;} 	/* [�_][��][B][1UP]�������Ă��A�`�F�C���ێ��B */
			src->jyumyou = JYUMYOU_NASI;				/* ��ʊO�ɏo���炨���܂� */
		}
	}
	else	/* �������W�Ȃ�΁A�����ɏW�܂� */
	{
		src->ITEM_DATA_04_count256 = t256(2.0/*1.0*/);	/* psp�͉𑜓x���Ⴂ�̂ō׍H(x2) */ 	/* (2.5==5*0.5) */
		src->callback_mover 		= move_item_type02;
	//	src->ITEM_DATA_flag_first	= (0);
		src->hatudan_register_speed65536	= (0);
	}
	gamen_gai_item_sub(src);

}

/*---------------------------------------------------------
	�A�C�e���̈ړ�(�ʏ펞���̂P)
	���񎞂̓����o�������B
	�����o�����͂����蔻��Ȃ��B(����ł��Ȃ�)
---------------------------------------------------------*/
static OBJ_CALL_FUNC(move_item_type00)
{
	src->ITEM_DATA_04_count256--;
	if (0 <= src->ITEM_DATA_04_count256)/*0���W�X�^�g��*/
	{
		;
			src->center.x256	+= src->math_vector.x256;
			src->center.y256	+= src->math_vector.y256;
	}
	else
//	if (1 > src->ITEM_DATA_04_count256)
	{
		src->atari_hantei				= (1/*�X�R�A���p*/);/*(�����蔻��on==����)*/
		src->ITEM_DATA_07_true_y256 	= (src->center.y256); /* ���z */
		src->ITEM_DATA_04_y_sum256		= (0);			/* �A�C�e�������o�������l(y���A������}�C�i�X) */
		src->callback_mover 			= move_item_type01;
	}
}

/*---------------------------------------------------------
	�A�C�e���o��������q�֐�
	-------------------------------------------------------
	-------------------------------------------------------
	ToDo:
	�e�V�X�e���Ɉڍs����ׁA�p�~�\��B
	�J�[�h�X�N���v�g�V�X�e�����ғ�����ׂɂ́A���e���ꃖ���ɓ������Ȃ���
	���x�I�Ȗʂł��̂܂܂̌`�Ԃł͖���������̂ŁA
	(�����̔��e�����̂����ő��̒e��������������)
	�����̔��e������ hatudan_system_regist_single �ɒu��������K�v������B
	-------------------------------------------------------
	�Ƃ͂�����������ȁ`�B
---------------------------------------------------------*/
/*int x, int y*/
static OBJ *aaa_item_mono_create(OBJ/**/ *src, int set_sel_type)/* */
{
	{
		// �`���m(�X��)�̏ꍇ�A�O���C�Y�Ńp���[�A�b�v����̂�[��p][��P][F]�͏o���Ȃ��B[�_](�X)�ɂȂ�B
		// ���~���A�̏ꍇ[P](��)�����o���Ȃ��B
		// �H�X�q�̏ꍇ[��p]����[�_]�A[��P]����[�{��]�A������ւ���Ă���B
		const u8 item_henkan[(PLAYERS8)*(8)] =
		{/* �얲(�아)		�얲(����)					������(����)				������(����)				���~���A(����)				�H�X�q(����)				�`���m(�X��)				�`���m(�H�n)			*/
(SP_ITEM_00_P001&0xff), 	(SP_ITEM_00_P001&0xff), 	(SP_ITEM_00_P001&0xff), 	(SP_ITEM_00_P001&0xff), 	(SP_ITEM_00_P001&0xff), 	(SP_ITEM_05_TENSU&0xff),	(SP_ITEM_05_TENSU&0xff),	(SP_ITEM_05_TENSU&0xff),
(SP_ITEM_01_P008&0xff), 	(SP_ITEM_01_P008&0xff), 	(SP_ITEM_01_P008&0xff), 	(SP_ITEM_01_P008&0xff), 	(SP_ITEM_01_P008&0xff), 	(SP_ITEM_01_P008&0xff), 	(SP_ITEM_05_TENSU&0xff),	(SP_ITEM_05_TENSU&0xff),
(SP_ITEM_02_P128&0xff), 	(SP_ITEM_02_P128&0xff), 	(SP_ITEM_02_P128&0xff), 	(SP_ITEM_02_P128&0xff), 	(SP_ITEM_01_P008&0xff), 	(SP_ITEM_04_BOMB&0xff), 	(SP_ITEM_05_TENSU&0xff),	(SP_ITEM_05_TENSU&0xff),
(SP_ITEM_03_1UP&0xff),		(SP_ITEM_03_1UP&0xff),		(SP_ITEM_03_1UP&0xff),		(SP_ITEM_03_1UP&0xff),		(SP_ITEM_03_1UP&0xff),		(SP_ITEM_03_1UP&0xff),		(SP_ITEM_03_1UP&0xff),		(SP_ITEM_03_1UP&0xff),
(SP_ITEM_04_BOMB&0xff), 	(SP_ITEM_04_BOMB&0xff), 	(SP_ITEM_04_BOMB&0xff), 	(SP_ITEM_04_BOMB&0xff), 	(SP_ITEM_01_P008&0xff), 	(SP_ITEM_02_P128&0xff), 	(SP_ITEM_04_BOMB&0xff), 	(SP_ITEM_04_BOMB&0xff),
(SP_ITEM_05_TENSU&0xff),	(SP_ITEM_05_TENSU&0xff),	(SP_ITEM_05_TENSU&0xff),	(SP_ITEM_05_TENSU&0xff),	(SP_ITEM_00_P001&0xff), 	(SP_ITEM_00_P001&0xff), 	(SP_ITEM_05_TENSU&0xff),	(SP_ITEM_05_TENSU&0xff),
(SP_ITEM_06_HOSI&0xff), 	(SP_ITEM_06_HOSI&0xff), 	(SP_ITEM_06_HOSI&0xff), 	(SP_ITEM_06_HOSI&0xff), 	(SP_ITEM_06_HOSI&0xff), 	(SP_ITEM_06_HOSI&0xff), 	(SP_ITEM_06_HOSI&0xff), 	(SP_ITEM_06_HOSI&0xff),
(SP_ITEM_07_SPECIAL&0xff),	(SP_ITEM_07_SPECIAL&0xff),	(SP_ITEM_07_SPECIAL&0xff),	(SP_ITEM_07_SPECIAL&0xff),	(SP_ITEM_07_SPECIAL&0xff),	(SP_ITEM_07_SPECIAL&0xff),	(SP_ITEM_07_SPECIAL&0xff),	(SP_ITEM_07_SPECIAL&0xff),
		};
		set_sel_type = (SP_ITEM_00_P001 | (item_henkan[((set_sel_type&0x07)<<3)+(cg_game_select_player)]));
	}
	//
	//	�A�C�e���̎�ނ�I��
	OBJ *obj;
	obj 							= obj_regist_tama();
	if (NULL != obj)
	{
		obj->obj_type_set			= set_sel_type; 	/* �A�C�e���̎�� == �O���t�B�b�N�̎�� */
		obj->m_zoom.y256			= M_ZOOM_Y256_NO_TILT;/* ����@�\�ŌX���Ȃ��悤�V�X�e���g��(r33)�B */
		reflect_sprite_spec(obj, OBJ_BANK_SIZE_01_ITEM);
		obj->m_Hit256R				= ZAKO_ATARI16_PNG;
	//
		if (SP_ITEM_06_HOSI == set_sel_type)	/* ���_�̂ݓ��ʏ��� */
		{
			/* �g�́A�����炵�� */
			if (USER_BOMOUT_WAIT > cg.bomber_time)
			{	/* �{����(�ݒ薳�G���Ԓ�)��100pts.(�҂��Ȃ�) */
				bonus_info_score_nodel(obj, SCORE_100);/*���������֎d�l�ύXs->jyumyou = JYUMYOU_NASI;*/
			}
			else/* ���_ */
			{	/* �{����̎������G���Ԓ��͂�����(�҂���) */
				/* ((graze/3)*10)+(500) pts */
				bonus_info_any_score_nodel(obj, (score(500)+(((cg.graze_point*86)>>8)/*(pd->graze_point/3)*/)) );/*���������֎d�l�ύXc->jyumyou = JYUMYOU_NASI;*/
			}
//			/*�����蔻��Ȃ�*/	/*(???????)*/
			obj->atari_hantei			= (ATARI_HANTEI_OFF/*�X�R�A���p*/);
		}
	//	else
	//	{
	//	}
	}
	#undef OFFSET_X64
	#undef ITEM_WIDTH16
	#undef ITEM_X_LIMIT
	return (obj);
}


/*---------------------------------------------------------
	�A�C�e����o�^���ďo��������
---------------------------------------------------------*/

static void s_item_convert_hosi(OBJ *obj)
{
	{
		#if (1)
		{
			int temp_cx256;
			int temp_cy256;
			temp_cx256 = obj->center.x256;
			temp_cy256 = obj->center.y256;
			/* �o�O��̂ŁA�Ƃ肠�����Ή��B */
			memset(obj, 0, sizeof(OBJ));
			obj->color32	= MAKE32RGBA(0xff, 0xff, 0xff, 0xff);		/*	obj->alpha		= 0xff;*/
			obj->jyumyou	= JYUMYOU_1MIN; 		/* 1�������珟��Ɏ��������B */
			obj->m_zoom.x256			= t256(1.00);	/* �\���g�嗦 256 == [ x 1.00 ] */
			obj->m_zoom.y256			= t256(1.00);	/* �\���g�嗦 256 == [ x 1.00 ] */
			#if (1)
			/*(���C���[[0]��(r36�ȍ~)�K��danmaku_00_standard_angle_mover�Ȃ̂Ń��C���[[0]�ɋ����ݒ�B)*/
			/*(�ǂ��킩��Ȃ�)*/
			obj->hatudan_register_spec_data = (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
			#endif
			obj->center.x256 = temp_cx256;
			obj->center.y256 = temp_cy256;
		}
		#endif
		obj->callback_mover 	= move_item_type01;
	//	if (SP_ITEM_06_HOSI == sel_type)	/* ���_�̂ݓ��ʏ��� */
		{
			/* �g�́A�����炵�� */
			if (USER_BOMOUT_WAIT > cg.bomber_time)
			{	/* �{����(�ݒ薳�G���Ԓ�)��100pts.(�҂��Ȃ�) */
				bonus_info_score_nodel(obj, SCORE_100);/*���������֎d�l�ύXs->jyumyou = JYUMYOU_NASI;*/
			}
			else/* ���_ */
			{	/* �{����̎������G���Ԓ��͂�����(�҂���) */
				/* ((graze/3)*10)+(500) pts */
				bonus_info_any_score_nodel(obj, (score(500)+(((cg.graze_point*86)>>8)/*(pd->graze_point/3)*/)) );/*���������֎d�l�ύXc->jyumyou = JYUMYOU_NASI;*/
			}
//			/*�����蔻��Ȃ�*/	/*(???????)*/
//			obj->atari_hantei			= (ATARI_HANTEI_OFF/*�X�R�A���p*/);
		}
	}
		/* �G�t�F�N�g�̏ꍇ�����_�ɂȂ�B */
		obj->obj_type_set = SP_ITEM_06_HOSI;
		obj->rotationCCW1024				= (0);			/* �`��p�p�x(����0�x�ō����(�����v���)) */	/* 0 == �X���Ȃ��B����0�x */
		{
			/* �G�t�F�N�g�̏ꍇ�������Ȃ̂ŁA���ɖ߂��B */
			obj->color32		= (0xffffffff);
			/* �G�t�F�N�g�̏ꍇ�������Ȃ̂ŁA�����ɖ߂��B */
		//	obj->m_zoom.x256	= t256(1.0);M_ZOOM_Y256_NO_TILT�͋���1.00�{�Ȃ̂ŗv��Ȃ��B
		//	obj->m_zoom.y256	= t256(1.0);M_ZOOM_Y256_NO_TILT�͋���1.00�{�Ȃ̂ŗv��Ȃ��B
			obj->m_zoom.y256	= M_ZOOM_Y256_NO_TILT;/* ����@�\�ŌX���Ȃ��悤�V�X�e���g��(r33)�B */
		}
		obj->ITEM_DATA_07_true_y256 	= (/*src*/obj->center.y256); /* ���z */
		obj->ITEM_DATA_02_flags00	= /*(DANMAKU_LAYER_03)|*/((ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY)&ITEM_MOVE_FLAG_01_COLLECT);
		//
		reflect_sprite_spec(obj, OBJ_BANK_SIZE_01_ITEM);
}


/*---------------------------------------------------------
	���Ă̓G�e���Ahosi�A�C�e���ɕς���
---------------------------------------------------------*/
extern void item_convert_hosi(OBJ */*src*/h);
global void bullets_to_hosi(void)
{
	unsigned int ii;
	for (ii=0; ii<OBJ_POOL_00_TAMA_1024_MAX; ii++)/* �S�����ׂ�B */
	{
		OBJ *obj;
		obj = &obj99[OBJ_HEAD_00_0x0000_TAMA+ii];
		if (JYUMYOU_NASI < (obj->jyumyou))/* ����� */
		{
			if (SP_ITEM_06_HOSI == obj->obj_type_set)
			{
				/*(r36-����[���_]�Ȃ�ϊ����Ȃ�)*/;
			}
			else
			if (OBJ_Z04_TAMA & obj->obj_type_set)
			{
			//	item_create(obj, /*put_item_num*/SP_IT EM_06_HOSI/*SP_IT EM_06_HOSI*/, 1, (IT EM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY) );
			//	obj->jyumyou = JYUMYOU_NASI;
			//	obj->type = SP_IT EM_06_HOSI;
				s_item_convert_hosi(obj);//, /*put_item_num*/SP_IT EM_06_HOSI/*SP_IT EM_06_HOSI*/, 1, (IT EM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY) );
			}
		}
	}
}


/*---------------------------------------------------------
	�A�C�e����o�^���ďo��������
---------------------------------------------------------*/
	#if 0
	if ((CIRNO_A-1) < (cg_game_select_player))/* 6:CIRNO_A or 7:CIRNO_Q */
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

/* up_flags �� ITEM_MOVE_FLAG_01_COLLECT �r�b�g���I���Ńv���C���[�ɏW�܂�܂��B */
static void item_create_flags(
	OBJ *src,
	int item_type,
	int set_item_flags
)
{
	{
		OBJ *h;
		h			= aaa_item_mono_create(src, item_type);
		#if 1
		if (NULL==h) return;
		#else
		if (NULL != h)
		#endif
		{
			h->ITEM_DATA_07_true_y256		= (src->center.y256); /* ���z */

			s32 x_offset;
			x_offset = 0;
			/*(WAIT���[�h�Ȃ�)*/
			if (set_item_flags & ITEM_MOVE_FLAG_02_WAIT)
			{
				h->ITEM_DATA_04_count256	= (64)/*(32)*/;
				h->atari_hantei 		= (ATARI_HANTEI_OFF/*�X�R�A���p*/);
				h->callback_mover		= move_item_type00;
				//------------------
			u32 my_rand;
				my_rand = ra_nd();
			s32 rnd_spd256				= (my_rand & 0x7f/*angCCW512*/) + t256(0.75);
				HATSUDAN_01_speed256	= ((rnd_spd256));/*(t256�`��)*/
				HATSUDAN_03_angle65536	= (my_rand & ((65536/4)-1)) + (65536/4) + (65536/8);
				sincos256();/*(�j�󃌃W�X�^�����̂Œ���)*/
				h->math_vector.x256 = REG_03_DEST_Y;//sin_value_t256	// �c
				h->math_vector.y256 = REG_02_DEST_X;//cos_value_t256	// ��
			}
			else/*(�ʏ탂�[�h)*/
			{
				h->callback_mover		= move_item_type01;
				/*(���v���C���ɍČ������K�v)*/
				#if 0
				/* ��{�I�ɉ�ʊO(��)�ɂ��܂��ݏo���Ȃ�(���^�C�v) */
				/* �A�C�e���}�[�J�[���쐬���Ȃ������O��̓��� */
				h->ITEM_DATA_04_y_sum256		= -(/*256*/((src->center.y256)>>7))-(ra_nd()&0xff); /* �A�C�e�������o�������l(y���A������}�C�i�X) */
				#else
				/* ��ʊO(��)�ɋ�ݏo��(��ݏo���}�[�J�[�K�{) */
				/* �A�C�e���}�[�J�[���쐬���鎖���O��̓��� */
			//	h->ITEM_DATA_04_y_sum256		= -(/*256*/((200*256)>>7))-(ra_nd()&0xff);		/* �A�C�e�������o�������l(y���A������}�C�i�X) */
				h->ITEM_DATA_04_y_sum256		= -(/*256*/((512)))-(ra_nd()&0xff); 			/* �A�C�e�������o�������l(y���A������}�C�i�X) */
				#endif
				{
				//	�o���ʒu�����߂�
					#define OFFSET_X64		(64*256)/* 64�͂��炵�� 2��n��̕K�v�L�� */
					#define ITEM_WIDTH16	(16*256)/* 16�̓A�C�e���� */
					#define ITEM_X_LIMIT	(GAME_WIDTH*256+OFFSET_X64-ITEM_WIDTH16)
					s32 x256;
					x256 = (src->center.x256);
					x256 += ((ra_nd()&((OFFSET_X64+OFFSET_X64)-1)));	/*(���v���C���ɍČ������K�v)*/
							if (x256 < OFFSET_X64)			{		x256 = OFFSET_X64;			}
					else	if (x256 > (ITEM_X_LIMIT))		{		x256 = (ITEM_X_LIMIT);		}
				//	�o�^����
					x_offset	= (x256)-(OFFSET_X64) - (src->center.x256);
				}
			}
			h->center.x256					= src->center.x256 + (x_offset);
			h->center.y256					= src->center.y256;
			h->ITEM_DATA_02_flags00 			= /*(DANMAKU_LAYER_03)|*/(set_item_flags & ITEM_MOVE_FLAG_01_COLLECT);
		}
	}
}
global void item_create_flags_num(
	OBJ *src,
	int item_type,
	int num_of_creates,
	int set_item_flags
)
{
	/*unsigned*/ int ii;
	for (ii=0; ii<num_of_creates; ii++)
	{
//		item_create_flags(src, item_type, (ITEM_MOVE_FLAG_06_RAND_XY) );
		item_create_flags(src, item_type, (set_item_flags) );
	}
}




global void item_create_mono(
	OBJ *src,
	int item_type
)
{
	item_create_flags(src, item_type, (ITEM_MOVE_FLAG_06_RAND_XY) );
}

/*---------------------------------------------------------
	�{�X�p�A�C�e���쐬
---------------------------------------------------------*/
//	#define ITEM_CREATE_MODE_01 	(12*0)
//	#define ITEM_CREATE_MODE_02 	(12*1)
global void item_create_for_boss(OBJ/**/ *src, int item_create_mode)
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
//	item_create_mono(src, item_tbl[ITEM_03+difficulty+item_create_mode]/*SP_ITEM_03_1UP */ );
//	item_create_mono(src, item_tbl[ITEM_02+difficulty+item_create_mode]/*SP_ITEM_02_P128 */ );
//	item_create(src, item_tbl[ITEM_01+difficulty+item_create_mode]/*SP_ITEM_01_P008 */, cg.game_now_stage/*5*/, ITEM_MOVE_FLAG_06_RAND_XY);
	unsigned int i;
	for (i=0; i<(1)+((((unsigned)cg.game_now_stage>>1))&0x0f); i++)
	{
		item_create_mono(src, OBJ_Z03_ITEM + u8_item_tbl[item_create_mode+(((cg.game_difficulty)+i)&((ITEM_MAX)-1))]/*SP_ITEM_03_1UP */ );
	}
	for (i=0; i<(16); i++)/*(7)*/
	{
		item_create_flags(src, SP_ITEM_05_TENSU, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY) );/*���_���o��*/
	}
}





/*-------------------------------------------------------*/
/*-------------------------------------------------------*/
/*-------------------------------------------------------*/

/*---------------------------------------------------------
	�����_���A�C�e���G�̏ꍇ�Ɏg���A���ʔ����e�[�u��
---------------------------------------------------------*/

static unsigned int random_item_seed;		/*=0*/

global void teki_random_item_table_initialize(void) 	/*int set_seed*/
{	/*(���v���C���ɍČ������K�v)*/
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

global /*static*/ OBJ_CALL_FUNC(item_create_random_table)
{
	item_create_mono(src, s_teki_get_random_item());//, (1), (/*IT EM_MOVE_FLAG_01_COLLECT|*/ITEM_MOVE_FLAG_06_RAND_XY)/*(up_flags)*/
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
	return (((convert_score)*(mul_tbl256[(cg.game_difficulty)]))>>8);
}

/*
���e��J�E���^�[�X�g�b�v
�@���_�̃J���X�g�@�@�X�X���X�X�X�X���X�X�X�O�_
�@������񐔁@�@�@�@�X�X�X�X�X��
�@�v���C���[���@�@�@�X�l
�@�J�[�h�񐔁@�X�X�X�X��
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
	/* �J���X�g(�X�R�A�J�E���^�[ �X�g�b�v)�`�F�b�N����1�b(60[frame])��1���
		�����I�ɂ͖��Ȃ��񂾂��ǁA�\�����ςɂȂ�Ǝv����B */
	#if (1==USE_MAX_SCORE_COUNTER_STOP_CHECK)
	/* �J���X�g(�X�R�A�J�E���^�[ �X�g�b�v)�`�F�b�N */
	if (PLAYER_MAX_SCORE < cg.game_score) /* �J���X�g �`�F�b�N */
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
/* score_panel.c: �G�N�X�e���h�`�F�b�N�͖�1�b(60[frame])��1��Ŗ��Ȃ��Ǝv���B */
global void player_check_extend_score(void)
{
	/* 1000���A2500���A5000���A10000��(1��)�ŃG�N�X�e���h */
	#if (1==USE_EXTEND_CHECK)
	if (extend_check_score < cg.game_score) /* �G�N�X�e���h �`�F�b�N */
	{
		cg.zanki++; 	/* �G�N�X�e���h */
		/*
			�X�R�A�ɂ��G�N�X�e���h���͓���Ȃ̂ŁA�ڗ��ׂ��B
			(�A�C�e���ɂ��G�N�X�e���h���ƈ���āA�ڂŊm�F���Ȃ�)
			���ۂ���Ă݂���Q�炵�āA���x�ǂ��B
			���ɂ��邳���͂Ȃ������B
		*/
		AUDIO_18_voice_number	= VOICE06_EXTEND;/* �G�N�X�e���h�� */
		AUDIO_19_voice_truck	= TRACK03_IVENT_DAN/*TRACK02_JIKI_BOMBER*/; 	cpu_voice_play();
		AUDIO_19_voice_truck	= TRACK01_PICHUN;/*�\��(�K�v)*/ 				cpu_voice_play();
		{
			/*
			�� �͕핗�� 1000�� �G�N�X�e���h�������̂́A
			1000�� �G�N�X�e���h������� 1�ʂŃG�N�X�e���h���Ă��܂��B
			1�ʂŃG�N�X�e���h����ƁA�p���Ă��C���킮�B
			-------------------------------------------------
			�� 2500�� �G�N�X�e���h���A����̂́A
			���ʂ̃m�[�~�X�Ȃ� 2�ʂŊm����(2500���_�ȏ�ɂȂ�̂�)�G�N�X�e���h����B
			1�ʂŃR���e�B�j���[���Ă���ꍇ�́A���� 2�ʂ�2500���_�ɂȂ�Ȃ��̂ŁA�G�N�X�e���h���Ȃ��B
			�m�[�~�X�ŃG�N�X�e���h����΁A�������C���o��̂ł́H�Ƃ����z���B
			-------------------------------------------------
			�� ���̕ӂ�dat�̃X�R�A�z���̃o�����X�ŕς�邪�A���Ȃ�O���炻�������X�R�A�z���o�����X�B
			*/
			static const u32 extend_score_tbl[8] =
			{
				0x7fffffff, 			/* �G�N�X�e���h �X�g�b�p�[(����ȏ�G�N�X�e���h���Ȃ�) */
			//	score(	 25000000),/*( 2500��[pts])*/	/* �ݒ�l�̈Ӗ�: ���ʂ̃m�[�~�X�Ȃ� 2�ʂŊm����extend. (1�ʂł͖������Ǝv��) */
			//	score(	 50000000),/*( 5000��[pts])*/	/* �ݒ�l�̈Ӗ�: �҂��� 2�ʂ�extend.(��Փx�ɂ����)  */
			//	score(	100000000),/*(1��[pts])*/		/* �͕핗�ł� 8000�� ������̕����ǂ��C������B */
			// �o�����X�����Ă݂�B
				score(	 25000000),/*( 2500��[pts])*/	/* �ݒ�l�̈Ӗ�: (1�ʂł͖������Ǝv��)	*/
				score(	 40000000),/*( 4000��[pts])*/	/* �ݒ�l�̈Ӗ�: (4000�� = 1500�� + 2500��)  */
				score(	 60000000),/*( 6000��[pts])*/	/* �ݒ�l�̈Ӗ�: (6000�� = 2000�� + 4000��)  */
				//
				score(	 90000000),/*( 9000��[pts])*/	/* �ݒ�l�̈Ӗ�: (9000�� = 3000�� + 6000��)  */
				score(	130000000),/*(1.3��[pts])*/ 	/* �ݒ�l�̈Ӗ�: (1.3��  = 4000�� + 9000��)  */
				score(	180000000),/*(1.8��[pts])*/ 	/* �ݒ�l�̈Ӗ�: (1.8��  = 5000�� + 1.3��)	*/
				score(	240000000),/*(2.4��[pts])*/ 	/* �ݒ�l�̈Ӗ�: (2.4��  = 6000�� + 1.8��)	*/
			};
			//	score(	 10000000),
			//	score( 9999999990), 	/*==0x3b9ac9ff*/
			//	score(21474836470), 	/*==0x7fffffff*/
			//	score(42949672950), 	/*==0xffffffff*/
			extend_check_counter++;
			extend_check_counter &= (8-1);
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
