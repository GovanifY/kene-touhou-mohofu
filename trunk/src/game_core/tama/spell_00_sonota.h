
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���̑��̃J�[�h���`���܂��B
---------------------------------------------------------*/

/*---------------------------------------------------------
	�������̗d��1(�V��l?)�����J�[�h
	-------------------------------------------------------
	28�x�� �� 100��e �� 7���� BULLET_UROKO14_05_MIDORI		speed256	= t256(3.25)+((difficulty)<<6);
	28�x�� �� 100�����e �� 8���� BULLET_UROKO14_04_MIZUIRO		speed256	= (t256(3.0)+((difficulty)<<6));
	-------------------------------------------------------
	�g�p���W�X�^
	REG_08_REG0 	�J�E���^�B	(�U��wait�p)	  int YOKAI1_DATA_wait2 = 0;	//(10-1)
	REG_09_REG1 	�J�E���^�B(�F�l�p)
	REG_0a_REG2 	�F�l(0��1)
---------------------------------------------------------*/
	//	REG_08_REG0--;
	//	if (0 > REG_08_REG0)
			/* 0: 20 == 20-(0*4) */
			/* 1: 16 == 20-(1*4) */
			/* 2: 12 == 20-(2*4) */
			/* 3:  8 == 20-(3*4) */
		//	REG_08_REG0 = ((20-1)-(((REG_0f_GAME_DIFFICULTY))<<2));/*(8-1)*/ /*(10-1)*/

local OBJ_CALL_FUNC(spell_create_11_tengu_shot)
{
	#if 0/*(�Ȃ������܂������Ȃ�???)*/
	count_up_limit_NUM(REG_NUM_09_REG1, 128);// �J�E���^�B(�F�l�p)
	if (1==REG_09_REG1) 	/* 1���( 0 �ɏ���������āA�J�E���g�A�b�v���� 1 �ɂȂ�̂ŁB) */
	{
		count_up_limit_NUM(REG_NUM_0a_REG2, 2);// (�F�l)
	}
	#else
	if (128 > ((REG_10_BOSS_SPELL_TIMER)))
			{	REG_0a_REG2 = (1);	}/* �����e �� 8���� */
	else	{	REG_0a_REG2 = (0);	}/* ��e �� 7���� */
	#endif
	/* ���߂� 28[frame]�͂��x�� */
	if (100 > ((REG_10_BOSS_SPELL_TIMER)&0x7f))
	{
		count_up_limit_NUM(REG_NUM_08_REG0, ((20-1)-(((REG_0f_GAME_DIFFICULTY))<<2)));//	if (0==((REG_10_BOSS_SPELL_TIMER)&0x03))
		if (0==(REG_08_REG0))
		{
			/*(�{�X�X�y�J�̏ꍇ�̂ݏȗ��\�A���{�X�̏ꍇ�ȗ��ł��Ȃ�)*/set_REG_DEST_XY(src);	/* �e��x256 y256 ���S���甭�e�B */
			calculate_jikinerai();
			//
		//	HATSUDAN_01_speed256			= (t256(3.0)+(((REG_0f_GAME_DIFFICULTY))<<6))+(REG_0a_REG2<<(8-2));
			HATSUDAN_01_speed256			= (t256(2.0)+(((REG_0f_GAME_DIFFICULTY))<<6))+(REG_0a_REG2<<(8-2));
			HATSUDAN_02_speed_offset		= t256(3/*6*/);/*(�e�X�g)*/
		//	HATSUDAN_03_angle65536			= ((HATSUDAN_03_angle65536));
		//	HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_3000_EFFECT_NONE)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
			HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_1000_EFFECT_MIDDLE)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
			HATSUDAN_05_bullet_obj_type 	= (BULLET_UROKO14_BASE + TAMA_IRO_04_MIZU_IRO) + (REG_0a_REG2);
			HATSUDAN_06_n_way				= (7)+(REG_0a_REG2);
			HATSUDAN_07_div_angle65536		= (int)(65536/24);
			hatudan_system_regist_n_way();/* (r33-) */
			AUDIO_18_voice_number	= VOICE15_BOSS_KOUGEKI_01;
			AUDIO_19_voice_truck	= TRACK04_TEKIDAN;/* �e�L�g�[ */
			cpu_voice_play();
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

/* [CW ���v���] */
local OBJ_CALL_FUNC(spell_create_20_sonota_debug_cw_ao/*CW*/)
{
	if (0==((REG_10_BOSS_SPELL_TIMER)&0x03))
	{
		src->tmp_angleCCW65536 -= (int)(65536/24);
		#if 0
		src->tmp_angleCCW65536 += (int)(ra_nd() & ((1<<difficulty)-1) );
		#else
		{static const u8 masked_tbl[4] = { 0x00, 0x0f, 0x3f, 0xff };
			src->tmp_angleCCW65536 += (int)(ra_nd() & (masked_tbl[(REG_0f_GAME_DIFFICULTY)]) );
		}
		#endif
		mask65536(src->tmp_angleCCW65536);
		//
			HATSUDAN_01_speed256				= (t256(1.0));	/* �e�� */
			HATSUDAN_02_speed_offset			= t256(1);/*(�e�X�g)*/
			HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
			HATSUDAN_05_bullet_obj_type 		= (BULLET_WAKU12_BASE + TAMA_IRO_03_AOI);				/* [�g�t���ےe] */
			HATSUDAN_06_n_way					= (6);									/* [6way] */
			HATSUDAN_07_div_angle65536			= (int)(65536/48);						/* �����p�x(1024/(6*8) ) �P����8���������̈�������6���������p�x */
		{
			HATSUDAN_03_angle65536				= (src->tmp_angleCCW65536); 							/* �p�x */
			hatudan_system_regist_katayori_n_way();/* (r33-) */
		//
			HATSUDAN_03_angle65536				= ((src->tmp_angleCCW65536+(65536/2))&(65536-1));		/* �p�x */
			hatudan_system_regist_katayori_n_way();/* (r33-) */
		}
		#if (1)
		AUDIO_18_voice_number	= VOICE15_BOSS_KOUGEKI_01;
	//	AUDIO_19_voice_truck	= TRACK04_TEKIDAN;
		cpu_bullet_play_05_auto();
		#endif
	}
}

/*[CCW �����v���]*/
local OBJ_CALL_FUNC(spell_create_21_sonota_debug_ccw_aka/*CCW*/)
{
	if (0==((REG_10_BOSS_SPELL_TIMER)&0x03))
	{
		src->tmp_angleCCW65536 += (int)(65536/24);
		#if 0
		src->tmp_angleCCW65536 -= (int)(ra_nd() & ((1<<difficulty)-1) );
		#else
		{static const u8 masked_tbl[4] = { 0x00, 0x0f, 0x3f, 0xff };
			src->tmp_angleCCW65536 -= (int)(ra_nd() & (masked_tbl[(REG_0f_GAME_DIFFICULTY)]) );
		}
		#endif
		mask65536(src->tmp_angleCCW65536);
		//
			HATSUDAN_01_speed256				= (t256(1.0));	/* �e�� */
			HATSUDAN_02_speed_offset			= t256(1);/*(�e�X�g)*/
			HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
			HATSUDAN_05_bullet_obj_type 		= (BULLET_WAKU12_BASE + TAMA_IRO_01_AKA);				/* [�g�t���Ԋےe] */
			HATSUDAN_06_n_way					= (6);									/* [6way] */
			HATSUDAN_07_div_angle65536			= (int)(65536/48);						/* �����p�x(1024/(6*8) ) �P����8���������̈�������6���������p�x */
		{
			HATSUDAN_03_angle65536				= (src->tmp_angleCCW65536); 							/* �p�x */
			hatudan_system_regist_katayori_n_way();/* (r33-) */
		//
			HATSUDAN_03_angle65536				= ((src->tmp_angleCCW65536+(65536/2))&(65536-1));		/* �p�x */
			hatudan_system_regist_katayori_n_way();/* (r33-) */
		}
		#if (1)
		AUDIO_18_voice_number	= VOICE15_BOSS_KOUGEKI_01;
	//	AUDIO_19_voice_truck	= TRACK04_TEKIDAN;
		cpu_bullet_play_05_auto();
		#endif
	}
}

/*---------------------------------------------------------
	�q�������w �J�[�h
	-------------------------------------------------------
	B.���^�`����
	-------------------------------------------------------
	(���ʂɕ��G�������̂ŁA
		A.���[�U�[�����@�\�ƁA
		B.���^��`���@�\�ƁA
		C.360�i�C�t�����@�\�ƁA
	�𕪗�������)
---------------------------------------------------------*/

#define REG_000_my_angle1024				REG_09_REG1 	/* �ێ��p�x[���^��`���ꍇ�Ɏg���p�x] */
#define REG_001_hosi_set_position_cx256 	REG_0a_REG2 	/* ���^�`���Ă���W�ʒu��ێ��B */
#define REG_002_hosi_set_position_cy256 	REG_0b_REG3 	/* ���^�`���Ă���W�ʒu��ێ��B */
#define REG_003_hosi_vx256					REG_0c_REG4 	/* ���^�̍����ړ��x�N�g���ʁBvx256 */
#define REG_004_hosi_vy256					REG_0d_REG5 	/* ���^�̍����ړ��x�N�g���ʁBvy256 */

#define HGT_giji_jyumyou	base_time_out

/* ���p�e */
#define HGT_DATA_angle1024			user_data03
#define HGT_DATA_speed256			user_data04 		/* ���x */
#define HGT_DATA_add_delta256		user_data05 		/* ���Z�A�����x */

#define HOSIGATA_DAN_LIMIT_01_512	(512+0)
static OBJ_CALL_FUNC(move_bullet_sakuya_hosi_gata)
{
	src->HGT_giji_jyumyou--;/* ���Ԍo�� */
	if (HOSIGATA_DAN_LIMIT_01_512 > (src->HGT_giji_jyumyou))/* �\�ߐݒ肳�ꂽ���Ԃœ��� */
	{
		src->HGT_giji_jyumyou = HOSIGATA_DAN_LIMIT_01_512;
		src->HGT_DATA_speed256 += (src->HGT_DATA_add_delta256); //(4) (3.84) == t256(0.015);
		//------------------
		HATSUDAN_01_speed256	= (src->HGT_DATA_speed256);
		HATSUDAN_03_angle65536	= deg1024to65536((((src->HGT_DATA_angle1024))));
		sincos256();/*(�j�󃌃W�X�^�����̂Œ���)*/
		src->math_vector.x256 = REG_03_DEST_Y;//sin_value_t256 // ��CCW�̏ꍇ(�ECW�̏ꍇ�Ƃ�xy���t)
		src->math_vector.y256 = REG_02_DEST_X;//cos_value_t256 // ��CCW�̏ꍇ(�ECW�̏ꍇ�Ƃ�xy���t)
		//------------------
	}
	//move_bullet_vector(src);
	src->center.x256 += (src->math_vector.x256); 	/*fps_factor*/
	src->center.y256 += (src->math_vector.y256); 	/*fps_factor*/
//	gamen_gai_nara_osimai(src);/* ��ʊO�Ȃ炨���܂� */
	hatudan_system_B_gamen_gai_tama_kesu(src);/*(��ʊO�e����)*/
}
		#if (0)//
		src->math_vector.x256 = ((si n1024((src->HGT_DATA_angle1024))*(src->HGT_DATA_speed256))>>8);/*fps_factor*/ 	/* CCW�̏ꍇ */
		src->math_vector.y256 = ((co s1024((src->HGT_DATA_angle1024))*(src->HGT_DATA_speed256))>>8);/*fps_factor*/
		//#el se
		{
			int sin_value_t256; 	//	sin_value_t256 = 0;
			int cos_value_t256; 	//	cos_value_t256 = 0;
			int256_si nco s1024( (src->HGT_DATA_angle1024), &sin_value_t256, &cos_value_t256);
			src->math_vector.x256 = ((sin_value_t256*(src->HGT_DATA_speed256))>>8);/*fps_factor*/
			src->math_vector.y256 = ((cos_value_t256*(src->HGT_DATA_speed256))>>8);/*fps_factor*/
		}
		#endif

/*
	-------------------------------------------------------
	ToDo:
	�e�V�X�e���Ɉڍs����ׁA�p�~�\��B
	�J�[�h�X�N���v�g�V�X�e�����ғ�����ׂɂ́A���e���ꃖ���ɓ������Ȃ���
	���x�I�Ȗʂł��̂܂܂̌`�Ԃł͖���������̂ŁA
	(�����̔��e�����̂����ő��̒e��������������)
	�����̔��e������ hatudan_system_regist_single �ɒu��������K�v������B
---------------------------------------------------------*/



/*---------------------------------------------------------
	����		B.���^��`�������w�p �e
	-------------------------------------------------------
	�e��S�e�������������Ɉڍs����̂ł����͖����Ȃ�\��B
	-------------------------------------------------------
	Todo:
	src->giji_jyumyou ��p�~���� src->jyumyou �Œu��������B
---------------------------------------------------------*/

local OBJ_CALL_FUNC(spell_init_27_hosigata_test)
{
	spell_init_mima_kaguya(src);/*(����)*/	/*(DANMAKU01�ɉJ�p�����蓖�Ă�)*/
		/* �e�������n�߂�ʒu(���^�`���n�߂�ʒu) */
		REG_001_hosi_set_position_cx256 	= (src->center.x256)+(ra_nd()&0xfff);
		REG_002_hosi_set_position_cy256 	= (src->center.y256)+(ra_nd()&0xfff);/* ��� ��������`�� */

		/* ���^��`������ */
		REG_000_my_angle1024			= (0);
	//	REG_003_hosi_vx256				= t256(1.0);	/*-t256(0.125)*/ /*t256(1.0)*/
	//	REG_004_hosi_vy256				= t256(0.0);	/* t256(2.0)*/ /*t256(0.0)*/
}


/*---------------------------------------------------------
	�q�������w �ړ�
---------------------------------------------------------*/
		/* �q�������w�A�z�u�ʒu */
		#if 0
		h->center.x256					= (src->center.x256);
		h->center.y256					= (src->center.y256)-t256(16);/*��� ����ɔz�u*/
		#endif
	//
//		h->boss_time_out			= ((64*8)+(5*16)+1);	/* �������� */

local OBJ_CALL_FUNC(spell_create_27_hosigata_test)
{
//	if ((64*8) < src->boss_time_out)
	{
		/* ���^��`���� */
		if (0==((REG_10_BOSS_SPELL_TIMER)&0x0f))
		{
			REG_000_my_angle1024 += (1024*2/5);
			mask1024(REG_000_my_angle1024);
			#define HGT_DOLL_SPEED10		(4)/*8*/
			#if (0)//
			REG_003_hosi_vx256 = ((si n1024((REG_000_my_angle1024))*(HGT_DOLL_SPEED10)));/*fps_factor*/ /* CCW�̏ꍇ */
			REG_004_hosi_vy256 = ((co s1024((REG_000_my_angle1024))*(HGT_DOLL_SPEED10)));/*fps_factor*/
			#else
			{
				int sin_value_t256; 		//	sin_value_t256 = 0;
				int cos_value_t256; 		//	cos_value_t256 = 0;
				int256_sincos1024( (REG_000_my_angle1024), &sin_value_t256, &cos_value_t256);
				REG_003_hosi_vx256 = ((sin_value_t256*(HGT_DOLL_SPEED10)));/*fps_factor*/
				REG_004_hosi_vy256 = ((cos_value_t256*(HGT_DOLL_SPEED10)));/*fps_factor*/
			}
			#endif
			#undef HGT_DOLL_SPEED10
		}
		else
		{
/*
0123 3333 4444	0+4
4567 7777 5555	1+4
89ab bbbb 6666	2+4
cdef ffff 7777	3+4
*/
			/* CCW�̏ꍇ */
		//	bullet_crate_sakuya_hosi_gata(/*&hosi_position_obj*/);
		//	static void bullet_crate_sakuya_hosi_gata(void/*OBJ *src*/)
			{		/*�ےe�W(�Ԃq�f�a�Ύቩ��)*/
				OBJ *h;
				h									= obj_regist_tama();
				if (NULL != h)
				{
					h->center.x256					= REG_001_hosi_set_position_cx256;
					h->center.y256					= REG_002_hosi_set_position_cy256;
					h->obj_type_set 				= (BULLET_MINI8_BASE + TAMA_IRO_01_AKA);
					reflect_sprite_spec(h, OBJ_BANK_SIZE_00_TAMA);
					h->m_Hit256R					= TAMA_ATARI_MARU16_PNG;
				//
					h->callback_mover				= move_bullet_sakuya_hosi_gata;
					h->hatudan_register_speed65536	= (0);
					h->hatudan_register_tra65536	= (0);
					h->hatudan_register_spec_data	= (DANMAKU_LAYER_03)|(TAMA_SPEC_3000_EFFECT_NONE)|(TAMA_SPEC_8000_NON_TILT);
					h->math_vector.x256				= (0);
					h->math_vector.y256				= (0);
					/* hosi_gata_time_out */
					h->HGT_giji_jyumyou 			= HOSIGATA_DAN_LIMIT_01_512 + ((REG_10_BOSS_SPELL_TIMER)&0x1ff); //((REG_10_BOSS_SPELL_TIMER)-(64*8))
					/* hosi_gata_angle1024 */
					h->HGT_DATA_angle1024			= ((REG_000_my_angle1024))+(((REG_10_BOSS_SPELL_TIMER)&0x0f)<<4/*3*/);
					h->HGT_DATA_speed256			= t256(0);
					/* hosi_gata_add_speed256 */	//(((REG_10_BOSS_SPELL_TIMER)&0xf)|0x03);	/* �������x(��-��������) */
					h->HGT_DATA_add_delta256		= (((REG_10_BOSS_SPELL_TIMER>>2)&0x3)+4);	/* �������x(��-��������) */
				}
			}
		}
		// ����
		REG_001_hosi_set_position_cx256 += (REG_003_hosi_vx256);/*fps_factor*/
		REG_002_hosi_set_position_cy256 += (REG_004_hosi_vy256);/*fps_factor*/
	}
//
}
