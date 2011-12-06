
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	SDL�t�H���g
	-------------------------------------------------------
	�Ȃ񂩃t�H���g����������VRAM��������Gu����j�ނ̂ŐF�X�p�~���B
	Gu�ɂȂ�΁A�g��k����F�ς�����ł��邵�B
	-------------------------------------------------------
16x6

 !"#$%&'()*+,-./
0123456789:;<=>?
@ABCDEFGHIJKLMNO
PQRSTUVWXYZ[\]^_
`abcdefghijklmno
pqrstuvwxyz{|}~d

---------------------------------------------------------*/

#include "111_my_file.h"/*(my_file_common_name)*/

global char my_font_text[MY_FONT_256_TEXT_LENGTH];
/*---------------------------------------------------------
	SDL�t�H���g
---------------------------------------------------------*/

/*static*/global SDL_Surface *FONT_fontimg;

/*---------------------------------------------------------
	SDL�t�H���g���N�����Ɉ�x��������������B
---------------------------------------------------------*/

/*only boot once*/global void font_system_boot_init(void)
{
	strcpy(my_file_common_name, DIRECTRY_NAME_DATA_STR "/fonts/" "r35font.png");
	#if (0)/*(�L���b�V��������}�Y���B�Ǝv���B�����B)*/
	FONT_fontimg = load_chache_bmp();//, 0, 1);
	#else/*(r35)*/
	{
		SDL_Surface *s1;
		s1 = IMG_Load(my_file_common_name);
		FONT_fontimg = SDL_DisplayFormat(s1);/* �T�[�t�F�X��\���t�H�[�}�b�g�ɕϊ�����B */
	}
	#endif
	SDL_SetColorKey(FONT_fontimg, (SDL_SRCCOLORKEY/*|SDL_RLEACCEL*/), 0x00000000);
}


//typedef struct
//{
//	u8 w;
//	u8 h;
//	u8 x_offset;
//	u8 y_offset;
//} FONT_SIZE;
//static const FONT_SIZE fonts[FONT_MAX] =
//{
//	{ 8, 10, 0,   0},/* �X�e�[�^�X�p�l����p�t�H���g�A�{�X���[�^�[ */
//	{16, 16, 0,  64},/* �����̂a�f�p */
//	{16, 16, 0,  32},/* �L�[�R���t�B�O�p */
//	{16, 16, 0, 160},/* ���j���[��ʂ̎菑�����A�����t�H���g(�g������) */
//};

/*---------------------------------------------------------
	�C�ӂ̃T�[�t�F�C�X�ɁA������������_�����O
---------------------------------------------------------*/
/*, char *text, int font_number, int x_offset, int y_offset*/
static void font16_render_surface_xy(SDL_Surface *txt_image_surface)
{
	SDL_Rect s;
	SDL_Rect d;
	s.w = (16); d.w = (16);
	s.h = (16); d.h = (16);
	unsigned int/*char*/ i;
	unsigned int/*char*/ char_number;
	i = 0;
//	for (i=0; i<strlen(my_font_text); i++)
	{
	loop_str:
		char_number = my_font_text[i];
		if (0==char_number)
		{
			return;
		}
		char_number ^= 0x80;// �㉺����ւ�
//
		s.x = ((char_number & 0x0f)<<4);
		s.y = (((char_number&0xf0) ));  /*+y_offset*/
		d.x = cg.PSPL_font_x + ((i)<<(4));
		d.y = cg.PSPL_font_y;			/*0*/
		PSPL_UpperBlit(FONT_fontimg, &s, txt_image_surface, &d);
		i++;
		goto loop_str;
	}
}

#if 1
/*---------------------------------------------------------
	�T�[�t�F�C�X�����Ȃ��ŁA���ډ�ʂɕ\��
---------------------------------------------------------*/

//char *text, int font_number, int x, int y)
//global void font88_print_screen_xy(void)
global void font_render_view_screen(void)
{
//	SDL_SetColorKey(fonts[font_number].fontimg/*screen*/, (SDL_SRCCOLORKEY|SDL_RLEACCEL), 0x00000000);
	font16_render_surface_xy(cb.sdl_screen[SDL_00_VIEW_SCREEN]/*, text, font_number, x, y*/);
}
#endif

/*---------------------------------------------------------
	[�p�~�\��] �V�K�T�[�t�F�C�X���쐬���A������������_�����O
---------------------------------------------------------*/
//static int my_strlen(u8 *my_font_text)
static int my_font_len16(void)
{
	int i;
	i = 0;
	int count;
	count = 0;
count_loop:
	if (0==my_font_text[i])
	{
		return (count);
	}
	i++;
	count += 16;
	goto count_loop;
}
/*char *text, int font_number*/
global SDL_Surface *font16_render_new_surface(void)
{
	SDL_Surface *txt_image_surface;
	txt_image_surface = /*()*/
	SDL_CreateSurface
	(
		/* ���̕�(�n���O�A�b�v����)�o�O����񂾂��ǁA�ǂ��킩��Ȃ��B */
		#if 0
		/* VRAM�Ɏ�������������͂������A���ۂ͒x���Ȃ����肷��B�n�߂��������āA���񂾂�x���Ȃ�(���t���[�����Ƃ�)�ς����Ȃ����x���Ȃ�B */
		/* PSP�p��SDL��VRAM�Ǘ� http://psp.jim.sh/svn/filedetails.php?repname=psp&path=%2Ftrunk%2FSDL%2Fsrc%2Fvideo%2Fpsp%2FSDL_pspvideo.c �ł� */
		/* ���X�g�����ŊǗ����Ă邪�A���̃��X�g�����̂����ȋC������Brealloc(); ���̂����������B(???) */
		/* sceGeEdramGetAddr(); �� sceGeEdramGetSize(); �̕ӂ́A���m�ɃT�C�Y���擾�ł��Ȃ��C������B(???) */
		SDL_SRCCOLORKEY|SDL_HWSURFACE,	/* VRAM�֊m�ۂ���(VRAM������Ȃ��ꍇ�́ASDL�̓��C���������֎��) */
		#else
		/* ���󂱂����̕����܂����ۂ��B(???) */
		/* ���Ԃ񃁃����[���[�N����B(gameover.c SDL_FreeSurface(go_surface1); SDL_FreeSurface(go_surface2); �Ƃ���) */
		SDL_SRCCOLORKEY|SDL_SWSURFACE/*SDL_HWSURFACE*/,/* ���C���������֊m�ۂ��� */
		#endif
	//	my_strlen(my_font_text)*(16)/*font width*/,
		my_font_len16()/*font width*/,
		(16)/*font height*/
	);
	SDL_FillRect(txt_image_surface, NULL, 0/*SD L_MapRGB(txt_image_surface->format,0,0,0)*/);
	SDL_SetColorKey(txt_image_surface, (SDL_SRCCOLORKEY/*|SDL_RLEACCEL*/), 0x00000000);
	cg.PSPL_font_x = 0;
	cg.PSPL_font_y = 0;
	font16_render_surface_xy(txt_image_surface);/*, text, font_number, 0, 0*/
	return (txt_image_surface);
}
