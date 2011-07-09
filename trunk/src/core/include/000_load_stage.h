
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
	u32 		v_time; 			/* 1/60 �P�ʂ̎��ԃJ�E���^ */
	struct _stage_command *next;
	u32 		user_x; 			/* x */
	int 		user_y; 			/* y */
//(16[Bytes] == (4x4) )
	u8			user_i_code;		/* intermediate code ���ԃR�[�h */
	u8			user_255_code;		/* 'E'�Ƃ�'T'�Ƃ�	*/
	u8			user_item8; 		/* item */
	u8			user_select_gazou;	/* �A�j��/�摜��� */
	int 		user_hp;			/* hp */
	int 		user_score; 		/* score */
	int 		user_kougeki_type;	/* �G���U������ꍇ�̃X�y�J�̎�� */
//(20[Bytes] == (4x5) )
	char		user_string[MAX_PARA1_36/*(64)*/];/* strings */
//(64[Bytes] == (4x4)+(4)+(44) )
} GAME_COMMAND; 	/* .align 64 [bytes] */
//	u16 	dummy_scroll_speed256;	/* para3 scroll speed256 */


/* ���ԃR�[�h�`���̃R�}���h */
/* �Q�[�����́A���ԃR�[�h�`���̃R�}���h�������\����͂͂��Ȃ��B(�\����͂�load_stage.c�ōs��) */

enum /*_game_command_code_*/
{
/*���ԃR�[�h*/
/*0x00*/	GC_CODE_00_SJIS_TEXT = 0,		/* �����\��[�ԕ���؂�Ƃ��Ă��K�v] */
/*0x01*/	GC_CODE_01_ALL_CLEAR,			/* �Q�[�� �S�X�e�[�W �N���A�[ */
/*0x02*/	GC_CODE_02_BG_CONTROL,
/*0x03*/	GC_CODE_03_BOSS_COMMON, 		/* �G (�{�X / ��-�{�X)�p �X�N���v�g�N�� */
/*0x04*/	GC_CODE_04_CHUU_BOSS_COMMON,	/* �G �U�R��-�{�X(�X�N���v�g�N�����Ȃ�) */
/*0x05*/	GC_CODE_05_ZAKO,				/* �G �U�R */
			//
/*0x06*/	/* �g���\ */
/*...*/ 	/* ... */
/*0x2f*/	/* �g���\ */
/*0x30*/	GC_CODE_MAX,		/* 0x30 == '0' == �e���R�}���h (== ���x�R�}���h�A�p�~) */
};

#endif /* _LOAD_STAGE_FILE_H_ */
