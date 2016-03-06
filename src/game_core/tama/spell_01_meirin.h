
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	����̃J�[�h���`���܂��B
---------------------------------------------------------*/


/*---------------------------------------------------------
	�g3�ʒ��{�X �g����
	�ʏ�U��1(1/2)
	16���ʕ΂�S���ʂ΂�T���e
	-------------------------------------------------------
---------------------------------------------------------*/
#if 0
#endif

/*---------------------------------------------------------
	�g3�ʒ��{�X �g����
	�ؕ��u�F�؈�ࣁv�ɂ�����Ƃ��������J�[�h(�\��)
	���v���A�����v����6way�����e(���F)		1�񖈂�(1024/(6*8))[1024/1024�x]��]
	48���ʑS���ʒe(�ԐF)						8�񖈂ɔ��e
	-------------------------------------------------------
	�F�؈��
	�𑜓x�Ƃ����x�Ƃ��̗v���ŁA���̂܂܂ł� psp�œ���C������B
	Lunatic �͂��̂܂܂ŁAHard �ȉ��͍׍H���ĈՂ�������B
	-------------------------------------------------------
	�g�p���W�X�^
//	REG_08_REG0 	�J�E���^�B
	REG_09_REG1 	aaa_angle65536
	REG_0a_REG2 	��Փx�ʒ萔1
	REG_0b_REG3 	��Փx�ʒ萔2
	REG_0c_REG4 	��Փx�ʒ萔3
	REG_0d_REG5 	��Փx�ʒ萔4
---------------------------------------------------------*/

local OBJ_CALL_FUNC(spell_init_0a_houka_kenran)
{
	REG_0c_REG4 	= const_init_nan_ido_bunkatu_nums_table [tama_const_H00_NUMS_HOUGA_YELLOW	+(REG_0f_GAME_DIFFICULTY)];
	REG_0a_REG2 	= const_init_nan_ido_bunkatu_nums_table [tama_const_H01_NUMS_HOUGA_RED		+(REG_0f_GAME_DIFFICULTY)];
	REG_0d_REG5 	= const_init_nan_ido_table				[tama_const_H02_DIVS_HOUGA_YELLOW	+(REG_0f_GAME_DIFFICULTY)];
	REG_0b_REG3 	= const_init_nan_ido_table				[tama_const_H03_DIVS_HOUGA_RED		+(REG_0f_GAME_DIFFICULTY)];//[tama_const_10_HOUGA_YELLOW_ROTATE_ANGLE]
}
#if 1
local OBJ_CALL_FUNC(spell_create_0a_houka_kenran)
{
//	if ((0x10)==((REG_10_BOSS_SPELL_TIMER)&0x1f))/* (16���1��)(128�Ȃ�v8��) */
	if ((0x40)==((REG_10_BOSS_SPELL_TIMER)&0x7f))/* (16���1��)(128�Ȃ�v8��) */
	{
		REG_02_DEST_X	= ((src->center.x256));
		REG_03_DEST_Y	= ((src->center.y256));
		calculate_jikinerai();/* 1:���������쐬������ۂ��B */
		HATSUDAN_01_speed256				= (t256(1.75)); 									/* �e�� */	/* 3.5 2.5 2.0 */
		HATSUDAN_02_speed_offset			= t256(0);/*(�e�X�g)*/
		HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
		HATSUDAN_05_bullet_obj_type 		= (BULLET_KOME_BASE + TAMA_IRO_01_AKA); 							/* [�ԐF�Ēe] */
		HATSUDAN_06_n_way					= REG_0a_REG2;			/*(48)*/				/* [48way] */	/* ���e�� */
		HATSUDAN_07_div_angle65536			= REG_0b_REG3;/*(�������A�g�F)*/	/*(int)(1024/(48))*/	/* �����p�x(1024[360/360�x]�� 48 ����) */	/* 1����n���������p�x */
		hatudan_system_regist_katayori_n_way();/* (r33-) */
		#if (1)
		cpu_bullet_play_15_auto();/* �{�Ƃ͂��������� */
		#endif
	}
//	if ((0x02)==((REG_10_BOSS_SPELL_TIMER)&0x03))/* (2���1��)(8�񖈂ɔ��e) */
	if ((0x08)==((REG_10_BOSS_SPELL_TIMER)&0x0f))/* (2���1��)(8�񖈂ɔ��e) */
	{
		HATSUDAN_01_speed256				= (t256(1.75)); 										/* �e�� */		/*3.5 2.5 2.0*/
		HATSUDAN_02_speed_offset			= t256(0);/*(�e�X�g)*/
		HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
		HATSUDAN_05_bullet_obj_type 		= (BULLET_KOME_BASE + TAMA_IRO_06_KI_IRO);								/* [���F�Ēe] */
		HATSUDAN_06_n_way					= REG_0c_REG4;				/*(6)*/ 				/* [1way] */	/* ���e�� */
		HATSUDAN_07_div_angle65536			= REG_0d_REG5;	/*(int)(1024/(6))*/ 	/* �����p�x(1024[360/360�x]�� 6 ����) */	/* 1����n���������p�x */
		// �����(��CCW������A�����CCW)
		HATSUDAN_03_angle65536				= ((/*0+*/(REG_09_REG1))&(65536-1));				/* ���˒��S�p�x / ����@�\(���@�_��/��) */
		hatudan_system_regist_katayori_n_way();/* (r33-) */
		// �t���(��CCW������A�E���CW)
		HATSUDAN_03_angle65536				= ((65536-(REG_09_REG1))&(65536-1));				/* ���˒��S�p�x / ����@�\(���@�_��/��) */
		hatudan_system_regist_katayori_n_way();/* (r33-) */
		// ��]��
		REG_09_REG1 += REG_0b_REG3; /*(��]�ʁA���F)==(�������A�g�F)*/		/*(1024/(6*8))*/		/* �p�x(1024[360/360�x]�� 48����) */
	}
}
#endif

#if 0
sta tic const s8 step_tbl[(4+4)] =
{
	(1024/24),	/* easy */
	(1024/28),	/* normal */
	(1024/30),	/* hard */
	(1024/32),	/* lunatic */
//
	(24),	/* easy */
	(28),	/* normal */
	(30),	/* hard */
	(32),	/* lunatic */
};
#endif


#if 1

/*---------------------------------------------------------
	����̒ʏ�U�����ۂ��J�[�h
	-------------------------------------------------------
	normal? �g�N�i�C	24����?
	hard	�g�N�i�C	35����(36�����ł͖���)
	-------------------------------------------------------
	�g�p���W�X�^
//	REG_08_REG0 	�J�E���^�B
	REG_09_REG1 	[�萔1]�J�̑��x
	REG_0a_REG2 	[�萔2]�N�i�C���Ȃ���p�x(180/360���傢��])
	-------------------------------------------------------
	sincos�v�Z�p
	REG_0b_REG3 	angle_65536
	REG_0c_REG4 	sine   �̒l�p�B
	REG_0d_REG5 	cosine �̒l�p�B
---------------------------------------------------------*/

/*---------------------------------------------------------
	[�e���O���[�v(1)�Z�N�V����]
	-------------------------------------------------------
---------------------------------------------------------*/
#define meirin_danmaku_01_amefuri_callback common_danmaku_01_amefuri_callback

/*---------------------------------------------------------
	[�e���O���[�v(3)�Z�N�V�����A���C���[(3)�e]
	-------------------------------------------------------
	�ԐN�i�C�p��
---------------------------------------------------------*/
local OBJ_CALL_FUNC(meirin_danmaku_03_aka_ao_kunai_time256_callback)
{
	// [128] �_������
	/*(���e�����4[�b]�o�߂����e�́A�ʏ�e�֕ϐg����)*/
	if ((HATUDAN_ITI_NO_JIKAN-(256)) > src->jyumyou/*(�����Ȃ̂Ŏ����Ō���)*/)/* ���e�G�t�F�N�g�ォ��[256]�J�E���g�ȏ�o�߂����e */
	{
		/* (�ʏ�e�֕ϐg����) */
		src->hatudan_register_spec_data = (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
	}
//	danmaku_00_standard_angle_mover(src);/*(�p�x�e�ړ�+��ʊO�e����)*/
	hatudan_system_B_move_angle_001(src);/*(�p�x�e�ړ�)*/
}
/*---------------------------------------------------------
	[�e���O���[�v(2)�Z�N�V�����A���C���[(2)�e]
	-------------------------------------------------------
	�ԐN�i�C�p��
---------------------------------------------------------*/
local OBJ_CALL_FUNC(meirin_danmaku_02_aka_ao_kunai_callback)
{
	if ((HATUDAN_ITI_NO_JIKAN-64) == src->jyumyou)/* ���e�G�t�F�N�g�ォ��64�J�E���g�o�߂����e */
	{
		src->hatudan_register_speed65536	= t256(1.0);	/* ����(�ł��o�����x) */
		src->hatudan_register_tra65536		= t256(6.0);	/* ���������e */
		src->tmp_angleCCW1024				= (REG_0a_REG2);	/* [�萔2]�ԐN�i�C���Ȃ���p�x */
		if (src->hatudan_register_spec_data & TAMA_SPEC_AKA_AO_KUNAI_BIT)
		{
			src->tmp_angleCCW1024			= (-(src->tmp_angleCCW1024));
		}
		src->rotationCCW1024				+= (src->tmp_angleCCW1024);
		mask1024(src->rotationCCW1024);
		/* (�ԐN�i�C�A���C���[(3)�e�֕ϐg����) */
		src->hatudan_register_spec_data = (DANMAKU_LAYER_03)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
	}
//	danmaku_00_standard_angle_mover(src);/*(�p�x�e�ړ�+��ʊO�e����)*/
	hatudan_system_B_move_angle_001(src);/*(�p�x�e�ړ�)*/
}
	#if (0)
	/* ���e�� 512 �J�E���g�ȏ�͒e�������B */
	if ((HATUDAN_ITI_NO_JIKAN-512) > src->jyumyou/*(�����Ȃ̂Ŏ����Ō���)*/)/* ���e�G�t�F�N�g�ォ��[512]�J�E���g�ȏ�o�߂����e */
	{
		/* (�ʏ�e�֕ϐg����) */
		src->hatudan_register_spec_data = (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
	}
	#endif
//		const int ao_aka_tbl[(2)]	=
//		{
//		//	 (1024/2)+(1024/32),/* 180/360���傢��] */ 	/* �� */
//		//	-(1024/2)-(1024/32),/* 180/360���傢��] */ 	/* �� */
//			 (1024/2)+(1024/24),/* 180/360���傢��] */ 	/* �� */
//			-(1024/2)-(1024/24),/* 180/360���傢��] */ 	/* �� */
//		};
	//	h->rotationCCW1024				+= (1024/2)+(1024/16);/* 180/360���傢��] */
	//	h->rotationCCW1024				+= (1024/2)+(1024/32);/* 180/360���傢��] */

/*---------------------------------------------------------
	[�������Z�N�V����]
	-------------------------------------------------------
---------------------------------------------------------*/
#define spell_init_12_meirin_magaru_kunai spell_init_12_common_amefuri

/*---------------------------------------------------------
	[���e�Z�N�V����]
	-------------------------------------------------------
---------------------------------------------------------*/

local OBJ_CALL_FUNC(spell_create_12_meirin_magaru_kunai)
{
	#if 1/*(�f�o�b�O�p)*/
//		REG_03_DEST_Y	 = t256(256);
	#endif
	/* �J */
	if (0x30>((REG_10_BOSS_SPELL_TIMER)&0xff))/* (256��� ??��) */
	{
		spell_create_common_amefuri(src);
	}
/*
time
f	1111	��1 			[���e]
e	1110
d	1101			��1 	[���e]
c	1100
b	1011	��2 			[�ϐg]
a	1010
9	1001			��2 	[�ϐg]
8	1000
7
...
0
*/
//	REG_0a_REG2 = (REG_10_BOSS_SPELL_TIMER);/*(�Ƃ肠����)*/
	/* [���e] */
	if (0xd0==((REG_10_BOSS_SPELL_TIMER)&0xdf))/* (256���2��) */
	{
		unsigned int jj;
		for (jj=0; jj<(6); jj++)	/*(6�A)*/
		{
			HATSUDAN_01_speed256			= (t256(4.0)-(jj<<6));				/* �e�� */
			HATSUDAN_02_speed_offset		= -t256(5); 						/* (-5) (-3)���������e */	/* ���̕����ɂȂ邩������ */
			HATSUDAN_03_angle65536			= (0/65536);						/* ������ */
			u8 check_00type 				= ((REG_10_BOSS_SPELL_TIMER>>5)&1); /* �ԐN�i�C�e +(0&1)*/
			HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_02)|((check_00type)<<8)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
			HATSUDAN_05_bullet_obj_type 	= (BULLET_KUNAI12_BASE + TAMA_IRO_01_AKA)+(check_00type+check_00type);
			HATSUDAN_06_n_way				= (24);//step_tbl[(difficulty+4)];/*(32)*/
			HATSUDAN_07_div_angle65536		= (65536/24);				/* �����p�x([360/360]�x��24����) */ 	//step_tbl[(difficulty)];/*(32)*/
			hatudan_system_regist_katayori_n_way();/* (r33-) */
		}
		#if (1)
		cpu_bullet_play_15_auto();
		#endif
	}
}
#endif

/*---------------------------------------------------------
	�����u�ؑz�����v
	-------------------------------------------------------
	�����u�ؑz�����v�́A�ꌩ�΂�΂�ɔz�u���Ă��镗�Ɍ����邪�A
	���e�ʒu���悭����ƁA�����ł͂Ȃ����^(5����)�ɔz�u���Ă���B
---------------------------------------------------------*/

/*---------------------------------------------------------
	�����u�ʓ��̕���v(normal)
	-------------------------------------------------------
	���������ǂ�������Ȃ����A
	��������56�����`64�������x�炵���B
	������ 7 �e���W���Ă��镔�����A�S�̂�(1/8)�����̗̈�Ɏ��܂��Ă��鎖�B
	(1/8)�̗̈�� 7 �e���W���Ă���̂�����A�Œ�ł�
	7 x 8 �� 56�����́A�K�v�ł���B
	-------------------------------------------------------
	�e���̕������� 12�Ŋ���؂��ꍇ������B
	8 x 8 �� 64�����Ȃ̂ŁA����ɋ߂��l�ŁA12 �Ŋ���؂����̂́A60�B
	������A60����(5x12)�̉\���͂���B
	-------------------------------------------------------
	�Ƃ��낪���ۂɒ��ׂČ������A60 ������64 �����͐F�̊Ԋu����A�ǂ��撣���Ă����낪����Ȃ��B
	-------------------------------------------------------
	64 ���� (8 x 8) 		  (���덇��Ȃ��B���e�̊Ԋu����������B���̔g�܂ł̊Ԋu���L������B)
	60 ���� (7 x 4 + 8 x 4)   (���덇��Ȃ��B���e�̊Ԋu����������B���̔g�܂ł̊Ԋu���L������B)
	-------------------------------------------------------
	56 ���� (7 x 8) 		  (���덇���B)
	-------------------------------------------------------
	�������́A�͕핗�̃V�X�e����CCW(�����v���)�Ȃ̂�CCW�Ƃ���B
	-------------------------------------------------------
	�n�߂ɔ���́A���v���Ō����Ă���(�r�����甽���v���ɂȂ�����߂����肷��)
---------------------------------------------------------
[]�̕����͌��B
  �̕����͌����Ȃ����A���Ɍ��Ƃ����炻�̐F�ɂȂ锤�B
<>�̕����͓���e�ŁA�p�x����v���Ȃ��B�F���Ⴄ�B
---------------------------------------------------------
	����56�����Ƃ��ĐF���l���Ă݂�B
�ʒu[00/56][���F]�ʒu[14/56] ���F �ʒu[28/56][���F]�ʒu[42/56]<��3A>/
�ʒu[01/56][�F]�ʒu[15/56] �F �ʒu[29/56] �F �ʒu[43/56]<��3B>/
�ʒu[02/56][���F]�ʒu[16/56][���F]�ʒu[30/56][���F]�ʒu[44/56][���F]/
�ʒu[03/56][�ΐF]�ʒu[17/56] �ΐF �ʒu[31/56][�ΐF]�ʒu[45/56] �ΐF /
�ʒu[04/56][���F]�ʒu[18/56][���F]�ʒu[32/56][���F]�ʒu[46/56][���F]/
�ʒu[05/56][��F]�ʒu[29/56]<��1A>�ʒu[33/56] ��F �ʒu[47/56] ��F /
�ʒu[06/56][�ԐF]�ʒu[20/56]<��1B>�ʒu[34/56][�ԐF]�ʒu[48/56] �ԐF /
---------------------------------------------------------------------
�ʒu[07/56] �Ȃ� �ʒu[21/56] �Ȃ� �ʒu[35/56] �Ȃ� �ʒu[49/56] �Ȃ� /
�ʒu[08/56] �Ȃ� �ʒu[22/56] �Ȃ� �ʒu[36/56] �Ȃ� �ʒu[50/56] �Ȃ� /
�ʒu[09/56] �Ȃ� �ʒu[23/56] �Ȃ� �ʒu[37/56] �Ȃ� �ʒu[51/56] �Ȃ� /
�ʒu[10/56] �Ȃ� �ʒu[24/56][��F]�ʒu[38/56][���Q]�ʒu[52/56] �Ȃ� /
�ʒu[11/56] �Ȃ� �ʒu[25/56] �Ȃ� �ʒu[39/56] �Ȃ� �ʒu[53/56] �Ȃ� /
�ʒu[12/56] �Ȃ� �ʒu[26/56] �Ȃ� �ʒu[40/56] �Ȃ� �ʒu[54/56] �Ȃ� /
�ʒu[13/56] �Ȃ� �ʒu[27/56] �Ȃ� �ʒu[41/56] �Ȃ� �ʒu[55/56] �Ȃ� /
---------------------------------------------------------
<���P>	�F����eA�B�p�x���኱����Ă���B<��1A>��<��1B>�̒��Ԃ��炢�B
[���Q]	�F����eB�B���̒e�͊p�x������Ă��Ȃ��B
<���R>	��F����eC�B�p�x���኱����Ă���B<��3A>��<��3B>�̒��Ԃ��炢�B
---------------------------------------------------------
����e�̊p�x�̂���́A�ǂꂭ�炢����Ă���̂��ǂ�������Ȃ��B
���ꂪ�������̈Ⴂ(����Ă����64�����Ƃ�)���炫�Ă���\��������B
---------------------------------------------------------
����𒆊Ԉʒu�Ƃ����ꍇ�ɍl���Ă݂�B
	7 x 8 x 2 �� 112�����B
---------------------------------------------------------
	����112�����Ƃ��ĐF���l���Ă݂�B
�ʒu[000/112][���F]�ʒu[028/112] ���F �ʒu[056/112][���F]�ʒu[084/112] �Ȃ� /
�ʒu[001/112] �Ȃ� �ʒu[029/112] �Ȃ� �ʒu[057/112] �Ȃ� �ʒu[085/112]<��3 >/
�ʒu[002/112][�F]�ʒu[030/112] �F �ʒu[058/112] �F �ʒu[086/112] �Ȃ� /
�ʒu[003/112] �Ȃ� �ʒu[031/112] �Ȃ� �ʒu[059/112] �Ȃ� �ʒu[087/112] �Ȃ� /
�ʒu[004/112][���F]�ʒu[032/112][���F]�ʒu[060/112][���F]�ʒu[088/112][���F]/
�ʒu[005/112] �Ȃ� �ʒu[033/112] �Ȃ� �ʒu[061/112] �Ȃ� �ʒu[089/112] �Ȃ� /
�ʒu[006/112][�ΐF]�ʒu[034/112] �ΐF �ʒu[062/112][�ΐF]�ʒu[090/112] �ΐF /
�ʒu[007/112] �Ȃ� �ʒu[035/112] �Ȃ� �ʒu[063/112] �Ȃ� �ʒu[091/112] �Ȃ� /
�ʒu[008/112][���F]�ʒu[036/112][���F]�ʒu[064/112][���F]�ʒu[092/112][���F]/
�ʒu[009/112] �Ȃ� �ʒu[037/112] �Ȃ� �ʒu[065/112] �Ȃ� �ʒu[093/112] �Ȃ� /
�ʒu[010/112][��F]�ʒu[038/112] �Ȃ� �ʒu[066/112] ��F �ʒu[094/112] ��F /
�ʒu[011/112] �Ȃ� �ʒu[039/112]<��1 >�ʒu[067/112] �Ȃ� �ʒu[095/112] �Ȃ� /
�ʒu[012/112][�ԐF]�ʒu[040/112] �Ȃ� �ʒu[068/112][�ԐF]�ʒu[096/112] �ԐF /
�ʒu[013/112] �Ȃ� �ʒu[041/112] �Ȃ� �ʒu[069/112] �Ȃ� �ʒu[097/112] �Ȃ� /
---------------------------------------------------------------------
�ʒu[014/112] �Ȃ� �ʒu[042/112] �Ȃ� �ʒu[070/112] �Ȃ� �ʒu[098/112] �Ȃ� /
�ʒu[015/112] �Ȃ� �ʒu[043/112] �Ȃ� �ʒu[071/112] �Ȃ� �ʒu[099/112] �Ȃ� /
�ʒu[016/112] �Ȃ� �ʒu[044/112] �Ȃ� �ʒu[072/112] �Ȃ� �ʒu[100/112] �Ȃ� /
�ʒu[017/112] �Ȃ� �ʒu[045/112] �Ȃ� �ʒu[073/112] �Ȃ� �ʒu[101/112] �Ȃ� /
�ʒu[018/112] �Ȃ� �ʒu[046/112] �Ȃ� �ʒu[074/112] �Ȃ� �ʒu[102/112] �Ȃ� /
�ʒu[019/112] �Ȃ� �ʒu[047/112] �Ȃ� �ʒu[075/112] �Ȃ� �ʒu[103/112] �Ȃ� /
�ʒu[020/112] �Ȃ� �ʒu[048/112][��F]�ʒu[076/112][���Q]�ʒu[104/112] �Ȃ� /
�ʒu[021/112] �Ȃ� �ʒu[049/112] �Ȃ� �ʒu[077/112] �Ȃ� �ʒu[105/112] �Ȃ� /
�ʒu[022/112] �Ȃ� �ʒu[050/112] �Ȃ� �ʒu[078/112] �Ȃ� �ʒu[106/112] �Ȃ� /
�ʒu[023/112] �Ȃ� �ʒu[051/112] �Ȃ� �ʒu[079/112] �Ȃ� �ʒu[107/112] �Ȃ� /
�ʒu[024/112] �Ȃ� �ʒu[052/112] �Ȃ� �ʒu[080/112] �Ȃ� �ʒu[108/112] �Ȃ� /
�ʒu[025/112] �Ȃ� �ʒu[053/112] �Ȃ� �ʒu[081/112] �Ȃ� �ʒu[109/112] �Ȃ� /
�ʒu[026/112] �Ȃ� �ʒu[054/112] �Ȃ� �ʒu[082/112] �Ȃ� �ʒu[110/112] �Ȃ� /
�ʒu[027/112] �Ȃ� �ʒu[055/112] �Ȃ� �ʒu[083/112] �Ȃ� �ʒu[111/112] �Ȃ� /
---------------------------------------------------------
���ǁA����e�͂S�B�ŕʏ����Ɛ����ł���B
	<��1 > �e�ԍ�40 (�H)
	[��F] �e�ԍ�49 (�n�I�ꂵ�ނ���H)
	[���Q] �e�ԍ�77 (���b�L�|7����H)
	<��3 > �e�ԍ�86 (pc-98x1��cpu�^��i80x86����H�i���R�V�X�e��86����H�Ԃ̃��r��AE86����H)
���Ǝv����B
---------------------------------------------------------*/