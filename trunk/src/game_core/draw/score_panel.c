
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�X�R�A�p�l��(�\��)�֘A
	-------------------------------------------------------
	����(r33)SDL�ׁ̈A������������Ƃ��Ȃ荓���ł��B
---------------------------------------------------------*/
#include "111_my_file.h"/*(my_file_common_name)*/
#include "gu_draw_screen.h"
#include "../menu/kaiwa_sprite.h"


static SDL_Surface *panel_base; 	// �p�l���x�[�X
//static S_DL_Surface *star_gauge;
/*static*/extern SDL_Surface *FONT_fontimg;

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
//	#define STR_EASY		"EFGH" " "		/*4��*/
//	#define STR_NORMAL		"IJKLM" 		/*5��*/
//	#define STR_HARD		"NOPQ" " "		/*4��*/
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
	#define STR_EASY		"EFGH" " "		/*4��*/
	#define STR_NORMAL		"JKLMN" 		/*5��*/
	#define STR_HARD		"PQRS" " "		/*4��*/
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
	SDL�]��
---------------------------------------------------------*/
static SDL_Rect rect_dest;
static SDL_Rect rect_src;
static void SDL_blit_common(void)
{
//	PSPL_UpperBlit(star_gauge, &rect_src, cb.sdl_screen[SDL_00_VIEW_SCREEN], &rect_dest);
//	PSPL_UpperBlit(star_gauge/*power_gauge*/, &rect_src, cb.sdl_screen[SDL_00_VIEW_SCREEN], &rect_dest);
//	PSPL_UpperBlit(FONT_fontimg, &rect_src, cb.sdl_screen[SDL_00_VIEW_SCREEN], &rect_dest);/*power_gauge*/
	PSPL_UpperBlit(FONT_fontimg, &rect_src, cb.sdl_screen[SDL_00_VIEW_SCREEN], &rect_dest);
//	SDL_LowerBlit(FONT_fontimg, &rect_src, cb.sdl_screen[SDL_00_VIEW_SCREEN], &rect_dest);
}


/*---------------------------------------------------------
	�C�ӂ̃T�[�t�F�C�X�ɁA������������_�����O
---------------------------------------------------------*/

global void s_font88_print_screen_xy(void)
{
	rect_src.w = ( 8); rect_dest.w = (rect_src.w);
	rect_src.h = (10); rect_dest.h = (rect_src.h);
	unsigned int/*char*/ i;
	unsigned int/*char*/ j;
	i = 0;
//	for (i=0; i<strlen(my_font_text); i++)
	{
	loop_str:
		j = my_font_text[i];
		if (0==j)
		{
			return;
		}
		j -= 0x20;//0x20==' ';(space)
		rect_src.x = (j & 0x0f)*(rect_src.w);
		rect_src.y = ((j>>4))*(rect_src.h) + (0)/* y_offset*/;
		rect_dest.x = cg.PSPL_font_x + (i)*(rect_src.w);
		rect_dest.y = cg.PSPL_font_y;			/*0*/
		SDL_blit_common();/*(�����F0�]��)*/
		i++;
		goto loop_str;
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
//	if (value<1)	{	value=0;}
	value = psp_max(value, 0);
//
//	rect_src.x = (0);
	rect_src.x = (156);
	rect_src.y = (img_num);/*(0)*/
	rect_src.w = (10*value);
	rect_src.h = (10);
//
	rect_dest.x = PSP_WIDTH480-4-(10*value);
	rect_dest.y = y_offset;
	rect_dest.w = (100);
	rect_dest.h = (11);
	SDL_blit_common();/*(�P���]��)*/
}

/*---------------------------------------------------------
	�v���C���[�̃E�F�|���Q�[�W�̕\���̎q�֐�
---------------------------------------------------------*/

static void draw_power_gauge(int weapon)/*, int dx, int dy*/
{
	#define WP_GAUGE_X_OFS (PPP+48-2)
	#define WP_GAUGE_Y_OFS (128+8+4)
//	rect_src.x = (0);
	rect_src.x = (156);
	rect_src.y = (20);/*(0)*/
	rect_src.h = (13);
	#if 0
//	rect_src.w = (int)((dou ble)weapon / 127/*128*/ * (dou ble)power_gauge->w);
	#else
	/* 1[pixel]���炢�덷���邩������Ȃ����Ǌȗ���(������) */
	rect_src.w = ((int)(weapon * (80)/*power_gauge->w*/)>>7);
	#endif
	rect_dest.w = (80);/*power_gauge->w*/
	rect_dest.h = (13);/*power_gauge->h*/
	rect_dest.x = WP_GAUGE_X_OFS;/*dx*/
	rect_dest.y = WP_GAUGE_Y_OFS;/*dy*/
	SDL_blit_common();/*(�P���]��)*/
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
	/*"60.00fps"[���ʂɍl����Ɛ����^�Ōv�Z����ɂ� 33bit �v��̂ŁAunsigned int (32bit) �ő���Ȃ��B]*/
	/*[�������A(60*60 == 3600) �́A16 �Ŋ���؂��B(60*60)/16==(225)]*/
	/*[������ 16(==4bit)�Ŋ���΁A�K�v�Ȃ̂� 29bit(==33-4)�Ȃ̂ŁAunsigned int (32bit) �ő����B]*/
		ttt = ( (unsigned int)((((60*60)/16)*(1666666))) / (unsigned int)(ttt>>4));
	}
	return (ttt);
}

//static u8 *pb_image;
static void 	blit_panel_base(void)
{


//#define USE_PANEL_BASE_DIRECT (1)


/*([A]�p�l���x�[�X���u���b�N�]���BSDL��)*/
#if 0//(0==USE_PANEL_BASE_DIRECT)/*�e�X�goff*/
	/* [ �p�l���x�[�X��\�� ] */
	{
		SDL_Rect panel_base_r = {GAME_WIDTH, 0, 0, 0};	// �f�[�^�E�B���h�E�prect_srct->w,h,x,y
		PSPL_UpperBlit(panel_base, NULL, cb.sdl_screen[SDL_00_VIEW_SCREEN], &panel_base_r);
	}
#endif

//#else

/*([B]�p�l���x�[�X���u���b�N�]���BSDL�ł�P���ϊ�������)*/
#if (0)/*(�ǂ��킩��Ȃ����A�Ƃ肠���������ŁB������8bit�̃|�C���^�ŃA�h���X�v�Z�����Ă�̂œ���)*/
//	#define BG_PANEL_HAIKEI_OFFSET		((512*10))
	#define BG_PANEL_HAIKEI_OFFSET		((480-128))
	int x;
	int y;
	x= 0;
	y= 0;
	int haikei_offset;
	haikei_offset	= (0);
	unsigned int dy;
	for (dy=0; dy<272/*16*/ /*KANJI_FONT_16_HEIGHT_P0*/; dy++)
	{
		unsigned int dx;
		for (dx=0; dx<(128)/*16*/ /*KANJI_FONT_08_HARF_WIDTH*/; dx++)
		{
				#if (0==USE_32BIT_DRAW_MODE)/*(16bit mode)*/
				/*
					���܂����ǂ��킩��Ȃ�(���A�Ƃ肠��������)�B
					�A�h���X�̓o�C�g�P�ʂȂ̂� u8 �|�C���^�łb����Ɍv�Z�����Ă�B
					(short �Ȃ�2�Ŋ���Ƃ��Aint �Ȃ�4�Ŋ���Ƃ�)
					�ς���ꍇ�́A�萔(BG_PANEL_HAIKEI_OFFSET_U16��)��ϐ�(dy,y,��)���ς��Ȃ��ƃ_���B
				*/
			//	s_getpixel16
			volatile u8 *src_p = (u8 *)(/*font_bg_bitmap_surface_image*/((panel_base->pixels)/*FONT_fontimg->pixels*/)/*cb.kanji_window_screen_image*/) /*surface*/
					+ (dy * (/*��128�Ȃ̂�*/256/*512*/ /*font_bg_bitmap_surface_pitch*/))		/*surface*/
					+ (dx+dx)
					+ (haikei_offset+haikei_offset);
			//	putpixel16
			volatile u8 *dst_p = (u8 *)(cb.sdl_screen[SDL_00_VIEW_SCREEN]->pixels/*cb.kanji_window_screen_image*/)/*surface->pixels*/
					+ (((dy+y))*(512*2))/*surface->pitch*/
					+ (dx+dx)
					+ (x+x)
					+ (BG_PANEL_HAIKEI_OFFSET*2);	/* 512[pixel]x 2[bytes](short) */
				*(u16 *)dst_p = (u32)(*(u16 *)src_p);
				#else/*(32bit mode)*/
				/*
					���܂����ǂ��킩��Ȃ�(���A�Ƃ肠��������)�B
					�A�h���X�̓o�C�g�P�ʂȂ̂� u8 �|�C���^�łb����Ɍv�Z�����Ă�B
					(short �Ȃ�2�Ŋ���Ƃ��Aint �Ȃ�4�Ŋ���Ƃ�)
					�ς���ꍇ�́A�萔(BG_PANEL_HAIKEI_OFFSET_U16��)��ϐ�(dy,y,��)���ς��Ȃ��ƃ_���B
				*/
			//	s_getpixel16
			volatile u8 *src_p = (u8 *)(/*font_bg_bitmap_surface_image*/((panel_base->pixels)/*FONT_fontimg->pixels*/)/*cb.kanji_window_screen_image*/) 	/*surface*/
					+ (dy * (2*512/*font_bg_bitmap_surface_pitch*/))		/*surface*/
					+ (dx+dx+dx+dx)
					+ (haikei_offset+haikei_offset+haikei_offset+haikei_offset);
			//	putpixel16
			volatile u8 *dst_p = (u8 *)(cb.sdl_screen[SDL_00_VIEW_SCREEN]->pixels/*cb.kanji_window_screen_image*/)/*surface->pixels*/
					+ (((dy+y))*(2*512*2))/*surface->pitch*/
					+ (dx+dx+dx+dx)
					+ (x+x+x+x)
					+ (BG_PANEL_HAIKEI_OFFSET*4);	/* 512[pixel]x 2[bytes](short) */
			//	*(u16 *)dst_p = (u32)(*(u16 *)src_p);
				*(u32 *)dst_p = (u32)(*(u16 *)src_p);
				#endif
		}
	}
#endif

/*([C]�p�l���x�[�X���u���b�N�]���B�Ƃ肠�������ʂɏ�������)*/
#if (1)/*(16bit�����Ă��ƁB�Ƃ肠���������ŁB)*/
	// 512 == (psp��)vram�� == cb.sdl_screen[SDL_00_VIEW_SCREEN]�̕��B
	// 128 == �E�̃p�l�����B
	//  32 == ���̃p�l�����B
	// 384 == 512-128 == 1�s�̉��Z�����B
	// 352 == 512-128-32 == �n�߂̈ʒu�͍��̃p�l�������l�����Ȃ��ƁB
				#if (0==USE_32BIT_DRAW_MODE)/*(16bit mode)*/
	//[�Ƃ肠��������]
	// 16[bit]�̃|�C���^�Ȃ̂� +1 ����� 2[byte] ���Z����鎖�ɒ��ӁB
	volatile u16 *dst_p = (u16 *)(cb.sdl_screen[SDL_00_VIEW_SCREEN]->pixels/*cb.kanji_window_screen_image*/) + (352);/*(�n�߂̈ʒu, 352x2[bytes])*/
	volatile u16 *src_p = (u16 *)(/*font_bg_bitmap_surface_image*/((panel_base->pixels)/*FONT_fontimg->pixels*/)/*cb.kanji_window_screen_image*/) /*surface*/;
	unsigned int dy;
	for (dy=0; dy<272; dy++)/*(psp�c272[pixel])*/
	{
		unsigned int dx;
		for (dx=0; dx<(128); dx++)/*(�E�̃p�l����128[pixel])*/
		{
		//	*dst_p = (u16)(0x11fa);/*(�f�o�b�O�p�_�~�[�F)*/
			*dst_p = *src_p;
			dst_p += (1);/*(1==�킴�Ƃ炵���ł���, +2[bytes])*/
			src_p += (1);/*(1==�킴�Ƃ炵���ł���, +2[bytes])*/
		}
		dst_p += (384);/*(1�s�̉��Z����, 384x2[bytes])*/
	}
				#else/*(32bit mode)*/
	//[���e�X�g�B��̂������̂œ����C�����邪���e�X�g]
	// 32[bit]�̃|�C���^�Ȃ̂� +1 ����� 4[byte] ���Z����鎖�ɒ��ӁB
	volatile u32 *dst_p = (u32 *)(cb.sdl_screen[SDL_00_VIEW_SCREEN]->pixels/*cb.kanji_window_screen_image*/) + (352);/*(�n�߂̈ʒu, 352x4[bytes])*/
	volatile u32 *src_p = (u32 *)(/*font_bg_bitmap_surface_image*/((panel_base->pixels)/*FONT_fontimg->pixels*/)/*cb.kanji_window_screen_image*/) /*surface*/;
	unsigned int dy;
	for (dy=0; dy<272; dy++)/*(psp�c272[pixel])*/
	{
		unsigned int dx;
		for (dx=0; dx<(128); dx++)/*(�E�̃p�l����128[pixel])*/
		{
		//	*dst_p = (u32)(0xff11ffaa);/*(�f�o�b�O�p�_�~�[�F)*/
			*dst_p = *src_p;
			dst_p += (1);/*(1==�킴�Ƃ炵���ł���, +4[bytes])*/
			src_p += (1);/*(1==�킴�Ƃ炵���ł���, +4[bytes])*/
		}
		dst_p += (384);/*(1�s�̉��Z����, 384x4[bytes])*/
	}
				#endif
#endif


/*([D]�p�l���x�[�X������(0)�ŏ����BGu�ł��Ƃœ]��)*/
#if 0
/*(��̓z�R�s�y����src_p�폜���āA�f�[�^�[��(0)�ɂ���BSDL������Ƃ߂�ǂ�������Ȃ��B)*/
/*(���ꂩmemcpy32�̕����ǂ������H)*/
/*(�p�l���x�[�X�̃R�s�[�́A sceGuCopyImage() ������ł��������ȋC������B)*/
	/* ����Ȋ������ȁH */
//	sceGuStart(GU_DIRECT, gulist);
//	sceGuCopyImage(
//		/*GU_PSM_5551*/SDL_GU_PSM_0000/*GU_PSM_5650*/,	//	int psm,	GU_PSM_8888 /* Image format */
//		0,						//	int sx, 		/* src location */
//		0,						//	int sy, 		/* src location */
//		(128)/*512*/,			//	int width,		/* Image size */�E�̃p�l����
//		272,					//	int height, 	/* Image size */�E�̃p�l������
//		(128)/*512*/, 			//	int srcw,		/* src buffer width */�E�̃p�l����
//		(panel_base->pixels),	//	void* src,		/* src Image from RAM */�]�����B
//		(352)/*0*/, 			//	int dx, 		/* dest location */
//		0,						//	int dy, 		/* dest location */
//		512,					//	int destw,		/* dest buffer width */
//		draw_frame/*dest_p*/	//	void* dest		/* dest Image to VRAM */
//	);
//	sceGuFinish();
//	sceGuSync(0, 0);
#endif
}


//static u32 top_score;
global void score_display(void)
{
	blit_panel_base();/*(�P���]��)*/
//
	/* [ �v���C���[���\�� ] */
	draw_stars_status( R_00_aka_hosi_png,  (cg.zanki), 10*8+4); /*R_01_mizu_hosi_png*/
//
	/* [ �{�����\�� ] */
	draw_stars_status( R_01_mizu_hosi_png, (cg.bombs), 14*8+1); /*R_00_aka_hosi_png*/

	//{/*�����̂��X�R�[�v���Ȃ������ǂ�(�������X�R�[�v����Ȃ���,�R�[�h���ς��)*/

	/* �ʏ펞(�f�o�b�O�ȊO�͎���(��炢�{�����[�h��)) */

		#if (0/*(r39Gu���ς�)*/)/* [ �n�C�X�R�A��r ] */
	//	if (top_score < cg.game_score)
	//	{	top_score = cg.game_score;}
		top_score = psp_max(top_score, cg.game_score);
		#endif
		//
		#if (0/*(r39Gu���ς�)*/)/* [ �n�C�X�R�A�\�� ] */
	//	sp rintf(my_font_text,"%09d0", top_score);
		strcpy(my_font_text, "0000000000");
		dec_print_format(top_score, 	9/*8*/, (char *)my_font_text);
		//
		cg.PSPL_font_x		= (PPP+5*8+4);
		cg.PSPL_font_y		= (3*8+2);
		s_font88_print_screen_xy();
		#endif
		//
		#if (0/*(r39Gu���ς�)*/)/* [ �X�R�A�\�� ] */
	//	sp rintf(my_font_text,"%09d0", pd_game_score);
		strcpy(my_font_text, "0000000000");
		dec_print_format(cg.game_score, 9/*8*/, (char *)my_font_text);
		//
	//	cg.PSPL_font_x		= (PPP+5*8+4);
		cg.PSPL_font_y		= (6*8+7);
		s_font88_print_screen_xy();
		#endif
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
		cg.PSPL_font_x = (PPP+10*8+7);//PPP+8*8+3
		cg.PSPL_font_y = (17*8+5);//125/*+1*/-2
		s_font88_print_screen_xy();
	//
		#if (0/*(r39Gu���ς�)*/)/* [ �O���C�Y�X�R�A�\�� ] */
		/* --- �d�̃O���C�Y�J���X�g 99999�� (5��) --- */
		//	sp rintf(my_font_text," %d", pd_graze_point);
			strcpy(my_font_text,"   0");
			dec_print_format(cg.graze_point, 4, (char *)&my_font_text[0]);
		//
			cg.PSPL_font_x		= (PPP+11*8+4);//(PPP+7*8+3)
			cg.PSPL_font_y		= (20*8);//(140)
			s_font88_print_screen_xy();/*4��(�҂���)*/ /*3��(����Ȃ�)*/
		#endif
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
			cg.PSPL_font_x		= (PPP+/*7*/1*8);
			cg.PSPL_font_y		= (256);
			s_font88_print_screen_xy();
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
	//		s_font88_print_screen_xy(my_font_text, FONT10W, PPP,256);
//
	//		/*"60.0fps"*/
	//		strcpy(my_font_text, STR_TIME_"0000");
	//		dec_print_format(ttt,	3, (char *)&my_font_text[0/*5*/]);
	//		my_font_text[8-5] = my_font_text[7-5];
	//		my_font_text[7-5] = CHR_PIRIOD_;/*' ' '.'*/
	//		s_font88_print_screen_xy(my_font_text, FONT10W, PPP,256);
//
			/*"60.00fps"*/
			strcpy(my_font_text, /*STR_TIME_*/"00000"STR_FPS_);
			dec_print_format(ttt,	4, (char *)&my_font_text[0/*5*/]);
			my_font_text[9-5] = my_font_text[8-5];
			my_font_text[8-5] = my_font_text[7-5];
			my_font_text[7-5] = CHR_PIRIOD_;/*' ' '.'*/
		//
			cg.PSPL_font_x		= (PPP+8*8+2);
			cg.PSPL_font_y		= (256);
			s_font88_print_screen_xy();
		}
		#endif
	//}
}

/*---------------------------------------------------------

---------------------------------------------------------*/
//extern void kaiwa_obj_set2n(unsigned int obj_number);
static/*global*/ void kaiwa_obj_set2n(unsigned int obj_number)
{
	KAIWA_OBJ *my_std_obj;
	my_std_obj = &kaiwa_sprite[obj_number]; /* �ėp�I�u�W�F */
	/*(Gu�̏ꍇ)*/
	my_std_obj->draw_flag = (1);	/* �`�悷��B */
	/*(�c�����A�����I��2^n�ɕϊ�)*/
	my_std_obj->width_2n	= (1<<(32 - __builtin_allegrex_clz((my_resource[TEX_09_TACHIE_L+obj_number].texture_width)-1)));/*/frames*/ 	//tmp->cw		= ((tmp->w)>>1);
	my_std_obj->height_2n	= (1<<(32 - __builtin_allegrex_clz((my_resource[TEX_09_TACHIE_L+obj_number].texture_height)-1)));				//tmp->ch		= ((tmp->h)>>1);
}

/*---------------------------------------------------------
	�p�l���\���A������
---------------------------------------------------------*/
extern void set_topscore(void);
global void score_panel_init(void)
{
	#if 0/*(???)*/
	/*(��b�X�v���C�g�̐ݒ�B�\����off�ɂ���B)*/
	kaiwa_sprite[0].draw_flag = (1);	/* �`�悷��B */
	kaiwa_sprite[1].draw_flag = (1);	/* �`�悷��B */
	kaiwa_sprite[0].cx256 = t256(480+256);
	kaiwa_sprite[1].cx256 = t256(480+256);
	kaiwa_sprite[0].cy256 = t256(272+256);
	kaiwa_sprite[1].cy256 = t256(272+256);
	#endif
	/*(��b�X�v���C�g�̐ݒ�B�T�C�Y��128x256�ɂ���B)*/
	unsigned int i;
	for (i=0; i<KAIWA_OBJ_99_MAX; i++)
	{
		my_resource[TEX_09_TACHIE_L+i].texture_width	= (128);/*(�]����摜�̉���)*/
		my_resource[TEX_09_TACHIE_L+i].texture_height	= (256);/*(�]����摜�̏c��)*/
		my_resource[TEX_09_TACHIE_L+i].buffer_width 	= (128);/*(�]�����摜�̃o�b�t�@��)*/
		kaiwa_obj_set2n(i);
	}
	/*(��b�X�v���C�g�̐ݒ�B�\����off�ɂ���B)*/
	kaiwa_sprite[0].draw_flag = (0);	/* �`�悵�Ȃ��B */
	kaiwa_sprite[1].draw_flag = (0);	/* �`�悵�Ȃ��B */
	//
	set_topscore();
	strcpy(my_file_common_name, (char*)DIRECTRY_NAME_DATA_STR"/fonts/" "panel_base.png");
	//#if 0// memory on load �ŉ�����Ȃ��B==�摜�L���b�V�����Ȃ��B(1==USE_KETM_IMAGE_CHACHE)
	#if (1==USE_KETM_IMAGE_CHACHE)/*(????)*/
	panel_base	= load_chache_bmp();
	#else
	panel_base	= IMG_Load(my_file_common_name);/*(���̂��A�I�������Ȃ�)*/
	#endif /*(1==USE_KETM_IMAGE_CHACHE)*/


//	#if (1==USE_PANEL_BASE_DIRECT)
//	pb_image = (panel_base->pixels);
//	#endif

}


/*---------------------------------------------------------
	��b�X�v���C�g�̐ݒ�B�T�C�Y�� 256x256�ɂ���B
---------------------------------------------------------*/

global void kaiwa_obj_set_256(void)
{
	unsigned int i;
	for (i=0; i<KAIWA_OBJ_99_MAX; i++)
	{
		my_resource[TEX_09_TACHIE_L+i].texture_width	= (256);/*(�]����摜�̉���)*/
		my_resource[TEX_09_TACHIE_L+i].texture_height	= (256);/*(�]����摜�̏c��)*/
		my_resource[TEX_09_TACHIE_L+i].buffer_width 	= (256);/*(�]�����摜�̃o�b�t�@��)*/
		kaiwa_obj_set2n(i);
	}
}

