
/*---------------------------------------------------------
 �����͕핗  �` Toho Imitation Style.
  �v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���̒e���𐶐����܂��B
---------------------------------------------------------*/


/*---------------------------------------------------------
	�g5�ʒ��{�X ��� ���U���ɂ�����Ƃ��������e��
	-------------------------------------------------------
	�{�Ƃ͌Œ�e��(?)�����ǁA����͑_���n�B
	(���ɌŒ�e���������Ƃ��Ă�)����������ɍs���Ȃ�����A������͏o���Ȃ��B
	(psp�̏c�𑜓x�ł͏�ɍs���X�y�[�X���Ȃ�)
	��������psp�̏c�𑜓x���S�R����Ȃ�����A�{�ƕ��̃`���������͖����B
	������x���E�ɓ�������Ĕ����ĖႤ����z�肵�Ă���B(������e���x��)
	����ɕ����Ė����e�͂����Ă��񂺂�Ⴄ�B(�ʕϐ��Ƃ�̂��ʓ|��������������)
---------------------------------------------------------*/

static void danmaku_create_01_sakuya_misogi(SPRITE *src)
{
	static int aaa_angle65536;
//
	if (0==((src->boss_base_danmaku_time_out)&0x07))
	{
		#if (1)
	//	voice_play(VOICE13_SAKUYA_SHOT02, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE13_SAKUYA_SHOT02);
		#endif
	//	src->tmp_angleCCW1024 -= (int)(256/5)+(10); /*CCW*/
	//	src->tmp_angleCCW1024 -= (int)(1024/20)+(10); /*CCW*/
		src->tmp_angleCCW65536 -= (int)(65536/20)+(640); /*CCW*/
		mask65536(src->tmp_angleCCW65536);
		obj_send1->x256 					= (src->x256);						/* �e��x256 */
		obj_send1->y256 					= (src->y256);						/* �e��y256 */
		br.BULLET_REGIST_speed256			= (t256(2.5));						/* �e�� */
		br.BULLET_REGIST_angle65536 		= ((src->tmp_angleCCW65536));		/* �e���p�x65536 */ 		/* [������e] */
	//	br.BULLET_REGIST_div_angle1024		= (int)(256/5);
	//	br.BULLET_REGIST_div_angle1024		= (int)(1024/20);
		br.BULLET_REGIST_div_angle65536 	= (int)(65536/20);					/* �����p�x([360/360]�x��20����) */
		br.BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_04_AOI;			/* [�i�C�t�e] */
		br.BULLET_REGIST_n_way				= (8);								/* [8way] */
		br.BULLET_REGIST_speed_offset		= (0);/*�Ă���*/
	//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*���ݎ�ނ�����*/
		bullet_regist_angle();/* bullet_regist_vector(); ���኱�V���v���ȈׁA�����Ɍ݊����Ȃ��B */

		if (0==((src->boss_base_danmaku_time_out)&0x1f))/* reset (1/32) */
		{
		//	aaa_angle1024	= (src->tmp_angleCCW1024)-(int)((1024/2));
			aaa_angle65536	= (src->tmp_angleCCW65536)-(int)((65536/2));
		}
	}
	else
//	if ((10-1)>((((u8)src->boss_base_danmaku_time_out)^0x20)&0x2f))/* x 10way */
	if ((10-1)>(((src->boss_base_danmaku_time_out)	  )&0x0f))/* x 10way */
	{
	//	aaa_angle1024 -= (int)(256/5);
	//	aaa_angle1024 -= (int)(1024/20);
		aaa_angle65536 -= (int)(65536/20);
		mask65536(aaa_angle65536);
		obj_send1->x256 					= (src->x256);						/* �e��x256 */
		obj_send1->y256 					= (src->y256);						/* �e��y256 */
		br.BULLET_REGIST_speed256			= (t256(2.0));						/* �e�� */
		br.BULLET_REGIST_angle65536 		= ((aaa_angle65536));				/* �e���p�x65536 */
	//	br.BULLET_REGIST_div_angle65536 	= (int)((difficulty+difficulty+1+1/*3*/)<<(6)); 	/* [�����e]�̕����p�x */
		br.BULLET_REGIST_div_angle65536 	= (int)((difficulty+1/*3*/)<<(6+1));		/* [�����e]�̕����p�x */
		br.BULLET_REGIST_bullet_obj_type	= BULLET_KUNAI12_01_AKA;			/* [�ԃN�i�C�e] */
		br.BULLET_REGIST_n_way				= (4);								/* [4way] */
		br.BULLET_REGIST_speed_offset		= (0);/*�Ă���*/
	//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*���ݎ�ނ�����*/
		bullet_regist_angle();/* bullet_regist_vector(); ���኱�V���v���ȈׁA�����Ɍ݊����Ȃ��B */
	}
}


/*---------------------------------------------------------
	�g5�ʒ��{�X ���u��p�F�~�X�f�B���N�V����(1/2)�v�ɂ�����Ƃ��������e��
	-------------------------------------------------------
	���@�_���A�S����24�����e
	-------------------------------------------------------
	[���F����bu llet_create_n_way_dan_sa_type()�̎��@�_���̓��U�g�_��Ȃ���e�Ȃ̂ŁA�����Ȃ���Γ�����܂��񂪁A
	���̂��������͋����e�ɂ���\��Ȃ̂ŁA�����Ȃ���Γ�����悤�ɂȂ�\��ł�]
---------------------------------------------------------*/
static void danmaku_create_02_24nerai(SPRITE *src)
{
	if (0==((src->boss_base_danmaku_time_out)&0x03))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
		tmp_angleCCW65536_jikinerai(obj_player, src);/*���@�_���p�쐬*/
		obj_send1->x256 					= (src->x256);						/* �e��x256 */
		obj_send1->y256 					= (src->y256);						/* �e��y256 */
	//	br.BULLET_REGIST_speed256			= (t256(2.0)+((src->boss_base_danmaku_time_out)<<2));	/* �e�� */
		br.BULLET_REGIST_speed256			= (t256(2.0)-((src->boss_base_danmaku_time_out)));	/* �e�� */
	//	#define TORIAEZU_000 (0)/* ��̒����l */
		#define TORIAEZU_000 (int)(6*64)/* ��̒����l */
		br.BULLET_REGIST_angle65536 		= ((src->tmp_angleCCW65536)) + TORIAEZU_000;		/* �e���p�x1024 */	/* ���@�_���e */
		br.BULLET_REGIST_div_angle65536 	= (int)(65536/24);					/* �����p�x */
		br.BULLET_REGIST_bullet_obj_type	= BULLET_KUNAI12_01_AKA;			/* [�ԃN�i�C�e] */
		br.BULLET_REGIST_n_way				= (24); 							/* [24way] */
		br.BULLET_REGIST_speed_offset		= (1);/*�Ă���*/
	//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*���ݎ�ނ�����*/
		bullet_regist_angle();/* bullet_regist_vector(); ���኱�V���v���ȈׁA�����Ɍ݊����Ȃ��B */
	}
}


/*---------------------------------------------------------
	�g5�ʒ��{�X ��� �u��p�F�~�X�f�B���N�V����(2/2)�v�ɂ�����Ƃ��������e��
	-------------------------------------------------------
	���@�_���A11�e(���S�ƁA���E��5�e���_��)
	-------------------------------------------------------
	[���F����bu llet_create_n_way_dan_sa_type()�̎��@�_���̓��U�g�_��Ȃ���e�Ȃ̂ŁA�����Ȃ���Γ�����܂��񂪁A
	���̂��������͋����e�ɂ���\��Ȃ̂ŁA�����Ȃ���Γ�����悤�ɂȂ�\��ł�]
---------------------------------------------------------*/
static void danmaku_create_03_11nife(SPRITE *src)
{
	if (0==((src->boss_base_danmaku_time_out)&0x07))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
		tmp_angleCCW65536_jikinerai(obj_player, src);/*���@�˂炢�p�쐬*/
		obj_send1->x256 					= (src->x256);						/* �e��x256 */
		obj_send1->y256 					= (src->y256);						/* �e��y256 */
		br.BULLET_REGIST_speed256			= (t256(2.0));						/* �e�� */
	//	#define TORIAEZU_111 (0)/* ��̒����l */
		#define TORIAEZU_111 (int)(4*64)/* ��̒����l */
		br.BULLET_REGIST_angle65536 		= ((src->tmp_angleCCW65536))-(int)(65536*6/(7*4)) + TORIAEZU_111;		/* �e���p�x1024 */
		br.BULLET_REGIST_div_angle65536 	= (int)(65536/(7*4));				/* �����p�x([90/360]�x��7����==28����) */
		br.BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_04_AOI;			/* [�i�C�t�e] */
		br.BULLET_REGIST_n_way				= (11); 							/* [11way] */
		br.BULLET_REGIST_speed_offset		= (0);/*�Ă���*/
	//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*���ݎ�ނ�����*/
		bullet_regist_angle();/* bullet_regist_vector(); ���኱�V���v���ȈׁA�����Ɍ݊����Ȃ��B */
	}
}


/*---------------------------------------------------------
	�g5�ʃ{�X ��� �u�ʏ�U��1(1/2)�v�ɂ�����Ƃ��������e��(�\��)
	-------------------------------------------------------
	24way =(�E12way)+(��12way)
	-------------------------------------------------------

---------------------------------------------------------*/
static void danmaku_create_04_pink_hearts(SPRITE *src)
{
	if (0==((src->boss_base_danmaku_time_out)&0x0f))/* �{���� 8 �e(128==0x80==danmaku_time_out) */
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		obj_send1->y256 						= (src->y256);						/* �e��y256 */	/* �{���͔��e�ʒu�� offset �p */
		obj_send1->tmp_angleCCW65536 = (0);/* ������ */
	//
		//	br.BULLET_REGIST_div_angle1024		= (0);								/* �_�~�[�����p�x(���g�p) */
			br.BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_06_YUKARI; 		/* [�s���N�i�C�t�e] */
			br.BULLET_REGIST_n_way				= (1);								/* [1way] */
			br.BULLET_REGIST_speed_offset		= (0);/*�Ă���*/
		//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*���ݎ�ނ�����*/
		//
		int jj;
		jj=0;
		int i;
		for (i=0; i<(13*12)/*144*/; i+= (13) )/*12����*/							/* �p�x(128[(90+a)/360�x]�� 10 ����) */
		{
			obj_send1->x256 					= src->x256+t256(4.0);				/* �{���͔��e�ʒu�� x offset ���� */
			br.BULLET_REGIST_speed256			= ((160-i)<<(1+1+1));				/* �e�� */
			br.BULLET_REGIST_angle65536			= (jj/*(0+i+i)&(1024-1)*/);
			bullet_regist_angle();/* bullet_regist_vector(); ���኱�V���v���ȈׁA�����Ɍ݊����Ȃ��B */
			//
			obj_send1->x256 					= src->x256-t256(4.0);				/* �{���͔��e�ʒu�� x offset ���� */
			br.BULLET_REGIST_speed256			= ((160-i)<<(1+1+1));				/* �e�� */
			br.BULLET_REGIST_angle65536			= ((0-jj)&(65536-1));
			bullet_regist_angle();/* bullet_regist_vector(); ���኱�V���v���ȈׁA�����Ɍ݊����Ȃ��B */
			//
			jj += (13*2*64);
		}
	}
}


/*---------------------------------------------------------
	�g5�ʃ{�X ��� �u�ʏ�U��1(2/2)�v�ɂ�����Ƃ��������e��(�\��)
	-------------------------------------------------------
	-------------------------------------------------------

---------------------------------------------------------*/
static void danmaku_create_05_32way_dual(SPRITE *src)
{
	if (0==((src->boss_base_danmaku_time_out)&0x3f))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
		//
			obj_send1->x256 					= (src->x256);						/* �e��x256 */
			obj_send1->y256 					= (src->y256);						/* �e��y256 */
		//
	//		br.BULLET_REGIST_div_angle1024		= (0);								/* �_�~�[�����p�x(���g�p) */
			br.BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_04_AOI;			/* [�i�C�t�e] */
			br.BULLET_REGIST_n_way				= (1);								/* [1way] */
			br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
		//
		int i;
		for (i=0; i<(1024); i+= 32 )/*32����*/	/* �p�x(1024[360/360�x]�� 32 ����)1024 == 32 x 32 */
		{
			br.BULLET_REGIST_speed256			= (t256(2.0)-((i&32)<<3));			/*��͍����e*/
			br.BULLET_REGIST_angle1024			= (i/*(0+i)&(1024-1)*/);			/* �e���p�x1024 */
			bullet_regist_vector();
		}
	}
}


/*---------------------------------------------------------
	���݁u�N���b�N�R�[�v�X(1/2)�v
---------------------------------------------------------*/
#if 0
#endif


/*---------------------------------------------------------
	���݁u�N���b�N�R�[�v�X(2/2)�v
---------------------------------------------------------*/
#if 0
#endif


/*---------------------------------------------------------
	���ہu���i�N���b�N(1/2)�v
	-------------------------------------------------------
	32way �Œ�A�e	x 4��
---------------------------------------------------------*/
static void danmaku_create_06_luna_clock_32way(SPRITE *src)
{
	if (0x40==((src->boss_base_danmaku_time_out)&0xcf))/* 4�� */
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
		br.BULLET_REGIST_speed256			= (t256(2.0));				/* �e�� */
		br.BULLET_REGIST_angle1024			= (0);						/* ���˒��S�p�x / ����@�\(���@�_��/��) */
		br.BULLET_REGIST_div_angle1024		= (int)(1024/(32)); 		/* �����p�x(1024[360/360�x]�� 32 ����) */	/* 1����n���������p�x */
		br.BULLET_REGIST_bullet_obj_type	= BULLET_CAP16_05_TUTU_SIROI;	/* [������e] */	/* �e�O�� */
		br.BULLET_REGIST_n_way				= (32); 					/* [32way] */		/* ���e�� */
		br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
		bullet_regist_vector();
	}
}


/*---------------------------------------------------------
	���ہu���i�N���b�N(2/2)�v
---------------------------------------------------------*/
#if 0
#endif


/*---------------------------------------------------------
	�u�ʏ�U��3(1/2)�v
	-------------------------------------------------------
	�g�U�^�S���ʃi�C�t�e
	80�����e == (8way[hi to 0]+8way[slow to 0]) x 5way(5����)
	-------------------------------------------------------
	�{���͉�ʏ�Ɖ�ʍ��Ɖ�ʉE�̒[�Ńi�C�t����x�������˂���B
---------------------------------------------------------*/
static void danmaku_create_07_80way_dual_five(SPRITE *src)
{
	if (0==((src->boss_base_danmaku_time_out)&0x3f))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
			obj_send1->x256 					= (src->x256);		/* �e��x256 */
			obj_send1->y256 					= (src->y256);		/* �e��y256 */
	//		br.BULLET_REGIST_div_angle1024		= (0);									/* �_�~�[�����p�x(���g�p) */
			br.BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_04_AOI;				/* [�i�C�t�e] */
			br.BULLET_REGIST_n_way				= (1);									/* [1way] */
			br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
	//
		int ii; 	/* 1����65536���������p�x */
		int j;		/* �J�E���^ */
		j=0;
		for (ii=0; ii<(0x10000); ii += 819 )/* 819.2(65536/80)	12.8==(1024/80) 80���� [5����] */	/* 204.8==(1024/5) �p�x(1024[72/360�x]�� 5 ����) */
		{
			j++;
			j &= (16-1);/* 16way(8way[hi to 0]+8way[slow to 0]) */
			br.BULLET_REGIST_speed256			= (t256(3.0)-((j)<<5)-((j&1)<<7));		/*��͍����e*/
			br.BULLET_REGIST_angle1024			= (((0-ii)>>6)&(1024-1));				/* �p�x */
			bullet_regist_vector();
		}
	}
}


/*---------------------------------------------------------
	���C�h��Z�u����h�[���v
---------------------------------------------------------*/
#if 0
#endif
