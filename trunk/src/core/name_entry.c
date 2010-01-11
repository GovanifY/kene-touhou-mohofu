
/*---------------------------------------------------------

---------------------------------------------------------*/

#include "game_main.h"
#include "name_entry.h"/**/

/*---------------------------------------------------------
	�n�C�X�R�A�\���f�����
---------------------------------------------------------*/

/*global*/
/*extern*/ SCORE_FORMAT high_score_table[MAX_8_SAVE_PLAYERS][MAX_5_RANKING/*5*/];

/*local static*/


typedef struct
{
	int amplifier256;		/* ���̃Y���ʂ̊����B amplifier x offset */
	int phase256;			/* ���̃Y���ʁB phase x offset */
	int phaseout256;		/* ���̃Y���ʁB phase x offset */
//	int phaseout_speed256;
	int direction;						/* �Y������� */
	int move_done_flag; 				/* �ړ������������A���ĂȂ����̏�� */
//	�萔
	int xg256;
	int yg256;
	int amplifier_speed256;
	int phase_speed256;
} MOVE_FONT_DATA;

static int move_done_lines; 			/* �ړ����������s�̐� */
static int more_show;					/* �ʂ̐�����܂����� */
static int show_player_num; 			/* �X�R�A���\�����̃v���C���[�ԍ� */

enum
{
	RESULT_DATA_00_XG256 = 0,
	RESULT_DATA_01_YG256,
	RESULT_DATA_02_PHASE_SPEED256,
	RESULT_DATA_03_MAX		/* �ő吔 */
};
#define MAX_7_LINES (7/*6*/)
static const signed int result_const_status[RESULT_DATA_03_MAX][MAX_7_LINES] =
{
	{/* RESULT_DATA_00_XG256 */
		/*(t256( 4)*0) +*/ t256(64/* 20*/), 	/*1ST*/
		/*(t256( 4)*1) +*/ t256(64/* 20*/), 	/*2ND*/
		/*(t256( 4)*2) +*/ t256(64/* 20*/), 	/*3RD*/
		/*(t256( 4)*3) +*/ t256(64/* 20*/), 	/*4TH*/
		/*(t256( 4)*4) +*/ t256(64/* 20*/), 	/*5TH*/
		t256( (64+32)/*30*/ ),					/*"RESULT OF SCORE"*/
		t256( (64-32)/*30*/ ),					/*"REIMU"*/
	},
	{/* RESULT_DATA_01_YG256 */
		(t256( (20*0)+(144+4)/*150*/) ),	/*1ST*/ 	//(25*0)+(110+25+5)/*150*/
		(t256( (20*1)+(144+4)/*150*/) ),	/*2ND*/ 	//(25*1)+(110+25+5)/*150*/
		(t256( (20*2)+(144+4)/*150*/) ),	/*3RD*/ 	//(25*2)+(110+25+5)/*150*/
		(t256( (20*3)+(144+4)/*150*/) ),	/*4TH*/ 	//(25*3)+(110+25+5)/*150*/
		(t256( (20*4)+(144+4)/*150*/) ),	/*5TH*/ 	//(25*4)+(110+25+5)/*150*/
		t256(110),								/*"RESULT OF SCORE"*/
		t256(128-2),							/*"REIMU"*/
	},
	{/* RESULT_DATA_02_PHASE_SPEED256 */
		4,		/*1ST*/
		4,		/*2ND*/
		4,		/*3RD*/
		4,		/*4TH*/
		4,		/*5TH*/
		6,		/*"RESULT OF SCORE"*/
		5,		/*"REIMU"*/
	},
};

/*---------------------------------------------------------
	�n�C�X�R�A�\���f�����(�����ړ��p)
---------------------------------------------------------*/

static void move_result(SPRITE *src)
{
	MOVE_FONT_DATA *data = (MOVE_FONT_DATA *)src->data;
	if (0==data->move_done_flag)	/* �ړ��͕K�v�H */
	{
		src->x256 = ((data->xg256) );
		src->y256 = ((data->yg256) );
		if (0==data->direction)	/* �ړ�������� */
		{
			/* slide-in */
			data->amplifier256 -= data->amplifier_speed256;
			if (data->amplifier256 <= t256(0.3) )
			{
				data->move_done_flag=1;	/* �ړ����� */
				move_done_lines++;
			}
			else
			{
				data->phase256 += data->phase_speed256/**fps_fa ctor*/;
				src->x256 += ((data->phase256*data->amplifier256)>>8);
			}
		}
		else
		{
			data->phaseout256 += data->phase_speed256/**fps_fa ctor*/;
			src->x256 += ((data->phaseout256*data->amplifier256)>>8);
		//	if (1==more_show)	/* psp��0���W�X�^������̂�0�Ɣ�r�����ق������� */
			if (0!=more_show)
			{
				src->y256 -= ((data->phase256*data->amplifier256)>>8);		/* phase256 �͗��p */
			}
			/* slide-out */
			data->amplifier256 += data->amplifier_speed256;
			if (
			//	(data->amplifier256 >= t256(100/*400*/) ) &&
			//	((src->x<-src->w)||(src->x>PSP_WIDTH480)) /* ��ʊO */
				((src->x256<-((src->w128+src->w128)))||(src->x256>t256(PSP_WIDTH480)))||	/* ��ʊO */
				(( (src->y256+t256(32)/*(s->h)*/) < 0))	/* ��ʊO */
				)
			{
				data->move_done_flag=1;	/* �ړ����� */
				move_done_lines++;
			}
		}
	}
}

/*---------------------------------------------------------
	�n�C�X�R�A�\���f�����
---------------------------------------------------------*/

static SPRITE *result_sprites[MAX_7_LINES/*6*/];
static SDL_Surface *result_surfaces[MAX_7_LINES/*6*/];

static void result_font_render(void)
{
	MOVE_FONT_DATA *hd;
	int i;
//
	static IMAGE_RESOURCE my_resource[1] =
	{
		{
			NULL,/*dummy*/
			0,/*dummy*/
			1,
		//	iyx(1,1),/*�p�~*/
			PRIORITY_01_SHOT,
			0/*dummy*/	/* �����蔻��~�̔��a(�����蔻��̑傫��) */
		}
	};
//
	for (i=0; i<MAX_7_LINES; i++)
	{
		if (i<5)
		{
			char ttmp[64/*50*/];
			sprintf(ttmp,"%1d %-3s %09d0",i+1,high_score_table[(show_player_num)][i].name,high_score_table[(show_player_num)][i].score);
		//	result_surfaces[i]		= font_render(ttmp,(0==i)?FONT05:FONT01);
		//	result_surfaces[i]		= font_render(ttmp,(0==i)?FONT03:FONT07);
			result_surfaces[i]		= font_render(ttmp, FONT03);
		}
		else
		if (5==i)
		{
			result_surfaces[5]		= font_render("RESULT OF SCORE"/*"TOP FIVE FIGHTERS"*/, FONT05);
		}
		else
		if (6==i)
		{
			/*const*/ char *score_name[8/*5*/] =
			{
			/* 0==*/	"REIMU_A",
			/* 1==*/	"REIMU_B",
			/* 2==*/	"MARISA_A",
			/* 3==*/	"MARISA_B",
			/* 5==*/	"REMILIA",
			/* 7==*/	"YUYUKO",
			/* 6==*/	"CHIRNO_A",
			/* 4==*/	"CHIRNO_Q",
			};
			result_surfaces[6]				= font_render((char *)score_name[(show_player_num)], FONT03/*FONT06*/);
		}
		result_sprites[i]					= sprite_add_res_list(
			result_surfaces[i],
		//	1,
		//	iyx(1,1),
		//	PRIORITY_01_SHOT,
			SP_FLAG_NOT_CACHE/*1*/,
		//	0
			(IMAGE_RESOURCE *)my_resource
			);
		hd									= mmalloc(sizeof(MOVE_FONT_DATA));
		result_sprites[i]->data 			= hd;
		hd->xg256							= result_const_status[RESULT_DATA_00_XG256][i];
		hd->yg256							= result_const_status[RESULT_DATA_01_YG256][i];
		hd->phase_speed256					= result_const_status[RESULT_DATA_02_PHASE_SPEED256][i];
		hd->amplifier_speed256				= t256(1.0);
		result_sprites[i]->flags			|= (SP_FLAG_VISIBLE);
		result_sprites[i]->type 			= SP_MENU_TEXT/*SP_MUTEKI*/;
		result_sprites[i]->x256 			= 0;					//	result_sprites[5]->x		= 0/*30*/;
		result_sprites[i]->y256 			= 0/*(i*25+110)*/;		//	result_sprites[5]->y		= 0/*50*/;
		result_sprites[i]->callback_mover	= move_result;
	}
	for (i=0; i<MAX_7_LINES/*5*/; i++)
	{
		hd=(MOVE_FONT_DATA *)result_sprites[i]->data;
		hd->phase256			= (i<<6);
		hd->phaseout256 		= ((4-i)<<6);
		hd->amplifier256		= t256(100/*300*/);
		hd->direction			= 0;
		hd->move_done_flag		= 0;
	}
//	hd=(MOVE_FONT_DATA *)result_sprites[5]->data;
//	hd->phase256			= (0);
//	hd->phaseout256 		= (5<<6);
//	hd->amplifier256		= t256(100/*300*/);
//	hd->direction			= 0;
//	hd->move_done_flag		= 0;
//
}
static void result_font_free(void)
{
	int i;
	for (i=0; i<MAX_7_LINES; i++)
	{
		result_sprites[i]->type = SP_DELETE;
		#if 1
		/* �����Ńn���O�A�b�v */
	//	if (result_surfaces[i])
		{
			SDL_FreeSurface(result_surfaces[i]);
		}
		#endif
	}
}
/*---------------------------------------------------------
	�n�C�X�R�A�\���f�����
---------------------------------------------------------*/

//static SDL_Surface *back;

enum
{
	RESULT_00_INIT = 0,
	RESULT_01_SET_LOCATION,
	RESULT_02_SLIDE_IN,
	RESULT_03_PAD_CHECK,
	RESULT_04_SLIDE_OUT,
	RESULT_05_RETRY_CHECK,
};

void result_work(void)
{
	if ( (ST_WORK_RESULT|RESULT_00_INIT) != (psp_loop) )
	{
		psp_pop_screen();	//SDL_BlitSurface(back,NULL,sdl_screen[SDL_00_SCREEN],NULL);
	}
	switch ((Uint8)(psp_loop&0xff)/*state.substate*/)
	{
	case RESULT_00_INIT:
		//void result_init(void)
		show_player_num=0;
		psp_push_screen();	//back=SDL_ConvertSurface(screen,screen->format,screen->flags);
		//if (NULL==back)
		//{
		//	CHECKPOINT;
		//	error(ERR_FATAL, "cant create background surface");
		//}
		psp_loop++;//newsta te(ST_RESULT,RESULT_01_SET_LOCATION,0);
		break;
	case RESULT_01_SET_LOCATION:
		{
			move_done_lines=0;
			result_font_render();
			psp_loop++;//newsta te(ST_RESULT,RESULT_02_SLIDE_IN,0);
		}
		break;
	case RESULT_02_SLIDE_IN:
		if (move_done_lines==MAX_7_LINES)
		{
			psp_loop++;//newsta te(ST_RESULT,RESULT_03_PAD_CHECK,0);
		}
		break;
	case RESULT_03_PAD_CHECK:
		if (0==my_pad)/* ������������ĂȂ��� */
		{
			/* ���̐l�̐�������� */
			if (my_pad_alter & (PSP_KEY_SHOT_OK|PSP_KEY_RIGHT|PSP_KEY_LEFT))	/* ������PSP_KEY_SHOT_OK��������Ă� */
			{
				show_player_num++;								/* ���̐l�ɂ��� */
			//	if (/*4*/7<show_player_num) {show_player_num=0;} 	/* �Ō�܂Ō�����n�߂��猩�� */
				show_player_num &= 7;								/* �Ō�܂Ō�����n�߂��猩�� */
				more_show=1;	/* �܂������ */
				psp_loop++;//newsta te(ST_RESULT,RESULT_04_SLIDE_OUT,0);
			}
			/* ����̂�߂ă��j���[�ɖ߂� */
			else
			if (my_pad_alter & PSP_KEY_BOMB_CANCEL/*PSP_KEY_OPTION*/)		// �L�����Z���{�^���̒ǉ�
			{
				more_show=0;	/* ���������܂� */
				psp_loop++;//newsta te(ST_RESULT,RESULT_04_SLIDE_OUT,0);
			}
		}
		break;
	case RESULT_04_SLIDE_OUT:
		{
			move_done_lines=0;
			int i;
			for (i=0;i<MAX_7_LINES;i++)
			{
				MOVE_FONT_DATA *hd;
				hd=(MOVE_FONT_DATA *)result_sprites[i]->data;
				hd->direction=1;
				hd->move_done_flag=0;
			}
			psp_loop++;//newsta te(ST_RESULT,RESULT_05_RETRY_CHECK,0);
		}
		break;
	case RESULT_05_RETRY_CHECK:
		if (move_done_lines==MAX_7_LINES)
		{
			result_font_free();
		//	if (1==more_show)	/* psp��0���W�X�^������̂�0�Ɣ�r�����ق������� */
			if (0!=more_show)	/* �܂������ */
			{
				psp_loop=(ST_WORK_RESULT|RESULT_01_SET_LOCATION);//newsta te(ST_RESULT,RESULT_01_SET_LOCATION,0);
			}
			else	/* if (0==more_show) ���������܂� */
			{
			//	SDL_FreeSurface(back);
			//	newsta te(ST_MENU,0,1);
				psp_loop=(ST_INIT_MENU|0/*ST_ME NU_SUB_MAIN_MENU*/);//newsta te(ST_MENU/*ST_INTRO*/,0,1);
			}
		}
		break;
	}
	sprite_work000(SP_GROUP_PAUSE_SP_MENU_TEXT);/*SP_GROUP_ETC*/
	pause_sprite_display();/*SP_GROUP_ETC*/
}


/*---------------------------------------------------------
	�n�C�X�R�A
---------------------------------------------------------*/
extern int select_player;
extern int	now_max_continue;
int last_score;
void check_high_score(void)
{
//	int my_flag;my_flag=0;
	if (
			#if (0==USE_CONTINUED_RANKING)
			( (/*3*/DEFAULT_MAX_CONTINUE-1) == now_max_continue ) &&
			#endif
			(last_score > high_score_table[select_player][4].score)
		)
	{
		/* you made it! enter your name in the hiscore-list */
//		my_flag=1;
		psp_loop=(ST_INIT_NAME_ENTRY|0);//newsta te(ST_NAME_ENTRY,0,1);
	}
	else
//	if (0==my_flag) 	/* psp��0���W�X�^������̂�0�Ɣ�r�����ق������� */
	{
		/* you'd better play barbie */
		psp_loop=(ST_INIT_MENU|0/*ST_ME NU_SUB_MAIN_MENU*/);//newsta te(ST_MENU/*ST_INTRO*/,0,1);
	}
}

/*---------------------------------------------------------
	�n�C�X�R�A���O���͉��
---------------------------------------------------------*/

static char *entry;

#define MAX_40_LETTER	(40)
static SDL_Surface *letter_surface[MAX_40_LETTER];

typedef struct
{
	int xpos;
	int ypos;
	int scale256;
	char ascii;
} LETTER;
static LETTER letter[MAX_40_LETTER];

#define MY_CODE_DEL_KEY (-1)
#define MY_CODE_OK_KEY	(-2)

static int now_select_name_chr;
static int plate_x;
static int plate_y;

static int sel;

enum
{
	NAME_ENTRY_00_INIT = 0,
//	NAME_ENTRY_01_SLIDE_IN,
//	NAME_ENTRY_02_SLIDE_OUT,
	NAME_ENTRY_03_ENTRY,
	NAME_ENTRY_04_QUIT,
};
	/*
	unsigned char c;
	for (c='A';c<='Z';c++) {	tmp_str[0] = c; 	letter[i].ascii= c; 	letter_surface[i++]=font_render(tmp_str,FONT02);	}	// 0-25
	for (c='0';c<='9';c++) {	tmp_str[0] = c; 	letter[i].ascii= c; 	letter_surface[i++]=font_render(tmp_str,FONT02);	}	// 26-36
													letter[i].ascii='.';	letter_surface[i++]=font_render(".",FONT02);			// 37
													letter[i].ascii='-';	letter_surface[i++]=font_render("-",FONT02);			// 38
													letter[i].ascii= -1;	letter_surface[i++]=font_render("DEL",FONT02);			// 39
													letter[i].ascii= -2;	letter_surface[i++]=font_render("OK",FONT02);			// 40
	for (i=0;i<40;i++) {
		SDL_SetColorKey(letter[i].surface,SDL_SRCCOLORKEY,0x00000000);
		letter[i].xpos=(i%10)*25+30;
		letter[i].ypos=(i/10)*25+100;
	}
	*/

/* �����L���O�\��y */
#define RANK_LOCATE_X_48	(40)
#define RANK_LOCATE_OFS_X_24	(24)

void name_entry_init(void)
{
	int i/*=0*/;
	int j;
	/* �L�[�{�[�h(�̕���)�̏����� */
//	char tmp_str[/*128*/32/*100*/];
	char tmp_str[64];
	tmp_str[1] = '\0';/* 0 == EOS ������̏I��� / End of String */
	for (i=0; i<(40-2); )
	{
		const char *str_aaa = /* 8*5==40==38(chars)+1(dummy)+1(0) */
		{ "ABCDEFG123" "HIJKLMN456" "OPQRSTU789" "VWXYZ.-0" /*.align*/" " };
		unsigned char c /*= 'A'*/;
		c = str_aaa[i];
		tmp_str[0] = c;
		letter[i].ascii = c; letter_surface[i] = font_render(tmp_str, FONT03/*FONT02*/); /* */
		i++;
	}
	letter[i].ascii= MY_CODE_DEL_KEY;	letter_surface[i++] = font_render(/*"DEL"*/"/",FONT03/*FONT02*/); /* 39 */
	letter[i].ascii= MY_CODE_OK_KEY;	letter_surface[i++] = font_render(/*"OK"*/"!", FONT03/*FONT02*/); /* 40 */
	{
		int k;
		k = 0;
		for (j=0; j<(4*20)/*(4*36)*/; j += (20)/*(36)*/ /*25*/)
		{
			for (i=0; i<(10*20)/*(10*30)*/; i += (20)/*(30)*/ /*25*/)
			{
				SDL_SetColorKey(letter_surface[k],SDL_SRCCOLORKEY,0x00000000);
				letter[k].xpos = (i) + (32)/*48*/ /* 30*/;
				letter[k].ypos = (j) + (160)/*84*/ /*100*/;
				k++;
			}
		}
	}
	/* �L�[�{�[�h(�̕���)�̊g�嗦�������� */
	for (i=0; i<40; i++)
	{
		letter[i].scale256=t256(1.0/*0.0*/);
	}
	/* �V���Ƀ����N�C�������X�R�A�ʒu�𒲂ׂ� */
	for (i=0; i<5; i++)
	{
		if (last_score > high_score_table[select_player][i].score)
		{
			break;
		}
	}
	/* �ȍ~ i �������N�C�������ʒu�B����ɂ����̂ŁA�ʖ��Œ�`���Ƃ��B */
	#define 	my_rank 	i
	/* �X�R�A�ʒu��艺���ړ������� */
	for (j=4; j>my_rank; j--)
	{
		high_score_table[select_player][j] = high_score_table[select_player][j-1];
	}
	/* �V���Ƀ����N�C�������X�R�A�ʒu�֑}�� */
	high_score_table[select_player][my_rank].score = last_score;
//
	entry=high_score_table[select_player][my_rank].name;
	entry[0] = ' ';
	entry[1] = ' ';
	entry[2] = ' ';
	entry[3] = 0;
	now_select_name_chr=0;
//	switch (my_rank)
//	{
//	case 0: 	sp rintf(tmp_str,"NEW HISCORE");						break;
//	default:	sp rintf(tmp_str,"PLACE %d IN THE LIST",my_rank+1); 	break;
//	}
//	char *aaaa[5] =
//	{
//		"1"/*"ST"*/ /*"NEW HISCORE"*/,
//		"2"/*"ND"*/ /*"PLACE 2 IN THE LIST"*/,
//		"3"/*"RD"*/ /*"PLACE 3 IN THE LIST"*/,
//		"4"/*"TH"*/ /*"PLACE 4 IN THE LIST"*/,
//		"5"/*"TH"*/ /*"PLACE 5 IN THE LIST"*/,
//	};
	/* ���ʃ^�C�g���`�� */
	{
		for (j=0; j<5; j++)
		{// /*static*/ SDL_Surface *headline;
		//	result_surfaces[j]		= font_render(/*tmp_str*/(&aaaa[my_rank][0]),FONT03);
			{// char ttmp[64/*50*/];
				sprintf(tmp_str,"%1d %-3s %09d0",j+1,high_score_table[(select_player)][j].name,high_score_table[(select_player)][j].score);
				result_surfaces[j]		= font_render(tmp_str, (my_rank==j)?(FONT03/*��*/):(FONT05/*�g(��)*/) );
			}
			SDL_Rect r;
		//	r.x = (GAME_WIDTH/2)-((result_surfaces[j]->w)>>1);
		//	r.y = (40);
			r.x = (RANK_LOCATE_OFS_X_24);
			r.y = (RANK_LOCATE_X_48)+(j*20);
			r.w = result_surfaces[j]->w;
			r.h = result_surfaces[j]->h;
			SDL_BlitSurface(result_surfaces[j], NULL, sdl_screen[SDL_00_SCREEN], &r);
			SDL_FreeSurface(result_surfaces[j]);
		}
	}
	#if 0
	/* ���O�v���[�g�`�� */
	{
		/*static*/ SDL_Surface *plate;
		plate = loadbmp0("panel/name_plate.png", 0, 0/*1*/);
		SDL_Rect r;
	//	plate_x = r.x = (GAME_WIDTH/2)-((plate->w)>>1);
	//	plate_y = r.y = (GAME_HEIGHT-12)-(plate->h);
		plate_x = r.x = (RANK_LOCATE_OFS_X_24+32);
		plate_y = r.y = (RANK_LOCATE_X_48)+(my_rank*20);
		r.w = plate->w;
		r.h = plate->h;
		SDL_BlitSurface(plate,NULL,sdl_screen[SDL_00_SCREEN],&r);
		if (plate)
		{
			unloadbmp_by_surface(plate);/*unload_bmp_by_name("panel/name_plate.png");*/
		}
	}
	#else
	plate_x = (RANK_LOCATE_OFS_X_24+32);
	plate_y = (RANK_LOCATE_X_48)+(my_rank*20);
	#endif
	psp_push_screen();	//back=SDL_ConvertSurface(screen,screen->format,screen->flags);
	//if (NULL==back)
	//{
	//	CHECKPOINT;
	//	error(ERR_FATAL, "cant create background surface");
	//}
//	sel=-1;
	sel = 0;
	psp_loop=(ST_WORK_NAME_ENTRY|NAME_ENTRY_03_ENTRY/*NAME_ENTRY_01_SLIDE_IN*/);//newsta te(ST_NAME_ENTRY,H CLISTE_ENTRY,0);
}

/*---------------------------------------------------------
	�n�C�X�R�A���O���͉�ʁA����(�̎q�֐�)[�\������]
---------------------------------------------------------*/

static void name_entry_draw(void)
{
	psp_pop_screen();	//SDL_BlitSurface(back,NULL,screen,NULL);	/* ����Ēu�����w�i�ŉ�ʂ�`�� */
//
	SDL_Rect s;
	SDL_Rect r;
	s.x=0;
	s.y=0;
	// ������`��
	{
		int i;
		for (i=0; i<40; i++)
		{
			if (i != sel)
			{
				s.w = letter_surface[i]->w;
				s.h = letter_surface[i]->h;
//				r.x = letter[i].xpos-((letter_surface[i]->w*letter[i].scale256)>>(8+1));
//				r.y = letter[i].ypos-((letter_surface[i]->h*letter[i].scale256)>>(8+1));
				r.x = letter[i].xpos-((letter_surface[i]->w*letter[i].scale256)>>(8+3));
				r.y = letter[i].ypos-((letter_surface[i]->h*letter[i].scale256)>>(8+3));
//				r.w = ((s.w*letter[i].scale256)>>8);
//				r.h = ((s.h*letter[i].scale256)>>8);
				r.w = ((s.w));
				r.h = ((s.h));
				/*blit_scaled*/SDL_BlitSurface(letter_surface[i],&s,sdl_screen[SDL_00_SCREEN],&r);
//				if (sel >= 0)	{	if (letter[i].scale256 > t256(1.0)) {	letter[i].scale256 -= t256(0.05);}}
				if (sel >= 0)	{	if (letter[i].scale256 > t256(1.0)) {	letter[i].scale256 -= t256(0.05);}}
			}
			else
			{
//				if (sel >= 0)	{	if (letter[i].scale256 <= t256(3.0))	{	letter[i].scale256 += t256(0.2);}}
				if (sel >= 0)	{	if (letter[i].scale256 <= t256(1.6))	{	letter[i].scale256 += t256(0.2);}}
			}
		}
	}
	//
	if (sel >= 0)
	{
		static int angle512/*=0*/;
		int xa;
	//	int ya;
#if 0
/* CW�̏ꍇ */
		angle512 += deg_360_to_512((5)/*15*/)/**fps_fa ctor*/;
#else
/* CCW�̏ꍇ */
		angle512 += deg_360_to_512CCW(360-(5)/*15*/)/**fps_fa ctor*/;
#endif
		mask512(angle512);//	if (angle>360)	{	angle-=360;}
		xa		= ((/*cos512*/sin512((angle512)))>>(8-3));
	//	xa		= ((/*cos512*/sin512((angle512))*10)>>8);
	//	ya		= ((sin512((angle512))*10)>>8);
		int sw; sw	= (letter_surface[sel]->w); s.w 	= (/*letter_surface[sel]->w*/sw);
		int sh; sh	= (letter_surface[sel]->h); s.h 	= (/*letter_surface[sel]->h*/sh);
//		r.w 	= ((/*s.w*/sw*(letter[sel].scale256))>>8);
//		r.h 	= ((/*s.h*/sh*(letter[sel].scale256))>>8);
		r.w 	= ((/*s.w*/sw));
		r.h 	= ((/*s.h*/sh));
		r.x 	= (letter[sel].xpos)-(((/*letter[sel].surface->w*/sw)*(letter[sel].scale256))>>(8+1))+((sin512((angle512)))>>5/*xa*/);
		r.y 	= (letter[sel].ypos)-(((/*letter[sel].surface->h*/sh)*(letter[sel].scale256))>>(8+1))+((sin512((angle512)))>>6/*xa>>1*/)/*ya*/;
		/*blit_scaled*/SDL_BlitSurface(letter_surface[sel],&s,sdl_screen[SDL_00_SCREEN],&r);
	}
	{
		SDL_Surface *e;
		e		= font_render(entry,FONT03/*FONT02*/);
		s.x 	= 0;
		s.y 	= 0;
		s.w 	= e->w;
		s.h 	= e->h;
		r.x 	= plate_x /*+ 5*/;
		r.y 	= plate_y /*+ 5*/;
		r.w 	= e->w/**2*/;
		r.h 	= e->h/**2*/;
		/*blit_scaled*/SDL_BlitSurface(e,&s,sdl_screen[SDL_00_SCREEN],&r);
		SDL_FreeSurface(e);
	}
}

/*---------------------------------------------------------
	�n�C�X�R�A���O���͉�ʁA����[�L�[���́���ԕύX����]
---------------------------------------------------------*/

void name_entry_work(void)
{
	int i;
	switch (psp_loop)
	{
//	case (ST_WORK_NAME_ENTRY|NAME_ENTRY_01_SLIDE_IN):/*�J�n��(���̂P)�ɃL�[�{�[�h���g�傷��*/
//		for (i=0; i<40; i++)	{	letter[i].scale256 += t256(0.1);	}
//		if (letter[0].scale256 >= t256(3)) {	psp_loop++; 			}
//		break;
//	case (ST_WORK_NAME_ENTRY|NAME_ENTRY_02_SLIDE_OUT):/*�J�n��(���̂Q)�ɃL�[�{�[�h���k������*/
//		for (i=0; i<40; i++)	{	letter[i].scale256 -= t256(0.1);	}
//		if (letter[0].scale256 <= t256(1)) {	psp_loop++; sel = 0;	}
//		letter[0].scale256 = t256(1);
//		{	psp_loop++; sel = 0;	}
//		break;
	case (ST_WORK_NAME_ENTRY|NAME_ENTRY_03_ENTRY):/* */
		if (0 == my_pad)/* ������������ĂȂ��� */
		{
			/* �΂߈ړ��͂��Ȃ� */
				 if (my_pad_alter & PSP_KEY_LEFT )	{		sel--;		if (sel<0)		sel=39; 	}
			else if (my_pad_alter & PSP_KEY_RIGHT)	{		sel++;		if (sel==40)	sel=0;		}
			else if (my_pad_alter & PSP_KEY_UP	 )	{		sel-=10;	if (sel<0)		sel+=40;	}
			else if (my_pad_alter & PSP_KEY_DOWN )	{		sel+=10;	if (sel>39) 	sel-=40;	}
			//
			if (my_pad_alter & PSP_KEY_SHOT_OK) 	/* ���͌��� == (������)���͌���{�^����������Ă��B */
			{
				switch (letter[sel].ascii)
				{
				case MY_CODE_DEL_KEY: /* [�폜�L�[](DEL)�����肵���ꍇ�B Delete last character */
					goto delete_last_character;
					break;
				case MY_CODE_OK_KEY: /* [���͏I���L�[](OK)�����肵���ꍇ�B Input completed. ���͏I���B Eingabe abgeschlossen. */
					goto agree_entry;
					break;
				default:
					if (now_select_name_chr < 3) /* 3�����ȉ����肵���ꍇ�͕������́B  3 chrs, name input entry. */
					{
						entry[now_select_name_chr] = letter[sel].ascii;
						now_select_name_chr++;
					}
					else /* 3�����ȏ�̌��肵���ꍇ�͎����I�ɓ��͏I���L�[�����������Ƃɂ���B */
					{
						sel = 39;/* [���͏I���L�[](OK)���������̂Ɠ����B force set [OK] */
					}
					break;
				}
			}
			else if (my_pad_alter & PSP_KEY_BOMB_CANCEL/*PSP_KEY_OPTION*/)	/* (������)�L�����Z���{�^����������Ă��B */
			{
			delete_last_character:
				if (0 < now_select_name_chr) /* ���O���͂̓��͕���������ꍇ�ŁB  at first chr? */
				{
					now_select_name_chr--;				/* [�폜�L�[](DEL) �����͂���Ă���̂ŁA���̕��߂�B */
					entry[now_select_name_chr] = ' ';	/* ���� */
				}
			}
			else if (my_pad_alter & PSP_KEY_PAUSE)		/* (������)�I��(��������)�{�^����������Ă��B */
			{
			agree_entry:	/* ���͏I������ */
			//	if (str cmp(entry,"   "))		// ���O���͂̓��͕���������ꍇ�ŁB�������͂���Ă���ꍇ�ŁB(str cmp��==����0��Ԃ�)
				if (0 < now_select_name_chr)	// ���O���͂̓��͕���������ꍇ�ŁB�������͂���Ă���ꍇ�ŁB(str cmp��==����0��Ԃ�)
				{
					/* �L�[�{�[�h�̕������J������ */
					for (i=0; i<40; i++)
					{
						#if 0
					//	if (letter_surface[i])
						{
							/* �����Ńn���O�A�b�v */
							SDL_FreeSurface(letter_surface[i]);
						}
						#endif
					}
				//	SDL_FreeSurface(back);
					psp_loop=(ST_INIT_MENU|0/*ST_ME NU_SUB_MAIN_MENU*/);//newsta te(ST_MENU/*ST_INTRO*/,0,1);
					return;
				}
			}
		}
		break;
//	case (ST_WORK_NAME_ENTRY|NAME_ENTRY_04_QUIT):/* */
//		break;
	}
	name_entry_draw();/*�`�敔��*/
}
