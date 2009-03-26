#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <SDL/SDL.h>
#include <stdlib.h>
#include <math.h>

/* Sprites */
//#define MAX_SPRITE 200

#if 1
/*Old r14*/
enum _priority
{
	PR_BACK0 = 0,
	PR_GROUNDER,
	PR_BACK1,
	PR_BACK2,
	PR_ENEMY,
	PR_ENEMY_WEAPON,
	PR_TARGET,
	PR_BONUS,
	PR_PLAYER,
	PR_PLAYER2,
	PR_TMP,
	PR_TEXT,
	PR_FRONT0,
	PR_FRONT1,
	PR_FRONT2
};
#else
/*New r17*/
enum _priority
{
	PR_BACK0 = 0,
	PR_GROUNDER,
	PR_BACK1,
	PR_BACK2,
	PR_PLAYER,
	PR_ENEMY,
	PR_ENEMY_WEAPON,
	PR_TARGET,
	PR_BONUS,
	PR_PLAYER2,
	PR_TMP,
	PR_TEXT,
	PR_FRONT0,
	PR_FRONT1,
	PR_FRONT2
};
#endif

#define SP_SHOW_PLAYER			0x0100
#define SP_SHOW_PLAYER_WEAPONS	0x0200
#define SP_SHOW_ENEMYS			0x0400
#define SP_SHOW_ENEMY_WEAPONS	0x0800
#define SP_SHOW_BONUS			0x1000
#define SP_SHOW_MENUTEXT		0x2000
#define SP_SHOW_ETC 			0x4000
#define SP_SHOW_ALL 			0xff00

enum SPRITE_TYPE
{
	SP_PLAYER		= /*0x0100*/SP_SHOW_PLAYER, 	/* Unser Held */
	SP_PLAYER2,

	SP_PL_PLASMA	= /*0x0200*/SP_SHOW_PLAYER_WEAPONS, 	/* Waffen des Helden */
	SP_PL_FIREBALL,
	SP_PL_KILLRAY,
	SP_PL_BOMB,
	SP_PL_HOMING,
	SP_PL_SHIELD,
	SP_PL_HLASER,

	SP_EN_EYEFO 	= /*0x0400*/SP_SHOW_ENEMYS, 	/* Die Boesen */
	SP_EN_XEV,
	SP_EN_CRUSHER,
	SP_EN_CUBE,
	SP_EN_DRAGER,
	SP_EN_HOMING,
	SP_EN_MINE,
	SP_EN_RWINGX,
	SP_EN_CIR,
	SP_EN_ZATAK,
	SP_EN_BADGUY,
	SP_EN_GROUNDER,
	SP_EN_BGPANEL,
	SP_EN_PROBALL,
	SP_EN_PLASMABALL,
	SP_EN_MING,
	SP_EN_GREETER,
	SP_EN_CURVER,
	SP_EN_SPLASH,
	SP_EN_FAIRY,
	SP_EN_GFAIRY,
	#if 0
	SP_EN_BOSS01,
	SP_EN_BOSS02,
	SP_EN_BOSS03,
	SP_EN_BOSS04,
	#else
	SP_EN_BOSS,	//[***090325
	#endif
	SP_EN_BULLET		= /*0x0800*/SP_SHOW_ENEMY_WEAPONS,		/* ... und ihre Waffen */
	SP_EN_LASER,
	SP_EN_BIGBULLET,
	SP_EN_BIGBULLET_DUMMY,
	SP_EN_BOSS02ICE,

	SP_BONUS_FIREPOWER	= /*0x1000*/SP_SHOW_BONUS,	/* Bonusitems */
	SP_BONUS_COIN,
	SP_BONUS_ADDSPEED,
	SP_BONUS_BOMB,
	SP_BONUS_EXTRA,
	SP_BONUS_HEALTH,
	SP_BONUS_LAST,
	SP_BONUS_FIREPOWER_G,

	SP_MENUTEXT 		= /*0x2000*/SP_SHOW_MENUTEXT,
	SP_ETC				= /*0x4000*/SP_SHOW_ETC,		/* diverses */
};

typedef struct _colmap_cache
{
	SDL_Surface *src;	/* Zeiger auf Quell-Src in Pixmap cache (nur fuer Index, *NO* free!) */
	int frames; 		/* Anzahl eintraege in dyn.Array **col */
	int refcount;		/* Wieviele Sprites nutzen diese Colmap? */
	Uint8 **col;		/* Dyn.Array mit Zeiger auf Colmaps */
	struct _colmap_cache *next;
} COLMAP_CACHE;

typedef struct _sprite
{
	int id; 						/* Eindeutige Sprite-ID */
	SDL_Surface *img;				/* Images */
	COLMAP_CACHE *cmap; 			/* Zeiger auf Col-Map-Cache entry */
	int type;						/* Sprite-Typ, (siehe enum SPRITE_TYPE), -1=remove */
	Uint8 priority; 				/* Prioritaet (0: unten, 9 oben) */
	Uint8 frames;					/* Anzahl Frames */
	Sint8 anim_speed;				/* Geschw. der Animation (negative Werte: Anim l�uft r�ckw�rts */
	// Sint8 anim_count;			/* intern: Zaehler f. anim_speed */
	Uint8 alpha;					/* 255: opak, -  0: durchsichtig */
	Uint8 flags;					/* siehe unten (SP_FLAG...) */
	int aktframe;					/* akt. Frame */
	double anim_count;
	int w,h;						/* Breite, H�he */
	int ticks;						// number of ticks since creation
	double x,y; 					/* akt. Position */
	void *data; 					/* Zeiger auf Strukur mit zus�tzlichen Daten */
	void (*mover)(struct _sprite *);/* Custom-Move-Routine */
	struct _sprite *next;
} SPRITE;

#define SP_FLAG_COLCHECK	0x01	/* Col-Check aktiv */
#define SP_FLAG_VISIBLE 	0x02	/* Sprite sichtbar */
#define SP_FLAG_NOCACHE 	0x04	/* Image nicht im Cache */
#define SP_FLAG_FREESURFACE 0x08	/* Surface loeschen */
#define SP_FLAG_GRAZE		0x10	/* �O���C�Y�ς݂��Ƃ����̃t���O */
//#define SP_FLAG_CHEAPALPHA	0x10	/* "Cheapalpha", nur jedes 2te Pixel setzen */
#define SP_FLAG_PARANOIA	0x20	/* SIE sind hinter mir her */

typedef struct _cont
{
//	int max;						/* Anzahl Sprites in e[] */
	void *e;						/* Zeiger auf Array mit sprite-ids */
	void (*con)(struct _cont *);	/* Custum controller routine */
	struct _cont *next; 			/* naechster controller oder NULL */
} CONTROLLER;

#include "support.h"

extern COLMAP_CACHE *sprite_add_colmap(SDL_Surface *img, int frames);
extern COLMAP_CACHE *sprite_get_colmap(SDL_Surface *img, int frames);
extern void sprite_remove_colmap(COLMAP_CACHE *c);
extern SPRITE *sprite_add_file(char *filename, int frames, Uint8 priority);
extern SPRITE *sprite_add_file2(char *filename, int frames, Uint8 priority);
extern SPRITE *sprite_add(SDL_Surface *surface, int frames, Uint8 priority, int nocache);

extern SDL_Surface *sprite_getcurrimg(SPRITE *s);
extern void sprite_display(int type);
//static void sprite_remove(SPRITE *sr);
extern void sprite_remove_all(int type);
extern void sprite_remove_all_type(int type);
extern void sprite_animate(int type);
extern void sprite_move(int type);
extern void sprite_work(int type);
extern int sprite_count(int type);
extern SPRITE *sprite_colcheck(SPRITE *tocheck, int type);
//extern SPRITE *sprite_colcheck2(SPRITE *tocheck, int type);/*[r13]*/


extern Uint32 sprite_getpixel(SDL_Surface *surface, Sint16 x, Sint16 y);
extern void sprite_create_colmap(SDL_Surface *src, Uint8 *dst,int w, int f);
extern SPRITE *sprite_get_by_id(int id);
extern CONTROLLER *controller_add();
extern void controller_remove(CONTROLLER *c);
extern void controller_remove_all();
extern void controller_work();

//static int sprite_col_bounding(SPRITE *a, SPRITE *b);
//extern int sprite_col_bounding2(SPRITE *a, SPRITE *b);/*[r13]*/
//static int sprite_col_pixel(SPRITE *a, SPRITE *b);
//static int sprite_memand(Uint8 *s1, Uint8 *s2, int shift1, int shift2, int n);
#endif
