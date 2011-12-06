
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	このファイルは直接インクルードしません。
	"game_main.h" からのみ間接的にインクルードします。
---------------------------------------------------------*/

#ifndef _GAME_MY_MATH_H_
#define _GAME_MY_MATH_H_

#if 0
enum PspCtrlButtons
{
	PSP_CTRL_SELECT 	= 0x00000001,	/* Select button. */
	PSP_CTRL_START		= 0x00000008,	/* Start button. */
	PSP_CTRL_UP 		= 0x00000010,	/* Up D-Pad button. */
	PSP_CTRL_RIGHT		= 0x00000020,	/* Right D-Pad button. */
	PSP_CTRL_DOWN		= 0x00000040,	/* Down D-Pad button. */
	PSP_CTRL_LEFT		= 0x00000080,	/* Left D-Pad button. */
//
	PSP_CTRL_LTRIGGER	= 0x00000100,	/* Left trigger. */
	PSP_CTRL_RTRIGGER	= 0x00000200,	/* Right trigger. */
4
8
	PSP_CTRL_TRIANGLE	= 0x00001000,	/* Triangle button. */
	PSP_CTRL_CIRCLE 	= 0x00002000,	/* Circle button. */
	PSP_CTRL_CROSS		= 0x00004000,	/* Cross button. */
	PSP_CTRL_SQUARE 	= 0x00008000,	/* Square button. */
//
	PSP_CTRL_HOME		= 0x00010000,	/* Home button. In user mode this bit is set if the exit dialog is visible. */
	PSP_CTRL_HOLD		= 0x00020000,	/* Hold button. */
	PSP_CTRL_NOTE		= 0x00800000,	/* Music Note button. */
	PSP_CTRL_SCREEN 	= 0x00400000,	/* Screen button. */
	PSP_CTRL_VOLUP		= 0x00100000,	/* Volume up button. */
	PSP_CTRL_VOLDOWN	= 0x00200000,	/* Volume down button. */
	PSP_CTRL_WLAN_UP	= 0x00040000,	/* Wlan swi tch up. */
	PSP_CTRL_REMOTE 	= 0x00080000,	/* Remote hold position. */
	PSP_CTRL_DISC		= 0x01000000,	/* Disc present. */
	PSP_CTRL_MS 		= 0x02000000,	/* Memory stick present. */
};
#endif

/* 割り当てる機能 */
//	#define PSP_KEY_NONE			0
	#define PSP_KEY_SELECT			PSP_CTRL_SELECT
	#define PSP_KEY_PAUSE			PSP_CTRL_START
	#if (1)
	#define PSP_KEY_UP				PSP_CTRL_UP
	#define PSP_KEY_RIGHT			PSP_CTRL_RIGHT
	#define PSP_KEY_DOWN			PSP_CTRL_DOWN
	#define PSP_KEY_LEFT			PSP_CTRL_LEFT
	#endif
	#define PSP_KEY_SNAP_SHOT		PSP_CTRL_LTRIGGER
	#define PSP_KEY_SYSTEM			PSP_CTRL_RTRIGGER
	#define PSP_KEY_SLOW			PSP_CTRL_TRIANGLE
	#define PSP_KEY_OPTION			PSP_CTRL_CIRCLE
	#define PSP_KEY_SHOT_OK 		PSP_CTRL_CROSS
	#define PSP_KEY_BOMB_CANCEL 	PSP_CTRL_SQUARE

/* 仮想キー機能(==キーコンフィグ)の順番 */
enum
{
	KINOU_00_SELECT = 0,		/* SELECT */	//	#define PSP_KEY_SELECT			PSP_CTRL_SELECT
	KINOU_01_START, 			/* START */ 	//	#define PSP_KEY_PAUSE			PSP_CTRL_START
//	KINOU_02__UP,				/* 上 */		/*(r35-使用しない)*/
//	KINOU_03__RIGHT,			/* 右 */		/*(r35-使用しない)*/
//	KINOU_04__DOWN, 			/* 下 */		/*(r35-使用しない)*/
//	KINOU_05__LEFT, 			/* 左 */		/*(r35-使用しない)*/
	KINOU_02_LTRIGGER,			//__SNAP_SHOT,			/* L */ 		//	#define PSP_KEY_SNAP_SHOT		PSP_CTRL_LTRIGGER
	KINOU_03_RTRIGGER,			//__SYSTEM, 			/* R */ 		//	#define PSP_KEY_SYSTEM			PSP_CTRL_RTRIGGER
	KINOU_04_TRIANGLE,			//__SLOW,				/* △ */		//	#define PSP_KEY_SLOW			PSP_CTRL_TRIANGLE
	KINOU_05_CIRCLE,			//__OPTION, 			/* ○ */		//	#define PSP_KEY_OPTION			PSP_CTRL_CIRCLE
	KINOU_06_CROSS, 			//__SHOT_OK,			/* × */		//	#define PSP_KEY_SHOT_OK 		PSP_CTRL_CROSS
	KINOU_07_SQUARE,			//__BOMB_CANCEL,		/* □ */		//	#define PSP_KEY_BOMB_CANCEL 	PSP_CTRL_SQUARE
	KINOU_08_WARIATE_MAX		/* 割り当て最大数 */
};


extern void psp_fatal_error(char *msg, ...);

extern void *my_calloc(size_t size);

/*  */
#define USE_KETM_IMAGE_CHACHE (1)
#if (1==USE_KETM_IMAGE_CHACHE)
	extern SDL_Surface *load_chache_bmp(void);
	extern void unloadbmp_by_surface(SDL_Surface *surface);
#endif /*(1==USE_KETM_IMAGE_CHACHE)*/

extern char get_stage_chr(int i);


#endif /* _GAME_MY_MATH_H_ */
