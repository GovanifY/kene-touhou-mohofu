
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
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
	�w�i�J�[�h
---------------------------------------------------------*/

/* �J�[�h���ɔw�i�̃v���O������ނ�ݒ� */

/*---------------------------------------------------------

---------------------------------------------------------*/

//extern void psp_push_screen(void);/* ���z�X�N���[����ޔ� */
//extern void psp_pop_screen(void);/* ���z�X�N���[���𕜊� */
#define psp_push_screen(aaa) psp_move_screen( SDL_00_VIEW_SCREEN, SDL_01_BACK_SCREEN )
#define psp_pop_screen(aaa)  psp_move_screen( SDL_01_BACK_SCREEN, SDL_00_VIEW_SCREEN )
extern void psp_move_screen(int src_screen_number, int dst_screen_number);


/*---------------------------------------------------------
	���j���[�V�X�e��
---------------------------------------------------------*/

extern void psp_pause_filter(void);/* ���z�X�N���[���ɂ������G�t�F�N�g�������� */
extern void psp_clear_screen(void);/* ���z�X�N���[�������ŏ��� */


#endif /* _PSP_GU_RENDER_H_ */
