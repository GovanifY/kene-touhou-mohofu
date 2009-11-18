#ifndef _LOAD_STAGE_FILE_H_
#define _LOAD_STAGE_FILE_H_

#include "game_main.h"

//#define  MAX_IVENT_ENTRY	512
#define  MAX_PARA1_44		/*64*/44-4/*(64-(4*5))*/

typedef struct _stage_data
{
	Uint32 v_time;		/* 1/60 �P�ʂ̎��ԃJ�E���^ */
	int done;
	Uint32 user_x;		/* para 0 */	/*ctype*/	/* [***090210	�ǉ� */
	int user_y; 		/* para 2 */
//(16[Bytes] == (4x4) )
	Uint16 scroll_speed256; /* para3 scroll speed256 */ /* [***090210	�ǉ� */
	char user_command;
	char user_command_B; /*resurved now*/		/* [***090210	�ǉ� */
//(20[Bytes] == (4x4)+(4) )
	char user_string[MAX_PARA1_44/*64*/];/* para 1 */
//(64[Bytes] == (4x4)+(4)+(44) )
	struct _stage_data *next;
} STAGE_DATA;

/* ���ԃR�[�h�`���̃R�}���h */
/* �Q�[�����́A���ԃR�[�h�`���̃R�}���h�������\����͂͂��Ȃ��B(�\����͂�loadlv.c�ōs��) */

enum
{
/*���ԃR�[�h*/
/*0x00*/		CTYPE_00_NONE = 0,/* [�ԕ���؂�] */	/*error*/
			//	���̑�
/*0x01*/		CTYPE_01_ALL_CLEAR,
			//	�{�X
/*0x02*/		CTYPE_19_BOSS_ALICE,
/*0x03*/	//	CTYPE_20_BOSS_CIRNO,
/*0x04*/		CTYPE_21_BOSS_FUMI,
/*0x05*/		CTYPE_22_BOSS_MITEI,		/* 3�ʂ͂ǂ����邩���� */
/*0x05*/		CTYPE_22_BOSS_KAGUYA,
/*0x06*/		CTYPE_22_BOSS_PACHE,
/*0x07*/		CTYPE_23_BOSS_SAKUYA,	// [***090207		�ǉ�
			//	����G[���^�G]
/*0x08*/		CTYPE_18_YOKAI1,		// [***090207		�ǉ�
/*0x09*/		CTYPE_18_YOKAI2,		//		�ǉ�
			//	���̑��U�R
/*0x0a*/		CTYPE_01_OBAKE,
/*0x0b*/		CTYPE_02_YUKARI1,
/*0x0c*/		CTYPE_03_YUKARI2,
			//	�ы�
/*0x0d*/		CTYPE_04_AKA_KEDAMA1,
/*0x0e*/		CTYPE_05_NIJI_KEDAMA1,
/*0x0f*/		CTYPE_06_MIDORI_KEDAMA1,
/*0x10*/		CTYPE_07_MIDORI_KEDAMA2,
/*0x11*/		CTYPE_08_KEDAMA1,
/*0x12*/		CTYPE_08_KEDAMA2,
			//	���� �A�z��
/*0x13*/		CTYPE_09_INYOU1,
/*0x14*/		CTYPE_10_TATSUMAKI1,
			//	�d��
/*0x15*/		CTYPE_11_KAKOMI1,
			//	���d��
/*0x16*/		CTYPE_12_MEIDO1,
/*0x17*/		CTYPE_13_MEIDO2,
/*0x18*/		CTYPE_14_MEIDO3,
/*0x19*/		CTYPE_15_MEIDO4,
			//	���d��
/*0x1a*/		CTYPE_16_AO_YOUSEI1,	// [***090124		�ǉ�
/*0x1b*/		CTYPE_17_AO_YOUSEI2,	// [***090207		�ǉ�
/*0x1c*/		CTYPE_17_AO_YOUSEI3,	//		�ǉ�
/*0x1d*/		CTYPE_17_AO_YOUSEI4,	//		�ǉ�
				//
/*0x1e*/		BTYPE_00_NONE,/* [�ԕ���؂�] */	/*error*/
/*0x1f*/		BTYPE_01_BGPANEL1,
/*0x20*/		BTYPE_02_BGPANEL2,
/*0x21*/		BTYPE_03_GROUNDER,
/*0x22*/		BTYPE_04_MAGIC_FORMATION,
/*0x23*/		BTYPE_05_BG_CONTROL,
				//
/*0x24*/		ETYPE_01_ENGLISH_TEXT,
/*0x25*/		ETYPE_02_LOAD_BG,
/*0x26*/	//	ETYPE_03_PICTURE,	/* ���̕����͏��������������ɂ����̂œs���ɂ��p�~ */
				//
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
/* ���� 23 ��� */
#define CTYPE_MAX_23_ BTYPE_00_NONE/*(23)*/
//#undef CTYPE_MAX_23_

#define BTYPE_MAX (BTYPE_05_BG_CONTROL+1)

#endif /* _LOAD_STAGE_FILE_H_ */
