
#include "bullet_object.h"

/*---------------------------------------------------------
	�e������
	-------------------------------------------------------
	���e���̓V�X�e���ŃT�|�[�g����\�z������܂��B(�\�z��)
	�V�X�e���ŃT�|�[�g�����ƁA�ǂ̃{�X���A�ǂ̒e���ł����Ă�悤�ɂȂ�܂��B
---------------------------------------------------------*/

/* BOSS_BASE */

/*---------------------------------------------------------
	���@�_���e�̊p�x���v�Z
---------------------------------------------------------*/
/*static*/ void tmp_angle_jikinerai512(SPRITE *p, SPRITE *t)
{
//	BOSS99_DATA *data = (BOSS99_DATA *)t->data;
//
	#if 1
		/* Gu����������܂ŁA�ǂ����Y����̂Ŋȗ��� */
	//	data->boss_base.tmp_angleCCW512 =  (atan_512(p->y256-t->y256		  ,p->x256-t->x256				  ));						/* �ȗ���(�e�Ǝ������傫���������Ȃ炸��Ȃ��A�Ⴄ�Ƃ��̕��덷�ɂȂ�) */
		t->tmp_angleCCW512 =  (atan_512(p->y256-t->y256+(16*256)			  ,p->x256-t->x256+(16*256) 	  ));						/* �Y����(�e�L�g�[�A�Y����) */
	#else
		/* Gu�����������炱���� */
		t->tmp_angleCCW512 =  (atan_512(p->y256-t->y256+((p->h128-t->h128)), p->x256-t->x256+((p->w128-t->w128))));/**/ 				/* �����e�̏ꍇ�Ɏ��@�_�� */
	#endif
}
	#if 0
	//	t->tmp_angleCCW512 =  (atan_512(p->y256-t->y256 					  ,p->x256-t->x256				  ));//2082417				/* �ȗ���(�e�Ǝ������傫���������Ȃ炸��Ȃ��A�Ⴄ�Ƃ��̕��덷�ɂȂ�) */
	//	t->tmp_angleCCW512 =  (atan_512(p->y256-t->y256+((p->h128-t->h128	)),p->x256-t->x256-((p->w128+t->w128))));/*???(original)*/	/* ��e�̏ꍇ�Ɏ��@�_��(?) �Ȃ񂩍��ɂ���� */
/// 	t->tmp_angleCCW512 =  (atan_512(p->y256-t->y256+((p->h128			)),p->x256-t->x256-((p->w128	 ))));
//		t->tmp_angleCCW512 =  (atan_512(p->y256-t->y256+((p->h128-t->h128	)),p->x256-t->x256-((p->w128-t->w128))));/**/				/* ��e�̏ꍇ�Ɏ��@�_�� */
		t->tmp_angleCCW512 =  (atan_512(p->y256-t->y256+((p->h128-t->h128	)),p->x256-t->x256+((p->w128-t->w128))));/**/				/* �����e�̏ꍇ�Ɏ��@�_�� */
	//	t->tmp_angleCCW512 =  (atan_512(p->y256-t->y256+((p->h128			)),p->x256-t->x256-((p->w128	 ))));
	//	t->tmp_angleCCW512 =  (atan_512(p->y256-t->y256-((p->h128			)),p->x256-t->x256-((p->w128	 ))));
	#endif

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
static void danmaku_create_01_sakuya_misogi(SPRITE *src)	/*, int nextstate*/ /*, int an im_frame*/
{
	static int aaa_angle512;
	BOSS99_DATA *data = (BOSS99_DATA *)src->data;
//
	if (0==((data->boss_base.danmaku_time_out)&0x07))
	{
		#if (0==USE_DESIGN_TRACK)
		play_voice_auto_track(VOICE13_SAKUYA_SHOT02);
		#else
	//	voice_play(VOICE13_SAKUYA_SHOT02, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE13_SAKUYA_SHOT02);
		#endif

/*CW*/
//		data->boss_base.tmp_angleCCW512 += (int)(128/5)+/*2*/(5)/*(difficulty<<2)*/; // 	/*3*/2+(difficulty);
/*CCW*/
		src->tmp_angleCCW512 -= (int)(128/5)+/*2*/(5)/*(difficulty<<2)*/; //	/*3*/2+(difficulty);
		mask512(src->tmp_angleCCW512);
	//	bullet_create_aka_lines(s);
	//	bullet_create_aka_maru_jikinerai(src, t256(2.5));
	//	bullet_create_aka_maru_jikinerai(src, t256(2.0));
	//	bullet_create_aka_maru_jikinerai(src, t256(3.5));
		send1_obj->x256 = src->x256;
		send1_obj->y256 = src->y256;
		#if 1
		/* ���Ƃŗv�� */
//		send1_obj->h128 = src->h128;
//		send1_obj->w128 = src->w128;
		#endif
	//	bullet_create_n_way_dan_sa_type(src,
		send1_obj->BULLET_REGIST_speed256			=	(t256(2.5));						/* �e�� */
		send1_obj->BULLET_REGIST_angle512			=	(src->tmp_angleCCW512); 			/* [������e] */
		send1_obj->BULLET_REGIST_div_angle512		=	(int)(128/5);
		send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_KNIFE20_04_AOI;				/* [�i�C�t�e] */
		send1_obj->BULLET_REGIST_n_way				=	(8);								/* [8way] */
		bullet_regist_basic();

		if (0==((data->boss_base.danmaku_time_out)&0x1f))/* reset (1/32) */
		{
			aaa_angle512=(src->tmp_angleCCW512)-(int)((512/2));
		}
	}
	else
//	if ((10-1)>((((u8)data->boss_base.danmaku_time_out)^0x20)&0x2f))/* x 10way */
	if ((10-1)>(((data->boss_base.danmaku_time_out)    )&0x0f))/* x 10way */
	{
	//	if (/*(64)*/(16)/*(64-(difficulty<<4))*/ < data->danmaku_time_out)
		{
			aaa_angle512 -= (int)(128/5);
			mask512(aaa_angle512);
			send1_obj->x256 = src->x256;
			send1_obj->y256 = src->y256;
			#if 1
			/* ���Ƃŗv�� */
//			send1_obj->h128 = src->h128;
//			send1_obj->w128 = src->w128;
			#endif
		//	bullet_create_n_way_dan_sa_type(src,
			send1_obj->BULLET_REGIST_speed256			=	(t256(2.0));					/* �e�� */
			send1_obj->BULLET_REGIST_angle512			=	(aaa_angle512);
			send1_obj->BULLET_REGIST_div_angle512		=	(int)(difficulty+1/*3*/);		/* [�����e]�̊p�x */
			send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_KUNAI12_01_AKA;			/* [�ԃN�i�C�e] */
			send1_obj->BULLET_REGIST_n_way				=	(4);							/* [4way] */
			bullet_regist_basic();
		}
	}
}

/*---------------------------------------------------------
	�g5�ʒ��{�X ���u��p�F�~�X�f�B���N�V����(1/2)�v�ɂ�����Ƃ��������e��
	-------------------------------------------------------
	���@�_���A�S����24�����e
	-------------------------------------------------------
	[���F����bullet_create_n_way_dan_sa_type()�̎��@�_���̓��U�g�_��Ȃ���e�Ȃ̂ŁA�����Ȃ���Γ�����܂��񂪁A
	���̂��������͋����e�ɂ���\��Ȃ̂ŁA�����Ȃ���Γ�����悤�ɂȂ�\��ł�]
---------------------------------------------------------*/
static void danmaku_create_02_24nerai(SPRITE *src)
{
	BOSS99_DATA *data = (BOSS99_DATA *)src->data;
	if (0==((data->boss_base.danmaku_time_out)&0x03))
	{
		#if (0==USE_DESIGN_TRACK)
		play_voice_auto_track(VOICE14_BOSS_KOUGEKI_01);
		#else
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
	int aaa_angle512;
			tmp_angle_jikinerai512(player, src);/*���@�_���p�쐬*/
			aaa_angle512=(src->tmp_angleCCW512);
		{
			send1_obj->x256 = src->x256;
			send1_obj->y256 = src->y256;
			#if 1
			/* ���Ƃŗv�� */
//			send1_obj->h128 = src->h128;
//			send1_obj->w128 = src->w128;
			#endif
		//	bullet_create_n_way_dan_sa_type(src,
			send1_obj->BULLET_REGIST_speed256			=	(t256(2.0)+((data->boss_base.danmaku_time_out)<<2));	/* �e�� */
			send1_obj->BULLET_REGIST_angle512			=	(aaa_angle512); 				/* ���@�_���e */
			send1_obj->BULLET_REGIST_div_angle512		=	(int)(512/24);					/* �p�x */
			send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_KUNAI12_01_AKA;			/* [�ԃN�i�C�e] */
			send1_obj->BULLET_REGIST_n_way				=	(24);							/* [24way] */
			bullet_regist_basic();
		}
	}
}

/*---------------------------------------------------------
	�g5�ʒ��{�X ��� �u��p�F�~�X�f�B���N�V����(2/2)�v�ɂ�����Ƃ��������e��
	-------------------------------------------------------
	���@�_���A11�e(���S�ƁA���E��5�e���_��)
	-------------------------------------------------------
	[���F����bullet_create_n_way_dan_sa_type()�̎��@�_���̓��U�g�_��Ȃ���e�Ȃ̂ŁA�����Ȃ���Γ�����܂��񂪁A
	���̂��������͋����e�ɂ���\��Ȃ̂ŁA�����Ȃ���Γ�����悤�ɂȂ�\��ł�]
---------------------------------------------------------*/
static void danmaku_create_03_11nife(SPRITE *src)
{
	BOSS99_DATA *data = (BOSS99_DATA *)src->data;
	if (0==((data->boss_base.danmaku_time_out)&0x07))
	{
		#if (0==USE_DESIGN_TRACK)
		play_voice_auto_track(VOICE14_BOSS_KOUGEKI_01);
		#else
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
	int aaa_angle512;
			tmp_angle_jikinerai512(player, src);/*���@�˂炢�p�쐬*/
			aaa_angle512=(src->tmp_angleCCW512);
		{
			send1_obj->x256 = src->x256;
			send1_obj->y256 = src->y256;
			#if 1
			/* ���Ƃŗv�� */
//			send1_obj->h128 = src->h128;
//			send1_obj->w128 = src->w128;
			#endif
	//		bullet_create_n_way_dan_sa_type(src,
			send1_obj->BULLET_REGIST_speed256			=	(t256(2.0));				/* �e�� */
			send1_obj->BULLET_REGIST_angle512			=	(aaa_angle512);
			send1_obj->BULLET_REGIST_div_angle512		=	(int)(512/(7*4));			/* �p�x([90/360]�x��7����) */
			send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_KNIFE20_04_AOI;		/* [�i�C�t�e] */
			send1_obj->BULLET_REGIST_n_way				=	(11);						/* [11way] */
			bullet_regist_basic();
		}
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
	BOSS99_DATA *data = (BOSS99_DATA *)src->data;
	if (0==((data->boss_base.danmaku_time_out)&0x0f))/* �{���� 8 �e(128==0x80==danmaku_time_out) */
	{
		#if (0==USE_DESIGN_TRACK)
		play_voice_auto_track(VOICE14_BOSS_KOUGEKI_01);
		#else
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		send1_obj->w128 = src->w128;
		send1_obj->h128 = src->h128;
		send1_obj->y256 = src->y256;	/* �{���͔��e�ʒu�� offset �p */
//		send1_obj->tmp_angleCCW512 = (0);/* ������ */
	//
		int i;
		for (i=0; i<(13*12)/*144*/; i+= 13 )/*12����*/		/* �p�x(128[(90+a)/360�x]�� 10 ����) */
		{
			send1_obj->x256 = src->x256+t256(4.0);/* �{���͔��e�ʒu�� x offset ���� */
	//		bullet_create_n_way_dan_sa_type(send1_obj/*src*/,
			send1_obj->BULLET_REGIST_speed256			=	((160-i)<<(1+1+1)); 				/* �e�� */
			send1_obj->BULLET_REGIST_angle512			=	(i/*(0+i)&(512-1)*/);
	//		send1_obj->BULLET_REGIST_div_angle512		=	(0);								/* �_�~�[�p�x(���g�p) */
			send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_KNIFE20_06_YUKARI;			/* [�s���N�i�C�t�e] */
			send1_obj->BULLET_REGIST_n_way				=	(1);								/* [1way] */
			bullet_regist_basic();
//
			send1_obj->x256 = src->x256-t256(4.0);	/* �{���͔��e�ʒu�� x offset ���� */
	//		bullet_create_n_way_dan_sa_type(send1_obj/*src*/,
			send1_obj->BULLET_REGIST_speed256			=	((160-i)<<(1+1+1)); 				/* �e�� */
			send1_obj->BULLET_REGIST_angle512			=	((0-i)&(512-1));
	//		send1_obj->BULLET_REGIST_div_angle512		=	(0);								/* �_�~�[�p�x(���g�p) */
			send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_KNIFE20_06_YUKARI;			/* [�s���N�i�C�t�e] */
			send1_obj->BULLET_REGIST_n_way				=	(1);								/* [1way] */
			bullet_regist_basic();
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
	BOSS99_DATA *data = (BOSS99_DATA *)src->data;
	if (0==((data->boss_base.danmaku_time_out)&0x3f))
	{
		#if (0==USE_DESIGN_TRACK)
		play_voice_auto_track(VOICE14_BOSS_KOUGEKI_01);
		#else
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
			send1_obj->x256 = src->x256;		/* �e��x256 */
			send1_obj->y256 = src->y256;		/* �e��y256 */
			#if 1
			/* ���Ƃŗv�� */
//			send1_obj->h128 = src->h128;		/* �e�� */
//			send1_obj->w128 = src->w128;		/* �e�� */
			#endif
//
		int i;
		for (i=0; i<(512); i+= 16 )/*32����*/	/* �p�x(512[360/360�x]�� 32 ����) */
		{
		//	bullet_create_n_way_dan_sa_type(src,
			send1_obj->BULLET_REGIST_speed256			=	(t256(2.0)-((i&16)<<4));	/*��͍����e*/
			send1_obj->BULLET_REGIST_angle512			=	(i/*(0+i)&(512-1)*/);		/* �e���p�x512 */
	//		send1_obj->BULLET_REGIST_div_angle512		=	(0);						/* �_�~�[�p�x(���g�p) */
			send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_KNIFE20_04_AOI;		/* [�i�C�t�e] */
			send1_obj->BULLET_REGIST_n_way				=	(1);						/* [1way] */
			bullet_regist_basic();
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
	BOSS99_DATA *data = (BOSS99_DATA *)src->data;
	if (0x40==((data->boss_base.danmaku_time_out)&0xcf))/* 4�� */
	{
		#if (0==USE_DESIGN_TRACK)
		play_voice_auto_track(VOICE14_BOSS_KOUGEKI_01);
		#else
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
	//	bullet_create_n_way_dan_sa_type(src,
		send1_obj->BULLET_REGIST_speed256			=	(t256(2.0));			/* �e�� */
		send1_obj->BULLET_REGIST_angle512			=	(0);					/* ���˒��S�p�x / ����@�\(���@�_��/��) */
		send1_obj->BULLET_REGIST_div_angle512		=	(int)(512/(32));		/* �p�x(512[360/360�x]�� 32 ����) */	/* 1����n���������p�x */
		send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_CAP16_05_SIROI;	/* [���Ēe] */	/* �e�O�� */
		send1_obj->BULLET_REGIST_n_way				=	(32);					/* [32way] */		/* ���e�� */
		bullet_regist_basic();
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
	BOSS99_DATA *data = (BOSS99_DATA *)src->data;
	if (0==((data->boss_base.danmaku_time_out)&0x3f))
	{
		#if (0==USE_DESIGN_TRACK)
		play_voice_auto_track(VOICE14_BOSS_KOUGEKI_01);
		#else
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
	//
			send1_obj->x256 = src->x256;		/* �e��x256 */
			send1_obj->y256 = src->y256;		/* �e��y256 */
			#if 1
			/* ���Ƃŗv�� */
//			send1_obj->h128 = src->h128;		/* �e�� */
//			send1_obj->w128 = src->w128;		/* �e�� */
			#endif
//
		int ii; 	/* 1����65536���������p�x */
		int j;		/* �J�E���^ */
		j=0;
		for (ii=0; ii<(0x10000); ii += 819 )/* 819.2(65536/80)	6.4==(512/80) 80���� [5����] */ 	/* 102.4==(512/5) �p�x(512[72/360�x]�� 5 ����) */
		{
			j++;
			j &= (16-1);/* 16way(8way[hi to 0]+8way[slow to 0]) */
		//	bullet_create_n_way_dan_sa_type(src,
			send1_obj->BULLET_REGIST_speed256			=	(t256(3.0)-((j)<<5)-((j&1)<<7));	/*��͍����e*/
			send1_obj->BULLET_REGIST_angle512			=	(((0-ii)>>7)&(512-1));				/* CW */
	//		send1_obj->BULLET_REGIST_div_angle512		=	(0);								/* �_�~�[�p�x(���g�p) */
			send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_KNIFE20_04_AOI;				/* [�i�C�t�e] */
			send1_obj->BULLET_REGIST_n_way				=	(1);								/* [1way] */
			bullet_regist_basic();
		}
	}
}

/*---------------------------------------------------------
	���C�h��Z�u����h�[���v
---------------------------------------------------------*/
#if 0
#endif

/*---------------------------------------------------------
	�A���X(��)	����@�e��
	-------------------------------------------------------
	�e�L�g�[
---------------------------------------------------------*/
static void danmaku_create_08_sentakki(SPRITE *src)
{
	BOSS99_DATA *data = (BOSS99_DATA *)src->data;
	if (0==((data->boss_base.danmaku_time_out)&0x03))
	{
		#if (0==USE_DESIGN_TRACK)
		play_voice_auto_track(VOICE14_BOSS_KOUGEKI_01);
		#else
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		src->tmp_angleCCW512 -= (int)(512/24);
		mask512(src->tmp_angleCCW512);
	//
			send1_obj->x256 = src->x256;		/* �e��x256 */
			send1_obj->y256 = src->y256;		/* �e��y256 */
			#if 1
			/* ���Ƃŗv�� */
//			send1_obj->h128 = src->h128;		/* �e�� */
//			send1_obj->w128 = src->w128;		/* �e�� */
			#endif
//
		{
		//	bullet_create_n_way_dan_sa_type(src,
			send1_obj->BULLET_REGIST_speed256			=	(t256(2.0)+((data->boss_base.danmaku_time_out)<<2));	/* �e�� */
			send1_obj->BULLET_REGIST_angle512			=	(src->tmp_angleCCW512);
			send1_obj->BULLET_REGIST_div_angle512		=	(int)(512/24);						/* �p�x */
			send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_UROKO14_00_AOI;				/* [�ؒe] */
			send1_obj->BULLET_REGIST_n_way				=	(8);								/* [8way] */
			bullet_regist_basic();
//
		//	bullet_create_n_way_dan_sa_type(src,
			send1_obj->BULLET_REGIST_speed256			=	(t256(2.0)+((data->boss_base.danmaku_time_out)<<2));	/* �e�� */
			send1_obj->BULLET_REGIST_angle512			=	((-src->tmp_angleCCW512)&(512-1));
			send1_obj->BULLET_REGIST_div_angle512		=	(int)(512/24);						/* �p�x */
			send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_UROKO14_04_MIZUIRO;			/* [���ؒe] */
			send1_obj->BULLET_REGIST_n_way				=	(8);								/* [8way] */
			bullet_regist_basic();
		}
	}
}

/*---------------------------------------------------------
	�g2�ʃ{�X �`���m	�ʏ�U��1(1/3)	���@�_���g�U�e(1-2-3-4-5-6-7-8�e)
	-------------------------------------------------------
	(1-2-3-4-5-6-7�e)���Ǝv���Ă����ǁA�����蒼������A
	(1-2-3-4-5-6-7-8�e)�������̂ŏC���B
���[��
	�����蒼���Ă悭������A
	(1-2-3-4-5-6-7�e)�� 			(easy�Ƃ�)
	(2-3-4-5-6-7-8�e)�Ȃ�ł��ˁB	(luna�Ƃ�)
---------------------------------------------------------*/
static void danmaku_create_09_cirno_misogi(SPRITE *src)
{
	BOSS99_DATA *data = (BOSS99_DATA *)src->data;
	if (128==((data->boss_base.danmaku_time_out)))
	{
		tmp_angle_jikinerai512(player, src);/*���@�˂炢�p�쐬*/
	}
	else
	if (0x40==((data->boss_base.danmaku_time_out)&0xc7))/* 8��(�C��1-2-3-4-5-6-7�e:0way�͌����Ȃ��̂Ō��̂�7��) */
	{
		#if (0==USE_DESIGN_TRACK)
		play_voice_auto_track(VOICE13_SAKUYA_SHOT02);
		#else
	//	voice_play(VOICE13_SAKUYA_SHOT02, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE13_SAKUYA_SHOT02);
		#endif
	//
		{
			send1_obj->x256 = src->x256;		/* �e��x256 */
			send1_obj->y256 = src->y256;		/* �e��y256 */
			#if 1
			/* ���Ƃŗv�� */
//			send1_obj->h128 = src->h128;		/* �e�� */
//			send1_obj->w128 = src->w128;		/* �e�� */
			#endif
//
		//	bullet_create_n_way_dan_sa_type(src,
			send1_obj->BULLET_REGIST_speed256			=	(t256(2.5));	/* �e�� */
			send1_obj->BULLET_REGIST_angle512			=	(src->tmp_angleCCW512);
			send1_obj->BULLET_REGIST_div_angle512		=	(int)(512/(64));		/* �p�x(512[360/360�x]�� 64 ����) */	/* 1����n���������p�x */
			send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_KOME_01_AOI; 	/* [�Ēe] �ɂȂ��Ă�̂͌��� �X�e �̃O������������ */
			send1_obj->BULLET_REGIST_n_way				=	((/*16*/7-((data->boss_base.danmaku_time_out)>>3))&7)+(1);/* [nway] */
			bullet_regist_basic();
		}
	}
}

/*---------------------------------------------------------
	�g2�ʃ{�X �`���m	�ʏ�U��1(2/3)	���@�_���S���ʒe
	-------------------------------------------------------
---------------------------------------------------------*/
#if 0
#endif

/*---------------------------------------------------------
	�g2�ʃ{�X �`���m	�ʏ�U��1(3/3)	���@�_�������e
	-------------------------------------------------------
---------------------------------------------------------*/
#if 0
#endif


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
	���v���A�����v����6way�����e(���F)		1�񖈂�(512/(6*8))[512/512�x]��]
	48���ʑS���ʒe(�ԐF)						8�񖈂ɔ��e
	-------------------------------------------------------
	�F�؈��
	�𑜓x�Ƃ����x�Ƃ��̗v���ŁA���̂܂܂ł� psp�œ���C������B
	Lunatic �͂��̂܂܂ŁAHard �ȉ��͍׍H���ĈՂ�������B
---------------------------------------------------------*/
#if 1
static void danmaku_create_11_houka_kenran(SPRITE *src)
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
	const static s16 houga_tbl[(H5*4)] =
	{/* easy				normal				hard				lunatic 			*/
		(4),				(8),				(5),				(6),					/* ��:3���4�̕����ȒP */
		(short)(512/(4)),	(short)(512/(8)),	(short)(512/(5)),	(short)(512/(6)),		/* ��:3���4�̕����ȒP */
		(short)(512/(4*8)), (short)(512/(8*8)), (short)(512/(5*8)), (short)(512/(6*8)), 	/* ��:3���4�̕����ȒP */
//
		(4*8),				(4*8),				(5*8),				(6*8),					/* ��:3���4�̕����ȒP */
		(short)(512/(4*8)), (short)(512/(4*8)), (short)(512/(5*8)), (short)(512/(6*8)), 	/* ��:3���4�̕����ȒP */
	};
	static int aaa_angle512;
	BOSS99_DATA *data = (BOSS99_DATA *)src->data;
	//
			send1_obj->x256 = src->x256;		/* �e��x256 */
			send1_obj->y256 = src->y256;		/* �e��y256 */
			#if 1
			/* ���Ƃŗv�� */
//			send1_obj->h128 = src->h128;		/* �e�� */
//			send1_obj->w128 = src->w128;		/* �e�� */
			#endif
//
//	if ((0x10)==((data->boss_base.danmaku_time_out)&0x1f))/* (16���1��)(128�Ȃ�v8��) */
	if ((0x40)==((data->boss_base.danmaku_time_out)&0x7f))/* (16���1��)(128�Ȃ�v8��) */
	{
		#if (0==USE_DESIGN_TRACK)
		play_voice_auto_track(VOICE14_BOSS_KOUGEKI_01);
		#else
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		{
			int aaa_angle512;
			tmp_angle_jikinerai512(player, src);/*���@�_���p�쐬*/
			aaa_angle512=(src->tmp_angleCCW512);
	//	bullet_create_n_way_dan_sa_type(src,
			send1_obj->BULLET_REGIST_speed256			=	(t256(2.0/*2.0*/)); 							/* �e�� */
			send1_obj->BULLET_REGIST_angle512			=	(aaa_angle512); 								/* ���@�_���e */
			send1_obj->BULLET_REGIST_div_angle512		=	houga_tbl[HOUGA_12_RED_DIV_ANGLE+difficulty];	/*(int)(512/(48))*/ 				/* �p�x(512[360/360�x]�� 48 ����) */	/* 1����n���������p�x */
			send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_KOME_02_AKA; 							/* [�ԐF�Ēe] */
			send1_obj->BULLET_REGIST_n_way				=	houga_tbl[HOUGA_08_RED_NUMS+difficulty];		/*(48)*/								/* [48way] */	/* ���e�� */
			bullet_regist_basic();
		}
	}
//	if ((0x02)==((data->boss_base.danmaku_time_out)&0x03))/* (2���1��)(8�񖈂ɔ��e) */
	if ((0x08)==((data->boss_base.danmaku_time_out)&0x0f))/* (2���1��)(8�񖈂ɔ��e) */
	{	// �����(��CCW������A�����CCW)
	//	bullet_create_n_way_dan_sa_type(src,
		send1_obj->BULLET_REGIST_speed256			=	(t256(2.0/*2.0*/)); 							/* �e�� */
		send1_obj->BULLET_REGIST_angle512			=	((/*0+*/(aaa_angle512))&(512-1));				/* ���˒��S�p�x / ����@�\(���@�_��/��) */
		send1_obj->BULLET_REGIST_div_angle512		=	houga_tbl[HOUGA_04_YEL_DIV_ANGLE+difficulty];	/*(int)(512/(6))*/					/* �p�x(512[360/360�x]�� 6 ����) */ 	/* 1����n���������p�x */
		send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_KOME_05_KIIRO;							/* [���F�Ēe] */
		send1_obj->BULLET_REGIST_n_way				=	houga_tbl[HOUGA_00_YEL_NUMS+difficulty];		/*(6)*/ 								/* [1way] */	/* ���e�� */
		bullet_regist_basic();
		// �t���(��CCW������A�E���CW)
	//	bullet_create_n_way_dan_sa_type(src,
		send1_obj->BULLET_REGIST_speed256			=	(t256(2.0/*2.0*/)); 							/* �e�� */
		send1_obj->BULLET_REGIST_angle512			=	((512-(aaa_angle512))&(512-1)); 				/* ���˒��S�p�x / ����@�\(���@�_��/��) */
		send1_obj->BULLET_REGIST_div_angle512		=	houga_tbl[HOUGA_04_YEL_DIV_ANGLE+difficulty];	/*(int)(512/(6))*/					/* �p�x(512[360/360�x]�� 6 ����) */ 	/* 1����n���������p�x */
		send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_KOME_05_KIIRO;							/* [���F�Ēe] */
		send1_obj->BULLET_REGIST_n_way				=	houga_tbl[HOUGA_00_YEL_NUMS+difficulty];		/*(6)*/ 								/* [1way] */	/* ���e�� */
		bullet_regist_basic();
		// ��]��
		aaa_angle512 += houga_tbl[HOUGA_16_YEL_ROTATE_ANGLE+difficulty];	/*(512/(6*8))*/ 				/* �p�x(512[360/360�x]�� 48����) */
	}
}
#endif

/*---------------------------------------------------------
	�g1�ʃ{�X ���[�~�A �镄�u�i�C�g�o�[�h�v�ɂ�����Ƃ��������e��(�\��)
	-------------------------------------------------------
	5�e�ڂ����@�_���� 32����16���� �|��e�B
	-------------------------------------------------------
0000 0000 0x00==(time_out)&0x11 ��(0x00==((time_out)&0x20))
0001 0000						��(0x00==((time_out)&0x20))
0010 0000 0x00==(time_out)&0x11 ��
0011 0000						��
0100 0000 0x00==(time_out)&0x11 ��(0x00==((time_out)&0x20))
0101 0000						��(0x00==((time_out)&0x20))
0110 0000 0x00==(time_out)&0x11 ��
0111 0000						��
//
1000 0000 start
---------------------------------------------------------*/
static void danmaku_create_04_night_bird(SPRITE *src)
{
	static int aaa_angle512;
	BOSS99_DATA *data = (BOSS99_DATA *)src->data;
//	if (128==((data->boss_base.danmaku_time_out)))
	if (0x10==((data->boss_base.danmaku_time_out)&0x10))
	{
		tmp_angle_jikinerai512(player, src);/*���@�˂炢�p�쐬*/
		aaa_angle512 = src->tmp_angleCCW512;/*���@�˂炢�p*/
		aaa_angle512 -= (int)(512*(5)/(32));/* 5�e�ڂ����@�_�� */
		/* �p�x(512[360/360�x]�� 32����) */
	}
	else
	if (0x00==((data->boss_base.danmaku_time_out)&0x10))/* 16�� */
	{
		#if (0==USE_DESIGN_TRACK)
		play_voice_auto_track(VOICE13_SAKUYA_SHOT02/*VOICE14_BOSS_KOUGEKI_01*/);
		#else
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE13_SAKUYA_SHOT02/*VOICE14_BOSS_KOUGEKI_01*/);
		#endif
		send1_obj->w128 = src->w128;	/* ���e�ʒu�� offset �p */
		send1_obj->h128 = src->h128;	/* ���e�ʒu�� offset �p */
		send1_obj->y256 = src->y256;	/* ���e�ʒu�� offset �p */
		{
			int iii;
				iii = (data->boss_base.danmaku_time_out);
			int aaa;/* �e�F */
			int bbb;/* ���� */
			if (0x00==((data->boss_base.danmaku_time_out)&0x20))
					{	bbb=-1;aaa=1;/* �g���e */}
			else	{	bbb= 1;aaa=0;/* �g���Βe */}
		//	send1_obj->x256 = src->x256+((16-(iii&0x0f))<<(3+8))*bbb;/* �{���͔��e�ʒu�� x offset ���� */
			send1_obj->x256 = src->x256+((16-(iii&0x0f))<<(3+7))*bbb;/* �{���͔��e�ʒu�� x offset ���� */
	//		bullet_create_n_way_dan_sa_type(send1_obj/*src*/,
			send1_obj->BULLET_REGIST_speed256			=	(t256(1.5));							/* �e�� */
			send1_obj->BULLET_REGIST_angle512			=	((/*0+*/(aaa_angle512)*bbb)&(512-1));	/* ���˒��S�p�x / ����@�\(���@�_��/��) */
	//		send1_obj->BULLET_REGIST_div_angle512		=	(0);									/* �_�~�[�p�x(���g�p) */
			send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_MARU12_02_MIDORI+aaa;			/* [�g���Βe]  [�g���e] */	/*BULLET_MARU12_03_AOI*/
			send1_obj->BULLET_REGIST_n_way				=	(1);									/* [1way] */
			bullet_regist_basic();
		}
		/* ���̒e�̊p�x */
		aaa_angle512 += (int)(512/(32));/* �p�x(512[360/360�x]�� 32����) */
	}
}

/*---------------------------------------------------------
	�������̗d��1(�V��l?)�����e��(�\��)
	-------------------------------------------------------
	���쐬
---------------------------------------------------------*/
static void danmaku_create_10_tengu_shot(SPRITE *src)
{
	static int aaa_angle512;
	static int data_wait2_start;/* ���e�Ԋustart */
	static int data_wait2;/* ���e�Ԋu */
	BOSS99_DATA *data = (BOSS99_DATA *)src->data;
//	if (128==((data->boss_base.danmaku_time_out)))
	if (0x10==((data->boss_base.danmaku_time_out)&0x10))
	{
		data_wait2_start = (20-((difficulty)<<2))/*8*/ /*10*/;
		data_wait2 = 10;
		tmp_angle_jikinerai512(player, src);/*���@�˂炢�p�쐬*/
		aaa_angle512 = src->tmp_angleCCW512;/*���@�˂炢�p*/
		aaa_angle512 -= (int)(512*(5)/(32));/* 5�e�ڂ����@�_�� */
		/* �p�x(512[360/360�x]�� 32����) */
	}
	else
//	if (0x00==((data->boss_base.danmaku_time_out)&0x10))/* 16�� */
	{
		data_wait2--;
		if (1 > data_wait2)
		{
			data_wait2 = data_wait2_start;
			#if (0==USE_DESIGN_TRACK)
			play_voice_auto_track(VOICE13_SAKUYA_SHOT02/*VOICE14_BOSS_KOUGEKI_01*/);
			#else
		//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE13_SAKUYA_SHOT02/*VOICE14_BOSS_KOUGEKI_01*/);
			#endif
			send1_obj->w128 = src->w128;	/* ���e�ʒu�� offset �p */
			send1_obj->h128 = src->h128;	/* ���e�ʒu�� offset �p */
			send1_obj->y256 = src->y256;	/* ���e�ʒu�� offset �p */
			{
			int iii;
				iii = (data->boss_base.danmaku_time_out);
			int aaa;/* �e�F */
			int bbb;/* ���� */
				if (0x00==((data->boss_base.danmaku_time_out)&0x20))
						{	bbb=-1;aaa=1;/* �g���e */}
				else	{	bbb= 1;aaa=0;/* �g���Βe */}
			//	send1_obj->x256 = src->x256+((16-(iii&0x0f))<<(3+8))*bbb;/* �{���͔��e�ʒu�� x offset ���� */
				send1_obj->x256 = src->x256+((16-(iii&0x0f))<<(3+7))*bbb;/* �{���͔��e�ʒu�� x offset ���� */
		//		bullet_create_n_way_dan_sa_type(send1_obj/*src*/,
				send1_obj->BULLET_REGIST_speed256			=	(t256(1.5));							/* �e�� */
				send1_obj->BULLET_REGIST_angle512			=	((/*0+*/(aaa_angle512)*bbb)&(512-1));	/* �e���p�x512 */
		//		send1_obj->BULLET_REGIST_div_angle512		=	(0);									/* �_�~�[�p�x(���g�p) */
				send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_MARU12_02_MIDORI+aaa;			/* [�g���Βe]  [�g���e] */	/*BULLET_MARU12_03_AOI*/
				send1_obj->BULLET_REGIST_n_way				=	(1);									/* [1way] */
				bullet_regist_basic();
			}
			/* ���̒e�̊p�x */
		aaa_angle512 += (int)(512/(32));/* �p�x(512[360/360�x]�� 32����) */
		}
	}
}

/*---------------------------------------------------------
	���ɔ�����ۂ��e���������Ă݂�e�X�g(�̔��������񂾂��ǁA
	���̂������̕����Ƃ͈Ⴄ�����Ɂc�c)
	-------------------------------------------------------
---------------------------------------------------------*/
#if 1
extern void bullet_create_aya_ice(SPRITE *src);

static void danmaku_create_12_aya_merin_test(SPRITE *src)
{
	BOSS99_DATA *data = (BOSS99_DATA *)src->data;
	if (0x10==((data->boss_base.danmaku_time_out)&0x1f))/* (16���1��)(128�Ȃ�v8��) */
	{
		#if (0==USE_DESIGN_TRACK)
		play_voice_auto_track(VOICE14_BOSS_KOUGEKI_01);
		#else
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		bullet_create_aya_ice(src);
	}
}
#endif

/*---------------------------------------------------------
	���ʒe��������
	-------------------------------------------------------

---------------------------------------------------------*/
/*static*/ void danmaku_generator(SPRITE *src)
{
	BOSS99_DATA *data=(BOSS99_DATA *)src->data;
	if (DANMAKU_00 != data->boss_base.danmaku_type) 	/* �e�������͕K�v�H */
	{
		data->boss_base.danmaku_time_out -= 1/*fps_fa ctor*/;	/* ���Ԍo�߂���  */
		if (0 < data->boss_base.danmaku_time_out)				/* ���Ԑ؂�H */
		{
			/* �e�������� */
			/*const*/static void (*danmaku_create_bbb[(DANMAKU_MAX/*16+1*/)])(SPRITE *sss) =
			{
				#define danmaku_create_99_mitei danmaku_create_09_cirno_misogi
				NULL,								/* 00 */	/* �e���������Ȃ� */
			//													/* [���] */
				danmaku_create_01_sakuya_misogi,	/* 01 */	/* ����S�e�� / �g5�ʒ��{�X ��� �u�ʏ�U���v */
				danmaku_create_02_24nerai,			/* 02 */	/* ���24�e�� (��p�u�~�X�f�B���N�V�������ǂ�(1/2)�v) */
				danmaku_create_03_11nife,			/* 03 */	/* ���11�e�� (��p�u�~�X�f�B���N�V�������ǂ�(2/2)�v) */
				danmaku_create_04_pink_hearts,		/* 04 */	/* �g5�ʃ{�X ��� �u�ʏ�U��1(1/2)�v�ɂ�����Ƃ��������e��(�\��) */
				danmaku_create_05_32way_dual,		/* 05 */	/* �g5�ʃ{�X ��� �u�ʏ�U��1/2(2/2)�v�ɂ�����Ƃ��������e��(�\��) */
				danmaku_create_06_luna_clock_32way, /* 06 */	/* �g5�ʃ{�X ��� ���ہu���i�N���b�N(1/2)�v�ɂ�����Ƃ��������e��(�\��) */
				danmaku_create_07_80way_dual_five,	/* 07 */	/* �g5�ʃ{�X ��� �u�ʏ�U��3(1/2)�v�ɂ�����Ƃ��������e��(�\��) */
			//													/* [�A���X(��)] */
				danmaku_create_04_night_bird,		/* 08 */	/* �g1�ʃ{�X ���[�~�A �镄�u�i�C�g�o�[�h�v�ɂ�����Ƃ��������e��(�\��) */
				danmaku_create_08_sentakki, 		/* 09 */	/* ����@�e�� */
				danmaku_create_11_houka_kenran, 	/* 0a */	/* �g���� �ؕ��u�F�؈�ࣁv�ɂ�����Ƃ��������e��(�\��) */
				danmaku_create_99_mitei,			/* 0b */	/* ����e�� */
				danmaku_create_99_mitei,			/* 0c */	/* ����e�� */
				danmaku_create_99_mitei,			/* 0d */	/* ����e�� */
				danmaku_create_99_mitei,			/* 0e */	/* ����e�� */
				danmaku_create_99_mitei,			/* 0f */	/* ����e�� */
			//													/* [�`���m(��)] */
				danmaku_create_09_cirno_misogi, 	/* 10 */	/* �`���m�S�e�� */
				danmaku_create_10_tengu_shot,		/* 11 */	/* �������̗d��1(�V��l?)�����e��(�\��) */
				danmaku_create_12_aya_merin_test,	/* 12 */	/* ���ɔ�����ۂ��e���������Ă݂�e�X�g */
				danmaku_create_99_mitei,			/* 13 */	/* ����e�� */
				danmaku_create_99_mitei,			/* 14 */	/* ����e�� */
				danmaku_create_99_mitei,			/* 15 */	/* ����e�� */
				danmaku_create_99_mitei,			/* 16 */	/* ����e�� */
				danmaku_create_99_mitei,			/* 17 */	/* ����e�� */
			//
				danmaku_create_99_mitei,			/* 18 */	/* ����e�� */
				danmaku_create_99_mitei,			/* 19 */	/* ����e�� */
				danmaku_create_99_mitei,			/* 1a */	/* ����e�� */
				danmaku_create_99_mitei,			/* 1b */	/* ����e�� */
				danmaku_create_99_mitei,			/* 1c */	/* ����e�� */
				danmaku_create_99_mitei,			/* 1d */	/* ����e�� */
				danmaku_create_99_mitei,			/* 1e */	/* ����e�� */
				danmaku_create_99_mitei,			/* 1f */	/* ����e�� */
			};
			(danmaku_create_bbb[(data->boss_base.danmaku_type/*-1*/)])(src);
		}
		else
		{
			data->boss_base.danmaku_type = DANMAKU_00;			/* �e�������I�� */
		}
	}
}
