
/*---------------------------------------------------------
 �����͕핗 �` Toho Imitation Style.
  �v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���[�~�A�̒e���𐶐����܂��B
---------------------------------------------------------*/

/*---------------------------------------------------------
	�g1�ʃ{�X ���[�~�A �镄�u�i�C�g�o�[�h�v�ɂ�����Ƃ��������e��(�\��)
	-------------------------------------------------------
	5�e�ڂ����@�_���� 32����16���� �|��e�B
	-------------------------------------------------------
---a aaaa [aaa]
0000 xxxx 16�� ���� 		�� (0x00==((time_out)&0x20))
0001 0000 ���@�_���p�쐬	�� (0x00==((time_out)&0x20))
0010 xxxx 16�� ���� 		��
0011 0000 ���@�_���p�쐬	��
0100 xxxx 16�� ���� 		�� (0x00==((time_out)&0x20))
0101 0000 ���@�_���p�쐬	�� (0x00==((time_out)&0x20))
0110 xxxx 16�� ���� 		��
0111 0000 ���@�_���p�쐬	��
//
1000 0000 start
---------------------------------------------------------*/
static void spell_create_08_rumia_night_bird(SPRITE *src)
{
	static int aaa_angle65536;
	if (0x10==((src->boss_spell_timer)&0x1f))/* ���@�_���p�쐬 */
	{
		#if 1
		tmp_angleCCW65536_jiki_nerai(src);/* ���@�_���p�쐬 */
		aaa_angle65536 = src->tmp_angleCCW65536;/* ���@�_���p */
		#endif
		//	2048 == (65536)/(32)		: 1����32���������p�x�B
		//	8192 == (2048) * (4)		: 5�e��(4�e���̕�)�B
		// 57344 == (65536) - (8192)	: �J�n�p�x�B(���Z���Ȃ��ƌ덷���o��B)
	//	aaa_angle65536 += (const int)(65536-(int)((65536*4)/(32)));/* 5�e�ڂ����@�_�� */
		if (0x00==((src->boss_spell_timer)&0x20))
				{	aaa_angle65536 += (57344);		}/* 5�e��(4�e���̕�)�����@�_��(65536-(4*(65536/32))) */
		else	{	aaa_angle65536 += (8192);		}/* 5�e��(4�e���̕�)�����@�_��(65536+(4*(65536/32))) */
		mask65536(aaa_angle65536);
		/* �p�x(65536[360/360�x]�� 32����) */
	}
	else
	if (0x00==((src->boss_spell_timer)&0x10))/* 16�� ���� */
	{
		bullet_play_04_auto(VOICE14_SAKUYA_SHOT02);/*VOICE15_BOSS_KOUGEKI_01*/
		{
			int tama_color;/* ���� */
			int iii;
				iii = ((src->boss_spell_timer)&0x0f);
			#if 0/* �f�o�b�O(���@�_���e�̐F��ς���) */
			if (0x0b==iii)/* 0x0b==5�e�� (0x0f��1�e�ڂ�����0x0b��5�e��) */
			{
				tama_color = -3;/* �g���Ԓe  �F�ς���B */
			}
			else
			#endif
			#if 1
			{
				if (0x00==((src->boss_spell_timer)&0x20))
						{	tama_color = -1;/* �g���e */ }
				else	{	tama_color =  1;/* �g���Βe */ }
			}
			#endif
		//	br.BULLET_REGIST_00_speed256				= (t256(1.5));								/* �e�� */
			br.BULLET_REGIST_00_speed256				= (t256(0.75))+((16-(iii))<<5); 			/* �e�� */
			/* �e�� 0.75 - 2.75 �{ */
			br.BULLET_REGIST_01_speed_offset			= t256(0);/*(�e�X�g)*/
			br.BULLET_REGIST_02_angle65536				= (aaa_angle65536); 	/* ���˒��S�p�x / ����@�\(���@�_��/��) */
			br.BULLET_REGIST_03_tama_data				= (TAMA_DATA_8000_NON_TILT);/* (r33-)��X���e */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_WAKU12_03_AOI+1+tama_color;			/* [�g�t���ےe] [�g�t���Ίےe] */
			tama_system_regist_single();/* (r33-) */
		}
		/* ���̒e�̊p�x */
		//	 2048 == (65536)/(32)		: 1����32���������p�x�B
		//	63488 == (65536) - (2048)	: ���Z�̏ꍇ(���Z���Ȃ��ƌ덷���o��B)
		if (0x00==((src->boss_spell_timer)&0x20))
				{	aaa_angle65536 += (2048);		}/* �p�x(65536[360/360�x]�� 32����) (int)(65536/(32)); */
		else	{	aaa_angle65536 += (63488);		}/* �p�x(65536[360/360�x]�� 32����) (65536)-(int)(65536/(32)); */
		mask65536(aaa_angle65536);
	}
}


/*---------------------------------------------------------
	�g1�ʃ{�X ���[�~�A �ŕ��u�f�B�}�[�P�C�V�����v�ɂ�����Ƃ��������e��(�\��)
	-------------------------------------------------------
	���[�~�A�����e:
	�����e�ɂ��ǂ�����Ǝ�ނ�����悤�ȋC�����Ă����B
	�ŕ��u�f�B�}�[�P�C�V�����v�̌����e�́B
		1. ��苗���ɔ�яo���B(�ʏ팸���e)
		2. �e��~�B
		3. �ˑR (�e�O����)90�x��]����B
		4.�e�O�����X���Ȃ���e���L����B
	-------------------------------------------------------
	16
	-------------------------------------------------------
	0010 01--	0x027f == 64*10-1
	0010 00--	0x023f == 64* 9-1
				...
	0000 00--	0x003f == 64* 1-1
	-------------------------------------------------------
640==0x280
512==0x200
	32*0		�T
	32*1		�U
	32*2		�R
	32*3		�S
	32*4		�P
	32*5		�Q
	32*6		�x��
	32*7		�x��
	32*8		�x��
	32*9			��
	32*10		�x��
	32*11			��
	32*12		�x��
	32*13			��
	32*14		�x��
	32*15		�x��

---------------------------------------------------------*/

static void s_check_aaa_kome_dan(SPRITE *src)/* �����e */
{
	if (0==(src->flags&SP_FLAG_COLISION_CHECK))/* �����蔻��̖����e�͏����Ȃ��B(���e�G�t�F�N�g�p) */
	{
		return;
	}
	//
	/* 0-32 �J�E���g�܂� */
	if ((JYUMYOU_1MIN-64-32) < src->jyumyou)/* ���e�G�t�F�N�g�ォ��0�J�E���g�o�߂����e */
	{
		;/* ���e�����͉������Ȃ� */
	}
	else
	/* 32-64 �J�E���g�܂� */
	if ((JYUMYOU_1MIN-64-64) < src->jyumyou)/* ���e�G�t�F�N�g�ォ��32�J�E���g�o�߂����e */
	{
		//src->rotationCCW1024 += (256);/* 90/360 �x �Ȃ��Ă݂�e�X�g */
		if ((src->tama_system_tama_data) & TAMA_DATA_RUMIA_SAYUU_BIT)
		{
		//	src->rotationCCW1024 += (8);/* 90/360 �x �Ȃ��Ă݂�e�X�g(32�J�E���g�����ċȂ���B256==8*32) */
			src->rotationCCW1024 += (10);/* 90/360 �x �Ȃ��Ă݂�e�X�g(32�J�E���g�����ċȂ���B256==8*32) */
		}
		else
		{
		//	src->rotationCCW1024 -= (8);/* 90/360 �x �Ȃ��Ă݂�e�X�g(32�J�E���g�����ċȂ���B256==8*32) */
			src->rotationCCW1024 -= (10);/* 90/360 �x �Ȃ��Ă݂�e�X�g(32�J�E���g�����ċȂ���B256==8*32) */
		}
	}
	else
	/* 64-256 �J�E���g�܂� */
	if ((JYUMYOU_1MIN-64-256) < src->jyumyou)/* ���e�G�t�F�N�g�ォ��64�J�E���g�o�߂����e */
	{
		if ((src->tama_system_tama_data) & TAMA_DATA_RUMIA_SAYUU_BIT)
		{
			src->rotationCCW1024++;/* �Ȃ��Ă݂�e�X�g */
		}
		else
		{
			src->rotationCCW1024--;/* �Ȃ��Ă݂�e�X�g */
		}
	}
	#if 0
	/* 256- �J�E���g�܂� */
//	else
	{
		;/* ���ȏ㎞�Ԃ���������A�������Ȃ� */
	}
	#endif
}

static void s_check_bbb_maru_dan(SPRITE *src)/* �A�e */
{
	if (0==(src->flags&SP_FLAG_COLISION_CHECK))/* �����蔻��̖����e�͏����Ȃ��B(���e�G�t�F�N�g�p) */
	{
		return;
	}
	/* 32 �J�E���g�Ȃ� */
	if ((JYUMYOU_1MIN-64-32) == src->jyumyou)/* ���e�G�t�F�N�g�ォ��32�J�E���g�o�߂����e */
	{
		if ((src->tama_system_tama_data) & TAMA_DATA_RUMIA_DIMMER_RENDAN_BIT)
		{
			src->tama_system_tama_data = (TAMA_DATA_8000_NON_TILT);/* (r33-)��X���e */
			tmp_angleCCW65536_jiki_nerai(src);/* ���@�_���p�쐬 */
			#if 0
			/*(�_��)*/
		//	src->rotationCCW1024 = (0);//�`��p�p�x((src->tmp_angleCCW65536)>>6);/* ���@�_���p */
			#else
			/*(���ʃP�[�X�A���̂܂�(������)�̊p�x�ňړ�����)*/
			src->rotationCCW1024 = ((src->tmp_angleCCW65536)>>6);/* ���@�_���p */
			/* �����̕`��p�p�x�͌�ŗ��p����B */
			#endif
			src->tama_system_speed65536 			= (t256(2.0)<<8);			/* �e�� */
		}
	}
}

static void dimmer_tama_check(void)
{
	int ii;
	for (ii=0; ii<OBJ_POOL_00_TAMA_MAX; ii++ )/* �S�����ׂ�B */
	{
		SPRITE *s;
		s = &obj99[OBJ_HEAD_00_TAMA+ii];
		if (is_tama_grouip08(BULLET_KOME_00_SIRO) == is_tama_grouip08(s->type) )	/* �Ēe�Ȃ� */
		{
			s_check_aaa_kome_dan(s);
		}
		else
		if (is_tama_grouip08(BULLET_WAKU12_00_SIRO) == is_tama_grouip08(s->type) )	/* �g�t���ےe�Ȃ� */
		{
			s_check_bbb_maru_dan(s);
		}
	}
}
			#if 0
			unsigned int ttt[4]=
			{
				BULLET_KOME_03_AOI, 	/* �e */
				BULLET_KOME_05_MIDORI,	/* �Βe */
				BULLET_KOME_01_AKA, 	/* �Ԓe */
				0
			};
			tama_color = ttt[tama_index];
			#endif


/*---------------------------------------------------------
	���@�_���A�e
---------------------------------------------------------*/

static void dimmer_shot_02_rendan(SPRITE *src)
{
	unsigned int ii;
	for (ii=0; ii<8; ii++)
	{
		int aaa_angle65536;
		#if 1
		tmp_angleCCW65536_jiki_nerai(src);/* ���@�_���p�쐬 */
		aaa_angle65536 = src->tmp_angleCCW65536;/* ���@�_���p */
		#endif
		//	2048 == (65536)/(32)		: 1����32���������p�x�B
		//	8192 == (2048) * (4)		: 5�e��(4�e���̕�)�B
		// 57344 == (65536) - (8192)	: �J�n�p�x�B(���Z���Ȃ��ƌ덷���o��B)
	//	aaa_angle65536 += (const int)(65536-(int)((65536*4)/(32)));/* 5�e�ڂ����@�_�� */
		if (0x00==((src->boss_spell_timer)&0x20))
				{	aaa_angle65536 += (57344);		}/* 5�e��(4�e���̕�)�����@�_��(65536-(4*(65536/32))) */
		else	{	aaa_angle65536 += (8192);		}/* 5�e��(4�e���̕�)�����@�_��(65536+(4*(65536/32))) */
		mask65536(aaa_angle65536);
		/* �p�x(65536[360/360�x]�� 32����) */
		unsigned int jj;
		for (jj=0; jj<16; jj++)
		{
		//	br.BULLET_REGIST_00_speed256				= (t256(1.5));								/* �e�� */
			br.BULLET_REGIST_00_speed256				= (t256(0.75))+((16-(jj))<<5);				/* �e�� */
			/* �e�� 0.75 - 2.75 �{ */
			br.BULLET_REGIST_01_speed_offset			= (ii<<6);//t256(0);/*(�e�X�g)*/
			br.BULLET_REGIST_02_angle65536				= (aaa_angle65536); 	/* ���˒��S�p�x / ����@�\(���@�_��/��) */
			br.BULLET_REGIST_03_tama_data				= (TAMA_DATA_RUMIA_DIMMER_RENDAN_BIT)|(TAMA_DATA_8000_NON_TILT);/* (r33-)��X���e */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_WAKU12_03_AOI; 			/* [�g�t���ےe] [�g�t���Ίےe] */
			tama_system_regist_single();/* (r33-) */
			/* ���̒e�̊p�x */
			//	 2048 == (65536)/(32)		: 1����32���������p�x�B
			//	63488 == (65536) - (2048)	: ���Z�̏ꍇ(���Z���Ȃ��ƌ덷���o��B)
			if (0x00==((src->boss_spell_timer)&0x20))
					{	aaa_angle65536 += (2048);		}/* �p�x(65536[360/360�x]�� 32����) (int)(65536/(32)); */
			else	{	aaa_angle65536 += (63488);		}/* �p�x(65536[360/360�x]�� 32����) (65536)-(int)(65536/(32)); */
			mask65536(aaa_angle65536);
		}
	}
}


/*---------------------------------------------------------
	�����e�B �e / �Βe / �Ԓe
---------------------------------------------------------*/

static void dimmer_shot_01_kousadan(SPRITE *src, u8 tama_type)
		{
			int tama_kakudo = (((tama_type)&0x06)<<8);/* �e�F�ʂɔ��e�p��ς���B(((tama_type)&0x06)<<6) */
			br.BULLET_REGIST_00_speed256				= (t256(1.0));			/* �e�� (t256(1.5)) */
			br.BULLET_REGIST_01_speed_offset			= t256(0);/*(�e�X�g)*/
			br.BULLET_REGIST_02_angle65536				= (tama_kakudo);					/* ���˒��S�p�x / ����@�\(���@�_��/��) */
		//	br.BULLET_REGIST_03_tama_data				= (TAMA_DATA_RUMIA_SAYUU_BIT);/* ���� */
			br.BULLET_REGIST_03_tama_data				= /* ���� */(TAMA_DATA_RUMIA_SAYUU_BIT)|(TAMA_DATA_0000_TILT);/* (r33-)�W���e */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KOME_00_SIRO+tama_type;			/* [�g���e] [�g���Βe] */
			br.BULLET_REGIST_06_n_way					= (32); 				/* [32way] */	/* (16)[16way] */
			br.BULLET_REGIST_07_div_angle65536			= (65536/32);			/* �����p�x (65536/16) */
			tama_system_regist_katayori_n_way();/*(r33-)*/
			/* br.BULLET_REGIST_02_angle65536��tama_system_regist_katayori_n_way();�Ŕj�󂳂��(�d�l��)�̂ōĐݒ肪�K�v�B */
			br.BULLET_REGIST_02_angle65536				= (tama_kakudo);					/* ���˒��S�p�x / ����@�\(���@�_��/��) */
		//	br.BULLET_REGIST_03_tama_data				= (0);/* �߂�(�K���߂����B�߂��Y���ƃV�X�e�����̐��퓮�삵�Ȃ�) */
			br.BULLET_REGIST_03_tama_data				= (TAMA_DATA_0000_TILT);/* (r33-)�W���e */
			tama_system_regist_katayori_n_way();
		}


/*---------------------------------------------------------

---------------------------------------------------------*/

static void spell_create_29_rumia_demarcation(SPRITE *src)
{
	dimmer_tama_check();/* �f�B�}�[�P�C�V�����p�e�`�F�b�N */
	u8 tama_type;/* �e�F */
	tama_type = (0);
	if ((0x1f)==((src->boss_spell_timer)&0x1f))
	{
		#define P0x80		0x80
		#define YASUMI000	0x00
		const u8 ttt[16] =
		{
		/*0x00*/	P0x80,	/* ���@�_���A�e */
		/*0x01*/	P0x80,	/* ���@�_���A�e */
		/*0x02*/	P0x80,	/* ���@�_���A�e */
		/*0x03*/	P0x80,	/* ���@�_���A�e */
		/*0x04*/	P0x80,	/* ���@�_���A�e */
		/*0x05*/	P0x80,	/* ���@�_���A�e */
		/*0x06*/	YASUMI000,	/* �x�� */
		/*0x07*/	YASUMI000,	/* �x�� */
		/*0x08*/	YASUMI000,	/* �x�� */
		/*0x09*/	(BULLET_KOME_01_AKA    - BULLET_KOME_00_SIRO),		/* �Ԓe */
		/*0x0a*/	YASUMI000,	/* �x�� */
		/*0x0b*/	(BULLET_KOME_05_MIDORI - BULLET_KOME_00_SIRO),		/* �Βe */
		/*0x0c*/	YASUMI000,	/* �x�� */
		/*0x0d*/	(BULLET_KOME_03_AOI    - BULLET_KOME_00_SIRO),		/* �e */
		/*0x0e*/	YASUMI000,	/* �x�� */
		/*0x0f*/	YASUMI000	/* �x�� */
		};
		tama_type = ttt[(((src->boss_spell_timer)>>5)&0x0f)];
	}
	if ((0) == tama_type)	/* ���x�� */
	{
		;
	}
	else
	if (P0x80 == tama_type) 	/* ���@�_���A�e */
	{
		bullet_play_04_auto(VOICE14_SAKUYA_SHOT02);/*VOICE15_BOSS_KOUGEKI_01*/
		dimmer_shot_02_rendan(src);
	}
	else		/* �����e�B �e / �Βe / �Ԓe */
	{
		bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
		dimmer_shot_01_kousadan(src, tama_type);
	}
}
