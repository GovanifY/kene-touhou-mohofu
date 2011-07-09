
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
	�����Ŏv����������2�`�Ԃ̍U���p�^�[�����J�I�X�ɂȂ��Ă��܂����B
	�����ƒ��J�ɍ�蒼�����������������B
	�V��̑��x���Փx���Ƃɕς��Ă݂��B
//
	�������̗�����e(���p�̗h�炮��e)�ɂ��Ď���(231)
	������e�� (easy�{�����e) �ӂ�܂łȂ�ʔ����̂ł����A
	���s���낵�����ʁA
	hard, lunatic ������̒e���V���[�����Ƃ��āA
	�e���x���������Ȃ��̂ŁA�������ۂ��Ȃ�Ȃ��ł��B
	�����������R�Ŏ~�߂܂����B
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

#define bullet_create_aya_kougeki_23	s_bullet_create_aya_oodama3
#define bullet_create_aya_kougeki_22	s_bullet_create_aya_oodama3
#define bullet_create_aya_kougeki_21	s_bullet_create_aya_oodama3
static void s_bullet_create_aya_oodama3(SPRITE *src)
{
	send1_xy(src);	/* �e��x256 y256 ���S���甭�e�B */
	//
	br.BULLET_REGIST_06_n_way					= (12); 				/* [12way] */
	br.BULLET_REGIST_07_div_angle65536			= (65536/12);			/* �����p�x */	/* 30�x�Â��(12==360/30������12����) */
	//
//	br.BULLET_REGIST_03_tama_data				= (TAMA_DATA_0000_TILT);/* (r33-)�W���e */
	br.BULLET_REGIST_03_tama_data				= (TAMA_DATA_8000_NON_TILT);/* (r33-)��X���e */
//
	tmp_angleCCW65536_jiki_nerai(src);
	br.BULLET_REGIST_02_angle65536				= (src->tmp_angleCCW65536);
	const u32 aaa								= ((u32)((((src->tmp_angleCCW65536)>>8)&(0x03))));
	br.BULLET_REGIST_04_bullet_obj_type 		= (BULLET_OODAMA32_00_AOI+(aaa));	/* �e�O�� */
//	br.BULLET_REGIST_00_speed256				= speed256; 			/* �e�� */
	br.BULLET_REGIST_00_speed256				= (t256(4.0)-((cg_game_difficulty)<<7));			/* �e�� */
	br.BULLET_REGIST_01_speed_offset			= t256(-2);/* �����e�H */
	tama_system_regist_katayori_n_way();/* (r33-) */
}
	#if 0
			if (0==aaa) 	{		br.BULLET_REGIST_00_speed256			= (t256(2.0))+(difficulty<<8);	}//= (src->aya_speed)-t256(2.0);						}
	else	if (1==aaa) 	{		br.BULLET_REGIST_00_speed256			= (t256(2.0))+(difficulty<<7);	}//= ((src->aya_speed)>>1); 						}
	else					{		br.BULLET_REGIST_00_speed256			= (t256(4.0))+(difficulty<<7);	}//= /*�d�l�ύX*/(t256(4.0)+(difficulty<<7));/*, 10*/	}
	#endif
//							{		br.BULLET_REGIST_00_speed256			= (t256(3.0))+(difficulty<<7);	}





/*---------------------------------------------------------
	?�e
---------------------------------------------------------*/

static void bullet_create_aya_kougeki_01(SPRITE *src)
{
	send1_xy(src);	/* �e��x256 y256 ���S���甭�e�B */
//	h->aya_speed								= t256(4.0)+(difficulty<<8);
//	br.BULLET_REGIST_00_speed256				= (src->aya_speed)-t256(2.0);				/* �e�� */	/*t256(5.0)*/
	br.BULLET_REGIST_00_speed256				= (t256(2.0)+((cg_game_difficulty)<<8));				/* �e�� */	/*t256(5.0)*/
	br.BULLET_REGIST_02_VECTOR_angle1024		= ANGLE_JIKI_NERAI_DAN;
//	br.BULLET_REGIST_03_VECTOR_regist_type		= VEC TOR_REGIST_TYPE_00_MULTI_VECTOR;
	br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KOME_00_SIRO + (ra_nd()&0x0f); 	/* �e�O�� */	/* �e�ɓœh���Ă���ݒ� */
	br.BULLET_REGIST_06_n_way					= (5);										/* [5way] */
	br.BULLET_REGIST_07_VECTOR_div_angle1024	= cv1024r(30);//cv1024r(360-(30));			/* CCW�̏ꍇ */
	bullet_regist_multi_vector_direct();
}
/*---------------------------------------------------------
	?�e
---------------------------------------------------------*/

static void bullet_create_aya_kougeki_00(SPRITE *src)
{
	voice_play(VOICE11_BOSS_KIRARIN, TRACK04_TEKIDAN);/*�e�L�g�[*/
//
	send1_xy(src);	/* �e��x256 y256 ���S���甭�e�B */
	br.BULLET_REGIST_02_VECTOR_angle1024		= ANGLE_JIKI_NERAI_DAN;
//	br.BULLET_REGIST_03_VECTOR_regist_type		= VEC TOR_REGIST_TYPE_00_MULTI_VECTOR;
	br.BULLET_REGIST_07_VECTOR_div_angle1024	= (int)(1024/24);
	/* ���� */
	{
		unsigned int aaa;
		aaa = ((cg_game_difficulty)<<6);/*src->aya_speed*/
		aaa += (t256(4.5)); 		/* 3.5����easy���������B t256(3.5) */
		unsigned int jj;
		for (jj=(0); jj<(8+(cg_game_difficulty)); jj++ )
		{
			aaa -= (t256(0.125));
			/* �e�ɓœh���Ă���ݒ� */
			br.BULLET_REGIST_00_speed256			= (aaa);										/* �e�� */
			br.BULLET_REGIST_04_bullet_obj_type = BULLET_KOME_00_SIRO + (ra_nd()&0x0f); 	/* �e�O�� */
			br.BULLET_REGIST_06_n_way				= (11-jj);										/* [10way] */
			bullet_regist_multi_vector_direct();
		}
	}
}


/*---------------------------------------------------------
	�{�X�s���A�� 5�`��
	�ړ��U���p�^�[��2(�����ړ�)
---------------------------------------------------------*/

enum
{
	SS00 = 0,	/* �ޔ� */
	SS01,		/* �ړ� */
	SS02,		/* �U�� */
};

global void aya_05_keitai(SPRITE *src)
{
	if (SS00 ==src->boss_base_state777) 	/* �ޔ� */
	{
	//	ca se SS00: /* �s��:�����ʒu���̎擾->SS02�� */
	//	ca se SS02: /* �s��:�����ʒu�֖߂�->SS03�� */
		src->target_x256 = t256((GAME_WIDTH-50)/2);
	//	src->target_y256 = t256(20.0);
		src->target_y256 = t256(30.0); /* */
	//	if (/*150*/200 > src->toutatu_wariai256 )	/* ���Ԃ� */
		if (t256(31.0) > src->cy256)
		{
			src->toutatu_wariai256 = t256(1.00);
			src->boss_base_state777++;/* = SS03*/
			bullet_create_aya_kougeki_00(src);	/* �U�� */
		}
	}
	else
	if (SS01 ==src->boss_base_state777) 	/* �ړ� */
	{
	//	ca se SS03: /* �����ʒu:��e3��->SS02, SS02, SS04 */
		if (/*150*/240 > src->toutatu_wariai256 )	/* ���Ԃ� */
		{
			src->toutatu_wariai256 = t256(1.00);
			src->boss_base_state777++;
			bullet_create_aya_kougeki_23(src);	/* �U�� */
		//	int angle_jikinerai_1024;
		//	angle_jikinerai_1024 = src->tmp_angleCCW1024;
					/*t256(5.0)*/ /* �e�� */		/*�d�l�ύX*/	// t256(/*1.5*/5.0/*5.0*/)
			u8 zzz_aaa;
			{
				const u8 sst[8] = { 0, 0, 0, 1, 1, 1, 2, 2 }; /* (1/3)�̊m���ŕ��򂷂�B */
				zzz_aaa = sst[((ra_nd()&(8-1)))];	/* SS02, SS02, SS04 */
			}
			if (zzz_aaa == 0)
			{
				SPRITE *zzz_player;
				zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
			//	src->target_x256 = t256(GAME_WIDTH/2); /* �^���փ��[�v */
				src->target_x256 = zzz_player->cx256;
				src->target_y256 = zzz_player->cy256;
				#define KYORI_AAA (t256(128.0)+((cg_game_difficulty)<<(8+4/*5*/)))
				if (src->target_y256 > (signed)KYORI_AAA)
				{
					src->target_y256 = KYORI_AAA;
				}
				#undef KYORI_AAA
			}
			else
			if (zzz_aaa == 1)
			{
				src->target_x256 = t256(0); /* */
				src->cy256 += t256(6.0);/*fps_factor*/	/* �悭�킩��Ȃ�(???) */
			}
			else
		//	if (zzz_aaa == 2)
			{
				src->target_x256 = t256((GAME_WIDTH-50)); /* */
				src->cy256 += t256(6.0);/*fps_factor*/	/* �悭�킩��Ȃ�(???) */
			}
		}
	}
	else
//	if (SS02 ==src->boss_base_state777) 	/* �U�� */
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
		if (/*150*/220 > src->toutatu_wariai256 )	/* ���Ԃ� */
		{
			src->toutatu_wariai256 = t256(1.00);
		//	src->boss_base_state777++;	//	sakuya_wait_state_bbb(src/*,SS06*/);
			src->boss_base_state777 = SS00; /* SS00�� */
			//ca se SS06: /* ��:�E��]�U��->SS00, SS03 */
				u8 zzz_aaa;
			{
				const u8 sst[8] = { 0, 0, 0, 1, 1, 1, 2, 2 }; /* (1/3)�̊m���ŕ��򂷂�B */
				zzz_aaa = sst[((ra_nd()&(8-1)))];	/* SS02, SS02, SS04 */
			}
			if (zzz_aaa == 0)
			{
				bullet_create_aya_kougeki_21(src);	/* �U�� */
				#if (1==USE_CONTROLLER)
				eee_boss02_sr_add1(src);/* ��:�E��]�U�� */
				eee_boss02_sr_add2(src);/* �E:����]�U�� */
				#endif //(1==USE_CONTROLLER)
			}
			else
			if (zzz_aaa == 1)
			{
				bullet_create_aya_kougeki_01(src);	/* �U�� */
				src->target_y256 = t256(GAME_HEIGHT/3); /* */
			}
			else
		//	if (zzz_aaa == 2)
			{
				bullet_create_aya_kougeki_22(src);	/* �U�� */
			}
		//	break;
		}
	}
	#if 1
	boss_yuudou_idou_nomi(src);
	boss_yuudou_hiritu_keisan(src);
	#endif
	bullet_angle_all_gamen_gai_nara_kesu();/*�p�x�e�̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�)*/
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

