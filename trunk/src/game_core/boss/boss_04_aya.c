
#include "boss.h"

/*static*/extern void add_zako_aya_doll(SPRITE *src);/* ���e */

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�˖��� ��
	Syameimaru Aya.
	-------------------------------------------------------
	-------------------------------------------------------
	�������F�����Ŏv����������2�`�Ԃ̍U���p�^�[�����J�I�X�ɂȂ��Ă��܂����B
	�������F�����ƒ��J�ɍ�蒼�����������������B
	�������F�V��̑��x���Փx���Ƃɕς��Ă݂��B
---------------------------------------------------------*/


/* 1�Ȃ̂͐����Â��d�l�B���݂̓V�X�e���� ketm ������ CONTROLLER �͖����B
������ɑ��铌���� ������ CONTROLLER �͍�邩������Ȃ��B
( CONTROLLER ���Ă̂́A�{�X�̎���ӂ�ӂ킵�Ēe�o���A���B) */
#define USE_CONTROLLER (0)



	#if 0
			if (0==aaa) 	{		HATSUDAN_01_speed256		= (t256(2.0))+(difficulty<<8);	}// = (src->aya_speed)-t256(2.0);						}
	else	if (1==aaa) 	{		HATSUDAN_01_speed256		= (t256(2.0))+(difficulty<<7);	}// = ((src->aya_speed)>>1);							}
	else					{		HATSUDAN_01_speed256		= (t256(4.0))+(difficulty<<7);	}// = /*�d�l�ύX*/(t256(4.0)+(difficulty<<7));/*, 10*/	}
//							{		HATSUDAN_01_speed256		= (t256(3.0))+(difficulty<<7);	}
	#endif


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
static int kougeki_type;

static void aya_kougeki_all(SPRITE *src)
{
	//(r35�ŃJ�[�h�̏ꍇ�̂ݏȗ��\)	set_REG_DEST_XY(src);	/* �e��x256 y256 ���S���甭�e�B */
	set_REG_DEST_XY(src);	/* �e��x256 y256 ���S���甭�e�B */
	calculate_jikinerai();
	#if 0/*(r35�̃V�X�e���ł́A�ȗ��\)*/
	HATSUDAN_03_angle65536				= (HATSUDAN_03_angle65536); 	/* [�j��] */
	#endif
	//
//	if (AYA_KOUGEKI_TYPE_00_NONE==kougeki_type)
//	{
//	}
//	else
	if (AYA_KOUGEKI_TYPE_01_RENDAN==kougeki_type)	/* �U���^�C�v00:	�ĘA�e */
	{	voice_play(VOICE11_BOSS_KIRARIN, TRACK04_TEKIDAN);/*�e�L�g�[*/
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
	if (AYA_KOUGEKI_TYPE_02_FUDADAN==kougeki_type)	/* �U���^�C�v01:	��ǂ��D�e */
	{	/*(�D�e�͈Ӑ}�I�ɒx������)*/
	//	HATSUDAN_01_speed256			= (t256(2.0)+((REG_0f_GAME_DIFFICULTY)<<8));	/* �e�� */	/*t256(5.0)*/
		HATSUDAN_01_speed256			= (t256(1.5)+((REG_0f_GAME_DIFFICULTY)<<4));	/* �e�� */	/*t256(5.0)*/
		HATSUDAN_02_speed_offset		= t256(1);/* �H */
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_3000_EFFECT_NONE)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
	//	HATSUDAN_05_bullet_obj_type 	= BULLET_KOME_BASE + (ra_nd()&0x0f);	/* �e�O�� */	/* �e�ɓœh���Ă���ݒ� */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_OFUDA12_00_AOI) + (src->BOSS_DATA_06_aya_atooidan_type);	/* �e�O�� */	/* �e�ɓœh���Ă���ݒ� */
		HATSUDAN_06_n_way				= (5);			/* [�j��] */	/* [5way] */
		HATSUDAN_07_div_angle65536		= (65536/12);	/* [�j��] */	/* 12���� == (30/360)[�x] */
		hatudan_system_regist_n_way();/* (r33-) */
	}
	else
//	if (AYA_KOUGEKI_TYPE_03_OODAMA==kougeki_type)	/* �U���^�C�v02:	��ʒe */
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

#define SS_TAN_I_JIKAN (4096)
/*
---0
�ޔ�
---1
---2
---3

*/

global void aya_05_keitai(SPRITE *src)
{
	const u8 sst[8] = { 0, 0, 0, 1, 1, 1, 2, 2 }; /* (1/3)�̊m���ŕ��򂷂�B */
	src->BOSS_DATA_05_boss_base_state777++;
	if ( (SS_TAN_I_JIKAN*1) > src->BOSS_DATA_05_boss_base_state777) 	/* [A]�ޔ� */
	{
	//	ca se SS00: /* �s��:�����ʒu���̎擾->SS02�� */
	//	ca se SS02: /* �s��:�����ʒu�֖߂�->SS03�� */
		src->BOSS_DATA_00_target_x256 = t256((GAME_WIDTH-50)/2);
	//	src->BOSS_DATA_01_target_y256 = t256(20.0);
		src->BOSS_DATA_01_target_y256 = t256(30.0); /* */
	//	if (/*150*/200 > src->BOSS_DATA_04_toutatu_wariai256 )	/* ���Ԃ� */
		if (t256(31.0) > src->cy256)
		{
			src->BOSS_DATA_04_toutatu_wariai256 = t256(1.00);
			src->BOSS_DATA_05_boss_base_state777 = (SS_TAN_I_JIKAN*1);
			kougeki_type = AYA_KOUGEKI_TYPE_01_RENDAN;	/* �U�� */
		}
	}
	else
	if ( (SS_TAN_I_JIKAN*2) > src->BOSS_DATA_05_boss_base_state777) /* [B]�ړ� */
	{
	//	ca se SS03: /* �����ʒu:��e3��->SS02, SS02, SS04 */
		if (/*150*/240 > src->BOSS_DATA_04_toutatu_wariai256 )	/* ���Ԃ� */
		{
			src->BOSS_DATA_04_toutatu_wariai256 = t256(1.00);
			src->BOSS_DATA_05_boss_base_state777 = (SS_TAN_I_JIKAN*3);
			kougeki_type = AYA_KOUGEKI_TYPE_03_OODAMA_23;	/* �U�� */
					/*t256(5.0)*/ /* �e�� */		/*�d�l�ύX*/	// t256(/*1.5*/5.0/*5.0*/)
			{
				src->BOSS_DATA_06_aya_atooidan_type = sst[((ra_nd()&(8-1)))];	/* SS02, SS02, SS04 */
			}
			if (0 == src->BOSS_DATA_06_aya_atooidan_type)
			{
				SPRITE *zzz_player;
				zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
			//	src->BOSS_DATA_00_target_x256 = t256(GAME_WIDTH/2); /* �^���փ��[�v */
				src->BOSS_DATA_00_target_x256 = zzz_player->cx256;
				src->BOSS_DATA_01_target_y256 = zzz_player->cy256;
			//	#define KYORI_AAA (t256(128.0)+((cg_game_di fficulty)<<(8+4/*5*/)))
				#define KYORI_AAA (t256(128.0)+((3)<<(8+4/*5*/)))
				if (src->BOSS_DATA_01_target_y256 > (signed)KYORI_AAA)
				{
					src->BOSS_DATA_01_target_y256 = KYORI_AAA;
				}
				#undef KYORI_AAA
			}
			else
			if (1 == src->BOSS_DATA_06_aya_atooidan_type)
			{
				src->BOSS_DATA_00_target_x256 = t256(0); /* */
				src->cy256 += t256(6.0);/*fps_factor*/	/* �悭�킩��Ȃ�(???) */
			}
			else
		//	if (2 == src->BOSS_DATA_06_aya_atooidan_type)
			{
				src->BOSS_DATA_00_target_x256 = t256((GAME_WIDTH-50)); /* */
				src->cy256 += t256(6.0);/*fps_factor*/	/* �悭�킩��Ȃ�(???) */
			}
		}
	}
	else
//	if (SS02 ==src->BOSS_DATA_05_boss_base_state777)	/* �U�� */
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
		if (/*150*/220 > src->BOSS_DATA_04_toutatu_wariai256 )	/* ���Ԃ� */
		{
			src->BOSS_DATA_04_toutatu_wariai256 = t256(1.00);
		//	src->BOSS_DATA_05_boss_base_state777++; //	sakuya_wait_state_bbb(src/*,SS06*/);
			src->BOSS_DATA_05_boss_base_state777 = (SS_TAN_I_JIKAN*0); /* SS00�� */
			//ca se SS06: /* ��:�E��]�U��->SS00, SS03 */
				u8 zzz_aaa;
			{
				zzz_aaa = sst[((ra_nd()&(8-1)))];	/* SS02, SS02, SS04 */
			}
			if (zzz_aaa == 0)
			{
				kougeki_type = AYA_KOUGEKI_TYPE_02_FUDADAN; 	/* �U�� */
				src->BOSS_DATA_01_target_y256 = t256(GAME_HEIGHT/3); /* */
			}
			else
			#if (1==USE_CONTROLLER)
			if (zzz_aaa == 1)
			{
				kougeki_type = AYA_KOUGEKI_TYPE_03_OODAMA_21;	/* �U�� */
				#if (1==USE_CONTROLLER)
				eee_boss02_sr_add1(src);/* ��:�E��]�U�� */
				eee_boss02_sr_add2(src);/* �E:����]�U�� */
				#endif //(1==USE_CONTROLLER)
			}
			else
			#endif //(1==USE_CONTROLLER)
		//	if (zzz_aaa == 2)
			{
				kougeki_type = AYA_KOUGEKI_TYPE_03_OODAMA_22;	/* �U�� */
			}
		}
	}
	if (AYA_KOUGEKI_TYPE_00_NONE!=kougeki_type)
	{
		aya_kougeki_all(src);
		kougeki_type = AYA_KOUGEKI_TYPE_00_NONE;
	}
	boss_yuudou_idou_nomi(src);
	boss_yuudou_hiritu_keisan(src);
}



/*---------------------------------------------------------
	�{�X�s���A�� 4�`��
	�ړ��p�^�[��1(�ᑬ�ړ�)
	-------------------------------------------------------
	�{�X�s���A�� 3�`��
	�ړ��p�^�[��0(�ᑬ�ړ�)
---------------------------------------------------------*/

/*---------------------------------------------------------
	�{�X�s���A�� 2�`��
	�ړ��p�^�[��2(�ؕ��u�F�؈�ࣁv�̃e�X�g)
	-------------------------------------------------------
	�{�X�s���A�� 1�`��
	�ړ��p�^�[��(�u�ʏ�U��3�v�̃e�X�g)
---------------------------------------------------------*/

