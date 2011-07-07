
/*---------------------------------------------------------
 �����͕핗 �` Toho Imitation Style.
  �v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�`���m�̒e���𐶐����܂��B
---------------------------------------------------------*/

/*---------------------------------------------------------
	�g2�ʃ{�X �`���m	�ʏ�U��1(1/3)	���@�_���g�U�e(1-2-3-4-5-6-7-8�e)
	-------------------------------------------------------
	(1-2-3-4-5-6-7�e)���Ǝv���Ă����ǁA�����蒼������A
	(1-2-3-4-5-6-7-8�e)�������̂ŏC���B
���[��
	�����蒼���Ă悭������A
	(1-2-3-4-5-6-7�e)�� 			(easy�Ƃ�)
	(3-4-5-6-7-8�e)�B				(hard)
	(3-4-5-6-7-8�e)�B				(lunatic)

	�`���m�͂ǂ����u���������N�v�ňႤ�e�������Ă���B�̂ŗǂ�����Ȃ��B


1100 0111 0xc7
0100 0000
--11 0111 0x37


	-------------------------------------------------------
0x38(56) --0
0x30(48) --1
0x28(40) --2 �S���ʐg�ےe
0x20(32) --3 �S���ʐg�ےe
0x18(24) --4
0x10(16) --5
0x08( 8) --6
0x00( 0) --7
	-------------------------------------------------------
	lunatic�Ƃ����ƑS���ʐg�ےe�������Ɍ����Ă���B(2�񂮂炢)
---------------------------------------------------------*/
static void spell_create_10_cirno_misogi(SPRITE *src)
{
//	if ((64-1)==((src->boss_base_spell_time_out)))
	if ((SPELL_TIME_0060-1)==((src->boss_base_spell_time_out)))
	{
		SPRITE *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
		tmp_angleCCW65536_src_nerai(zzz_player, src);/* ���@�_���p�쐬 */
	}
//	else
//	if (0x40==((src->boss_base_spell_time_out)&0xc7))/* 8��(�C��1-2-3-4-5-6-7�e:0way�͌����Ȃ��̂Ō��̂�7��) */
	if (0x00==((src->boss_base_spell_time_out)&0x07))/* 8��(�C��1-2-3-4-5-6-7�e:0way�͌����Ȃ��̂Ō��̂�7��) */
	{
		#if (1)
	//	voice_play(VOICE13_SAKUYA_SHOT02, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE13_SAKUYA_SHOT02);
		#endif
	//
		int n_way;
		n_way = 0;
		n_way = ((/*16*/7-((src->boss_base_spell_time_out)>>3))&7)+(1)+((cg_game_difficulty)>>1);
		if (n_way)
		{
			obj_send1->cx256							= (src->cx256); 	/* �e��x256 �{�X���S���甭�e�B */
			obj_send1->cy256							= (src->cy256); 	/* �e��y256 �{�X���S���甭�e�B */
		//
			br.BULLET_REGIST_00_speed256				= (t256(2.0));				/* �e��(2.5) */
			br.BULLET_REGIST_01_speed_offset			= t256(6);/*(�e�X�g)*/

		//	br.BULLET_REGIST_02_angle65536				= (src->tmp_angleCCW65536)-(n_way*65536/128);
			br.BULLET_REGIST_02_angle65536				= (src->tmp_angleCCW65536);

			br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)�W���e */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KOME_03_AOI;		/* [�Ēe] �ɂȂ��Ă�̂͌��� �X�e �̃O������������ */
		//����br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* �W���e�ɐݒ肷��(r33-) */

			br.BULLET_REGIST_06_n_way					= n_way;/* [nway] */
			br.BULLET_REGIST_07_div_angle65536			= (int)(65536/(64));		/* �����p�x(1024[360/360�x]�� 64 ����) */	/* 1����n���������p�x */
			tama_system_regist_n_way();/* (r33-) */
		}
	}
#if 1
	if (0x20==((src->boss_base_spell_time_out)&0x37))/* 2�� */
	{
		if (0 != spell_bunkatsu_nums_table[b_chrno_00_BLUE_NUMS+(cg_game_difficulty)])
		{
			// hard 18����(1��)
			// Luna 36����(2��)
			set_sub_jikinerai(src);/* 1:���������쐬������ۂ��B */
		//

			br.BULLET_REGIST_00_speed256				= (t256(2.0));				/* �e��(2.5) */
			br.BULLET_REGIST_01_speed_offset			= t256(1);/*(�e�X�g)*/

			br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_8000_NON_TILT);/* (r33-)��X���e */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_WAKU12_03_AOI; 	/* [�g�t���ےe] */
		//����br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_01_ANGLE_NON_TILT;/* (r33-)��X���e */

			br.BULLET_REGIST_06_n_way					= spell_bunkatsu_nums_table[b_chrno_00_BLUE_NUMS+(cg_game_difficulty)];//(36);/* [nway] */
			br.BULLET_REGIST_07_div_angle65536			= spell_nan_ido_table[chrno_04_BLUE_DIV_ANGLE+(cg_game_difficulty)];//(int)(65536/(36));		/* �����p�x(1024[360/360�x]�� 64 ����) */	/* 1����n���������p�x */
			tama_system_regist_katayori_n_way();/* (r33-) */
		}
	}
#endif /*0000*/
}


/*---------------------------------------------------------
	�g2�ʃ{�X �`���m	�X���u�A�C�V�N���t�H�[���v
	-------------------------------------------------------

---------------------------------------------------------*/
static void spell_create_1b_cirno_icecle_fall(SPRITE *src)
{
//	if ((64-1)==((src->boss_base_spell_time_out)))
	if ((SPELL_TIME_0060-1)==((src->boss_base_spell_time_out)))
	{
		SPRITE *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
		tmp_angleCCW65536_src_nerai(zzz_player, src);/* ���@�_���p�쐬 */
	}
//	else
//	if (0x40==((src->boss_base_spell_time_out)&0xc7))/* 8��(�C��1-2-3-4-5-6-7�e:0way�͌����Ȃ��̂Ō��̂�7��) */
	if (0x00==((src->boss_base_spell_time_out)&0x07))/* 8��(�C��1-2-3-4-5-6-7�e:0way�͌����Ȃ��̂Ō��̂�7��) */
	{
		#if (1)
	//	voice_play(VOICE13_SAKUYA_SHOT02, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE13_SAKUYA_SHOT02);
		#endif
	//
			obj_send1->cx256							= (src->cx256); 	/* �e��x256 �{�X���S���甭�e�B */
			obj_send1->cy256							= (src->cy256); 	/* �e��y256 �{�X���S���甭�e�B */
		//
		int jj;
		for (jj=0; jj<5; jj++ )/* �S�����ׂ�B */
		{
			br.BULLET_REGIST_00_speed256				= (t256(2.0)-(jj<<5));				/* �e��(2.5) */
			br.BULLET_REGIST_01_speed_offset			= t256(6);/*(�e�X�g)*/

			br.BULLET_REGIST_02_angle65536				= (0);	/* ������ */
			br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)�W���e */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KOME_03_AOI;		/* [�Ēe] �ɂȂ��Ă�̂͌��� �X�e �̃O������������ */
		//����br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* �W���e�ɐݒ肷��(r33-) */
			br.BULLET_REGIST_06_n_way					= (2);/* [nway] */
			br.BULLET_REGIST_07_div_angle65536			= (int)(65536/(5))+((src->boss_base_spell_time_out)<<7);		/* �����p�x(1024[360/360�x]�� 64 ����) */	/* 1����n���������p�x */
			tama_system_regist_n_way();/* (r33-) */
		}
	}
	if (0x20==((src->boss_base_spell_time_out)&0x37))/* 2�� */
	{
		if (0 < (cg_game_difficulty) )
		{
			set_sub_jikinerai(src);/* 1:���������쐬������ۂ��B */
			br.BULLET_REGIST_00_speed256				= (t256(2.0));				/* �e��(2.5) */
			br.BULLET_REGIST_01_speed_offset			= t256(1);/*(�e�X�g)*/

			br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_8000_NON_TILT);/* (r33-)��X���e */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_WAKU12_06_KI_IRO;		/* [�g�t�����ےe] */
		//����br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_01_ANGLE_NON_TILT;/* ��X���e�ɐݒ肷��(r33-) */
			br.BULLET_REGIST_06_n_way					= (5);/* [nway] */
			br.BULLET_REGIST_07_div_angle65536			= (int)(65536/(36));		/* �����p�x(1024[360/360�x]�� 64 ����) */	/* 1����n���������p�x */
			tama_system_regist_n_way();/* (r33-) */
		}
	}
}


/*---------------------------------------------------------
	�g2�ʃ{�X �`���m	�ʏ�U��1(2/3)	���@�_���S���ʒe
	-------------------------------------------------------
---------------------------------------------------------*/
#if 0
#endif


/*---------------------------------------------------------
	�g2�ʃ{�X �`���m	�ʏ�U��1(3/3)	���@�_�������e
	-------------------------------------------------------
---------------------------------------------------------*/
#if 0
#endif


/*---------------------------------------------------------
	�p�[�t�F�N�g�t���[�Y�̌���
	-------------------------------------------------------
	�{���̓p�[�t�F�N�g�t���[�Y�ɗ����͈�������̂ł́H
	�ƌl�I�ɂ͍l���Ă���B
	-------------------------------------------------------
	�Ƃ肠�����A���O�̗��������֐�(�P�Ɏ����̒����֐�)���A���ƕi���̗ǂ��������o���̂ŁA
	�������׍H���Ȃ��Ă��΂�Ȃ��̂œ��ɖ��Ȃ������B
	�ł��ǂ����Ȃ�S���Ă݂�B
//
	�܂��`���m�͖��t���[���e���΂�T���Ă���Ɍ��܂��Ă���̂�(���̑O�񂪂��������\���͂��܂薳���Ǝv����)�B
	�`���m���΂�T���e�ʂ���A1�b�Ԃ�60�e���x�A1�b�Ԃ�120�e���x�A1�b�Ԃ�180�e���x�̒��ł́A1�b�Ԃ�120�e���x���߂��B
	�]����1�t���[��2�e�Ƃ���B
//
	�p�b�g���ɂ͔���Ȃ����A�`���m�̗����e�͂��邮����Ȃ���A�΂��ďo�Ă���B�炵���̂ŁB
	�ǂꂮ�炢�̑��x�ŉ��̂Ȃ�A����Č����Ȃ������������Ă݂�B(����͌����Ȃ���Δ���Ȃ����x)
	(16����==)��1�b��(64�t���[��)��4��(0.25�b��1��)���ƁA�I���ɉ���Ă��銴���B�Ȃ̂ŁA����ł͖��炩�ɂ��������B
	(8����==)��1�b��(64�t���[��)��8��(0.125�b��1��)���ƁA�����Ȃ���Δ���Ȃ����x�B
	(4����==)��1�b��(64�t���[��)��16��(0.0625�b��1��)���ƁA�����̈悪���������ς����āA��������Ӗ����Ȃ��̂ł͂Ȃ����H�B
	1�t���[��2�e�Ƒ��e�ʂ����܂��Ă���ׁA���񐔂𑝂��΂��̕��A�����̈悪�L���Ȃ�A��������Ӗ����Ȃ��Ȃ�B
	(������1�͕������������Ȃ��̂Ɠ���������)
	�܂�A�e�ʂ�1�t���[��2�e�̏ꍇ�A8�����ɋ߂����̂������肦�Ȃ��Ƃ�����������B
//
	��L�̎�������A1�t���[���ӂ�� 8����1���x�̗̈�� 2�e���m�肵���B
	�������A���̗̈�͌����� 8����1 �ł͂Ȃ��B
	�Ȃ��Ȃ�A���̗̈悪 n����1�Ŋ���؂��ƁA����Č����₷����ɃQ�[���I�Ɉ��S�n�т��o���₷���B
	������A���̗̈�͌����� n����1 �ł͂Ȃ��B
	65536/8 == 8192�B65536/10 == 6553.6�B�����ɋ߂��l�Ǝv����B
	8����1(8192) > ? > 10����1(6554) �e�L�g�[�� KOTEITI_7777 �ɂ��Ă݂�B
	7777�̏ꍇ 8.42689983284042689983284042689983����1�B(==65536/7777)
//
	��L������ 8�����Ȃ̂ŁA2�e�ڂ̊p�x�́A16 �������ꂽ��́A16 �������x�̗����l(0-4095)���ǂ��Ǝv����B
	( 8���� == 16����(�Œ�l) + 16����(�����l) )
	-------------------------------------------------------
	�� �v�C��:
	�p�[�t�F�N�g�t���[�Y�̌�ǂ��e(��)�͂�蒼������S�R������B
	���ʂ͎~�܂��Ĕ����邩�画��Ȃ������B(���Ă��������U���U�����l���Ȃ�����)
	�Ȃ񂩓����Ȃ��������ƁA�\�����킩��B
	�����_���Ƃ�����Ȃ��āA(�W�����e��)�u�O�Z�b�g�v(�������ۂ�)�����ė���݂����B
	(���@�_��Ȃ�)���@�_�������e�B
	���񎞂Ɏ��@�_���pa, b, c���쐬(?)�B
	a��b��c�͓Ɨ��B
---------------------------------------------------------*/

//#define KOTEITI_7777	(7777<<6)
//#define KOTEITI_7777	(497728)
#define KOTEITI_7777	(7777)

/*---------------------------------------------------------
	�p�[�t�F�N�g�t���[�Y(�g��12�ےe��S�e����������)
	-------------------------------------------------------

---------------------------------------------------------*/

static void exchange_damnaku_chiruno_gensoku(void)
{
//	int check_type;
//	check_type = br.BULLET_REGIST_04_bullet_obj_type;/* ���ׂ�^�C�v���󂯎�� */
	int ii;
	for (ii=0; ii<OBJ_POOL_00_TAMA_MAX; ii++ )/* �S�����ׂ�B */
	{
		SPRITE *s;
		s = &obj99[OBJ_HEAD_00_TAMA+ii];
	//	if (check_type == (s->type) )	/* ���̐e���Ԓe�Ȃ� */
		if (is_tama_grouip08(BULLET_WAKU12_00_SIRO) == is_tama_grouip08(s->type) )	/* �g��12�ےe�Ȃ� */
		{
		//	s_change_meirin_yajirusi_one(s);
		//	s->tama_system_tra65536 					= t256(-6); 		/* (-5.12==98%)���������e x 1.0 */
		//	s->tama_system_tra65536 					= t256(-12);		/* (-5.12==98%)���������e x 2.0 */	/* -12==�����߂� */
			s->tama_system_tra65536 					= t256(-9); 		/* (-5.12==98%)���������e x 1.5 */
		}
	}
}


/*---------------------------------------------------------
	�p�[�t�F�N�g�t���[�Y(�g��12�ےe��S�e��~�B���݂̍��W��V��_���W�ɕύX����B������ς���)
	-------------------------------------------------------

---------------------------------------------------------*/
#if 1
static void exchange_damnaku_chiruno_tomeru(void)
{
//	int check_type;
//	check_type = br.BULLET_REGIST_04_bullet_obj_type;/* ���ׂ�^�C�v���󂯎�� */
	int ii;
	for (ii=0; ii<OBJ_POOL_00_TAMA_MAX; ii++ )/* �S�����ׂ�B */
	{
		SPRITE *s;
		s = &obj99[OBJ_HEAD_00_TAMA+ii];
	//	if (check_type == (s->type) )	/* ���̐e���Ԓe�Ȃ� */
		if (is_tama_grouip08(BULLET_WAKU12_00_SIRO) == is_tama_grouip08(s->type) )	/* �g��12�ےe�Ȃ� */
		{
			s->tama_system_tra65536 					= t256(0);		/* ���������e */
			s->tama_system_speed65536					= t256(0);		/* �e�� */
			s->type 						= (BULLET_WAKU12_00_SIRO);
			reflect_sprite_spec444(s, OBJ_BANK_SIZE_00_TAMA);	/* �e�O���ƒe�����蔻���ύX����B */
		}
	}
}
#endif


/*---------------------------------------------------------
	�p�[�t�F�N�g�t���[�Y(�g��12�ےe��S�e����������)
	-------------------------------------------------------
	�����e(���m�ɂ͓������e)�Ȃ̂ɁA�����e�Ɍ����Ȃ���
	�����e(���m�ɂ͓����x�e�A����==0)�Ɍ������Ⴄ��肪����B

---------------------------------------------------------*/
#if 1
static void exchange_damnaku_chiruno_saikasoku(void)
{
//	int check_type;
//	check_type = br.BULLET_REGIST_04_bullet_obj_type;/* ���ׂ�^�C�v���󂯎�� */
	int add_angle = 0;
	int jj = 0;
	int ii;
	for (ii=0; ii<OBJ_POOL_00_TAMA_MAX; ii++ )/* �S�����ׂ�B */
	{
		SPRITE *s;
		s = &obj99[OBJ_HEAD_00_TAMA+ii];
	//	if (check_type == (s->type) )	/* ���̐e���Ԓe�Ȃ� */
		if (is_tama_grouip08(BULLET_WAKU12_00_SIRO) == is_tama_grouip08(s->type) )	/* �g��12�ےe�Ȃ� */
		{
		//	s_change_meirin_yajirusi_one(s);
			/* �e�̌��ݍ��W���A�V��_���W�Ƃ���B */
			s->tx256 = (s->cx256);		/* ��_���W�ɃZ�b�g */
			s->ty256 = (s->cy256);		/* ��_���W�ɃZ�b�g */
			s->tama_system_radius256 = t256(0); 	/* ��_���W�ɂȂ����̂Ŕ��a�� 0 �ɂ���B */
			/*	*/
			#if 0
		//	s->rotationCCW1024				= (ra_nd()&((1024)-1)); 			/* ���� */	/* �����͗����łȂ��C������ */
			#else
			/* ���Ȃ��Ƃ��������͖{���ɋ߂��B ���A�S�R�Ⴄ�B */
			/* ���Z�Œ�l(KOTEITI_7777)�ȊO�̗v���Ƃ��āA�e�����X�g�̃T�C�Y�Ń��[�v����ׁA�e�����X�g�̃T�C�Y�� */
			/* ���݂� 1024 ����A�{���Ɠ��� 640 �Ɍ��点�΁A����\���͂���B */
			add_angle += (KOTEITI_7777);
			s->rotationCCW1024				= (add_angle>>6);			/* ���� */
			#endif
			jj++;
		//	jj &= 0x07; 	// 2 3 4 5	6 7 8 9
			jj &= 0x03; 	// 3 4 5 6
		//	s_change_meirin_yajirusi_one(s);
		//	s->tama_system_tra65536 					= t256(6);		/* ���������e */
		//	s->tama_system_tra65536 					= (jj<<8)+t256(2);	/* (2 ... 9) ���������e */
			s->tama_system_tra65536 					= (jj<<8)+t256(3);	/* (3 ... 6) ���������e */
			/* (1�͒x�������̂ŏ��O) 9 ���Ƒ��߂��邩�� */
		}
	}
}
#endif
/*---------------------------------------------------------
	�p�[�t�F�N�g�t���[�Y
	1��������10[�b]�Ƃ���B
0x280  0000 0010 1000 0000	>>=6  0x000a
	-------------------------------------------------------
64 x  0 	(640==0x0280) a
	�����_������
64 x  1 	(576==0x0240) 9
	�����_������
64 x  2 	(512==0x0200) 8
	�����_������
64 x  3 	(448==0x01c0) 7 �����J�n
	//����
64 x  4 	(384==0x0180) 6 ��~
	//��~	// ���@�ˑ��ɂ��8��������
64 x  5 	(320==0x0140) 5 �ĉ���
	//�ĉ���
64 x  6 	(256==0x0100) 4
64 x  7 	(192==0x00c0) 3
64 x  8 	(128==0x0080) 2
64 x  9 	( 64==0x0040) 1
64 x 10 	(  0==0x0000) 0
---------------------------------------------------------*/
#if 1
static void spell_create_13_perfect_freeze(SPRITE *src)
{
//	int i;
	// �ŏ��̃����_������ (64*3�t���[��==��3�b)�΂�T���B
	if (((640)-(64*3))<(src->boss_base_spell_time_out)) // 192== (64*3)
	{
		#if (1)
		if (0==((src->boss_base_spell_time_out)&0x07))
		{
		//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		}
		#endif
		{
			/*
				����ł͒e�ɂǂ̐F�����蓖�Ă邩�Ƃ������ɂ͍S�肪����B
				�`���m�� 5��ނ̐F�̒e�������A���e�̐F�̏o��m�����ϓ��ł͂Ȃ��΂��Ă���B
				���̐F�̕΂�́A�v���O�����̓s�����ŋ��R�΂������̂ł͂Ȃ��A��דI�ȉ��o�ł���B
				�����ł̓`���m����(aho)�Ȃ̂ŁA�Q�[���I�Ƀ`���m��ڗ������邽�߂ɐe�����Ȃ�����Ƃ������o�Ӑ}������B
				(�e�������ƃ`���m�������ɂ��邩����Ȃ��Ȃ�Ƃ����Q�[����̓s��������B)
				���̐F�̕΂肪�A�`���m�炵�������o���Ă���̂ŁA�����̍Č��͎��͏d�v���Ǝv���B
				(aho�Ƃ������ݒ�͊֌W�Ȃ���)�e�̐F�̕΂肪����ƈႤ�ƁA�����炭�u�Q�[���I�Ƀ`���m�炵���Ȃ�Ȃ��v�B
				�`���m�炵��: �� -> ���� -> ao -> aho -> �A�z -> baka -> baka==�H(�ԉf�ˌ�������������)
				//
				�����œK���ȃX�N�V������e�̐��𒲂ׂ�B
				1(1).�� 27/141��	3/16�� == 3.06382978723404255319148936170213 = 27*16/141	(R+G+B�͔��Ȃ̂Ő��኱���Ȃ����قړ��l�ɂ���)
				2(3).�� 21/141��	2/16�� == 2.38297872340425531914893617021277 = 21*16/141	(R+G+B�͔��Ȃ̂Ő��኱���Ȃ����قړ��l�ɂ���)
				3(5).�� 26/141��	3/16�� == 2.95035460992907801418439716312057 = 26*16/141	(R+G+B�͔��Ȃ̂Ő��኱���Ȃ����قړ��l�ɂ���)
				4(6).�� 34/141��	4/16�� == 3.85815602836879432624113475177305 = 34*16/141	�����Ȃ킿(R+G)�Ȃ̂Ő����Ȃ��Ƃ������o�B
				5(7).�� 33/141��	4/16�� == 3.74468085106382978723404255319149 = 33*16/141	�򂷂Ȃ킿(R+R+G)�Ȃ̂Ő����Ȃ��Ƃ������o�B
				�]���āA���L�̊m���e�[�u��������B
			*/
			const u8 p_tama_tbl[(16)] =
			{
				/* �m���𓙂�������B */
				/* �F���U��΂�悤�Ȕz�u���l����B */
				 1, 3, 5, 6, 7,
				 1, 3, 5, 6, 7,
				 1, 6, 5, 6, 7, 7,
			};
			static int add_angle;
			add_angle += (KOTEITI_7777);	/* �Œ�l�B8����1 ���� 10����1 ���x�̒l�ŁAn����1�Ŋ���؂�Ȃ��l�B */
			const u16 ra_nd16 = (ra_nd()/*&0xffff*/);
		//	br.BULLET_REGIST_00_speed256				= (t256(1.9)+(ra_nd16>>7)); 		/* �e�� */	/* (ra_nd16>>7) == ����(0-511) */
			br.BULLET_REGIST_00_speed256				= (t256(1.5)+(ra_nd16>>7)); 		/* �e�� */	/* (ra_nd16>>7) == ����(0-511) */
		//	br.BULLET_REGIST_01_speed_offset			= t256(-5); 		/* (-5) (-3)���������e */	/* ���̕����ɂȂ邩������ */
			br.BULLET_REGIST_01_speed_offset			= t256(0);/*(�e�X�g)*/
			/*
				�����̒e�������́A�P���ɑ����������������A�x���������������Ƃ������͖����B
				����܂葬�����߂���(�{�����炢�H)�ƁA�����̃G�^�[�i���~�[�N�ɂȂ����Ⴄ�͔̂���Ǝv�����ǁB
				//
				�����ꍇ�́A�n�߂̒e�������̂͂��̕�����Ȃ邪�A�㔼�͒e��������̂ō����ȒP�ɂȂ�B
				�x���ꍇ�́A�n�߂̒e�������̂͂��̕��ȒP�ɂȂ邪�A�㔼�͒e����������̂ł��̕�����Ȃ�B
			*/
		//	br.BULLET_REGIST_02_angle65536				= (0/65536);					/* ������ */
		//	br.BULLET_REGIST_02_angle65536				= (ra_nd()&((32768)-1)) + ((src->boss_base_spell_time_out)<<(8+6)); 		/* ���� */
		//	br.BULLET_REGIST_02_angle65536				= (ra_nd()&((32768/32)-1)) + ((src->boss_base_spell_time_out)<<(8+4));		/* ���� */
		//	br.BULLET_REGIST_02_angle65536				= (ra_nd()&((65536)-1)) ;		/* ���� */
			br.BULLET_REGIST_02_angle65536				= (add_angle)/*8����(0.125�b��1��)*/;		/* ���� */
			br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_8000_NON_TILT);/* (r33-)��X���e */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_WAKU12_00_SIRO+(p_tama_tbl[((src->boss_base_spell_time_out)&0x0f)]);
		//����br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_01_ANGLE_NON_TILT;		/* ��X���e */
			#if 0
			br.BULLET_REGIST_06_n_way					= (2);										/* [2way] */	/* 1�t���[����2�e */
			#else
			br.BULLET_REGIST_06_n_way					= (1+(cg_game_difficulty)); /* �͕핗�Ȃ̂ŁA��Փx���� */		/* [2way] */	/* 1�t���[����2�e */
			#endif
			br.BULLET_REGIST_07_div_angle65536			= (65536/16)+(ra_nd16>>4);				/* �����p�x([360/360]�x��16����) */ 	/* (ra_nd16>>4) == ����(0-4095) */
			{
				obj_send1->cx256					= (src->cx256); 	/* �e��x256 �{�X���S���甭�e�B */
				obj_send1->cy256					= (src->cy256); 	/* �e��y256 �{�X���S���甭�e�B */
				tama_system_regist_katayori_n_way();	/* (r33-) */
			}
		}
	}
	else
	if (((640)-(64*4))<(src->boss_base_spell_time_out)) // 192== (64*3)
	{
		;/*�������Ȃ�*/
	}
	else
	if (((640)-(64*5))<(src->boss_base_spell_time_out)) // 192== (64*3)
	{
		/*
		Normal�œK���ɂ���Ă݂��B
		��ǂ��e�́A���x�̈Ⴄ�e(�O��)�𓯎��ɏo���B�̃Z�b�g��6�񂾂����B
		*/
		if (0x00==((src->boss_base_spell_time_out)&0x07)) /* 8[frame]��1�� */
		{
			if ((4+(cg_game_difficulty))>((src->boss_base_spell_time_out)&0x07))
			{
				set_sub_jikinerai(src);/* 1:���������쐬������ۂ��B */
				int aaa_angle65536;
				aaa_angle65536 = (src->tmp_angleCCW65536);/* ���@�_���p */
				int jj;
				for (jj=0; jj<3; jj++)
				{
				//	br.BULLET_REGIST_00_speed256				= (t256(2.5));				/* �e�� */
				//	br.BULLET_REGIST_00_speed256				= (t256(1.75)+(jj<<7)); 	/* �e�� */
					br.BULLET_REGIST_00_speed256				= (t256(2.00)+(jj<<8)); 	/* �e�� */
					br.BULLET_REGIST_01_speed_offset			= t256(0);/*(�e�X�g)*/
					br.BULLET_REGIST_02_angle65536				= (aaa_angle65536)+(jj<<8); 		/* ���@�_���p */
					br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_8000_NON_TILT);/* (r33-)��X���e */
					br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_MARU10_03_AOI;
				//����br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_01_ANGLE_NON_TILT;		/* ��X���e */
					br.BULLET_REGIST_06_n_way					= (8);								/* [8way] */
					br.BULLET_REGIST_07_div_angle65536			= (65536/16);						/* �����p�x([360/360]�x��16����) */
					{
						tama_system_regist_n_way(); 		/* (r33-) */
					}
				}
			}
		}
	}
//
	#if 0
	if (((640)-(64*3))==(src->boss_base_spell_time_out))	// 192== (64*3)
	{
		exchange_damnaku_chiruno_gensoku();/*�����J�n*/
	}
	else
//	if (((640)-(64*3)-48)==(src->boss_base_spell_time_out)) // 192== (64*3-48)
	if (((640)-(64*4))==(src->boss_base_spell_time_out))		// 256== (64*4)
	{
//		// 48 �t���[���o�ߌ�A�S�ăX�g�b�v�����A�������A�J�E���g���Z�b�g
		exchange_damnaku_chiruno_tomeru();/*�S�ăX�g�b�v*/
	}
	else
	if (((640)-(64*5))==(src->boss_base_spell_time_out))		// 320== (64*5)
	{
		exchange_damnaku_chiruno_saikasoku();/*�ĉ���*/
	}
	#else
	if ((7<<6)==(src->boss_base_spell_time_out))	{	exchange_damnaku_chiruno_gensoku();/*�����J�n*/ 	}
	else
	if ((6<<6)==(src->boss_base_spell_time_out))	{	exchange_damnaku_chiruno_tomeru();/*�S�ăX�g�b�v*/	}
	else
	if ((5<<6)==(src->boss_base_spell_time_out))	{	exchange_damnaku_chiruno_saikasoku();/*�ĉ���*/ 	}
	#endif

}
#endif


/*---------------------------------------------------------
	�ᕄ�u�_�C�A�����h�u���U�[�h�v
	-------------------------------------------------------
	�`���m�̎���ŕX�e���΂�T��
	�����Ɍ����e���ۂ��B
		1.�`���m�̎��藣�ꂽ�ʒu���甭�e�B(�����_��?)
		2.������x�܂ōL����B
		3.�������e�B
---------------------------------------------------------*/

static void spell_create_16_diamond_blizzard(SPRITE *src)
{
	if (0==((src->boss_base_spell_time_out)&0x1f))
	{
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE16_BOSS_KYUPIN);
	//	voice_play(VOICE16_BOSS_KYUPIN, TRACK04_TEKIDAN);
	}
//	if (0==((src->boss_base_spell_time_out)&0x07))
	{
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
	//	bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		{
			const u32 ra_nd32 = (ra_nd()/*&0xffff*/);
			obj_send1->cx256							= (src->cx256) -t256(64)+(ra_nd32&0x7f00);				/* �e��x256 */
			obj_send1->cy256							= (src->cy256) -t256(32)+((ra_nd32>>8)&0x3f00); 		/* �e��y256 */
		//
		//	br.BULLET_REGIST_00_speed256				= (t256(1.0));				/* �e��(2.5) */
			br.BULLET_REGIST_00_speed256				= (t256(0.0));				/* �e��(2.5) */
		//	br.BULLET_REGIST_01_speed_offset			= t256(6);/* (6)...���� �Ă���*/
			br.BULLET_REGIST_01_speed_offset			= t256(2);/* �Ă���*/
			br.BULLET_REGIST_02_angle65536				= (ra_nd32);
			br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)�W���e */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KOME_03_AOI;		/* [�Ēe] �ɂȂ��Ă�̂͌��� �X�e �̃O������������ */
		//����br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)�W���e */
		//	br.BULLET_REGIST_06_n_way					= (1);/* [nway] */
			br.BULLET_REGIST_06_n_way					= (3);/* [nway] */
		//	br.BULLET_REGIST_07_div_angle65536			= (int)(65536/(64));		/* �����p�x(1024[360/360�x]�� 64 ����) */	/* 1����n���������p�x */
			br.BULLET_REGIST_07_div_angle65536			= (ra_nd32);				/* �����p�x(1024[360/360�x]�� 64 ����) */	/* 1����n���������p�x */
			tama_system_regist_katayori_n_way();/* (r33-) */
		}
	}
}

/*
	�ŋ��u�G�^�[�i���t�H�[�X�u���U�[�h�v
	(Eternal Force Blizzard, EFB)

	�G�^�[�i���t�H�[�X�u���U�[�h
		��u�ő���̎��͂̑�C���ƕX��������
		����͎���

	2�����˂�̃j���[�X����̃X���b�h(�ʏ�VIP)�ōl���o���ꂽ���@(�K�E�Z�H)
	���܂�̃N�H���e�B�̍����ɗl�X��AA�������B
	�܂��A�����̉{���҂Ɏ������ߋ��ɓ����悤�Ȏ����l���Ă����ƌ����g���E�}���Ăъo�܂������B

*/


