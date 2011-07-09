
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
---------------------------------------------------------*/

#ifndef _BOSS_H_
#define _BOSS_H_

#include "game_main.h"
//#include "spell_card_value.h"

extern void common_boss_move(SPRITE *src);

/* �ڕW��ݒ肵�A�U���ړ� */
extern void boss_yuudou_idou_nomi(SPRITE *src);  		/* �U���ړ��̂� */
extern void boss_yuudou_hiritu_keisan(SPRITE *src); 	/* �U���v�Z�̂� */

/* ���Ԑ؂�̏ꍇ�̓����x�ݒ� */
extern void set_timeout_alpha(SPRITE *src);

#if 1
/* �{�X���ʋK�i */
	#define target_x256 			user_data00 	/* �ڕWx���W */
	#define target_y256 			user_data01 	/* �ڕWy���W */
	#define toutatu_wariai256		user_data02 	/* �ڕW���W�ւ̓��B���� */
	#define kougeki_anime_count 	user_data03 	/* �U���A�j���[�V�����p�J�E���^ / 0�ȉ��Ȃ�ړ��A�j���[�V���� */
	#define boss_time_out			user_data04 	/* �������� */
	#define boss_base_state777		user_data04 	/* ��������(boss_time_out�Ɠ���) */
//
	#define boss_spell_timer		user_data05 	/* �X�y������ */
#endif

extern void add_zako_alice_doll(SPRITE *src);/* �A���X�l�`�e�� */
extern void add_zako_sakuya_doll_02_tama_oki(SPRITE *src);/* ��u���e�� */


/*static*/extern void add_zako_sakuya_doll_01_laser(SPRITE *src);/* ���[�U�[ */

/*static*/extern void add_zako_sakuya_doll_03_star(SPRITE *src);/* �q�������w(���^��`��) */
/*static*/extern void add_zako_sakuya_doll_04_360knife(SPRITE *src);/* �q�������w(360�i�C�t������) */
#endif /* _BOSS_H_ */

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
---------------------------------------------------------*/

#ifndef _BOSS_OPTION_H_
#define _BOSS_OPTION_H_

//#include "game_main.h"
//#include "spell_card_value.h"

extern void lose_option_00(SPRITE *src);


#endif /* _BOSS_OPTION_H_ */
