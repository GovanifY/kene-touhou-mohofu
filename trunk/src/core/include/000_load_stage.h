
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���̃t�@�C���͒��ڃC���N���[�h���܂���B
	"game_main.h" ����̂݊ԐړI�ɃC���N���[�h���܂��B
---------------------------------------------------------*/

#ifndef _LOAD_STAGE_FILE_H_
#define _LOAD_STAGE_FILE_H_

//#define  MAX_IVENT_ENTRY	512
#define  MAX_PARA1_36		/*64*/(64-20-8-4+32)/*(64-(4*5))*/

typedef struct _stage_command
{
	u32 		v_time; 			/* 1/60 �P�ʂ̎��ԃJ�E���^ */	//	int 		dummy_done;
	struct _stage_command *next;
	u32 		user_x; 			/* para 0 */	/*ctype*/	/* [***090210	�ǉ� */
	int 		user_y; 			/* para 2 */
//(16[Bytes] == (4x4) )
	u8			user_i_code;		/* intermediate code ���ԃR�[�h */
	u8			user_255_code;		/* 'E'�Ƃ�'T'�Ƃ�	*/
	u8			user_item8; 		/* item */
	u8			user_select_gazou;	/* �A�j��/�摜��� */
	int 		user_hp;			/* para 2 */
	int 		user_score; 		/* para 2 */
	int 		user_kougeki_type;	/* �G���U������ꍇ�̃X�y�J�̎�� */
//(20[Bytes] == (4x5) )
	char		user_string[MAX_PARA1_36/*(64)*/];/* para 1 */
//(64[Bytes] == (4x4)+(4)+(44) )
} GAME_COMMAND; 	/* .align 64 [bytes] */
//	u16 	dummy_scroll_speed256;	/* para3 scroll speed256 */ /* [***090210	�ǉ� */


/* ���ԃR�[�h�`���̃R�}���h */
/* �Q�[�����́A���ԃR�[�h�`���̃R�}���h�������\����͂͂��Ȃ��B(�\����͂�load_stage.c�ōs��) */

enum
{
/*���ԃR�[�h*/
/*0x00*/		CTYPE_00_NONE = 0,/* [�ԕ���؂�] */	/*error*/
			/* ���̑� */
/*0x01*/		CTYPE_01_ALL_CLEAR, 			/* �Q�[�� �S�X�e�[�W �N���A�[ */
/*0x02*/		CTYPE_02_BG_CONTROL,
				// �G
/*0x03*/		CTYPE_03_BOSS_COMMON,			/* (�{�X / ��-�{�X)�p �X�N���v�g�N�� */
/*0x04*/		CTYPE_04_CHUU_BOSS_COMMON,		/* �U�R��-�{�X(�X�N���v�g�N�����Ȃ�) */
/*0x05*/		CTYPE_05_ZAKO,					/* �U�R */
				//
/*0x06*/		ETYPE_01_SJIS_TEXT,
/*0x07*/		ETYPE_02_LOAD_BG,
/*0x08*/	//	ETYPE_03_PICTURE,	/* ���̕����͏��������������ɂ����̂œs���ɂ��p�~ */
				//
/*0x09*/		/* �g���\ */
/*...*/ 		/* ... */
/*0x2f*/		/* �g���\ */
/*0x30*/		ETYPE_MAX,		/* 0x30 == '0' == �e���R�}���h (== ���x�R�}���h�A�p�~) */
};

/* ���� 32 ��� */
//#define CTYPE_99_MAX (CTYPE_17_AO_YOUSEI4+1)
#define CTYPE_99_MAX (ETYPE_01_SJIS_TEXT)

#endif /* _LOAD_STAGE_FILE_H_ */
