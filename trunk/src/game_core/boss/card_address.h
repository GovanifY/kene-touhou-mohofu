
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�J�[�h�A�h���X
	-------------------------------------------------------
	�J�[�h�̐�Έʒu��������ӂ̒l�i�J�[�h�A�h���X�j���`���܂��B
	-------------------------------------------------------
	��Έʒu�́A��Փx�ʂɈقȂ�܂��B
	�܂��ܖʂ̏ꍇ�́A�L�����ʂɂ���Đ�Έʒu���قȂ�܂��B
	-------------------------------------------------------
	�J�[�h�A�h���X�̓J�[�h�ԍ��Ƃ͈قȂ�܂��B
	�Ⴄ�J�[�h�A�h���X�ɓ����J�[�h�ԍ���z�u���鎖���o���܂��B
	-------------------------------------------------------
	��̗�ł́A
	�p�`�F 5��	�얲 B (����)�ŁA8�`�Ԗځi�J�[�h�A�h���X�j��
	�y�������u�G�������h���K���X�v�i�J�[�h�ԍ��j�ŁA
	�p�`�F 5��	������ A (����)�A7�`�Ԗځi�J�[�h�A�h���X�j��
	�y�������u�G�������h���K���X�v�i�J�[�h�ԍ��j�ƌ������ł��B
---------------------------------------------------------*/

#ifndef _CARD_ADDRESS_VALUE_H_
#define _CARD_ADDRESS_VALUE_H_

//	CARD_ADDRESS_00_OFF = 0,
enum
{
// �`���m �G�L�X�g���X�e�[�W
	CARD_ADDRESS_00E_chrno_000 = 0, /* "�`�ԕύX" */
	CARD_ADDRESS_00N_chrno_000, 	/* "�`�ԕύX" */
	CARD_ADDRESS_00H_chrno_000, 	/* "�`�ԕύX" */
	CARD_ADDRESS_00L_chrno_000, 	/* "�`�ԕύX" */
	CARD_ADDRESS_01E_chrno_aaa, 	/* ���`��: "�ʏ�U��1"*/
	CARD_ADDRESS_01N_chrno_aaa, 	/* ���`��: "�ʏ�U��1"*/
	CARD_ADDRESS_01H_chrno_aaa, 	/* ���`��: "�ʏ�U��1"*/
	CARD_ADDRESS_01L_chrno_aaa, 	/* ���`��: "�ʏ�U��1"*/
	CARD_ADDRESS_02E_chrno_bbb, 	/* ���`��: �X���u�A�C�V�N���t�H�[���v*/
	CARD_ADDRESS_02N_chrno_bbb, 	/* ���`��: �X���u�A�C�V�N���t�H�[���v*/
	CARD_ADDRESS_02H_chrno_bbb, 	/* ���`��: �X���u�A�C�V�N���t�H�[���v*/
	CARD_ADDRESS_02L_chrno_bbb, 	/* ���`��: �X���u�A�C�V�N���t�H�[���v*/
	CARD_ADDRESS_03E_chrno_ccc, 	/* ��O�`��:�u�ʏ�U��2�v */
	CARD_ADDRESS_03N_chrno_ccc, 	/* ��O�`��:�u�ʏ�U��2�v */
	CARD_ADDRESS_03H_chrno_ccc, 	/* ��O�`��:�u�ʏ�U��2�v */
	CARD_ADDRESS_03L_chrno_ccc, 	/* ��O�`��:�u�ʏ�U��2�v */
	CARD_ADDRESS_04E_chrno_ddd, 	/* ��l�`��: �����u�p�[�t�F�N�g�t���[�Y�v*/
	CARD_ADDRESS_04N_chrno_ddd, 	/* ��l�`��: �����u�p�[�t�F�N�g�t���[�Y�v*/
	CARD_ADDRESS_04H_chrno_ddd, 	/* ��l�`��: �����u�p�[�t�F�N�g�t���[�Y�v*/
	CARD_ADDRESS_04L_chrno_ddd, 	/* ��l�`��: �����u�p�[�t�F�N�g�t���[�Y�v*/
	CARD_ADDRESS_05E_chrno_eee, 	/* ��܌`��: �ᕄ�u�_�C�A�����h�u���U�[�h�v*/
	CARD_ADDRESS_05N_chrno_eee, 	/* ��܌`��: �ᕄ�u�_�C�A�����h�u���U�[�h�v*/
	CARD_ADDRESS_05H_chrno_eee, 	/* ��܌`��: �ᕄ�u�_�C�A�����h�u���U�[�h�v*/
	CARD_ADDRESS_05L_chrno_eee, 	/* ��܌`��: �ᕄ�u�_�C�A�����h�u���U�[�h�v*/
	CARD_ADDRESS_06E_chrno_gekiha,	/*(r36�����I�Ɍ��j)*/
	CARD_ADDRESS_06N_chrno_gekiha,	/*(r36�����I�Ɍ��j)*/
	CARD_ADDRESS_06H_chrno_gekiha,	/*(r36�����I�Ɍ��j)*/
	CARD_ADDRESS_06L_chrno_gekiha,	/*(r36�����I�Ɍ��j)*/
// ��� 6��
	CARD_ADDRESS_00E_sakuya_000,	/* "�`�ԕύX" */
	CARD_ADDRESS_00N_sakuya_000,	/* "�`�ԕύX" */
	CARD_ADDRESS_00H_sakuya_000,	/* "�`�ԕύX" */
	CARD_ADDRESS_00L_sakuya_000,	/* "�`�ԕύX" */
	CARD_ADDRESS_01E_sakuya_aaa,	/* ���`��: �ʏ�U��(���E�ɓ������S�e����) */
	CARD_ADDRESS_01N_sakuya_aaa,	/* ���`��: �ʏ�U��(���E�ɓ������S�e����) */
	CARD_ADDRESS_01H_sakuya_aaa,	/* ���`��: �ʏ�U��(���E�ɓ������S�e����) */
	CARD_ADDRESS_01L_sakuya_aaa,	/* ���`��: �ʏ�U��(���E�ɓ������S�e����) */
	CARD_ADDRESS_02E_sakuya_bbb,	/* ���`��: ��p�u�~�X�f�B���N�V�����v(�S���ʁA���܂�) */
	CARD_ADDRESS_02N_sakuya_bbb,	/* ���`��: ��p�u�~�X�f�B���N�V�����v(�S���ʁA���܂�) */
	CARD_ADDRESS_02H_sakuya_bbb,	/* ���`��: ��p�u�~�X�f�B���N�V�����v(�S���ʁA���܂�) */
	CARD_ADDRESS_02L_sakuya_bbb,	/* ���`��: ��p�u�~�X�f�B���N�V�����v(�S���ʁA���܂�) */
	CARD_ADDRESS_03E_sakuya_ccc,	/* ��O�`��: �����ԃi�C�t  */
	CARD_ADDRESS_03N_sakuya_ccc,	/* ��O�`��: �����ԃi�C�t  */
	CARD_ADDRESS_03H_sakuya_ccc,	/* ��O�`��: �����ԃi�C�t  */
	CARD_ADDRESS_03L_sakuya_ccc,	/* ��O�`��: �����ԃi�C�t  */
	CARD_ADDRESS_04E_sakuya_ddd,	/* ��l�`��:  */
	CARD_ADDRESS_04N_sakuya_ddd,	/* ��l�`��:  */
	CARD_ADDRESS_04H_sakuya_ddd,	/* ��l�`��:  */
	CARD_ADDRESS_04L_sakuya_ddd,	/* ��l�`��:  */
	CARD_ADDRESS_05E_sakuya_eee,	/* ��܌`��:  */
	CARD_ADDRESS_05N_sakuya_eee,	/* ��܌`��:  */
	CARD_ADDRESS_05H_sakuya_eee,	/* ��܌`��:  */
	CARD_ADDRESS_05L_sakuya_eee,	/* ��܌`��:  */
	CARD_ADDRESS_06E_sakuya_fff,	/* ��Z�`��:  */
	CARD_ADDRESS_06N_sakuya_fff,	/* ��Z�`��:  */
	CARD_ADDRESS_06H_sakuya_fff,	/* ��Z�`��:  */
	CARD_ADDRESS_06L_sakuya_fff,	/* ��Z�`��:  */
	CARD_ADDRESS_07E_sakuya_ggg,	/* �掵�`��: (���U�����w)�ǉ��v�撆 */
	CARD_ADDRESS_07N_sakuya_ggg,	/* �掵�`��: (���U�����w)�ǉ��v�撆 */
	CARD_ADDRESS_07H_sakuya_ggg,	/* �掵�`��: (���U�����w)�ǉ��v�撆 */
	CARD_ADDRESS_07L_sakuya_ggg,	/* �掵�`��: (���U�����w)�ǉ��v�撆 */
	CARD_ADDRESS_08E_sakuya_hhh,	/* �攪�`��: (���~�߃i�C�t)�ǉ��v�撆 */
	CARD_ADDRESS_08N_sakuya_hhh,	/* �攪�`��: (���~�߃i�C�t)�ǉ��v�撆 */
	CARD_ADDRESS_08H_sakuya_hhh,	/* �攪�`��: (���~�߃i�C�t)�ǉ��v�撆 */
	CARD_ADDRESS_08L_sakuya_hhh,	/* �攪�`��: (���~�߃i�C�t)�ǉ��v�撆 */
	CARD_ADDRESS_09E_sakuya_iii,	/* ���`��: �ŏI�`��(����1) */
	CARD_ADDRESS_09N_sakuya_iii,	/* ���`��: �ŏI�`��(����1) */
	CARD_ADDRESS_09H_sakuya_iii,	/* ���`��: �ŏI�`��(����1) */
	CARD_ADDRESS_09L_sakuya_iii,	/* ���`��: �ŏI�`��(����1) */
	CARD_ADDRESS_10E_sakuya_jjj,	/* ��10�`��: �ŏI�`��(����2) */
	CARD_ADDRESS_10N_sakuya_jjj,	/* ��10�`��: �ŏI�`��(����2) */
	CARD_ADDRESS_10H_sakuya_jjj,	/* ��10�`��: �ŏI�`��(����2) */
	CARD_ADDRESS_10L_sakuya_jjj,	/* ��10�`��: �ŏI�`��(����2) */
	CARD_ADDRESS_11E_sakuya_kkk,	/* ��11�`��: �ŏI�`��(����3) */
	CARD_ADDRESS_11N_sakuya_kkk,	/* ��11�`��: �ŏI�`��(����3) */
	CARD_ADDRESS_11H_sakuya_kkk,	/* ��11�`��: �ŏI�`��(����3) */
	CARD_ADDRESS_11L_sakuya_kkk,	/* ��11�`��: �ŏI�`��(����3) */
	CARD_ADDRESS_12E_sakuya_gekiha, /*(r36�����I�Ɍ��j)*/
	CARD_ADDRESS_12N_sakuya_gekiha, /*(r36�����I�Ɍ��j)*/
	CARD_ADDRESS_12H_sakuya_gekiha, /*(r36�����I�Ɍ��j)*/
	CARD_ADDRESS_12L_sakuya_gekiha, /*(r36�����I�Ɍ��j)*/
// �p�`�FA 5��
	CARD_ADDRESS_00E_pacheA_000,	/* "�`�ԕύX" */
	CARD_ADDRESS_00N_pacheA_000,	/* "�`�ԕύX" */
	CARD_ADDRESS_00H_pacheA_000,	/* "�`�ԕύX" */
	CARD_ADDRESS_00L_pacheA_000,	/* "�`�ԕύX" */
	CARD_ADDRESS_01E_pacheA_bbb,	/* ���`��: �ʏ�U��1 */
	CARD_ADDRESS_01N_pacheA_bbb,	/* ���`��: �ʏ�U��1 */
	CARD_ADDRESS_01H_pacheA_bbb,	/* ���`��: �ʏ�U��1 */
	CARD_ADDRESS_01L_pacheA_bbb,	/* ���`��: �ʏ�U��1 */
	CARD_ADDRESS_02E_pacheA_ccc,	/* ���`��: �J�[�h�T */
	CARD_ADDRESS_02N_pacheA_ccc,	/* ���`��: �J�[�h�T */
	CARD_ADDRESS_02H_pacheA_ccc,	/* ���`��: �J�[�h�T */
	CARD_ADDRESS_02L_pacheA_ccc,	/* ���`��: �J�[�h�T */
	CARD_ADDRESS_03E_pacheA_ddd,	/* ��O�`��: �ʏ�U��2 */
	CARD_ADDRESS_03N_pacheA_ddd,	/* ��O�`��: �ʏ�U��2 */
	CARD_ADDRESS_03H_pacheA_ddd,	/* ��O�`��: �ʏ�U��2 */
	CARD_ADDRESS_03L_pacheA_ddd,	/* ��O�`��: �ʏ�U��2 */
	CARD_ADDRESS_04E_pacheA_eee,	/* ��l�`��: �J�[�h�U */
	CARD_ADDRESS_04N_pacheA_eee,	/* ��l�`��: �J�[�h�U */
	CARD_ADDRESS_04H_pacheA_eee,	/* ��l�`��: �J�[�h�U */
	CARD_ADDRESS_04L_pacheA_eee,	/* ��l�`��: �J�[�h�U */
	CARD_ADDRESS_05E_pacheA_fff,	/* ��܌`��: �J�[�h�V */
	CARD_ADDRESS_05N_pacheA_fff,	/* ��܌`��: �J�[�h�V */
	CARD_ADDRESS_05H_pacheA_fff,	/* ��܌`��: �J�[�h�V */
	CARD_ADDRESS_05L_pacheA_fff,	/* ��܌`��: �J�[�h�V */
	CARD_ADDRESS_06E_pacheA_ggg,	/* ��Z�`��: �J�[�h�W */
	CARD_ADDRESS_06N_pacheA_ggg,	/* ��Z�`��: �J�[�h�W */
	CARD_ADDRESS_06H_pacheA_ggg,	/* ��Z�`��: �J�[�h�W */
	CARD_ADDRESS_06L_pacheA_ggg,	/* ��Z�`��: �J�[�h�W */
	CARD_ADDRESS_07E_pacheA_hhh,	/* �掵�`��: �J�[�h�X */
	CARD_ADDRESS_07N_pacheA_hhh,	/* �掵�`��: �J�[�h�X */
	CARD_ADDRESS_07H_pacheA_hhh,	/* �掵�`��: �J�[�h�X */
	CARD_ADDRESS_07L_pacheA_hhh,	/* �掵�`��: �J�[�h�X */
	CARD_ADDRESS_08E_pacheA_gekiha, /*(r36�����I�Ɍ��j)*/
	CARD_ADDRESS_08N_pacheA_gekiha, /*(r36�����I�Ɍ��j)*/
	CARD_ADDRESS_08H_pacheA_gekiha, /*(r36�����I�Ɍ��j)*/
	CARD_ADDRESS_08L_pacheA_gekiha, /*(r36�����I�Ɍ��j)*/
// �p�`�FB 5��
	CARD_ADDRESS_10E_pacheB_000,	/* "�`�ԕύX" */
	CARD_ADDRESS_10N_pacheB_000,	/* "�`�ԕύX" */
	CARD_ADDRESS_10H_pacheB_000,	/* "�`�ԕύX" */
	CARD_ADDRESS_10L_pacheB_000,	/* "�`�ԕύX" */
	CARD_ADDRESS_11E_pacheB_bbb,	/* ���`��: �ʏ�U��1 */
	CARD_ADDRESS_11N_pacheB_bbb,	/* ���`��: �ʏ�U��1 */
	CARD_ADDRESS_11H_pacheB_bbb,	/* ���`��: �ʏ�U��1 */
	CARD_ADDRESS_11L_pacheB_bbb,	/* ���`��: �ʏ�U��1 */
	CARD_ADDRESS_12E_pacheB_ccc,	/* ���`��: �J�[�h�T */
	CARD_ADDRESS_12N_pacheB_ccc,	/* ���`��: �J�[�h�T */
	CARD_ADDRESS_12H_pacheB_ccc,	/* ���`��: �J�[�h�T */
	CARD_ADDRESS_12L_pacheB_ccc,	/* ���`��: �J�[�h�T */
	CARD_ADDRESS_13E_pacheB_ddd,	/* ��O�`��: �ʏ�U��2 */
	CARD_ADDRESS_13N_pacheB_ddd,	/* ��O�`��: �ʏ�U��2 */
	CARD_ADDRESS_13H_pacheB_ddd,	/* ��O�`��: �ʏ�U��2 */
	CARD_ADDRESS_13L_pacheB_ddd,	/* ��O�`��: �ʏ�U��2 */
	CARD_ADDRESS_14E_pacheB_eee,	/* ��l�`��: �J�[�h�U */
	CARD_ADDRESS_14N_pacheB_eee,	/* ��l�`��: �J�[�h�U */
	CARD_ADDRESS_14H_pacheB_eee,	/* ��l�`��: �J�[�h�U */
	CARD_ADDRESS_14L_pacheB_eee,	/* ��l�`��: �J�[�h�U */
	CARD_ADDRESS_15E_pacheB_fff,	/* ��܌`��: �J�[�h�V */
	CARD_ADDRESS_15N_pacheB_fff,	/* ��܌`��: �J�[�h�V */
	CARD_ADDRESS_15H_pacheB_fff,	/* ��܌`��: �J�[�h�V */
	CARD_ADDRESS_15L_pacheB_fff,	/* ��܌`��: �J�[�h�V */
	CARD_ADDRESS_16E_pacheB_ggg,	/* ��Z�`��: �J�[�h�W */
	CARD_ADDRESS_16N_pacheB_ggg,	/* ��Z�`��: �J�[�h�W */
	CARD_ADDRESS_16H_pacheB_ggg,	/* ��Z�`��: �J�[�h�W */
	CARD_ADDRESS_16L_pacheB_ggg,	/* ��Z�`��: �J�[�h�W */
	CARD_ADDRESS_17E_pacheB_hhh,	/* �掵�`��: �J�[�h�X */
	CARD_ADDRESS_17N_pacheB_hhh,	/* �掵�`��: �J�[�h�X */
	CARD_ADDRESS_17H_pacheB_hhh,	/* �掵�`��: �J�[�h�X */
	CARD_ADDRESS_17L_pacheB_hhh,	/* �掵�`��: �J�[�h�X */
	CARD_ADDRESS_18E_pacheB_gekiha, /*(r36�����I�Ɍ��j)*/
	CARD_ADDRESS_18N_pacheB_gekiha, /*(r36�����I�Ɍ��j)*/
	CARD_ADDRESS_18H_pacheB_gekiha, /*(r36�����I�Ɍ��j)*/
	CARD_ADDRESS_18L_pacheB_gekiha, /*(r36�����I�Ɍ��j)*/
// �p�`�FC 5��
	CARD_ADDRESS_20E_pacheC_000,	/* "�`�ԕύX" */
	CARD_ADDRESS_20N_pacheC_000,	/* "�`�ԕύX" */
	CARD_ADDRESS_20H_pacheC_000,	/* "�`�ԕύX" */
	CARD_ADDRESS_20L_pacheC_000,	/* "�`�ԕύX" */
	CARD_ADDRESS_21E_pacheC_bbb,	/* ���`��: �ʏ�U��1 */
	CARD_ADDRESS_21N_pacheC_bbb,	/* ���`��: �ʏ�U��1 */
	CARD_ADDRESS_21H_pacheC_bbb,	/* ���`��: �ʏ�U��1 */
	CARD_ADDRESS_21L_pacheC_bbb,	/* ���`��: �ʏ�U��1 */
	CARD_ADDRESS_22E_pacheC_ccc,	/* ���`��: �J�[�h�T */
	CARD_ADDRESS_22N_pacheC_ccc,	/* ���`��: �J�[�h�T */
	CARD_ADDRESS_22H_pacheC_ccc,	/* ���`��: �J�[�h�T */
	CARD_ADDRESS_22L_pacheC_ccc,	/* ���`��: �J�[�h�T */
	CARD_ADDRESS_23E_pacheC_ddd,	/* ��O�`��: �ʏ�U��2 */
	CARD_ADDRESS_23N_pacheC_ddd,	/* ��O�`��: �ʏ�U��2 */
	CARD_ADDRESS_23H_pacheC_ddd,	/* ��O�`��: �ʏ�U��2 */
	CARD_ADDRESS_23L_pacheC_ddd,	/* ��O�`��: �ʏ�U��2 */
	CARD_ADDRESS_24E_pacheC_eee,	/* ��l�`��: �J�[�h�U */
	CARD_ADDRESS_24N_pacheC_eee,	/* ��l�`��: �J�[�h�U */
	CARD_ADDRESS_24H_pacheC_eee,	/* ��l�`��: �J�[�h�U */
	CARD_ADDRESS_24L_pacheC_eee,	/* ��l�`��: �J�[�h�U */
	CARD_ADDRESS_25E_pacheC_fff,	/* ��܌`��: �J�[�h�V */
	CARD_ADDRESS_25N_pacheC_fff,	/* ��܌`��: �J�[�h�V */
	CARD_ADDRESS_25H_pacheC_fff,	/* ��܌`��: �J�[�h�V */
	CARD_ADDRESS_25L_pacheC_fff,	/* ��܌`��: �J�[�h�V */
	CARD_ADDRESS_26E_pacheC_ggg,	/* ��Z�`��: �J�[�h�W */
	CARD_ADDRESS_26N_pacheC_ggg,	/* ��Z�`��: �J�[�h�W */
	CARD_ADDRESS_26H_pacheC_ggg,	/* ��Z�`��: �J�[�h�W */
	CARD_ADDRESS_26L_pacheC_ggg,	/* ��Z�`��: �J�[�h�W */
	CARD_ADDRESS_27E_pacheC_hhh,	/* �掵�`��: �J�[�h�X */
	CARD_ADDRESS_27N_pacheC_hhh,	/* �掵�`��: �J�[�h�X */
	CARD_ADDRESS_27H_pacheC_hhh,	/* �掵�`��: �J�[�h�X */
	CARD_ADDRESS_27L_pacheC_hhh,	/* �掵�`��: �J�[�h�X */
	CARD_ADDRESS_28E_pacheC_gekiha, /*(r36�����I�Ɍ��j)*/
	CARD_ADDRESS_28N_pacheC_gekiha, /*(r36�����I�Ɍ��j)*/
	CARD_ADDRESS_28H_pacheC_gekiha, /*(r36�����I�Ɍ��j)*/
	CARD_ADDRESS_28L_pacheC_gekiha, /*(r36�����I�Ɍ��j)*/
// �p�`�FD 5��
	CARD_ADDRESS_30E_pacheD_000,	/* "�`�ԕύX" */
	CARD_ADDRESS_30N_pacheD_000,	/* "�`�ԕύX" */
	CARD_ADDRESS_30H_pacheD_000,	/* "�`�ԕύX" */
	CARD_ADDRESS_30L_pacheD_000,	/* "�`�ԕύX" */
	CARD_ADDRESS_31E_pacheD_bbb,	/* ���`��: �ʏ�U��1 */
	CARD_ADDRESS_31N_pacheD_bbb,	/* ���`��: �ʏ�U��1 */
	CARD_ADDRESS_31H_pacheD_bbb,	/* ���`��: �ʏ�U��1 */
	CARD_ADDRESS_31L_pacheD_bbb,	/* ���`��: �ʏ�U��1 */
	CARD_ADDRESS_32E_pacheD_ccc,	/* ���`��: �J�[�h�T */
	CARD_ADDRESS_32N_pacheD_ccc,	/* ���`��: �J�[�h�T */
	CARD_ADDRESS_32H_pacheD_ccc,	/* ���`��: �J�[�h�T */
	CARD_ADDRESS_32L_pacheD_ccc,	/* ���`��: �J�[�h�T */
	CARD_ADDRESS_33E_pacheD_ddd,	/* ��O�`��: �ʏ�U��2 */
	CARD_ADDRESS_33N_pacheD_ddd,	/* ��O�`��: �ʏ�U��2 */
	CARD_ADDRESS_33H_pacheD_ddd,	/* ��O�`��: �ʏ�U��2 */
	CARD_ADDRESS_33L_pacheD_ddd,	/* ��O�`��: �ʏ�U��2 */
	CARD_ADDRESS_34E_pacheD_eee,	/* ��l�`��: �J�[�h�U */
	CARD_ADDRESS_34N_pacheD_eee,	/* ��l�`��: �J�[�h�U */
	CARD_ADDRESS_34H_pacheD_eee,	/* ��l�`��: �J�[�h�U */
	CARD_ADDRESS_34L_pacheD_eee,	/* ��l�`��: �J�[�h�U */
	CARD_ADDRESS_35E_pacheD_fff,	/* ��܌`��: �J�[�h�V */
	CARD_ADDRESS_35N_pacheD_fff,	/* ��܌`��: �J�[�h�V */
	CARD_ADDRESS_35H_pacheD_fff,	/* ��܌`��: �J�[�h�V */
	CARD_ADDRESS_35L_pacheD_fff,	/* ��܌`��: �J�[�h�V */
	CARD_ADDRESS_36E_pacheD_ggg,	/* ��Z�`��: �J�[�h�W */
	CARD_ADDRESS_36N_pacheD_ggg,	/* ��Z�`��: �J�[�h�W */
	CARD_ADDRESS_36H_pacheD_ggg,	/* ��Z�`��: �J�[�h�W */
	CARD_ADDRESS_36L_pacheD_ggg,	/* ��Z�`��: �J�[�h�W */
	CARD_ADDRESS_37E_pacheD_hhh,	/* �掵�`��: �J�[�h�X */
	CARD_ADDRESS_37N_pacheD_hhh,	/* �掵�`��: �J�[�h�X */
	CARD_ADDRESS_37H_pacheD_hhh,	/* �掵�`��: �J�[�h�X */
	CARD_ADDRESS_37L_pacheD_hhh,	/* �掵�`��: �J�[�h�X */
	CARD_ADDRESS_38E_pacheD_gekiha, /*(r36�����I�Ɍ��j)*/
	CARD_ADDRESS_38N_pacheD_gekiha, /*(r36�����I�Ɍ��j)*/
	CARD_ADDRESS_38H_pacheD_gekiha, /*(r36�����I�Ɍ��j)*/
	CARD_ADDRESS_38L_pacheD_gekiha, /*(r36�����I�Ɍ��j)*/
// �� 4��
	CARD_ADDRESS_00E_aya_000,		/* "�`�ԕύX" */
	CARD_ADDRESS_00N_aya_000,		/* "�`�ԕύX" */
	CARD_ADDRESS_00H_aya_000,		/* "�`�ԕύX" */
	CARD_ADDRESS_00L_aya_000,		/* "�`�ԕύX" */
	CARD_ADDRESS_01E_aya_bbb,		/* ���`��: �ʏ�U��1 */
	CARD_ADDRESS_01N_aya_bbb,		/* ���`��: �ʏ�U��1 */
	CARD_ADDRESS_01H_aya_bbb,		/* ���`��: �ʏ�U��1 */
	CARD_ADDRESS_01L_aya_bbb,		/* ���`��: �ʏ�U��1 */
	CARD_ADDRESS_02E_aya_ccc,		/* ���`��: �J�[�h�T */
	CARD_ADDRESS_02N_aya_ccc,		/* ���`��: �J�[�h�T */
	CARD_ADDRESS_02H_aya_ccc,		/* ���`��: �J�[�h�T */
	CARD_ADDRESS_02L_aya_ccc,		/* ���`��: �J�[�h�T */
	CARD_ADDRESS_03E_aya_ddd,		/* ��O�`��: �ʏ�U��2 */
	CARD_ADDRESS_03N_aya_ddd,		/* ��O�`��: �ʏ�U��2 */
	CARD_ADDRESS_03H_aya_ddd,		/* ��O�`��: �ʏ�U��2 */
	CARD_ADDRESS_03L_aya_ddd,		/* ��O�`��: �ʏ�U��2 */
	CARD_ADDRESS_04E_aya_eee,		/* ��l�`��: �J�[�h�U */
	CARD_ADDRESS_04N_aya_eee,		/* ��l�`��: �J�[�h�U */
	CARD_ADDRESS_04H_aya_eee,		/* ��l�`��: �J�[�h�U */
	CARD_ADDRESS_04L_aya_eee,		/* ��l�`��: �J�[�h�U */
	CARD_ADDRESS_05E_aya_fff,		/* ��܌`��: �J�[�h�V */
	CARD_ADDRESS_05N_aya_fff,		/* ��܌`��: �J�[�h�V */
	CARD_ADDRESS_05H_aya_fff,		/* ��܌`��: �J�[�h�V */
	CARD_ADDRESS_05L_aya_fff,		/* ��܌`��: �J�[�h�V */
	CARD_ADDRESS_06E_aya_ggg,		/* ��Z�`��: �J�[�h�W */
	CARD_ADDRESS_06N_aya_ggg,		/* ��Z�`��: �J�[�h�W */
	CARD_ADDRESS_06H_aya_ggg,		/* ��Z�`��: �J�[�h�W */
	CARD_ADDRESS_06L_aya_ggg,		/* ��Z�`��: �J�[�h�W */
	CARD_ADDRESS_07E_aya_hhh,		/* �掵�`��: �J�[�h�X */
	CARD_ADDRESS_07N_aya_hhh,		/* �掵�`��: �J�[�h�X */
	CARD_ADDRESS_07H_aya_hhh,		/* �掵�`��: �J�[�h�X */
	CARD_ADDRESS_07L_aya_hhh,		/* �掵�`��: �J�[�h�X */
	CARD_ADDRESS_08E_aya_gekiha,	/*(r36�����I�Ɍ��j)*/
	CARD_ADDRESS_08N_aya_gekiha,	/*(r36�����I�Ɍ��j)*/
	CARD_ADDRESS_08H_aya_gekiha,	/*(r36�����I�Ɍ��j)*/
	CARD_ADDRESS_08L_aya_gekiha,	/*(r36�����I�Ɍ��j)*/
// �P�� 3��
	CARD_ADDRESS_00E_kaguya_000,
	CARD_ADDRESS_00N_kaguya_000,
	CARD_ADDRESS_00H_kaguya_000,
	CARD_ADDRESS_00L_kaguya_000,
	CARD_ADDRESS_51E_kaguya_bbb,
	CARD_ADDRESS_51N_kaguya_bbb,
	CARD_ADDRESS_51H_kaguya_bbb,
	CARD_ADDRESS_51L_kaguya_bbb,
	CARD_ADDRESS_52E_kaguya_ccc,
	CARD_ADDRESS_52N_kaguya_ccc,
	CARD_ADDRESS_52H_kaguya_ccc,
	CARD_ADDRESS_52L_kaguya_ccc,
	CARD_ADDRESS_53E_kaguya_ddd,
	CARD_ADDRESS_53N_kaguya_ddd,
	CARD_ADDRESS_53H_kaguya_ddd,
	CARD_ADDRESS_53L_kaguya_ddd,
	CARD_ADDRESS_54E_kaguya_eee,
	CARD_ADDRESS_54N_kaguya_eee,
	CARD_ADDRESS_54H_kaguya_eee,
	CARD_ADDRESS_54L_kaguya_eee,
	CARD_ADDRESS_55E_kaguya_fff,
	CARD_ADDRESS_55N_kaguya_fff,
	CARD_ADDRESS_55H_kaguya_fff,
	CARD_ADDRESS_55L_kaguya_fff,
	CARD_ADDRESS_56E_kaguya_ggg,
	CARD_ADDRESS_56N_kaguya_ggg,
	CARD_ADDRESS_56H_kaguya_ggg,
	CARD_ADDRESS_56L_kaguya_ggg,
	CARD_ADDRESS_57E_kaguya_hhh,
	CARD_ADDRESS_57N_kaguya_hhh,
	CARD_ADDRESS_57H_kaguya_hhh,
	CARD_ADDRESS_57L_kaguya_hhh,
	CARD_ADDRESS_58E_kaguya_gekiha, 	/*(r36�����I�Ɍ��j)*/
	CARD_ADDRESS_58N_kaguya_gekiha, 	/*(r36�����I�Ɍ��j)*/
	CARD_ADDRESS_58H_kaguya_gekiha, 	/*(r36�����I�Ɍ��j)*/
	CARD_ADDRESS_58L_kaguya_gekiha, 	/*(r36�����I�Ɍ��j)*/
// ���� 2��
	CARD_ADDRESS_00E_mima_000,
	CARD_ADDRESS_00N_mima_000,
	CARD_ADDRESS_00H_mima_000,
	CARD_ADDRESS_00L_mima_000,
	CARD_ADDRESS_61E_mima_bbb,
	CARD_ADDRESS_61N_mima_bbb,
	CARD_ADDRESS_61H_mima_bbb,
	CARD_ADDRESS_61L_mima_bbb,
	CARD_ADDRESS_62E_mima_ccc,
	CARD_ADDRESS_62N_mima_ccc,
	CARD_ADDRESS_62H_mima_ccc,
	CARD_ADDRESS_62L_mima_ccc,
	CARD_ADDRESS_63E_mima_ddd,
	CARD_ADDRESS_63N_mima_ddd,
	CARD_ADDRESS_63H_mima_ddd,
	CARD_ADDRESS_63L_mima_ddd,
	CARD_ADDRESS_64E_mima_eee,
	CARD_ADDRESS_64N_mima_eee,
	CARD_ADDRESS_64H_mima_eee,
	CARD_ADDRESS_64L_mima_eee,
	CARD_ADDRESS_65E_mima_fff,
	CARD_ADDRESS_65N_mima_fff,
	CARD_ADDRESS_65H_mima_fff,
	CARD_ADDRESS_65L_mima_fff,
	CARD_ADDRESS_66E_mima_ggg,
	CARD_ADDRESS_66N_mima_ggg,
	CARD_ADDRESS_66H_mima_ggg,
	CARD_ADDRESS_66L_mima_ggg,
	CARD_ADDRESS_67E_mima_hhh,
	CARD_ADDRESS_67N_mima_hhh,
	CARD_ADDRESS_67H_mima_hhh,
	CARD_ADDRESS_67L_mima_hhh,
	CARD_ADDRESS_68E_mima_gekiha,	/*(r36�����I�Ɍ��j)*/
	CARD_ADDRESS_68N_mima_gekiha,	/*(r36�����I�Ɍ��j)*/
	CARD_ADDRESS_68H_mima_gekiha,	/*(r36�����I�Ɍ��j)*/
	CARD_ADDRESS_68L_mima_gekiha,	/*(r36�����I�Ɍ��j)*/
// �A���X 1��
	CARD_ADDRESS_00E_alice_000,
	CARD_ADDRESS_00N_alice_000,
	CARD_ADDRESS_00H_alice_000,
	CARD_ADDRESS_00L_alice_000,
	CARD_ADDRESS_71E_alice_bbb,
	CARD_ADDRESS_71N_alice_bbb,
	CARD_ADDRESS_71H_alice_bbb,
	CARD_ADDRESS_71L_alice_bbb,
	CARD_ADDRESS_72E_alice_ccc,
	CARD_ADDRESS_72N_alice_ccc,
	CARD_ADDRESS_72H_alice_ccc,
	CARD_ADDRESS_72L_alice_ccc,
	CARD_ADDRESS_73E_alice_ddd,
	CARD_ADDRESS_73N_alice_ddd,
	CARD_ADDRESS_73H_alice_ddd,
	CARD_ADDRESS_73L_alice_ddd,
	CARD_ADDRESS_74E_alice_eee,
	CARD_ADDRESS_74N_alice_eee,
	CARD_ADDRESS_74H_alice_eee,
	CARD_ADDRESS_74L_alice_eee,
	CARD_ADDRESS_75E_alice_fff,
	CARD_ADDRESS_75N_alice_fff,
	CARD_ADDRESS_75H_alice_fff,
	CARD_ADDRESS_75L_alice_fff,
	CARD_ADDRESS_76E_alice_gekiha,	/*(r36�����I�Ɍ��j)*/
	CARD_ADDRESS_76N_alice_gekiha,	/*(r36�����I�Ɍ��j)*/
	CARD_ADDRESS_76H_alice_gekiha,	/*(r36�����I�Ɍ��j)*/
	CARD_ADDRESS_76L_alice_gekiha,	/*(r36�����I�Ɍ��j)*/
	//
	CARD_ADDRESS_MAX/* �ő吔 */
};


/*
	�X�y���V�X�e��==�J�[�h�̊Ǘ��V�X�e���B(�J�[�h�V�X�e���Ƃ͈Ⴄ�̂Œ���)
*/


	/*---------------------------------------------------------
	�X�y���V�X�e��(���炩���߃J�[�h�̒�`�����Ă����V�X�e��)
	-------------------------------------------------------
	�͕핗�ł̓X�y���̓V�X�e���ŃT�|�[�g���Ă��܂��B
	�X�y���Ƃ��Ē�`(�J�[�h)���Ă����ƁA�ǂ̃{�X���A�ǂ̃X�y���ł����Ă�悤�ɂȂ�܂��B
	-------------------------------------------------------
	�܂��X�y���̓J�[�h�V�X�e���ɔԍ���o�^���Ă����ƁA�J�[�h�V�X�e���ŃX�y�����擾�ł��܂��B
	(�܂�J�[�h�V�X�e���o�R�ŃX�y�������Ă܂�)
	-------------------------------------------------------
	(r32)���݁A�G��(�͕핗��dat�̒�-�{�X)���X�y�������Ă܂����A
	���̋@�\�͏����Ȃ��Ȃ邩���m��܂���B
	�G��(�͕핗��dat�̒�-�{�X)���X�y�������Ă�@�\�́u���_�\�v�Ŏv�������̂ł����A
	���������@�\�����������Q�[���̑��x�������o����C�����邵�A
	�����������炱�������@�\�͖{�Ƃɖ����̂����Ǝv�����Ƃ�����܂��B�ǂ�����܂���B
	-------------------------------------------------------
	card.spell_used_number �Ɍ��������X�y�����Z�b�g����ƁA�X�y�������J�n�B
	�X�y���������I������Ǝ����I�� card.spell_used_number �� DANMAKU_00 �ɂȂ�B
	-------------------------------------------------------
	�X�y���������� REG_10_BOSS_SPELL_TIMER ������ɖ��t���[�����Z����A0 �ɂȂ�ƃX�y�������I���B
	���̎��Ԃ̓X�y�����ƂɈႤ�W�����Ԃ��ݒ肳��Ă���B
	����̓J�[�h�̏��������Ŏ��������������B
	-------------------------------------------------------
	�J�[�h�̏������B
	�J�[�h���ς��Ɩ���s���K�v������B
	---------------------------------------------------------*/

/* �J�[�h�ԍ�(�e�X�y������ʂ���ׂ̔ԍ�) */
enum
{
	SPELL_00 = 0,						/* �X�y�������I���t���O�B */
//	/* �G���p */
	SPELL_01_sakuya_misogi_normal,		/* [���] �g5�ʒ�-�{�X �S�J�[�h / �g5�ʒ�-�{�X ��� �u�ʏ�U���v */
	SPELL_02_sakuya_old_test,			/* [���] �g5�ʒ�-�{�X 24�J�[�h (��p�u�~�X�f�B���N�V�������ǂ�(1/2)�v) */
	SPELL_03_sakuya_old_test,			/* [���] �g5�ʒ�-�{�X 11�J�[�h (��p�u�~�X�f�B���N�V�������ǂ�(2/2)�v) */
	SPELL_04_sakuya_pink_hearts,		/* [���] �g5�ʃ{�X [���] �u�ʏ�U��1(1/2)�v�ɂ�����Ƃ��������J�[�h(�\��) */
	SPELL_05_sakuya,					/* [���] �g5�ʃ{�X [���] �u�ʏ�U��1/2(2/2)�v�ɂ�����Ƃ��������J�[�h(�\��) */
	SPELL_06_sakuya,					/* [���] �g5�ʃ{�X [���] ���ہu���i�N���b�N(1/2)�v�ɂ�����Ƃ��������J�[�h(�\��) */
	SPELL_07_sakuya,					/* [���] �g5�ʃ{�X [���] �u�ʏ�U��3(1/2)�v�ɂ�����Ƃ��������J�[�h(�\��) */
//
	SPELL_08_rumia, 					/* [���[�~�A] �g1�ʃ{�X [���[�~�A] �镄�u�i�C�g�o�[�h�v�ɂ�����Ƃ��������J�[�h(�\��) */
	SPELL_09_alice, 					/* [�A���X] ���ۂ��H�e�L�g�[�J�[�h */
	SPELL_0a_dai_yousei_01, 			/* [��d��] �S�� 48�����A���v���A�΃N�i�C�e */
	SPELL_0b_dai_yousei_02, 			/* [��d��] �S�� 48�����A�����v���A�ԃN�i�C�e */
	SPELL_0c_sakuya_jack32, 			/* [���] ���H�u�W���b�N�E�U�E���h�r���v�ɂȂ邩������Ȃ��e�X�g */
	SPELL_0d_sakuya_miss_direction, 	/* [���] ��p�u�~�X�f�B���N�V�����v */
	SPELL_0e_remilia_00,				/* [���~���A] �e�X�g */
	SPELL_0f_tengu, 					/* [���̑�] �������̗d��1(�V��l?)�����J�[�h(�\��) */ 	/*???*/
//
	SPELL_10_cirno, 					/* [�`���m] �S�J�[�h */
	SPELL_11_perfect_freeze,			/* [�`���m] �p�[�t�F�N�g�t���[�Y */
	SPELL_12_diamond_blizzard,			/* [�`���m] �ᕄ�u�_�C�A�����h�u���U�[�h�v */
	SPELL_13_alice_suwako,				/* [�A���X] �z�K�q���ۂ�(?)�J�[�h */
	SPELL_14_alice_youmu300,			/* [�A���X] �d3�ʃ{�X  normal�ʏ�U��3(�̈ꕔ) */
	SPELL_15_alice_aka_2nd, 			/* [�A���X] ��EX�ʃ{�X �Ԃ̖��@2�i�K�ڂ��ǂ� */
	SPELL_16_alice_zako_doll,			/* [�A���X] �d3�ʃ{�X [�A���X]�l�`�J�[�h(���ǂ�) */ // 40[sec]
	SPELL_17_alice_tama_doll_02,		/* [�A���X] �쐬�� */
//
	SPELL_18_hana_test, 				/* [����] �ԂĂ��ƃJ�[�h */
	SPELL_40_SONOTA_DEBUG_CW_AO,		/* [���̑�] �f�o�b�O�J�[�hCW�� */
	SPELL_41_SONOTA_DEBUG_CCW_AKA,		/* [���̑�] �f�o�b�O�J�[�hCCW�� */
	SPELL_1b_cirno_icecle_fall, 		/* [�`���m] �X���u�A�C�V�N���t�H�[���v */
	SPELL_44_ruiz3_test,				/* [���C�Y] �J�[�h��̂Ă��ƁB */
	SPELL_1d_amefuri_test,				/* [] �J�Ă��� */
	SPELL_1e_momiji_no_nonoji,			/* [��] �̂̂��J�[�h */
	SPELL_1f_koakuma,					/* [������] �Ă��Ɨp */
//	/* �{�X�p */
	SPELL_20_aya_misogi1,				/* 1[��] �̒ʏ�e���ۂ��J�[�h�������Ă݂�e�X�g1 */
	SPELL_21_aya_misogi2,				/* 2[��] �̒ʏ�e���ۂ��J�[�h�������Ă݂�e�X�g2 */
	SPELL_22_ame_no_yatimata,			/* 3[��] �򕄁u�V�̔��ˁv */
	SPELL_23_meirin_magaru_kunai,		/* 4[����] �ʏ�U�� */
	SPELL_24_aya_doll,					/* 5[��] �l�`�J�[�h */ //40[sec]
	SPELL_25_houka_kenran,				/* 6[�g����] �ؕ��u�F�؈�ࣁv�ɂ�����Ƃ��������J�[�h(�\��) */
	SPELL_26_aya_saifu, 				/* 7[��] �Ǖ� */
	SPELL_27_hosigata_test, 			/* 8[���̑�] ���^�e�X�g */
//
	SPELL_28,							/* ���� */
	SPELL_29_rumia_demarcation, 		/* 10[���[�~�A] �g1�ʃ{�X �ŕ��u�f�B�}�[�P�C�V�����v�ɂ�����Ƃ��������J�[�h(�\��) */
	SPELL_2a_sakuya_baramaki1,			/* 11[���] �g5�ʒ�-�{�X �΂�T��1 �J�[�h () */
	SPELL_2b_sakuya_baramaki2,			/* 12[���] �g5�ʒ�-�{�X �΂�T��2 �J�[�h () */
	SPELL_2c_sakuya_blue_red,			/* 13[���] �g5�ʒ�-�{�X �u�ʏ�U��3�v�ԃi�C�t */
	SPELL_2d_sakuya_misogi_lunatic, 	/* 14[���] �g5�ʒ�-�{�X �S�J�[�h / �g5�ʒ�-�{�X ��� �u�ʏ�U���vlunatic */
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
	SPELL_42_PACHE_LASER1,				/* [�p�`�F] �u�ʏ�U��1�v�̃��[�U�[���ǂ� */
	SPELL_43_PACHE_LASER2,				/* [�p�`�F] �u�ʏ�U��3�v�̃��[�U�[���ǂ� */
	SPELL_19_mima_sekkin,				/* [����] �ڋ߃J�[�h */
	SPELL_1a_mima_toge, 				/* [����] �g�Q�Ă��ƃJ�[�h */
	SPELL_1e_kaguya01,					/* [�P��] �ނ���ϊ�1 */
	SPELL_1f_kaguya04,					/* [�P��] �ނ���ϊ�2 */
	SPELL_1c_kakuya_tamanoe,			/* [�P��]�A�H���̋ʂ̎}���ǂ��B�쐬�� */
	SPELL_47_sakuya_festival_knife, 	/* [���] ���z�u�t�F�X�e�B�o���i�C�t�v(?) �ނ���ϊ� */
//
	#if (1)
	SPELL_48_r34_gokan_kinou,			/* [r34]r34�ȑO�̌݊��@�\�B(r35-)�V�X�e���ŕK���K�v�B */
	SPELL_49,							/* ���� */
	SPELL_4a,							/* ���� */
	SPELL_4b,							/* ���� */
	SPELL_4c,							/* ���� */
	SPELL_4d_remilia_rasen_knife,		/* 9[���/���~���A] �g6�ʃ{�X �ԃi�C�t�����e(�\��) */
	SPELL_4e_remilia_vlad_tepes,		/* 9[���/���~���A] �g6�ʃ{�X ���f�u�u���h�E�c�F�y�V���̎􂢁v*/
	SPELL_4f_remilia_red_magic, 		/* 9[���/���~���A] �g6�ʃ{�X �u���b�h�}�W�b�N�v���ۂ�(?) */
	#endif
//
	SPELL_MAX,
};
//	SPELL_1f_silent_selena, 			/* [�p�`�F] �T�C�����g�Z���i �� �Ă��Ɨp */

#endif /* _CARD_ADDRESS_VALUE_H_ */
