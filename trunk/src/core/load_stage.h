#ifndef _LOAD_STAGE_FILE_H_
#define _LOAD_STAGE_FILE_H_

#include "game_main.h"

//#define  MAX_IVENT_ENTRY	512
#define  MAX_PARA1_36		/*64*/(64-20-8)/*(64-(4*5))*/

typedef struct _stage_data
{
	u32 		v_time; 			/* 1/60 �P�ʂ̎��ԃJ�E���^ */	//	int 		dummy_done;
	struct _stage_data *next;
	u32 		user_x; 			/* para 0 */	/*ctype*/	/* [***090210	�ǉ� */
	int 		user_y; 			/* para 2 */
//(16[Bytes] == (4x4) )
	char		user_i_code;		/* intermediate code ���ԃR�[�h */
	char		user_1_moji;		/* 'E'�Ƃ�'T'�Ƃ�	*/
	u16 		user_item16;		/* item */
	int 		user_hp; 			/* para 2 */
	int 		user_score; 		/* para 2 */
//(20[Bytes] == (4x5) )
	char		user_string[MAX_PARA1_36/*(64)*/];/* para 1 */
//(64[Bytes] == (4x4)+(4)+(44) )
} STAGE_DATA;	/* .align 64 [bytes] */
//	u16 	dummy_scroll_speed256;	/* para3 scroll speed256 */ /* [***090210	�ǉ� */



/* ���ԃR�[�h�`���̃R�}���h */
/* �Q�[�����́A���ԃR�[�h�`���̃R�}���h�������\����͂͂��Ȃ��B(�\����͂�load_stage.c�ōs��) */

enum
{
/*���ԃR�[�h*/
/*0x00*/		CTYPE_00_NONE = 0,/* [�ԕ���؂�] */	/*error*/
			/* ���̑� */
/*0x01*/		CTYPE_01_ALL_CLEAR,
/*0x02*/		CTYPE_02_BG_CONTROL,
			/* �{�X */
/*0x03*/		CTYPE_03_BOSS_KENE,
/*0x04*/		CTYPE_04_BOSS_CIRNO,
/*0x05*/		CTYPE_05_BOSS_SAKUYA,	// [***090207		�ǉ�
/*0x06*/		CTYPE_06_BOSS_PACHE,
/*0x07*/		CTYPE_07_BOSS_KAGUYA,
/*0x08*/		CTYPE_08_BOSS_EIRIN,		/* 3�ʂ͂ǂ����邩���� */
/*0x09*/		CTYPE_09_BOSS_FUMI,
/*0x0a*/		CTYPE_10_BOSS_ALICE,
			/* ����G[���^�G] */
/*0x0b*/		CTYPE_11_6, 			//		�ǉ�
/*0x0c*/		CTYPE_12_KOAKUMA,		//		�ǉ�
/*0x0d*/		CTYPE_13_UDONGE,		//		�ǉ�
/*0x0e*/		CTYPE_14_TEI,			//		�ǉ�
/*0x0f*/		CTYPE_15_2, 			//		�ǉ�
/*0x10*/		CTYPE_16_RUMIA, 		//		�ǉ�
			//		�p�~?
/*0x11*/		CTYPE_17_YOKAI2,		//		�ǉ�
/*0x12*/		CTYPE_18_YOKAI1,		// [***090207		�ǉ�
			/* �����w */
/*0x13*/		CTYPE_19_MAHOUJIN_A,
/*0x14*/		CTYPE_20_MAHOUJIN_B,
			/* ���� �A�z�� */
/*0x15*/		CTYPE_21_INYOU1,
/*0x16*/		CTYPE_22_TATSUMAKI1,
			/* �d�� */
/*0x17*/		CTYPE_23_KAKOMI1,
/*0x1a*/		CTYPE_26_AKA_KEDAMA1,		/* �� */
			/* �ы� */
/*0x18*/		CTYPE_24_YUKARI1,			/* ���̑��U�R */
/*0x19*/		CTYPE_25_YUKARI2,			/* ���̑��U�R */
/*0x1b*/		CTYPE_27_MIDORI_KEDAMA1,
/*0x1c*/		CTYPE_28_MIDORI_KEDAMA2,
/*0x1d*/		CTYPE_29_KEDAMA1,
/*0x1e*/		CTYPE_30_KEDAMA2,
			/* [C�d��]���̑��U�R */
/*0x1f*/		CTYPE_31_OBAKE1,
/*0x20*/		CTYPE_32_OBAKE2,	/* ��"���ы�1" */
/*0x21*/		CTYPE_33_KARASU,
			/* [B�d��]���d�� */
/*0x22*/		CTYPE_34_MEIDO1,
/*0x23*/		CTYPE_35_MEIDO2,
/*0x24*/		CTYPE_36_MEIDO3,
/*0x25*/		CTYPE_37_MEIDO4,
			/* [A�d��]���d�� */
/*0x26*/		CTYPE_38_AO_YOUSEI1,	// [***090124		�ǉ�
/*0x27*/		CTYPE_39_AO_YOUSEI2,	// [***090207		�ǉ�
/*0x28*/		CTYPE_40_AO_YOUSEI3,	//		�ǉ�
/*0x29*/		CTYPE_41_AO_YOUSEI4,	//		�ǉ�
				//
				//
/*0x2a*/		ETYPE_01_SJIS_TEXT,
/*0x2b*/		ETYPE_02_LOAD_BG,
/*0x2c*/	//	ETYPE_03_PICTURE,	/* ���̕����͏��������������ɂ����̂œs���ɂ��p�~ */
				//
/*0x2d*/		/*�g���\*/
/*0x2e*/		/*�g���\*/
/*0x2f*/		/*�g���\*/
/*0x30*/		ETYPE_MAX,		/* 0x30 == '0' == �e���R�}���h (== ���x�R�}���h�A�p�~) */
};

/* ���� 32 ��� */
//#define CTYPE_99_MAX (CTYPE_17_AO_YOUSEI4+1)
#define CTYPE_99_MAX (ETYPE_01_SJIS_TEXT)

#endif /* _LOAD_STAGE_FILE_H_ */
