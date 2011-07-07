
/*---------------------------------------------------------
 �����͕핗 �` Toho Imitation Style.
  �v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���̒e���𐶐����܂��B
---------------------------------------------------------*/


/*---------------------------------------------------------
	�g5�ʒ�-�{�X ��� ���U���ɂ�����Ƃ��������e��
	-------------------------------------------------------
	���̒e���̖{���́A
	�P�D��r�I����(�Ƃ����������x)�̐i�C�t�e�̕ǂ�������Ȃ���A
	�ُ�ɒx�����������Ȃ̂Œ������܂��Ĕ�����ԃN�i�C�e������鎖�B
	�Q�D�i�C�t�e�ƐԃN�i�C�e�̑��x���Ⴄ�̂ŁA�d�Ȃ��Ĕ�����ꍇ���������鎖�B
	�R�D�������Ȃ���A�d�Ȃ�͂�����x�̎��Ԃ��ώ@���Ă���Δ�������B
	�S�D(�R�D)�ɖ������āA�ώ@���Ԃ��Z��(���������ꍇ���K����������)�Ɖ^�������Ɣ������Ȃ��B
	-------------------------------------------------------
	�� �i�C�t
		�i�C�t�͋Ȃ������ǂɂȂ�Ȃ��Ɩ��͖͂����B
		��r�I�������x�����A���߂���Ɩ��͖͂����B�����ɂ��炢�炷��x�����K�v�B
	�� �ԃN�i�C
		�ԃN�i�C�ɂ͂��炢�炷��x�����K�v�B
		��ʉ��Ŕ����Ă����ꍇ�ɁA�u��ʉ�������ł̂݋}���ɑ��x���ω�����v�B
		�������������x�Œ�������K�v����B
		�O�L�Q�|�S�͎�ɉ�ʉ�������Ŕ������錻�ہB
		��ʒ�������ł́A���̕��G�ɋ߂��̂Ŕ��ː_�o�͗v�邪�A�O�L�Q�|�S�̌��ۂ͔������ɂ����B
	-------------------------------------------------------
	�� �ԃN�i�C
		�Ȃ񂩒m��񂪌����蒼������u�����e�v�������B
		�u�����e�v�{�u�����e�v���Ď����Ȃ��H
	-------------------------------------------------------
	����ƈႢ�A����������ɍs���Ȃ�����A������͏o���Ȃ��B
	(psp�̏c�𑜓x�ł͏�ɍs���X�y�[�X���Ȃ�)
	��������psp�̏c�𑜓x���S�R����Ȃ�����A�{�ƕ��̃`���������͖����B
	������x���E�ɓ�������Ĕ����ĖႤ����z�肵�Ă���B(������e���x��)
	����ɕ����Ė����e�͂����Ă��񂺂�Ⴄ�B(�ʕϐ��Ƃ�̂��ʓ|��������������)
	-------------------------------------------------------
	�i�C�t�̒e���͂��ꂭ�炢�����H�ԃN�i�C�̒e���͑��߂���B
	�ԃN�i�C�e�̎����͌���͂����Ԃ���Ă���̂ŕς������񂾂��ǁA
	�u�X�y�J�V�X�e����d���v���Ȃ��Ə�肭�����Ȃ������H�B
	-------------------------------------------------------
	�u�X�y�J�V�X�e����d���v�F2��ނ̈قȂ����X�y�J�������Ɍ��Ă�X�y�J�V�X�e���B
	�����������̂ɂ���ׂ������Ȃ��ׂ����������B(�p�`�F)
	-------------------------------------------------------
	�܂��S(����)�Ȃ̂ŁA(�v���C���[�ɂ���ȏ���)����ł�����Ă͍���B
	-------------------------------------------------------
	�i�C�tspeed256	speed_offset	/	�ԃN�i�Cspeed256	speed_offset
	t256(2.5)			(0) 			/	(0) 				(4) 			// �ǂ��񂾂��ǖ{��������B
	t256(2.0)			(0) 			/	(0) 				(2) 			// �ԃN�i�C�̏����x�����邩�Ȃ��B// �i�C�t�ƐԃN�i�C�����ꂷ���B�{��������B
	t256(1.75)			(0) 			/	(0) 				(1)
---------------------------------------------------------*/

//#define ADJUST3916	((65536/20)+(640))
#define ADJUST3916	(4000)
#define ADJUST4444	(4200)
static void spell_create_01_sakuya_misogi_normal(SPRITE *src)
{
//	SPELL_TIME_0256 SPELL_TIME_0128
	static int aaa_angle65536;
	static int ao_knife_timer;			// �i�C�t�^�C�}�[
	static int aka_kunai_timer; 		// �ԃN�i�C�^�C�}�[
	if (127 == (src->boss_base_spell_time_out))/* �����l�Ƀ��Z�b�g�B */
	{
		#if 1
		/* �i�C�t: �E�㑤���猂���n�߂�B�E���Ȃ̂�90�x���炵�āAtama_system_regist_katayori_n_way()�̕␳���s���B */
	//	src->tmp_angleCCW65536	= (65536+(65536/4)-((65536*8)/20))+(ADJUST4444);				// �i�C�t
	//	src->tmp_angleCCW65536	= (((65536*2)/20))+(ADJUST4444);				// �i�C�t 	tama_system_regist_katayori_n_way();�ŉ�����ɁA���v����(2/20)���猂���n�߂�B
		src->tmp_angleCCW65536	= (((65536*1)/18))+(ADJUST4444);				// �i�C�t 	tama_system_regist_katayori_n_way();�ŉ�����ɁA���v����(2/20)���猂���n�߂�B
		// �{���͑ł��n�߂̐i�C�t�̒[���E��60�x���炢����ł��n�߁A
		// �ł��I���́A����60�x���炢�̈ʒu�ɂȂ�B
		#endif
		#if 1
		/* �ԃN�i�C: */
	//	aaa_angle65536			= (65536/2);		// �ԃN�i�C
		aaa_angle65536			= (65536/4)-(0);	// �ԃN�i�C (�E������)
		#endif
		ao_knife_timer = 0; 	// �i�C�t
//		aka_kunai_timer = 0;	// �ԃN�i�C
	}
//	if (DANMAKU_0096_TIME > (src->boss_base_spell_time_out))/* �b���������Ȃ��B */
	if (100 > (src->boss_base_spell_time_out))/* �b���������Ȃ��B */
	{
		ao_knife_timer--;
		if (0 >= ao_knife_timer)
	//	if (0==((src->boss_base_spell_time_out)&0x07))
		{
			obj_send1->cx256					= (src->cx256); 	/* �e��x256 �{�X���S���甭�e�B */
			obj_send1->cy256					= (src->cy256); 	/* �e��y256 �{�X���S���甭�e�B */
		//	ao_knife_timer = (12);
			ao_knife_timer = (8);
			/*	�i�C�t */
			#if (1)
		//	voice_play(VOICE13_SAKUYA_SHOT02, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE13_SAKUYA_SHOT02);
			#endif
		//	src->tmp_angleCCW1024 -= (int)(256/5)+(10); /*CCW*/
		//	src->tmp_angleCCW1024 -= (int)(1024/20)+(10); /*CCW*/
		//	src->tmp_angleCCW65536 -= (int)(65536/20)+(640); /*CCW*/
		//	src->tmp_angleCCW65536 -= (int)(65536/20)+(640)+(128); /*CCW*/	/* 3276== (65536/20) */
			src->tmp_angleCCW65536 -= (ADJUST4444); /*CCW*/
			mask65536(src->tmp_angleCCW65536);
		//	br.BULLET_REGIST_00_speed256				= (t256(2.5));					/* �e�� (t256(2.5)�Q�l�B�g�m�[�}��5��) */
		//	br.BULLET_REGIST_00_speed256				= (t256(2.0));					/* �e�� psp�͏c�𑜓x���Ȃ��̂łƂɂ����x�����Ȃ��ƁB */
//			br.BULLET_REGIST_00_speed256				= (t256(1.75)); 				/* �e�� psp�͏c�𑜓x���Ȃ��̂łƂɂ����x�����Ȃ��ƁB */
		//	br.BULLET_REGIST_00_speed256				= (t256(1.00)); 				/* �e�� psp�͏c�𑜓x���Ȃ��̂łƂɂ����x�����Ȃ��ƁB */
			br.BULLET_REGIST_00_speed256				= (t256(1.25)); 				/* �e�� psp�͏c�𑜓x���Ȃ��̂łƂɂ����x�����Ȃ��ƁB */
		//	br.BULLET_REGIST_01_speed_offset			= t256(0.0);/* �Ă��� */
		//	br.BULLET_REGIST_01_speed_offset			= t256(1.0);/* �������Ă݂�Ă��� */
		//	br.BULLET_REGIST_01_speed_offset			= t256(6.0);/* �������Ă݂�Ă��� */
			br.BULLET_REGIST_01_speed_offset			= t256(4.0);/* �������Ă݂�Ă��� */
			br.BULLET_REGIST_02_angle65536				= (src->tmp_angleCCW65536); 	/* �e���p�x65536 */ 		/* [������e] */
			br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)�W���e */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KNIFE20_04_AOI;		/* [�i�C�t�e] */
		//����br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)�W���e */
			br.BULLET_REGIST_06_n_way					= (8);							/* [8way] */
		//	br.BULLET_REGIST_07_VECTOR_div_angle1024	= (int)(256/5);
		//	br.BULLET_REGIST_07_VECTOR_div_angle1024	= (int)(1024/20);
		//	br.BULLET_REGIST_07_div_angle65536			= (int)(65536/20);				/* �����p�x([360/360]�x��20����) */
			br.BULLET_REGIST_07_div_angle65536			= (int)(65536/18);				/* �����p�x([360/360]�x��20����)[�ł��I��肪���΂ߏ�ɂȂ�] */
			tama_system_regist_katayori_n_way();/* (r33-) */

		//	if (0==((src->boss_base_spell_time_out)&0x1f))/* reset (1/32) */
		//	{
		//	//	aaa_angle1024	= (src->tmp_angleCCW1024)-(int)((1024/2));
		//		aaa_angle65536	= (src->tmp_angleCCW65536)-(int)((65536/2));
		//	}
		}
	}
	aka_kunai_timer--;
	if (0 > aka_kunai_timer)
	{
		aka_kunai_timer = 32/*64*/+(ra_nd()&0x3f);
	}
	else
//	/* �ԃN�i�C�̕����x���̂ŁA�i�C�t����Ɍ����n�߂鎖�ɂ���B */
//	if (DANMAKU_0032_TIME > (aka_kunai_timer))/* �b���������Ȃ��B */
//	if ( (41) > (aka_kunai_timer))/* �b���������Ȃ��B */
	if ( (41-16+((cg_game_difficulty)<<4)) > (aka_kunai_timer))/* �b���������Ȃ��B */
//	if ((128-40) < (aka_kunai_timer))/* �Ō�͎b���������Ȃ��B */
	{
	//	else/* �������̒e�͖{���͂���ďo�銴���ɂ������B */
//	//	if ((10-1)>((((u8)aka_kunai_timer)^0x20)&0x2f))/* x 10set */
//		if ((10-1)>(((aka_kunai_timer)	  )&0x0f))/* x 10set */
		/* 4��Ɉ��A�v10set */
		if ((0)==(((aka_kunai_timer)	  )&0x03))/* x 10set */
		{
			obj_send1->cx256					= (src->cx256); 	/* �e��x256 �{�X���S���甭�e�B */
			obj_send1->cy256					= (src->cy256); 	/* �e��y256 �{�X���S���甭�e�B */
			/* �������̐ԃN�i�C�͑S�R�_���D */
		//	aaa_angle1024 -= (int)(256/5);
		//	aaa_angle1024 -= (int)(1024/20);
			aaa_angle65536 -= (int)(65536/20);
		//	if (0 > (int)aaa_angle65536)	{	aaa_angle65536 = (65536/2); 	}
			mask65536(aaa_angle65536);
		//	br.BULLET_REGIST_00_speed256				= (t256(2.0));						/* �e�� ���߂��� */
		//	br.BULLET_REGIST_00_speed256				= (t256(1.0));						/* �e�� (�o�n�߂͒x��) */
		//	br.BULLET_REGIST_00_speed256				= (t256(0.5));						/* �e�� (�o�n�߂͒x��) */
		//	br.BULLET_REGIST_00_speed256				= (t256(0.25)); 					/* �e�� (�o�n�߂͒x��) */
		//	br.BULLET_REGIST_02_angle65536				= (((aaa_angle65536)+(65536*3/4))&(65536-1));	/* �e���p�x65536 */ 	/*��CCW*/
			br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)�W���e */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KUNAI12_01_AKA;			/* [�ԃN�i�C�e] */
		//����br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)�W���e */
			br.BULLET_REGIST_06_n_way					= (4);								/* [4way] */
			br.BULLET_REGIST_07_div_angle65536			= (int)(512-64);					/* [�����e]�̕����p�x(r32) */
//-------------------------
	// ��  [�����e]
		//	br.BULLET_REGIST_00_speed256				= (0);								/* �e�� (�o�n�߂͒x��) */
//			br.BULLET_REGIST_00_speed256				= (t256(0.25)); 					/* �e�� (�o�n�߂͒x��) */
		//	br.BULLET_REGIST_00_speed256				= (t256(3.25)); 					/* �e�� (�o�n�߂͒x��) */
		//	br.BULLET_REGIST_00_speed256				= (t256(3.00)); 					/* �e�� (�o�n�߂͒x��) */
			br.BULLET_REGIST_00_speed256				= (t256(2.00)); 					/* �e�� (�o�n�߂͒x��) */

		//	br.BULLET_REGIST_01_speed_offset			= t256(3/*0*/);/* �������Ă݂� �Ă��� */
//			br.BULLET_REGIST_01_speed_offset			= t256(2);/* �������H(�������e�Ȏ��͊ԈႢ�Ȃ��H) */
		//	br.BULLET_REGIST_01_speed_offset			= t256(1);/* �������H(�������e�Ȏ��͊ԈႢ�Ȃ��H) */
			br.BULLET_REGIST_01_speed_offset			= t256(-1);/* �����H(�������e�Ȏ��͊ԈႢ�Ȃ��H) */
			br.BULLET_REGIST_02_angle65536				= (aaa_angle65536); 	/* �e���p�x65536 */ 	/*��CCW*/
			tama_system_regist_katayori_n_way();/* (r33-) */
//-------------------------
	// ��  [�����e]
		//	br.BULLET_REGIST_00_speed256				= (t256(0.25)); 					/* �e�� (�o�n�߂͒x��) */
		//	br.BULLET_REGIST_00_speed256				= (t256(1.25)); 					/* �e�� (�o�n�߂͒x��) */
			br.BULLET_REGIST_00_speed256				= (t256(2.25)); 					/* �e�� (�o�n�߂͒x��) */
			br.BULLET_REGIST_01_speed_offset			= t256(-1);/* �����H(�������e�Ȏ��͊ԈႢ�Ȃ��H) */
			br.BULLET_REGIST_02_angle65536				= (aaa_angle65536); 	/* �e���p�x65536 */ 	/*��CCW*/
			tama_system_regist_katayori_n_way();/* (r33-) */
		}
	}
}


/*---------------------------------------------------------
	�g5�ʒ�-�{�X ��� ���U���ɂ�����Ƃ��������e��
	-------------------------------------------------------
	�͕핗�Ȃ񂾂��班���͕핗�Ȓe����������ƃe�X�g�őn���Ă݂��B
	��x�I�ɂ�psp-2000�Ȃ�A�gnormal5�ʂɒʏ�e���Ƃ��ďo�Ă����Ƃ��ē������炢���A
	������҂������Ȃ��Ċ����B�^�u���B
	�ȒP�ł��Ȃ����ǁA������Ȃ����ĕӂ��_���Ă݂��񂾂��ǁA��Փx�ǂ��������B
	psp-1000�̕����L�[�͎n�߂���lunatic�����A�i���O���ƈʒu������������邵�ȁB
	-------------------------------------------------------
	�݌v�����̂́A
	�ԃN�i�C�͂��܂ł���ʂɎc��̂Ō���(�߂�ǂ�����)�����B
	�i�C�t�͒ʏ�A�ԃi�C�t�͔��Ύ���B�i�C�t�͕K�����Ԋu�B
	�i�C�t�R��B��ʉ����ƃi�C�t��A�����Ɍ�������̂ő��x�����Ȋ����B
	�R��ɂ����͈̂Ӑ}�I�łQ������Ĕ��Α��ɐ؂�Ԃ��p�^�[��(���̎�����)������B
	(�Q�򂾂Ə��Ȃ����A�S�򂾂ƃ����Q���ۂ�)
	(�n�߂Č������͂R�򒼐��I�ɔ����邾�낤���A������Ɗ����Β����C���t�����A
	���������́u���̎������v�B�v�������Ԋu�������̂ŁA����邯�ǃ^�C�~���O���኱�V�r�A�B)
	-------------------------------------------------------
	lunatic�Ƃ����A����ȏ��Ŏ���Ŗ���Ă͍���̂Ŋ�{�I�ɂ������ǂ��B
	�ꌩ����������ǁA��̃i�C�t�Q�𔲂��邾��������A
	���Ƃ������ǂ��ňӊO�ƊȒP���ƁB(�^�u��)
	��{�I�ɖڐ�̃i�C�t�̑��x���x����āB�ԃN�i�C�ǂ����ĂȂ��Ƃ��ʂ�^�C�v�B
	�ԃN�i�C�����̓����_���Ȃ񂾂���A�l���Đ�ǂ݂��Ȃ��ƒǂ��l�߂���B
	�ǂ��l�߂�ꂽ�ꍇ�{�����Ă��ǂ����ǁA������x���Ȃ�i�C�t�Ɍ��Ԃ������̂Ō��Ԃ𔲂�����B
	�ǂ��l�߂��ă{����ꍇ�A�i�C�t�̑��x�������񂾂���A���R(���̒e���Ń{����ꍇ���)��������
	���Ȃ��ƂȂ�Ȃ���(��炢�{�����Ԃ��₽�璷���얲�͏���)�A�����Ɍ��ȃp�^�[���̐ԃN�i�C��
	���f�݂��Ă�̂��|�C���g�Ƃ����΃|�C���g���ȁH
	���Ȃ݂ɗH�X�q�Ŕ�����鑬�x�ɒ������Ă���܂��B
	�i�C�t���R�򒼐��I�ɔ�����p�^�[���ȊO�ɁA
	�i�C�t���Q������Ĕ��Α��ɐ؂�Ԃ��p�^�[��������̂�����A
	���̕ӁA�Ջ@���ςɐ؂�ւ���̂��R�c�Ƃ����΃R�c���ȁB
	(�i�C�t����W�O�U�O�ɂ����p�^�[���͈ꉞ�o���邯�ǁA���Ȃ����Ǝv����B
	���[��[�̂̕������ӂȐl�����邯�ǈ�ʓI(?)�ɂ͑哹�|���ۂ�����)
	(�i�C�t���P������Ĕ��Α��ɐ؂�Ԃ��p�^�[�������邯�ǁA�g����������B�m��Ȃ��Ă��������ȁB
	���̕����ƂŐԃN�i�C���o��̂�����A�ԃN�i�C�𑁂������ǂ݂��Ƃ��Ȃ��ƂȂ�Ȃ��B
	��ǂݎ��s�����ꍇ�A�W�O�U�O�ɓ�����̂͋Z�p�I�ɓ���̂�����A���̕��s���ɂȂ�B)
---------------------------------------------------------*/

static void spell_create_2d_sakuya_misogi_lunatic(SPRITE *src)
{
//	SPELL_TIME_0256 SPELL_TIME_0128
	static int aaa_angle65536;
//	static int ao_knife_timer;			// �i�C�t�^�C�}�[
	static int aka_kunai_timer; 		// �ԃN�i�C�^�C�}�[
	static int knife_kaiten;			// 0(�i�C�t) or 1(�ԃi�C�t)

	if (127 == (src->boss_base_spell_time_out))/* �����l�Ƀ��Z�b�g�B */
	{
		#if 1
		/* �i�C�t: �E�㑤���猂���n�߂�B�E���Ȃ̂�90�x���炵�āAtama_system_regist_katayori_n_way()�̕␳���s���B */
	//	src->tmp_angleCCW65536	= (65536+(65536/4)-((65536*8)/20))+(ADJUST3916);				// �i�C�t
	//	src->tmp_angleCCW65536	= (((65536*2)/20))+(ADJUST3916);				// �i�C�t 	tama_system_regist_katayori_n_way();�ŉ�����ɁA���v����(2/20)���猂���n�߂�B
		src->tmp_angleCCW65536	= (((65536*1)/18))+(ADJUST3916);				// �i�C�t 	tama_system_regist_katayori_n_way();�ŉ�����ɁA���v����(2/20)���猂���n�߂�B
		// �{���͑ł��n�߂̐i�C�t�̒[���E��60�x���炢����ł��n�߁A
		// �ł��I���́A����60�x���炢�̈ʒu�ɂȂ�B
		#endif
		#if 1
		/* �ԃN�i�C: */
	//	aaa_angle65536			= (65536/2);		// �ԃN�i�C
		aaa_angle65536			= (65536/4)-(0);		// �ԃN�i�C (�E������)
		#endif
//		ao_knife_timer = 0; 	// �i�C�t
//		aka_kunai_timer = 0;	// �ԃN�i�C
		knife_kaiten &= 1;/* �O�̂��߃}�X�N */
		knife_kaiten ^= 1;/* ���] */
	}
//	if (DANMAKU_0096_TIME > (src->boss_base_spell_time_out))/* �b���������Ȃ��B */
	if (48 > (src->boss_base_spell_time_out))/* �b���������Ȃ��B */
	{
//		ao_knife_timer--;
//		if (0 >= ao_knife_timer)
	//	if (0==((src->boss_base_spell_time_out)&0x07))
		{
		//	ao_knife_timer = (12);
//			ao_knife_timer = (8);
			/*	�i�C�t */
			#if (1)
		//	voice_play(VOICE13_SAKUYA_SHOT02, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE13_SAKUYA_SHOT02);
			#endif
			obj_send1->cx256					= (src->cx256); 	/* �e��x256 �{�X���S���甭�e�B */
			obj_send1->cy256					= (src->cy256); 	/* �e��y256 �{�X���S���甭�e�B */
			if (0==knife_kaiten)
			{	/* 0(�i�C�t) */
			//	src->tmp_angleCCW1024 -= (int)(256/5)+(10); /*CCW*/
			//	src->tmp_angleCCW1024 -= (int)(1024/20)+(10); /*CCW*/
			//	src->tmp_angleCCW65536 -= (int)(65536/20)+(640); /*CCW*/
			//	src->tmp_angleCCW65536 -= (int)(65536/20)+(640)+(128); /*CCW*/	/* 3276== (65536/20) */
				src->tmp_angleCCW65536 -= (ADJUST3916); /*CCW*/
			}
			else	/* 1(�ԃi�C�t) */
			{
				src->tmp_angleCCW65536 += (ADJUST3916); /*CCW*/
			}
			mask65536(src->tmp_angleCCW65536);
			br.BULLET_REGIST_02_angle65536				= (src->tmp_angleCCW65536);		/* �e���p�x65536 */ 		/* [������e] */
			//
		//	br.BULLET_REGIST_00_speed256				= (t256(2.5));						/* �e�� (t256(2.5)�Q�l�B�g�m�[�}��5��) */
		//	br.BULLET_REGIST_00_speed256				= (t256(2.0));						/* �e�� psp�͏c�𑜓x���Ȃ��̂łƂɂ����x�����Ȃ��ƁB */
//			br.BULLET_REGIST_00_speed256				= (t256(1.75)); 					/* �e�� psp�͏c�𑜓x���Ȃ��̂łƂɂ����x�����Ȃ��ƁB */
			br.BULLET_REGIST_00_speed256				= (t256(1.00)); 					/* �e�� psp�͏c�𑜓x���Ȃ��̂łƂɂ����x�����Ȃ��ƁB */
			br.BULLET_REGIST_01_speed_offset			= t256(6);/*�������Ă݂�Ă���*/
			br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)�W���e */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KNIFE20_04_AOI+knife_kaiten;			/* [�i�C�t�e] */
		//����br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)�W���e */
			br.BULLET_REGIST_06_n_way					= (8);								/* [8way] */
		//	br.BULLET_REGIST_07_VECTOR_div_angle1024	= (int)(256/5);
		//	br.BULLET_REGIST_07_VECTOR_div_angle1024	= (int)(1024/20);
		//	br.BULLET_REGIST_07_div_angle65536			= (int)(65536/20);					/* �����p�x([360/360]�x��20����) */
			br.BULLET_REGIST_07_div_angle65536			= (int)(65536/18);					/* �����p�x([360/360]�x��20����)[�ł��I��肪���΂ߏ�ɂȂ�] */
			tama_system_regist_katayori_n_way();/* (r33-) */

		//	if (0==((src->boss_base_spell_time_out)&0x1f))/* reset (1/32) */
		//	{
		//	//	aaa_angle1024	= (src->tmp_angleCCW1024)-(int)((1024/2));
		//		aaa_angle65536	= (src->tmp_angleCCW65536)-(int)((65536/2));
		//	}
		}
	}
	aka_kunai_timer--;
	if (0 > aka_kunai_timer)
	{
		aka_kunai_timer = 64+(ra_nd()&0x3f);
	}
	else
//	/* �ԃN�i�C�̕����x���̂ŁA�i�C�t����Ɍ����n�߂鎖�ɂ���B */
//	if (DANMAKU_0032_TIME > (aka_kunai_timer))/* �b���������Ȃ��B */
	if (41 > (aka_kunai_timer))/* �b���������Ȃ��B */
//	if ((128-40) < (aka_kunai_timer))/* �Ō�͎b���������Ȃ��B */
	{
	//	else/* �������̒e�͖{���͂���ďo�銴���ɂ������B */
//	//	if ((10-1)>((((u8)aka_kunai_timer)^0x20)&0x2f))/* x 10set */
//		if ((10-1)>(((aka_kunai_timer)	  )&0x0f))/* x 10set */
		/* 4��Ɉ��A�v10set */
		if ((0)==(((aka_kunai_timer)	  )&0x03))/* x 10set */
		{
			obj_send1->cx256					= (src->cx256); 	/* �e��x256 �{�X���S���甭�e�B */
			obj_send1->cy256					= (src->cy256); 	/* �e��y256 �{�X���S���甭�e�B */
			/* �������͐ԃN�i�C */
		//	aaa_angle1024 -= (int)(256/5);
		//	aaa_angle1024 -= (int)(1024/20);
			aaa_angle65536 -= (int)(65536/20);
		//	if (0 > (int)aaa_angle65536)	{	aaa_angle65536 = (65536/2); 	}
			mask65536(aaa_angle65536);
		//	br.BULLET_REGIST_02_angle65536				= (((aaa_angle65536)+(65536*3/4))&(65536-1));	/* �e���p�x65536 */ 	/*��CCW*/
		//	br.BULLET_REGIST_02_angle65536				= (((aaa_angle65536)+(65536*3/4))&(65536-1));	/* �e���p�x65536 */ 	/*��CCW*/
			br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)�W���e */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KUNAI12_01_AKA;			/* [�ԃN�i�C�e] */
		//����br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)�W���e */
			br.BULLET_REGIST_06_n_way					= (4);								/* [4way] */
			br.BULLET_REGIST_07_div_angle65536			= (int)(512-64);					/* [�����e]�̕����p�x(r32) */
//-------------------------
	// ��  [�����e]
			br.BULLET_REGIST_00_speed256				= (t256(2.00)); 							/* �e�� (�o�n�߂͒x��) */
			br.BULLET_REGIST_01_speed_offset			= t256(-1);/* �����H(�������e�Ȏ��͊ԈႢ�Ȃ��H) */
			br.BULLET_REGIST_02_angle65536				= (aaa_angle65536); 	/* �e���p�x65536 */ 	/*��CCW*/
			tama_system_regist_katayori_n_way();/* (r33-) */
//-------------------------
	// ��  [�����e]
			br.BULLET_REGIST_00_speed256				= (t256(2.25)); 							/* �e�� (�o�n�߂͒x��) */
			br.BULLET_REGIST_01_speed_offset			= t256(-1);/* �����H(�������e�Ȏ��͊ԈႢ�Ȃ��H) */
			br.BULLET_REGIST_02_angle65536				= (aaa_angle65536); 	/* �e���p�x65536 */ 	/*��CCW*/
			tama_system_regist_katayori_n_way();/* (r33-) */
		}
	}
}


/*---------------------------------------------------------
	�g5�ʒ�-�{�X ���u��p�F�~�X�f�B���N�V�����v�ɂ�����Ƃ��������e��
	-------------------------------------------------------
	Lunatic �ԃN�i�C�e 19way
	-------------------------------------------------------

---------------------------------------------------------*/
/*
------------------
256
	�ԃN�i�C
208 208==	64+144
	�Ȃɂ����Ȃ�
112 112==	64+48
	�i�C�t
64	64==	64+0
------------
64	64==	((64+256)-256)
	�Ȃɂ����Ȃ�
48	48==	((64+240)-256)
	�i�C�t�̉������B
0	0== 	((64+192)-256)
	���]�ړ�
0	0== 	((64+192)-256)
*/
static void spell_create_22_sakuya_miss_direction(SPRITE *src)
{
	if (/*256-64-48*/(64+144)< src->boss_base_spell_time_out)
	{
		/* �ԃN�i�C�e */
		if (0==((src->boss_base_spell_time_out)&0x03))
		{
			#if (1)
		//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
			#endif
			//	br.BULLET_REGIST_00_speed256				= (t256(2.0)+((src->boss_base_spell_time_out)<<2)); /* �e�� */
				br.BULLET_REGIST_00_speed256				= (t256(2.0)-((src->boss_base_spell_time_out)));	/* �e�� */
				br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)�W���e */
				br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KUNAI12_01_AKA;			/* [�ԃN�i�C�e] */
			//����br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)�W���e */
				br.BULLET_REGIST_06_n_way					= (24); 							/* [24way] */
			//	Lunatic �ԃN�i�C�e 19way
				br.BULLET_REGIST_07_div_angle65536			= (int)(65536/24);					/* �����p�x */
			// lunatic ��[19way]
		//
			unsigned int ii;
			for (ii=0; ii<((cg_game_difficulty)+1); ii++)
			{
				obj_send1->cx256							= (src->cx256)+(ii<<10);	/* �e��x256 */
				obj_send1->cy256							= (src->cy256); 			/* �e��y256 �{�X���S���甭�e�B */
			//
				br.BULLET_REGIST_01_speed_offset			= t256(1)+(ii<<8);/*(�e�X�g)*/
				SPRITE *zzz_player;
				zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
				tmp_angleCCW65536_src_nerai(zzz_player, obj_send1);/* ���@�_���p�쐬 */
				br.BULLET_REGIST_02_angle65536				= ((obj_send1->tmp_angleCCW65536)); 	/* �e���p�x65536 */ 	/* ���@�_���e */
				tama_system_regist_katayori_n_way();/* (r33-) */
			}
		}
	}
	else
//	if (256-192< src->boss_base_spell_time_out)
	if ((64+48)< src->boss_base_spell_time_out)
	{;}/* �Ȃɂ����Ȃ� */
	else
//	if (256-192< src->boss_base_spell_time_out)
	if ((64+0)< src->boss_base_spell_time_out)
	{
		/* �i�C�t */
		if (0==((src->boss_base_spell_time_out)&0x0f))
		{
			#if (0)
		//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);/* �i�C�t�̉��͂��炷�K�v������ */
			#endif
			set_sub_jikinerai(src);/* 1:���������쐬������ۂ��B */
				int int_aaa									= ((src->tmp_angleCCW65536));		/* �e���p�x1024 */
				br.BULLET_REGIST_01_speed_offset			= t256(0);/*(�e�X�g)*/
				br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)�W���e */
				br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KNIFE20_04_AOI;			/* [�i�C�t�e] */
			//����br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)�W���e */
			int jj;
			for (jj=(0); jj<(3); jj++)
			{
				// �e1 �e2  1.0 (1.5 2.0)
				br.BULLET_REGIST_00_speed256				= (t256(1.0)+(jj<<7));						/* �e�� */
				br.BULLET_REGIST_02_angle65536				= (int_aaa);		/* �e���p�x1024 */
				br.BULLET_REGIST_06_n_way					= (11); 							/* [11way] */
				br.BULLET_REGIST_07_div_angle65536			= (int)(65536/(7*4));				/* �����p�x([90/360]�x��7����==28����) */
				tama_system_regist_n_way();/* (r33-) */
			}
		}
	}
	else
	if (/*256-64*/(48/*(64+240)-256*/) < src->boss_base_spell_time_out)
	{;}/* �Ȃɂ����Ȃ� */
	else
	//if (/*256-64*/(0/*(64+192)-256*/) < src->boss_base_spell_time_out)
	{
		/* �i�C�t�̉�����(3��) */
		if (0==((src->boss_base_spell_time_out)&0x0f))
		{
			#if (1)
		//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);/* �i�C�t�̉��͂��炷�K�v������ */
			#endif
		}
		;
	}
}


/*---------------------------------------------------------
	�g5�ʒ�-�{�X ���u��p�F�~�X�f�B���N�V����(1/2)�v�ɂ�����Ƃ��������e��
	-------------------------------------------------------
	���@�_���A�S����24�����e
	-------------------------------------------------------
	[���F����bu llet_create_n_way_dan_sa_type()�̎��@�_���̓��U�g�_��Ȃ���e�Ȃ̂ŁA�����Ȃ���Γ�����܂��񂪁A
	���̂��������͋����e�ɂ���\��Ȃ̂ŁA�����Ȃ���Γ�����悤�ɂȂ�\��ł�]
---------------------------------------------------------*/
//SPELL_TIME_0048
static void spell_create_02_24nerai(SPRITE *src)
{
	if (0==((src->boss_base_spell_time_out)&0x03))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
		set_sub_jikinerai(src);/* 1:���������쐬������ۂ��B */
	//	br.BULLET_REGIST_00_speed256				= (t256(2.0)+((src->boss_base_spell_time_out)<<2)); /* �e�� */
		br.BULLET_REGIST_00_speed256				= (t256(2.0)-((src->boss_base_spell_time_out)));	/* �e�� */
		br.BULLET_REGIST_01_speed_offset			= t256(1);/*(�e�X�g)*/
		br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)�W���e */
		br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KUNAI12_01_AKA;			/* [�ԃN�i�C�e] */
	//����br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)�W���e */
		br.BULLET_REGIST_06_n_way					= (24); 							/* [24way] */
		br.BULLET_REGIST_07_div_angle65536			= (int)(65536/24);					/* �����p�x */
		tama_system_regist_katayori_n_way();/* (r33-) */
	}
}


/*---------------------------------------------------------
	�g5�ʒ�-�{�X ��� �u��p�F�~�X�f�B���N�V����(2/2)�v�ɂ�����Ƃ��������e��
	-------------------------------------------------------
	���@�_���A11�e(���S�ƁA���E��5�e���_��)
	-------------------------------------------------------
	[���F����bu llet_create_n_way_dan_sa_type()�̎��@�_���̓��U�g�_��Ȃ���e�Ȃ̂ŁA�����Ȃ���Γ�����܂��񂪁A
	���̂��������͋����e�ɂ���\��Ȃ̂ŁA�����Ȃ���Γ�����悤�ɂȂ�\��ł�]
	-------------------------------------------------------
	������Ȃ�������A�����͂R�񌂂B
---------------------------------------------------------*/
//SPELL_TIME_0064
static void spell_create_03_11nife(SPRITE *src)
{
	if (0==((src->boss_base_spell_time_out)&0x07))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
		set_sub_jikinerai(src);/* 1:���������쐬������ۂ��B */
		br.BULLET_REGIST_00_speed256				= (t256(2.0));						/* �e�� */
		br.BULLET_REGIST_01_speed_offset			= t256(0);/*(�e�X�g)*/
		br.BULLET_REGIST_02_angle65536				= ((src->tmp_angleCCW65536))-(int)(65536*6/(7*4));		/* �e���p�x1024 */
		br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)�W���e */
		br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KNIFE20_04_AOI;			/* [�i�C�t�e] */
	//����br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)�W���e */
		br.BULLET_REGIST_06_n_way					= (11); 							/* [11way] */
		br.BULLET_REGIST_07_div_angle65536			= (int)(65536/(7*4));				/* �����p�x([90/360]�x��7����==28����) */
		tama_system_regist_katayori_n_way();/* (r33-) */
	}
}


/*---------------------------------------------------------
	�g5�ʃ{�X ��� �u�ʏ�U��1(1/2)�v�ɂ�����Ƃ��������e��(�\��)
	-------------------------------------------------------
	24way =(�E12way)+(��12way)
	-------------------------------------------------------
---------------------------------------------------------*/
static void spell_create_04_pink_hearts(SPRITE *src)
{
	if (0==((src->boss_base_spell_time_out)&0x07))/* �{���� 8 �e(128==0x80==spell_time_out) */
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
	//	bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		bullet_play_04_auto(VOICE13_SAKUYA_SHOT02);
		#endif
			br.BULLET_REGIST_01_speed_offset			= t256(1/*0*/);/*(�e�X�g)*/
			br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)�W���e */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KNIFE20_06_YUKARI; 		/* [�s���N�i�C�t�e] */
		//����br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)�W���e */
			obj_send1->cy256							= (src->cy256); 	/* �e��y256 �{�X���S���甭�e�B */
		//
		int jj;
		jj = 0;
		int ii;
		for (ii=0; ii<(13*12*4)/*144*/; ii+= (13*4) )/*12����*/ 						/* �p�x(128[(90+a)/360�x]�� 10 ����) */
		{
			br.BULLET_REGIST_00_speed256				= (((160*4)-ii));						/* �e�� */
			//
			obj_send1->cx256							= (src->cx256) + t256(4.0); 			/* �e��x256 �{�X�E�����甭�e�B */
			br.BULLET_REGIST_02_angle65536				= (jj/*(0+i+i)&(65536-1)*/);
			tama_system_regist_single();/* (r33-) */
			//
			obj_send1->cx256							= (src->cx256) - t256(4.0); 			/* �e��x256 �{�X�������甭�e�B */
			br.BULLET_REGIST_02_angle65536				= ((0-jj)&(65536-1));
			tama_system_regist_single();/* (r33-) */
			//
			jj += (13*2*64);
		}
	}
}


/*---------------------------------------------------------
	�g5�ʃ{�X ��� �u�ʏ�U��1(2/2)�v�ɂ�����Ƃ��������e��(�\��)
	-------------------------------------------------------
	32�����ɕ����A�����͒ᑬ�e�A��͍����e�B
	16������2��o�^����B
	-------------------------------------------------------
	32����	�p�x(1024[360/360�x]�� 32 ����)1024 == 32 x 32
---------------------------------------------------------*/
static void spell_create_05_32way_dual(SPRITE *src)
{
	if (0==((src->boss_base_spell_time_out)&0x3f))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
			obj_send1->cx256					= (src->cx256); 	/* �e��x256 �{�X���S���甭�e�B */
			obj_send1->cy256					= (src->cy256); 	/* �e��y256 �{�X���S���甭�e�B */
		//
			br.BULLET_REGIST_01_speed_offset			= t256(1/*0*/);/*(�e�X�g)*/
			br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)�W���e */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KNIFE20_04_AOI;			/* [�i�C�t�e] */
		//����br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)�W���e */
			br.BULLET_REGIST_06_n_way					= (16); 							/* [16way] */
			br.BULLET_REGIST_07_div_angle65536			= (int)(65536/(16));		/* �����p�x(65536[360/360�x]�� 16 ����) */	/* 1����n���������p�x */
		//
			br.BULLET_REGIST_00_speed256				= (t256(0.75)); 		/* �����͒ᑬ�e */
			br.BULLET_REGIST_02_angle65536				= (0);					/* �e���p�x65536 */
			tama_system_regist_katayori_n_way();/* (r33-) */
		//
			br.BULLET_REGIST_00_speed256				= (t256(1.25)); 		/* ��͍����e */
			br.BULLET_REGIST_02_angle65536				= (65536/32);			/* �e���p�x65536 */
			tama_system_regist_katayori_n_way();/* (r33-) */
	}
}


/*---------------------------------------------------------
	3�u�ʏ�U��1�v�x���e�Ƃ̑g�ݍ��킹
---------------------------------------------------------*/
static void spell_create_2a_sakuya_baramaki1(SPRITE *src)
{
//	if (64 == (src->boss_base_spell_time_out))
//	{
//	}
//	else
	if (64 > (src->boss_base_spell_time_out))
	{
		spell_create_04_pink_hearts(src);
	}
	else
	if (0 == ((src->boss_base_spell_time_out)&0x7f))	/* 128 256 384 512 (640 768) */
	{
		spell_create_05_32way_dual(src);
	}
}


/*---------------------------------------------------------
	5�u�ʏ�U��3�v�����e�Ƃ̑g�ݍ��킹
---------------------------------------------------------*/
static void spell_create_2b_sakuya_baramaki2(SPRITE *src)
{
	if (64 > (src->boss_base_spell_time_out))
	{
		spell_create_04_pink_hearts(src);
	}
	else
//	if (0 == ((src->boss_base_spell_time_out)&0xff))	/* 256 512 (768) */
	if (0 == ((src->boss_base_spell_time_out)&0xff))	/* 256 512 (768) */
	{
		spell_create_05_32way_dual(src);
	}
}

/*---------------------------------------------------------
	���݁u�N���b�N�R�[�v�X(1/2)�v
---------------------------------------------------------*/
#if 0
#endif


/*---------------------------------------------------------
	���݁u�N���b�N�R�[�v�X(2/2)�v
---------------------------------------------------------*/
#if 0
#endif


/*---------------------------------------------------------
	���ہu���i�N���b�N(1/2)�v
	-------------------------------------------------------
	32way �Œ�A�e	x 4��
---------------------------------------------------------*/
static void spell_create_06_luna_clock_32way(SPRITE *src)
{
	if (0x40==((src->boss_base_spell_time_out)&0xcf))/* 4�� */
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
		br.BULLET_REGIST_00_speed256				= (t256(1.0));				/* �e�� */
		br.BULLET_REGIST_01_speed_offset			= t256(1/*0*/);/*(�e�X�g)*/
		br.BULLET_REGIST_02_angle65536				= (0);						/* ���˒��S�p�x / ����@�\(���@�_��/��) */
		br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)�W���e */
		br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_CAP16_04_TUTU_SIROI;	/* [������e] */	/* �e�O�� */
	//����br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)�W���e */
		br.BULLET_REGIST_06_n_way					= (32); 					/* [32way] */		/* ���e�� */
		br.BULLET_REGIST_07_div_angle65536			= (int)(65536/(32));		/* �����p�x(1024[360/360�x]�� 32 ����) */	/* 1����n���������p�x */
		tama_system_regist_katayori_n_way();/* (r33-) */
	}
}


/*---------------------------------------------------------
	���ہu���i�N���b�N(2/2)�v
---------------------------------------------------------*/
#if 0
#endif


/*---------------------------------------------------------
	�u�ʏ�U��3(1/2)�v
	-------------------------------------------------------
	�g�U�^�S���ʃi�C�t�e
	80�����e == (8way[hi to 0]+8way[slow to 0]) x 5way(5����)
	-------------------------------------------------------
	�{���͉�ʏ�Ɖ�ʍ��Ɖ�ʉE�̒[�Ńi�C�t����x�������˂���B
---------------------------------------------------------*/
static void spell_create_07_80way_dual_five(SPRITE *src)
{
	if (0==((src->boss_base_spell_time_out)&0x3f))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
			obj_send1->cx256					= (src->cx256); 	/* �e��x256 �{�X���S���甭�e�B */
			obj_send1->cy256					= (src->cy256); 	/* �e��y256 �{�X���S���甭�e�B */
			br.BULLET_REGIST_01_speed_offset			= t256(1/*0*/);/*(�e�X�g)*/
			br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)�W���e */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KNIFE20_04_AOI;				/* [�i�C�t�e] */
		//����br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)�W���e */
	//
		int ii; 	/* 1����65536���������p�x */
		int j;		/* �J�E���^ */
		j=0;
		for (ii=0; ii<(0x10000); ii += 819 )/* 819.2(65536/80)	12.8==(1024/80) 80���� [5����] */	/* 204.8==(1024/5) �p�x(1024[72/360�x]�� 5 ����) */
		{
			j++;
			j &= (16-1);/* 16way(8way[hi to 0]+8way[slow to 0]) */
			br.BULLET_REGIST_00_speed256			= (t256(3.0)-((j)<<5)-((j&1)<<7));		/*��͍����e*/
			br.BULLET_REGIST_02_angle65536			= (((0-ii))&(65536-1)); 			/* �p�x */
			tama_system_regist_single();/* (r33-) */
		}
	}
}
/*---------------------------------------------------------
	�ʏ�U���A�ԃi�C�t
	��CW�A�Ԃ�CCW
---------------------------------------------------------*/
static void spell_create_2c_sakuya_blue_red_knife(SPRITE *src)
{
	if ((0!=((src->boss_base_spell_time_out)&0x0180)))
	{
		if (0==((src->boss_base_spell_time_out)&0x3f))
		{
			#if (1)
		//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
			#endif
				obj_send1->cx256					= (src->cx256); 	/* �e��x256 �{�X���S���甭�e�B */
				obj_send1->cy256					= (src->cy256); 	/* �e��y256 �{�X���S���甭�e�B */
			unsigned int kk;
			kk= (((src->boss_base_spell_time_out)&0x0100)>>8);
				br.BULLET_REGIST_01_speed_offset			= t256(1/*0*/);/*(�e�X�g)*/
				br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)�W���e */
				br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KNIFE20_04_AOI+kk; 			/* [�i�C�t�e] */
			//����br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)�W���e */
		//
			int ii; 	/* 1����65536���������p�x */
			int j;		/* �J�E���^ */
			j=0;
			for (ii=0; ii<(0x10000); ii += 819 )/* 819.2(65536/80)	12.8==(1024/80) 80���� [5����] */	/* 204.8==(1024/5) �p�x(1024[72/360�x]�� 5 ����) */
			{
				j++;
				j &= (16-1);/* 16way(8way[hi to 0]+8way[slow to 0]) */
				br.BULLET_REGIST_00_speed256			= (t256(3.0)-((j)<<5)-((j&1)<<7));		/*��͍����e*/
				br.BULLET_REGIST_02_angle65536			= ((kk?(0-ii):(ii))&(65536-1)); 			/* �p�x */
				tama_system_regist_single();/* (r33-) */
			}
		}
	}
}


/*---------------------------------------------------------
	���C�h��Z�u����h�[���v
---------------------------------------------------------*/
#if 0
#endif


/*---------------------------------------------------------
	�p�[�t�F�N�g�t���[�Y(�g��12�ےe��S�e����������)
	-------------------------------------------------------
	�����e(���m�ɂ͓������e)�Ȃ̂ɁA�����e�Ɍ����Ȃ���
	�����e(���m�ɂ͓����x�e�A����==0)�Ɍ������Ⴄ��肪����B

---------------------------------------------------------*/
#if 1
static void exchange_damnaku_sakuya_saikasoku(void)
{
//	int check_type;
//	check_type = br.BULLET_REGIST_04_bullet_obj_type;/* ���ׂ�^�C�v���󂯎�� */
//	int jj = 0;
	int ii;
	for (ii=0; ii<OBJ_POOL_00_TAMA_MAX; ii++ )/* �S�����ׂ�B */
	{
		SPRITE *s;
		s = &obj99[OBJ_HEAD_00_TAMA+ii];
	//	if (check_type == (s->type) )	/* ���̐e���Ԓe�Ȃ� */
		if ((BULLET_OODAMA32_02_KIIRO & 0xfff8) == (s->type & 0xfff8) ) 	/* �g��12�ےe�Ȃ� */
		{
//			jj++;
		//	jj &= 0x07; 	// 2 3 4 5	6 7 8 9
//			jj &= 0x03; 	// 3 4 5 6
		//	s_change_meirin_yajirusi_one(s);
		//	s->tama_system_tra65536 					= t256(6);			/* ���������e */
		//	s->tama_system_tra65536 					= (jj<<8)+t256(2);	/* (2 ... 9) ���������e */
//			s->tama_system_tra65536 					= (jj<<8)+t256(3);	/* (3 ... 6) ���������e */
			s->tama_system_tra65536 					= t256(3);			/* (3 ... 6) ���������e */
			/* (1�͒x�������̂ŏ��O) 9 ���Ƒ��߂��邩�� */
		}
		if ((BULLET_KNIFE20_04_AOI & 0xfff8) == (s->type & 0xfff8) )	/* �i�C�t�e�Ȃ� */
		{
//			s->tama_system_tra65536 					= (jj<<8)+t256(3);	/* (3 ... 6) ���������e */
			s->tama_system_tra65536 					= t256(2);			/* (3 ... 6) ���������e */
		}
	}
}
#endif

/*---------------------------------------------------------
	�p�[�t�F�N�g�t���[�Y(�g��12�ےe��S�e��~�B���݂̍��W��V��_���W�ɕύX����B������ς���)
	-------------------------------------------------------

---------------------------------------------------------*/
#if 1
static void exchange_damnaku_sakuya_tomeru(void)
{
//	int check_type;
//	check_type = br.BULLET_REGIST_04_bullet_obj_type;/* ���ׂ�^�C�v���󂯎�� */
//	int add_angle = 0;
	int ii;
	for (ii=0; ii<OBJ_POOL_00_TAMA_MAX; ii++ )/* �S�����ׂ�B */
	{
		SPRITE *s;
		s = &obj99[OBJ_HEAD_00_TAMA+ii];
	//	if (check_type == (s->type) )	/* ���̐e���Ԓe�Ȃ� */
		if ((BULLET_OODAMA32_01_AKA & 0xfff8) == (s->type & 0xfff8) )	/* [�ԑ�e]�Ȃ� */
		{
		//	s_change_meirin_yajirusi_one(s);
			/* �e�̌��ݍ��W���A�V��_���W�Ƃ���B */
			s->tx256 = (s->cx256);		/* ��_���W�ɃZ�b�g */
			s->ty256 = (s->cy256);		/* ��_���W�ɃZ�b�g */
			s->tama_system_radius256 = t256(0); 	/* ��_���W�ɂȂ����̂Ŕ��a�� 0 �ɂ���B */
			/*	*/
			s->tama_system_tra65536 					= t256(0);		/* ���������e */
			s->tama_system_speed65536					= t256(0);		/* �e�� */
			#if 0
		//	s->rotationCCW1024				= (ra_nd()&((1024)-1)); 			/* ���� */	/* �����͗����łȂ��C������ */
			#else
			/* ���Ȃ��Ƃ��������͖{���ɋ߂��B ���A�S�R�Ⴄ�B */
			/* ���Z�Œ�l(KOTEITI_7777)�ȊO�̗v���Ƃ��āA�e�����X�g�̃T�C�Y�Ń��[�v����ׁA�e�����X�g�̃T�C�Y�� */
			/* ���݂� 1024 ����A�{���Ɠ��� 640 �Ɍ��点�΁A����\���͂���B */
//			add_angle += (KOTEITI_7777);
//			s->rotationCCW1024				= (add_angle>>6);			/* ���� */
			#endif
			s->type 						= (BULLET_OODAMA32_02_KIIRO);
		}
	}
}
#endif
#if 1
static SPRITE obj_work;

static void exchange_damnaku_sakuya_tuika(void)
{
	voice_play(VOICE16_BOSS_KYUPIN, TRACK04_TEKIDAN);
//
	br.BULLET_REGIST_00_speed256				= (t256(0));			/* �e�� */
	br.BULLET_REGIST_01_speed_offset			= t256(0);/*(�e�X�g)*/
	br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)�W���e */
	br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KNIFE20_04_AOI;			/* [�i�C�t�e] */
//����br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)�W���e */
//
	obj_work.cx256					= (obj_send1->cx256);	/* �e��x256 �{�X���S���甭�e�B */
	obj_work.cy256					= (obj_send1->cy256);	/* �e��y256 �{�X���S���甭�e�B */
//	/*const*/ SPRITE *obj_work_ptr = &obj_work;

//	int check_type;
//	check_type = br.BULLET_REGIST_04_bullet_obj_type;/* ���ׂ�^�C�v���󂯎�� */
	int ii;
	for (ii=0; ii<OBJ_POOL_00_TAMA_MAX; ii++ )/* �S�����ׂ�B */
	{
		SPRITE *s;
		s = &obj99[OBJ_HEAD_00_TAMA+ii];
	//	if (check_type == (s->type) )	/* ���̐e���Ԓe�Ȃ� */
		if ((BULLET_OODAMA32_02_KIIRO & 0xfff8) == (s->type & 0xfff8) ) 	/* [����e]�Ȃ� */
		{
		//	s_change_meirin_yajirusi_one(s);
			/* �e�̌��ݍ��W�Ƀi�C�t�e��o�^����B */
			obj_send1->cx256					= (s->cx256);	/* �e��x256 �{�X���S���甭�e�B */
			obj_send1->cy256					= (s->cy256);	/* �e��y256 �{�X���S���甭�e�B */
			//
		//	tmp_angleCCW65536_src_nerai(obj_player, obj_send1);/* ���@�_���p�쐬 */
			tmp_angleCCW65536_src_nerai(&obj_work, s);/* ���@�_���p�쐬 */
			//
			br.BULLET_REGIST_02_angle65536			= (s->tmp_angleCCW65536);/* ���@�_���p */
			tama_system_regist_single();/* (r33-) */
		}
	}
}
#endif

/*
	���H�u�W���b�N�E�U�E���h�r���v�̌����B
	���̃X�y�J�͎n�߂�[�ԑ�e]�������_�����ۂ��΂�T���B
	���̂΂�T���́A���Ď��̂Ȃ������̓����΂�T���Ȃ񂾂��ǁB
	�ǂ������d�g�݂Ȃ̂������ς�킩���̂ŁA�������Ă݂�B
	�܂��A�΂�T�������œK���ɃX�N�V���B�e�B
	��ʂ�2/3�ʂ����e������Ȃ��B
	�����Ń`�}�`�}���𐔂����20�e�`26�e���炢�B
	��ʔ�������16�`18�e���炢�Ȃ̂ŁA�����S����32�e�`36�e�Ȃ̂ł́H�Ɛ����B
	32�e�A36�e�B�ǂ��������肻���Ȃ̂ŉ��x�������Ă݂�B�悭����Ȃ���32�e���ۂ��B
	���Q�[���t�B�[���h��384x448, ��邳��̍��W��291x49,
	�������̍��W��ڎ�(�摜���S���W)�{���G�����\�t�g�Ōv�����鎖�ɂ����B
	��e�Ȃ̂ŁA���G�����\�t�g�œ��ډ~�ɂȂ�悤�Ɏl�p��`���B���̂��ƑΊp���ɐ���`���Ē��S���W������o���B
	----------------
	�ԍ�,�摜���S���W,	���΍��W
	 1�e�ڂ� 94x 87,	-(291- 94)x 87-49,	==(-197,+38)
	 2�e�ڂ�171x 69,	-(291-171)x 69-49,	==(-120,+20)
	 3�e�ڂ�192x 65,	-(291-192)x 65-49,	==(- 99,+16)
	 4�e�ڂ�151x119,	-(291-151)x119-49,	==(-140,+70)
	 5�e�ڂ�181x116,	-(291-181)x116-49,	==(-110,+67)
	 6�e�ڂ�221x 80,	-(291-221)x 80-49,	==(- 70,+31)
	 7�e�ڂ�175x139,	-(291-175)x139-49,	==(-116,+90)
	 8�e�ڂ�159x153,	-(291-159)x153-49,	==(-132,+104)
	 9�e�ڂ�210x152,	-(291-210)x152-49,	==(- 81,+103)
	10�e�ڂ�258x119,	-(291-258)x119-49,	==(- 33,+70)
	11�e�ڂ�255x152,	-(291-255)x152-49,	==(- 36,+103)
	12�e�ڂ�215x178,	-(291-215)x178-49,	==(- 76,+129)
	13�e�ڂ�254x170,	-(291-254)x170-49,	==(- 37,+121)
	14�e�ڂ�206x236,	-(291-206)x236-49,	==(- 85,+187)
	15�e�ڂ�233x273,	-(291-233)x273-49,	==(- 58,+224)
	16�e�ڂ�280x255,	-(291-280)x255-49,	==(- 11,+206)
	------------------���̕ӂ��������ۂ��B
	�����܂ő���B
	���΍��W����p�x������o���Ă݂�B�悭�킩��Ȃ����ǁA
	tan(197/38)�Ŋp�x���o��̂��ȁH
//							(x/y)									tan(x/y)	���Ԃ�p�x
	 1�e�ڂ�(-197,+38)		5.18421052631578947368421052631579		0.0907292755019070511126536298905576
	 2�e�ڂ�(-120,+20)		6										0.105104235265676462511502380139882
	 3�e�ڂ�(- 99,+16)		6.1875									0.108414028737611378659694904675177
	 4�e�ڂ�(-140,+70)		2										0.0349207694917477305004026257737253
	 5�e�ڂ�(-110,+67)		1.64179104477611940298507462686567		0.0286625046177796138852122880504812
	 6�e�ڂ�(- 70,+31)		2.25806451612903225806451612903226		0.0394310774284007674631847355929523
	 7�e�ڂ�(-116,+90)		1.28888888888888888888888888888889		0.0224991500955683949518891752039385
	 8�e�ڂ�(-132,+104) 	1.26923076923076923076923076923077		0.0221558801384605992607822428428949
	 9�e�ڂ�(- 81,+103) 	0.786407766990291262135922330097087 	0.0137262667569302851091532667488299
	10�e�ڂ�(- 33,+70)		0.471428571428571428571428571428571 	0.00822816644161695626063183252739627
	11�e�ڂ�(- 36,+103) 	0.349514563106796116504854368932039 	0.00610025557803738263689086699115253
	12�e�ڂ�(- 76,+129) 	0.589147286821705426356589147286822 	0.010282922344805178445884641481058
	13�e�ڂ�(- 37,+121) 	0.305785123966942148760330578512397 	0.00533700788847012290939546575203758
	14�e�ڂ�(- 85,+187) 	0.454545454545454545454545454545455 	0.00793348122027169612990511645481441
	15�e�ڂ�(- 58,+224) 	0.258928571428571428571428571428571 	0.00451918686372985895525414665154542
	16�e�ڂ�(- 11,+206) 	0.0533980582524271844660194174757282	0.000931972200504819213720633933431873
//
	Windows�̓d����g���Ă݂�B
	Windows�̓d���tan(1)��0.0174550649282175857651288952197278�������ȁB
	tan(1)��(45�x)�̔�������A���W�A��(�P����2�΂̒P�ʌn)�Ƃ���
	(2*pai)/360==tan(1)�H�H�H�B�悭�킩���ȁB
	�ǂ�����tan�Ȃ̂��悭�킩��Ȃ��B
	tan(99)==-6.31375151467504309897946424476819()
	tan(90)==�G���[�����Ȓl�ł��B
	tan(89)==57.2899616307594246872781475371126
	tan(88)==28.6362532829156035507565093209464
	tan(55)==1.4281480067421145021606184849985
//
�킩�����Barc_tan()����Ȃ��Ƒʖڂ��B	Windows�̓d��́Atan(360�x�̊p�x)����B
	----------------
�Ƃɂ����~��ɍL����Ȃ��̂ŁABULLET_REGIST_01_speed_offset�ɍ������Ȃ肠��̂����H
*/
/*---------------------------------------------------------
	���H�u�W���b�N�E�U�E���h�r���v�ɂȂ邩������Ȃ��e�X�g
---------------------------------------------------------*/
static void spell_create_14_sakuya_jack_oodama32(SPRITE *src)
{
	/* (r33)���e�G�t�F�N�g���ǉ����ꂽ�̂ŁA�ǉ����Ԃ𑁂߂āA���덇�킹�B */
	if ((96+HATUDAN_FRAME64)==(src->boss_base_spell_time_out))	// 192== (64*3)
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
			obj_send1->cx256					= (src->cx256); 	/* �e��x256 �{�X���S���甭�e�B */
			obj_send1->cy256					= (src->cy256); 	/* �e��y256 �{�X���S���甭�e�B */
		br.BULLET_REGIST_03_tama_data   		= (TAMA_DATA_8000_NON_TILT);/* (r33-)��X���e */
		br.BULLET_REGIST_04_bullet_obj_type 	= BULLET_OODAMA32_01_AKA;			/* [�ԑ�e] */
	//����br.BULLET_REGIST_05_regist_type 		= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)�W���e */
	//
		int ii; 	/* ii */
	//	int jj; 	/* jj */
		int kk; 	/* �J�E���^ */
	//	int mm; 	/* �J�E���^ */
		kk=0;
	//	mm=0;
	//	jj=(1024-960);
		const u32 ra_nd32 = ra_nd();
		for (ii=0; ii<(32); ii ++)/* 32�e�΂�T�� */
		{
			/* 960==30720/32,  30720 == 32768 -1024 -1024 */
			/* 936 = 936.228571428571428571428571428571 == (32768/(32+2���E�̗]�T��))	*/
			//	jj += (960);/* (=936)< 1024 [180�x(32768)���኱�����͈͂ɂ΂�T��] */
			//	kk += 386;
				kk += ra_nd32;
			//	kk += 42857142;
			//	mm += 42857142;
			//	br.BULLET_REGIST_01_speed_offset			= t256(0);/*(�e�X�g)*/
			//	br.BULLET_REGIST_01_speed_offset			= t256(8);/*(�e�X�g)*/
			//	br.BULLET_REGIST_01_speed_offset			= t256(12);/*(�e�X�g)*/
				br.BULLET_REGIST_01_speed_offset			= (t256(4)+((kk&0x07)<<8));/*(�e�X�g)*/
			//	br.BULLET_REGIST_00_speed256				= ((kk)&(512-1))+t256(0.25);	/*��͍����e*/
				br.BULLET_REGIST_00_speed256				= ((kk)&(1024-1))+t256(0.25);	/*��͍����e*/
				br.BULLET_REGIST_02_angle65536				= ((kk)&(32768-1))+(int)(65536*3/4);			/* �p�x */
			//	br.BULLET_REGIST_02_angle65536				= ((mm)&(1024-1))+jj+(int)(65536*3/4);			/* �p�x */
				tama_system_regist_single();/* (r33-) */
		}
	}
	else
	if (64==(src->boss_base_spell_time_out))	// 192== (64*3)
	{
//		// 48 �t���[���o�ߌ�A�S�ăX�g�b�v�����A�������A�J�E���g���Z�b�g
		exchange_damnaku_sakuya_tomeru();/*�S�ăX�g�b�v*/
	}
	else
	if (56==(src->boss_base_spell_time_out))	// 192== (64*3)
	{
		obj_send1->cx256					= t256(0);					/* �e��x256 */
		obj_send1->cy256					= t256(256);				/* �e��y256 */
		exchange_damnaku_sakuya_tuika();/*�i�C�t�ǉ�*/
	}
	else
	if (48==(src->boss_base_spell_time_out))	// 192== (64*3)
	{
		obj_send1->cx256					= t256(GAME_WIDTH); 		/* �e��x256 */
		obj_send1->cy256					= t256(256);				/* �e��y256 */
		exchange_damnaku_sakuya_tuika();/*�i�C�t�ǉ�*/
	}
	else
	if (40==(src->boss_base_spell_time_out))	// 192== (64*3)
	{
		SPRITE *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
		obj_send1->cx256					= zzz_player->cx256;			/* �e��x256 */
		obj_send1->cy256					= zzz_player->cy256;			/* �e��y256 */
		exchange_damnaku_sakuya_tuika();/*�i�C�t�ǉ�*/
	}
	else
	if (32==(src->boss_base_spell_time_out))	// 192== (64*3)
	{
		exchange_damnaku_sakuya_saikasoku();/*�ĉ���*/
	}

}
