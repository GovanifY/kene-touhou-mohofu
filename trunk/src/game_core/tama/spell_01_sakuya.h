
/*---------------------------------------------------------
 �����͕핗 �` Toho Imitation Style.
  �v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���̃J�[�h���`���܂��B
---------------------------------------------------------*/


/*---------------------------------------------------------
	�g5�ʒ�-�{�X ��� ���U���ɂ�����Ƃ��������J�[�h
	-------------------------------------------------------
	���̃J�[�h�̖{���́A
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
	�u�J�[�h�V�X�e����d���v���Ȃ��Ə�肭�����Ȃ������H�B
	-------------------------------------------------------
	�u�J�[�h�V�X�e����d���v�F2��ނ̈قȂ����J�[�h�������Ɍ��Ă�J�[�h�V�X�e���B
	�����������̂ɂ���ׂ������Ȃ��ׂ����������B(�p�`�F)
	-------------------------------------------------------
	�܂��S(����)�Ȃ̂ŁA(�v���C���[�ɂ���ȏ���)����ł�����Ă͍���B
    -------------------------------------------------------
    �i�C�tspeed256    speed_offset    /   �ԃN�i�Cspeed256    speed_offset
    t256(2.5)           (0)             /   (0)                 (4)             // �ǂ��񂾂��ǖ{��������B
    t256(2.0)           (0)             /   (0)                 (2)             // �ԃN�i�C�̏����x�����邩�Ȃ��B// �i�C�t�ƐԃN�i�C�����ꂷ���B�{��������B
    t256(1.75)          (0)             /   (0)                 (1)
    -------------------------------------------------------
	�g�p���W�X�^
//	REG_08_REG0 	�J�E���^�B
	REG_09_REG1 	aaa_angle65536
	REG_0a_REG2 	aka_kunai_timer 	�ԃN�i�C�^�C�}�[
	REG_0b_REG3 	ao_knife_timer		�i�C�t�^�C�}�[
---------------------------------------------------------*/

//#define ADJUST3916	((65536/20)+(640))
#define ADJUST3916	(4000)
//#define ADJUST4444	(4200)
#define ADJUST4444	(4190)
local void spell_create_01_sakuya_misogi_normal(SPRITE *src)
{
//	SPELL_TIME_0256 SPELL_TIME_0128
	if (127 == (REG_10_BOSS_SPELL_TIMER))/* �����l�Ƀ��Z�b�g�B */
	{
		#if 1
		/* �i�C�t: �E�㑤���猂���n�߂�B�E���Ȃ̂�90�x���炵�āAhatudan_system_regist_katayori_n_way()�̕␳���s���B */
	//	src->tmp_angleCCW65536	= (65536+(65536/4)-((65536*8)/20))+(ADJUST4444);				// �i�C�t
	//	src->tmp_angleCCW65536	= (((65536*2)/20))+(ADJUST4444);				// �i�C�t 	hatudan_system_regist_katayori_n_way();�ŉ�����ɁA���v����(2/20)���猂���n�߂�B
		src->tmp_angleCCW65536	= (((65536*1)/18))+(ADJUST4444);				// �i�C�t 	hatudan_system_regist_katayori_n_way();�ŉ�����ɁA���v����(2/20)���猂���n�߂�B
		// �{���͑ł��n�߂̐i�C�t�̒[���E��60�x���炢����ł��n�߁A
		// �ł��I���́A����60�x���炢�̈ʒu�ɂȂ�B
		#endif
		#if 1
		/* �ԃN�i�C: */
	//	REG_09_REG1 		= (65536/2);		// �ԃN�i�C
		REG_09_REG1 		= (65536/4)-(0);	// �ԃN�i�C (�E������)
		#endif
		REG_0b_REG3 = 0;	// �i�C�t
//		REG_0a_REG2 = 0;	// �ԃN�i�C
	}
//	if (DANMAKU_0096_TIME > (REG_10_BOSS_SPELL_TIMER))/* �b���������Ȃ��B */
	if (100 > (REG_10_BOSS_SPELL_TIMER))/* �b���������Ȃ��B */
	{
		REG_0b_REG3--;
		if (0 >= REG_0b_REG3)
	//	if (0==((REG_10_BOSS_SPELL_TIMER)&0x07))
		{
		//	REG_0b_REG3 = (12);
			REG_0b_REG3 = (8);
			/*	�i�C�t */
		//	src->tmp_angleCCW1024 -= (int)(256/5)+(10); /*CCW*/
		//	src->tmp_angleCCW1024 -= (int)(1024/20)+(10); /*CCW*/
		//	src->tmp_angleCCW65536 -= (int)(65536/20)+(640); /*CCW*/
		//	src->tmp_angleCCW65536 -= (int)(65536/20)+(640)+(128); /*CCW*/	/* 3276== (65536/20) */
			src->tmp_angleCCW65536 -= (ADJUST4444); /*CCW*/
			mask65536(src->tmp_angleCCW65536);
		//	HATSUDAN_01_speed256			= (t256(2.5));					/* �e�� (t256(2.5)�Q�l�B�g�m�[�}��5��) */
		//	HATSUDAN_01_speed256			= (t256(2.0));					/* �e�� psp�͏c�𑜓x���Ȃ��̂łƂɂ����x�����Ȃ��ƁB */
//			HATSUDAN_01_speed256			= (t256(1.75)); 				/* �e�� psp�͏c�𑜓x���Ȃ��̂łƂɂ����x�����Ȃ��ƁB */
		//	HATSUDAN_01_speed256			= (t256(1.00)); 				/* �e�� psp�͏c�𑜓x���Ȃ��̂łƂɂ����x�����Ȃ��ƁB */
			HATSUDAN_01_speed256			= (t256(1.25)); 				/* �e�� psp�͏c�𑜓x���Ȃ��̂łƂɂ����x�����Ȃ��ƁB */
		//	HATSUDAN_02_speed_offset		= t256(0.0);/* �Ă��� */
		//	HATSUDAN_02_speed_offset		= t256(1.0);/* �������Ă݂�Ă��� */
		//	HATSUDAN_02_speed_offset		= t256(6.0);/* �������Ă݂�Ă��� */
			HATSUDAN_02_speed_offset		= t256(4.0);/* �������Ă݂�Ă��� */
			HATSUDAN_03_angle65536			= (src->tmp_angleCCW65536); 	/* �e���p�x65536 */ 		/* [������e] */
			HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
			HATSUDAN_05_bullet_obj_type 	= BULLET_KNIFE20_04_AOI;		/* [�i�C�t�e] */
			HATSUDAN_06_n_way				= (8);							/* [8way] */
		//	HATSUDAN_07_div_angle65536		= (int)(65536/(5*4));
		//	HATSUDAN_07_div_angle65536		= (int)(65536/20);				/* �����p�x([360/360]�x��20����) */
			HATSUDAN_07_div_angle65536		= (int)(65536/18);				/* �����p�x([360/360]�x��20����)[�ł��I��肪���΂ߏ�ɂȂ�] */
			hatudan_system_regist_katayori_n_way();/* (r33-) */
		//	if (0==((REG_10_BOSS_SPELL_TIMER)&0x1f))/* reset (1/32) */
		//	{
		//	//	aaa_angle1024	= (src->tmp_angleCCW1024)-(int)((1024/2));
		//		REG_09_REG1 	= (src->tmp_angleCCW65536)-(int)((65536/2));
		//	}
			#if (1)
		//	voice_play(VOICE14_SAKUYA_SHOT02, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE14_SAKUYA_SHOT02);
			#endif
		}
	}
	REG_0a_REG2--;
	if (0 > REG_0a_REG2) /* REG_0a_REG2 �����̏ꍇ */
	{
		REG_0a_REG2 = 32/*64*/+(ra_nd()&0x3f);
	}
	else /* REG_0a_REG2 �����̏ꍇ */
//	/* �ԃN�i�C�̕����x���̂ŁA�i�C�t����Ɍ����n�߂鎖�ɂ���B */
//	if (DANMAKU_0032_TIME > (REG_0a_REG2))/* �b���������Ȃ��B */
//	if ( (41) > (REG_0a_REG2))/* �b���������Ȃ��B */
	if ( (41-16+(((unsigned)REG_0f_GAME_DIFFICULTY)<<4)) > (u32)(REG_0a_REG2))/* �b���������Ȃ��B */
//	if ((128-40) < (REG_0a_REG2))/* �Ō�͎b���������Ȃ��B */
	{
	//	else/* �������̒e�͖{���͂���ďo�銴���ɂ������B */
//	//	if ((10-1)>((((u8)REG_0a_REG2)^0x20)&0x2f))/* x 10set */
//		if ((10-1)>(((REG_0a_REG2)	  )&0x0f))/* x 10set */
		/* 4��Ɉ��A�v10set */
		if ((0)==(((REG_0a_REG2)	  )&0x03))/* x 10set */
		{
			/* �������̐ԃN�i�C�͑S�R�_���D */
		//	aaa_angle1024 -= (int)(256/5);
		//	aaa_angle1024 -= (int)(1024/20);
			REG_09_REG1 -= (int)(65536/20);
		//	if (0 > (int)REG_09_REG1)	{	REG_09_REG1 = (65536/2);	}
			mask65536(REG_09_REG1);
		//	HATSUDAN_01_speed256			= (t256(2.0));						/* �e�� ���߂��� */
		//	HATSUDAN_01_speed256			= (t256(1.0));						/* �e�� (�o�n�߂͒x��) */
		//	HATSUDAN_01_speed256			= (t256(0.5));						/* �e�� (�o�n�߂͒x��) */
		//	HATSUDAN_01_speed256			= (t256(0.25)); 					/* �e�� (�o�n�߂͒x��) */
		//	HATSUDAN_03_angle65536			= (((REG_09_REG1)+(65536*3/4))&(65536-1));	/* �e���p�x65536 */ 	/*��CCW*/
			HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
			HATSUDAN_05_bullet_obj_type 	= (BULLET_KUNAI12_BASE + TAMA_IRO_01_AKA);			/* [�ԃN�i�C�e] */
			HATSUDAN_06_n_way				= (4);								/* [4way] */
			HATSUDAN_07_div_angle65536		= (int)(512-64);					/* [�����e]�̕����p�x(r32) */
//-------------------------
	// ��  [�����e]
		//	HATSUDAN_01_speed256			= (0);								/* �e�� (�o�n�߂͒x��) */
//			HATSUDAN_01_speed256			= (t256(0.25)); 					/* �e�� (�o�n�߂͒x��) */
		//	HATSUDAN_01_speed256			= (t256(3.25)); 					/* �e�� (�o�n�߂͒x��) */
		//	HATSUDAN_01_speed256			= (t256(3.00)); 					/* �e�� (�o�n�߂͒x��) */
			HATSUDAN_01_speed256			= (t256(2.00)); 					/* �e�� (�o�n�߂͒x��) */

		//	HATSUDAN_02_speed_offset		= t256(3/*0*/);/* �������Ă݂� �Ă��� */
//			HATSUDAN_02_speed_offset		= t256(2);/* �������H(�������e�Ȏ��͊ԈႢ�Ȃ��H) */
		//	HATSUDAN_02_speed_offset		= t256(1);/* �������H(�������e�Ȏ��͊ԈႢ�Ȃ��H) */
			HATSUDAN_02_speed_offset		= t256(-1);/* �����H(�������e�Ȏ��͊ԈႢ�Ȃ��H) */
			HATSUDAN_03_angle65536			= (REG_09_REG1);	/* �e���p�x65536 */ 	/*��CCW*/
			hatudan_system_regist_katayori_n_way();/* (r33-) */
//-------------------------
	// ��  [�����e]
		//	HATSUDAN_01_speed256			= (t256(0.25)); 					/* �e�� (�o�n�߂͒x��) */
		//	HATSUDAN_01_speed256			= (t256(1.25)); 					/* �e�� (�o�n�߂͒x��) */
			HATSUDAN_01_speed256			= (t256(2.25)); 					/* �e�� (�o�n�߂͒x��) */
			HATSUDAN_02_speed_offset		= t256(-1);/* �����H(�������e�Ȏ��͊ԈႢ�Ȃ��H) */
			HATSUDAN_03_angle65536			= (REG_09_REG1);	/* �e���p�x65536 */ 	/*��CCW*/
			hatudan_system_regist_katayori_n_way();/* (r33-) */
		}
	}
}


/*---------------------------------------------------------
	�g5�ʒ�-�{�X ��� ���U���ɂ�����Ƃ��������J�[�h
	-------------------------------------------------------
	�͕핗�Ȃ񂾂��班���͕핗�ȃJ�[�h��������ƃe�X�g�őn���Ă݂��B
	��x�I�ɂ�psp-2000�Ȃ�A�gnormal5�ʂɒʏ�J�[�h�Ƃ��ďo�Ă����Ƃ��ē������炢���A
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
	�ǂ��l�߂��ă{����ꍇ�A�i�C�t�̑��x�������񂾂���A���R(���̃J�[�h�Ń{����ꍇ���)��������
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
	-------------------------------------------------------
	�g�p���W�X�^
//	REG_08_REG0 	�J�E���^�B
	REG_09_REG1 	aaa_angle65536
	REG_0a_REG2 	aka_kunai_timer 	�ԃN�i�C�^�C�}�[
	REG_0b_REG3 	knife_kaiten		0(�i�C�t) or 1(�ԃi�C�t)
//	sta tic int ao_knife_timer; 		// �i�C�t�^�C�}�[
---------------------------------------------------------*/

local void spell_create_2d_sakuya_misogi_lunatic(SPRITE *src)
{
//	SPELL_TIME_0256 SPELL_TIME_0128
	if (127 == (REG_10_BOSS_SPELL_TIMER))/* �����l�Ƀ��Z�b�g�B */
	{
		#if 1
		/* �i�C�t: �E�㑤���猂���n�߂�B�E���Ȃ̂�90�x���炵�āAhatudan_system_regist_katayori_n_way()�̕␳���s���B */
	//	src->tmp_angleCCW65536	= (65536+(65536/4)-((65536*8)/20))+(ADJUST3916);				// �i�C�t
	//	src->tmp_angleCCW65536	= (((65536*2)/20))+(ADJUST3916);				// �i�C�t 	hatudan_system_regist_katayori_n_way();�ŉ�����ɁA���v����(2/20)���猂���n�߂�B
		src->tmp_angleCCW65536	= (((65536*1)/18))+(ADJUST3916);				// �i�C�t 	hatudan_system_regist_katayori_n_way();�ŉ�����ɁA���v����(2/20)���猂���n�߂�B
		// �{���͑ł��n�߂̐i�C�t�̒[���E��60�x���炢����ł��n�߁A
		// �ł��I���́A����60�x���炢�̈ʒu�ɂȂ�B
		#endif
		#if 1
		/* �ԃN�i�C: */
	//	REG_09_REG1 		= (65536/2);		// �ԃN�i�C
		REG_09_REG1 		= (65536/4)-(0);	// �ԃN�i�C (�E������)
		#endif
//		ao_knife_timer = 0; 	// �i�C�t
//		REG_0a_REG2 = 0;	// �ԃN�i�C
		REG_0b_REG3 &= 1;/* �O�̂��߃}�X�N */
		REG_0b_REG3 ^= 1;/* ���] */
	}
//	if (DANMAKU_0096_TIME > (REG_10_BOSS_SPELL_TIMER))/* �b���������Ȃ��B */
	if (48 > (REG_10_BOSS_SPELL_TIMER))/* �b���������Ȃ��B */
	{
//		ao_knife_timer--;
//		if (0 >= ao_knife_timer)
	//	if (0==((REG_10_BOSS_SPELL_TIMER)&0x07))
		{
		//	ao_knife_timer = (12);
//			ao_knife_timer = (8);
			/*	�i�C�t */
			if (0==REG_0b_REG3)
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
			HATSUDAN_03_angle65536			= (src->tmp_angleCCW65536); 	/* �e���p�x65536 */ 		/* [������e] */
			//
		//	HATSUDAN_01_speed256			= (t256(2.5));						/* �e�� (t256(2.5)�Q�l�B�g�m�[�}��5��) */
		//	HATSUDAN_01_speed256			= (t256(2.0));						/* �e�� psp�͏c�𑜓x���Ȃ��̂łƂɂ����x�����Ȃ��ƁB */
//			HATSUDAN_01_speed256			= (t256(1.75)); 					/* �e�� psp�͏c�𑜓x���Ȃ��̂łƂɂ����x�����Ȃ��ƁB */
			HATSUDAN_01_speed256			= (t256(1.00)); 					/* �e�� psp�͏c�𑜓x���Ȃ��̂łƂɂ����x�����Ȃ��ƁB */
			HATSUDAN_02_speed_offset		= t256(6);/*�������Ă݂�Ă���*/
			HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
			HATSUDAN_05_bullet_obj_type 	= BULLET_KNIFE20_04_AOI+REG_0b_REG3;			/* [�i�C�t�e] */
			HATSUDAN_06_n_way				= (8);								/* [8way] */
		//	HATSUDAN_07_div_angle65536		= (int)(65536/(5*4));
		//	HATSUDAN_07_div_angle65536		= (int)(65536/20);					/* �����p�x([360/360]�x��20����) */
			HATSUDAN_07_div_angle65536		= (int)(65536/18);					/* �����p�x([360/360]�x��20����)[�ł��I��肪���΂ߏ�ɂȂ�] */
			hatudan_system_regist_katayori_n_way();/* (r33-) */

		//	if (0==((REG_10_BOSS_SPELL_TIMER)&0x1f))/* reset (1/32) */
		//	{
		//	//	aaa_angle1024	= (src->tmp_angleCCW1024)-(int)((1024/2));
		//		REG_09_REG1 = (src->tmp_angleCCW65536)-(int)((65536/2));
		//	}
			#if (1)
		//	voice_play(VOICE14_SAKUYA_SHOT02, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE14_SAKUYA_SHOT02);
			#endif
		}
	}
	REG_0a_REG2--;
	if (0 > REG_0a_REG2)
	{
		REG_0a_REG2 = 64+(ra_nd()&0x3f);
	}
	else
//	/* �ԃN�i�C�̕����x���̂ŁA�i�C�t����Ɍ����n�߂鎖�ɂ���B */
//	if (DANMAKU_0032_TIME > (REG_0a_REG2))/* �b���������Ȃ��B */
	if (41 > (REG_0a_REG2))/* �b���������Ȃ��B */
//	if ((128-40) < (REG_0a_REG2))/* �Ō�͎b���������Ȃ��B */
	{
	//	else/* �������̒e�͖{���͂���ďo�銴���ɂ������B */
//	//	if ((10-1)>((((u8)REG_0a_REG2)^0x20)&0x2f))/* x 10set */
//		if ((10-1)>(((REG_0a_REG2)	  )&0x0f))/* x 10set */
		/* 4��Ɉ��A�v10set */
		if ((0)==(((REG_0a_REG2)	  )&0x03))/* x 10set */
		{
			/* �������͐ԃN�i�C */
		//	aaa_angle1024 -= (int)(256/5);
		//	aaa_angle1024 -= (int)(1024/20);
			REG_09_REG1 -= (int)(65536/20);
		//	if (0 > (int)REG_09_REG1)	{	REG_09_REG1 = (65536/2);	}
			mask65536(REG_09_REG1);
		//	HATSUDAN_03_angle65536			= (((REG_09_REG1)+(65536*3/4))&(65536-1));	/* �e���p�x65536 */ 	/*��CCW*/
		//	HATSUDAN_03_angle65536			= (((REG_09_REG1)+(65536*3/4))&(65536-1));	/* �e���p�x65536 */ 	/*��CCW*/
			HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
			HATSUDAN_05_bullet_obj_type 	= (BULLET_KUNAI12_BASE + TAMA_IRO_01_AKA);			/* [�ԃN�i�C�e] */
			HATSUDAN_06_n_way				= (4);								/* [4way] */
			HATSUDAN_07_div_angle65536		= (int)(512-64);					/* [�����e]�̕����p�x(r32) */
//-------------------------
	// ��  [�����e]
			HATSUDAN_01_speed256			= (t256(2.00)); 							/* �e�� (�o�n�߂͒x��) */
			HATSUDAN_02_speed_offset		= t256(-1);/* �����H(�������e�Ȏ��͊ԈႢ�Ȃ��H) */
			HATSUDAN_03_angle65536			= (REG_09_REG1);	/* �e���p�x65536 */ 	/*��CCW*/
			hatudan_system_regist_katayori_n_way();/* (r33-) */
//-------------------------
	// ��  [�����e]
			HATSUDAN_01_speed256			= (t256(2.25)); 							/* �e�� (�o�n�߂͒x��) */
			HATSUDAN_02_speed_offset		= t256(-1);/* �����H(�������e�Ȏ��͊ԈႢ�Ȃ��H) */
			HATSUDAN_03_angle65536			= (REG_09_REG1);	/* �e���p�x65536 */ 	/*��CCW*/
			hatudan_system_regist_katayori_n_way();/* (r33-) */
		}
	}
}


/*---------------------------------------------------------
	�g5�ʒ�-�{�X ���u��p�F�~�X�f�B���N�V�����v�ɂ�����Ƃ��������J�[�h
	-------------------------------------------------------
	Lunatic �ԃN�i�C�e 19way
	-------------------------------------------------------

---------------------------------------------------------*/
/*
------------------
		/	256
		/		�ԃN�i�C
		/	//(-r33)208 208==	64+144==256-48
27*8	/	216 == 64+152== 256-40//(r34-)
		/		�Ȃɂ����Ȃ�
14*8	/	112 112==	64+48
		/		�i�C�t
		/	64	64==	64+0
		/	------------
		/	64	64==	((64+256)-256)
		/		�Ȃɂ����Ȃ�
		/	48	48==	((64+240)-256)
		/		�i�C�t�̉������B
		/	0	0== 	((64+192)-256)
		/		���]�ړ�
		/	0	0== 	((64+192)-256)
*/
/*---------------------------------------------------------
	[�������Z�N�V����]
	-------------------------------------------------------
---------------------------------------------------------*/

local void spell_init_22_sakuya_miss_direction(SPRITE *src)
{
	if(1<REG_0f_GAME_DIFFICULTY)
	// Hard or Lunatic only.
	{
		card.danmaku_callback[0] = danmaku_01_standard_angle_sayuu_hansya_mover;/*(�ʏ�e�p)*/	/*(��ʊO�Ȃ甽�ˌ���)*/
	}
	// Easy or Normal only.(�W�����)
//	card.danmaku_callback[0] = danmaku_00_standard_angle_mover;/*(�ʏ�e�p)*/	/*(��ʊO�Ȃ�e������)*/
//	card.danmaku_callback[1] = NULL;/*(���g�p)*/
//	card.danmaku_callback[2] = NULL;/*(���g�p)*/
//	card.danmaku_callback[3] = NULL;/*(���g�p)*/
}

local void spell_create_22_sakuya_miss_direction(SPRITE *src)
{
//	if (/*256-64-48*/(64+144)< REG_10_BOSS_SPELL_TIMER)
	if (/*256-64-48*/(256-40)< REG_10_BOSS_SPELL_TIMER)
	{
		/* �ԃN�i�C�e */
		if (0==((REG_10_BOSS_SPELL_TIMER)&0x03))
		{
		//	���e�ʒu���{�X���S�փZ�b�g�B
			//	HATSUDAN_01_speed256				= (t256(2.0)+((REG_10_BOSS_SPELL_TIMER)<<2)); /* �e�� */
				HATSUDAN_01_speed256				= (t256(2.0)-((REG_10_BOSS_SPELL_TIMER)));	/* �e�� */
				HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_1000_EFFECT_MIDDLE)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
				HATSUDAN_05_bullet_obj_type 		= (BULLET_KUNAI12_BASE + TAMA_IRO_01_AKA);			/* [�ԃN�i�C�e] */
				HATSUDAN_06_n_way					= (24); 							/* [24way] */
			//	Lunatic �ԃN�i�C�e 19way
				HATSUDAN_07_div_angle65536			= (int)(65536/24);					/* �����p�x */
			// lunatic ��[19way]
		//
			unsigned int ii;
			for (ii=0; ii<((((unsigned)REG_0f_GAME_DIFFICULTY)+1)); ii++)
			{
			//	REG_02_DEST_X							+= (ii<<10);	/* �e��x256 */
				REG_02_DEST_X							+= (1<<10); 	/* �e��x256 */		//	���e�ʒu�����炷�B
			//
				HATSUDAN_02_speed_offset			= t256(1)+(ii<<8);/*(�e�X�g)*/
				calculate_jikinerai();/* ���@�_���p�쐬 */
				#if 1/*(r35�̃V�X�e���ł́A�ȗ��\)*/
			//	HATSUDAN_03_angle65536				= (HATSUDAN_03_angle65536); /* �e���p�x65536 */ 	/* ���@�_���e */
				#endif
				hatudan_system_regist_katayori_n_way();/* (r33-) */
			}
			#if (1)
		//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
			#endif
		}
	}
	else
//	if (256-192< REG_10_BOSS_SPELL_TIMER)
	if ((64+48)< REG_10_BOSS_SPELL_TIMER)
	{;}/* �Ȃɂ����Ȃ� */
	else
//	if (256-192< REG_10_BOSS_SPELL_TIMER)
	if ((64+0)< REG_10_BOSS_SPELL_TIMER)
	{
		/* �i�C�t */
		if (0==((REG_10_BOSS_SPELL_TIMER)&0x0f))
		{
			REG_02_DEST_X	= ((src->cx256));
			REG_03_DEST_Y	= ((src->cy256));
			calculate_jikinerai();/* 1:���������쐬������ۂ��B */
				int int_aaa 						= ((HATSUDAN_03_angle65536));		/* �e���p�x1024 */
				HATSUDAN_02_speed_offset		= t256(0);/*(�e�X�g)*/
				HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_3000_EFFECT_NONE)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
				HATSUDAN_05_bullet_obj_type 	= BULLET_KNIFE20_04_AOI;			/* [�i�C�t�e] */
			int jj;
			for (jj=(0); jj<(3); jj++)
			{
				// �e1 �e2	1.0 (1.5 2.0)
				HATSUDAN_01_speed256			= (t256(1.0)+(jj<<7));					/* �e�� */
				HATSUDAN_03_angle65536			= (int_aaa);			/* [�j��] */	/* �e���p�x1024 */
				HATSUDAN_06_n_way				= (11); 				/* [�j��] */	/* [11way] */
				HATSUDAN_07_div_angle65536		= (int)(65536/(7*4));	/* [�j��] */	/* �����p�x([90/360]�x��7����==28����) */
				hatudan_system_regist_n_way();/* (r33-) */
			}
			#if (1)
		//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);/* �i�C�t�̉��͂��炷�K�v������ */
			#endif
		}
	}
	#if 0
	else
	if (/*256-64*/(48/*(64+240)-256*/) < REG_10_BOSS_SPELL_TIMER)
	{;}/* �Ȃɂ����Ȃ� */
	else
	//if (/*256-64*/(0/*(64+192)-256*/) < REG_10_BOSS_SPELL_TIMER)
	{
		/* �i�C�t�̉�����(3��) */
		if (0==((REG_10_BOSS_SPELL_TIMER)&0x0f))
		{
			#if (1)
		//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);/* �i�C�t�̉��͂��炷�K�v������ */
			#endif
		}
	}
	#endif
}


/*---------------------------------------------------------
	�g5�ʒ�-�{�X ���u��p�F�~�X�f�B���N�V����(1/2)�v�ɂ�����Ƃ��������J�[�h
	-------------------------------------------------------
	���@�_���A�S����24�����e
	-------------------------------------------------------
	[���F����bu llet_create_n_way_dan_sa_type()�̎��@�_���̓��U�g�_��Ȃ���e�Ȃ̂ŁA�����Ȃ���Γ�����܂��񂪁A
	���̂��������͋����e�ɂ���\��Ȃ̂ŁA�����Ȃ���Γ�����悤�ɂȂ�\��ł�]
---------------------------------------------------------*/
//SPELL_TIME_0048
local void spell_create_02_24nerai(SPRITE *src)
{
	if (0==((REG_10_BOSS_SPELL_TIMER)&0x03))
	{
		REG_02_DEST_X	= ((src->cx256));
		REG_03_DEST_Y	= ((src->cy256));
		calculate_jikinerai();/* 1:���������쐬������ۂ��B */
	//	HATSUDAN_01_speed256			= (t256(2.0)+((REG_10_BOSS_SPELL_TIMER)<<2)); /* �e�� */
		HATSUDAN_01_speed256			= (t256(2.0)-((REG_10_BOSS_SPELL_TIMER)));	/* �e�� */
		HATSUDAN_02_speed_offset		= t256(1);/*(�e�X�g)*/
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_2000_EFFECT_MINI)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_KUNAI12_BASE + TAMA_IRO_01_AKA);			/* [�ԃN�i�C�e] */
		HATSUDAN_06_n_way				= (24); 							/* [24way] */
		HATSUDAN_07_div_angle65536		= (int)(65536/24);					/* �����p�x */
		hatudan_system_regist_katayori_n_way();/* (r33-) */
		#if (1)
	//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
		#endif
	}
}


/*---------------------------------------------------------
	�g5�ʒ�-�{�X ��� �u��p�F�~�X�f�B���N�V����(2/2)�v�ɂ�����Ƃ��������J�[�h
	-------------------------------------------------------
	���@�_���A11�e(���S�ƁA���E��5�e���_��)
	-------------------------------------------------------
	[���F����bu llet_create_n_way_dan_sa_type()�̎��@�_���̓��U�g�_��Ȃ���e�Ȃ̂ŁA�����Ȃ���Γ�����܂��񂪁A
	���̂��������͋����e�ɂ���\��Ȃ̂ŁA�����Ȃ���Γ�����悤�ɂȂ�\��ł�]
	-------------------------------------------------------
	������Ȃ�������A�����͂R�񌂂B
---------------------------------------------------------*/
//SPELL_TIME_0064
local void spell_create_03_11nife(SPRITE *src)
{
	if (0==((REG_10_BOSS_SPELL_TIMER)&0x07))
	{
		REG_02_DEST_X	= ((src->cx256));
		REG_03_DEST_Y	= ((src->cy256));
		calculate_jikinerai();/* 1:���������쐬������ۂ��B */
		HATSUDAN_01_speed256				= (t256(2.0));						/* �e�� */
		HATSUDAN_02_speed_offset			= t256(0);/*(�e�X�g)*/
		HATSUDAN_03_angle65536				= ((HATSUDAN_03_angle65536))-(int)(65536*6/(7*4));		/* �e���p�x1024 */
		HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_2000_EFFECT_MINI)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
		HATSUDAN_05_bullet_obj_type 		= BULLET_KNIFE20_04_AOI;			/* [�i�C�t�e] */
		HATSUDAN_06_n_way					= (11); 							/* [11way] */
		HATSUDAN_07_div_angle65536			= (int)(65536/(7*4));				/* �����p�x([90/360]�x��7����==28����) */
		hatudan_system_regist_katayori_n_way();/* (r33-) */
		#if (1)
	//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
		#endif
	}
}


/*---------------------------------------------------------
	�g5�ʃ{�X ��� �u�ʏ�U��1(1/2)�v�ɂ�����Ƃ��������J�[�h(�\��)
	-------------------------------------------------------
	24way =(�E12way)+(��12way)
	-------------------------------------------------------
---------------------------------------------------------*/
local void spell_create_04_pink_hearts(SPRITE *src)
{
	if (0==((REG_10_BOSS_SPELL_TIMER)&0x07))/* �{���� 8 �e(128==0x80==spell_time_out) */
	{
			HATSUDAN_02_speed_offset		= t256(1/*0*/);/*(�e�X�g)*/
			HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_3000_EFFECT_NONE)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
			HATSUDAN_05_bullet_obj_type 	= BULLET_KNIFE20_06_YUKARI; 		/* [�s���N�i�C�t�e] */
			REG_03_DEST_Y						= (src->cy256); 	/* �e��y256 �{�X���S���甭�e�B */
		//	���e�����ʒu�̏C��
			REG_02_DEST_X						-= t256(4.0);		/* �e��x256 �{�X�����֔��e�B */
		//
		int jj;
		jj = 0;
		int ii;
		for (ii=0; ii<(13*12*4)/*144*/; ii+= (13*4) )/*12����*/ 						/* �p�x(128[(90+a)/360�x]�� 10 ����) */
		{
			HATSUDAN_01_speed256			= (((160*4)-ii));						/* �e�� */
			//
		//	REG_02_DEST_X						= (src->cx256) + t256(4.0); 			/* �e��x256 �{�X�E�����甭�e�B */
			REG_02_DEST_X						+= t256(8.0);			/* �e��x256 �{�X�E�����甭�e�B */
			HATSUDAN_03_angle65536			= (jj/*(0+i+i)&(65536-1)*/);
			hatudan_system_regist_single();/* (r33-) */
			//
		//	REG_02_DEST_X						= (src->cx256) - t256(4.0); 			/* �e��x256 �{�X�������甭�e�B */
			REG_02_DEST_X						-= t256(8.0);			/* �e��x256 �{�X�������甭�e�B */
			HATSUDAN_03_angle65536			= ((0-jj)&(65536-1));
			hatudan_system_regist_single();/* (r33-) */
			//
			jj += (13*2*64);
		}
		#if (1)
	//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_SAKUYA_SHOT02);
		#endif
	}
}


/*---------------------------------------------------------
	�g5�ʃ{�X ��� �u�ʏ�U��1(2/2)�v�ɂ�����Ƃ��������J�[�h(�\��)
	-------------------------------------------------------
	32�����ɕ����A�����͒ᑬ�e�A��͍����e�B
	16������2��o�^����B
	-------------------------------------------------------
	32����	�p�x(1024[360/360�x]�� 32 ����)1024 == 32 x 32
---------------------------------------------------------*/
local void spell_create_05_32way_dual(SPRITE *src)
{
	if (0==((REG_10_BOSS_SPELL_TIMER)&0x3f))
	{
		HATSUDAN_02_speed_offset			= t256(1/*0*/);/*(�e�X�g)*/
		HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_3000_EFFECT_NONE)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
		HATSUDAN_05_bullet_obj_type 		= BULLET_KNIFE20_04_AOI;			/* [�i�C�t�e] */
		HATSUDAN_06_n_way					= (16); 							/* [16way] */
		HATSUDAN_07_div_angle65536			= (int)(65536/(16));		/* �����p�x(65536[360/360�x]�� 16 ����) */	/* 1����n���������p�x */
	//
		HATSUDAN_01_speed256				= (t256(0.75)); 		/* �����͒ᑬ�e */
		HATSUDAN_03_angle65536				= (0);					/* �e���p�x65536 */
		hatudan_system_regist_katayori_n_way();/* (r33-) */
	//
		HATSUDAN_01_speed256				= (t256(1.25)); 		/* ��͍����e */
		HATSUDAN_03_angle65536				= (65536/32);			/* �e���p�x65536 */
		hatudan_system_regist_katayori_n_way();/* (r33-) */
		#if (1)
	//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
		#endif
	}
}


/*---------------------------------------------------------
	3�u�ʏ�U��1�v�x���e�Ƃ̑g�ݍ��킹
---------------------------------------------------------*/
local void spell_create_2a_sakuya_baramaki1(SPRITE *src)
{
//	if (64 == (REG_10_BOSS_SPELL_TIMER))
//	{
//	}
//	else
	if (64 > (REG_10_BOSS_SPELL_TIMER))
	{
		spell_create_04_pink_hearts(src);
	}
	else
	if (0 == ((REG_10_BOSS_SPELL_TIMER)&0x7f))	/* 128 256 384 512 (640 768) */
	{
		spell_create_05_32way_dual(src);
	}
}


/*---------------------------------------------------------
	5�u�ʏ�U��3�v�����e�Ƃ̑g�ݍ��킹
---------------------------------------------------------*/
local void spell_create_2b_sakuya_baramaki2(SPRITE *src)
{
	if (64 > (REG_10_BOSS_SPELL_TIMER))
	{
		spell_create_04_pink_hearts(src);
	}
	else
//	if (0 == ((REG_10_BOSS_SPELL_TIMER)&0xff))	/* 256 512 (768) */
	if (0 == ((REG_10_BOSS_SPELL_TIMER)&0xff))	/* 256 512 (768) */
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
local void spell_create_06_luna_clock_32way(SPRITE *src)
{
	if (0x40==((REG_10_BOSS_SPELL_TIMER)&0xcf))/* 4�� */
	{
		HATSUDAN_01_speed256			= (t256(1.0));				/* �e�� */
		HATSUDAN_02_speed_offset		= t256(1/*0*/);/*(�e�X�g)*/
		HATSUDAN_03_angle65536			= (0);						/* ���˒��S�p�x / ����@�\(���@�_��/��) */
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
		HATSUDAN_05_bullet_obj_type 	= BULLET_CAP16_04_TUTU_SIROI;	/* [������e] */	/* �e�O�� */
		HATSUDAN_06_n_way				= (32); 					/* [32way] */		/* ���e�� */
		HATSUDAN_07_div_angle65536		= (int)(65536/(32));		/* �����p�x(1024[360/360�x]�� 32 ����) */	/* 1����n���������p�x */
		hatudan_system_regist_katayori_n_way();/* (r33-) */
		#if (1)
	//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
		#endif
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
local void spell_create_07_80way_dual_five(SPRITE *src)
{
	if (0==((REG_10_BOSS_SPELL_TIMER)&0x3f))
	{
			HATSUDAN_02_speed_offset		= t256(1/*0*/);/*(�e�X�g)*/
			HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
			HATSUDAN_05_bullet_obj_type 	= BULLET_KNIFE20_04_AOI;				/* [�i�C�t�e] */
	//
		int ii; 	/* 1����65536���������p�x */
		int j;		/* �J�E���^ */
		j=0;
		for (ii=0; ii<(0x10000); ii += 819 )/* 819.2(65536/80)	12.8==(1024/80) 80���� [5����] */	/* 204.8==(1024/5) �p�x(1024[72/360�x]�� 5 ����) */
		{
			j++;
			j &= (16-1);/* 16way(8way[hi to 0]+8way[slow to 0]) */
			HATSUDAN_01_speed256			= (t256(3.0)-((j)<<5)-((j&1)<<7));		/*��͍����e*/
			HATSUDAN_03_angle65536			= (((0-ii))&(65536-1)); 			/* �p�x */
			hatudan_system_regist_single();/* (r33-) */
		}
		#if (1)
	//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
		#endif
	}
}
/*---------------------------------------------------------
	�ʏ�U���A�ԃi�C�t
	��CW�A�Ԃ�CCW
---------------------------------------------------------*/
/*---------------------------------------------------------
	[�������Z�N�V����]
	-------------------------------------------------------
---------------------------------------------------------*/

local void spell_init_2c_sakuya_blue_red_knife(SPRITE *src)
{
	card.danmaku_callback[0] = danmaku_01_standard_angle_sayuu_hansya_mover;/*(�ʏ�e�p)*/	/*(��ʊO�Ȃ甽�ˌ���)*/
//	card.danmaku_callback[1] = NULL;/*(���g�p)*/
//	card.danmaku_callback[2] = NULL;/*(���g�p)*/
//	card.danmaku_callback[3] = NULL;/*(���g�p)*/
}
/*---------------------------------------------------------
	[���e�Z�N�V����]
	-------------------------------------------------------
---------------------------------------------------------*/
local void spell_create_2c_sakuya_blue_red_knife(SPRITE *src)
{
	if ((0!=((REG_10_BOSS_SPELL_TIMER)&0x0180)))
	{
		if (0==((REG_10_BOSS_SPELL_TIMER)&0x3f))
		{
			unsigned int kk;
			kk = (((REG_10_BOSS_SPELL_TIMER)&0x0100)>>8);
				HATSUDAN_02_speed_offset		= t256(1/*0*/);/*(�e�X�g)*/
				HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
				HATSUDAN_05_bullet_obj_type 	= BULLET_KNIFE20_04_AOI+kk; 			/* [�i�C�t�e] */
		//
			int ii; 	/* 1����65536���������p�x */
			int j;		/* �J�E���^ */
			j = 0;
			for (ii=0; ii<(0x10000); ii += 819 )/* 819.2(65536/80)	12.8==(1024/80) 80���� [5����] */	/* 204.8==(1024/5) �p�x(1024[72/360�x]�� 5 ����) */
			{
				j++;
				j &= (16-1);/* 16way(8way[hi to 0]+8way[slow to 0]) */
				HATSUDAN_01_speed256			= (t256(3.0)-((j)<<5)-((j&1)<<7));		/*��͍����e*/
				HATSUDAN_03_angle65536			= ((kk?(0-ii):(ii))&(65536-1)); 			/* �p�x */
				hatudan_system_regist_single();/* (r33-) */
			}
			#if (1)
		//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
			#endif
		}
	}
}


#if 0
/*---------------------------------------------------------
	��11�`��: �ŏI�`��(����3)
---------------------------------------------------------*/
local void spell_create_47_sakuya_meek(SPRITE *src)
{
	if (0==(REG_10_BOSS_SPELL_TIMER))
	{
		add_zako_sakuya_doll_01_laser(src);/* ���[�U�[�ǉ� */
	}
//
	src->color32	= MAKE32RGBA(0xff, 0xff, 0xff, 0xff);		/*	src->alpha = 0xff;*/
//
}
#endif

/*---------------------------------------------------------
	���C�h��Z�u����h�[���v
---------------------------------------------------------*/
#if 0
#endif





/*
	���H�u�W���b�N�E�U�E���h�r���v�̌����B
	���̃J�[�h�͎n�߂�[�ԑ�e]�������_�����ۂ��΂�T���B
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
	-------------------------------------------------------
---------------------------------------------------------*/



/*---------------------------------------------------------
	[�e���O���[�v(1)�Z�N�V����]
	-------------------------------------------------------
	�ԑ�e�p
---------------------------------------------------------*/

local void sakuya_danmaku_01_callback(SPRITE *src)
{
		SPRITE *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
	//	/*[(24-12)x8]*/if ((32-8)==(REG_09_REG1))	// 192== (64*3)
	if ((HATUDAN_ITI_NO_JIKAN-((24-18)*8)) == src->jyumyou)/* ���e�G�t�F�N�g�ォ��[??]�J�E���g�o�߂����e */
	{
		/* ��e���A�S�ăX�g�b�v */
		/* ��e���A�ԑ�e�֐F�ύX�B */
		src->hatudan_register_tra65536				= t256(0);		/* ���������e */
		src->hatudan_register_speed65536				= t256(0);		/* �e�� */
		src->type									= (BULLET_OODAMA32_01_AKA); 	/* [�ԑ�e](��==����) */
		reflect_sprite_spec444(src, OBJ_BANK_SIZE_00_TAMA); 	/* �e�O���ƒe�����蔻���ύX����B */
//	}
//	else
//	if ((32-8)==(REG_08_REG0))	// 192== (64*3)
//	{
	//	���̔��e�ʒu��ݒ�B	/* �e��x256 y256 ��ʒu01�֌����Ĕ��e�B */
		REG_00_SRC_X	= t256(0);					/* �e��x256 */
		REG_01_SRC_Y	= t256(256);				/* �e��y256 */
		goto exchange_damnaku_sakuya_tuika;/*�i�C�t�ǉ�*/
	}
	else
	//	/*[(25-12)x8]*/if ((32-7)==(REG_09_REG1))		// 192== (64*3)
	if ((HATUDAN_ITI_NO_JIKAN-((25-18)*8)) == src->jyumyou)/* ���e�G�t�F�N�g�ォ��[??]�J�E���g�o�߂����e */
	{
	//	���̔��e�ʒu��ݒ�B	/* �e��x256 y256 ��ʒu02�֌����Ĕ��e�B */
		REG_00_SRC_X	= t256(GAME_WIDTH); 		/* �e��x256 */
		REG_01_SRC_Y	= t256(256);				/* �e��y256 */
		goto exchange_damnaku_sakuya_tuika;/*�i�C�t�ǉ�*/
	}
	else
	//	/*[(26-12)x8]*/if ((32-6)==(REG_09_REG1))		// 192== (64*3)
	if ((HATUDAN_ITI_NO_JIKAN-((26-18)*8)) == src->jyumyou)/* ���e�G�t�F�N�g�ォ��[??]�J�E���g�o�߂����e */
	{
	//	���̔��e�ʒu��ݒ�B		/* �e��x256 y256 �v���C���[���S�֌����Ĕ��e�B */
		REG_00_SRC_X	= zzz_player->cx256;			/* �e��x256 */
		REG_01_SRC_Y	= zzz_player->cy256;			/* �e��y256 */
		goto exchange_damnaku_sakuya_tuika;/*�i�C�t�ǉ�*/
	}
	else
	/*(�i�C�t�̔��e�G�t�F�N�g�̕��Ƒ�ʂ̔��e�G�t�F�N�g�̕��łQ�񕪂��炷�B)*/
	//	if ((32-5)==(REG_09_REG1))		// 192== (64*3)
	//	/*[(29-12)x8]*/if ((32-3)==(REG_09_REG1))		// 192== (64*3)
	if ((HATUDAN_ITI_NO_JIKAN-((29-18)*8)) == src->jyumyou)/* ���e�G�t�F�N�g�ォ��[??]�J�E���g�o�߂����e */
	{/* 29 */
		/*(�ĉ���)*/
	//	int jj = 0;
	//	jj++;
	//	jj &= 0x07; 	// 2 3 4 5	6 7 8 9
	//	jj &= 0x03; 	// 3 4 5 6
	//	src->hatudan_register_tra65536	= t256(6);			/* ���������e */
	//	src->hatudan_register_tra65536	= (jj<<8)+t256(2);	/* (2 ... 9) ���������e */
	//	src->hatudan_register_tra65536	= (jj<<8)+t256(3);	/* (3 ... 6) ���������e */
		src->hatudan_register_tra65536	= t256(3);			/* (3 ... 6) ���������e */
		/* (1�͒x�������̂ŏ��O) 9 ���Ƒ��߂��邩�� */
		/* (�ʏ�e�֕ϐg����) */
		src->hatudan_register_spec_data = (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
	//	goto my_quit;
	}
	goto my_quit;
	/*---------------------------------------------------------
		�i�C�t�ǉ��T�u���[�`�������B(�J�[�h�X�N���v�g�������ꍇ�A�T�u���[�`�������͏o�������̂ŋz������K�v����)
	---------------------------------------------------------*/
exchange_damnaku_sakuya_tuika:
	{
		/* �e�̌��ݍ��W�Ƀi�C�t�e��o�^����B */
		REG_02_DEST_X	= ((src->cx256));	/* �e�̌��ݍ��W */
		REG_03_DEST_Y	= ((src->cy256));	/* �e�̌��ݍ��W */
		if (
			( (16*256) > abs(REG_02_DEST_X-REG_00_SRC_X) ) &&
			( (16*256) > abs(REG_03_DEST_Y-REG_01_SRC_Y) )
		)/*(�X�N���v�g���炠���蔻������@�\�����肻�����ȁB)*/
		{
			;/*(���e�ʒu���߂�����ꍇ�A�ǉ����Ȃ�)*/
		}
		else
		{
			HATSUDAN_01_speed256		= (t256(0));			/* �e�� */
			HATSUDAN_02_speed_offset	= t256(0);/*(�e�X�g)*/
			HATSUDAN_04_tama_spec		= (DANMAKU_LAYER_02)|(TAMA_SPEC_3000_EFFECT_NONE)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
			HATSUDAN_05_bullet_obj_type = BULLET_KNIFE20_04_AOI;			/* [�i�C�t�e] */
		//	���e�ʒu���󂯎��B
		//	���e�ʒu��ݒ肷��B
			{
				tmp_angleCCW65536_src_nerai();/* src�_���p�쐬 */
				hatudan_system_regist_single();/* (r33-) */
			}
			voice_play(VOICE16_BOSS_KYUPIN, TRACK04_TEKIDAN);/*(�i�C�t�ǉ���)*/
		}
	}
my_quit:
	;
	#if (0)
	/* ���e�� 512 �J�E���g�ȏ�͒e�������B */
	if ((HATUDAN_ITI_NO_JIKAN-512) > src->jyumyou/*(�����Ȃ̂Ŏ����Ō���)*/)/* ���e�G�t�F�N�g�ォ��[512]�J�E���g�ȏ�o�߂����e */
	{
		/* (�ʏ�e�֕ϐg����) */
		src->hatudan_register_spec_data = (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
	}
	#endif
//	danmaku_00_standard_angle_mover(src);/*(�p�x�e�ړ�+��ʊO�e����)*/
	hatudan_system_B_move_angle_001(src);/*(�p�x�e�ړ�)*/
}


/*---------------------------------------------------------
	[�e���O���[�v(2)�Z�N�V����]
	-------------------------------------------------------
	�ǉ������i�C�t�e�p
---------------------------------------------------------*/

local void sakuya_danmaku_02_callback(SPRITE *src)
{
	/*(���ʂ�8���邪�Ƃ肠�����̑Ή�)*/
	if (29==REG_09_REG1)
	{
		/*(�~�߂Ă���i�C�t���A�΃i�C�t�ɂȂ蓮���o���B)*/
	//	src->hatudan_register_tra65536	= (jj<<8)+t256(3);	/* (3 ... 6) ���������e */
		src->hatudan_register_tra65536	= t256(2);			/* (3 ... 6) ���������e */
		//
		src->type						= BULLET_KNIFE20_07_MIDORI;/* (���΃i�C�t�ɕϐg) */
		reflect_sprite_spec444(src, OBJ_BANK_SIZE_00_TAMA); 	/* �e�O���ƒe�����蔻���ύX����B */
		//
		/* (�ʏ�e�֕ϐg����) */
		src->hatudan_register_spec_data = (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
	}
	#if (0)
	/* ���e�� 512 �J�E���g�ȏ�͒e�������B */
	if ((HATUDAN_ITI_NO_JIKAN-512) > src->jyumyou/*(�����Ȃ̂Ŏ����Ō���)*/)/* ���e�G�t�F�N�g�ォ��[512]�J�E���g�ȏ�o�߂����e */
	{
		/* (�ʏ�e�֕ϐg����) */
		src->hatudan_register_spec_data = (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
	}
	#endif
//	danmaku_00_standard_angle_mover(src);/*(�p�x�e�ړ�+��ʊO�e����)*/
	hatudan_system_B_move_angle_001(src);/*(�p�x�e�ړ�)*/
}
/*---------------------------------------------------------
	[�������Z�N�V����]
	-------------------------------------------------------
---------------------------------------------------------*/
local void spell_init_14_sakuya_jack_oodama32(SPRITE *src)
{
	REG_0a_REG2 	= const_init_nan_ido_table[JACK_14_TAMAKAZU+(REG_0f_GAME_DIFFICULTY)];
//	card.danmaku_callback[0] = danmaku_00_standard_angle_mover;/*(�ʏ�e�p)*/
	card.danmaku_callback[1] = sakuya_danmaku_01_callback;/*(�ԑ�e�p)*/
	card.danmaku_callback[2] = sakuya_danmaku_02_callback;/*(�i�C�t�e�p)*/
//	card.danmaku_callback[3] = NULL;/*(���g�p)*/
}
/*---------------------------------------------------------
	[���e�Z�N�V����]
	-------------------------------------------------------
	�g�p���W�X�^
	REG_08_REG0 	�J�E���^�B(8���1�񔭒e����)
	REG_09_REG1 	�J�E���^�B(32��Ń��[�v==(SPELL_TIME_0256==256/8))
	REG_0a_REG2 	��Փx�ʒ萔1
	REG_0b_REG3 	�J�E���^�B(ii)�c��񐔁B
	REG_0c_REG4 	�J�E���^�B(kk)
	REG_0d_REG5 	�����p�x�A�ꎞ�ۑ��p�B(ra_nd32)
---------------------------------------------------------*/
local void spell_create_14_sakuya_jack_oodama32(SPRITE *src)
{
	count_up_limit_NUM(REG_NUM_08_REG0, 8);//	�J�E���^�B(8���1�񔭒e����)
	if (1==(REG_08_REG0))	/* 1���( 0 �ɏ���������āA�J�E���g�A�b�v���� 1 �ɂȂ�̂ŁB) */
	{
		count_up_limit_NUM(REG_NUM_09_REG1, 32);//	�J�E���^�B(32��Ń��[�v)
		/* (r33)���e�G�t�F�N�g���ǉ����ꂽ�̂ŁA�ǉ����Ԃ𑁂߂āA���덇�킹�B */
	//	if (((12)+(HATUDAN_FRAME64/8))==(REG_08_REG0))	// 192== (64*3)
		/*[12x8]*/if (((32-20))==(REG_09_REG1)) 		// 192== (64*3)
		{
			HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_01)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
			HATSUDAN_05_bullet_obj_type 	= BULLET_OODAMA32_00_AOI;			/* [��e](��==�Ö�) */
		//
			REG_0c_REG4 = (0);
		//	mm = 0; /* �J�E���^ */
		//	jj = (1024-960);
			REG_0d_REG5 = ra_nd();/*(�����p)*/
			REG_0b_REG3 = REG_0a_REG2;/*(��Փx�ʂɎc��񐔂�ݒ�B)*/	/* 32�e�΂�T�� */
		loop:
			{
			/* 960==30720/32,  30720 == 32768 -1024 -1024 */
			/* 936 = 936.228571428571428571428571428571 == (32768/(32+2���E�̗]�T��))	*/
			//	jj += (960);/* (=936)< 1024 [180�x(32768)���኱�����͈͂ɂ΂�T��] */
			//	REG_0c_REG4 += 386;
				REG_0c_REG4 += REG_0d_REG5;/*(�����p���Z)*/
			//	REG_0c_REG4 += 42857142;
			//	mm += 42857142;
			//	HATSUDAN_02_speed_offset		= t256(0);/*(�e�X�g)*/
			//	HATSUDAN_02_speed_offset		= t256(8);/*(�e�X�g)*/
			//	HATSUDAN_02_speed_offset		= t256(12);/*(�e�X�g)*/
				HATSUDAN_02_speed_offset		= (t256(4)+((REG_0c_REG4&0x07)<<8));/*(�e�X�g)*/
			//	HATSUDAN_01_speed256			= ((REG_0c_REG4)&(512-1))+t256(0.25);	/*��͍����e*/
				HATSUDAN_01_speed256			= ((REG_0c_REG4)&(1024-1))+t256(0.25);	/*��͍����e*/
				HATSUDAN_03_angle65536			= ((REG_0c_REG4)&(32768-1))+(int)(65536*3/4);	/* �p�x */
			//	HATSUDAN_03_angle65536			= ((mm)&(1024-1))+jj+(int)(65536*3/4);			/* �p�x */
				hatudan_system_regist_single();/* (r33-) */
			}
			REG_0b_REG3--;/*(�c��񐔌��炷�B)*/
			if (0 < REG_0b_REG3)/*(�c��񐔂�����΃��[�v�B)*/
		//	if (0 != REG_0b_REG3)/*(�c��񐔂�����΃��[�v�B)*/
			{
				goto loop;
			}
			#if (1)
		//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
			#endif
		}
	}
}
