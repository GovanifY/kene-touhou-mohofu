
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
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

global void font_init(void)
{
	strcpy(my_file_common_name, DIRECTRY_NAME_DATA_STR"/fonts/""r35font.png");
	#if (0)/*(�L���b�V��������}�Y���B�Ǝv���B�����B)*/
	FONT_fontimg = load_chache_bmp();//, 0, 1);
	#else/*(r35)*/
	{
		SDL_Surface *s1;
		s1 = IMG_Load(my_file_common_name);
		FONT_fontimg = SDL_DisplayFormat(s1);/* �T�[�t�F�X��\���t�H�[�}�b�g�ɕϊ�����B */
	}
	#endif
	SDL_SetColorKey(FONT_fontimg, (SDL_SRCCOLORKEY|SDL_RLEACCEL), 0x00000000);
}


typedef struct
{
	u8 w;
	u8 h;
	u8 x_offset;
	u8 y_offset;
} FONT_SIZE;
static const FONT_SIZE fonts[FONT_MAX] =
{
	{ 8, 10, 0,   0},/* �X�e�[�^�X�p�l����p�t�H���g�A�{�X���[�^�[ */
	{16, 16, 0,  64},/* �����̂a�f�p */
	{16, 16, 0,  32},/* �L�[�R���t�B�O�p */
	{16, 16, 0, 160},/* ���j���[��ʂ̎菑�����A�����t�H���g(�g������) */
};

/*---------------------------------------------------------
	�C�ӂ̃T�[�t�F�C�X�ɁA������������_�����O
---------------------------------------------------------*/
/*, char *text, int font_number, int x_offset, int y_offset*/
static void font_render_surface_xy(SDL_Surface *txt_image_surface)
{
	SDL_Rect s;
	SDL_Rect d;
	s.w = fonts[cg.SDL_font_type].w; d.w = (s.w);	/*fonts[cg.SDL_font_type].w*/
	s.h = fonts[cg.SDL_font_type].h; d.h = (s.h);	/*fonts[cg.SDL_font_type].h*/
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
	//	s.x = j*(s.w);						/*fonts[cg.SDL_font_type].w*/
		s.x = (j & 0x0f)*(s.w); 			/*fonts[cg.SDL_font_type].w*/
	//	s.y = 0;
		s.y = ((j>>4))*(s.h) + fonts[cg.SDL_font_type].y_offset;//((cg.SDL_font_type)<<6);
		d.x = cg.SDL_font_x + (i)*(s.w);	/*fonts[cg.SDL_font_type].w*/
		d.y = cg.SDL_font_y;			/*0*/
	//	PSPL_UpperBlit(fonts[cg.SDL_font_type].fontimg, &s, txt_image_surface, &d);
		PSPL_UpperBlit(FONT_fontimg, &s, txt_image_surface, &d);
		i++;
		goto loop_str;
	}
}


/*---------------------------------------------------------
	�T�[�t�F�C�X�����Ȃ��ŁA���ډ�ʂɕ\��
---------------------------------------------------------*/

//char *text, int font_number, int x, int y)
global void font88_print_screen_xy(void)
{
//	SDL_SetColorKey(fonts[font_number].fontimg/*screen*/, (SDL_SRCCOLORKEY|SDL_RLEACCEL), 0x00000000);
	font_render_surface_xy(cb.sdl_screen[SDL_00_VIEW_SCREEN]/*, text, font_number, x, y*/);
}


/*---------------------------------------------------------
	[�p�~�\��] �V�K�T�[�t�F�C�X���쐬���A������������_�����O
---------------------------------------------------------*/
/*char *text, int font_number*/
global SDL_Surface *font_render_new_surface(void)
{
	SDL_Surface *txt_image_surface;
	txt_image_surface = SDL_CreateRGBSurface(
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
		strlen(my_font_text)*fonts[cg.SDL_font_type].w,
		fonts[cg.SDL_font_type].h,
		cb.sdl_screen[SDL_00_VIEW_SCREEN]->format->BitsPerPixel,
		cb.sdl_screen[SDL_00_VIEW_SCREEN]->format->Rmask,
		cb.sdl_screen[SDL_00_VIEW_SCREEN]->format->Gmask,
		cb.sdl_screen[SDL_00_VIEW_SCREEN]->format->Bmask,
		cb.sdl_screen[SDL_00_VIEW_SCREEN]->format->Amask);
	SDL_FillRect(txt_image_surface, NULL, 0/*SD L_MapRGB(txt_image_surface->format,0,0,0)*/);
	SDL_SetColorKey(txt_image_surface, (SDL_SRCCOLORKEY|SDL_RLEACCEL), 0x00000000);
	cg.SDL_font_x = 0;
	cg.SDL_font_y = 0;
	font_render_surface_xy(txt_image_surface);/*, text, font_number, 0, 0*/
	return (txt_image_surface);
}
