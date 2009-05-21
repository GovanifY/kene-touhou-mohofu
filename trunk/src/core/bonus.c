
/*---------------------------------------------------------
	�A�C�e���֘A
---------------------------------------------------------*/

#include "support.h"
#include "bonus.h"


/*---------------------------------------------------------
	�A�C�e���̈ړ�
---------------------------------------------------------*/

/* �X�̃A�C�e���̃X�e�[�^�X�f�[�^�[ */
typedef struct
{
	#if 1
	int strength;/*union WEAPON_BASE*/		/*�Ȃ񂩃A�C�e���ɎE�����̂Œǉ��H�H�H*/
	#endif
	/*dou ble*/int y_sum256;	/* �A�C�e�������o���p y�� �ώZ�l(y���A������}�C�i�X) */
	/*dou ble*/int angle512;
	int flags00;				// ���W�t���O	// [***090116		�ύX
//	�������W
	int flag_first; 			/* first�t���O */
	/*dou ble*/int x_sa256; 	/* ���� x */
	/*dou ble*/int y_sa256; 	/* ���� y */
} ITEM_DATA;
extern int effect_sound_hosi;
static void move_items(SPRITE *src)
{
	ITEM_DATA *data = (ITEM_DATA *)src->data;
	/* �������W���[�h�̂ǂꂩ���쓮���Ă���A */
	if (0 != (
		(((PLAYER_DATA *)player->data)->state_flag) &
		(STATE_FLAG_01_PLAYER_UP_AUTO_GET_ITEM |		/* MAX���̏��� */
		 STATE_FLAG_02_BOMB_AUTO_GET_ITEM | 			/* �{���������̂݉�� */
		 STATE_FLAG_03_SCORE_AUTO_GET_ITEM				/* �{�X���j���̈ꎞ��� */
		) ))
	{
		data->flags00 |= ITEM_MOVE_FLAG_01_COLLECT/*1*/;	/* ���̃A�C�e���͎������W�ɂ��� */
	}
	/* �������W�łȂ���΁A�P���ɕ�������`���ė����� */
	if (0 == ((data->flags00)&ITEM_MOVE_FLAG_01_COLLECT))
	{
		if (data->y_sum256 < t256(3) )			// [***090123.0220	�ύX5=>4=>3
		{
			data->y_sum256 += data->angle512;		/* x1.5 */
		}
		src->y256 += (/*t256_to_dou ble*/(data->y_sum256))/**fps_fa ctor*/;
		if (src->y256 > GAME_HEIGHT*256)
		{
			((PLAYER_DATA *)player->data)->chain_point = 0;
			src->type = SP_DELETE;				/* ��ʊO�ɏo���炨���܂� */
		}
	}
	else	/* �������W�Ȃ�΁A�����ɏW�܂� */
	{
		data->y_sum256--;
		if (1 > data->y_sum256)
		{
			data->y_sum256 = 0;
		}
		if (0 == data->flag_first)
		{
			data->flag_first = 1;
			data->y_sum256 = t256(1.0); 	/*5*0.5*/
		}
		data->x_sa256 = (src->x256 - player->x256);
		data->y_sa256 = (src->y256 - player->y256);
		/*�����ɏW�܂�*/
		int aaa_x256;
		int aaa_y256;
		aaa_x256 = ((data->x_sa256 * data->y_sum256)>>8); 	/**fps_fa ctor*/
		aaa_y256 = ((data->y_sa256 * data->y_sum256)>>8); 	/**fps_fa ctor*/
		if ( (SP_ITEM_05_HOSI) == (src->type))	/* ���_�̂ݓ��ʏ��� */
		{
			if (
				//	(/*10*/64/*16*/ > data->y_sum256) ||
				(
					(t256(8) > abs(aaa_x256)) &&
					(t256(8) > abs(aaa_y256))
				)
			)
			{
				src->type = SP_DELETE;/* ���_�̂ݓ��ʏ��� */				/* �����܂� */
				/*effect_sound_number=*/ effect_sound_hosi=1;/*play_voice_auto_track(VOICE05_BONUS);*/
			}
		}
		src->x256 = player->x256 + (aaa_x256); 	/**fps_fa ctor*/
		src->y256 = player->y256 + (aaa_y256); 	/**fps_fa ctor*/
	}
}

/*---------------------------------------------------------
	�A�C�e���o��������q�֐�
---------------------------------------------------------*/

static SPRITE *item_mono_create(SPRITE *src/*int x, int y*/, int sel_type)
{
//	�A�C�e���̎�ނ�I��
	SPRITE *s;
//	s			= sprite_add_file 0("bonus_items.png", 8, PRIORITY_04_ITEM, 0); 	s->anim_speed=0;
	s			= sprite_add_res(BASE_BONUS_ITEMS_PNG);
	sel_type &= 0x07;
	s->anim_frame = ((/*SP_ITEM_FIRE_POWER-*/sel_type)/*&0x07*/);
//	�o���ʒu�����߂�
	#define OFFSET_X64		(32*256)/* 64�͂��炵�� 2��n��̕K�v�L�� */
	#define ITEM_WIDTH16	(16*256)/* 16�̓A�C�e���� */
	#define ITEM_X_LIMIT	(GAME_WIDTH*256+OFFSET_X64-ITEM_WIDTH16)
	int x256;
	x256 = src->x256;
	x256 += ((ra_nd()&((OFFSET_X64+OFFSET_X64)-1)));
	if (x256 < OFFSET_X64)
	{
		x256 = OFFSET_X64;
	}
	else
	if (x256 > (ITEM_X_LIMIT))
	{
		x256 = (ITEM_X_LIMIT);
	}
//	�o�^����
	s->x256 			= (x256)-(OFFSET_X64);
	s->y256 			= src->y256;
//	sel_type &= 0x07;
	sel_type |= SP_ITEM_00_P001;
	s->type 			= sel_type;
	s->callback_mover	= move_items;
	if ( SP_ITEM_05_HOSI == sel_type )	/* ���_�̂ݓ��ʏ��� */
	{
		/* �g�́A�����炵�� */
		if ( USER_BOMOUT_WAIT > ((PLAYER_DATA *)player->data)->bomber_time )
		{	/* �{����(�ݒ薳�G���Ԓ�)��100pts.(�҂��Ȃ�) */
			bonus_info_score_nodel(s, SCORE_100);/*���������֎d�l�ύXs->type=SP_DELETE;*/
		}
		else/*���_*/
		{	/* �{����̎������G���Ԓ��͂�����(�҂���) */
			/* ((graze/3)*10)+(500) pts */
			bonus_info_any_score_nodel(s, (score(500)+(((((PLAYER_DATA *)player->data)->graze_point*86)>>8)/*(pd->graze_point/3)*/)) );/*���������֎d�l�ύXc->type=SP_DELETE;*/
		}
		s->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);/*�����蔻��Ȃ�*/
	}
	else
	{
		s->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	}
	return (s);
	#undef OFFSET_X64
	#undef ITEM_WIDTH16
	#undef ITEM_X_LIMIT
}

/*---------------------------------------------------------
	�A�C�e����o�^���ďo��������
---------------------------------------------------------*/

/* [***090125	�ǉ�: up_flags �� ITEM_MOVE_FLAG_01_COLLECT �r�b�g���I���Ńv���C���[�ɏW�܂�܂��B */
void item_create(SPRITE *src/*int x, int y*/, int item_type, int num_of_creates, int up_flags)
{
	int i;
	for (i=0; i<num_of_creates; i++)
	{
		SPRITE *s;
		s			= item_mono_create(src, item_type);
		if (NULL==s) return;
//
		ITEM_DATA *data;
		data				= mmalloc(sizeof(ITEM_DATA));
		s->data 			= data;
		data->strength		= 0;		/*�Ȃ񂩃A�C�e���ɎE�����̂Œǉ��H�H�H*/
		/* ��ʉ����ɋ�����A�����o�����x�𑬂����� */
	//	data->y_sum256		= t256(-5*0.5); 	//������ƕύX
	//	data->y_sum256		= t256(-2*0.5); 	/* �A�C�e�������o�������l(y���A������}�C�i�X) */
	//	data->y_sum256		= -(/*256*/((int)(src->y))<<1)-(ra_nd()&0xff);	/* �A�C�e�������o�������l(y���A������}�C�i�X) */
		data->y_sum256		= -(/*256*/((src->y256)>>7))-(ra_nd()&0xff);	/* �A�C�e�������o�������l(y���A������}�C�i�X) */
		data->angle512		= 6;	/* 6.51898646904403967309077986986488 ra d2deg512(0.08);*/
		data->flags00		= (up_flags&ITEM_MOVE_FLAG_01_COLLECT);
		data->flag_first	= 0;
	}
}

/*---------------------------------------------------------
	�{�X�p�A�C�e���쐬
---------------------------------------------------------*/
//	#define ITEM_CREATE_MODE_01 	(12*0)
//	#define ITEM_CREATE_MODE_02 	(12*1)
void item_create_for_boss(SPRITE *src, int item_create_mode)
{
	#define ITEM_01 	(4*0)
	#define ITEM_02 	(4*1)
	#define ITEM_03 	(4*2)
	#define ITEM_MAX	(4*3)
	const signed short item_tbl[ITEM_MAX*2] =
	{	/* easy, normal, hard, lunatic */
	/*mode 0: ���j�� */
		SP_ITEM_00_P001,	SP_ITEM_02_BOMB,	SP_ITEM_01_P008,	SP_ITEM_06_TENSU,
		SP_ITEM_04_P128,	SP_ITEM_01_P008,	SP_ITEM_02_BOMB,	SP_ITEM_01_P008,
		SP_ITEM_03_1UP, 	SP_ITEM_00_P001,	SP_ITEM_02_BOMB,	SP_ITEM_02_BOMB,
	/*mode 1: ���j�� */
		SP_ITEM_01_P008,	SP_ITEM_01_P008,	SP_ITEM_01_P008,	SP_ITEM_06_TENSU,
		SP_ITEM_00_P001,	SP_ITEM_00_P001,	SP_ITEM_00_P001,	SP_ITEM_01_P008,
		SP_ITEM_02_BOMB,	SP_ITEM_00_P001,	SP_ITEM_00_P001,	SP_ITEM_00_P001,
	};
	item_create(src, item_tbl[		   difficulty+item_create_mode]/*SP_ITEM_01_P008 */, player_now_stage/*5*/, ITEM_MOVE_FLAG_06_RAND_XY);
	item_create(src, item_tbl[ ITEM_02+difficulty+item_create_mode]/*SP_ITEM_04_P128 */, 1, ITEM_MOVE_FLAG_06_RAND_XY);
	item_create(src, item_tbl[ ITEM_03+difficulty+item_create_mode]/*SP_ITEM_03_1UP  */, 1, ITEM_MOVE_FLAG_06_RAND_XY);
	item_create(src, SP_ITEM_05_HOSI, 16/*7*/, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY) );/*���_���o��*/
}
//	item_create(sb03[i], SP_ITEM_05_HOSI, 16/*7*/, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY) );/*���_���o��*/
//	item_create(sb01[i], SP_ITEM_05_HOSI, 16/*7*/, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY) );/*���_���o��*/
//
//	item_create(s, SP_ITEM_05_HOSI, 16/*7*/, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY) );/*���_���o��*/
//	item_create(s, SP_ITEM_00_P001, 5, ITEM_MOVE_FLAG_06_RAND_XY);/*3*/
//	item_create(s, SP_ITEM_01_P008, 1, ITEM_MOVE_FLAG_06_RAND_XY);
//
//	item_create(s, SP_ITEM_05_HOSI, 5, ITEM_MOVE_FLAG_01_COLLECT);/*���_���o��*/
//	item_create(s, SP_ITEM_00_P001, 5, ITEM_MOVE_FLAG_06_RAND_XY);
//	item_create(s, SP_ITEM_01_P008, 1, ITEM_MOVE_FLAG_06_RAND_XY);
//
//	/* �ނ������̂�F�Ǝc�@�o���ׂ� */
//	item_create(s, SP_ITEM_01_P008, 	5, ITEM_MOVE_FLAG_06_RAND_XY);
//	item_create(s, SP_ITEM_04_P128, 	1, ITEM_MOVE_FLAG_06_RAND_XY);
//	item_create(s, SP_ITEM_03_1UP,		1, ITEM_MOVE_FLAG_06_RAND_XY);
//
//	item_create(sb01[i], SP_ITEM_COIN,7);

/*---------------------------------------------------------
	���Ă̓G�e�������āA�w��A�C�e���ɕς���
---------------------------------------------------------*/
extern SPRITE *sprite_list000_head;
void item_from_bullets(int put_item_num)
{
	SPRITE *s = sprite_list000_head;/* �X�v���C�g ���X�g�̐擪 ����T�� */
	while (NULL != s)/* �X�v���C�g ���X�g�̍Ō�܂Œ��ׂ� */
	{
		if (/*SP_BULLET*/SP_GROUP_BULLETS & s->type)
		{
			item_create(s, put_item_num/*SP_ITEM_05_HOSI*/, 1, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY) );
			s->type = SP_DELETE;
		}
		s = s->next;/*��*/
	}
}

/*-------------------------------------------------------*/
/*-------------------------------------------------------*/
/*-------------------------------------------------------*/


/*---------------------------------------------------------
	�X�R�A�֘A(��������_�������Ȋ����Ő����\��)
---------------------------------------------------------*/

/* �X�R�A�L���b�V���ő�4096���� */
#define MAX_SCORE_CHACHE (0x1000)

/* ��\���̎��� */
#define SCORE_DELETE (0)

/* �X�R�A�L���b�V���̃f�[�^�[�`�� */
typedef struct
{
	int time_out;	/* �\������ / �\���A���t�@�l */
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
	int i;
	for (i=0; i<MAX_SCORE_CHACHE; i++)
	{
		score_number_chache[i].time_out = SCORE_DELETE;
	}
}

/*---------------------------------------------------------
	screen�T�[�t�F�C�X�ɁA�X�R�A�������_�����O
---------------------------------------------------------*/

//	   void font07_render_scorenum_xy( int time_out, int number, int x256, int y256)
//atic void font07_render_scorenum_xy(SCORE_CHACHE_DATA *aaa){}
void draw_score_chache(void)
{
//				SDL_Rect s,d;
//				s.w=(8); d.w=(8);
//				s.h=(8); d.h=(8);
	if (SDL_MUSTLOCK(screen))	{	SDL_LockSurface(screen);	}/*���b�N����*/
	int i;
	for (i=0; i<MAX_SCORE_CHACHE; i++)
	{
		SCORE_CHACHE_DATA *aaa;
		aaa = &(score_number_chache[i]);
		if (SCORE_DELETE < aaa->time_out)
		{
			aaa->time_out -= 2;
			{
			//	font07_render_scorenum_xy(
				//	(aaa->time_out),
				//	(aaa->number),
				//	(aaa->x256)/*dx*/,
				//	(aaa->y256)/*dy*/
			//		&(score_number_chache[i])
			//		);
//				s.x = ((aaa->number)<<3)+(8);
//				s.y = 0;
//				d.x = ((aaa->x256)>>8);
//				d.y = ((aaa->y256)>>8)/*+((aaa->time_out)>>5) ��Ɉړ�*/;
//				#if 0
//				/*�x��*/
//				SDL_SetAlpha(font07_img,SDL_SRCALPHA,(aaa->time_out));
//				#endif
//				SDL_BlitSurface(font07_img,&s,screen,&d);
				Uint8 ddd;
				Uint16 alpha256;	alpha256 = (aaa->time_out);
				int iii;	iii = (aaa->number);

				Uint16 *pd;
				{
					Uint32 y2562 = ((aaa->y256)&0x1ff00);
					pd = (Uint16 *)screen->pixels + (y2562+y2562) + ((aaa->x256)>>8);/*x 512/256 */
				}
				int kkk;
				int yyy;
				for (yyy=0; yyy<6; yyy++)
				{
					#define v (0)
					#define M (1)
					#define f8(a,b,c,d, e,f,g,h) ((a<<0)|(b<<1)|(c<<2)|(d<<3)|(e<<4)|(f<<5)|(g<<6)|(h<<7))
					static const Uint8 score_font_08x05[/*(6*10)*/(64)] =
					{
f8(v,v,v,M,M,v,v,v),f8(v,v,v,v,v,v,v,v),f8(v,v,v,v,v,v,v,v),f8(v,v,v,v,v,v,v,v),f8(v,v,v,v,v,v,v,v),f8(v,M,M,M,M,M,v,v),f8(v,v,v,M,v,v,v,v),f8(v,M,v,v,v,v,v,v),f8(v,v,M,M,v,v,v,v),f8(v,v,M,v,v,v,v,v),
f8(v,v,M,v,v,M,v,v),f8(v,v,v,v,v,v,v,v),f8(v,v,v,v,v,v,v,v),f8(v,M,M,M,M,v,v,v),f8(M,M,M,M,M,M,M,v),f8(v,v,v,M,v,v,v,v),f8(v,M,M,M,M,M,M,v),f8(v,M,v,v,M,M,M,v),f8(v,v,v,M,M,v,v,v),f8(v,v,M,M,M,M,v,v),
f8(v,M,v,v,v,v,M,v),f8(v,v,v,v,v,v,v,v),f8(v,v,M,M,M,v,v,v),f8(v,v,v,v,v,v,v,v),f8(M,v,M,v,M,v,M,v),f8(v,v,M,M,M,M,v,v),f8(v,v,v,v,v,v,v,v),f8(M,M,M,M,M,v,v,v),f8(v,v,v,v,M,v,v,v),f8(M,M,M,v,v,M,v,v),
f8(v,M,v,v,v,v,M,v),f8(v,M,M,M,M,M,M,v),f8(v,v,v,v,v,v,v,v),f8(v,v,M,M,v,v,v,v),f8(M,M,M,v,M,M,M,v),f8(v,v,M,v,v,M,v,v),f8(v,v,M,v,v,M,v,v),f8(v,M,v,v,v,v,v,v),f8(v,M,v,v,M,M,v,v),f8(v,v,M,v,M,M,v,v),
f8(v,v,M,v,v,M,v,v),f8(v,v,v,v,v,v,v,v),f8(v,v,v,v,v,v,v,v),f8(v,v,v,v,v,v,v,v),f8(M,v,v,v,v,v,M,v),f8(v,M,M,v,M,M,v,v),f8(v,M,M,v,v,M,M,v),f8(v,M,v,v,v,v,v,v),f8(v,M,v,v,v,M,M,v),f8(v,M,M,v,M,v,v,v),
f8(v,v,v,M,M,v,v,v),f8(v,v,v,v,v,v,v,v),f8(M,M,M,M,M,M,M,v),f8(M,M,M,M,M,M,M,v),f8(M,M,M,M,M,M,M,v),f8(M,M,M,M,M,M,M,v),f8(M,M,v,v,v,v,M,M),f8(v,v,M,M,M,M,M,v),f8(M,v,v,v,v,v,M,M),f8(M,M,v,v,v,M,M,M),
						/*dummy*/0,0,0,0
					};
					#undef v
					#undef M
					#undef f8
					kkk = 1;
					ddd = score_font_08x05[iii];
					int xxx;
					for (xxx=0; xxx<8; xxx++)
					{
						if (ddd & (kkk) )
						{	/* �ėp�]��(�A���t�@�C��) */
						#define bgRGB (*pd)
						#define fgRGB (0xffff)
						(*pd) = (Uint16)MAKECOL16(
							(((GETR16F(fgRGB) * (alpha256)) + (GETR16F(bgRGB) * (256 - alpha256))) >> 8),
							(((GETG16F(fgRGB) * (alpha256)) + (GETG16F(bgRGB) * (256 - alpha256))) >> 8),
							(((GETB16F(fgRGB) * (alpha256)) + (GETB16F(bgRGB) * (256 - alpha256))) >> 8)	);
						}
						pd++;
						kkk += kkk;
					}
					pd += (512-8);
					iii += 10;
				}
			}
		}
	}
	if (SDL_MUSTLOCK(screen))	{	SDL_UnlockSurface(screen);	}/*���b�N����*/
}

/*---------------------------------------------------------
	�ꌅ�̐������A�L���b�V���ɓo�^����
---------------------------------------------------------*/

static void regist_score(int number, int x256, int y256)
{
	if (t256(380) < x256)	{ return; }
//
	static int index=0; 	/* �o�^�o�������Ȉʒu */
//	int iii;
//	iii = 0;	/* �ő�o�^���܂őS���T�� */
//	do
//	{
		index++;
		index &= (MAX_SCORE_CHACHE-1);
		/* �g�p�� */
//		if (SCORE_DELETE < score_number_chache[index].time_out)
//		{
//			;	/* �o�^�ł��Ȃ��̂Ŏ���T���B */
//		}
//		/* ���g�p */
//		else //if (1 > score_number_chache[index].time_out)
		{
			/* �L���b�V���ɓo�^���� */
			score_number_chache[index].time_out = (127*2);/*	60*2*2 =:= 2 [sec]*/
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

static void bonus_info_shered_add_score10_value(SPRITE *src, int score_value)
{
	int y256;	y256 = src->y256;
	if (t256((0)) > y256)	{ return; }
//
	int x256;	x256 = src->x256;
	int jjj;
	int i;
	jjj=0;
	/* 999990�_�ȏ��999990�_�̕\���ɂ��� */
	if (99999 < score_value) {	score_value = 99999;  }   /* MAX 999990[pts] */
	i = 0; while ( 9999 < score_value) { score_value -= 10000; i++; jjj=1; }; if (0!=jjj) { regist_score(i, x256, y256); }	x256 += t256(8);
	i = 0; while (	999 < score_value) { score_value -=  1000; i++; jjj=1; }; if (0!=jjj) { regist_score(i, x256, y256); }	x256 += t256(8);
	i = 0; while (	 99 < score_value) { score_value -=   100; i++; jjj=1; }; if (0!=jjj) { regist_score(i, x256, y256); }	x256 += t256(8);
	i = 0; while (	  9 < score_value) { score_value -=    10; i++; jjj=1; }; if (0!=jjj) { regist_score(i, x256, y256); }	x256 += t256(8);
	i = 0; while (	  0 < score_value) { score_value--; 	   i++; jjj=1; }; if (0!=jjj) { regist_score(i, x256, y256); }	x256 += t256(8);
	regist_score(0, x256, y256);/* "0" �X�R�A�̖����͕K���O */
}

void player_add_score(int score_num)
{
	((PLAYER_DATA *)player->data)->my_score += score_num;
	/* �G�N�X�e���h�`�F�b�N */

}
/*---------------------------------------------------------
	������A�C�e���̓��_������\��(�C�ӓ��_�̏ꍇ)
---------------------------------------------------------*/

void bonus_info_any_score_nodel(SPRITE *src/*int x, int y*/, int score_num)
{
	player_add_score(score_num);
	bonus_info_shered_add_score10_value(src, score_num);
}

/*---------------------------------------------------------
	������A�C�e���̓��_������\��(�Œ蓾�_�̏ꍇ)
---------------------------------------------------------*/

void bonus_info_score_nodel(SPRITE *src/*int x, int y*/, int score_type)
{
	static const unsigned short score_tbl[32] =
	{
		score(	 10), score(   20), score(	 30), score(   40),
		score(	 50), score(   60), score(	 70), score(   80),
		score(	 90), score(  100), score(	200), score(  300),
		score(	400), score(  500), score(	600), score(  700),
		//
		score(	800), score(  900), score( 1000), score( 2000),
		score( 3000), score( 4000), score( 5000), score( 6000),
		score( 7000), score( 8000), score( 9000), score(10000),
		score(11000), score(12000), score(51200), score(76800),
	};
	int score_num;	score_num = score_tbl[(score_type)/*&(32-1)*/];
	bonus_info_any_score_nodel(src, score_num);
}

