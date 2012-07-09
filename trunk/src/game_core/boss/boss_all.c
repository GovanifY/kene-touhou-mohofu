
#include "boss.h"

extern OBJ_CALL_FUNC(boss_move_00_card_settei_seisei);
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	����
	Mitei Madakimattenai.
	-------------------------------------------------------
---------------------------------------------------------*/

/*---------------------------------------------------------
	�{�X�s���A�� 1�`��
---------------------------------------------------------*/

global OBJ_CALL_FUNC(boss_move_10_mima_keitai)
{
	static int mima_zako_tuika_timer = 0;	/* �G���A�ǉ��^�C�}�[�B�ǉ��Ԋu�����܂�Z�����Ȃ��B */
	if (0 < mima_zako_tuika_timer)
	{
		mima_zako_tuika_timer--;
	}
	//
	static int mima_jikan;	/* �J�[�h�ǉ��^�C�}�[�B�e�L�g�[�B */
	mima_jikan--;
	/* �J�[�h�������ĂȂ��ꍇ�ɒǉ� */
//	if (SPELL_00==card.spell_used_number)	/* �X�y�������I���Ȃ�J�[�h���� */
	if (0 > mima_jikan) 					/* �X�y�������I���Ȃ�J�[�h���� */
	{
		const unsigned char aa_ra_nd = ra_nd();
		if (0==(aa_ra_nd&0x03))
		{
			if (0==mima_zako_tuika_timer)
			{
			//	mima_zako_tuika_timer = (128);
				mima_zako_tuika_timer = (256);
				init_set_dolls_mima_T04(src);/*(�g�����V�X�e��)*/
			}
		}
		mima_jikan = /*(123)*/(64) + (aa_ra_nd&0x3f);
		//
		src->BOSS_DATA_05_boss_base_state777++;
		src->BOSS_DATA_05_boss_base_state777 &= (8-1);
	//	if (0!=(src->BOSS_DATA_05_boss_base_state777))/*(r32)*/
		if (0!=(aa_ra_nd & 0xc0))/*(r33)*/
		{
			static const POINT_u16 hosi_no_zahyou[(8)] = /* �ڕW���W */
			{	/* {(x),(y)}, */
				{(171),( 18)},	/* ��l�������� */
				{(202),( 92)},	/* �k�l����(5)�� */
				{(257),(107)},	/* �k�l����(6)�� */
				{(339),( 74)},	/* �k�l����(7)�� */
				{(	5),(116)},	/* �k�l����(1)�� */
				{(	6),( 46)},	/* �k�l����(2)�� */
				{(107),( 26)},	/* �k�l����(3)�� */
				{(133),( 78)},	/* �k�l����(4)�� */
			};
			/* �ړ����W�����߂� */
		//	src->BOSS_DATA_00_target_x256 = ((hosi_no_zahyou[src->BOSS_DATA_05_boss_base_state777].x)<<8);
		//	src->BOSS_DATA_01_target_y256 = ((hosi_no_zahyou[src->BOSS_DATA_05_boss_base_state777].y)<<8);
			REG_02_DEST_X	= ((hosi_no_zahyou[src->BOSS_DATA_05_boss_base_state777].x)<<8);/*(t256()�`��)*/
			REG_03_DEST_Y	= ((hosi_no_zahyou[src->BOSS_DATA_05_boss_base_state777].y)<<8);/*(t256()�`��)*/
			boss_set_new_position(src);/*(�U���ړ����W��ݒ�)*/
			/* �J�[�h�����߂� */
			card_maikai_init_and_get_spell_number(src); 	/* ���݂̌`�ԂŌ��Ă�J�[�h�ԍ����J�[�h�V�X�e���ɐݒ�B */
		}
		else
		{
			/* �ړ����W�����߂� */
			OBJ *zzz_player;
			zzz_player = &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER];
		//	src->BOSS_DATA_00_target_x256 = (zzz_player->center.x256);
		//	src->BOSS_DATA_01_target_y256 = (zzz_player->center.y256);
			REG_02_DEST_X	= (zzz_player->center.x256);/*(t256()�`��)*/
			REG_03_DEST_Y	= (zzz_player->center.y256);/*(t256()�`��)*/
			boss_set_new_position(src);/*(�U���ړ����W��ݒ�)*/
			#if 1
			/* �X�y�������߂� */
			card.spell_used_number	= SPELL_19_mima_sekkin; 	/* �J�[�h���Z�b�g */
			card_maikai_init(src);/*(r35-, �J�[�h�̏������B�J�[�h���ς��Ɩ���s���K�v������)*/
			#endif
		}
	//	src->BOSS_DATA_04_toutatu_wariai256 = t256(1.0);	/* �U���A�j���[�V���� */
	}
//r36���Ԃ�B	boss_re gacy_yuudou(src);
}


//#include "boss.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�H���R �P��
	Houraisan Kaguya.
	-------------------------------------------------------
---------------------------------------------------------*/

static OBJ_CALL_FUNC(kaguya_common_shot_check)
{
	/* �ړ����������߂� */
	src->BOSS_DATA_05_boss_base_state777++;
	src->BOSS_DATA_05_boss_base_state777 &= (4-1);
	#if (1)
	if ((/*(unsigned)*/REG_0f_GAME_DIFFICULTY) >= /*(unsigned)*/(src->BOSS_DATA_05_boss_base_state777) )/*(���O�̘_��AND�ŕK��unsigned���ۏ؂���邩��unsigned��r���g��)*/
	{
		card_maikai_init_and_get_spell_number(src);
	}
	#endif
}

/*---------------------------------------------------------
	�{�X�s���A�� 4�`��
	�΂߈ړ�
	-------------------------------------------------------
	�g�p���W�X�^
	REG_08_REG0 	�J�E���^�B()
	REG_0e_REG6 	�ڕW���W x256
---------------------------------------------------------*/

global OBJ_CALL_FUNC(boss_move_12_kaguya_funya_idou)
{
	/* �J�[�h�������ĂȂ��ꍇ�ɒǉ� */
	if (SPELL_00==card.spell_used_number)	/* �X�y�������I���Ȃ�J�[�h���� */
	{
		kaguya_common_shot_check(src);
		/* �C���ŖڕW�����߂�B */
		REG_0e_REG6 = ((ra_nd()&0xffff))+(t256(48));
	}
	REG_08_REG0++;
	if (8 < REG_08_REG0)
	{
		REG_08_REG0 = 0;
		/* �ڕW�ɂ����Â����Ɠw�́B */
		src->BOSS_DATA_00_target_x256 += ((src->BOSS_DATA_00_target_x256 < REG_0e_REG6) ? t256(8) : -t256(8) );
		src->BOSS_DATA_00_target_x256 &= 0xffff;
	{
		int sin_value_t256; 		//	sin_value_t256 = 0;
		int cos_value_t256; 		//	cos_value_t256 = 0;
		int256_sincos1024( (((src->BOSS_DATA_00_target_x256>>4)/*&(1024-1)*/)), &sin_value_t256, &cos_value_t256);
		src->BOSS_DATA_01_target_y256 = (t256(25)+( (sin_value_t256)<<6));
	}
		REG_02_DEST_X	= (src->BOSS_DATA_00_target_x256);
		REG_03_DEST_Y	= (src->BOSS_DATA_01_target_y256);
		boss_set_new_position(src);/*(�U���ړ����W��ݒ�)*/
	//	src->BOSS_DATA_04_toutatu_wariai256 = t256(1.0);
	}
//r36���Ԃ�B	boss_re gacy_yuudou(src);
}


/*---------------------------------------------------------
	�{�X�s���A�� 1�`��
	���㉺�ړ�
	-------------------------------------------------------
	�g�p���W�X�^
	REG_08_REG0 	�J�E���^�B()
---------------------------------------------------------*/

global OBJ_CALL_FUNC(boss_move_11_kaguya_yureru)
{
	/* �J�[�h�������ĂȂ��ꍇ�ɒǉ� */
	if (SPELL_00==card.spell_used_number)	/* �X�y�������I���Ȃ�J�[�h���� */
	{
		kaguya_common_shot_check(src);
	}
	REG_08_REG0 += (10);
//	mask1024(REG_08_REG0);
	{
		int sin_value_t256; 		//	sin_value_t256 = 0;
		int cos_value_t256; 		//	cos_value_t256 = 0;
		int256_sincos1024( ((((REG_08_REG0))/*&(1024-1)*/)), &sin_value_t256, &cos_value_t256);
		src->center.y256 = (t256(25)+( ((sin_value_t256))<<2));
	}
//
}

//#include "boss.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�˖��� ��
	Syameimaru Aya.
	-------------------------------------------------------
	�������̍�����U�����X�y�J�ɂ���B
	-------------------------------------------------------
	�������F�����Ŏv����������2�`�Ԃ̍U���p�^�[�����J�I�X�ɂȂ��Ă��܂����B
	�������F�����ƒ��J�ɍ�蒼�����������������B
	�������F�V��̑��x���Փx���Ƃɕς��Ă݂��B
	-------------------------------------------------------
	�g�p���W�X�^
	REG_08_REG0 	�J�E���^�B()
	REG_09_REG1 	�U���^�C�v(���e���֎󂯓n���p)
	REG_0a_REG2 	��ǂ��e�̎��	aya_atooidan_type
---------------------------------------------------------*/

/* 1�Ȃ̂͐����Â��d�l�B���݂̓V�X�e���� ketm ������ CONTROLLER �͖����B
������ɑ��铌���� ������ CONTROLLER �͍�邩������Ȃ��B
( CONTROLLER ���Ă̂́A�{�X�̎���ӂ�ӂ킵�Ēe�o���A���B) */
#define USE_CONTROLLER (0)


/*---------------------------------------------------------
	�V��p�̑�ʒe(��)�B�����ŉ������ȓ���������B
	-------------------------------------------------------
	�����ځ��������蔻��ł��B
--------------------------------------------------------*/
enum
{
	AYA_KOUGEKI_TYPE_00_NONE = (0),
	AYA_KOUGEKI_TYPE_01_RENDAN, 	// (1)
	AYA_KOUGEKI_TYPE_02_FUDADAN,	// (2)
	AYA_KOUGEKI_TYPE_03_OODAMA, 	// (3)
};
#define AYA_KOUGEKI_TYPE_03_OODAMA_23	AYA_KOUGEKI_TYPE_03_OODAMA
#define AYA_KOUGEKI_TYPE_03_OODAMA_22	AYA_KOUGEKI_TYPE_03_OODAMA
#define AYA_KOUGEKI_TYPE_03_OODAMA_21	AYA_KOUGEKI_TYPE_03_OODAMA

/*---------------------------------------------------------
	�U���^�C�v
---------------------------------------------------------*/

static OBJ_CALL_FUNC(aya_kougeki_all)
{
	//(r35�ŃJ�[�h�̏ꍇ�̂ݏȗ��\)	set_REG_DEST_XY(src);	/* �e��x256 y256 ���S���甭�e�B */
	set_REG_DEST_XY(src);	/* �e��x256 y256 ���S���甭�e�B */
	calculate_jikinerai();
	#if 0/*(r35�̃V�X�e���ł́A�ȗ��\)*/
	HATSUDAN_03_angle65536				= (HATSUDAN_03_angle65536); 	/* [�j��] */
	#endif
	//
//	if (AYA_KOUGEKI_TYPE_00_NONE==REG_09_REG1)
//	{
//	}
//	else
	if (AYA_KOUGEKI_TYPE_01_RENDAN==REG_09_REG1)	/* �U���^�C�v00:	�ĘA�e */
	{
		AUDIO_18_voice_number	= VOICE11_BOSS_KIRARIN;
		AUDIO_19_voice_truck	= TRACK04_TEKIDAN;/*�e�L�g�[*/
		cpu_voice_play();
		HATSUDAN_02_speed_offset			= t256(1);/* �H */
		HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_3000_EFFECT_NONE)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
		{	/* ���� */
			s32 aaa;
			aaa = ((REG_0f_GAME_DIFFICULTY)<<6);/*src->aya_speed*/
			aaa += (t256(4.5)); 		/* 3.5����easy���������B t256(3.5) */
			const s32 ggg_angle 				= (HATSUDAN_03_angle65536); 	/* [�j��] */
			unsigned int jj;
			for (jj=(0); jj<(8+((unsigned)REG_0f_GAME_DIFFICULTY)); jj++)
			{
				aaa -= (t256(0.125));
				/* �e�ɓœh���Ă���ݒ� */
				HATSUDAN_01_speed256			= (aaa);									/* �e�� */
				HATSUDAN_05_bullet_obj_type 	= (BULLET_KOME_BASE) + (ra_nd()&0x0f);		/* �e�O�� */
				HATSUDAN_03_angle65536			= (ggg_angle);		/* [�j��] */
				HATSUDAN_06_n_way				= (11-jj);			/* [�j��] */			/* [10way] */
				HATSUDAN_07_div_angle65536		= (int)(65536/24);	/* [�j��] */
				hatudan_system_regist_n_way();/* (r33-) */
			}
		}
	}
	else
	if (AYA_KOUGEKI_TYPE_02_FUDADAN==REG_09_REG1)	/* �U���^�C�v01:	��ǂ��D�e */
	{	/*(�D�e�͈Ӑ}�I�ɒx������)*/
	//	HATSUDAN_01_speed256			= (t256(2.0)+((REG_0f_GAME_DIFFICULTY)<<8));	/* �e�� */	/*t256(5.0)*/
		HATSUDAN_01_speed256			= (t256(1.5)+((REG_0f_GAME_DIFFICULTY)<<4));	/* �e�� */	/*t256(5.0)*/
		HATSUDAN_02_speed_offset		= t256(1);/* �H */
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_3000_EFFECT_NONE)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
	//	HATSUDAN_05_bullet_obj_type 	= BULLET_KOME_BASE + (ra_nd()&0x0f);		/* �e�O�� */	/* �e�ɓœh���Ă���ݒ� */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_OFUDA12_00_AOI) + (REG_0a_REG2);	/* �e�O�� */	/* �e�ɓœh���Ă���ݒ� */
		HATSUDAN_06_n_way				= (5);			/* [�j��] */	/* [5way] */
		HATSUDAN_07_div_angle65536		= (65536/12);	/* [�j��] */	/* 12���� == (30/360)[�x] */
		hatudan_system_regist_n_way();/* (r33-) */
	}
	else
//	if (AYA_KOUGEKI_TYPE_03_OODAMA==REG_09_REG1)	/* �U���^�C�v02:	��ʒe */
	{	/*(�A�e���኱�x������)*/
	//	HATSUDAN_01_speed256			= speed256; 			/* �e�� */
	//	HATSUDAN_01_speed256			= (t256(4.0)-((REG_0f_GAME_DIFFICULTY)<<7));			/* �e�� */
		HATSUDAN_01_speed256			= (t256(3.5)-((REG_0f_GAME_DIFFICULTY)<<6));			/* �e�� */
		HATSUDAN_02_speed_offset		= t256(-2);/* �����e�H */
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(0)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
		const s32 aaa					= ((u32)((((HATSUDAN_03_angle65536)>>8)&(0x03))));
		HATSUDAN_05_bullet_obj_type 	= (BULLET_OODAMA32_00_AOI+(aaa));	/* �e�O�� */
		HATSUDAN_06_n_way				= (12); 						/* [12way] */
		HATSUDAN_07_div_angle65536		= (65536/12);					/* 12���� == (30/360)[�x] */
		hatudan_system_regist_katayori_n_way();/* (r33-) */
	}
}

/*---------------------------------------------------------
	�{�X�s���A�� 5�`��
	�ړ��U���p�^�[��2(�����ړ�)
---------------------------------------------------------*/

/*(SS_TAN_I_JIKAN: need 2^n, 2��n��Ŋ���؂��K�v����)*/
#define SS_TAN_I_JIKAN (4096)
/*
---0
[A]�ޔ����W�ݒ�
---1
[B]�ޔ��s��
---2
[C]�U���ړ�
---3
[D]�U��
---3+220 -> 0 �ցB

*/
/*(���̕�50[pixel])*/
//#define AYA_OBJ_WIDTH50 (50)

/* �o����x���W */
//#define BOSS_XP256		(t256(GAME_WIDTH/2))	/* ���S���W�Ȃ̂� */
#define BOSS_XP256			(t256(GAME_X_OFFSET)+t256(GAME_320_WIDTH/2))	/* ���S���W�Ȃ̂� */

global OBJ_CALL_FUNC(boss_move_13_aya_taifu)
{
	const u8 sst[8] = { 0, 0, 0, 1, 1, 1, 2, 2 }; /* (1/3)�̊m���ŕ��򂷂�B */
	REG_08_REG0++;
	if ((SS_TAN_I_JIKAN*1) > REG_08_REG0)	/* [A]�ޔ����W�ݒ� */
	{
		REG_08_REG0 = (SS_TAN_I_JIKAN*1);
	//	src->BOSS_DATA_00_target_x256 = t256(GAME_X_OFFSET)+t256((GAME_320_WIDTH-AYA_OBJ_WIDTH50)/2);
	//	src->BOSS_DATA_00_target_x256 = BOSS_XP256;
		src->BOSS_DATA_01_target_y256 = t256(30.0); /* t256(20.0) */
		/* �ړ����W�����߂� */
		REG_02_DEST_X	= (src->BOSS_DATA_00_target_x256);/*(t256()�`��)*/
		REG_03_DEST_Y	= (src->BOSS_DATA_01_target_y256);/*(t256()�`��)*/
		boss_set_new_position(src);/*(�U���ړ����W��ݒ�)*/

	}
	if ((SS_TAN_I_JIKAN*2) > REG_08_REG0)	/* [B]�ޔ��s�� */
	{
	//	ca se SS00: /* �s��:�����ʒu���̎擾->SS02�� */
	//	ca se SS02: /* �s��:�����ʒu�֖߂�->SS03�� */
	//	if (/*150*/200 > src->BOSS_DATA_04_toutatu_wariai256 )	/* ���Ԃ� */
		/*(�U������[A])*/
		if (t256(31.0) > src->center.y256)
		{
//m 		src->BOSS_DATA_04_toutatu_wariai256 = t256(1.00);
			REG_08_REG0 = (SS_TAN_I_JIKAN*2);
			REG_09_REG1 = AYA_KOUGEKI_TYPE_01_RENDAN;	/* �U�� */
		}
	}
	else
	if ((SS_TAN_I_JIKAN*3) > REG_08_REG0) /* [C]�U���ړ� */
	{
	//	ca se SS03: /* �����ʒu:��e3��->SS02, SS02, SS04 */
	//	if (/*150*/240 > src->BOSS_DATA_04_toutatu_wariai256 )	/* ���Ԃ� */
		/*(�U������[B])*/
	//	if (/*150*/240+(SS_TAN_I_JIKAN*2) < REG_08_REG0 )	/* ���Ԃ� */
		if (/*150*/24-((REG_0f_GAME_DIFFICULTY)<<2)+(SS_TAN_I_JIKAN*2) < REG_08_REG0 )	/* ���Ԃ� */
		{
//m 		src->BOSS_DATA_04_toutatu_wariai256 = t256(1.00);
			REG_08_REG0 = (SS_TAN_I_JIKAN*3);
			REG_09_REG1 = AYA_KOUGEKI_TYPE_03_OODAMA_23;	/* �U�� */
					/*t256(5.0)*/ /* �e�� */		/*�d�l�ύX*/	// t256(/*1.5*/5.0/*5.0*/)
			{
				REG_0a_REG2 = sst[((ra_nd()&(8-1)))];	/* SS02, SS02, SS04 */
			}
			if (0 == REG_0a_REG2)
			{
				OBJ *zzz_player;
				zzz_player = &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER];
			//	src->BOSS_DATA_00_target_x256 = BOSS_XP256; /* �^���փ��[�v */
				src->BOSS_DATA_00_target_x256 = zzz_player->center.x256;
				src->BOSS_DATA_01_target_y256 = zzz_player->center.y256;
			//	#define KYORI_AAA (t256(128.0)+((cg_game_di fficulty)<<(8+4/*5*/)))
				#define KYORI_AAA (t256(128.0)+((3)<<(8+4/*5*/)))
				if (src->BOSS_DATA_01_target_y256 > (signed)KYORI_AAA)
				{
					src->BOSS_DATA_01_target_y256 = KYORI_AAA;
				}
				#undef KYORI_AAA
			}
			else
			if (1 == REG_0a_REG2)
			{
				src->BOSS_DATA_00_target_x256 = t256(GAME_X_OFFSET); /* */
			//	src->center.y256 += t256(6.0);/*fps_factor*/	/* �悭�킩��Ȃ�(???) */
				src->BOSS_DATA_01_target_y256 += t256(6.0);/*fps_factor*/	/* �悭�킩��Ȃ�(???) */
			}
			else
		//	if (2 == REG_0a_REG2)
			{
			//	src->BOSS_DATA_00_target_x256 = t256((GAME_X_OFFSET+GAME_320_WIDTH-AYA_OBJ_WIDTH50)); /* */
				src->BOSS_DATA_00_target_x256 = t256((GAME_X_OFFSET+GAME_320_WIDTH)); /* */
			//	src->center.y256 += t256(6.0);/*fps_factor*/	/* �悭�킩��Ȃ�(???) */
				src->BOSS_DATA_01_target_y256 += t256(6.0);/*fps_factor*/	/* �悭�킩��Ȃ�(???) */
			}
		}
	}
	else
//	if (SS02 ==REG_08_REG0) 	/* [D]�U�� */
	{
//-------- ���� [SS02]
//-------- ���� [SS02]
//-------- [�U��A]
//-------- ���� [SS04]
//		//	/* (1/2)�̊m����[�U��A]�֕��򂷂�B */
//		//	if (0==(ra_nd()&(2-1)))
//			/* (1/4)�̊m����[�U��A]�֕��򂷂�B */
//			{
//				/* (1/2)�̊m����[�U��A]�֕��򂵂Ȃ��B */
//			}
//	ca se SS02: /* ���ړ���->SS05�� */
//	ca se SS02: /* �E�ړ���->SS04�� */
//	ca se SS05: /* ��:�ҋ@->SS06�� */
//	ca se SS03: /* ��:�O�� -> SS11 */
//	ca se SS04: /* �v���C���[�ʒu�t�߈ړ��� -> �v���C���[�ʒu�t��:��e3�� -> SS00�� */
//		sakuya_anime00(src);//		sakuya_anime08(src);
		/* aya�̉ғ��͈� */
	//	if (/*150*/220 > src->BOSS_DATA_04_toutatu_wariai256 )	/* ���Ԃ� */
		/*(�U������[C])*/
	//	if (/*150*/220+(SS_TAN_I_JIKAN*3) < REG_08_REG0 )	/* ���Ԃ� */
		if (/*150*/22-((REG_0f_GAME_DIFFICULTY)<<2)+(SS_TAN_I_JIKAN*3) < REG_08_REG0 )	/* ���Ԃ� */
		{
//m 		src->BOSS_DATA_04_toutatu_wariai256 = t256(1.00);
		//	REG_08_REG0++; //	sakuya_wait_state_bbb(src/*,SS06*/);
			REG_08_REG0 = (SS_TAN_I_JIKAN*0); /* SS00�� */
			//ca se SS06: /* ��:�E��]�U��->SS00, SS03 */
				u8 zzz_aaa;
			{
				zzz_aaa = sst[((ra_nd()&(8-1)))];	/* SS02, SS02, SS04 */
			}
			if (zzz_aaa == 0)
			{
				REG_09_REG1 = AYA_KOUGEKI_TYPE_02_FUDADAN;	/* �U�� */
				src->BOSS_DATA_01_target_y256 = t256(GAME_HEIGHT/3); /* */
			}
			else
			#if (1==USE_CONTROLLER)
			if (zzz_aaa == 1)
			{
				REG_09_REG1 = AYA_KOUGEKI_TYPE_03_OODAMA_21;	/* �U�� */
				#if (1==USE_CONTROLLER)
				eee_boss02_sr_add1(src);/* ��:�E��]�U�� */
				eee_boss02_sr_add2(src);/* �E:����]�U�� */
				#endif //(1==USE_CONTROLLER)
			}
			else
			#endif //(1==USE_CONTROLLER)
		//	if (zzz_aaa == 2)
			{
				REG_09_REG1 = AYA_KOUGEKI_TYPE_03_OODAMA_22;	/* �U�� */
			}
		}
	}
	if (AYA_KOUGEKI_TYPE_00_NONE!=REG_09_REG1)
	{
		aya_kougeki_all(src);
		REG_09_REG1 = AYA_KOUGEKI_TYPE_00_NONE;
		/* �ړ����W�����߂� */
		REG_02_DEST_X	= (src->BOSS_DATA_00_target_x256);/*(t256()�`��)*/
		REG_03_DEST_Y	= (src->BOSS_DATA_01_target_y256);/*(t256()�`��)*/
		boss_set_new_position(src);/*(�U���ړ����W��ݒ�)*/
	}
//r36���Ԃ�B	boss_re gacy_yuudou(src);
}
	/*(��؂莞�Ԃ̏ꍇ�A���W�ݒ�)*/
//	if (0==(REG_08_REG0&(SS_TAN_I_JIKAN-1)))
//	{
//	}


//#include "boss.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�\�Z�� ���
	Izayoi Sakuya.
	-------------------------------------------------------
	ToDo:
	�ړ��ƒe�̏����𕪗����悤�B�łȂ��Ɩ�킩���B
	�{�X�^�C�}�[�Ή���
	-------------------------------------------------------
	���{�X�^�C�}�[�̓V�X�e���ŃT�|�[�g�\��ł��B(��ƒ�)
---------------------------------------------------------*/


/*---------------------------------------------------------
	���w��̖����w�ɂ��ă���
	-------------------------------------------------------
	5�ʒ�-�{�X�ŏo�Ă��鎞�́A�傫��60x60(���Ԃ�64x64obj)���炢�B
	5�ʃ{�X�ŏo�Ă��鎞�́A�傫��125x125(���Ԃ�128x128obj)���炢�B
	�͕핗�͊�{�I�ɏk����75%�Ȃ̂ŁA���w��̖����w��
	48x48[pixel](2�{�g�厞��96x96[pixel])���炢���Ó����ȁH
---------------------------------------------------------*/



/*---------------------------------------------------------
	��10�`��: �ŏI�`��(����2)
---------------------------------------------------------*/

global OBJ_CALL_FUNC(boss_move_16_sakuya_nazo_keitai)
{
	{
		if (src->color32 > 0xf9000000)	/*	if (src->alpha > 0xf9) */	/* (src->alpha>0xff) */
		{
			src->color32 = 0xffffffff;	/*	src->alpha = 0xff;*/
		}
		else /*if (src->alpha<0xff)*/
		{
			src->color32 += 0x05000000; /*	src->alpha += 0x05;*/
		}
		#if (0)//
		src->center.x256 -= ((si n1024((src->tmp_angleCCW1024))*(t256(0.03)))>>8);/*fps_factor*/		/* CCW�̏ꍇ */
		src->center.y256 -= ((co s1024((src->tmp_angleCCW1024))*(t256(0.03)))>>8);/*fps_factor*/
		#else
		{
			int sin_value_t256; 	//	sin_value_t256 = 0;
			int cos_value_t256; 	//	cos_value_t256 = 0;
			int256_sincos1024( (src->tmp_angleCCW1024), &sin_value_t256, &cos_value_t256);
			src->center.x256 -= ((sin_value_t256*(t256(0.03)))>>8);/*fps_factor*/
			src->center.y256 -= ((cos_value_t256*(t256(0.03)))>>8);/*fps_factor*/
		}
		#endif
	}
	//	[SAKUYA_ANIME_12_HATUDAN_START)
	/*(�ȉ��͏����I�ɋ��ʉ������\��)*/
	boss_move_00_card_settei_seisei(src);/*(�J�[�h�����I���Ȃ�J�[�h�ݒ肵�A�����B)*/
}


/*---------------------------------------------------------
	���`��: �����u�t�F�X�e�B�o���i�C�t�v �ŏI�`��(����1)
	-------------------------------------------------------
	�g�p���W�X�^
	REG_08_REG0 	�J�E���^�B()
	REG_0c_REG4 	BO SS_DATA_07_sakuya_data_common_wait256
	-------------------------------------------------------
	REG_0e_REG6 	[���e�A�����W�X�^]
		0:	�Ȃ��B
		1:	extern OBJ_CALL_FUNC(bullet_create_sakuya_ ryoute_knife); ��� ����i�C�t�J�[�h
		2:	extern OBJ_CALL_FUNC(bullet_create_sakuya_ kurukuru);     ��� ���邭��i�C�t�J�[�h
	-------------------------------------------------------
---------------------------------------------------------*/
/*
0
	[A] �������Ȃ�(�ҋ@����)
80
	[B] �ҋ@���Ԃ��I�������ړ�����
81
	[C] �^�񒆂ɗ���܂ňړ� //case SG01:
1024
	[D] �������Ȃ�(�ҋ@����)
2048 ==(1024+1024 )
	[E] ����i�C�t�ōU��
2049 ==(1024+1024+1)
	[F]�ړ�
2089 ==(1024+10024+1+40)
	[G] �ҋ@���ԍ쐬���A�߂�B
*/

global OBJ_CALL_FUNC(boss_move_15_sakuya_festival_knife)
{
	REG_08_REG0++;
	if (80 > REG_08_REG0) /* [A] �������Ȃ�(�ҋ@����) */
	{
		;/* �������Ȃ�(�ҋ@����) */
	}
	else
	if (81 > REG_08_REG0) /* [B] �ҋ@���Ԃ��I�������ړ����� */
	{
//++		pd->bo ssmode = B00_NONE/*B01_BA TTLE*/;
		//(r35�ŃJ�[�h�̏ꍇ�̂ݏȗ��\)	REG_02_DEST_X	= ((src->center.x256));
		//(r35�ŃJ�[�h�̏ꍇ�̂ݏȗ��\)	REG_03_DEST_Y	= ((src->center.y256));
		//(r35�ŃJ�[�h�̏ꍇ�̂ݏȗ��\)	set_REG_DEST_XY(src);	/* �e��x256 y256 ���S���甭�e�B */
			REG_00_SRC_X	= (t256(GAME_X_OFFSET+(GAME_320_WIDTH/2))); 		/* �e��x256 */
			REG_01_SRC_Y	= (t256(30));					/* �e��y256 */
			tmp_angleCCW65536_src_nerai();
			src->BOSS_DATA_06_sakuya_data_move_angle1024 = (deg65536to1024(HATSUDAN_03_angle65536));	/* �u1����65536�����v����u1����1024�����v�֕ϊ�����B */
	}
	else
	if (1024 > REG_08_REG0) /* [C] �^�񒆂ɗ���܂ňړ� */
	{
		if ( (src->center.x256 < t256(GAME_X_OFFSET+(GAME_320_WIDTH/2)+20) ) &&
			 (src->center.x256 > t256(GAME_X_OFFSET+(GAME_320_WIDTH/2)-20) ) &&
			 (src->center.y256 < t256(50.0)) ) /* �^�񒆂ɗ����� */
		{
			REG_08_REG0 = (1024+1024-1)-(20);// REG_08_REG0++;/* = SG02*/
			REG_0c_REG4 = 0;
			//	[SAKUYA_ANIME_03_CENTER_A;
		}
		else
		{
			#if (0)//
			src->center.x256 += ((si n1024((src->BOSS_DATA_06_sakuya_data_move_angle1024))*(t256(3.0)))>>8);/*fps_factor*/	/* CCW�̏ꍇ */
			src->center.x256 += ((si n1024((src->BOSS_DATA_06_sakuya_data_move_angle1024))*(t256(3.0)))>>8);/*fps_factor*/	/* CCW�̏ꍇ */
			src->center.y256 += ((co s1024((src->BOSS_DATA_06_sakuya_data_move_angle1024))*(t256(3.0)))>>8);/*fps_factor*/
			src->center.y256 += ((co s1024((src->BOSS_DATA_06_sakuya_data_move_angle1024))*(t256(3.0)))>>8);/*fps_factor*/
			#else
			{
				int sin_value_t256; 		//	sin_value_t256 = 0;
				int cos_value_t256; 		//	cos_value_t256 = 0;
				int256_sincos1024( (src->BOSS_DATA_06_sakuya_data_move_angle1024), &sin_value_t256, &cos_value_t256);
				src->center.x256 += ((sin_value_t256*(t256(3.0)))>>8);/*fps_factor*/
				src->center.y256 += ((cos_value_t256*(t256(3.0)))>>8);/*fps_factor*/
			}
			#endif
		}
	}
	else
	if ((1024+1024 ) > REG_08_REG0) /* [D] �������Ȃ�(�ҋ@����) */
	{
		;/* �������Ȃ�(�ҋ@����) */
	}
	else
	if ((1024+1024 +1) > REG_08_REG0) /* [E] ����i�C�t�ōU�� */
	{
		{
			//	[SAKUYA_ANIME_12_HATUDAN_START]
			//	[SAKUYA_ANIME_15_HATUDAN_END]
			REG_0e_REG6=(1);/*[��� ����i�C�t�J�[�h]*/
			if (src->center.y256 > t256(150))
		//	{	src->BOSS_DATA_06_sakuya_data_move_angle1024 = cv1024r(   210) + (((ra_nd()&(4096-1))*21)>>8) /*(ra_nd()%(deg_360_to_512(120)))*/;} 	/* CW�̏ꍇ */
		//	{	src->BOSS_DATA_06_sakuya_data_move_angle1024 = cv1024r(360-210) + (((ra_nd()&(4096-1))*21)>>8) /*(ra_nd()%(deg_360_to_512(120)))*/;}	/* CCW�̏ꍇ */
			{	src->BOSS_DATA_06_sakuya_data_move_angle1024 = cv1024r(360-210) + (((ra_nd()&(4096-1))*21)>>8) /*(ra_nd()%(deg_360_to_512(120)))*/;}
			else
			{	src->BOSS_DATA_06_sakuya_data_move_angle1024 = ((ra_nd()&(1024-1)));}
			#if 1
			/*�O�̈�*/
			mask1024(src->BOSS_DATA_06_sakuya_data_move_angle1024);
			#endif
		}
	}
	else
	if ((1024+1024 +1+40) > REG_08_REG0) /* [F]�ړ� */
	{
		#if (0)//
		src->center.x256 += ((si n1024((src->BOSS_DATA_06_sakuya_data_move_angle1024))*(t256(3.5)))>>8);/*fps_factor*/	/* CCW�̏ꍇ */
		src->center.y256 += ((co s1024((src->BOSS_DATA_06_sakuya_data_move_angle1024))*(t256(3.5)))>>8);/*fps_factor*/
		#else
		{
			int sin_value_t256; 	//	sin_value_t256 = 0;
			int cos_value_t256; 	//	cos_value_t256 = 0;
			int256_sincos1024( (src->BOSS_DATA_06_sakuya_data_move_angle1024), &sin_value_t256, &cos_value_t256);
			src->center.x256 += ((sin_value_t256*(t256(3.5)))>>8);/*fps_factor*/
			src->center.y256 += ((cos_value_t256*(t256(3.5)))>>8);/*fps_factor*/
		}
		#endif
	}
	else /* [G] �ҋ@���ԍ쐬���A[D]�ɖ߂�B */
	{
			REG_08_REG0 = (1024+1024-1)-(120)-(3*64)+(3<<6);
	}
	//
	OBJ *zzz_player;
	zzz_player = &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER];
	#if 1/*Gu(���S���W)*/
	if ((zzz_player->center.x256 < src->center.x256 + t256(25)) &&	/* ???????????? */
		(zzz_player->center.x256 > src->center.x256 - t256(25)))
	#endif
	{
		REG_0c_REG4++;/*fps_factor*/
	}
	if (REG_0c_REG4 > 24)/*(21 r35u1)*/
	{
		REG_0c_REG4 	= 0;
		{
	//		src->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x00);			/*	src->alpha			= 0x00;*/
			src->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x80);			/*	src->alpha			= 0x00;*/	/* ������ */
			src->tmp_angleCCW1024	= (0);
		}
		//	[SAKUYA_ANIME_12_HATUDAN_START]
		//	[SAKUYA_ANIME_15_HATUDAN_END]
		REG_0e_REG6=(2);/*[��� ���邭��i�C�t�J�[�h]*/
	}
	/*(�ȉ��͏����I�ɋ��ʉ������\��)*/
	boss_move_00_card_settei_seisei(src);/*(�J�[�h�����I���Ȃ�J�[�h�ݒ肵�A�����B)*/
//r36���Ԃ�B	boss_re gacy_yuudou(src);
}


/*---------------------------------------------------------
	��02�`��: ��p�u�~�X�f�B���N�V�����v
	-------------------------------------------------------
	�g�p���W�X�^
//	REG_08_REG0 	�J�E���^�B()
//	REG_09_REG1 	BO SS_DATA_07_sakuya_data_common_wait256
---------------------------------------------------------*/

/*
		64		256-64		256-0
	//	�^���փ��[�v
		128 				256-64
	// �Ȃɂ����Ȃ�
		192 				256-128
	// ���]�ړ������Z�b�g
		256 				256-192
	// �Ȃɂ����Ȃ�
		0		256 		256-256
	//	���Α��փ��[�v
-------------------------

256 	�^���փ��[�v

250 	���Α��փ��[�v
*/
/*(�Q�[����ʂ�x���W���S�ʒu)==(���[�̃p�l������)+(�Q�[���̈��ʂ̔���)*/
#define SAKUYA_POINT_X_MID			(t256(GAME_X_OFFSET+(GAME_320_WIDTH/2)))

global OBJ_CALL_FUNC(boss_move_14_sakuya_miss_direction)
{
	if (/*(256-64)*/(255/*192+64*/) == REG_10_BOSS_SPELL_TIMER) /* ���]�ړ������Z�b�g */
	{
		/* ���`�Ԃł͎g���Ė������痘�p(���p)���� */
		#define sakuya_data_sayuu_mode BOSS_DATA_07_sakuya_data_common_wait256
		{	/* ��邳��̗U���|�C���g���W */
			src->sakuya_data_sayuu_mode ^= (2-1);/* ���] */
			if (/*0==*/src->sakuya_data_sayuu_mode & (2-1))
					{	src->BOSS_DATA_00_target_x256 = SAKUYA_POINT_X_MID-(t256(128)); }//96==128*0.75
			else	{	src->BOSS_DATA_00_target_x256 = SAKUYA_POINT_X_MID+(t256(128)); }//
		}
		#if (1)
		/* �ړ����W�����߂� */
	//	src->BOSS_DATA_04_toutatu_wariai256 = t256(1.0);	/* �U���A�j���[�V���� */
		REG_02_DEST_X	= (src->BOSS_DATA_00_target_x256);/*(t256()�`��)*/
		REG_03_DEST_Y	= (src->BOSS_DATA_01_target_y256);/*(t256()�`��)*/
		boss_set_new_position(src);/*(�U���ړ����W��ݒ�)*/
		#endif
	}
	//
	else
//	if (/*(256-256)*/(59)/*((251+64)-256)*/ == REG_10_BOSS_SPELL_TIMER) /* �^���փ��[�v */
	if (/*(256-256)*/(59)/*((251+64)-256)*/ == REG_10_BOSS_SPELL_TIMER) /* �^���փ��[�v */
	{
		src->BOSS_DATA_00_target_x256 = src->center.x256 = SAKUYA_POINT_X_MID; /* �^���փ��[�v */
		#if (1)
		/* �ړ����W�����߂� */
	//	src->BOSS_DATA_04_toutatu_wariai256 = t256(1.0);	/* �U���A�j���[�V���� */
		REG_02_DEST_X	= (src->BOSS_DATA_00_target_x256);/*(t256()�`��)*/
		REG_03_DEST_Y	= (src->BOSS_DATA_01_target_y256);/*(t256()�`��)*/
		boss_set_new_position(src);/*(�U���ړ����W��ݒ�)*/
		#endif
	}
	else
	if (/*(256-256)*/(58)/*((250+64)-256)*/ == REG_10_BOSS_SPELL_TIMER) /* ���Α��փ��[�v */
//	if (/*(256-256)*/(0) == REG_10_BOSS_SPELL_TIMER) /* �^���փ��[�v */
//	if (/*(256-192)*/(64) == REG_10_BOSS_SPELL_TIMER) /* �^���փ��[�v */
//	if (/*(256-180)*/(76) == REG_10_BOSS_SPELL_TIMER) /* �^���փ��[�v */
	{
	/*
		 ���Α��̍��W==(���[�̃p�l������)+(�Q�[���̈���)-(���݂̍��W-(���[�̃p�l������));
		 ���Α��̍��W==(���[�̃p�l������)+(���[�̃p�l������)+(�Q�[���̈���)-(���݂̍��W));
	*/
	//	src->BOSS_DATA_00_target_x256 = src->center.x256 = t256(GAME_X_OFFSET) + ( t256(GAME_320_WIDTH) - ( (src->center.x256) - t256(GAME_X_OFFSET) ) );	/* ���Α��փ��[�v */
		src->BOSS_DATA_00_target_x256 = src->center.x256 = t256((GAME_X_OFFSET+GAME_X_OFFSET+GAME_320_WIDTH)) - (src->center.x256); 			/* ���Α��փ��[�v */
		#if (1)
		/* �ړ����W�����߂� */
	//	src->BOSS_DATA_04_toutatu_wariai256 = t256(1.0);	/* �U���A�j���[�V���� */
		REG_02_DEST_X	= (src->BOSS_DATA_00_target_x256);/*(t256()�`��)*/
		REG_03_DEST_Y	= (src->BOSS_DATA_01_target_y256);/*(t256()�`��)*/
		boss_set_new_position(src);/*(�U���ړ����W��ݒ�)*/
		#endif
	}
	/*(�ȉ��͏����I�ɋ��ʉ������\��)*/
	boss_move_00_card_settei_seisei(src);/*(�J�[�h�����I���Ȃ�J�[�h�ݒ肵�A�����B)*/
//r36���Ԃ�B	boss_re gacy_yuudou(src);
}

