
/*---------------------------------------------------------
 �����͕핗 �` Toho Imitation Style.
  �v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	����̒e���𐶐����܂��B
---------------------------------------------------------*/


/*---------------------------------------------------------
	�g3�ʒ�-�{�X �g����
	�ʏ�U��1(1/2)
	16���ʕ΂�S���ʂ΂�T���e
	-------------------------------------------------------
---------------------------------------------------------*/
#if 0
#endif

/*---------------------------------------------------------
	�g3�ʒ�-�{�X �g����
	�ؕ��u�F�؈�ࣁv�ɂ�����Ƃ��������e��(�\��)
	���v���A�����v����6way�����e(���F)		1�񖈂�(1024/(6*8))[1024/1024�x]��]
	48���ʑS���ʒe(�ԐF)						8�񖈂ɔ��e
	-------------------------------------------------------
	�F�؈��
	�𑜓x�Ƃ����x�Ƃ��̗v���ŁA���̂܂܂ł� psp�œ���C������B
	Lunatic �͂��̂܂܂ŁAHard �ȉ��͍׍H���ĈՂ�������B
---------------------------------------------------------*/
#if 1
static void spell_create_0a_houka_kenran(SPRITE *src)
{
//	if ((0x10)==((src->boss_base_spell_time_out)&0x1f))/* (16���1��)(128�Ȃ�v8��) */
	if ((0x40)==((src->boss_base_spell_time_out)&0x7f))/* (16���1��)(128�Ȃ�v8��) */
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);/* �{�Ƃ͂��������� */
		#endif
		set_sub_jikinerai(src);/* 1:���������쐬������ۂ��B */
		br.BULLET_REGIST_00_speed256				= (t256(1.75)); 									/* �e�� */	/* 3.5 2.5 2.0 */
		br.BULLET_REGIST_01_speed_offset			= t256(0);/*(�e�X�g)*/
		br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)�W���e */
		br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KOME_01_AKA;								/* [�ԐF�Ēe] */
	//����br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)�W���e */
		br.BULLET_REGIST_06_n_way					= spell_bunkatsu_nums_table[b_HOUGA_08_RED_NUMS+(cg_game_difficulty)];			/*(48)*/				/* [48way] */	/* ���e�� */
		br.BULLET_REGIST_07_div_angle65536			= spell_nan_ido_table[HOUGA_12_RED_DIV_ANGLE+(cg_game_difficulty)]; 	/*(int)(1024/(48))*/	/* �����p�x(1024[360/360�x]�� 48 ����) */	/* 1����n���������p�x */
		tama_system_regist_katayori_n_way();/* (r33-) */
	}
//	if ((0x02)==((src->boss_base_spell_time_out)&0x03))/* (2���1��)(8�񖈂ɔ��e) */
	if ((0x08)==((src->boss_base_spell_time_out)&0x0f))/* (2���1��)(8�񖈂ɔ��e) */
	{
		obj_send1->cx256							= (src->cx256); 	/* �e��x256 �{�X���S���甭�e�B */
		obj_send1->cy256							= (src->cy256); 	/* �e��y256 �{�X���S���甭�e�B */
		static int aaa_angle65536;
		br.BULLET_REGIST_00_speed256				= (t256(1.75)); 										/* �e�� */		/*3.5 2.5 2.0*/
		br.BULLET_REGIST_01_speed_offset			= t256(0);/*(�e�X�g)*/
		br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)�W���e */
		br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KOME_06_KI_IRO;								/* [���F�Ēe] */
	//����br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)�W���e */
		br.BULLET_REGIST_06_n_way					= spell_bunkatsu_nums_table[b_HOUGA_00_YEL_NUMS+(cg_game_difficulty)];				/*(6)*/ 				/* [1way] */	/* ���e�� */
		br.BULLET_REGIST_07_div_angle65536			= spell_nan_ido_table[HOUGA_04_YEL_DIV_ANGLE+(cg_game_difficulty)]; 	/*(int)(1024/(6))*/ 	/* �����p�x(1024[360/360�x]�� 6 ����) */	/* 1����n���������p�x */
		// �����(��CCW������A�����CCW)
		br.BULLET_REGIST_02_angle65536				= ((/*0+*/(aaa_angle65536))&(65536-1)); 				/* ���˒��S�p�x / ����@�\(���@�_��/��) */
		tama_system_regist_katayori_n_way();/* (r33-) */
		// �t���(��CCW������A�E���CW)
		br.BULLET_REGIST_02_angle65536				= ((65536-(aaa_angle65536))&(65536-1)); 				/* ���˒��S�p�x / ����@�\(���@�_��/��) */
		tama_system_regist_katayori_n_way();/* (r33-) */
		// ��]��
		aaa_angle65536 += spell_nan_ido_table[HOUGA_16_YEL_ROTATE_ANGLE+(cg_game_difficulty)];			/*(1024/(6*8))*/		/* �p�x(1024[360/360�x]�� 48����) */
	}
}
#endif

#if 0
static const s8 step_tbl[(4+4)] =
{
	(1024/24),	/* easy */
	(1024/28),	/* normal */
	(1024/30),	/* hard */
	(1024/32),	/* lunatic */
//
	(24),	/* easy */
	(28),	/* normal */
	(30),	/* hard */
	(32),	/* lunatic */
};
#endif

/*---------------------------------------------------------
	���ɔ�����ۂ��e���������Ă݂�e�X�g(�̔��������񂾂��ǁA
	���̂������̕����Ƃ͈Ⴄ�����Ɂc�c)
	-------------------------------------------------------
---------------------------------------------------------*/
#if 1

//extern void exchange_damnaku_check_type(void);
/*---------------------------------------------------------
normal? �g�N�i�C	24����?
hard	�g�N�i�C	35����(36�����ł͖���)

---------------------------------------------------------*/

static void s_change_meirin_yajirusi_one(SPRITE *h)
{
	/* ���݂̒e���W���A��_���W�ɂ���B */
	h->tx256 = h->cx256;/*fps_factor*/
	h->ty256 = h->cy256;/*fps_factor*/
	//
//	h->type 						= (BULLET_KUNAI12_00_AOI+1);/* �e���Ԓe�ɕϐg */
//	h->tama_system_speed256 					= t256(0.5);		/* ���x */	/* ����(�ł��o�����x) */
//	h->tama_system_speed256 					= t256(0.0)+((h->tama_system_radius256)>>5);	/* ���a���傫�����A���������� */	/* ����(�ł��o�����x) */
//	h->tama_system_speed256 					= t256(0.0)+((h->tama_system_radius256)>>6);	/* ���a���傫�����A���������� */	/* ����(�ł��o�����x) */
	h->tama_system_speed65536					= ((t256(0.0)+((h->tama_system_radius256)>>6))<<8); /* ���a���傫�����A���������� */	/* ����(�ł��o�����x) */
	h->tama_system_radius256					= t256(0);/* ���a */
	h->tama_system_tra65536 					= t256(1.0);		/* ���������e */
	const int ao_aka_tbl[(2)]	=
	{
	//	 (1024/2)+(1024/32),/* 180/360���傢��] */ 	/* �� */
	//	-(1024/2)-(1024/32),/* 180/360���傢��] */ 	/* �� */
		 (1024/2)+(1024/24),/* 180/360���傢��] */ 	/* �� */
		-(1024/2)-(1024/24),/* 180/360���傢��] */ 	/* �� */
	};
//	h->rotationCCW1024				+= (1024/2)+(1024/16);/* 180/360���傢��] */
//	h->rotationCCW1024				+= (1024/2)+(1024/32);/* 180/360���傢��] */
	h->rotationCCW1024				+= ao_aka_tbl[(((br.BULLET_REGIST_04_bullet_obj_type)&2)>>1)];/* 180/360���傢��] */
	mask1024(h->rotationCCW1024);
}
//global void exchange_damnaku_check_type(void)
static void s_exchange_damnaku_check_type(void)
{
	int check_type;
	check_type = br.BULLET_REGIST_04_bullet_obj_type;/* ���ׂ�^�C�v���󂯎�� */
	int ii;
	for (ii=0; ii<OBJ_POOL_00_TAMA_MAX; ii++ )/* �S�����ׂ�B */
	{
		SPRITE *s;
		s = &obj99[OBJ_HEAD_00_TAMA+ii];
		if (check_type == (s->type) )	/* ���̐e���Ԓe�Ȃ� */
		{
			s_change_meirin_yajirusi_one(s);
		}
	}
}

static void spell_create_12_aya_merin_test(SPRITE *src)
{
/*
time
f	1111	��1 			[���e]
e	1110
d	1101			��1 	[���e]
c	1100
b	1011	��2 			[�ϐg]
a	1010
9	1001			��2 	[�ϐg]
8	1000
7
...
0
*/
	/* [���e] */
	if (0xd0==((src->boss_base_spell_time_out)&0xdf))/* (256���2��) */
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		obj_send1->cx256						= (src->cx256); 	/* �e��x256 �{�X���S���甭�e�B */
		obj_send1->cy256						= (src->cy256); 	/* �e��y256 �{�X���S���甭�e�B */
		unsigned int jj;
		for (jj=(0); jj<(6); jj++)
		{
			br.BULLET_REGIST_00_speed256				= (t256(4.0)-(jj<<6));				/* �e�� */
			br.BULLET_REGIST_01_speed_offset			= -t256(5); 						/* (-5) (-3)���������e */	/* ���̕����ɂȂ邩������ */
			br.BULLET_REGIST_02_angle65536				= (0/65536);						/* ������ */
			int check_type = BULLET_KUNAI12_01_AKA+((src->boss_base_spell_time_out>>4)&2);	/* �ԐN�i�C�e +(0&1)*/
			br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)�W���e */
			br.BULLET_REGIST_04_bullet_obj_type 		= check_type;
		//����br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)�W���e */
			br.BULLET_REGIST_06_n_way					= (24);//step_tbl[(difficulty+4)];/*(32)*/
			br.BULLET_REGIST_07_div_angle65536			= (65536/24);				/* �����p�x([360/360]�x��24����) */ 	//step_tbl[(difficulty)];/*(32)*/
			tama_system_regist_katayori_n_way();/* (r33-) */
		}
	}
	else
	/* [�ϐg] */
//	if (0x90==((src->boss_base_spell_time_out)&0xdf))/* (256���2��) (r32) */
	if (0x50==((src->boss_base_spell_time_out)&0xdf))/* (256���2��) (r33)���e�G�t�F�N�g���Ԃ�����̂�64�J�E���g���炷 */
	{
		/* �ϐg�\�Ȓe�̏�����ݒ� */
		br.BULLET_REGIST_04_bullet_obj_type 	= BULLET_KUNAI12_01_AKA+((src->boss_base_spell_time_out>>4)&2); /*+(0&1)*/
		/* ����ʂɂ���e��S�����ׂāA�e��ϐg������B */
		s_exchange_damnaku_check_type();
	}
	/* �J */
	if (0x30>((src->boss_base_spell_time_out)&0xff))/* (256��� ??��) */
	{
		if (0==((src->boss_base_spell_time_out)&0x03))	// 4�J�E���g��1����8way�e������
		{
			obj_send1->cx256						= (src->cx256); 	/* �e��x256 �{�X���S���甭�e�B */
			obj_send1->cy256						= (src->cy256); 	/* �e��y256 �{�X���S���甭�e�B */
			//
			unsigned int jj;
			for (jj=(0); jj<(8); jj++)
			{
				/* �V���b�g */
			//	b05_fire_flags &= (~(doll_data->identity_bit));/* off */
				br.BULLET_REGIST_00_speed256						= (t256(1.5)+((cg_game_difficulty)<<6));
				br.BULLET_REGIST_02_VECTOR_angle1024				= (1024/4)+(1024/8)+(jj<<5)+((src->boss_base_spell_time_out>>2)&0x3f);
				br.BULLET_REGIST_04_bullet_obj_type 				= (BULLET_KOME_00_SIRO);	/* �e�O�� */	/* [���Ēe] */
				br.BULLET_REGIST_05_regist_type 					= REGIST_TYPE_02_GRAVITY02;
	//			br.BULLET_REGIST_06_n_way							= (8);
//				br.BULLET_REGIST_07_VECTOR_div_angle1024			= (int)(1024/64);
				br.BULLET_REGIST_07_VECTOR_jyuryoku_dan_delta256	= ((ra_nd()&0x03)+2);//t256(0.04)
				bullet_regist_vector();
			}
		}
	}
}
#endif

/*---------------------------------------------------------
	�����u�ؑz�����v�͐��^(5����)�ɔz�u����B
	-------------------------------------------------------
---------------------------------------------------------*/
