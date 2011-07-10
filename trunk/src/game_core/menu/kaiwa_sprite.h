
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
---------------------------------------------------------*/

#ifndef _KAIWA_SPRITE_00_H_
#define _KAIWA_SPRITE_00_H_

typedef struct /* _kaiwa_sprite_tag_ */
{
	int cx256;			/* obj�̒��S x ���W, ���x�m�ۗp(256�Œ菬���_�`��) */
	int cy256;			/* obj�̒��S y ���W, ���x�m�ۗp(256�Œ菬���_�`��) */
	int target_x256;	/* �ڕW x ���W */
	int target_y256;	/* �ڕW y ���W */
	int origin_x256;	/* ���_ x ���W */
	int origin_y256;	/* ���_ y ���W */
	int offset_x256;	/* ���_����̍��� x ���W */
	int offset_y256;	/* ���_����̍��� y ���W */
//[4]
	int speed256;		/* �ړ����x */		//	int move_wait;	/* �������������Ƃ��̐���p */
	int angle1024;		/* ���m�ȕ��� */	//	int r_course;
	int w_bit;				/* obj�̕�(1<<bit) */
	int h_bit;				/* obj�̍���(1<<bit) */
//[8]
	int move_flag;		/* 0:��~���B 0�ȊO(���� 1):�ړ����B */
	SDL_Surface *img;	/* SDL �摜 Images. NULL�̏ꍇ�͉摜�������B */
//[12]
} KAIWA_SPRITE;
//	u16 *render_image;	/* �摜data */
//	int used;
//
//	u8 alpha255;		/* alpha�l */
//	u8 flags;			/* 0:��\��, 1:�\��, 2:tachie_window(2nd screen)�ɕ\��	�����ł͕\�������邩�ǂ������� */
/*
-3 ==1 29
-2 ==0 30
-1 ==1 31
 0 ==0 32
 1 ==1
 2 ==0
 3 ==1
*/
enum
{
	KAIWA_SPRITE_00_TACHIE_RIGHT = 0,	//
	KAIWA_SPRITE_01_TACHIE_LEFT_,		//
	KAIWA_SPRITE_99_MAX/*�ő�l*/		//	(2/*32*/)	/*32*/ /*20*/
};
enum
{
	KAIWA_SPRITE_MOVE_FLAG_00_MOVE_COMPLETE = 0,
	KAIWA_SPRITE_MOVE_FLAG_01_MOVE_START,
	KAIWA_SPRITE_MOVE_FLAG_99_MAX/*�ő�l*/
};

/*static*/extern KAIWA_SPRITE kaiwa_sprite[KAIWA_SPRITE_99_MAX];	/* �ėp�X�v���C�g */


#endif /* _KAIWA_SPRITE_00_H_ */
