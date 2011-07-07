
/*---------------------------------------------------------
 �����͕핗 �` Toho Imitation Style.
  �v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�A���X�̒e���𐶐����܂��B
---------------------------------------------------------*/

/*---------------------------------------------------------
	�A���X(��)	�Ԃ̖��@(2�i�K��)�̂悤�Ȃ��́B
	-------------------------------------------------------
	�ړ��𐧌�����e���B
	�Ƃ肠�����A���X������A�Ԃ̖��@��2�i�K�ځB
---------------------------------------------------------*/
static void spell_create_0e_aka_2nd(SPRITE *src)
{
	static int os2x;
	if (0==((src->boss_base_spell_time_out)&0x03))
	{
		obj_send1->cx256					= (src->cx256); 	/* �e��x256 �{�X���S���甭�e�B */
		obj_send1->cy256					= (src->cy256); 	/* �e��y256 �{�X���S���甭�e�B */
		if (0==((src->boss_base_spell_time_out)&0x0f))
		{
			br.BULLET_REGIST_00_speed256				= (t256(1.0));							/* �e�� */
			br.BULLET_REGIST_01_speed_offset			= t256(1/*6*/);/*(�e�X�g)*/
			br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)�W���e */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KOME_01_AKA;					/* [�ԕĒe] */
		//����br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)�W���e */
			br.BULLET_REGIST_06_n_way					= (8);									/* [8way] */
			br.BULLET_REGIST_07_div_angle65536			= (int)(65536/48);						/* �����p�x(1024/(6*8) ) �P����8���������̈�������6���������p�x */
			{
				/* tama_system_regist_katayori_n_way�̎d�l���P���ȈׁA�␳���� */
				#define BA_HOSEI48	((65536/(48/8))/2)	/* ��� 65536�n�� 48�����e x 8 way �̐^��(�܂�/2)�t�� */
				const int tmp_kakudo =
					(65536/(4*2)) + 	/* (65536/(4*2)) == �܂�(90/360)�x�̔���������B(���2�{�ɂȂ�) */	/* �e����΂ɂ��Ȃ��͈� */
				//	(65536/(8*2)) + 	/* (65536/(8*2)) == �܂�(45/360)�x�̔���������B(���2�{�ɂȂ�) */	/* �e����΂ɂ��Ȃ��͈� */
				//	(65536/128) +	/* �܂�(2.8125/360)�x������B(���E�Ȃ̂�2�{==5.625�x�ɂȂ�) */	/* �e����΂ɂ��Ȃ��͈� */
					((256+sin1024((src->boss_base_spell_time_out<<2)&(1024-1)))<<4);				/* ���ɗh�炬����������B */
				/* �E�� */
				br.BULLET_REGIST_02_angle65536		= (( tmp_kakudo-BA_HOSEI48)&(65536-1)); 		/* �p�x(��CCW�Ȃ̂Ő�����==�E��) */
				tama_system_regist_katayori_n_way();/* (r33-) */
			//
				/* ���� */
				br.BULLET_REGIST_02_angle65536		= ((-tmp_kakudo-BA_HOSEI48)&(65536-1)); 		/* �p�x(��CCW�Ȃ̂ŕ�����==����) */
				tama_system_regist_katayori_n_way();/* (r33-) */
			}
		}
		if (0x00==((src->boss_base_spell_time_out)&0x1f))
		{
			#if (1)
		//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
			#endif
		//
			SPRITE *zzz_player;
			zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
			tmp_angleCCW65536_src_nerai(zzz_player, obj_send1);/* ���@�_���p�쐬 */
			src->tmp_angleCCW65536		= obj_send1->tmp_angleCCW65536;/* ���@�_���p */
			os2x = (src->cx256)+(ra_nd()&0x1fff)-t256((32)/2);							/* �e��x256 */
		}
		//	br.BULLET_REGIST_00_speed256				= (t256(1.1));							/* �e�� */
			br.BULLET_REGIST_00_speed256				= (t256(1.0)+((0x1f-((src->boss_base_spell_time_out)&0x1f))));	/* �e�� */ /* ��Ō��ق������� */
			br.BULLET_REGIST_01_speed_offset			= t256(3/*6*/);/*(�e�X�g)*/
			br.BULLET_REGIST_02_angle65536				= (src->tmp_angleCCW65536);/* ���@�_���p */
			br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)�W���e */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_UROKO14_01_AKA;				/* [�ԗؒe] */
		//����br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)�W���e */
		//
		{
			obj_send1->cx256							= os2x; 						/* �e��x256 */
			/* ���@�_�� */
			tama_system_regist_single();
		}
	}
}


/*---------------------------------------------------------
	�A���X normal�ʏ�U��3(�̈ꕔ)
	-------------------------------------------------------
	�Ȃ񂩑S�R�܂��������ĂȂ��B
	�d�X�����̂���őn���Ă��������Y�k���ɂȂ����B
	���������Ă�̂��킩��Ȃ��Ǝv�����A�킽���ɂ��킩��Ȃ���B
	���낵�����̂̕З؂��ȉ����B
	psp�𑜓x���S�R�Ⴄ����A�����Ɏ����悤�Ƃ��Ȃ������ǂ������H
---------------------------------------------------------*/

static void spell_create_17_alice_nejiri10sec(SPRITE *src)
{
	if ((64*1)<(src->boss_base_spell_time_out))
	{
		/* [CW ���v���] */
		if (0==((src->boss_base_spell_time_out)&0x03))
		{
			#if (1)
		//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
			#endif
		//
			src->tmp_angleCCW65536 -= (int)(65536/24);
			#if 0/*1885624*/
			src->tmp_angleCCW65536 += (int)(ra_nd() & ((1<<difficulty)-1) );
			#else
			{static const u8 masked_tbl[4] = { 0x00, 0x0f, 0x3f, 0xff };
				src->tmp_angleCCW65536 += (int)(ra_nd() & (masked_tbl[(cg_game_difficulty)]) );
			}
			#endif
			mask65536(src->tmp_angleCCW65536);
		//
				obj_send1->cx256						= (src->cx256); 						/* �e��x256 */
				obj_send1->cy256						= (src->cy256); 						/* �e��y256 */
				//
				br.BULLET_REGIST_00_speed256				= (t256(1.0));							/* �e�� */
				br.BULLET_REGIST_01_speed_offset			= t256(1);/*(�e�X�g)*/
			//	br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)�W���e */
				br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_8000_NON_TILT);/* (r33-)��X���e */
				br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_WAKU12_03_AOI; 				/* [�g�t���ےe] */
			//����br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)�W���e */
				br.BULLET_REGIST_06_n_way					= (6);									/* [6way] */
				br.BULLET_REGIST_07_div_angle65536			= (int)(65536/48);						/* �����p�x(1024/(6*8) ) �P����8���������̈�������6���������p�x */
		//
			{
				br.BULLET_REGIST_02_angle65536				= (src->tmp_angleCCW65536); 							/* �p�x */
				tama_system_regist_katayori_n_way();/* (r33-) */
			//
				br.BULLET_REGIST_02_angle65536				= ((src->tmp_angleCCW65536+(65536/2))&(65536-1));					/* �p�x */
				tama_system_regist_katayori_n_way();/* (r33-) */
			}
		}
	}
	else
	if ((64*2)<(src->boss_base_spell_time_out))
	{
		/*[CCW �����v���]*/
		if (0==((src->boss_base_spell_time_out)&0x03))
		{
			#if (1)
		//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
			#endif
		//
			src->tmp_angleCCW65536 += (int)(65536/24);
			#if 0/*1885624*/
			src->tmp_angleCCW65536 -= (int)(ra_nd() & ((1<<difficulty)-1) );
			#else
			{static const u8 masked_tbl[4] = { 0x00, 0x0f, 0x3f, 0xff };
				src->tmp_angleCCW65536 -= (int)(ra_nd() & (masked_tbl[(cg_game_difficulty)]) );
			}
			#endif
			mask65536(src->tmp_angleCCW65536);
		//
				obj_send1->cx256						= (src->cx256); 						/* �e��x256 */
				obj_send1->cy256						= (src->cy256); 						/* �e��y256 */
				//
				br.BULLET_REGIST_00_speed256				= (t256(1.0));							/* �e�� */
				br.BULLET_REGIST_01_speed_offset			= t256(1);/*(�e�X�g)*/
			//	br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)�W���e */
				br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_8000_NON_TILT);/* (r33-)��X���e */
				br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_WAKU12_01_AKA; 				/* [�g�t���Ԋےe] */
			//����br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)�W���e */
				br.BULLET_REGIST_06_n_way					= (6);									/* [6way] */
				br.BULLET_REGIST_07_div_angle65536			= (int)(65536/48);						/* �����p�x(1024/(6*8) ) �P����8���������̈�������6���������p�x */
		//
			{
				br.BULLET_REGIST_02_angle65536				= (src->tmp_angleCCW65536); 							/* �p�x */
				tama_system_regist_katayori_n_way();/* (r33-) */
			//
				br.BULLET_REGIST_02_angle65536				= ((src->tmp_angleCCW65536+(65536/2))&(65536-1));					/* �p�x */
				tama_system_regist_katayori_n_way();/* (r33-) */
			}
		}
	}
}


/*---------------------------------------------------------
	�A���X	�A���X�l�`�e��
	-------------------------------------------------------
�����u�����̕������l�` -easy-�v
�A���X�̕������l�̂̐l�`���A��񕪗􂷂�7way�e�����B
4 x 7 x 7 == �ő� 196[�e](�d�X��)
3 x 7 x 7 == �ő� 147[�e](�͕핗)	//3 x 8 x 8 == 192
	-------------------------------------------------------
�����u�����̕������l�`�v
�A���X�̕������Z�̂̐l�`���A��񕪗􂷂�7way�e�����B
6 x 7 x 7 == �ő� 294[�e](�d�X��)
5 x 7 x 7 == �ő� 245[�e](�͕핗)	//5 x 8 x 8 == 320
	-------------------------------------------------------
�����u�����̕������l�` -hard-�v
�A���X�̕��������̂̐l�`���A��񕪗􂷂�7way�e�����B
8 x 7 x 7 == �ő� 392[�e](�d�X��)
9 x 7 x 7 == �ő� 441[�e](�͕핗)	//7 x 8 x 8 == 448
	-------------------------------------------------------
�����u�����̃I�����A���l�`�v�Ԓe���Βe
�A���X�̕��������̂̐l�`���A�O�񕪗􂷂�7way�e�����B
8 x 7 x 7 x 7 == �ő�2744[�e](�d�X��)
2 x 7 x 7 x 7 == �ő� 686[�e](�͕핗)	//9 x 8 x 8 == 576
---------------------------------------------------------*/
extern void add_zako_alice_doll(SPRITE *src);/* �A���X�l�`�e�� */
static void spell_create_0b_alice_doll(SPRITE *src)
{
	if (50==((src->boss_base_spell_time_out) ))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		add_zako_alice_doll(src);
	}
}

/*---------------------------------------------------------
	�v7�ɕ��􂷂�B
	-------------------------------------------------------


---------------------------------------------------------*/
#if 0
static void exchange_damnaku_alice_7_bunretu(void)
{
//	int check_type;
//	check_type = br.BULLET_REGIST_04_bullet_obj_type;/* ���ׂ�^�C�v���󂯎�� */
	int jj = 0;
	int ii;
	for (ii=0; ii<OBJ_POOL_00_TAMA_MAX; ii++ )/* �S�����ׂ�B */
	{
		SPRITE *s;
		s = &obj99[OBJ_HEAD_00_TAMA+ii];
	//	if (check_type == (s->type) )	/* �F�Ɋ֌W�Ȃ��ؒe[ ... ]�Ȃ� */
		if (((BULLET_UROKO14_03_AOI+1) & 0xfff8) == (s->type & 0xfff8) )	/* [�ԗؒe]�Ȃ� */
		{
			jj++;
		//	jj &= 0x07; 	// 2 3 4 5	6 7 8 9
			jj &= 0x03; 	// 3 4 5 6
		//	s_change_meirin_yajirusi_one(s);
		//	s->tra65536 					= t256(6);		/* ���������e */
		//	s->tra65536 					= (jj<<8)+t256(2);	/* (2 ... 9) ���������e */
			s->tra65536 					= (jj<<8)+t256(3);	/* (3 ... 6) ���������e */
			/* (1�͒x�������̂ŏ��O) 9 ���Ƒ��߂��邩�� */
		}
	}
}
#endif
extern void add_zako_alice_doll_type_b(SPRITE *src);/* �A���X�l�`�e�� */
static void spell_create_1e_alice_doll(SPRITE *src)
{
	if (250==((src->boss_base_spell_time_out) ))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		add_zako_alice_doll_type_b(src);
	}
	if (0x00==((src->boss_base_spell_time_out)&0x3f))
	{
		/* �e���� */
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
//		exchange_damnaku_arice_7_bunretu();
	}
}
/*
// �����u�����̕������l�`�v
	�e�l�`��1�e����(�e)
	�O���Ɏˏo�B
//
	180�x���]����B
	�V�e�ɕ���(�e�����e)
  A   A    A   A   --- ���x����

		A------------- ���x��
		|
   A	|	 A	------ ���x�x��
		+
//
	180�x���]����B
	�V�e�ɕ���(���e���Ԓe)
  A   A    A   A   --- ���x����

		A------------- ���x��
		|
   A	|	 A	------ ���x�x��
		+
// �����u�����̃I�����A���l�`�v�̂�
	180�x���]����B
	�V�e�ɕ���(�Ԓe���Βe)
  A   A    A   A   --- ���x����

		A------------- ���x��
		|
   A	|	 A	------ ���x�x��
		+
//

*/

/*---------------------------------------------------------
	�A���X	�z�K�q���ۂ�(?)�e��
	-------------------------------------------------------
	�e�L�g�[
---------------------------------------------------------*/

static void spell_create_25_alice_suwako(SPRITE *src)
{
//	if ((0) ==((src->boss_base_spell_time_out)&0x01))/* 2���1�� */
	{
		static int jjj;
	//	if ((0) ==((src->boss_base_spell_time_out)&0x07))/*  8��1�� */
		if ((0) ==((src->boss_base_spell_time_out)&0x0f))/* 16��1�� */
		{
		//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
			jjj -= (int)(65536*8/256);
		}
	//	jjj -= (65536/256); 	/* 1����256���������p�x */
	//	jjj -= (65536/512); 	/* 1����256���������p�x�̔����B */
		jjj -= (200);	/*	�e�L�g�[ 1����256���������p�x�̔����B */
		//
			obj_send1->cx256					= (src->cx256); 	/* �e��x256 �{�X���S���甭�e�B */
			obj_send1->cy256					= (src->cy256); 	/* �e��y256 �{�X���S���甭�e�B */
		/* �e���� */
		{
			br.BULLET_REGIST_00_speed256				= (t256(1.0) ); 					/* �e��(psp�̉�ʂ͋���) */
			br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)�W���e */
		//����br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)�W���e */
			br.BULLET_REGIST_06_n_way					= (8);								/* [8way] */			/* 8�����e */
			br.BULLET_REGIST_07_div_angle65536			= (65536/8);						/* �����p�x(�e�����Ȃ��͈͂�����32����) */	/* (ra_nd16>>4) == ����(0-4095) */
			//
			/* 0 [...��] 150 [...��+��] 180 [...��] 255 */
			if (0==((src->boss_base_spell_time_out)&1))
			{
				if ((128+((cg_game_difficulty)<<2))>(src->boss_base_spell_time_out))
				{
					br.BULLET_REGIST_01_speed_offset			= t256(2);					/* ���������e */	/* ���̕����ɂȂ邩������ */
					br.BULLET_REGIST_02_angle65536				= (jjj);					/* ���� */
					br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KOME_01_AKA;		/* �ԕĒe */
					tama_system_regist_katayori_n_way();/* (r33-) */
				}
			}
			else
			{
				if ((128-((cg_game_difficulty)<<2))<(src->boss_base_spell_time_out))
				{
					br.BULLET_REGIST_01_speed_offset			= t256(1);					/* ���������e */	/* ���̕����ɂȂ邩������ */
					br.BULLET_REGIST_02_angle65536				= (65536-jjj); 				/* ���� */
					br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KOME_03_AOI;		/* �Ēe */
					tama_system_regist_katayori_n_way();/* (r33-) */
				}
			}
		}
	}

}
