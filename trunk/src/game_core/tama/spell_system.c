
#include "./../boss/boss.h"//#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�X�y�������V�X�e��(�J�[�h�𐶐�)
	-------------------------------------------------------
	����u�{�X�v�ȊO�ɂ��u���U�R�v�A�u���U�R�v���X�y�������ł���̂ŁA
	�u�X�y�������v�Ɓu�J�[�h�Ǘ��v�͕ʃV�X�e���ł��B
	(����u��-�{�X�v�͍��\�肪���邪����ĂȂ��B�J�[�h�����V�X�e�����܂�����ĂȂ�)
	-------------------------------------------------------
	����		�J�[�h����		�J�[�h	�X�y������
	�{�X		��				��				��	// �{�X
	��-�{�X 	�~(?)			��				��	// <�\��>			(�G���̗�����u�~�߂�v)
	���U�R		�~				�~				��	// ��[�݂� 		�J�[�h�����ĂȂ��̂ŃJ�[�h��(����)�͖����B
	���U�R		�~				�~				��	// �������U�R		�J�[�h�����ĂȂ��̂ŃJ�[�h��(����)�͖����B
	�U�R		�~				�~				�~	// ���̑��̃U�R 	�X�y�������o���Ȃ��B
	-------------------------------------------------------
	����			�G������	�J�[�h
	�{�X��p�R�A	�~			��					// �G�������֘A�̏��������Ȃ��̂ł��̕������B
	������p�R�A	��			�~					// �J�[�h�Ǘ��֘A�̏��������Ȃ��̂ł��̕������B
	-------------------------------------------------------
	(r33)��L�̐�p�R�A�v��͔p�~�B
	���̕���(r33)���Ƃނ����p�R�A�ɂ����������x�ቺ����B
---------------------------------------------------------*/

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���C���[�V�X�e��
	-------------------------------------------------------
	���C���[�V�X�e���́u�e���v�̕����ʂ�A
	�e��P�̂Ƃ��Ă͔F�������A���Ƃ��ĔF������V�X�e���B
	-------------------------------------------------------
---------------------------------------------------------*/

#include "./../boss/card_address.h"//#include "spell_address.h"

/*---------------------------------------------------------
	(r35)�萔��
---------------------------------------------------------*/
//#define MAX_I_COUNT	(8192)/* (1/60)������̍ő又�����߁B */

global int spell_register[REG_NUM_99_MAX];


/*---------------------------------------------------------
	(r35)�J�[�h�X�N���v�g�p����(�J�[�h���W�X�^�S������)
---------------------------------------------------------*/

static CPU_FUNC(spell_cpu_initialize_all_register)
{
	unsigned int i;
	for (i=0; i<REG_NUM_99_MAX; i++)
	{
		spell_register[i] = (0);	/* �l(0)�ŏ����� */
	}
	//
	REG_0f_GAME_DIFFICULTY	= cg.game_difficulty;/* ��Փx���W�X�^�B */
}

#if (1)/*(�����I�ɃJ�[�h�C���^�v���^��œ��삷�閽�߂Ɉڍs����)*/
/*---------------------------------------------------------
	(r35)�����̊֐��́A���W�X�^�ł͂Ȃ��A���W�X�^�ԍ���n���̂Œ��ӂ̎��B
	�ԈႦ�ă��W�X�^�n�����ꍇ�A�ԈႢ����psp���n���O�A�b�v����B
	�ԈႢ�₷���̂ŁA�֐�����NUM����ꂽ�B
	-------------------------------------------------------
	(r35)�����̊֐����A���̃��W�X�^�l��n���Ȃ����Ƃ����ƁA
	�J�[�h�X�N���v�g�p���߂Ɉڍs�����ꍇ�A
	�J�[�h�X�N���v�g�ł͂����������W�X�^�l��n���Ȃ��B
	�Ƃ����d�l�̃X�N���v�g��\�肵�Ă���B
	���R�̓��W�X�^�l��n���ƃX�N���v�g�̎��s���x���ቺ���邩��B
---------------------------------------------------------*/


#if (0)
	/*(�J�����A�f�o�b�O�ŁA�x��)*/
	#define DEBUG_global global
#else
	/*(�����[�X�ŁA�Ƃ肠����)*/
	#define DEBUG_global static
#endif
/*---------------------------------------------------------
	(r35)�J�[�h�X�N���v�g�p����(�J�E���^�[)
---------------------------------------------------------*/
/*([global]���̃��W���[�������g��Ȃ����A�X�N���v�g�ڍs����ׂ̂��̂Ȃ̂ōœK���h�~�ׁ̈B[static]���ƃC�����C���W�J����Ă��܂��̂ł܂���)*/
DEBUG_global/*static*/ void count_up_limit_NUM(int register_number_counter, int max_num)
{
	spell_register[register_number_counter]++;
	if (max_num <= (spell_register[register_number_counter]))
	{
		spell_register[register_number_counter] = (0);
	}
}
#endif

/*---------------------------------------------------------
	(r36)�J�[�h�X�N���v�g�p����(sincos)
	-------------------------------------------------------
	�g�p���W�X�^:
	REG_11		== HATSUDAN_01_speed256 	����(���x�A���a, t256�`��)
	REG_13		== HATSUDAN_03_angle65536	����(�p�x65536, d65536�`��)
	REG_02: 	�o��(cos(angle) x radius, t256�`��)
	REG_03: 	�o��(sin(angle) x radius, t256�`��)
---------------------------------------------------------*/

global CPU_FUNC(sincos256)
{
	#if (0)//
	h->math_vector.x256  = si n1024((int_angle1024))*(/*p->speed*/(16*KETM075));/*fps_factor*/		/* CCW�̏ꍇ */
	h->math_vector.y256  = co s1024((int_angle1024))*(/*p->speed*/(16*KETM075));/*fps_factor*/
	REG_02_DEST_X	= (((int)(co s(de g1024tor ad(REG_10_ANGLE1024))*REG_11_SPEED256)));/*(fps_factor)*/
	REG_03_DEST_Y	= (((int)(si n(de g1024tor ad(REG_10_ANGLE1024))*REG_11_SPEED256)));/*(fps_factor)*/
	#else
	{
		int sin_value_t256; 		// sin_value_t256 = 0;
		int cos_value_t256; 		// cos_value_t256 = 0;
		int256_sincos1024( (deg65536to1024(HATSUDAN_03_angle65536)), &sin_value_t256, &cos_value_t256);
		REG_02_DEST_X	 = (((cos_value_t256)*(HATSUDAN_01_speed256))>>8);/*fps_factor*/
		REG_03_DEST_Y	 = (((sin_value_t256)*(HATSUDAN_01_speed256))>>8);/*fps_factor*/
	}
	#endif
}
/*---------------------------------------------------------

---------------------------------------------------------*/

global OBJ_CALL_FUNC(set_REG_DEST_XY)
{
	REG_02_DEST_X	= (src->center.x256);	/* �e��x256 �{�X���S���甭�e�B */
	REG_03_DEST_Y	= (src->center.y256);	/* �e��y256 �{�X���S���甭�e�B */
}

/*---------------------------------------------------------
	���@(src)�_���e(dest)�̊p�x(���S�����S)��
	(�J�[�hcpu�̓���������HATSUDAN_03_angle65536��)�v�Z�B
	-------------------------------------------------------
	��(src)�̒��S���W����A
	��(dest)�̒��S���W�֑_���p�x���v�Z���A
	�ꎞ�p�x�ϐ�(HATSUDAN_03_angle65536)�Ɍv�Z����B
	-------------------------------------------------------
	�Ⴆ�΁A��(src)==���@�A��(dest)==�G�e�Ȃ�A���@�_���e�B
	-------------------------------------------------------
	(r35)�_���p�v�Z�͑S�āA�J�[�hCPU���o�R����悤�ɕύX�����B
	�_���p�v�Z���v���O�����̕��������ɂ���ƁA���s���x���x�����Ēe���͖����B
	���̕ύX�Ŗ��̂��镔�����o�Ă��邪�A
	�{�Ƃł͂���Ǝ����d�l�ɂȂ��Ă���Ǝv����B
---------------------------------------------------------*/
extern int atan_65536(int y, int x);/*(vfpu/math_atan65536i.c)*/
global CPU_FUNC(tmp_angleCCW65536_src_nerai)
{
	#if (0)/*(���d�l����)*/
	REG_00_SRC_X	= (src->center.x256);		/*(�_����)*/
	REG_01_SRC_Y	= (src->center.y256);		/*(�_����)*/
	REG_02_DEST_X	= (dest->center.x256);		/*(�_����)*/
	REG_03_DEST_Y	= (dest->center.y256);		/*(�_����)*/
	#endif
	HATSUDAN_03_angle65536 = (atan_65536((REG_01_SRC_Y)-(REG_03_DEST_Y), (REG_00_SRC_X)-(REG_02_DEST_X)));
}

/*---------------------------------------------------------
	���@�_���p�쐬
	-------------------------------------------------------
	dest(x,y)�̈ʒu����A�v���C���[(x,y)�Ɍ������p�x��
	HATSUDAN_03_angle65536�Ɍv�Z����B
---------------------------------------------------------*/
global CPU_FUNC(calculate_jikinerai)
{
	OBJ *zzz_player;
	zzz_player = &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER];
	REG_00_SRC_X	= (zzz_player->center.x256);		/*(�_����)*/
	REG_01_SRC_Y	= (zzz_player->center.y256);		/*(�_����)*/
	#if (0)/*(���d�l����)*/
	REG_02_DEST_X	= (dest->center.x256);			/*(�_����)*/
	REG_03_DEST_Y	= (dest->center.y256);			/*(�_����)*/
	#endif
	// ���d�l����(zzz_player, dest);
	tmp_angleCCW65536_src_nerai();
}

/*---------------------------------------------------------
	(r36)�J�[�h�X�N���v�g�p����(multiprex_rate_vector)
	�������������B
	-------------------------------------------------------
	�g�p���W�X�^:
	REG_00: 	����(x0)
	REG_01: 	����(y0)
	REG_02: 	����(x1)/�o��(x2)
	REG_03: 	����(y1)/�o��(y2)
	REG_11: 	����(��������, t256�`��)
---------------------------------------------------------*/
			#if (0)/*(���B�������l�����āA��������B)*/
			REG_00_SRC_X	= ((my_menu_resource[i].x_offset[move_mode	])<<8); 	/*(�����ʒu[A]t256()�`��)*/
		//	REG_01_SRC_Y	= ((my_menu_resource[i].y_offset[move_mode	])<<8); 	/*(�����ʒu[A]t256()�`��)*/
			REG_02_DEST_X	= ((my_menu_resource[i].x_offset[move_mode+1])<<8); 	/*(�����ʒu[B]t256()�`��)*/
		//	REG_03_DEST_Y	= ((my_menu_resource[i].y_offset[move_mode+1])<<8); 	/*(�����ʒu[B]t256()�`��)*/
			REG_11_TAMA1	= move_rate256;/*(��������t256()�`��)*/
			multiprex_rate();
			#endif

global CPU_FUNC(multiprex_rate_vector)
{
	#if 1/*([X]�x�N�g������)*/
	/*(���������͊���[A]�Ɗ���[B]�ŁA�ǂ��炪��ł��\��Ȃ��B)*/
	int x_sa256;/*(t256()�`��)*/
	/*�����ʒu[A]*/x_sa256	= (((REG_00_SRC_X )*(256-REG_11_GOUSEI_WARIAI256))>>8);/*(����[A]������)*/
	/*�����ʒu[B]*/x_sa256 += (((REG_02_DEST_X)*(	 REG_11_GOUSEI_WARIAI256))>>8);/*(����[B]������)*/
	REG_02_DEST_X	= x_sa256;/*(����[X])*/
	#endif
	#if 1/*([Y]�x�N�g������)*/
	/*(���������͊���[A]�Ɗ���[B]�ŁA�ǂ��炪��ł��\��Ȃ��B)*/
	int y_sa256;/*(t256()�`��)*/
	/*�����ʒu[A]*/y_sa256	= (((REG_01_SRC_Y )*(256-REG_11_GOUSEI_WARIAI256))>>8);/*(����[A]������)*/
	/*�����ʒu[B]*/y_sa256 += (((REG_03_DEST_Y)*(	 REG_11_GOUSEI_WARIAI256))>>8);/*(����[B]������)*/
	REG_03_DEST_Y	= y_sa256;/*(����[Y])*/
	#endif
}

/*---------------------------------------------------------
	(r36)�J�[�h�X�N���v�g�p����(replay_rand_init)
	���v���C�Ή����������B
	-------------------------------------------------------
	�g�p���W�X�^:
	REG_00: 	����(���v���C�Ή������V�[�h�ݒ�)
---------------------------------------------------------*/

/*---------------------------------------------------------
	(r36)�J�[�h�X�N���v�g�p����(replay_rand)
	���v���C�Ή����������B
	-------------------------------------------------------
	�g�p���W�X�^:
	REG_03: 	�o��(���v���C�Ή������l)
---------------------------------------------------------*/


/*---------------------------------------------------------
	�J�[�h��������
---------------------------------------------------------*/

#define SPELL_TIME_0032 	(32)
#define SPELL_TIME_0048 	(48)
#define SPELL_TIME_0060 	(60)
#define SPELL_TIME_0064 	(64)
#define SPELL_TIME_0070 	(70)
#define SPELL_TIME_0096 	(96)
#define SPELL_TIME_0128 	(128)
#define SPELL_TIME_0192 	(192)
#define SPELL_TIME_0256 	(256)
#define SPELL_TIME_0352 	(352)/*(352==32*11)*/
#define SPELL_TIME_0512 	(512)
//#define SPELL_TIME_64_640 (512+64)
//#define SPELL_TIME_64_640 (640-64)
#define SPELL_TIME_64_640	(512+32)
#define SPELL_TIME_0640 	(640)
//efine SPELL_TIME_0768 	(768)
#define SPELL_TIME_1024 	(1024)
#define SPELL_TIME_9999 	(16384)
/*
704==768-64;
704== (64���e����)+(10x64);
*/

/*---------------------------------------------------------
	�X�y����Փx�ʃe�[�u��(const_init_nan_ido_�Ȃ�Ƃ�[];)
	-------------------------------------------------------
	�J�[�h�X�N���v�g�Ɉڍs�����ꍇ[�������Z�N�V����]�ŋL�q���鐔���̃e�[�u���B
	-------------------------------------------------------
	[�������Z�N�V����](spell_init)�̂݁A�Q�Ƃł���B
	[���e�Z�N�V����](spell_create)�ł́A�Q�Ƃł��Ȃ��B
	-------------------------------------------------------
	����Aspell_create�Ȃ�Ƃ�(){};�ŁA�Q�Ƃ��Ă���΃o�O�B
	-------------------------------------------------------
	spell_create�Ȃ�Ƃ�(){};�ŁA�Q�Ƃ��Ă���ꍇ�B
	�J�[�h�X�N���v�g�Ɉڍs�o���Ȃ��̂ŁA�Q�Ƃ��Ȃ����B
---------------------------------------------------------*/
#if (1)/*(�J�[�h�X�N���v�g�Ɉڍs�����ꍇ�A�����Ȃ�B(�J�[�h�X�N���v�g����[�������Z�N�V����]�ōs���B))*/
	enum
	{
		H0 = 0,
		H1,
		H2,
		H3,
		H4,
		H5,
		H6,
		H7,
		H8,
		H9,
		H10,
		H11_MAX,
	};
	// �� �g ����A�ؕ��u�F�؈�ࣁv�p
	#define tama_const_H00_NUMS_HOUGA_YELLOW		(H0*4)/*(�������A���F)*/
	#define tama_const_H01_NUMS_HOUGA_RED			(H1*4)/*(�������A�g�F)*/
	#define tama_const_H02_DIVS_HOUGA_YELLOW		(H2*4)/*(�����p�A���F)*/
	#define tama_const_H03_DIVS_HOUGA_RED			(H3*4)/*(�����p�A�g�F)*/
	// �� �`���m�u�v�p
	#define tama_const_H04_NUMS_CIRNO_BLUE			(H4*4)/*(�������A�F)*/
	#define tama_const_H05_DIVS_CIRNO_BLUE			(H5*4)/*(�����p�A�F)*/
	// �� �A���X �u�Ԃ̖��@�v�p
	#define tama_const_H06_NUMS_ALICE_RED			(H6*4)/*(�������A�g�F)*/
	#define tama_const_H07_DIVS_ALICE_RED			(H7*4)/*(�����p�A�g�F)*/
//	#define AK A_03_DIV_ANGLE			(H8*4)
	// �� �u���b�h�}�W�b�N�v���ǂ��p
	#define tama_const_H09_NUMS_REMI_MAGIC			(H8*4)/*(�������A�g�F)*/
	#define tama_const_H10_DIVS_REMI_MAGIC			(H9*4)/*(�����p�A�g�F)*/
	// �� ���H�u�W���b�N�E�U�E���h�r���v���ǂ��p
	#define tama_const_H08_NUMS_SAKUYA_JACK 		(H10*4)/*(�������A�F)*/

	#define const_init_nan_ido_bunkatu_nums_table	const_init_nan_ido_table
	static const /*s32*/u16/*s16 �ł��������H*/ const_init_nan_ido_table[(H11_MAX*4)] =
	{
		// �� �g ����A�ؕ��u�F�؈�ࣁv�p
		// easy 	�ؕ��u�F�؈�ࣁv�Ɠ����B(easy�͎��ԂŊȒP�ɂȂ�悤�ɒ�������)
		// normal	�ؕ��u�F�؈�ࣁv�Ɠ����B
		// hard 	--�B
		// lunatic	17�����ł����Ɣ������̊m�F�ς݁B(����ȏ㍂���x���Ɛh������)
		/* easy 				normal					hard					lunatic 			*/
	//	(6),					(6),					(10),					(17),					/* �ؕ��u�F�؈�ࣁv�p */
	//	(6*8),					(6*8),					(10*8), 				(17*8), 				/* �ؕ��u�F�؈�ࣁv�p */
	//	(int)(65536/(6)),		(int)(65536/(6)),		(int)(65536/(10)),		(int)(65536/(17)),		/* �ؕ��u�F�؈�ࣁv�p */
	//	(int)(65536/(6*8)), 	(int)(65536/(6*8)), 	(int)(65536/(10*8)),	(int)(65536/(17*8)),	/* �ؕ��u�F�؈�ࣁv�p */
//		// (r39)�{�X�����S�ɋ����ė���d�l�ɕύX�����ׁA�]���̂܂܂ł͍����x�Ĕ�����Ȃ��B
		(6),					(6),					(8),					(10),					/* �ؕ��u�F�؈�ࣁv�p */	/* (r39-)�d�l�������ς����̂ŊȒP�ɂ��Ȃ��ƃQ�[���ɂȂ�Ȃ��B */
		(6*8),					(6*8),					(8*8),  				(10*8), 				/* �ؕ��u�F�؈�ࣁv�p */	/* (r39-)�d�l�������ς����̂ŊȒP�ɂ��Ȃ��ƃQ�[���ɂȂ�Ȃ��B */
		(int)(65536/(6)),		(int)(65536/(6)),		(int)(65536/(8)),		(int)(65536/(10)),		/* �ؕ��u�F�؈�ࣁv�p */	/* (r39-)�d�l�������ς����̂ŊȒP�ɂ��Ȃ��ƃQ�[���ɂȂ�Ȃ��B */
		(int)(65536/(6*8)), 	(int)(65536/(6*8)), 	(int)(65536/(8*8)), 	(int)(65536/(10*8)),	/* �ؕ��u�F�؈�ࣁv�p */	/* (r39-)�d�l�������ς����̂ŊȒP�ɂ��Ȃ��ƃQ�[���ɂȂ�Ȃ��B */
//
		// �� �`���m�u�v�p
		(0),					(0),					(18),					(36),					/* ��:3���4�̕����ȒP */
		(0),					(0),					(int)(65536/(18)),		(int)(65536/(36)),		/* ��:3���4�̕����ȒP */
		// �� �A���X �u�Ԃ̖��@�v�p
	//	(3),								(4),								(5),								(6),								/* ������:(-r34) */
		(6),								(3),								(5),								(7),								/* ������:(r35-) */ 	/* ��:3���4�̕����ȒP */
	//	(int)(65536/(18)),					(int)(65536/(24)),					(int)(65536/(40)),					(int)(65536/(64)),					/* �����p�x:  (-r34) */
	//	(int)(65536-(int)(65536/(18))), 	(int)(65536-(int)(65536/(24))), 	(int)(65536-(int)(65536/(40))), 	(int)(65536-(int)(65536/(64))), 	/* �����p�x_R:(-r34) */
		(int)(65536/(12)),					(int)(65536/(18)),					(int)(65536/(20)),					(int)(65536/(24)),					/* �����p�x:  (r35-) */
	//	(int)(65536-(int)(65536/(12))), 	(int)(65536-(int)(65536/(18))), 	(int)(65536-(int)(65536/(20))), 	(int)(65536-(int)(65536/(24))), 	/* �����p�x_R:(r35-) */
		// �� �u���b�h�}�W�b�N�v���ǂ��p
		(10),					(10),					(13),					(18),					/* �u���b�h�}�W�b�N�v���ǂ��p */
		(int)(65536/(10)),		(int)(65536/(10)),		(int)(65536/(13)),		(int)(65536/(18)),		/* �u���b�h�}�W�b�N�v���ǂ��p */
		// �� ���H�u�W���b�N�E�U�E���h�r���v���ǂ��p
		/* easy 				normal					hard					lunatic 			*/
	//	(6),					(12),					(24),					(32),					/* ���H�u�W���b�N�E�U�E���h�r���v���ǂ��p */
		(16),					(32),					(40),					(48),					/* ���H�u�W���b�N�E�U�E���h�r���v���ǂ��p */
	};
#endif

/*---------------------------------------------------------
	�J�[�h�𐶐��B
	-------------------------------------------------------
---------------------------------------------------------*/
/* static�ϐ� �𒲂ׂ��Ƃׁ̈A�ꎞ�I��local �ɂ���B */
#define local static

/*---------------------------------------------------------
	[�e���O���[�v(1)�Z�N�V����]
	-------------------------------------------------------
	�����p�e�����R�[���o�b�N
	�e���A�N�V����#02
	-------------------------------------------------------
	�e���̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�)�B
	�e������ʊO�̏ꍇ�́A�e�������B
---------------------------------------------------------*/
global RECT_CLIP_CLASS rect_clip;
local OBJ_CALL_FUNC(hatudan_system_B_side_hansya)/* �e���˃R�[���o�b�N */
{
	{
		/* [���̕ǂŔ��˂���] �e���e���ݒ�̈��ʂ�菭��(4[pixel])��O�̈ʒu�ɗ����ꍇ�B */
		if (0 < (rect_clip.bullet_clip_min.x256) + t256(4) - (src->center.x256) )
		{
			goto common_hansya;
		}
		else
		/* [�E�̕ǂŔ��˂���] �e���e���ݒ�̈��ʂ�菭��(4[pixel])��O�̈ʒu�ɗ����ꍇ�B */
		if (0 > (rect_clip.bullet_clip_max.x256) - t256(4) - (src->center.x256) )
		{
			goto common_hansya;
		}
		goto done_hansya;/*(���˂��Ȃ�)*/
	common_hansya:/*(���˂���\������)*/
		/* ���ɔ��˂��Ă���ꍇ�͔��˂��Ȃ��ŁA�e�������B */
		if (0!=(src->hatudan_register_spec_data & TAMA_SPEC_KABE_SAYUU_HANSYA_BIT))
		{	/*(���ˏ������s��Ȃ�)*/
			src->jyumyou = JYUMYOU_NASI;/*(�e������)*/
		}
		else
	//	if (JYUMYOU_NASI != src->jyumyou)
		{	/*(���ˏ������s��)*/
			src->hatudan_register_spec_data |= TAMA_SPEC_KABE_SAYUU_HANSYA_BIT;
		//	src->rotationCCW1024 += (1024*3/4);/* -90�x��] */		/* �_��[��2] */
		//	src->rotationCCW1024 += (1024/4);/* 90�x��] */ 		/* �_��[��2] */
			src->rotationCCW1024 = (1024)-(src->rotationCCW1024);	/* ���][��1] */
			mask1024(src->rotationCCW1024);
			src->hatudan_register_speed65536		= ((t256(1.0)<<8));/*(����)*/
			src->hatudan_register_tra65536		= (1);/*(������)*/
		}
	done_hansya:/*(���ˏ����I���)*/
		;
	}
	/* �㉺�̏ꍇ�͒e�������B */
	if (
		(0 < (rect_clip.bullet_clip_min.y256) - (src->center.y256) )||
		(0 > (rect_clip.bullet_clip_max.y256) - (src->center.y256) )
	)
	{
		src->jyumyou = JYUMYOU_NASI;
	}
}


/*---------------------------------------------------------
	[�e���O���[�v(1)�Z�N�V����]
	-------------------------------------------------------
	�����p�e�����R�[���o�b�N
	�e���A�N�V����#00
	-------------------------------------------------------
	�e���̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�)�B
	�e������ʊO�̏ꍇ�́A�e�������B
---------------------------------------------------------*/
local OBJ_CALL_FUNC(hatudan_system_B_gamen_gai_tama_kesu)/* ��ʊO�e�����R�[���o�b�N */
{
	/* ��ʊO�̏ꍇ�͒e�������B */
	if (
	(0 < (rect_clip.bullet_clip_min.x256)-(src->center.x256) ) ||
	(0 > (rect_clip.bullet_clip_max.x256)-(src->center.x256) ) ||
	(0 < (rect_clip.bullet_clip_min.y256)-(src->center.y256) ) ||
	(0 > (rect_clip.bullet_clip_max.y256)-(src->center.y256) ) )
	{
		src->jyumyou = JYUMYOU_NASI;
	}
}


/*---------------------------------------------------------
	�J�[�h�𐶐��B
	-------------------------------------------------------
---------------------------------------------------------*/

#include "spell_00_common.h"
//
#include "spell_00_zako.h"
#include "spell_00_sonota.h"
//
#include "spell_01_mima.h"
#include "spell_01_alice.h"
#include "spell_01_rumia.h"
#include "spell_01_meirin.h"
#include "spell_01_chrno.h"
#include "spell_01_kaguya.h"
#include "spell_01_daiyousei.h"
#include "spell_01_aya.h"
#include "spell_01_momiji.h"

//
#include "spell_01_pache.h"
#include "spell_01_sakuya.h"
#include "spell_01_remilia.h"

//
#include "spell_01_ruiz.h"


/*---------------------------------------------------------
	���ʃJ�[�h������
	-------------------------------------------------------
	�J�[�h���Ƃɗ\�ߐ������Ԃ��o�^���Ă���B
---------------------------------------------------------*/
enum
{
	TAMA_MAP_06_KOUMA	= 0,	/* �g����/�d�X��(�i�C�t�e/���e) */
	TAMA_MAP_08_EIYA,			/* �i�鏴/���_�\(�D�e/���e) */
	TAMA_MAP_10_TIREI,			/* �n��a/�ԉf��(�n�[�g�e/�K�e) */
};
enum
{
	RUN_00_CARD_INTERPRETER 	= 0,	/* �J�[�h�C���^�v���^�B */
	RUN_01_C_LANGUAGE,					/* C����(�p�~�\��)�B(C�����S�Ĕp�~�ł���΁A���s���x�����シ�遦1) */
};

typedef struct
{
	void (*spell_generate_section)(OBJ/**/ *sss);		/* [���e(�e����)�Z�N�V����]���s���� */		/* �J�[�h�X�N���v�g�Ɉڍs�����ꍇ��[���e�Z�N�V����]�B */
	void (*spell_initialze_section)(OBJ/**/ *sss);		/* [�������Z�N�V����]���������� */			/* �J�[�h�X�N���v�g�Ɉڍs�����ꍇ��[�������Z�N�V����]�B */
	u16 spell_limit_max_time;							/* �J�[�h�������� */						/* �J�[�h�X�N���v�g�Ɉڍs�����ꍇ�A�{��[�������Z�N�V����]���ōs���B */
	u8 tama_map;										/* �e�摜�̃}�b�v�ԍ� */					/* �J�[�h�X�N���v�g�Ɉڍs�����ꍇ�A�{��[�������Z�N�V����]���ōs���B */
	u8 scr_ipt_run_flag;								/* ���s����(�J�[�h�C���^�v���^ / C����) */
} SPELL_RESOURCE;
//	/*const*/ static void (*spell_create_bbb[(SPELL_MAX/*16+1*/)])(OBJ/**/ *sss) =
	/*const*/ static SPELL_RESOURCE system_spell_resource[(SPELL_MAX/*16+1*/)] =
{
	#define spell_create_99_mitei spell_create_08_rumia_night_bird
/* 00 00 */ {	NULL,									NULL,										SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 00 */	/* �J�[�h�������Ȃ� */
			// 0x00- ���U�R / �{�X���p�J�[�h
/* 01 01 */ {	spell_create_01_sakuya_misogi_normal,	NULL,										SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 01 */	/* [���] �g5�ʒ�-�{�X �u�ʏ�U��1�v����S�J�[�h */
/* 02 02 */ //{ spell_create_02_24nerai,				NULL,										SPELL_TIME_0048,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 02 */	/* [���] �g5�ʒ�-�{�X 24�J�[�h (��p�u�~�X�f�B���N�V�������ǂ�(1/2)�v) */
/* 03 03 */ //{ spell_create_03_11nife, 				NULL,										SPELL_TIME_0064,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 03 */	/* [���] �g5�ʒ�-�{�X 11�J�[�h (��p�u�~�X�f�B���N�V�������ǂ�(2/2)�v) */
/* 02 02 */ {	spell_create_99_mitei,					NULL,										SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 02 */	/* [�G��] (���g�p) */
/* 03 03 */ {	spell_create_99_mitei,					NULL,										SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 03 */	/* [�G��] (���g�p) */
/* 04 04 */ {	spell_create_04_pink_hearts,			NULL,										SPELL_TIME_0640,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 04 */	/* [���] �g5�ʃ{�X �u�ʏ�U��1(1/2)�v�ɂ�����Ƃ��������J�[�h(�\��) SPELL_TIME_0128(r32) */
/* 05 05 */ {	spell_create_05_32way_dual, 			NULL,										SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 05 */	/* [���] �g5�ʃ{�X �u�ʏ�U��1/2(2/2)�v�ɂ�����Ƃ��������J�[�h(�\��) */
/* 06 06 */ {	spell_create_06_luna_clock_32way,		NULL,										SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 06 */	/* [���] �g5�ʃ{�X ���ہu���i�N���b�N(1/2)�v�ɂ�����Ƃ��������J�[�h(�\��) */
/* 07 07 */ {	spell_create_07_80way_dual_five,		NULL,										SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 07 */	/* [���] �g5�ʃ{�X �u�ʏ�U��3(1/2)�v�ɂ�����Ƃ��������J�[�h(�\��) */
			// 0x08- ���U�R / �{�X���p�J�[�h
/* 08 08 */ {	spell_create_08_rumia_night_bird,		NULL,										SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 08 */	/* [���[�~�A] �g1�ʃ{�X �镄�u�i�C�g�o�[�h�v�ɂ�����Ƃ��������J�[�h(�\��) */
/* 09 09 */ {	spell_create_09_zako_sentakki,			NULL,										SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 09 */	/* [�G��] ����@�J�[�h(�Ƃ肠�����J�[�h�V�X�e���쐬�̎����ō���Ă݂��z) */
/* 10 0a */ {	spell_create_1a_dai_yousei_midori,		spell_init_1a_dai_yousei_midori,			SPELL_TIME_0064,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 1a */	/* [��d��] �Ί��� */
/* 11 0b */ {	spell_create_1b_dai_yousei_aka, 		spell_init_1b_dai_yousei_aka,				SPELL_TIME_0064,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 1b */	/* [��d��] �Ԋ��� */
/* 12 0c */ {	spell_create_14_sakuya_jack_oodama32,	spell_init_14_sakuya_jack_oodama32, 		SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 14 */	/* [���] ���H�u�W���b�N�E�U�E���h�r���v�ɂȂ邩������Ȃ��e�X�g */
/* 13 0d */ {	spell_create_22_sakuya_miss_direction,	spell_init_22_sakuya_miss_direction,		SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 1c */	/* [���] ��p�u�~�X�f�B���N�V�����v */
/* 14 0e */ {	spell_create_1f_remilia_kougeki2,		NULL,										SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 1f */	/* [���~���A] �ʏ�U��2 ���ۂ����� */
/* 15 0f */ {	spell_create_11_tengu_shot, 			NULL,										SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 11 */	/* [�V��l] �������̗d��1(�V��l?)�����J�[�h */
			// 0x10- ���U�R / �{�X���p�J�[�h
/* 16 10 */ {	spell_create_10_cirno_misogi,			spell_init_10_cirno_misogi, 				SPELL_TIME_0096,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 10 */	/* [�`���m] �g2�ʃ{�X �`���m�S�J�[�h */
/* 17 11 */ {	spell_create_13_perfect_freeze, 		spell_init_13_perfect_freeze,				SPELL_TIME_64_640,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 13 */	/* [�`���m] �g2�ʃ{�X �p�[�t�F�N�g�t���[�Y */
/* 18 12 */ {	spell_create_16_diamond_blizzard,		NULL,										SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 16 */	/* [�`���m] �g2�ʃ{�X �ᕄ�u�_�C�A�����h�u���U�[�h�v */
/* 19 13 */ {	spell_create_25_alice_suwako,			NULL,										SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 25 */	/* [�A���X] �A���X	�z�K�q���ۂ�(?)�J�[�h */
/* 20 14 */ {	spell_create_17_alice_nejiri10sec,		spell_init_17_alice_nejiri10sec,			SPELL_TIME_0192,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 17 */	/* [�A���X] �ʏ�U��(���ǂ�) �˂���e10�b SPELL_TIME_0640 */
/* 21 15 */ {	spell_create_0e_aka_2nd,				spell_init_0e_aka_2nd,						SPELL_TIME_1024,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 0e */	/* [�A���X] ��EX�ʃ{�X �Ԃ̖��@2�i�K�ڂ��ǂ� */
/* 22 16 */ {	spell_create_0b_alice_zako_doll,		NULL,										SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 0b */	/* [�A���X] SPELL_TIME_0128 �d3�ʃ{�X �l�`�J�[�h(���ǂ�)(256>240=4x60) */
/* 23 17 */ {	spell_create_1e_alice_tama_doll,		NULL,										SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 1e */	/* [�A���X] �d3�ʃ{�X �l�`�J�[�h(���ǂ�)(256>240=4x60) �����u�����̕������l�`�v */
			// 0x18- ���U�R / �{�X���p�J�[�h
/* 24 18 */ {	spell_create_0c_hana_test,				spell_init_0c_hana_test,					SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 0c */	/* [����] �ԂĂ��ƃJ�[�h */
/* 64 40 */ {	spell_create_20_sonota_debug_cw_ao, 	NULL,										SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 40 */	/* [���̑�] �f�o�b�O�J�[�hCW�� */
/* 65 41 */ {	spell_create_21_sonota_debug_ccw_aka,	NULL,										SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 41 */	/* [���̑�] �f�o�b�O�J�[�hCCW�� */
/* 27 1b */ {	spell_create_1b_cirno_icecle_fall,		spell_init_1b_cirno_icecle_fall,			SPELL_TIME_0352,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 26 */	/* [�`���m] �X���u�A�C�V�N���t�H�[���v */
/* 68 44 */ {	spell_create_44_ruiz3,					NULL,										SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 44 */	/* [���C�Y] ���E���J�[�h */
/* 29 1d */ {	spell_create_1d_amefuri_test,			spell_init_1d_amefuri_test, 				SPELL_TIME_0640,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 1d */	/* [(���A��)] �J�Ă��� */
/* 30 1e */ {	spell_create_1e_momiji_no_nonoji,		NULL,										SPELL_TIME_0640,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 26 */	/* [��] �̂̂��J�[�h */
/* 31 1f */ {	spell_create_1f_koakuma,				NULL,										SPELL_TIME_0640,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 26 */	/* [�p�`�F] �T�C�����g�Z���i �� �Ă��Ɨp */
	// 0x20- �{�X��p�J�[�h
/* 32 20 */ {	spell_create_15_aya_misogi1,			NULL,										SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 15 */	/* [��] �̒ʏ�e���ۂ��J�[�h�������Ă݂�e�X�g1 */
/* 33 21 */ {	spell_create_23_aya_misogi2,			NULL,										SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 23 */	/* [��]�̒ʏ�e���ۂ��J�[�h�������Ă݂�e�X�g2 */
/* 34 22 */ {	spell_create_22_aya_yatimata,			spell_init_22_aya_yatimata, 				SPELL_TIME_0640,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 22 */	/* [��] �򕄁u�V�̔��ˁv */
/* 35 23 */ {	spell_create_12_meirin_magaru_kunai,	spell_init_12_meirin_magaru_kunai,			SPELL_TIME_0640,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 12 */	/* [����] �g3�ʒ��{�X �u�ʏ�U���v�̋Ȃ���N�i�C�B */
/* 36 24 */ {	spell_create_0f_aya_doll,				NULL,										SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 0f */	/* [��] �l�`�J�[�h(�Ă��ƃJ�[�h�u�댯�Ȏ�v) */
/* 37 25 */ {	spell_create_0a_houka_kenran,			spell_init_0a_houka_kenran, 				SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 0a */	/* [�g����] �g3�ʃ{�X �ؕ��u�F�؈�ࣁv�ɂ�����Ƃ��������J�[�h(�\��) */
/* 38 26 */ {	spell_create_26_aya_saifu,				NULL,										SPELL_TIME_9999,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 26 */	/* [��] �Ǖ� */
/* 39 27 */ {	spell_create_27_hosigata_test,			spell_init_27_hosigata_test,				SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 27 */	/* [���̑�] ���^�e�X�g */
			// 0x28- �{�X��p�J�[�h
/* 40 28 */ {	spell_create_99_mitei,					NULL,										SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 30 */	/* [] */
/* 41 29 */ {	spell_create_29_rumia_demarcation,		spell_init_29_rumia_demarcation,			SPELL_TIME_0512,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 31 */	/* [���[�~�A] �g1�ʃ{�X �ŕ��u�f�B�}�[�P�C�V�����v�ɂ�����Ƃ��������J�[�h(�\��) dimmercation / demarcation ���E */
/* 42 2a */ {	spell_create_2a_sakuya_baramaki1,		NULL,										SPELL_TIME_0640,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 26 */	/* [���] �g5�ʒ�-�{�X �΂�T��1 �J�[�h () */
/* 43 2b */ {	spell_create_2b_sakuya_baramaki2,		NULL,										SPELL_TIME_0640,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 26 */	/* [���] �g5�ʒ�-�{�X �΂�T��2 �J�[�h () */
/* 44 2c */ {	spell_create_2c_sakuya_blue_red_knife,	spell_init_2c_sakuya_blue_red_knife,		SPELL_TIME_0512,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 26 */	/* [���] �g5�ʒ�-�{�X �u�ʏ�U��3�v�ԃi�C�t */
/* 45 2d */ {	spell_create_2d_sakuya_misogi_lunatic,	NULL,										SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 01 */	/* [���] �g5�ʒ�-�{�X �u�ʏ�U��1�v����S�J�[�h(lunatic only) */
/* 46 2e */ {	spell_create_2e_pache_agni_shine_1, 	spell_init_2e_pache_agni_shine_1,			SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 2e */	/* [�p�`�F] "�@�@�@�@�Ε��u�A�O�j�V���C���v" No.15 */	// (�΂������ς�������) (�P��)�A�O�j(�C���h�_�b�̉΂̐_)
/* 47 2f */ {	spell_create_2f_pache_princess_undine,	NULL,										SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 2f */	/* [�p�`�F] "�@�����u�v�����Z�X�E���f�B�l�v" No.16 */	// (���������ς�������) �u�d���̏�(����:�B���p�t�p���P���X�X)�v�ɓo�ꂷ�鐅�̐��B
			// 0x30- �{�X��p�J�[�h
/* 48 30 */ {	spell_create_30_pache_sylphy_horn_1,	spell_init_30_pache_sylphy_horn_1,			SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 30 */	/* [�p�`�F] "�@�@�@�@�ؕ��u�V���t�B�z�����v" No.17 */	// (�؂������ς�������) (�����i�鐸���)�p�J
/* 49 31 */ {	spell_create_31_pache_rage_tririton_1,	spell_init_31_pache_rage_tririton_1,		SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 31 */	/* [�p�`�F] "�@�@�y���u���C�W�B�g�����g���v" No.18 */	// (�y�������ς�������) (�{��)�g�����g��(�X�g�[���w���W)
/* 50 32 */ {	spell_create_32_pache_metal_fatigue,	spell_init_32_pache_metal_fatigue,			SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 32 */	/* [�p�`�F] "�@�@�����u���^���t�@�e�B�[�O�v" No.19 */	// (��(����)�������ς�������) ������J
/* 51 33 */ {	spell_create_33_pache_agni_shine_2, 	spell_init_33_pache_agni_shine_2,			SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 33 */	/* [�p�`�F] "�@�@�Ε��u�A�O�j�V���C���㋉�v" No.20 */	// (�΂������ς�������) (�P��)�A�O�j(�C���h�_�b�̉΂̐_)
/* 52 34 */ {	spell_create_34_pache_sylphy_horn_2,	spell_init_34_pache_sylphy_horn_2,/*���p*/	SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 34 */	/* [�p�`�F] "�@�@�ؕ��u�V���t�B�z�����㋉�v" No.21 */	// (�؂������ς�������) (�����i�鐸���)�p�J
/* 53 35 */ {	spell_create_35_pache_rage_tririton_2,	spell_init_31_pache_rage_tririton_1,/*(?)*/ SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 35 */	/* [�p�`�F] "�y���u���C�W�B�g�����g���㋉�v" No.22 */	// (�y�������ς�������) (�{��)�g�����g��(�X�g�[���w���W)
/* 54 36 */ {	spell_create_36_pache_agni_radiance,	NULL,										SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 36 */	/* [�p�`�F] "�@�Ε��u�A�O�j���C�f�B�A���X�v" No.23 */	// (�΂������ς������ς�������) �A�O�j(�C���h�_�b�̉΂̐_)
/* 55 37 */ {	spell_create_37_pache_bury_in_lake, 	NULL,										SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 37 */	/* [�p�`�F] "�@�@�@�����u�x���[�C�����C�N�v" No.24 */	// (���������ς������ς�������)  �΂ɖ��߂�(����)
			// 0x38- �{�X��p�J�[�h
/* 56 38 */ {	spell_create_38_pache_green_storm,		spell_init_38_pache_green_storm,/*���p*/	SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 38 */	/* [�p�`�F] "�@�@�@�ؕ��u�O���[���X�g�[���v" No.25 */	// (�؂������ς������ς�������) �΂̗�
/* 57 39 */ {	spell_create_39_pache_tririton_shake,	spell_init_31_pache_rage_tririton_1,/*���p*/SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 39 */	/* [�p�`�F] "�@�@�y���u�g�����g���V�F�C�N�v" No.26 */	// (�y�������ς������ς�������) (�h���)�g�����g��(�X�g�[���w���W)
/* 58 3a */ {	spell_create_3a_pache_silver_dragon,	spell_init_3a_pache_silver_dragon,/*(??)*/	SPELL_TIME_0032,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 3a */	/* [�p�`�F] "�@�@�@�����u�V���o�[�h���S���v" No.27 */	// (��(����)�������ς������ς�������) �u���Ƌ�(����:���R)�v
/* 59 3b */ {	spell_create_3b_pache_lava_cromlech,	spell_init_31_pache_rage_tririton_1,/*���p*/SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 3b */	/* [�p�`�F] "�΁��y���u���[���@�N�������N�v" No.28 */	// (�΁{�y���n��) ����(�X�g�[���w���W)
/* 60 3c */ {	spell_create_3c_pache_forest_blaze, 	NULL,										SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 3c */	/* [�p�`�F] "�؁��Ε��u�t�H���X�g�u���C�Y�v" No.29 */	// (�؁{�΁�)�X�щ΍�
/* 61 3d */ {	spell_create_3d_pache_water_elf,		NULL,										SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 3d */	/* [�p�`�F] "�@�����ؕ��u�E�H�[�^�[�G���t�v" No.30 */	// (���{�؁�)���n��(���d��)
/* 62 3e */ {	spell_create_3e_pache_mercury_poison,	spell_init_3e_pache_mercury_poison, 		SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 3e */	/* [�p�`�F] "���������u�}�[�L�����|�C�Y���v" No.31 */	// (��(����)�{����)���⒆��
/* 63 3f */ {	spell_create_3f_pache_emerald_megalith, spell_init_3f_pache_emerald_megalith,		SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 3f */	/* [�p�`�F] "�y�������u�G�������h���K���X�v" No.32 */	// (�y�{��(����)��)�G�������h�̋���
			// 0x40
/* 66 42 */ {	spell_create_42_pache_laser1,			NULL,										SPELL_TIME_0640,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 42 */	/* [�p�`�F] �ʏ�U��(���ǂ�) �̃��[�U�[���ǂ� */
/* 67 43 */ {	spell_create_43_pache_laser2,			spell_init_43_pache_laser2, 				SPELL_TIME_0640,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 43 */	/* [�p�`�F] �ʏ�U��(���ǂ�) �̃��[�U�[���ǂ� */
/* 25 19 */ {	spell_create_0d_mima_sekkin,			spell_init_mima_kaguya,/*(����)*/			SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 0d */	/* [����] �ڋ߃J�[�h */
/* 26 1a */ {	spell_create_24_mima_toge,				spell_init_mima_kaguya,/*(����)*/			SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 24 */	/* [����] �g�Q�J�[�h */
/* 69 45 */ {	spell_create_18_kaguya01,				spell_init_mima_kaguya,/*(����)*/			SPELL_TIME_0064,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 18 */	/* [�P��] �Ƃ肠���������ɕϊ� */
/* 70 46 */ {	spell_create_19_kaguya04,				spell_init_mima_kaguya,/*(����)*/			SPELL_TIME_0064,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 19 */	/* [�P��] �Ƃ肠���������ɕϊ� */
/* 28 1c */ {	spell_create_23_kaguya_tamanoe, 		spell_init_mima_kaguya,/*(����)*/			SPELL_TIME_0640,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 1d */	/* [�P��] �H���̋ʂ̎}���ǂ� */
/* 71 47 */ {	spell_create_47_sakuya_festival_knife,	spell_init_47_sakuya_festival_knife,		SPELL_TIME_9999,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 47 */	/* [���] ���z�u�t�F�X�e�B�o���i�C�t�v(?) �ނ���ϊ� */
			// 0x48
/* 48 48 */ {	spell_create_48_r34_gokan_kinou,		NULL,										SPELL_TIME_9999,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 48 */	/* [r34]r34�ȑO�̌݊��@�\�B(r35-)�V�X�e���ŕK���K�v�B */
/* 49 49 */ {	spell_create_99_mitei,					NULL,										SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 49 */	/* [] */
/* 4a 4a */ {	spell_create_99_mitei,					NULL,										SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 4a */	/* [] */
/* 4b 4b */ {	spell_create_99_mitei,					NULL,										SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 4b */	/* [] */
/* 4c 4c */ {	spell_create_99_mitei,					NULL,										SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 4c */	/* [] */
/* 4d 4d */ {	spell_create_4d_remilia_rasen_knife,	NULL,										SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 4d */	/* [���/���~���A] �g6�ʃ{�X �ʏ�U��  */
/* 4e 4e */ {	spell_create_4e_remilia_vlad_tepes, 	spell_init_4e_remilia_vlad_tepes,			SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 30 */	/* [���/���~���A] �g6�ʃ{�X ���f�u�u���h�E�c�F�y�V���̎􂢁v */
/* 4f 4f */ {	spell_create_4f_remilia_red_magic,		spell_init_4f_remilia_red_magic,			SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 4f */	/* [���/���~���A] �g6�ʃ{�X �u���b�h�}�W�b�N�v */

};

/*---------------------------------------------------------
	���ʕ����B
---------------------------------------------------------*/
static void spell_cpu_common_init(void)
{
	#if (1==USE_BOSS_JIKANGIRE)/*(�g�p�\�肠��B������)*/
	cg.state_flag &= (~JIKI_FLAG_16_0x8000_BOSS_JIKAN_GIRE);/*(���Ԑ؂�t���Ooff)*/
	#endif
	/* �J�[�h���W�X�^�S������ */
	spell_cpu_initialize_all_register();
	#if (1)
	/* �e���R�[���o�b�N�V�X�e���̏����� */
	{
		card.danmaku_callback[0] = danmaku_00_standard_angle_mover;/*(r36-���������֎~)*/	/*(�ʏ�e�p)*/
		card.danmaku_callback[1] = NULL;
		card.danmaku_callback[2] = NULL;
		card.danmaku_callback[3] = NULL;
	}
	#endif
}


/*---------------------------------------------------------
	�����̑O�ɏ���������K�v�����镔���B
---------------------------------------------------------*/

global void spell_cpu_douchuu_init(void)
{
	spell_cpu_common_init();/*(���ʕ���)*/
}


/*---------------------------------------------------------
	�J�[�h�̏������B�J�[�h���ς��Ɩ���s���K�v������B
---------------------------------------------------------*/

global OBJ_CALL_FUNC(card_maikai_init)
{
	spell_cpu_common_init();/*(���ʕ���)*/
	#if (1)/* �J�[�h�X�N���v�g�Ɉڍs�����ꍇ��[�������Z�N�V����]�B */
	/* �J�[�h�������Z�N�V���������s����B */
	if (NULL!=(system_spell_resource[(card.spell_used_number/*-1*/)].spell_initialze_section))
	{
		(system_spell_resource[(card.spell_used_number/*-1*/)].spell_initialze_section)(src);
	}
	/* �J�[�h���Ԃɗ\�ߓo�^���ꂽ�������Ԃ�ݒ肷��B */
	/* �J�[�h�̐������Ԃ�ݒ�(�\�߃J�[�h���Ƃɐݒ肳��Ă���W�����Ԃɐݒ�) */
	/* �J�[�h�̎��Ԑ؂��ݒ� */
	REG_10_BOSS_SPELL_TIMER = system_spell_resource[(card.spell_used_number/*-1*/)].spell_limit_max_time;
	#endif
}


/*---------------------------------------------------------
	�X�y��CPU�����s���A�J�[�h���P�t���[����������B
	-------------------------------------------------------
	�J�[�h��������Ǝ��Ԍo�߂��A�J�[�h�������Ԃ������Ō���B
	�J�[�h�������Ԃ������Ȃ����ꍇ�́A�J�[�h�^�C�v������(SPELL_00)�ɂȂ�B
---------------------------------------------------------*/

global OBJ_CALL_FUNC(card_generate)
{
	if (SPELL_00 != card.spell_used_number) 	/* �X�y�������͕K�v�H */
	{
		REG_10_BOSS_SPELL_TIMER--;				/* ���Ԍo�߂��� */	/*fps_factor*/
		if ((0) < (REG_10_BOSS_SPELL_TIMER))	/* ���Ԑ؂�H */
		{
			/* �e��x256 y256 �{�X�̒��S���甭�e�B */
			set_REG_DEST_XY(src);
			#if (1)/* �J�[�h�X�N���v�g�Ɉڍs�����ꍇ��[���e�Z�N�V����]�B */
			/* �J�[�h������ */
			(system_spell_resource[(card.spell_used_number/*-1*/)].spell_generate_section)(src);
			#endif
		}
		else
		{
			card.spell_used_number = SPELL_00;		/* �J�[�h�����I�� */
		}
	}
}
