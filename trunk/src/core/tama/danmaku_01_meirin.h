
/*---------------------------------------------------------
 �����͕핗  �` Toho Imitation Style.
  �v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	����̒e���𐶐����܂��B
---------------------------------------------------------*/


/*---------------------------------------------------------
	�g3�ʒ��{�X �g����
	�ʏ�U��1(1/2)
	16���ʕ΂�S���ʂ΂�T���e
	-------------------------------------------------------
---------------------------------------------------------*/
#if 0
#endif

/*---------------------------------------------------------
	�g3�ʒ��{�X �g����
	�ؕ��u�F�؈�ࣁv�ɂ�����Ƃ��������e��(�\��)
	���v���A�����v����6way�����e(���F)		1�񖈂�(1024/(6*8))[1024/1024�x]��]
	48���ʑS���ʒe(�ԐF)						8�񖈂ɔ��e
	-------------------------------------------------------
	�F�؈��
	�𑜓x�Ƃ����x�Ƃ��̗v���ŁA���̂܂܂ł� psp�œ���C������B
	Lunatic �͂��̂܂܂ŁAHard �ȉ��͍׍H���ĈՂ�������B
---------------------------------------------------------*/
#if 1
static void danmaku_create_0a_houka_kenran(SPRITE *src)
{
	enum
	{
		H0=0,H1,H2,H3,H4,H5
	};
	#define HOUGA_00_YEL_NUMS			(H0*4)
	#define HOUGA_04_YEL_DIV_ANGLE		(H1*4)
	#define HOUGA_16_YEL_ROTATE_ANGLE	(H2*4)
//
	#define HOUGA_08_RED_NUMS			(H3*4)
	#define HOUGA_12_RED_DIV_ANGLE		(H4*4)
	const static s32/*s16 �ł��������H*/ houga_tbl[(H5*4)] =
	{/* easy					normal					hard					lunatic 			*/
		(4),					(8),					(5),					(6),					/* ��:3���4�̕����ȒP */
		(int)(65536/(4)),		(int)(65536/(8)),		(int)(65536/(5)),		(int)(65536/(6)),		/* ��:3���4�̕����ȒP */
		(int)(65536/(4*8)), 	(int)(65536/(8*8)), 	(int)(65536/(5*8)), 	(int)(65536/(6*8)), 	/* ��:3���4�̕����ȒP */
//
		(4*8),					(4*8),					(5*8),					(6*8),					/* ��:3���4�̕����ȒP */
		(int)(65536/(4*8)), 	(int)(65536/(4*8)), 	(int)(65536/(5*8)), 	(int)(65536/(6*8)), 	/* ��:3���4�̕����ȒP */
	};

	//
			obj_send1->x256 					= (src->x256)+t256(25.0);				/* �e��x256 */
			obj_send1->y256 					= (src->y256)+t256(16.0);				/* �e��y256 */
//
//	if ((0x10)==((src->boss_base_danmaku_time_out)&0x1f))/* (16���1��)(128�Ȃ�v8��) */
	if ((0x40)==((src->boss_base_danmaku_time_out)&0x7f))/* (16���1��)(128�Ȃ�v8��) */
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		{
			tmp_angleCCW65536_jikinerai(obj_player, src);/*���@�_���p�쐬*/
			br.BULLET_REGIST_speed256			= (t256(1.75)); 									/* �e�� */	/* 3.5 2.5 2.0*/
			br.BULLET_REGIST_angle65536 		= (src->tmp_angleCCW65536); 								/* ���@�_���e */
			br.BULLET_REGIST_div_angle65536 	= houga_tbl[HOUGA_12_RED_DIV_ANGLE+difficulty]; 	/*(int)(1024/(48))*/	/* �����p�x(1024[360/360�x]�� 48 ����) */	/* 1����n���������p�x */
			br.BULLET_REGIST_bullet_obj_type	= BULLET_KOME_02_AKA;								/* [�ԐF�Ēe] */
			br.BULLET_REGIST_n_way				= houga_tbl[HOUGA_08_RED_NUMS+difficulty];			/*(48)*/				/* [48way] */	/* ���e�� */
			br.BULLET_REGIST_speed_offset		= (0);/*�Ă���*/
		//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*���ݎ�ނ�����*/
			bullet_regist_angle();/* bullet_regist_vector(); ���኱�V���v���ȈׁA�����Ɍ݊����Ȃ��B */
		}
	}
//	if ((0x02)==((src->boss_base_danmaku_time_out)&0x03))/* (2���1��)(8�񖈂ɔ��e) */
	if ((0x08)==((src->boss_base_danmaku_time_out)&0x0f))/* (2���1��)(8�񖈂ɔ��e) */
	{
		static int aaa_angle65536;
		br.BULLET_REGIST_speed256			= (t256(1.75)); 										/* �e�� */		/*3.5 2.5 2.0*/
		br.BULLET_REGIST_div_angle65536 	= houga_tbl[HOUGA_04_YEL_DIV_ANGLE+difficulty]; 		/*(int)(1024/(6))*/ 	/* �����p�x(1024[360/360�x]�� 6 ����) */	/* 1����n���������p�x */
		br.BULLET_REGIST_bullet_obj_type	= BULLET_KOME_05_KIIRO; 								/* [���F�Ēe] */
		br.BULLET_REGIST_n_way				= houga_tbl[HOUGA_00_YEL_NUMS+difficulty];				/*(6)*/ 				/* [1way] */	/* ���e�� */
		br.BULLET_REGIST_speed_offset		= (0);/*�Ă���*/
	//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*���ݎ�ނ�����*/
//
		// �����(��CCW������A�����CCW)
		br.BULLET_REGIST_angle65536 		= ((/*0+*/(aaa_angle65536))&(65536-1)); 				/* ���˒��S�p�x / ����@�\(���@�_��/��) */
		bullet_regist_angle();/* bullet_regist_vector(); ���኱�V���v���ȈׁA�����Ɍ݊����Ȃ��B */
		// �t���(��CCW������A�E���CW)
		br.BULLET_REGIST_angle65536 		= ((65536-(aaa_angle65536))&(65536-1)); 				/* ���˒��S�p�x / ����@�\(���@�_��/��) */
		bullet_regist_angle();/* bullet_regist_vector(); ���኱�V���v���ȈׁA�����Ɍ݊����Ȃ��B */
		// ��]��
		aaa_angle65536 += houga_tbl[HOUGA_16_YEL_ROTATE_ANGLE+difficulty];							/*(1024/(6*8))*/		/* �p�x(1024[360/360�x]�� 48����) */
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

extern void exchange_damnaku_check_type(void);

static void danmaku_create_12_aya_merin_test(SPRITE *src)
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
	if (0xd0==((src->boss_base_danmaku_time_out)&0xdf))/* (256���2��) */
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		int jj;
		for (jj=0; jj<(6); jj++)
		{
			br.BULLET_REGIST_speed256				= (t256(4.0)-(jj<<6));		/* �e�� */
			br.BULLET_REGIST_angle65536 			= (0/65536);				/* ������ */
			br.BULLET_REGIST_div_angle65536 		= (65536/24);				/* �����p�x([360/360]�x��24����) */ 	//step_tbl[(difficulty)];/*(32)*/
			int check_type = BULLET_KUNAI12_00_AOI+1-((src->boss_base_danmaku_time_out>>5)&1);	/*+(0&1)*/	/*(BULLET_KUNAI12_01_AKA-1)*/
			br.BULLET_REGIST_bullet_obj_type		= check_type;
			br.BULLET_REGIST_n_way					= (24);//step_tbl[(difficulty+4)];/*(32)*/
			br.BULLET_REGIST_speed_offset			= (-5); 		/* (-5) (-3)���������e */	/* ���̕����ɂȂ邩������ */
		//	br.BULLET_REGIST_regist_type			= REGIST_TYPE_00_MULTI_VECTOR;		/* ���݂��ꂵ���Ȃ����v�� */
			{
				obj_send1->x256 					= (src->x256)+t256(25.0);				/* �e��x256 */
				obj_send1->y256 					= (src->y256)+t256(16.0);				/* �e��y256 */
			//	obj_send1->m_angleCCW1024			= (src->m_angleCCW1024);				/* �e��y256 */
				bullet_regist_angle();	/* �p�x�e�Ƃ��ēo�^ */
			}
		}
	}
	else
	/* [�ϐg] */
	if (0x90==((src->boss_base_danmaku_time_out)&0xdf))/* (256���2��) */
	{
		/* �ϐg�\�Ȓe�̏�����ݒ� */
		br.BULLET_REGIST_bullet_obj_type		= BULLET_KUNAI12_00_AOI+1-((src->boss_base_danmaku_time_out>>5)&1); /*+(0&1)*/	/*(BULLET_KUNAI12_01_AKA-1)*/
		/* ����ʂɂ���e��S�����ׂāA�e��ϐg������B */
		exchange_damnaku_check_type();
	}
}
#endif
