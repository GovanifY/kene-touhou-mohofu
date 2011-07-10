
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
---------------------------------------------------------*/

#ifndef _BOSS_H_
#define _BOSS_H_

#include "game_main.h"
//#include "card_address.h"

extern void common_boss_move(SPRITE *src);

/* �ڕW��ݒ肵�A�U���ړ� */
extern void boss_yuudou_idou_nomi(SPRITE *src); 		/* �U���ړ��̂� */
extern void boss_yuudou_hiritu_keisan(SPRITE *src); 	/* �U���v�Z�̂� */

/* ���Ԑ؂�̏ꍇ�̓����x�ݒ� */
extern void set_timeout_alpha(SPRITE *src);

#if 1
/* �{�X���ʋK�i */
	#define BOSS_DATA_00_target_x256					user_data00 	/* �ڕWx���W */
	#define BOSS_DATA_01_target_y256					user_data01 	/* �ڕWy���W */
//
// �o�O�΍�B user_data02 �ǂ����J���i�C���ǋ󂯂Ƃ��B()
//
	#define BOSS_DATA_03_kougeki_anime_count			user_data03 	/* �U���A�j���[�V�����p�J�E���^ / 0�ȉ��Ȃ�ړ��A�j���[�V���� */
	#define BOSS_DATA_04_toutatu_wariai256				user_data04 	/* �ڕW���W�ւ̓��B���� */
//
	#define BOSS_DATA_05_move_jyumyou					user_data05 	/* �������� */
	#define BOSS_DATA_05_boss_base_state777 			BOSS_DATA_05_move_jyumyou	/* ��������(boss_move_timer�Ɠ���) */
//

	/*(���)*/
	#define BOSS_DATA_06_sakuya_data_move_angle1024 	user_data06 	/* ���̊p�x */
	#define BOSS_DATA_07_sakuya_data_common_wait256 	user_data07 	/* ���� */
	/*(��)*/
	#define BOSS_DATA_06_aya_atooidan_type				user_data06 	/* ��ǂ��e�̎�� */


//�p�~	#define bo ss_spell_timer		user_data05 	/* �X�y���o�ߎ��� */
// REG_10_BOSS_SPELL_TIMER �ֈڍs�B
#endif

extern void add_zako_alice_doll(SPRITE *src);/* �A���X�l�`�J�[�h */
extern void add_zako_sakuya_doll_02_tama_oki(SPRITE *src);/* ��u���J�[�h */


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
//#include "card_address.h"

extern void lose_tukaima_00(SPRITE *src);
extern void move_tukaima00(SPRITE *src);

#endif /* _BOSS_OPTION_H_ */
