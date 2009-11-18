#ifndef _LOAD_STAGE_FILE_H_
#define _LOAD_STAGE_FILE_H_

#include "game_main.h"

//#define  MAX_IVENT_ENTRY	512
#define  MAX_PARA1_44		/*64*/44-4/*(64-(4*5))*/

typedef struct _stage_data
{
	Uint32		v_time; 		/* 1/60 �P�ʂ̎��ԃJ�E���^ */
	int 		done;
	Uint32		user_x; 		/* para 0 */	/*ctype*/	/* [***090210	�ǉ� */
	int 		user_y; 		/* para 2 */
//(16[Bytes] == (4x4) )
//	Uint16		scroll_speed256; /* para3 scroll speed256 */ /* [***090210	�ǉ� */
	char		user_i_code;	/* intermediate code ���ԃR�[�h */
	char		user_1_moji;	/* "E"�Ƃ�'T'�Ƃ�	*/
	Uint16		user_16;		/* */
//(20[Bytes] == (4x4)+(4) )
	char		user_string[MAX_PARA1_44/*64*/];/* para 1 */
//(64[Bytes] == (4x4)+(4)+(44) )
	struct _stage_data *next;
} STAGE_DATA;

/* ���ԃR�[�h�`���̃R�}���h */
/* �Q�[�����́A���ԃR�[�h�`���̃R�}���h�������\����͂͂��Ȃ��B(�\����͂�loadlv.c�ōs��) */

enum
{
/*���ԃR�[�h*/
/*0x00*/		CTYPE_00_NONE = 0,/* [�ԕ���؂�] */	/*error*/
			/* ���̑� */
/*0x01*/		CTYPE_01_ALL_CLEAR,
/*0x02*/		CTYPE_02_BG_CONTROL,
			/* �{�X */
/*0x03*/		CTYPE_03_BOSS_ALICE,
/*0x03*/	//	CTYPE_04_BOSS_CIRNO,
/*0x04*/		CTYPE_04_BOSS_FUMI,
/*0x05*/		CTYPE_05_BOSS_MITEI,		/* 3�ʂ͂ǂ����邩���� */
/*0x06*/		CTYPE_06_BOSS_KAGUYA,
/*0x07*/		CTYPE_07_BOSS_PACHE,
/*0x08*/		CTYPE_08_BOSS_SAKUYA,	// [***090207		�ǉ�
			/* ����G[���^�G] */
/*0x09*/		CTYPE_09_RUMIA, 		//  	�ǉ�
/*0x09*/		CTYPE_09_YOKAI1,		// [***090207		�ǉ�
/*0x0a*/		CTYPE_10_YOKAI2,		//		�ǉ�
			/* �����w */
/*0x0b*/		CTYPE_11_MAHOUJIN_A,
/*0x0c*/		CTYPE_12_MAHOUJIN_B,
			/* ���� �A�z�� */
/*0x0d*/		CTYPE_13_INYOU1,
/*0x0e*/		CTYPE_14_TATSUMAKI1,
			/* �d�� */
/*0x0f*/		CTYPE_15_KAKOMI1,
			/* ���̑��U�R */
/*0x10*/		CTYPE_16_OBAKE1,
/*0x14*/		CTYPE_20_OBAKE2,	/* ��"���ы�1" */
/*0x11*/		CTYPE_17_YUKARI1,
/*0x12*/		CTYPE_18_YUKARI2,
			/* �ы� */
/*0x13*/		CTYPE_19_AKA_KEDAMA1,
/*0x15*/		CTYPE_21_MIDORI_KEDAMA1,
/*0x16*/		CTYPE_22_MIDORI_KEDAMA2,
/*0x17*/		CTYPE_23_KEDAMA1,
/*0x18*/		CTYPE_24_KEDAMA2,
			/* ���d�� */
/*0x19*/		CTYPE_25_MEIDO1,
/*0x1a*/		CTYPE_26_MEIDO2,
/*0x1b*/		CTYPE_27_MEIDO3,
/*0x1c*/		CTYPE_28_MEIDO4,
			/* ���d�� */
/*0x1d*/		CTYPE_29_AO_YOUSEI1,	// [***090124		�ǉ�
/*0x1e*/		CTYPE_30_AO_YOUSEI2,	// [***090207		�ǉ�
/*0x1f*/		CTYPE_31_AO_YOUSEI3,	//		�ǉ�
/*0x20*/		CTYPE_32_AO_YOUSEI4,	//		�ǉ�
				//

				//
/*0x21*/		ETYPE_01_ENGLISH_TEXT,
/*0x22*/		ETYPE_02_LOAD_BG,
/*0x23*/	//	ETYPE_03_PICTURE,	/* ���̕����͏��������������ɂ����̂œs���ɂ��p�~ */
				//
/*0x23*/		/*�g���\*/
/*0x24*/		/*�g���\*/
/*0x25*/		/*�g���\*/
/*0x26*/		/*�g���\*/
/*0x27*/		/*�g���\*/
/*0x28*/		/*�g���\*/
/*0x29*/		/*�g���\*/
/*0x2a*/		/*�g���\*/
/*0x2b*/		/*�g���\*/
/*0x2c*/		/*�g���\*/
/*0x2d*/		/*�g���\*/
/*0x2e*/		/*�g���\*/
/*0x2f*/		/*�g���\*/
/*0x30*/		ETYPE_MAX,		/* 0x30 == '0' == ���x�R�}���h */
};

/* ���� 32 ��� */
//#define CTYPE_99_MAX (CTYPE_17_AO_YOUSEI4+1)
#define CTYPE_99_MAX (ETYPE_01_ENGLISH_TEXT)

#endif /* _LOAD_STAGE_FILE_H_ */
