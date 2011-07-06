#ifndef _BOSS_H_
#define _BOSS_H_

#include "game_main.h"
//#include "spell_card_value.h"

/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
---------------------------------------------------------*/
extern unsigned int alice_anime_count;

extern void move_alice(SPRITE *src);

/* �ڕW��ݒ肵�A�U���ړ� */
extern void alice_yuudou_move_only(SPRITE *src);	/* �ړ��̂� */
extern void alice_yuudou_calc(SPRITE *src); 		/* �U���v�Z�̂� */

#if 1
/* �{�X���ʋK�i */
	#define target_x256 		user_data00 	/* �ڕWx���W */
	#define target_y256 		user_data01 	/* �ڕWy���W */
	#define vvv256				user_data02 	/* �ڕW���W�ւ̓��B���� */
	#define boss_time_out		user_data03 	/* �������� */
	#define boss_base_state777	user_data03 	/* ��������(boss_time_out�Ɠ���) */
#endif


//extern void add_zako_alice_doll(SPRITE *src);/* �A���X�l�`�e�� */

#endif /* _BOSS_H_ */
