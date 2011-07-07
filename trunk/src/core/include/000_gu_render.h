
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���̃t�@�C���͒��ڃC���N���[�h���܂���B
	"game_main.h" ����̂݊ԐړI�ɃC���N���[�h���܂��B
---------------------------------------------------------*/

#ifndef _PSP_GU_RENDER_H_
#define _PSP_GU_RENDER_H_

enum
{
	GU_HAIKEI_00_2D_256 = 0,	/* 256x256�e�N�X�`����~���l�߂�2D�B */
	GU_HAIKEI_01_3D_TEST01, 	/* 3D�̃e�X�g */
	GU_HAIKEI_02_EIENTEI,		/* "�i����"�̗\��B */
	GU_HAIKEI_03_FAKE_3D,		/* �e�N�X�`���ό`�ɂ��[��3D �����I�ɖ����Ȃ�\��B */
	GU_HAIKEI_99_MAX,
};

/*---------------------------------------------------------
	�w�i�w���p�[
---------------------------------------------------------*/

/* �w�i�������F�� 32bit color�Ŏw�肷��B */
extern void gu_set_bg_u32_clear_color(u32 set_u32_clear_color);


/*---------------------------------------------------------
	�w�i�̕`��v���O������o�^����B
	�w�i�X�y�J

---------------------------------------------------------*/

/* �X�y�J���łȂ��ʏ펞�̃v���O�������w�� */
extern void *callback_gu_draw_haikei_modosu;	/* (�X�y�J������ʏ펞��)�߂��p�B(==�ʏ펞) */

/* �X�y�J���̃v���O�������w�� */
extern void *callback_gu_draw_haikei_supeka;	/* �X�y�J���p */

/* ���ݎg�p����v���O������ݒ� */
extern void (*callback_gu_draw_haikei)(void);//unsigned int dr aw_bg_screen;	/* �w�i�E�B���h�E�\���t���O */

/* �Q�[�����A */
//extern void (*callback_gu_draw_haikei)(void);//unsigned int dr aw_bg_screen;				/* �w�i�E�B���h�E�\���t���O */
//extern void *callback_gu_draw_haikei_supeka;
//extern void *callback_gu_draw_haikei_modosu;


/* �X�y�J���ɔw�i�̃v���O������ނ�ݒ� */


#endif /* _PSP_GU_RENDER_H_ */
