
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���̃t�@�C���̓V�i���I�Ɋ֌W������ꍇ�̂݁A�C���N���[�h���܂��B
	"game_main.h"���A��ŃC���N���[�h���܂��B
---------------------------------------------------------*/

#ifndef _SCENARIO_SCRIPT_SYSTEM_H_
#define _SCENARIO_SCRIPT_SYSTEM_H_

//static extern int thescript(void);
//extern int script_init(char *filename, char *bg_name,int width);
//static extern int script_start(char *filename);

/* SDL_FreeSurface(); �� psp�ł͑��������Ɠ����Ȃ��̂� ���̑΍� */
extern void story_script_system_init(void);/* �g�ݍ��� */
extern void script_system_exit(void);/* �O�� */

extern void script_system_set_re_draw(void);	/* �X�N���v�g�V�X�e����SDL�ĕ`��w���B */


#endif /* _SCENARIO_SCRIPT_SYSTEM_H_ */


///////////////////////////////////// �����\��

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���̃t�@�C���̓V�i���I�Ɋ֌W������ꍇ�̂݁A�C���N���[�h���܂��B
	"game_main.h"���A��ŃC���N���[�h���܂��B
---------------------------------------------------------*/

#ifndef _GAME_SCRIPT_SYSTEM_H_
#define _GAME_SCRIPT_SYSTEM_H_

extern void game_script_system_init(void);/* �g�ݍ��� */

#endif /* _GAME_SCRIPT_SYSTEM_H_ */


///////////////////////////////////// �����\��
