
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�X�R�A�p�l��(�\��)�֘A
---------------------------------------------------------*/

static SDL_Surface *panel_base; 	// �p�l���x�[�X
static SDL_Surface *star_gauge;

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
//	#define STR_ENEMY		"klmno" 		/*5��*/
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
global void dec_print_format(
		unsigned int num,
		int size,
//		int sprite,
//		int x,
//		int y
		char *my_str
)
{
	int sss;
//	int j;
	//unsigned int n;
	unsigned int ch;
	const u32 sub10[10/*8*/]=
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
	sss = 0;
	num++;/*�Ō㌅�̒��덇�킹*/
//	j = 256* (x);
	while (size)
	{
		size--;
		ch = 0;
		while (num > sub10[size])
		{
			num -= sub10[size];
			ch++;
			sss = (1);
		}
		if (
			(0 != sss) //&& /* ��ʌ��� 0 �͕\�����Ȃ� */
			//(0!=size) 	/* 1���ڂ͕K���\������ */
		)
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
	//		obj_01_panel[sprite+size].m_RollZ		= 0.0;
	//		obj_01_panel[sprite+size].m_Use 		= TRUE;
	//	//	obj_01_panel[sprite+size].m_PosCenter	= FALSE;
			(*my_str) = ('0'+ch);
		}
	//	else
	//	{
	//		;// 	obj_01_panel[sprite+size].m_Use 	= FALSE;
	//	}
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
	SDL_Rect rect_dest;
	SDL_Rect rect_src;
	if (value<1)	{	value=0;}
//
	rect_src.x = (0);
	rect_src.y = (img_num);/*(0)*/
	rect_src.w = (10*value);
	rect_src.h = (10);
//
	rect_dest.x = PSP_WIDTH480-4-(10*value);
	rect_dest.y = y_offset;
	rect_dest.w = 100;
	rect_dest.h = 11;
	SDL_BlitSurface(star_gauge, &rect_src, sdl_screen[SDL_00_VIEW_SCREEN], &rect_dest);
}

/*---------------------------------------------------------
	�v���C���[�̃E�F�|���Q�[�W�̕\���̎q�֐�
---------------------------------------------------------*/

static void draw_power_gauge(int weapon)/*, int dx, int dy*/
{
	#define WP_GAUGE_X_OFS (PPP+48-2)
	#define WP_GAUGE_Y_OFS (128+8+4)
	SDL_Rect rect_dest;
	SDL_Rect rect_src;
	rect_src.x = (0);
	rect_src.y = (20);/*(0)*/
	rect_src.h = (13);
	#if 0
//	rect_src.w = (int)((dou ble)weapon / 127/*128*/ * (dou ble)power_gauge->w); 	// [***090123		�ύX
	#else
	/* 1 dot ���炢�덷���邩������Ȃ����Ǌȗ���(������) */
	rect_src.w = ((int)(weapon * (80)/*power_gauge->w*/)>>7); // [***090123 	�ύX
	#endif
	rect_dest.w = (80);/*power_gauge->w*/
	rect_dest.h = (13);/*power_gauge->h*/
	rect_dest.x = WP_GAUGE_X_OFS;/*dx*/
	rect_dest.y = WP_GAUGE_Y_OFS;/*dy*/
	SDL_BlitSurface(star_gauge/*power_gauge*/, &rect_src, sdl_screen[SDL_00_VIEW_SCREEN], &rect_dest);
}

/*---------------------------------------------------------
	�X�R�A�p�l���̃X�e�[�^�X�\��
---------------------------------------------------------*/
#define USE_DEBUG	(0)
#if (1==USE_DEBUG)
global int debug_num1;
global int debug_num2;
#endif

static int top_score;
//static unsigned int psp_get_fps60(void);

#if (1==USE_EXTEND_CHECK)
extern void player_check_extend_score(void);
#endif

/*---------------------------------------------------------
	60�t���[�����ƂɈ��ĂԂ��Ƃɂ��A
	�����Ԃ���Z�o����fps���擾����B
	fps �� 100 �{���������`���ŕԂ��B
	-------------------------------------------------------
	�p�r�Ffps�v���p�B
	���ӁF60�t���[�����ƂɕK���ĂԎ��B
	���ӁF���������ŌĂ΂�鎖�͍l������ĂȂ��̂ŁA�K���ꃖ������ĂԎ��B
	���ӁF�O�񂩂��72���ȓ��ɕK���ĂԎ��B(60�t���[����72���ȏ����...�ǂ�ȏ󋵂�˂�)
	��������͕핗���ăX���[�v�o����񂾂ˁB
	�X���[�v���Ė�72���ȏソ�����ꍇ�́A�n�߂�fps�����m�ɏo�Ȃ�(�{�����ǂ��o��)
	�����Ȃ̂œ��ɖ��Ȃ��ł��B
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

global void score_display(void)
{
	/* [ �p�l���x�[�X��\�� ] */
	{
		SDL_Rect panel_base_r = {GAME_WIDTH, 0, 0, 0};	// �f�[�^�E�B���h�E�prect_srct->w,h,x,y
		SDL_BlitSurface(panel_base, NULL, sdl_screen[SDL_00_VIEW_SCREEN], &panel_base_r);
	}
//
	/* [ �v���C���[���\�� ] */
	draw_stars_status( R_00_aka_hosi_png,  (pd_zanki), 10*8+4); /*R_01_mizu_hosi_png*/
//
	/* [ �{�����\�� ] */
	draw_stars_status( R_01_mizu_hosi_png, (pd_bombs), 14*8+1); /*R_00_aka_hosi_png*/
//
	#if (1==USE_DEBUG)/* �e�D�揇�ʂ��Ƃǂꂭ�炢���邩���ׂĂ݂� */
	/* �p�l���̃X�R�A����debug_num1���A�O���C�Y����debug_num2��\��������B���Ă����������������Ⴄ�B */
	pd_score		= debug_num1;
	pd_graze_point	= debug_num2;
	#endif
//
	//{/*�����̂��X�R�[�v���Ȃ������ǂ�(�������X�R�[�v����Ȃ���,�R�[�h���ς��)*/
		char buffer[64/*100*/];
		/* [ �n�C�X�R�A�\�� ] */
		if (top_score < pd_my_score)
		{	top_score = pd_my_score;}
	//	sp rintf(buffer,"%09d0", top_score);
		strcpy(buffer,"0000000000");
		dec_print_format(top_score, 	9/*8*/, (char *)buffer);		font_print_screen_xy(buffer, FONT10W, PPP+5*8+4,3*8+2);
	//
		/* [ �X�R�A�\�� ] */
	//	sp rintf(buffer,"%09d0", pd_my_score);
		strcpy(buffer,"0000000000");
		dec_print_format(pd_my_score,	9/*8*/, (char *)buffer);		font_print_screen_xy(buffer, FONT10W, PPP+5*8+4,6*8+7);
	//
		/* [ �p���[�Q�[�W�\�� ] */
		draw_power_gauge(pd_weapon_power); /*,PPP+7,124*/
		if (pd_weapon_power > (MAX_POWER_IS_128-1) /*== 128*/)/*max==MAX_POWER_IS_128==�u129�i�K�v*/
		{
			strcpy(buffer, STR_MAX);
		}
		else
		{
			#if 0/* 100% �\�L */
		//	sp rintf(buffer, "%d", (int)((dou ble)p->weapon / 128 * 100 )); 	// [***090123		�ύX
		//	sp rintf(buffer," %d", (((int)(pd_weapon_power) * 200) >>8) );		// [***090214		�ύX
			strcpy(buffer,"  0");
			dec_print_format( (((int)(pd_weapon_power) * 200) >>8), 2, (char *)&buffer[1]);
			#endif
			#if 0/* [P]�� �\�L */
			strcpy(buffer,"  0");
			dec_print_format( (((int)(pd_weapon_power) ) ), 3, (char *)&buffer[0]);
			#endif
			#if 1/* 5.00 �\�L */
		//	sp rintf(buffer, "%d", (int)((dou ble)p->weapon / 128 * 100 )); 	// [***090123		�ύX
		//	sp rintf(buffer," %d", (((int)(pd_weapon_power) * 200) >>8) );		// [***090214		�ύX
			/* "5.00" */
			strcpy(buffer,"0000");
			dec_print_format( (((int)(pd_weapon_power) * (200*5)) >>8), 3, (char *)&buffer[0]);
		//	buffer[4] = 0;
			buffer[3] = buffer[2];
			buffer[2] = buffer[1];
			buffer[1] = CHR_PIRIOD_;
			#endif
		}
		font_print_screen_xy(buffer, FONT10W, PPP+10*8+7,17*8+5);
		//font_print_screen_xy(buffer, FONT10W, PPP+8*8+3,125/*+1*/-2);
	//
		/* [ �{���L�����ԕ\�� ] */
		//if (p->ex tra_type!=PLX_NONE)
		if (0 != pd_bomber_time)
		{
		//	sp rintf(buffer, STR_TIME_"%3d",(int)(((int)pd_bomber_time)/10));
		//	font_print_screen_xy(buffer, FONT10W, PPP+3*8-6,160);
			strcpy(buffer, STR_TIME_"   ");
			dec_print_format( (int)(((int)pd_bomber_time) ), 3, (char *)&buffer[5]);
			buffer[7] = (0);	/*' '*/ 	/* 1���ڂ͕\�����Ȃ� */
			font_print_screen_xy(buffer, FONT10W, PPP+8*8+4,22*8);
		}
	//
		/* --- �d�̃O���C�Y�J���X�g 99999�� (5��) --- */
		/* [ �O���C�Y�X�R�A�\�� ] */
		//	sp rintf(buffer," %d", pd_graze_point);
			strcpy(buffer,"   0");
			dec_print_format( pd_graze_point, 4, (char *)&buffer[0]);
		//	font_print_screen_xy(buffer, FONT10W, PPP+7*8+3,140);/*3��(����Ȃ�)*/
			font_print_screen_xy(buffer, FONT10W, PPP+11*8+4,20*8);/*4��(�҂���)*/
	//
		/* [ ��Փx�\�� ] */
		{
			static const char *rank_name[(4)] =
			{
			/* 0==PLX_NONE*/		STR_EASY,
			/* 1==PLX_HOMING:*/ 	STR_NORMAL,
			/* 2==PLX_HLASER:*/ 	STR_HARD,
			/* 3==PLX_SHIELD:*/ 	STR_LUNATIC,
		//	/* 4==PLX_BOMB:*/		STR_EXTRA,
			//	/*default:*/		"UNKNOWN ???",
			};
			font_print_screen_xy( (char *)rank_name[(difficulty)&(4-1)], FONT10W, PPP+/*7*/1*8,256);
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
				/* �G�N�X�e���h�`�F�b�N��1�b��1��(��)�Ŗ��Ȃ��Ǝv���B */
				#if (1==USE_EXTEND_CHECK)
				player_check_extend_score();
				#endif
			//	[1/60sec]	0.016 666 [nsec] 6666666666666666666666666667
			//	16666.66666 / x == 60.0,  16666.666666/60.0 == x, x== 277.7777777777777
			//	16666.00(int) / 60.00(int) == 60.1660649819494584837545126353791 = 60.000 (int)
				ttt = psp_get_fps60_00();/* �O��v�����ԂƂ̍����� 1[nano sec] �P�ʂ� signed int == �����t32bit�`�ŕԂ��B */
			}
//
			/*"60fps"*/
	//	//	sp rintf(buffer, STR_TIME_"%3d",(int)(((int)ttt)));
	//		strcpy(buffer, STR_TIME_"00");
	//	//	dec_print_format(ttt,	3, (char *)&buffer[0/*5*/]);
	//		dec_print_format(ttt,	2, (char *)&buffer[0/*5*/]);
	//		font_print_screen_xy(buffer, FONT10W, PPP,256);
//
	//		/*"60.0fps"*/
	//		strcpy(buffer, STR_TIME_"0000");
	//		dec_print_format(ttt,	3, (char *)&buffer[0/*5*/]);
	//		buffer[8-5] = buffer[7-5];
	//		buffer[7-5] = CHR_PIRIOD_;/*' ' '.'*/
	//		font_print_screen_xy(buffer, FONT10W, PPP,256);
//
			/*"60.00fps"*/
			strcpy(buffer, /*STR_TIME_*/"00000"STR_FPS_);
			dec_print_format(ttt,	4, (char *)&buffer[0/*5*/]);
			buffer[9-5] = buffer[8-5];
			buffer[8-5] = buffer[7-5];
			buffer[7-5] = CHR_PIRIOD_;/*' ' '.'*/
			font_print_screen_xy(buffer, FONT10W, PPP+8*8+2,256);
		}
		#endif
	//}
}

/*---------------------------------------------------------
	�p�l���\���A������
---------------------------------------------------------*/

extern int select_player;
global void score_panel_init(void)
{
	top_score			= high_score_table[select_player][0].score; 	// ��ɕ\������n�C�R�A�̎擾=>score.c�ŗ��p
	panel_base			= load_chache_bmp((char*)"panel/panel_base.png");//, 0, 1);
	star_gauge			= load_chache_bmp((char*)"panel/hosi_gauge.png");//, 0, 1); 	/*(char *)img_name[img_num]*/
	SDL_SetColorKey(star_gauge, (SDL_SRCCOLORKEY|SDL_RLEACCEL), 0x00000000);/* ���� SDL�����̂��ߕK�v */
}

/*---------------------------------------------------------
	�R���e�j���[�񐔂̕\��
---------------------------------------------------------*/
extern int	now_max_continue;
global void render_continue(void)/*int now_max_continue*/
{
	char buffer[64/*100*/];
//	/* ����n��R���e�B�j���[�ł��܂� */
//	sp rintf(buffer,  "TRY CHANCE STILL AT %2d", now_max_continue);
	strcpy(buffer,	"TRY CHANCE STILL AT  0");
	dec_print_format( now_max_continue, 2, (char *)&buffer[20]);
//
	font_print_screen_xy(buffer, FONT16R, 0/*10*/, 16/*50*/);
	#if (0==USE_CONTINUED_RANKING)
//	/* �R���e�B�j���[�����ꍇ�A�X�R�A�����L���O����܂��� */
	font_print_screen_xy("IF YOU GOT CONTINUED,", FONT16W, 0/*10*/, 200);
	font_print_screen_xy("CAN NOT HAVE RANKING.", FONT16W, 16/*26*/, 220);
//
	/* ����܂蒷�����炵���p��ɂ��Ă�����[���Ȃ�����A(�����ƕ\���ł��Ȃ���)
		�Ӗ������Ȃ��͈͂œK���ɏȗ� */
	#endif
}

