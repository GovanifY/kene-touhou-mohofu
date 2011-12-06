
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
---------------------------------------------------------*/

#ifndef _BOSS_H_
#define _BOSS_H_

#include "game_main.h"
#include "card_address.h"


/* �ڕW��ݒ肵�A�U���ړ� */
extern OBJ_CALL_FUNC(boss_set_new_position); 	/*(�U���ړ����W��ݒ�)*/

/* ���Ԑ؂�̏ꍇ�̓����x�ݒ� */
extern OBJ_CALL_FUNC(set_timeout_alpha);

#if 1
/* �{�X���ʋK�i */
	#define BOSS_DATA_00_target_x256					user_data00 	/* �ڕWx���W */
	#define BOSS_DATA_01_target_y256					user_data01 	/* �ڕWy���W */
//
// �o�O�΍�B user_data02�͎g�p�֎~�B �ǂ����J���i�C���ǋ󂯂Ƃ��B()
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


//�p�~	#define bo ss_spell_timer		user_data05 	/* �X�y���o�ߎ��� */
// REG_10_BOSS_SPELL_TIMER �ֈڍs�B
#endif


extern OBJ_CALL_FUNC(add_zako_sakuya_doll_02_tama_oki);/* ��u���J�[�h */


/*static*/extern OBJ_CALL_FUNC(add_zako_sakuya_doll_01_laser);/* ���[�U�[ */

/*static*/extern OBJ_CALL_FUNC(add_zako_sakuya_doll_03_star);/* �q�������w(���^��`��) */
/*static*/extern OBJ_CALL_FUNC(add_zako_sakuya_doll_04_360knife);/* �q�������w(360�i�C�t������) */
#endif /* _BOSS_H_ */

#ifndef _TUKAIMA_SYSTEM_H_
#define _TUKAIMA_SYSTEM_H_
extern OBJ_CALL_FUNC(tukaima_system_add_dolls);/*(�g�����V�X�e��)*/
//
extern OBJ_CALL_FUNC(init_set_dolls_kaguya_T01);
extern OBJ_CALL_FUNC(init_set_dolls_kaguya_T02);
extern OBJ_CALL_FUNC(init_set_dolls_kaguya_T03);
extern OBJ_CALL_FUNC(init_set_dolls_mima_T04);
extern OBJ_CALL_FUNC(init_set_dolls_sakuya_T05); 	/* ��� �΂�T��2�e �I�v�V���� */
extern OBJ_CALL_FUNC(init_set_dolls_sakuya_T06); 	/* ��� �΂�T��1�e �I�v�V���� */
extern OBJ_CALL_FUNC(init_set_dolls_alice_T07);  	/*(�ʏ�e)*/

enum
{
	TUKAIMA_00_OFF = 0,
	TUKAIMA_01_kaguya_T01,
	TUKAIMA_02_kaguya_T02,
	TUKAIMA_03_kaguya_T03,
	TUKAIMA_04_mima_T04,
	TUKAIMA_05_sakuya_T05,
	TUKAIMA_06_sakuya_T06,
	TUKAIMA_07_alice_T07,/*(�ʏ�e)*/
	//
	TUKAIMA_08_aya_T08,
	TUKAIMA_09_,/*(����)*/
	TUKAIMA_0a_,/*(����)*/
	TUKAIMA_0b_,/*(����)*/
	TUKAIMA_0c_,/*(����)*/
	TUKAIMA_0d_,/*(����)*/
	TUKAIMA_0e_,/*(����)*/
	TUKAIMA_0f_,/*(����)*/
	//
	TUKAIMA_08_MAX/*(�ő吔)*/
};

#endif /* _TUKAIMA_SYSTEM_H_ */
