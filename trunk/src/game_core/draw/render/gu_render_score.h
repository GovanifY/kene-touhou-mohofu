
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�X�R�A�֘A(��������_�������Ȋ����Ő����\��)
---------------------------------------------------------*/

/* �X�R�A�L���b�V���ő�4096���� */
#define MAX_SCORE_CHACHE (0x1000)

/* ��\���̎��� */
#define SCORE_DELETE (0)

/* �X�R�A�L���b�V���̃f�[�^�[�`�� */
typedef struct
{
	u32 time_out_color8888; /* �\������ / �\���A���t�@�l */
	int number; 	/* �\�������ԍ�(0 ... 9) [�ꌅ�̐���] */
	int x256;		/* �\�� x ���W(256�Œ菬���_�`��) */
	int y256;		/* �\�� y ���W(256�Œ菬���_�`��) */
} SCORE_CHACHE_DATA;

static SCORE_CHACHE_DATA score_number_chache[MAX_SCORE_CHACHE];


/*---------------------------------------------------------
	�X�R�A�L���b�V�����ׂď���
---------------------------------------------------------*/

void clear_score_chache(void)
{
	unsigned int i;
	for (i=0; i<MAX_SCORE_CHACHE; i++)
	{
		score_number_chache[i].time_out_color8888 = SCORE_DELETE;
	}
}

/*---------------------------------------------------------
	�ꌅ�̐������A�L���b�V���ɓo�^����
---------------------------------------------------------*/

static void regist_score(int number, u32 color8888, int x256, int y256)
{
	if (t256((GAME_X_OFFSET-8)) > x256) 	{ return; } 	/* (8)? (���[��荶�Ȃ�`���Ȃ�) */
	if (t256((GAME_WIDTH-8)) < x256)		{ return; } 	/* (8)? (�E�[���E�Ȃ�`���Ȃ�) */
	if (t256((GAME_HEIGHT-8)) < y256)		{ return; } 	/* (6)? (���܂艺�Ȃ�`���Ȃ�) */
//
	static unsigned int index = 0;	/* �o�^�o�������Ȉʒu */
//	int iii;
//	iii = 0;	/* �ő�o�^���܂őS���T�� */
//	do
//	{
		index++;
		index &= (MAX_SCORE_CHACHE-1);
		/* �g�p�� */
//		if (SCORE_DELETE < score_number_chache[index].time_out_color8888)
//		{
//			;	/* �o�^�ł��Ȃ��̂Ŏ���T���B */
//		}
//		/* ���g�p */
//		else //if (1 > score_number_chache[index].time_out_color8888)
		{
			/* �L���b�V���ɓo�^���� */
			score_number_chache[index].time_out_color8888 = color8888;//(127*2);/*	60*2*2 =:= 2 [sec]*/
			score_number_chache[index].number	= number;
			score_number_chache[index].x256 	= x256;
			score_number_chache[index].y256 	= y256;
//			iii = MAX_SCORE_CHACHE; 	/* ���������� */
		}
//		iii++;
//	} while (MAX_SCORE_CHACHE > iii);	/* �S���T�����H */
}


/*---------------------------------------------------------
	�X�R�A���ꌅ�̐����ɕ������A�L���b�V���ɓo�^����
---------------------------------------------------------*/

static void bonus_info_shered_add_score10_value(OBJ/**/ *src, s32 score_value)
{
	int y256;	y256 = src->center.y256;
	if (t256((0)) > y256)	{ return; }
//
	int x256;	x256 = src->center.x256;
	int jjj;
	u32 color8888;
	int i;
	jjj=0;
	color8888 = 0xffffffff;
	/* ���Z�X�R�A�� 999990�_ �ȏ�̏ꍇ�� 999990�_ �̕\���ɂ���(�\���̂�999990�_�Ŏ��ۂ́A�����Ƃ��̕����Z�����) */
//
			  if (99999 < score_value)	{	score_value = 99999;	}	/* MAX 999990[pts] */
	i = 0; while ( 9999 < score_value)	{	score_value -= 10000; i++; jjj=1;	}; if (0!=jjj)	{	color8888=0xff00ffff;	regist_score(i, color8888, (x256), y256); } x256 += t256(8);/* 100000 �\�� */
	i = 0; while (	999 < score_value)	{	score_value -=	1000; i++; jjj=1;	}; if (0!=jjj)	{	color8888=0xff00ffff;	regist_score(i, color8888, (x256), y256); } x256 += t256(8);/* 10000  �ꖜ */
	i = 0; while (	 99 < score_value)	{	score_value -=	 100; i++; jjj=1;	}; if (0!=jjj)	{							regist_score(i, color8888, (x256), y256); } x256 += t256(8);/* 1000 	�� */
	i = 0; while (	  9 < score_value)	{	score_value -=	  10; i++; jjj=1;	}; if (0!=jjj)	{							regist_score(i, color8888, (x256), y256); } x256 += t256(8);/* 100		�S */
	i = 0; while (	  0 < score_value)	{	score_value--;		  i++; jjj=1;	}; if (0!=jjj)	{							regist_score(i, color8888, (x256), y256); } x256 += t256(8);/* 10		�\ */
//
	regist_score(0, color8888, (x256), y256);/* "0" �X�R�A�̖����͕K���O */
}


/*---------------------------------------------------------
	������A�C�e���̓��_������\��(�C�ӓ��_�̏ꍇ)
---------------------------------------------------------*/

void bonus_info_any_score_nodel(OBJ/**/ *src, u32 score_num_pts)/*int x, int y*/
{
	player_dummy_add_score(score_num_pts);
	bonus_info_shered_add_score10_value(src, (s32)score_num_pts);
}


/*---------------------------------------------------------
	������A�C�e���̓��_������\��(�Œ蓾�_�̏ꍇ)
---------------------------------------------------------*/

void bonus_info_score_nodel(OBJ/**/ *src, int score_type)/*int x, int y*/
{
	static const unsigned short score_tbl[32] =
	{
		score(76800), score(   10), score(	 20), score(   30),
		score(	 40), score(   50), score(	 60), score(   70),
		score(	 80), score(   90), score(	100), score(  200),
		score(	300), score(  400), score(	500), score(  600),
		//
		score(	700), score(  800), score(	900), score( 1000),
		score( 2000), score( 3000), score( 4000), score( 5000),
		score( 6000), score( 7000), score( 8000), score( 9000),
		score(10000), score(11000), score(12000), score(51200),
	};
	int score_num;	score_num = score_tbl[(score_type)/*&(32-1)*/];
	bonus_info_any_score_nodel(src, score_num);
}


