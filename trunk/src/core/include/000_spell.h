
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���̃t�@�C���͒��ڃC���N���[�h���܂���B
	"game_main.h" ����̂݊ԐړI�ɃC���N���[�h���܂��B
---------------------------------------------------------*/

#ifndef _SPELL_H_
#define _SPELL_H_
	/*---------------------------------------------------------
	�X�y��(�e��)�V�X�e��(���炩���ߒe���̒�`�����Ă����V�X�e��)
	-------------------------------------------------------
	�͕핗�ł̓X�y��(�e��)�̓V�X�e���ŃT�|�[�g���Ă��܂��B
	�X�y��(�e��)�Ƃ��Ē�`���Ă����ƁA�ǂ̃{�X���A�ǂ̃X�y��(�e��)�ł����Ă�悤�ɂȂ�܂��B
	-------------------------------------------------------
	�܂��X�y��(�e��)�̓X�y���J�[�h�V�X�e���ɔԍ���o�^���Ă����ƁA�X�y���J�[�h�V�X�e���ŃX�y��(�e��)���擾�ł��܂��B
	(�܂�X�y���J�[�h�V�X�e���o�R�ŃX�y��(�e��)�����Ă܂�)
	-------------------------------------------------------
	(r32)���݁A�G��(�͕핗��dat�̒�-�{�X)���X�y��(�e��)�����Ă܂����A
	���̋@�\�͏����Ȃ��Ȃ邩���m��܂���B
	�G��(�͕핗��dat�̒�-�{�X)���X�y��(�e��)�����Ă�@�\�́u���_�\�v�Ŏv�������̂ł����A
	���������@�\�����������Q�[���̑��x�������o����C�����邵�A
	�����������炱�������@�\�͖{�Ƃɖ����̂����Ǝv�����Ƃ�����܂��B�ǂ�����܂���B
	-------------------------------------------------------
	boss->boss_base_spell_type �Ɍ��������X�y��(�e��)���Z�b�g����ƁA�X�y��(�e��)�����J�n�B
	�X�y��(�e��)�������I������Ǝ����I�� boss->boss_base_spell_type �� DANMAKU_00 �ɂȂ�B
	-------------------------------------------------------
	�X�y��(�e��)�������� boss->boss_base_spell_time_out ������ɖ��t���[�����Z����A0 �ɂȂ�ƃX�y��(�e��)�����I���B
	���̎��Ԃ̓X�y��(�e��)���ƂɈႤ�W�����Ԃ��ݒ肳��Ă���̂ŁA�����ݒ肷��ɂ�
	spell_set_time_out(boss);	// �X�y��(�e��)�̎��Ԑ؂��ݒ�
	�Ƃ���B
	---------------------------------------------------------*/

/* �X�y��(�e��)�ԍ�(�e�X�y��(�e��)����ʂ���ׂ̔ԍ�) */
enum
{
	SPELL_00 = 0,						/* �X�y��(�e��)�����I���t���O�B */
//	/* �G���p */
	SPELL_01_sakuya_misogi_normal,		/* [���] �g5�ʒ�-�{�X �S�e�� / �g5�ʒ�-�{�X ��� �u�ʏ�U���v */
	SPELL_02_sakuya_old_test,			/* [���] �g5�ʒ�-�{�X 24�e�� (��p�u�~�X�f�B���N�V�������ǂ�(1/2)�v) */
	SPELL_03_sakuya_old_test,			/* [���] �g5�ʒ�-�{�X 11�e�� (��p�u�~�X�f�B���N�V�������ǂ�(2/2)�v) */
	SPELL_04_sakuya_pink_hearts,		/* [���] �g5�ʃ{�X [���] �u�ʏ�U��1(1/2)�v�ɂ�����Ƃ��������e��(�\��) */
	SPELL_05_sakuya,					/* [���] �g5�ʃ{�X [���] �u�ʏ�U��1/2(2/2)�v�ɂ�����Ƃ��������e��(�\��) */
	SPELL_06_sakuya,					/* [���] �g5�ʃ{�X [���] ���ہu���i�N���b�N(1/2)�v�ɂ�����Ƃ��������e��(�\��) */
	SPELL_07_sakuya,					/* [���] �g5�ʃ{�X [���] �u�ʏ�U��3(1/2)�v�ɂ�����Ƃ��������e��(�\��) */
//
	SPELL_08_rumia, 					/* [���[�~�A] �g1�ʃ{�X [���[�~�A] �镄�u�i�C�g�o�[�h�v�ɂ�����Ƃ��������e��(�\��) */
	SPELL_09_alice, 					/* [�A���X] ���ۂ��H�e�L�g�[�e�� */
	SPELL_0a_dai_yousei_01, 			/* [��d��] �S�� 48�����A���v���A�΃N�i�C�e */
	SPELL_0b_dai_yousei_02, 			/* [��d��] �S�� 48�����A�����v���A�ԃN�i�C�e */
	SPELL_0c_sakuya_jack32, 			/* [���] ���H�u�W���b�N�E�U�E���h�r���v�ɂȂ邩������Ȃ��e�X�g */
	SPELL_0d_sakuya_miss_direction, 	/* [���] ��p�u�~�X�f�B���N�V�����v */
	SPELL_0e_remilia_00,				/* [���~���A] �e�X�g */
	SPELL_0f_tengu, 					/* [���̑�] �������̗d��1(�V��l?)�����e��(�\��) */		/*???*/
//
	SPELL_10_cirno, 					/* [�`���m] �S�e�� */
	SPELL_11_perfect_freeze,			/* [�`���m] �p�[�t�F�N�g�t���[�Y */
	SPELL_12_diamond_blizzard,			/* [�`���m] �ᕄ�u�_�C�A�����h�u���U�[�h�v */
	SPELL_13_alice_suwako,				/* [�A���X] �z�K�q���ۂ�(?)�e�� */
	SPELL_14_alice_youmu300,			/* [�A���X] �d3�ʃ{�X  normal�ʏ�U��3(�̈ꕔ) */
	SPELL_15_alice_aka_2nd, 			/* [�A���X] ��EX�ʃ{�X �Ԃ̖��@2�i�K�ڂ��ǂ� */
	SPELL_16_alice_doll,				/* [�A���X] �d3�ʃ{�X [�A���X]�l�`�e��(���ǂ�) */	// 40[sec]
	SPELL_17_alice_doll_02, 			/* [�A���X] �쐬�� */
//
	SPELL_18_hana_test, 				/* [����] �ԂĂ��ƒe�� */
	SPELL_19_mima_sekkin,				/* [����] �ڋߒe�� */
	SPELL_1a_mima_toge, 				/* [����] �g�Q�Ă��ƒe�� */
	SPELL_1b_cirno_icecle_fall, 		/* [�`���m] �X���u�A�C�V�N���t�H�[���v */
	SPELL_1c_kakuya_tamanoe,			/* [�P��]�A�H���̋ʂ̎}���ǂ��B�쐬�� */
	SPELL_1d_amefuri_test,				/* [] �J�Ă��� */
	SPELL_1e_kaguya01,					/* [�P��] �ނ���ϊ�1 */
	SPELL_1f_kaguya04,					/* [�P��] �ނ���ϊ�2 */
//	/* �{�X�p */
	SPELL_20_aya_misogi1,				/*	1[��] �̒ʏ�e���ۂ��e���������Ă݂�e�X�g1 */
	SPELL_21_aya_misogi2,				/*	2[��] �̒ʏ�e���ۂ��e���������Ă݂�e�X�g2 */
	SPELL_22_ame_no_yatimata,			/*	3[��] �򕄁u�V�̔��ˁv */
	SPELL_23_aya_merin_test,			/*	4[��] �ɔ�����ۂ��e���������Ă݂�e�X�g */
	SPELL_24_aya_doll,					/*	5[��] �l�`�e�� */ //40[sec]
	SPELL_25_houka_kenran,				/*	6[�g����] �ؕ��u�F�؈�ࣁv�ɂ�����Ƃ��������e��(�\��) */
	SPELL_26_aya_saifu, 				/*	7[��] �Ǖ� */
	SPELL_27_,							/*	8[�p�`�F] �̃��[�U�[���ǂ� */
//
	SPELL_28_remilia_tamaoki1,			/*	9[���] �g �ʃ{�X  ��݂����ۂ����̋ʒu�e��(�\��) */	//	SPE LL_28_rumia_night_bird, 		/*	9[���[�~�A] �g1�ʃ{�X �镄�u�i�C�g�o�[�h�v�ɂ�����Ƃ��������e��(�\��) */
	SPELL_29_rumia_demarcation, 		/* 10[���[�~�A] �g1�ʃ{�X �ŕ��u�f�B�}�[�P�C�V�����v�ɂ�����Ƃ��������e��(�\��) */
	SPELL_2a_sakuya_baramaki1,			/* 11[���] �g5�ʒ�-�{�X �΂�T��1 �e�� () */
	SPELL_2b_sakuya_baramaki2,			/* 12[���] �g5�ʒ�-�{�X �΂�T��2 �e�� () */
	SPELL_2c_sakuya_blue_red,			/* 13[���] �g5�ʒ�-�{�X �u�ʏ�U��3�v�ԃi�C�t */
	SPELL_2d_sakuya_misogi_lunatic, 	/* 14[���] �g5�ʒ�-�{�X �S�e�� / �g5�ʒ�-�{�X ��� �u�ʏ�U���vlunatic */
	SPELL_2e_pache_agni_shine_1,		/* [�p�`�F] "�@�@�@�@�Ε��u�A�O�j�V���C���v" No.15 */	// agni shine (�A�O�j�̋P��)�A (�A�O�j==�C���h�_�b�̉΂̐_)
	SPELL_2f_pache_princess_undine, 	/* [�p�`�F] "�@�����u�v�����Z�X�E���f�B�l�v" No.16 */	// princess undine (�E���f�B�l�̕P)�A(�E���f�B�l==�B���p�t�p���P���X�X�́u�d���̏��v�ɓo�ꂷ�鐅�̐��̖��O)
//
	SPELL_30_pache_sylphy_horn_1,		/* [�p�`�F] "�@�@�@�@�ؕ��u�V���t�B�z�����v" No.17 */	// sylphy horn (�V���t�B�̃z����)�A(�V���t�B==�����i�鐸��)�A(�z����==�y��==�p�J)
	SPELL_31_pache_rage_tririton_1, 	/* [�p�`�F] "�@�@�y���u���C�W�B�g�����g���v" No.18 */	// rage tririton (�{��g�����g��)�A (�g�����g��==���E��Y�X�g�[���w���W�̎O��)
	SPELL_32_pache_metal_fatigue,		/* [�p�`�F] "�@�@�����u���^���t�@�e�B�[�O�v" No.19 */	// metal fatigue (������J)
	SPELL_33_pache_agni_shine_2,		/* [�p�`�F] "�@�@�Ε��u�A�O�j�V���C���㋉�v" No.20 */
	SPELL_34_pache_sylphy_horn_2,		/* [�p�`�F] "�@�@�ؕ��u�V���t�B�z�����㋉�v" No.21 */
	SPELL_35_pache_rage_tririton_2, 	/* [�p�`�F] "�y���u���C�W�B�g�����g���㋉�v" No.22 */
	SPELL_36_pache_agni_radiance,		/* [�p�`�F] "�@�Ε��u�A�O�j���C�f�B�A���X�v" No.23 */	// agni radiance (�A�O�j�̋P��)�A (�A�O�j==�C���h�_�b�̉΂̐_)
	SPELL_37_pache_bury_in_lake,		/* [�p�`�F] "�@�@�@�����u�x���[�C�����C�N�v" No.24 */	// bury in lake (����)
//
	SPELL_38_pache_green_storm, 		/* [�p�`�F] "�@�@�@�ؕ��u�O���[���X�g�[���v" No.25 */	// green storm (�΂̗�)
	SPELL_39_pache_tririton_shake,		/* [�p�`�F] "�@�@�y���u�g�����g���V�F�C�N�v" No.26 */	// tririton shake (�h���g�����g��)�A (�g�����g��==���E��Y�X�g�[���w���W�̎O��)
	SPELL_3a_pache_silver_dragon,		/* [�p�`�F] "�@�@�@�����u�V���o�[�h���S���v" No.27 */	// silver dragon (�◃��)
	SPELL_3b_pache_lava_cromlech,		/* [�p�`�F] "�΁��y���u���[���@�N�������N�v" No.28 */	// lava cromlech (�n�����)
	SPELL_3c_pache_forest_blaze,		/* [�p�`�F] "�؁��Ε��u�t�H���X�g�u���C�Y�v" No.29 */	// forest blaze (�X�щ΍�)
	SPELL_3d_pache_water_elf,			/* [�p�`�F] "�@�����ؕ��u�E�H�[�^�[�G���t�v" No.30 */	// water (��) / elf (�d��)	(�M���O:���n��==���ɗn�����Ⴄ�d��)
	SPELL_3e_pache_mercury_poison,		/* [�p�`�F] "���������u�}�[�L�����|�C�Y���v" No.31 */	// mercury poison (���⒆��)
	SPELL_3f_pache_emerald_megalith,	/* [�p�`�F] "�y�������u�G�������h���K���X�v" No.32 */	// emerald megalith (����ȃG�������h��)
//
	SPELL_40_SONOTA_DEBUG_CW_AO,		/* [���̑�] �f�o�b�O�e��CW�� */
	SPELL_41_SONOTA_DEBUG_CCW_AKA,		/* [���̑�] �f�o�b�O�e��CCW�� */
	SPELL_42_PACHE_LASER1,				/* [�p�`�F] �u�ʏ�U��1�v�̃��[�U�[���ǂ� */
	SPELL_43_PACHE_LASER2,				/* [�p�`�F] �u�ʏ�U��3�v�̃��[�U�[���ǂ� */
//
	SPELL_44,				/* */
	SPELL_45,				/* */
	SPELL_46,				/* */
	SPELL_47_sakuya_meek,				/* [���] �ނ���ϊ�3 */
//	SPELL_2e_SONOTA_DEBUG_CW_AO,		/* [���̑�] �f�o�b�O�e��CW�� */
//	SPELL_2f_SONOTA_DEBUG_CCW_AKA,		/* [���̑�] �f�o�b�O�e��CCW�� */
//
	SPELL_MAX,
};

/* target->tmp_angleCCW65536 �� ���@�_���e�̊p�x���v�Z */
extern void tmp_angleCCW65536_src_nerai(SPRITE *src, SPRITE *dest);
extern void tmp_angleCCW65536_jiki_nerai(SPRITE *target);

/* �X�y��(�e��)�̎��Ԑ؂��ݒ� */
extern void spell_set_time_out(SPRITE *src);

/* �X�y��(�e��)���� */
extern void spell_generator(SPRITE *src);



#endif /* _SPELL_H_ */


#ifndef _BOSS_COMMON_H_
#define _BOSS_COMMON_H_
enum
{
	DESTROY_CHECK_00_WIN_BOSS = 0,
	DESTROY_CHECK_01_IS_TIME_OUT,
};
/*static*/extern  void boss_destroy_check_type(SPRITE *src/*�G����*/, int check_type);/*	�� �U���̏ꍇ�̎��S���� 	�� ���Ԑ؂�̏ꍇ�̎��S���� */
/*static*/extern  void callback_hit_boss_dummy(SPRITE *src, SPRITE *tama); /*, int angle*/
/*static*/extern  void callback_hit_boss(SPRITE *src, SPRITE *tama); /*, int angle*/

/*static*/extern  void common_boss_put_items(SPRITE *src);

/*static*/extern  void lose_boss(SPRITE *src);






/* �X�y�J�V�X�e�� */
/*static*/extern void spell_card_get_spell_number(SPRITE *src);
#endif /* _BOSS_H_ */
