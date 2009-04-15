#ifndef _SUPPORT_H_
#define _SUPPORT_H_


#include <psptypes.h>
#include <pspaudio.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <pspgu.h>
#include <pspiofilemgr.h>
#include <pspkernel.h>
#include <psppower.h>
#include <psprtc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include <pspdebug.h>

#include <unistd.h>
#include <stdarg.h>
//#include <math.h>
#include <dirent.h>
#include <ctype.h>

#ifdef ENABLE_PSP
	//# /* �J�X�^�����C�u�������g�� */
	#include "SDL.h"
#else
	//# /* �W�����C�u�������g�� */
	#include <SDL/SDL.h>
#endif





//#include <SDL/SDL.h>
//#include <pspkernel.h>
//#include <pspdebug.h>
//#include <pspctrl.h>
//#include <stdlib.h>
//#include <string.h>
//#include "stdarg.h"
#include <math.h>

#define clip_xmin(pnt) pnt->clip_rect.x
#define clip_xmax(pnt) pnt->clip_rect.x + pnt->clip_rect.w-1
#define clip_ymin(pnt) pnt->clip_rect.y
#define clip_ymax(pnt) pnt->clip_rect.y + pnt->clip_rect.h-1
#define sign(x) ((x) > 0 ? 1 : ((x) == 0 ? 0 : (-1) ))

/* PSP�̕\����ʗ̈� */
#define PSP_WIDTH   480
#define PSP_HEIGHT  272

/* �V���[�e�B���OGAME�����̕\����ʗ̈� */
#define GAME_WIDTH  380
#define GAME_HEIGHT 272

#ifndef M_PI
	#define M_PI		3.14159265358979323846
#endif

//#define M_PI_H		0.52359877559829887307		/* �� �� 6 */

#define degtorad(x) (((M_PI*2)/360.0)*(x))		/* 2�� �� 360 * X */
#define radtodeg(x) ( (int)((x)*((360.0)/(M_PI*2))+360)%360 )

/* �P����360�x�̒P�ʌn(deg360)���P�����Q�΂̒P�ʌn(radian)�֕ϊ��B�y�ыt�ϊ��B */
#define deg360_2rad(x) (((M_PI*2)/(360.0))*(x))
#define rad2deg360(x) ( (int)((x)*((360.0)/(M_PI*2))+360)%360 )

/* �P����512�x�̒P�ʌn(deg512)���P�����Q�΂̒P�ʌn(radian)�֕ϊ��B�y�ыt�ϊ��B */
#define deg512_2rad(x) (((M_PI*2)/(512.0))*(x))
//#define rad2deg512(x) ( (int)((x)*((512.0)/(M_PI*2))+512)%512 )
//#define rad2deg512(x) ( (int)((x)*((512.0)/(M_PI*2))+512)&(512-1) )
#define rad2deg512(x) ( (int)((x)*((512.0)/(M_PI*2))/*+512*/)&(512-1) )

/* �P����4096�x�̒P�ʌn(deg4096)���P�����Q�΂̒P�ʌn(radian)�֕ϊ��B�y�ыt�ϊ��B */
#define deg4096_2rad(x) (((M_PI*2)/(4096.0))*(x))
//#define rad2deg4096(x) ( (int)((x)*((4096.0)/(M_PI*2))+4096)%4096 )
//#define rad2deg4096(x) ( (int)((x)*((4096.0)/(M_PI*2))+4096)&(4096-1) )
#define rad2deg4096(x) ( (int)((x)*((4096.0)/(M_PI*2))/*+4096*/)&(4096-1) )

/* �P����65536�x�̒P�ʌn(deg65536)���P�����Q�΂̒P�ʌn(radian)�֕ϊ��B�y�ыt�ϊ��B */
#define deg65536_2rad(x) (((M_PI*2)/(65536.0))*(x))
//#define rad2deg65536(x) ( (int)((x)*((65536.0)/(M_PI*2))+65536)%65536 )
//#define rad2deg65536(x) ( (int)((x)*((65536.0)/(M_PI*2))+65536)&(65536-1) )
#define rad2deg65536(x) ( (int)((x)*((65536.0)/(M_PI*2))/*+65536*/)&(65536-1) )

/* �P����360�x�̒P�ʌn(deg360)���P����512�x�̒P�ʌn(deg512)�֕ϊ��B�y�ыt�ϊ��B */
#define deg_360_to_512(x) ((int)((x)*(512.0/360.0)))
#define deg_512_to_360(x) ((int)((x)*(360.0/512.0)))

/* �P����360�x�̒P�ʌn(deg360)���P����1024�x�̒P�ʌn(deg1024)�֕ϊ��B�y�ыt�ϊ��B */
#define deg_360_to_1024(x) ((int)((x)*(1024.0/360.0)))
#define deg_1024_to_360(x) ((int)((x)*(360.0/1024.0)))

/* �P����360�x�̒P�ʌn(deg360)���P����4096�x�̒P�ʌn(deg4096)�֕ϊ��B�y�ыt�ϊ��B */
#define deg_360_to_4096(x) ((int)((x)*(4096.0/360.0)))
#define deg_4096_to_360(x) ((int)((x)*(360.0/4096.0)))

/* �P���͈͓̔��ɃN���b�s���O */
#define mask512(aaa) {aaa &= (512-1);}
#define mask4096(aaa) {aaa &= (4096-1);}

/* �������X�R�A�ɕϊ��B�y�ыt�ϊ��B */
#define score(x)   ((int)(((int)x)/10))
#define score_r(x) ((int)(((int)x)*10))

/* ������256�Œ菬���_�`���ɕϊ��B�y��double�֋t�ϊ��B */
#define t256(x)             ((int)(x*256))
#define t256_to_double(x)   (((double)x)*(1.0/256.0))


/* ���W�A���o�ő΍�Bpsp�� double�̒l��ێ����邾���ł��x���B(�Ⴆ�Ί֐��̈����Ƃ�)
int�Œl��ێ����āA�g���x�ɕϊ��A�t�ϊ����������A�����Ƒ����B
�����Ń��W�A���o�ł���ׂ̃}�N�� */
//2687682 2688146
#if 0
	/* �P����512�x�̒P�ʌn�� atan2(y,x), cos(x), sin(x)  */
	#define atan_512(y,x)   (rad2deg512(atan2(y,x)))
	#define co_s512(x)   	(cos(deg512_2rad(x)))
	#define si_n512(x)   	(sin(deg512_2rad(x)))
	/* �ςȖ��O�ɂȂ��Ă�̂� cos( �Ƃ� sin( �Ō������邽�� */
#else
	#define SINTABLE_SIZE	512
	#define SINTABLE_MASK	(SINTABLE_SIZE-1)

	#define OFFS_SIN 0
	#define OFFS_COS ((SINTABLE_SIZE/4)-1)/*127 90*/
	extern int atan_512(int y, int x);
	extern double sin_tbl512[SINTABLE_SIZE];
	#define co_s512(x)   	(sin_tbl512[((x+OFFS_COS)&(512-1))])
	#define si_n512(x)   	(sin_tbl512[((x         )&(512-1))])
#endif

/* rand()�o�ő΍�B�{�Ƃɂ̓��v���C�@�\������B
���v���C�@�\���Č�����ׂɂ͓����̑g�ݍ��݊֐�rand()�͎g���Ȃ��B
�����̒������O�̋[�������̊֐���p�ӂ���K�v������B
���v���C�J�n����Q�[���J�n��(�ʂ̎n��)�ɗ����n��̏�����������B */
#define ra_nd(x)    	(rand(x))
/* �ςȖ��O�ɂȂ��Ă�̂� rand( �Ō������邽�� */


enum _errlevel
{
	ERR_FATAL=0,
	ERR_WARN,
//	ERR_INFO,
//	ERR_DEBUG,
};
#if 0
	#define CHECKPOINT error(ERR_DEBUG,"Checkpoint file:%s line:%d function:%s",__FILE__,__LINE__,__FUNCTION__)
#else
	#define CHECKPOINT ;
#endif

enum _state
{
	ST_GAME_QUIT=0,
//	ST_START_INTRO,
//	ST_INTRO,
	ST_MENU,
//	ST_GAME_DEMO,
	ST_GAME_PLAY,
	ST_GAME_OVER,
	ST_STORY,		/* [***20090223	�ǉ�  */
	ST_SHOW_HCLIST,
	ST_ENTRY_HCLIST,
	ST_KEY_CONFIG,
	ST_PLAYER_SELECT,
};
enum _keynum_		//�L�[�R���t�B�O�p
{
	KEY_NONE,
	KEY_SHOT,
	KEY_BOMB,
	KEY_SLOW,
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_PAUSE,
	KEY_CANCEL,
	KEY_SC_SHOT,
//	KEY_MAX 	/* �L�[�R���t�B�O�p�̍ő吔 */
};
#define PSP_KEY_NONE	0
#define PSP_KEY_SELECT	PSP_CTRL_SELECT
#define PSP_KEY_PAUSE	PSP_CTRL_START
#define PSP_KEY_UP		PSP_CTRL_UP
#define PSP_KEY_RIGHT	PSP_CTRL_RIGHT
#define PSP_KEY_DOWN	PSP_CTRL_DOWN
#define PSP_KEY_LEFT	PSP_CTRL_LEFT
#define PSP_KEY_SYSTEM	PSP_CTRL_LTRIGGER
#define PSP_KEY_SLOW	PSP_CTRL_RTRIGGER
#define PSP_KEY_SC_SHOT PSP_CTRL_TRIANGLE
#define PSP_KEY_CANCEL	PSP_CTRL_CIRCLE
#define PSP_KEY_SHOT	PSP_CTRL_CROSS
#define PSP_KEY_BOMB	PSP_CTRL_SQUARE

extern Uint32 my_pad;		/*�������*/
extern Uint32 my_pad_alter;	/*�O�����*/


#define IS_KEYBOARD_PULLED ((0==my_pad)&&(my_pad^my_pad_alter))/* �����L�[�𗣂��ꂽ�� */
/*  �L�[�𗣂����u�Ԃ��m��Ƃ���B�������u�Ԃ��m�肾�ƁA�����ꂽ�Ԃ������삷�镨�������ɂ���Ɠs���������B */

enum _game_rank_
{
	RANK_EASY=0,
	RANK_NORMAL/*=1*/,
	RANK_HARD/*=2*/,
	RANK_LUNATIC/*=3*/,
	RANK_MAX	/* �����N�̍ő吔==(�ō������N+1) */
};


struct _gamestate
{
	int mainstate;		/* Aktueller Status */
	int substate;		/* Aktueller Status Subsystem */
	int newstate;		/* 1: Statuswechsel */
};
typedef struct _gamestate GAMESTATE;

typedef struct
{
	int u;	//��
	int d;	//��
	int l;	//��
	int r;	//�E
	int ba; //�~
	int ma; //��
	int sa; //��
	int si; //��
	int rt; //R
	int lt; //L
	int sl; //SELECT
	int st; //START
} KEYCONFIG;


#include "font.h"
#include "menu.h"
#include "hiscore.h"
#include "fps.h"
#include "soundmanager.h"
#include "bg.h"
#include "startintro.h"

//#include "_resource.h"

char moddir[20];
char password[20];		/* [***090222 */

extern void game_init(void/*int argc, char *argv[]*/);
//void toggle_fullscreen(void);
extern void error(int errorlevel, char *msg, ...);
extern SDL_Surface *loadbmp0(char *filename, int use_alpha);
extern SDL_Surface *loadbmp(char *filename);
extern SDL_Surface *loadbmp2(char *filename);




extern void unloadbmp_by_surface(SDL_Surface *s);
extern void unloadbmp_by_name(char *name);
//static void imglist_add(SDL_Surface *s, char *name);
//static SDL_Surface *imglist_search(char *name);
//static void imglist_garbagecollect(void);
extern Uint32 getpixel(SDL_Surface *surface, int x, int y);
extern void putpixel(SDL_Surface *surface, int x, int y, Uint32 color);
//extern void draw_line(SDL_Surface *s, int x1, int y1, int x2, int y2, Uint32 farbe1, Uint32 farbe2);
//extern void draw_line_simple(SDL_Surface *s, int x1, int y1, int x2, int y2, Uint32 farbe1);
extern void blit_scaled(SDL_Surface *src, SDL_Rect *src_rct, SDL_Surface *dst, SDL_Rect *dst_rct);
//extern void blit_calpha(SDL_Surface *src, SDL_Rect *src_rct, SDL_Surface *dst, SDL_Rect *dst_rct);
extern void keyboard_clear(void);
extern void keyboard_poll(void);
//extern int keyboard_keypressed(void);
extern void newstate(int m, int s, int n);
extern void *mmalloc(size_t size);
//static void preload_gfx(void);
extern void load_ing(void);
extern void display_vidinfo(void);


extern void psp_clear_screen(void);
extern void psp_push_screen(void);
extern void psp_pop_screen(void);

/* 1:�G���f�B���O�f�o�b�O�@�\�B0:���̋@�\OFF */
#define USE_ENDING_DEBUG (1)

#endif
