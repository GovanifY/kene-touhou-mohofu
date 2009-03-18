#include "sprite.h"

SPRITE *sprite=NULL;		//スプライトのリスト構造

extern SDL_Surface *screen;
extern double fps_factor;
extern GAMESTATE state; 	//[***090125		追加


static CONTROLLER	*cont			= NULL;
static COLMAP_CACHE *colmap_cache	= NULL;

/*------------------------------*/
/*---- resource ----*/

/*------------------------------*/
/*---- collision ----*/
static SDL_Rect rc;/* チェック用、矩形判定領域 */
#if 0
static int sprite_memand(Uint8 *s1, Uint8 *s2, unsigned int shift1, unsigned int shift2, int length)
{
	Uint16 i1=1<<shift1, i2=1<<shift2;
	int b;
	for (b=0; b<length; b++)
	{
		if (i1==0x100) { i1=0x01; s1++; }
		if (i2==0x100) { i2=0x01; s2++; }
		if ((*s1&i1) && (*s2&i2)) return b+1;
		i1<<=1; i2<<=1;
	}
	return 0;
}
#else
static int sprite_memand(Uint8 *s1, Uint8 *s2, unsigned int shift1, unsigned int shift2, int length)
{
	const Uint8 stbl[8] =
	{
		0x01, 0x02, 0x04, 0x08,
		0x10, 0x20, 0x40, 0x80,
	};
	int b;
	for (b=0; b<length; b++)
	{
		if (( (*s1) & (/*1<<*/stbl[shift1]) ) &&
			( (*s2) & (/*1<<*/stbl[shift2]) )	)
		{
			return (b+1);
		}
		shift1++;	if (shift1==8 ) { shift1 = 0; s1++; }
		shift2++;	if (shift2==8 ) { shift2 = 0; s2++; }
	}
	return 0;
}
#endif
static int sprite_collision_pixel(SPRITE *a, SPRITE *b)
{
	Sint16 w1 = a->w;
	Sint16 h1 = a->h;
	Sint16 x1 = a->x;
	Sint16 y1 = a->y;
	Sint16 w2 = b->w;
	Sint16 h2 = b->h;
	Sint16 x2 = b->x;
	Sint16 y2 = b->y;

	Sint32 x1o = 0;
	Sint32 x2o = 0;
	Sint32 y1o = 0;
	Sint32 y2o = 0;
	Sint32 offs;
	unsigned int i1 = 0;
	unsigned int i2 = 0;
//
	Sint16 y;
	Sint16 length;
	Uint8 *map1 = a->cmap->col[a->aktframe];
	Uint8 *map2 = b->cmap->col[b->aktframe];
//
	if (rc.x==x2 && rc.y==y2)
	{
		x1o = x2-x1;
		y1o = y2-y1;
		offs = (w1 * y1o) + x1o;
		map1 += (offs>>3);	i1 = (offs&(8-1));
	}
	else if (rc.x==x2 && rc.y==y1)
	{
		x1o = x2-x1;
		y2o = y1-y2;
		map1 += (x1o>>3);	i1 = (x1o&(8-1));
		offs = (w2 * y2o);
		map2 += (offs>>3);	i2 = (offs&(8-1));
	}
	else if (rc.x==x1 && rc.y==y1)
	{
		x2o = x1-x2;
		y2o = y1-y2;
		offs = (w2 * y2o) + x2o;
		map2 += (offs>>3);	i2 = (offs&(8-1));
	}
	else if (rc.x==x1 && rc.y==y2)
	{
		y1o = y2-y1;
		x2o = x1-x2;
		offs = (w1 * y1o);
		map1 += (offs>>3);	i1 = (offs&(8-1));
		map2 += (x2o>>3);	i2 = (x2o&(8-1));
	}
	else
	{
		return 0;
	}
//
	if (x1+w1 < x2+w2)
	{
		length = w1-x1o;
	}
	else
	{
		length = w2-x2o;
	}
//
	for (y=rc.y; ((y<=y1+h1) && (y<=y2+h2)); y++)
	{
		offs = sprite_memand(map1, map2, i1, i2, length);
		if (offs)
		{
			// col-koord: x=rc.x+offs-1, y=y
			return 1;
		}
		offs = ((y-y1) * w1) + x1o;
		map1 = a->cmap->col[a->aktframe];
		map1 += (offs>>3);
		i1 = (offs&(8-1));

		offs = ((y-y2) * w2) + x2o;
		map2 = b->cmap->col[b->aktframe];
		map2 += (offs>>3);
		i2 = (offs&(8-1));
	}
	return 0;
}
/*------------------------------*/
static int sprite_collision_bounding(SPRITE *a, SPRITE *b)
{
	if (a->x < b->x) {
		if (a->x+a->w > b->x) {
			if (a->y < b->y) {
				if (a->y+a->h > b->y) {
					rc.x=b->x;
					rc.y=b->y;
					return 1;
				}
			} else {
				if (b->y+b->h > a->y) {
					rc.x=b->x;
					rc.y=a->y;
					return 1;
				}
			}
		}
	} else {
		if (b->x+b->w > a->x) {
			if (b->y < a->y) {
				if (b->y+b->h > a->y) {
					rc.x=a->x;
					rc.y=a->y;
					return 1;
				}
			} else {
				if (a->y+a->h > b->y) {
					rc.x=a->x;
					rc.y=b->y;
					return 1;
				}
			}
		}
	}
	return 0;
}

SPRITE *sprite_colcheck(SPRITE *tocheck, int type)
{
	SPRITE *s = sprite;
	while (s != NULL)
	{
		if ( (s!=tocheck) &&	/* zu testendes Sprite ueberspringen */
			((s->flags&(SP_FLAG_COLCHECK|SP_FLAG_VISIBLE))==(SP_FLAG_COLCHECK|SP_FLAG_VISIBLE)) &&
			(s->type!=-1) &&
			(s->type&type)) 	/* Passender Sprite-Type? */
		{
			if (sprite_collision_bounding(s,tocheck)) 	/* 矩形あたり判定 */	/* Bounding Box Collosion? */
			{
				if (sprite_collision_pixel(s,tocheck))	/* 図形あたり判定 */	/* Pixel-Level Collison */
				{
					return s;
				}
			}
		}
		s=s->next;
	}
	return NULL;
}

/*------------------------------*/
/*---- sprite ----*/


#if 0
int sprite_count(int type)
{
	SPRITE *s=sprite;
	int c=0;

	while (s!=NULL)
	{
		if (s->type&type)
		{	c++;}
		s=s->next;
	}
	return c;
}
SDL_Surface *sprite_getcurrimg(SPRITE *s)
{
	SDL_Surface *i;
	SDL_Rect r;
	if ((i=SDL_CreateRGBSurface(SDL_SRCCOLORKEY|SDL_HWSURFACE,s->w,s->h,
		screen->format->BitsPerPixel,
		screen->format->Rmask,
		screen->format->Gmask,
		screen->format->Bmask,
		screen->format->Amask))==NULL)
	{
		CHECKPOINT;
		error(ERR_FATAL,"cant create surface from sprite");
	}
	SDL_SetColorKey(i,SDL_SRCCOLORKEY| SDL_RLEACCEL,0x00000000); //denis

	r.x=s->w*s->aktframe;
	r.y=0;
	r.w=s->w;
	r.h=s->h;
	SDL_BlitSurface(s->img,&r,i,NULL);

	return i;
}
#endif
/*---------------*/
SPRITE *sprite_get_by_id(int id)
{
	SPRITE *s=sprite;

	if (id==-1)
		return NULL;

	while (s!=NULL) {
		if (s->id==id)
			return(s);
		s=s->next;
	}
	return NULL;
}
/*---------------*/

static void sprite_remove(SPRITE *sr)
{
	SPRITE *s=sprite, *p=NULL, *n=NULL;
	while (s!=NULL)
	{
		n=s->next;
		if (s==sr) {
			if (p==NULL) {
				sprite=n;
			} else {
				p->next=n;
			}
			if (!s->flags&SP_FLAG_NOCACHE) {
				unloadbmp_by_surface(s->img);
			}
			if (s->flags&SP_FLAG_FREESURFACE) {
				//SDL_FreeSurface(s->img);
			}
			sprite_remove_colmap(s->cmap);
			if (s->data!=NULL) free(s->data);
			free(s);
			return;
		}
		p=s;
		s=n;
	}

	CHECKPOINT;
	error(ERR_WARN,"sprite not found in list");
}

void sprite_remove_all(int type)
{
	SPRITE *s=sprite, *n=NULL;
	while (s!=NULL)
	{
		n=s->next;
		if (s->type&type)
		{	sprite_remove(s);}
		s=n;
	}
}

void sprite_remove_all_type(int type)
{
	SPRITE *s=sprite, *n=NULL;
	while (s!=NULL)
	{
		n=s->next;
		if (s->type==type)
		{	sprite_remove(s);}
		s=n;
	}
}


/*---------------*/

void sprite_remove_colmap(COLMAP_CACHE *c)
{
	if (c->refcount>0) {
		c->refcount--;
	} else {
		CHECKPOINT;
		error(ERR_WARN,"COLMAP_CACHE refcount already zero");
	}
}

COLMAP_CACHE *sprite_get_colmap(SDL_Surface *img, int frames)
{
	COLMAP_CACHE *c=colmap_cache;

	while (c!=NULL) {
		if ((c->src==img)&&(c->frames==frames))
			return(c);
		c=c->next;
	}
	return(NULL);
}

COLMAP_CACHE *sprite_add_colmap(SDL_Surface *img, int frames)
{
	COLMAP_CACHE *c;
	int i;
	int colbsize;

	if ((c=sprite_get_colmap(img,frames))!=NULL) {
		c->refcount++;
		return(c);
	}

	c=mmalloc(sizeof(COLMAP_CACHE));
	c->src=img;
	c->frames=frames;
	c->refcount=1;
	c->col=mmalloc(sizeof(Uint8 *)*frames);

	colbsize=((img->w/frames)*(img->h+2))/8;

	for (i=0;i<frames;i++) {
		c->col[i]=mmalloc(colbsize);
		memset(c->col[i],0x00,(colbsize));
		sprite_create_colmap(img,c->col[i],img->w/frames,i);
	}

	if (colmap_cache==NULL) {
		c->next=NULL;
	} else {
		c->next=colmap_cache;
	}
	colmap_cache=c;
	return(c);
}

/*---------------*/

void sprite_work(int type)
{
	SPRITE *s=sprite, *n;

	while (s!=NULL) {
		n=s->next;
		/* animate */
		//s->ticks++;
		if (s->type&type)
		{
			if (s->anim_speed!=0)
			{
				// s->anim_count++;
				s->anim_count+=fps_factor;
				// if (s->anim_count==abs(s->anim_speed))
				if (s->anim_count>=abs(s->anim_speed))
				{
					s->anim_count=0;
					if (s->anim_speed>0)
					{
						s->aktframe++;
						if(s->aktframe>=s->frames)
							s->aktframe = 0;
					}
					else
					{
						s->aktframe--;
						if (s->aktframe<0) s->aktframe=s->frames-1;
					}
				}
			}
			/* move */
			if (s->mover!=NULL) {
				(s->mover)(s);
			}
		}
		s=n;
	}
	if (ST_MENU != state.mainstate) { /* メニュー以外の場合、自動的に消える機能 */
		s=sprite;
		while (s!=NULL) {
			n=s->next;
			s->ticks++;
			if (s->ticks>1000) {
				s->ticks=0;
				if ((s->priority==PR_ENEMY || s->priority== PR_ENEMY_WEAPON) && s->type!=SP_EN_BOSS01 && s->type!=SP_EN_BOSS02 && s->type!=SP_EN_BOSS03 && s->type!=SP_EN_BOSS04 && s->type!=SP_MENUTEXT)
					s->type=-1;
			}
			if (s->type==-1) {
				sprite_remove(s);
			}
			s=n;
		}
	}
}

void sprite_display(int type)
{
	SPRITE *s=sprite;
	SDL_Rect r;
	SDL_Rect t;

	t.y=0;
	while (s!=NULL)
	{
		if ((s->flags&SP_FLAG_VISIBLE) && (s->type&type))
		{
			r.x=(int)s->x;
			r.y=(int)s->y;
			r.w=s->w;
			r.h=s->h;

			t.w=s->w;
			t.h=s->h;
			t.x=s->w*s->aktframe;

//			if (s->flags&SP_FLAG_CHEAPALPHA) {
//				blit_calpha(s->img,&t,screen,&r);
//			} else {
				SDL_SetAlpha(s->img,SDL_SRCALPHA,s->alpha);
				SDL_BlitSurface(s->img,&t,screen,&r);
//			}
		}
		s=s->next;
	}
}


/*---------------*/

void sprite_create_colmap(SDL_Surface *src, Uint8 *dst,int w,int f)
{
	Sint16 xstart,xstop,x,y, bit=1;
	Uint32 color_key;

	color_key=src->format->colorkey;
	xstart=w*f;
	xstop=w*(f+1);

	if (SDL_MUSTLOCK(src)) SDL_LockSurface(src);
	for (y=0; y<src->h; y++) {
		for (x=xstart; x<xstop; x++) {
			if (bit==0x100) { bit=1; dst++; }
			if (getpixel(src,x,y) != color_key) {
				*dst=*dst|bit;
			}
			bit<<=1;
		}
	}
	if (SDL_MUSTLOCK(src)) SDL_UnlockSurface(src);
}

/*---------------*/




SPRITE *sprite_add(SDL_Surface *surface, int frames, Uint8 priority, int nocache)		//スプライトをリストに追加する。
{
	SPRITE *s, *t, *p=NULL;
	static int current_id=0;

	s=mmalloc(sizeof(SPRITE));

	t=sprite;		//スプライトリストの呼び出し
	p=NULL;
	while ((t!=NULL)&&(t->priority<priority)) {
		p=t;
		t=t->next;
	}
	s->next=t;
	if (p!=NULL)
	{	p->next=s;}
	else	//t == NULL
	{	sprite=s;}

	s->id=current_id;
	current_id++;
	s->priority=priority;
	s->frames=frames;
	s->aktframe=0;
	s->anim_speed=0;
	s->anim_count=0;
	s->mover=NULL;
	s->alpha=255;
	s->img=surface;
	s->w=surface->w/frames;
	s->h=surface->h;
	s->cmap=sprite_add_colmap(surface, frames);
	s->flags=0;
	s->ticks=0;
	if (nocache) s->flags|=SP_FLAG_NOCACHE;
	s->data=NULL;

	return(s);
}

SPRITE *sprite_add_file(char *filename, int frames, Uint8 priority)
{
	SDL_Surface *s;
	SPRITE *sp;
	s=loadbmp(filename);
	SDL_SetColorKey(s,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
	sp=sprite_add(s,frames,priority,0);
	return sp;
}


SPRITE *sprite_add_file2(char *filename, int frames, Uint8 priority)
{
	SDL_Surface *s;
	SPRITE *sp;
	s=loadbmp2(filename);
	sp=sprite_add(s,frames,priority,0);
	return sp;
}

/*------------------------------*/
/*---- sprite controller ----*/

void controller_work()
{
	CONTROLLER *c=cont, *n=NULL;
	while (c!=NULL)
	{
		n = c->next;
		if (c->con!=NULL)
		{
			(c->con)(c);/*コントローラー関数を実行する*/
		}
		c = n;
	}
}

CONTROLLER *controller_add()
{
	CONTROLLER *new_controller;
	new_controller			= mmalloc(sizeof(CONTROLLER));
	new_controller->next	= cont;
	cont					= new_controller;
	new_controller->e		= NULL;
	new_controller->con 	= NULL;
//	new_controller->max 	= 0;
	return new_controller;
}

void controller_remove(CONTROLLER *id)
{
	CONTROLLER *c=cont, *p=NULL;
	while (c!=NULL)
	{
		if (c==id) {
			if (p!=NULL)
				p->next=c->next;
			else
				cont=c->next;
			if (c->e!=NULL)
				free(c->e);
			free(c);
			return;
		}
		p=c;
		c=c->next;
	}
	error(ERR_WARN,"controller %x not found",id);
}

void controller_remove_all()
{
	CONTROLLER *c=cont, *n;
	while (c!=NULL)
	{
		n=c->next;
		if (c->e)
			free(c->e);
		free(c);
		c=n;
	}
	cont=NULL;
}
