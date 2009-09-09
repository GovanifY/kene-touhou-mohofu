
#include "bullet_object.h"

/*---------------------------------------------------------
	�e������
	-------------------------------------------------------
	���e���̓V�X�e���ŃT�|�[�g����\�z������܂��B(�\�z��)
	�V�X�e���ŃT�|�[�g�����ƁA�ǂ̃{�X���A�ǂ̒e���ł����Ă�悤�ɂȂ�܂��B
---------------------------------------------------------*/

/*---------------------------------------------------------
	���@�_���e�̊p�x���v�Z
---------------------------------------------------------*/
/*static*/ void tmp_angle_jikinerai512(SPRITE *p, SPRITE *t)
{
	BOSS99_DATA *data = (BOSS99_DATA *)t->data;
	#if 1
		data->boss_base.tmp_angleCCW512 =  (atan_512(p->y256-t->y256					  ,p->x256-t->x256				  ));//2082417				/* �ȗ���(�e�Ǝ������傫���������Ȃ炸��Ȃ��A�Ⴄ�Ƃ��̕��덷�ɂȂ�) */
	//	data->boss_base.tmp_angleCCW512 =  (atan_512(p->y256-t->y256+((p->h128-t->h128	)),p->x256-t->x256-((p->w128+t->w128))));/*???(original)*/	/* ��e�̏ꍇ�Ɏ��@�_��(?) �Ȃ񂩍��ɂ���� */
/// 	data->boss_base.tmp_angleCCW512 =  (atan_512(p->y256-t->y256+((p->h128			)),p->x256-t->x256-((p->w128	 ))));
//		data->boss_base.tmp_angleCCW512 =  (atan_512(p->y256-t->y256+((p->h128-t->h128	)),p->x256-t->x256-((p->w128-t->w128))));/**/				/* ��e�̏ꍇ�Ɏ��@�_�� */
	//	data->boss_base.tmp_angleCCW512 =  (atan_512(p->y256-t->y256+((p->h128-t->h128	)),p->x256-t->x256+((p->w128-t->w128))));/**/				/* �����e�̏ꍇ�Ɏ��@�_�� */
	//	data->boss_base.tmp_angleCCW512 =  (atan_512(p->y256-t->y256+((p->h128			)),p->x256-t->x256-((p->w128	 ))));
	//	data->boss_base.tmp_angleCCW512 =  (atan_512(p->y256-t->y256-((p->h128			)),p->x256-t->x256-((p->w128	 ))));
	#endif
}

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

static void danmaku_create_01_misogi(SPRITE *s/*, int nextstate*/ /*, int anim_frame*/)
{
	static int aaa_angle512;
	BOSS99_DATA *data = (BOSS99_DATA *)s->data;
//
	if (0==((data->boss_base.danmaku_time_out)&0x07))
	{
		play_voice_auto_track(VOICE13_SAKUYA_SHOT02);
/*CW*/
//		data->boss_base.tmp_angleCCW512 += (int)(128/5)+/*2*/(5)/*(difficulty<<2)*/; // 	/*3*/2+(difficulty);
/*CCW*/
		data->boss_base.tmp_angleCCW512 -= (int)(128/5)+/*2*/(5)/*(difficulty<<2)*/; // 	/*3*/2+(difficulty);
		mask512(data->boss_base.tmp_angleCCW512);
	//	bullet_create_aka_lines(s);
	//	bullet_create_aka_maru_jikinerai(s, t256(2.5));
	//	bullet_create_aka_maru_jikinerai(s, t256(2.0));
	//	bullet_create_aka_maru_jikinerai(s, t256(3.5));
		bullet_create_n_way_dan_sa_type(s,
			(t256(2.5)),							/* �e�� */
			(data->boss_base.tmp_angleCCW512),
			(int)(128/5),
			BULLET_KNIFE20_04_AOI,
			(8));/* [������8way�i�C�t�e] */
		if (0==((data->boss_base.danmaku_time_out)&0x1f))/* reset (1/32) */
		{
			aaa_angle512=(data->boss_base.tmp_angleCCW512)-(int)((512/2));
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
			bullet_create_n_way_dan_sa_type(s,
				(t256(2.0)),					/* �e�� */
				(aaa_angle512),
				(int)(difficulty+1/*3*/),		/* �����e�̊p�x */
				BULLET_KUNAI12_00_AKA,
				(4));/* [4way�����ԃN�i�C�e] */
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

//static void enemy_boss05_more_angle(SPRITE *s, int speed256, int angle4096/*, int a_angle4096, int kaiten_houkou*/)
static void danmaku_create_02_24nerai(SPRITE *s)
{
	BOSS99_DATA *data = (BOSS99_DATA *)s->data;
	if (0==((data->boss_base.danmaku_time_out)&0x03))
	{
		play_voice_auto_track(VOICE11_SAKUYA_E_SHOT00);
	//
	int aaa_angle512;
			tmp_angle_jikinerai512(player, s);/*���@�˂炢�p�쐬*/
			aaa_angle512=(data->boss_base.tmp_angleCCW512);
		{
			bullet_create_n_way_dan_sa_type(s,
				(t256(2.0)+((data->boss_base.danmaku_time_out)<<2)/*+aa*/), 	/* �e�� */
				(aaa_angle512/*+aa*/),
				(int)(512/24),		/* �p�x */
				BULLET_KUNAI12_00_AKA,
				(24));/* [24way�ԃN�i�C�e] */
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

//static void enemy_boss05_more_angle(SPRITE *s, int speed256, int angle4096/*, int a_angle4096, int kaiten_houkou*/)
static void danmaku_create_03_11nife(SPRITE *s)
{
	BOSS99_DATA *data = (BOSS99_DATA *)s->data;
	if (0==((data->boss_base.danmaku_time_out)&0x07))
	{
		play_voice_auto_track(VOICE11_SAKUYA_E_SHOT00);
	//
	int aaa_angle512;
			tmp_angle_jikinerai512(player, s);/*���@�˂炢�p�쐬*/
			aaa_angle512=(data->boss_base.tmp_angleCCW512);
		{
			bullet_create_n_way_dan_sa_type(s,
				(t256(2.0)),			/* �e�� */
				(aaa_angle512),
				(int)(512/(7*4)),		/* �p�x([90/360]�x��7����) */
				BULLET_KNIFE20_04_AOI,
				(11));/* [11way�i�C�t�e] */
		}
	}
}
/*---------------------------------------------------------
	�g5�ʃ{�X ��� �u�ʏ�U��1(1/2)�v�ɂ�����Ƃ��������e��(�\��)
	-------------------------------------------------------
	24way =(�E12way)+(��12way)
	-------------------------------------------------------

---------------------------------------------------------*/
#if 1
//static void enemy_boss05_more_angle(SPRITE *s, int speed256, int angle4096/*, int a_angle4096, int kaiten_houkou*/)
static void danmaku_create_04_pink_hearts(SPRITE *s)
{
	BOSS99_DATA *data = (BOSS99_DATA *)s->data;
	if (0==((data->boss_base.danmaku_time_out)&0x0f))/* �{���� 8 �e(128==0x80==danmaku_time_out) */
	{
		play_voice_auto_track(VOICE11_SAKUYA_E_SHOT00);
	//
		int i;
		for (i=0; i<(13*12)/*144*/; i+= 13 )/*12����*/		/* �p�x(128[(90+a)/360�x]�� 10 ����) */
		{
			bullet_create_n_way_dan_sa_type(s,/* �{���͔��e�ʒu�� x offset ���� */
				((160-i)<<(1+1)),		/* �e�� */
				(i/*(0+i)&(512-1)*/),
				(int)(512/(7*4)),		/* �_�~�[�p�x(���g�p) */
				BULLET_KNIFE20_06_YUKARI,
				(1));/* [1way�s���N�i�C�t�e] */
			bullet_create_n_way_dan_sa_type(s,
				((160-i)<<(1+1)),		/* �e�� */
				((0-i)&(512-1)),
				(int)(512/(7*4)),		/* �_�~�[�p�x(���g�p) */
				BULLET_KNIFE20_06_YUKARI,
				(1));/* [1way�s���N�i�C�t�e] */
		}
	}
}
#endif
/*---------------------------------------------------------
	�g5�ʃ{�X ��� �u�ʏ�U��1(2/2)�v�ɂ�����Ƃ��������e��(�\��)
	-------------------------------------------------------
	-------------------------------------------------------

---------------------------------------------------------*/
#if 1
//static void enemy_boss05_more_angle(SPRITE *s, int speed256, int angle4096/*, int a_angle4096, int kaiten_houkou*/)
static void danmaku_create_05_32way_dual(SPRITE *s)
{
	BOSS99_DATA *data = (BOSS99_DATA *)s->data;
	if (0==((data->boss_base.danmaku_time_out)&0x3f))
	{
		play_voice_auto_track(VOICE11_SAKUYA_E_SHOT00);
	//
		int i;
		for (i=0; i<(512); i+= 16 )/*32����*/	/* �p�x(512[360/360�x]�� 32 ����) */
		{
			bullet_create_n_way_dan_sa_type(s,
				(t256(2.0)-((i&16)<<4)),	/*��͍����e*/
				(i/*(0+i)&(512-1)*/),
				(int)(512/(7*4)),		/* �_�~�[�p�x(���g�p) */
				BULLET_KNIFE20_04_AOI,
				(1));/* [1way�i�C�t�e] */
		}
	}
}
#endif

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
	32way �Œ�A�e  x 4��
---------------------------------------------------------*/
#if 1
static void danmaku_create_06_luna_clock_32way(SPRITE *s)
{
	BOSS99_DATA *data = (BOSS99_DATA *)s->data;
	if (0x40==((data->boss_base.danmaku_time_out)&0xcf))/* 4�� */
	{
		play_voice_auto_track(VOICE11_SAKUYA_E_SHOT00);
	//
		bullet_create_n_way_dan_sa_type(s,
			(t256(2.0)),			/* �e�� */
			(0),					/* ���˒��S�p�x / ����@�\(���@�_��/��) */
			(int)(512/(32)),		/* �p�x(512[360/360�x]�� 32 ����) */	/* 1����n���������p�x */
			BULLET_CAP16_05_SIROI,	/* �e�O�� */
			(32));/* [32way���Ēe] */ 	/* ���e�� */
	}
}
#endif
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
#if 1
static void danmaku_create_07_80way_dual_five(SPRITE *s)
{
	BOSS99_DATA *data = (BOSS99_DATA *)s->data;
	if (0==((data->boss_base.danmaku_time_out)&0x3f))
	{
		play_voice_auto_track(VOICE11_SAKUYA_E_SHOT00);
	//
		int ii;
		int j;	/* �J�E���^ */
		j=0;
		for (ii=0; ii<(0x10000); ii += 819 )/* 819.2(65536/80)  6.4==(512/80) 80���� [5����] */	/* 102.4==(512/5) �p�x(512[72/360�x]�� 5 ����) */
		{
			j++;
			j &= (16-1);/* 16way(8way[hi to 0]+8way[slow to 0]) */
			bullet_create_n_way_dan_sa_type(s,
				(t256(3.0)-((j)<<5)-((j&1)<<7)),	/*��͍����e*/
				(((0-ii)>>7)&(512-1)),	/* CW */
				(int)(512/(7*4)),		/* �_�~�[�p�x(���g�p) */
				BULLET_KNIFE20_04_AOI,
				(1));/* [1way�i�C�t�e] */
		}
	}
}
#endif
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

//static void enemy_boss05_more_angle(SPRITE *s, int speed256, int angle4096/*, int a_angle4096, int kaiten_houkou*/)
static void danmaku_create_08_sentakki(SPRITE *s)
{
	BOSS99_DATA *data = (BOSS99_DATA *)s->data;
	if (0==((data->boss_base.danmaku_time_out)&0x03))
	{
		play_voice_auto_track(VOICE11_SAKUYA_E_SHOT00);
	//
		data->boss_base.tmp_angleCCW512 -= (int)(512/24);
		mask512(data->boss_base.tmp_angleCCW512);
		{
			bullet_create_n_way_dan_sa_type(s,
				(t256(2.0)+((data->boss_base.danmaku_time_out)<<2)/*+aa*/), 	/* �e�� */
				(data->boss_base.tmp_angleCCW512/*+aa*/),
				(int)(512/24),		/* �p�x */
				BULLET_UROKO14_00_AOI,
				(8));/* [8way�ؒe] */
			bullet_create_n_way_dan_sa_type(s,
				(t256(2.0)+((data->boss_base.danmaku_time_out)<<2)/*+aa*/), 	/* �e�� */
				((-data->boss_base.tmp_angleCCW512/*+aa*/)&(512-1)),
				(int)(512/24),		/* �p�x */
				BULLET_UROKO14_04_MIZUIRO,
				(8));/* [8way���ؒe] */
		}
	}
}
/*---------------------------------------------------------
	���ʒe��������
	-------------------------------------------------------
---------------------------------------------------------*/
/*static*/ void danmaku_generator(SPRITE *s)
{
	BOSS99_DATA *data=(BOSS99_DATA *)s->data;
	if (DANMAKU_00 != data->boss_base.danmaku_type) 	/* �e�������͕K�v�H */
	{
		data->boss_base.danmaku_time_out -= 1/*fps_fa ctor*/;	/* ���Ԍo�߂���  */
		if (0 < data->boss_base.danmaku_time_out)				/* ���Ԑ؂�H */
		{;}
		else
		{
			data->boss_base.danmaku_type = DANMAKU_00;	/* �e�������I�� */
		}
		/* �e�������� */
		switch (data->boss_base.danmaku_type)
		{
	//	case DANMAKU_00:									break;	/* �e���������Ȃ� */
//	���
		case DANMAKU_01:	danmaku_create_01_misogi(s);			break;	/* �S�e�� / �g5�ʒ��{�X ��� �u�ʏ�U���v */
		case DANMAKU_02:	danmaku_create_02_24nerai(s);			break;	/* 24�e�� (��p�u�~�X�f�B���N�V�������ǂ�(1/2)�v) */
		case DANMAKU_03:	danmaku_create_03_11nife(s);			break;	/* 11�e�� (��p�u�~�X�f�B���N�V�������ǂ�(2/2)�v) */
		case DANMAKU_04:	danmaku_create_04_pink_hearts(s);		break;	/* �g5�ʃ{�X ��� �u�ʏ�U��1(1/2)�v�ɂ�����Ƃ��������e��(�\��) */
		case DANMAKU_05:	danmaku_create_05_32way_dual(s);		break;	/* �g5�ʃ{�X ��� �u�ʏ�U��1/2(2/2)�v�ɂ�����Ƃ��������e��(�\��) */
		case DANMAKU_06:	danmaku_create_06_luna_clock_32way(s);	break;	/* �g5�ʃ{�X ��� ���ہu���i�N���b�N(1/2)�v�ɂ�����Ƃ��������e��(�\��) */
		case DANMAKU_07:	danmaku_create_07_80way_dual_five(s);	break;	/* �g5�ʃ{�X ��� �u�ʏ�U��3(1/2)�v�ɂ�����Ƃ��������e��(�\��) */
//	�A���X(��)
		case DANMAKU_08:	danmaku_create_08_sentakki(s);			break;	/* ����@�e�� */
		case DANMAKU_09:	danmaku_create_04_pink_hearts(s);		break;	/* ��� �e�X�g */
		case DANMAKU_10:	danmaku_create_08_sentakki(s);			break;	/* ����@�e�� */
		case DANMAKU_11:	danmaku_create_05_32way_dual(s);		break;	/* ��� �e�X�g */
		case DANMAKU_12:	danmaku_create_08_sentakki(s);			break;	/* ����@�e�� */
		case DANMAKU_13:	danmaku_create_06_luna_clock_32way(s);	break;	/* ��� �e�X�g */
		case DANMAKU_14:	danmaku_create_08_sentakki(s);			break;	/* ����@�e�� */
		case DANMAKU_15:	danmaku_create_07_80way_dual_five(s);	break;	/* ��� �e�X�g */
		case DANMAKU_16:	danmaku_create_08_sentakki(s);			break;	/* ����@�e�� */
		}
	}
}

