
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�X�R�A�p�l��(�\��)�֘A
	-------------------------------------------------------
	����(r33)SDL�ׁ̈A������������Ƃ��Ȃ荓���ł��B
---------------------------------------------------------*/
#include "111_my_file.h"/*(my_file_common_name)*/

static SDL_Surface *panel_base; 	// �p�l���x�[�X
static SDL_Surface *star_gauge;

/* �T�C�h�p�l���̉��\���ʒu pannel x offset */
//#define PPP (380)
#define PPP (GAME_WIDTH)
#if 0/*(����)*/
	#define STR_MAX 		"MAX"
	#define STR_MISSILE 	"MISSILE"
	#define STR_WEAPON_UP	"WEAPON_UP"
	#define STR_LASER		"LASER"
	#define STR_BOMB		"BOMB"
	#define STR_TIME_		"TIME :"
	#define STR_EXTRA_		"EXTRA  :"
	//#define STR_MAX		"ABCD"			/*4��*/
	//#define STR_MISSILE	"EFGHIJ"		/*6��*/
	//#define STR_WEAPON_UP "KLMNOPQR"		/*8��*/
	//#define STR_LASER 	"STUVW" 		/*5��*/
	//#define STR_BOMB		"XYZab" 		/*5��*/
	//#define STR_TIME_ 	"cdefg" 		/*5��*/
	//#define STR_EXTRA_	"hijkl" 		/*5��*/
//
//	#define STR_MAX 		"ABCD"			/*4��*/
//	#define STR_EASY		"EFGH" " "  	/*4��*/
//	#define STR_NORMAL		"IJKLM"     	/*5��*/
//	#define STR_HARD		"NOPQ" " "  	/*4��*/
//	#define STR_LUNATIC 	"RSTUV" 		/*5��*/
//	#define CHR_PIRIOD_ 	'W' 			/*1��*/
//	#define STR_FPS_		"XYZ"			/*3��*/
//	#define STR_EXTRA		"XYZab" 		/*5��*/ 	/* ���g�p(r33) */
//	#define STR_TIME_		"cdef"			/*4��*/ 	/* �n���ɏ��������v���ɂȂ��Ă����̂Ŕp�~�B(r33) */
//	#define STR_RANK_		"hijkl" 		/*5��*/
//	#define STR_FPS_		"hij"			/*3��*/
//	#define CHR_PIRIOD_ 	'g' 			/*1��*/
//	#define STR_ENEMY		"klmno" 		/*5��*/
#endif

	//(r35font)
	#define STR_MAX 		"ABCD"			/*4��*/
	#define STR_EASY		"EFGH" " "  	/*4��*/
	#define STR_NORMAL		"JKLMN"     	/*5��*/
	#define STR_HARD		"PQRS" " "  	/*4��*/
	#define STR_LUNATIC 	"UVWXY" 		/*5��*/
	#define CHR_PIRIOD_ 	'a' 			/*1��*/
	#define STR_FPS_		"bcd"			/*3��*/

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
	PSPL_UpperBlit(star_gauge, &rect_src, cb.sdl_screen[SDL_00_VIEW_SCREEN], &rect_dest);
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
//	rect_src.w = (int)((dou ble)weapon / 127/*128*/ * (dou ble)power_gauge->w);
	#else
	/* 1[dot]���炢�덷���邩������Ȃ����Ǌȗ���(������) */
	rect_src.w = ((int)(weapon * (80)/*power_gauge->w*/)>>7);
	#endif
	rect_dest.w = (80);/*power_gauge->w*/
	rect_dest.h = (13);/*power_gauge->h*/
	rect_dest.x = WP_GAUGE_X_OFS;/*dx*/
	rect_dest.y = WP_GAUGE_Y_OFS;/*dy*/
	PSPL_UpperBlit(star_gauge/*power_gauge*/, &rect_src, cb.sdl_screen[SDL_00_VIEW_SCREEN], &rect_dest);
}

/*---------------------------------------------------------
	�X�R�A�p�l���̃X�e�[�^�X�\��
---------------------------------------------------------*/

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
//	if (0x0f < ttt)/* �[�� 0 �Ŋ���ꍇ���������(Devision by zero �h�~) */
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


static u32 top_score;
global void score_display(void)
{
	#if 1/*�e�X�goff*/
	/* [ �p�l���x�[�X��\�� ] */
	{
		SDL_Rect panel_base_r = {GAME_WIDTH, 0, 0, 0};	// �f�[�^�E�B���h�E�prect_srct->w,h,x,y
		PSPL_UpperBlit(panel_base, NULL, cb.sdl_screen[SDL_00_VIEW_SCREEN], &panel_base_r);
	}
	#endif
//
	/* [ �v���C���[���\�� ] */
	draw_stars_status( R_00_aka_hosi_png,  (cg.zanki), 10*8+4); /*R_01_mizu_hosi_png*/
//
	/* [ �{�����\�� ] */
	draw_stars_status( R_01_mizu_hosi_png, (cg.bombs), 14*8+1); /*R_00_aka_hosi_png*/

	//{/*�����̂��X�R�[�v���Ȃ������ǂ�(�������X�R�[�v����Ȃ���,�R�[�h���ς��)*/

	/* �ʏ펞(�f�o�b�O�ȊO�͎���(��炢�{�����[�h��)) */
		/* [ �n�C�X�R�A�\�� ] */
		if (top_score < cg.game_score)
		{	top_score = cg.game_score;}
	//	sp rintf(my_font_text,"%09d0", top_score);
		strcpy(my_font_text, "0000000000");
		dec_print_format(top_score, 	9/*8*/, (char *)my_font_text);
		//
		cg.SDL_font_type 	= FONT10W;
		cg.SDL_font_x 		= (PPP+5*8+4);
		cg.SDL_font_y 		= (3*8+2);
		font88_print_screen_xy();
	//
		/* [ �X�R�A�\�� ] */
	//	sp rintf(my_font_text,"%09d0", pd_game_score);
		strcpy(my_font_text, "0000000000");
		dec_print_format(cg.game_score, 9/*8*/, (char *)my_font_text);
		//
	//	cg.SDL_font_type 	= FONT10W;
	//	cg.SDL_font_x 		= (PPP+5*8+4);
		cg.SDL_font_y 		= (6*8+7);
		font88_print_screen_xy();
	//


		/* [ �p���[�Q�[�W�\�� ] */
		draw_power_gauge(cg.weapon_power); /*,PPP+7,124*/
		if (cg.weapon_power > (MAX_POWER_IS_128-1) /*== 128*/)/*max==MAX_POWER_IS_128==�u129�i�K�v*/
		{
			strcpy(my_font_text, STR_MAX);
		}
		else
		{
			#if 0/* 100% �\�L */
		//	sp rintf(my_font_text, "%d", (int)((dou ble)p->weapon / 128 * 100 ));
		//	sp rintf(my_font_text," %d", (((int)(pd_weapon_power) * 200) >>8) );
			strcpy(my_font_text,"  0");
			dec_print_format( (((int)(cg.weapon_power) * 200) >>8), 2, (char *)&my_font_text[1]);
			#endif
			#if 1/* [P]�� �\�L(r33) */
			strcpy(my_font_text,"  0");
			dec_print_format( (((int)(cg.weapon_power) ) ), 3, (char *)&my_font_text[0]);
			#endif
			#if 0/* 5.00 �\�L(r32) */
		//	sp rintf(my_font_text, "%d", (int)((dou ble)p->weapon / 128 * 100 ));
		//	sp rintf(my_font_text," %d", (((int)(pd_weapon_power) * 200) >>8) );
			/* "5.00" */
			strcpy(my_font_text,"0000");
			dec_print_format( (((int)(cg.weapon_power) * (200*5)) >>8), 3, (char *)&my_font_text[0]);
		//	my_font_text[4] = 0;
			my_font_text[3] = my_font_text[2];
			my_font_text[2] = my_font_text[1];
			my_font_text[1] = CHR_PIRIOD_;
			#endif
		}
		//
	//	cg.SDL_font_type = FONT10W;
		cg.SDL_font_x = (PPP+10*8+7);//PPP+8*8+3
		cg.SDL_font_y = (17*8+5);//125/*+1*/-2
		font88_print_screen_xy();
	//
		#if 0/* �n���ɏ��������v���ɂȂ��Ă����̂Ŕp�~�B(r33) */
		/* [ �{���L�����ԕ\�� ] */
		//if (p->ex tra_type!=PLX_NONE)
		if (0 != cg.bomber_time)
		{
		//	sp rintf(my_font_text, STR_TIME_"%3d",(int)(((int)pd_bomber_time)/10));
		//	cg.SDL_font_type	= FONT10W;
		//	cg.SDL_font_x		= (PPP+3*8-6);
		//	cg.SDL_font_y		= (160);
		//	font88_print_screen_xy();
			strcpy(my_font_text, STR_TIME_"   ");
			dec_print_format( (int)(((int)cg.bomber_time) ), 3, (char *)&my_font_text[5]);
			my_font_text[7] = (0);	/*' '*/ 	/* 1���ڂ͕\�����Ȃ� */
		//
		//	cg.SDL_font_type	= FONT10W;
			cg.SDL_font_x		= (PPP+8*8+4);
			cg.SDL_font_y		= (22*8);
			font88_print_screen_xy();
		}
		#endif
	//
		/* --- �d�̃O���C�Y�J���X�g 99999�� (5��) --- */
		/* [ �O���C�Y�X�R�A�\�� ] */
		//	sp rintf(my_font_text," %d", pd_graze_point);
			strcpy(my_font_text,"   0");
			dec_print_format( cg.graze_point, 4, (char *)&my_font_text[0]);
		//
		//	cg.SDL_font_type	= FONT10W;
			cg.SDL_font_x		= (PPP+11*8+4);//(PPP+7*8+3)
			cg.SDL_font_y		= (20*8);//(140)
			font88_print_screen_xy();/*4��(�҂���)*/ /*3��(����Ȃ�)*/
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
		//
			strcpy(my_font_text, (char *)rank_name[((cg.game_difficulty))&(4-1)]);
		//	cg.SDL_font_type	= FONT10W;
			cg.SDL_font_x		= (PPP+/*7*/1*8);
			cg.SDL_font_y		= (256);
			font88_print_screen_xy();
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
	//	//	sp rintf(my_font_text, STR_TIME_"%3d",(int)(((int)ttt)));
	//		strcpy(my_font_text, STR_TIME_"00");
	//	//	dec_print_format(ttt,	3, (char *)&my_font_text[0/*5*/]);
	//		dec_print_format(ttt,	2, (char *)&my_font_text[0/*5*/]);
	//		font88_print_screen_xy(my_font_text, FONT10W, PPP,256);
//
	//		/*"60.0fps"*/
	//		strcpy(my_font_text, STR_TIME_"0000");
	//		dec_print_format(ttt,	3, (char *)&my_font_text[0/*5*/]);
	//		my_font_text[8-5] = my_font_text[7-5];
	//		my_font_text[7-5] = CHR_PIRIOD_;/*' ' '.'*/
	//		font88_print_screen_xy(my_font_text, FONT10W, PPP,256);
//
			/*"60.00fps"*/
			strcpy(my_font_text, /*STR_TIME_*/"00000"STR_FPS_);
			dec_print_format(ttt,	4, (char *)&my_font_text[0/*5*/]);
			my_font_text[9-5] = my_font_text[8-5];
			my_font_text[8-5] = my_font_text[7-5];
			my_font_text[7-5] = CHR_PIRIOD_;/*' ' '.'*/
		//
		//	cg.SDL_font_type	= FONT10W;
			cg.SDL_font_x		= (PPP+8*8+2);
			cg.SDL_font_y		= (256);
			font88_print_screen_xy();
		}
		#endif
	//}
}

/*---------------------------------------------------------
	�p�l���\���A������
---------------------------------------------------------*/

global void score_panel_init(void)
{
	top_score			= high_score_table[(cg_game_select_player)][0].score;	// ��ɕ\������n�C�R�A�̎擾=>score.c�ŗ��p
	strcpy(my_file_common_name, (char*)DIRECTRY_NAME_DATA_STR"/fonts/" "panel_base.png");	panel_base	= load_chache_bmp();//, 0, 1);
	strcpy(my_file_common_name, (char*)DIRECTRY_NAME_DATA_STR"/fonts/" "hosi_gauge.png");	star_gauge	= load_chache_bmp();//, 0, 1);	/*(char *)img_name[img_num]*/
	SDL_SetColorKey(star_gauge, (SDL_SRCCOLORKEY|SDL_RLEACCEL), 0x00000000);/* ���� SDL�����̂��ߕK�v */
}

