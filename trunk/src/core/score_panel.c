
/*---------------------------------------------------------
	�X�R�A�p�l��(�\��)�֘A
---------------------------------------------------------*/

#include "game_main.h"
#include "bullet_object.h"

static SDL_Surface *panel_base; 	// �p�l���x�[�X
static SDL_Surface *star_gauge;
//static SDL_Surface *power_gauge;	// �p���[�Q�[�W�B���̊�����loadbmp2�ō��ƕ��ʂɕ\�������悤�ɂȂ����B
//static SDL_Surface *boss_gauge; 	// [***090305		�ǉ�:�{�XHP�Q�[�W

/* �T�C�h�p�l���̉��\���ʒu pannel x offset */
//#define PPP (380)
#define PPP (GAME_WIDTH)
#if 0
	#define STR_MAX 		"MAX"
	#define STR_MISSILE 	"MISSILE"
	#define STR_WEAPON_UP	"WEAPON_UP"
	#define STR_LASER		"LASER"
	#define STR_BOMB		"BOMB"
	#define STR_TIME_		"TIME :"
	#define STR_EXTRA_		"EXTRA  :"
#else
	//#define STR_MAX		"ABCD"			/*4��*/
	//#define STR_MISSILE	"EFGHIJ"		/*6��*/
	//#define STR_WEAPON_UP "KLMNOPQR"		/*8��*/
	//#define STR_LASER 	"STUVW" 		/*5��*/
	//#define STR_BOMB		"XYZab" 		/*5��*/
	//#define STR_TIME_ 	"cdefg" 		/*5��*/
	//#define STR_EXTRA_	"hijkl" 		/*5��*/
//
	#define STR_MAX 		"ABCD"			/*4��*/
	#define STR_EASY		"EFGH" "  " 	/*4��*/
	#define STR_NORMAL		"IJKLM" " " 	/*5��*/
	#define STR_HARD		"NOPQ" "  " 	/*4��*/
	#define STR_LUNATIC 	"RSTUVW"		/*6��*/
	#define STR_EXTRA		"XYZab" 		/*5��*/
	#define STR_TIME_		"cdef"			/*4��*/
//	#define STR_RANK_		"hijkl" 		/*5��*/
	#define STR_FPS_		"hij"			/*3��*/
	#define CHR_PIRIOD_ 	'g' 			/*1��*/
	#define STR_ENEMY		"klmno" 		/*5��*/
#endif


/*---------------------------------------------------------
 * 10�i���\�����s���B
 * �������Ȃ���ʌ��� 0 �� �\�����Ȃ��B
 * ��ɍő包�����̕\���ɂȂ�B
 *
 * @param num [in] �\������l
 * @param size [in] �\������ő包��
 * @param sprite [in] �g�p����X�v���C�g���[�N�̐擪�ԍ�
 * @param x [in] �\������p�l�����ʒu X
 * @param y [in] �\������p�l�����ʒu Y
 * @return ����
 *
---------------------------------------------------------*/
static void dec_display(
		unsigned int num,
		int size,
//		int sprite,
//		int x,
//		int y
		char *my_str
)
{
	int s;
//	int j;
	//unsigned int n;
	unsigned int c;
	const unsigned int sub10[10/*8*/]=
	{
		1,
		10,
		100,
		1000,
		//
		10000,
		100000,
		1000000,
		10000000,
		//
		100000000,
		1000000000//,
	//	2147483647	== 0x7fffffff == signed int 32bit  max
	};
	s=0;
	num++;/*�Ō㌅�̒��덇�킹*/
//	j = 256* (x);
	while (size)
	{
		size--;
		c = 0;
		while (num > sub10[size])
		{
			num -= sub10[size];
			c++;
			s=1;
		}
		if (
			(0==s) //&& /* ��ʌ��� 0 �͕\�����Ȃ� */
			//(0!=size) 	/* 1���ڂ͕K���\������ */
		)
		{
	;// 	obj_01_panel[sprite+size].m_Use 	= FALSE;
		}
		else
		{
	//	//	obj_01_panel[sprite+size].x256			= 256* (x + ((size - 1 - i) * 16/DIV2));
	//		obj_01_panel[sprite+size].x256			= (j);
	//		obj_01_panel[sprite+size].y256			= 256* (y);
	//		obj_01_panel[sprite+size].w 			= TEX1_W_STR_09;
	//		obj_01_panel[sprite+size].h 			= TEX1_H_STR_09;
	//	//	obj_01_panel[sprite+size].m_Priority	= BS_DEPTH_PANEL_ITEM;
	//		obj_01_panel[sprite+size].alpha 		= 255;	/* ���l(255 �ŕs�����A0 �œ���) */
	//		obj_01_panel[sprite+size].texture_x 	= TEX1_X_STR_09+(/*9 **/16* c);
	//		obj_01_panel[sprite+size].texture_y 	= TEX1_Y_STR_09;
	//	//	obj_01_panel[sprite+size].m_TexW		= TEX1_W_STR_09;
	//	//	obj_01_panel[sprite+size].m_TexH		= TEX1_H_STR_09;
	//	//	obj_01_panel[sprite+size].m_RollX		= 0.0;
	//	//	obj_01_panel[sprite+size].m_RollY		= 0.0;
	//		obj_01_panel[sprite+size].m_RollZ		= 0/*.0*/;
	//		obj_01_panel[sprite+size].m_Use 		= TRUE;
	//	//	obj_01_panel[sprite+size].m_PosCenter	= FALSE;
			(*my_str)=('0'+c);
		}
		my_str++;
	//	j += ( 256* (16/DIV2) );
	}
}



/*---------------------------------------------------------
	�v���C���[�̐��̕\���̎q�֐�
---------------------------------------------------------*/

enum
{
	R_00_aka_hosi_png = 0,
	R_01_mizu_hosi_png = 10,
};

static void draw_stars_status(int img_num, int value, int y_offset)
{
//	/*const*/ char *img_name[2] =
//	{
//		"panel/aka_hosi.png",
//		"panel/mizu_hosi.png",
//	};
	SDL_Rect rect_dest;
	SDL_Rect rect_src;
	if (value<1)	{	value=0;}
//
	rect_src.x = (0);
	rect_src.y = (img_num)/*(0)*/;
	rect_src.w = (10*value);
	rect_src.h = (10);
//
	rect_dest.x = PSP_WIDTH480-4-(10*value);
	rect_dest.y = y_offset;
	rect_dest.w = 100;
	rect_dest.h = 11;
	SDL_BlitSurface(star_gauge, &rect_src, sdl_screen[SDL_00_SCREEN], &rect_dest);
}

/*---------------------------------------------------------
	�v���C���[�̃E�F�|���Q�[�W�̕\���̎q�֐�
---------------------------------------------------------*/

// ketm�ɍ������㏑������Ɖ��̂����ʂɕ\�������B
static void draw_power_gauge(int weapon/*, int dx, int dy*/)
{
	#define WP_GAUGE_X_OFS (PPP+48-2)
	#define WP_GAUGE_Y_OFS (128+8+4)
	SDL_Rect rect_dest;
	SDL_Rect rect_src;
	rect_src.x = (0);
	rect_src.y = 20/*(0)*/;
	rect_src.h = 13;
	#if 0
//	rect_src.w = (int)((dou ble)weapon / 127/*128*/ * (dou ble)power_gauge->w); 	// [***090123		�ύX
	#else
	/* 1 dot ���炢�덷���邩������Ȃ����Ǌȗ���(������) */
	rect_src.w = ((int)(weapon * 80/*power_gauge->w*/)>>7);	// [***090123		�ύX
	#endif
	rect_dest.w = 80/*power_gauge->w*/;
	rect_dest.h = 13/*power_gauge->h*/;
	rect_dest.x = WP_GAUGE_X_OFS/*dx*/;
	rect_dest.y = WP_GAUGE_Y_OFS/*dy*/;
	SDL_BlitSurface(star_gauge/*power_gauge*/, &rect_src, sdl_screen[SDL_00_SCREEN], &rect_dest);
}

/*---------------------------------------------------------
	�{�X�� HP �\���̎q�֐�
---------------------------------------------------------*/

extern int boss_x256;
extern int boss_y256;
static int draw_boss_hp_value_set;
static void draw_boss_gauge(void/*int dx, int dy*/) 	// [***090305		�ύX
{
	SPRITE *s = ((PLAYER_DATA *)player->data)->boss;
	if (NULL == s)
	{
		return;/* �{�X�������ꍇ�͉������Ȃ� */
	}
//
	int boss_hp_low_value=(/*((BOSS_BASE *)s->data)->boss_*/s->base_health);/*(???)141477*/
//	int bo ss_life_value=(((BOSS_BASE *)s->data)->bo ss_life);/*(???)141477*/

	#if 0
	if (0 > boss_hp_low_value)	return;/* �����̏ꍇ�͉������Ȃ� */
	if (9*1024 < boss_hp_low_value) return;/* �͈͊O�̏ꍇ�͉������Ȃ� */
	#else
//	if (0 != ((boss_hp_low_value)&(0xffffc000)) )	return;/* �͈͊O�̏ꍇ�͉������Ȃ� */
	if (0 > boss_hp_low_value)
	{
	//	(((BOSS_BASE *)s->data)->boss_health) = 0;/* �܂������� */
		boss_hp_low_value = 0;
	}
	if (0==(boss_hp_low_value/*+bo ss_life_value*/))
	{
		return;/* �͈͊O�̏ꍇ�͉������Ȃ� */
	}
	boss_x256 = (/*((BOSS_BASE *)s->data)->boss_*/s->x256);/*(???)141477*/
	boss_y256 = (/*((BOSS_BASE *)s->data)->boss_*/s->y256);/*(???)141477*/

	draw_boss_hp_value_set = ((boss_hp_low_value & 0x03ff)>>2);/* �{�Xhp�`��l */
	if (draw_boss_hp_value < (draw_boss_hp_value_set))
			{	draw_boss_hp_value++;	}
	else	{	draw_boss_hp_value--;	}
	#endif
//
	#if 1/*�{�X���Ԍo��*/
//	if ()
//	if ((STATE_FLAG_05_IS_BOSS|0) == (pd->state_flag&(STATE_FLAG_05_IS_BOSS|STATE_FLAG_06_IS_SCRIPT)))
	{
		/* (�Ƃ肠����)�X�y�J���[�h���̂ݎ��Ԍo�� */
		if (0/*off*/!=spell_card_mode)/*on���̂�*/
		{
			/*((BOSS_BASE *)s->data)->*/spell_card_boss_timer -= 1/*fps_fa ctor*/;
			if ((/*((BOSS_BASE *)s->data)->*/spell_card_boss_timer < 0/*1*/))
			{
				spell_card_boss_timer		= 0;
				spell_card_mode 			= 0/*off*/;
				/*((BOSS_BASE *)s->data)->boss_*/s->base_health 	= spell_card_limit_health;		/* (�Ƃ肠����) */
				boss_destroy_check_type(s/*�G����*/, DESTROY_CHECK_01_IS_TIME_OUT);/*	�� �U���̏ꍇ�̎��S���� 	�� ���Ԑ؂�̏ꍇ�̎��S���� */
				#if 000
				/*((BOSS_BASE *)s->data)->*/spell_card_boss_timer	= (60*64);		/* (�Ƃ肠����) */
				/*((BOSS_BASE *)s->data)->boss_*/s->base_health 	= 0;			/* (�Ƃ肠����) */
				#endif
			}
		}
	}
	unsigned char boss_timer_low	= ((/*((BOSS_BASE *)s->data)->*/spell_card_boss_timer)&0x3f);/* */
	unsigned int boss_timer_value	= ((/*((BOSS_BASE *)s->data)->*/spell_card_boss_timer)>>6);/* */
	#endif
	//	99 �ȏ�� 99 �\��
	if (99<boss_timer_value)
	{
		boss_timer_value = 99;
	}

/*
	�J�E���^�ɂ��āF
	�J�E���^�� 9 ����_�Ł�����炷�Ƃ�������
	�S���I�Ɏ󂯂Ƃ��悤�ɂ��Ă���B
	�����{���� 9 ���珈������Ɛl�Ԃ̊��o�ɍ���Ȃ��B
//
	���� 10 (�̏I���)����炷�B����� 10 �̎n�߂� 9 �̏I���Ȃ̂ŁA
	9 ������Ă���Ƃ݂Ȃ���B(������Y���͂Ȃ�)
//
	�_�ł� 10 ����_�ł�����B����� 10 ����{���ɓ_�ł��Ă���B
	���Ӑ[������� 10 �������Ă���B(������O�� 10 �������Ă���)
	�������l�Ԃ̊��o���炷��Ɓu 9 ����_�ł��Ă���悤�Ɋ�����v
	�l�Ԃ̎��o�͗\���������Ƌ�؂�ɔ������Ȃ��B
//
	11 11 11 10 10 10 09 09 09 08 08 08 (���Ԃ̗���)
	�� �� �� �� �� �� �� �� �� �� �� �� (���� 10 (�̏I���)����炷�B������Y���͂Ȃ�)
	�_ �_ �_ �_ �_ �� �_ �_ �� �_ �_ �� (�_�ł� 10 ����_�ł�����B�l�Ԃ̊��o���炷��Ɓu 9 ����_�ł��Ă���悤�Ɋ�����v)
				   ��
*/

	if (0==boss_timer_low)
	{
		/* �J�E���g 9 ���特��炷�D */
	//	if ((10  )>boss_timer_value)	/* (10	)==�ݒ�l 10 �ŁA�J�E���g 8 ���特����悤�ɕ�������D */
		if ((10+1)>boss_timer_value)	/* (10+1)==�ݒ�l 11 �ŁA�J�E���g 9 ���特����悤�ɕ�������D */
		{
			#if (0==USE_DESIGN_TRACK)
			play_voice_auto_track(VOICE15_COUNT_TIMER);
			#else
			voice_play(VOICE15_COUNT_TIMER, TRACK03_SHORT_MUSIC);/*�e�L�g�[*/
			#endif
		}
	}


	/* boss hp �摜�̕������� / gauge stringth x width */
//	#define HPGAUGE_X_OFS (50)
//	#define HPGAUGE_Y_OFS (6)
	#define HP_FONT_X_OFS (4)	/*(50-(8*5)-3)*/	/*(37+10) dx*/
	#define HP_FONT_Y_OFS (1)/*(0+0) dy*/
	//
	#define BOSS_TIMER_X_OFS (PPP-16-4)/* dx*/
	#define BOSS_TIMER_Y_OFS (1)/*(0+0) dy*/
	/* 1024�ȏ�̒l�͐����ŕ\�� */
	{	char buffer[8/* 4 3*/];
	#if 1
		//	�X�y���c�莞�ԕ\��
		if (
			//	(9<boss_timer_value) || 	/* 10�ȏ�͖������ŕ\�� */		/* �J�E���g 8 ����_�ł��Ă�悤�Ɍ�����D */
				(10<boss_timer_value) ||	/* 11�ȏ�͖������ŕ\�� */		/* �J�E���g 9 ����_�ł��Ă�悤�Ɍ�����D */
				(20<boss_timer_low) 		/* �_�� 20=(64/3) */
			)
		{
			strcpy(buffer,"00");
			dec_display( (boss_timer_value), 2, (char *)buffer);
			font_print_screen_xy(buffer, FONT01/*FONT06*/, BOSS_TIMER_X_OFS, BOSS_TIMER_Y_OFS);
		}
	#endif
	//	�c�胉�C�t�\��
	//	sp rintf(buffer,"%d", (boss_hp_low_value>>10));/*(???)141477*/
	//	sp rintf(buffer,"%d", (boss_hp_low_value>>10));/*(???)141477*/
		strcpy(buffer, STR_ENEMY "0");
		dec_display( /*(bo ss_life_value)*/(boss_hp_low_value>>10)/*(boss_hp_low_value>>10)*/, 1, (char *)&buffer[5]);
		font_print_screen_xy(buffer, FONT01/*FONT06*/, HP_FONT_X_OFS, HP_FONT_Y_OFS);
	}
}

	/* 1024�����ׂ̍����l�̓O���t�\�� */
//	{
	//	SDL_Rect rect_src;
	//	SDL_Rect rect_dest;
	//	rect_src.x = 0;
	//	rect_src.y = 0;
	//	rect_src.h = 10;
	//	rect_src.w = HPGAUGE_X_OFS+((boss_hp_low_value	& 0x03ff)>>2); /* 1023�l �� 255�h�b�g */
	//	rect_dest.w = boss_gauge->w;
	//	rect_dest.h = boss_gauge->h;
	//	rect_dest.x = 10/*dx*/;
	//	rect_dest.y =  0/*dy*/+HPGAUGE_Y_OFS;
//		SDL_BlitSurface(boss_gauge, NULL/*&rect_src*/, sdl_screen[SDL_00_SCREEN], NULL/*&rect_dest*/);
//	}

/*---------------------------------------------------------
	�X�R�A�p�l���̃X�e�[�^�X�\��
---------------------------------------------------------*/
#define USE_DEBUG	(0)
#if (1==USE_DEBUG)
int debug_num1;
int debug_num2;
#endif

static int top_score;
//static unsigned int psp_get_fps60(void);

/*---------------------------------------------------------
	60�t���[�����ƂɈ��ĂԂ��Ƃɂ��A
	�����Ԃ���Z�o����fps���擾����B
	fps �� 100 �{���������`���ŕԂ��B
	-------------------------------------------------------
	�p�r�Ffps�v���p�B
	���ӁF60�t���[�����ƂɕK���ĂԎ��B
	���ӁF���������ŌĂ΂�鎖�͍l������ĂȂ��̂ŁA�K���ꃖ������ĂԎ��B
	���ӁF�O�񂩂��72���ȓ��ɕK���ĂԎ��B(60�t���[����72���ȏ����...�ǂ�ȏ󋵂�˂�)
---------------------------------------------------------*/

/*static*/static unsigned int psp_get_fps60_00(void)
{
	static u64 ticks_alt;	/* �O��̎��� the clock time of alter ticks. */
	static u64 ticks_now;	/* ����̎��� the clock time of current ticks. */
	ticks_alt	= ticks_now;
	sceRtcGetCurrentTick(&ticks_now);
	/*	�O��v�����ԂƂ̍����� 1[nano sec] �P��  ( 1[nano sec] == 1/(1000*1000)[sec] )
	�� unsigned int == �����Ȃ�32bit�`���֕ϊ� */
	unsigned int ttt;
	ttt= ((unsigned int)(ticks_now - ticks_alt));/* �������� the clock time of differencial ticks. */
	/* fps �� 100 �{���������`���֕ϊ� */
	if (0 != ttt)/* �[�� 0 �Ŋ���ꍇ���������(Devision by zero �h�~) */
	{
	//	ttt = ( (unsigned int)(60*60*16666) / (ttt));					/*"60fps"*/
	//	ttt = ( (unsigned int)(60*60*166666) / (unsigned int)(ttt));	/*"60.0fps"*/
	//	ttt = ( (u64)(60*60*1666666) / (u64)(ttt)); 					/*"60.00fps"*/
	/*"60.00fps"(�����^�Ōv�Z����ɂ� 33bit �v��̂ŁAunsigned int (32bit) �ő���Ȃ�)*/
		ttt = ( (unsigned int)((((60*60)/16)*(1666666))) / (unsigned int)(ttt>>4));
		/* 60*60 == 3600 �́A 16 �Ŋ���؂��̂ŁA225 �ɂȂ�B */
	}
	return (ttt);
}

void score_display(void)
{
	/* [ �p�l���x�[�X��\�� ] */
	{
		SDL_Rect panel_base_r = {GAME_WIDTH, 0, 0, 0};	// �f�[�^�E�B���h�E�prect_srct->w,h,x,y
		SDL_BlitSurface(panel_base, NULL, sdl_screen[SDL_00_SCREEN], &panel_base_r);
	}
//
	PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
	/* [ �{�X�̗͕̑\�� ] */
//	if (B01_BA TTLE == pd->bo ssmode)
//	if ((STATE_FLAG_05_IS_BOSS|0) == (pd->state_flag&(STATE_FLAG_05_IS_BOSS|STATE_FLAG_06_IS_SCRIPT)))
	if ((pd->state_flag&(STATE_FLAG_13_DRAW_BOSS_GAUGE)))
	{
		draw_boss_gauge(/*10, 6-6*/);
	}
//
	/* [ �v���C���[���\�� ] */
	draw_stars_status( R_00_aka_hosi_png,  (pd->zanki), 10*8+4);	/*R_01_mizu_hosi_png*/
//
	/* [ �{�����\�� ] */
	draw_stars_status( R_01_mizu_hosi_png, (pd->bombs), 14*8+1);	/*R_00_aka_hosi_png*/
//
	#if (1==USE_DEBUG)/* �e�D�揇�ʂ��Ƃǂꂭ�炢���邩���ׂĂ݂� */
	/* �p�l���̃X�R�A����debug_num1���A�O���C�Y����debug_num2��\��������B���Ă����������������Ⴄ�B */
	((PLAYER_DATA *)player->data)->score		= debug_num1;
	((PLAYER_DATA *)player->data)->graze_point	= debug_num2;
	#endif
//
	//{/*�����̂��X�R�[�v���Ȃ������ǂ�(�������X�R�[�v����Ȃ���,�R�[�h���ς��)*/
		char buffer[64/*100*/];
		/* [ �n�C�X�R�A�\�� ] */
		if (top_score < pd->my_score)
		{	top_score = pd->my_score;}
	//	sp rintf(buffer,"%09d0", top_score);
		strcpy(buffer,"0000000000");
		dec_display(top_score,		9/*8*/, (char *)buffer);		font_print_screen_xy(buffer,FONT01,PPP+5*8+4,3*8+2);
	//
		/* [ �X�R�A�\�� ] */
	//	sp rintf(buffer,"%09d0", pd->my_score);
		strcpy(buffer,"0000000000");
		dec_display(pd->my_score,	9/*8*/, (char *)buffer);		font_print_screen_xy(buffer,FONT01,PPP+5*8+4,6*8+7);
	//
		/* [ �p���[�Q�[�W�\�� ] */
		draw_power_gauge(pd->weapon_power /*,PPP+7,124*/);
		if (pd->weapon_power > (127-1) /*== 128*/)/*max==127==�u128�i�K�v*/
		{
			strcpy(buffer, STR_MAX);
		}
		else
		{
			#if 1
		//	sp rintf(buffer, "%d", (int)((dou ble)p->weapon / 128 * 100 )); 	// [***090123		�ύX
		//	sp rintf(buffer," %d", (((int)(pd->weapon_power) * 200) >>8) ); 	// [***090214		�ύX
			strcpy(buffer,"  0");
			dec_display( (((int)(pd->weapon_power) * 200) >>8), 2, (char *)&buffer[1]);
			#endif
		}
		font_print_screen_xy(buffer,FONT01,PPP+10*8+7,17*8+5);
		//font_print_screen_xy(buffer,FONT01,PPP+8*8+3,125/*+1*/-2);
	//
		/* [ �{���L�����ԕ\�� ] */
		//if (p->ex tra_type!=PLX_NONE)
		if (0 != pd_bomber_time)
		{
		//	sp rintf(buffer, STR_TIME_"%3d",(int)(((int)pd_bomber_time)/10));
		//	font_print_screen_xy(buffer,FONT01,PPP+3*8-6,160);
			strcpy(buffer, STR_TIME_"   ");
			dec_display( (int)(((int)pd_bomber_time) ), 3, (char *)&buffer[5]);
			buffer[7] = 0/*' '*/;/* 1���ڂ͕\�����Ȃ� */
			font_print_screen_xy(buffer,FONT01,PPP+8*8+4,22*8);
		}
	//
		/* [ �O���C�Y�X�R�A�\�� ] */
		//	sp rintf(buffer," %d", pd->graze_point);
			strcpy(buffer,"   0");
			dec_display( pd->graze_point, 4, (char *)&buffer[0]);
		//	font_print_screen_xy(buffer,FONT01,PPP+7*8+3,140);/*3��(����Ȃ�)*/
			font_print_screen_xy(buffer,FONT01,PPP+11*8+4,20*8);/*4��(�҂���)*/
	//
		/* [ ��Փx�\�� ] */
		{
			/*const*/ char *rank_name[4] =
			{
			/* 0==PLX_NONE*/		STR_EASY,
			/* 1==PLX_HOMING:*/ 	STR_NORMAL,
			/* 2==PLX_HLASER:*/ 	STR_HARD,
			/* 3==PLX_SHIELD:*/ 	STR_LUNATIC,
		//	/* 4==PLX_BOMB:*/		STR_EXTRA,
			//	/*default:*/		"UNKNOWN ???",
			};
			font_print_screen_xy( (char *)rank_name[(difficulty)&(4-1)], FONT01,PPP+/*7*/1*8,256);
		}
	//
		#if 1
		/* [ fps �\��(60�t���[���P�ʂŕ\��) ] */
		/* (psp��1�b��59.9�Ȃ񂽂�t���[���ŁA������60�łȂ��炵���ł��B ) */
		{
		//	static int ttt;
			static unsigned int ttt;
			static int fps_draw_wait_counter;
			fps_draw_wait_counter--;
			if (fps_draw_wait_counter < 1)
			{
				fps_draw_wait_counter = 60;
			//	[1/60sec]	0.016 666 [nsec] 6666666666666666666666666667
			//	16666.66666 / x == 60.0,  16666.666666/60.0 == x, x== 277.7777777777777
			//	16666.00(int) / 60.00(int) == 60.1660649819494584837545126353791 = 60.000 (int)
				ttt = psp_get_fps60_00();/* �O��v�����ԂƂ̍����� 1[nano sec] �P�ʂ� signed int == �����t32bit�`�ŕԂ��B */
			}
//
			/*"60fps"*/
	//	//	sp rintf(buffer, STR_TIME_"%3d",(int)(((int)ttt)));
	//		strcpy(buffer, STR_TIME_"00");
	//	//	dec_display(ttt,	3, (char *)&buffer[0/*5*/]);
	//		dec_display(ttt,	2, (char *)&buffer[0/*5*/]);
	//		font_print_screen_xy(buffer,FONT01,PPP,256);
//
	//		/*"60.0fps"*/
	//		strcpy(buffer, STR_TIME_"0000");
	//		dec_display(ttt,	3, (char *)&buffer[0/*5*/]);
	//		buffer[8-5] = buffer[7-5];
	//		buffer[7-5] = CHR_PIRIOD_;/*' ' '.'*/
	//		font_print_screen_xy(buffer,FONT01,PPP,256);
//
			/*"60.00fps"*/
			strcpy(buffer, /*STR_TIME_*/"00000"STR_FPS_);
			dec_display(ttt,	4, (char *)&buffer[0/*5*/]);
			buffer[9-5] = buffer[8-5];
			buffer[8-5] = buffer[7-5];
			buffer[7-5] = CHR_PIRIOD_;/*' ' '.'*/
			font_print_screen_xy(buffer,FONT01,PPP+8*8+2,256);
		}
		#endif
	//}
}

/*---------------------------------------------------------
	�p�l���\���A������
---------------------------------------------------------*/
#include "name_entry.h"/**/
extern int select_player;
void score_panel_init(void)
{
	top_score			= high_score_table[select_player][0].score; 	// ��ɕ\������n�C�R�A�̎擾=>score.c�ŗ��p
	panel_base			= loadbmp0("panel/panel_base.png", 0, 1);
	star_gauge			= loadbmp0("panel/hosi_gauge.png", 0, 1);		/*(char *)img_name[img_num]*/
	SDL_SetColorKey(star_gauge,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
//	power_gauge 		= loadbmp0("panel/power_gauge.png", 0, 1);
//	SDL_SetColorKey(power_gauge,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
//	boss_gauge			= loadbmp0("panel/boss_gauge.png", 1, 1);/*2*/
}

/*---------------------------------------------------------
	�R���e�j���[�񐔂̕\��
---------------------------------------------------------*/
extern int now_max_continue;
void render_continue(void/*int now_max_continue*/)
{
	char buffer[64/*100*/];
//	/* ����n��R���e�B�j���[�ł��܂� */
//	sp rintf(buffer,  "TRY CHANCE STILL AT %2d", now_max_continue);
	strcpy(buffer,	"TRY CHANCE STILL AT  0");
	dec_display( now_max_continue, 2, (char *)&buffer[20]);
//
	font_print_screen_xy(buffer, FONT03, 0/*10*/, 16/*50*/);
	#if (0==USE_CONTINUED_RANKING)
//	/* �R���e�B�j���[�����ꍇ�A�X�R�A�����L���O����܂��� */
	font_print_screen_xy("IF YOU GOT CONTINUED,", FONT05, 0/*10*/, 200);
	font_print_screen_xy("CAN NOT HAVE RANKING.", FONT05, 16/*26*/, 220);
//
	/* ����܂蒷�����炵���p��ɂ��Ă�����[���Ȃ�����A(�����ƕ\���ł��Ȃ���)
		�Ӗ������Ȃ��͈͂œK���ɏȗ� */
	#endif
}




/*---------------------------------------------------------
	�ȉ�RESULT�\��(stage_clear.c)
	-------------------------------------------------------
	���ݓs���ɂ�� score_panel.c ���ɂ���B
	Gu�����i�߂ΓƗ�����\��B
---------------------------------------------------------*/

#include "game_main.h"
#include "scenario_script.h"

	#if (1==USE_RESULT_WAIT)
#define TIME_20_RESULT_WAIT /*20*/(60*5)/*2*/
static int result_time_wait;
//static int result_state;
	#endif
void player_result_init(void)
{
	#if (1==USE_RESULT_WAIT)
	result_time_wait = TIME_20_RESULT_WAIT;
//	result_state = 0;
	#endif
}




/*---------------------------------------------------------
	RESULT�\��
---------------------------------------------------------*/

static void render_result(void/*int now_max_continue*/)
{
	PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
	char buffer[64/*100*/];
	strcpy(buffer,	"RESULT" ); 																			font_print_screen_xy(buffer, FONT03, 0,  32);
	strcpy(buffer,	"STAGE   0 X 1000 PTS.");	dec_display( player_now_stage, 1, (char *)&buffer[ 8]); 	font_print_screen_xy(buffer, FONT05, 8, 100);
	strcpy(buffer,	"POWER 000 X  100 PTS.");	dec_display( pd->weapon_power, 3, (char *)&buffer[ 6]); 	font_print_screen_xy(buffer, FONT05, 8, 120);
	strcpy(buffer,	"GRAZE 000 X   10 PTS.");	dec_display( pd->graze_point,  3, (char *)&buffer[ 6]); 	font_print_screen_xy(buffer, FONT05, 8, 140);
	/*const*/ char *level_name[4] =
	{
					"EASY      X 0.5",
					"NORMAL    X 1.0",
					"HARD      X 1.2",
					"LUNATIC   X 1.5",
	};
	font_print_screen_xy( (char *)level_name[(difficulty/*&0x03*/)], FONT03, 0/*26*/, 160);
}
/*---------------------------------------------------------
	RESULT�\��(�Q�[���e�ʃN���A�[��)
---------------------------------------------------------*/
extern void player_stage_clear(void);
void stage_clear_work(void)
{
	#if (1==USE_RESULT_WAIT)
	result_time_wait--;
	switch (result_time_wait)
	{
	case (TIME_20_RESULT_WAIT-1):
	#endif
		script_message_window_clear();
		msg_time = (60*5);
		print_kanji(/*SDL_Rect *rect_srct*/0, /*text*/"CHALLENGE NEXT STAGE!" "\n" "\n" "�����F����...", /*int color_type*/7, /*int wait*/0);
		render_result();
	#if (1==USE_RESULT_WAIT)
		break;
	case 0:
	//	if (0==result_time_wait)
		{
			result_time_wait = TIME_20_RESULT_WAIT;
	#endif
			//PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
			player_stage_clear();
		//
			psp_loop = (ST_INIT_GAME_PLAY_common|0);
		//	common_load_init();
	#if (1==USE_RESULT_WAIT)
		}
		break;
	}
	#endif
}


/*---------------------------------------------------------
	�ȉ�����(�t�H���g�̌`�����ς�����̂ŕ\���ł��Ȃ���)
---------------------------------------------------------*/

	//	sp rintf(buffer, STR_EXTRA_);					font_print_screen_xy(buffer,FONT01,PPP+1*8-2,170);

	//sp rintf(buffer,"SHIPS : %d",p->zanki);		//	font_print_screen_xy(buffer,FONT01,0,10);
	//sp rintf(buffer,"SPEED : %d",p->player_speed); // font_print_screen_xy(buffer,FONT01,0,20);
	//sp rintf(buffer,"STAGE : %d",p->level);		//	font_print_screen_xy(buffer,FONT01,0,30);

	//sp rintf(buffer,"H_SCORE:");					//	font_print_screen_xy(buffer,FONT01,PPP+1*8,5);
	//sp rintf(buffer,"SCORE  :");					//	font_print_screen_xy(buffer,FONT01,PPP+1*8,30);
	//sp rintf(buffer,"GRAZE  :");					//	font_print_screen_xy(buffer,FONT01,PPP+1*8,140);
	//sp rintf(buffer,"POWER  :");					//	font_print_screen_xy(buffer,FONT01,PPP+1*8,110);
	//sp rintf(buffer,"PLAYER");					//	font_print_screen_xy(buffer,FONT01,PPP+1*8,60);
	//sp rintf(buffer,"BOMB");						//	font_print_screen_xy(buffer,FONT01,PPP+1*8,85);

	/*
	switch (weapon_List) {
	case WP_PLASMA: 			strcat(buffer,"REIFU-1");		break;
	case WP_DOUBLEPLASMA:		strcat(buffer,"REIFU-2");		break;
	case WP_QUADPLASMA: 		strcat(buffer,"REIFU-4");		break;
	case WP_FIREBALL:			strcat(buffer,"YUMEFU-1");		break;
	case WP_DOUBLEFIREBALL: 	strcat(buffer,"YUMEFU-2");		break;
	case WP_QUADFIREBALL:		strcat(buffer,"YUMEFU-4");		break;
	case WP_FIFTHFIREBALL:		strcat(buffer,"YUMEFU-5");		break;
	case WP_KILLRAY:			strcat(buffer,"OFUDA"); 		break;
	default:					strcat(buffer,"UNKNOWN ???");	break;
	}
	font_print_screen_xy(buffer,FONT01,PPP+3*8,120);
	*/

//void score_cleanup()
//{
//}

