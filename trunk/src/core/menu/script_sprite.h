
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
---------------------------------------------------------*/

#ifndef _SCRIPT_SPRITE_00_H_
#define _SCRIPT_SPRITE_00_H_

typedef struct /*_script_sprite*/
{
	int cx256;			/* obj�̒��S���W, ���x�m�ۗp(256�Œ菬���_�`��) */	//	int/*int*/ x;		/* x���W���� */
	int cy256;			/* obj�̒��S���W, ���x�m�ۗp(256�Œ菬���_�`��) */	//	int/*int*/ y;		/* y���W���� */
//[4]
	int w;				/* obj�̕� */			//	int cw; 	/* ���S���W(��) */
	int h;				/* obj�̍��� */ 		//	int ch; 	/* ���S���W(����) */
//
	int target_x256;	/* �ڕW���W */
	int target_y256;	/* �ڕW���W */
//[8]
	int set_speed;		/* �ړ����x */		//	int move_wait;	/* �������������Ƃ��̐���p */
	int angle1024;		/* ���m�ȕ��� */	//	int r_course;	/* ��̂̕��� */
//[12]
	SDL_Surface *img;		/* SDL �摜 Images. NULL�̏ꍇ�͉摜�������B */
//	UINT16 *render_image;	/* �摜data */
//	int used;
//
//	u8 alpha255;		/* alpha�l */
//	u8 flags;			/* 0:��\��, 1:�\��, 2:tachie_window(2nd screen)�ɕ\��	�����ł͕\�������邩�ǂ������� */
} SCRIPT_SPRITE;
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
	/*#define*/ SCRIPT_SPRITE_00_TACHIE_RIGHT = 0,	//; (30&1)/*(-2)*/	/*(==0)*/
	//#define SPRITE_tachie_m (31)/*(-1)*/		//	/*(==1)*/
	/*#define*/ SCRIPT_SPRITE_01_TACHIE_LEFT_,		//	(29&1)/*(-3)*/	/*(==1)*/
	/*#define*/ SCRIPT_SPRITE_99_MAX/*�ő�l*/		//	(2/*32*/)
};

//#define SPR ITE_tachie_l (29&1)/*-3*/
//#define SPR ITE_tachie_r (30&1)/*-2*/
//#define SPR ITE_tachie_m (31)/*-1*/
//#define SCRIPT_TACHIE_OBJ_MAX		(2/*32*/)

//static SCRIPT_SPRITE *tachie_r;
//static SCRIPT_SPRITE *tachie_l;
//static SCRIPT_SPRITE *std_obj[SCRIPT_TACHIE_OBJ_MAX/*32*/ /*20*/];		/* �ėp�X�v���C�g */
/*static*/extern SCRIPT_SPRITE standard_script_sprite[SCRIPT_SPRITE_99_MAX/*32*/ /*20*/];


#endif /* _SCRIPT_SPRITE_00_H_ */
