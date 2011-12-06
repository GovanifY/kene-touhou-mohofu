
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
---------------------------------------------------------*/

#ifndef _KAIWA_OBJ_00_H_
#define _KAIWA_OBJ_00_H_

/*(���݋K�i�́A�Ƃ肠�������̂��́B���OBJ�Ɠ�������B)*/
typedef struct /* _kaiwa_obj_tag_ */
{
	POINT256 	center;		/* obj�̒��S x ���W, ���x�m�ۗp(256�Œ菬���_�`��) */
							/* obj�̒��S y ���W, ���x�m�ۗp(256�Œ菬���_�`��) */
	POINT256 	target; 	/* �ڕW x ���W */
							/* �ڕW y ���W */
//[4==1*4]
	POINT256 	origin; 	/* ���_ x ���W */
							/* ���_ y ���W */
	POINT256 	offset; 	/* ���_����̍��� x ���W */
							/* ���_����̍��� y ���W */
//[8==2*4]
	POINT256 	alt;		/* �ȑO�� x ���W(��Ԉړ��p) */
							/* �ȑO�� y ���W(��Ԉړ��p) */
	int width_2n;// 	2^n �P�ʂł̉摜�����B(256�Ƃ�128�Ƃ�)			int w_bit;				/* obj�̕�(1<<w_bit) */
	int height_2n;// 	2^n �P�ʂł̉摜�����B(256�Ƃ�128�Ƃ�)			int h_bit;				/* obj�̍���(1<<h_bit) */
//[12==3*4]
	int move_flag;		/* 0:��~���B 0�ȊO(���� 1):�ړ����B */
	int draw_flag;		/* 0:�`�悵�Ȃ��B 0�ȊO(���� 1):�`�悷��B */
	u32 color32;		/* �������p */
	int toutatu_wariai256;	/* (r36)�����G�ړ���t256�`���B(0%...100% == 0...256)  */
//[16==4*4]
} KAIWA_OBJ;
/*
����: width_2n �� 2^n �ł��鎖�Ƃ��������ȊO�ɁAGu�`��̐����� 64�Ŋ���؂��K�v������܂��B
�܂�A 64, 128, 256, 512 �ȊO�̒l�͏o���܂���B
*/
	//	int dummy_speed256;		/* �ړ����x */		//	int move_wait;	/* �������������Ƃ��̐���p */
	//	int dummy_angle1024;		/* ���m�ȕ��� */	//	int r_course;


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
	KAIWA_OBJ_00_TACHIE_RIGHT = 0,	//
	KAIWA_OBJ_01_TACHIE_LEFT_,		//
	KAIWA_OBJ_99_MAX/*�ő�l*/		//	(2/*32*/)	/*32*/ /*20*/
};
enum
{
	KAIWA_OBJ_MOVE_FLAG_00_MOVE_COMPLETE = 0,
	KAIWA_OBJ_MOVE_FLAG_01_MOVE_START,
	KAIWA_OBJ_MOVE_FLAG_99_MAX/*�ő�l*/
};

/*static*/extern KAIWA_OBJ kaiwa_sprite[KAIWA_OBJ_99_MAX];	/* �ėp�X�v���C�g */

extern void kaiwa_obj_set_256(void);/*(��b�X�v���C�g�̐ݒ�B�T�C�Y�� 256x256�ɂ���B)*/

	/* �����G��`�悷��/���Ȃ��B�S�Ă̂��������ΏہB */
extern void kaiwa_all_obj_draw_on_off(unsigned int on_off);

#endif /* _KAIWA_OBJ_00_H_ */
